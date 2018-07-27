/*   $Id: blocks.c,v 6.3 1999/11/26 15:42:23 vakatov Exp $
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
* File Name:  blocks.c
*
* Author:  Sarah Wheelan
*
* Version Creation Date:   5/99
*
* $Revision: 6.3 $
*
* File Description: Indexed SeqAlign Functions
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: blocks.c,v $
* Revision 6.3  1999/11/26 15:42:23  vakatov
* Fixed for the C++ and/or MSVC DLL compilation
*
* Revision 6.2  1999/08/27 14:54:03  durand
* fix memory leaks in PopSet Viewer
*
* Revision 6.1  1999/07/06 19:49:14  kans
* initial public checkin
*
* Revision 1.22  1999/07/02 13:20:42  wheelan
* fixed more Blockify bugs
*
* Revision 1.21  1999/07/01 20:57:15  wheelan
* fixed minus strand problems by adding CalcMinusBounds, FlipSequence, and PropagateStrandInfo; also adds PatricksSABlock structure printing function; also lots of changes in SAMergeBlocks to correctly manage minus strands
*
* Revision 1.20  1999/06/29 16:09:57  wheelan
* fixed bugs in Blockify; forced SplitBlocks to correctly set strand
*
* Revision 1.19  1999/06/25 20:56:11  wheelan
* fixed major bug in IndexBlocks, fixed a few problems in Blockify
*
* Revision 1.18  1999/06/24 20:44:36  wheelan
* added IndexNewBlocks to correctly index SABlocks made from DenseDiags
*
* Revision 1.17  1999/06/24 11:37:00  wheelan
* fixed Blockify to set visible to 1
*
* Revision 1.16  1999/06/23 16:52:34  wheelan
* removed some unused variables
*
* Revision 1.15  1999/06/21 12:08:13  wheelan
* added SeqAlignListMergeAll, debugged SAMerge and SquishBlocks
*
* Revision 1.14  1999/06/11 19:15:49  wheelan
* Fixed bugs in SAMergeBlocks, and modified IndexBlocks to propagate the SeqId across each bioseq (cannot write to dense-seg structure if no SeqId for every segment of first block)
*
* Revision 1.13  1999/06/11 18:01:50  wheelan
* Changed seqalign type to 255 in DeBlockify, since type = 0 does not work with
salsa
*
* Revision 1.11  1999/06/09 21:18:38  wheelan
* Modified Blockify to work for dense-diags.  Fixed bugs in IndexBlocks, TeenyBlock, and SASplitBlock.
*
* Revision 1.10  1999/05/29 00:09:15  lewisg
* new editing functions
*
* Revision 1.9  1999/05/24 23:10:50  lewisg
* more initialization for AddEmptyBlocks
*
*
* ==========================================================================
*/


#include <blocks.h>
#include <ncbi.h>
#include <math.h>

static void FlipSequence(SegmentPtr segp, Int4 len);
static void CalcMinusBounds(SegmentPtr segp, SegmentPtr PNTR segp_storage);


/*******************************************************************************

  Function : blk_PrintSABP()
  
  Purpose : display an indexed seqalign as a matrix (debug purpose only)
  
  Parameters :  sabp;header of the indexed seqalign
                                
  Return value : -

*******************************************************************************/
NLM_EXTERN void blk_PrintSABP(SABlockPtr sabp)
{
FILE        *hFile;
SABlockPtr      sabp2;  /*used to scan "SeqALign" Index blocks*/
SegmentPtr      segp,segp2;     /*used to scan each Bsp segment in a block*/

        hFile=fopen("zsabp.txt","w");
        sabp2=sabp;
        /*write the align coord*/
        while (sabp2){
                fprintf(hFile,"{%4i}[%7i..%7i]",sabp2->SegID,sabp2->from,sabp2->to);
                sabp2=sabp2->next;
                if (sabp2) fprintf(hFile,"<-->");
        }
        fprintf(hFile,"\n");
        sabp2=sabp;
        while (sabp2){
                fprintf(hFile,"============================");
                sabp2=sabp2->next;
        }
        segp=sabp->segp_head;
        fprintf(hFile,"\n\n");
        /*write the SABP : line by line, bsp by bsp*/
        while(segp){/*bsp loop*/
                segp2=segp;
                while(segp2){/*segment loop for one bsp; from-to*/
                        fprintf(hFile,"(%4i)[%7i..%7i]",segp2->BspID,segp2->from,segp2->to);
                        segp2=segp2->bsp_next;
                        if (segp2) fprintf(hFile,"<-->");
                }
                segp2=segp;
                fprintf(hFile,"\n");
                while(segp2){/*segment loop for one bsp; gap strand*/
                        fprintf(hFile,"      (%c) (%c) (%c)           ",
                                (segp2->gap>0 ? '-':'a' ),
                                (segp2->strand!=Seq_strand_minus ? '+':'-' ),
                                (segp2->visible>0 ? '1':'0' ));
                        segp2=segp2->bsp_next;
                }
                segp2=segp;
                fprintf(hFile,"\n");
                while (segp2){
                        fprintf(hFile,"----------------------------");
                        segp2=segp2->next;
                }
                fprintf(hFile,"\n");
                segp=segp->next;
        }
        fprintf(hFile,"\n\n");
        fclose(hFile);  
}

/******************************************************************
*  Blockify converts a seqalign to the SABlock structure.  It
*  does all the necessary memory allocation, so the structure
*  will need to be freed with CleanupBlocks once it's no longer
*  used.  The top-level coordinates (in the SABlocks) refer to
*  the alignment coordinates, starting from zero, and the 
*  coordinates in each chain of Segments refer to bioseq 
*  coordinates, also starting from zero.  Gaps are coded by the
*  last real residue before the gap (e.g.  for aaacct----ttt 
*  the gap has segp->from = segp->to = 5, which is the coordinate
*  of the 't' before the gap).  Therefore, segp->gap MUST be
*  set to something other than zero to signify that this is a
*  gap and not a one-residue block.
*******************************************************************/

