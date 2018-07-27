/* asncode.h
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
* File Name: asncode.h
*
* Author:  Karl Sirotkin
*
* Version Creation Date: 12/3/97
*
* $Revision: 6.2 $
*
* File Description:
*   Includes for building asntool and libraries
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: asntool.h,v $
* Revision 6.2  1998/03/25 23:31:32  kans
* params to register new object manager type, give optional non-default label
*
* Revision 6.1  1997/12/16 14:51:52  kans
* header needed for asntool/asncode merge
*
*
* ==========================================================================
*/

#ifndef _ASNCODE_
#define _ASNCODE_

typedef struct struct_AsnCodeInfo {
   Int4            debug_level;     /* > 0, some debugging  if> 3 max */
   FILE           *bug_fp ;   /* file pointer for debugging print */
   Int2     maxDefineLength;        /* maximum token length */

   CharPtr loadname;  /* goes in "#include <%s>\n\n", in generated
                         .h file, with a forced extension of .h */
   CharPtr filename;  /* for generated code, .h and .c appended */
   Boolean do_bit_twiddle; /* add slot in generated code to
                              track whether slots are present,
                              so a zero value can be output */
   CharPtr include_filename; /* similar to loadname, except for .c file */
   CharPtr object_manager_entry; /* to create a registered object manager type */
   CharPtr object_label; /* label (defaults to object_manager_entry) */
   AsnModulePtr amp;         /* standard ncbi usage, all modules */
   AsnModulePtr last_amp;    /* stop generating code when get to this module */
} AsnCodeInfo, PNTR AsnCodeInfoPtr;

void AsnCode    PROTO ((AsnCodeInfoPtr acip));

#endif

