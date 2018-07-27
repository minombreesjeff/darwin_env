/* $Id: cddserver.c,v 1.44 2004/04/01 13:43:05 lavr Exp $
*===========================================================================
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
* File Name:  cddserver.c
*
* Author:  Aron Marchler-Bauer
*
* Initial Version Creation Date: 2/10/2000
*
* $Revision: 1.44 $
*
* File Description:
*         CD WWW-Server, Cd summary pages and alignments directly from the
*         CD ASN.1 data
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cddserver.c,v $
* Revision 1.44  2004/04/01 13:43:05  lavr
* Spell "occurred", "occurrence", and "occurring"
*
* Revision 1.43  2004/02/17 16:32:54  bauer
* added citation, remove books-links, ancestors etc. from published CDs
*
* Revision 1.42  2003/11/19 14:37:52  bauer
* more consistent use of PSSM-IDs
*
* Revision 1.41  2003/10/07 21:21:09  bauer
* initial changes to support drawing of hierarchies
*
* Revision 1.40  2003/01/10 14:47:46  bauer
* fixed problem with CDART connectivity
*
* Revision 1.39  2002/12/24 18:21:41  bauer
* changes for v1.60
*
* Revision 1.38  2002/11/25 19:01:20  bauer
* retrieve query sequence from BLAST queue in alignment formatting
*
* Revision 1.37  2002/10/09 20:31:13  bauer
* increased max. number of CDART neighbors, and other additions
*
* Revision 1.36  2002/08/16 19:52:37  bauer
* switched to Ben's CddSrvGetStyle2
*
* Revision 1.35  2002/08/06 12:55:14  bauer
* fixes to accomodate COGs
*
* Revision 1.34  2002/07/31 03:23:08  bauer
* evidence viewer now shows a single structure
*
* Revision 1.33  2002/07/05 21:10:54  bauer
* added CDtrack access functionality
*
* Revision 1.32  2002/06/25 21:24:48  bauer
* fix stored sequence format for CDDAlignView
*
* Revision 1.31  2002/06/12 15:20:54  bauer
* 6/11/02 update
*
* Revision 1.30  2002/05/23 20:18:24  bauer
* intermediate version of CDDserver
*
* Revision 1.29  2002/04/25 14:31:22  bauer
* layout changes to the CDD server
*
* Revision 1.28  2002/03/07 19:12:14  bauer
* major revisions to cgi-bins and the CD-dumper
*
* Revision 1.27  2002/01/04 19:50:57  bauer
* initial changes to deal with PSSM-Ids
*
* Revision 1.26  2001/11/13 19:46:53  bauer
* Biostrucs now read from file, support for new mmdbsrv
*
* Revision 1.25  2001/10/01 18:14:24  bauer
* minor changes in logic
*
* Revision 1.24  2001/06/20 20:50:50  bauer
* fixed a problem with gi's for PDB-derived sequences
*
* Revision 1.23  2001/06/19 16:02:37  bauer
* fixed URL for linking to SMART by accession
*
* Revision 1.22  2001/05/31 22:04:45  bauer
* changes to accomodate new type of Smart accessions
*
* Revision 1.21  2001/05/23 21:19:02  bauer
* fix a problem with displaying CDs without consensus
*
* Revision 1.20  2001/03/07 20:29:19  bauer
* cddserver.c
*
* Revision 1.19  2001/03/07 16:31:49  bauer
* bullet-proof against service failures?
*
* Revision 1.18  2001/03/02 23:35:46  bauer
* FASTA format now done by CddAlignView
*
* Revision 1.17  2001/02/28 19:45:11  bauer
* changed options menu
*
* Revision 1.16  2001/02/16 03:11:38  bauer
* more support for CddAlignView
*
* Revision 1.15  2001/02/14 17:10:41  bauer
* added bit-threshold to CddAlignView call
*
* Revision 1.14  2001/02/13 23:11:57  bauer
* integrated CddAlignView
*
* Revision 1.13  2001/02/10 01:57:26  bauer
* use of CddAlignView library conditional
*
* Revision 1.12  2001/02/06 22:54:52  bauer
* make Paul's CddAlignView the default
*
* Revision 1.11  2001/02/05 23:02:11  bauer
* changes for consensus CDs
*
* Revision 1.10  2001/01/24 03:08:47  bauer
* changes for consensus CDs
*
* Revision 1.9  2001/01/11 21:50:45  bauer
* fixed typo
*
* Revision 1.8  2000/12/08 20:17:32  bauer
* changed status text for oAsIs CDs
*
* Revision 1.7  2000/08/03 22:02:23  bauer
* added support for 3D-structure link highlighting
*
* Revision 1.6  2000/08/01 21:24:25  bauer
* changed [DC] label to [CD]
*
* Revision 1.5  2000/07/28 17:58:50  bauer
* added DomainComposition hotlinks in CD summary pages
*
* Revision 1.4  2000/07/24 15:44:50  bauer
* Changed Get Cn3D message for query-added alignment displays
*
* Revision 1.3  2000/07/20 17:50:44  bauer
* fixed bug in manual sequence selection
*
* Revision 1.2  2000/07/19 19:23:12  bauer
* added modification logging
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <stdio.h>
#include <lsqfetch.h>
#include <netentr.h>
#include <www.h>
#include <sys/resource.h>
#include <asn.h>
#include <accpubseq.h>
#include <accutils.h>
#include <mmdbapi.h>
#include <mmdbapi1.h>
#include <objmime.h>
#include <strimprt.h>
#include <objcdd.h>
#include "cddsrv.h"
#include <pgppop.h> 
#include <alignmgr.h>
#include <taxutil.h>
#include <txcommon.h>
#include <cdd.h>
#include "cddalignview.h"
#include "cddutil.h"
#include "dart.h"
#include <objcn3d.h>
#include <cdtrkapi.h>
#include <qblastnet.h>
#include <gifgen.h>


#undef DEBUG
#undef NOCN3D4
#define USE_CDTRK
#define DRAW_TREES
  
typedef struct _private_tree_node_ {
  CdTreeNodePtr      pcdtree;
  Int4               x, y, width;
  Int4               iNChildren;
  Boolean            bIsCurrent;
  struct _private_tree_node_ *parent;
  struct _private_tree_node_ **children;
  struct _private_tree_node_ *next;
} PrivateTreeNode, *PrivateTreeNodePtr;

unsigned iDartFam[DARTFAMILYNUM];
Int4     iDartFamNum = 0;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* report Errors in processing and exit immediately                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CddHtmlError(CharPtr cErrTxt) 
{
  CDDSrvHead(stdout,"NCBI CDDsrv Error");
  printf("<h2>CDDsrv Error:</h2>\n");
  printf("<h3>%s</h3>\n",cErrTxt);
  CDDSrvFoot(stdout);
  exit(1);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* read parameters from configuration file                                   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean CddGetParams()
{
  URLBase[0] = URLcgi[0] = ENTREZurl[0] = DOCSUMurl[0] = MAILto[0] = '\0';
  MMDBpath[0] = gunzip[0] = CTBcgi[0] = '\0';

  GetAppParam("cdd", "CDDSRV", "URLBase", "", URLBase, PATH_MAX);
  if (URLBase[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no URLBase...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "URLcgi", "", URLcgi, PATH_MAX);
  if (URLcgi[0] == '\0') {
                ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no URLcgi...\n");
                return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CTBcgi", "", CTBcgi, PATH_MAX);
  if (CTBcgi[0] == '\0') {
                Nlm_StrCpy(CTBcgi, URLcgi);
  }
  GetAppParam("cdd", "CDDSRV", "PFAMcgiUS", "", PFAMcgiUS, PATH_MAX);
  if (PFAMcgiUS[0] == '\0') {
                ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no PFAMcgiUS...\n");
                return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "PFAMcgiUK", "", PFAMcgiUK, PATH_MAX);
  if (PFAMcgiUK[0] == '\0') {
                ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no PFAMcgiUK...\n");
                return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "SMARTcgi", "", SMARTcgi, PATH_MAX);
  if (SMARTcgi[0] == '\0') {
                ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no SMARTcgi...\n");
                return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "SMACCcgi", "", SMACCcgi, PATH_MAX);
  if (SMACCcgi[0] == '\0') {
                ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no SMACCcgi...\n");
                return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "COGcgi", "", COGcgi, PATH_MAX);
  if (COGcgi[0] == '\0') {
                ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no COGcgi...\n");
                return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "KOGcgi", "", KOGcgi, PATH_MAX);
  if (KOGcgi[0] == '\0') {
                ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no KOGcgi...\n");
                return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "ENTREZurl", "", ENTREZurl, PATH_MAX);
  if (ENTREZurl[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no ENTREZurl...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "DOCSUMurl", "", DOCSUMurl, PATH_MAX);
  if (DOCSUMurl[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no DOCSUMurl...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "Gunzip", "", gunzip, (size_t) 256*(sizeof(Char)));
  if (gunzip[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no Gunzip...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "Database", "", MMDBpath, PATH_MAX);
  if (MMDBpath[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"MMDB config file\nMMDBSRV section has no Database...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "MAILto", "", MAILto, PATH_MAX);
  if (MAILto[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no MAILto...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "DATApath", "", DATApath, PATH_MAX);
  if (DATApath[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no VAST Data path...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDatabase", "", CDDdpath, PATH_MAX);
  if (CDDdpath[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no CDD data path...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CVDatabase", "", CDDvpath, PATH_MAX);
  if (CDDvpath[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no CDD/VAST data path...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDextens", "", CDDextens, PATH_MAX);
  if (CDDextens[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no CDD file name extension...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "RAWextens", "", RAWextens, PATH_MAX);
  if (RAWextens[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no RAW file name extension...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDdescr", "", CDDdescr, PATH_MAX);
  if (CDDdescr[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no description file name extension...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "Database", "", database, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no VAST data path...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "PUBcgi", "", PUBcgi, PATH_MAX);
  if (PUBcgi[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no PUBcgi ...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "TAXcgi", "", TAXcgi, PATH_MAX);
  if (TAXcgi[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no TAXcgi ...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDPrefix", "", CDDPrefix, PATH_MAX);
  if (CDDPrefix[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no CDDPrefix...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDPost_O", "", CDDPost_O, PATH_MAX);
  if (CDDPost_O[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no CDDPost_O...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDPost_C", "", CDDPost_C, PATH_MAX);
  if (CDDPost_C[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no CDDPost_C...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDefault", "", CDDefault, PATH_MAX);
  if (CDDefault[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no CDDefault...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDdbtype", "", CDDdbtype, PATH_MAX);
  if (CDDdbtype[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no CDDdbtype...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "ODBCINI", "", ODBCINI, PATH_MAX);
  if (ODBCINI[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no ODBCINI...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "DARTUSER", "", DARTUSER, PATH_MAX);
  if (DARTUSER[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no DARTUSER...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "DARTPASS", "", DARTPASS, PATH_MAX);
  if (DARTPASS[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no DARTPASS...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDlocat", "", CDDlocat, PATH_MAX);
  if (CDDlocat[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no CDDlocat...\n");
    return FALSE;
  }
#ifdef USE_CDTRK
  GetAppParam("cdd", "CDDSRV", "CDTRKDBS", "", CDTRKDBS, PATH_MAX);
  if (CDTRKDBS[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no CDTRKDBS...\n");
    return FALSE;
  }
#endif

  return TRUE;
}                                                       /* end CddGetParams */

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* return a common style dictionary for Cn3D 4.0                             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Cn3dStyleDictionaryPtr CddSrvGetStyle(Boolean bFeature)
{
  Cn3dStyleDictionaryPtr    csdp;
  Cn3dStyleSettingsPtr      cssp;
  Cn3dBackboneStylePtr      cbsp;
  Cn3dGeneralStylePtr       cgsp;
  Cn3dColorPtr              ccp;
  Cn3dBackboneLabelStylePtr cblsp;
  Cn3dStyleTableItemPtr     cstip;

  csdp = Cn3dStyleDictionaryNew();
    cssp = Cn3dStyleSettingsNew();
      cssp->next = NULL;
      cssp->name = NULL;
      cbsp = Cn3dBackboneStyleNew();
        cbsp->type = Cn3d_backbone_type_trace;
        cbsp->style = Cn3d_drawing_style_tubes;
        cbsp->color_scheme = Cn3d_color_scheme_weighted_variety;
        ccp = Cn3dColorNew();
          ccp->scale_factor = 10000;
          ccp->red = 5000;
          ccp->green = 5000;
          ccp->blue = 5000;
          ccp->alpha = 10000;
        cbsp->user_color = ccp;
      cssp->protein_backbone = cbsp;
      cbsp = Cn3dBackboneStyleNew();
        cbsp->type = Cn3d_backbone_type_trace;
        cbsp->style = Cn3d_drawing_style_tubes;
        cbsp->color_scheme = Cn3d_color_scheme_molecule;
        ccp = Cn3dColorNew();
          ccp->scale_factor = 10000;
          ccp->red = 5000;
          ccp->green = 5000;
          ccp->blue = 5000;
          ccp->alpha = 10000;
        cbsp->user_color = ccp;
      cssp->nucleotide_backbone = cbsp;
        cgsp = Cn3dGeneralStyleNew();
	cgsp->is_on = TRUE;
	cgsp->style = Cn3d_drawing_style_wire;
	cgsp->color_scheme = Cn3d_color_scheme_weighted_variety;
        ccp = Cn3dColorNew();
          ccp->scale_factor = 10000;
          ccp->red = 5000;
          ccp->green = 5000;
          ccp->blue = 5000;
          ccp->alpha = 10000;
	cgsp->user_color = ccp;
      cssp->protein_sidechains = cgsp;
        cgsp = Cn3dGeneralStyleNew();
	cgsp->is_on = TRUE;
	cgsp->style = Cn3d_drawing_style_wire;
	cgsp->color_scheme = Cn3d_color_scheme_molecule;
        ccp = Cn3dColorNew();
          ccp->scale_factor = 10000;
          ccp->red = 5000;
          ccp->green = 5000;
          ccp->blue = 5000;
          ccp->alpha = 10000;
	cgsp->user_color = ccp;
      cssp->nucleotide_sidechains = cgsp;
        cgsp = Cn3dGeneralStyleNew();
	cgsp->is_on = TRUE;
	cgsp->style = Cn3d_drawing_style_ball_and_stick;
	cgsp->color_scheme = Cn3d_color_scheme_element;
        ccp = Cn3dColorNew();
          ccp->scale_factor = 10000;
          ccp->red = 5000;
          ccp->green = 5000;
          ccp->blue = 5000;
          ccp->alpha = 10000;
	cgsp->user_color = ccp;
       cssp->heterogens = cgsp;
        cgsp = Cn3dGeneralStyleNew();
	cgsp->is_on = FALSE;
	cgsp->style = Cn3d_drawing_style_ball_and_stick;
	cgsp->color_scheme = Cn3d_color_scheme_element;
        ccp = Cn3dColorNew();
          ccp->scale_factor = 10000;
          ccp->red = 5000;
          ccp->green = 5000;
          ccp->blue = 5000;
          ccp->alpha = 10000;
	cgsp->user_color = ccp;
      cssp->solvents = cgsp;
        cgsp = Cn3dGeneralStyleNew();
	cgsp->is_on = TRUE;
	cgsp->style = Cn3d_drawing_style_tubes;
	cgsp->color_scheme = Cn3d_color_scheme_user_select;
        ccp = Cn3dColorNew();
          ccp->scale_factor = 10000;
          ccp->red = 9000;
          ccp->green = 9000;
          ccp->blue = 10000;
          ccp->alpha = 10000;
	cgsp->user_color = ccp;
       cssp->connections = cgsp;
        cgsp = Cn3dGeneralStyleNew();
	cgsp->is_on = FALSE;
	cgsp->style = Cn3d_drawing_style_with_arrows;
	cgsp->color_scheme = Cn3d_color_scheme_object;
        ccp = Cn3dColorNew();
          ccp->scale_factor = 10000;
          ccp->red = 5000;
          ccp->green = 5000;
          ccp->blue = 5000;
          ccp->alpha = 10000;
	cgsp->user_color = ccp;
      cssp->helix_objects = cgsp;
        cgsp = Cn3dGeneralStyleNew();
	cgsp->is_on = FALSE;
	cgsp->style = Cn3d_drawing_style_with_arrows;
	cgsp->color_scheme = Cn3d_color_scheme_object;
        ccp = Cn3dColorNew();
          ccp->scale_factor = 10000;
          ccp->red = 5000;
          ccp->green = 5000;
          ccp->blue = 5000;
          ccp->alpha = 10000;
	cgsp->user_color = ccp;
      cssp->strand_objects = cgsp;
      cssp->virtual_disulfides_on = TRUE;
        ccp = Cn3dColorNew();
	ccp->scale_factor = 10000;
	ccp->red = 9300;
	ccp->green = 5500;
	ccp->blue = 500;
	ccp->alpha = 10000;
      cssp->virtual_disulfide_color = ccp;
      cssp->hydrogens_on = FALSE;
        ccp = Cn3dColorNew();
	ccp->scale_factor = 10000;
	ccp->red = 0;
	ccp->green = 0;
	ccp->blue = 0;
	ccp->alpha = 10000;
      cssp->background_color = ccp;
      cssp->scale_factor = 10000;
      cssp->space_fill_proportion = 10000;
      cssp->ball_radius = 4000;
      cssp->stick_radius = 2000;
      cssp->tube_radius = 3000;
      cssp->tube_worm_radius = 3000;
      cssp->helix_radius = 18000;
      cssp->strand_width = 20000;
      cssp->strand_thickness = 5000;
        cblsp = Cn3dBackboneLabelStyleNew();
	cblsp->spacing = 0;
	cblsp->type = Cn3d_backbone_label_style_type_three_letter;
	cblsp->number = Cn3d_backbone_label_style_number_sequential;
	cblsp->termini = FALSE;
	cblsp->white = TRUE;
      cssp->protein_labels = cblsp;
        cblsp = Cn3dBackboneLabelStyleNew();
	cblsp->spacing = 0;
	cblsp->type = Cn3d_backbone_label_style_type_three_letter;
	cblsp->number = Cn3d_backbone_label_style_number_sequential;
	cblsp->termini = FALSE;
	cblsp->white = TRUE;
      cssp->nucleotide_labels =cblsp;
      cssp->ion_labels = TRUE;  
    csdp->global_style = cssp;
  if (bFeature) {
      cstip = Cn3dStyleTableItemNew();
      cstip->id = 1;
        cssp = Cn3dStyleSettingsNew();
        cssp->next = NULL;
        cssp->name = NULL;
          cbsp = Cn3dBackboneStyleNew();
          cbsp->type = Cn3d_backbone_type_trace;
          cbsp->style = Cn3d_drawing_style_tubes;
          cbsp->color_scheme = Cn3d_color_scheme_user_select;
            ccp = Cn3dColorNew();
            ccp->scale_factor = 10000;
            ccp->red = 0;
            ccp->green = 10000;
            ccp->blue = 0;
            ccp->alpha = 10000;
          cbsp->user_color = ccp;
        cssp->protein_backbone = cbsp;
          cbsp = Cn3dBackboneStyleNew();
          cbsp->type = Cn3d_backbone_type_complete;
          cbsp->style = Cn3d_drawing_style_ball_and_stick;
          cbsp->color_scheme = Cn3d_color_scheme_user_select;
            ccp = Cn3dColorNew();
            ccp->scale_factor = 10000;
            ccp->red = 0;
            ccp->green = 5019;
            ccp->blue = 10000;
            ccp->alpha = 10000;
          cbsp->user_color = ccp;
        cssp->nucleotide_backbone = cbsp;
          cgsp = Cn3dGeneralStyleNew();
	  cgsp->is_on = TRUE;
	  cgsp->style = Cn3d_drawing_style_tubes;
	  cgsp->color_scheme = Cn3d_color_scheme_user_select;
            ccp = Cn3dColorNew();
            ccp->scale_factor = 10000;
            ccp->red = 0;
            ccp->green = 10000;
            ccp->blue = 0;
            ccp->alpha = 10000;
	  cgsp->user_color = ccp;
        cssp->protein_sidechains = cgsp;
          cgsp = Cn3dGeneralStyleNew();
	  cgsp->is_on = TRUE;
	  cgsp->style = Cn3d_drawing_style_ball_and_stick;
	  cgsp->color_scheme = Cn3d_color_scheme_user_select;
            ccp = Cn3dColorNew();
            ccp->scale_factor = 10000;
            ccp->red = 10000;
            ccp->green = 5019;
            ccp->blue = 0;
            ccp->alpha = 10000;
	  cgsp->user_color = ccp;
        cssp->nucleotide_sidechains = cgsp;
          cgsp = Cn3dGeneralStyleNew();
	  cgsp->is_on = TRUE;
	  cgsp->style = Cn3d_drawing_style_space_fill;
	  cgsp->color_scheme = Cn3d_color_scheme_user_select;
            ccp = Cn3dColorNew();
            ccp->scale_factor = 10000;
            ccp->red = 10000;
            ccp->green = 5019;
            ccp->blue = 0;
            ccp->alpha = 10000;
	  cgsp->user_color = ccp;
        cssp->heterogens = cgsp;
          cgsp = Cn3dGeneralStyleNew();
	  cgsp->is_on = TRUE;
	  cgsp->style = Cn3d_drawing_style_ball_and_stick;
	  cgsp->color_scheme = Cn3d_color_scheme_user_select;
            ccp = Cn3dColorNew();
            ccp->scale_factor = 10000;
            ccp->red = 10000;
            ccp->green = 5019;
            ccp->blue = 0;
            ccp->alpha = 10000;
	  cgsp->user_color = ccp;
        cssp->solvents = cgsp;
          cgsp = Cn3dGeneralStyleNew();
	  cgsp->is_on = TRUE;
	  cgsp->style = Cn3d_drawing_style_tubes;
	  cgsp->color_scheme = Cn3d_color_scheme_user_select;
            ccp = Cn3dColorNew();
            ccp->scale_factor = 10000;
            ccp->red = 9000;
            ccp->green = 9000;
            ccp->blue = 10000;
            ccp->alpha = 10000;
	  cgsp->user_color = ccp;
        cssp->connections = cgsp;
          cgsp = Cn3dGeneralStyleNew();
	  cgsp->is_on = FALSE;
	  cgsp->style = Cn3d_drawing_style_with_arrows;
	  cgsp->color_scheme = Cn3d_color_scheme_object;
            ccp = Cn3dColorNew();
            ccp->scale_factor = 10000;
            ccp->red = 5000;
            ccp->green = 5000;
            ccp->blue = 5000;
            ccp->alpha = 10000;
	  cgsp->user_color = ccp;
        cssp->helix_objects = cgsp;
          cgsp = Cn3dGeneralStyleNew();
	  cgsp->is_on = FALSE;
	  cgsp->style = Cn3d_drawing_style_with_arrows;
	  cgsp->color_scheme = Cn3d_color_scheme_object;
            ccp = Cn3dColorNew();
            ccp->scale_factor = 10000;
            ccp->red = 5000;
            ccp->green = 5000;
            ccp->blue = 5000;
            ccp->alpha = 10000;
	  cgsp->user_color = ccp;
        cssp->strand_objects = cgsp;
        cssp->virtual_disulfides_on = TRUE;
          ccp = Cn3dColorNew();
	  ccp->scale_factor = 10000;
	  ccp->red = 9300;
	  ccp->green = 5500;
	  ccp->blue = 500;
	  ccp->alpha = 10000;
        cssp->virtual_disulfide_color = ccp;
        cssp->hydrogens_on = FALSE;
          ccp = Cn3dColorNew();
	  ccp->scale_factor = 10000;
	  ccp->red = 0;
	  ccp->green = 0;
	  ccp->blue = 0;
	  ccp->alpha = 10000;
        cssp->background_color = ccp;
        cssp->scale_factor = 10000;
        cssp->space_fill_proportion = 10000;
        cssp->ball_radius = 4000;
        cssp->stick_radius = 2000;
        cssp->tube_radius = 3000;
        cssp->tube_worm_radius = 3000;
        cssp->helix_radius = 18000;
        cssp->strand_width = 20000;
        cssp->strand_thickness = 5000;
          cblsp = Cn3dBackboneLabelStyleNew();
	  cblsp->spacing = 0;
	  cblsp->type = Cn3d_backbone_label_style_type_three_letter;
	  cblsp->number = Cn3d_backbone_label_style_number_sequential;
	  cblsp->termini = FALSE;
	  cblsp->white = TRUE;
        cssp->protein_labels = cblsp;
          cblsp = Cn3dBackboneLabelStyleNew();
	  cblsp->spacing = 0;
	  cblsp->type = Cn3d_backbone_label_style_type_three_letter;
	  cblsp->number = Cn3d_backbone_label_style_number_sequential;
	  cblsp->termini = FALSE;
	  cblsp->white = TRUE;
        cssp->nucleotide_labels =cblsp;
        cssp->ion_labels = TRUE;  
      cstip->style = cssp;    
      cstip->next = NULL;
    csdp->style_table = cstip;
  } else csdp->style_table = NULL;
  return(csdp);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* return a list of CD accessions which are "related" to the current CD      */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static ValNodePtr CddGetRelatedCDs(CddPtr pcdd, CharPtr thisacc, Int4 iPSSMid, Boolean *bIsArch)
{
  ValNodePtr    vnpHead = NULL;
  ValNodePtr    vnp, thisid;
  GlobalIdPtr   pGid;
  Dart_Connect *Connection;
  int           Size, i;
  unsigned      Gilist[DARTSIZELIMIT];
  char          Accession[DARTSIZELIMIT][30];
  Boolean       unique;
  CharPtr       cOutString;
  
  thisid = pcdd->siblings;
  while (thisid) {
    if (thisid->choice == CddId_gid) {
      pGid = thisid->data.ptrvalue;
      vnp = ValNodeCopyStr(&(vnpHead),0,(CharPtr) pGid->accession);
    }
    thisid = thisid->next;
  }

  cOutString = MemNew(PATH_MAX*sizeof(Char));
  sprintf(cOutString,"ODBCINI=%s",ODBCINI);
  putenv(cOutString);
  putenv("LD_LIBRARY_PATH=/opt/machine/merant/lib");
  Connection = Dart_Init2("CDart", DARTUSER, DARTPASS); 
  if (Connection) {
    Dart_CdFamily(Connection, iDartFam, DARTFAMILYNUM, &iDartFamNum);
    for (i=0;i<iDartFamNum;i++) {
      if (iDartFam[i] == iPSSMid) {
        *bIsArch = TRUE;
        break;      
      }    
    }
  }
  if (Connection) {
    if (iPSSMid > 0) {
      if (Dart_SameSim(Connection,iPSSMid,Gilist,DARTSIZELIMIT,&Size)) {
        for (i=0;i<Size;i++) {
          if (!Dart_CDGi2Acc(Connection,Gilist[i],Accession[i],30)) {
	    Accession[i][0] = '\0';
	  }
        }
      }
    } else {
      if (Dart_Related(Connection,thisacc,Gilist,DARTSIZELIMIT,&Size,NULL)) {
        for (i=0;i<Size;i++) {
          if (!Dart_CDGi2Acc(Connection,Gilist[i],Accession[i],30)) {
	    Accession[i][0] = '\0';
	  }
        }
      }
    }
  } else return(vnpHead);
  for (i=0;i<Size;i++) {
    if (Nlm_StrCmp(Accession[i],thisacc)!= 0) {
      vnp = vnpHead; unique = TRUE; while (vnp) {
        if (Nlm_StrCmp(Accession[i],(CharPtr) vnp->data.ptrvalue) == 0) {
	  unique = FALSE;
	  break;
	}
	vnp = vnp->next;
      }
      if (unique) {
        vnp = ValNodeCopyStr(&(vnpHead),0,(CharPtr) Accession[i]);
      }
    }
  }
  Dart_Fini(Connection);
  return vnpHead;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* allocate a new CddSum linked list entry                                   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static CddSumPtr CddSumNew()
{
  CddSumPtr   pcds;
  
  pcds=(CddSumPtr)MemNew(sizeof(CddSum));
  if (pcds==NULL) return(pcds);
  pcds->bIsPdb      = FALSE;
  pcds->bIsMaster   = FALSE;
  pcds->bIs3dRep    = FALSE;
  pcds->cPdbId[0]   = '\0';
  pcds->cChainId[0] = '\0';
  pcds->cPKBMDom[0] = '\0';
  pcds->cPKBDom[0]  = '\0';
  pcds->cDefLine[0] = '\0';
  pcds->iFsid       = -1;
  pcds->iFid        = -1;
  pcds->iMMDBId     = -1;
  pcds->iCddIdx     = -1;
  pcds->uid         = 0;
  pcds->sip         = NULL;
  pcds->next        = NULL;
  return pcds;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Free a CddSum linked list                                                 */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static CddSumPtr CddSumFree(CddSumPtr pcds)
{
  CddSumPtr    next;
  
  while (pcds) {
    next = pcds->next;
    Nlm_MemFree(pcds);
    pcds = next;
  }
  return NULL;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* adds a to a linked list of CddSumPtr, always returns the beginning of the */
/* list and always adds to the end of the list!!                             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static CddSumPtr CddSumLink(CddSumPtr PNTR head, CddSumPtr newnode)
{
  CddSumPtr     pcds;
 
  if (head == NULL) return newnode;
  pcds = *head;
  if (pcds != NULL) {
    while(pcds->next != NULL) pcds = pcds->next;
    pcds->next = newnode;
  } else *head = newnode;
  return *head;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Check whether a particular gi has occurred previously in the CddSum lnklst*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean CDSUniqueUid(Int4 uid, CddSumPtr pcds) {
  
  CddSumPtr    pcdsThis;

  if (!pcds) return TRUE;
  pcdsThis = pcds;
  while (pcdsThis) {
    if (pcdsThis->uid == uid) return FALSE;
    pcdsThis = pcdsThis->next;
  }
  return TRUE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Check whether a particular gi has occurred previously in the CddSum lnklst*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean UniqueSip(SeqIdPtr sip, CddSumPtr pcds) {
  
  CddSumPtr    pcdsThis;

  if (!pcds) return TRUE;
  pcdsThis = pcds;
  while (pcdsThis) {
    if (CddSameSip(pcdsThis->sip, sip)) return FALSE;
    pcdsThis = pcdsThis->next;
  }
  return TRUE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Check whether a particular gi has been supplied as cgi-argument           */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean UseThisGi(Int4 Gi, ValNodePtr pvnGis) 
{
  ValNodePtr    pvnGi;

  pvnGi = pvnGis;
  while (pvnGi) {
    if (Gi == pvnGi->data.intvalue) return TRUE;
    pvnGi = pvnGi->next;
  }
  return FALSE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* local version of tax1_join                                                */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Int4 Cdd_tax1_join(Int4 taxid1, Int4 taxid2)
{
  TXC_TreeNodePtr *txtnpp1;
  TXC_TreeNodePtr *txtnpp2;
  Int4             inLineage1, inLineage2;
  Int4             i, j;
  Int4             retid = -1;
  

  if (taxid1 == taxid2) return (taxid1);
  if (taxid1 <= 1 || taxid2 <=1) return(1);
  txtnpp1 = (TXC_TreeNodePtr *)txc_getLineage(taxid1,&inLineage1);
  txtnpp2 = (TXC_TreeNodePtr *)txc_getLineage(taxid2,&inLineage2);
  if (txtnpp1 && txtnpp2 && inLineage1 > 0 && inLineage2 > 0) {
    for (i=0;i<inLineage1;i++) {
      for (j=0;j<inLineage2;j++) {
        if (txtnpp1[i]->tax_id == txtnpp2[j]->tax_id) {
          retid = txtnpp1[i]->tax_id;
	  break;
	}
      }
      if (retid != -1) break;
    }
  }
  

  for (i=0;i<inLineage1;i++) MemFree(txtnpp1[i]);
  MemFree(txtnpp1);
  for (i=0;i<inLineage2;i++) MemFree(txtnpp2[i]);
  MemFree(txtnpp2);

  if (retid != -1) return (retid);
  return (1);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* return the common tax node for a selected subset of nodes                 */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Int4 CddGetTaxCommonNode(Int4 nTaxids, Int4Ptr iTaxids, CharPtr *txname)
{
  Int4          iTxid1;
  Int4          i;
  Taxon1DataPtr t1dp;
  OrgRefPtr     pOrgRef;

  if (nTaxids > 1) 
  iTxid1 = iTaxids[0];
  if (nTaxids > 1) for (i = 1;i<nTaxids;i++) {
    if (iTaxids[i] >= 1) {
      if (iTxid1 == -1) {
        iTxid1 = iTaxids[i];
      } else {
        iTxid1 = Cdd_tax1_join(iTaxids[i], iTxid1);
      }
    }
  }
  t1dp = (Taxon1DataPtr) tax1_getbyid(iTxid1);
  pOrgRef = t1dp->org;
  *txname = StringSave(pOrgRef->taxname);
  return(iTxid1);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Check whether a sequence falls into a taxonomy group                      */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean CddSameTaxLineage(SeqIdPtr sip, Int4 taxid, SeqEntryPtr sep, ValNodePtr txids)
{
  Int4            inLineage;
  TXC_TreeNodePtr *txtnpp;
  Boolean         bMatch = FALSE;
  CddSumPtr       pcdsThis;
  Int4            i,j;
  Int4            thistaxid;
  BioseqPtr       bsp;
  ValNodePtr      descr;
  BioSourcePtr    bsop;
  ObjectIdPtr     oidp;
  DbtagPtr        dbtp;
  OrgRefPtr       pOrgRef;
  ValNodePtr      vnp;

  bsp = (BioseqPtr) CddFindSip(sip,sep);
  if (bsp) {
    descr = bsp->descr;
    while (descr) {
      if (descr->choice == Seq_descr_source) {
        bsop = descr->data.ptrvalue;
        pOrgRef = bsop->org;
        if (pOrgRef->db) {
          dbtp = pOrgRef->db->data.ptrvalue;
          oidp = dbtp->tag;
          thistaxid = oidp->id;
        } else {
          thistaxid = tax_getIdByName(pOrgRef->taxname,NULL,0);
        }
        break;
      }
      descr = descr->next;
    }
    if (txids) {
      vnp = txids; while (vnp) {
        if (thistaxid == vnp->data.intvalue) return (TRUE);
	vnp = vnp->next;
      }
      return (FALSE);
    }
    
    txtnpp = (TXC_TreeNodePtr *)txc_getLineage(thistaxid,&inLineage);
    if (txtnpp && inLineage > 0) {
      for (i=0; i<inLineage;i++) if (txtnpp[i]->tax_id == taxid) {
        bMatch = TRUE; break;
      }
    }
  } else return(FALSE);
  return(bMatch);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Count how many sequences in the Cdd fall into a certain taxonomy group    */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CddCountTaxLineage(TXC_TreeNodePtr *tnpp, Int4 iLen, CddSumPtr pcds)
{
  Int4            inLineage;
  TXC_TreeNodePtr *txtnpp;
  Int4            iCount = 0;
  CddSumPtr       pcdsThis;
  Int4            i,j;
  Int4            thistaxid;
  BioseqPtr       bsp;
  ValNodePtr      descr;
  BioSourcePtr    bsop;
  ObjectIdPtr     oidp;
  DbtagPtr        dbtp;
  OrgRefPtr       pOrgRef;
 

  for (i=0;i<iLen;i++) tnpp[i]->flags = 0;
  pcdsThis = pcds; 
  while (pcdsThis) {
    bsp = BioseqLockById(pcdsThis->sip);
    if (bsp) {
      descr = bsp->descr; thistaxid = 0;
      while (descr) {
        if (descr->choice == Seq_descr_source) {
          bsop = descr->data.ptrvalue;
          pOrgRef = bsop->org;
	  if (NULL != pOrgRef) {
	    if (NULL != pOrgRef->taxname) {
	      pcdsThis->cTaxName = StringSave(pOrgRef->taxname);
	    }
	  }
          if (pOrgRef->db) {
            dbtp = pOrgRef->db->data.ptrvalue;
            oidp = dbtp->tag;
            thistaxid = oidp->id;
          } else {
            thistaxid = tax_getIdByName(pOrgRef->taxname,NULL,0);
          }
          break;
        }
        descr = descr->next;
      }
      pcdsThis->iTaxId = thistaxid;
      txtnpp = (TXC_TreeNodePtr*)txc_getLineage(thistaxid,&inLineage);
      if (txtnpp && inLineage > 0) {
        for (i=0; i<inLineage;i++) {
          for (j=0;j<iLen;j++) {
            if (txtnpp[i]->tax_id == tnpp[j]->tax_id) tnpp[j]->flags++;
          }
        }
      }
      BioseqUnlock(bsp);
    }
    pcdsThis = pcdsThis->next;
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* return a list of Tax-Ids as found in the CddSum Data Structure            */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Int4Ptr CddGetTaxIds(Int4 *ntids, CddSumPtr pcds)
{
  CddSumPtr       pcdsThis;
  Int4Ptr         pi;
  BioseqPtr       bsp;
  ValNodePtr      descr;
  BioSourcePtr    bsop;
  Int4            thistaxid;
  OrgRefPtr       pOrgRef;
  DbtagPtr        dbtp;
  ObjectIdPtr     oidp;
  
  *ntids = 0;
  if (!TaxArchInit()) return NULL;
  pi = MemNew(sizeof(Int4) * 1000);

  pcdsThis = pcds; while (pcdsThis) {
    bsp = BioseqLockById(pcdsThis->sip);
    if (bsp) {
      descr = bsp->descr; thistaxid = 0;
      while (descr) {
        if (descr->choice == Seq_descr_source) {
          bsop = descr->data.ptrvalue;
          pOrgRef = bsop->org;
	  if (NULL != pOrgRef) {
	    if (NULL != pOrgRef->taxname) {
	      pcdsThis->cTaxName = StringSave(pOrgRef->taxname);
	    }
	  }
          if (pOrgRef->db) {
            dbtp = pOrgRef->db->data.ptrvalue;
            oidp = dbtp->tag;
            thistaxid = oidp->id;
          } else {
            thistaxid = tax_getIdByName(pOrgRef->taxname,NULL,0);
          }
          break;
        }
        descr = descr->next;
      }
      pcdsThis->iTaxId = thistaxid;
      if (thistaxid > 0) {
        pi[*ntids] = thistaxid;
        (*ntids)++;
        if (*ntids >= 1000) {
	  pi = Nlm_MemMore(pi, (*ntids+1) * sizeof(Int4));
	}
      }
      BioseqUnlock(bsp);
    }
    pcdsThis = pcdsThis->next;
  }
  TaxArchFini();
  return(pi);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* print header and footer for the CDDServer pages                           */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CDDSrvHead(FILE *table, CharPtr title)
{
  CharPtr cTitle;

  cTitle = MemNew(128 * sizeof (Char));
  if (!title) {
    StringCpy(cTitle,"NCBI CDD");
  } else (StringCpy(cTitle, title));
  fprintf(table,"Content-type: text/html\n\n");
  fprintf(table,"<html>\n");
  fprintf(table,"  <head>\n");
  fprintf(table,"    <META name=\"keywords\" content=\"CDD, Conserved Domain Database, protein domains, NCBI, National Center for Biotechnology  Information, National Library of Medicine, NLM, PubMed\">\n");
  fprintf(table,"    <META name=\"description\" content=\"CDD is a protein domain classification resource of the National Center for Biotechnology Information\">\n");
  fprintf(table,"    <title>%s</title>\n",cTitle);
  fprintf(table,"    <link rel=\"stylesheet\" href=\"http://www.ncbi.nlm.nih.gov/corehtml/ncbi_test.css\">\n");
  fprintf(table,"  </head>\n");
/*  fprintf(table,"  <body bgcolor=\"#FFFFFF\" background=\"http://www.ncbi.nlm.nih.gov/corehtml/bkgd.gif\" text=\"#000000\" link=\"#0033CC\" vlink=\"#CC3300\">\n"); */
  fprintf(table,"  <body bgcolor=\"#FFFFFF\" text=\"#000000\" link=\"#0033CC\" vlink=\"#CC3300\">\n");
  fprintf(table,"<!--  the header   -->\n");
  fprintf(table,"    <img src=\"http://www.ncbi.nlm.nih.gov/corehtml/transparent.gif\" width=\"600\" height=\"1\" border=\"0\">\n");
  fprintf(table," <!--start of first table -->\n");
  fprintf(table,"    <table width=\"100%%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">\n");
  fprintf(table,"      <tr>                  \n");
  fprintf(table,"<!-- logo -->\n");
  fprintf(table,"        <td>\n");
  fprintf(table,"          <table width=\"100%%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">\n");
  fprintf(table,"            <tr>\n");
  fprintf(table,"              <td align=\"left\"><a href=\"http://www.ncbi.nlm.nih.gov\"><img src=\"/Structure/invleft.GIF\" width=\"130\" height=\"45\" border=\"0\" alt=\"NCBI\"></a>\n");
  fprintf(table,"              </td>\n");
  fprintf(table,"              <td align=\"left\">\n");
  fprintf(table,"                <span class=\"H2\">Conserved Domain Database</span>\n");
  fprintf(table,"              </td>\n");
  fprintf(table,"            </tr>\n");
  fprintf(table,"          </table>\n");
  fprintf(table,"        </td>\n");
  fprintf(table,"      </tr>\n");
  fprintf(table,"      <tr>\n");
  fprintf(table,"<!--site map-->\n");
  fprintf(table,"        <td>\n");
  fprintf(table,"          <table class=\"TEXT\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\" bgcolor=\"#000000\" width=\"100%%\">\n");
  fprintf(table,"            <tr class=\"TEXT\" align=\"CENTER\">\n");
  fprintf(table,"              <td width=\"14.28%%\"><a href=\"http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?db=PubMed\" class=\"BAR\"><FONT COLOR=\"#FFFFFF\">PubMed</FONT></a></td>\n");
  fprintf(table,"              <td width=\"14.28%%\"><a href=\"http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?db=Nucleotide\" class=\"BAR\"><FONT COLOR=\"#FFFFFF\">Nucleotide</FONT></a></td>\n");
  fprintf(table,"              <td width=\"14.28%%\"><a href=\"http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?db=Protein\" class=\"BAR\"><FONT COLOR=\"#FFFFFF\">Protein</FONT></a></td>\n");
  fprintf(table,"              <td width=\"14.28%%\"><a href=\"http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?db=Structure\" class=\"BAR\"><FONT COLOR=\"#FFFFFF\">Structure</FONT></a></td>\n");
  fprintf(table,"              <td width=\"14.28%%\"><a href=\"http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?db=cdd\" class=\"BAR\"><FONT COLOR=\"#FFFF00\"><b>CDD</b></FONT></a></td>\n");
  fprintf(table,"              <td width=\"14.28%%\"><a href=\"http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?db=Taxonomy\" class=\"BAR\"><FONT COLOR=\"#FFFFFF\">Taxonomy</FONT></a></td>\n");
  fprintf(table,"              <td width=\"14.28%%\"><a href=\"cdd_help.shtml\" class=\"BAR\"><FONT COLOR=\"#FFFF00\"><b>Help?</b></FONT></a></td>\n");
  fprintf(table,"            </tr>\n");
  fprintf(table,"	  </table>\n");
  fprintf(table,"       </td>\n");
  fprintf(table,"      </tr>\n");
  fprintf(table,"    </table>\n");
  fprintf(table,"<!--end of first table -->\n");
  MemFree(cTitle);
}

/*---------------------------------------------------------------------------*/
/* lower part of the page - the "footer"                                     */
/*---------------------------------------------------------------------------*/
static void CDDSrvFoot(FILE *table)
{
  fprintf(table,"<!-- -------- end of view --------- -->\n");
  fprintf(table,"         </td>\n");
  fprintf(table,"<!-- end of right content column  -->                         \n");
  fprintf(table,"      </tr>\n");
  fprintf(table,"      <tr valign=\"TOP\"> \n");
  fprintf(table,"        <td width=\"100%%\"> \n");
  fprintf(table,"          <br>\n");
  fprintf(table,"          <div align=\"left\" class=\"medium1\">  \n");
  fprintf(table,"            <p><b><a href=\"http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=retrieve&db=pubmed&list_uids=12520028&dopt=Abstract\">Citing CDD:</a></b> Marchler-Bauer A, Anderson JB, DeWeese-Scott C, Fedorova ND, Geer LY, He S, Hurwitz DI, Jackson JD, Jacobs AR, Lanczycki CJ, Liebert CA, Liu C, Madej T, Marchler GH, Mazumder R, Nikolskaya AN, Panchenko AR, Rao BS, Shoemaker BA, Simonyan V, Song JS, Thiessen PA, Vasudevan S, Wang Y, Yamashita RA, Yin JJ, Bryant SH (2003), \"<i>CDD: a curated Entrez database of conserved domain alignments</i>\", <b>Nucleic Acids Res. 31</b>: 383-387</p>\n");
  fprintf(table,"            <p>&nbsp;</p>\n");
  fprintf(table,"          </div>\n");
  fprintf(table,"        </td>\n");
  fprintf(table,"      </tr>\n");
  fprintf(table,"      <tr valign=\"TOP\"> \n");
  fprintf(table,"        <td width=\"100%%\"> \n");
  fprintf(table,"          <br>\n");
  fprintf(table,"          <div align=\"center\" class=\"medium1\">  \n");
  fprintf(table,"            <p><b><a href=\"help.html\">Help</a></b> | <a href=\"http://www.ncbi.nlm.nih.gov/About/disclaimer.html\">Disclaimer</a> | <a href=\"mailto:info@ncbi.nlm.nih.gov\">Write to the Help Desk</a><BR><a href=\"http://www.ncbi.nlm.nih.gov\">NCBI</a> | <a href=\"http://www.nlm.nih.gov\">NLM</a> | <a href=\"http://www.nih.gov\">NIH</a> </p>\n");
  fprintf(table,"            <p>&nbsp;</p>\n");
  fprintf(table,"          </div>\n");
  fprintf(table,"        </td>\n");
  fprintf(table,"      </tr>\n");
  fprintf(table,"    </table>\n");
  fprintf(table,"<!--  end of content  -->\n");
  fprintf(table,"  </body>\n");
  fprintf(table,"</html>\n");
  fflush(table);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
  static PrivateTreeNodePtr PrivateTreeNodeNew() {
    PrivateTreeNodePtr ptnp;
    ptnp = (PrivateTreeNodePtr) MemNew(sizeof(PrivateTreeNode));
    ptnp->x = 0; ptnp->y = 0; ptnp->width = 0;
    ptnp->pcdtree = NULL;
    ptnp->bIsCurrent = FALSE;
    ptnp->parent = NULL;
    ptnp->next   = NULL;
    ptnp->iNChildren = 0;
    ptnp->children = MemNew(MAXTREELEVELS * sizeof(PrivateTreeNodePtr));
    return (ptnp);
  }

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* find a node in a hierarchical tree                                        */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static PrivateTreeNodePtr FindTreeNodeLinear(PrivateTreeNodePtr root, CharPtr acc)
{
  if (Nlm_StrCmp(acc,root->pcdtree->NodeAccession) == 0) {
    return(root);
  } else {
    if (NULL != root->next) {
      return(FindTreeNodeLinear(root->next, acc));
    } else return NULL;
  }
}

static PrivateTreeNodePtr FindTreeNode(PrivateTreeNodePtr root, CharPtr acc)
{
  PrivateTreeNodePtr child, found;
  Int4               i;
  
  
  if (Nlm_StrCmp(acc,root->pcdtree->NodeAccession) == 0) {
    return(root);
  } else {
    for (i=0;i<root->iNChildren;i++) {
      found = FindTreeNode(root->children[i], acc);
      if (found) return (found);
    }
  }
  return NULL;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* creates an image visualizing a family's hierarchical structure            */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean CDDSrvTreeViewIterator(Int4 iMode, PrivateTreeNodePtr ptnp,
                                      Int4Ptr iLayerSize, Int4Ptr iUsedInLayer,
				      Int4Ptr iLastPosInLayer, Int4 iSuggX,
				      Int4 iImgWidth, Int4 iImgHeight, FILE *table,
				      Int4 *iNBlocks, Int4 *iNLines)
{
  Int4               i, iLayer, iSuggChild;
  Int4               iMinX, iMaxX, iY, iPreferredX, iX;
  
  
  if (!ptnp) return FALSE;
  switch (iMode) {
    case 0:
      iLayer = ptnp->pcdtree->NodeLevel - 1;
      iPreferredX = (iUsedInLayer[iLayer] * iImgWidth / iLayerSize[iLayer]) +
                    (iImgWidth / iLayerSize[iLayer]) / 2 - CDD_GRAPH_WIDTH / 2;
      iMinX = iUsedInLayer[iLayer] * (CDD_GRAPH_WIDTH + CDD_GRAPH_SPACER_X) +
              CDD_GRAPH_SPACER_X;
      iMaxX = iImgWidth - (iLayerSize[iLayer]-iUsedInLayer[iLayer]) *
              (CDD_GRAPH_WIDTH + CDD_GRAPH_SPACER_X);
      if (iLastPosInLayer[iLayer]+CDD_GRAPH_WIDTH+CDD_GRAPH_SPACER_X > iMinX) {
        iMinX = iLastPosInLayer[iLayer]+CDD_GRAPH_WIDTH+CDD_GRAPH_SPACER_X;
      }
      if (iPreferredX < iMinX) iPreferredX = iMinX;
      if (iPreferredX > iMaxX) iPreferredX = iMaxX;
      if (iSuggX >= 0 && iMaxX > iSuggX && iMinX < iSuggX) {
	iX = (iSuggX + iPreferredX) / 2;
      } else if (iMaxX >= iMinX) {
        iX = iPreferredX;
      } else CddHtmlError("Can not draw tree: error in block position");
      iY = iLayer * (CDD_GRAPH_HEIGHT + CDD_GRAPH_SPACER_Y) + CDD_GRAPH_SPACER_Y;
      ptnp->x = iX;
      ptnp->y = iY;
      ptnp->width = CDD_GRAPH_WIDTH;
      
      iUsedInLayer[iLayer]++;
      iLastPosInLayer[iLayer] = iX;
      fprintf(table,"<area shape=rect coords=%d,%d,%d,%d href=\"%scddsrv.cgi?uid=%s\">\n",
              ptnp->x,ptnp->y,ptnp->x+CDD_GRAPH_WIDTH,ptnp->y+CDD_GRAPH_HEIGHT,URLBase,
	      ptnp->pcdtree->NodeAccession);
      (*iNBlocks)++;      
      break;
    case 1:
      fprintf(table,",%d,%d,%d,%d,%s",ptnp->x,ptnp->y,
              CDD_GRAPH_WIDTH,ptnp->bIsCurrent,ptnp->pcdtree->NodeAccession);
      break;
    case 2:
      if (ptnp->parent) {
        fprintf(table,",%d,%d,%d,%d",(ptnp->parent->x+CDD_GRAPH_WIDTH/2),
	        ptnp->parent->y+CDD_GRAPH_HEIGHT,
		(ptnp->x+CDD_GRAPH_WIDTH/2),ptnp->y);
      }
      break;
    default: 
      break;
  }
  if (ptnp->iNChildren > 0) {
    for (i=0;i<ptnp->iNChildren;i++) {
      if (iMode == 0) {
        (*iNLines)++;
        iSuggChild = ptnp->x - (ptnp->iNChildren * (CDD_GRAPH_WIDTH+CDD_GRAPH_SPACER_X) / 2) +
	             i * (CDD_GRAPH_WIDTH+CDD_GRAPH_SPACER_X);
      }
      if (!CDDSrvTreeViewIterator(iMode,ptnp->children[i],iLayerSize,
                                  iUsedInLayer,iLastPosInLayer,iSuggChild,
				  iImgWidth, iImgHeight,table,iNBlocks,iNLines))
        return FALSE;
    }
  }
  return TRUE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CDDSrvMakeTreeView(CdTreeNodePtr pcdtree, FILE *table)
{
  CdTreeNodePtr      pcdtreeThis, pcdtreeThat;
  Int4               iNumLayers = 0, iLayerSize[MAXTREELEVELS];
  Int4               i, iMaxLayerSize = 0;
  Int4               iImgHeight = 0, iImgWidth = 0;
  Int4               iUsedInLayer[MAXTREELEVELS];
  Int4               iLastPosInLayer[MAXTREELEVELS];  
  PrivateTreeNodePtr ptnp, ptnpRoot = NULL, ptnpFound, ptnpHead, ptnpTail;
  Int4               iNBlocks = 0, iNLines = 0;

  for (i=0;i<MAXTREELEVELS;i++) {
    iLayerSize[i] = 0;
    iUsedInLayer[i] = 0;
  }
/* create linear chain of PrivateTreeNodes */
  ptnpHead = NULL; ptnpTail = NULL;
  pcdtreeThis = pcdtree;
  while (pcdtreeThis) {
    ptnp = PrivateTreeNodeNew();
    ptnp->pcdtree = pcdtreeThis;
    i = pcdtreeThis->NodeLevel - 1;
    if (i>=MAXTREELEVELS) CddHtmlError("Can not draw tree: too many levels");
    if ((i+1) > iNumLayers) iNumLayers = i+1;
    iLayerSize[i]++;
    if (iLayerSize[i] > iMaxLayerSize) iMaxLayerSize = iLayerSize[i];
    if (Nlm_StrCmp(pcdtreeThis->NodeAccession,cCDDid) == 0) ptnp->bIsCurrent = TRUE;
    if (!ptnpRoot) {
      if (Nlm_StrCmp(pcdtreeThis->RootAccession,"NoAcc")== 0) ptnpRoot = ptnp;
/*      if (Nlm_StrCmp(pcdtreeThis->RootAccession,cCDDid)== 0) ptnpRoot = ptnp; */
      if (Nlm_StrCmp(pcdtreeThis->RootAccession,pcdtreeThis->NodeAccession)== 0) ptnpRoot = ptnp;
    }
    if (!ptnpHead) ptnpHead = ptnp;
    if (ptnpTail) ptnpTail->next = ptnp;
    ptnpTail = ptnp;
    pcdtreeThis = pcdtreeThis->next;
  }
  if (!ptnpRoot) CddHtmlError("Can not draw tree: no apparent root");
/* discover hierarchical tree structure and record */
  ptnp = ptnpHead;
  while (ptnp) {
    pcdtreeThis = ptnp->pcdtree;
    if (pcdtreeThis->ParentAccession && Nlm_StrLen(pcdtreeThis->ParentAccession)) {
      ptnpFound = FindTreeNodeLinear(ptnpHead, pcdtreeThis->ParentAccession);
      if (ptnpFound) {
        ptnpFound->children[ptnpFound->iNChildren]=ptnp;
	ptnp->parent = ptnpFound;
        ptnpFound->iNChildren++;
      } else CddHtmlError("Can not draw tree: missing parent node");
    

    }
    ptnp = ptnp->next;
  }
/* first iteration through tree - determine positions of blocks */
  iImgWidth  = iMaxLayerSize * (CDD_GRAPH_WIDTH +  CDD_GRAPH_SPACER_X) + CDD_GRAPH_SPACER_X;
  iImgHeight = iNumLayers    * (CDD_GRAPH_HEIGHT + CDD_GRAPH_SPACER_Y) + CDD_GRAPH_SPACER_Y;
  fprintf(table,"<map name=\"img_map\">\n");
  if (!CDDSrvTreeViewIterator(0,ptnpRoot,iLayerSize,iUsedInLayer,iLastPosInLayer,-1,iImgWidth,iImgHeight,table,&iNBlocks,&iNLines))
    CddHtmlError("Cannot not draw tree: error in assigning positions");
  fprintf(table,"</map>\n");
/* second iteration through tree - draw blocks */
  fprintf(table,"<img src=\"%scddsrv.cgi?PIC=%d,%d,%d",URLBase,iImgWidth,iImgHeight,iNBlocks);
  if (!CDDSrvTreeViewIterator(1,ptnpRoot,iLayerSize,iUsedInLayer,iLastPosInLayer,-1,iImgWidth,iImgHeight,table,&iNBlocks,&iNLines))
    CddHtmlError("Cannot not draw tree: error in drawing blocks");
/* third iteration through tree - draw lines */
  fprintf(table,",%d",iNLines);
  if (!CDDSrvTreeViewIterator(2,ptnpRoot,iLayerSize,iUsedInLayer,iLastPosInLayer,-1,iImgWidth,iImgHeight,table,&iNBlocks,&iNLines))
    CddHtmlError("Cannot not draw tree: error in drawing lines");
  fprintf(table,"\" usemap=\"#img_map\" border=0 ISMAP>\n");

}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* prints the table at the top of a CD-Server generated page                 */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CDDSrvInfoBlk(CddPtr pcdd, FILE *table, CharPtr dbversion,
                          Int4 thisTax, Boolean bHasPdb, Boolean bHasConsensus,
			  CddSumPtr pcds, CharPtr QuerySeq, CharPtr QueryAlign,
			  Int4 iQueryGi, CharPtr QueryName, ValNodePtr txids,
			  Int2 iPDB, Int4 alen, Int4 nTaxIds, Int4Ptr iTaxids,
			  CdTreeNodePtr pcdtree)
{
  CddDescrPtr       pcdsc;
  CddSumPtr         pcdsThis;
  CharPtr           psource    = NULL;
  CharPtr           psource_id = NULL;
  CharPtr           cParentTaxName;
  DatePtr           dtp, utp   = NULL;
  DbtagPtr          dbtp;
  Int4Ptr           iPMids;
  ObjectIdPtr       oidp;
  OrgRefPtr         orp;
  SeqPortPtr        spp;
  SeqIntPtr         sintp;
  ValNodePtr        pub, vnp;
  Int4              iPssmId    = -1;
  Int4              iStatus;
  Int4              iNCit      = 0;
  Int4              pmid, i;
  Int4              iParent    = 0;
  Char              buf[61];
  Char              source[PATH_MAX];
  Char              source_id[PATH_MAX];
  Boolean           bRefOpen   = FALSE;
  Boolean           bIsArch    = FALSE;


  iPssmId = CddGetPssmId(pcdd);

/* Start of yellow-colored info block */

  fprintf(table,"    <table border=\"0\" cellspacing=\"0\" cellpadding=\"2\" width=\"100%%\" bgcolor=\"#FFFFCC\">\n");
  fprintf(table,"      <tr>\n");
  fprintf(table,"        <td align=\"RIGHT\" class=\"TEXT\" NOWRAP><strong>CD:</strong></td>\n");
  if (iPssmId > 0) {
    fprintf(table,"	 <td align=\"LEFT\" class=\"TEXT\" NOWRAP><A HREF=\"%scddsrv.cgi?uid=%s&version=%s\"><B>%s.%d, %s</B></A>",
            URLBase, cCDDid, dbversion, cCDDid, CddGetVersion(pcdd),pcdd->name);
  } else {
    fprintf(table,"	 <td align=\"LEFT\" class=\"TEXT\" NOWRAP COLSPAN=\"3\"><A HREF=\"%scddsrv.cgi?uid=%s&version=%s\"><B>%s.%d, %s</B></A>",
            URLBase, cCDDid, dbversion, cCDDid, CddGetVersion(pcdd),pcdd->name);
  }
  if (thisTax != 0 || txids) fprintf(table,", selected subset");
  if (QuerySeq || iQueryGi != -1)  fprintf(table,", Query added");
  fprintf(table,"</td>\n");
  if (iPssmId > 0) {
    fprintf(table,"        <td align=\"RIGHT\" class=\"TEXT\" NOWRAP><strong>PSSM-Id:</strong></td>\n");
    fprintf(table,"        <td align=\"LEFT\" class=\"TEXT\" NOWRAP>%d</td>\n",iPssmId);
  }
  fprintf(table,"        <td align=\"RIGHT\" class=\"TEXT\" NOWRAP><strong>Source:</strong></td>\n");
  psource    = CddGetSource(pcdd);
  psource_id = CddGetSourceId(pcdd);
  if (!psource_id) {
    Nlm_StrCpy(source_id,cCDDid);
  } else Nlm_StrCpy(source_id,psource_id);
  if (!psource) {
    Nlm_StrCpy(source,"unknown"); 
  } else  Nlm_StrCpy(source,psource);
  if (Nlm_StrCmp("Smart",source) == 0) {
    if (StrNCmp(source_id,"smart0",6) == 0) {
      fprintf(table,"        <td align=\"LEFT\" class=\"TEXT\" NOWRAP><A HREF=\"%s%s\">%s</A></td>\n",SMACCcgi,&source_id[5],source);
    } else {
      fprintf(table,"        <td align=\"LEFT\" class=\"TEXT\" NOWRAP><A HREF=\"%s%s\">%s</A></td>\n",SMARTcgi,&source_id,source);
    }
  } else if (Nlm_StrCmp("Pfam",source) == 0) {
    fprintf(table,"        <td align=\"LEFT\" class=\"TEXT\" NOWRAP><A HREF=\"%s%s\">%s[US]</A>, <A HREF=\"%s%s\">%s[UK]</A></td>\n",
            PFAMcgiUS,&source_id[4],source,PFAMcgiUK,&source_id[4],source);
  } else if (Nlm_StrCmp("Cog",source) == 0) {
      fprintf(table,"        <td align=\"LEFT\" class=\"TEXT\" NOWRAP><A HREF=\"%s%s\">%s</A></td>\n",COGcgi,source_id,source);
  } else if (Nlm_StrCmp("Kog",source) == 0) {
      fprintf(table,"        <td align=\"LEFT\" class=\"TEXT\" NOWRAP><A HREF=\"%s%s\">%s</A></td>\n",KOGcgi,source_id,source);
  } else {
    fprintf(table,"        <td align=\"LEFT\" class=\"TEXT\" NOWRAP>%s</td>\n",source);
  }
  fprintf(table,"      </tr>\n");
  fprintf(table,"      <tr VALIGN=\"TOP\">\n");
  fprintf(table,"        <td align=\"RIGHT\" class=\"medium1\" NOWRAP><strong>Description:</strong></td>\n");
  if (pcdtree) {
    fprintf(table,"        <td align=\"LEFT\" class=\"medium1\" COLSPAN=\"4\">%s</td>\n",CddGetDescr(pcdd));
    fprintf(table,"        <td align=\"CENTER\" class=\"medium1\" COLSPAN=\"2\">\n");
    CDDSrvMakeTreeView(pcdtree,table);
    fprintf(table,"        </td>\n");
  } else {
    fprintf(table,"        <td align=\"LEFT\" class=\"medium1\" COLSPAN=\"5\">%s</td>\n",CddGetDescr(pcdd));
  }
  fprintf(table,"      </tr>\n");
  iPMids = MemNew(100*sizeof(Int4));
  pcdsc = pcdd->description;
  while (pcdsc) {
    if (pcdsc->choice == CddDescr_reference) {
      pub = pcdsc->data.ptrvalue;
      if (pub->choice == PUB_PMid) {
        iPMids[iNCit] = pub->data.intvalue;
        iNCit++;
        if (iNCit >= 100) break;
      } else  {
        pmid = 0;
	if (!bRefOpen) {
	  if (MedArchInit()) bRefOpen = TRUE;
	}
	if (bRefOpen) {
          pmid = MedArchCitMatch(pub);
	  if (pmid > 0) {
	    iPMids[iNCit] = pmid; 
	    iNCit++;
            if (iNCit >= 100) break;
	  }
	}
      }
    }
    pcdsc = pcdsc->next;
  }
  if (bRefOpen) {
    MedArchFini();
    bRefOpen = FALSE;
  }
  vnp = CddGetRelatedCDs(pcdd,cCDDid,iPssmId, &bIsArch);
  orp = CddGetOrgRef(pcdd);
  if (iNCit || vnp || orp || bIsArch) {
    fprintf(table,"      <tr valign=\"TOP\">\n");
    if (orp) {
      dbtp = orp->db->data.ptrvalue;
      oidp = dbtp->tag;
      iParent = oidp->id;
      cParentTaxName = orp->taxname;
      fprintf(table,"        <td align=\"RIGHT\" class=\"medium1\" NOWRAP><strong>Taxa:</strong></td>\n");
      if (iNCit || (vnp || bIsArch)) {
        fprintf(table,"        <td align=\"LEFT\" class=\"medium1\" NOWRAP>");
      } else {
        fprintf(table,"        <td align=\"LEFT\" class=\"medium1\" NOWRAP COLSPAN=\"5\">");
      }
      fprintf(table,"<A HREF=\"%s%d\">&nbsp;%s</A></td>\n",TAXcgi,iParent,cParentTaxName);
    }
    if (iNCit) {
      fprintf(table,"        <td align=\"RIGHT\" class=\"medium1\" NOWRAP><strong>References:</strong></td>\n");
      if ((vnp || bIsArch)) {
        fprintf(table,"        <td align=\"LEFT\" class=\"medium1\" NOWRAP><A HREF=\"%s",PUBcgi);
      } else if (orp) {
        fprintf(table,"        <td align=\"LEFT\" class=\"medium1\" NOWRAP COLSPAN=\"3\"><A HREF=\"%s",PUBcgi);
      } else {
        fprintf(table,"        <td align=\"LEFT\" class=\"medium1\" NOWRAP COLSPAN=\"5\"><A HREF=\"%s",PUBcgi);
      }
      for (i=0;i<iNCit;i++) fprintf(table,"%d,",iPMids[i]);
      if (iNCit > 1) {
        fprintf(table,"\">%d Pubmed Links",iNCit);
      } else fprintf(table,"\">%d Pubmed Link",iNCit);
      fprintf(table,"</A></td>\n");
    }
    if (vnp || bIsArch) {
      fprintf(table,"        <td align=\"RIGHT\" class=\"medium1\" NOWRAP><strong>Related:</strong></td>\n");
      if (iNCit && orp) {
        fprintf(table,"        <td align=\"LEFT\" class=\"medium1\">");
      } else if (iNCit || orp) {
        fprintf(table,"        <td align=\"LEFT\" class=\"medium1\" COLSPAN=\"3\">");
      } else {
        fprintf(table,"        <td align=\"LEFT\" class=\"medium1\" COLSPAN=\"5\">");
      }
      while (vnp) {
        fprintf(table,"<A HREF=\"%scddsrv.cgi?uid=%s\">%s</A>",URLBase,vnp->data.ptrvalue,vnp->data.ptrvalue);
        if (vnp->next) fprintf(table,", ");
        vnp = vnp->next;
      }
      if (bIsArch) fprintf(table,"may span multiple domains");
      fprintf(table,"</td>\n");
    }
    fprintf(table,"      </tr>\n");
  }
  
  fprintf(table,"      <tr>\n");
  fprintf(table,"        <td align=\"RIGHT\" class=\"medium1\" NOWRAP><strong>Status:</strong></td>\n");
  fprintf(table,"        <td align=\"LEFT\" class=\"medium1\" NOWRAP>");
  iStatus = CddGetStatus(pcdd);
  switch(iStatus) {
    case 0:
      fprintf(table,"unassigned");
      break;        
    case 1:
      fprintf(table,"curated CD");
      break;        
    case 2:
      fprintf(table,"curated CD pending");
      break;        
    case 3:
      fprintf(table,"Alignment from source\n");
      break;        
    case 4:
      fprintf(table,"checkpoint/PSSM only\n");
      break;        
    default:
      fprintf(table,"unknown");
      break;        
  }          
  fprintf(table,"</td>\n");
  dtp = CddGetCreateDate(pcdd);
  utp = CddGetUpdateDate(pcdd);
  fprintf(table,"        <td align=\"RIGHT\" class=\"medium1\" NOWRAP><strong>Created:</strong></td>\n");
  if (utp) {
    fprintf(table,"        <td align=\"LEFT\" class=\"medium1\" NOWRAP>%2d-%3s-%4d</td>\n",(int)dtp->data[3],NCBI_months[dtp->data[2]-1],(int)dtp->data[1]+1900);
  } else {
    fprintf(table,"        <td align=\"LEFT\" class=\"medium1\" NOWRAP COLSPAN=\"3\">%2d-%3s-%4d</td>\n",(int)dtp->data[3],NCBI_months[dtp->data[2]-1],(int)dtp->data[1]+1900);
  }
  if (utp) {
    fprintf(table,"        <td align=\"RIGHT\" class=\"medium1\" NOWRAP><strong>Updated:</strong></td>\n");
    fprintf(table,"        <td align=\"LEFT\" class=\"medium1\" NOWRAP>%2d-%3s-%4d</td>\n",(int)utp->data[3],NCBI_months[utp->data[2]-1],(int)utp->data[1]+1900);
  }
  fprintf(table,"      </tr>\n");
  fprintf(table,"      <tr>\n");
  if (alen > 0) {
    fprintf(table,"        <td align=\"RIGHT\" class=\"medium1\" NOWRAP><strong>Aligned:</strong></td>\n");
    fprintf(table,"        <td align=\"LEFT\" class=\"medium1\" NOWRAP>%d rows</td>\n",alen);
  }
  sintp = (SeqIntPtr) pcdd->profile_range;
  if (sintp) {
    fprintf(table,"        <td align=\"RIGHT\" class=\"medium1\" NOWRAP><strong>PSSM:</strong></td>\n");
    fprintf(table,"        <td align=\"LEFT\" class=\"medium1\" NOWRAP>%d columns</td>\n",sintp->to - sintp->from + 1);
  }
  pcdsThis = pcds;
  if (pcdsThis) {
    fprintf(table,"        <td align=\"RIGHT\" class=\"medium1\" NOWRAP><strong>Representative:</strong></td>\n");
    fprintf(table,"        <td align=\"LEFT\" class=\"medium1\" NOWRAP>");
    if (pcdsThis->bIsPdb) {
      fprintf(table, "<FONT COLOR=#660000>pdb</FONT>|<A HREF=\"%s%s\">%s %s</A>|", MMDBCALL, pcdsThis->cPdbId,
              pcdsThis->cPdbId, pcdsThis->cChainId);
      if (pcdsThis->cChainId[0] == '\0' || pcdsThis->cChainId[0] == ' ') {
        fprintf(table,"<FONT COLOR=#660000>%s</FONT> [<A HREF=\"%s%s\">CD</A>]\n",pcdsThis->cDefLine,WRPSBCALL,pcdsThis->cPdbId);
      } else {
        fprintf(table,"<FONT COLOR=#660000>%s</FONT> [<A HREF=\"%s%s%s\">CD</A>]\n",pcdsThis->cDefLine,WRPSBCALL,pcdsThis->cPdbId,pcdsThis->cChainId);
      }
    } else if (!bHasConsensus) {
      fprintf(table, "gi|<A HREF=\"%s%d\">%d</A>|", ENTREZCALL, pcdsThis->uid,pcdsThis->uid);
      fprintf(table,"%s [<A HREF=\"%s%d\">CD</A>]\n",pcdsThis->cDefLine,WRPSBCALL,pcdsThis->uid);
    } else {
      fprintf(table,"Consensus");
    }
    fprintf(table,"</td>\n");
  }
  fprintf(table,"      </tr>\n");
  if (iPssmId > 0) {
    fprintf(table,"      <tr>\n");
    fprintf(table,"        <td align=\"RIGHT\" class=\"medium1\" NOWRAP><strong>Proteins:</strong></td>\n");  
    fprintf(table,"        <td align=\"LEFT\" class=\"medium1\" COLSPAN=\"5\"><a href=\"/Structure/lexington/lexington.cgi?cmd=cdd&uid=%d\">\n",iPssmId);  
    fprintf(table,"        [Click here for CDART summary of Proteins containing %s]</a>\n",cCDDid);
    if (Nlm_StrCmp(CDDlocat,"inhouse")==0) {
      fprintf(table,"<a href=\"/Structure/lexington/rosaparks.cgi?cmd=reply&input=%s\">[Click here for rosaparks display]</a> ",cCDDid);
    
    }
    fprintf(table,"      </td></tr>\n");
  } else if (Nlm_StrCmp(CDDlocat,"inhouse")==0){
    fprintf(table,"      <tr>\n");
    fprintf(table,"        <td align=\"RIGHT\" class=\"medium1\" NOWRAP><strong>Proteins:</strong></td>\n");  
    fprintf(table,"        <td align=\"LEFT\" class=\"medium1\" COLSPAN=\"5\">");  
    fprintf(table,"        <a href=\"/Structure/lexington/rosaparks.cgi?cmd=reply&input=%s\">[Click here for rosaparks display]</a> ",cCDDid);
    fprintf(table,"        </td></tr>\n");
  }
  fprintf(table,"    </table>\n");

/* End of yellow-colored info-block */

  fprintf(table,"    <table border=\"0\" cellspacing=\"0\" cellpadding=\"2\" width=\"100%%\" bgcolor=\"#FFFFFF\" class=\"TEXT\">\n");
  fprintf(table,"      <tr class=\"TEXT\">\n");
  fprintf(table,"        <td align=\"LEFT\" class=\"TEXT\">\n");

  if (orp) {
    if (iTaxids && nTaxIds > 0) {
      fprintf(table,"<form action=\"http://www.ncbi.nlm.nih.gov/Taxonomy/CommonTree/wwwcmt.cgi\" enctype=\"application/x-www-form-urlencoded\" method=\"POST\" name=\"taxform\">\n");
      for (i=0;i<nTaxIds;i++) {
        fprintf(table,"<input name=\"id\" type=\"hidden\" value=\"%d\">\n",iTaxids[i]);
      }
      fprintf(table,"<input name=\"opt\" type=\"hidden\" value=\"cutall,smallfont,compact\">\n");
      fprintf(table,"<input name=\"expand\" type=\"hidden\" value=\"Collapse+All\">\n");
      fprintf(table,"<input type=hidden name=exturl value=\"%scddsrv.cgi?uid=%s&version=%s\">\n",
              CTBcgi,cCDDid,dbversion);
      MemFree(iTaxids);
      fprintf(table,"</form>\n");
    }
  }    
  fprintf(table,"<FORM METHOD=\"POST\" ACTION=\"%scddsrv.cgi\" name=\"listform\" enctype=\"application/x-www-form-urlencoded\">\n",URLBase);
  if (iPssmId > 0) {
    fprintf(table,"            <INPUT TYPE=\"HIDDEN\" NAME=\"uid\" VALUE=\"%d\">\n",iPssmId);
  } else
    fprintf(table,"            <INPUT TYPE=\"HIDDEN\" NAME=\"uid\" VALUE=\"%s\">\n",cCDDid);
  fprintf(table,"            <INPUT TYPE=\"HIDDEN\" NAME=\"version\" VALUE=\"%s\">\n",dbversion);
  fprintf(table,"            <INPUT TYPE=\"HIDDEN\" NAME=\"ascbin\" VALUE=\"5\">\n");
  fprintf(table,"</FORM>\n");

  fprintf(table,"<SCRIPT LANGUAGE=\"JavaScript\">\n"); 
  fprintf(table,"function picktaxa(selstr) {\n"); 
  fprintf(table,"  if (selstr == \"select by taxonomy\") {\n"); 
  fprintf(table,"    document.taxform.submit()\n"); 
  fprintf(table,"  }\n"); 
  fprintf(table,"  if (selstr == \"select from list\") {\n"); 
  fprintf(table,"    document.listform.submit()\n"); 
  fprintf(table,"  }\n"); 
  fprintf(table,"}\n"); 
  fprintf(table,"</SCRIPT>\n"); 


/*---------------------------------------------------------------------------*/
/* Begin Viewing Form                                                        */
/*---------------------------------------------------------------------------*/
  fprintf(table,"          <FORM METHOD=\"POST\" ACTION=\"%scddsrv.cgi\" name=\"mainform\">\n",URLBase);
  if (iPssmId > 0) {
    fprintf(table,"            <INPUT TYPE=\"HIDDEN\" NAME=\"uid\" VALUE=\"%d\">\n",iPssmId); 
  } else
    fprintf(table,"            <INPUT TYPE=\"HIDDEN\" NAME=\"uid\" VALUE=\"%s\">\n",cCDDid);
  fprintf(table,"            <INPUT TYPE=\"HIDDEN\" NAME=\"version\" VALUE=\"%s\">\n",dbversion);
  if (txids) {
    vnp = txids; while (vnp) {
      fprintf(table, "<INPUT TYPE=\"HIDDEN\" NAME=\"taxon\" VALUE=\"%d:\">\n",vnp->data.intvalue);
      vnp = vnp->next;
    }
  }
  if (QuerySeq) fprintf(table, "<INPUT TYPE=\"HIDDEN\" NAME=\"query\" VALUE=\"%s\">\n", QuerySeq);
  if (QueryAlign) fprintf(table, "<INPUT TYPE=\"HIDDEN\" NAME=\"aln\" VALUE=\"%s\">\n", QueryAlign);
  if (QueryName) fprintf(table, "<INPUT TYPE=\"HIDDEN\" NAME=\"querynm\" VALUE=\"%s\">\n", QueryName);
  if (iQueryGi != -1) fprintf(table, "<INPUT TYPE=\"HIDDEN\" NAME=\"querygi\" VALUE=\"%d\">\n",iQueryGi);
  if (thisTax > 0) fprintf(table, "<INPUT TYPE=\"HIDDEN\" NAME=\"tax\" VALUE=\"%d\">\n",thisTax);
  fprintf(table,"<TABLE NOBORDER CELLPADDING=\"2\" CELLSPACING=\"2\">\n");
  if (bHasPdb) {
    fprintf(table,"            <TR><TD CLASS=\"medium1\">\n");
    fprintf(table,"            <INPUT TYPE=\"SUBMIT\" NAME=\"3Dsub\" VALUE=\"View 3D Structure\">&nbsp;with&nbsp;\n");
    fprintf(table,"            <SELECT NAME=\"3Dopt\">\n");
#ifdef NOCN3D4
    fprintf(table,"              <OPTION SELECTED VALUE=\"0\">Cn3D\n");
#else
    fprintf(table,"              <OPTION SELECTED VALUE=\"6\">Cn3D\n");
    fprintf(table,"              <OPTION VALUE=\"7\">Cn3D/cache\n");
    fprintf(table,"              <OPTION VALUE=\"0\">Cn3D 3.0\n");
#endif
    fprintf(table,"            </SELECT>&nbsp;using&nbsp;\n");
    fprintf(table,"            <SELECT NAME=\"ato\">\n");
    fprintf(table,"              <OPTION VALUE=\"0\" SELECTED>Virtual Bonds\n");
    fprintf(table,"              <OPTION VALUE=\"1\">All Atoms\n");
    fprintf(table,"            </SELECT>");
    fprintf(table, "&nbsp;(To display structure, download <A HREF=\"http://www.ncbi.nlm.nih.gov/Structure/CN3D/cn3d.shtml\"><STRONG>Cn3D</STRONG></A>)\n");
    fprintf(table,"            </TD></TR>\n");
  }
  fprintf(table,"            <TR><TD CLASS=\"medium1\">\n");
  fprintf(table,"            <INPUT TYPE=\"SUBMIT\" NAME=\"ALsub\" VALUE=\"View Alignment\">&nbsp;as&nbsp;\n");
  fprintf(table,"            <SELECT NAME=\"ALopt\">\n");
  fprintf(table,"              <OPTION SELECTED VALUE=\"2\">Hypertext\n");
  fprintf(table,"              <OPTION VALUE=\"8\">Compact Hypertext\n");
  fprintf(table,"              <OPTION VALUE=\"3\">Plain Text\n");
  fprintf(table,"              <OPTION VALUE=\"9\">Compact Text\n");
  fprintf(table,"              <OPTION VALUE=\"4\">mFasta\n");
  fprintf(table,"            </SELECT>\n");
  fprintf(table,"            <SELECT NAME=\"pwidth\">\n");
  fprintf(table,"              <OPTION VALUE=\"40\">width 40</OPTION>\n");
  fprintf(table,"              <OPTION SELECTED VALUE=\"60\">width 60\n");
  fprintf(table,"              <OPTION VALUE=\"80\">width 80\n");
  fprintf(table,"              <OPTION VALUE=\"100\">width 100\n");
  fprintf(table,"              <OPTION VALUE=\"120\">width 120\n");
  fprintf(table,"              <OPTION VALUE=\"140\">width 140\n");
  fprintf(table,"              <OPTION VALUE=\"160\">width 160\n");
  fprintf(table,"            </SELECT> color at\n");
  fprintf(table,"            <SELECT NAME=\"bit\">\n");
  fprintf(table,"              <OPTION VALUE=\"0.5\">0.5 bits\n");
  fprintf(table,"              <OPTION VALUE=\"1.0\">1.0 bits\n");
  fprintf(table,"              <OPTION VALUE=\"1.5\">1.5 bits\n");
  fprintf(table,"              <OPTION SELECTED VALUE=\"2.0\">2.0 bits\n");
  fprintf(table,"              <OPTION VALUE=\"2.5\">2.5 bits\n");
  fprintf(table,"              <OPTION VALUE=\"3.0\">3.0 bits\n");
  fprintf(table,"              <OPTION VALUE=\"3.5\">3.5 bits\n");
  fprintf(table,"              <OPTION VALUE=\"4.0\">4.0 bits\n");
  fprintf(table,"              <OPTION VALUE=\"0.0\">Identity\n");
  fprintf(table,"            </SELECT>\n");
  if (CddHasAnnotation(pcdd)) {
    vnp = CddGetAnnotNames(pcdd);
    fprintf(table, "&nbsp;feature&nbsp\n");
    fprintf(table, "<SELECT NAME=\"feature\">\n");
    i = 0;
    while (vnp) {
      fprintf(table, "<OPTION VALUE=\"%d\">%s\n",i,(CharPtr)vnp->data.ptrvalue);
      i++;
      vnp = vnp->next;
    }
    fprintf(table, "</SELECT>\n");
  }
  fprintf(table,"            </TD></TR>\n");
  fprintf(table,"            <TR><TD CLASS=\"medium1\">\n");
  fprintf(table,"            <INPUT TYPE=\"SUBMIT\" NAME=\"ALsub\" VALUE=\"Subset Rows\">&nbsp\n");
  fprintf(table,"            <SELECT NAME=\"maxaln\">\n");
  fprintf(table,"              <OPTION VALUE=\"5\">up to 5\n");
  fprintf(table,"              <OPTION SELECTED VALUE=\"10\">up to 10\n");
  fprintf(table,"              <OPTION VALUE=\"25\">up to 25\n");
  fprintf(table,"              <OPTION VALUE=\"50\">up to 50\n");
  fprintf(table,"              <OPTION VALUE=\"75\">up to 75\n");
  fprintf(table,"              <OPTION VALUE=\"100\">up to 100\n");
  fprintf(table,"              <OPTION VALUE=\"-1\">all\n");
  fprintf(table,"            </SELECT>\n");
  fprintf(table,"            <SELECT NAME=\"seltype\" onChange=\"picktaxa(document.mainform.seltype.options[selectedIndex].text)\">\n");
  if (iPDB == 5) fprintf(table,"              <OPTION VALUE=\"0\">sequences selected below\n");
  fprintf(table,"              <OPTION VALUE=\"1\">top listed sequences\n");
  if (QuerySeq || iQueryGi != -1) {
    fprintf(table,"              <OPTION VALUE=\"2\">of the most diverse members\n");
    fprintf(table,"              <OPTION SELECTED VALUE=\"3\">sequences most similar to the query\n");
  } else {
    fprintf(table,"              <OPTION SELECTED VALUE=\"2\">of the most diverse members\n");
    fprintf(table,"              <OPTION VALUE=\"4\">select by taxonomy\n");
    fprintf(table,"              <OPTION VALUE=\"5\">select from list\n");
  }
  fprintf(table,"            </SELECT>\n");
  fprintf(table,"            </TD></TR></TABLE>\n");
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* dumps the HTML to show the list of CD sequences (tracks according to the  */
/* juke-box analogy                                                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CddServerShowTracks(CddSumPtr pcds, CddPtr pcdd, Int4 thisTax,
                                Boolean bHasPdb, CharPtr dbversion, Boolean bHasConsensus,
				Boolean bShowTax, ValNodePtr txids, Int2 iPDB, Int4 alen,
				Int4 nTaxIds, Int4Ptr iTaxids, CdTreeNodePtr pcdtree)
{
  CddDescrPtr       pCddesc;
  CddSumPtr         pcdsThis;
  CharPtr           cCurrDesc;
  Taxon1DataPtr     t1dp;
  TaxNamePtr        *tnpp;
  TXC_TreeNodePtr   *txtnpp; 
  ValNodePtr        pub;
  Boolean           bProfileOnly = FALSE;
  Boolean           bIsOasis = FALSE;
  Char              cTax[PATH_MAX];
  Char              descript[CDD_MAX_DESCR];
  Char              tableName[PATH_MAX]; 
  Int4              i;
  Int4              inChildren;
  Int4              iParent;
  Int4              muid;
  Int4              *piChildren;
  FILE              *table = NULL;

  if (Nlm_StrCmp(CDDdbtype,"oasis")==0) bIsOasis = TRUE;
  if (Nlm_StrNCmp(cCDDid,"cd",2) == 0) bIsOasis = FALSE;
/*-----------------------------------------------------
  strcpy(tableName,GetTempName("cddsrv")); 
  if  (!(table = FileOpen(tableName,WRITE)))
    CddHtmlError("Temp File Open Failed on Server");
------------------------------------------------------*/
  table = stdout;
  sprintf(tableName, "NCBI CDD %s",cCDDid);
  CDDSrvHead(table, tableName);
  CDDSrvInfoBlk(pcdd,table,dbversion,thisTax,bHasPdb,bHasConsensus,pcds,NULL,NULL,-1,NULL,txids,iPDB,alen,nTaxIds,iTaxids,pcdtree);
  pcdsThis = pcds;
  if (pcdsThis) {
    fprintf(table, "<TABLE BORDER=\"1\" CELLPADDING=\"2\" CELLSPACING=\"2\" WIDTH=100%%>\n");  
    if ((bHasPdb && bHasConsensus && bIsOasis) && bShowTax)
      fprintf(table, "<TR><TD COLSPAN=5 ALIGN=CENTER VALIGN=TOP NOWRAP>pick aligned sequences (will be added to selection above)</TD></TR>");
    else if ((bHasPdb && bHasConsensus && bIsOasis) || bShowTax)
      fprintf(table, "<TR><TD COLSPAN=4 ALIGN=CENTER VALIGN=TOP NOWRAP>pick aligned sequences (will be added to selection above)</TD></TR>");
    else fprintf(table, "<TR><TD COLSPAN=3 ALIGN=CENTER VALIGN=TOP NOWRAP>pick aligned sequences (will be added to selection above)</TD></TR>");
    pcdsThis = pcdsThis->next;

    fprintf(table, "<TR><TH>&nbsp;</TH>");
    if (bHasConsensus && bHasPdb && bIsOasis) fprintf(table,"<TH>3D</TH>");
    fprintf(table, "<TH>PDB-Id/gi</TH>");
    if (bShowTax) fprintf(table,"<TH>Organism</TH>");
    fprintf(table, "<TH ALIGN=\"LEFT\">Definition</TH></TR>\n");
    
    while (pcdsThis) {
      fprintf(table, "<TR>\n");
      fprintf(table, "<TD>\n");
      fprintf(table, "<INPUT TYPE=\"CHECKBOX\" NAME=\"gi\" VALUE=\"%d\">\n",pcdsThis->iCddIdx);
      fprintf(table, "</TD>\n");
      if (pcdsThis->bIsPdb && bHasConsensus && bIsOasis) {
        if (pcdsThis->iCddIdx == 1) {
          fprintf(table,"<TD><INPUT TYPE=\"RADIO\" NAME=\"Tdrep\" VALUE=%d CHECKED></TD>\n",pcdsThis->iCddIdx);
	} else {
          fprintf(table,"<TD><INPUT TYPE=\"RADIO\" NAME=\"Tdrep\" VALUE=%d></TD>\n",pcdsThis->iCddIdx);
        }
      }
      if (pcdsThis->bIsPdb) {
        fprintf(table, "<TD NOWRAP>\n");
        fprintf(table, "<a href=\"%s%s\">%s %s</a></TD>\n", MMDBCALL, pcdsThis->cPdbId, 
                        pcdsThis->cPdbId, pcdsThis->cChainId);
      } else {
        if (bHasConsensus && bIsOasis && bHasPdb) fprintf(table,"<TD NOWRAP COLSPAN=2>\n");
	else fprintf(table,"<TD NOWRAP>\n");    
        fprintf(table, "<A HREF=\"%s%d\">%d</a>\n", ENTREZCALL, pcdsThis->uid, pcdsThis->uid);
        fprintf(table, "</TD>\n");
      }
      if (bShowTax) {
        fprintf(table,"<TD NOWRAP><A HREF=\"%s%d\">%s</A></TD>\n",TAXcgi,pcdsThis->iTaxId,pcdsThis->cTaxName);
      }
      fprintf(table, "<TD>\n");
      if (pcdsThis->bIsPdb) {
        if (pcdsThis->cChainId[0] == '\0' || pcdsThis->cChainId[0] == ' ') {
          fprintf(table, "<FONT COLOR=#660000>%s</FONT> [<a href=\"%s%s\">CD</a>]\n",pcdsThis->cDefLine,WRPSBCALL,pcdsThis->cPdbId);
        } else {
          fprintf(table, "<FONT COLOR=#660000>%s</FONT> [<a href=\"%s%s%s\">CD</a>]\n",pcdsThis->cDefLine,WRPSBCALL,pcdsThis->cPdbId,pcdsThis->cChainId);
	}
      } else {
        fprintf(table, "%s [<a href=\"%s%d\">CD</a>]\n",pcdsThis->cDefLine,WRPSBCALL,pcdsThis->uid);
      }
      fprintf(table, "</TD>\n");
      fprintf(table, "</TR>\n");
      pcdsThis = pcdsThis->next;
    }
    fprintf(table, "</TABLE>\n");
/*
    fprintf(table, "</FORM>\n");
    fprintf(table, "<FORM METHOD=\"POST\" ACTION=\"%scddfind.cgi\">\n",URLBase);
    fprintf(table, "<INPUT TYPE=\"SUBMIT\" VALUE=\"Search CDD\"> for \n");
    fprintf(table, "<INPUT TYPE=\"TEXT\" NAME=\"query\" SIZE=\"40\">\n");
    fprintf(table, "<INPUT TYPE=\"RESET\">\n");
    fprintf(table, "</FORM>\n");
*/
  }
  CDDSrvFoot(table);


/*
  pCddesc = pcdd->description;
  while (pCddesc) {
      switch(pCddesc->choice) {
       case CddDescr_tax_source:
          if (TaxArchInit()) {
            orp  = pCddesc->data.ptrvalue;
            dbtp = orp->db->data.ptrvalue;
            oidp = dbtp->tag;
            iParent = oidp->id;
            cParentTaxName = orp->taxname;
            if (thisTax > 0) {
              iParent = thisTax;
              t1dp = tax1_getbyid(iParent);
              cParentTaxName = t1dp->org->taxname;
            }
            txtnpp = NULL;
            txtnpp = (TXC_TreeNodePtr *)txc_getChildren(iParent,&inChildren);
            if (txtnpp && inChildren > 0) {
              CddCountTaxLineage(txtnpp,inChildren,pcds);
            }
            if (iParent > 1) {
              sprintf(cTax,"%s%d",TAXcgi,iParent);
            } else {
              sprintf(cTax,"%s%d",TAXcgi,0);
            }
            fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Taxonomy spanned:</FONT></STRONG></TD>");
            fprintf(table, "<TD BGCOLOR=#FFFFCC><TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0><TR><TD VALIGN=TOP>");
            fprintf(table, "<A HREF=\"%s\">%s&nbsp</A>->&nbsp</TD>\n",cTax,cParentTaxName);
            if (txtnpp && inChildren > 0) {
              fprintf(table, "<TD><TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>");
              for (i=0;i<inChildren;i++) {
                if (txtnpp[i]->flags > 0) {
                  t1dp = tax1_getbyid(txtnpp[i]->tax_id);
                  sprintf(cTax,"%s%d",TAXcgi,txtnpp[i]->tax_id);
                  fprintf(table, "<TR><TD ALIGN=RIGHT>");
                  fprintf(table, "<A HREF=\"%scddsrv.cgi?uid=%s&tax=%d&version=%s\">[%3d]&nbsp</A> from&nbsp</TD>",
                          URLBase,cCDDid,txtnpp[i]->tax_id,dbversion,(Int4)txtnpp[i]->flags);
                  fprintf(table, "<TD><A HREF=\"%s\">%s</A></TD></TR>\n",cTax,t1dp->org->taxname);
                }            
              }
              fprintf(table, "</TABLE></TD>\n");
            }
            fprintf(table, "</TR></TABLE></TD></TR>\n");
            TaxArchFini();
          }
          break;
      }
    pCddesc = pCddesc->next;
  }
*/

  fflush(table);
  if (table != stdout) {
    fclose(table);
    PrintFile(tableName);
  }
  RemoveTempFiles();
  exit(0);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Stolen from mmdbsrv.c - converts PDB-Id to numerical mmdb-id              */
/* modified - does no longer check whether the string corresponds to an      */
/* integer in the first place, so that PDB-Id's like "1914" can be read as   */
/* well ..                                                                   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Int4 ConvertMMDBUID(CharPtr pcString)  
{
  Int4    iUID;
  CharPtr pcTemp = NULL;
        
  if (pcString == NULL) return 0;
  iUID = 0;
  pcTemp = StringSave(pcString);
  CleanSpaces(pcTemp);
	iUID = MMDBEvalPDB_FF(pcTemp);
  MemFree(pcTemp);
  return iUID; 
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Check if  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean CddUseThisMMDBid(ValNodePtr location, CddSumPtr pcds)
{
  ChemGraphAlignmentPtr cgap;
  ValNodePtr            vnp;
  Int4                  iThisId;
  CddSumPtr             pcdsThis;

  cgap = (ChemGraphAlignmentPtr) location->data.ptrvalue;
  vnp = cgap->biostruc_ids;
  vnp = vnp->next;
  iThisId = vnp->data.intvalue;
  pcdsThis = pcds;
  while (pcdsThis) {
    if (iThisId == pcdsThis->iMMDBId) {
      pcdsThis->iMMDBId = -1;
      return TRUE ;
    }
    pcdsThis = pcdsThis->next;
  }
  return FALSE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* use Paul's function to output HTML or Text-formatted alignments           */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Boolean CddInvokeAlignView(NcbiMimeAsn1Ptr pvnNcbi, CharPtr CDDalign, Int2 iPDB,
                           CharPtr QuerySeq, CharPtr QueryAlign, CharPtr dbversion,
			   CddPtr pcdd, Boolean bHasPdb, FloatHi tbit, Uint2 pwidth,
                           Int4 iQueryGi, CharPtr QueryName, Int4 iFeatNum, CddSumPtr pcds,
			   Int4 alen, Int4 nTaxIds, Int4Ptr iTaxids, ValNodePtr txids,
			   CdTreeNodePtr pcdtree)
{
  Uint4                 size = 2 * FileLength(CDDalign);
  Uint4                 uCAVoptions = 0;
  BytePtr               buf;
  BiostrucAnnotSetPtr   basp;
  BiostrucFeatureSetPtr bfsp;
  AsnIoMemPtr           aimp;
  CddDescrPtr           pCddesc;
  CharPtr               cCurrDesc;
  Char                  source[PATH_MAX];
  AlignmentFeature     *pafeat;
  AlignAnnotPtr         aap;
  Int4                  nEvidence   = 0;
  Int4                  i           = 0;
  Int4                  nFeatures   = 0;
  Int4Ptr               piSize;
  Char                  featname[PATH_MAX];
  Char                  tableName[PATH_MAX];
  ValNodePtr            pevidence   = NULL;
  ValNodePtr            vnpThis, pub;
  SeqAlignPtr           salp;
  SeqAnnotPtr           sap;
  BiostrucAlignSeqPtr   pbsaSeq;
  
    
  if (size == 0) {
    pbsaSeq = pvnNcbi->data.ptrvalue;
    sap = pbsaSeq->seqalign;
    salp = sap->data;
    piSize = (Int4Ptr) GetAlignmentSize(salp);
    size = (Uint4) (piSize[0] * (8000 + piSize[1] * 400));
  }
  buf = MemNew(size);
  aimp = (AsnIoMemPtr) AsnIoMemOpen("wb",buf,size);
  if (NULL == aimp) return(FALSE);
  if (!NcbiMimeAsn1AsnWrite(pvnNcbi,aimp->aip,NULL)) return(FALSE);
  AsnIoFlush(aimp->aip);
  AsnIoMemClose(aimp);

  if (CddHasAnnotation(pcdd)) {
    aap = pcdd->alignannot;
    while (aap) {
      if (iFeatNum == i) {
        pafeat = (AlignmentFeature *)MemNew(sizeof(AlignmentFeature));
	pafeat->description = /*aap->description*/ NULL;
	pafeat->featChar = '#';
	sprintf(featname,"Feature %d",iFeatNum+1);
	pafeat->shortName = featname;
	pafeat->locations = CddGetFeatLocList(aap->location, &(pafeat->nLocations));
	nFeatures = 1;
	pevidence = aap->evidence;
	break;
      }
      i++; aap = aap->next;
    }
  }

  uCAVoptions = CAV_TEXT;
  if (iPDB == 2 || iPDB == 8) uCAVoptions = CAV_HTML;
  else if (iPDB == 4) {
    uCAVoptions = CAV_FASTA;
    uCAVoptions |= CAV_LEFTTAILS;
    uCAVoptions |= CAV_RIGHTTAILS;
    uCAVoptions |= CAV_FASTA_LOWERCASE;
  }
  if (iPDB ==8 || iPDB ==9) uCAVoptions |= CAV_CONDENSED;
  if (tbit <= 0.0) uCAVoptions |= CAV_SHOW_IDENTITY;
/*  uCAVoptions |= CAV_ANNOT_BOTTOM; */
  if (QuerySeq || iQueryGi != -1) {
    sprintf(tableName, "NCBI CDD %s with Query Sequence added",cCDDid);
  } else sprintf(tableName, "NCBI CDD %s",cCDDid);
  CDDSrvHead(stdout, tableName);
  
  if ((QuerySeq || iQueryGi != -1) && QueryAlign) { 
    CDDSrvInfoBlk(pcdd,stdout,dbversion,0,bHasPdb,CddHasConsensus(pcdd),pcds,QuerySeq,QueryAlign,iQueryGi,QueryName,txids,iPDB,alen,nTaxIds,iTaxids,pcdtree);
  } else {
    CDDSrvInfoBlk(pcdd,stdout,dbversion,0,bHasPdb,CddHasConsensus(pcdd),pcds,QuerySeq,QueryAlign,iQueryGi,QueryName,txids,iPDB,alen,nTaxIds,iTaxids,pcdtree);
  }
    printf("</FORM>\n");
  if (iPDB != 2 && iPDB != 8) {
    if ((QuerySeq || iQueryGi != -1) && QueryAlign) {
      printf("         </td>\n");
      printf("      </tr>\n");
    }
    printf("      <tr valign=\"TOP\">\n"); 
    printf("        <td width=\"100%%\"> \n");
    printf("<PRE>\n");
  }
  CAV_DisplayMultiple(buf, uCAVoptions, pwidth, tbit, NULL, nFeatures, pafeat);
  MemFree(buf);
  if (iPDB != 2 && iPDB != 8) printf("</PRE>\n");
  
  if (pevidence && iPDB != 4) {
    printf("<TABLE BORDER=\"0\" cellspacing=\"0\" cellpadding=\"2\" width=\"100%%\" bgcolor=\"#FFFFFF\">\n");
    printf("<TR><TD CLASS=\"medium1\" VALIGN=\"TOP\" NOWRAP><B>Feature %d:</B></TD><TD CLASS=\"medium1\" VALIGN=TOP>%s</TD></TR>\n",
           iFeatNum+1,aap->description);
    printf("<TR><TD ROWSPAN=\"25\" CLASS=\"medium1\" VALIGN=TOP><B>Evidence:</B></TD>\n");
    while (pevidence) {
      switch (pevidence->choice) {
        case FeatureEvidence_comment:
          printf("<TD CLASS=\"medium1\">Comment:</TD><TD CLASS=\"medium1\">%s</TD></TR>\n",
	         (CharPtr)pevidence->data.ptrvalue);
          break;
        case FeatureEvidence_reference:
	  pub = pevidence->data.ptrvalue;
          printf("<TD CLASS=\"medium1\">Citation:</TD><TD CLASS=\"medium1\"><A HREF=\"%s%d\">PMID %d</A></TD></TR>\n",
	         PUBcgi,pub->data.intvalue,pub->data.intvalue);
          break;
        case FeatureEvidence_bsannot:
          basp = (BiostrucAnnotSetPtr) pevidence->data.ptrvalue;
	  if (basp->features) {
	    bfsp = basp->features;
	    while(bfsp) {
              vnpThis = bfsp->descr;
	      while (vnpThis) {
		if (vnpThis->choice == BiostrucFeatureSetDescr_name) {
	          printf("<TD CLASS=\"medium1\">Structure:</TD><TD CLASS=\"medium1\">%s</TD></TR>\n",vnpThis->data.ptrvalue);
		}
		vnpThis = vnpThis->next;
	      }
	      bfsp = bfsp->next;
	    }  
	  }
          printf("<TR><TD CLASS=\"medium1\">&nbsp;</TD><TD CLASS=\"medium1\"><FORM METHOD=\"POST\" ACTION=\"%scddsrv.cgi\">\n",URLBase); 
	  printf("<INPUT TYPE=\"HIDDEN\" NAME=\"feature\" VALUE=\"%d\">\n",iFeatNum);         
	  printf("<INPUT TYPE=\"HIDDEN\" NAME=\"evidence\" VALUE=\"%d\">\n",nEvidence);         
	  printf("<INPUT TYPE=\"HIDDEN\" NAME=\"ascbin\" VALUE=\"6\">\n");         
	  printf("<INPUT TYPE=\"HIDDEN\" NAME=\"maxaln\" VALUE=\"5\">\n");         
	  printf("<INPUT TYPE=\"HIDDEN\" NAME=\"seltype\" VALUE=\"2\">\n");         
	  printf("<INPUT TYPE=\"HIDDEN\" NAME=\"chn\" VALUE=\"0\">\n");         
	  printf("<INPUT TYPE=\"HIDDEN\" NAME=\"ato\" VALUE=\"1\">\n");         
          printf("<INPUT TYPE=\"HIDDEN\" NAME=\"uid\" VALUE=\"%s\">\n", cCDDid);
          printf("<INPUT TYPE=\"HIDDEN\" NAME=\"version\" VALUE=\"%s\">\n",dbversion);
          printf("<INPUT TYPE=\"SUBMIT\" VALUE=\"View Structure Evidence\">&nbsp; with Cn3D 4.1\n");
          printf("</FORM></TD></TR>\n");
          break;
        default:
	  break;
      }  
      if (pevidence->next) printf("<TR>\n");
      nEvidence++;
      pevidence = pevidence->next;
    }
    printf("</TABLE><HR>\n");
  }
  CDDSrvFoot(stdout);
  return(TRUE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
SeqIdPtr CddGetEvidenceSip(CddPtr pcdd, Int4 iMMDBid)
{
  SeqEntryPtr  sep;
  BioseqPtr    bsp;
  SeqIdPtr     sip = NULL, asip;
  BioseqSetPtr bssp;
  SeqAnnotPtr  annot;
  DbtagPtr     dbtp;  
  ObjectIdPtr  oidp;
  
  bssp = pcdd->sequences->data.ptrvalue;
  sep = bssp->seq_set;
  while (sep) {
    if (sep->choice == 1) {
      bsp = sep->data.ptrvalue;
      sip = bsp->id;
      annot = bsp->annot; 
      while (annot) {
        if (annot->type == 4) {
	  asip = annot->data;
	  while (asip) {
	    if (asip->choice == SEQID_GENERAL) {
	      dbtp = asip->data.ptrvalue;
	      if (Nlm_StrCmp(dbtp->db,"mmdb") == 0) {
	        oidp = dbtp->tag;
	        if (oidp->id == iMMDBid) return (sip);
	      }
	    }
	    asip = asip->next;
	  }
	}
        annot = annot->next;
      }  
    }
    sep = sep->next;
  }
  return NULL;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Find out which of the structures must be used for evidence visualization  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Int4 CddGetEvidenceMMDBId(CddPtr pcdd, Int4 iFeatNum, Int4 iEvidence)
{
  AlignAnnotPtr       aap;
  Int4                iF = 0;
  Int4                iE = 0;
  ValNodePtr          vnp, idp;
  BiostrucAnnotSetPtr basp;
  
  if (iEvidence < 0) return(-1);
  aap = pcdd->alignannot;
  while (aap) {
    if (iF == iFeatNum) {
      vnp = aap->evidence;
      while (vnp) {
        if (iE == iEvidence && vnp->choice == FeatureEvidence_bsannot) {
          basp = (BiostrucAnnotSetPtr) vnp->data.ptrvalue;
	  idp = basp->id;
	  while (idp) {
	    if (idp->choice == BiostrucId_mmdb_id) {
	      return(idp->data.intvalue);
	    }
	    idp = idp->next;
	  }
	}
        vnp = vnp->next; iE++;
      }
    }
    aap = aap->next; iF++;
  }
  return(-1);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Transcribe Structure Feature Evidence into a Cn3D user annotation         */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Cn3dUserAnnotationsPtr CddMakeUserAnnot(CddPtr pcdd, Int4 iFeatNum, Int4 iEvidence)
{
  AlignAnnotPtr            aap;
  BiostrucAnnotSetPtr      basp;
  BiostrucFeatureSetPtr    bfsp;
  BiostrucFeaturePtr       features;
  Cn3dUserAnnotationsPtr   cuasp;
  Cn3dUserAnnotationPtr    cuap;
  Cn3dObjectLocationPtr    colp;
  Cn3dMoleculeLocationPtr  cmlp, cmlpHead, cmlpTail;
  Cn3dResidueRangePtr      crrp, crrpTail;
  CharPtr                  descr;
  ChemGraphPntrsPtr        cgps;
  Int4                     iF = 0;
  Int4                     iE = 0;
  Int4                     iMMDBid, iFeatSetId;
  Int4                     iMolId, iFrom, iTo;
  ResidueIntervalPntrPtr   ripp;
  ValNodePtr               vnp, idp, vnpdesc, vnploc, resptrs;
  Char                     cTemp[PATH_MAX];
  
  cmlpHead = NULL; cmlpTail = NULL; crrpTail = NULL;
  if (iEvidence < 0) return(NULL);
  cuasp = (Cn3dUserAnnotationsPtr) Cn3dUserAnnotationsNew();
  cuap = Cn3dUserAnnotationNew();
  cuasp->annotations = cuap;
  sprintf(cTemp,"Feature %d",iFeatNum+1);
  cuap->name = StringSave(cTemp);
  cuap->style_id = 1;
  cuap->is_on = TRUE;
  colp = Cn3dObjectLocationNew();
  cuap->residues = colp;
  aap = pcdd->alignannot;
  while (aap) {
    if (iF == iFeatNum) {
      vnp = aap->evidence;
      while (vnp) {
        if (iE == iEvidence && vnp->choice == FeatureEvidence_bsannot) {
          basp = (BiostrucAnnotSetPtr) vnp->data.ptrvalue;
	  idp = basp->id;
	  while (idp) {
	    if (idp->choice == BiostrucId_mmdb_id) {
              iMMDBid = idp->data.intvalue;
	      colp->structure_id = ValNodeNew(NULL);
	      colp->structure_id->choice = BiostrucId_mmdb_id;
	      colp->structure_id->data.intvalue = iMMDBid;
	      break;
	    }
	    idp = idp->next;
	  }
	  bfsp = basp->features;
	  vnpdesc = bfsp->descr;
	  while (vnpdesc) {
	    if (vnpdesc->choice == BiostrucFeatureSetDescr_name) {
	      descr = (CharPtr) vnpdesc->data.ptrvalue;
	      cuap->description = StringSave(descr);
	    }
	    vnpdesc = vnpdesc->next;
	  }
	  iFeatSetId = bfsp->id;
	  features = bfsp->features;
	  vnploc = features->Location_location;
	  if (vnploc->choice == Location_location_subgraph) {
	    cgps = vnploc->data.ptrvalue;
	    if (cgps->choice == ChemGraphPntrs_residues) {
	      resptrs = cgps->data.ptrvalue;
	      if (resptrs->choice == ResiduePntrs_interval) {
	        ripp = resptrs->data.ptrvalue;
		while (ripp) {
		  iMolId = ripp->molecule_id;
		  iFrom  = ripp->from;
		  iTo    = ripp->to;
		  crrp = Cn3dResidueRangeNew();
		  crrp->from = iFrom;
		  crrp->to = iTo;
		  crrp->next = NULL;
		  if (cmlpTail) {
		    if (iMolId == cmlpTail->molecule_id) {
		      if (crrpTail) {
		        crrpTail->next = crrp;
		        crrpTail = crrp;
		      }
		    } else {
		      cmlp = Cn3dMoleculeLocationNew();
		      if (!cmlpHead) cmlpHead = cmlp;
		      cmlp->molecule_id = iMolId;
                      cmlp->residues = crrp;
		      cmlpTail->next = cmlp;
		      cmlpTail = cmlp;
		      crrpTail = crrp;
		    }
		  } else {
		    cmlp = Cn3dMoleculeLocationNew();
		    if (!cmlpHead) cmlpHead = cmlp;
		    cmlp->molecule_id = iMolId;
                    cmlp->residues = crrp;
		    cmlpTail = cmlp;
		    crrpTail = crrp;
		  }
		  ripp = ripp->next;
		}
	      }
	    }
	  }
	}
        vnp = vnp->next; iE++;
      }
    }
    aap = aap->next; iF++;
  }
  colp->residues = cmlpHead;
  return(cuasp);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CddWWWReport(WWWInfoPtr www_info)
{
  Int4            i;
  WWWInfoDataPtr  wip;
  
  if ((wip = (WWWInfoDataPtr) www_info) == NULL) {
    CddHtmlError("No Info Data at all!");
  }
  
  CDDSrvHead(stdout,"CDD Server Error");
  printf("<BR>Method: %d (0..CmdLine, 1..GET, 2..POST, 3..FORM_DATA)<BR>\n",wip->method);
  printf("Port: %d<BR>\n",wip->port);
  printf("Server_Name: %s<BR>\n",wip->server_name);
  printf("Doc_Root: %s<BR>\n",wip->doc_root);
  printf("Script_Name: %s<BR>\n",wip->script_name);
  printf("Host: %s<BR>\n",wip->host);
  printf("Address: %s<BR>\n",wip->address);
  printf("Proxied_Ip: %s<BR>\n",wip->proxied_ip);
  printf("Agent: %s<BR>\n",wip->agent);
  if (wip->query) {
    printf("Query: %s<BR>\n",wip->query);
  } else printf("Query:<BR>\n");
  printf("Query_Len: %d<BR>\n",wip->query_len);
  printf("Browser: %d<BR>\n",wip->browser);
  printf("Num_Entries: %d<BR>\n",wip->num_entries);
  for (i=0;i<wip->num_entries;i++) {
    printf("  Name: %s<BR>\n",wip->entries[i]->name);
    printf("  Val: %s<BR>\n",wip->entries[i]->val);
    printf("  Size: %d\n",wip->entries[i]->size);
  }
  if (getenv("CONTENT_LENGTH") != NULL) {
    printf("<BR><BR>CONTENT_LENGTH: %s<BR>\n",getenv("CONTENT_LENGTH"));
  } else printf("<BR><BR>environment variable CONTENT_LENGTH not set!<BR>\n");
  if (getenv("REQUEST_METHOD") != NULL) {
    printf("<BR><BR>REQUEST_METHOD: %s<BR>\n",getenv("REQUEST_METHOD"));
  } else printf("<BR><BR>environment variable REQUEST_METHOD not set!<BR>\n");
  CDDSrvFoot(stdout);
  return;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* change format of stored sequences if necessary, for CDDAlignView          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CddFixSequenceFormat(SeqEntryPtr sep)
{
  SeqEntryPtr  sepThis;
  BioseqPtr    bsp;

  sepThis = sep;
  while (sepThis) {
    if (sepThis->choice == 1) {
      bsp = sepThis->data.ptrvalue;
      if (bsp->seq_data_type != Seq_code_ncbieaa) {
        BioseqRawConvert(bsp, Seq_code_ncbieaa);
      }
    }
    sepThis = sepThis->next;
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* retrieve a CD from CDtrack's published table, if available                */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static CddPtr CddGetFromCDtrack(Int4 iPssmId, CharPtr cCDDId)
{
  CddPtr          pcdd          = NULL;
  Char            errmsg[1024];

  if (iPssmId > 0) {
    pcdd=RetrievePublishedCdBlobByPssmId(CDTRKDBS,"bauer",(Uint4) iPssmId,FALSE,TRUE,errmsg,1024);
  } else {
    pcdd=RetrievePublishedCdBlobByAcc(CDTRKDBS,"bauer",cCDDid,0,FALSE,TRUE,errmsg,1024);
  }
/*  if (!pcdd) CddHtmlError(errmsg); */
  return pcdd;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* graphical display of family relationships for the CD-Server               */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CddDrawFamilyTree(CharPtr pc)
{
  Int4                 gw, gh, numblks, numlines;
  Int4                 white, black, red, blue;
  Int4                 i, ulx, uly, lrx, lry, blwidth, self;
  gdImagePtr           im;
  CharPtr              name;
  

  gw      = (Int4) atoi(strtok(pc,","));
  gh      = (Int4) atoi(strtok(NULL,","));
  im = gdImageCreate(gw,gh);
  white = gdImageColorAllocate(im, 255, 255, 255);
  black = gdImageColorAllocate(im,   0,   0,   0);
  red   = gdImageColorAllocate(im, 255,  51,  51);
  blue  = gdImageColorAllocate(im, 102, 102, 255);
  numblks = (Int4) atoi(strtok(NULL,","));
  for (i=0;i<numblks;i++) {
    ulx = (Int4) atoi(strtok(NULL,","));
    uly = (Int4) atoi(strtok(NULL,","));
    blwidth = (Int4) atoi(strtok(NULL,","));
    self = (Int4) atoi(strtok(NULL,","));
    name = StringSave(strtok(NULL,","));
    lrx = ulx + blwidth;
    lry = uly + CDD_GRAPH_HEIGHT;
    if (self) {
      gdImageRoundRectangle(im,ulx,uly,lrx,lry,5,3,red,1);    
    } else {
      gdImageRoundRectangle(im,ulx,uly,lrx,lry,5,3,blue,1);    
    }
    ulx = ulx + (lrx - ulx)/2;
    uly = uly + (lry - uly)/2;
    gdImageString(im,gdFont5X8,ulx-(strlen(name)*gdFont5X8->w/2),uly-gdFont5X8->h/2,name,white);
  }
  numlines = (Int4) atoi(strtok(NULL,","));
  for (i=0;i<numlines;i++) {
    ulx = (Int4) atoi(strtok(NULL,","));
    uly = (Int4) atoi(strtok(NULL,","));
    lrx = (Int4) atoi(strtok(NULL,","));
    lry = (Int4) atoi(strtok(NULL,","));
    gdImageLine(im,ulx,uly,lrx,lry,black);
  }
  printf("Content-type: image/gif\r\n\r\n");
  gdImageGif(im,stdout);
  gdImageDestroy(im);
  exit(0);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* remove notes, book references, and new-style parents from a published CD  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CddSrvRemoveNotes(CddPtr pcdd)
{
  CddDescrPtr  pcddsc, pcddsc_last;
  
  pcddsc_last = NULL;
  pcddsc = pcdd->description;
  while (pcddsc) {
    if (pcddsc->choice != CddDescr_scrapbook) {
      if (pcddsc_last) pcddsc_last->next = pcddsc;
      pcddsc_last = pcddsc;
    }
    pcddsc = pcddsc->next;
  }
  pcddsc_last = NULL;
  pcddsc = pcdd->description;
  while (pcddsc) {
    if (pcddsc->choice != CddDescr_book_ref) {
      if (pcddsc_last) pcddsc_last->next = pcddsc;
      pcddsc_last = pcddsc;
    }
    pcddsc = pcddsc->next;
  }
  if (NULL != pcdd->ancestors) pcdd->ancestors = NULL;
  if (NULL != pcdd->scoreparams) pcdd->scoreparams = NULL;
  if (NULL != pcdd->seqtree) pcdd->seqtree = NULL;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* return the consensus sequence as a Seq-Entry                              */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CddSrvReturnConsensus(CddPtr pcdd, Int4 iPssmId)
{

  AsnIoPtr              aip;
  SeqEntryPtr           sep, sepThis;
  BioseqSetPtr          bssp;
  BioseqPtr             bsp;
  SeqIdPtr              sip, sipNew;
  DbtagPtr              dbtp;
  ObjectIdPtr           oidp;
  
  bssp = (BioseqSetPtr) pcdd->sequences->data.ptrvalue;
  sepThis = (SeqEntryPtr) bssp->seq_set;
  while (sepThis) {
    bsp = sepThis->data.ptrvalue;
    sip = bsp->id;
    if (SipIsConsensus(sip)) {
      sep = sepThis;
      sep->next = NULL;
      if (iPssmId > 0) {   /* add a SeqId to consensus, which records PSSMid */
        sipNew = (SeqIdPtr) ValNodeNew(NULL);
	sipNew->choice = SEQID_GENERAL;
	dbtp = (DbtagPtr) DbtagNew();
	dbtp->db = StringSave("CDD");
	oidp = ObjectIdNew();
	oidp->id = iPssmId;
	dbtp->tag = oidp;
	sipNew->data.ptrvalue = dbtp;
	sipNew->next = sip;
	bsp->id = sipNew;
      }
      break;
    }
    sepThis = sepThis->next;
  }
  strcpy(OutputName,GetTempName("cddsrv"));
  aip = AsnIoOpen(OutputName, "w");
  SeqEntryAsnWrite((SeqEntryPtr) sep, aip, NULL);
  AsnIoClose(aip);
  printf("Content-type: text/html\n\n");
  PrintFile(OutputName);
  RemoveTempFiles();   
  return;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* MAIN Function for cddserver                                               */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Int2 Main()
{
  AsnIoPtr                 paiFile;
  AsnIoMemPtr              aimp;
  BioseqPtr                bsp, bspQuery      = NULL;
  BioseqPtr                bspRet;
  BioseqSetPtr             bssp;
  BiostrucAlignPtr         pbsaStruct;
  BiostrucAlignSeqPtr      pbsaSeq;
  BiostrucAnnotSetPtr      pbsa = NULL;
  BiostrucAnnotSetPtr      pbsaShort = NULL;
  BiostrucSeqsAlignsCddPtr pbsaCdd = NULL;
  BiostrucFeaturePtr       pbsf, pbsfCopy, pbsfTail;
  BiostrucIdPtr            pbsi;
  BiostrucPtr              pbsMaster, pbsSlave;
  BiostrucPtr              pbsSlaveHead       = NULL;
  BiostrucPtr              pbsSlaveTail, pbsTemp;
  BiostrucSeqsPtr          pbsaStrSeq;
  BytePtr                  buf;
  CddPtr                   pcdd               = NULL;
  CddSumPtr                pcds               = NULL;
  CddSumPtr                pcdsCopy, pcdsTail, pcdsHead;
  CddSumPtr                pcdsThis           = NULL;
  CdTreeNodePtr            pcdtree            = NULL;
  CharPtr                  Name, tempchar;
  CharPtr                  outptr             = NULL;
  CharPtr                  www_arg, cPart, blast_program, blast_database;
  CharPtr                  QuerySeq           = NULL;
  CharPtr                  QueryAlign         = NULL;
  CharPtr                  QueryName          = NULL;
  DenseDiagPtr             ddp;
  DenseDiagPtr             ddpQuery, ddpQTail;
  Int4Ptr                  iGiList            = NULL;
  Int4Ptr                  iTaxids;
  NcbiMimeAsn1Ptr          pvnNcbi;
  ObjectIdPtr              oidp;
  PDBSeqIdPtr              pdb_seq_id;
  ScorePtr                 psc;
  ValNodePtr               psadsad = NULL;
  SeqAlignPtr              salpHead, salpTemp;
  SeqAlignPtr              salpTail;
  SeqAlignPtr              salpCopy, salpFlat;
  SeqAlignPtr              salpQuery          = NULL;
  SeqAnnotPtr              psaCAlignHead      = NULL;
  SeqAnnotPtr              sap;
  SeqEntryPtr              sep, sequences     = NULL;
  SeqEntryPtr              sepQuery           = NULL;
  SeqIdPtr                 sip, sipNew, sipQuery, sipRet;
  SeqIdPtr                 sipMaster          = NULL;
  SeqIdPtr                 sipEvidence        = NULL;
  SeqIntPtr                sintp;
  ValNodePtr               pvnGi              = NULL;
  ValNodePtr               pvnGis             = NULL;
  ValNodePtr               txids              = NULL;
  ValNodePtr               other_returns, error_returns;
  WWWInfoPtr               www_info;
  Boolean                  bSelect            = FALSE;
  Boolean                  bAtom              = FALSE;
  Boolean                  bChain             = FALSE;
  Boolean                  bFix               = TRUE;
  Boolean                  bHaveMaster        = FALSE;
  Boolean                  bHave3dRep         = FALSE;
  Boolean                  bMode              = CDDALIGNMENT;
  Boolean                  bTax               = FALSE;
  Boolean                  bBlast             = FALSE;
  Boolean                  bIsOasis           = FALSE;
  Boolean                  bWantedPick        = FALSE;
  Boolean                  bHasPdb            = FALSE;
  Boolean                  is_network, bRemove;
  Boolean                  bHasConsensus      = FALSE;
  Boolean                  bShowTax           = FALSE;
  Boolean                  bEvidenceViewer    = FALSE;
  Boolean                  bConsensusOnly     = FALSE;
  Char                     CDDalign[PATH_MAX], CDDidx[PATH_MAX];
  Char                     CDDfile[PATH_MAX], ErrMsg[PATH_MAX];
  Char                     chain[2], cChain;
  Char                     cMode;
  Char                     dbversion[6];
  Char                     szName[5];
  Char                     errmsg[1024];
  Int2                     Qstatus;
  Int2                     iPDB               = 2;
  Int2                     iSeqStrMode        = CDDSEQUONLY; 
  Int4                     i3dRepIndex        = 1;
  Int4                     Gi, i, iQueryGi    = -1;
  Int4                     iCddSize           = 0;
  Int4                     iIndex;
  Int4                     iMaxAln            = 10;
  Int4                     iSelType           = 2;
  Int4                     iModelComplexity   = ONECOORDRES;
  Int4                     iNsegments, iRepId = 0;
  Int4                     indx, iPssmId      = 0;
  Int4                     iTaxId             = 0;
  Int4                     NumLabels          = 0;
  Int4                     nGi                = 0;
  Int4                     nPdb               = 0;
  Int4                     iFeatNum           = 0;
  Int4                     iEvidence          = -1;
  Int4                     iEvidenceMMDBId    = -1;
  Int4                     alen               = 0;
  Int4                     nTaxIds, tempuid;
  Int4Ptr                  pStyles[2];
  struct rlimit            rl;\
  FILE                    *fp;
  FloatHi                  tbit               = 2.0;
  Uint2                    pwidth             = 60;

/*---------------------------------------------------------------------------*/
/* Yanli's fix for making binary reading work                                */
/*---------------------------------------------------------------------------*/
  objmmdb1AsnLoad(); objmmdb2AsnLoad(); objmmdb3AsnLoad();

/*---------------------------------------------------------------------------*/
/* this sets up the unix time limit                                          */
/*---------------------------------------------------------------------------*/
  getrlimit(RLIMIT_CPU, &rl);
  rl.rlim_max = rl.rlim_cur = CPUTIME_MAX;
  setrlimit(RLIMIT_CPU, &rl);

/*---------------------------------------------------------------------------*/
/* retrieve names for directories etc.                                       */
/*---------------------------------------------------------------------------*/
  if (!CddGetParams()) CddHtmlError("Couldn't read from config file...");
  Nlm_StrCpy(CDDidx,DATApath);
  Nlm_StrCat(CDDidx,"/cdd.idx");

/*---------------------------------------------------------------------------*/
/* Get ready for sequence retrieval                                          */
/*---------------------------------------------------------------------------*/
/*  if (!PUBSEQBioseqFetchEnable("cddsrv", TRUE))
    CddHtmlError("Unable to enable PUBSEQBioseqFetch"); */
  if (!PubSeqFetchEnable())
    CddHtmlError("Unable to enable PubSeqFetch");

/*---------------------------------------------------------------------------*/
/* Begin processing www information block                                    */
/*---------------------------------------------------------------------------*/
  if (WWWGetArgs(&www_info) != WWWErrOk)
    CddHtmlError("Failed to process posting - check your get/post syntax.");    


/*---------------------------------------------------------------------------*/
/* debugging code - look at the content of the WWW request at this point     */
/*---------------------------------------------------------------------------*/
/*  CddWWWReport(www_info);
  exit(0); */

  if ((NumLabels = WWWGetNumEntries(www_info)) == 0) {
    CddHtmlError("No input - nothing to report.");
  }
/*---------------------------------------------------------------------------*/
/* CD-Server as image formatter for family relationships                     */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "PIC")) >= 0) {
    www_arg =  WWWGetValueByIndex(www_info, indx);
    CddDrawFamilyTree(www_arg);
    exit(0);
  }

/*---------------------------------------------------------------------------*/
/* CD-Server as server for consensus sequences (formatted as SeqEntry)       */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "GETCSEQ")) >= 0) {
    bConsensusOnly = TRUE;
  }

/*---------------------------------------------------------------------------*/
/* retrieve the Cdd unique identifier                                        */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "uid")) < 0) 
    CddHtmlError("No accession (CDD-ID) was input - nothing to report.");
  www_arg =  WWWGetValueByIndex(www_info, indx);
/*---------------------------------------------------------------------------*/
/* check to see if identifier is entirely numerical - if so, convert to acc. */
/*---------------------------------------------------------------------------*/
#ifdef USE_CDTRK
  CdTrkInitialize(TRUE);
/*  sprintf(ErrMsg,"ODBCINI=%s",ODBCINI);
  putenv(ErrMsg); */
#endif
  iPssmId = (Int4) atoi(www_arg);
  if (iPssmId > 0) {                 /* successful conversion, uid is PSSMid */
#ifdef USE_CDTRK
    PssmId2Accession(CDTRKDBS,"bauer",iPssmId,cCDDid,TRUE,errmsg,1024);
    if (errmsg[0] != '\0') {
      /*sprintf(ErrMsg,"Got Accession %s from CDtrack via PSSMId %d: %s\n",cCDDid,iPssmId, errmsg);
      CddHtmlError(ErrMsg); */
      CddAccFromPssmId(iPssmId, cCDDid, CDDidx);     
    }
#else
    CddAccFromPssmId(iPssmId, cCDDid, CDDidx);     
#endif
  } else {
    strcpy(cCDDid,www_arg);          /* uid was an accession, no PSSMid known*/
#ifdef USE_CDTRK
    iPssmId = Accession2PssmId(CDTRKDBS,"bauer",cCDDid,TRUE,errmsg,1024);
    if (errmsg[0] != '\0') {
      /*sprintf(ErrMsg,"Used Accession %s in CDtrack to retrieve PSSMId %d: %s\n",cCDDid,iPssmId, errmsg);
      CddHtmlError(ErrMsg); */
      CddPssmIdFromAcc(&iPssmId, cCDDid, CDDidx);
    }
#else
    CddPssmIdFromAcc(&iPssmId, cCDDid, CDDidx);
#endif
  }

/*---------------------------------------------------------------------------*/
/* retrieve the database version string                                      */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "version")) < 0) {
    strcpy(dbversion,CDDefault);
  } else {
    www_arg = WWWGetValueByIndex(www_info, indx);
    strcpy(dbversion,www_arg);
  }
/*---------------------------------------------------------------------------*/
/* kludge to reset version number to 1.51 if an OLD Smart accession is used  */
/*---------------------------------------------------------------------------*/
  if (StringNCmp(cCDDid,"smart0",6)) {
    if (StringNCmp(cCDDid,"pfam",4)) {
      if (StringNCmp(cCDDid,"LOAD_",5)) {
        if (StringNCmp(cCDDid,"COG",3)) {
          if (StringNCmp(cCDDid,"KOG",3)) {
            if (StringNCmp(cCDDid,"cd0",3)) {
              if (StringCmp(dbversion,"v1.51") &&
	          StringCmp(dbversion,"v1.50") &&
	          StringCmp(dbversion,"v1.01") &&
	          StringCmp(dbversion,"v1.00")) {
		CddHtmlError("This type of accession is not supported with newer versions of CDD!");
                Nlm_StrCpy(dbversion,"v1.51"); 
              }
            }
          }
        }
      }
    }
  }

/*---------------------------------------------------------------------------*/
/* read in the CDD data structure and fill in the pcds linked list           */
/*---------------------------------------------------------------------------*/
/* access the information contained in the master CDD . No need to continue  */
/* if this file can not be found/accessed                                    */
/*---------------------------------------------------------------------------*/
  if (Nlm_StrCmp(CDDdbtype,"oasis")==0 || Nlm_StrCmp(CDDdbtype,"OASIS")==0 ||
      Nlm_StrCmp(CDDdbtype,"Oasis")==0 || Nlm_StrCmp(CDDdbtype,"oAsIs")==0) {
    bIsOasis = TRUE;
  }
  strcpy(CDDalign,CDDPrefix);
  strcat(CDDalign,dbversion);
  if (bIsOasis) strcat(CDDalign,CDDPost_O); else strcat(CDDalign, CDDPost_C);
  strcat(CDDalign,"/");
  strcat(CDDalign,cCDDid);
  strcat(CDDalign,CDDextens);
  if (Nlm_StrNCmp(cCDDid,"cd",2) == 0) bIsOasis = FALSE;

/*---------------------------------------------------------------------------*/
/* changed reading of the CD asn.1 to BINARY, 6/12/00, as v1.00 is near      */
/*---------------------------------------------------------------------------*/
#ifdef USE_CDTRK
  pcdd = CddGetFromCDtrack(iPssmId,cCDDid);
  if (pcdd) {
    tempchar = StringSave(cCDDid);
    pcdtree = GetPublishedCDFamilyInfoForAcc(CDTRKDBS,"bauer",(char *) tempchar,TRUE,errmsg,1024);
    MemFree(tempchar);
    if (!pcdtree) CddHtmlError(errmsg);
    if (NULL == pcdtree->next) pcdtree = NULL;
  }
#endif
#ifndef DRAW_TREES
  pcdtree = NULL;
#endif

  if (!pcdd) {
    pcdd = (CddPtr) CddReadFromFile(CDDalign,TRUE);
    if (!pcdd) CddHtmlError("Could not access CDD data!");
  }
  CddSrvRemoveNotes(pcdd);
  bHasConsensus = CddHasConsensus(pcdd);
/*---------------------------------------------------------------------------*/
/* return consensus sequence if required                                     */
/*---------------------------------------------------------------------------*/
  if (bConsensusOnly) {
    if (bHasConsensus) {
      CddSrvReturnConsensus(pcdd,iPssmId);
    } else {
      CddHtmlError("This CD does not have a consensus sequence!");
    }
    exit(0);
  }

  alen = CddGetAlignmentLength(pcdd);

/*---------------------------------------------------------------------------*/
/* retrieve data required for using cddserver as a rps-blast output formatter*/
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info,"querynm")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    QueryName = StringSave(www_arg);
  }
  if ((indx = WWWFindName(www_info,"querygi")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info,indx);
    iQueryGi = (Int4) atoi(www_arg);
    sipRet = (SeqIdPtr) ValNodeNew(NULL);
    sipRet->choice = SEQID_GI;
    sipRet->data.intvalue = iQueryGi;
    bspRet = BioseqLockById(sipRet);
    if (!bspRet) CddHtmlError("Could not retrieve query sequence!");
    sipQuery = (SeqIdPtr) ValNodeNew(NULL);
    sipQuery->choice = SEQID_LOCAL;
    oidp = ObjectIdNew();
    if (QueryName) {
      oidp->str = StringSave(QueryName);
    } else {
      oidp->str = StringSave("query");
    }
    sipQuery->data.ptrvalue = oidp;
    sipQuery->next = NULL;
    bspQuery = (BioseqPtr) BioseqCopy(sipQuery,sipRet,0,bspRet->length-1,0,FALSE);
    BioseqUnlock(bspRet);
    sepQuery = SeqEntryNew();
    sepQuery->choice = 1;
    sepQuery->data.ptrvalue = bspQuery;    
  } else if ((indx = WWWFindName(www_info,"query")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info,indx);
    if((sepQuery=FastaToSeqBuffEx(www_arg, &outptr,FALSE,NULL,FALSE))==NULL)
      CddHtmlError("Can not convert FASTA formatted sequence!");
    if(sepQuery->choice != 1) CddHtmlError("Conversion from FASTA failed!");
    QuerySeq = StringSave(www_arg);
    bspQuery = (BioseqPtr) sepQuery->data.ptrvalue;
    sipQuery = bspQuery->id;
    oidp = ObjectIdNew();
    if (QueryName) {
      oidp->str = StringSave(QueryName);
    } else {
      oidp->str = StringSave("query");
    }
    MemFree(sipQuery->data.ptrvalue);
    sipQuery->choice = SEQID_LOCAL;
    sipQuery->data.ptrvalue = oidp;
    sipQuery->next = NULL;
  } else if ((indx = WWWFindName(www_info, "queryrid")) >= 0) {
/*---------------------------------------------------------------------------*/
/* set the BLASTDB environment variable                                      */
/*---------------------------------------------------------------------------*/
    if (putenv("BLASTDB=/blast/db/blast")) {
      CddHtmlError("Error setting environment variable BLASTDB");
    }
    www_arg = WWWGetValueByIndex(www_info,indx);
    Qstatus = (Int2) QBlastGetResults(www_arg,&salpTemp,&bspQuery,&blast_program,&blast_database,&other_returns, &error_returns);
    if (Qstatus != 0) CddHtmlError("Could not retrieve query sequence from BLAST queue!");
    sipQuery = bspQuery->id;
    oidp = ObjectIdNew();
    if (QueryName) {
      oidp->str = StringSave(QueryName);
    } else {
      oidp->str = StringSave("query");
    }
    MemFree(sipQuery->data.ptrvalue);
    sipQuery->choice = SEQID_LOCAL;
    sipQuery->data.ptrvalue = oidp;
    sipQuery->next = NULL;
  }
  if (bspQuery) {
/*---------------------------------------------------------------------------*/
/* add query sequence to the linked list of bioseqs in the CD                */
/*---------------------------------------------------------------------------*/
    bssp = (BioseqSetPtr) pcdd->sequences->data.ptrvalue;
    ValNodeLink(&(bssp->seq_set),sepQuery);
    SeqMgrAdd(OBJ_BIOSEQ,bspQuery);
    bBlast = TRUE;
  } else {
    bBlast = FALSE;
  }
  if ((indx = WWWFindName(www_info,"aln")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info,indx);
    QueryAlign = StringSave(www_arg);
    sintp = (SeqIntPtr) pcdd->profile_range;
    salpQuery = SeqAlignNew();
    salpQuery->dim = 2;
    salpQuery->type = SAT_PARTIAL;
    salpQuery->segtype = SAS_DENDIAG;
    salpQuery->next = NULL;
    cPart = strtok(www_arg,",");
    iNsegments = (Int4) atoi(cPart);
    for (i=0;i<iNsegments;i++) {
      ddpQuery = DenseDiagNew();
      ddpQuery->dim = 2;
      ddpQuery->starts = MemNew(2*sizeof(Int4));
      cPart = strtok(NULL,",");
      ddpQuery->starts[0] = sintp->from + (Int4)atoi(cPart);
      cPart = strtok(NULL,",");
      ddpQuery->starts[1] = (Int4)atoi(cPart);
      cPart = strtok(NULL,",");
      ddpQuery->len = (Int4)atoi(cPart);
      ddpQuery->next = NULL;
      if (!salpQuery->segs) {
        salpQuery->segs = ddpQuery;
        ddpQTail = ddpQuery;
      } else {
        ddpQTail->next = ddpQuery;
        ddpQTail = ddpQuery;
      }      
    }
    bBlast = TRUE;
  } else {
    bBlast = FALSE;
  }
  if (bBlast) {
    bMode = CDDALIGNMENT;
    iSeqStrMode = CDDSEQUONLY;
  }
/*---------------------------------------------------------------------------*/
/* retrieve parameters describing which kind of selection to make on the     */
/* sequences in the set                                                      */
/* iSelType:   0 .... do not use any of the selection mechanisms             */
/*             1 .... use first "maxaln" sequences, as listed in the CD      */
/*             2 .... use the most divergent subset size maxaln              */
/*             3 .... use the maxaln sequences most similar to query         */
/* if iMaxAln is 0 don't use any of these mechanisms (selection must be      */
/* "everything" or single sequences picked from the table)                   */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info,"maxaln")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info,indx);
    iMaxAln = (Int4) atoi(www_arg);
    bMode = CDDALIGNMENT;
  } else iMaxAln = 10;
  if ((indx = WWWFindName(www_info,"seltype")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info,indx);
    iSelType = (Int4) atoi(www_arg);
    if (iSelType == 0) bWantedPick = TRUE;
    if (iSelType < 0) iSelType = 0;
    if (iSelType > 3) iSelType = 3;
    if (!bBlast && iSelType==3) iSelType = 0;
    bMode = CDDALIGNMENT;
  } else iSelType = 2;

/*---------------------------------------------------------------------------*/
/* retrieve the list of gi's selected for visualization                      */
/*---------------------------------------------------------------------------*/
  if (bWantedPick) {
    if (!UseThisGi(0,pvnGis)) {
      bMode = CDDALIGNMENT;
      nGi++;
      pvnGi = ValNodeAddInt(&pvnGis,0,0);
      bSelect = TRUE;
    }
  }
  NumLabels = WWWGetNumEntries(www_info);
  for (indx = 0; indx < NumLabels; indx++) {
    Name = WWWGetNameByIndex(www_info, indx);
    if (StrICmp(Name, "gi") == 0) {
      www_arg = WWWGetValueByIndex(www_info, indx);
      if (isdigit(www_arg[0])) {
        Gi     = (Int4) atol(www_arg);
        if (!UseThisGi(Gi,pvnGis)) {
          bMode  = CDDALIGNMENT;
          nGi++;
          pvnGi  = ValNodeAddInt(&pvnGis, 0, Gi);
	  bSelect = TRUE;
	}
      } else CddHtmlError("Non-numeric identifier - no results.");
    }
  }

/*---------------------------------------------------------------------------*/
/* retrieve, if present, the indicator for which structure to use as master  */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "Tdrep")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) {
      i3dRepIndex = (Int4) atoi(www_arg);
    } else CddHtmlError("Non numerical parameter supplied for \"3drep\"");
  }

/*---------------------------------------------------------------------------*/
/* retrieve the boolean specifying whether the aligned chain only should be  */
/* displayed --- option no longer offered for visualization with Cn3d -----  */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "chn")) >= 0) {
    www_arg =  WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) {
      if (atoi(www_arg)) bChain = TRUE; 
      else bChain = FALSE;
    } else CddHtmlError("Non numerical parameter supplied for \"chn\"");
  }

/*---------------------------------------------------------------------------*/
/* retrieve the boolean specifying whether an all-atom model should be       */
/* displayed                                                                 */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "ato")) >= 0) {
    www_arg =  WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) {
      if (atoi(www_arg) > 0) bAtom = TRUE; 
      else bAtom = FALSE;
    } else CddHtmlError("Non numerical parameter supplied for \"ato\"");
  }
  if (bAtom) iModelComplexity = ONECOORDATOM;

/*---------------------------------------------------------------------------*/
/* retrieve the taxonomy subtree node                                        */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "tax")) >= 0) {
    www_arg =  WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) {
      iTaxId = (Int4) atoi(www_arg);  
      bTax = TRUE;
    } else CddHtmlError("Non numerical parameter supplied for \"tax\"");
  }

