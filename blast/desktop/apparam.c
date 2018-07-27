/*   apparam.c
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
* File Name:  apparam.c
*
* Author:  Sergei Egorov
*
* Version Creation Date:   9/23/94
*
* $Revision: 6.1 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#include <apparam.h>

CharPtr APArrayIndexKey (CharPtr key, Int4 i)
{
  static Char buf[41];
  if (key == NULL) 
    sprintf (buf,"_%ld", (long)i);
  else
    sprintf (buf,"%.30s_%ld", (char *)key, (long)i);  
  return buf;
}

Boolean APOpenSection (CharPtr file, CharPtr sect, APSectPtr sp)
{
  if (file == NULL || sect == NULL || sp == NULL) return FALSE;
  StringNCpy_0 (&sp->base[0], file, MAX_APS_BASE); sp->base[MAX_APS_BASE] = '\0';
  StringNCpy_0 (&sp->sect[0], sect, MAX_APS_SECT); sp->sect[MAX_APS_SECT] = '\0';
  return TRUE;
}

/* empty string or NULL does not work as default??? */
static CharPtr default_string = "@[#$*-=%^'#$(%^)&";
static CharPtr deleted_ss_string = "<deleted>";

#define APV_ABSENT  0
#define APV_DELETED 1
#define APV_FOUND   2


static Boolean ap_delete_section (APSectPtr sp)
{
  if (sp == NULL) return FALSE;
  return SetAppParam (&sp->base[0], &sp->sect[0], NULL, NULL); 
}

static Boolean ap_delete_key (APSectPtr sp, CharPtr key)
{
  Char buf [MAX_APS_VALUE+1];
  if (sp == NULL || key == NULL) return FALSE;
  /* don't delete if config file already has no (empty) value - JK */
  if (! GetAppParam (&sp->base[0], &sp->sect[0], key, "", buf, MAX_APS_VALUE)) {
    return TRUE;
  }
  return SetAppParam (&sp->base[0], &sp->sect[0], key, NULL); 
}

static Int2 ap_get_value (APSectPtr sp, CharPtr key, CharPtr PNTR pval)
{
  static Char val_buf [MAX_APS_VALUE+1];
  Int2 nchars;
  if (sp == NULL || key == NULL) return APV_ABSENT; /*???*/
  nchars = GetAppParam (&sp->base[0], &sp->sect[0], key, default_string, 
                        val_buf, MAX_APS_VALUE);  
  if (pval != NULL) *pval = val_buf;
  if (nchars == 0) {
    StringNCpy_0 (val_buf, default_string, MAX_APS_VALUE); /* JK */
  }
  if (StrCmp (val_buf, default_string) == 0)
    return APV_ABSENT; 
  else if (StrCmp (val_buf, deleted_ss_string) == 0)
    return APV_DELETED;
  else 
    return APV_FOUND;
}

static Boolean ap_set_value (APSectPtr sp, CharPtr key, CharPtr val)
{
  Char buf [MAX_APS_VALUE+1];
  if (sp == NULL || key == NULL || val == NULL) return FALSE;
  /* don't write if config file already contains proper value - JK */
  if (GetAppParam (&sp->base[0], &sp->sect[0], key, default_string, buf, MAX_APS_VALUE)) {
    if (StrCmp (buf, val) == 0) {
      return TRUE;
    }
  }
  return SetAppParam (&sp->base[0], &sp->sect[0], key, val); 
}

static CharPtr gen_ss_key (APSectPtr sp, Int2 i)
{
  static Char buf[41];
  if (sp == NULL) 
    sprintf (buf,"#%d", (int)i);
  else
    sprintf (buf,"%.30s#%d", (char *)(&sp->sect[0]), (int)i);  
  return buf;
}

static Boolean fill_ssp (APSectPtr ssp, APSectPtr sp, CharPtr sskey)
{
  if (ssp == NULL || sp == NULL || sskey == NULL) return FALSE; /*???*/
  StringNCpy_0 (&ssp->base[0], &sp->base[0], MAX_APS_BASE); ssp->base[MAX_APS_BASE] = '\0';
  StringNCpy_0 (&ssp->sect[0], sskey, MAX_APS_SECT); ssp->sect[MAX_APS_SECT] = '\0';
  return TRUE;
}


/* subsections */

/* arbitrary limit to number of subsections */
/* (to ensure that app won't hang up if something isn't working) */
#define MAX_SS_INDEX 456 

