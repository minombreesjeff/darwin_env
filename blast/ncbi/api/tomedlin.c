/*  tomedlin.c
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
* File Name:  tomedlin.c
*
* Author:  Jonathan Kans
*   
* Version Creation Date: 10/15/91
*
* $Revision: 6.11 $
*
* File Description:  conversion to medlars format
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: tomedlin.c,v $
* Revision 6.11  2004/03/10 15:19:47  kans
* ParseMedline loops on journal, only saves one of iso-jta or ml-jta to avoid memory leak
*
* Revision 6.10  2003/09/28 20:22:47  kans
* added PubmedEntryToXXXFile functions
*
* Revision 6.9  2003/09/26 18:57:51  kans
* MedlineEntryToDataFile calls MakeMLAuthString for structured author
*
* Revision 6.8  2001/10/29 20:37:06  kans
* MakeAuthorString for structured authors
*
* Revision 6.7  2001/10/22 13:37:12  kans
* break up huge author list
*
* Revision 6.6  2001/10/15 12:24:05  kans
* MedlineEntryToDataFile breaks up long mesh, substance, and xref lists
*
* Revision 6.5  1999/10/26 20:17:04  kans
* allocate separate string for abstract since some may be very long
*
* Revision 6.4  1999/10/01 17:20:08  kans
* fixed stack overflow found by DV
*
* Revision 6.3  1999/03/11 23:32:08  kans
* sprintf casts
*
* Revision 6.2  1998/06/12 20:05:50  kans
* fixed unix compiler warnings
*
* Revision 6.1  1997/12/02 17:42:29  kans
* added cast in sprintf
*
* Revision 6.0  1997/08/25 18:07:55  madden
* Revision changed to 6.0
*
* Revision 5.16  1997/06/19 18:39:27  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.15  1997/04/14 18:02:31  grisha
* set MedlarsEntryToDataFile() to be external
*
 * Revision 5.14  1997/04/11  16:24:13  levitsky
 * *** empty log message ***
 *
 * Revision 5.13  1997/04/11  15:58:58  levitsky
 * New functions for support tomedlars added
 *
 * Revision 5.12  1997/04/10  18:16:06  levitsky
 * New version of MEDLARS output
 *
 * Revision 5.11  1997/04/08  19:35:43  levitsky
 * *** empty log message ***
 *
 * Revision 5.10  1997/04/08  17:30:46  levitsky
 * fixed break error
 *
 * Revision 5.9  1997/04/07  21:34:40  levitsky
 * Added SO field for medlars report
 *
 * Revision 5.8  1997/03/19  18:06:23  kans
 * fixed use of mep->pub_type
 *
 * Revision 5.7  1997/03/19  16:53:08  levitsky
 * publication type added to output
 *
 * Revision 5.6  1997/02/20  21:25:17  kans
 * added MedlineEntryToAbsFile and MedlarsEntryToAbsFile
 *
 * Revision 5.5  1997/02/19  15:09:22  levitsky
 * pmid added to output
 *
 * Revision 5.4  1997/02/12  21:30:02  grisha
 * add code to support MEDLARS report generation
 * for MedlarsEntry
 *
 * Revision 5.3  1997/01/27  16:15:39  grisha
 * Fix bugs with SO field generation
 *
 * Revision 5.2  1996/08/27  23:10:43  kans
 * protect prefixes (e.g., van, el, den) with initials that can be
 * mistaken for suffixes (e.g., I).  this won't protect against
 * names with the Van incorrectly entered at the end.
 *
 * Revision 5.1  1996/05/31  21:04:12  kans
 * check for individual suffixes before stripping
 *
 * Revision 5.0  1996/05/28  13:23:23  ostell
 * Set to revision 5.0
 *
 * Revision 4.2  1996/01/23  20:59:15  kans
 * now allows iso-jta as well as ml-jta
 *
 * Revision 4.1  1995/12/27  17:50:11  kans
 * changed StringAppend to StrngAppend to avoid conflict with future ncbistr.h
 *
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 2.5  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#include <objmedli.h>
#include <tofile.h>
#include <tomedlin.h>

#ifdef VAR_ARGS
#include <varargs.h>
#else
#include <stdarg.h>
#endif

#ifdef COMP_MPW
#pragma segment CdrSegD
#endif

#define BUFSIZE 8192

static CharPtr  buffer;
static CharPtr  pos;

/* ----- Function Prototypes ----- */

static void ClearString PROTO((void));
static void AddString PROTO((CharPtr string));
static CharPtr CleanAuthorString PROTO((CharPtr auth));
static CharPtr CDECL StrngAppend VPROTO((CharPtr first, ...));

/* ----- Function Bodies ----- */

static void ClearString (void)

{
  pos = buffer;
  *pos = '\0';
}

static void AddString (CharPtr string)

{
  pos = StringMove (pos, string);
  *pos = '\0';
}

static ColData table [2] = {{0, 6, 0, 'l', TRUE, TRUE, FALSE},
                            {0, 72, 0, 'l', TRUE, TRUE, TRUE}};


static Char *months[13] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                           "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

static CharPtr MakeMLAuthString (
  CharPtr name,
  CharPtr initials,
  CharPtr suffix
)

{
  Char     ch;
  size_t   len;
  CharPtr  ptr;
  CharPtr  str;
  CharPtr  tmp;

  if (name == NULL) return NULL;

  len = StringLen (name) + StringLen (initials) * 3 + StringLen (suffix);
  str = MemNew (sizeof (Char) * (len + 4));
  if (str == NULL) return NULL;

  tmp = str;

  tmp = StringMove (tmp, name);

  ptr = initials;
  if (! StringHasNoText (initials)) {
    tmp = StringMove (tmp, " ");
    ch = *ptr;
    while (ch != '\0') {
      if (ch == '-') {
        *tmp = '-';
        tmp++;
      } else if (ch != '.') {
        *tmp = ch;
        tmp++;
      }
      ptr++;
      ch = *ptr;
    }
    *tmp = '\0';
  }

  if (! StringHasNoText (suffix)) {
    tmp = StringMove (tmp, " ");
    tmp = StringMove (tmp, suffix);
  }

  return str;
}

static Boolean MedlineEntryToDataFileEx (MedlineEntryPtr mep, Int4 pmid, FILE *fp)