/*---------------------------------------------------------------------------*/
/* retrieve multiple taxon ids as returned by wwwcmt.cgi                     */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "taxon")) >= 0) {
    while (indx >= 0) {
      www_arg = WWWGetValueByIndex(www_info, indx);
/*      iTaxId = atoi(Nlm_StrTok(www_arg,":")); */
      iTaxId = atoi(www_arg);
      if (iTaxId >= 0) {
        bTax = TRUE;
        ValNodeAddInt(&txids,0,iTaxId);
      }
      indx = WWWFindNameEx(www_info, "taxon",indx+1);
    }
    iTaxId = -1;
    iMaxAln = -1;
  }

/*---------------------------------------------------------------------------*/
/* optionally, retrieve multiple taxon ids as returned by wwwcmt.cgi as "id" */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "id")) >= 0) {
    while (indx >= 0) {
      www_arg = WWWGetValueByIndex(www_info, indx);
      iTaxId = atoi(Nlm_StrTok(www_arg,":"));
      if (iTaxId >= 0) {
        bTax = TRUE;
        ValNodeAddInt(&txids,0,iTaxId);
      }
      indx = WWWFindNameEx(www_info, "id",indx+1);
    }
    iTaxId = -1;
    iMaxAln = -1;
  }

/*---------------------------------------------------------------------------*/
/* retrieve the informativeness threshold for alignment column coloring      */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "bit")) >= 0) {
    www_arg =  WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) {
      tbit = (FloatHi) atof(www_arg);  
    } else CddHtmlError("Non numerical parameter supplied for \"bit\"");
  }

