/* $Id: wwwutils.c,v 6.18 2003/05/05 12:17:32 rsmith Exp $
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
* File Name:  $RCSfile: wwwutils.c,v $
*
* Author:  Sergei Shavirin
*
* Version Creation Date: 11/03/1996
*
* $Revision: 6.18 $
*
* File Description:
*   This file contains functions to read and process HTTP 
*   protocols input for WWW CGI programs
*   Currently it works for all ncbi supported platforms.
*   
*---------------------------------------------------------------------------
* $Log: wwwutils.c,v $
* Revision 6.18  2003/05/05 12:17:32  rsmith
* Codewarrior compiling for Win32 does not know about setmode
*
* Revision 6.17  2002/02/07 14:48:22  ivanov
* Added WWWGetEntriesEx(), WWWGetEntriesFormDataEx(), WWWReadFileInMemoryEx(),
* WWWGetValueSizeByIndex() -- support binary files in the multipart form-data.
*
* Revision 6.16  2002/02/01 18:05:42  ivanov
* Little changes in WWWGetArgsInternal()
*
* Revision 6.15  2002/01/29 20:22:24  ivanov
* Added missed realization WWWGetArgsEx()
*
* Revision 6.14  2002/01/29 15:28:48  ivanov
* Typo fixed
*
* Revision 6.13  2002/01/28 21:27:00  ivanov
* Added WWWGetArgsEx() and WWWGetArgsAttr_...() functions.
* Added structure SWWWGetArgsAttr definition.
*
* Revision 6.12  2001/05/10 14:58:34  shavirin
* Fixed typo.
*
* Revision 6.11  2001/05/09 19:25:35  shavirin
* Added function WWWGetProxiedIP() to get 'real' address of the client
* using PROXIED_IP environment set by proxy server.
*
* Revision 6.10  2000/03/16 16:34:47  shavirin
* Removed check for NetscapeOK in case of mutipart/form-data protocol.
*
* Revision 6.9  1999/09/29 19:08:29  shavirin
* Added new functions: WWWGetLastValueByName and WWWFindNameEx
*
* Revision 6.8  1999/01/26 19:43:28  vakatov
* Adopted for 32-bit MS-Windows DLLs
*
* Revision 6.7  1998/06/11 19:00:04  shavirin
* Fixed some compiler warnings.
*
* Revision 6.6  1998/05/21 14:41:46  shavirin
* Made more user- freandly to Microsoft Internet Explorer.
*
* Revision 6.5  1998/05/21 14:21:18  shavirin
* Increased buffer size for boundary string in Form-data parser.
*
* Revision 6.4  1997/11/26 21:26:35  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.3  1997/11/06 23:04:40  vakatov
* WWWInfoFree(), WWWGetValueBy...() -- check for "info->entries" != NULL
* for the special case -- first cmd.-line arg exists AND is ""(empty string)
*
* Revision 6.2  1997/10/29 02:45:53  vakatov
* Type castings to pass through the C++ compiler
*
* Revision 6.1  1997/10/28 23:57:32  vakatov
* WWWInfoFree() -- fixed mem.leak(free num_entries+1 items)
*
* Revision 5.4  1997/06/23 15:10:33  shavirin
* Added new functions WWWSubstututeValue() and WWWSubstituteValueByName()
*
* Revision 5.3  1997/06/10 18:52:35  shavirin
* Added StringSave(info->query) in WWWReadPosting()
*
* Revision 5.2  1997/05/23 15:18:43  shavirin
* Fixed few warnings detected by Windows NT C++ compiler
*
* Revision 5.1  1997/05/09 16:01:27  vakatov
* "ncbicoll.[ch]" is not being used anywhere anymore -- remove it!
* Move "ncbiwww.h" and "wwwutils.c" from /network/www2(ncbiwww2.lib)
* to /corelib(ncbi.lib)
*
* Revision 1.12  1997/04/04  21:28:36  savchuk
* WWWInfoNew() is static now.
* Fixed "Out of bounds write" problem in the function WWWGetEntries()
* 
* Revision 1.11  1997/04/03  17:31:09  shavirin
* Fixed memory leakage in WWWGetEntries()
* 
* Revision 1.10  1997/03/06  19:06:31  shavirin
* Fixed bug with function WWWGetAddress()
* 
* Revision 1.9  1997/02/26  15:20:26  shavirin
* Added function WWWGetDocRoot()
* 
* Revision 1.8  1997/02/21  02:26:38  shavirin
* Added check for NULL buffer in post method
*
* Revision 1.7  1996/12/19  19:37:25  shavirin
* Fixed problem with form-data protocol handling.
*
* Revision 1.6  1996/12/13  22:54:16  shavirin
* Added new functions WWWGetServer(), WWWGetQuery(). removed bug
* in function WWWGetArgs()
*
* Revision 1.5  1996/12/13  16:46:17  shavirin
* WWWGetArgsInternal changed to "static"
*
* Revision 1.4  1996/12/12  19:25:34  shavirin
* Added new functions WWWGetArgs() and static function WWWGetArgsInternal()
*
* Revision 1.3  1996/12/11  18:14:35  shavirin
* Main WWWInfoPtr changed to Void Pointer to hide real structure,
* that called now WWWInfoDataPtr
* Added set of new function to get specific information from
* WWWInfoPtr.
* ==========================================================================
*/