NLM_EXTERN SABlockPtr Blockify(SeqAlignPtr sap)
{
   Boolean       added;
   Int4          bioseq;
   Int4          block;
   Int4          counter;
   DenseDiagPtr  ddp;
   DenseSegPtr   dsp;
   SABlockPtr    head_sabp;
   Int4          i;
   Int2          index;
   Int4          master_from;
   Int4          master_to;
   SABlockPtr    prev_sabp;
   SABlockPtr    sabp;
   SABlockPtr    sabp_new;
   SegmentPtr    segp;
   SegmentPtr    segp_head;
   SegmentPtr    segp_prev;
   SegmentPtr    segp_storage;
   SegmentPtr    segp_temp;
   SegmentPtr    segp_temp2;
   SegmentPtr    segp_temp3;
   Int4          shift;
   SeqIdPtr      sip;
   SeqLocPtr     slp;
   StdSegPtr     ssp;

   bioseq = block = 1;
   if (sap->segtype == 1)
   {
      ddp = (DenseDiagPtr)sap->segs;
      if (ddp == NULL)
         return NULL;
      if (ddp->dim != 2)
      {
         ErrPostEx(SEV_WARNING,0,0,"Need pairwise alignment as input\n");
         return NULL;
      }
      head_sabp = NULL;
      segp_storage = segp_temp = NULL;
      segp_storage = (SegmentPtr)MemNew(sizeof(Segment));
      segp_temp = (SegmentPtr)MemNew(sizeof(Segment));
      segp_storage->next = segp_temp;
      segp_storage->sip = SeqIdDup(ddp->id);
      segp_temp->sip = SeqIdDup(ddp->id->next);
      segp_storage->from = ddp->starts[0];
      segp_storage->to = segp_storage->from + ddp->len - 1;
      segp_storage->link = TRUE;
      segp_storage->visible = 1;
      segp_temp->link = TRUE;
      segp_temp->from = ddp->starts[1];
      segp_temp->to = segp_temp->from + ddp->len - 1;
      segp_temp->prev = segp_storage;
      segp_temp->visible = 1;
      head_sabp = sabp = (SABlockPtr)MemNew(sizeof(SABlock));
      sabp->segp_head = segp_storage;
      sabp->aligned = TRUE;
      sabp->from = ddp->starts[0];
      sabp->to = ddp->starts[0] + ddp->len - 1;
      sabp->visible = 1;
      if (ddp->strands)
      {
         if (ddp->strands[0] == Seq_strand_minus)
         {
            segp_storage->strand = Seq_strand_plus;
            if (ddp->strands[1] != Seq_strand_minus)
               segp_temp->strand = Seq_strand_minus;
            else
               segp_temp->strand = Seq_strand_plus;
         } else
         {
            segp_storage->strand = ddp->strands[0];
            segp_temp->strand = ddp->strands[1];
         }
      } else
      {
         segp_storage->strand = segp_temp->strand = Seq_strand_unknown;
      }
      counter = 1;
      ddp = ddp->next;
      while (ddp)
      {
         if (ddp->dim != 2)
         {
            ErrPostEx(SEV_WARNING,0,0,"Need pairwise alignment as input\n");
            return NULL;
         }
         while (!SeqIdComp(segp_storage->sip, ddp->id))
         {
            ddp = ddp->next;
         }
         if (ddp->strands)
         {
            if (ddp->strands[0] == Seq_strand_minus)
            {
               ddp->strands[0] = Seq_strand_plus;
               if (ddp->strands[1] == Seq_strand_minus)
                  ddp->strands[1] = Seq_strand_plus;
               else
                  ddp->strands[1] = Seq_strand_minus;
            }
         }
         segp_storage->next->sip = SeqIdDup(ddp->id->next);
         head_sabp = AddEmptySegments(head_sabp);
         counter += 1;
         sabp = head_sabp;
         added = FALSE;
         while (!added)
         {
            if (ddp->starts[0] + ddp->len - 1 < sabp->from)
            {
               sabp_new = (SABlockPtr)MemNew(sizeof(SABlock));
               sabp_new->from = ddp->starts[0];
               sabp_new->to = ddp->starts[0] + ddp->len - 1;
               sabp_new->aligned = TRUE;
               sabp_new->next = sabp;
               sabp_new->visible = 1;
               if (sabp->prev)
               {
                  sabp_new->prev = sabp->prev;
                  sabp->prev->next = sabp_new;
                  sabp->prev = sabp_new;
               } else
               {
                  head_sabp->prev = sabp_new;
                  head_sabp = sabp_new;
               }
               segp_head = NULL;
               for (i = 0; i<= counter; i++)
               {
                  segp_temp = (SegmentPtr)MemNew(sizeof(Segment));
                  if (segp_head)
                  {
                     segp_temp->prev = segp_prev;
                     segp_prev->next = segp_temp;
                     segp_prev = segp_temp;
                     segp_temp->visible = 1;
                     segp_temp->gap = 2;
                  } else
                  {
                     segp_head = segp_prev = segp_temp;
                     segp_temp->from = sabp_new->from;
                     segp_temp->to = sabp_new->to;
                     segp_temp->sip = segp_storage->sip;
                     segp_temp->link = TRUE;
                     segp_temp->visible = 1;
                  }
               }
               segp_temp->from = ddp->starts[1];
               segp_temp->to = ddp->starts[1] + ddp->len - 1;
               segp_temp->sip = segp_storage->next->sip;
               segp_temp->gap = 0;
               segp_temp->link = TRUE;
               segp_temp->visible = 1;
               if (ddp->strands)
               {
                  segp_head->strand = ddp->strands[0];
                  segp_temp->strand = ddp->strands[1];
               } else
               {
                  segp_head->strand = segp_temp->strand = Seq_strand_unknown;
               }
               sabp_new->segp_head = segp_head;
               ddp = ddp->next;
               added = TRUE;
            } else if (ddp->starts[0] < sabp->from)
            {
               sabp_new = (SABlockPtr)MemNew(sizeof(SABlock));
               sabp_new->next = sabp;
               sabp_new->aligned = TRUE;
               sabp_new->from = ddp->starts[0];
               sabp_new->to = sabp->from - 1;
               sabp_new->visible = 1;
               if (sabp->prev)
               {
                  sabp_new->prev = sabp->prev;
                  sabp->prev->next = sabp_new;
                  sabp->prev = sabp_new;
               } else
               {
                  sabp->prev = sabp_new;
                  head_sabp = sabp_new;
               }
               segp_head = NULL;
               for (i = 0; i<= counter; i++)
               {
                  segp_temp = (SegmentPtr)MemNew(sizeof(Segment));
                  if (segp_head)
                  {
                     segp_temp->prev = segp_prev;
                     segp_prev->next = segp_temp;
                     segp_prev = segp_temp;
                     segp_temp->gap = 2;
                     segp_temp->visible = 1;
                  } else
                  {
                     segp_head = segp_prev = segp_temp;
                     segp_temp->from = sabp_new->from;
                     segp_temp->to = sabp_new->to;
                     segp_temp->sip = segp_storage->sip;
                     segp_temp->link = TRUE;
                     segp_temp->visible = 1;
                  }
               }
               shift = sabp_new->to - sabp_new->from;
               segp_temp->from = ddp->starts[1];
               segp_temp->to = ddp->starts[1] + shift;
               segp_temp->gap = 0;
               segp_temp->sip = segp_storage->next->sip;
               segp_temp->link = TRUE;
               segp_temp->visible = 1;
               if (ddp->strands)
               {
                  segp_head->strand = ddp->strands[0];
                  segp_temp->strand = ddp->strands[1];
               } else
               {
                  segp_head->strand = segp_temp->strand = Seq_strand_unknown;
               }
               sabp_new->segp_head = segp_head;
               ddp->starts[0] += (shift+1);
               ddp->starts[1] += (shift+1);
               ddp->len -= (shift+1);
            } else if (ddp->starts[0] == sabp->from)
            {
               if (ddp->starts[0] + ddp->len - 1 > sabp->to)
               {
                  segp_temp = sabp->segp_head;
                  shift = sabp->to - sabp->from;
                  for (i = 0; i< counter; i++)
                  {
                     segp_temp = segp_temp->next;
                  }
                  segp_temp->from = ddp->starts[1];
                  segp_temp->to = ddp->starts[1] + shift;
                  segp_temp->gap = 0;
                  segp_temp->sip = segp_storage->next->sip;
                  segp_temp->link = TRUE;
                  segp_temp->visible = 1;
                  if (ddp->strands)
                  {
                     segp_temp->strand = ddp->strands[1];
                  } else
                  {
                     segp_temp->strand = Seq_strand_unknown;
                  }
                  ddp->starts[0] = ddp->starts[0] + shift + 1;
                  ddp->starts[1] = ddp->starts[1] + shift + 1;
                  ddp->len = ddp->len - shift - 1;
                  sabp = sabp->next;
               } else if (ddp->starts[0] + ddp->len - 1 == sabp->to)
               {
                  segp_temp = sabp->segp_head;
                  for (i=0; i<counter; i++)
                  {
                     segp_temp = segp_temp->next;
                  }
                  segp_temp->from = ddp->starts[1];
                  segp_temp->to = ddp->starts[1] + ddp->len - 1;
                  segp_temp->gap = 0;
                  segp_temp->sip = segp_storage->next->sip;
                  segp_temp->link = TRUE;
                  segp_temp->visible = 1;
                  if (ddp->strands)
                  {
                     segp_temp->strand = ddp->strands[1];
                  } else
                  {
                     segp_temp->strand = Seq_strand_unknown;
                  }
                  ddp = ddp->next;
                  added = TRUE;
               } else
               {
                  sabp = SASplitBlock(sabp, (ddp->starts[0] + ddp->len));
                  segp_temp = sabp->segp_head;
                  for (i = 0; i<counter; i++)
                  {
                     segp_temp = segp_temp->next;
                  }
                  segp_temp->from = ddp->starts[1];
                  segp_temp->to = ddp->starts[1] + ddp->len - 1;
                  segp_temp->gap = 0;
                  segp_temp->sip = segp_storage->next->sip;
                  segp_temp->link = TRUE;
                  segp_temp->visible = 1;
                  if (ddp->strands)
                  {
                     segp_temp->strand = ddp->strands[1];
                  } else
                  {
                     segp_temp->strand = Seq_strand_unknown;
                  }
                  ddp = ddp->next;
                  added = TRUE;
               }
            } else if (ddp->starts[0] > sabp->from && ddp->starts[0] <= sabp->to)
            {
               sabp = SASplitBlock(sabp, ddp->starts[0]);
               sabp = sabp->next;
            } else
            {
               sabp = sabp->next;
            }
            if (!sabp && !added)
            {
               sabp = head_sabp;
               while (sabp->next)
               {
                  sabp = sabp->next;
               }
               sabp_new = (SABlockPtr)MemNew(sizeof(SABlock));
               sabp_new->from = ddp->starts[0];
               sabp_new->to = ddp->starts[0] + ddp->len - 1;
               sabp_new->aligned = TRUE;
               sabp_new->prev = sabp;
               sabp_new->visible = 1;
               sabp->next = sabp_new;
               segp_head = NULL;
               for (i = 0; i<= counter; i++)
               {
                  segp_temp = (SegmentPtr)MemNew(sizeof(Segment));
                  if (segp_head)
                  {
                     segp_prev->next = segp_temp;
                     segp_temp->prev = segp_prev;
                     segp_prev = segp_temp;
                     segp_temp->gap = 2;
                     segp_temp->visible = 1;
                  } else
                  {
                     segp_head = segp_prev = segp_temp;
                     segp_temp->from = ddp->starts[0];
                     segp_temp->to = ddp->starts[0] + ddp->len - 1;
                     segp_temp->sip = segp_storage->sip;
                     segp_temp->link = TRUE;
                     segp_temp->visible = 1;
                  }
               }
               segp_temp->from = ddp->starts[1];
               segp_temp->to = ddp->starts[1] + ddp->len - 1;
               segp_temp->gap = 0;
               segp_temp->sip = segp_storage->next->sip;
               segp_temp->link = TRUE;
               segp_temp->visible = 1;
               if (ddp->strands)
               {
                  segp_head->strand = ddp->strands[0];
                  segp_temp->strand = ddp->strands[1];
               } else
               {
                  segp_head->strand = segp_temp->strand = Seq_strand_unknown;
               }
               sabp_new->segp_head = segp_head;
               ddp = ddp->next;
               added = TRUE;
            }
         }
      }
      head_sabp = FillInUnaligned(head_sabp);
      head_sabp = IndexNewBlocks(head_sabp);
      segp_temp = head_sabp->segp_head;
      segp_temp3 = (SegmentPtr)MemNew(sizeof(Segment));
      while (segp_temp)
      {
         if (segp_temp->strand == Seq_strand_unknown)
         {
            segp_temp = PropagateStrandInfo(segp_temp);
         }
         if (segp_temp->strand == Seq_strand_minus)
         {
            CalcMinusBounds(segp_temp, &segp_temp3);
            FlipSequence(segp_temp, (segp_temp3->to + segp_temp3->from));
         }
         segp_temp = segp_temp->next;
      }
      MemFree(segp_temp3);
      return head_sabp;
   } else if (sap->segtype == 2) /*DENSE-SEG*/
   {
      dsp = sap->segs;
      if (!dsp)
         return NULL;
      if (dsp->strands)
      {
         if (dsp->strands[0] == Seq_strand_minus)
         {
            sap = SeqAlignListReverseStrand(sap);
            dsp = sap->segs;
            if (!dsp)
               return NULL;
         }
      }
      index = 0;
      head_sabp = NULL;
      master_from = master_to = 0;
      segp_storage = segp_temp = NULL;
      while (index < dsp->numseg)
      {
         if (index == 0)
         {
            for (counter = 0; counter < dsp->dim; counter++)
            {
               segp_temp = (SegmentPtr)MemNew(sizeof(Segment));
               if (segp_storage)
               {
                  segp_temp2->next = segp_temp;
                  segp_temp2 = segp_temp;
               } else
               {
                  segp_storage = segp_temp2 = segp_temp;
               }
               segp_temp->to = -1;
            }
         }
         master_from = master_to + 1;
         master_to += dsp->lens[index];
         sabp = (SABlockPtr)MemNew(sizeof(SABlock));
         if (!head_sabp)
         {
            head_sabp = sabp;
            prev_sabp = sabp;
            sabp->prev = NULL;
         } else
         {
            prev_sabp->next = sabp;
            sabp->prev = prev_sabp;
            prev_sabp = sabp;
         }
         sabp->from = master_from-1;/*switch to zero-based values*/
         sabp->to = master_to-1;
         sabp->visible = 1;
         sabp->next = NULL;
         /* lyg: initialize the RegionID to correspond to alignment */
         sabp->RegionID = block;
         /* lyg: indicate that the block is aligned */
         sabp->aligned = TRUE; 
         sabp->SegID=block++;
         segp_head = NULL;
         sip=dsp->ids;
         segp_temp = segp_storage;
         for (i = 0; i< dsp->dim; i++,sip=sip->next)
         {
            segp = (SegmentPtr)MemNew(sizeof(Segment));
            if (!segp_head)
            {
               segp_head = segp;
               segp_prev = segp;
            } else
            {
               segp_prev->next = segp;
               segp->prev = segp_prev;
               segp_prev = segp;
            }
            if (dsp->starts[(dsp->dim*index) + i] == -1)
            {
               segp->gap = 1;
               segp->from = segp->to = segp_temp->to;
               segp->strand = Seq_strand_unknown;
            } else
            {
               segp->gap = 0;
               segp->from = dsp->starts[dsp->dim*index + i];
               segp->to = segp->from + dsp->lens[index]-1;
               segp_temp->to += dsp->lens[index];
               if(dsp->strands)
                   segp->strand = dsp->strands[dsp->dim*index + i];
               else
                   segp->strand = Seq_strand_plus;
            }
            segp->link=TRUE;
            segp->visible = 1;
            segp->move = FALSE;  /* lyg: no pending move */
            if (index == 0) segp->sip=SeqIdDup(sip);
            segp->BspID=bioseq++;
            segp_temp = segp_temp->next;
         }
         sabp->segp_head = segp_head;
         index += 1;
         bioseq=1;
      }
      segp_temp = segp_storage;
      while(segp_temp)
      {
         segp_temp2 = segp_temp->next;
         MemFree(segp_temp);
         segp_temp = segp_temp2;
      }
      head_sabp = IndexBlocks(head_sabp);
      return head_sabp;
   } else if (sap->segtype == 3) /*STD-SEG*/
   {
      ssp = sap->segs;
      if (!ssp)
         return NULL;
      head_sabp = NULL;
      while (ssp)
      {
         sabp = (SABlockPtr)MemNew(sizeof(SABlock));
         sabp->visible = 1;
         if (head_sabp)
         {
            prev_sabp->next = sabp;
            sabp->prev = prev_sabp;
         } else
         {
            prev_sabp = head_sabp = sabp;
         }
         slp = ssp->loc;
         segp_head = NULL;
         while (slp)
         {
            segp = (SegmentPtr)MemNew(sizeof(Segment));
            if (segp_head)
            {
               segp_prev->next = segp;
               segp->prev = segp_prev;
            } else
            {
               segp_prev = segp_head = segp;
            }
            segp->visible = 1;
            segp->from = SeqLocStart(slp);
            segp->to = SeqLocStop(slp);
            segp->sip = SeqIdDup(SeqLocId(slp));
            slp = slp->next;
         }
         sabp->segp_head = segp_head;
         ssp = ssp->next;
      }
      return head_sabp;
   } else
   {
      return NULL;
   }
}

/************************************************************
*  DeBlockify converts the SABlock structure to a SeqAlign
*  of segtype dense-seg.  For the structures that were 
*  originally diags, each diag ends up as a new "sequence" in
*  the alignment, with gaps on either side to fill out the
*  alignment.  DeBlockify does not free the SABlock structure.
************************************************************/

