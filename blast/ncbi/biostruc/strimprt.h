/* ===========================================================================
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
* File Name:  strimprt.h
*
* Author:  NCBI
*
* Version Creation Date:   Circa 1994
*
* $Revision: 6.1 $
*
* File Description: General Header For Extra MMDB Defines
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: strimprt.h,v $
* Revision 6.1  2003/12/09 15:55:32  bauer
* renamed Ascii and Binart to BIOSTRUC_ASCII and BIOSTRUC_BINARY
*
* Revision 6.0  1997/08/25 18:11:53  madden
* Revision changed to 6.0
*
* Revision 5.3  1997/04/02 20:20:19  levitsky
* suppress NLM_EXTERN_LOADS redefinition warning
*
 * Revision 5.2  1997/03/10  22:10:35  hogue
 * Added RCS Logging Header.
 *
*
* ==========================================================================
*/

#ifdef NLM_EXTERN_LOADS
#undef NLM_EXTERN_LOADS
#endif
#define  NLM_EXTERN_LOADS { AllObjLoad(); }
#include <asn.h>
#include <objall.h>

/* legitimate hack */
#define struct_Org_ref orgref
#define struct_User_object userobj
#define struct_Alternate_conformation_id valnode
#define struct_AtomPntr valnode

/* illegitimate hacks */

#include <objmmdb1.h>
#include <objmmdb2.h>
#include <objmmdb3.h>

#define ResidueGraphPtr_local 1
#define ResidueGraphPtr_biostruc 2
#define ResidueGraphPtr_standard_graphs 3

#define BIOSTRUC_ASCII 0
#define BIOSTRUC_BINARY 1

#define PDBSECS 0
#define NCBISECS 1

#define ONECOORDATOM 0
#define ONECOORDRES 1
#define ALLMDL 2
#define VECMODEL 3
#define ALLSIMPLEMDL 0

#define NUMARG 6

/* AsnIoOptions for special cases */
#define BIOSTRUC_CHECK_MODEL 1
#define BIOSTRUC_MAX_MODELS 2