{
  CharPtr         abstract;
  AffilPtr        affil;
  AuthorPtr       ap;
  AuthListPtr     authors = NULL;
  CitArtPtr       cit;
  CitJourPtr      citjour;
  Int2            count;
  CharPtr         curr;
  DatePtr         date = NULL;
  ValNodePtr      gene;
  Int2            i;
  ImprintPtr      imp = NULL;
  CharPtr         issue = NULL;
  size_t          len;
  MedlineMeshPtr  mesh;
  ValNodePtr      names;
  NameStdPtr      nsp;
  CharPtr         p;
  CharPtr         pages = NULL;
  ParData         para;
  PersonIdPtr     pid;
  CharPtr         ptr;
  ValNodePtr      qual;
  Boolean         rsult;
  Char            str [32];
  MedlineRnPtr    substance;
  ValNodePtr      title;
  CharPtr         tmp;
  CharPtr         volume = NULL;
  ValNodePtr      xref;
  CharPtr         lpTemp;
  CharPtr         lpMedAbbr = NULL;
  CharPtr         lpMedCode = NULL;
  CharPtr         lpIssn = NULL;

  rsult = TRUE;
  buffer = MemNew (BUFSIZE);
  if (buffer != NULL) {
    para.openSpace = FALSE;
    ClearString ();
    AddString ("UI  -\t");
    sprintf (str, "%ld", (long) mep->uid);
    AddString (str);
    AddString ("\n");
    rsult = (Boolean) (SendTextToFile (fp, buffer, &para, table) && rsult);
    ClearString ();

    if ( mep->pmid > 0 ) {
      ClearString ();
      AddString ("PM  -\t");
      sprintf (str, "%ld", (long) mep->pmid);
      AddString (str);
      AddString ("\n");
      rsult = (Boolean) (SendTextToFile (fp, buffer, &para, table) && rsult);
      ClearString ();
    }

    if ( mep->pub_type != NULL ) {
      ClearString ();
      AddString ("PT  -\t");
      AddString ((CharPtr) mep->pub_type->data.ptrvalue);
      AddString ("\n");
      rsult = (Boolean) (SendTextToFile (fp, buffer, &para, table) && rsult);
      ClearString ();
    }

    cit = mep->cit;
    if (cit != NULL) {
      authors = cit->authors;
      if (authors != NULL) {
        if (authors->choice == 1) {
          names = authors->names;
          count = 0;
          while (names != NULL) {
            if (count >= 20) {
              rsult = (Boolean) (SendTextToFile (fp, buffer, &para, table) && rsult);
              ClearString ();
              count = 0;
            }
            curr = NULL;
            ap = (AuthorPtr) names->data.ptrvalue;
            if (ap != NULL) {
              pid = ap->name;
              if (pid != NULL) {
                if (pid->choice == 2) {
                  nsp = (NameStdPtr) pid->data;
                  if (nsp != NULL) {
                    if (! StringHasNoText (nsp->names [0])) {
                      curr = MakeMLAuthString (nsp->names [0], nsp->names [4], nsp->names [5]);
                    } else if (! StringHasNoText (nsp->names [3])) {
                      curr = MakeMLAuthString (nsp->names [3], NULL, NULL);
                    }
                  }
                } else if (pid->choice == 3 || pid->choice == 4) {
                  curr = MakeMLAuthString ((CharPtr) pid->data, NULL, NULL);
                }
              }
            }
            if (curr != NULL) {
              AddString ("AU  -\t");
              AddString (curr);
              AddString ("\n");
              curr = MemFree (curr);
            }
            names = names->next;
            count++;
          }
        } else if (authors->choice == 2 || authors->choice == 3) {
          names = authors->names;
          count = 0;
          while (names != NULL) {
            if (count >= 20) {
              rsult = (Boolean) (SendTextToFile (fp, buffer, &para, table) && rsult);
              ClearString ();
              count = 0;
            }
            AddString ("AU  -\t");
            AddString (names->data.ptrvalue);
            AddString ("\n");
            names = names->next;
            count++;
          }
        }
      }
      rsult = (Boolean) (SendTextToFile (fp, buffer, &para, table) && rsult);
      ClearString ();
      title = cit->title;
      count = 0;
      while (title != NULL) {
        if (count >= 20) {
          rsult = (Boolean) (SendTextToFile (fp, buffer, &para, table) && rsult);
          ClearString ();
          count = 0;
        }
        if (title->choice == 1) {
          AddString ("TI  -\t");
          AddString (title->data.ptrvalue);
          AddString ("\n");
        }
        title = title->next;
        count++;
      }
      rsult = (Boolean) (SendTextToFile (fp, buffer, &para, table) && rsult);
      ClearString ();
    }

    mesh = mep->mesh;
    count = 0;
    while (mesh != NULL) {
      if (count >= 20) {
        rsult = (Boolean) (SendTextToFile (fp, buffer, &para, table) && rsult);
        ClearString ();
        count = 0;
      }
      AddString ("MH  -\t");
      if (mesh->mp) {
        AddString ("*");
      }
      AddString (mesh->term);
      qual = mesh->qual;
      while (qual != NULL) {
        AddString ("/");
        if (qual->choice != 0) {
          AddString ("*");
        }
        p = (CharPtr) qual->data.ptrvalue;
        while (*p != '\0') {
          *p = TO_UPPER (*p);
          p++;
        }
        AddString (qual->data.ptrvalue);
        qual = qual->next;
      }
      AddString ("\n");
      mesh = mesh->next;
      count++;
    }
    rsult = (Boolean) (SendTextToFile (fp, buffer, &para, table) && rsult);
    ClearString ();
    substance = mep->substance;
    count = 0;
    while (substance != NULL) {
      if (count >= 20) {
        rsult = (Boolean) (SendTextToFile (fp, buffer, &para, table) && rsult);
        ClearString ();
        count = 0;
      }
      AddString ("RN  -\t");
      switch (substance->type) {
        case 0:
          AddString ("0 (");
          AddString (substance->name);
          AddString (")");
          break;
        case 1:
          AddString (substance->cit);
          AddString (" (");
          AddString (substance->name);
          AddString (")");
          break;
        case 2:
          AddString ("EC ");
          AddString (substance->cit);
          AddString (" (");
          AddString (substance->name);
          AddString (")");
          break;
        default:
          break;
      }
      AddString ("\n");
      substance = substance->next;
      count++;
    }
    rsult = (Boolean) (SendTextToFile (fp, buffer, &para, table) && rsult);
    ClearString ();
    gene = mep->gene;
    while (gene != NULL) {
      AddString ("GS  -\t");
      AddString (gene->data.ptrvalue);
      AddString ("\n");
      gene = gene->next;
    }
    xref = mep->xref;
    count = 0;
    while (xref != NULL) {
      if (count >= 20) {
        rsult = (Boolean) (SendTextToFile (fp, buffer, &para, table) && rsult);
        ClearString ();
        count = 0;
      }
      AddString ("SI  -\t");
      switch (xref->choice) {
        case 1:
          AddString ("DDBJ/");
          break;
        case 2:
          AddString ("CARBBANK/");
          break;
        case 3:
          AddString ("EMBL/");
          break;
        case 4:
          AddString ("HDB/");
          break;
        case 5:
          AddString ("GENBANK/");
          break;
        case 6:
          AddString ("HGML/");
          break;
        case 7:
          AddString ("MIM/");
          break;
        case 8:
          AddString ("MSD/");
          break;
        case 9:
          AddString ("PDB/");
          break;
        case 10:
          AddString ("PIR/");
          break;
        case 11:
          AddString ("PRFSEQDB/");
          break;
        case 12:
          AddString ("PSD/");
          break;
        case 13:
          AddString ("SWISSPROT/");
          break;
        default:
          AddString ("?/");
          break;
      }
      AddString (xref->data.ptrvalue);
      AddString ("\n");
      xref = xref->next;
      count++;
    }
    rsult = (Boolean) (SendTextToFile (fp, buffer, &para, table) && rsult);
    ClearString ();
    citjour = cit->fromptr;
    if (citjour != NULL) {
      imp = citjour->imp;
      if (imp != NULL) {
        date = imp->date;
        if (date != NULL) {
          switch (date->data [0]) {
            case 0:
              AddString ("DP  -\t");
              StringNCpy (str, date->str, sizeof (str) - 1);
              AddString (str);
              AddString ("\n");
              break;
            case 1:
              AddString ("DP  -\t");
              sprintf (str, "%d", date->data [1] + 1900);
              AddString (str);
              AddString (" ");
              i = date->data [2];
              if (i >= 0 && i <= 11) {
                AddString (months [i]);
              }
              if (date->data [3] > 0) {
                AddString (" ");
                sprintf (str, "%d", date->data [3]);
                AddString (str);
              }
              AddString ("\n");
            default:
              break;
          }
        }
      }
      title = citjour->title;
      while (title != NULL) {
        if ( title->choice == Cit_title_jta ) {
            lpTemp = "JC  -\t";
            lpMedCode = title->data.ptrvalue;
        } else if ( title->choice == Cit_title_ml_jta ) {
            lpTemp = "TA  -\t";
            lpMedAbbr = title->data.ptrvalue;
        } else if ( title->choice == Cit_title_issn ) {
            lpTemp = "IS  -\t";
            lpIssn = title->data.ptrvalue;
        } else {
            title = title->next;
            continue;
        }
        AddString (lpTemp);
        AddString (title->data.ptrvalue);
        AddString ("\n");
        title = title->next;
      }
      if (imp != NULL) {
        pages = imp->pages;
        if (pages != NULL) {
          AddString ("PG  -\t");
          AddString (pages);
          AddString ("\n");
        }
        issue = imp->issue;
        if (issue != NULL) {
          AddString ("IP  -\t");
          AddString (issue);
          AddString ("\n");
        }
        volume = imp->volume;
        if (volume != NULL) {
          AddString ("VI  -\t");
          AddString (volume);
          AddString ("\n");
        }
      }
    }
    rsult = (Boolean) (SendTextToFile (fp, buffer, &para, table) && rsult);
    ClearString ();
    abstract = mep->abstract;
    if (abstract != NULL) {
      len = StringLen (abstract);
      tmp = (CharPtr) MemNew (sizeof (Char) * (len + 10));
      ptr = StringMove (tmp, "AB  -\t");
      ptr = StringMove (ptr, abstract);
      ptr = StringMove (ptr, "\n");
      rsult = (Boolean) (SendTextToFile (fp, tmp, &para, table) && rsult);
      MemFree (tmp);
    }
    ClearString ();
    if (cit != NULL && authors != NULL) {
      affil = authors->affil;
      if (affil != NULL && affil->choice == 1) {
        AddString ("AD  -\t");
        AddString (affil->affil);
        AddString ("\n");
      }
    }
    if (citjour != NULL && imp != NULL) {
      AddString ("SO  -\t");
      if ( lpMedAbbr != NULL ) {
          AddString(lpMedAbbr);
      } else if ( lpIssn != NULL ) {
          AddString(lpIssn);
      } else if ( lpMedCode != NULL ) {
          AddString(lpMedCode);
      }
      AddString (" ");
      if (date != NULL) {
        switch (date->data [0]) {
          case 0:
            StringNCpy (str, date->str, sizeof (str) - 1);
            AddString (str);
            AddString (";");
            break;
          case 1:
            sprintf (str, "%d", date->data [1] + 1900);
            AddString (str);
            AddString (" ");
            i = date->data [2];
            if (i >= 0 && i <= 11) {
              AddString (months [i]);
            }
            if (date->data [3] > 0) {
              AddString (" ");
              sprintf (str, "%d", date->data [3]);
              AddString (str);
            }
            AddString (";");
          default:
            break;
        }
      }
      if (volume != NULL) {
        AddString (volume);
      }
      if (issue != NULL) {
        AddString ("(");
        AddString (issue);
        AddString (")");
      }
      if (pages != NULL) {
        AddString (":");
        AddString (pages);
      }
      AddString ("\n");
    }
    rsult = (Boolean) (SendTextToFile (fp, buffer, &para, table) && rsult);
    ClearString ();
    buffer = MemFree (buffer);
  }
  return rsult;
}

