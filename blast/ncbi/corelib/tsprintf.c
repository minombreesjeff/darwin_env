/*   tsprintf.c
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
* File Name:  tsprintf.c
*
* Author:  Denis Vakatov
*
* Version Creation Date:   07/10/96
*
* $Revision: 6.10 $
*
* File Description:
*   	Memory- and MT-safe "sprintf()"
*
* Modifications:
* --------------------------------------------------------------------------
*
* $Log: tsprintf.c,v $
* Revision 6.10  2003/12/12 23:28:25  dondosha
* Correction for Opteron, at suggestion from Nicolas Joly
*
* Revision 6.9  2002/03/11 16:55:43  ivanov
* Fixed fp_count() -- error with round-up numbers
*
* Revision 6.8  2001/04/17 13:49:55  beloslyu
* changes for Linux PPC, contributed by Gary Bader <gary.bader@utoronto.ca>
*
* Revision 6.7  2001/03/23 14:04:14  beloslyu
* fix the name of strnlen to my_strnlen. It appears IBM's AIX has it's own 
* function with this name
*
* Revision 6.6  2000/12/28 21:25:14  vakatov
* Comment fixed
*
* Revision 6.5  2000/12/28 21:16:44  vakatov
* va_args():  use "int" to fetch a "short int" argument
*
* Revision 6.4  2000/07/19 20:54:48  vakatov
* minor cleanup
*
* Revision 6.3  1998/01/28 15:57:24  vakatov
* Nlm_TSPrintfArgs():  always ignore the "vsprintf()"'s return value;
* count the resultant string length using StrLen
*
* Revision 6.2  1997/12/04 22:05:35  vakatov
* Check for NULL string arg;  cut the output instead of crashing the program
*
* Revision 6.1  1997/11/26 21:26:33  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.0  1997/08/25 18:17:40  madden
* Revision changed to 6.0
*
* Revision 1.5  1997/07/15 16:51:48  vakatov
* vsprintf_count_args() -- allow "fmt" be NULL(just return 0, don't crash)
*
* Revision 1.4  1996/12/03 21:48:33  vakatov
* Adopted for 32-bit MS-Windows DLLs
*
* Revision 1.3  1996/07/23  16:23:20  epstein
* fix for non-SYSV UNIX systems (e.g., SunOS 4)
*
* Revision 1.2  1996/07/22  15:27:31  vakatov
* Fixed "%c"-formatting bug
*
* Revision 1.1  1996/07/16  20:02:06  vakatov
* Initial revision
*
* ==========================================================================
*/


#include <string.h>
#include <ctype.h>
#include <math.h>

#include <ncbi.h>

#include <tsprintf.h>

#ifndef va_copy
# ifdef __va_copy
#  define va_copy __va_copy
# else
#  define va_copy(d,s) ((d) = (s))
# endif
#endif

/***********************************************************************
 *  INTERNAL
 ***********************************************************************/

static size_t my_strnlen(const char * s, size_t count)
{
  const char *sc;

  for (sc = s; count-- && *sc != '\0'; ++sc)
    /* nothing */;
  return sc - s;
}


/* we use this so that we can do without the ctype library */
#define is_digit(c)	((c) >= '0' && (c) <= '9')

static int skip_atoi(const char **s)
{
  int i=0;

  while (is_digit(**s))
    i = i*10 + *((*s)++) - '0';
  return i;
}


#define ZEROPAD	1		/* pad with zero */
#define SIGNED	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SPECIAL	32		/* 0x */
#define LARGE	64		/* use 'ABCDEF' instead of 'abcdef' */


static int do_div(long *n, int base)
  {
    int res = ((unsigned long) *n) % (unsigned) base;
    *n =      ((unsigned long) *n) / (unsigned) base;
    return res;
  }


static int fp_count(double fp, char type, int size, int precision, int flags)
{
  char    xx_type  =  (char)tolower( type );
  double  fp_abs   =  (fp > 0) ? fp : -fp;
  double  power10  =  (fp_abs ? log10( fp_abs ) : 0.0);
  int     counter;

  if (precision < 0)
    precision = 6;

  {{
  switch ( xx_type )
    {
    case 'e':
      counter = 1 + 1 + precision + 5;
      break;
    case 'f':
      counter = (power10 > 0.0 ? (int)power10 + 1 : 0) + 1 + 1 + precision;
      break;
    case 'g':
      {
        int e_count = 1 + precision + 5;
        int f_count = 1 + precision +
          ((power10 < 0.0) ? (int)(-power10) + 2 : 0);
        counter = (f_count < e_count) ? f_count : e_count;
        break;
      }
    default:
      return 0;
    }
  }}

  if (precision == 0)
    counter--;

  if (((fp > 0)  &&  (flags & (PLUS|SPACE)))  ||  (fp < 0))
    counter++;

  return ((counter > size) ? counter : size);
}