#include <ncbiwww.h>

#ifdef WIN32          /* for setmode() */
#  include <fcntl.h>
#  include <io.h>
#endif


/****************************************************************************/
/* INTERNAL STRUCTURES DEFINITIONS */
/****************************************************************************/

typedef struct SWWWGetArgsAttr_tag
{
    Boolean filter_non_print;       /* filtering non-printable characters */
    Boolean read_argv;              /* read arguments from command line   */
} SWWWGetArgsAttrData;


/****************************************************************************/
/* STATIC FINCTION DEFINITIONS */
/****************************************************************************/

static void WWWGetWord(CharPtr word, CharPtr line, Char stop);
static void PlusToSpace(CharPtr str);
static void WWWUnescapeUrl(CharPtr url);
static Char WWWx2c(CharPtr what);
static Boolean WWWReadEnvironment(WWWInfoDataPtr info);
static WWWErrorCode WWWGetArgsInternal(WWWInfoPtr PNTR info, 
                                       SWWWGetArgsAttr attr);
static WWWInfoPtr WWWInfoNew(void);

/****************************************************************************/
/* EXTERNAL FINCTION */
/****************************************************************************/

NLM_EXTERN void WWWInfoFree(WWWInfoPtr info_in)
{
    WWWInfoDataPtr info = (WWWInfoDataPtr)info_in;

    if( !info )
        return;

    info->server_name = (CharPtr) MemFree(info->server_name);
    info->script_name = (CharPtr) MemFree(info->script_name);
    info->host        = (CharPtr) MemFree(info->host);
    info->address     = (CharPtr) MemFree(info->address);
    info->proxied_ip  = (CharPtr) MemFree(info->proxied_ip);
    info->agent       = (CharPtr) MemFree(info->agent);
    info->doc_root    = (CharPtr) MemFree(info->doc_root);
    info->query       = (CharPtr) MemFree(info->query);
    if ( info->entries ) {
        Int4 i;
        for (i=0;  i <= info->num_entries;  i++) {
            info->entries[i]->name = (CharPtr) MemFree(info->entries[i]->name);
            info->entries[i]->val  = (CharPtr) MemFree(info->entries[i]->val);
            info->entries[i]       = (WWWEntryPtr) MemFree(info->entries[i]);
        }
        info->entries = (WWWEntryPtr PNTR) MemFree(info->entries);
    }
    info = (WWWInfoDataPtr) MemFree(info);
}


/* ----------------------  WWWInfoNew  -------------------------
   Purpose:     Allocates WWWInfo structure
   Parameters:  None
   Returns:     WWWInfo structure
  ------------------------------------------------------------------*/

static WWWInfoPtr WWWInfoNew(void)
{
    return (WWWInfoPtr) MemNew(sizeof(WWWInfoData));
}


NLM_EXTERN WWWEntryPtr PNTR WWWGetWWWEntries(WWWInfoPtr info_in)
{
    WWWInfoDataPtr info;

    if((info = (WWWInfoDataPtr) info_in) == NULL)
        return NULL;

    return(info->entries);
}


NLM_EXTERN Int4 WWWGetMethod(WWWInfoPtr info_in)
{
    WWWInfoDataPtr info;

    if ((info = (WWWInfoDataPtr) info_in) == NULL)
        return -1;

    return info->method;
}


