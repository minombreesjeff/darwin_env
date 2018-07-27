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
 * Revision 6.4  2003/04/22 14:20:26  camacho
 * Added missing include
 *
 * Revision 6.3  2003/03/20 16:22:16  camacho
 * Added reverse mode
 *
 * Revision 6.2  2001/05/25 19:28:21  vakatov
 * Nested comment typo fixed
 *
 * Revision 6.1  2000/02/09 19:44:30  madden
 * Converts text gi list to binary gi list
 */


#include <ncbi.h>
#include <blast.h>
#include <blastdef.h>
#include <readdb.h>


static Args myargs [] = {
  { "Input gilist", 
	NULL, NULL, NULL, FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  { "output (binary) gilist", 
	NULL, NULL, NULL, FALSE, 'o', ARG_FILE_IN, 0.0, 0, NULL},
  { "Reverse mode (binary to text)", 
	"F", NULL, NULL, TRUE, 'r', ARG_BOOLEAN, 0.0, 0, NULL}
};

static void MakeGiFileText(CharPtr input_file, CharPtr output_file)
{
    FILE *outfp = NULL;
    BlastDoubleInt4Ptr gilist = NULL;
    Int4 i, ngis;

    if (!(outfp = FileOpen(output_file, "w"))) {
        ErrPostEx(SEV_ERROR, 0, 0, "Unable to open file %s", output_file);
        return;
    }

    gilist = GetGisFromFile(input_file, &ngis);

    for (i = 0; i < ngis; i++)
        fprintf(outfp, "%ld\n", gilist[i].gi);
    
    MemFree(gilist);

    return;
}

Int2 Main (void)
{
    if (! GetArgs ("gil2bin", DIM(myargs), myargs))
        return (1);

    if (myargs[2].intvalue == 0)
        readdb_MakeGiFileBinary(myargs[0].strvalue, myargs[1].strvalue);
    else
        MakeGiFileText(myargs[0].strvalue, myargs[1].strvalue);

	return 0;
}
	

