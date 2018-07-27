/*************************************************************************
* gbfeat.h:
*
* $Log: gbfeat.h,v $
* Revision 6.2  2000/02/02 21:03:12  tatiana
* CkNumberType() added
*
* Revision 6.1  1998/02/10 17:00:25  tatiana
* GBQualValidToAdd(0 added
*
* Revision 6.0  1997/08/25 18:05:57  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/06/19 18:37:58  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.0  1996/05/28 13:23:23  ostell
* Set to revision 5.0
*
 * Revision 4.1  1995/08/15  22:05:52  tatiana
 * additional check for citation
 *
 * Revision 1.6  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*                                                              10-11-93
*************************************************************************/
#ifndef _GBFEAT_
#define _GBFEAT_

#include <stdio.h>
#include <ncbi.h>
#include <seqport.h>
#include <sequtil.h>


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

/*************************************************************************/

NLM_EXTERN Int2 GBQualNameValid PROTO((CharPtr qual));
/*  -- return index of the ParFlat_GBQual_names array if it is a valid
       qualifier (ignore case), qual; otherwise return (-1)
                                                                10-12-93  */

NLM_EXTERN Int2 GBQualSplit PROTO((CharPtr qual));

NLM_EXTERN Int2 GBFeatKeyNameValid PROTO((CharPtr PNTR keystr, Boolean error_msgs));
/* return "index" of the ParFlat_GBFeat array if it is a valid feature key (
  ignore case), keystr; otherwise, return (-1), UnknownFeatKey
                                                                  
                                                                  10-11-93 */
                                                                  
/*   --  GB_ERR returns values */

#define GB_FEAT_ERR_NONE 0
#define GB_FEAT_ERR_SILENT 1
#define GB_FEAT_ERR_REPAIRABLE 2
#define GB_FEAT_ERR_DROP 3 

NLM_EXTERN Boolean GBQualValidToAdd PROTO((Int2 keyindx, CharPtr curr));

NLM_EXTERN int GBFeatKeyQualValid PROTO((ValNodePtr cit, Int2 keyindx, GBQualPtr PNTR gbqp, Boolean error_msgs, Boolean perform_corrections));

NLM_EXTERN int SplitMultiValQual PROTO((GBQualPtr PNTR gbqp, 
     Boolean error_msgs, Boolean perform_corrections));

NLM_EXTERN int GBQualSemanticValid PROTO((GBQualPtr PNTR gbqp, Boolean error_msgs, Boolean perform_corrections));
/* -- returns GB_ERR value
  -- routine also drops out any unknown qualifier, if 'perform corrections' is set */

/* The following routines will return a GB_ERR code  */

NLM_EXTERN int CkQualPosSeqaa PROTO((GBQualPtr PNTR head_gbqp, GBQualPtr gbqp, GBQualPtr preq,
   Boolean error_msgs, Boolean perform_corrections, CharPtr aa, CharPtr eptr));

NLM_EXTERN int CkQualPosaa PROTO((GBQualPtr PNTR gbqp,  GBQualPtr curq, GBQualPtr preq, Boolean error_msgs, Boolean perform_corrections));

NLM_EXTERN int CkQualNote PROTO((GBQualPtr PNTR gbqp,  GBQualPtr curq, GBQualPtr preq, Boolean error_msgs, Boolean perform_corrections));

NLM_EXTERN void ConvertEmbedQual PROTO((CharPtr value));

NLM_EXTERN int CkQualText PROTO((GBQualPtr PNTR gbqp,  GBQualPtr curq, GBQualPtr preq, Boolean PNTR has_embedded, Boolean from_note, Boolean error_msgs, Boolean perform_corrections));

NLM_EXTERN CharPtr ScanEmbedQual PROTO((CharPtr value));
/* -- retun NULL if no embedded qualifiers found; otherwise, return the
      embedded qualifier. */
NLM_EXTERN int CkQualSeqaa PROTO((GBQualPtr PNTR gbqp,  GBQualPtr curq, GBQualPtr preq, Boolean error_msgs, Boolean perform_corrections));

NLM_EXTERN int CkQualMatchToken PROTO((GBQualPtr PNTR gbqp,  GBQualPtr curq, GBQualPtr preq, Boolean error_msgs, Boolean perform_corrections, CharPtr array_string[],
                                                         Int2 totalstr));

NLM_EXTERN int CkQualEcnum PROTO((GBQualPtr PNTR gbqp,  GBQualPtr curq, GBQualPtr preq, Boolean error_msgs, Boolean perform_corrections));

NLM_EXTERN int CkQualSite PROTO((GBQualPtr PNTR gbqp,  GBQualPtr curq, GBQualPtr preq, Boolean error_msgs, Boolean perform_corrections));

NLM_EXTERN int CkQualTokenType PROTO((GBQualPtr PNTR gbqp,  GBQualPtr curq, GBQualPtr preq, Boolean error_msgs, Boolean perform_corrections, Uint1 type));

/*********************************************************************/

NLM_EXTERN CharPtr GBFeatErrSpec PROTO((Uint1 indx));
/*  -- return a err_specific string by a given index        */

NLM_EXTERN void DeleteGBQualFromList PROTO((GBQualPtr PNTR gbqp, GBQualPtr curq, GBQualPtr preq));
/* does list management to delete curq qual from gbqp head of list */


NLM_EXTERN CharPtr CkBracketType PROTO((CharPtr str));

NLM_EXTERN CharPtr CkLabelType PROTO((CharPtr str));

NLM_EXTERN CharPtr CkNumberType PROTO((CharPtr str));

NLM_EXTERN GBQualPtr tie_qual PROTO((GBQualPtr head, GBQualPtr next));

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
