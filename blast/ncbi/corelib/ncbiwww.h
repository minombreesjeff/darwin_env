/* $Id: ncbiwww.h,v 6.7 2002/02/07 14:48:22 ivanov Exp $
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
* File Name:  $RCSfile: ncbiwww.h,v $
*
* Author:  Sergei Shavirin
*
* Version Creation Date: 11/03/1996
*
* $Revision: 6.7 $
*
* File Description:
*   This file contains main definitions to read and process HTTP
*   protocols input for WWW CGI programs
*   Currently it works for all ncbi supported platforms.
*
* $Log: ncbiwww.h,v $
* Revision 6.7  2002/02/07 14:48:22  ivanov
* Added WWWGetEntriesEx(), WWWGetEntriesFormDataEx(), WWWReadFileInMemoryEx(),
* WWWGetValueSizeByIndex() -- support binary files in the multipart form-data.
*
* Revision 6.6  2002/01/28 21:27:00  ivanov
* Added WWWGetArgsEx() and WWWGetArgsAttr_...() functions.
* Added structure SWWWGetArgsAttr definition.
*
* Revision 6.5  2001/05/10 14:58:35  shavirin
* Fixed typo.
*
* Revision 6.4  2001/05/09 19:25:35  shavirin
* Added function WWWGetProxiedIP() to get 'real' address of the client
* using PROXIED_IP environment set by proxy server.
*
* Revision 6.3  1999/09/29 19:08:43  shavirin
* Added new functions: WWWGetLastValueByName and WWWFindNameEx
*
* Revision 6.2  1999/08/18 18:48:40  shavirin
* Increased number of MAX_WWW_ENTRIES to 4096.
*
* Revision 6.1  1999/01/26 19:43:27  vakatov
* Adopted for 32-bit MS-Windows DLLs
*
* Revision 5.1  1997/05/09 16:01:25  vakatov
* "ncbicoll.[ch]" is not being used anywhere anymore -- remove it!
* Move "ncbiwww.h" and "wwwutils.c" from /network/www2(ncbiwww2.lib)
* to /corelib(ncbi.lib)
*
* Revision 1.7  1997/04/04  21:26:32  savchuk
* WWWInfoPtr definition has been changed.
* WWWInfoNew() prototype has been removed
*
* Revision 1.6  1997/02/26  15:20:50  shavirin
* Added definition of function WWWGetDocRoot()
*
* Revision 1.5  1996/12/18  17:44:48  shavirin
* Added support for CC++ compiler usage.
*
* Revision 1.4  1996/12/13  22:53:18  shavirin
* Added definitions to new functions.
* ..
*
* Revision 1.3  1996/12/12  19:24:35  shavirin
* Changed definitions of WWWReadPosting() and entered new function
* WWWGetArgs(). Added WWWErrorCode definitions.
*
* Revision 1.2  1996/12/11  18:13:31  shavirin
* Main WWWInfoPtr changed to Void Pointer to hide real structure,
* that called now WWWInfoDataPtr
*
* Revision 1.1  1996/12/03  22:47:18  shavirin
* Initial revision
*
* ==========================================================================
*/

#ifndef _NCBI_WWW_
#define _NCBI_WWW_ ncbiwww

#ifndef _NCBI_
#include <ncbi.h>
#endif

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


/****************************************************************************/
/* DEFINES */
/****************************************************************************/

#define MAX_WWW_ENTRIES  4096 /* maximum number of html tags in input */
#define WWW_MAX_NAME_LEN 512  /* Limit for Name in HTML tag */

#define MISC_BROWSER     0    /* Any Browser Netscape Ver. 1 included */
#define NETSCAPE         1    /* Netscape Ver. 2 and higher */
#define EXPLORER         2    /* Microsoft Internet Explorer. Any Version */

#define COMMAND_LINE 0        /* program used from command line */
#define WWW_GET      1        /* method with ?name=value&name=value&.. form */
#define WWW_POST     2        /* input through stdin in ?..=..&..=.. form */
#define FORM_DATA    3        /* RFC 1867 multipart/form-data */

