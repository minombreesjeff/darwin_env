/* $Id: bl2bag.c,v 1.3 2003/07/15 19:57:11 coulouri Exp $
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
* File Name:  $RCSfile: bl2bag.c,v $
*
* Initial Creation Date: 10/23/2000
*
* $Revision: 1.3 $
*
* File Description:
*        BLAST 2 Sequences CGI program for some drawing
*
* $Log: bl2bag.c,v $
* Revision 1.3  2003/07/15 19:57:11  coulouri
* use sigaction()
*
* Revision 1.2  2003/05/09 21:09:04  dondosha
* Removed unused variables
*
* Revision 1.1  2002/12/02 18:05:53  dondosha
* Moved from different locations to a common one
*
* Revision 6.4  2002/08/06 21:32:59  dondosha
* Added sys/resource.h include for rlimit
*
* Revision 6.3  2002/08/06 21:26:25  dondosha
* Changed exit to return in Main
*
* Revision 6.2  2000/11/03 16:37:09  shavirin
* Added standrard header and started logging.
*
*
* ==========================================================================
*/

#include <signal.h>
#include <ncbi.h>
#include <asn.h>

#include <ncbigif.h>
#include <gifgen.h>
#include <sys/resource.h>

typedef struct {
	CharPtr tag;
	CharPtr val;
} TAG;

static TAG tag[] = {
	{ NULL, NULL }
};

static TAG empty_utag = { NULL, NULL };
static Int4 nutag = 1;
static TAG *utag = &empty_utag;

TAG *get_utag(void)
{
	return utag;
}

static void content(void)
{
	static Boolean was = FALSE;

	if (was) {
		return;
	}
	printf("Content-type: text/html\n\n");
	was = TRUE;
}

static void AbortPage(CharPtr mess)
{
	content();
	printf("<TITLE>ERROR</TITLE>\n");
		printf("<h2>\n");
	printf("<img src=images/confused.gif align=middle>\n");
	printf("%s</h2>\n", mess);
	exit(1);
}

static Int4 hexdigit(Char c)
{
   switch (c) {
   default: return 0;
   case '0': return 0;
   case '1': return 1;
   case '2': return 2;
   case '3': return 3;
   case '4': return 4;
   case '5': return 5;
   case '6': return 6;
   case '7': return 7;
   case '8': return 8;
   case '9': return 9;
   case 'a':
   case 'A': return 10;
   case 'b':
   case 'B': return 11;
   case 'c':
   case 'C': return 12;
   case 'd':
   case 'D': return 13;
   case 'e':
   case 'E': return 14;
   case 'f':
   case 'F': return 15;
   }
}

static void TimeExpired(Int4 i)
{
	content();
	printf("<HTML>\n");
	printf("<HEAD>\n");
	printf("<TITLE>\n");
	printf("Time Expired\n");
	printf("</TITLE>\n");
	printf("</HEAD>\n");
	printf("<BODY>\n");
	printf("<H1>\n");
	/*printf("<img src=/IMAGE/time-exp.gif align=middle hspace=21>\n");*/
	printf("Time expired\n");
	printf("</H1>\n");
	printf("<HR>\n");
	printf("</BODY>\n");
	printf("</HTML>\n");
	exit(0);
}

void fill_tag(CharPtr str, TAG *tag)
{
	Int4 i, l;
	CharPtr s;
	TAG *buftag;

	for (i=0; tag[i].tag != NULL; i++) {
		l = strlen(tag[i].tag);
		if (strncmp(tag[i].tag, str, l) == 0) {
			str += l;
			for (s = str, l = 0; *s != '\0' && *s != '&'; s++) {
				if (!IS_WHITESP(*s)) {
					l++;
				}
			}
			if (l > 0) {
				tag[i].val = str;
			}
			return;
		}
	}
	if ((buftag = (TAG *)MemNew((nutag+1) * sizeof(utag[0]))) == NULL)
           AbortPage("Not enough memory");
        
	if (nutag > 1) {
		MemCpy(buftag, utag, nutag * sizeof(utag[0]));
	/*	free(utag);*/
	}
	utag = buftag;
	buftag = utag + nutag - 1;
	buftag->tag = str;
	for (s = str; *s != '\0' && *s != '='; s++) ;
	if (*s == '=') {
		*s++ = '\0';
		buftag->val = s;
	}
	buftag[1].tag = NULL;
	buftag[1].val = NULL;
	nutag++;
}

