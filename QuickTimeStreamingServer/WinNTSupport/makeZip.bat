
IF EXIST DarwinStreamingServer ( RMDIR /Q /S DarwinStreamingServer) 
MKDIR DarwinStreamingServer

IF EXIST build ( RMDIR /Q /S build )
MKDIR build 

DEL /Q dynmodules_disabled\*.dll

IF option%1 EQU optiondebug ( copy /Y Debug\*.exe build\*.exe
copy /Y dynmodules_disabled\Debug\*.dll dynmodules_disabled\*.dll
) ELSE (
 copy /Y Release\*.exe build\
 copy /Y dynmodules_disabled\Release\*.dll dynmodules_disabled\
)


copy build\DarwinStreamingServer.exe DarwinStreamingServer\DarwinStreamingServer.exe
copy build\RegistrySystemPathEditor.exe DarwinStreamingServer\RegistrySystemPathEditor.exe
copy build\StreamingLoadTool.exe DarwinStreamingServer\StreamingLoadTool.exe
copy build\MP3Broadcaster.exe DarwinStreamingServer\MP3Broadcaster.exe
copy build\PlaylistBroadcaster.exe DarwinStreamingServer\PlaylistBroadcaster.exe
copy build\qtpasswd.exe DarwinStreamingServer\qtpasswd.exe

copy streamingserver.xml DarwinStreamingServer\streamingserver.xml
copy relayconfig.xml-Sample DarwinStreamingServer\relayconfig.xml-Sample
copy qtusers DarwinStreamingServer\qtusers
copy ..\qtgroups DarwinStreamingServer\qtgroups
copy ..\WebAdmin\src\streamingadminserver.pl DarwinStreamingServer\streamingadminserver.pl
copy ..\StreamingLoadTool\streamingloadtool.cfg DarwinStreamingServer\streamingloadtool.cfg
copy WinPasswdAssistant.pl DarwinStreamingServer\WinPasswdAssistant.pl
copy Install.bat DarwinStreamingServer\Install.bat

copy ..\sample_100kbit.mov DarwinStreamingServer\sample_100kbit.mov
copy ..\sample_300kbit.mov DarwinStreamingServer\sample_300kbit.mov
copy ..\sample_100kbit.mp4 DarwinStreamingServer\sample_100kbit.mp4
copy ..\sample_300kbit.mp4 DarwinStreamingServer\sample_300kbit.mp4
copy ..\sample.mp3 DarwinStreamingServer\sample.mp3

copy dynmodules_disabled\QTSSSpamDefenseModule.dll DarwinStreamingServer\QTSSSpamDefenseModule.dll
copy dynmodules_disabled\QTSSRawFileModule.dll DarwinStreamingServer\QTSSRawFileModule.dll
copy ..\Documentation\ReadMe.rtf DarwinStreamingServer\ReadMe.rtf

mkdir DarwinStreamingServer\AdminHtml
copy ..\WebAdmin\WebAdminHtml\*.pl DarwinStreamingServer\AdminHtml
copy ..\WebAdmin\WebAdminHtml\*.cgi DarwinStreamingServer\AdminHtml
copy ..\WebAdmin\WebAdminHtml\*.html DarwinStreamingServer\AdminHtml

mkdir DarwinStreamingServer\AdminHtml\images
copy ..\WebAdmin\WebAdminHtml\images\*.gif DarwinStreamingServer\AdminHtml\images

mkdir DarwinStreamingServer\AdminHtml\includes
copy ..\WebAdmin\WebAdminHtml\includes\*.js DarwinStreamingServer\AdminHtml\includes

mkdir DarwinStreamingServer\AdminHtml\html_en
copy ..\WebAdmin\WebAdminHtml\html_en\messages DarwinStreamingServer\AdminHtml\html_en
copy ..\WebAdmin\WebAdminHtml\html_en\genres DarwinStreamingServer\AdminHtml\html_en

mkdir DarwinStreamingServer\AdminHtml\html_de
copy ..\WebAdmin\WebAdminHtml\html_de\messages DarwinStreamingServer\AdminHtml\html_de
copy ..\WebAdmin\WebAdminHtml\html_de\genres DarwinStreamingServer\AdminHtml\html_de

mkdir DarwinStreamingServer\AdminHtml\html_fr
copy ..\WebAdmin\WebAdminHtml\html_fr\messages DarwinStreamingServer\AdminHtml\html_fr
copy ..\WebAdmin\WebAdminHtml\html_fr\genres DarwinStreamingServer\AdminHtml\html_fr

mkdir DarwinStreamingServer\AdminHtml\html_ja
copy ..\WebAdmin\WebAdminHtml\html_ja\messages DarwinStreamingServer\AdminHtml\html_ja
copy ..\WebAdmin\WebAdminHtml\html_ja\genres DarwinStreamingServer\AdminHtml\html_ja