#define LIVE_SERVER_PORT 80   /* default HTTPD live port */

#define INIT_BUFF_SIZE 4028   /* temporary buffer to read from file/stdin */

/****************************************************************************/
/* TYPEDEFS */
/****************************************************************************/

typedef struct WWWEntry {
  CharPtr name;           /* HTML tag NAME=..  */
  CharPtr val;            /* HTML tag VALUE=.. */
  Int4    size;           /* Size of data in "val" */
} WWWEntry, PNTR WWWEntryPtr;

/* typedef VoidPtr WWWInfo; */
typedef VoidPtr WWWInfoPtr;

typedef struct WWWInfoData {
  Int4    method;            /* GET, POST or COMMAND_LINE */
  Int4    port;              /* Server port - current server */
  CharPtr server_name;       /* Server name - current server */
  CharPtr doc_root;          /* Document directory of current server */
  CharPtr script_name;       /* Script name - CGI program */
  CharPtr host;              /* remote host of client */
  CharPtr address;           /* remote address of client - may be proxy */
  CharPtr proxied_ip;        /* 'real' remote addr of client as set by proxy */
  CharPtr agent;             /* Label of remote client */
  CharPtr query;             /* Complete input buffer */
  Int4    query_len;         /* Length of data in input buffer */
  Int4    browser;           /* Value derived from Label */
  WWWEntryPtr PNTR entries;  /* Parced input data */
  Int4    num_entries;       /* Number of HTML tags */
} WWWInfoData, PNTR WWWInfoDataPtr;

typedef enum {
  WWWErrOk      = 0,
  WWWErrNoMem   = 1,
  WWWErrNetwork = 2
} WWWErrorCode;


struct SWWWGetArgs_tag;
typedef struct SWWWGetArgsAttr_tag PNTR SWWWGetArgsAttr;

/****************************************************************************/
/* FINCTION DEFINITIONS */
/****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------  WWWGetEntries  -------------------------
   Purpose:     Assuming, that input buffer is in HTTP or RFC 1867
                this function converts input into array of name, value
                pairs in the form of WWWEntry -es.
   Parameters:  num_entries - number of paires returned
                WWWBuffer   - main input HTTP buffer
                NetscapeOK - if TRUE check for RFC 1867 will
                be performed before standard processing (Not used now)
                WWWBuffer_len - length of data in the WWWBuffer

   Returns:     Pointer to array of WWWEntry pairs
   NOTE:        RFC 1867 may be enabled only with Netscape Version 2 and
                higher.
  ------------------------------------------------------------------*/
NLM_EXTERN WWWEntryPtr PNTR WWWGetEntries(Int4Ptr num_entries,
                                          CharPtr WWWBuffer,
                                          Boolean NetscapeOK);

NLM_EXTERN WWWEntryPtr PNTR WWWGetEntriesEx(Int4Ptr num_entries,
                                            CharPtr WWWBuffer,
                                            Int4    WWWBuffer_len);


/* --------------------  WWWGetEntriesFomData  -----------------------
   Purpose:     Assuming, that input buffer is in RFC 1867
                ftp://ds.internic.net/rfc/rfc1867.txt
                (multipart/form-data) encoding this function
                converts input into array of name, value pairs
                in the form of WWWEntry -es.
   Parameters:  WWWBuffer   - main input HTTP buffer
                entries - pointer to array of WWWEntry -es
   Returns:     Number of WWW entries returned
   NOTE:        RFC 1867 may be enabled only with Netscape Version 2 and
                higher.
  ------------------------------------------------------------------*/
NLM_EXTERN Int4 WWWGetEntriesFormData(WWWEntryPtr PNTR entries,
                                      CharPtr WWWBuffer);

NLM_EXTERN Int4 WWWGetEntriesFormDataEx(WWWEntryPtr PNTR entries,
                                        CharPtr WWWBuffer,
                                        Int4 WWWBuffer_len);


