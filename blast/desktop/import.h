/*   import.h
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
* File Name:  import.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   6/18/95
*
* $Revision: 6.6 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#ifndef _IMPORT_
#define _IMPORT_

#include <dlogutil.h>


#ifdef __cplusplus
extern "C" {
#endif

#define REGISTER_IMPORT_EDIT(PROCNAME,PROCLABEL,SUBTYPE) ObjMgrProcLoad(OMPROC_EDIT,PROCNAME,PROCLABEL,OBJ_SEQFEAT,SUBTYPE,OBJ_SEQFEAT,SUBTYPE,NULL,ImportGenFunc,PROC_PRIORITY_DEFAULT)

extern ForM CreateImportForm (Int2 left, Int2 top, CharPtr title,
                              SeqFeatPtr sfp, SeqEntryPtr sep,
                              FormActnFunc actproc);

extern Int2 LIBCALLBACK ImportGenFunc (Pointer data);
extern EnumFieldAssocPtr import_featdef_alist (Boolean notJustImpFeats, Boolean allowPeptideFeats, Boolean allowRnaImpFeats);


#define REGISTER_BOND_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit Bond","Bond",OBJ_SEQFEAT,FEATDEF_BOND,OBJ_SEQFEAT,FEATDEF_BOND,NULL,EnumGenFunc,PROC_PRIORITY_DEFAULT)
#define REGISTER_SITE_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit Site","Site",OBJ_SEQFEAT,FEATDEF_SITE,OBJ_SEQFEAT,FEATDEF_SITE,NULL,EnumGenFunc,PROC_PRIORITY_DEFAULT)
#define REGISTER_PSEC_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit SecStruc","Secondary Structure",OBJ_SEQFEAT,FEATDEF_PSEC_STR,OBJ_SEQFEAT,FEATDEF_PSEC_STR,NULL,EnumGenFunc,PROC_PRIORITY_DEFAULT)

extern ForM CreateEnumForm (Int2 left, Int2 top, CharPtr title,
                            SeqFeatPtr sfp, SeqEntryPtr sep,
                            Uint2 subtype, FormActnFunc actproc);

extern Int2 LIBCALLBACK EnumGenFunc (Pointer data);


#define REGISTER_RGNFEAT_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit RgnFeat","Region",OBJ_SEQFEAT,FEATDEF_REGION,OBJ_SEQFEAT,FEATDEF_REGION,NULL,RegionOrCommentGenFunc,PROC_PRIORITY_DEFAULT)
#define REGISTER_CMNTFEAT_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit CmntFeat","Comment",OBJ_SEQFEAT,FEATDEF_COMMENT,OBJ_SEQFEAT,FEATDEF_COMMENT,NULL,RegionOrCommentGenFunc,PROC_PRIORITY_DEFAULT)

extern ForM CreateRegionOrCommentForm (Int2 left, Int2 top, CharPtr title,
                                       SeqFeatPtr sfp, SeqEntryPtr sep,
                                       Uint2 subtype, FormActnFunc actproc);

extern Int2 LIBCALLBACK RegionOrCommentGenFunc (Pointer data);


#define REGISTER_MOLINFO_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit MolInfo","Molecule Description",OBJ_SEQDESC,Seq_descr_molinfo,OBJ_SEQDESC,Seq_descr_molinfo,NULL,MolInfoGenFunc,PROC_PRIORITY_DEFAULT)

extern DialoG CreateMolInfoDialog (GrouP h, CharPtr title, Uint1 biomol, Uint1 tech,
                                   Boolean showComplete, Boolean nucsOK, Boolean protsOK,
                                   Boolean separateGenomicDNAandRNA);

extern ForM CreateMolInfoForm (Int2 left, Int2 top, Int2 width, Int2 height,
                               CharPtr title, Uint1 biomol, Uint1 tech,
                               Boolean nucsOK, Boolean protsOK, FormActnFunc actproc);

extern Int2 LIBCALLBACK MolInfoGenFunc (Pointer data);


#define REGISTER_GENBANK_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit GenBank","GenBank Block",OBJ_SEQDESC,Seq_descr_genbank,OBJ_SEQDESC,Seq_descr_genbank,NULL,GenBankGenFunc,PROC_PRIORITY_DEFAULT)

extern ForM CreateGenBankForm (Int2 left, Int2 top, CharPtr title,
                               ValNodePtr sdp, SeqEntryPtr sep,
                               FormActnFunc actproc);

extern Int2 LIBCALLBACK GenBankGenFunc (Pointer data);


#define REGISTER_TITLE_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit Title","Title",OBJ_SEQDESC,Seq_descr_title,OBJ_SEQDESC,Seq_descr_title,NULL,VisStrGenFunc,PROC_PRIORITY_DEFAULT)
#define REGISTER_COMMENT_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit Comment","Comment",OBJ_SEQDESC,Seq_descr_comment,OBJ_SEQDESC,Seq_descr_comment,NULL,VisStrGenFunc,PROC_PRIORITY_DEFAULT)
#define REGISTER_REGION_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit Region","Region",OBJ_SEQDESC,Seq_descr_region,OBJ_SEQDESC,Seq_descr_region,NULL,VisStrGenFunc,PROC_PRIORITY_DEFAULT)
#define REGISTER_NAME_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit Name","Name",OBJ_SEQDESC,Seq_descr_name,OBJ_SEQDESC,Seq_descr_name,NULL,VisStrGenFunc,PROC_PRIORITY_DEFAULT)

extern ForM CreateVisStrForm (Int2 left, Int2 top, CharPtr title,
                              Uint2 subtype, FormActnFunc actproc);

extern Int2 LIBCALLBACK VisStrGenFunc (Pointer data);


#define REGISTER_CREATE_DATE_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit Create Date","Create Date",OBJ_SEQDESC,Seq_descr_create_date,OBJ_SEQDESC,Seq_descr_create_date,NULL,DateGenFunc,PROC_PRIORITY_DEFAULT)
#define REGISTER_UPDATE_DATE_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit Update Date","Update Date",OBJ_SEQDESC,Seq_descr_update_date,OBJ_SEQDESC,Seq_descr_update_date,NULL,DateGenFunc,PROC_PRIORITY_DEFAULT)

extern ForM CreateDateForm (Int2 left, Int2 top, CharPtr title,
                            FormActnFunc actproc);

extern Int2 LIBCALLBACK DateGenFunc (Pointer data);


#ifdef __cplusplus
}
#endif

#endif /* ndef _IMPORT_ */