NLM_EXTERN Boolean MedlineEntryToDataFile (MedlineEntryPtr mep, FILE *fp)

{
  return MedlineEntryToDataFileEx (mep, 0, fp);
}

NLM_EXTERN Boolean PubmedEntryToDataFile (PubmedEntryPtr pep, FILE *fp)

{
  MedlineEntryPtr  mep;

  if (pep == NULL || fp == NULL) return FALSE;
  mep = (MedlineEntryPtr) pep->medent;
  if (mep == NULL) return FALSE;
  return MedlineEntryToDataFileEx (mep, pep->pmid, fp);
}

#ifdef VAR_ARGS
static CharPtr CDECL StrngAppend (first, va_alist)
CharPtr first;
va_dcl
#else
static CharPtr CDECL StrngAppend (CharPtr first, ...)
#endif

{
  va_list  args;
  Uint2    len;
  CharPtr  rsult;
  CharPtr  str;

#ifdef VAR_ARGS
  va_start (args);
#else
  va_start (args, first);
#endif
  len = (Uint2) StringLen (first);
  str = va_arg (args, CharPtr);
  while (str != NULL) {
    len += StringLen (str);
    str = va_arg (args, CharPtr);
  }
  va_end(args);

#ifdef VAR_ARGS
  va_start (args);
#else
  va_start (args, first);
#endif
  rsult = MemNew (len + 1);
  StringCpy (rsult, first);
  str = va_arg (args, CharPtr);
  while (str != NULL) {
    StringCat (rsult, str);
    str = va_arg (args, CharPtr);
  }
  va_end(args);
  return rsult;
}

