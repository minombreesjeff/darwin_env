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
 * File Name: vast_to_mage.h
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
 * $Log: vast2cn3d.h,v $
 * Revision 6.1  1998/03/16 18:00:03  lewisg
 * added temporary hooks to communicate to cn3d
 *
 * Revision 6.1  1998/03/12 17:09:27  madej
 * *** empty log message ***
 *
 * Revision 6.1  1998/03/10  16:30:28  madej
 * *** empty log message ***
 *
 *
 * ==========================================================================
 */

#ifdef __cplusplus
extern "C" {
#endif

BiostrucAnnotSetPtr LIBCALL BiostrucAnnotSetGetByFid(BiostrucAnnotSetPtr basp,
	Int4 feature_id, Int4 feature_set_id);

#ifdef __cplusplus
}
#endif