/*---------------------------------------------------------------------------*/
/* retrieve the alignment display width                                      */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "pwidth")) >= 0) {
    www_arg =  WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) {
      pwidth = (Uint2) atoi(www_arg);  
    } else CddHtmlError("Non numerical parameter supplied for \"pwidth\"");
  }
/*---------------------------------------------------------------------------*/
/* retrieve feature highlight information                                    */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "feature")) >= 0) {
    www_arg =  WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) {
      iFeatNum = (Uint2) atoi(www_arg);
      if (iFeatNum < 0) iFeatNum = 0; 
    } else CddHtmlError("Non numerical parameter supplied for \"feature\"");
  }
  if ((indx = WWWFindName(www_info, "evidence")) >= 0) {
    www_arg =  WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) {
      iEvidence = (Uint2) atoi(www_arg);
      if (iEvidence < 0) iEvidence = 0;
      bEvidenceViewer = TRUE; 
    } else CddHtmlError("Non numerical parameter supplied for \"evidence\"");
  }
/*---------------------------------------------------------------------------*/
/* retrieve ascbin information as supplied by default viewing form           */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "3Dsub")) >= 0) {
    if ((indx = WWWFindName(www_info, "3Dopt")) >= 0) {
      www_arg =  WWWGetValueByIndex(www_info, indx);
      if (isdigit(www_arg[0])) {
        iPDB = (Int2) atoi(www_arg); 
        if (iPDB < 0) iPDB=0;
        if (iPDB > 7) iPDB=7;
        if (iPDB == 5) {
	  bMode = CDDSUMMARY;
	  iSeqStrMode = NOALIGN; 
	  iMaxAln = -1; 
	}
      } else CddHtmlError("Non numerical parameter supplied for \"3Dopt\"");
    }
  } else if ((indx = WWWFindName(www_info, "ALsub")) >= 0) {
    if ((indx = WWWFindName(www_info, "ALopt")) >= 0) {
      www_arg =  WWWGetValueByIndex(www_info, indx);
      if (isdigit(www_arg[0])) {
        iPDB = (Int2) atoi(www_arg); 
        if (iPDB < 0) iPDB=0;
        if (iPDB > 9) iPDB=9;
        if (iPDB == 5) {
	  bMode = CDDSUMMARY;
	  iSeqStrMode = NOALIGN; 
	  iMaxAln = -1; 
	}
      } else CddHtmlError("Non numerical parameter supplied for \"3Dopt\"");
    }
  }

