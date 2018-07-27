/*   sequinx.c
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
* File Name:  sequinx.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   1/22/95
*
* $Revision: 6.0 $
*
* File Description:  This file is for you to modify to add your own custom functions
*                       to sequin. 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#include "sequin.h"

                /* if DO_DEMO is define when this is compiled and sequiny.c is compiled
				   and linked, then the simple demo of adding a proceedure of your own
				   (in this case to edit Bioseqs, but it could be anything) will work. */


#ifdef DO_DEMO
#include "sequiny.h"
#endif

extern void InitSequinExtras (void)
{

#ifdef DO_DEMO
REGISTER_MY_BIOSEQ_EDIT;   /* register the function in sequiny.c */
                           /* tough isn't it? */
#endif

}

extern void FiniSequinExtras (void)

{
}


