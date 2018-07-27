/*
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
* File Name: findrepl.c
*
* Author:  Jonathan Kans, Tim Ford
*
* Version Creation Date:   10/17/00
*
* File Description:
*   Complete redesign of find/replace from original of Yuri Sadykov
*
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* ==========================================================================
*
*
* RCS Modification History:
* -------------------------
* $Log: findrepl.c,v $
* Revision 6.5  2001/12/12 17:38:38  kans
* added new subsource qualifiers, four now have empty name
*
* Revision 6.4  2001/12/07 13:49:34  kans
* workingBuffer needs to be large enough for terminal null byte
*
* Revision 6.3  2001/08/06 22:13:12  kans
* using NUM_SEQID, added TPA ids to arrays
*
* Revision 6.2  2000/11/03 20:36:00  kans
* FindReplaceInEntity replaces FindInEntity and FindInEntityX - complete redesign, no longer using AsnExpOptExplore because of the difficulty of replacing with a larger string (TF + JK)
*
* Revision 6.1  1999/03/05 23:31:07  kans
* FindInEntityX was not initializing flen, replen
*
* Revision 6.0  1997/08/25 18:05:38  madden
* Revision changed to 6.0
*
* Revision 5.3  1997/06/19 18:37:41  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.2  1997/03/17 23:44:39  kans
* added whole_word parameter to FindInEntity and FindInEntityX, and protected
* against multiple ObjMgrAlsoSelects on a single itemID
*
 * Revision 5.1  1996/09/06  20:20:41  kans
 * keeps going even if ObjMgrTypeFind returns NULL (e.g., on OBJ_BIOSEQ_SEG),
 * and adds a case_counts parameter for case sensitive/insensitive searches.
 *
 * Revision 5.0  1996/05/28  13:23:23  ostell
 * Set to revision 5.0
 *
 * Revision 1.7  1996/02/28  04:53:06  ostell
 * fix to prevernt recursion on substring replaces
 *
 * Revision 1.6  1996/02/26  20:24:05  kans
 * replace needs MemCopy instead of StringMove (JO), and set dirty flag
 *
 * Revision 1.5  1996/01/03  23:06:32  ostell
 * support for longer replaces, controlled updating
 *
 * Revision 1.3  1996/01/02  18:40:07  ostell
 * simplified code.
 *
 * Revision 1.2  1996/01/01  00:05:14  kans
 * replaced StringStr with StringISearch to ignore case
 *
 * Revision 1.1  1995/12/31  18:13:14  kans
 * Initial revision
 *
* Revision 1.1.1.1  1995/10/19 18:42:10  sad
* Initial version
*
*/

#include <ncbi.h>
#include <objall.h>
#include <objfdef.h>
#include <objsub.h>
#include <gather.h>
#include <sqnutils.h>
#include <subutil.h>
#include <findrepl.h>

/* internal structure passed to callbacks */

typedef struct findstruct {
  Uint2    entityID;
  CharPtr  find_string;
  CharPtr  replace_string;
  Boolean  case_counts;
  Boolean  whole_word;
  Boolean  do_replace;
  Boolean  select_item;
  Int2     send_update;
  Boolean  did_find;
  Boolean  did_replace;
  Boolean  dirty;
  Boolean  descFilter [SEQDESCR_MAX];
  Boolean  featFilter [FEATDEF_MAX];
  Boolean  seqidFilter [NUM_SEQID];
  int      d [256];
  size_t   subLen;
} FindStruct, PNTR FindStructPtr;

#define PID_NOTSET 0
#define PID_DBTAG  1
#define PID_NAME   2
#define PID_ML     3
#define PID_STR    4

#define NAMESTD_LAST     0
#define NAMESTD_FIRST    1
#define NAMESTD_MIDDLE   2
#define NAMESTD_FULL     3
#define NAMESTD_INITIALS 4
#define NAMESTD_SUFFIX   5
#define NAMESTD_TITLE    6

#define AUTHLIST_STRUCTURED 1
#define AUTHLIST_ML         2
#define AUTHLIST_STRING     3
  
#define FINDREPL_BUFFER_MAX  1000000

/* StringSearch and StringISearch use the Boyer-Moore algorithm, as described
   in Niklaus Wirth, Algorithms and Data Structures, Prentice- Hall, Inc.,
   Englewood Cliffs, NJ., 1986, p. 69.  The original had an error, where
   UNTIL (j < 0) OR (p[j] # s[i]) should be UNTIL (j < 0) OR (p[j] # s[k]). */

static CharPtr FindSubString (
  CharPtr str,
  CharPtr sub,
  Boolean case_counts,
  size_t strLen,
  size_t subLen,
  int *d
)

{
  int  ch;
  int  i;
  int  j;
  int  k;

  i = subLen;
  do {
    j = subLen;
    k = i;
    do {
      k--;
      j--;
    } while (j >= 0 &&
             (case_counts ? sub [j] : TO_UPPER (sub [j])) ==
             (case_counts ? str [k] : TO_UPPER (str [k])));
    if (j >= 0) {
      ch = (int) (case_counts ? str [i - 1] : TO_UPPER (str [i - 1]));
      if (ch >= 0 && ch <= 255) {
        i += d [ch];
      } else {
        i++;
      }
    }
  } while (j >= 0 && i <= (int) strLen);

  if (j < 0) {
    i -= subLen;
    return (CharPtr) (str + i);
  }

  return NULL;
}

/* passed subLen and d array to avoid repeated initialization of the Boyer-Moore
   displacement table */

static CharPtr SearchForString (
  CharPtr str,
  CharPtr sub,
  Boolean case_counts,
  Boolean whole_word,
  size_t subLen,
  int * d
)

{
  CharPtr  ptr = NULL;
  size_t   strLen;
  CharPtr  tmp;

  if (str == NULL || *str == '\0') return NULL;
  strLen = StringLen (str);
  if (subLen > strLen) return NULL;

  ptr = FindSubString (str, sub, case_counts, strLen, subLen, d);
  if (ptr == NULL) return NULL;

  if (whole_word) {
    if (ptr > str) {
      tmp = ptr - 1;
      if (! IS_WHITESP (*tmp)) return NULL;
    }
    tmp = ptr + StringLen (sub);
    if (*tmp != '\0' && (! IS_WHITESP (*tmp))) return NULL;
  }

  return ptr;
}

