/* $Id: debruijn.c,v 1.4 2004/02/09 21:24:59 ucko Exp $

* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's offical duties as a United States Government employee and
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

*/

static char const rcsid[] = "$Id: debruijn.c,v 1.4 2004/02/09 21:24:59 ucko Exp $";

/*
 * example driver for de Bruijn sequences.
 *
 * this code generates all n-mers over a protein
 * or dna alphabet. useful for creating test sequences.
 */

#include <ncbi.h>
#include <algo/blast/core/lookup_util.h>

static Args myargs[] = {
  { "word size",
    NULL, NULL, NULL, FALSE, 'n', ARG_INT, 0.0, 0, NULL },
  { "alphabet\n"
    "(supply 'ncbistdaa' or 'ncbi2na' for standard\n"
    "alphabets, or supply your own alphabet)\n",
    NULL, NULL, NULL, FALSE, 'a', ARG_STRING, 0.0, 0, NULL },
};

Uint1 ncbistdaa[] = "-abcdefghiklmnpqrstvwxyzu*";
Uint1 ncbi2na[] = "acgt";

Int2 Main(void)
{
  Int4 i;
  Int4 n, k;
  Uint1 *output;
  Int4 outputsize;
  Uint1 *alphabet=NULL;
  
  if ( ! GetArgs("debruijn", sizeof(myargs)/sizeof(myargs[0]), myargs) )
   return 1;
  
  n = myargs[0].intvalue;

  if (n < 1)
    {
      fprintf(stderr,"n must be greater than one.\n");
      return 1;
    }

  alphabet = myargs[1].strvalue;

  if (strcmp("ncbistdaa", myargs[1].strvalue) == 0)
    alphabet = ncbistdaa;

  if (strcmp("ncbi2na", myargs[1].strvalue) == 0)
    alphabet = ncbi2na;

  k = strlen(alphabet);
  
  /* output array needs:
   * k^n bytes - to store the de Bruijn sequence
   * n-1 bytes - to unwrap (see below)
   * 1   byte  - for the terminating NUL
   */

  outputsize = iexp(k,n) + (n-1);
  output = (char *) malloc(outputsize + 1);

  /* compute the (n,k) de Bruijn sequence */  
  debruijn(n,k,output,alphabet);
  
  /* We don't want a true cyclical de Bruijn sequence; we want
   * all words in a straight line- copy the first n-1 letters
   * to the end.
   */

  for(i=0;i<(n-1);i++)
    output[outputsize-n+1+i] = output[i];

  /* Terminate the string. */

  output[outputsize] = '\0';

  puts(output);

  free(output);
}
