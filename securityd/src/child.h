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
#ifndef _CHILD_H_
#define _CHILD_H_  1

#include "securityserver.h"

class Child;

//
// ChildManager - Singleton Child Manager class
//
class ChildManager
{
public:
    // The singleton childManager.
    static ChildManager childManager;

    ChildManager();
    ~ChildManager();

    // Return true in the child.  Return false in the parent and set outChildPort to 0 if the child died prematuely.  In this case outWaitStatus contains the result of a waitpid() on the child.  if outChildPort is non 0 it's a send right to contact the child on.
    bool forkChild(mach_port_t &outChildPort, int &outWaitStatus);

    // SigChild signal handler
    void handleSignal(int signal);

    // Be notified a child just contacted us on our main server port telling us it's child port.  Set childPort to 0 iff we don't wish it to be deallocated by our caller.
    void registerChild(pid_t pid, mach_port_t &childPort);

    // Assumes mLock is not locked (called by Child's eraseFromMap() function).
    void eraseChild(pid_t pid);

    // Assumes mLock is already locked (called by Child's insertInMap() function).
    void insertChild(pid_t pid, Child *child);

private:
    Child *findChild(pid_t pid);

    void handleSigChild();

    // Be notifed of a term or signal waitpid() status.
    void waitStatusForPid(pid_t pid, int status);


    typedef std::map<pid_t, Child *> ChildMap;
    ChildMap mChildMap;
    Mutex mLock;
};


#endif // _CHILD_H_
