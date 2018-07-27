/*  $RCSfile: salogif.c,v $  $Revision: 6.8 $  $Date: 2005/03/11 22:07:56 $
* ==========================================================================
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
* ==========================================================================
*
* Author:	Jinghui Zhang
*
* $Revision: 6.8 $
*
* File Description:
*	 The Blast Search result visualization utilities  
*
* --------------------------------------------------------------------------
* $Log: salogif.c,v $
* Revision 6.8  2005/03/11 22:07:56  merezhuk
* GIFTMP_BASE environment variable introduced to changed TmpGifs directory location.
*
* Revision 6.7  2004/02/02 23:40:25  dondosha
* Modify query bioseq id so it is a gi id, if possible, because this is the rule in seqaligns coming from BLAST
*
* Revision 6.6  2004/01/23 22:48:57  jianye
* restore seqid in PrintOneAlignmentOverview and initialize some varialbes
*
* Revision 6.5  2003/11/20 18:36:08  dondosha
* Added function PrintOneAlignmentOverview to create an overview gif for one of the multiple query sequences
*
* Revision 6.4  2003/10/31 02:51:14  dondosha
* Always use first seqid for master (query) sequence instead of "best" id in ModMaxAlign
*
* Revision 6.3  2003/07/15 14:36:31  dondosha
* Added #defines for substitutes to fprintf and fflush, needed for gzip compression of Web BLAST results
*
* Revision 6.2  2000/05/12 19:21:45  shavirin
* Fixed memory leak in print_defline_for_sequence().
*
* Revision 6.1  2000/03/20 19:00:28  shavirin
* Initial revision in the new location.
*
* Revision 1.3  1999/04/29 16:37:50  shavirin
* Changed error message when opening Gif failed.
*
* Revision 1.2  1999/04/29 14:50:15  shavirin
* Changed directory for printing GIF.
*
* Revision 1.1  1999/04/15 15:20:20  egorov
* Initial commit
*
* Revision 1.2  1998/07/14 20:16:34  madden
* Changes for random access BLAST
*
* Revision 1.1  1998/05/13 20:20:09  egorov
* This revision borns salogif.[hc] files for store functions for visualization results of BLAST search
*
* ==========================================================================
*/

#define DEFAULT_X_SIZE 550
#define DEFAULT_Y_SIZE 900
#define GIF_WIDTH_MAX 2500
#define GIF_HEIGHT_MAX 550

#define TMP_GIF_DIR   "TmpGifs"

#include <ncbi.h>
#include <lsqfetch.h>
#include <ncbigif.h>
#include <glbpic.h>
#include <fstyle.h>
#include <txalign.h>
#include <seqmgr.h>
#include <salogif.h>

int (*gif_fprintf)(FILE*, const char *, ...) = fprintf;
int (*gif_fflush)(FILE*) = fflush;
#define fprintf gif_fprintf
#define fflush gif_fflush

/*******************************************************************
 *  STATIC
 */

static Boolean print_score_evalue
(FloatHi evalue, FloatHi bit_score, CharPtr buf)
{
  Char eval_buff[101];
  Char bit_score_buff[101];

  buf[0] = '\0';
  eval_buff[0] = '\0';
  bit_score_buff[0] = '\0';

  if (evalue < 1.0e-180)
    sprintf(eval_buff, "0.0");
  else if (evalue < 1.0e-99)
    sprintf(eval_buff, "%2.0e", evalue);
  else if (evalue < 0.0009)
    sprintf(eval_buff, "%3.0e", evalue);
  else if (evalue < 0.1)
    sprintf(eval_buff, "%4.3f", evalue);
  else if (evalue < 1.0)
    sprintf(eval_buff, "%3.2f", evalue);
  else if (evalue < 10.0)
    sprintf(eval_buff, "%2.1f", evalue);
  else
    sprintf(eval_buff, "%5.0f", evalue);

  if (bit_score > 9999)
    sprintf(bit_score_buff, "%4.3e", bit_score);
  else if (bit_score > 99.9)
    sprintf(bit_score_buff, "%4.0ld", (long)bit_score);
  else
    sprintf(bit_score_buff, "%4.1f", bit_score);

  if (eval_buff[0] != '\0' && bit_score_buff[0] != '\0') {
    sprintf(buf, "S=%s E=%s", bit_score_buff, eval_buff);
    return TRUE;
  }
  if (eval_buff[0] != '\0') {
    sprintf(buf, "E=%s", eval_buff);
    return TRUE;
  }

  if (bit_score_buff[0] != '\0') {
    sprintf(buf, "S=%s", bit_score_buff);
    return TRUE;
  }

  return FALSE;
}