/*---------------------------------------------------------------------------*/
/* retrieve the boolean specifying whether the results shall be dumped as    */
/* binary or ascii data                                                      */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "ascbin")) >= 0) {
    www_arg =  WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) {
      iPDB = (Int2) atoi(www_arg); 
      if (iPDB < 0) iPDB=0;
      if (iPDB > 9) iPDB=9;
      if (iPDB == 5) {
        bMode = CDDSUMMARY;
	iSeqStrMode = NOALIGN;
	iMaxAln = -1; 
      }
    } else CddHtmlError("Non numerical parameter supplied for \"ascbin\"");
  }
  
/*---------------------------------------------------------------------------*/
/* retrieve the boolean specifying whether tax. should be displayed explic.  */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "showtax")) >= 0) {
    www_arg =  WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) {
      bShowTax = (Boolean) atoi(www_arg); 
    } else CddHtmlError("Non numerical parameter supplied for \"showtax\"");
  }
  
  
/*---------------------------------------------------------------------------*/
/* end of WWW-parameter parsing block                                        */
/* the only parameter retrieved later is cMode, overrides default alignment  */
/* display settings when dealing with multiple structure CD's (CDDproper..)  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* if the Cdd contains an alignment, fill the pcds data structure            */
/*---------------------------------------------------------------------------*/
  if (pcdd->seqannot) {
    bssp = (BioseqSetPtr) pcdd->sequences->data.ptrvalue;
    salpHead = (SeqAlignPtr) pcdd->seqannot->data;
    if (salpHead->dim !=2)  CddHtmlError("Wrong dimension in Cdd alignment!");
    while (salpHead) {
      iIndex = 0;
      ddp = (DenseDiagPtr) salpHead->segs; sip = ddp->id;
      while(sip) {
        if (iIndex > 0 || !bHaveMaster) {
          pcdsThis = CddSumNew();
	  if (!bHaveMaster) pcdsThis->sip = SeqIdDup(sip);
          else pcdsThis->sip = sip;
	  pcdsThis->salp = salpHead;
	  pcdsThis->iCddIdx = iCddSize++;
          if (sip->choice == SEQID_PDB) {
	    bHasPdb = TRUE;
            pcdsThis->bIsPdb = TRUE;
            pdb_seq_id = (PDBSeqIdPtr) GetPdbSeqId(sip);
            pcdsThis->cChainId[0] = pdb_seq_id->chain; pcdsThis->cChainId[1] = '\0';
            pcdsThis->cPdbId[0] = pdb_seq_id->mol[0]; pcdsThis->cPdbId[1] = pdb_seq_id->mol[1];
            pcdsThis->cPdbId[2] = pdb_seq_id->mol[2]; pcdsThis->cPdbId[3] = pdb_seq_id->mol[3];
            pcdsThis->cPdbId[4] = '\0';
	    if (!bHave3dRep && pcdsThis->iCddIdx == i3dRepIndex) {
	      pcdsThis->bIs3dRep = TRUE; /* first 3D-seq is 3D representative*/
	      bHave3dRep = TRUE;
	    }
          } else {
            pcdsThis->bIsPdb = FALSE;
            if (pcdsThis->sip->choice == SEQID_GI)
	      pcdsThis->uid = sip->data.intvalue;
	    else pcdsThis->uid = -1;
          }
          if (!bHaveMaster && iIndex == 0) {
            bHaveMaster = TRUE;
            pcdsThis->bIsMaster = TRUE;
          }
          CddSumLink(&(pcds),pcdsThis);
        } 
        iIndex++;
        sip = sip->next;
      }
      salpHead = salpHead->next;
    }
/* for Blast alignment visualization, add pcds */
    if (bBlast) {      
      pcdsThis = CddSumNew();
      pcdsThis->sip = SeqIdDup(sipQuery);
      pcdsThis->bIsPdb = FALSE;
      pcdsThis->uid = 0;
      pcdsThis->bIsMaster = FALSE;
      pcdsThis->iCddIdx = iCddSize + 1;
      pcdsThis->next = NULL;
      CddSumLink(&(pcds),pcdsThis);
    }
  } else bMode = CDDSUMMARY;       /* can't show alignment if none available */