static CharPtr suffixes [] =
{"Jr", "Sr", "I", "II", "III", "IV", "V", "VI",
"1st", "2nd", "3rd", "4th", "5th", "6th",
"1d", "2d", "3d", NULL};

static CharPtr prefixes [] =
{"van ", "ten ", "ter ", "Mac ", "Mc ", "el ", "al ",
"Ben ", "ben ", "Bar ", "den ", NULL};

static CharPtr CleanAuthorString (CharPtr auth)

{
  Char     ch;
  CharPtr  first;
  Int2     i;
  CharPtr  last;
  Uint4    len;
  CharPtr  p;
  CharPtr  per;
  CharPtr  ps;
  CharPtr  str;

  if (StringLen (auth) > 0) {
    if (StringCmp (auth, "et al") == 0) {
      str = StringSave ("...");
    } else {
      p = NULL;
      first = NULL;
      last = StringSave (auth);
      ps = StringRChr (last, ' ');
      if (ps != NULL && ps != StringChr (last, ' ')) {
        i = 0;
        while (prefixes [i] != NULL) {
          len = StringLen (prefixes [i]);
          if (len > 0 && StringNCmp (last, prefixes [i], len) == 0) {
            last [len - 1] = '_';
          }
          i++;
        }
        ps = StringRChr (last, ' ');
      }
      if (ps != NULL && ps != StringChr (last, ' ')) {
        per = StringChr (ps, '.');
        if (per != NULL) {
          *per = '\0';
        }
        i = 0;
        while (suffixes [i] != NULL) {
          if (StringCmp (ps + 1, suffixes [i]) == 0) {
            *ps = '\0';
          }
          i++;
        }
        /*
        p = ps + 1;
        while (*p != '\0') {
          if (! IS_UPPER(*p)) {
            *ps = '\0';
            p = ps;
          } else {
            p++;
          }
        }
        */
      }
      ps = StringRChr (last, ' ');
      if (ps != NULL) {
        p = ps + 1;
        *ps = '\0';
      }
      if (p != NULL) {
        len = StringLen (p);
        first = MemNew (3 * (size_t) len + 1);
        i = 0;
        while (*p != '\0') {
          first [i] = *p;
          i++;
          first [i] = '.';
          i++;
          first [i] = ' ';
          i++;
          p++;
        }
      }
      ps = last;
      ch = *ps;
      while (ch != '\0') {
        if (ch == '_') {
          *ps = ' ';
        }
        ps++;
        ch = *ps;
      }
      if (first != NULL) {
        str = StrngAppend (first, (CharPtr) last, (CharPtr) NULL);
        last = MemFree (last);
        first = MemFree (first);
      } else {
        str = last;
      }
    }
  } else {
    str = StringSave ("");
  }
  return str;
}

static CharPtr MakeAuthorString (
  CharPtr name,
  CharPtr initials,
  CharPtr suffix
)

{
  Char     ch;
  size_t   len;
  CharPtr  ptr;
  CharPtr  str;
  CharPtr  tmp;

  if (name == NULL) return NULL;

  len = StringLen (name) + StringLen (initials) * 3 + StringLen (suffix);
  str = MemNew (sizeof (Char) * (len + 4));
  if (str == NULL) return NULL;

  tmp = str;

  ptr = initials;
  if (! StringHasNoText (initials)) {
    ch = *ptr;
    while (ch != '\0') {
      if (ch == '-') {
        *tmp = '-';
        tmp++;
      } else if (ch != '.') {
        *tmp = ch;
        tmp++;
        *tmp = '.';
        tmp++;
        *tmp = ' ';
        tmp++;
      }
      ptr++;
      ch = *ptr;
    }
    *tmp = '\0';
  }

  tmp = StringMove (tmp, name);
  if (! StringHasNoText (suffix)) {
    tmp = StringMove (tmp, " ");
    tmp = StringMove (tmp, suffix);
  }

  return str;
}

NLM_EXTERN MedlinePtr ParseMedline (MedlineEntryPtr mep)