NLM_EXTERN SeqAlignPtr DeBlockify(SABlockPtr sabp)
{
   Int4         count_blocks;
   Int4         count_segments;
   DenseSegPtr  dsp;
   Int4         i;
   SABlockPtr   sabp_head;
   SeqAlignPtr  sap;
   SegmentPtr   segp;
   SeqIdPtr     sip;

   if (!sabp)
      return NULL;
   sap = SeqAlignNew();
   dsp = DenseSegNew();
   if (!dsp)
      return NULL;
   sabp_head = sabp;
   count_blocks = count_segments = 0;
   while (sabp)
   {
      count_blocks++;
      sabp = sabp->next;
   }
   sabp = sabp_head;
   segp = sabp->segp_head;
   while (segp)
   {
      count_segments++;
      segp = segp->next;
   }
   sap->segs = (Pointer)dsp;
   sap->type = 255;
   sap->segtype = 2;
   sap->dim = count_segments;
   dsp->dim = count_segments;
   dsp->numseg = count_blocks;
   dsp->lens = (Int4Ptr)MemNew((size_t)((count_blocks+2)*sizeof(Int4)));
   dsp->strands = (Uint1Ptr)MemNew((size_t)((count_segments*count_blocks+4)*sizeof(Uint1)));
   dsp->starts = (Int4Ptr)MemNew((size_t)((count_segments*count_blocks+4)*sizeof(Int4)));
   sip = GetSeqIdList(sabp);
   dsp->ids = sip;
   sabp = sabp_head;
   count_segments = count_blocks = 0;
   i = 0;
   while (sabp)
   {
      count_blocks++;
      dsp->lens[count_blocks-1] = sabp->to - sabp->from;
      segp = sabp->segp_head;
      while (segp)
      {
         dsp->strands[i] = segp->strand;
         if (segp->gap != 0)
         {
            dsp->starts[i] = -1;
         } else
         {
            dsp->starts[i] = segp->from;
         }
         segp = segp->next;
         i++;
      }
      sabp = sabp->next;
   }
   sap->segs = dsp;   
   return sap;
}

NLM_EXTERN SeqAlignPtr SeqAlignListMergeAll(SeqAnnotPtr sap)
{
   SABlockPtr   sabp;
   SABlockPtr   sabp_next;
   SeqAlignPtr  salp;

   if (!sap)
      return NULL;
   salp = (SeqAlignPtr)sap->data;
   if (!salp->next)
      return salp;
   sabp = sabp_next = NULL;
   while (salp)
   {
      if (sabp)
      {
         sabp_next = Blockify(salp);
         sabp = SAMergeBlocks(sabp, sabp_next);
      } else
      {
         sabp = Blockify(salp);
         sabp_next = Blockify(salp->next);
         sabp = SAMergeBlocks(sabp, sabp_next);
         salp = salp->next;
      }
      salp = salp->next;
   }
   salp = DeBlockify(sabp);
   return salp;
}

NLM_EXTERN SeqIdPtr GetSeqIdList(SABlockPtr sabp)
{
   SegmentPtr  segp;
   SeqIdPtr    sip;
   SeqIdPtr    sip_head;
   SeqIdPtr    sip_tmp;

   segp = sabp->segp_head;
   sip_head = NULL;
   while (segp)
   {
      sip = (SeqIdPtr)MemNew(sizeof(SeqId));
      if (segp->sip)
         sip = SeqIdDup(segp->sip);
      if (sip_head)
      {
         sip_tmp->next = sip;
         sip_tmp = sip;
      } else
      {
         sip_head = sip_tmp = sip;
      }
      segp = segp->next;
   }
   return sip_head;
}

/************************************************************
*  CleanupBlocks just frees all the memory associated with
*  the given SABlock structure, including all the memory
*  for all associated segment structures.
************************************************************/

NLM_EXTERN void CleanupBlocks(SABlockPtr sabp)
{
   Boolean     done;
   SegmentPtr  segp;
   SABlockPtr  sabp2;

   sabp2 = sabp;
   done = FALSE;
   while (sabp)
   {
      while (sabp->segp_head)
      {
         segp = sabp->segp_head;
         sabp->segp_head = segp->next;
         segp->prev = NULL;
		 if (!done && segp->sip){
		 	segp->sip=SeqIdFree(segp->sip);
		 }
         MemFree(segp);
         segp = sabp->segp_head;
      }
      sabp = sabp->next;
      done = TRUE;
   }
   while (sabp2)
   {
      sabp = sabp2->next;
      sabp2->prev = NULL;
      MemFree(sabp2);
      sabp2 = sabp;
   }
   return;
}

/***************************************************************
*  IndexBlocks takes an assembled SABlock structure and 
*  recalculates the SegID and BspID for each block.  It also
*  recalculates the alignment coordinates by taking the length
*  of the first non-gap sequence in each block as the length
*  of the block (is there a better way to do this?).  The 
*  function uses several Segment structures to store the 
*  coordinates of the alignment and of each bioseq.  This way,
*  the gaps in the bioseqs can also be numbered (they are not
*  always numbered by the SAMergeBlocks function).  IndexBlocks
*  also creates the "horizontal" bsp_prev and bsp_next links
*  across each bioseq in the alignment.
*
*  lyg: please do not reindex RegionID
*
****************************************************************/

NLM_EXTERN SABlockPtr IndexBlocks(SABlockPtr sabp)
{
   Int4        bioseq;
   Int4        block;
   Boolean     found;
   SABlockPtr  head;
   SegmentPtr  segp;
   SegmentPtr  segp_align;
   SegmentPtr  segp_curr;
   SegmentPtr  segp_prev;
   SegmentPtr  segp_storage;

   if (!sabp)
   {
      return NULL;
   }
   block = 1;
   head = sabp;
   segp_align = (SegmentPtr)MemNew(sizeof(Segment));
   segp_align->to = segp_align->to = -1;
   segp_curr = segp_storage = NULL;
   while (sabp)
   {
      sabp->SegID = block;
      block++;
      segp = sabp->segp_head;
      if (!segp)
      {
         if (sabp->prev)
            sabp->prev->next = sabp->next;
         if (sabp->next)
            sabp->next->prev = sabp->prev;
         MemFree(sabp);
         block--;
      }
      bioseq = 1;
      found = FALSE;
      segp_curr = segp_storage;
      while (segp)
      {
         if (block == 2)
         {
            segp_curr = (SegmentPtr)MemNew(sizeof(Segment));
            if (segp_storage)
            {
               segp_prev->next = segp_curr;
               segp_prev = segp_curr;
            } else
            {
               segp_storage = segp_prev = segp_curr;
            }
            segp_curr->bsp_prev = segp;
         } else
         {
            segp->bsp_prev = segp_curr->bsp_prev;
            segp_curr->bsp_prev = segp;
            segp->bsp_prev->bsp_next = segp;
         }
         if (segp->gap != 0)
         {
            if (segp->gap == 1)
               segp->from = segp->to = segp_curr->to;
         } else
         {
            segp_curr->to = segp->to;
            if (!found)
            {
               segp_align->from = segp_align->to + 1;
               segp_align->to = segp_align->from + (segp->to - segp->from);
               found = TRUE;
            }
         }
         segp->BspID = bioseq;
         segp = segp->next;
         bioseq++;
         if (block > 2)
         {
            segp_curr = segp_curr->next;
         }
      }
      sabp->from = segp_align->from;
      sabp->to = segp_align->to;
      sabp = sabp->next;
   }
   MemFree(segp_align);
   segp_curr = segp_storage;
   while (segp_curr)
   {
      segp_curr = segp_storage->next;
      segp_storage->bsp_prev = segp_storage->bsp_next = NULL;
      MemFree(segp_storage);
      segp_storage = segp_curr;
   }
   segp_storage = head->segp_head;
   segp_curr = segp_storage;
   while (segp_curr)
   {
      if (!segp_curr->sip)
      {
         segp_align = segp_curr;
         while (segp_align->sip == NULL)
         {
            segp_align = segp_align->bsp_next;
         }
         segp_curr->sip = segp_align->sip;
      }
      segp_align = segp_curr->bsp_next;
      while (segp_align)
      {
         segp_align->sip = segp_curr->sip;
         segp_align = segp_align->bsp_next;
      }
      segp_curr = segp_curr->next;
   }
   return head;
}

NLM_EXTERN SegmentPtr PropagateStrandInfo(SegmentPtr segp)
{
   Boolean     done;
   SegmentPtr  segp_head;
   SegmentPtr  segp_temp;
   Uint1       strand;

   segp_head = segp;
   done = FALSE;
   strand = Seq_strand_unknown;
   while (!done && segp)
   {
      if (segp->strand != Seq_strand_unknown)
      {
         strand = segp->strand;
         done = TRUE;
      }
      segp = segp->bsp_next;
   }
   if (strand != Seq_strand_unknown)
   {
      segp_temp = segp_head;
      while (segp_temp)
      {
         segp_temp->strand = strand;
         segp_temp = segp_temp->bsp_next;
      }
   }
   return segp_head;
}


/***********************************************************
*  IndexNewBlocks does the same thing as IndexBlocks, but
*  for new structures created from DenseDiags, it sets all
*  the flags to visible, as Blockify does not traverse all
*  the segments to be able to do this.
***********************************************************/

NLM_EXTERN SABlockPtr IndexNewBlocks(SABlockPtr sabp)
{
   Int4        bioseq;
   Int4        block;
   Boolean     found;
   SABlockPtr  head;
   SegmentPtr  segp;
   SegmentPtr  segp_align;
   SegmentPtr  segp_curr;
   SegmentPtr  segp_prev;
   SegmentPtr  segp_storage;

   if (!sabp)
   {
      return NULL;
   }
   block = 1;
   head = sabp;
   segp_align = (SegmentPtr)MemNew(sizeof(Segment));
   segp_align->to = segp_align->to = -1;
   segp_curr = segp_storage = NULL;
   while (sabp)
   {
      sabp->SegID = block;
      block++;
      segp = sabp->segp_head;
      if (!segp)
      {
         if (sabp->prev)
            sabp->prev->next = sabp->next;
         if (sabp->next)
            sabp->next->prev = sabp->prev;
         MemFree(sabp);
         block--;
      }
      bioseq = 1;
      found = FALSE;
      segp_curr = segp_storage;
      while (segp)
      {
         if (block == 2)
         {
            segp_curr = (SegmentPtr)MemNew(sizeof(Segment));
            if (segp_storage)
            {
               segp_prev->next = segp_curr;
               segp_prev = segp_curr;
            } else
            {
               segp_storage = segp_prev = segp_curr;
            }
            segp_curr->bsp_prev = segp;
         } else
         {
            segp->bsp_prev = segp_curr->bsp_prev;
            segp_curr->bsp_prev = segp;
            segp->bsp_prev->bsp_next = segp;
         }
         if (segp->gap != 0)
         {
            if (segp->gap == 1)
               segp->from = segp->to = segp_curr->to;
            else if (segp->gap == 2)
               segp->from = segp->to = 0;
         } else
         {
            segp_curr->to = segp->to;
            if (!found)
            {
               segp_align->from = segp_align->to + 1;
               segp_align->to = segp_align->from + (segp->to - segp->from);
               found = TRUE;
            }
         }
         segp->BspID = bioseq;
         segp->visible = 1;
         segp = segp->next;
         bioseq++;
         if (block > 2)
         {
            segp_curr = segp_curr->next;
         }
      }
      sabp->from = segp_align->from;
      sabp->to = segp_align->to;
      sabp = sabp->next;
   }
   MemFree(segp_align);
   segp_curr = segp_storage;
   while (segp_curr)
   {
      segp_curr = segp_storage->next;
      segp_storage->bsp_prev = segp_storage->bsp_next = NULL;
      MemFree(segp_storage);
      segp_storage = segp_curr;
   }
   segp_storage = head->segp_head;
   segp_curr = segp_storage;
   while (segp_curr)
   {
      if (!segp_curr->sip)
      {
         segp_align = segp_curr;
         while (segp_align->sip == NULL)
         {
            segp_align = segp_align->bsp_next;
         }
         segp_curr->sip = segp_align->sip;
      }
      segp_align = segp_curr->bsp_next;
      while (segp_align)
      {
         segp_align->sip = segp_curr->sip;
         segp_align = segp_align->bsp_next;
      }
      segp_curr = segp_curr->next;
   }
   return head;
}


/******************************************************************
*  This function returns either the total length of the alignment
*  (in alignment coordinates), or the number of sequences involved
*  in the alignment, or the number of blocks in the alignment, or
*  all three, depending on whether any of length, numbioseqs or 
*  numblocks are NULL.  If the Int4Ptr validate is not NULL, the
*  function also goes through the segments for each block to make
*  sure that each block has the same number of segments, in the 
*  same order.  If this is not the case, *validate is set to the
*  SegID of the first block that contains a mistake.  If the 
*  alignment is valid, *validate will be zero.
******************************************************************/