/*---------------------------------------------------------------------------*/
/* retrieve list of taxonomy id's for all sequences in the CDD alignment     */
/*---------------------------------------------------------------------------*/
  iTaxids = CddGetTaxIds(&nTaxIds,pcds);

/*---------------------------------------------------------------------------*/
/* fix problem with displaying CDs that have no consensus, 3D structures,    */
/* but a regular sequence as the representative/master                       */
/*---------------------------------------------------------------------------*/
  if (!bHave3dRep) bHasPdb = FALSE;
  if (!bHasConsensus && !pcds->bIsPdb) bHasPdb=FALSE;

/*---------------------------------------------------------------------------*/
/* in case we're dealing with Blast output formatting, add alignment on top  */
/* Must take care of query sequence later                                    */
/*---------------------------------------------------------------------------*/
  if (bBlast) {
    ddp = salpQuery->segs;
    while (ddp) {                     /* need to properly allocate SeqIdPtrs */
      sipMaster = SeqIdDup(pcds->sip);
      sipNew = SeqIdDup(sipQuery);
      ASSERT(FALSE == CddSameSip(sipMaster,sipNew));
      sipMaster->next = sipNew;
      sipNew->next = NULL;
      ddp->id = sipMaster;
      ddp = ddp->next; 
    }
    salpQuery->next = (SeqAlignPtr) pcdd->seqannot->data;
    pcdsThis->salp = salpQuery;
  }

