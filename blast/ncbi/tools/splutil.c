static char const rcsid[] = "$Id: splutil.c,v 6.7 2003/05/30 17:25:38 coulouri Exp $";

/*   splutil.c
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* File Name:  splutil.c
*
* Author:  Sarah Wheelan
*
* $Revision: 6.7 $
*
* Contents:  splice site matrices and associated utilites 
*
* ==========================================================================
*
* $Log: splutil.c,v $
* Revision 6.7  2003/05/30 17:25:38  coulouri
* add rcsid
*
* Revision 6.6  2000/04/22 15:56:04  wheelan
* changed is_donor to take a FloatHiPtr instead of returning a FloatHi
*
* Revision 6.5  1998/09/24 12:46:17  wheelan
* put in log and revision stuff
*
*
*
*/

#include <ncbi.h>
#include <sequtil.h>
#include <sqnutils.h>
#include <splutil.h>
/**********
#include <salutil.h>
#include <bandalgn.h>
#include <blast.h>
**********/

extern void is_donor (CharPtr str, Int4 len, FloatHiPtr score)
{
  FloatHi one[4]={0.35, 0.35, 0.19, 0.11};
  FloatHi two[4]={0.59, 0.13, 0.14, 0.14};
  FloatHi three[4]={0.08, 0.02, 0.82, 0.08};
  FloatHi four[4]={0.01, 0.01, 1.00, 0.01};
  FloatHi five[4]={0.01, 0.01, 0.01, 1.00};
  FloatHi six[4]={0.51, 0.03, 0.43, 0.03};
  FloatHi seven[4]={0.71, 0.08, 0.12, 0.09};
  FloatHi eight[4]={0.06, 0.05, 0.84, 0.05};
  FloatHi nine[4]={0.15, 0.16, 0.17, 0.52};
  FloatHi score1;
  Int4    i;
  Int4    PNTR num=NULL;

  if (score == NULL)
     return;
  if ((num = MemNew((len+2)*sizeof(Int4)))==NULL) {
    /* memory error */	
    return;
  }

  for (i = 0; i <= len; i++){
    if (str[i]=='A')
      num[i] = 0;
    if (str[i]=='C')
      num[i] = 1;
    if (str[i]=='G')
      num[i] = 2;
    if (str[i]=='T')
      num[i] = 3;
  }
  score1 = one[num[0]];
  score1 *= two[num[1]];
  score1 *= three[num[2]];
  score1 *= four[num[3]];
  score1 *= five[num[4]];
  score1 *= six[num[5]];
  score1 *= seven[num[6]];
  score1 *= eight[num[7]];
  score1 *= nine[num[8]];

  MemFree(num);
  num=NULL;

  *score = score1;
  return;
}

extern Int4 getSplicePos (CharPtr str, Int4 overlaplength)
{
  Int4     offset = -1;
  Int4     xcursor = 0;
  Int4     c;
  FloatHi    topscore = 0.00,score; 
  Char	     tmpstr[9];

  if (str == NULL)
    return 0;

  while (xcursor <= overlaplength)
    { 
      for (c = 0; c < 9; c++)
      {
        tmpstr[c] = str[xcursor+c];
      }
      is_donor(tmpstr, 8, &score);
      if (score > topscore)
      {
        topscore = score;
        offset = xcursor;
      }
      xcursor += 1;
    }
  if (topscore > 0 && offset >=0)
    return offset;
  return 0;
} 


extern FloatHi is_acceptor (CharPtr str, Int4 len)
{
  FloatHi aone[4]={0.06, 0.44, 0.05, 0.44};
  FloatHi atwo[4]={0.08, 0.37, 0.05, 0.49};
  FloatHi athree[4]={0.27, 0.28, 0.22, 0.23};
  FloatHi afour[4]={0.04, 0.74, 0.001, 0.22};
  FloatHi afive[4]={1.00, 0.01, 0.01, 0.01};
  FloatHi asix[4]={0.01, 0.01, 0.99, 0.01};
  FloatHi aseven[4]={0.25, 0.16, 0.50, 0.09};
  FloatHi ascore;
  Int4   i;
  Int4 PNTR num;
  
  
  if ((num = MemNew(len*sizeof(Int4)))==NULL) {
    /* memory error */	
    return(0);
  }

  for (i = 0; i <= len; i++){
    if (str[i]=='A')
      num[i] = 0;
    if (str[i]=='C')
      num[i] = 1;
    if (str[i]=='G')
      num[i] = 2;
    if (str[i]=='T')
      num[i] = 3;
  }


  ascore = aone[num[len-6]];
  ascore *= atwo[num[len-5]];
  ascore *= athree[num[len-4]];
  ascore *= afour[num[len-3]];
  ascore *= afive[num[len-2]];
  ascore *= asix[num[len-1]];
  ascore *= aseven[num[len]];

  MemFree(num);
  num=NULL;

  return ascore;

}


  

