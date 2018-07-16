About Darwin Streaming Server 4.1.3

Contents

What's New with Darwin Streaming Server 4.1.3
What's New with Darwin Streaming Server 4.1.2
What's New with Darwin Streaming Server 4.1
What's New with Darwin Streaming Server 4.0
System Requirements
Installing On UNIX-based platforms
Installing On Windows 2000/NT
Set Up
Troubleshooting
Public Mailing Lists

What's New with Darwin Steaming Server 4.1.3

This release adds the latest security fixes and support for large mpeg4 hinted files.

What's New with Darwin Steaming Server 4.1.2

This release adds web-based administration on Mac OS X for the QuickTime Broadcaster.
It also adds playback of QuickTime Broadcaster recorded movies in the PlaylistBroadcaster.

What's New with Darwin Steaming Server 4.1

This release adds support for Instant-On streaming with QuickTime 6, provides user interface for setting the user name and password for the QuickTime Broadcaster, and provides updated user documentation.

What's New with Darwin Steaming Server 4.0

Darwin Streaming Server 4, which is the complete open source code for the QuickTime Streaming Server, allows you to stream QuickTime media to clients across the Internet using industry-standard RTP and RTSP protocols.  Darwin Streaming Server 4 includes all QuickTime Streaming Server 4 features. Key enhancements include:

- Administration Enhancements:
Provides a new Setup Assistant where you can configure basic settings.
Provides the capability to configure and manage relays in the web admin. 

- Native Hinted MPEG-4 Streaming – support for native MPEG-4 Hinted files as defined by the technical standard for interoperability proposed by the Internet Streaming Media Alliance (ISMA).

- Quality of Service Enhancements - tuning improvements to the congestion control, retransmit, and thinning server algorithms on reliable UDP streams.

- Native MP3/Icecast Streaming Support – http style streaming of native MP3 files including the ability to broadcast MP3 play lists, reflect Icecast streams, parsing and sending of MP3 ID tags.

- PlaylistBroadcaster User Interface Enhancements 

Darwin Streaming Server allows the highest level of customizability and community involvement.

It can be ported to other platforms by modifying a handful of platform-specific source files. For more information about the source code and how to port to other platforms, see the files AboutTheSource.html and SourceFAQ.html provided with the Darwin Streaming Server source code.

For more information about the Darwin Streaming Server project and to obtain the Darwin Streaming Server 4.1.3 release, see Apple's Open Source Web site at: <http://developer.apple.com/darwin>.


System Requirements

Darwin Streaming Server is currently available on the following platforms:

*Mac OS X (version 10.1 or later)
*Linux (RedHat 7.1, Intel)
*Solaris 8 (SPARC)
*Windows 2000 Server/NT

Darwin Streaming Server is compatible with QuickTime 4 or later client software. Digest mode Authentication and Skip Protection (first introduced in QuickTime Streaming Server 3.0) require QuickTime 5 or later client software.

Installing Darwin Streaming Server (Mac OS X)

To install Darwin Streaming Server 4.1.3 software, follow these 
steps:

1. After downloading Darwin Streaming Server 4.1.3, double-click the DarwinStreamingServer_4_1_3.dmg file. DarwinStreamingServer_4_1_3 will mount a desktop image that contains DarwinStreamingServer.pkg.  

2. Double-click the DarwinStreamingServer.pkg file. This will launch the installer.

3. Click on the "lock" icon to make changes when prompted during installation. You will need to authenticate with the administrator username and password.

4. Follow the onscreen instructions. After you have read and agreed to the license, you can proceed with the installation.

Set Up 

From a server machine with Darwin Streaming Server installed, launch your web browser.

   Enter the URL for your Darwin Streaming Server:
http://myserver.com:1220

Replace "myserver.com" with the name of your Darwin Streaming Server computer. 
1220 is the port number.


Installing Darwin Streaming Server (Linux, Solaris)

To install Darwin Streaming Server 4.1.3 software, follow these steps on the server computer:

Stop any Darwin Streaming Server related processes.

IMPORTANT: Installing Darwin Streaming Server will remove older version of Darwin Streaming Server. 
 
Expand the compressed (.gz) tar file and "cd" into one of the following directories, depending on the platform: 

DarwinStreamingSrvr4.1.3-Linux 
DarwinStreamingSrvr4.1.3-SunOS

Then type: 

./Install

During the install, the streamingadminserver.pl application will automatically launch. To avoid the need to manually relaunch streamingadminserver.pl following reboots, you may want to configure your server machine to launch it automatically at boot time.


Installing Darwin Streaming Server (Windows 2000/NT/)

The Streaming Admin requires ActivePerl 5.6 (or later) to be running on the server machine. You must install a Perl interpreter in order to use the web-based administration software. 


To install Darwin Streaming Server 4.1.3 software, follow these steps on the server computer:

Stop any Darwin Streaming Server related processes.

When the Server package is unzipped, a folder with Darwin Streaming Server and associated files will be created. Inside this folder is an Install script, named "Install.bat". Double-click this file to install the server and its components on the server machine. The installer also starts up the Streaming Server Admin, so keep the command prompt window open.
 
The Install script will create the following directory:

c:\Program Files\Darwin Streaming Server\

Inside this directory you will find:

