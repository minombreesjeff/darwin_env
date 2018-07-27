/*   splutil.h
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
* File Name:  splutil.h
*
* Author:  Sarah Wheelan
*
* $Revision: 6.3 $
*
* Contents:  header file for splice utilities program
*
* Modifications:
*
* $Log: splutil.h,v $
* Revision 6.3  2000/04/22 15:56:04  wheelan
* changed is_donor to take a FloatHiPtr instead of returning a FloatHi
*
* Revision 6.2  1998/09/24 12:49:26  wheelan
* added contents, revision and log stuff
*
*
* ==========================================================================
*/

#ifndef _SPLUTIL_
#define _SPLUTIL_

extern void is_donor (CharPtr str, Int4 len, FloatHiPtr score);
extern Int4 getSplicePos (CharPtr str, Int4 overlaplength);
extern FloatHi is_acceptor (CharPtr str, Int4 len);

#endif
  