static Boolean print_defline_for_sequence
(SeqAlignPtr align, AlignRegionPtr arp, CharPtr buffer, Int4Ptr gi_return)
{
    SeqIdPtr sip;
    Char name[101], buffer_id[101];
    BioseqPtr bsp;
    Char buf[101];
    CharPtr title;
    Int4 score_len;
    Char score_buf[101];
    Int4 totle_len = 80;
    Int4 title_len, len, j;
    SeqIdPtr gi_list, tmp_sip, bestid;
    Boolean title_allocated = FALSE;

    while(align) {
        sip = TxGetSubjectIdFromSeqAlign(align);
        if(sip != NULL) {
            MuskSeqIdWrite(sip, name, 19, PRINTID_TEXTID_ACCESSION, FALSE, FALSE);
            if(StringCmp(name, arp->seq_name) == 0) {
                gi_list = GetUseThisGi(align);
                bsp = BioseqLockById(sip);
                title = NULL;
                if(bsp != NULL) {
                    if (gi_list == NULL) {
			MuskSeqIdWrite(bsp->id, name, 10, PRINTID_TEXTID_ACCESSION, TRUE, FALSE);
			title = BioseqGetTitle(bsp);
                    } else {
			FilterTheDefline(bsp, gi_list, buffer_id, 100, &title);
                        title_allocated = TRUE;
			tmp_sip = SeqIdParse(buffer_id);
			bestid = SeqIdFindBest(tmp_sip, SEQID_GI);
			if (bestid != NULL && bestid->choice == SEQID_GI)
                            *gi_return = bestid->data.intvalue;
			MuskSeqIdWrite(tmp_sip, name, 10, PRINTID_TEXTID_ACCESSION, TRUE, FALSE);
                        SeqIdSetFree(tmp_sip);
                    }
                } else {
                    StringCpy(name, arp->seq_name);
                }

                score_len = 0;
                score_buf[0] = '\0';
                if(print_score_evalue(arp->e_val, arp->score, score_buf))
                    score_len = StringLen(score_buf);
                
                title_len = totle_len - StringLen(name) - score_len -2;
                if(score_len > 0)
                    title_len -= 2;
                StringCpy(buffer, name);
                if(title != NULL) {
                    for(j = 0, len = 0; len < title_len; ++len) {
                        if(title[len] == '\0')
                            break;
                        else {
                            /* if(title[len] = '<')
                               {
                               buf[j] = '&';
                               buf[j+1] = 'l';
                               buf[j+2] = 't';
                               buf[j+3] = ';';
                               j+=4;
                               }
                               else if(title[len] == '>')
                               {
                               buf[j] = '&';
                               buf[j+1] = 'g';
                               buf[j+2] = 't';
                               buf[j+3] = ';';
                               j+=4;
                               }
                               else
                               buf[j++] = title[len]; */
                            buf[len] = title[len];
                            if(buf[len] == '<' || buf[len] == '>')
                                buf[len] = ' ';
                            else if(buf[len] == '\'')
                                buf[len] = '-';
                            else if(buf[len] == '\"')
                                buf[len] = ' ';
                        }
                    }
                    buf[len] = '\0';
                    
                    len = StringLen(buffer);
                    sprintf(buffer+len, "  %s", buf);

                    if(title_allocated)
                        MemFree(title);
                }
                if(score_len > 0) {
                    len = StringLen(buffer);
                    sprintf(buffer+len, "..%s", score_buf);
                }
                
                if(bsp != NULL)
                    BioseqUnlock(bsp);
                return TRUE;
            }
        }
        align = align->next;
    }
        
    return FALSE;
}


