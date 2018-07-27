/*   cdrgn.h
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
* File Name:  cdrgn.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   1/22/95
*
* $Revision: 6.3 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: cdrgn.h,v $
* Revision 6.3  2000/07/08 20:44:00  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* ==========================================================================
*/

#ifndef _CDRGN_
#define _CDRGN_

#include <dlogutil.h>

#ifdef __cplusplus
extern "C" {
#endif

#define REGISTER_CDRGN_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit CdRgn","CDS",OBJ_SEQFEAT,FEATDEF_CDS,OBJ_SEQFEAT,FEATDEF_CDS,NULL,CdRgnGenFunc,PROC_PRIORITY_DEFAULT)

extern ForM CreateCdRgnForm (Int2 left, Int2 top, CharPtr title,
                             SeqFeatPtr sfp, SeqEntryPtr sep,
                             FormActnFunc actproc);
extern Int2 LIBCALLBACK CdRgnGenFunc (Pointer data);

extern void CdRgnFeatFormActnProc (ForM f);
extern void CdRgnTranslateWithFrame (ForM f, Uint1 frame);

extern SeqLocPtr PredictCodingRegion (BioseqPtr nuc, BioseqPtr prot, Int2 genCode);

#define REGISTER_GENE_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit Gene","Gene",OBJ_SEQFEAT,FEATDEF_GENE,OBJ_SEQFEAT,FEATDEF_GENE,NULL,GeneGenFunc,PROC_PRIORITY_DEFAULT)

extern ForM CreateGeneForm (Int2 left, Int2 top, CharPtr title,
                            SeqFeatPtr sfp, SeqEntryPtr sep,
                            FormActnFunc actproc);
extern Int2 LIBCALLBACK GeneGenFunc (Pointer data);

#define REGISTER_PROT_EDIT(PROCNAME,PROCLABEL,SUBTYPE) ObjMgrProcLoad(OMPROC_EDIT,PROCNAME,PROCLABEL,OBJ_SEQFEAT,SUBTYPE,OBJ_SEQFEAT,SUBTYPE,NULL,ProtGenFunc,PROC_PRIORITY_DEFAULT)

extern ForM CreateProtForm (Int2 left, Int2 top, CharPtr title,
                            SeqFeatPtr sfp, SeqEntryPtr sep,
                            FormActnFunc actproc);
extern Int2 LIBCALLBACK ProtGenFunc (Pointer data);

#define REGISTER_RNA_EDIT(PROCNAME,PROCLABEL,SUBTYPE) ObjMgrProcLoad(OMPROC_EDIT,PROCNAME,PROCLABEL,OBJ_SEQFEAT,SUBTYPE,OBJ_SEQFEAT,SUBTYPE,NULL,RnaGenFunc,PROC_PRIORITY_DEFAULT)

extern ForM CreateRnaForm (Int2 left, Int2 top, CharPtr title,
                           SeqFeatPtr sfp, SeqEntryPtr sep,
                           Uint2 subtype, FormActnFunc actproc);
extern Int2 LIBCALLBACK RnaGenFunc (Pointer data);


#ifdef __cplusplus
}
#endif

#endif /* ndef _CDRGN_ */