/*---------------------------------------------------------------------------*/
/* reduce number of sequences pulled out if the query needs to be included   */
/*---------------------------------------------------------------------------*/
  if (iMaxAln>iCddSize  || iMaxAln<0) iMaxAln = iCddSize;
  if (iMaxAln==iCddSize && iSelType > 1) iSelType = 1; 
  if (bBlast && iMaxAln && iMaxAln < iCddSize) iMaxAln--;

/*---------------------------------------------------------------------------*/
/* generate a list of "GI's" in case the most divergent set is selected      */
/*---------------------------------------------------------------------------*/
  if (bMode != CDDSUMMARY && iMaxAln && iSelType == 2) {
    if (iMaxAln > MAXDIV) iMaxAln = MAXDIV;
    iGiList = (Int4Ptr) CddMostDiverse(pcdd->distance,iMaxAln,MAXDIV);
    if (iGiList) {
      for (indx =0; indx < iMaxAln; indx++) {
        if (!UseThisGi(iGiList[indx],pvnGis)) {
	  nGi ++;
	  pvnGi  = ValNodeAddInt(&pvnGis, 0, iGiList[indx]);
	}
      }
      if (!UseThisGi(0,pvnGis)) {
	pvnGi  = ValNodeAddInt(&pvnGis, 0, 0);
      }
      free(iGiList);
    } 
  }