/* -----------------------  WWWGetArgs  ---------------------------
   Purpose:     This function read HTML input in POST, GET or
                multipart/form-data encoding - depends upon
                environment. If used from command-line this
                function will return valid WWWInfo structure
                with all field blank exept info->method, that
                will be set to COMMAND_LINE.
                If argc == 1 this function will read WWW buffer
                from STDIN, otherwise it will treat argv[1] as
                WWW buffer.
   Parameters:  None
   Returns:     WWWInfoPtr structure with processed HTTP input and
                environment
   NOTE:        This function will filer input for non-printing
                characters. Transfer of binary files is not supported.

  ------------------------------------------------------------------*/
NLM_EXTERN WWWErrorCode WWWGetArgs(WWWInfoPtr PNTR info);


/* -----------------------  WWWGetArgsEx  ---------------------------
   Purpose:     Identical to previous function, but it have additional
                parameter with working attributes
   Parameters:  attr - function's working attributes
   Returns:     WWWInfoPtr structure with processed HTTP input and
                environment

  ------------------------------------------------------------------*/


NLM_EXTERN WWWErrorCode WWWGetArgsEx(WWWInfoPtr PNTR info,
                                     SWWWGetArgsAttr attr);

/* -----------------------  WWWGetArgsAttr...  -----------------------
   Purpose:     This functions create, destroy and set parameter's
                values to SWWWGetArgsAttr structure.
   NOTE:        Created structure use into WWWGetArgsEx()
  ------------------------------------------------------------------*/

NLM_EXTERN SWWWGetArgsAttr WWWGetArgsAttr_Create(void);

NLM_EXTERN void WWWGetArgsAttr_Destroy(SWWWGetArgsAttr attr);

NLM_EXTERN Boolean WWWGetArgsAttr_SetFilter(SWWWGetArgsAttr attr,
                                            Boolean filter_non_print);
NLM_EXTERN Boolean WWWGetArgsAttr_SetReadArgv(SWWWGetArgsAttr attr,
                                            Boolean read_argv);


/* ----------------------  WWWReadPosting  -------------------------
   Purpose:     This function read HTML input in POST, GET or
                multipart/form-data encoding - depends upon
                environment. If used from command-line this
                function will return valid WWWInfo structure
                with all field blank exept info->method, that
                will be set to COMMAND_LINE
                No more proccesing will be performed.
   Parameters:  None
   Returns:     WWWInfoPtr structure with processed HTTP input and
                environment
   NOTE:        This function will filer input for non-printing
                characters. Transfer of binary files is not supported.

  ------------------------------------------------------------------*/
NLM_EXTERN WWWErrorCode WWWReadPosting(WWWInfoPtr PNTR info);


/* -------------------  WWWReadFileInMemory  -----------------------
   Purpose:     Function reads data from file or stdin into
                string buffer (terminated by NULLB).

   Parameters:  fd - opened file
                len - number of bytes to read. If this value set
                      to 0 file will be read until EOF or closing
                      external connection (for sockets).
                      If len != 0 NOT MORE THAN len bytes will
                      be read from input streem
                filter - if TRUE filtering of non-printed characters
                      will be performed
                rsize - return size of read data
   Returns:     Pointer to allocated buffer.
   NOTE:        Please be carefull with "len": function read input
                absolutely differently if len == 0 or len != 0

  ------------------------------------------------------------------*/
NLM_EXTERN CharPtr WWWReadFileInMemory(FILE *fd, Int4 len, Boolean filter);

NLM_EXTERN CharPtr WWWReadFileInMemoryEx(FILE *fd, Int4 len, Boolean filter,
                                         Int4Ptr rsize);


/* ----------------------  WWWInfoFree  -------------------------
   Purpose:     Free WWWInfo structure
   Parameters:  WWWInfo structure
   Returns:     None
  ------------------------------------------------------------------*/
NLM_EXTERN void WWWInfoFree(WWWInfoPtr info);


/* ----------------------  WWWGetWWWEntry  -------------------------
   Purpose:     Return pointer to array of name=value tags
   Parameters:  WWWInfoPtr
   Returns:     Method used or -1 if error
  ------------------------------------------------------------------*/