Boolean APCreateSubsection (APSectPtr sp, CharPtr ss, APSectPtr ssp)
{
  Int2 i; CharPtr sskey; Int2 deleted_i = -1;
  if (sp == NULL || ss == NULL || ssp == NULL) return FALSE;
  for (i = 0; i < MAX_SS_INDEX; i++) {
    CharPtr name;
    sskey = gen_ss_key (sp, i);
    switch (ap_get_value (sp, sskey, &name)) {
      case APV_ABSENT:
        if (deleted_i >= 0) i = deleted_i;
        if (i >= MAX_SS_INDEX) return FALSE;
        sskey = gen_ss_key (sp, i);
        return (ap_set_value (sp, sskey, ss) && fill_ssp (ssp, sp, sskey));
      case APV_DELETED:
        deleted_i = i;
        break;
      case APV_FOUND:
        if (StrICmp (name, ss) == 0)
          return fill_ssp (ssp, sp, sskey);
        break;
      default: return FALSE; /*????*/
    }
  }
  Message(MSG_ERROR, "in APCreateSubsection");
  return FALSE; /*????*/
}


Boolean APLookupSubsection (APSectPtr sp, CharPtr ss, APSectPtr ssp)
{
  Int2 i; 
  if (sp == NULL || ss == NULL || ssp == NULL) return FALSE;
  for (i = 0; i < MAX_SS_INDEX; i++) {
    CharPtr name;
    CharPtr sskey = gen_ss_key (sp, i);
    switch (ap_get_value (sp, sskey, &name)) {
      case APV_ABSENT:
        return FALSE;
      case APV_DELETED:
        break;
      case APV_FOUND:
        if (StrICmp (name, ss) == 0)
          return fill_ssp (ssp, sp, sskey);
        break;
      default: return FALSE; /*????*/
    }
  }
  Message(MSG_ERROR, "in APLookupSubsection");
  return FALSE; /*????*/
}


Boolean APDeleteSubsection (APSectPtr sp, CharPtr ss)
{
  Int2 i; APSect sbs;
  if (sp == NULL || ss == NULL) return FALSE;
  for (i = 0; i < MAX_SS_INDEX; i++) {
    CharPtr name;
    CharPtr sskey = gen_ss_key (sp, i);
    switch (ap_get_value (sp, sskey, &name)) {
      case APV_ABSENT:
        return FALSE;
      case APV_DELETED:
        break;
      case APV_FOUND:
        if (StrCmp (name, ss) == 0)
          return (ap_set_value (sp, sskey, deleted_ss_string)
                  && fill_ssp (&sbs, sp, sskey)
                  && ap_delete_section (&sbs));
        break;
      default: return FALSE; /*????*/
    }
  }
  Message(MSG_ERROR, "in APDeleteSubsection");
  return FALSE; /*????*/
}

void APForEachSubsection (APSectPtr sp, void (*proc) (CharPtr ss))
{
  Int2 i; 
  if (sp == NULL || proc == NULL) return;
  for (i = 0; i < MAX_SS_INDEX; i++) {
    CharPtr name;
    CharPtr sskey = gen_ss_key (sp, i);
    switch (ap_get_value (sp, sskey, &name)) {
      case APV_ABSENT:
        return;
      case APV_DELETED:
        break;
      case APV_FOUND:
        proc (name);
    }
  }
  Message(MSG_ERROR, "in APForEachParamSubsection");
}


/* special font dir hacks ??? */

Boolean APFindEntry (APSectPtr sp, CharPtr ss, Int2Ptr pindex)
{
  Int2 i; 
  if (sp == NULL || ss == NULL) return FALSE;
  for (i = 0; i < MAX_SS_INDEX; i++) {
    CharPtr name;
    CharPtr sskey = gen_ss_key (sp, i);
    switch (ap_get_value (sp, sskey, &name)) {
      case APV_ABSENT:
        return FALSE;
      case APV_DELETED:
        break;
      case APV_FOUND:
        if (StrICmp (name, ss) == 0) {
          if (pindex != NULL) *pindex = i;
          return TRUE;
        }
        break;
      default: return FALSE; /*????*/
    }
  }
  Message(MSG_ERROR, "in APFindEntry");
  return FALSE; /*????*/
}

Boolean APEntrySubsection (APSectPtr sp, Int2 index, APSectPtr ssp)
{
  CharPtr sskey = gen_ss_key (sp, index);
  CharPtr name;
  if (ap_get_value (sp, sskey, &name) == APV_FOUND)
    return fill_ssp (ssp, sp, sskey);
  else
    return FALSE;
}


static Char format_buf [120];

/* basic operations */

void APForEachKey (APSectPtr sp, void (*proc) (CharPtr key))
{
  return; /* NYI */
}

Boolean APDeleteSection (APSectPtr sp)
{
  return ap_delete_section (sp);
}

Boolean APFindKey (APSectPtr sp, CharPtr key)
{
  if (sp == NULL  || key == NULL) return FALSE;
  return (ap_get_value (sp, key, NULL) == APV_FOUND);
}