static int number_count(long num, int base, int size, int precision, int type)
{
  int counter = 0;
  int i       = 0;

  if (type & LEFT)
    type &= ~ZEROPAD;
  if (base < 2 || base > 36)
    return 0;

  if ((type & (PLUS|SPACE))  &&  (num >= 0))
    {
      counter++;
      size--;
    }

  if ((type & SIGNED)  &&  (num < 0))
    {
      num = -num;
      counter++;
      size--;
    }

  if (type & SPECIAL) {
    if (base == 16)
      size -= 2;
    else if (base == 8)
      size--;
  }

  if (num == 0)
    i++;
  else while (num != 0)
    {
      do_div(&num, base);
      i++;
    }

  if (i > precision)
    precision = i;
  size -= precision;
  if (!(type&(ZEROPAD+LEFT)))
    while (size-- > 0)
      counter++;
  if (type & SPECIAL) {
    if (base==8)
      counter++;
    else if (base==16)
      counter += 2;
  }

  if (!(type & LEFT)) {
    while (size-- > 0)
      counter++;
  }
  while (i < precision--)
    counter++;
  while (i-- > 0)
    counter++;
  while (size-- > 0)
    counter++;

  return counter;
}


static size_t vsprintf_count_args(const Char PNTR fmt, va_list args,
                                  size_t *cut_fmt)
{
  size_t counter = 0;

  const Char PNTR start_fmt = fmt;
  unsigned long num;
  int base;
  int flags;         /* flags to number() */
  int field_width;   /* width of output field */
  int precision;     /* min. # of digits for integers; max
			number of chars for from string */
  int qualifier;     /* 'h', 'l', or 'L' for integer fields */

  *cut_fmt = 0;

  if ( !fmt )
    return 0;

  for ( ;  *fmt;  fmt++)
    {
      if (*fmt != '%')
        {
          counter++;
          continue;
        }
			
      /* process flags */
      flags = 0;
    repeat:
      ++fmt;		/* this also skips first '%' */
      switch (*fmt) {
      case '-': flags |= LEFT; goto repeat;
      case '+': flags |= PLUS; goto repeat;
      case ' ': flags |= SPACE; goto repeat;
      case '#': flags |= SPECIAL; goto repeat;
      case '0': flags |= ZEROPAD; goto repeat;
      }
		
      /* get field width */
      field_width = -1;
      if (is_digit(*fmt))
        field_width = skip_atoi(&fmt);
      else if (*fmt == '*') {
        ++fmt;
        /* it's the next argument */
        field_width = va_arg(args, int);
        if (field_width < 0) {
          field_width = -field_width;
          flags |= LEFT;
        }
      }

      /* get the precision */
      precision = -1;
      if (*fmt == '.') {
        ++fmt;	
        if (is_digit(*fmt))
          precision = skip_atoi(&fmt);
        else if (*fmt == '*') {
          ++fmt;
          /* it's the next argument */
          precision = va_arg(args, int);
        }
        if (precision < 0)
          precision = 0;
      }

      /* get the conversion qualifier */
      qualifier = -1;
      if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
        qualifier = *fmt;
        ++fmt;
      }

      /* default base */
      base = 10;

      switch (*fmt) {
      case 'c':
        (void)va_arg(args, int);
        if (field_width > 0)
          counter += field_width;
        else
          counter++;
        continue;

      case 's':
        {
          int len;
          const char *s = va_arg(args, const char *);
#ifdef OS_UNIX_PPCLINUX
/*
 *  comment from Gary Bader <gary.bader@utoronto.ca>
 *	LinuxPPC has problems with using variable arguments in certain cases.
 *	I'm not completely sure if this is necessary, but it works
 */
			counter += 8;   /*add 8 for PPCLINUX*/
#endif
          if ( !s ) {
            while (*fmt != '%')
              fmt--;
            *cut_fmt = fmt - start_fmt;
            va_end( args );
            return counter;
          }
          len = my_strnlen(s, precision);
          if (len < field_width)
            len = field_width;
          counter += len;
        }
        continue;

      case 'p':
        if (field_width == -1) {
          field_width = 2 * sizeof(void *);
          flags |= ZEROPAD;
        }
        counter += number_count((unsigned long) va_arg(args, void *), 16,
			  	field_width, precision, flags);
        continue;

      case 'n':
        if (qualifier == 'l') {
          long * ip = va_arg(args, long *);
          *ip = (long)counter;
        } else {
          int * ip = va_arg(args, int *);
          *ip = (int)counter;
        }
        continue;

      case 'f':
      case 'F':
      case 'e':
      case 'E':
      case 'g':
      case 'G':
        counter += fp_count(va_arg(args, double), *fmt,
                            field_width, precision, flags);
        continue;


        /* integer number formats - set up the flags and "break" */
      case 'o':
        base = 8;
        break;

      case 'X':
      case 'x':
        base = 16;
        break;

      case 'd':
      case 'i':
        flags |= SIGNED;
      case 'u':
        break;

      default:
        if (*fmt != '%')
          counter++;
        if (*fmt)
          counter++;
        else
          --fmt;
        continue;
      }

      if (qualifier == 'l')
        num = va_arg(args, unsigned long);
      else if (qualifier == 'h')
        if (flags & SIGNED)
          num = va_arg(args, int);  /* sic! -- not a "short"! */
        else
          num = va_arg(args, unsigned int);
      else if (flags & SIGNED)
        num = va_arg(args, int);
      else
        num = va_arg(args, unsigned int);
      counter += number_count(num, base, field_width, precision, flags);
    }

  va_end( args );
  return counter;
}


