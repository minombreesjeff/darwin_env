/*
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <syslog.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netdb.h>
*/
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
/*
#include <ctype.h>
*/
#include <string.h>
#include <sys/wait.h>
#include "socks.h"

extern	int	GetAddr();
extern	char	*inet_ntoa();

main(argc, argv)
int	argc;
char	*argv[];
{
	FILE		*infile, *outfile, *tempout;
	char	*tempfn;
	static char	buf[1024];
	char		temp[1024];
	char	lineout[1024];
	char		*bp;
	int		linenum = 0;
	struct in_addr	smask, dmask;
	char	*cmdp, *commentp;
	int	child_pid, wait_pid, exitcode;

	if (argc != 3) {
		fprintf(stderr," Usage: %s input_file output_file\n", argv[0]);
		exit(1);
	}

	if ((infile = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr,"Unable to open input file %s\n", argv[1]);
		exit(1);
	}
	
	if ((tempout = fopen(tempfn = tmpnam(NULL), "w")) == NULL) {
		fprintf(stderr, "Unable to create temporary file.\n");
		exit(1);
	}

	while (fgets(buf, sizeof(buf) - 1, infile) != NULL) {
		linenum++;
		/*
		**  Comments start with a '#' anywhere on the line
		*/
		cmdp = (char *)0;
		commentp = (char *)0;
		if ((bp = strchr(buf, '\n')) != NULL)
			*bp = '\0';
		strcpy(temp, buf);
		for (bp = buf; *bp; bp++) {
			if (*bp == ':') {
				*bp++ = '\0';
				cmdp = bp;
				break;
			} else if (*bp == '#') {
				*bp++ = '\0';
				commentp = bp;
				break;
			}
		}
		if ((bp = strtok(buf, " \t")) == NULL) {
			fprintf(tempout,"%s\n", temp);
			continue;
		}

		strcpy(lineout, bp);
		if ((bp = strtok(NULL, " \t")) == NULL)
			goto badline;
		strcat(lineout, "\t");
		strcat(lineout, bp);
		if (strncmp(bp, "@=", 2) == 0) {
			if ((bp = strtok(NULL, " \t")) == NULL)
				goto badline;
			strcat(lineout, "\t");
			strcat(lineout, bp);
		}
		if (strncmp(bp, "*=", 2) == 0) {
			if ((bp = strtok(NULL, " \t")) == NULL)
				goto badline;
			strcat(lineout, "\t");
			strcat(lineout, bp);
		}
		if ((bp = strtok(NULL, " \t")) == NULL)
				goto badline;
		GetAddr(bp, &smask);
		smask.s_addr = ~smask.s_addr;
		strcat(lineout, " ");
		strcat(lineout, inet_ntoa(smask));
		if ((bp = strtok(NULL, " \t")) == NULL)
			goto printline;
		if (strcmp(bp, "eq") && strcmp(bp, "neq") && strcmp(bp, "lt")
			&& strcmp(bp, "gt") && strcmp(bp, "le")
			&& strcmp(bp, "ge")) {
			strcat(lineout, "\t");
			strcat(lineout, bp);
			if ((bp = strtok(NULL, " \t")) == NULL)
				goto badline;
			GetAddr(bp, &dmask);
			dmask.s_addr = ~dmask.s_addr;
			strcat(lineout, " ");
			strcat(lineout, inet_ntoa(dmask));
		}
		if ((bp = strtok(NULL, " \t")) == NULL)
			goto printline;
		strcat(lineout, "\t");
		strcat(lineout, bp);
		if ((bp = strtok(NULL, " \t")) == NULL)
			goto badline;
		strcat(lineout, " ");
		strcat(lineout, bp);
printline:
		if (cmdp != NULL) {
			strcat(lineout, " :");
			strcat(lineout, cmdp);
		}
		if (commentp != NULL) {
			strcat(lineout, " #");
			strcat(lineout, commentp);
		}
		fprintf(tempout, "%s\n", lineout);
		continue;

badline:
		fprintf(stderr, "Invalid entry at line %d:\n%s\n", linenum, temp);
		fprintf(tempout, "%s\n", temp);
		continue;
	}

	fclose(infile);
	fclose(tempout);

/* Fork to do cp */
	switch (child_pid = fork()) {
	case 0:
		sprintf(lineout, "cp %s %s", tempfn, argv[2]);
		execl("/bin/sh", "sh", "-c", lineout, (char *)0);
	case -1:
		fprintf(stderr,"Uable to save to file %s. Results saved in file %s\n", argv[2], tempfn);
		exit(1);
	default:
		while ((wait_pid = wait(&exitcode)) != -1 && wait_pid != child_pid)
			;
		if (exitcode) {
			fprintf(stderr,"Uable to save to file %s. Results saved in file %s\n", argv[2], tempfn);
			exit(1);
		}
		unlink(tempfn);
		exit(0);
	}


	 	
}