/*---------------------------------------------------------------------------*/
/* generate a list of "GI's" in case the most similar set is selected        */
/*---------------------------------------------------------------------------*/
  if (bMode != CDDSUMMARY && iMaxAln && iSelType == 3 && bBlast) {
    psc = (ScorePtr) CddCalculateQuerySim(pcdd,salpQuery);
    if (psc) {
      iGiList = (Int4Ptr) CddMostSimilarToQuery(psc,iMaxAln);
      ScoreSetFree(psc);
      if (iGiList) {
        for (indx =0; indx < iMaxAln; indx++) {
          if (!UseThisGi(iGiList[indx],pvnGis)) {
	    nGi ++;
  	    pvnGi  = ValNodeAddInt(&pvnGis, 0, iGiList[indx]);
	  }
        }
        if (!UseThisGi(0,pvnGis)) {
	  pvnGi  = ValNodeAddInt(&pvnGis, 0, 0);
        }
        free(iGiList);
      }
    }
  }
/*---------------------------------------------------------------------------*/
/* generate a list of "GI's" in case a simple max. number is selected        */
/*---------------------------------------------------------------------------*/
  if (bMode != CDDSUMMARY && iMaxAln && iSelType == 1) {
    for (indx = 0; indx < iMaxAln; indx++) {
      if (!UseThisGi(indx,pvnGis)) {
        nGi ++;
	pvnGi  = ValNodeAddInt(&pvnGis, 0, indx);
      }
    }
  }

/*---------------------------------------------------------------------------*/
/* If used as a Cn3D-4.0 structure evidence viewer, make sure the right      */
/* structure is in the "gi-list" and add it if it's missing                  */
/*---------------------------------------------------------------------------*/
  if (bEvidenceViewer) {
    bHaveMaster = FALSE;
    iEvidenceMMDBId = CddGetEvidenceMMDBId(pcdd, iFeatNum, iEvidence);  
    sipEvidence = CddGetEvidenceSip(pcdd, iEvidenceMMDBId);
    pcdsThis = pcds;
    while (pcdsThis) {
      if (!bHaveMaster && CddSameSip(sipEvidence,pcdsThis->sip)) {
        if (!UseThisGi(pcdsThis->iCddIdx,pvnGis)) {
          nGi++;
	  pvnGi = ValNodeAddInt(&pvnGis, 0, pcdsThis->iCddIdx);
        }
	pcdsThis->bIs3dRep = TRUE;
	iRepId = pcdsThis->iMMDBId;
	i3dRepIndex = pcdsThis->iCddIdx;
	bHaveMaster = TRUE;
      } else {
	pcdsThis->bIs3dRep = FALSE;
      }
      pcdsThis = pcdsThis->next;
    }
  }

/*---------------------------------------------------------------------------*/
/* if no GI's are selected for alignment display reset the mode to listing   */
/*---------------------------------------------------------------------------*/
  if ((nGi==0 && !bBlast) || bMode==CDDSUMMARY) {
    iSeqStrMode = NOALIGN;
    bMode = CDDSUMMARY;
  }

/*---------------------------------------------------------------------------*/
/* this should happen if no slave is selected (only master gi listed)        */
/*---------------------------------------------------------------------------*/
  if ((bMode && nGi <= 1)) {
    CddHtmlError("Select at least one sequence from the table!");
  }
  
/*---------------------------------------------------------------------------*/
/* check whether the 3D representative is needed and included in the Gi list */
/*---------------------------------------------------------------------------*/
  if ((iPDB < 2 || iPDB == 6 || iPDB == 7) && !UseThisGi(i3dRepIndex,pvnGis)) {
    nGi ++;
    pvnGi = ValNodeAddInt(&pvnGis, 0, i3dRepIndex);
  }