/***********************************************************************
 *  EXTERNAL
 ***********************************************************************/

#ifdef VAR_ARGS
NLM_EXTERN const Char PNTR Nlm_TSPrintf(fmt, va_alist)
const Char PNTR fmt;
va_dcl
#else
NLM_EXTERN const Char PNTR Nlm_TSPrintf(const Char PNTR fmt, ...)
#endif
{
  va_list args;
  const Char PNTR str = NULL;

#ifdef VAR_ARGS
  va_start(args);
#else
  va_start(args, fmt);
#endif

  str = Nlm_TSPrintfArgs(fmt, args);

  va_end(args);
  return str;
}
 
  
NLM_EXTERN const Char PNTR Nlm_TSPrintfArgs(const Char PNTR fmt, va_list args)
{
  size_t  parsed_size;
  int     n_written;
  CharPtr temp_buf;
  size_t  cut_fmt;
  char   *x_fmt;
  va_list save;

#ifdef OS_UNIX_PPCLINUX
/*
 *  comment from Gary Bader <gary.bader@utoronto.ca>
 *	LinuxPPC has problems with using variable arguments in certain cases.
 *	I'm not completely sure if this is necessary, but it works
 */
  parsed_size = 2048;
  cut_fmt = 0;
#else
  va_copy(save, args);
  parsed_size = vsprintf_count_args(fmt, save, &cut_fmt);
  va_end(save);
  if (parsed_size == 0)
    return NULL;
#endif

  temp_buf = Nlm_GetScratchBuffer(parsed_size + 1);
  if (temp_buf == NULL)
    return NULL;

  if ( cut_fmt ) {
    if ( !(x_fmt = (char *)Malloc(cut_fmt+1)) )
      return NULL;
    MemCpy(x_fmt, fmt, cut_fmt);
    x_fmt[cut_fmt] = '\0';
  }
  else
    x_fmt = (char *)fmt;

  temp_buf[0] = '\0';
  vsprintf(temp_buf, x_fmt, args);

  n_written = StrLen(temp_buf);
  if (n_written > 0  &&  (size_t)n_written > parsed_size)
    abort();

  if ( cut_fmt )
    Free(x_fmt);

  return temp_buf;
}


#ifdef TEST_MODULE_TSPRINTF

/***********************************************************************
 *  TEST
 ***********************************************************************/

#include <stdio.h>


#ifdef VAR_ARGS
static size_t vsprintf_count(cut_fmt, fmt, va_alist)
size_t *cut_fmt;
const char *fmt;
va_dcl
#else
static size_t vsprintf_count(size_t *cut_fmt, const Char PNTR fmt, ...)
#endif
{
  va_list args;
  size_t  counter = 0;

#ifdef VAR_ARGS
  va_start(args);
#else
  va_start(args, fmt);
#endif

  counter = vsprintf_count_args(fmt, args, cut_fmt);

  va_end(args);
  return counter;
}