NLM_EXTERN WWWEntryPtr PNTR WWWGetWWWEntries(WWWInfoPtr info);


/* ----------------------  WWWGetMethod  -------------------------
   Purpose:     Return method used in WWW Request or COMMAND_LINE
   Parameters:  WWWInfoPtr
   Returns:     Method used or -1 if error
  ------------------------------------------------------------------*/
NLM_EXTERN Int4 WWWGetMethod(WWWInfoPtr info);


/* ----------------------  WWWGetBrowser  -------------------------
   Purpose:     Return browser used in WWW Request
   Parameters:  WWWInfoPtr
   Returns:     Browser used or -1 if error
  ------------------------------------------------------------------*/
NLM_EXTERN Int4 WWWGetBrowser(WWWInfoPtr info);


/* ----------------------  WWWGetNumEntries  -------------------------
   Purpose:     Return number of name=value tags in WWW Request
   Parameters:  WWWInfoPtr
   Returns:     Number of Entries or -1 if error
  ------------------------------------------------------------------*/
NLM_EXTERN Int4 WWWGetNumEntries(WWWInfoPtr info);


/* ----------------------  WWWGetAgent  -------------------------
   Purpose:     Return agent used in WWW Request
   Parameters:  WWWInfoPtr
   Returns:     Agent used or NULL if error
  ------------------------------------------------------------------*/
NLM_EXTERN CharPtr WWWGetAgent(WWWInfoPtr info);


/* ----------------------  WWWGetAddress  -------------------------
   Purpose:     Return address used in WWW Request
   Parameters:  WWWInfoPtr
   Returns:     Address used or NULL if error
   ------------------------------------------------------------------*/
NLM_EXTERN CharPtr WWWGetAddress(WWWInfoPtr info);


/* ----------------------  WWWGetDocRoot  -------------------------
   Purpose:     Return DOCUMENT_ROOT directory of current server
   Parameters:  WWWInfoPtr
   Returns:     Document root directory or NULL if error
  ------------------------------------------------------------------*/
NLM_EXTERN CharPtr WWWGetDocRoot(WWWInfoPtr info_in);

/* ----------------------  WWWGetProxedIP  -------------------------
   Purpose:     Return 'real' client address as set by proxy server
   Parameters:  WWWInfoPtr
   Returns:     Host used or NULL if error
  ------------------------------------------------------------------*/
NLM_EXTERN CharPtr WWWGetProxiedIP(WWWInfoPtr info_in);


/* ----------------------  WWWGetHost  -------------------------
   Purpose:     Return host used in WWW Request
   Parameters:  WWWInfoPtr
   Returns:     Host used or NULL if error
  ------------------------------------------------------------------*/
NLM_EXTERN CharPtr WWWGetHost(WWWInfoPtr info);


/* ----------------------  WWWGetServer  -------------------------
   Purpose:     Return HTTPD server name used in WWW Request
   Parameters:  WWWInfoPtr
   Returns:     Server name used or NULL if error
  ------------------------------------------------------------------*/
NLM_EXTERN CharPtr WWWGetServer(WWWInfoPtr info_in);


/* ----------------------  WWWGetQuery  -------------------------
   Purpose:     Return full query used in WWW Request

   Parameters:  WWWInfoPtr

   Returns:     Query used or NULL if error
  ------------------------------------------------------------------*/
NLM_EXTERN CharPtr WWWGetQuery(WWWInfoPtr info);


/* ----------------------  WWWGetPort  -------------------------
   Purpose:     Return port used in WWW Request
   Parameters:  WWWInfoPtr
   Returns:     Port used or -1 if error
  ------------------------------------------------------------------*/
NLM_EXTERN Int4 WWWGetPort(WWWInfoPtr info);


/* ----------------------  WWWInfoNew  -------------------------
   Purpose:     Allocates WWWInfo structure
   Parameters:  None
   Returns:     WWWInfo structure
  ------------------------------------------------------------------*/
/* NLM_EXTERN WWWInfoPtr WWWInfoNew(void); */