NLM_EXTERN void GetBlockInfo(SABlockPtr sabp, Int4Ptr length, Int4Ptr numbioseqs, Int4Ptr numblocks, Int4Ptr validate)
{
   Int4        blocks;
   Int4        count;
   SegmentPtr  segp;
   ValNodePtr  vnp;
   ValNodePtr  vnp_head;

   if (!sabp)
   {
      return;
   }
   segp = sabp->segp_head;
   vnp = vnp_head = NULL;
   if (numbioseqs)
   {
      *numbioseqs = 0;
      while (segp)
      {
         *numbioseqs += 1;
         if (validate)
         {
            vnp = ValNodeAdd(&vnp_head);
            vnp->data.intvalue = segp->BspID;
         }
         segp = segp->next;
      }
   }
   if (length || numblocks || validate)
   {
      if (length)
         *length = 0;
      blocks = 0;
      if (validate)
      {
         *validate = 0;
      }
      while (sabp)
      {
         if (length)
            *length += (sabp->to - sabp->from + 1);
         blocks += 1;
         if (validate)
         {
            vnp = vnp_head;
            segp = sabp->segp_head;
            count = 0;
            while (segp)
            {
               count += 1;
               if (segp->BspID != vnp->data.intvalue)
               {
                  *validate = blocks;
                  ValNodeFree(vnp_head);
                  return;
               }
               vnp = vnp->next;
               segp = segp->next;
            }
            if (count != *numbioseqs)
            {
               *validate = blocks;
               ValNodeFree(vnp_head);
               return;
            }
         }
         sabp = sabp->next;
      }
      if (numblocks)
         *numblocks = blocks;
   }
   ValNodeFree(vnp_head);
   return;
}


/******************************************************************
*  SAMergeBlocks first finds the first sequence in the first
*  alignment that is also in the second alignment, and then merges
*  the two alignments using that sequence as a guide.  It is
*  fairly conservative with memory allocation -- when possible
*  it just modifies existing SABlocks and links them together.  The
*  structure that is returned has the same AlignID as the first 
*  alignment passed to the function.
*  SAMergeBlocks calls GetBlockOrientation to figure out how the
*  relationships between the shared sequences in each set of blocks,
*  and then uses this information to merge and split the blocks
*  as needed.  Note that the shared sequence is removed from the 
*  second set of blocks, so it shouldn't appear twice in the final
*  alignment.
*******************************************************************/

