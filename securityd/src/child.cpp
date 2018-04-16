/*
 * Copyright (c) 2004 Apple Computer, Inc. All Rights Reserved.
 * 
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */


//
// child - track a single child process and its belongings
//
#include "child.h"
#include <sys/wait.h>
#include <security_utilities/debugging.h>
#include <pthread.h>

kern_return_t
ucsp_server_handleSignal(mach_port_t sport,
                         mach_port_t task_port,
                         int signal_number)
{
    try {
        if (task_port != mach_task_self()) {
            Syslog::error("handleSignal: recieved from someone other than myself");
        } else {
            ChildManager::childManager.handleSignal(signal_number);
        }
    } catch(...) {}
    mach_port_deallocate(mach_task_self(), task_port);

    return 0;
}

kern_return_t
ucsp_server_registerChild(mach_port_t sport,
                          mach_port_t rport,
                          mach_port_t task_port)
{
    mach_port_t childPort = rport;
    try {
        pid_t pid;
        kern_return_t kt = pid_for_task(task_port, &pid);
        if (kt) {
            Syslog::error("registerChild: pid_for_task returned: %d", kt);
        } else {
            ChildManager::childManager.registerChild(pid, childPort);
        }
    } catch(...) {}

    if (childPort) {
        // Dealloc the childPort unless registerChild set it to zero, which indicates it took over ownership.
        mach_port_deallocate(mach_task_self(), childPort);
    }

    if (task_port)
        mach_port_deallocate(mach_task_self(), task_port);

    return 0;
}


//
// A Child object represents a UNIX process that was forked by us
// and may have some state associated with it.
// Although some children may be created per session (such as SecurityAgent instances)
// in general child processes are PerGlobal.
//
class Child
{
public:
	Child(pid_t pid);
    ~Child();

    void waitStatus(int status);
    int waitStatus() const          { return mStatus; }

    void childPort(mach_port_t &childPort);

    // Return our childPort and transfer ownership of it.
    mach_port_t childPort()         { mach_port_t childPort = mChildPort; mChildPort = 0; return childPort; }

    void eraseFromMap();
    void insertInMap();

    // Wait for the child to register or die.  If it registered mChildPort will be non zero, it will be 0 if it didn't.
    void waitForRegister();

private:
    Mutex mLockInternal;
    StLock<Mutex> mLock;
    pid_t mPid;

    // Service port for this child.
    // This should only be nonzero while this object owns the port.  Once it is retrieved ownership is handed off.
    mach_port_t mChildPort; 

    int mStatus;                        // Exit status if child died.
    bool mInMap;
};



//
// Construct a Child object.
//
Child::Child(pid_t pid) :
    mLock(mLockInternal), mPid(pid), mChildPort(0), mStatus(0), mInMap(false)
{
}

Child::~Child()
{
    try {
        eraseFromMap();
        if (mChildPort) {
            // Dealloc our mChildPort someone else took over ownership.
            mach_port_deallocate(mach_task_self(), mChildPort);
        }
    } catch (...) {}
}

void
Child::eraseFromMap()
{
    if (mInMap)
        ChildManager::childManager.eraseChild(mPid);
}

void
Child::insertInMap()
{
    ChildManager::childManager.insertChild(mPid, this);
    mInMap = true;
}

void
Child::waitStatus(int status)
{
    mStatus = status;
    mLock.unlock(); // Unlock the lock to unblock waitForRegister()
}

void
Child::childPort(mach_port_t &childPort)
{
    mChildPort = childPort;
    childPort = 0; // Tell our caller that we consumed the child port.

    mLock.unlock(); // Unlock the lock to unblock waitForRegister()
}

void
Child::waitForRegister()
{
    // Try to lock the lock again (this won't succeed until we get a register or wait result).
    mLock.lock();
    // Unlock as soon as we get the lock.
    mLock.unlock();
}


//
// ChildManager - Singleton Child Manager class
//

// The signleton ChildManager.
ChildManager ChildManager::childManager;