void get_str_tag(CharPtr str, TAG *tag)
{
	CharPtr s, beg;
        Char c;
	Int4 i1, i2;

	beg = str;
	for (s = str; *s != '\0'; s++) {
		if (*s == '+') {
			*s = ' ';
		} else if (*s == '&') {
			fill_tag(beg, tag);
			beg = s + 1;
			*s = '\0';
		} else if (*s == '%') {
			i1 = hexdigit(s[1]);
			i2 = hexdigit(s[2]);
			c = (i1 << 4) + i2;
			if (c == '\r') {
				strcpy(s, s+3);
				s--;
			} else {
				strcpy(s+1, s+3);
				*s = c;
			}
		}
	}
	fill_tag(beg, tag);
}

/* RLIMIT_CPU      The maximum amount of CPU  time  in  seconds
                     used  by  a  process.   This is a soft limit */
#define limitCPU 300;

Int4 get_tag(tag, method)
TAG tag[];
CharPtr method;
{
	Int4 n;
	CharPtr str;
	static Char buf[133];
	struct sigaction sa;
	sigset_t sigset;
	struct rlimit rl;

	getrlimit(RLIMIT_CPU, &rl);
	rl.rlim_cur = limitCPU;
	rl.rlim_max = rl.rlim_cur + 30;

	setrlimit(RLIMIT_CPU, &rl);

	sigfillset(&sigset);
	sa.sa_mask = sigset;
	sa.sa_flags = SA_RESETHAND | SA_RESTART;
	sa.sa_handler = TimeExpired;
	sigaction(SIGXCPU, &sa, NULL);

	str = getenv("REQUEST_METHOD");
	if (str == NULL) {
		return FALSE;
	}
	if (method == NULL) {
		method = str;
	}
	if (StringCmp(method, "POST") == 0) {
		if (StringCmp(str, "POST") != 0) {
			sprintf(buf, "Not a METHOD of POST (%s).", str);
                        AbortPage(buf);
		}
		str = getenv("CONTENT_TYPE");
		if (str == NULL) {
			sprintf(buf, "No CONTENT_TYPE");
			AbortPage(buf);
		}
		if (StringCmp(str, "application/x-www-form-urlencoded") != 0) {
			sprintf(buf, "Wrong CONTENT_TYPE: (%s).", str);
                        AbortPage(buf);
		}
		if ((n = atoi(getenv("CONTENT_LENGTH"))) <= 0) {
			    AbortPage("No arguments");
		}
		if ((str = (CharPtr)MemNew((n + 1)*sizeof(Char))) == NULL)
                   AbortPage("Not enough memory");
		if (fread(str, 1, n, stdin) != n) {
			AbortPage("Arguments were not transfered correctly");
		}
	} else if (StringCmp(method, "GET") == 0) {
		if (StringCmp(str, "GET") != 0) {
			sprintf(buf, "Not a METHOD of GET (%s).", str);
			    AbortPage(buf);
		}
		str = getenv("QUERY_STRING");
		if (str == NULL) {
			AbortPage("No query information to decode");
		}
	} else {
		sprintf(str, "Bad requested method: %s", method);
		AbortPage(str);
	}
	get_str_tag(str, tag);
	return TRUE;
}

static gdImagePtr GIF;

#define WIDTH    28
#define HEIGHT   28

static Int4 height = 100, width=100;

static Int4 dig62(char c)
{
	Int4 num;

	num = isdigit(c) ? c-'0' :
		isupper(c) ? c-'A'+10 :
		islower(c) ? c-'a'+36 : 0;
	return num;
}

static Int4 sym2num(CharPtr PNTR ps)
{
	Int4 num;
	CharPtr s = *ps;

	num = dig62(*s++) * 62;
	num += dig62(*s++);
	*ps = s;
	return num;
}

static Int4 hex1(char c)
{
	return isdigit(c) ? c-'0' :
			isupper(c) ? c-'A'+10 :
			islower(c) ? c-'a'+10 : 0;
}

static Int4 hex2(CharPtr s)
{
	return hex1(s[0]) * 16 +  hex1(s[1]);
}

