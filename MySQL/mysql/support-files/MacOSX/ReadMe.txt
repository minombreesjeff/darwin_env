Installing MySQL on Mac OS X
----------------------------

Beginning with MySQL 4.0.11, you can install MySQL on Mac OS X 10.2
("Jaguar") using a Mac OS X binary package in `PKG' format instead of
the binary tarball distribution. Please note that older versions of Mac
OS X (for example, 10.1.x) are not supported by this package.

The package is located inside a disk image (`.dmg') file, that you
first need to mount by double-clicking its icon in the Finder. It should
then mount the image and display its contents.

*NOTE*: Before proceeding with the installation, be sure to shut down
all running MySQL server instances by either using the MySQL Manager
Application (on Mac OS X Server) or via `mysqladmin shutdown' on the
command line.

To actually install the MySQL PKG file, double click on the package
icon. This launches the Mac OS X Package Installer, which will guide
you through the installation of MySQL.

Due to a bug in the Mac OS X package installer, you may sometimes see
the error message `You cannot install this software on this disk.
(null)' in the destination disk selection dialogue. If this error
occurs, simply click the `Go Back' button once to return to the
previous screen. Then click `Continue' to advance to the destination
disk selection again, and you should be able to choose the destination
disk correctly. We have reported this bug to Apple and they are
investigating this problem.

The Mac OS X PKG of MySQL will install itself into
`/usr/local/mysql-<version>' and will also install a symbolic link
`/usr/local/mysql', pointing to the new location. If a directory named
`/usr/local/mysql' already exists, it will be renamed to
`/usr/local/mysql.bak' first. Additionally, it will install the grant
tables in the `mysql' database by executing `mysql_install_db' after
the installation.

The installation layout is similar to the one of the binary
distribution; all MySQL binaries are located in the directory
`/usr/local/mysql/bin'.  The MySQL socket file is created as
`/tmp/mysql.sock' by default.  *Note Installation layouts::.

MySQL installation requires a Mac OS X user account named `mysql' (a
user account with this name should exist by default on Mac OS X 10.2
and up).

If you are running Mac OS X Server, you already have a version of MySQL
installed.  The versions of MySQL that ship with Mac OS X Server
versions are shown in the following table:

*Mac OS X Server       *MySQL Version*
Version*               
10.2-10.2.2            3.23.51
10.2.3-10.2.6          3.23.53
10.3                   4.0.14
10.3.2                 4.0.16

This manual section covers the installation of the official MySQL Mac
OS X PKG only.  Make sure to read Apple's help about installing MySQL
(Run the "Help View" application, select "Mac OS X Server" help, and do
a search for "MySQL" and read the item entitled "Installing MySQL").

Especially note that for pre-installed versions of MySQL on Mac OS X
Server, you should start `mysqld' with `safe_mysqld' instead of
`mysqld_safe' if the MySQL is older than version 4.0.

If you previously used Marc Liyanage's MySQL packages for Mac OS X from
`http://www.entropy.ch', you can simply follow the update instructions
for packages using the binary installation layout as given on his pages.

If you are upgrading from Marc's 3.23.xx versions or from the Mac OS X
Server version of MySQL to the official MySQL PKG, you also need to
convert the existing MySQL privilege tables to the current format,
because some new security privileges have been added.  *Note
Upgrading-grant-tables::.

If you would like to automatically start up MySQL during system bootup,
you also need to install the MySQL Startup Item. Starting with MySQL
4.0.15, it is part of the Mac OS X installation disk images as a
separate installation package. Simply double-click the
`MySQLStartupItem.pkg' icon and follow the instructions to install it.

Note that the Startup Item need be installed only once! There is no
need to install it each time you upgrade the MySQL package later.

The Startup Item will be installed into `/Library/StartupItems/MySQL'.
It adds a variable `MYSQLCOM=-YES-' to the system configuration file
`/etc/hostconfig'. If you would like to disable the automatic startup
of MySQL, simply change this variable to `MYSQLCOM=-NO-'.

On Mac OS X Server, the default MySQL installation uses the variable
`MYSQL' in `/etc/hostconfig'.  The MySQL AB Startup Item installer
disables this variable by setting it to `MYSQL=-NO-'. This avoids boot
time conflicts with the `MYSQLCOM' variable used by the MySQL AB
Startup Item.  However, it does not shut down an already running MySQL
server.

After the installation, you can start up MySQL by running the following
commands in a terminal window. Please note that you must have
administrator privileges to perform this task.

If you have installed the Startup Item:

     shell> sudo /Library/StartupItems/MySQL/MySQL start
     (Enter your password, if necessary)
     (Press Control-D or enter "exit" to exit the shell)

If you don't use the Startup Item, enter the following command sequence:

     shell> cd /usr/local/mysql
     shell> sudo ./bin/mysqld_safe
     (Enter your password, if necessary)
     (Press Control-Z)
     shell> bg
     (Press Control-D or enter "exit" to exit the shell)

You should now be able to connect to the MySQL server, for example, by
running `/usr/local/mysql/bin/mysql'.

If you are installing MySQL for the first time, *please remember to set
a password for the MySQL `root' user!*

This is done with the following two commands:

     /usr/local/mysql/bin/mysqladmin -u root password <password>
     /usr/local/mysql/bin/mysqladmin -u root -h `hostname` password <password>

Please make sure that the `hostname' command in the second line is
enclosed by *backtick* characters (``'), so the shell can replace it
with the output of the command (which is the hostname of your system)!

You might want to also add aliases to your shell's resource file to
access `mysql' and `mysqladmin' from the command line. The syntax for
`tcsh' is:

     alias mysql /usr/local/mysql/bin/mysql
     alias mysqladmin /usr/local/mysql/bin/mysqladmin

For `bash', use:

     alias mysql=/usr/local/mysql/bin/mysql
     alias mysqladmin=/usr/local/mysql/bin/mysqladmin

Even better, add `/usr/local/mysql/bin' to your `PATH' environment
variable. For example, add the following line to your `$HOME/.tcshrc'
file if your shell is `tcsh':

     setenv PATH ${PATH}:/usr/local/mysql/bin

If no `.tcshrc' file exists in your home directory, create it with a
text editor.

If you are upgrading an existing installation, please note that
installing a new MySQL PKG does not remove the directory of an older
installation. Unfortunately, the Mac OS X Installer does not yet offer
the functionality required to properly upgrade previously installed
packages.

To use your existing databases with the new installation, you'll need
to copy the contents of the old data directory to the new data
directory. Make sure neither the old server nor the new one is running
when you do this.  After you have copied over the MySQL database files
from the previous installation and have successfully started the new
server, you should consider removing the old installation files to save
disk space.  Additionally, you should also remove older versions of the
Package Receipt directories located in
`/Library/Receipts/mysql-<version>.pkg'.

