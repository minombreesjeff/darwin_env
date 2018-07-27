/*   cn3dopen.h
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
* File Name:  cn3dopen.h
*
* Author:  Christopher Hogue
*
* Version Creation Date:   1/31/96
*
* $Revision: 6.17 $
*
* File Description: Cn3d file opening routines 
*                   
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* $Log: cn3dopen.h,v $
* Revision 6.17  2000/07/08 20:43:57  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* Revision 6.16  2000/06/16 14:57:03  lewisg
* move entrez calls out of desktop
*
* Revision 6.15  2000/05/15 23:39:34  lewisg
* shred cblast, add menu items for gapped/ungapped, fix pdbheaders
*
* Revision 6.14  2000/04/03 21:05:53  lewisg
* fix launch of cn3d from sequin
*
* Revision 6.13  2000/03/27 22:15:05  lewisg
* add show/hide row dialog
*
* Revision 6.12  2000/03/24 20:34:57  lewisg
* add blast from file, bug fixes, get rid of redundant code, etc.
*
* Revision 6.11  2000/03/20 18:18:33  thiessen
* fixed header problem causing network unavailability
*
* Revision 6.10  2000/03/18 00:06:00  lewisg
* add blast, new help, new menus
*
* Revision 6.9  2000/03/01 22:49:41  lewisg
* import bioseq, neatlyindex, get rid of dead code
*
* Revision 6.8  2000/01/21 15:59:05  lewisg
* add check for binary/ascii files
*
* Revision 6.7  2000/01/04 15:55:51  lewisg
* don't hang on disconnected network and fix memory leak/hang at exit
*
* Revision 6.6  1999/10/29 14:15:30  thiessen
* ran all Cn3D source through GNU Indent to prettify
*
* Revision 6.5  1999/08/04 21:18:01  lewisg
* modularized open operations to allow sequin to launch cn3d
*
* Revision 6.4  1999/01/14 19:07:17  kans
* network availability is configurable
*
* Revision 6.3  1998/06/29 19:28:02  lewisg
* on the fly update of conservation color
*
* Revision 6.2  1998/04/28 19:38:41  lewisg
* codewarrior fixes
*
* Revision 6.1  1998/04/28 15:14:31  lewisg
* moved OpenMimeFileWithDeletion to cn3dopen
*
* Revision 6.0  1997/08/25 18:13:40  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 14:05:44  ostell
* Set to revision 5.0
*
 * Revision 1.1  1996/02/01  18:47:38  kans
 * Initial revision
 *
*
* ==========================================================================
*/

/* cn3dopen.h */

#ifndef _CN3DOPEN2_
#define _CN3DOPEN2_ 1

#include <objmime.h>
#include <algorend.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_MDLNO 1000
#define PRINT_FORM_MIME_NAME "Ncbi-mime-asn1"
#define PRINT_FORM_BIOSTRUC "Biostruc"
extern Boolean OpenMimeFileWithDeletion
    PROTO((CharPtr filename, Boolean removeIt));
extern void LIBCALLBACK fnClearMarkedResidues
    PROTO((PFB pfbThis, Int4 iModel, Int4 iIndex, Pointer ptr));
extern ValNodePtr fnMarkAlignedResidues
    PROTO(
        (PDNMS pdnmsMaster, PDNMS pdnmsSlave,
         BiostrucFeaturePtr pbsfThis));
NLM_EXTERN Boolean MMDB_ReadMime(NcbiMimeAsn1Ptr mime);
NLM_EXTERN void Cn3D_OpenEnd();
NLM_EXTERN void Cn3D_OpenStart();
NLM_EXTERN Boolean Cn3D_StartNet(Boolean UseNetwork);
NLM_EXTERN void MMDB_OpenTraverse(PMSD pmsd);
NLM_EXTERN void Cn3D_SetPars(PARS parsThis, PDNMS pdnms);

extern Boolean Cn3D_UsingEntrez(void);
extern Boolean Cn3D_EntrezOn(void);

/*******************************************************************************

Function : Cn3D_BlastDlg()
  
Purpose : Blast alignment dialog for multiple alignment
  
Parameters : i; menu

*******************************************************************************/
NLM_EXTERN void Cn3D_BlastDlg(IteM i);

/*****************************************************************************

Function: Cn3D_UseNetwork()

Purpose:  Determines if Cn3D should use the network
  
Returns:  TRUE if yes

*****************************************************************************/
NLM_EXTERN Boolean Cn3D_CheckNetworkUse();

/*******************************************************************************

  Function : Cn3D_ImportBioseq()
  
  Purpose : Import a single bioseq via the net

*******************************************************************************/
NLM_EXTERN void Cn3D_ImportBioseq(IteM i);
NLM_EXTERN void Cn3D_ImportBioseqGap(IteM i);
NLM_EXTERN void Cn3D_OpenBiostruc(IteM i);
NLM_EXTERN void Cn3D_NetOpenBiostruc(IteM i);

/*******************************************************************************

  Function : Cn3D_ImportBioseqFile()
  
  Purpose : Import a single bioseq from a fasta file

*******************************************************************************/
NLM_EXTERN void Cn3D_ImportBioseqFile(IteM i);
NLM_EXTERN void Cn3D_ImportBioseqFileGap(IteM i);

NLM_EXTERN Int4 Cn3D_Accession2Gi (CharPtr string, Boolean IsAmino);

#ifdef __cplusplus
}
#endif
#endif