DarwinStreamingServer.exe - Server executable
PlaylistBroadcaster.exe - PlaylistBroadcaster executable
MP3Broadcaster.exe – MP3 Broadcaster executable
qtpasswd.exe - Command-line utility for generating password files for access control
StreamingLoadTool.exe - RTSP simulated client stress tool
streamingadminserver.pl - Admin Server that is used for administering the Streaming Server
streamingserver.xml- Default server configuration file
relayconfig.xml-Sample - Sample relay configuration file
QTSSModules\ - Folder containing QTSS API modules
Movies\ - Media folder
Playlists\ - Folder containing Playlist configuration
Logs\ - Folder containing access and error logs
AdminHtml\ - Folder containing the CGIs and the HTMl files required by the Admin Server
Documentation\ - Documentation folder

The Install script also installs Darwin Streaming Server as a service in the Service Manager. It is possible to start, stop, and check server status from the Service control panel.

The Install script will attempt to launch the Admin Server. Make sure that the Perl interpreter installed on your machine is in the system PATH.

The Admin Server can be launched from the command prompt by typing:

C:\> perlpath "C:\Program Files\Darwin Streaming Server\streamingadminserver.pl"

where perlpath is the path to the Perl interpreter on your machine.


Set Up 

To configure and administer your server from either the server machine or a remote client machine, follow these steps: 

1. From a server machine with Darwin Streaming Server installed, launch your web browser.

OR 

From a remote machine launch Microsoft Internet Explorer, versions 4.5 or later, Netscape Navigator or Netscape Communicator (Netscape Communicator versions 6.0 and 6.1 are not supported at this time):

2. Enter the URL for your Streaming Server Admin:
http://myserver.com:1220
Replace "myserver.com" with the name of your Streaming Server computer.  1220 is the port number.

3. The first time you run the Streaming Server admin, Setup Assistant will prompt for your username and password.

For help on using Streaming Server Admin, setting up secure administration (SSL), and setting up your server to stream hinted media, refer to the online Help by selecting the Question Mark button from the Streaming Server Admin.

To configure and administer your server from either the server machine or a remote client machine, follow the steps in above section.


Troubleshooting

* File Locations


Darwin Streaming Server (Mac OS X)
/usr/sbin/DarwinStreamingServer - Streaming Server app
/usr/sbin/streamingadminserver.pl - QTSS Web Admin server
/Library/QuickTimeStreaming/Modules/ - QTSS plug-ins
/usr/bin/PlaylistBroadcaster - The PlaylistBroadcaster
/usr//bin/MP3Broadcaster - The MP3Broadcaster
/usr/bin/qtpasswd - Generates password files for access control
/usr//bin/StreamingLoadTool - RTSP simulated client stress tool
/Library/QuickTimeStreaming/Config/ - QTSS config files
/Library/QuickTimeStreaming/Movies/ - Media files
/Library/QuickTimeStreaming/Docs/  - readme.html & user manual.pdf files
/Library/QuickTimeStreaming/logs/ - Logs
/Library/QuickTimeStreaming/playlists - Web Admin Playlist files

Darwin Streaming Server (Unix)
/usr/local/sbin/DarwinStreamingServer - Streaming Server app
/usr/local/sbin/streamingadminserver.pl - QTSS Web Admin server
/usr/local/sbin/StreamingServerModules/ - QTSS plug-ins
/usr/local/bin/PlaylistBroadcaster - The PlaylistBroadcaster
/usr/local/bin/MP3Broadcaster - The MP3Broadcaster
/usr/local/bin/qtpasswd - Generates password files for access control
/usr/local/bin/StreamingLoadTool - RTSP simulated client stress tool
/etc/streaming/ - QTSS config files
/usr/local/movies/ - Media files
/var/streaming/  - readme.html & user manual.pdf files
/var/streaming/logs - Logs
/var/streaming/playlists - Web Admin Playlist files

Darwin Streaming Server (Windows)
C:\Program Files\Darwin Streaming Server\
C:\Program Files\Darwin Streaming Server\Movies
C:\Program Files\Darwin Streaming Server\Playlists
C:\Program Files\Darwin Streaming Server\Logs
C:\Program Files\Darwin Streaming Server\QTSSModules
C:\Program Files\Darwin Streaming Server\AdminHtml


Public Mailing Lists

Through the Apple public mailing lists you can share experiences, questions, and comments with others who use the software. Apple employees may monitor the list, but Apple does not guarantee that questions sent to this list will be answered. For more information about joining the mailing lists, see the Apple mailing lists Web site at www.lists.apple.com.

For Darwin Streaming Server administration, join the Streaming Server mailing list, “streaming-server-users”. 

If you are interested in plug-in API or Open Source development, join the Streaming Server developer public mailing list, “streaming-server-developers”. 

The Darwin Streaming Server release is not supported by Apple Computer.


© 2003 Apple Computer, Inc. All rights reserved. Apple, the Apple logo, Mac, Macintosh, PowerBook, Power Macintosh, and QuickTime are trademarks of Apple Computer, Inc., registered in the United States and other countries.  eMac, iBook,  iMac, Power Mac and Xserve are trademarks of Apple Computer, Inc. All other product names are trademarks or registered trademarks of their respective holders.

