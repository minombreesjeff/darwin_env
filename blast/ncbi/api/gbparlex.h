/* gbparlex.h
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
* File Name:  objseq.h
*
* Author:  Karl Sirotkin
*
* $Log: gbparlex.h,v $
* Revision 6.2  2004/07/22 15:26:24  bazhin
* Added "#define GBPARSE_INT_UNK_GAP 20" to handle gaps of unknown
* lengths within location strings.
*
* Revision 6.1  1997/10/24 18:44:42  bazhin
* Added "#define GBPARSE_INT_GAP 20".
*
* Revision 6.0  1997/08/25 18:06:10  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 13:23:23  ostell
* Set to revision 5.0
*
 * Revision 4.1  1995/11/17  21:28:35  kans
 * asn2ff now uses gather (Tatiana)
 *
 * Revision 1.2  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*/


#ifndef _NCBI_GBParseLex_
#define _NCBI_GBParseLex_

#define GBPARSE_INT_UNKNOWN 0
#define GBPARSE_INT_JOIN 1
#define GBPARSE_INT_COMPL 2
#define GBPARSE_INT_LEFT 3
#define GBPARSE_INT_RIGHT 4
#define GBPARSE_INT_CARET 5
#define GBPARSE_INT_DOT_DOT 6
#define GBPARSE_INT_ACCESION 7
#define GBPARSE_INT_GT 8
#define GBPARSE_INT_LT 9
#define GBPARSE_INT_COMMA 10
#define GBPARSE_INT_NUMBER 11
#define GBPARSE_INT_ORDER 12
#define GBPARSE_INT_SINGLE_DOT 13
#define GBPARSE_INT_GROUP 14
#define GBPARSE_INT_ONE_OF 15
#define GBPARSE_INT_REPLACE 16
#define GBPARSE_INT_SITES 17
#define GBPARSE_INT_STRING 18
#define GBPARSE_INT_ONE_OF_NUM 19
#define GBPARSE_INT_GAP 20
#define GBPARSE_INT_UNK_GAP 21

#define ERR_NCBIGBPARSE_LEX 1
#define ERR_NCBIGBPARSE_INT 2

#endif