NLM_EXTERN SABlockPtr SAMergeBlocks (SABlockPtr sabp1, SABlockPtr sabp2)
{
   Int4        c;
   Boolean     done;
   Int4        i;
   Int4        index1;
   Int4        index2;
   SABlockPtr  merged;
   SABlockPtr  merged_head;
   Boolean     minus1;
   Boolean     minus2;
   SegmentPtr  minus_head1;
   SegmentPtr  minus_head2;
   SegmentPtr  minus_prev;
   SegmentPtr  minus_tmp;
   Int4        orient;
   SABlockPtr  new_sabp1;
   SABlockPtr  new_sabp2;
   Int4        num_bioseqs1;
   Int4        num_bioseqs2;
   SegmentPtr  prev;
   SABlockPtr  prev_sabp;
   SegmentPtr  segp1;
   SegmentPtr  segp2;
   Int4        shift;
   Int4        thisalign;
   SegmentPtr  tmp_curr;
   SegmentPtr  tmp_head;
   SegmentPtr  tmp1;
   SegmentPtr  tmp2;
   SegmentPtr  tmp3;

   if (!sabp1 || !sabp2)
   {
      if (!sabp1)
      {
         if (!sabp2)
         {
            return NULL;
         } else
         {
            return sabp2;
         }
      } else if (!sabp2)
      {
         return sabp1;
      }
   }
   thisalign = sabp1->AlignID;
   segp1 = sabp1->segp_head;
     /* Find the bioseqs in common -- this just finds the first bioseq  */
     /* in the first alignment that matches a bioseq in the second      */
     /* alignment -- change later to specify which common bioseq to use?*/
   index1 = 0;
   index2 = 0;
   done = FALSE;
   while (segp1 && !done)
   {
      segp2 = sabp1->segp_head;
      while (segp2 && !done)
      {
         if (SeqIdComp(segp1->sip, segp2->sip))
         {
            index1 = segp1->BspID;
            index2 = segp2->BspID;
            done = TRUE;
         }
         segp2 = segp2->next;
      }
      segp1 = segp1->next;
   }
   if (index1 == 0 || index2 == 0)
   {
      return NULL;
   }
   GetBlockInfo(sabp1, NULL, &num_bioseqs1, NULL, &num_bioseqs2);
   GetBlockInfo(sabp2, NULL, &num_bioseqs2, NULL, NULL);
   if (num_bioseqs1 == 0 || num_bioseqs2 == 0)
   {
      return NULL;
   }
   segp1 = sabp1->segp_head;
   segp2 = sabp2->segp_head;
   i = 0;
   minus_head1 = minus_head2 = FALSE;
   while (segp1)
   {
      i++;
      minus_tmp = (SegmentPtr)MemNew(sizeof(Segment));
      if (minus_head1)
      {
         minus_prev->next = minus_tmp;
         minus_prev = minus_tmp;
      } else
      {
         minus_head1 = minus_prev = minus_tmp;
      }
      if (segp1->strand == Seq_strand_minus)
      {
         if (i == index1)
            return sabp1;
         CalcMinusBounds(segp1, &minus_tmp);
         FlipSequence(segp1, (minus_tmp->to + minus_tmp->from));
         minus_tmp->strand = Seq_strand_minus;
         minus1 = TRUE;
      }
      segp1 = segp1->next;
   }
   i = 0;
   while (segp2)
   {
      i++;
      minus_tmp = (SegmentPtr)MemNew(sizeof(Segment));
      if (minus_head2)
      {
         minus_prev->next = minus_tmp;
         minus_prev = minus_tmp;
      } else
      {
         minus_head2 = minus_prev = minus_tmp;
      }
      if (segp2->strand == Seq_strand_minus)
      {
         if (i == index2)
            return sabp1;
         CalcMinusBounds(segp2, &minus_tmp);
         FlipSequence(segp2, (minus_tmp->from + minus_tmp->to));
         minus_tmp->strand = Seq_strand_minus;
         minus2 = TRUE;
      }
      segp2 = segp2->next;
   }
   segp1 = sabp1->segp_head;
   segp2 = sabp2->segp_head;
   merged = merged_head = NULL;
   done = FALSE;
   while (!done)
   {
      orient = GetBlockOrientation(&shift, segp1, segp2, index1, index2);
      if (orient == -2)
      {
         new_sabp1 = (SABlockPtr)MemNew(sizeof(SABlock));
         new_sabp1->AlignID = thisalign;
         if (merged_head)
         {
            merged->next = new_sabp1;
            new_sabp1->prev = merged;
            merged = new_sabp1;
         } else
         {
            merged = merged_head = new_sabp1;
            merged->prev = NULL;
         }
         tmp1 = sabp1->segp_head;
         tmp_head = NULL;
         for (i = 1; i<=num_bioseqs1; i++)
         {
            tmp2 = (SegmentPtr)MemNew(sizeof(Segment));
            if (tmp_head)
            {
               tmp_curr->next = tmp2;
               tmp2->prev = tmp_curr;
               tmp_curr = tmp2;
            } else
            {
               tmp_head = tmp_curr = tmp2;
            }
            tmp2->from = tmp1->from;
            tmp2->to = tmp2->from + shift;
            tmp2->sip = tmp1->sip;
            tmp1 = tmp1->next;
         }
         tmp2 = sabp2->segp_head;
         for (i = 1; i<=num_bioseqs2; i++)
         {
            if (i!=index2)
            {
               tmp1 = (SegmentPtr)MemNew(sizeof(Segment));
               tmp_curr->next = tmp1;
               tmp1->prev = tmp_curr;
               tmp_curr = tmp1;
               tmp1->from = tmp1->to = tmp2->from -1;
               tmp1->sip = tmp2->sip;
            }
            tmp2 = tmp2->next;
         }
         new_sabp1->segp_head = tmp_head;
         new_sabp1->next = sabp2;
         new_sabp2 = sabp2->next;
         sabp2->next = NULL;
         sabp2->prev = merged;
         merged = sabp2;
         sabp2->AlignID = thisalign;
         tmp1 = sabp1->segp_head;
         for (i = 1; i<=num_bioseqs1; i++)
         {
            tmp2 = (SegmentPtr)MemNew(sizeof(Segment));
            if (i>1)
            {
               tmp2->prev = tmp_curr;
               tmp_curr->next = tmp2;
            } else
            {
               tmp_head = tmp_curr = tmp2;
            }
            tmp_curr = tmp2;
            tmp2->from = tmp2->to = -1;
            tmp2->gap = 1;
            tmp2->sip = tmp1->sip;
            tmp1->from += shift+1;
            tmp1 = tmp1->next;
         }
         tmp1 = segp2;
         for (i=1; i<=num_bioseqs2; i++)
         {
            if (i!=index2)
            {
               tmp_curr->next = tmp1;
               tmp1->prev = tmp_curr;
               tmp_curr = tmp1;
               tmp1 = tmp1->next;
            } else
            {
               tmp3 = tmp1->next;
               tmp1->next = tmp1->prev = NULL;
               MemFree(tmp1);
               tmp1 = tmp3;
            }
         }
         merged->segp_head = tmp_head;
         if (sabp1)
            segp1 = sabp1->segp_head;
         sabp2 = sabp2->next;
         if (sabp2)
            segp2 = sabp2->segp_head;
      } else if (orient == -1)
      {
         new_sabp2 = (SABlockPtr)MemNew(sizeof(SABlock));
         new_sabp2->AlignID = thisalign;
         if (merged_head)
         {
            merged->next = new_sabp2;
            new_sabp2->prev = merged;
            merged = new_sabp2;
         } else
         {
            merged = merged_head = new_sabp2;
            merged->prev = NULL;
         }
         tmp1 = segp1;
         for (i=1; i<=num_bioseqs1; i++)
         {
            tmp2 = (SegmentPtr)MemNew(sizeof(Segment));
            if (i == 1)
            {
               tmp_head = tmp_curr = tmp2;
            } else
            {
               tmp_curr->next = tmp2;
               tmp2->prev = tmp_curr;
               tmp_curr = tmp2;
            }
            tmp2->from = tmp2->to = -1;
            tmp2->gap = 1;
            tmp2->sip = tmp1->sip;
         }
         tmp1 = segp2;
         for (i=1; i<=num_bioseqs2; i++)
         {
            if (i!=index2)
            {
               tmp2 = (SegmentPtr)MemNew(sizeof(Segment));
               tmp_curr->next = tmp2;
               tmp2->prev = tmp_curr;
               tmp_curr=tmp2;
               tmp2->from = tmp1->from;
               tmp2->to = tmp2->from + shift;
               tmp1->from += (shift+1);
            }
            tmp1 = tmp1->next;
         }
         merged->segp_head = tmp_head;
         merged->next = sabp1;
         merged = sabp1;
         tmp_curr = merged->segp_head;
         for (i=1; i<num_bioseqs1; i++)
         {
            tmp_curr = tmp_curr->next;
         }
         tmp2 = segp2;
         for (i=1; i<=num_bioseqs2; i++)
         {
            if (i!=index2)
            {
               tmp1 = (SegmentPtr)MemNew(sizeof(Segment));
               tmp_curr->next = tmp1;
               tmp1->prev = tmp_curr;
               tmp_curr = tmp1;
               tmp1->from = tmp1->to = tmp2->from-1;
               tmp1->sip = tmp2->sip;
            }
            tmp2 = tmp2->next;
         }
         sabp1 = sabp1->next;
         if (sabp1)
            segp1 = sabp1->segp_head;
      } else if (orient == 1)
      {
         if (shift > 0)
         {
            new_sabp1 = (SABlockPtr)MemNew(sizeof(SABlock));
            new_sabp1->AlignID = thisalign;
         }
         if (merged_head)
         {
            prev_sabp = merged;
            merged->next = sabp1;
            merged = merged->next;
            merged->prev = prev_sabp;
         } else
         {
            merged = merged_head = sabp1;
            merged->prev = NULL;
         }
         new_sabp2 = sabp1->next;
         tmp1 = merged->segp_head;
         for (i = 1; i<num_bioseqs1; i++)
         {
            tmp1 = tmp1->next;
         }
         for (i = 1; i<=num_bioseqs2; i++)
         {
            if (i!= index2)
            {
               prev = tmp1;
               tmp1->next = (SegmentPtr)MemNew(sizeof(Segment));
               tmp1 = tmp1->next;
               tmp1->from = tmp1->to = -1;
               tmp1->gap = 1;
               tmp1->sip = segp2->sip;
               tmp1->prev = prev;
            }
            segp2 = segp2->next;
         }
         if (shift > 0)
         {
            merged->next = new_sabp1;
            new_sabp1->prev = merged;
            merged = new_sabp1;
            segp1 = merged->prev->segp_head;
            tmp_head = NULL;
            i = 0;
            while (segp1)
            {
               i++;
               tmp1 = (SegmentPtr)MemNew(sizeof(Segment));
               if (tmp_head)
               {
                  prev->next = tmp1;
                  tmp1->prev = prev;
                  prev = tmp1;
               } else
               {
                  tmp_head = prev = tmp1;
               }
               if (i == index1)
               {
                  tmp1->from = segp1->to+1;
                  tmp1->to = tmp1->from + shift - 1;
                  tmp1->gap = 0;
                  tmp1->sip = segp1->sip;
               } else
               {
                  tmp1->from = tmp1->to = segp1->to;
                  tmp1->gap = 1;
                  tmp1->sip = segp1->sip;
               }
               segp1 = segp1->next;
            }
            new_sabp1->segp_head = tmp_head;
         }
         sabp1 = new_sabp2;
         if (sabp1)
            segp1 = sabp1->segp_head;
         if (sabp2)
            segp2 = sabp2->segp_head;
      } else if (orient == 2)
      {
         new_sabp1 = (SABlockPtr)MemNew(sizeof(SABlock));
         new_sabp1->AlignID = thisalign;
         new_sabp1->from = sabp1->from;
         new_sabp1->to = sabp1->from + shift - 1;
         tmp_head = NULL;
         tmp2 = sabp1->segp_head;
         for (i = 1; i<=num_bioseqs1; i++)
         {
            tmp1 = (SegmentPtr)MemNew(sizeof(Segment));
            if (tmp_head)
            {
               prev->next = tmp1;
               tmp1->prev = prev;
               prev = tmp1;
            } else
            {
               tmp_head = prev = tmp1;
            }
            tmp1->from = tmp2->from;
            tmp1->to = tmp1->from + shift - 1;
            tmp1->sip = tmp2->sip;
            tmp2 = tmp2->next;
         }
         tmp2 = sabp2->segp_head;
         for (i = 1; i<=num_bioseqs2; i++)
         {
            if (i != index2)
            {
               tmp1 = (SegmentPtr)MemNew(sizeof(Segment));
               prev->next = tmp1;
               tmp1->prev = prev;
               prev = tmp1;
               tmp1->from = tmp2->from-1;
               tmp1->to = tmp1->from;
               tmp1->sip = tmp2->sip;
               tmp1->gap = 1;
            }
            tmp2 = tmp2->next;
         }
         new_sabp1->segp_head = tmp_head;
         if (merged_head)
         {
            prev_sabp = merged;
            merged->next = new_sabp1;
            merged = merged->next;
            merged->prev = prev_sabp;
         } else
         {
            merged = merged_head = new_sabp1;
            merged->prev = NULL;
         }
         tmp1 = sabp1->segp_head;
         for (i = 1; i<num_bioseqs1; i++)
         {
            tmp1->from = tmp1->from + shift;
            tmp1 = tmp1->next;
         }
         tmp1->from = tmp1->from + shift;
         sabp1->prev = NULL;
         merged->next = NULL;
         segp1 = tmp1;
         if (sabp2)
            segp2 = sabp2->segp_head;
      } else if (orient == 3)
      {
         if (shift == 0)
         {
            if (merged_head && (sabp1 != merged))
            {
               merged->next = sabp1;
               sabp1->prev = merged;
               merged = sabp1;
            } else
            {
               merged = merged_head = sabp1;
               merged->prev = NULL;
            }
            tmp1 = sabp1->segp_head;
            for (i = 1; i< num_bioseqs1; i++)
            {
               tmp1 = tmp1->next;
            }
            tmp2 = sabp2->segp_head;
            for (i = 1; i<=num_bioseqs2; i++)
            {
               if (i!= index2)
               {
                  tmp1->next = tmp2;
                  tmp2->prev = tmp1;
                  tmp1 = tmp1->next;
                  tmp2 = tmp2->next;
               } else
               {
                  tmp3 = tmp2->next;
                  tmp2->next = tmp2->prev = NULL;
                  MemFree(tmp2);
                  tmp2 = tmp3;
               }
            }
            sabp1 = sabp1->next;
            if (sabp1)
               segp1 = sabp1->segp_head;
            sabp2->segp_head = NULL;
            new_sabp2 = sabp2->next;
            sabp2->prev = sabp2->next = NULL;
            MemFree(sabp2);
            sabp2 = new_sabp2;
            if (sabp2)
               segp2 = sabp2->segp_head;
         } else if (shift > 0)
         {
            new_sabp1 = (SABlockPtr)MemNew(sizeof(SABlock));
            new_sabp1->AlignID = thisalign;
            if (merged_head)
            {
               merged->next = new_sabp1;
               new_sabp1->prev = merged;
               merged = new_sabp1;
            } else
            {
               merged = merged_head = new_sabp1;
               merged->prev = NULL;
            }
            tmp1 = sabp1->segp_head;
            tmp_head = NULL;
            for (i = 1; i<=num_bioseqs1; i++)
            {
               tmp2 = (SegmentPtr)MemNew(sizeof(Segment));
               if (tmp_head)
               {
                  tmp_curr->next = tmp2;
                  tmp2->prev = tmp_curr;
                  tmp_curr = tmp2;
               } else
               {
                  tmp_head = tmp_curr = tmp2;
               }
               tmp2->sip = tmp1->sip;
               tmp2->from = tmp1->from;
               tmp2->gap = tmp1->gap;
               if (tmp1->gap == 0)
               {
                  tmp2->to = tmp1->to - shift;
                  tmp1->from = tmp1->to - shift + 1;
               } else
               {
                  tmp2->to = tmp2->from;
               }
               tmp1 = tmp1->next;
            }
            tmp2 = sabp2->segp_head;
            for (i = 1; i<=num_bioseqs2; i++)
            {
               if (i!=index2)
               {
                  tmp_curr->next = tmp2;
                  tmp2->prev = tmp_curr;
                  tmp2->next = NULL;
                  tmp_curr = tmp2;
                  tmp2 = tmp2->next;
               } else
               {
                  tmp3 = tmp2->next;
                  tmp2->next = tmp2->prev = NULL;
                  MemFree(tmp2);
                  tmp2 = tmp3;
               }
            }
            tmp_curr->next = NULL;
            new_sabp1->segp_head = tmp_head;
            new_sabp2 = sabp2->next;
            sabp2->next = sabp2->prev = NULL;
            MemFree(sabp2);
            sabp2 = new_sabp2;
            if (sabp2)
               segp2 = sabp2->segp_head;
            if (sabp1)
               segp1 = sabp1->segp_head;
         } else if (shift < 0)
         {
            if (merged_head && (sabp1!=merged))
            {
               merged->next = sabp1;
               sabp1->prev = merged;
               merged = sabp1;
            } else
            {
               merged = merged_head = sabp1;
               merged->prev = NULL;
            }
            tmp1 = sabp1->segp_head;
            for (i = 1; i<=num_bioseqs1; i++)
            {
               tmp_head= tmp1;
               tmp1 = tmp1->next;
            }
            tmp2 = sabp2->segp_head;
            for (i = 1; i<=num_bioseqs2; i++)
            {
               if (i != index2)
               {
                  tmp_curr = (SegmentPtr)MemNew(sizeof(Segment));
                  tmp_curr->prev = tmp_head;
                  tmp_head->next = tmp_curr;
                  tmp_head = tmp_curr;
                  tmp_curr->from = tmp2->from;
                  if (tmp2->gap == 0)
                  {
                     tmp_curr->to = tmp2->to + shift;
                  } else
                  {
                     tmp_curr->to = tmp_curr->from;
                     tmp_curr->gap = tmp2->gap;
                  }
                  tmp_curr->sip = tmp2->sip;
               }
               if (tmp2->gap == 0)
                  tmp2->from = tmp2->to + shift + 1;
               tmp2 = tmp2->next;
            }
            if (sabp2)
               segp2 = sabp2->segp_head;
            sabp1 = sabp1->next;
            if (sabp1)
               segp1 = sabp1->segp_head;
         }
      } else if (orient == 4)
      {
         if(merged_head)
         {
            new_sabp1 = (SABlockPtr)MemNew(sizeof(SABlock));
            new_sabp1->AlignID = thisalign;
            merged->next = new_sabp1;
            new_sabp1->prev = merged;
         } else
         {
            new_sabp1 = (SABlockPtr)MemNew(sizeof(SABlock));
            new_sabp1->AlignID = thisalign;
            merged_head = merged = new_sabp1;
            merged->prev = NULL;
         }
         tmp1 = sabp1->segp_head;
         tmp_head = NULL;
         for (i = 1; i<=num_bioseqs1; i++)
         {
            tmp2 = (SegmentPtr)MemNew(sizeof(Segment));
            if (tmp_head)
            {
               tmp_curr->next = tmp2;
               tmp2->prev = tmp_curr;
               tmp_curr = tmp2;
            } else
            {
               tmp_head = tmp_curr = tmp2;
            }
            tmp2->from = tmp1->from;
            if (tmp1->gap == 0)
            {
               tmp2->to = tmp2->from + shift - 1;
               tmp1->from = tmp1->from + shift;
            }
            tmp2->sip = tmp1->sip;
            tmp1 = tmp1->next;
         }
         tmp2 = sabp2->segp_head;
         for (i = 1; i<=num_bioseqs2; i++)
         {
            if (i!=index2)
            {
               tmp1 = (SegmentPtr)MemNew(sizeof(Segment));
               tmp_curr->next = tmp1;
               tmp1->prev = tmp_curr;
               tmp1->from = tmp1->to = -1;
               tmp1->gap = 1;
               tmp1->sip = tmp2->sip;
            }
            tmp2 = tmp2->next;
         }
         tmp1->next = NULL;
         if (sabp1)
            segp1 = sabp1->segp_head;
         if (sabp2)
            segp2 = sabp2->segp_head;
      } else if (orient == 5 || orient == 6)
      {
         new_sabp1 = (SABlockPtr)MemNew(sizeof(SABlock));
         new_sabp1->AlignID = thisalign;
         if (merged_head)
         {
            merged->next = new_sabp1;
            new_sabp1->prev = merged;
            merged = new_sabp1;
         } else
         {
            merged = merged_head = new_sabp1;
            merged->prev = NULL;
         }
         tmp2 = sabp1->segp_head;
         tmp_head = NULL;
         for (i = 1; i<=num_bioseqs1; i++)
         {
            tmp1 = (SegmentPtr)MemNew(sizeof(Segment));
            if (tmp_head)
            {
               tmp_curr->next = tmp1;
               tmp1->prev = tmp_curr;
               tmp_curr = tmp1;
            } else
            {
               tmp_curr = tmp_head = tmp1;
            }
            tmp1->from = tmp2->from - 1;
            tmp1->to = tmp2->from - 1;
            tmp1->gap = 1;
            tmp1->sip = tmp2->sip;
            tmp2 = tmp2->next;
         }
         tmp2 = segp2;
         for (i = 1; i<=num_bioseqs2; i++)
         {
            if (i!=index2)
            {
               tmp1 = (SegmentPtr)MemNew(sizeof(Segment));
               tmp1->prev = tmp_curr;
               tmp_curr->next = tmp1;
               tmp_curr = tmp1;
               tmp1->from = tmp2->from;
               tmp1->to = tmp2->from + shift - 1;
            }
            tmp2->from += shift;
            tmp2 = tmp2->next;
         }
         if (sabp1)
            segp1 = sabp1->segp_head;
         if (sabp2)
            segp2 = sabp2->segp_head;
      } else if (orient == 7)
      {
         new_sabp1 = (SABlockPtr)MemNew(sizeof(SABlock));
         new_sabp1->AlignID = thisalign;
         if (merged_head)
         {
            merged->next = new_sabp1;
            new_sabp1->prev = merged;
            merged = new_sabp1;
         } else
         {
            merged = merged_head = new_sabp1;
            merged->prev = NULL;
         }
         tmp2 = sabp1->segp_head;
         tmp_head = NULL;
         for (i = 1; i<=num_bioseqs1; i++)
         {
            tmp1 = (SegmentPtr)MemNew(sizeof(Segment));
            if (tmp_head)
            {
               tmp_curr->next = tmp1;
               tmp1->prev = tmp_curr;
               tmp_curr = tmp1;
            } else
            {
               tmp_curr = tmp_head = tmp1;
            }
            if (i == index1)
            {
               prev = sabp2->segp_head;
               for (c = 1; c<=num_bioseqs2; c++)
               {
                  if (c==index2)
                  {
                     tmp1->from = prev->from;
                     tmp1->to = prev->to;
                     tmp1->gap = 0;
                  }
                  prev = prev->next;
               }
            } else
            {
               tmp1->from = -1;
               tmp1->to =  -1;
               tmp1->gap = 1;
            }
            tmp1->sip = tmp2->sip;
            tmp2 = tmp2->next;
         }
         tmp2 = segp2;
         for (i = 1; i<=num_bioseqs2; i++)
         {
            if (i!=index2)
            {
               tmp_curr->next = tmp2;
               tmp2->prev = tmp_curr;
               tmp_curr = tmp2;
               tmp2 = tmp2->next;
            } else
            {
               tmp3 = tmp2;
               tmp2 = tmp2->next;
               tmp3->next = tmp3->prev = NULL;
               MemFree(tmp3);
            }
         }
         new_sabp1->segp_head = tmp_head;
         sabp2->segp_head = NULL;
         new_sabp2 = sabp2->next;
         sabp2->next = sabp2->prev = NULL;
         MemFree(sabp2);
         sabp2 = new_sabp2;
         if (shift > 0)
         {
            new_sabp1 = (SABlockPtr)MemNew(sizeof(SABlock));
            tmp_head = NULL;
            tmp1 = merged->segp_head;
            i = 0;
            while (tmp1)
            {
               i++;
               tmp_curr = (SegmentPtr)MemNew(sizeof(Segment));
               if (tmp_head)
               {
                  prev->next = tmp_curr;
                  tmp_curr->prev = prev;
                  prev = tmp_curr;
               } else
               {
                  tmp_head = prev = tmp_curr;
               }
               if (i == index1)
               {
                  tmp_curr->from = tmp1->to + 1;
                  tmp_curr->to = tmp_curr->from + shift - 1;
                  tmp_curr->gap = 0;
               } else
               {
                  tmp_curr->from = tmp_curr->to = tmp1->to;
                  tmp_curr->gap = 1;
               }
               tmp_curr->sip = tmp1->sip;
               tmp1 = tmp1->next;
            }
            new_sabp1->segp_head = tmp_head;
            new_sabp1->prev = merged;
            merged->next = new_sabp1;
            merged = new_sabp1;
         }
         if (sabp1)
            segp1 = sabp1->segp_head;
         if (sabp2)
            segp2 = sabp2->segp_head;
      }
      if (sabp1 == NULL)
      {
         new_sabp2 = NULL;
         if (sabp2)
            new_sabp2 = sabp2;
         while (sabp2)
         {
            segp1 = merged->segp_head;
            for (i = 1; i<=num_bioseqs1; i++)
            {
               tmp1 = (SegmentPtr)MemNew(sizeof(Segment));
               if (i == 1)
               {
                  tmp2 = sabp2->segp_head;
                  sabp2->segp_head = tmp1;
                  tmp_curr = tmp1;
               } else
               {
                  tmp_curr->next = tmp1;
                  tmp1->prev = tmp_curr;
                  tmp_curr = tmp1;
               }
               tmp1->from = tmp1->to = segp1->from + 1;
               tmp1->gap = 1;
               tmp1->sip = segp1->sip;
               segp1 = segp1->next;
            }
            for (i = 1; i<=num_bioseqs2; i++)
            {
               if (i!=index2)
               {
                  tmp_curr->next = tmp2;
                  tmp2->prev = tmp_curr;
                  tmp_curr = tmp2;
               }
               tmp2 = tmp2->next;
            }
            sabp2 = sabp2->next;
         }
         if (new_sabp2)
         {
            merged->next = new_sabp2;
            new_sabp2->prev = merged;
            merged = new_sabp2;
         }
         done = TRUE;
      } else if (sabp2 == NULL)
      {
         new_sabp1 = sabp1;
         while (sabp1)
         {
            tmp2 = merged->prev->segp_head;
            tmp1 = sabp1->segp_head;
            for (i = 1; i<num_bioseqs1; i++)
            {
               tmp1 = tmp1->next;
               tmp2 = tmp2->next;
            }
            tmp2 = tmp2->next;
            for (i = 1; i<=num_bioseqs2; i++)
            {
               if (i != index2)
               {
                  tmp_curr = (SegmentPtr)MemNew(sizeof(Segment));
                  tmp1->next = tmp_curr;
                  tmp_curr->prev = tmp1;
                  tmp1 = tmp_curr;
                  tmp1->from = tmp1->to = tmp2->to;
                  tmp1->gap = 1;
                  tmp1->sip = tmp2->sip;
                  tmp2 = tmp2->next;
               }
            }
            sabp1 = sabp1->next;
         }
         merged->next = new_sabp1;
         new_sabp1->prev = merged;
         while (merged->next)
            merged = merged->next;
         done = TRUE;
      }
   }
   if (!done)
      merged->next = NULL;
   merged_head = IndexBlocks(merged_head); /* fix IndexBlocks to number gaps */
   merged_head = SquishBlocks(merged_head);
   i = 0;
   if (minus1)
   {
      segp1 = merged_head->segp_head;
      minus_tmp = minus_head1;
      while (minus_tmp)
      {
         i++;
         if (minus_tmp->strand == Seq_strand_minus && i!= index1)
         {
            FlipSequence(segp1, (minus_tmp->to + minus_tmp->from));
         }
         segp1 = segp1->next;
         minus_tmp = minus_tmp->next;
      }
   }
   i = 0;
   if (minus2)
   {
      minus_tmp = minus_head2;
      while (minus_tmp)
      {
         i++;
         if (i == index2)
            minus_tmp = minus_tmp->next;
         if (minus_tmp->strand == Seq_strand_minus)
         {
            FlipSequence(segp1, (minus_tmp->to + minus_tmp->from));
         }
         segp1 = segp1->next;
         minus_tmp = minus_tmp->next;
      }
   }
   merged = merged_head;
   while (merged->next)
      merged = merged->next;
   return merged_head;
}


