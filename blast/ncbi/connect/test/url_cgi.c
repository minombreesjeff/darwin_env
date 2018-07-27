/* $Id: url_cgi.c,v 6.4 1999/07/19 17:55:37 vakatov Exp $ */
/* This is a simple CGI script that(mostly) just pumps STDIN to STDOUT */
/* cc url_cgi.c -o con_url.cgi */

#include <stdio.h>

extern int main(int argc, char** argv)
{
  char buf[1000];
  FILE* fp;
  size_t r_total = 0;
  size_t w_total = 0;

  fprintf(stdout, "SomeThing0: BEFORE ContentType\n");
  fprintf(stdout, "Content-Type: x-ncbi-data/x-unknown\n");
  fprintf(stdout, "SomeThing1: AFTER  ContentType\r\n\r\n");

  fp = fopen("con_url.log", "wb");
  if ( !fp )
    fprintf(stdout, "NOTE+cannot+open+log+file!+++");
  fflush(stdout);

  /* bounce the input data */
  for (;;) {
    size_t n_written;

    size_t n_read = fread(buf, 1, sizeof(buf), stdin);
    if (0 < n_read  &&  n_read <= sizeof(buf))
      r_total += n_read;

    if (fp  &&  n_read > 0)
      fwrite(buf, 1, n_read, fp);

    if ( ferror(stdin) ) {
      fprintf(stdout, "STDIN+read+ERROR!+++");
      fprintf(stdout, "The+last+read+affected+%ld+bytes.+++", (long)n_read);
      fflush(stdout);
      break;
    }

    n_written = fwrite(buf, 1, n_read, stdout);
    if (0 < n_written  &&  n_written <= n_read)
      w_total += n_written;
    if (n_written != n_read) {
      fprintf(stdout, "STDOUT+write+error!+++");
      fprintf(stdout, "The+last+read++affected+%ld+bytes.+++", (long)n_read);
      fprintf(stdout, "The+last+write+affected+%ld+bytes.+++", (long)n_written);
      fflush(stdout);
      break;
    }

    if ( feof(stdin) ) {
      fprintf(stdout, "STDIN+EOF+reached.+++");
      fflush(stdout);
      break;
    }
  }

  fprintf(stdout, "+++Total++%ld+bytes+read,+%ld+bytes+bounced.+++",
          (long)r_total, (long)w_total);

  if ( fp )
    fclose(fp);

  /* printout cmd.-line args */
  fprintf(stdout, "CON_URL_CGI++Command-line+args...+++");
  while ( argc-- ) {
    printf("+++argv(%d)+'%s'.+++", argc, argv[argc]);
    fflush(stdout);
  }

  return 0;
}
