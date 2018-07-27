/*   accmmdbs.c
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
 * File Name:  accmmdbs.c
 *
 * Author:  Jonathan Kans
 *
 * Version Creation Date:   6/23/95
 *
 * File Description: 
 *       Stub file for MMDB functions (real functions in objmmdb*.c)
 *
 * Modifications:  
 * --------------------------------------------------------------------------
 * Date     Name        Description of modification
 * -------  ----------  -----------------------------------------------------
 *
 * $Log: accmmdbs.c,v $
 * Revision 6.2  2001/06/21 14:21:55  thiessen
 * add BiostrucId... stubs
 *
 * Revision 6.1  1999/03/03 21:27:34  kans
 * added stub for ClearStructures
 *
 * Revision 6.0  1997/08/25 18:12:33  madden
 * Revision changed to 6.0
 *
 * Revision 5.5  1997/06/19 18:40:16  vakatov
 * [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
 *
 * Revision 5.4  1997/03/05 19:56:12  epstein
 * add a few more symbols WritePDBRemarks() and WriteStructSummary() required by new Entrez
 *
 * Revision 5.3  1996/08/14  20:53:46  epstein
 * add missing stubs
 *
 * Revision 5.2  1996/06/20  19:58:03  epstein
 * move cn3d stubs to new acccn3ds.c file
 *
 * Revision 5.1  1996/06/12  18:51:17  epstein
 * integration of CN3d
 *
 * Revision 5.0  1996/05/28  13:55:34  ostell
 * Set to revision 5.0
 *
 * Revision 4.7  1996/05/06  21:38:51  hogue
 * Added stubs for OpenMMDBAPI interface.
 *
 * Revision 4.6  1996/03/08  16:13:34  epstein
 * add new symbols made necessary by NCBI ASN.1 MIMEing
 *
 * Revision 4.5  1996/01/31  21:43:01  hogue
 * Changed prototype to match new Biostruc2Modelstruc calls.
 *
 * Revision 4.4  1995/08/31  13:38:40  kans
 * new prototypes for Biostruc2Modelstruc post-processing
 *
 * Revision 4.3  1995/08/29  17:02:25  kans
 * removed last parameter from Biostruc2Modelstruc stub
 *
 * Revision 4.2  1995/08/28  23:20:47  kans
 * stub for new mmdbapi functions
 *
 * Revision 4.1  1995/08/11  20:26:37  epstein
 * add max-models support for biostrucs
 *
 * Revision 4.0  1995/07/26  13:50:32  ostell
 * force revision to 4.0
 *
 * Revision 1.9  1995/07/25  18:47:57  kans
 * revert to no Biostruc_supported
 *
 * Revision 1.8  1995/07/05  19:15:46  ostell
 * change BiostrucAvail to LIBCALL
 *
 * Revision 1.7  1995/07/03  20:50:31  kans
 * *** empty log message ***
 *
 * Revision 1.6  1995/07/03  20:46:42  kans
 * changed Biostruc2Report to Biostruc2SeqEntryPtr
 *
 * Revision 1.5  1995/07/02  01:49:32  ostell
 * move LIBCALL declaration
 *
 * Revision 1.3  1995/06/29  16:53:48  epstein
 * add stub for BiostrucAsnGet()
 *
 * Revision 1.2  95/06/26  13:08:20  kans
 * #ifdef Biostruc_supported around MMDB header include statements
 * 
 * Revision 1.1  1995/06/23  16:01:01  kans
 * Initial revision
 *
 *
 * ==========================================================================
 */

#include <accentr.h>
#ifdef Biostruc_supported
#include <mmdbapi.h>
#endif

NLM_EXTERN Int2 LIBCALL OpenMMDBAPI(Byte bExtent, CharPtr pcDictName)
{ return 0; }

NLM_EXTERN void LIBCALL CloseMMDBAPI(void)
{ return; }

NLM_EXTERN PRGD LIBCALL GetPRGDDictionary(void)
{ return NULL; }

NLM_EXTERN Boolean LIBCALL BiostrucAvail (void)
{ return FALSE; }

NLM_EXTERN void LIBCALL FreeAModelstruc(PDNMS pdnmsThis)
{ return; }

