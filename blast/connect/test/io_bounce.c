/* $Id: io_bounce.c,v 6.6 1999/10/12 16:28:08 vakatov Exp $ */
/* Author:  Denis Vakatov, NCBI */
/* This is a simple prog that pumps STDIN to STDOUT and to log file */
/* cc io_bounce.c -o io_bounce */
/* NOTE:  "io_bounce.log" must already exist and be 0666 in the server dir! */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

extern int main(int argc, char** argv)
{
  char buf[10];
  int  fd = -1;

  close(2);

  if (argc > 1  &&  *argv[1] != '-') {
    fd = creat(argv[1], 0666);
    if (fd < 0) {
      fprintf(stdout, "ERROR(#%d): cannot open log file!", (int)errno);
      fflush(stdout);
      exit(1);
    }
  }

  /* bounce and log the input data */
  for (;;) {
    ssize_t n_read = read(0, buf, sizeof(buf));
    if (n_read <= 0)
      break;

    write(fd, buf, n_read);
    if (write(1, buf, n_read) != n_read)
      return 2;
  }

  if (fd >= 0)
    close(fd);
  return 0;
}