/*=======================================================================*/
/*                                                                       */
/* FindReplString () - Does a search and replace in a given string.      */
/*                                                                       */
/*    Main Parameters:                                                   */
/*                                                                       */
/*         strp : The string to operate on. Passed as a pointer to       */
/*                a string so that it can be replaced by the             */
/*                resulting string.                                      */
/*                                                                       */
/*         fsp->find_string : The substring that is being replaced       */
/*                            in strp.                                   */
/*                                                                       */
/*         fsp->replace_string : The substring that is replacing         */
/*                               find_string in strp.                    */
/*                                                                       */
/*=======================================================================*/

static Boolean FindReplString (
  CharPtr PNTR strp,
  FindStructPtr fsp
)

{
  Boolean wasChanged;
  Int4    replaceLen;
  Int4    findLen;
  Int4    searchLen;
  Int4    buffSize;
  CharPtr workingBuffer;
  CharPtr searchString;
  CharPtr substringPtr;

  if (strp == NULL || fsp == NULL) return FALSE;

  replaceLen = StringLen (fsp->replace_string);
  findLen    = StringLen (fsp->find_string);
  searchLen  = StringLen (*strp);

  searchString = *strp;
  wasChanged = FALSE;

  if (! fsp->do_replace) {
    if (SearchForString (searchString, fsp->find_string,
          fsp->case_counts, fsp->whole_word,
          findLen, fsp->d) != NULL) {

      fsp->did_find = TRUE;
    }
    return TRUE;
  }

  /*------------------------------------------------*/
  /* Make a guess of how big a working buffer we'll */
  /* need based on a worst case scenario.           */
  /*                                                */
  /*   A = Max occurances of find string =          */
  /*               searchLen / findLen              */
  /*                                                */
  /*   B = Size increase for each replacement =     */
  /*               replaceLen - findLen             */
  /*                                                */
  /*   Maximum resultant string size =              */
  /*               searchLen + (A * B)              */
  /*                                                */
  /*------------------------------------------------*/

  if (replaceLen > findLen)
    {
      buffSize = searchLen + ((searchLen/findLen) * (replaceLen - findLen));
      if (buffSize > FINDREPL_BUFFER_MAX)
        buffSize = FINDREPL_BUFFER_MAX;
    }
  else
    buffSize = searchLen;

  workingBuffer = (CharPtr) MemNew (buffSize + 2);
  if (workingBuffer == NULL)
    return FALSE;

  workingBuffer[0] = '\0';

  /*----------------------------------------*/
  /* Create a new string with all instances */
  /* of the find string replaced by the     */
  /* replace string.                        */
  /*----------------------------------------*/

  while ((substringPtr = SearchForString (searchString, fsp->find_string,
          fsp->case_counts, fsp->whole_word,
          findLen, fsp->d)) != NULL)
    {
      wasChanged = TRUE;
      substringPtr[0] = '\0';

      if (StringLen (workingBuffer) + StringLen (searchString) > buffSize)
        return FALSE;

      StringCat (workingBuffer, searchString);
      StringCat (workingBuffer, fsp->replace_string);
      substringPtr[0] = 'x';
      searchString = substringPtr + findLen;
    }

  if (searchString != NULL)
    StringCat (workingBuffer, searchString);

  /*-------------------------------------*/
  /* If any replacements were made, then */
  /* swap in the new string for the old. */
  /*-------------------------------------*/

  if (wasChanged)
    {
      MemFree (*strp);
      (*strp) = workingBuffer;

      fsp->did_replace = TRUE;
      fsp->dirty = TRUE;
    }
  else
    MemFree (workingBuffer);

  /*---------------------*/
  /* Return successfully */
  /*---------------------*/

  return TRUE;
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplStringList()                                                 */
/*                                                                       */
/*=======================================================================*/

static void FindReplStringList (
  ValNodePtr vnp,
  FindStructPtr fsp
)

{
  while (vnp != NULL) {
    FindReplString ((CharPtr PNTR) &(vnp->data.ptrvalue), fsp);
    vnp = vnp->next;
  }
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplDbxrefs() -                                                  */
/*                                                                       */
/*=======================================================================*/

static void FindReplDbxrefs (
  ValNodePtr vnp,
  FindStructPtr fsp
)

{
  DbtagPtr     dbt;
  ObjectIdPtr  oip;

  while (vnp != NULL) {
    dbt = (DbtagPtr) vnp->data.ptrvalue;
    if (dbt != NULL) {
      FindReplString (&(dbt->db), fsp);
      oip = dbt->tag;
      if (oip != NULL && oip->str != NULL) {
        FindReplString (&(oip->str), fsp);
      }
    }
    vnp = vnp->next;
  }
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplAffil() -                                                    */
/*                                                                       */
/*=======================================================================*/

static void FindReplAffil (
  AffilPtr pAffil,
  FindStructPtr pFindStruct
)

{
  if (pAffil == NULL)
    return;

  if (pAffil->choice == 1) {
    FindReplString (&(pAffil->affil)      , pFindStruct);
  } else {
    FindReplString (&(pAffil->affil)      , pFindStruct);
    FindReplString (&(pAffil->div)        , pFindStruct);
    FindReplString (&(pAffil->city)       , pFindStruct);
    FindReplString (&(pAffil->sub)        , pFindStruct);
    FindReplString (&(pAffil->country)    , pFindStruct);
    FindReplString (&(pAffil->street)     , pFindStruct);
    FindReplString (&(pAffil->email)      , pFindStruct);
    FindReplString (&(pAffil->fax)        , pFindStruct);
    FindReplString (&(pAffil->phone)      , pFindStruct);
    FindReplString (&(pAffil->postal_code), pFindStruct);
  }
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplAuthor() -                                                   */
/*                                                                       */
/*=======================================================================*/

static void FindReplAuthor (
  AuthorPtr pAuthor,
  FindStructPtr pFindStruct
)

{
  NameStdPtr pNameStandard;
  CharPtr    pNameStr;
  ValNodePtr pDbxref;

  if (pAuthor == NULL)
    return;

  FindReplAffil (pAuthor->affil, pFindStruct);
  
  switch (pAuthor->name->choice)
    {
    case PID_NOTSET :
      break;
    case PID_DBTAG :
      pDbxref = pAuthor->name->data;
      FindReplDbxrefs (pDbxref, pFindStruct);
      break;
    case PID_NAME :
      pNameStandard = pAuthor->name->data;
      if (pNameStandard != NULL)
      {
        FindReplString (&(pNameStandard->names [NAMESTD_LAST])    , pFindStruct);
        FindReplString (&(pNameStandard->names [NAMESTD_FIRST])   , pFindStruct);
        FindReplString (&(pNameStandard->names [NAMESTD_MIDDLE])  , pFindStruct);
        FindReplString (&(pNameStandard->names [NAMESTD_FULL])    , pFindStruct);
        FindReplString (&(pNameStandard->names [NAMESTD_INITIALS]), pFindStruct);
        FindReplString (&(pNameStandard->names [NAMESTD_SUFFIX])  , pFindStruct);
        FindReplString (&(pNameStandard->names [NAMESTD_TITLE])   , pFindStruct);
      }
      break;
    case PID_ML :
    case PID_STR :
      pNameStr = pAuthor->name->data;
      FindReplString (&pNameStr, pFindStruct);
      break;
    default:
      break;
    }
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplAuthList() -                                                 */
/*                                                                       */
/*=======================================================================*/

static void FindReplAuthlist (
  AuthListPtr alp,
  FindStructPtr fsp
)

{
  ValNodePtr vnpNames;
  CharPtr    szAuthor;
  AuthorPtr  pAuthor;

  if (alp == NULL)
    return;

  FindReplAffil (alp->affil, fsp);
  vnpNames = alp->names;
  while (vnpNames != NULL)
    {
      if (alp->choice == AUTHLIST_STRUCTURED)
      {
        pAuthor = (AuthorPtr) vnpNames->data.ptrvalue;
        if (pAuthor != NULL)
          FindReplAuthor (pAuthor, fsp);
      }
      else
      {
        szAuthor = (CharPtr) vnpNames->data.ptrvalue;
        if (szAuthor != NULL)
          {
            FindReplString (&szAuthor, fsp);
            vnpNames->data.ptrvalue = szAuthor;
          }
      }
      vnpNames = vnpNames->next;
    }
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplCitRetract() -                                               */
/*                                                                       */
/*=======================================================================*/

static void FindReplCitRetract (
  CitRetractPtr pCitRetract,
  FindStructPtr pFindStruct
)

{
  if (pCitRetract == NULL)
    return;

  FindReplString (&(pCitRetract->exp), pFindStruct);
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplImprint() -                                                  */
/*                                                                       */
/*=======================================================================*/

static void FindReplImprint (
  ImprintPtr pImprint,
  FindStructPtr pFindStruct
)

{

  /*------------------*/
  /* Check parameters */
  /*------------------*/

  if (pImprint == NULL)
    return;

  /*-------------------------*/
  /* Do the find and replace */
  /*-------------------------*/

  FindReplString (&(pImprint->volume)   , pFindStruct);
  FindReplString (&(pImprint->issue)    , pFindStruct);
  FindReplString (&(pImprint->pages)    , pFindStruct);
  FindReplString (&(pImprint->section)  , pFindStruct);
  FindReplString (&(pImprint->part_sup) , pFindStruct);
  FindReplString (&(pImprint->language) , pFindStruct);
  FindReplString (&(pImprint->part_supi), pFindStruct);

  FindReplAffil (pImprint->pub, pFindStruct);
  FindReplCitRetract (pImprint->retract, pFindStruct);
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplCitBook() -                                                  */
/*                                                                       */
/*=======================================================================*/

static void FindReplCitBook (
  CitBookPtr pCitBook,
  FindStructPtr pFindStruct
)

{
  AffilPtr    afp;
  ValNodePtr  vnp;
  CharPtr     tmpStr;

  if (pCitBook == NULL) return;

  FindReplStringList (pCitBook->title, pFindStruct);
  FindReplImprint (pCitBook->imp, pFindStruct);
  FindReplAuthlist (pCitBook->authors, pFindStruct);
  FindReplStringList (pCitBook->title, pFindStruct);
  FindReplStringList (pCitBook->coll, pFindStruct);

  if (pCitBook->othertype == 1) {
    for (vnp = (ValNodePtr) pCitBook->otherdata; vnp != NULL; vnp = vnp->next) {
      switch (vnp->choice) {
        case 1 :
          FindReplString ((CharPtr PNTR) &(vnp->data.ptrvalue), pFindStruct);
          break;
        case 3 :
          afp = (AffilPtr) vnp->data.ptrvalue;
          FindReplAffil (afp, pFindStruct);
          break;
        default :
          break;
      }
    }
  } else if (pCitBook->othertype == 2) {
    tmpStr = (CharPtr) pCitBook->otherdata;
    FindReplString (&tmpStr, pFindStruct);
    pCitBook->otherdata = tmpStr;
  }
}

static void FindReplCitArt (
  CitArtPtr pCitArt,
  FindStructPtr pFindStruct
)

{
  CitJourPtr pCitJournal;
  CitBookPtr pCitBook;

  if (pCitArt == NULL)
    return;

  FindReplAuthlist (pCitArt->authors, pFindStruct);
  if (pCitArt->fromptr != NULL) {
    switch (pCitArt->from) {
    case 1 :
      pCitJournal = (CitJourPtr) pCitArt->fromptr;
      FindReplStringList (pCitArt->title, pFindStruct);
      FindReplImprint (pCitJournal->imp, pFindStruct);
      break;
    case 2 :
    case 3 :
      pCitBook = (CitBookPtr) pCitArt->fromptr;
      FindReplCitBook (pCitBook, pFindStruct);
      break;
    default :
      break;
    }
  }
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplMedlineEntry() -                                             */
/*                                                                       */
/*=======================================================================*/

static void FindReplMedlineEntry (
  MedlineEntryPtr pMedlineEntry,
  FindStructPtr pFindStruct
)

{
  MedlineFieldPtr pField;
  MedlineMeshPtr  pMesh;
  MedlineRnPtr    pRn;
  CharPtr         tmpStr;

  if (pMedlineEntry == NULL)
    return;

  FindReplCitArt(pMedlineEntry->cit, pFindStruct);
  FindReplString (&(pMedlineEntry->abstract), pFindStruct);

  pRn = pMedlineEntry->substance;
  while (pRn != NULL)
    {
      FindReplString (&(pRn->cit), pFindStruct);
      FindReplString (&(pRn->name), pFindStruct);
      pRn = pRn->next;
    }

  pMesh = pMedlineEntry->mesh;
  while (pMesh != NULL)
    {
      FindReplString (&(pMesh->term), pFindStruct);
      pMesh = pMesh->next;
    }

  if (pMedlineEntry->xref != NULL)
    {
      tmpStr = (CharPtr) pMedlineEntry->xref->data.ptrvalue;
      FindReplString (&tmpStr, pFindStruct);
      pMedlineEntry->xref->data.ptrvalue = tmpStr;
    }

  if (pMedlineEntry->idnum != NULL)
    {
      tmpStr = (CharPtr) pMedlineEntry->idnum->data.ptrvalue;
      FindReplString (&tmpStr, pFindStruct);
      pMedlineEntry->idnum->data.ptrvalue = tmpStr;
    }

  if (pMedlineEntry->pub_type != NULL)
    {
      tmpStr = (CharPtr) pMedlineEntry->pub_type->data.ptrvalue;
      FindReplString (&tmpStr, pFindStruct);
      pMedlineEntry->pub_type->data.ptrvalue = tmpStr;
    }

  if (pMedlineEntry->gene != NULL)
    {
      tmpStr = (CharPtr) pMedlineEntry->gene->data.ptrvalue;
      FindReplString (&tmpStr, pFindStruct);
      pMedlineEntry->gene->data.ptrvalue = tmpStr;
    }

  pField = pMedlineEntry->mlfield;
  while (pField != NULL)
    {
      FindReplString (&(pField->str), pFindStruct);
      pField = pField->next;
    }
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplPub() -                                                      */
/*                                                                       */
/*=======================================================================*/

static void FindReplPub (
  ValNodePtr vnp,
  FindStructPtr fsp
)

{
  CitArtPtr       cap;
  CitBookPtr      cbp;
  CitGenPtr       cgp;
  CitJourPtr      cjp;
  CitPatPtr       cpp;
  ValNodePtr      cpvnp;
  CitSubPtr       csp;
  IdPatPtr        ipp;
  MedlineEntryPtr mep;
  CharPtr         tmpStr;
  ValNodePtr      pub;

  if (vnp == NULL) return;

  /* check for numerical pub types, NULL ptrvalue */

  switch (vnp->choice) {
    case PUB_PMid :
    case PUB_Muid :
      return;
    default :
      break;
  }
  if (vnp->data.ptrvalue == NULL) return;

  switch (vnp->choice)
    {
    case PUB_Gen :
      cgp = (CitGenPtr) vnp->data.ptrvalue;
      FindReplAuthlist (cgp->authors, fsp);
      FindReplString (&(cgp->cit), fsp);
      FindReplString (&(cgp->volume), fsp);
      FindReplString (&(cgp->issue), fsp);
      FindReplString (&(cgp->pages), fsp);
      FindReplString (&(cgp->title), fsp);
      if (cgp->journal != NULL)
      {
        tmpStr = (CharPtr) cgp->journal->data.ptrvalue;
        FindReplString (&tmpStr, fsp);
        cgp->journal->data.ptrvalue = tmpStr;
      }
      break;
    case PUB_Sub :
      csp = (CitSubPtr) vnp->data.ptrvalue;
      FindReplAuthlist (csp->authors, fsp);
      FindReplString (&(csp->descr), fsp);
      break;
    case PUB_Medline :
      mep = (MedlineEntryPtr) vnp->data.ptrvalue;
      FindReplMedlineEntry(mep, fsp);
      break;
    case PUB_Article :
      cap = (CitArtPtr) vnp->data.ptrvalue;
      FindReplCitArt(cap,fsp);
      break;
    case PUB_Journal :
      cjp = (CitJourPtr) vnp->data.ptrvalue;
      if (cjp->title != NULL)
      {
        tmpStr = (CharPtr) cjp->title->data.ptrvalue;
        FindReplString (&tmpStr, fsp);
        cjp->title->data.ptrvalue = tmpStr;
      }
      FindReplImprint (cjp->imp, fsp);
      break;
    case PUB_Book :
      cbp = (CitBookPtr) vnp->data.ptrvalue;
      FindReplCitBook (cbp, fsp);
      break;
    case PUB_Proc :
      cbp = (CitBookPtr) vnp->data.ptrvalue;
      cpvnp = cbp->otherdata;
      while (cpvnp != NULL)
      {
        if (cpvnp->choice == 1)
          {
            tmpStr = (CharPtr) cpvnp->data.ptrvalue;
            FindReplString (&tmpStr, fsp);
            cpvnp->data.ptrvalue = tmpStr;
          }
        else if (cpvnp->choice == 3)
          FindReplAffil((AffilPtr) cpvnp->data.ptrvalue, fsp);
        cpvnp = cpvnp->next;
      }
      break;
    case PUB_Patent :
      cpp = (CitPatPtr) vnp->data.ptrvalue;
      FindReplAuthlist (cpp->authors, fsp);
      FindReplAuthlist (cpp->applicants, fsp);
      FindReplAuthlist (cpp->assignees, fsp);
      FindReplString (&(cpp->country), fsp);
      FindReplString (&(cpp->doc_type), fsp);
      FindReplString (&(cpp->title), fsp);
      FindReplString (&(cpp->number), fsp);
      FindReplString (&(cpp->app_number), fsp);
      FindReplString (&(cpp->abstract), fsp);
      break;
    case PUB_Pat_id :
      ipp = (IdPatPtr) vnp->data.ptrvalue;
      FindReplString (&(ipp->country), fsp);
      FindReplString (&(ipp->number), fsp);
      FindReplString (&(ipp->app_number), fsp);
      FindReplString (&(ipp->doc_type), fsp);
      break;
    case PUB_Man :
      cbp = (CitBookPtr) vnp->data.ptrvalue;
      FindReplCitBook (cbp, fsp);
      break;
    case PUB_Equiv :
      /* recursive */
      for (pub = vnp->data.ptrvalue; pub != NULL; pub = pub->next) {
        FindReplPub (pub, fsp);
      }
    default:
      break;
    }
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplPubDesc() -                                                  */
/*                                                                       */
/*=======================================================================*/

static void FindReplPubdesc (
  PubdescPtr pdp,
  FindStructPtr fsp
)

{
  ValNodePtr  vnp;

  if (pdp == NULL) return;

  FindReplString (&(pdp->comment), fsp);

  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    FindReplPub (vnp, fsp);
  }
}

static void FindReplBioSource (
  BioSourcePtr biop,
  OrgRefPtr orp,
  FindStructPtr fsp
)

{
  OrgModPtr      omp;
  OrgNamePtr     onp;
  SubSourcePtr   ssp;

  if (biop != NULL) {
    orp = biop->org;
    for (ssp = biop->subtype; ssp != NULL; ssp = ssp->next) {
      if (ssp->subtype != SUBSRC_germline &&
          ssp->subtype != SUBSRC_rearranged &&
          ssp->subtype != SUBSRC_transgenic &&
          ssp->subtype != SUBSRC_environmental_sample) {
        FindReplString (&(ssp->name), fsp);
      }
      FindReplString (&(ssp->attrib), fsp);
    }
  }
  if (orp != NULL) {
    FindReplString (&(orp->taxname), fsp);
    FindReplString (&(orp->common), fsp);
    FindReplStringList (orp->mod, fsp);
    FindReplStringList (orp->syn, fsp);
    FindReplDbxrefs (orp->db, fsp);
    onp = orp->orgname;
    while (onp != NULL) {
      FindReplString (&(onp->attrib), fsp);
      FindReplString (&(onp->lineage), fsp);
      FindReplString (&(onp->div), fsp);
      for (omp = onp->mod; omp != NULL; omp = omp->next) {
        FindReplString (&(omp->subname), fsp);
        FindReplString (&(omp->attrib), fsp);
      }
      onp = onp->next;
    }
  }
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplPatentSeqId() -                                              */
/*                                                                       */
/*=======================================================================*/

static void FindReplPatentSeqId (
  PatentSeqIdPtr pPatentSeqId,
  FindStructPtr pFindStruct
)

{
  if (pPatentSeqId == NULL)
    return;

  if (pPatentSeqId->cit == NULL)
    return;

  FindReplString (&(pPatentSeqId->cit->country), pFindStruct);
  FindReplString (&(pPatentSeqId->cit->number), pFindStruct);
  FindReplString (&(pPatentSeqId->cit->app_number), pFindStruct);
  FindReplString (&(pPatentSeqId->cit->doc_type), pFindStruct);
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplTextSeqId() -                                                */
/*                                                                       */
/*=======================================================================*/

static void FindReplTextSeqId (
  TextSeqIdPtr pTextSeqId, 
  FindStructPtr pFindStruct
)

{
  if (pTextSeqId == NULL)
    return;

  FindReplString (&(pTextSeqId->name), pFindStruct);
  FindReplString (&(pTextSeqId->accession), pFindStruct);
  FindReplString (&(pTextSeqId->release), pFindStruct);
} 

/*=======================================================================*/
/*                                                                       */
/*  FindReplGiim() -                                                     */
/*                                                                       */
/*=======================================================================*/

static void FindReplGiim (
  GiimPtr pGiim, 
  FindStructPtr pFindStruct
)

{
  if (pGiim == NULL)
    return;

  FindReplString (&(pGiim->db), pFindStruct);
  FindReplString (&(pGiim->release), pFindStruct);
} 

/*=======================================================================*/
/*                                                                       */
/*  FindReplPDBSeqId() -                                                 */
/*                                                                       */
/*=======================================================================*/

static void FindReplPDBSeqId (
  PDBSeqIdPtr pPDBSeqId, 
  FindStructPtr pFindStruct
)

{
  if (pPDBSeqId == NULL)
    return;

  FindReplString (&(pPDBSeqId->mol), pFindStruct);
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplObjectId() -                                                 */
/*                                                                       */
/*=======================================================================*/

static void FindReplObjectId (
  ObjectIdPtr pObjectId, 
  FindStructPtr pFindStruct
)

{
  if (pObjectId == NULL)
    return;

  FindReplString (&(pObjectId->str), pFindStruct);
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplSeqId() -                                                    */
/*                                                                       */
/*=======================================================================*/

static void FindReplSeqId (
  SeqIdPtr sip,
  Pointer userdata
)

{
  FindStructPtr  fsp;
  Uint1          subtype;

  /*------------------*/
  /* Check parameters */
  /*------------------*/

  if (sip == NULL)
    return;

  fsp = (FindStructPtr) userdata;
  if (fsp == NULL)
    return;

  /*-----------------------------------*/
  /* Check to see if we're supposed to */
  /* process this subtype or not.      */
  /*-----------------------------------*/

  subtype = sip->choice;
  if (subtype >= NUM_SEQID)
    return;
  if (! fsp->seqidFilter [subtype])
    return;

  /*------------------------------*/
  /* Do search/replace on all the */
  /* different SeqId types.       */
  /*------------------------------*/

  switch (subtype)
    {
    case SEQID_NOT_SET :
      break;
    case SEQID_LOCAL :
      FindReplObjectId((ObjectIdPtr) sip->data.ptrvalue, fsp);
      break;
    case SEQID_GIBBSQ :
    case SEQID_GIBBMT :
      break;
    case SEQID_GIIM :
      FindReplGiim((GiimPtr) sip->data.ptrvalue, fsp);
      break;
    case SEQID_GENBANK :
    case SEQID_EMBL :
    case SEQID_PIR :
    case SEQID_SWISSPROT :
    case SEQID_OTHER :
    case SEQID_DDBJ :
    case SEQID_PRF :
    case SEQID_TPG :
    case SEQID_TPE :
    case SEQID_TPD :
      FindReplTextSeqId((TextSeqIdPtr) sip->data.ptrvalue, fsp);
      break;
    case SEQID_PATENT :
      FindReplPatentSeqId((PatentSeqIdPtr) sip->data.ptrvalue, fsp);
      break;
    case SEQID_GENERAL :
      FindReplDbxrefs((ValNodePtr) sip->data.ptrvalue, fsp);
      break;
    case SEQID_GI :
      break;
    case SEQID_PDB :
      FindReplPDBSeqId((PDBSeqIdPtr) sip->data.ptrvalue, fsp);
      break;
    default:
      break;
    }
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplSendMessages() -                                             */
/*                                                                       */
/*=======================================================================*/

static void FindReplSendMessages (
  FindStructPtr fsp,
  Uint2 itemID,
  Uint2 itemtype
)

{
  if (fsp->send_update == UPDATE_EACH && fsp->did_replace) {
    ObjMgrSetDirtyFlag (fsp->entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, fsp->entityID, 0, 0);
  }
  if (fsp->select_item && (fsp->did_find || fsp->did_replace)) {
    ObjMgrAlsoSelect (fsp->entityID, itemID, itemtype, 0, NULL);
  }
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplBioseqs() -                                                  */
/*                                                                       */
/*=======================================================================*/

static void FindReplBioseqs (
  BioseqPtr bsp,
  Pointer userdata
)

{
  FindStructPtr  fsp;
  SeqIdPtr       sip;

  fsp = (FindStructPtr) userdata;
  fsp->did_find = FALSE;
  fsp->did_replace = FALSE;

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    FindReplSeqId (sip, userdata);
  }

  SeqMgrReplaceInBioseqIndex(bsp);

  FindReplSendMessages (fsp, bsp->idx.itemID, bsp->idx.itemtype);
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplAligns() -                                                   */
/*                                                                       */
/*=======================================================================*/

static void FindReplAligns (
  SeqAlignPtr sap,
  Pointer userdata
)

{
  DenseDiagPtr   ddp;
  DenseSegPtr    dsp;
  FindStructPtr  fsp;
  SeqIdPtr       sip;
  SeqLocPtr      slp;
  StdSegPtr      ssp;

  fsp = (FindStructPtr) userdata;
  fsp->did_find = FALSE;
  fsp->did_replace = FALSE;

  VisitSeqIdsInSeqLoc (sap->bounds, userdata, FindReplSeqId);
  FindReplSeqId (sap->master, userdata);

  if (sap->segs == NULL) return;

  switch (sap->segtype) {
    case SAS_DENDIAG :
      ddp = (DenseDiagPtr) sap->segs;
      for (sip = ddp->id; sip != NULL; sip = sip->next) {
        FindReplSeqId (sip, userdata);
      }
      break;
    case SAS_DENSEG :
      dsp = (DenseSegPtr) sap->segs;
      for (sip = dsp->ids; sip != NULL; sip = sip->next) {
        FindReplSeqId (sip, userdata);
      }
      break;
    case SAS_STD :
      ssp = (StdSegPtr) sap->segs;
      for (slp = ssp->loc; slp != NULL; slp = slp->next) {
        VisitSeqIdsInSeqLoc (slp, userdata, FindReplSeqId);
      }
      break;
    case SAS_DISC :
      /* recursive */
      for (sap = (SeqAlignPtr) sap->segs; sap != NULL; sap = sap->next) {
        FindReplAligns (sap, userdata);
      }
      break;
    default :
      break;
  }

  FindReplSendMessages (fsp, sap->idx.itemID, sap->idx.itemtype);
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplGraphs() -                                                   */
/*                                                                       */
/*=======================================================================*/

static void FindReplGraphs (
  SeqGraphPtr sgp,
  Pointer userdata
)

{
  FindStructPtr  fsp;

  fsp = (FindStructPtr) userdata;
  fsp->did_find = FALSE;
  fsp->did_replace = FALSE;

  VisitSeqIdsInSeqLoc (sgp->loc, userdata, FindReplSeqId);

  FindReplSendMessages (fsp, sgp->idx.itemID, sgp->idx.itemtype);
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplFeats() -                                                    */
/*                                                                       */
/*=======================================================================*/

static void FindReplFeats (
  SeqFeatPtr sfp,
  Pointer userdata
)

{
  BioSourcePtr   biop;
  CodeBreakPtr   cbp;
  CdRegionPtr    crp;
  FindStructPtr  fsp;
  GBQualPtr      gbq;
  GeneRefPtr     grp;
  ImpFeatPtr     ifp;
  OrgRefPtr      orp = NULL;
  PubdescPtr     pdp;
  ProtRefPtr     prp;
  RnaRefPtr      rrp;
  Uint1          subtype;
  tRNAPtr        trp;

  fsp = (FindStructPtr) userdata;
  fsp->did_find = FALSE;
  fsp->did_replace = FALSE;

  /* change seqids on location and product */

  VisitSeqIdsInSeqLoc (sfp->location, userdata, FindReplSeqId);
  VisitSeqIdsInSeqLoc (sfp->product, userdata, FindReplSeqId);

  /* check subtype against filter */

  subtype = sfp->idx.subtype;
  if (subtype >= FEATDEF_MAX) return;
  if (! fsp->featFilter [subtype]) return;

  /* common fields */

  FindReplString (&(sfp->comment), fsp);
  FindReplString (&(sfp->title), fsp);
  FindReplString (&(sfp->except_text), fsp);

  for (gbq = sfp->qual; gbq != NULL; gbq = gbq->next) {
    FindReplString (&(gbq->qual), fsp);
    FindReplString (&(gbq->val), fsp);
  }

  FindReplDbxrefs (sfp->dbxref, fsp);

  /* check for numerical features, NULL ptrvalue */

  switch (sfp->data.choice) {
    case SEQFEAT_BOND :
    case SEQFEAT_SITE :
    case SEQFEAT_PSEC_STR :
    case SEQFEAT_COMMENT:
      return;
    default :
      break;
  }
  if (sfp->data.value.ptrvalue == NULL) return;

  /* feature-specific fields */

  switch (sfp->data.choice) {
    case SEQFEAT_GENE :
      grp = (GeneRefPtr) sfp->data.value.ptrvalue;
      FindReplString (&(grp->locus), fsp);
      FindReplString (&(grp->allele), fsp);
      FindReplString (&(grp->desc), fsp);
      FindReplString (&(grp->maploc), fsp);
      FindReplStringList (grp->syn, fsp);
      FindReplDbxrefs (grp->db, fsp);
      break;
    case SEQFEAT_ORG :
      orp = (OrgRefPtr) sfp->data.value.ptrvalue;
      FindReplBioSource (NULL, orp, fsp);
      break;
    case SEQFEAT_CDREGION :
      crp = (CdRegionPtr) sfp->data.value.ptrvalue;
      for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next) {
        VisitSeqIdsInSeqLoc (cbp->loc, userdata, FindReplSeqId);
      }
      break;
    case SEQFEAT_PROT :
      prp = (ProtRefPtr) sfp->data.value.ptrvalue;
      FindReplString (&(prp->desc), fsp);
      FindReplStringList (prp->name, fsp);
      FindReplStringList (prp->ec, fsp);
      FindReplStringList (prp->activity, fsp);
      FindReplDbxrefs (prp->db, fsp);
      break;
    case SEQFEAT_RNA :
      rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
      if (rrp->ext.choice == 1) {
        FindReplString ((CharPtr PNTR) &(rrp->ext.value.ptrvalue), fsp);
      } else if (rrp->ext.choice == 2) {
        trp = (tRNAPtr) rrp->ext.value.ptrvalue;
        VisitSeqIdsInSeqLoc (trp->anticodon, userdata, FindReplSeqId);
      }
      break;
    case SEQFEAT_PUB :
      pdp = (PubdescPtr) sfp->data.value.ptrvalue;
      FindReplPubdesc (pdp, fsp);
      break;
    case SEQFEAT_SEQ :
      break;
    case SEQFEAT_IMP :
      ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
      FindReplString (&(ifp->key), fsp);
      FindReplString (&(ifp->loc), fsp);
      FindReplString (&(ifp->descr), fsp);
      break;
    case SEQFEAT_REGION :
      FindReplString ((CharPtr PNTR) &(sfp->data.value.ptrvalue), fsp);
      break;
    case SEQFEAT_RSITE :
      break;
    case SEQFEAT_USER :
      break;
    case SEQFEAT_TXINIT :
      break;
    case SEQFEAT_NUM :
      break;
    case SEQFEAT_NON_STD_RESIDUE :
      break;
    case SEQFEAT_HET :
      break;
    case SEQFEAT_BIOSRC :
      biop = (BioSourcePtr) sfp->data.value.ptrvalue;
      FindReplBioSource (biop, NULL, fsp);
      break;
    default :
      break;
  }

  FindReplSendMessages (fsp, sfp->idx.itemID, sfp->idx.itemtype);
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplDescs() -                                                    */
/*                                                                       */
/*=======================================================================*/

static void FindReplDescs (
  SeqDescrPtr sdp,
  Pointer userdata
)

{
  BioSourcePtr   biop;
  FindStructPtr  fsp;
  GBBlockPtr     gbp;
  OrgRefPtr      orp = NULL;
  ObjValNodePtr  ovp;
  PubdescPtr     pdp;
  Uint1          subtype;

  fsp = (FindStructPtr) userdata;
  fsp->did_find = FALSE;
  fsp->did_replace = FALSE;

  /* check subtype against filter */

  subtype = sdp->choice;
  if (subtype >= SEQDESCR_MAX) return;
  if (! fsp->descFilter [subtype]) return;

  /* check for numerical descriptors, NULL ptrvalue */

  switch (sdp->choice) {
    case Seq_descr_mol_type :
    case Seq_descr_method :
      return;
    default :
      break;
  }
  if (sdp->data.ptrvalue == NULL) return;

  /* descriptor-specific fields */

  switch (sdp->choice) {
    case Seq_descr_modif :
      break;
    case Seq_descr_name :
      FindReplString ((CharPtr PNTR) &(sdp->data.ptrvalue), fsp);
      break;
    case Seq_descr_title :
      FindReplString ((CharPtr PNTR) &(sdp->data.ptrvalue), fsp);
      break;
    case Seq_descr_org :
      orp = (OrgRefPtr) sdp->data.ptrvalue;
      FindReplBioSource (NULL, orp, fsp);
      break;
    case Seq_descr_comment :
      FindReplString ((CharPtr PNTR) &(sdp->data.ptrvalue), fsp);
      break;
    case Seq_descr_num :
      break;
    case Seq_descr_maploc :
      break;
    case Seq_descr_pir :
      break;
    case Seq_descr_genbank :
      gbp = (GBBlockPtr) sdp->data.ptrvalue;
      FindReplStringList (gbp->extra_accessions, fsp);
      FindReplStringList (gbp->keywords, fsp);
      FindReplString (&(gbp->source), fsp);
      FindReplString (&(gbp->origin), fsp);
      FindReplString (&(gbp->date), fsp);
      FindReplString (&(gbp->div), fsp);
      FindReplString (&(gbp->taxonomy), fsp);
      break;
    case Seq_descr_pub :
      pdp = (PubdescPtr) sdp->data.ptrvalue;
      FindReplPubdesc (pdp, fsp);
      break;
    case Seq_descr_region :
      FindReplString ((CharPtr PNTR) &(sdp->data.ptrvalue), fsp);
      break;
    case Seq_descr_user :
      break;
    case Seq_descr_sp :
      break;
    case Seq_descr_dbxref :
      break;
    case Seq_descr_embl :
      break;
    case Seq_descr_create_date :
      break;
    case Seq_descr_update_date :
      break;
    case Seq_descr_prf :
      break;
    case Seq_descr_pdb :
      break;
    case Seq_descr_het :
      break;
    case Seq_descr_source :
      biop = (BioSourcePtr) sdp->data.ptrvalue;
      FindReplBioSource (biop, NULL, fsp);
      break;
    case Seq_descr_molinfo :
      break;
    default :
      break;
  }

  if (sdp->extended != 0) {
    ovp = (ObjValNodePtr) sdp;
    FindReplSendMessages (fsp, ovp->idx.itemID, ovp->idx.itemtype);
  }
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplSubmitBlock() -                                              */
/*                                                                       */
/*=======================================================================*/

static void FindReplSubmitBlock (
  SeqSubmitPtr ssp,
  FindStructPtr fsp
)

{
  ContactInfoPtr  cip;
  CitSubPtr       csp;
  SubmitBlockPtr  sub;

  if (ssp == NULL) return;
  sub = ssp->sub;
  if (sub == NULL) return;
  fsp->did_find = FALSE;
  fsp->did_replace = FALSE;

  FindReplString (&(sub->tool), fsp);
  FindReplString (&(sub->user_tag), fsp);
  FindReplString (&(sub->comment), fsp);

  cip = sub->contact;
  if (cip != NULL) {
    FindReplString (&(cip->name), fsp);
    FindReplStringList (cip->address, fsp);
    FindReplString (&(cip->phone), fsp);
    FindReplString (&(cip->fax), fsp);
    FindReplString (&(cip->email), fsp);
    FindReplString (&(cip->telex), fsp);
    FindReplObjectId (cip->owner_id, fsp);
    FindReplString (&(cip->last_name), fsp);
    FindReplString (&(cip->first_name), fsp);
    FindReplString (&(cip->middle_initial), fsp);
    FindReplAuthor (cip->contact, fsp);
  }

  csp = sub->cit;
  if (csp != NULL) {
    FindReplAuthlist (csp->authors, fsp);
    FindReplString (&(csp->descr), fsp);
  }

  FindReplSendMessages (fsp, ssp->idx.itemID, ssp->idx.itemtype);
}

/*=======================================================================*/
/*                                                                       */
/*  FindReplaceInEntity() - New find/replace function.                   */
/*                                                                       */
/*=======================================================================*/

NLM_EXTERN void FindReplaceInEntity (
  Uint2 entityID,
  CharPtr find_string,
  CharPtr replace_string,
  Boolean case_counts,
  Boolean whole_word,
  Boolean do_replace,
  Boolean select_item,
  Int2 send_update,
  BoolPtr descFilter,
  BoolPtr featFilter,
  BoolPtr seqidFilter,
  Boolean do_seqid_local
)

{
  int            ch;
  FindStruct     fs;
  int            j;
  ObjMgrDataPtr  omdp;
  SeqEntryPtr    sep = NULL;
  SeqSubmitPtr   ssp = NULL;

  if (entityID == 0 || StringHasNoText (find_string)) return;

  omdp = ObjMgrGetData (entityID);
  if (omdp != NULL) {
    switch (omdp->datatype) {
      case OBJ_SEQSUB :
        ssp = (SeqSubmitPtr) omdp->dataptr;
        if (ssp != NULL && ssp->datatype == 1) {
          sep = (SeqEntryPtr) ssp->data;
        }
        break;
      case OBJ_BIOSEQ :
        sep = (SeqEntryPtr) omdp->choice;
      case OBJ_BIOSEQSET :
        sep = (SeqEntryPtr) omdp->choice;
      default :
        break;
    }
  }
  /* sep = GetTopSeqEntryForEntityID (entityID); */
  if (sep == NULL) return;

  MemSet ((Pointer) &fs, 0, sizeof (FindStruct));

  fs.entityID = entityID;
  fs.find_string = find_string;
  fs.replace_string = replace_string;
  fs.case_counts = case_counts;
  fs.whole_word = whole_word;
  fs.do_replace = do_replace;
  fs.select_item = select_item;
  fs.send_update = send_update;

  fs.did_find = FALSE;
  fs.did_replace = FALSE;
  fs.dirty = FALSE;

  /* build Boyer-Moore displacement array in advance */

  fs.subLen = StringLen (find_string);

  for (ch = 0; ch < 256; ch++) {
    fs.d [ch] = fs.subLen;
  }
  for (j = 0; j < (int) (fs.subLen - 1); j++) {
    ch = (int) (case_counts ? find_string [j] : TO_UPPER (find_string [j]));
    if (ch >= 0 && ch <= 255) {
      fs.d [ch] = fs.subLen - j - 1;
    }
  }

  /* if desc or feat filter arrays not supplied, default to all TRUE */

  if (descFilter != NULL) {
    MemCopy ((Pointer) &fs.descFilter, (Pointer) descFilter, sizeof (fs.descFilter));
  } else {
    MemSet ((Pointer) &fs.descFilter, (int) TRUE, sizeof (fs.descFilter));
  }

  if (featFilter != NULL) {
    MemCopy ((Pointer) &fs.featFilter, (Pointer) featFilter, sizeof (fs.featFilter));
  } else {
    MemSet ((Pointer) &fs.featFilter, (int) TRUE, sizeof (fs.featFilter));
  }

  /* if seqid filter array not supplied, default to all FALSE */

  if (seqidFilter != NULL) {
    MemCopy ((Pointer) &fs.seqidFilter, (Pointer) seqidFilter, sizeof (fs.seqidFilter));
  } else if (do_seqid_local) {
    MemSet ((Pointer) &fs.seqidFilter, (int) FALSE, sizeof (fs.seqidFilter));
    fs.seqidFilter [SEQID_LOCAL] = TRUE;
  } else {
    MemSet ((Pointer) &fs.seqidFilter, (int) FALSE, sizeof (fs.seqidFilter));
  }

  /* ensure feature subtype is set in sfp->idx block */

  AssignIDsInEntity (entityID, 0, NULL);

  /* visit callbacks that find/replace specific fields */

  VisitBioseqsInSep (sep, (Pointer) &fs, FindReplBioseqs);

  VisitFeaturesInSep (sep, (Pointer) &fs, FindReplFeats);

  VisitAlignmentsInSep (sep, (Pointer) &fs, FindReplAligns);

  VisitGraphsInSep (sep, (Pointer) &fs, FindReplGraphs);

  VisitDescriptorsInSep (sep, (Pointer) &fs, FindReplDescs);

  if (ssp != NULL) {
    FindReplSubmitBlock (ssp, &fs);
  }

  /* send select message, if applicable */

  if (fs.send_update == UPDATE_ONCE && fs.dirty) {
    ObjMgrSetDirtyFlag (entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
  }
}