{
  AffilPtr         affil;
  AuthorPtr        ap;
  AuthListPtr      authors;
  CharPtr          chptr;
  CitArtPtr        cit;
  CitJourPtr       citjour;
  CharPtr          curr;
  DatePtr          date;
  ValNodePtr       gene;
  ImprintPtr       imp;
  CharPtr          iso_jta;
  CharPtr          last;
  MedlineMeshPtr   mesh;
  CharPtr          ml_jta;
  MedlinePtr       mPtr = NULL;
  ValNodePtr       names;
  NameStdPtr       nsp;
  PersonIdPtr      pid;
  ValNodePtr       qual;
  Char             str [32];
  MedlineRnPtr     substance;
  ValNodePtr       title;

  buffer = MemNew (BUFSIZE);
  if (buffer != NULL) {
    ClearString ();
    mPtr = MemNew (sizeof (MedlineData));
    if (mPtr != NULL && mep != NULL) {
      sprintf (str, "%1ld", (long) mep->uid);
      mPtr->uid = StringSave (str);
      cit = mep->cit;
      if (cit != NULL) {
        if (cit->from == 1) {
          citjour = (CitJourPtr) cit->fromptr;
          if (citjour != NULL) {
            iso_jta = NULL;
            ml_jta = NULL;
            title = citjour->title;
            while (title != NULL) {
              if (title->choice == 5) {
                iso_jta = title->data.ptrvalue;
              } else if (title->choice == 6) {
                ml_jta = title->data.ptrvalue;
              }
              title = title->next;
            }
            if (iso_jta != NULL) {
              AddString (iso_jta);
              AddString (" ");
              mPtr->journal = StringSave (buffer);
              ClearString ();
            } else if (ml_jta != NULL) {
              AddString (ml_jta);
              AddString (" ");
              mPtr->journal = StringSave (buffer);
              ClearString ();
            }
            imp = citjour->imp;
            if (imp != NULL) {
              date = imp->date;
              if (date != NULL) {
                switch (date->data [0]) {
                  case 0:
                    StringNCpy (str, date->str, sizeof (str) - 1);
                    chptr = StringChr (str, ' ');
                    if (chptr != NULL) {
                      *chptr = '\0';
                    }
                    mPtr->year = StringSave (str);
                    break;
                  case 1:
                    sprintf (str, "%0d", (Int2) (date->data [1] + 1900));
                    mPtr->year = StringSave (str);
                  default:
                    break;
                }
              }
              mPtr->volume = StringSave (imp->volume);
              mPtr->pages = StringSave (imp->pages);
            }
          }
        }
        title = cit->title;
        while (title != NULL) {
          switch (title->choice) {
            case 1:
              mPtr->title = StringSave ((CharPtr) title->data.ptrvalue);
              break;
            case 2:
              break;
            case 3:
              mPtr->transl = StringSave ((CharPtr) title->data.ptrvalue);
              break;
            default:
              break;
          }
          title = title->next;
        }
        authors = cit->authors;
        if (authors != NULL) {
          if (authors->choice == 1 || authors->choice == 2 || authors->choice == 3) {
            names = authors->names;
            while (names != NULL) {
              last = mPtr->authors;
              curr = NULL;
              if (authors->choice == 1) {
                ap = (AuthorPtr) names->data.ptrvalue;
                if (ap != NULL) {
                  pid = ap->name;
                  if (pid != NULL) {
                    if (pid->choice == 2) {
                      nsp = (NameStdPtr) pid->data;
                      if (nsp != NULL) {
                        if (! StringHasNoText (nsp->names [0])) {
                          curr = MakeAuthorString (nsp->names [0], nsp->names [4], nsp->names [5]);
                        } else if (! StringHasNoText (nsp->names [3])) {
                          curr = MakeAuthorString (nsp->names [3], NULL, NULL);
                        }
                      }
                    } else if (pid->choice == 3 || pid->choice == 4) {
                      curr = MakeAuthorString ((CharPtr) pid->data, NULL, NULL);
                    }
                  }
                }
              } else if (authors->choice == 2 || authors->choice == 3) {
                curr = CleanAuthorString ((CharPtr) names->data.ptrvalue);
              }
              if (last != NULL) {
                if (names->next != NULL) {
                  mPtr->authors = StrngAppend (last, (CharPtr) ", ",
                                                (CharPtr) curr, (CharPtr) NULL);
                } else {
                  mPtr->authors = StrngAppend (last, (CharPtr) " & ",
                                                (CharPtr) curr, (CharPtr) NULL);
                }
                last = MemFree (last);
                curr = MemFree (curr);
              } else {
                mPtr->authors = curr;
              }
              names = names->next;
            }
          }
          affil = authors->affil;
          if (affil != NULL) {
            if (affil->choice == 1) {
              mPtr->affil = StringSave (affil->affil);
            }
          }
        }
      }
      mPtr->abstract = StringSave (mep->abstract);
      mesh = mep->mesh;
      while (mesh != NULL) {
        if (mesh->mp) {
          StringNCpy (str, "*", sizeof (str));
        } else {
          StringNCpy (str, "", sizeof (str));
        }
        last = mPtr->mesh;
        if (last != NULL) {
          mPtr->mesh = StrngAppend (last, (CharPtr) "\n  ", (CharPtr) str,
                                     (CharPtr) mesh->term, (CharPtr) NULL);
          last = MemFree (last);
        } else {
          mPtr->mesh = StrngAppend ((CharPtr) "  ", (CharPtr) str,
                                     (CharPtr) mesh->term, (CharPtr) NULL);
        }
        qual = mesh->qual;
        while (qual != NULL) {
          if (qual->choice != 0) {
            StringNCpy (str, "*", sizeof (str));
          } else {
            StringNCpy (str, "", sizeof (str));
          }
          last = mPtr->mesh;
          mPtr->mesh = StrngAppend (last, (CharPtr) "/", (CharPtr) str,
                                    (CharPtr) qual->data.ptrvalue, (CharPtr) NULL);
          last = MemFree (last);
          qual = qual->next;
        }
        mesh = mesh->next;
      }
      if (mPtr->mesh != NULL) {
        last = mPtr->mesh;
        mPtr->mesh = StrngAppend (last, (CharPtr) "\n", (CharPtr) NULL);
        last = MemFree (last);
      }
      gene = mep->gene;
      if (gene != NULL) {
        ClearString ();
        while (gene != NULL) {
          AddString ("  ");
          AddString (gene->data.ptrvalue);
          AddString ("\n");
          gene = gene->next;
        }
        mPtr->gene = StringSave (buffer);
      }
      substance = mep->substance;
      if (substance != NULL) {
        ClearString ();
        while (substance != NULL) {
          AddString ("  ");
          switch (substance->type) {
            case 0:
              AddString (substance->name);
              break;
            case 1:
              AddString (substance->name);
              AddString (" (");
              AddString ("CAS ");
              AddString (substance->cit);
              AddString (")");
              break;
            case 2:
              AddString (substance->name);
              AddString (" (");
              AddString ("EC ");
              AddString (substance->cit);
              AddString (")");
              break;
            default:
              break;
          }
          AddString ("\n");
          substance = substance->next;
        }
        mPtr->substance = StringSave (buffer);
      }
    }
    ClearString ();
    buffer = MemFree (buffer);
  }
  return mPtr;
}

NLM_EXTERN MedlinePtr FreeMedline (MedlinePtr mPtr)

{
  if (mPtr != NULL) {
    mPtr->journal = MemFree (mPtr->journal);
    mPtr->volume = MemFree (mPtr->volume);
    mPtr->pages = MemFree (mPtr->pages);
    mPtr->year = MemFree (mPtr->year);
    mPtr->title = MemFree (mPtr->title);
    mPtr->transl = MemFree (mPtr->transl);
    mPtr->authors = MemFree (mPtr->authors);
    mPtr->affil = MemFree (mPtr->affil);
    mPtr->abstract = MemFree (mPtr->abstract);
    mPtr->mesh = MemFree (mPtr->mesh);
    mPtr->gene = MemFree (mPtr->gene);
    mPtr->substance = MemFree (mPtr->substance);
    mPtr->uid = MemFree (mPtr->uid);
    mPtr = MemFree (mPtr);
  }
  return NULL;
}

static ColData  colFmt [3] = {{0, 0, 0, 'l', TRUE, TRUE, FALSE},
                              {0, 0, 0, 'l', TRUE, TRUE, FALSE},
                              {0, 0, 0, 'l', TRUE, TRUE, TRUE}};

static ColData  mshFmt [1] = {{0, 80, 0, 'l', FALSE, FALSE, TRUE}};

static Boolean MedlineEntryToDocOrAbsFile (MedlineEntryPtr mep, Int4 pmid, FILE *fp, Boolean showMesh)