NLM_EXTERN PDNMS LIBCALL MakeAModelstruc(BiostrucPtr pbsThis)
{ return NULL; }

NLM_EXTERN void LIBCALL ClearStructures(void)
{ }

NLM_EXTERN BiostrucPtr LIBCALL BiostrucAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{ return NULL; }

NLM_EXTERN Boolean LIBCALL BiostrucAsnWrite(BiostrucPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{ return FALSE; }

NLM_EXTERN BiostrucPtr LIBCALL BiostrucFree(BiostrucPtr ptr)
{ return NULL; }

NLM_EXTERN Boolean LIBCALL objmmdb1AsnLoad(void)
{ return TRUE; }

NLM_EXTERN Boolean LIBCALL objmmdb2AsnLoad(void)
{ return TRUE; }

NLM_EXTERN Boolean LIBCALL objmmdb3AsnLoad(void)
{ return TRUE; }

NLM_EXTERN BiostrucResidueGraphSetPtr LIBCALL BiostrucResidueGraphSetFree (BiostrucResidueGraphSetPtr ptr)
{ return NULL; }

NLM_EXTERN void LIBCALL FreeListDNMS(PDNMS pdnmsList)
{ }

NLM_EXTERN PRGD LIBCALL LoadDict(CharPtr pcDictName)
{ return NULL; }

NLM_EXTERN PDNMS LIBCALL Biostruc2Modelstruc(PDNMS PNTR ppdnmsList, BiostrucPtr pbsBS, PRGD prgdDict, Byte  bExtent, Int2 iChoice, CharPtr pcFetch, Int2 iType)
{ return NULL; }

NLM_EXTERN Int2 LIBCALL WritePDBAllModel(PDNMS pdnmsThis,  FILE *pFile)
{ return 0; }

NLM_EXTERN void LIBCALL WritePDBRemarks(PDNMS pdnmsThis,  FILE *pFile)
{ }

NLM_EXTERN Int2 LIBCALL WriteStructSummary(PDNMS pdnmsThis,  FILE *pFile)
{ return 0; }

NLM_EXTERN Int2 LIBCALL WriteKinAllModel(PDNMS pdnmsThis, FILE *pFile, Int2 iColor, Byte bKinRender)
{ return 0; }

NLM_EXTERN Int4 LIBCALL AssignAtomLocId (PDNMS pdnmsThis)
{ return 0L; }

NLM_EXTERN void LIBCALL AssignBackBone (PDNMS pdnmsThis)
{ }

NLM_EXTERN void LIBCALL AssignVirtual (PDNMS pdnmsThis)
{ }

NLM_EXTERN void LIBCALL AssignIons (PDNMS pdnmsThis)
{ }

NLM_EXTERN BiostrucPtr LIBCALL BiostrucAsnGet(AsnIoPtr aip, AsnTypePtr atp, Int4 mdlLvl, Int4 maxModels)
{ return NULL; }

NLM_EXTERN Boolean LIBCALL BiostrucAnnotSetAsnWrite(BiostrucAnnotSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{ return FALSE; }

NLM_EXTERN BiostrucAnnotSetPtr LIBCALL BiostrucAnnotSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{ return NULL; }

NLM_EXTERN BiostrucAnnotSetPtr LIBCALL BiostrucAnnotSetFree(BiostrucAnnotSetPtr ptr)
{ return NULL; }

NLM_EXTERN BiostrucAnnotSetPtr LIBCALL BiostrucAnnotSetNew(void)
{ return NULL; }

NLM_EXTERN BiostrucFeatureSetPtr LIBCALL BiostrucFeatureSetNew(void)
{ return NULL; }

NLM_EXTERN BiostrucFeaturePtr LIBCALL BiostrucFeatureNew(void)
{ return NULL; }

NLM_EXTERN BiostrucIdPtr LIBCALL BiostrucIdFree(BiostrucIdPtr ptr)
{ return NULL; }

NLM_EXTERN BiostrucIdPtr LIBCALL BiostrucIdAsnRead(AsnIoPtr io, AsnTypePtr type)
{ return NULL; }

NLM_EXTERN Boolean LIBCALL BiostrucIdAsnWrite(BiostrucIdPtr id, AsnIoPtr io, AsnTypePtr type)
{ return FALSE; }