NLM_EXTERN Int4 WWWGetBrowser(WWWInfoPtr info_in)
{
    WWWInfoDataPtr info;

    if ((info = (WWWInfoDataPtr) info_in) == NULL)
        return -1;

    return info->browser;
}


NLM_EXTERN Int4 WWWGetNumEntries(WWWInfoPtr info_in)
{
    WWWInfoDataPtr info;

    if ((info = (WWWInfoDataPtr) info_in) == NULL)
        return -1;

    return info->num_entries;
}


NLM_EXTERN CharPtr WWWGetAgent(WWWInfoPtr info_in)
{
    WWWInfoDataPtr info;

    if ((info = (WWWInfoDataPtr) info_in) == NULL)
        return NULL;

    return info->agent;
}


NLM_EXTERN CharPtr WWWGetAddress(WWWInfoPtr info_in)
{
    WWWInfoDataPtr info;

    if ((info = (WWWInfoDataPtr) info_in) == NULL)
        return NULL;

    return info->address;
}


NLM_EXTERN CharPtr WWWGetServer(WWWInfoPtr info_in)
{
    WWWInfoDataPtr info;

    if ((info = (WWWInfoDataPtr) info_in) == NULL)
        return NULL;

    return info->server_name;
}


NLM_EXTERN CharPtr WWWGetDocRoot(WWWInfoPtr info_in)
{
    WWWInfoDataPtr info;

    if ((info = (WWWInfoDataPtr) info_in) == NULL)
        return NULL;

    return info->doc_root;
}


NLM_EXTERN CharPtr WWWGetHost(WWWInfoPtr info_in)
{
    WWWInfoDataPtr info;

    if ((info = (WWWInfoDataPtr) info_in) == NULL)
        return NULL;

    return info->host;
}


NLM_EXTERN CharPtr WWWGetProxiedIP(WWWInfoPtr info_in)
{
    WWWInfoDataPtr info;

    if ((info = (WWWInfoDataPtr) info_in) == NULL)
        return NULL;

    return info->proxied_ip;
}


NLM_EXTERN CharPtr WWWGetQuery(WWWInfoPtr info_in)
{
    WWWInfoDataPtr info;

    if ((info = (WWWInfoDataPtr) info_in) == NULL)
        return NULL;

    return info->query;
}


NLM_EXTERN Int4 WWWGetPort(WWWInfoPtr info_in)
{
    WWWInfoDataPtr info;

    if ((info = (WWWInfoDataPtr) info_in) == NULL)
        return -1;

    return info->port;
}


NLM_EXTERN WWWErrorCode WWWReadPosting(WWWInfoPtr PNTR info)
{
    WWWErrorCode result;
    SWWWGetArgsAttr attr = WWWGetArgsAttr_Create();
    if (attr == NULL) {
        return WWWErrNoMem;
    }
    WWWGetArgsAttr_SetReadArgv(attr, FALSE);
    result = WWWGetArgsInternal(info, attr);
    WWWGetArgsAttr_Destroy(attr);
    return result;
}


NLM_EXTERN WWWErrorCode WWWGetArgs(WWWInfoPtr PNTR info)
{
    WWWErrorCode result;
    SWWWGetArgsAttr attr = WWWGetArgsAttr_Create();
    if (attr == NULL) {
        return WWWErrNoMem;
    }
    WWWGetArgsAttr_SetReadArgv(attr, TRUE);
    result = WWWGetArgsInternal(info, attr);
    WWWGetArgsAttr_Destroy(attr);
    return result;
}


NLM_EXTERN WWWErrorCode WWWGetArgsEx(WWWInfoPtr PNTR info,
                                     SWWWGetArgsAttr attr)
{
    return WWWGetArgsInternal(info, attr);
}



NLM_EXTERN Boolean WWWSubstituteValue(WWWInfoPtr info_in,
                                      CharPtr old, CharPtr new_value)
{
    Int4 i;
    WWWInfoDataPtr info = (WWWInfoDataPtr) info_in;

    if (info_in == NULL || old == NULL)
        return FALSE;

    for (i = 0; i < info->num_entries; i++) {
        if (!StringICmp(info->entries[i]->val, old)) {
            MemFree(info->entries[i]->val);
            info->entries[i]->val = StringSave(new_value);
            info->entries[i]->size = StringLen(info->entries[i]->val);
            return TRUE;
        }
    }
    return FALSE;
}