static Boolean PrintImageMapForGlobal
(GlobalDrawPtr g_draw, Int4 pntX, Int4 pntY, 
 Int4 scale, CharPtr title_buf, SeqAlignPtr align,
 FILE *fp, const Char* formname, int query_index)
{
  AlignRegionPtr arp;
  ValNodePtr curr;
  GlobalBspPtr gbp;
  ValNodePtr gbp_list;
  Boolean retval = FALSE;
  Char seq_buffer[101], buffer[101];
  Int4 gi_value;

  if(g_draw == NULL)
    return FALSE;
  for(gbp_list = g_draw->gbp_list; gbp_list != NULL; gbp_list = gbp_list->next)
    {
      gbp = (GlobalBspPtr)gbp_list->data.ptrvalue;
      if(gbp->has_fish_align == FALSE && gbp->arp_list != NULL)
        {
          if(retval == FALSE)
            fprintf(fp, "<map name=img_map%d>\n", query_index); 
          for(curr = gbp->arp_list; curr != NULL; curr = curr->next)
            {
              arp = (AlignRegionPtr)curr->data.ptrvalue;
              seq_buffer[0] = '\0';
	      gi_value = 0;
              print_defline_for_sequence(align, arp, seq_buffer, &gi_value);
              if(seq_buffer[0] == '\0')
                StringCpy(seq_buffer, arp->seq_name);

	      if (gi_value != 0)
                sprintf(buffer, "%ld", gi_value);
              else
                sprintf(buffer, "%s", arp->seq_name);

              fprintf(fp, "<area shape=rect coords=%d,%d,%d,%d "
                      "href=\"#%s\" "
                      "ONMOUSEOVER=\'document.%s.defline.value=\"%s\"\' "
                      "ONMOUSEOUT=\'document.%s.defline.value=\"%s\"\' "
                      ">\n",
                      (int)(((arp->g_left)-pntX)/scale), 
                      (int)(abs(arp->g_top- pntY)-1),
                      (int)(((arp->g_right)-pntX)/scale),
                      (int)(abs(arp->g_bottom - pntY)+2),
                      buffer,
                      formname, seq_buffer, 
                      formname, title_buf); 
            }
          retval = TRUE;
        }
    }
  if(retval)
    fprintf(fp, "</map>\n"); 
  return retval;
}

static Int4 get_number_alignment(SeqAlignPtr align)
{
  Int4 num = 0;

  while(align)
    {
      ++num;
      align = align->next;
    }

  return num;
}

typedef struct align_info {
  SeqAlignPtr align;
  Int4 align_len;
}AlignInfo, PNTR AlignInfoPtr;


static Int4 get_redundant_len(Uint1Ptr val, Uint1 level, Int4 start, Int4 stop)
{
  Int4 i;
  Int4 len = 0;

  for(i = start; i <= stop; ++i)
    {
      if(val[i] >= level)
        ++len;
    }
  return len;
}