NLM_EXTERN Int4 GetBlockOrientation(Int4Ptr shift, SegmentPtr segp1, SegmentPtr segp2, Int4 index1, Int4 index2)
{
   Int4  i;
   Int4  orient;

   for (i = 1; i < index1; i++)
   {
      segp1 = segp1->next;
   }
   for (i = 1; i < index2; i++)
   {
      segp2 = segp2->next;
   }
   if (segp1->gap != 0)
   {
      if (segp2->gap == 0)
      {
         if (segp1->from <= segp2->from)
         {
            *shift = 0;
            orient = 1;
         }
         else if (segp2->from < segp1->from && segp2->to > segp1->from)
            /*   .....       */
            /*  _________    */
         {
            *shift = segp1->from - segp2->from;
            orient = -1;
         } else if (segp2->to <= segp1->from)
            /*        .... */
            /*  ______     */
         {
            *shift = 0;
            orient = 7;
         }
      } else
      {
         if (segp1->from == segp2->from)
         {
            *shift = 0;
            orient = 3;
         } else if (segp1->from > segp2->from)
         {
            *shift = 0;
            orient = 7;
         } else
         {
            *shift = 0;
            orient = 1;
         }
      }
   } else if (segp2->gap != 0)
   {
      if (segp1->from < segp2->from && segp1->to > segp2->from)
         /* ---------- */
         /*   ......   */
      {
         *shift = segp2->from - segp1->from;
         orient = -2;
      } else if (segp1->from >= segp2->from)
         /*      --------*/
         /* ....         */
      {
         *shift = 0;
         orient = 7;
      } else if (segp1->to < segp2->from)
         /*  -------         */
         /*            ..... */
      {
         *shift = 0;
         orient = 1;
      }
   } else if (segp1->from < segp2->from && segp1->to < segp2->from)
/* ---------               */
/*               _________*/
   {
      if (segp1->bsp_next)
      {
         if (segp1->bsp_next->from > segp1->to+1)
         {
            *shift = MIN((segp2->from - segp1->to - 1), (segp1->bsp_next->from-segp1->to));
         } else
         {
            *shift = 0;
         }
      } else
      {
         *shift = segp2->from - segp1->to;
      }
      orient = 1;
   } else if (segp1->from < segp2->from && segp1->to < segp2->to)
/* ------------       */
/*        ____________*/
   {
      *shift = segp2->from - segp1->from;
      orient = 2;
   } else if (segp1->from == segp2->from)
/*    ----------      or         -------        */ 
/*    ______  */            /*  ___________    */
   {
      *shift = segp1->to - segp2->to;
      orient = 3;
   } else if (segp1->from < segp2->from)
/* ---------------     */
/*    ______      */
   {
      *shift = segp2->from - segp1->from;
      orient = 4;
   } else if (segp1->from > segp2->from && segp1->to < segp2->to)
/*    -------     */
/* ______________    */
   {
      *shift = segp1->from - segp2->from;
      orient = 5;
   } else if (segp1->from > segp2->from && segp1->from <= segp2->to)
/*         ----------*/
/* __________       */
   {
      *shift = segp1->from - segp2->from;
      orient = 6;
   } else if (segp1->from > segp2->to)
/*          ---------*/
/*______            */
   {
      *shift = segp1->from - segp2->to - 1;
      orient = 7;
   }
   return orient;
}

NLM_EXTERN SABlockPtr SARemoveSequence(Int4 BspID, SeqIdPtr sip, SABlockPtr sabp)
{
   SABlockPtr  sabp_head;
   SegmentPtr  segp;

   sabp_head = sabp;
   if (BspID)
   {
      while (sabp)
      {
         segp = sabp->segp_head;
         while (segp)
         {
            if (segp->BspID == BspID)
            {
               if (segp->prev == NULL)
               {
                  sabp->segp_head = segp->next;
                  segp->next = NULL;
                  MemFree(segp);
               } else
               {
                  segp->prev->next = segp->next;
                  if (segp->next)
                     segp->next->prev = segp->prev;
                  segp->prev = segp->next = NULL;
                  MemFree(segp);
               }
            }
            segp = segp->next;
         }
         sabp = sabp->next;
      }
      sabp_head = SquishBlocks(sabp_head);
   } else if (sip)
   { 
      while (sabp)
      {
         segp = sabp->segp_head;
         while (segp)
         {
            if (SeqIdComp(sip, segp->sip))
            {
               if (segp->prev == NULL)
               {
                  sabp->segp_head = segp->next;
                  segp->next = NULL;
                  MemFree(segp);
               } else
               {
                  segp->prev->next = segp->next;
                  if (segp->next)
                     segp->next->prev = segp->prev;
                  segp->prev = segp->next = NULL;
                  MemFree(segp);
               }
            }
            segp = segp->next;
         }
         sabp = sabp->next;
      }
      sabp_head = SquishBlocks(sabp_head);
   }
   return sabp_head;
}


/*******************************************************************
*  SquishBlocks looks through an SABlock structure and merges
*  together adjacent series of Segments if the Segments, looking
*  pairwise, have the same pattern, e.g.
*   ...      .....   (gap)              ........
*   ___      _____   (sequence)         ________
*   ___      _____   (sequence)   ->    ________
*   ...      .....   (gap)              ........
*
*  The function also removes an SABlock if all its segments consist
*  only of gaps.
*  Note that the SABlock structure must be regular; that is, every
*  SABlock must have the same number of segments, in the same order,
*  or things will get strangely truncated.
********************************************************************/