{
  size_t      len;
  MedlinePtr  mPtr;
  ParData     para;
  CharPtr     ptr;
  Boolean     rsult;
  CharPtr     tmp;

  rsult = TRUE;
  if (fp != NULL && mep != NULL) {
    mPtr = ParseMedline (mep);
    if (mPtr != NULL) {
      buffer = MemNew (BUFSIZE);
      if (buffer != NULL) {
        para.openSpace = FALSE;
        ClearString ();
        AddString (mPtr->journal);
        AddString ("\t");
        AddString (mPtr->volume);
        AddString (":\t");
        AddString (mPtr->pages);
        AddString ("  (");
        AddString (mPtr->year);
        AddString (")");
        AddString (" [");
        AddString (mPtr->uid);
        AddString ("]");
        AddString ("\n");
        rsult = (Boolean) (SendTextToFile (fp, buffer, &para, colFmt) && rsult);
        ClearString ();
        AddString (mPtr->title);
        AddString ("\n");
        rsult = (Boolean) (SendTextToFile (fp, buffer, NULL, NULL) && rsult);
        ClearString ();
        if (mPtr->transl != NULL) {
          AddString ("[");
          AddString (mPtr->transl);
          AddString ("]\n");
          rsult = (Boolean) (SendTextToFile (fp, buffer, NULL, NULL) && rsult);
          ClearString ();
        }
        AddString (mPtr->authors);
        AddString ("\n");
        rsult = (Boolean) (SendTextToFile (fp, buffer, NULL, NULL) && rsult);
        ClearString ();
        if (mPtr->affil != NULL) {
          AddString (mPtr->affil);
          AddString ("\n");
          rsult = (Boolean) (SendTextToFile (fp, buffer, NULL, NULL) && rsult);
          ClearString ();
        }
        if (mPtr->abstract != NULL) {
          len = StringLen (mPtr->abstract);
          tmp = (CharPtr) MemNew (sizeof (Char) * (len + 10));
          ptr = StringMove (tmp, mPtr->abstract);
          ptr = StringMove (ptr, "\n");
          rsult = (Boolean) (SendTextToFile (fp, tmp, NULL, NULL) && rsult);
          MemFree (tmp);
        }
        if (showMesh) {
          if (mPtr->mesh != NULL) {
            rsult = (Boolean) (SendTextToFile (fp, "MeSH Terms:\n", NULL, NULL) && rsult);
            rsult = (Boolean) (SendTextToFile (fp, mPtr->mesh, &para, mshFmt) && rsult);
          }
          if (mPtr->gene != NULL) {
            rsult = (Boolean) (SendTextToFile (fp, "Gene Symbols:\n", NULL, NULL) && rsult);
            rsult = (Boolean) (SendTextToFile (fp, mPtr->gene, &para, mshFmt) && rsult);
          }
          if (mPtr->substance != NULL) {
            rsult = (Boolean) (SendTextToFile (fp, "Substances:\n", NULL, NULL) && rsult);
            rsult = (Boolean) (SendTextToFile (fp, mPtr->substance, &para, mshFmt) && rsult);
          }
        }
        buffer = MemFree (buffer);
      }
      mPtr = FreeMedline (mPtr);
    }
  }
  return rsult;
}

NLM_EXTERN Boolean MedlineEntryToDocFile (MedlineEntryPtr mep, FILE *fp)

{
  return MedlineEntryToDocOrAbsFile (mep, 0, fp, TRUE);
}

NLM_EXTERN Boolean MedlineEntryToAbsFile (MedlineEntryPtr mep, FILE *fp)

{
  return MedlineEntryToDocOrAbsFile (mep, 0, fp, FALSE);
}

NLM_EXTERN Boolean PubmedEntryToDocFile (PubmedEntryPtr pep, FILE *fp)

{
  MedlineEntryPtr  mep;

  if (pep == NULL || fp == NULL) return FALSE;
  mep = (MedlineEntryPtr) pep->medent;
  if (mep == NULL) return FALSE;
  return MedlineEntryToDocOrAbsFile (mep, pep->pmid, fp, TRUE);
}

NLM_EXTERN Boolean PubmedEntryToAbsFile (PubmedEntryPtr pep, FILE *fp)

{
  MedlineEntryPtr  mep;

  if (pep == NULL || fp == NULL) return FALSE;
  mep = (MedlineEntryPtr) pep->medent;
  if (mep == NULL) return FALSE;
  return MedlineEntryToDocOrAbsFile (mep, pep->pmid, fp, FALSE);
}

#define IBM_MEDLINE_DIVSS '$'
#define IBM_MEDLINE_DIVSV '&'
#define IBM_MEDLINE_DIVEV '#'
#define SIZE_OF_ARRAY(x)  (sizeof(x)/sizeof(x[0]))

typedef struct {
    char  cSubHead[4];
    char* pFullName;
  } SSubheadAbbr;

static SSubheadAbbr theSubHead [] = {
  { "VE", "veterinary" },
  { "UR", "urine" },
  { "UT", "utilization" },
  { "UY", "Uruguay" },
  { "UL", "ultrastructure" },
  { "US", "ultrasonography" },
  { "TD", "trends" },
  { "TR", "transplantation" },
  { "TM", "transmission" },
  { "TO", "toxicity" },
  { "TH", "therapy" },
  { "TU", "therapeutic use" },
  { "SU", "surgery" },
  { "SD", "supply & distribution" },
  { "SN", "statistics & numerical data" },
  { "ST", "standards" },
  { "SE", "secretion" },
  { "SC", "secondary" },
  { "RH", "rehabilitation" },
  { "RT", "radiotherapy" },
  { "RI", "radionuclide imaging" },
  { "RA", "radiography" },
  { "RE", "radiation effects" },
  { "PX", "psychology" },
  { "PC", "prevention & control" },
  { "PO", "poisoning" },
  { "PP", "physiopathology" },
  { "PH", "physiology" },
  { "PD", "pharmacology" },
  { "PK", "pharmacokinetics" },
  { "PA", "pathology" },
  { "PY", "pathogenicity" },
  { "PS", "parasitology" },
  { "OG", "organization & administration" },
  { "NU", "nursing" },
  { "NP", "Nepal" },
  { "MO", "mortality" },
  { "MI", "microbiology" },
  { "MT", "methods" },
  { "ME", "metabolism" },
  { "MA", "manpower" },
  { "LJ", "legislation & jurisprudence" },
  { "IP", "isolation & purification" },
  { "IS", "instrumentation" },
  { "IR", "innervation" },
  { "IN", "injuries" },
  { "PR", "in pregnancy" },
  { "OA", "in old age" },
  { "MY", "in middle age" },
  { "IC", "in infancy & childhood" },
  { "AU", "in adulthood" },
  { "AO", "in adolescence" },
  { "IM", "immunology" },
  { "IL", "Illinois" },
  { "HI", "history" },
  { "GD", "growth & development" },
  { "GE", "genetics" },
  { "ET", "etiology" },
  { "EH", "ethnology" },
  { "EP", "epidemiolgy" },
  { "EN", "enzymology" },
  { "EM", "embryology" },
  { "ED", "education" },
  { "EC", "economics" },
  { "DT", "drug therapy" },
  { "DE", "drug effects" },
  { "DH", "diet therapy" },
  { "DU", "diagnostic use" },
  { "DI", "diagnosis" },
  { "DF", "deficiency" },
  { "CI", "chemically induced" },
  { "CY", "cytology" },
  { "CT", "contraindications" },
  { "CN", "congenital" },
  { "CO", "complications" },
  { "CL", "classification" },
  { "CH", "chemistry" },
  { "CL", "chemistry induced" },
  { "CS", "chemical synthesis" },
  { "CD", "Chad" },
  { "CF", "cerebrospinal fluid" },
  { "BS", "blood supply" },
  { "BL", "blood" },
  { "BI", "biosynthesis" },
  { "BT", "Bhutan" },
  { "AI", "antagonists & inhibitors" },
  { "AH", "anatomy & histology" },
  { "AN", "analysis" },
  { "AA", "analogs & derivatives" },
  { "AE", "adverse effects" },
  { "AD", "administration & dosage" },
  { "AB", "abnormalities" },
  { "AG", "AG" },
  { "VI", "VI" }
};