#define test(n,t,v) \
{{ \
  size_t cut_fmt; \
  int    n_actual; \
  char  *x_fmt; \
  char   xx_fmt[1024]; \
  size_t n_parsed = vsprintf_count(&cut_fmt, fmt[n][t], v);\
  if (n_parsed  &&  !cut_fmt)  { x_fmt = (char *)fmt[n][t]; } \
  else { ASSERT( cut_fmt < sizeof(xx_fmt) ); \
    MemCpy(xx_fmt, fmt[n][t], cut_fmt);  xx_fmt[cut_fmt] = '\0'; x_fmt = xx_fmt; /* printf("::%d  '%s'  '%s'\n", (int)cut_fmt, xx_fmt, fmt[n][t]); */ } \
  n_actual = sprintf(str, x_fmt, v);\
  if ((long)n_parsed != (long)n_actual) {\
    printf("fmt='%s', parsed= %d,\t actual= %d:\t '%s'\n",\
           fmt[n][t], (int)n_parsed, n_actual, str);\
    if ((long)n_parsed < (long)n_actual)\
      abort(); \
  } \
  Nlm_ErrPostEx(SEV_WARNING, 7, 3, fmt[n][t], v); \
}}


static Int2 TEST__vsprintf_count( void )
{
  char str[8182];
  int    i = 11000;
  long   l = 222222000;
  float  f = (float)-0.01234567890123456;/*(float)339348798353465673479834573.33385873457487657643583475874e10;*/
  double d = -0.01234567890123456;/*444439837983783875378.44493759837895639853746365387698374E+100;*/
  static const char s  [] = "01234567890123456789012345678901234567890";
  static const char sz [] = "";
  const  char       *sNULL = NULL;
  static const char *fmt[10][5] =
  {
    "i = %3d",    "l = %5ld",    "f = %8.3f",   "d = %8.3g",   "s = '%s'",
    "i = % d",    "l = %ld",     "f = %f",      "d = %g",      "s = '%s'",
    "i = %8.0d",  "l = %-8.0ld", "f = %20.0f",  "d = %20.0g",  "s = '%3s'",
    "i = %11.3d", "l = %11.3ld", "f = %33.3f",  "d = %33.3g",  "s = '%s'",
    "i = %-5.5d", "l = %5.5ld",  "f = %10.10f", "d = %10.10g", "s = '%20s'",
    "",           "l = %+5.1ld", "f = %-9.7f",  "d = %5.10g",  "s = '%s20.3'",
    "i = %12.6d", "l = %15ld",   "f = %10.5f",  "d = %44.10g", "s = '%19s'",
    "i = %10.9d", "l = %15.5ld", "f = %22.10f", "d = %16.8g",  "s = '%s'",
    "i = %6.7d",  "l = %20.9ld", "f = %0.10f",  "d = %22g",    "%8s = s",
    "i = %11.2d", "l = %+2.7ld", "f = %8.7f",   "d = %33g",    "s = '%-6s'"
  };

  size_t n;
  int iii;

  {{
    static char *ptr = "eeeeeee"; 
    ErrPostEx(SEV_WARNING, 7, 3, "%c", *ptr);
  }}

  for (iii = 0;  iii < 100;  iii++)
    {
      for (n = 0;  n < sizeof( fmt ) / sizeof(const char *) / 5;  n++)
        {
          printf("Test: %ld\n", (long) n);
          i = -i;
          l = -l;
          f = -f;
          d = -d;
          test(n,0,i);
          test(n,1,l);
          test(n,2,f);
          test(n,3,d);
          test(n,4,s);
        }
/*      scanf("%s", str);*/

      i = (int)    ((l - f + 100) * d);
      l = (long)   ((i - d * cos( f ) + 10) * l);
      f = (float)  ((i - l + f + 888) * sin( d ) + i);
      d = (double) ((cos( d ) + sin( i )) * f / (l * l + 1000 + f * cos( f )));
    }


  printf("\nTest ZERO:\n");
  i = 0;
  l = 0;
  f = (float)0;
  d = 0;
  for (n = 0;  n < sizeof( fmt ) / sizeof(const char *) / 5;  n++)
    {
      printf("Test: %ld\n", (long) n);
      test(n,0,i);
      test(n,1,l);
      test(n,2,f);
      test(n,3,d);
      test(n,4,sz);
      test(n,4,sNULL);
    }
  scanf("%s", str);

  return 0;  
}


int main()
{
  return TEST__vsprintf_count();
}

#endif  /* TEST_MODULE_TSPRINTF */