/*---------------------------------------------------------------------------*/
/* default mode if more than one gi present is to show alignments, at least  */
/*---------------------------------------------------------------------------*/
  if ((nGi >= 2 || bBlast) && bMode && iPDB != 5) {
    iSeqStrMode = CDDSEQUONLY;
    bMode = CDDALIGNMENT;
  }

/*---------------------------------------------------------------------------*/
/* if CD has consensus, and 3D visualization is selected, need to transfer   */
/* alignment annotations to new master                                       */
/*---------------------------------------------------------------------------*/
  if ((bHasConsensus || bEvidenceViewer) && (iPDB < 2 || iPDB == 6 || iPDB == 7)) {
    pcdsThis = pcds; while (pcdsThis) {
      if (pcdsThis->bIs3dRep) {
        sipMaster = pcdsThis->sip;
        break;
      }
      pcdsThis = pcdsThis->next;
    }
    if (pcdd->alignannot)         /* move alignment annotation to new master */
      CddTransferAlignAnnot(pcdd->alignannot,sipMaster,pcdd->seqannot->data,bssp);
  }
  
/*---------------------------------------------------------------------------*/
/* The pcds data structure has been filled already. Now subset the alignments*/
/* selected for visualization and create the copy SeqAlign, pile up sequences*/
/*---------------------------------------------------------------------------*/
  pcdsThis = pcds; pcdsTail = NULL; pcdsHead = NULL; 
  salpCopy = NULL; salpTail = NULL; 
  nPdb = 0; iCddSize = 0;
  if (pcdd->seqannot) {
    if (bTax) {
      if (!TaxArchInit()) bTax = FALSE;
    }
    while (pcdsThis) {
      if (((UseThisGi(pcdsThis->iCddIdx,pvnGis) || !bMode) && 
          (!bTax || CddSameTaxLineage(pcdsThis->sip,iTaxId,bssp->seq_set,txids))) ||
	  pcdsThis->bIsMaster || pcdsThis->bIs3dRep) {
	if (iCddSize < iMaxAln || bSelect || iMaxAln==0) {
          iCddSize++;
	  if (pcdsThis->bIsPdb) nPdb++;
          if (!pcdsHead) {
	    pcdsHead = pcdsThis;
	  } else {
	    pcdsTail->next = pcdsThis;
	  }
	  pcdsTail = pcdsThis;
          bsp = (BioseqPtr) CddFindSip(pcdsThis->sip, bssp->seq_set);
	  if (bMode) {
            if (!pcdsThis->bIsMaster) {
	      if (!salpCopy) {
	        salpCopy = pcdsThis->salp;
	      } else {
	        salpTail->next = pcdsThis->salp;
	      }
	      salpTail = pcdsThis->salp;
	    }
            if (bsp) {
	      if (bsp!= (BioseqPtr) CddFindSip(pcdsThis->sip,sequences)) {
                sep = SeqEntryNew();
                sep->choice = 1;
                sep->data.ptrvalue = bsp;
                ValNodeLink(&(sequences),sep);
	      }
	    }
          } else {
            if (bsp) CreateDefLine(NULL,bsp,pcdsThis->cDefLine,256,0,NULL,NULL);
	  }
        }
      }
      pcdsThis = pcdsThis->next;
    }
    if (bTax) TaxArchFini();
  }
  if (pcdsHead) pcds = pcdsHead;
  if (pcdsTail) pcdsTail->next = NULL;
  if (salpTail) salpTail->next = NULL;

/*---------------------------------------------------------------------------*/
/* again, add the query (alignment) in case this is a BLAST result           */
/*---------------------------------------------------------------------------*/
  if (bBlast) {
    salpQuery->next = salpCopy;
    salpCopy = salpQuery;
    sep = SeqEntryNew();
    sep->choice = 1;
    sep->data.ptrvalue = bspQuery;
    ValNodeLink(&(sequences), sep); 
  }
  
/*---------------------------------------------------------------------------*/
/* default action: Output a list of gi's and PDB-Id's for subset selection   */
/* CddServerShowTracks ends program  - all code after this assumes that bMode*/
/* is TRUE and that alignments need to be shown                              */
/*---------------------------------------------------------------------------*/
  if (bMode == CDDSUMMARY) {
    if (pcds) {
      CddServerShowTracks(pcds,pcdd,iTaxId,bHasPdb,dbversion,bHasConsensus,bShowTax,txids,iPDB,alen,nTaxIds,iTaxids, pcdtree);
    } else {
      CddServerShowTracks(pcds,pcdd,iTaxId,FALSE,dbversion,bHasConsensus,bShowTax,txids,iPDB,alen,nTaxIds,iTaxids, pcdtree);
    }
  }

/*---------------------------------------------------------------------------*/
/* Initialize the data structure needed to collect alignments and sequences  */
/*---------------------------------------------------------------------------*/
  pbsaSeq    = BiostrucAlignSeqNew();
  pbsaStrSeq = BiostrucSeqsNew();
  pbsaStruct = BiostrucAlignNew();
  pbsaCdd    = BiostrucSeqsAlignsCddNew();
  psadsad = ValNodeNew(NULL);
  psadsad->choice = SeqAlignData_seq_align_data_cdd;
  psadsad->data.ptrvalue = pcdd;
  pbsaCdd->SeqAlignData_seq_align_data = psadsad;
  if (iModelComplexity == ONECOORDRES) {
    pbsaCdd->structure_type = Biostruc_seqs_aligns_cdd_structure_type_ncbi_backbone;
  } else {
    pbsaCdd->structure_type = Biostruc_seqs_aligns_cdd_structure_type_ncbi_all_atom;
  }
  pbsaSeq->sequences = sequences;
  pbsaStruct->sequences = sequences;
  pbsaStrSeq->sequences = sequences;
  psaCAlignHead = SeqAnnotNew();
  psaCAlignHead->data = salpCopy;
  psaCAlignHead->desc = pcdd->seqannot->desc;
  psaCAlignHead->type = pcdd->seqannot->type;
 
  OpenMMDBAPI((POWER_VIEW /* ^ FETCH_ENTREZ */), NULL);
  if (!MMDBInit_FF()) CddHtmlError("MMDB Initialization failed");

  pcdsThis = pcds;
  while (pcdsThis) {
    if (pcdsThis->bIsPdb && (iPDB < 2 || iPDB == 6 || iPDB == 7)) {
      pcdsThis->iMMDBId = ConvertMMDBUID(pcdsThis->cPdbId);
      if (pcdsThis->bIs3dRep) iRepId = pcdsThis->iMMDBId;
    }
    pcdsThis=pcdsThis->next;
  }

/*---------------------------------------------------------------------------*/
/* if this is an oAsIs CD, remove MMDB-Ids from all pdb-derived bioseqs if   */
/* not the master ...                                                        */
/*---------------------------------------------------------------------------*/
  if (bHasPdb && (bIsOasis || bEvidenceViewer) && bHave3dRep) {
    pcdsThis = pcds;
    while (pcdsThis) {
      if (pcdsThis->bIsPdb && !pcdsThis->bIs3dRep && pcdsThis->iMMDBId != iRepId) {
        bsp = CddFindSip(pcdsThis->sip,bssp->seq_set);
	if (bsp) {
	  sap = CddFindMMDBIdInBioseq(bsp,&i);
	  if (sap) {
	    sap->next = NULL;
	  } else bsp->annot = NULL;
	}
      }
      pcdsThis = pcdsThis->next;
    }
  }

/*---------------------------------------------------------------------------*/
/* automatically change the iSeqStrMode settings if a structure is available */
/*---------------------------------------------------------------------------*/
  if (bHasPdb) {
    if (iSeqStrMode==CDDSEQUONLY) iSeqStrMode = CDDONESTRUC;
    if (nPdb > 1 && CddGetStatus(pcdd)!=3) {
      if (iSeqStrMode==CDDONESTRUC) iSeqStrMode = CDDSEVSTRUC;
    }
  }

/*---------------------------------------------------------------------------*/
/* retrieve Master structure in the case of a single-structure CD            */
/*---------------------------------------------------------------------------*/
  if (bMode && bHasPdb && (iPDB < 2 || iPDB == 6)) { 
    if (!bHasConsensus) {
      strcpy(szName,pcds->cPdbId);
    } else {
      pcdsThis = pcds; while (pcdsThis) {
        if (pcdsThis->bIsPdb && pcdsThis->bIs3dRep) {
          strcpy(szName,pcdsThis->cPdbId);
	  break;
	}
        pcdsThis = pcdsThis->next;
      }
    }
    tempuid = ConvertMMDBUID(szName);
    pbsMaster = (BiostrucPtr) MMDBBiostrucGet_FF(tempuid,iModelComplexity,1);
    if (!pbsMaster) CddHtmlError("Unable to load Master structure!");
    if (bChain) {
      if (!bHasConsensus) strcpy(chain,pcds->cChainId);
      else strcpy(chain,pcdsThis->cChainId);
      if (chain[0] != ' ') {
        pbsTemp = (BiostrucPtr)PruneBiostruc(pbsMaster,chain);
        pbsMaster = NULL;
        pbsMaster = pbsTemp;
      }
    }
    pbsaStrSeq->structure = pbsMaster;    
    pbsaStruct->master = pbsMaster;
    if (iPDB < 7) pbsaCdd->structures = pbsMaster;
  }

/*---------------------------------------------------------------------------*/
/* retrieve the program mode if present in the input, it may override        */
/* automatically generated settings                                          */
/*  options for mode are:  'c' .... A CDD summary is generated               */
/*                         'a' .... alignseq generated, single alignment     */
/*                         's' .... strucseqs generated, single alignment    */
/*                         'v' .... alignstruc generated, single alignment   */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "mode")) >= 0) {
    www_arg =  WWWGetValueByIndex(www_info, indx);
    cMode = www_arg[0];
    if (cMode == 'a') {
      bMode = CDDALIGNMENT;
      iSeqStrMode = CDDSEQUONLY;
    } else if (cMode == 's') {
      if (bHasPdb){ bMode = CDDALIGNMENT;
        iSeqStrMode = CDDONESTRUC;
      }
    } else if (cMode == 'v') {
      if (bHasPdb && nPdb > 1) { bMode = CDDALIGNMENT;
        iSeqStrMode = CDDSEVSTRUC;
      }
    } else if (cMode == 'c' || cMode == 'C') bMode = CDDSUMMARY;       
#ifdef DEBUG
    printf(" DEBUG: mode selected as %c, interpreted as %d\n",cMode,bMode);
#endif
  } else cMode = '\0';
  if (bMode != CDDSUMMARY && ((iPDB > 1 && iPDB < 6) || iPDB == 8 || iPDB == 9)) {
    bMode = CDDALIGNMENT;
    iSeqStrMode = CDDSEQUONLY;
  }

/*---------------------------------------------------------------------------*/
/* retrieve slave structures in the case of a multi-structure CD             */
/*---------------------------------------------------------------------------*/
  if (iSeqStrMode == CDDSEVSTRUC && (iPDB < 2 || iPDB == 6) && cMode != 's') {
    pcdsThis = pcds->next;
    while (pcdsThis) {
      if (pcdsThis->bIsPdb && !pcdsThis->bIs3dRep) {
        strcpy(szName,pcdsThis->cPdbId);
        if (!pbsSlaveHead) {
          pbsSlaveHead = (BiostrucPtr) MMDBBiostrucGet_FF(ConvertMMDBUID(szName),iModelComplexity,1);
          if (!pbsSlaveHead) CddHtmlError("Unable to load slave structure!");
          if (bChain) {
            strcpy(chain,pcdsThis->cChainId);
            if (chain[0] != ' ') {
              pbsTemp = (BiostrucPtr)PruneBiostruc(pbsSlaveHead,chain);
              pbsSlaveHead = NULL;
              pbsSlaveHead = pbsTemp;
            }
          }
          pbsSlaveTail = pbsSlaveHead;
        } else {
          pbsSlave = (BiostrucPtr) MMDBBiostrucGet_FF(ConvertMMDBUID(szName),iModelComplexity, 1);
          if (!pbsSlave) CddHtmlError("Unable to load slave structure!");
          if (bChain) {
            strcpy(chain,pcdsThis->cChainId);
            if (chain[0] != ' ') {
              pbsTemp = (BiostrucPtr)PruneBiostruc(pbsSlave,chain);
              pbsSlave = NULL;
              pbsSlave = pbsTemp;
            }
          }
          pbsSlaveTail->next = pbsSlave;
          pbsSlaveTail = pbsSlaveTail->next;
          pbsSlaveTail->next = NULL;
        }
      }
      pcdsThis = pcdsThis->next;
    }
    pbsaStruct->slaves = pbsSlaveHead;
  }

/*---------------------------------------------------------------------------*/
/* if more than one structure present, VAST results have to be retrieved     */
/*---------------------------------------------------------------------------*/
  if (bHasPdb && nPdb > 1 && iSeqStrMode != CDDSEQUONLY) {
    pbsa = (BiostrucAnnotSetPtr) pcdd->features;
    pbsfCopy = NULL;
    pbsfTail = NULL;
    if (pbsa) {
      pbsf = pbsa->features->features;
      while (pbsf) {
        if (CddUseThisMMDBid(pbsf->Location_location,pcds)) {
          if (!pbsfCopy) {
            pbsfCopy = pbsf;
            pbsfTail = pbsf;
          } else {
            pbsfTail->next = pbsf;
            pbsfTail = pbsf;
          }
        }  
        pbsf = pbsf->next;
      }
      if (pbsfTail) pbsfTail->next = NULL;
      pbsa->features->features = pbsfCopy;
    }
/*---------------------------------------------------------------------------*/
/* need to remove slave structures not aligned by VAST, otherwise Cn3D may   */
/* behave weird.                                                             */
/*---------------------------------------------------------------------------*/
    pcdsThis = pcds; nPdb = 1;
    while (pcdsThis) {
      if (pcdsThis->bIsPdb && !pcdsThis->bIsMaster && !pcdsThis->bIs3dRep) {
        if (pcdsThis->iMMDBId != -1) {
          pbsSlaveHead = pbsaStruct->slaves;
          pbsSlave = pbsSlaveHead;
          pbsSlaveTail = NULL;
          while (pbsSlaveHead) {
            bRemove = FALSE;
            pbsi=pbsSlaveHead->id;
            while (pbsi) {
              if (pbsi->choice == BiostrucId_mmdb_id) {
                if (pbsi->data.intvalue == pcdsThis->iMMDBId) {
                  bRemove = TRUE;
                  if (!pbsSlaveTail) {
                    pbsSlave = pbsSlaveHead->next;
                    pbsSlaveTail = pbsSlave;
                  } else {
                    pbsSlaveTail->next = pbsSlaveHead->next;
                    pbsSlaveTail = pbsSlaveTail->next;
                  }
                  break;
                }
              }
              pbsi = pbsi->next;
            }
            if (!bRemove) pbsSlaveTail = pbsSlaveHead;
            pbsSlaveHead = pbsSlaveHead->next;
          }
          pbsaStruct->slaves = pbsSlave;
        } else nPdb++;
      }
      pcdsThis = pcdsThis->next;
    }
    if (pbsfCopy) pbsaStruct->alignments = pbsa;
    if (nPdb == 1) {
      if (iSeqStrMode == CDDSEVSTRUC) iSeqStrMode = CDDONESTRUC;
    }
  }

/*---------------------------------------------------------------------------*/
/* need to add gi's to Seq-Id's for PDB-derived sequences in order to make   */
/* Cn3D happy                                                                */
/*---------------------------------------------------------------------------*/
  sequences = pbsaSeq->sequences;
  sep = sequences;
  while (sep) {
    bsp = sep->data.ptrvalue;
    if (bsp) {
      sip = bsp->id;
      if (sip->choice == SEQID_PDB && NULL == sip->next) {
        sipNew = ValNodeNew(NULL);
        sipNew->choice = SEQID_GI;
        sipNew->data.intvalue = GetGIForSeqId(sip);
        ValNodeLink(&(sip),sipNew);
      }
    }
    sep = sep->next;
  }

/*---------------------------------------------------------------------------*/
/* if CD has consensus, and 3D visualization is selected, need to reindex    */
/* alignment to use the 3D representative as the master!                     */
/*---------------------------------------------------------------------------*/
  if ((bHasConsensus || bEvidenceViewer) && (iPDB < 2 || iPDB == 6 || iPDB == 7)) {
    salpCopy = psaCAlignHead->data;
    pcdsThis = pcds; while (pcdsThis) {
      if (pcdsThis->bIs3dRep) {
        sipMaster = pcdsThis->sip;
        break;
      }
      pcdsThis = pcdsThis->next;
    }
    psaCAlignHead->data = CddReindexSeqAlign(salpCopy, sipMaster, bssp);
    pcdd->master3d = (SeqIdPtr) SeqIdDup(sipMaster);
  }
  
/*---------------------------------------------------------------------------*/
/* prepare data for output                                                   */
/*---------------------------------------------------------------------------*/
  pvnNcbi=ValNodeNew(NULL);
  if (iSeqStrMode == CDDONESTRUC || iSeqStrMode == CDDSEVSTRUC) {
    if (iPDB == 6 || iPDB == 7) iSeqStrMode = CDDASCDD;
  }
  switch(iSeqStrMode) {
    case CDDSEQUONLY: pvnNcbi->choice=NcbiMimeAsn1_alignseq;      break;
    case CDDONESTRUC: pvnNcbi->choice=NcbiMimeAsn1_strucseqs;     break;
    case CDDSEVSTRUC: pvnNcbi->choice=NcbiMimeAsn1_alignstruc;    break;
    case CDDASCDD:    pvnNcbi->choice=NcbiMimeAsn1_general;       break;
    default: CddHtmlError("Could not interpret alignment mode!"); break;
  }
  switch(iSeqStrMode) {
    case CDDSEQUONLY: pbsaSeq->seqalign    = psaCAlignHead;          break;
    case CDDONESTRUC: pbsaStrSeq->seqalign = psaCAlignHead;          break;
    case CDDSEVSTRUC: pbsaStruct->seqalign = psaCAlignHead;          break;
    case CDDASCDD:    
      pcdd->seqannot = psaCAlignHead;
      bssp->seq_set = pbsaSeq->sequences;
      pcdd->posfreq = NULL;
      if (bEvidenceViewer) pcdd->features = NULL;
      if (!bIsOasis && cMode != 's') { 
        if (iPDB < 7) pbsaCdd->structures->next = pbsaStruct->slaves;
      }
      if (bEvidenceViewer) {
/*        pStyles[0] = cdd_def_style;
	pStyles[1] = cdd_evidence_style;
        pcdd->style_dictionary = CddSrvGetStyle2(pStyles,2); */
        pcdd->style_dictionary = CddSrvGetStyle(TRUE);
	pcdd->user_annotations = CddMakeUserAnnot(pcdd, iFeatNum, iEvidence);
      } else {
/*        pStyles[0] = cdd_def_style;
        pcdd->style_dictionary = CddSrvGetStyle2(pStyles,1); */
        pcdd->style_dictionary = CddSrvGetStyle(FALSE);
      }
      break;
  }
  switch(iSeqStrMode) {
    case CDDSEQUONLY: pvnNcbi->data.ptrvalue = pbsaSeq;    break;
    case CDDONESTRUC: pvnNcbi->data.ptrvalue = pbsaStrSeq; break;
    case CDDSEVSTRUC: pvnNcbi->data.ptrvalue = pbsaStruct; break;
    case CDDASCDD:    pvnNcbi->data.ptrvalue = pbsaCdd;    break;
  }

/*---------------------------------------------------------------------------*/
/* use Paul's CddAlignView, write data into a buffer                         */
/*---------------------------------------------------------------------------*/
  if (iSeqStrMode == CDDSEQUONLY) {
    CddFixSequenceFormat(pbsaSeq->sequences);
    if (!CddInvokeAlignView(pvnNcbi,CDDalign,iPDB,QuerySeq,QueryAlign,dbversion,
                            pcdd,bHasPdb,tbit,pwidth,iQueryGi,QueryName,iFeatNum,
			    pcds,alen,nTaxIds,iTaxids,txids,pcdtree)) 
      CddHtmlError("Could not display alignment");
    return 0;  
  }

/*---------------------------------------------------------------------------*/
/* Insert local Id's instead of duplicate sequence Id's                      */ 
/*---------------------------------------------------------------------------*/
/*  pvnNcbi = (NcbiMimeAsn1Ptr) CheckId(pvnNcbi,NULL);                       */
/*---------------------------------------------------------------------------*/
/* Generate temporary file                                                   */
/*---------------------------------------------------------------------------*/
  strcpy(OutputName,GetTempName("cddsrv"));
  if(!(OutputFile = FileOpen(OutputName,WRITE))) {
    CddHtmlError("Temp File Open Failed at CDD WWW-Server");
  }
/*---------------------------------------------------------------------------*/
/* cn3d file generation                                                      */
/*---------------------------------------------------------------------------*/
  if (iPDB == 0 || iPDB == 6 || iPDB == 7)
    fprintf(OutputFile, "Content-type: chemical/ncbi-asn1-binary\n\n");
  else if (iPDB == 1) {                         /* corresponds to "See File" */
    fprintf(OutputFile, "Content-type: text/html\n\n");
    fprintf(OutputFile, "<HTML><body><pre>\n");
  } else fprintf(OutputFile, "Content-type: application/octet-stream\n\n");
  fflush(OutputFile);
  if (OutputFile != stdout) {
    if (iPDB == 0 || iPDB == 6 || iPDB == 7)
      paiFile = AsnIoNew(ASNIO_BIN_OUT, OutputFile, NULL, NULL, NULL);
    else
      paiFile = AsnIoNew(ASNIO_TEXT_OUT, OutputFile, NULL, NULL, NULL);
    NcbiMimeAsn1AsnWrite(pvnNcbi, paiFile, NULL);
    AsnIoFlush(paiFile);
    AsnIoClose(paiFile);
    if (iPDB == 1) fprintf(OutputFile, "</pre></body></HTML>\n");
    PrintFile(OutputName);    
  }
  CloseMMDBAPI();
  MMDBFini_FF();
  VASTFini();
/*  PUBSEQFini(); */
  RemoveTempFiles();   
  return 0;
}