/****************************************************************************/
/*.doc GetTopicSubHead (internal) */
/*+
  This function search subhead using subhead abbreviation. Return
  pointer to founded subhead or original subhead abbreviation if
  subhead not found.
-*/
/****************************************************************************/
static char*
/*FCN*/GetTopicSubHead (
  char* lpSubHead
){
    register int i;            /* fast index */

    for ( i = 0; i < SIZE_OF_ARRAY(theSubHead); i++ ) {
         if ( StringICmp (lpSubHead, theSubHead[i].cSubHead)
                                                                == 0 ) {
             return theSubHead[i].pFullName; /* return full name */
	   }
       }

    return lpSubHead;          /* return original subhead */
  }                                /* GetTopicSubHead() */

/****************************************************************************/
/*.doc TranslateMesh (internal) */
/*+
   This function translate MeSH string to standart printable form
-*/
/****************************************************************************/
static void
/*FCN*/TranslateMesh (
  char* lpOut,
  char* lpRecord
){
    register char* lpTemp;               /* Temporary pointer */
    char*          lpMainTerm;           /* Main termin */
    int            iDescClass;           /* Descriptor Class */
    char           cInputType;           /* Input type */

    /* extract Mesh Term from Mesh record */
    if ( (lpTemp = strchr (lpRecord, IBM_MEDLINE_DIVSS)) != NULL ) {
        *lpTemp = '\0';         /* set EOS for Mesh Term */
        lpTemp++;               /* lpTemp set to next character after $ */
      } else {
        strcpy(lpOut,lpRecord);
        return;
      }

    lpMainTerm = lpRecord;
    strcpy(lpOut,lpMainTerm);

    /* Skip descriptor class. A number that designates to
       what class the MeSH heading belongs as follows:
       1 - an INDEX MEDICUS major descriptor
       2 - a citation type major descriptor, e.g., ENGLISH ABSTRACT
       3 - a check tag major descriptor, e.g., HUMAN
       4 - a geographic major descriptor, e.g., FRANCE
       5 - a non-MeSH major descriptor, e.g., PATIENT CARE
           MANAGEMENT (NON MESH)
       6 - a withdrawn major descriptor
    */
    if ( isdigit((int)(*lpTemp)) ) {
        lpRecord = lpTemp;
        lpTemp++;
        iDescClass = atoi (lpRecord);
        iDescClass = iDescClass;       /* Do not use descriptor class */
      }

    /* Skip next subelement. Value is I or M. 'I' means an indexer
       input the Heading. 'M' means it was attached throwgh
       computer mapping
    */
    if (    *lpTemp == IBM_MEDLINE_DIVSS
         && (*(lpTemp+1) == 'I' || *(lpTemp+1) == 'M') ) {
        lpTemp++;                         /* skip IBM_MEDLINE_DIVSS */
        cInputType = *lpTemp++;
        cInputType = cInputType;
      }

    /* Qualifier List. A list of two-letter subheading (qualifier)
       abbreviations, separated by slashes if there is more than
       one value. Each subheading abbreviation may have an *
       (central concept indicator) preceding it. This subelement
       may consist of a single *, which indicates the MeSH
       heading alone is a central concept of the article.
    */
    if ( *lpTemp == IBM_MEDLINE_DIVSS ) {
        int        bIsCentralConcept;
        char       cSubHead[3];

        lpTemp++;               /* skip IBM_MEDLINE_DIVSS */
        cSubHead[2] = '\0';     /* set EOL */

        /* read tokens from qualifier list */
        while ( (cSubHead[0]=*lpTemp++) != '\0' ) {
             bIsCentralConcept = 0;
             if ( cSubHead[0] == '*' ) { /* is central concept mark? */
                 cSubHead[0] = *lpTemp++;
                 if ( cSubHead[0] == '/' ) {
                     strcpy(lpOut,"*");
                     strcat(lpOut,lpMainTerm);
                     continue;
		   }
                 if (    cSubHead[0] == '\0'
                      || cSubHead[0] == IBM_MEDLINE_DIVEV
                      || cSubHead[0] == IBM_MEDLINE_DIVSV ) {
                     strcpy(lpOut,"*");
                     strcat(lpOut,lpMainTerm);
                     break;
		   }
                 bIsCentralConcept = 1;
	       }
             if ( (cSubHead[1] = *lpTemp++) == '\0' ) {
                 /* Special case for mii.d80.back66.rf, it has a bad data
                    in MeSH heading for entry 68324942
                 */
                 break;
	       }
             strcat(lpOut,"/");
             if ( bIsCentralConcept == 1 ) {
                 strcat(lpOut,"*");
	       }
             strcat(lpOut,GetTopicSubHead(cSubHead));
             if ( *lpTemp == '/' ) {
                 lpTemp++;
	       } else if (    *lpTemp == IBM_MEDLINE_DIVEV
                         || *lpTemp == IBM_MEDLINE_DIVSV ) {
                 /* ignore Sort version and Entry version */
                 break;
	       }
	   }
      }

    return;
  }                                /* TranslateMesh() */