NLM_EXTERN Boolean WWWSubstituteValueByName(WWWInfoPtr info_in,
                                            CharPtr new_value, CharPtr name)
{
    Int4 i;
    WWWInfoDataPtr info = (WWWInfoDataPtr) info_in;

    if (info_in == NULL || name == NULL)
        return FALSE;

    for (i = 0; i < info->num_entries; i++) {
        if (!StringICmp(info->entries[i]->name, name)) {
            MemFree(info->entries[i]->val);
            info->entries[i]->val = StringSave(new_value);
            info->entries[i]->size = StringLen(info->entries[i]->val);
            return TRUE;
        }
    }
    return FALSE;
}


static WWWErrorCode WWWGetArgsInternal(WWWInfoPtr PNTR info_out,
                                       SWWWGetArgsAttr attr)
{
    Int4  WWWLen;
    WWWInfoDataPtr info;
    CharPtr PNTR WWWargv;
    Int4  WWWargc;
    Int4  rsize;

#if defined(WIN32)  &&  ! defined(COMP_METRO) 
    setmode(fileno(stdin), _O_BINARY);
#endif

    if ((info = (WWWInfoDataPtr) WWWInfoNew()) == NULL)
        return WWWErrNoMem;

    /* Reading environment from HTTPD */
    if (!WWWReadEnvironment(info)) {
        info->method = COMMAND_LINE;

        if (attr->read_argv == TRUE) {
            WWWargc = GetArgc();
            WWWargv = GetArgv();
            /* Now try to initilalize WWWInfo from STDIN or command line */
            if (WWWargc == 1) { /* reading STDIN */
                if((info->query = WWWReadFileInMemoryEx(stdin, 0,
                                                        attr->filter_non_print,
                                                        &rsize)) != NULL) {
                    info->query_len = rsize;
                    info->entries = WWWGetEntriesEx(&info->num_entries,
                                                    info->query,
                                                    info->query_len);
                }
            } else { /* treat 1st parameter as input buffer */
                if ((info->query = StringSave(WWWargv[1])) != NULL) {
                    info->query_len = StringLen(info->query);
                    info->entries = WWWGetEntriesEx(&info->num_entries,
                                                    info->query,
                                                    info->query_len);
                }
            }
        }
        *info_out = (VoidPtr) info;
        return WWWErrOk;
    } /* COMMAND_LINE */

    if (info->method == WWW_GET) { /* Getting GET data */
        info->query = StringSave(getenv("QUERY_STRING"));
        info->query_len = StringLen(info->query);
    } else if (info->method == WWW_POST) { /* Getting POST data */
        if ((getenv("CONTENT_LENGTH") != NULL) &&
           (WWWLen = atol(getenv("CONTENT_LENGTH"))) > 0) {
            if ((info->query = WWWReadFileInMemoryEx(stdin, WWWLen,
                                                    attr->filter_non_print,
                                                     &rsize)) == NULL) {
                return WWWErrNetwork;
            }
            info->query_len = rsize;
        }
    } /* Method == POST */

    if (info->query != NULL)
        info->entries = WWWGetEntriesEx(&info->num_entries, info->query,
                                        info->query_len);
    *info_out = (VoidPtr) info;
    return WWWErrOk;
}


NLM_EXTERN Int4 WWWFindName(WWWInfoPtr info_in, CharPtr find)
{

    Int4 i;
    WWWInfoDataPtr info = (WWWInfoDataPtr) info_in;

    if (!info || !find)
        return -1;

    for (i = 0; i < info->num_entries; i++) {
        if (!StringICmp(info->entries[i]->name, find)) {
            return i;
        }
    }

    return -1;
}


NLM_EXTERN Int4 WWWFindNameEx(WWWInfoPtr info_in, CharPtr find, Int4 index)
{

    Int4 i;
    WWWInfoDataPtr info = (WWWInfoDataPtr) info_in;

    if (!info || !find)
        return -1;

    if (index >= info->num_entries)
        return -1;

    for (i = index; i < info->num_entries; i++) {
        if (!StringICmp(info->entries[i]->name, find)) {
            return i;
        }
    }

    return -1;
}


NLM_EXTERN CharPtr WWWGetNameByIndex(WWWInfoPtr info_in, Int4 index)
{
    WWWInfoDataPtr info = (WWWInfoDataPtr) info_in;

    if (!info || !info->entries || index < 0 || index > info->num_entries)
        return NULL;

    return info->entries[index]->name;
}