static Boolean alignment_is_redundant
(SeqAlignPtr align, SeqIdPtr master_id, Uint1Ptr val, Uint1 level)
{
  Int4 len = 0;
  Int2 order;
  DenseSegPtr dsp;
  DenseDiagPtr ddp;
  StdSegPtr ssp;
  SeqLocPtr slp;
  Int4 c_len, r_len;
  Boolean load;
  Int2 i;
  Int4 start = 0 , stop = 0;
  SeqIdPtr sip;

  len = 0;
  r_len = 0;
  while(align) {
    switch(align->segtype) {
    case 3:
      ssp = (StdSegPtr)align->segs;
      while(ssp) {
        load = TRUE;
        for(slp = ssp->loc; slp != NULL; slp = slp->next) {
          if(slp->choice != SEQLOC_NULL && slp->choice != SEQLOC_EMPTY)
            {
              if(SeqIdMatch(master_id, SeqLocId(slp)))
                {
                  start = SeqLocStart(slp);
                  stop = SeqLocStop(slp);
                  c_len = SeqLocLen(slp);
                }
            }
          else
            {
              load = FALSE;
              break;
            }
        }
        if(load)
          {
            len += c_len;
            r_len += get_redundant_len(val, level, start, stop);
          }
        ssp = ssp->next;
      }
      break;
    case 1:
      ddp = (DenseDiagPtr)align->segs;
      while(ddp) {
        for(order = 0, sip = ddp->id; sip != NULL; sip = sip->next, ++order) {
          if(SeqIdMatch(sip, master_id)) {
            r_len += get_redundant_len(val, level, 
                                       ddp->starts[order],
                                       ddp->starts[order]+ddp->len-1);
            break;
          }
        }
        len += ddp->len;
        ddp = ddp->next;

      }
      break;
    case 2:
      dsp = (DenseSegPtr)align->segs;
      for(sip = dsp->ids, order = 0; sip != NULL; sip = sip->next, ++order) {
        if(SeqIdMatch(sip, master_id))
          break;
      }
      for(i = 0; i<dsp->numseg; ++i) {
        if(dsp->starts[2*i] != -1 && dsp->starts[2*i+1] != -1) {
          r_len += get_redundant_len(val, level, 
                                     dsp->starts[2*i+order],
                                     dsp->starts[2*i+order]+dsp->lens[i] -1);
          len += dsp->lens[i];
        }
      }
      break;
    default:
      break;
    }
    align = align->next;
  }

  if(len == 0)
    return TRUE;
  return ((FloatHi)r_len/(FloatHi)len > 0.7);
}


static void update_this_interval
(Uint1Ptr val, Int4 start, Int4 stop)
{
  while(start <= stop)
    ++val[start++];
}

#define MAX_GAP_VAL 20
static void update_redundant_val
(SeqAlignPtr align, Uint1Ptr val, SeqIdPtr master_sip)
{
  SeqIdPtr sip;
  Int2 order;
  DenseSegPtr dsp;
  DenseDiagPtr ddp;
  StdSegPtr ssp;
  SeqLocPtr slp;
  Int4 c_len;
  Boolean load;
  Int2 i;
  Int4 start = 0, stop = 0;

  switch(align->segtype) {
  case 3:
    ssp = (StdSegPtr)align->segs;
    while(ssp) {
      load = TRUE;
      c_len = 0;
      for(slp = ssp->loc; slp != NULL; slp = slp->next) {
        if(slp->choice != SEQLOC_NULL && slp->choice != SEQLOC_EMPTY) {
          if(SeqIdMatch(master_sip, SeqLocId(slp)))
            {
              start = SeqLocStart(slp);
              stop = SeqLocStop(slp);
              c_len = SeqLocLen(slp);
            }
        }
        else
          {
            load = FALSE;
          }
      }
      if(load || (c_len > 0 && c_len < MAX_GAP_VAL))
        update_this_interval(val, start, stop);
      ssp = ssp->next;
    }
    break;
  case 1:
    ddp = (DenseDiagPtr)align->segs;
    while(ddp) {
      for(order = 0, sip = ddp->id; sip != NULL; sip = sip->next, ++order) {
        if(SeqIdMatch(sip, master_sip)) {
          update_this_interval(val, ddp->starts[order],
                               ddp->starts[order]+ddp->len-1);
          break;
        }
      }
      ddp = ddp->next;

    }
    break;
  case 2:
    dsp = (DenseSegPtr)align->segs;
    for(sip = dsp->ids, order = 0; sip != NULL; sip = sip->next, ++order) {
      if(SeqIdMatch(sip, master_sip))
        break;
    }
    for(i = 0; i<dsp->numseg; ++i) {
      if(dsp->starts[2*i] != -1) {
        if(dsp->starts[2*i+1] != -1 || dsp->lens[i] < MAX_GAP_VAL)
          update_this_interval(val, dsp->starts[2*i+order],
                               dsp->starts[2*i+order]+dsp->lens[i] -1);
      }
    }
    break;
  default:
    break;
  }
}