/***********************************************************************/
/*.doc MedlarsEntryToDataFile (external) */
/*+
   Function send MEDLARS entry to I/O stream
-*/
/***********************************************************************/
NLM_EXTERN Boolean
/*FCN*/MedlarsEntryToDataFile (
  MedlarsEntryPtr mep,
  FILE *fp
){
    ParData thePara;
    CharPtr lpBuffer;
    MedlarsRecordPtr pRecord;
    Boolean bResult = TRUE;
    char buf[BUFSIZE];
    int i, exist;
    char* ptr;
#define MAX_ELHILL 24
    int ElhillOrder[MAX_ELHILL] = {
      /* UI */ MEDLINE_UI,
      /* AU */ MEDLINE_AU,
      /* TI */ MEDLINE_TI,
      /* LA */ MEDLINE_LA,
      /* MH */ MEDLINE_MH,
      /* RN */ MEDLINE_RN,
      /* PT */ MEDLINE_PT,
      /* ID */ MEDLINE_ID,
      /* DA */ MEDLINE_DA,
      /* DP */ MEDLINE_DP,
      /* IS */ MEDLINE_IS,
      /* TA */ MEDLINE_TA,
      /* PG */ MEDLINE_PG,
      /* SB */ MEDLINE_SB,
      /* CY */ MEDLINE_CY,
      /* IP */ MEDLINE_IP,
      /* VI */ MEDLINE_VI,
      /* JC */ MEDLINE_JC,
      /* AA */ MEDLINE_AA,
      /* EM */ MEDLINE_EM,
      /* AB */ MEDLINE_AB,
      /* AD */ MEDLINE_AD,
      /* RF */ MEDLINE_RF,
      /* RO */ MEDLINE_RO
    };

    char* TA="";
    char* DP="";
    char* VI="";
    char* IP="";
    char* PG="";

    if ( mep == NULL || fp == NULL ) {
        return FALSE;
      }

    MemSet (&thePara, 0, sizeof(thePara));

    if ( (lpBuffer=MemNew(BUFSIZE)) == NULL ) return FALSE;

    /* First run to print known Elhill codes */
    for( i = 0; i < MAX_ELHILL; i++ ) {

        for ( pRecord = mep->recs;
              pRecord != NULL;
              pRecord = pRecord->next ) {

          if( pRecord->code != ElhillOrder[i] ) continue;

          if ( pRecord->abbr != NULL ) {
            /* This record has record type abbreviation */
            sprintf( lpBuffer, "%-4s-\t", pRecord->abbr );
          } else {
            /* This record has no record type abbreviation, use number */
            sprintf ( lpBuffer, "%-4d-\t", pRecord->code );
          }

          if ( MEDLINE_TA == pRecord->code )
            TA = pRecord->data; /* TA - name of journal */
          if ( MEDLINE_DP == pRecord->code )
            DP = pRecord->data; /* DP - date of publication */
          if ( MEDLINE_VI == pRecord->code )
            VI = pRecord->data; /* VI - volume number */
          if ( MEDLINE_IP == pRecord->code )
            IP = pRecord->data; /* IP - issue number */
          if ( MEDLINE_PG == pRecord->code )
            PG = pRecord->data; /* PG - page numbers */


           switch( pRecord->code ) {

             case MEDLINE_MH:
               TranslateMesh( buf, pRecord->data);
               StringCat(lpBuffer,buf);
               break;
            
             case MEDLINE_RO:
               strncpy( buf, pRecord->data, BUFSIZE-1 );
               for ( ptr = buf; *ptr; ptr++ )
                 if( '$' == *ptr ) *ptr = ':';
               StringCat( lpBuffer, buf );
               break;
            
             case MEDLINE_ID:
               strncpy( buf, pRecord->data, BUFSIZE-1 );
               for ( ptr = buf; *ptr; ptr++ )
                 if( '$' == *ptr ) *ptr = '/';
               StringCat( lpBuffer, buf );
               break;
            
             default:
               StringCat(lpBuffer,pRecord->data);
               break;            

          }

          StringCat(lpBuffer,"\n");

          if ( SendTextToFile (fp, lpBuffer, &thePara, table) != TRUE ) {
            MemFree(lpBuffer);
            return FALSE;
          }
      }
  }

     /* Second run to print unknown Elhill codes */
     for ( pRecord = mep->recs;
           pRecord != NULL;
           pRecord = pRecord->next ) {

       exist = 0;
       for( i = 0; i < MAX_ELHILL; i++ ) {
         if( pRecord->code == ElhillOrder[i] ) {
           exist = 1;
           break;
         }
       }
       if( exist ) continue;
       if ( pRecord->abbr != NULL ) {
         /* This record has record type abbreviation */
         sprintf (
           lpBuffer,
           "%-4s-\t",
           pRecord->abbr
         );
       } else {
         /* This record has no record type abbreviation, use number */
         sprintf (
           lpBuffer,
           "%-4d-\t",
           pRecord->code
         );
       }

       StringCat(lpBuffer,pRecord->data);
       StringCat(lpBuffer,"\n");

       if ( SendTextToFile (fp, lpBuffer, &thePara, table) != TRUE ) {
         MemFree(lpBuffer);
         return FALSE;
       }
     }

    /* Generate SO field */
    strcpy( lpBuffer,"SO  -\t");
    strcat( lpBuffer, TA );
    strcat( lpBuffer," " );
    strcat( lpBuffer, DP );
    strcat( lpBuffer,";" );
    strcat( lpBuffer, VI );
    if ( *IP != '\0' ) {
      strcat( lpBuffer,"(");
      strcat( lpBuffer, IP );
      strcat( lpBuffer,")" );
    }
    strcat( lpBuffer, ":" );
    strcat( lpBuffer, PG );
    strcat(lpBuffer,"\n");

    if ( SendTextToFile (fp, lpBuffer, &thePara, table) != TRUE ) 
      bResult = FALSE;

    MemFree(lpBuffer);
    return bResult;
}                               /* MedlarsEntryToDataFile() */

/***********************************************************************/
/*.doc MedlarsEntryToDocFile (external) */
/*+
   Function send MEDLARS entry to I/O stream
-*/
/***********************************************************************/
NLM_EXTERN Boolean
/*FCN*/MedlarsEntryToDocFile (
  MedlarsEntryPtr mep,
  FILE *fp
){
    if ( mep == NULL || fp == NULL ) {
        return FALSE;
    }
    return TRUE;
}                               /* MedlarsEntryToDocFile() */

/***********************************************************************/
/*.doc MedlarsEntryToAbsFile (external) */
/*+
   Function send MEDLARS entry to I/O stream
-*/
/***********************************************************************/
NLM_EXTERN Boolean
/*FCN*/MedlarsEntryToAbsFile (
  MedlarsEntryPtr mep,
  FILE *fp
){
    if ( mep == NULL || fp == NULL ) {
        return FALSE;
    }
    return TRUE;
}                               /* MedlarsEntryToAbsFile() */