NLM_EXTERN CharPtr WWWGetValueByIndex(WWWInfoPtr info_in, Int4 index)
{
    WWWInfoDataPtr info = (WWWInfoDataPtr) info_in;

    if (!info || !info->entries || index < 0 || index > info->num_entries)
        return NULL;

    return info->entries[index]->val;
}


NLM_EXTERN Int4 WWWGetValueSizeByIndex(WWWInfoPtr info_in, Int4 index)
{
    WWWInfoDataPtr info = (WWWInfoDataPtr) info_in;

    if (!info || !info->entries || index < 0 || index > info->num_entries)
        return -1;

    return info->entries[index]->size;
}


NLM_EXTERN CharPtr WWWGetValueByName(WWWInfoPtr info_in, CharPtr find)
{
    Int4 index;
    WWWInfoDataPtr info = (WWWInfoDataPtr) info_in;

    if (!info || !info->entries || !find)
        return NULL;

    if ((index = WWWFindName(info_in, find)) < 0)
        return NULL;

    return info->entries[index]->val;
}


NLM_EXTERN CharPtr WWWGetLastValueByName(WWWInfoPtr info_in, CharPtr find)
{
    Int4 index;
    WWWInfoDataPtr info = (WWWInfoDataPtr) info_in;
    CharPtr retvalue = NULL;

    if (!info || !info->entries || !find)
        return NULL;

    index = 0;

    while ((index = WWWFindNameEx(info_in, find, index)) >= 0)
        retvalue = info->entries[index]->val;

    return retvalue;
}


NLM_EXTERN WWWEntryPtr PNTR WWWGetEntries(Int4Ptr num_entries,
                                          CharPtr WWWBuffer_in,
                                          Boolean NetscapeOK)
{
    register Int4 i;
    Int4 size;
    CharPtr WWWBuffer;
    WWWEntryPtr PNTR entries;

    if (WWWBuffer_in == NULL || WWWBuffer_in[0] == NULLB)
        return NULL;

    NetscapeOK = TRUE; /* Forget it... all browsers should handle it */

    if ((entries = (WWWEntryPtr*)MemNew(sizeof(WWWEntryPtr) *
                                        MAX_WWW_ENTRIES)) == NULL)
        return NULL;

    if (NetscapeOK && StringStr(WWWBuffer_in,
                               "Content-Disposition: form-data;") != NULL) {
        *num_entries = WWWGetEntriesFormData(entries, WWWBuffer_in);
        return entries;
    }

    WWWBuffer = StringSave(WWWBuffer_in); /* this copy may be used for logs */
    size = StringLen(WWWBuffer) + 1;

    for (i=0; WWWBuffer[0] != NULLB; i++) {
        entries[i] = (WWWEntryPtr) MemNew(sizeof(WWWEntry));
        entries[i]->name = (CharPtr) MemNew(WWW_MAX_NAME_LEN);
        entries[i]->val  = (CharPtr) MemNew(size);

        WWWGetWord(entries[i]->val,WWWBuffer,'&');
        PlusToSpace(entries[i]->val);
        WWWUnescapeUrl(entries[i]->val);
        WWWGetWord(entries[i]->name,entries[i]->val,'=');

        entries[i]->name = (CharPtr) Realloc(entries[i]->name,
                                             StringLen(entries[i]->name)+1);
        entries[i]->val  = (CharPtr) Realloc(entries[i]->val,
                                             StringLen(entries[i]->val)+1);
        entries[i]->size = StringLen(entries[i]->val);
    }

    ASSERT ( i < MAX_WWW_ENTRIES );
    entries[i] = (WWWEntryPtr) MemNew(sizeof(WWWEntry));
    entries[i]->name = NULL;
    entries[i]->val  = NULL;

    MemFree(WWWBuffer);
    *num_entries = i;
    return entries;
}


