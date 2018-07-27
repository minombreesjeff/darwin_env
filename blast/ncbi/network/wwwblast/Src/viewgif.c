#include <signal.h>
#include <fcntl.h>
#include <stdio.h>

static void SigAlrmHandler(int);
static void SigTermHandler(int);

static char FileName[128];

main(void) 
{
    FILE *pp;
    char tmp_buff[1024];
    char *PidFile;

    int  bytes;
    char *ContentGif  =  "Content-type: image/gif\r\n\r\n";
    struct sigaction sa;
    sigset_t sigset;

    sigfillset(&sigset);
    sa.sa_mask = sigset;

    sa.sa_flags = SA_RESETHAND | SA_RESTART;
    sa.sa_handler = SigAlrmHandler;
    sigaction(SIGALRM, &sa, NULL);

    sa.sa_handler = SigTermHandler;
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGPIPE, &sa, NULL);
    
    PidFile = (char *) getenv("QUERY_STRING"); 
    
    sprintf(FileName, "TmpGifs/%s", PidFile);
    
    if((pp = fopen(FileName, "r")) == NULL) {

        /* Just do nothing */
        sprintf(tmp_buff, "HTTP/1.0 204 Not Modified\n");
        write(1, tmp_buff, strlen(tmp_buff));
        sprintf(tmp_buff, "Server: %s\n", (char *) getenv("SERVER_SOFTWARE"));
        write(1, tmp_buff, strlen(tmp_buff));
        sprintf(tmp_buff, "MIME-Version: 1.0\n");
        write(1, tmp_buff, strlen(tmp_buff));
        write(1, ContentGif, strlen(ContentGif)); 
    } else {
        sprintf(tmp_buff, "HTTP/1.0 200 OK\r\n");
        write(1, tmp_buff, strlen(tmp_buff));
        sprintf(tmp_buff, "Server: %s\n", (char *) getenv("SERVER_SOFTWARE"));
        write(1, tmp_buff, strlen(tmp_buff));
        sprintf(tmp_buff, "MIME-Version: 1.0\r\n");
        write(1, tmp_buff, strlen(tmp_buff));
        write(1, ContentGif, strlen(ContentGif)); 
        
        while ((bytes =fread(tmp_buff, 1, 256, pp)) >0)
            write(1, tmp_buff, bytes); 
    }
    remove(FileName); 
    return 0;
}
static void SigAlrmHandler(int id)
{
    
    char tmp_buff[1024];
    char *ContentGif  =  "Content-type: image/gif\r\n\r\n";
    
    sprintf(tmp_buff, "HTTP/1.0 204 Not Modified\n");
    write(1, tmp_buff, strlen(tmp_buff));
    sprintf(tmp_buff, "Server: %s\n", (char *) getenv("SERVER_SOFTWARE"));
    write(1, tmp_buff, strlen(tmp_buff));
    sprintf(tmp_buff, "MIME-Version: 1.0\n");
    write(1, tmp_buff, strlen(tmp_buff));
    write(1, ContentGif, strlen(ContentGif)); 
    remove(FileName); 
    exit(1);   
}

static void SigTermHandler(int id)
{
    remove(FileName); 
    exit(1);
}