ChildManager::ChildManager()
{
}

ChildManager::~ChildManager()
{
}

bool
ChildManager::forkChild(mach_port_t &outChildPort, int &outWaitStatus)
{
    StLock<Mutex> lock(mLock, false);
    pid_t pid;

    /* Retry fork 10 times on failure after that just give up. */
    for (int tries = 0; tries < 10; ++tries)
    {
        lock.lock(); // aquire the lock
        pid = fork();
        if (pid != pid_t(-1))
            break;

        /* Something went wrong. */
        lock.unlock(); // Release the lock so we aren't holding it during the usleep below.

        int err = errno;
        if (err == EINTR)
            continue;

        if (err == EAGAIN || err == ENOMEM)
            usleep(100 * tries);
        else
            UnixError::throwMe(err);
    }

    if (pid == 0)
    {
        // Child - return true.
        return true;
    }
    else
    {
        Child child(pid);
        child.insertInMap(); // Insert child into the map.
        lock.unlock(); // Unlock as soon as we are done accessing mChildMap.

        child.waitForRegister();

        // Remove child from the map.
        child.eraseFromMap();

        // Transfer ownership of child's childPort to our caller.
        outChildPort = child.childPort();

        if (!outChildPort)
            outWaitStatus = child.waitStatus();

        // Parent - return false
        return false;
    }
}

void
ChildManager::handleSignal(int signal_number)
{
    if (signal_number == SIGCHLD)
        handleSigChild();
}

void
ChildManager::registerChild(pid_t pid, mach_port_t &childPort)
{
    StLock<Mutex> _(mLock);
    Child *child = findChild(pid);
    if (child)
        child->childPort(childPort);
}

// Assumes mLock is not locked.
void
ChildManager::eraseChild(pid_t pid)
{
    StLock<Mutex> _(mLock);
    mChildMap.erase(pid);
}

// Assumes mLock is already locked.
void
ChildManager::insertChild(pid_t pid, Child *child)
{
    mChildMap[pid] = child;
}


//
// Private functions
//

Child *
ChildManager::findChild(pid_t pid)
{
    ChildMap::iterator it = mChildMap.find(pid);
    if (it == mChildMap.end())
        return NULL;

    return it->second;
}

void
ChildManager::handleSigChild()
{
    for (int tries = 0; tries < 10; ++tries)
    {
        int status;
        pid_t pid = waitpid(-1, &status, WNOHANG|WUNTRACED);
        switch (pid)
        {
        case 0:
            if (tries == 0)
                Syslog::notice("Spurious SIGCHLD ignored");
            break;
        case -1:
            {
                int err = errno;
                if (err == ECHILD)
                {
                    if (tries == 0)
                        Syslog::notice("Spurious SIGCHLD ignored");
                    break;
                }

                if (err != EINTR)
                {
                    Syslog::error("waitpid after SIGCHLD failed: %s", strerror(err));
                    break;
                }
            }
        default:
            if (WIFEXITED(status) || WIFSIGNALED(status))
            {
                if (WIFEXITED(status))
                    secdebug("child", "child with pid: %d exited: %d", pid, WEXITSTATUS(status));
                else if (WCOREDUMP(status))
                    secdebug("child", "child with pid: %d terminated by signal: %d and dumped core", pid, WTERMSIG(status));
                else
                    secdebug("child", "child with pid: %d terminated by signal: %d", pid, WTERMSIG(status));

                waitStatusForPid(pid, status);
            }
            else if (WSTOPSIG(status))
            {
                secdebug("child", "child with pid: %d stopped by signal: %d", pid, WSTOPSIG(status));
            }
            else
            {
                Syslog::error("child with pid: %d bogus waitpid status: %d", pid, status);
            }

            tries = 1;
        }
    }
}

void
ChildManager::waitStatusForPid(pid_t pid, int status)
{
    StLock<Mutex> _(mLock);
    Child *child = findChild(pid);
    if (child)
        child->waitStatus(status);
}