NLM_EXTERN WWWEntryPtr PNTR WWWGetEntriesEx(Int4Ptr  num_entries,
                                            CharPtr  WWWBuffer_in,
                                            Int4     WWWBuffer_len)
{
    register Int4 i;
    Int4 size;
    CharPtr WWWBuffer;
    WWWEntryPtr PNTR entries;

    if (WWWBuffer_in == NULL || WWWBuffer_in[0] == NULLB)
        return NULL;

    if ((entries = (WWWEntryPtr*)MemNew(sizeof(WWWEntryPtr) *
                                        MAX_WWW_ENTRIES)) == NULL)
        return NULL;

    if (StringStr(WWWBuffer_in, "Content-Disposition: form-data;") != NULL) {
        *num_entries = WWWGetEntriesFormDataEx(entries, WWWBuffer_in,
                                               WWWBuffer_len);
        return entries;
    }

    WWWBuffer = StringSave(WWWBuffer_in); /* this copy may be used for logs */
    size = StringLen(WWWBuffer) + 1;

    for (i=0; WWWBuffer[0] != NULLB; i++) {
        entries[i] = (WWWEntryPtr) MemNew(sizeof(WWWEntry));
        entries[i]->name = (CharPtr) MemNew(WWW_MAX_NAME_LEN);
        entries[i]->val  = (CharPtr) MemNew(size);

        WWWGetWord(entries[i]->val,WWWBuffer,'&');
        PlusToSpace(entries[i]->val);
        WWWUnescapeUrl(entries[i]->val);
        WWWGetWord(entries[i]->name,entries[i]->val,'=');

        entries[i]->name = (CharPtr) Realloc(entries[i]->name,
                                             StringLen(entries[i]->name)+1);
        entries[i]->val  = (CharPtr) Realloc(entries[i]->val,
                                             StringLen(entries[i]->val)+1);
    }

    ASSERT ( i < MAX_WWW_ENTRIES );
    entries[i] = (WWWEntryPtr) MemNew(sizeof(WWWEntry));
    entries[i]->name = NULL;
    entries[i]->val  = NULL;

    MemFree(WWWBuffer);
    *num_entries = i;
    return entries;
}


NLM_EXTERN Int4 WWWGetEntriesFormDataEx(WWWEntryPtr PNTR entries,
                                        CharPtr WWWBuffer_in,
                                        Int4 WWWBuffer_len)
{
    Int4 FieldLen, pos;
    register Int4 i;
    Char BoundaryString[512];
    CharPtr FieldValue, FieldTmp;
    CharPtr WWWBuffer = WWWBuffer_in;

    if (WWWBuffer == NULL || WWWBuffer[0] == NULLB)
        return -1;

    if (WWWBuffer_len < 0)
        WWWBuffer_len = StringLen(WWWBuffer);

    for (i = 0; !isspace(WWWBuffer[i]); i++) {
        BoundaryString[i] = WWWBuffer[i];
    }
    BoundaryString[i] = NULLB;

    for (i=0; WWWBuffer < WWWBuffer_in + WWWBuffer_len; i++) {

        entries[i] = (WWWEntryPtr) MemNew(sizeof(WWWEntry));
        entries[i]->name = (CharPtr) MemNew(128);

        if ((pos = MemSearch(WWWBuffer,
                             WWWBuffer_in + WWWBuffer_len - WWWBuffer,
                             BoundaryString,
                             StringLen(BoundaryString))) < 0)
            break;
        WWWBuffer += pos;
        if (*(WWWBuffer = WWWBuffer + StringLen(BoundaryString) + 1) == '-')
            break;
        if ((WWWBuffer = StringStr(WWWBuffer , "form-data;")) == NULL)
            break;

        WWWBuffer += 11;

        if ((FieldLen = MemSearch(WWWBuffer,
                                 WWWBuffer_in + WWWBuffer_len - WWWBuffer,
                                 BoundaryString,
                                 StringLen(BoundaryString))) < 0)
            break;

        FieldValue = (CharPtr) MemNew(FieldLen+1);
        MemCopy(FieldValue, WWWBuffer, FieldLen);
        FieldValue[FieldLen] = NULLB;

        sscanf(FieldValue, "name=\"%s ", entries[i]->name);

        if (entries[i]->name[StringLen(entries[i]->name)-1] != ';')
            entries[i]->name[StringLen(entries[i]->name)-1] = NULLB;
        else
            entries[i]->name[StringLen(entries[i]->name)-2] = NULLB;

        if ((FieldTmp = StringStr(FieldValue, "\r\n\r\n")) != NULL) {
            FieldTmp = FieldTmp + 4;
            pos = FieldValue + FieldLen - FieldTmp;
            FieldTmp[pos-2] = NULLB;
            MemMove(FieldValue, FieldTmp, pos - 1);
            entries[i]->val = Realloc(FieldValue, pos -1);
            entries[i]->size = pos - 2;
        } else {
            MemFree(FieldValue);
            entries[i]->val = "";
            entries[i]->size = 0;
        }
    }

    entries[i] = (WWWEntryPtr) MemNew(sizeof(WWWEntry));
    entries[i]->name = NULL;
    entries[i]->val  = NULL;

    return i;
}