NLM_EXTERN SABlockPtr SquishBlocks(SABlockPtr sabp)
{
   Boolean     all_gap1;
   Boolean     all_gap2;
   Boolean     merge;
   SABlockPtr  sabp_head;
   SABlockPtr  sabp_next;
   SABlockPtr  sabp_tmp;
   SegmentPtr  segp1;
   SegmentPtr  segp2;

   sabp_head = sabp;
   while (sabp)
   {
      sabp_next = sabp->next;
      if (!sabp_next)
         sabp = sabp->next;
      else
      {
         segp1 = sabp->segp_head;
         segp2 = sabp_next->segp_head;
         merge = all_gap1 = all_gap2 = TRUE;
         while (segp1 && segp2)
         {
            if (segp1->gap == 0 && segp2->gap != 0)
            {
               all_gap1 = FALSE;
               merge = FALSE;
            } else if (segp1->gap != 0 && segp2->gap == 0)
            {
               all_gap2 = FALSE;
               merge = FALSE;
            } else if (segp1->gap == 0 && segp2->gap == 0)
            {
               all_gap1 = FALSE;
               all_gap2 = FALSE;
            }
            segp1 = segp1->next;
            segp2 = segp2->next;
         }
         if (merge)
         {
            if (all_gap1)
            {
               if (sabp->prev)
               {
                  sabp->prev->next = sabp_next->next;
                  if (sabp_next->next)
                     sabp_next->next->prev = sabp->prev;
                  sabp_tmp = sabp_next->next;
                  sabp->prev = sabp->next = NULL;
                  sabp_next->prev = sabp_next->next = NULL;
                  CleanupBlocks(sabp);
                  CleanupBlocks(sabp_next);
                  sabp = sabp_tmp;
               } else
               {
                  sabp_head = sabp_next->next;
                  sabp->next = NULL;
                  if (sabp_next->next)
                     sabp_next->next->prev = NULL;
                  sabp_next->prev = sabp_next->next = NULL;
                  CleanupBlocks(sabp);
                  CleanupBlocks(sabp_next);
                  sabp = sabp_head;
               }
            } else
            {
               segp1 = sabp->segp_head;
               segp2 = sabp_next->segp_head;
               while (segp1 && segp2)
               {
                  segp1->to = segp2->to;
                  segp1 = segp1->next;
                  segp2 = segp2->next;
               }
               sabp_tmp = sabp_next->next;
               sabp->next = sabp_next->next;
               if (sabp_next->next)
                  sabp_next->next->prev = sabp;
               sabp_next->prev = sabp_next->next = NULL;
               CleanupBlocks(sabp_next);
               if (sabp_tmp)
                  sabp_tmp->prev = sabp;
            }
         } else if (all_gap1)
         {
            if (sabp->prev)
            {
               sabp->prev->next = sabp->next;
               sabp_tmp = sabp->next;
               sabp->prev = sabp->next = NULL;
               CleanupBlocks(sabp);
               sabp = sabp_tmp;
            } else
            {
               sabp_head = sabp->next;
               sabp->prev = sabp->next = NULL;
               CleanupBlocks(sabp);
               sabp = sabp_head;
            }
         } else if (all_gap2)
         {
            sabp->next = sabp_next->next;
            if (sabp_next->next)
               sabp_next->next->prev = sabp;
            sabp_next->prev = sabp_next->next = NULL;
            CleanupBlocks(sabp_next);
         } else
         {
            sabp = sabp->next;
         }
      }
      all_gap1 = TRUE;  
      if (sabp)
      {
         segp1 = sabp->segp_head;
         while (segp1 && all_gap1)
         {
            if (segp1->gap == 0)
            all_gap1 = FALSE;
            segp1 = segp1->next;
         }
         if (all_gap1)
         {
            if (sabp->prev)
               sabp->prev->next = sabp->next;
            if (sabp->next)
               sabp->next->prev = sabp->prev;
            sabp->prev = sabp->next = NULL;
            CleanupBlocks(sabp);
         }
      }
   }
   sabp_head = IndexBlocks(sabp_head);
   return sabp_head;
}


/***************************************************************
*  RearrangeSegments puts any BspID into any position.  If the 
*  position given is greater than the number of bioseqs, or if the
*  BspID is already in that position, or if the BspID is not 
*  found in the structure, the function gives back the SABlock
*  structure unchanged.
****************************************************************/

NLM_EXTERN SABlockPtr RearrangeSegments (Int4 BspID, SABlockPtr sabp, Int4 position)
{
   Int4        counter;
   Boolean     found;
   SABlockPtr  sabp_head;
   SegmentPtr  segp;
   SegmentPtr  segp_target;

   sabp_head = sabp;
   while (sabp)
   {
      counter = 0;
      segp = sabp->segp_head;
      found = FALSE;
      while (segp && !found)
      {
         counter += 1;
         if (BspID == segp->BspID)
         {
            if (counter == position)
               return sabp_head;
            segp_target = segp;
            if (segp_target->next)
            {
               if (segp->prev)
               {
                  segp_target->next->prev = segp->prev;
                  segp->prev->next = segp_target->next;
               } else
               {
                  sabp->segp_head = segp_target->next;
                  segp_target->next->prev = NULL;
               }
            } else
            {
               if (segp->prev)
               {
                  segp->prev->next = NULL;
               } else
               {
                  return sabp_head;
               }
            }
            segp_target->next = segp_target->prev = NULL;
            found = TRUE;
         }
         segp = segp->next;
      }
      if (!found)
         return sabp_head;
      segp = sabp->segp_head;
      found = FALSE;
      counter = 0;
      while (segp && !found)
      {
         counter += 1;
         if (counter == position)
         {
            segp_target->next = segp;
            if (segp->prev)
            {
               segp_target->prev = segp->prev;
               segp->prev->next = segp_target;
               segp->prev = segp_target;
            } else
            {
               sabp->segp_head = segp_target;
               segp_target->prev = NULL;
               segp_target->next = segp;
               segp->prev = segp_target;
            }
            found = TRUE;
         }
         segp = segp->next;
      }
      if (!found)
         return sabp_head;
      sabp = sabp->next;
   }
   return sabp_head;
}

static void FlipSequence(SegmentPtr segp, Int4 len)
{
   Int4        temp;
   SegmentPtr  segp_tmp;

   segp_tmp = segp;
   while (segp_tmp)
   {
      if (segp_tmp->gap!=2)
      {
         temp = segp_tmp->to;
         segp_tmp->to = len - segp_tmp->from;
         segp_tmp->from = len - temp;
    /*  if (segp_tmp->strand == Seq_strand_minus)
         segp_tmp->strand = Seq_strand_plus;
      else
         segp_tmp->strand = Seq_strand_minus; */
      }
   segp_tmp = segp_tmp->bsp_next;
   }
   return;
}

static void CalcMinusBounds(SegmentPtr segp, SegmentPtr PNTR segp_storage)
{
   SegmentPtr  segp_tmp;

   segp_tmp = segp;
   (*segp_storage)->from = (*segp_storage)->to = -3;
   while (segp_tmp)
   {
      if (segp_tmp->gap == 0)
      {
         if ((*segp_storage)->from < -1)
         {
            (*segp_storage)->from = segp_tmp->from;
         }
         (*segp_storage)->to = segp_tmp->to;
      }
      segp_tmp = segp_tmp->bsp_next;
   }
   return;
}

/***************************************************************
*  SASplitBlock takes the input block and splits it:
*  agaaacttttccgctccacc
*  7 9                26
*
*  if from = 10, the function will return a pointer to a block
*  containing the alignment from 7 to 9, whose 'next' pointer
*  will point to a block containing the alignment from 10 to 26.
*  The function copies the RegionID of the parent block into 
*  both of the new blocks, and it does not try to renumber the
*  SegIDs or to horizontally link the new segments; therefore,
*  IndexBlocks must be called after using this function.  If
*  from is the same as the first residue of the block, or if
*  from is greater than the last residue in the block, nothing
*  happens.
***************************************************************/

NLM_EXTERN SABlockPtr SASplitBlock (SABlockPtr sabp, Int4 from)
{
   SABlockPtr  sabp_head;
   SABlockPtr  sabp_new;
   SegmentPtr  segp1;
   SegmentPtr  segp2;
   SegmentPtr  segp2_prev;
   Int4        shift;

   sabp_head = sabp;
   if (from > sabp->from && from <= sabp->to)
   {
      sabp_new = (SABlockPtr)MemNew(sizeof(SABlock));
      sabp_new->from = from;
      sabp_new->to = sabp->to;
      sabp->to = from-1;
      shift = sabp->to - sabp->from;
      sabp_new->next = sabp->next;
      if (sabp_new->next)
      {
        sabp_new->next->prev = sabp_new;
      }
      sabp->next = sabp_new;
      sabp_new->prev = sabp;
      sabp_new->AlignID = sabp->AlignID;
      sabp_new->RegionID = sabp->RegionID;
      sabp_new->visible = sabp->visible;
      /* lyg: preserve align state */
      sabp_new->aligned = sabp->aligned;
      segp1 = sabp->segp_head;
      sabp_new->segp_head = NULL;
      while (segp1)
      {
         segp2 = (SegmentPtr)MemNew(sizeof(Segment));
         if (sabp_new->segp_head)
         {
            segp2_prev->next = segp2;
            segp2->prev = segp2_prev;
            segp2_prev = segp2;
         } else
         {
            sabp_new->segp_head = segp2_prev = segp2;
         }
         if (segp1->gap != 0)
         {
            segp2->gap = segp1->gap;
            segp2->from = segp2->to = segp1->to;
            segp2->sip = segp1->sip;
            segp2->visible = segp1->visible;
         } else
         {
            segp2->from = segp1->from + shift + 1;
            segp2->to = segp1->to;
            segp1->to = segp1->from + shift;
            segp2->sip = segp1->sip;
            segp2->visible = segp1->visible;
         }
         segp2->link = segp1->link;
         segp2->strand = segp1->strand;
         segp1 = segp1->next;
      }
   }
   return sabp_head;
}


/****************************************************************
*  TeenyBlock makes 1-residue blocks beginning at the alignment
*  coordinate "from" and ending with the alignment coordinate
*  "to" (inclusive).  It calls SASplitBlock recursively to 
*  create the little blocks.  If from > to, the function makes the
*  entire alignment into 1-residue blocks starting at from.  If
*  from and to are both 0, the function splits the whole alignment.
*  If from and to are not included in the alignment coordinates,
*  the function does nothing.
****************************************************************/

NLM_EXTERN SABlockPtr TeenyBlock (SABlockPtr sabp, Int4 from, Int4 to)
{
   SABlockPtr  sabp_head;
   SABlockPtr  sabp_prev;

   sabp_head = sabp;
   if (!sabp)
      return NULL;
   if(from <= to) {
        while (from > sabp->to){
            sabp = sabp->next;
            if (!sabp) 
               return sabp_head;
        }
        if (to < sabp->to) 
           sabp = SASplitBlock(sabp, to+1);
   }
   while (to >= sabp->to || from > to || to == 0)
   {
      while (from <= sabp->to && sabp)
      {
         sabp_prev = sabp->prev;
         sabp = SASplitBlock(sabp, from);
         if (sabp_prev)
         {
            sabp_prev->next = sabp;
            sabp->prev = sabp_prev;
         } else
         {
            sabp_head = sabp;
         }
         sabp = sabp->next;
         from += 1;
      }
      from++;  /* lyg don't need to split existing split */
      sabp = sabp->next;
      if(sabp == NULL) break;
      if (to < sabp->to)
      {
         sabp = SASplitBlock(sabp, to+1);
      }
   }
   sabp_head = IndexBlocks(sabp_head);
   return sabp_head;
}


/**********************************************************************
* AddEmptyBlocks adds n blocks to the beginning and/or to the end of
* the given SABlock structure.  The blocks consist of the same number
* of segments, with seqids, and BspIDs all set as expected, as 
* the rest of the blocks, but the 'from' and 'to' numbers are all left
* as zero.  The segments are horizontally linked in correct order.  The
* entire structure is not reindexed, though, so any blocks added at
* the beginning may have SegIDs of zero or negative values.
**********************************************************************/

