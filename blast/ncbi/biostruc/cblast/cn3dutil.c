/*===========================================================================
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
* File Name: cn3dutils.c 
*
* Author:  Yanli Wang
*
* Initial Version Creation Date: 8/12/2002
*
* File Description:
*         facility function for adding drawing-style and user annotations 
*
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cn3dutil.c,v $
* Revision 1.5  2002/12/12 15:59:45  ywang
* wording improvement
*
*
* ==========================================================================
*/
#include <ncbi.h>  /* the NCBI SDK header */
#include <asn.h>   /* the ASN.1 header */
#include <mmdbapi.h>   /* the MMDB-API header */
#include <sequtil.h>
#include <objsset.h>
#include <mmdbdata.h>
#include <mmdblocl.h>
#include <string.h>
#include <accentr.h>
#include <saledit.h>
#include <lsqfetch.h>
#include <objseq.h>
#include <asnseq.h>
#include <objmgr.h>
#include <sequtil.h>
#include <objmime.h>
#include <blast.h>
#include <www.h>
#include <accutils.h>
#include <netentr.h>
#include <cddutil.h>
#include <objcn3d.h>
/*-------- set cn3dcolor ---------*/
void SetCn3dColor(Cn3dColorPtr ccp, Int4 scale_factor, Int4 red, Int4 green, Int4 blue, Int4 alpha)
{
  ccp->scale_factor = scale_factor;
  ccp->red = red;
  ccp->green = green;
  ccp->blue = blue;
  ccp->alpha = alpha;
 
}
/*-------- get default style setting -----------*/
Cn3dStyleSettingsPtr GetDefaultCn3DStyleSettings()
{
  Cn3dStyleSettingsPtr      cssp = NULL;
  Cn3dBackboneStylePtr      cbsp = NULL;
  Cn3dGeneralStylePtr       cgsp = NULL;
  Cn3dColorPtr              ccp = NULL;
  Cn3dBackboneLabelStylePtr cblsp = NULL;

    cssp = Cn3dStyleSettingsNew();
    cssp->next = NULL;
    cssp->name = NULL;

    cbsp = Cn3dBackboneStyleNew();
      cbsp->type = Cn3d_backbone_type_trace;
/*    cbsp->style = Cn3d_drawing_style_tubes; */
      cbsp->style = Cn3d_drawing_style_wire;
/*    cbsp->color_scheme = Cn3d_color_scheme_weighted_variety;  */
      cbsp->color_scheme = Cn3d_color_scheme_identity; 
      ccp = Cn3dColorNew();
      SetCn3dColor(ccp, 10000, 5000, 5000, 5000, 10000);
      cbsp->user_color = ccp;
    cssp->protein_backbone = cbsp;

    cbsp = Cn3dBackboneStyleNew();
      cbsp->type = Cn3d_backbone_type_trace;
/*    cbsp->style = Cn3d_drawing_style_tubes; */
      cbsp->style = Cn3d_drawing_style_wire;
      cbsp->color_scheme = Cn3d_color_scheme_molecule;
      ccp = Cn3dColorNew();
      SetCn3dColor(ccp, 10000, 5000, 5000, 5000, 10000);
      cbsp->user_color = ccp;
    cssp->nucleotide_backbone = cbsp;

    cgsp = Cn3dGeneralStyleNew();
      cgsp->is_on = TRUE;
      cgsp->style = Cn3d_drawing_style_wire;
/*    cgsp->color_scheme = Cn3d_color_scheme_weighted_variety; */ 
      cgsp->color_scheme = Cn3d_color_scheme_identity; 
      ccp = Cn3dColorNew();
      SetCn3dColor(ccp, 10000, 5000, 5000, 5000, 10000);    
      cgsp->user_color = ccp;
    cssp->protein_sidechains = cgsp;

    cgsp = Cn3dGeneralStyleNew();
      cgsp->is_on = TRUE;
      cgsp->style = Cn3d_drawing_style_wire;
      cgsp->color_scheme = Cn3d_color_scheme_molecule;
      ccp = Cn3dColorNew();
      SetCn3dColor(ccp, 10000, 5000, 5000, 5000, 10000);
      cgsp->user_color = ccp;
    cssp->nucleotide_sidechains = cgsp;

    cgsp = Cn3dGeneralStyleNew();
      cgsp->is_on = TRUE;
      cgsp->style = Cn3d_drawing_style_ball_and_stick;
      cgsp->color_scheme = Cn3d_color_scheme_element;
      ccp = Cn3dColorNew();
      SetCn3dColor(ccp, 10000, 5000, 5000, 5000, 10000);
      cgsp->user_color = ccp;
    cssp->heterogens = cgsp;

    cgsp = Cn3dGeneralStyleNew();
      cgsp->is_on = FALSE;
      cgsp->style = Cn3d_drawing_style_ball_and_stick;
      cgsp->color_scheme = Cn3d_color_scheme_element;
      ccp = Cn3dColorNew();
      SetCn3dColor(ccp, 10000, 5000, 5000, 5000, 10000);
      cgsp->user_color = ccp;
    cssp->solvents = cgsp;

    cgsp = Cn3dGeneralStyleNew();
      cgsp->is_on = TRUE;
      cgsp->style = Cn3d_drawing_style_tubes;
      cgsp->color_scheme = Cn3d_color_scheme_user_select;
      ccp = Cn3dColorNew();
      SetCn3dColor(ccp, 10000, 5000, 5000, 5000, 10000);
      cgsp->user_color = ccp;
    cssp->connections = cgsp;

    cgsp = Cn3dGeneralStyleNew();
      cgsp->is_on = FALSE;
      cgsp->style = Cn3d_drawing_style_with_arrows;
      cgsp->color_scheme = Cn3d_color_scheme_object;
      ccp = Cn3dColorNew();
      SetCn3dColor(ccp, 10000, 5000, 5000, 5000, 10000);
      cgsp->user_color = ccp;
    cssp->helix_objects = cgsp;

    cgsp = Cn3dGeneralStyleNew();
      cgsp->is_on = FALSE;
      cgsp->style = Cn3d_drawing_style_with_arrows;
      cgsp->color_scheme = Cn3d_color_scheme_object;
      ccp = Cn3dColorNew();
      SetCn3dColor(ccp, 10000, 5000, 5000, 5000, 10000);
      cgsp->user_color = ccp;
    cssp->strand_objects = cgsp;

    cssp->virtual_disulfides_on = TRUE;
      ccp = Cn3dColorNew();
      SetCn3dColor(ccp, 10000, 9300, 5500, 500, 10000);
    cssp->virtual_disulfide_color = ccp;
    cssp->hydrogens_on = FALSE;
      ccp = Cn3dColorNew();
      SetCn3dColor(ccp, 10000, 0, 0, 0, 10000);
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

    return cssp;
}
