/*   saled.h
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
* File Name:  saled.h
*
* Author:  Colombe Chappey
*
* Version Creation Date:   1/27/96
*
* $Revision: 6.5 $
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

#ifndef _SALED_
#define _SALED_

#include <vibrant.h>
#include <salsa.h>

extern Uint2 OBJ_ (Uint2 feattype);
extern SelStructPtr locate_region (SelStructPtr ssp, Int4 from, Int4 to, SeqIdPtr sip, Uint1 strand, Boolean is_fuzz);
extern SelStructPtr replace_region (SelStructPtr ssp, Uint2 ssp_ed, Uint2 ssp_id, Uint2 ssp_it, Int4 from, Int4 to, SeqIdPtr sip, Uint1 strand, Boolean is_fuzz);

extern void GoToButton (ButtoN b);
extern void LookAtButton (ButtoN b);

extern Boolean do_cut (PaneL pnl, EditAlignDataPtr adp, SelStructPtr ssp, Boolean cut);
extern Boolean do_paste (PaneL pnl, EditAlignDataPtr adp, SeqIdPtr targetid);
extern void do_copy (IteM i);

extern void to_update_prompt (PaneL pnl, SelStructPtr ssp, SeqAlignPtr salp, ValNodePtr sqlocs, Boolean sel, Uint2 choice);
extern void update_edititem (PaneL pnl);
extern void update_translateitem (PaneL pnl, ValNodePtr seqfeathead, ValNodePtr feathead);
extern void update_codonstartbt (PaneL pnl, ValNodePtr seqfeathead, ValNodePtr feathead);

extern Uint1 locate_point (PoinT pt, RecT rp, Uint2 *item_id, Uint2 *the_entity_id, Uint2 *item_type, Uint2 *item_subtype, Int4 *position, Int2 *line, EditAlignDataPtr adp);
extern void on_click (PaneL pnl, PoinT pt);
extern void on_drag (PaneL pnl, PoinT pt);
extern void on_hold (PaneL pnl, PoinT pt);
extern void on_release (PaneL pnl, PoinT pt);
extern void on_time (WindoW w);
extern void on_key (SlatE s, Char ch);

#endif


