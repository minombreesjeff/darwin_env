
/*
** Sniff browser
*/
if (navigator.appVersion.indexOf("Mac") != -1) {
    platform = "mac";
} else {
    platform = "win";
}

if (platform == 'win' && (parseInt(navigator.appVersion) < 4 )) {
          browser = 'ie3';
}
else if ((navigator.appName.indexOf("Netscape") != -1) ||
         (navigator.appName.indexOf("Safari") != -1) ||
         (navigator.appName.indexOf("Gecko") != -1)) {
          browser = 'netscape';
} else {
          browser = 'ie';
}
document.write("<LINK REL=stylesheet HREF=\"",platform,"-",browser,".css\" TYPE=\"text/css\">");