static SeqIdPtr get_hit_id
(SeqAlignPtr align, SeqIdPtr master_sip)
{
  SeqIdPtr sip;
  DenseSegPtr dsp;
  DenseDiagPtr ddp;
  StdSegPtr ssp;
  SeqLocPtr slp;

  if(align == NULL || master_sip == NULL)
    return NULL;
  switch(align->segtype)
    {
    case 3:
      ssp = (StdSegPtr)align->segs;
      while(ssp)
        {
          for(slp = ssp->loc; slp != NULL; slp = slp->next)
            {
              if(slp->choice != SEQLOC_NULL && slp->choice != SEQLOC_EMPTY)
                {
                  if(!SeqIdMatch(master_sip, SeqLocId(slp)))
                    return SeqLocId(slp);
                }
            }
          ssp = ssp->next;
        }
      return NULL;

    case 1:
      ddp = (DenseDiagPtr)align->segs;
      while(ddp)
        {
          for(sip = ddp->id; sip != NULL; sip = sip->next)
            {
              if(!SeqIdMatch(sip, master_sip))
                return sip;
            }
          ddp = ddp->next;
        }
      return NULL;
    case 2:
      dsp = (DenseSegPtr)align->segs;
      for(sip = dsp->ids; sip != NULL; sip = sip->next)
        {
          if(!SeqIdMatch(sip, master_sip))
            return sip;
        }
      return NULL;
    default:
      return NULL;
    }
}


static SeqAlignPtr extract_align_with_same_id
(SeqAlignPtr PNTR align, SeqIdPtr c_sip, SeqIdPtr m_sip)
{
  SeqAlignPtr prev, next;
  SeqAlignPtr curr;
  SeqAlignPtr list = NULL, p_align;
  SeqIdPtr n_sip;

  if(*align == NULL)
    return NULL;
  list = NULL;
  prev = NULL;
  p_align = NULL;
  curr = *align;
  while(curr)
    {
      next = curr->next;
      n_sip = get_hit_id(curr, m_sip);
      if(SeqIdMatch(n_sip, c_sip))
        {
          if(prev == NULL)
            *align = next;
          else
            prev->next = next;
          curr->next = NULL;
          if(p_align == NULL)
            list = curr;
          else
            p_align->next = curr;
          p_align = curr;
        }
      else
        prev = curr;
      curr = next;
    }

  return list;
}

static void free_align_chain(SeqAlignPtr align)
{
  SeqAlignPtr next;

  while(align)
    {
      next = align->next;
      align->next = NULL;
      SeqAlignFree(align);
      align = next;
    }
}


/*
 *
 *	modify the current chain of alignment so that for the same region, only 
 *	number==level of alignment exists 
 *	
 */
