Installing MySQL on Mac OS X
----------------------------

Beginning with MySQL 4.0.11, you can install MySQL on Mac OS X 10.2
("Jaguar") using a Mac OS X `PKG' binary package instead of the binary
tarball distribution. Please note that older versions of Mac OS X (for
example, 10.1.x) are not supported by this package.

The package is located inside a disk image (`.dmg') file, that you
first need to mount by double-clicking its icon in the Finder. It should
then mount the image and display its contents.

*NOTE*: Before proceeding with the installation, be sure to shut down
all running MySQL server instances by either using the MySQL Manager
Application (on Mac OS X Server) or via `mysqladmin shutdown' on the
command line.

To actually install the MySQL PKG, double click on the package icon.
This launches the Mac OS Package Installer, which will guide you through
the installation of MySQL.

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
installed:

   * Mac OS X Server 10.2-10.2.2 come with MySQL 3.23.51 installed

   * Mac OS X Server 10.2.3-10.2.6 ship with MySQL 3.23.53

This manual section covers the installation of the official MySQL Mac
OS X PKG only.  Make sure to read Apple's help about installing MySQL
(Run the "Help View" application, select "Mac OS X Server" help, and do
a search for "MySQL" and read the item entitled "Installing MySQL").

Especially note that the pre-installed version of MySQL on Mac OS X
Server starts the MySQL user with the command `safe_mysqld' instead of
`mysqld_safe'.

If you previously used Marc Liyanage's MySQL packages for Mac OS X from
`http://www.entropy.ch', you can simply follow the update instructions
for packages using the binary installation layout as given on his pages.

If you are upgrading from Marc's version or from the Mac OS X Server
version of MySQL to the official MySQL PKG, you also need to convert
the existing MySQL privilege tables using the
`mysql_fix_privilege_tables' script, since some new security privileges
have been added.  *Note Upgrading-from-3.23::.

After the installation, you can start up MySQL by running the following
commands in a terminal window. Please note that you need to have
administrator privileges to perform this task.

     shell> cd /usr/local/mysql
     shell> sudo ./bin/mysqld_safe
     (Enter your password, if necessary)
     (Press Control-Z)
     shell> bg
     (Press Control-D to exit the shell)

You should now be able to connect to the MySQL server, for example, by
running `/usr/local/mysql/bin/mysql'.

If you installed MySQL for the first time, *please remember to set a
password for the MySQL `root' user!*

This is done with the following two commands:

     /usr/local/mysql/bin/mysqladmin -u root password <password>
     /usr/local/mysql/bin/mysqladmin -u root -h `hostname` password <password>

You might want to also add aliases to your shell's resource file to
access `mysql' and `mysqladmin' from the command line:

     alias mysql '/usr/local/mysql/bin/mysql'
     alias mysqladmin '/usr/local/mysql/bin/mysqladmin'

Alternatively, you could simply add `/usr/local/mysql/bin' to your
`PATH' environment variable, for example, by adding the following to
`$HOME/.tcshrc':

     setenv PATH ${PATH}:/usr/local/mysql/bin

To enable the automatic startup of MySQL on bootup, you can download
Marc Liyanage's MySQL StartupItem from the following location:

`http://www2.entropy.ch/download/mysql-startupitem.pkg.tar.gz'

We plan to add a StartupItem to the official MySQL PKG in the near
future.

Please note that installing a new MySQL PKG does not remove the
directory of an older installation. Unfortunately, the Mac OS X
Installer does not yet offer the functionality required to properly
upgrade previously installed packages.

After you have copied over the MySQL database files from the previous
version and have successfully started the new version, you should
consider removing the old installation files to save disk space.
Additionally, you should also remove older versions of the Package
Receipt directories located in `/Library/Receipts/mysql-<version>.pkg'.