static void dxy2point(po, x, y)
gdPoint *po;
FloatHi x, y;
{
	po->x = x + .5;
	po->y = y + .5;
}

static void xy2point(po, x, y)
gdPoint *po;
Int4 x, y;
{
	po->x = x;
	po->y = y;
}

Int2 Main(void)
{
	CharPtr s, str;
	TAG *utag;
	Int4 x1, x2, y1, y2, w, xw, yw;
	Int4 col1, col2, col3, colw, colb, colp;
	FloatHi al, wsin, wcos;
	Int4 rect, six;
	static gdPoint po[6];

	get_tag(tag, NULL);
	utag = get_utag();
	for (; utag->tag != NULL && utag->tag[0] == '\0'; utag++) ;
	if ((str = utag->tag) == NULL) {
		AbortPage("No arguments");
	}
	s = str;
	width = sym2num(&s);
	height = sym2num(&s);
	if (*s == '-') {
		s++;
		if ((GIF = gdImageCreate(width, height)) == NULL) {
			AbortPage("No memory to create picture");
		}
		colw = gdImageColorAllocate(GIF, 255, 255, 255);
		gdImageColorTransparent(GIF, colw);
	} else {
		col1 = hex2(s+0);
		col2 = hex2(s+2);
		col3 = hex2(s+4);
		s += 7;
		if ((GIF = gdImageCreate(width, height)) == NULL) {
			AbortPage("No memory to create picture");
		}
		colw = gdImageColorAllocate(GIF, col1, col2, col3);
	}
	colb = gdImageColorAllocate(GIF, 1, 1, 1);
	gdImageFilledRectangle(GIF, 0, 0, width-1, height-1, colw);
	gdImageRectangle(GIF, 0, 0, width-1, height-1, colb);
	do {
		rect = FALSE;
		six = FALSE;
		if (*s == 'r') {
			rect = TRUE;
			s++;
		} else if (*s == 's') {
			six = TRUE;
			s++;
		}
		if (*s == '(') {
			AbortPage("Bad format (no color)");
		} else {
			col1 = hex2(s);
			col2 = hex2(s+2);
			col3 = hex2(s+4);
			s += 6;
		}
		colp = gdImageColorAllocate(GIF, col1, col2, col3);
		if (isdigit(*s)) {
			w = atoi(s);
			for (; isdigit(*s); s++) ;
		} else {
			w = 0;
		}
		if (*s == '(') {
			s++;
		} else {
			AbortPage("Bad format (no '(')");
		}
		if (*s != ')') {
			do {
				x1 = sym2num(&s);
				y1 = sym2num(&s);
				x2 = sym2num(&s);
				y2 = sym2num(&s);
				al = atan2((FloatHi) (y2-y1), (FloatHi) (x2-x1));
				wsin = w * sin(al);
				wcos = w * cos(al);
				if (rect) {
					gdImageFilledRectangle(GIF, x1, y1, x2, y2, colp);
					for (; w > 0; w--) {
						gdImageRectangle(GIF, x1-w, y1-w, x2+w, y2+w, colb);
					}
				} else if (six) {
					xw = (x1 < x2) ? w : -w;
					yw = (y1 < y2) ? w : -w;
					xy2point(po  , x1   , y1   );
					xy2point(po+1, x1+xw, y1   );
					xy2point(po+2, x2   , y2-yw);
					xy2point(po+3, x2   , y2   );
					xy2point(po+4, x2-xw, y2   );
					xy2point(po+5, x1   , y1+yw);
					gdImageFilledPolygon(GIF, po, 6, colp);
				} else if (w == 0) {
					gdImageLine(GIF, x1, y1, x2, y2, colp);
				} else {
					dxy2point(po  , x1 - wsin, y1 + wcos);
					dxy2point(po+1, x1 + wsin, y1 - wcos);
					dxy2point(po+2, x2 + wsin, y2 - wcos);
					dxy2point(po+3, x2 - wsin, y2 + wcos);
					gdImageFilledPolygon(GIF, po, 4, colp);
					gdImagePolygon(GIF, po, 4, colb);
				}
			} while (*s != ')' && *s != '\0') ;
		}
		if (*s != '\0') {
			s++;
		}
	} while (*s != '\0') ;
	printf("Content-type: image/gif\n\n");
	gdImageGif(GIF, stdout);
	return 0;
}