static SeqAlignPtr ModMaxAlign
(SeqAlignPtr PNTR h_align, BioseqPtr m_bsp, Uint1 level)
{
  Uint1Ptr val;
  SeqIdPtr m_sip;
  SeqAlignPtr curr, next;
  SeqAlignPtr new_align, prev_align;
  SeqIdPtr c_sip;
  SeqAlignPtr list;

  if(level == 0)
    return (*h_align);
  if(*h_align == NULL || m_bsp == NULL)
    return (*h_align); 
  m_sip = m_bsp->id;

  val = (Uint1Ptr)MemNew((size_t)m_bsp->length * sizeof(Int1));
  curr = *h_align;
  new_align = NULL;
  prev_align = NULL;
  curr = *h_align;
  while(curr)
    {
      next = curr->next;
      curr->next = NULL;
      if(next != NULL)
        {
          c_sip = get_hit_id(curr, m_sip);
          curr->next = extract_align_with_same_id(&next, c_sip, m_sip);
        }	
      if(alignment_is_redundant(curr, m_sip, val, level))
        free_align_chain(curr);
      else
        {
          if(prev_align == NULL)
            new_align = curr;
          else
            prev_align->next = curr;
          for(list = curr; list != NULL; list = list->next)
            update_redundant_val(list, val, m_sip);
          while(curr->next != NULL)
            curr = curr->next;
          prev_align = curr;
        }
      curr = next;
    }

  MemFree(val);

  *h_align = new_align;

  return *h_align;
}


/*******************************************************************
 *  EXTERNAL
 */

#define MAX_LEVEL	50

NLM_EXTERN Boolean PrintAlignmentOverview
(SeqAnnotPtr h_annot,
 FILE*       fp,
 const Char* formname,
 const Char* href,
 const Char* gif_name,
 const Char* title)
{
   return PrintOneAlignmentOverview(h_annot, fp, formname, href, gif_name, 
                                    title, 1);
}

