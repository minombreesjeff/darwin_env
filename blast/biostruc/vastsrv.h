/*
 *===========================================================================
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
 * File Name: vastsrvp.h
 *
 * Author: Tom Madej
 *
 * Version Creation Date: 07/16/97
 *
 * File Description: Header for Vast server operations.
 *
 * Modifications:  
 * --------------------------------------------------------------------------
 * Date     Name        Description of modification
 * -------  ----------  -----------------------------------------------------
 *
 * $Log: vastsrv.h,v $
 * Revision 6.6  1998/11/20 20:03:19  addess
 * related to platform independence of VAST Search
 *
 * Revision 6.5  1998/10/14  17:12:55  addess
 * pagination and aligned chain
 *
 * Revision 6.4  1998/05/19  20:24:06  madej
 * Modify external declarations to pass WWWInfoPtr.
 *
 * Revision 6.3  1998/03/30  19:12:03  madej
 * *** empty log message ***
 *
 * Revision 6.2  1998/03/16  18:00:01  lewisg
 * added temporary hooks to communicate to cn3d
 *
 * Revision 6.1  1998/03/10 16:30:41  madej
 * *** empty log message ***
 *
 *
 * ==========================================================================
 */

#ifndef _VASTSRVP_
#define _VASTSRVP_

#ifdef __cplusplus
extern "C" {
#endif

Boolean LIBCALL VastToMage(WWWInfoPtr www_info);
Boolean LIBCALL VastToCn3D(WWWInfoPtr www_info);
Boolean LIBCALL VastToPDB(WWWInfoPtr www_info);
Int4 LIBCALL GetNumberOfDomains(void);
Int4 LIBCALL GetNumberOfSubsets(void);
Int4 LIBCALL GetSubsetNum(Char *subname);
CharPtr LIBCALL GetSubsetName(Int4 num);
Int4 LIBCALL BelongsToSubset(Char *domid, Int4 sub, Int4 *grpnum, Int4 *grank);
BiostrucAnnotSetPtr LIBCALL LocalGetBiostrucAnnotSet(Int4 mmdbid, CharPtr JobID);
BiostrucAnnotSetPtr LIBCALL LocalGetFeatureSet(Int4 mmdbid, Int4 feature_set_id, CharPtr JobID);
Int2 LIBCALL Check_VastSearch_Password(CharPtr pcPassNew, CharPtr JobID);
BiostrucAnnotSetPtr LIBCALL PruneBiostrucAnnotHits(BiostrucAnnotSetPtr basp, Int4 FSID, ValNodePtr pvnFids);
extern Boolean Chain;
extern Char SlaveChain[2];
extern Char MasterChain[2];
extern Char VSPATH[PATH_MAX];

#ifdef __cplusplus
}
#endif

#endif