NLM_EXTERN SABlockPtr AddEmptyBlocks(SABlockPtr sabp, Int4 n, Boolean beginning, Boolean end)
{
   Int4        count;
   Int4        i;
   Int4        j;
   SABlockPtr  sabp_head;
   SABlockPtr  sabp_new;
   SABlockPtr  sabp_newhead;
   SABlockPtr  sabp_tmp;
   SegmentPtr  segp;
   SegmentPtr  segp_head;
   SegmentPtr  segp_orig;
   SegmentPtr  segp_prev;
   SegmentPtr  segp_storage;
   SegmentPtr  segp_tmp;

   if (!sabp)
      return NULL;
   sabp_head = sabp;
   GetBlockInfo(sabp, NULL, &count, NULL, NULL);
   if (!count)
      return NULL;
   if (beginning)
   {
      segp = segp_head = NULL;
      segp_storage = NULL;
      sabp_tmp = sabp_newhead = NULL;
      for (i=1; i<=n; i++)
      {
         sabp_new = (SABlockPtr)MemNew(sizeof(SABlock));
         if (sabp_newhead)
         {
            sabp_tmp->prev = sabp_new;
            sabp_new->next = sabp_tmp;
            sabp_tmp = sabp_new;
            sabp_new->SegID = sabp_new->next->SegID - 1;
         } else
         {
            sabp_tmp = sabp_newhead = sabp_new;
            sabp_new->SegID = sabp->SegID - 1;
         }
         sabp_new->AlignID = sabp->AlignID;
         /* begin lyg */
         sabp_new->RegionID = sabp->RegionID; 
         sabp_new->aligned = sabp->aligned;   
         sabp_new->from = sabp_new->to = sabp->from - 1; 
         /* end lyg */
         segp_tmp = segp_storage;
         segp_orig = sabp->segp_head;
         segp_head = NULL;
         for (j=1; j<=count; j++)
         {
            segp = (SegmentPtr)MemNew(sizeof(Segment));
            if (segp_head)
            {   
               segp_prev->next = segp;
               segp->prev = segp_prev;
               segp_prev = segp;
            } else
            {
               segp_prev = segp_head = segp;
            }
            if (i > 1)
            {
               segp->BspID = segp_tmp->BspID;
               /* begin lyg */
               segp->gap = 1;  /* make single gaps */
               segp->from = segp->to = segp_tmp->from;
               segp->strand = segp_tmp->strand;
               segp->move = 0;
               segp->link = 0;
               /* end lyg */
               segp->sip = segp_tmp->sip;
               segp->bsp_next = segp_tmp;
               segp_tmp->bsp_prev = segp;
               segp_tmp = segp_tmp->next;
            } else
            {
               if (segp_storage)
               {
                  segp_tmp->next = segp;
                  segp_tmp = segp_tmp->next;
               } else
               {
                  segp_storage = segp_tmp = segp;
               }
               segp->BspID = segp_orig->BspID;
               /* begin lyg */
               segp->gap = 1;  /* make single gaps */
               segp->from = segp->to = segp_orig->from;
               segp->strand = segp_orig->strand;
               segp->move = 0;
               segp->link = 0;
               /* end lyg */
               segp->sip = segp_orig->sip;
               segp->bsp_next = segp_orig;
               segp_orig->bsp_prev = segp;
               segp_orig = segp_orig->next;
            }
         }
         segp_storage = segp_head;
         sabp_tmp->segp_head = segp_head;
      }
      sabp_newhead->next = sabp_head;
      sabp_head->prev = sabp_newhead;
      while (sabp_newhead)
      {
         sabp_head = sabp_newhead;
         sabp_newhead = sabp_newhead->prev;
      }
   }
   if (end)
   {
      segp = segp_head = NULL;
      segp_storage = NULL;
      sabp_tmp = sabp_newhead = NULL;
      while (sabp->next)
      {
         sabp = sabp->next;
      }
      for (i=1; i<=n; i++)
      {
         sabp_new = (SABlockPtr)MemNew(sizeof(SABlock));
         if (sabp_newhead)
         {
            sabp_tmp->next = sabp_new;
            sabp_new->prev = sabp_tmp;
            sabp_tmp = sabp_new;
            sabp_new->SegID = sabp_new->prev->SegID + 1;
         } else
         {
            sabp_tmp = sabp_newhead = sabp_new;
            sabp_new->SegID = sabp->SegID + 1;
         }
         sabp_new->AlignID = sabp->AlignID;
         /* begin lyg */
         sabp_new->RegionID = sabp->RegionID; 
         sabp_new->aligned = sabp->aligned;   
         sabp_new->from = sabp_new->to = sabp->to + 1; 
         /* end lyg */
         segp_tmp = segp_storage;
         segp_orig = sabp->segp_head;
         segp_head = NULL;
         for (j=1; j<=count; j++)
         {
            segp = (SegmentPtr)MemNew(sizeof(Segment));
            if (segp_head)
            {
               segp_prev->next = segp;
               segp->prev = segp_prev;
               segp_prev = segp;
            } else
            {
               segp_prev = segp_head = segp;
            }
            if (i > 1)
            {
               segp->BspID = segp_tmp->BspID;
               /* begin lyg */
               segp->gap = 1;  /* make single gaps */
               segp->from = segp->to = segp_tmp->to;
               segp->strand = segp_tmp->strand;
               segp->move = 0;
               segp->link = 0;
               /* end lyg */
               segp->sip = segp_tmp->sip;
               segp->bsp_prev = segp_tmp;
               segp_tmp->bsp_next = segp;
               segp_tmp = segp_tmp->next;
            } else
            {
               if (segp_storage)
               {
                  segp_tmp->next = segp;
                  segp_tmp = segp_tmp->next;
               } else
               {
                  segp_storage = segp_tmp = segp;
               }
               segp->BspID = segp_orig->BspID;
               /* begin lyg */
               segp->gap = 1;  /* make single gaps */
               segp->from = segp->to = segp_orig->to;
               segp->strand = segp_orig->strand;
               segp->move = 0;
               segp->link = 0;
               /* end lyg */
               segp->sip = segp_tmp->sip;
               segp->bsp_prev = segp_orig;
               segp_orig->bsp_next = segp;
               segp_orig = segp_orig->next;
            }
         }
         segp_storage = segp_head;
         sabp_tmp->segp_head = segp_head;
      }
      sabp->next = sabp_newhead;
      sabp_newhead->prev = sabp;
   }
   return sabp_head;
}

/****************************************************************
*  AddEmptySegments adds an empty segment structure onto
*  the end of the segment list for each SABlock.  The newly
*  added segment is uninitialized, except that the gap value is
*  set to 2.
****************************************************************/

NLM_EXTERN SABlockPtr AddEmptySegments(SABlockPtr sabp)
{
   SABlockPtr  sabp_head;
   SegmentPtr  segp;
   SegmentPtr  segp_temp;

   if (!sabp)
      return NULL;
   sabp_head = sabp;
   while (sabp)
   {
      segp = sabp->segp_head;
      while (segp->next)
      {
         segp = segp->next;
      }
      segp_temp = (SegmentPtr)MemNew(sizeof(Segment));
      segp_temp->prev = segp;
      segp->next = segp_temp;
      segp_temp->gap = 2;
      sabp = sabp->next;
   }
   return sabp_head;
}

/**********************************************************
*  FillInUnaligned is a helper function for Blockify for
*  Dense-diags.  If there are any blocks missing from
*  the master sequence (the first sequence, also the
*  alignment coordinates), the function allocates those
*  blocks and their segments, and fills in the information
*  for the first segment, leaving the others blank except
*  for filling in gap = 2 in each.  The function does not
*  add blocks to the beginning or end of the alignment,
*  so the alignment may not start at position 0 of the
*  master and it may not end at the end of the master.
*  Use other functions to fill in the "complete" 
*  alignment.  This function also switches the alignment
*  coordinates to zero-based values.
**********************************************************/

NLM_EXTERN SABlockPtr FillInUnaligned(SABlockPtr sabp)
{
   Int4        c;
   Int4        i;
   SABlockPtr  sabp_head;
   SABlockPtr  sabp_new;
   SABlockPtr  sabp_prev;
   SegmentPtr  segp;
   SegmentPtr  segp_head;
   SegmentPtr  segp_prev;
   SeqIdPtr    sip;

   if (!sabp)
      return NULL;
   sabp_head = sabp_prev = sabp;
   segp = sabp->segp_head;
   sip = segp->sip;
   i = 0;
   while (segp)
   {
      i += 1;
      segp = segp->next;
   }
   sabp->from -= 1;
   sabp->to -= 1;
   sabp = sabp->next;
   while (sabp)
   {
      sabp->from -= 1;
      sabp->to -= 1;
      if (sabp->from > sabp_prev->to + 1)
      {
         sabp_new = (SABlockPtr)MemNew(sizeof(SABlock));
         sabp_new->prev = sabp_prev;
         sabp_new->next = sabp;
         sabp->prev = sabp_new;
         sabp_prev->next = sabp_new;
         sabp_new->from = sabp_prev->to + 1;
         sabp_new->to = sabp->from - 1;
         sabp_new->aligned = FALSE;
         sabp_new->visible = 1;
         segp_head = NULL;
         for (c = 0; c < i; c++)
         {
            segp = (SegmentPtr)MemNew(sizeof(Segment));
            segp->visible = 1;
            if (segp_head)
            {
               segp->prev = segp_prev;
               segp_prev->next = segp;
               segp_prev = segp;
               segp->from = segp->to = 0;
               segp->gap = 2;
            } else
            {
               segp_head = segp_prev = segp;
               segp->from = sabp_new->from+1;
               segp->to = sabp_new->to+1;
               segp->sip = sip;
            }
         }
         sabp_new->segp_head = segp_head;
      }
      sabp_prev = sabp;
      sabp = sabp->next;
   }
   return sabp_head;
}

/*****************************************************************************
*
*   For a given column, return a pointer to the block that contains the
*   position.  Returns NULL if a containing block is not found. 
*
*****************************************************************************/

NLM_EXTERN SABlock * SAReturnBlock(SABlock *sabpHead, Int4 lColumn)
{
    SABlock *sabp;
    
    if(sabpHead == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on SAReturnBlock");
        return NULL;
    }
    
    for(sabp = sabpHead; sabp != NULL; sabp = sabp->next)
        if(lColumn >= sabp->from && lColumn <= sabp->to) break;
    return sabp;
}

/*****************************************************************************
*
*   For a given position, return a pointer to the Segment that contains the
*   position.  Returns NULL if a containing Segment is not found. Note that
*   the lRow argument is position, not BspID.
*
*****************************************************************************/

NLM_EXTERN Segment * SAReturnSegment(SABlock *sabpHead, Int4 lColumn, Int4 lRow)
{
    SABlock *sabp;
    Int4 i;
    Segment *segp;

    if(sabpHead == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on SAReturnSegment");
        return NULL;
    }

    sabp = SAReturnBlock(sabpHead, lColumn);
    if(sabp == NULL) return NULL;
    
    for(i = 0, segp = sabp->segp_head; segp != NULL && i < lRow;
             i++, segp = segp->next) continue;
    
    return segp;
}

/*****************************************************************************
*
*   Generic Segment traversal function with callback
*
*****************************************************************************/

NLM_EXTERN Int4 SATraverse(SABlock *sabpHead, pfnSAFunction pFunction, void *pData)
{
    Segment *segp;
    SABlock *sabp;
    
    if(sabpHead == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on SATraverse");
        return 0;
    }
    
    for(sabp = sabpHead; sabp != NULL; sabp = sabp->next)
        for(segp = sabp->segp_head; segp != NULL; segp = segp->next)
            (*pFunction)(segp, pData);

    return 1;
}

/*****************************************************************************
*
*   Traverser to clear the move bit
*
*****************************************************************************/

NLM_EXTERN void SAClearMove(Segment *pSegment, void *pData)
{
    if(pSegment == NULL) return;
    pSegment->move = FALSE;
}


/*****************************************************************************
*
*   Generic SABlock traversal function with callback
*
*****************************************************************************/

NLM_EXTERN Int4 SATraverseBlock(SABlock *sabpHead, pfnSABlockFunction pFunction,
         void * pData)
{
    SABlock *sabp;
    
    if(sabpHead == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid call on SABlockTraverse");
        return 0;
    }
    
    for(sabp = sabpHead; sabp != NULL; sabp = sabp->next)
            (*pFunction)(sabp, pData);

    return 1;
}

/*****************************************************************************
*
*   Traverser to find the highest valued RegionID
*
*****************************************************************************/

NLM_EXTERN void SARegionBounds(SABlock *sabp, void *pData)
{
    if(sabp == NULL) return;
    if(sabp->RegionID > *(Int4 *)pData) *(Int4 *)pData = sabp->RegionID;
}