NLM_EXTERN Boolean PrintOneAlignmentOverview
(SeqAnnotPtr h_annot,
 FILE*       fp,
 const Char* formname,
 const Char* href,
 const Char* gif_name,
 const Char* title,
 int query_index)
{
  SeqEntryPtr sep;
  Boolean is_new_sep;
  SeqAnnotPtr sap, annot, prev;
  GlobalDrawPtr g_draw;
  BoxInfo pBox;
  Int2 width, height;
  Char label[101];
  Boolean retval = FALSE;
  SegmenT pic;
  Uint2 entityID;
  BioseqPtr master_bsp, bsp;
  SeqIdPtr sip;
  FILE *gif_fp;
  Int4 align_num;
  Char title_buf[201], tmpbuf[1024];
  SeqIdPtr idtemp, idtail;
  char *tmp_base_ptr = NULL;
  char tmp_base[1024];

  if(h_annot == NULL || fp == NULL)
    return FALSE;
  sip = make_master((SeqAlignPtr)h_annot->data);
  if(sip == NULL)
    return FALSE;

  master_bsp = BioseqLockById(sip);
  if(master_bsp == NULL)
    return FALSE;
  bsp = master_bsp;
  /* Make sure a gi id is used, if possible, since this is the id that is 
     saved in the seqalign. */
  idtemp= bsp->id;
  bsp->id = SeqIdFindBest(bsp->id, SEQID_GI);
  idtail = bsp->id->next;
  bsp->id->next = NULL;
  sep = SeqEntryFind(bsp->id);
  if(sep == NULL)
    {
      sep = SeqEntryNew();
      sep->choice = 1;
      sep->data.ptrvalue = bsp;
      is_new_sep = TRUE;
      SeqMgrSeqEntry (SM_BIOSEQ, (Pointer)bsp, sep);
      entityID = ObjMgrRegister(OBJ_SEQENTRY, (Pointer)sep);
    }
  else
    is_new_sep = FALSE;

  align_num = get_number_alignment((SeqAlignPtr)(h_annot->data)); 
  annot = (SeqAnnotPtr)AsnIoMemCopy((Pointer)h_annot,
                                    (AsnReadFunc)SeqAnnotAsnRead,
                                    (AsnWriteFunc)SeqAnnotAsnWrite);
  ModMaxAlign((SeqAlignPtr PNTR)&(annot->data), bsp, MAX_LEVEL); 

  sap = bsp->annot;
  if(sap == NULL)
    bsp->annot = annot;
  else
    {
      while(sap->next != NULL)
        sap = sap->next;
      sap->next = annot;
    } 

  InitMuskStyles();
  SetMuskCurrentSt("style2");
  Nlm_ResetDrawingTools ();

  g_draw = NULL;
  pic = SequinGlobalPicture(sep, DEFAULT_X_SIZE, DEFAULT_X_SIZE, NULL,
                            &g_draw, TRUE);
  if (pic != NULL) {
    RecalculateSegment(pic, 1, 1);
    SegmentBox ( pic, &pBox);
    width  = (Int2)MIN(GIF_WIDTH_MAX, (pBox.right - pBox.left));
    height = (Int2)MIN(GIF_HEIGHT_MAX, pBox.top - pBox.bottom);

    fprintf(fp, "</PRE>\n");

    fprintf(fp, "<CENTER>\n");
#if 0    
    fprintf(fp, "<H3><a href=\"/BLAST/newoptions.html#graphical-overview\"> "
            "Distribution of %ld Blast Hits on the Query Sequence</a></H3>\n",
            (long)align_num);  
#else
    fprintf(fp, "%s", title);
#endif    
    StringCpy(title_buf, "Mouse-over to show defline and scores. "
              "Click to show alignments");
    fprintf(fp, "<input name=defline size=80 value=\"%s\">\n", title_buf);
    fprintf(fp, "</CENTER>\n");


    /*print out the image map*/
    if(!PrintImageMapForGlobal(g_draw, pBox.left, pBox.top, 1, title_buf,
                               (SeqAlignPtr)(annot->data), fp, formname,
                               query_index))
      printf("Fail to print the image map\n");
    fflush(fp);

    /*print out the title*/
    MuskSeqIdWrite (bsp->id, label, 100, PRINTID_FASTA_LONG, TRUE, TRUE);

    /* These temporary gifs will be stored in the {TMP_BASE} + TmpGif directory */
    tmp_base_ptr = getenv("GIFTMP_BASE");
    if( tmp_base_ptr ){
       strcpy(tmp_base,tmp_base_ptr);
       if( tmp_base[ strlen(tmp_base) -1 ] != '/') strcat(tmp_base,"/");
       sprintf(tmpbuf, "%s%s/%s",tmp_base, TMP_GIF_DIR, gif_name);
    }
    else {  
       sprintf(tmpbuf, "%s/%s", TMP_GIF_DIR, gif_name);
    }
    gif_fp = FileOpen(tmpbuf, "wb");

    if ( !gif_fp ) {
        fprintf(fp, "fail to open file %s", tmpbuf);
        FileRemove((CharPtr)tmpbuf);
    }
    else {
      PictureToGIF(gif_fp, width, height, pic, pBox.left, pBox.top,
                   UPPER_LEFT, 1, 1, FALSE);
      FileClose(gif_fp);
      retval = TRUE;
    }

    DeletePicture(pic);
    ExitMuskStyles();
    fprintf(fp, "<CENTER>\n");

    fprintf(fp, "<IMG WIDTH=%d HEIGHT=%d USEMAP=#img_map%d ORDER=1 "
            "SRC=\"%s%s\" ISMAP>", 
            (int)width, (int)height, query_index, href, gif_name);

    fprintf(fp, "</CENTER>\n");
    fprintf(fp, "<HR>\n<PRE>\n");

    fflush(fp);
  }

  if(g_draw != NULL)
    free_global_draw(g_draw);
  /*
    if(is_new_sep)
    {
    sep->data.ptrvalue = NULL;
    SeqEntryFree(sep);
    }
    */

  sap = bsp->annot;
  prev = NULL;
  while(sap->next != NULL)
    {
      prev = sap;
      sap = sap->next;
    }
  if(prev == NULL)
    bsp->annot = NULL;
  else
    prev->next = NULL;
  SeqAnnotFree(sap);
  /* Restore the link of seqids if it was broken */
  bsp->id->next = idtail;
  /* Restore the original first seqid in the bioseq */
  master_bsp->id = idtemp; 
  BioseqUnlock(master_bsp);

  return retval;
}

/* EOF */