NLM_EXTERN Int4 WWWGetEntriesFormData(WWWEntryPtr PNTR entries,
                                      CharPtr WWWBuffer)
{
    return WWWGetEntriesFormDataEx(entries, WWWBuffer, -1);
}


NLM_EXTERN CharPtr WWWReadFileInMemory(FILE *fd, Int4 len, Boolean filter)
{
    Int4     bytes = 0;
    CharPtr  in_buff;
    Int4     new_size = INIT_BUFF_SIZE;
    Int4     buff_len = 0;
    register Int4 i;

    if (fd == NULL)
        return NULL;

    if (len == 0) {
        /* initial allocation of memory */

        if ((in_buff = (CharPtr)MemNew(INIT_BUFF_SIZE)) == NULL) {
            ErrLogPrintf("Error in allocating memory\n");
            return NULL;
        }

        while ((bytes = FileRead(in_buff + buff_len, 1,
                                 INIT_BUFF_SIZE, fd)) > 0) {
            new_size += bytes;
            buff_len += bytes;

            if ((in_buff = (CharPtr)Realloc(in_buff, new_size)) == NULL) {
                ErrLogPrintf("Error in reallocating memory\n");
                exit(1);
            }
        }
        in_buff[buff_len] = NULLB;

    } else {

        in_buff = (CharPtr) MemNew(len+1);

        while (buff_len < len) {
            if ((bytes = FileRead(in_buff+buff_len, 1, len, stdin)) == 0
               || bytes == len) {
                buff_len += bytes;
                break; /* EOF or done*/
            } else if (bytes < 0) {
                /* some error setting may be here */
                return NULL;
            }
            buff_len += bytes;
        }
        in_buff[buff_len] = NULLB;
    }

    /* some filtering of non-printing characters */

    if (filter) {
        for (i = 0; i < buff_len; i++) {
            if (!isprint(in_buff[i]) && !isspace(in_buff[i]))
                in_buff[i] = ' ';
        }
    }

    return(in_buff);
}


NLM_EXTERN CharPtr WWWReadFileInMemoryEx(FILE *fd, Int4 len, Boolean filter, Int4Ptr rsize)
{
    Int4     bytes = 0;
    CharPtr  in_buff;
    Int4     new_size = INIT_BUFF_SIZE;
    Int4     buff_len = 0;
    register Int4 i;

    if (rsize)
        *rsize = 0;

    if (fd == NULL)
        return NULL;

    if (len == 0) {
        /* initial allocation of memory */

        if ((in_buff = (CharPtr)MemNew(INIT_BUFF_SIZE)) == NULL) {
            ErrLogPrintf("Error in allocating memory\n");
            return NULL;
        }

        while ((bytes = FileRead(in_buff + buff_len, 1,
                                 INIT_BUFF_SIZE, fd)) > 0) {
            new_size += bytes;
            buff_len += bytes;

            if ((in_buff = (CharPtr)Realloc(in_buff, new_size)) == NULL) {
                ErrLogPrintf("Error in reallocating memory\n");
                exit(1);
            }
        }
        in_buff[buff_len] = NULLB;

    } else {

        in_buff = (CharPtr)MemNew(len+1);

        while (buff_len < len) {
            if ((bytes = FileRead(in_buff+buff_len, 1, len, stdin)) == 0
               || bytes == len) {
                buff_len += bytes;
                break; /* EOF or done*/
            } else if (bytes < 0) {
                /* some error setting may be here */
                if (rsize)
                    *rsize = buff_len;
                return NULL;
            }
            buff_len += bytes;
        }
        in_buff[buff_len] = NULLB;
    }

    /* some filtering of non-printing characters */

    if (filter) {
        for (i = 0; i < buff_len; i++) {
            if (!isprint(in_buff[i]) && !isspace(in_buff[i]))
                in_buff[i] = ' ';
        }
    }
    if (rsize) {
        *rsize = buff_len;
    }
    return(in_buff);
}


