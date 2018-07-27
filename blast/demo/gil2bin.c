/**************************************************************************
*                                                                         *
*                             COPYRIGHT NOTICE                            *
*                                                                         *
* This software/database is categorized as "United States Government      *
* Work" under the terms of the United States Copyright Act.  It was       *
* produced as part of the author's official duties as a Government        *
* employee and thus can not be copyrighted.  This software/database is    *
* freely available to the public for use without a copyright notice.      *
* Restrictions can not be placed on its present or future use.            *
*                                                                         *
* Although all reasonable efforts have been taken to ensure the accuracy  *
* and reliability of the software and data, the National Library of       *
* Medicine (NLM) and the U.S. Government do not and can not warrant the   *
* performance or results that may be obtained by using this software,     *
* data, or derivative works thereof.  The NLM and the U.S. Government     *
* disclaim any and all warranties, expressed or implied, as to the        *
* performance, merchantability or fitness for any particular purpose or   *
* use.                                                                    *
*                                                                         *
* In any work or product derived from this material, proper attribution   *
* of the author(s) as the source of the software or data would be         *
* appreciated.                                                            *
*                                                                         *
**************************************************************************/

/* $Revision 1.0$
 *
 * $Log: gil2bin.c,v $
 * Revision 6.2  2001/05/25 19:28:21  vakatov
 * Nested comment typo fixed
 *
 * Revision 6.1  2000/02/09 19:44:30  madden
 * Converts text gi list to binary gi list
 */


#include <ncbi.h>
#include <readdb.h>


#define NUMARG 2

static Args myargs [NUMARG] = {
  { "Input gilist", 
	NULL, NULL, NULL, FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  { "output (binary) gilist", 
	NULL, NULL, NULL, FALSE, 'o', ARG_FILE_IN, 0.0, 0, NULL}
};

Int2 Main (void)
 
{
        if (! GetArgs ("gil2bin", NUMARG, myargs))
        {
                return (1);
        }

	readdb_MakeGiFileBinary(myargs[0].strvalue, myargs[1].strvalue);

	return 0;
}
	