/* ----------------------  WWWFindName  -------------------------
   Purpose:     This function look for Name in WWW Entries structure
   Parameters:  info - WWWInfo structure
                find - Name to find
   Returns:     index in WWWEntry structue if "find" found and -1 if not
  ------------------------------------------------------------------*/
NLM_EXTERN Int4 WWWFindName(WWWInfoPtr info, CharPtr find);


/* ----------------------  WWWFindName  -------------------------
   Purpose:     This function look for Name in WWW Entries structure
                starting from specifix index value
   Parameters:  info - WWWInfo structure
                find - Name to find
                index - index value to start with
   Returns:     index in WWWEntry structue if "find" found and -1 if not
  ------------------------------------------------------------------*/
NLM_EXTERN Int4 WWWFindNameEx(WWWInfoPtr info_in, CharPtr find, Int4 index);


/* ----------------------  WWWGetNameByIndex  ----------------------
   Purpose:     This function get Name correspondig to specific
                index.
   Parameters:  info - WWWInfo structure
                index - Index in WWW Entries structure
   Returns:     Pointer to Name or NULL if index invalid
  ------------------------------------------------------------------*/
NLM_EXTERN CharPtr WWWGetNameByIndex(WWWInfoPtr info, Int4 index);


/* -------------------  WWWGetValueByIndex  ---------------------
   Purpose:     This function get Value correspondig to specific
                index.
   Parameters:  info - WWWInfo structure
                index - Index in WWW Entries structure
   Returns:     Pointer to Value or NULL if index invalid
  ------------------------------------------------------------------*/
NLM_EXTERN CharPtr WWWGetValueByIndex(WWWInfoPtr info, Int4 index);


/* -------------------  WWWGetValueSizeByIndex  ---------------------
   Purpose:     This function get size of Value correspondig to
                specific index.
   Parameters:  info - WWWInfo structure
                index - Index in WWW Entries structure
   Returns:     Number of bytes stored into the Value
  ------------------------------------------------------------------*/
NLM_EXTERN Int4 WWWGetValueSizeByIndex(WWWInfoPtr info, Int4 index);


/* -------------------  WWWGetValueByName  ---------------------
   Purpose:     This function get Value correspondig to specific
                Name.
   Parameters:  info - WWWInfo structure
                name - name to look for
                start - Index in WWW Entries structure to start from
   Returns:     Pointer to Value or NULL if Name was not found
  ------------------------------------------------------------------*/
NLM_EXTERN CharPtr WWWGetValueByName(WWWInfoPtr info, CharPtr name);


/* -------------------  WWWGetValueByName  ---------------------
   Purpose:     This function get LAST Value correspondig to specific
                Name if there are more then one.
   Parameters:  info - WWWInfo structure
                name - name to look for
                start - Index in WWW Entries structure to start from
   Returns:     Pointer to Value or NULL if Name was not found
  ------------------------------------------------------------------*/
NLM_EXTERN CharPtr WWWGetLastValueByName(WWWInfoPtr info_in, CharPtr find);


/* -------------------  WWWSubstituteValue  ---------------------
   Purpose:     This function substitute "old" value by "new"
                value in WWWInfo structure.
   Parameters:  info_in - WWWInfo structure
                old - value to change
                new - new value to assign
   Returns:     FALSE if "old" value not found, otherwise TRUE
  ------------------------------------------------------------------*/
NLM_EXTERN Boolean WWWSubstituteValue(WWWInfoPtr info_in,
                                      CharPtr old, CharPtr new_value);


/* -------------------  WWWSubstituteValueByName  -------------------
   Purpose:     This function substitute  value corresponding to "name"
                by "new" value in WWWInfo structure.
   Parameters:  info_in - WWWInfo structure
                name - corresponding name
                new - new value to assign
   Returns:     FALSE if "name" was not found, otherwise TRUE
  ------------------------------------------------------------------*/
NLM_EXTERN Boolean WWWSubstituteValueByName(WWWInfoPtr info_in,
                                            CharPtr new_value, CharPtr name);

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