NLM_EXTERN SWWWGetArgsAttr WWWGetArgsAttr_Create(void)
{
    SWWWGetArgsAttr attr =(SWWWGetArgsAttr)MemNew(sizeof(SWWWGetArgsAttrData));
    attr->filter_non_print = TRUE;
    attr->read_argv = FALSE;
    return attr;
}


NLM_EXTERN void WWWGetArgsAttr_Destroy(SWWWGetArgsAttr attr)
{
    MemFree(attr);
}


NLM_EXTERN Boolean WWWGetArgsAttr_SetFilter(SWWWGetArgsAttr attr,
                                            Boolean filter_non_print)
{
    Boolean prev_val = attr->filter_non_print;
    attr->filter_non_print = filter_non_print;
    return prev_val;
}


NLM_EXTERN Boolean WWWGetArgsAttr_SetReadArgv(SWWWGetArgsAttr attr,
                                              Boolean read_argv)
{
    Boolean prev_val = attr->read_argv;
    attr->read_argv = read_argv;
    return prev_val;
}


/****************************************************************************/
/* STATIC FINCTIONS */
/****************************************************************************/

static Boolean WWWReadEnvironment(WWWInfoDataPtr info)
{
    CharPtr Method;

    if (!info) 
        return 0;

    info->method = COMMAND_LINE;

    if ((Method = getenv("REQUEST_METHOD")) != NULL) {

        if (!StringICmp(Method, "GET")) {
            info->method = WWW_GET;
        } else if (!StringICmp(Method, "POST")) {
            info->method= WWW_POST;
        }
    }

    if ((info->host = StringSave(getenv("REMOTE_HOST"))) == NULL)
        info->host = StringSave("Host unknown");

    if ((info->address = StringSave(getenv("REMOTE_ADDR"))) == NULL)
        info->address =StringSave("Address unknown");

    if ((info->proxied_ip = StringSave(getenv("PROXIED_IP"))) == NULL)
        info->proxied_ip = StringSave(info->address);

    if ((info->doc_root = StringSave(getenv("DOCUMENT_ROOT"))) == NULL)
        info->doc_root =StringSave("_unknown_");

    if ((info->agent = StringSave(getenv("HTTP_USER_AGENT"))) == NULL)
        info->agent =StringSave("Agent unknown");

    if ((getenv("SERVER_PORT") == NULL) ||
       (info->port = atol(getenv("SERVER_PORT"))) == 0)
        info->port = -1;

    if ((info->server_name = StringSave(getenv("SERVER_NAME"))) == NULL)
        info->server_name = StringSave("Server unknown");

    if ((info->script_name = StringSave(getenv("SCRIPT_NAME"))) == NULL)
        info->script_name = StringSave("Script unknown");

    info->browser = MISC_BROWSER;

    if (StringStr(info->agent, "Mozilla"))
        info->browser = NETSCAPE;

    /*  if(StringStr(info->agent, "MSIE") ||
        StringStr(info->agent, "Microsoft"))
        info->browser = EXPLORER; */

    if (info->method == WWW_POST || info->method == WWW_GET)
        return TRUE;
    else
        return FALSE;
}


static void WWWGetWord(CharPtr word, CharPtr line, Char stop)
{
    register Int4 x = 0,y =0;

    for (x=0; ((line[x]) && (line[x] != stop)); x++)
        word[x] = line[x];

    word[x] = '\0';

    if (line[x]) ++x;

    while ((line[y++] = line[x++]) != NULLB)
        continue;

}


static void PlusToSpace(CharPtr str)
{
    register Int4 x;

    for (x=0; str[x]; x++) if(str[x] == '+')
        str[x] = ' ';
}


static void WWWUnescapeUrl(CharPtr url)
{
    register Int4 x,y;

    for (x=0, y=0; url[y]; ++x, ++y) {
        if ((url[x] = url[y]) == '%') {
            url[x] = WWWx2c(&url[y+1]);
            y+=2;
        }
    }
    url[x] = '\0';
}


static Char WWWx2c(CharPtr what)
{
    register Char digit;

    digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A')+10 : (what[0] - '0'));
    digit *= 16;
    digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A')+10 : (what[1] - '0'));

    return(digit);
}