Boolean APDeleteKey (APSectPtr sp, CharPtr key)
{
  return ap_delete_key (sp, key);
}

Boolean APGetKeyValue (APSectPtr sp, CharPtr key, CharPtr PNTR pval)
{
  if (sp == NULL  || key == NULL || pval == NULL) return FALSE;
  return (ap_get_value (sp, key, pval) == APV_FOUND); 
}

Boolean APSetKeyValue (APSectPtr sp, CharPtr key, CharPtr val)
{
  return ap_set_value (sp, key, val);
}


/* parameter read/write */

Boolean APArchiveBoolean (Boolean writep, APSectPtr sp, CharPtr key, BoolPtr pb)
{
  CharPtr val;
  if (writep) {
    if (pb == NULL) return FALSE;
    return ap_set_value (sp, key, *pb ? "TRUE" : "FALSE");
  }
  if (ap_get_value (sp, key, &val) == APV_FOUND) {
    if (pb != NULL) *pb = (StrICmp(val, "FALSE") == 0) ? FALSE : TRUE;
    return TRUE;
  }
  return FALSE; 
}

Boolean APArchiveEnum (Boolean writep, APSectPtr sp, CharPtr key, UIEnumPtr pe)
{
  CharPtr val;
  if (writep) {
    if (pe == NULL) return FALSE;
    sprintf (format_buf, "%ld", (long) *pe);
    return ap_set_value (sp, key, format_buf);
  }
  if (ap_get_value (sp, key, &val) == APV_FOUND) {
    long l;
    if (sscanf (val, "%ld", &l) != 1) return FALSE;
    if (pe != NULL) *pe = (UIEnum)l;
    return TRUE;
  }
  return FALSE; 
}

Boolean APArchiveInt2 (Boolean writep, APSectPtr sp, CharPtr key, Int2Ptr pi2)
{
  CharPtr val;
  if (writep) {
    if (pi2 == NULL) return FALSE;
    sprintf (format_buf, "%d", (int) *pi2);
    return ap_set_value (sp, key, format_buf);
  }
  if (ap_get_value (sp, key, &val) == APV_FOUND) {
    int i;
    if (sscanf (val, "%d", &i) != 1) return FALSE;
    if (pi2 != NULL) *pi2 = (Int2)i;
    return TRUE;
  }
  return FALSE; 
}

Boolean APArchiveUint2 (Boolean writep, APSectPtr sp, CharPtr key, Uint2Ptr pu2)
{
  CharPtr val;
  if (writep) {
    if (pu2 == NULL) return FALSE;
    sprintf (format_buf, "%u", (unsigned int) *pu2);
    return ap_set_value (sp, key, format_buf);
  }
  if (ap_get_value (sp, key, &val) == APV_FOUND) {
    unsigned int u;
    if (sscanf (val, "%u", &u) != 1) return FALSE;
    if (pu2 != NULL) *pu2 = (Uint2)u;
    return TRUE;
  }
  return FALSE; 
}

Boolean APArchiveInt4 (Boolean writep, APSectPtr sp, CharPtr key, Int4Ptr pi4)
{
  CharPtr val;
  if (writep) {
    if (pi4 == NULL) return FALSE;
    sprintf (format_buf, "%ld", (long) *pi4);
    return ap_set_value (sp, key, format_buf);
  }
  if (ap_get_value (sp, key, &val) == APV_FOUND) {
    long l;
    if (sscanf (val, "%ld", &l) != 1) return FALSE;
    if (pi4 != NULL) *pi4 = (Int4)l;
    return TRUE;
  }
  return FALSE; 
}

Boolean APArchiveUint4 (Boolean writep, APSectPtr sp, CharPtr key, Uint4Ptr pu4)
{
  CharPtr val;
  if (writep) {
    if (pu4 == NULL) return FALSE;
    sprintf (format_buf, "%lu", (unsigned long) *pu4);
    return ap_set_value (sp, key, format_buf);
  }
  if (ap_get_value (sp, key, &val) == APV_FOUND) {
    unsigned long lu;
    if (sscanf (val, "%lu", &lu) != 1) return FALSE;
    if (pu4 != NULL) *pu4 = (Uint4)lu;
    return TRUE;
  }
  return FALSE; 
}

Boolean APArchiveString (Boolean writep, APSectPtr sp, CharPtr key, CharPtr psb, Int2 bsz)
{
  CharPtr val;
  if (writep) return ap_set_value (sp, key, psb);
  if (ap_get_value (sp, key, &val) == APV_FOUND) {
    if (psb != NULL && bsz >= 0) StringNCpy_0 (psb, val, bsz);
    return TRUE;
  }
  return FALSE; 
}

