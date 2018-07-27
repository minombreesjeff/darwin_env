/*   sqnutil2.c
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
* File Name:  sqnutil2.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   9/2/97
*
* $Revision: 6.112 $
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

#include <sqnutils.h>
#include <gather.h>
#include <subutil.h>
#include <objfdef.h>
#include <seqport.h>
#include <objproj.h>
#include <gbfeat.h>
#include <gbftdef.h>
#include <edutil.h>
#include <tofasta.h>
#include <simple.h>
#include <validerr.h>

static CharPtr SqnTrimSpacesAroundString (CharPtr str)

{
  Uchar    ch;	/* to use 8bit characters in multibyte languages */
  CharPtr  dst;
  CharPtr  ptr;

  if (str != NULL && str [0] != '\0') {
    dst = str;
    ptr = str;
    ch = *ptr;
    while (ch != '\0' && ch <= ' ') {
      ptr++;
      ch = *ptr;
    }
    while (ch != '\0') {
      *dst = ch;
      dst++;
      ptr++;
      ch = *ptr;
    }
    *dst = '\0';
    dst = NULL;
    ptr = str;
    ch = *ptr;
    while (ch != '\0') {
      if (ch != ' ') {
        dst = NULL;
      } else if (dst == NULL) {
        dst = ptr;
      }
      ptr++;
      ch = *ptr;
    }
    if (dst != NULL) {
      *dst = '\0';
    }
  }
  return str;
}

static CharPtr SqnStringSave (CharPtr from)

{
  size_t  len;
  CharPtr to;

  to = NULL;
  len = StringLen (from);
  if (len > 0) {
    to = (CharPtr) MemGet (len + 1, FALSE);
    if (to != NULL) {
      MemCpy (to, from, len + 1);
      SqnTrimSpacesAroundString (to);
    }
  }
  return to;
}

NLM_EXTERN void UpdateLocalId (BioseqPtr bsp, CharPtr localId)

{
  Char         ch;
  ObjectIdPtr  oip;
  CharPtr      ptr;
  SeqIdPtr     sip;
  long         val;

  if (bsp != NULL) {
    if (localId != NULL) {
      sip = bsp->id;
      while (sip != NULL && sip->choice != SEQID_LOCAL) {
        sip = sip->next;
      }
      oip = NULL;
      if (sip != NULL) {
        oip = (ObjectIdPtr) sip->data.ptrvalue;
      } else {
        sip = ValNodeNew (bsp->id);
        if (bsp->id == NULL) {
          bsp->id = sip;
        }
        if (sip != NULL) {
          oip = ObjectIdNew ();
          sip->choice = SEQID_LOCAL;
          sip->data.ptrvalue = (Pointer) oip;
        }
      }
      if (oip != NULL) {
        oip->str = MemFree (oip->str);
        if (sscanf (localId, "%ld", &val) == 1) {
          oip->id = (Int4) val;
        } else {
          oip->str = SqnStringSave (localId);
          ptr = oip->str;
          ch = *ptr;
          while (ch != '\0') {
            if (ch == '|') {
              *ptr = '~';
            }
            ptr++;
            ch = *ptr;
          }
        }
      }
      SeqMgrReplaceInBioseqIndex (bsp);
    }
  }
}

NLM_EXTERN void UpdateTitle (BioseqPtr bsp, CharPtr title)

{
  ValNodePtr  vnp;

  if (bsp != NULL) {
    if (title != NULL) {
      vnp = NULL;
      if (bsp->descr != NULL) {
        vnp = ValNodeFindNext (bsp->descr, NULL, Seq_descr_title);
      }
      if (vnp == NULL) {
        vnp = ValNodeNew (bsp->descr);
        if (vnp != NULL) {
          vnp->choice = Seq_descr_title;
        }
        if (bsp->descr == NULL) {
          bsp->descr = vnp;
        }
      }
      if (vnp != NULL) {
        vnp->data.ptrvalue = MemFree (vnp->data.ptrvalue);
        vnp->data.ptrvalue = SqnStringSave (title);
      }
    }
  }
}

NLM_EXTERN GeneRefPtr CreateNewGeneRef (CharPtr locus, CharPtr allele,
                             CharPtr desc, Boolean pseudo)

{
  GeneRefPtr  geneRef;

  geneRef = GeneRefNew ();
  if (geneRef != NULL) {
    geneRef->locus = SqnStringSave (locus);
    geneRef->allele = SqnStringSave (allele);
    geneRef->desc = SqnStringSave (desc);
    geneRef->pseudo = pseudo;
    if (geneRef->locus == NULL && geneRef->allele == NULL && geneRef->desc == NULL) {
      geneRef = GeneRefFree (geneRef);
    }
  }
  return geneRef;
}

NLM_EXTERN ProtRefPtr CreateNewProtRef (CharPtr name, CharPtr desc,
                             CharPtr ec, CharPtr activity)

{
  ProtRefPtr  protRef;
  ValNodePtr  vnp;

  protRef = ProtRefNew ();
  if (protRef != NULL) {
    if (name != NULL && *name != '\0') {
      vnp = ValNodeNew (NULL);
      if (vnp != NULL) {
        vnp->data.ptrvalue = SqnStringSave (name);
        protRef->name = vnp;
      }
    }
    protRef->desc = SqnStringSave (desc);
    if (ec != NULL && *ec != '\0') {
      vnp = ValNodeNew (NULL);
      if (vnp != NULL) {
        vnp->data.ptrvalue = SqnStringSave (ec);
        protRef->ec = vnp;
      }
    }
    if (activity != NULL && *activity != '\0') {
      vnp = ValNodeNew (NULL);
      if (vnp != NULL) {
        vnp->data.ptrvalue = SqnStringSave (activity);
        protRef->activity = vnp;
      }
    }
    if (protRef->name == NULL && protRef->desc == NULL &&
        protRef->ec == NULL && protRef->activity == NULL) {
      protRef = ProtRefFree (protRef);
    }
  }
  return protRef;
}

NLM_EXTERN CdRegionPtr CreateNewCdRgn (Int2 frame, Boolean orf, Int2 genCode)

{
  CdRegionPtr  cdRgn;
  ValNodePtr   code;
  ValNodePtr   vnp;

  cdRgn = CdRegionNew ();
  if (cdRgn != NULL) {
    cdRgn->orf = orf;
    cdRgn->conflict = FALSE;
    cdRgn->frame = frame;
    cdRgn->gaps = 0;
    cdRgn->mismatch = 0;
    cdRgn->stops = 0;
    code = ValNodeNew (NULL);
    if (code != NULL) {
      code->choice = 254;
      vnp = ValNodeNew (NULL);
      code->data.ptrvalue = vnp;
      if (vnp != NULL) {
        vnp->choice = 2;
        vnp->data.intvalue = (Int4) genCode;
      }
    }
    cdRgn->genetic_code = code;
    cdRgn->code_break = NULL;
  }
  return cdRgn;
}

NLM_EXTERN void SetSeqFeatData (SeqFeatPtr sfp, Pointer data)

{
  if (sfp != NULL) {
    sfp->data.value.ptrvalue = (Pointer) data;
  }
}

NLM_EXTERN void SetSeqFeatProduct (SeqFeatPtr sfp, BioseqPtr bsp)

{
  ValNodePtr  slp;

  if (sfp != NULL) {
    sfp->product = SeqLocFree (sfp->product);
    if (bsp != NULL && bsp->id != NULL) {
      slp = ValNodeNew (NULL);
      if (slp != NULL) {
        slp->choice = 3;
        slp->data.ptrvalue = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (bsp->id, 0)));
      }
      sfp->product = slp;
    }
  }
}

NLM_EXTERN void ResetSeqFeatInterval (SeqFeatPtr sfp)

{
  if (sfp != NULL) {
    sfp->location = SeqLocFree (sfp->location);
  }
}

NLM_EXTERN void AddSeqFeatInterval (SeqFeatPtr sfp, BioseqPtr bsp, Int4 from,
                         Int4 to, Boolean partial5, Boolean partial3)

{
  Int2  fuzz_from;
  Int2  fuzz_to;
  Int2  strand;
  Int4  tmp;

  if (sfp != NULL && bsp != NULL) {
    strand = Seq_strand_plus;
    if (from > to) {
      tmp = from;
      from = to;
      to = tmp;
      strand = Seq_strand_minus;
    }
    fuzz_from = -1;
    fuzz_to = -1;
    if (partial5) {
      fuzz_from = 2;
    }
    if (partial3) {
      fuzz_to = 1;
    }
    AddIntToSeqFeat (sfp, from - 1, to - 1, bsp, fuzz_from, fuzz_to, strand);
  }
}

NLM_EXTERN void AddSeqFeatPoint (SeqFeatPtr sfp, BioseqPtr bsp, Int4 location,
                      Boolean fuzz_before, Boolean fuzz_after, Int2 strand)

{
	SeqLocPtr slp, tmp, tmp2;
	SeqPntPtr spp;
	IntFuzzPtr ifp;
	Int2 fuzz;

	spp = SeqPntNew();
	spp->point = location - 1;
	spp->id = SeqIdDup(SeqIdFindBest(bsp->id, 0));
	spp->strand = (Uint1)strand;

	fuzz = -1;
    if (fuzz_before) {
      fuzz = 4;        /* tl */
    } else if (fuzz_after) {
      fuzz = 3;        /* tr */
    }
	if (fuzz >= 0)
	{
		ifp = IntFuzzNew();
		ifp->choice = 4;   /* lim */
		ifp->a = (Int4)fuzz;
		spp->fuzz = ifp;
	}

	slp = ValNodeNew(NULL);
	slp->choice = SEQLOC_PNT;
	slp->data.ptrvalue = (Pointer)spp;

	if (sfp->location == NULL)
	{
		sfp->location = slp;
		return;
	}

	tmp = sfp->location;
	if (tmp->choice == SEQLOC_MIX)   /* second one already */
	{
		tmp2 = (ValNodePtr)(tmp->data.ptrvalue);
		while (tmp2->next != NULL)
			tmp2 = tmp2->next;
		tmp2->next = slp;
	}
	else                             /* create a chain */
	{
		tmp2 = ValNodeNew(NULL);
		tmp2->choice = SEQLOC_MIX;
		tmp2->data.ptrvalue = (Pointer)tmp;
		tmp->next = slp;
		sfp->location = tmp2;
	}
}

typedef struct seqlocrange {
  Int4		left;
  Int4		right;
  Uint1		strand;
  struct seqlocrange PNTR next;
 } SeqLocRange, PNTR SeqLocRangePtr;
 
static SeqLocRangePtr SeqLocRangeFree (SeqLocRangePtr slrp)

{
  SeqLocRangePtr  next;

  while (slrp != NULL) {
    next = slrp->next;
    MemFree (slrp);
    slrp = next;
  }
  return NULL;
}

static SeqLocRangePtr CollectRanges (BioseqPtr target, SeqLocPtr slp)

{
  SeqLocRangePtr  change;
  SeqLocPtr       curr;
  SeqLocRangePtr  head;
  SeqLocRangePtr  last;
  Int4            left;
  Int4            right;
  SeqLocRangePtr  slrp;
  Uint1           strand;

  if (target == NULL) return NULL;
  head = NULL;
  last = NULL;
  curr = SeqLocFindNext (slp, NULL);
  while (curr != NULL) {
    if (curr->choice != SEQLOC_NULL) {
      left = GetOffsetInBioseq (curr, target, SEQLOC_LEFT_END);
      right = GetOffsetInBioseq (curr, target, SEQLOC_RIGHT_END);
      strand = SeqLocStrand (curr);
      /* left > right if within a minus strand delta seq component, flip strand here */
      if (left > right) {
        if (strand == Seq_strand_minus) {
          strand = Seq_strand_plus;
        } else {
          strand = Seq_strand_minus;
        }
      }
      if (left != -1 && right != -1) {
        slrp = MemNew (sizeof (SeqLocRange));
        if (slrp != NULL) {
          slrp->left = left;
          slrp->right = right;
          slrp->strand = strand;
          if (head == NULL) {
            head = slrp;
          } else if (last != NULL) {
            last->next = slrp;
          } else {
            ErrPostEx (SEV_ERROR, 0, 0, "SeqLocMerge list problem");
            SeqLocRangeFree (head);
            return NULL;
          }
          last = slrp;
        }
      }
    }
    curr = SeqLocFindNext (slp, curr);
  }
  if (head == NULL || target->topology != TOPOLOGY_CIRCULAR) return head;
  left = GetOffsetInBioseq (slp, target, SEQLOC_LEFT_END);
  right = GetOffsetInBioseq (slp, target, SEQLOC_RIGHT_END);
  if (left == -1 || right == -1 || left <= right) return head;
  /* feature spans origin */
  change = NULL;
  left = head->left;
  strand = SeqLocStrand (slp);
  if (strand == Seq_strand_minus) {
    for (slrp = head->next; slrp != NULL; slrp = slrp->next) {
      if (slrp->left > left && change == NULL) {
        change = slrp;
      }
    }
  } else {
    for (slrp = head->next; slrp != NULL; slrp = slrp->next) {
      if (slrp->left < left && change == NULL) {
        change = slrp;
      }
    }
  }
  if (change == NULL) return head;
  if (strand == Seq_strand_minus) {
    for (slrp = change; slrp != NULL; slrp = slrp->next) {
      slrp->left -= target->length;
      slrp->right -= target->length;
    }
  } else {
    for (slrp = head; slrp != NULL && slrp != change; slrp = slrp->next) {
      slrp->left -= target->length;
      slrp->right -= target->length;
    }
  }
  return head;
}

static int LIBCALLBACK CompareRanges (VoidPtr ptr1, VoidPtr ptr2)

{
  SeqLocRangePtr   slrp1;
  SeqLocRangePtr   slrp2;

  if (ptr1 != NULL && ptr2 != NULL) {
    slrp1 = *((SeqLocRangePtr PNTR) ptr1);
    slrp2 = *((SeqLocRangePtr PNTR) ptr2);
    if (slrp1 != NULL && slrp2 != NULL) {
      if (slrp1->left > slrp2->left) {
        return 1;
      } else if (slrp1->left < slrp2->left) {
        return -1;
      } else if (slrp1->right > slrp2->right) {
        return 1;
      } else if (slrp1->right < slrp2->right) {
        return -1;
      } else {
        return 0;
      }
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

static int LIBCALLBACK CompareReverseRanges (VoidPtr ptr1, VoidPtr ptr2)

{
  return (0 - CompareRanges (ptr1, ptr2));
}

static SeqLocRangePtr SortRanges (SeqLocRangePtr list, Boolean reverse)

{
  size_t          count;
  SeqLocRangePtr  PNTR head;
  size_t          i;
  SeqLocRangePtr  tmp;

  if (list != NULL) {
    count = 0;
    tmp = list;
    while (tmp != NULL) {
      count++;
      tmp = tmp->next;
    }
    if (count > 0) {
      head = MemNew ((count + 1) * sizeof (SeqLocRangePtr));
      if (head != NULL) {
        tmp = list;
        i = 0;
        while (tmp != NULL && i < count) {
          head [i] = tmp;
          tmp = tmp->next;
          i++;
        }
        if (reverse) {
          HeapSort (head, count, sizeof (SeqLocRangePtr), CompareReverseRanges);
        } else {
          HeapSort (head, count, sizeof (SeqLocRangePtr), CompareRanges);
        }
        for (i = 0; i < count; i++) {
          tmp = head [i];
          tmp->next = head [i + 1];
        }
        list = head [0];
        MemFree (head);
      }
    }
  }
  return list;
}

static SeqLocRangePtr MergeOverlaps (SeqLocRangePtr list, Boolean fuse_joints, Boolean merge_overlaps)

{
  SeqLocRangePtr  last;
  SeqLocRangePtr  next;
  SeqLocRangePtr  this;

  if (list != NULL) {
    this = list->next;
    last = list;
    while (this != NULL) {
      next = this->next;
      if (merge_overlaps && this->left <= last->right) {
        last->right = MAX (this->right, last->right);
        MemFree (this);
        last->next = next;
      } else if (fuse_joints &&
                 (this->left == last->right + 1 && last->right != -1)) {
        last->right = MAX (this->right, last->right);
        MemFree (this);
        last->next = next;
      } else {
        last = this;
      }
      this = next;
    }
  }
  return list;
}

static SeqLocPtr SeqLocFromRange (SeqLocRangePtr head, BioseqPtr target,
                                  Boolean partial5, Boolean partial3,
                                  Boolean add_null)

{
  SeqLocPtr   firstSlp;
  Int4        from;
  Int2        fuzz_from;
  Int2        fuzz_to;
  IntFuzzPtr  ifp;
  SeqLocPtr   lastSlp;
  Boolean     notFirst;
  SeqFeatPtr  sfp;
  SeqIntPtr   sip;
  SeqLocPtr   slp;
  Int2        strand;
  Int4        tmp;
  SeqLocPtr   tmploc1;
  SeqLocPtr   tmploc2;
  Int4        to;

  if (head == NULL) return NULL;
  slp = NULL;
  sfp = SeqFeatNew ();
  if (sfp != NULL) {
    notFirst = FALSE;
    while (head != NULL) {
      fuzz_from = -1;
      fuzz_to = -1;
      from = head->left;
      to = head->right;
      strand = head->strand;
      if (from > to) {
        tmp = from;
        from = to;
        to = tmp;
      }
      if (add_null  && notFirst) {
        slp = ValNodeNew (NULL);
        if (slp != NULL) {
          slp->choice = SEQLOC_NULL;
          tmploc1 = sfp->location;
          if (tmploc1 != NULL) {
            if (tmploc1->choice == SEQLOC_MIX) {
              tmploc2 = (ValNodePtr) (tmploc1->data.ptrvalue);
              if (tmploc2 != NULL) {
                while (tmploc2->next != NULL) {
                  tmploc2 = tmploc2->next;
                }
                tmploc2->next = slp;
              }
            } else {
              tmploc2 = ValNodeNew (NULL);
              if (tmploc2 != NULL) {
                tmploc2->choice = SEQLOC_MIX;
                tmploc2->data.ptrvalue = (Pointer) tmploc1;
                tmploc1->next = slp;
                sfp->location = tmploc2;
              }
            }
          }
        }
      }
      AddIntToSeqFeat (sfp, from, to, target,
                       fuzz_from, fuzz_to, strand);
      notFirst = TRUE;
      head = head->next;
    }
    firstSlp = NULL;
    lastSlp = NULL;
    slp = SeqLocFindNext (sfp->location, NULL);
    while (slp != NULL) {
      if (firstSlp == NULL) {
        firstSlp = slp;
      }
      lastSlp = slp;
      slp = SeqLocFindNext (sfp->location, slp);
    }
    if (firstSlp != NULL && firstSlp->choice == SEQLOC_INT &&
        firstSlp->data.ptrvalue != NULL && partial5) {
      sip = (SeqIntPtr) firstSlp->data.ptrvalue;
      ifp = IntFuzzNew ();
      if (ifp != NULL) {
        ifp->choice = 4;
        if (sip->strand == Seq_strand_minus ||
            sip->strand == Seq_strand_both_rev) {
          sip->if_to = ifp;
          ifp->a = 1;
        } else {
          sip->if_from = ifp;
          ifp->a = 2;
        }
      }
    }
    if (lastSlp != NULL && lastSlp->choice == SEQLOC_INT &&
        lastSlp->data.ptrvalue != NULL && partial3) {
      sip = (SeqIntPtr) lastSlp->data.ptrvalue;
      ifp = IntFuzzNew ();
      if (ifp != NULL) {
        ifp->choice = 4;
        if (sip->strand == Seq_strand_minus ||
            sip->strand == Seq_strand_both_rev) {
          sip->if_from = ifp;
          ifp->a = 2;
        } else {
          sip->if_to = ifp;
          ifp->a = 1;
        }
      }
    }
    slp = sfp->location;
    sfp->location = NULL;
    SeqFeatFree (sfp);
  }
  return slp;
}

NLM_EXTERN SeqLocPtr SeqLocMergeEx (BioseqPtr target, SeqLocPtr to, SeqLocPtr from,
                       Boolean single_interval, Boolean fuse_joints,
                       Boolean merge_overlaps, Boolean add_null)

{
  SeqLocRangePtr  slrp;
  SeqLocRangePtr  head;
  SeqLocRangePtr  last;
  Boolean         partial5;
  Boolean         partial3;
  SeqLocPtr       slp;
  Int2            strand;

  if (target == NULL) return NULL;
  if (to == NULL && from == NULL) return NULL;

  slp = NULL;
  partial5 = FALSE;
  partial3 = FALSE;
  head = CollectRanges (target, to);
  if (head == NULL) {
    head = CollectRanges (target, from);
  } else {
    last = head;
    while (last->next != NULL) {
      last = last->next;
    }
    last->next = CollectRanges (target, from);
  }
  if (head != NULL) {
    strand = head->strand;
    head = SortRanges (head, FALSE);
    head = MergeOverlaps (head, fuse_joints, merge_overlaps);
    if (single_interval) {
      last = head;
      while (last->next != NULL) {
        last = last->next;
      }
      head->left = MIN (head->left, last->left);
      head->right = MAX (head->right, last->right);
      head->next = SeqLocRangeFree (head->next);
    }
    last = head;
    while (last != NULL) {
      last->strand = strand;
      last = last->next;
    }
    if (strand == Seq_strand_minus) {
      head = SortRanges (head, TRUE);
    }
    for (slrp = head; slrp != NULL; slrp = slrp->next) {
      if (slrp->left < 0) {
        slrp->left += target->length;
      }
      if (slrp->right < 0) {
        slrp->right += target->length;
      }
    }
    slp = SeqLocFromRange (head, target, partial5, partial3, add_null);
    head = SeqLocRangeFree (head);
  }
  return slp;
}

NLM_EXTERN SeqLocPtr SeqLocMerge (BioseqPtr target, SeqLocPtr to, SeqLocPtr from,
                       Boolean single_interval, Boolean fuse_joints,
                       Boolean add_null)

{
  return SeqLocMergeEx (target, to, from, single_interval, fuse_joints, TRUE, add_null);
}

NLM_EXTERN Boolean SeqLocBadSortOrder (BioseqPtr bsp, SeqLocPtr slp)

{
  SeqLocRangePtr  curr;
  SeqLocRangePtr  head;
  SeqLocRangePtr  last;

  if (bsp == NULL || slp == NULL) return FALSE;
  if (SeqLocCheck (slp) == SEQLOCCHECK_WARNING) return FALSE;
  /*
  if (SeqLocId (slp) == NULL) return FALSE;
  */
  head = CollectRanges (bsp, slp);
  if (head == NULL) return FALSE;
  if (head->next == NULL) {
    SeqLocRangeFree (head);
    return FALSE;
  }
  last = head;
  curr = head->next;
  while (curr != NULL) {
    if (curr->strand == Seq_strand_minus) {
      if (last->right < curr->right) {
        SeqLocRangeFree (head);
        return TRUE;
      }
    } else {
      if (last->left > curr->left) {
        SeqLocRangeFree (head);
        return TRUE;
      }
    }
    last = curr;
    curr = curr->next;
  }
  SeqLocRangeFree (head);
  return FALSE;
}

NLM_EXTERN Boolean SeqLocMixedStrands (BioseqPtr bsp, SeqLocPtr slp)

{
  SeqLocRangePtr  curr;
  SeqLocRangePtr  head;
  SeqLocRangePtr  last;
  Uint1           strand;

  if (bsp == NULL || slp == NULL) return FALSE;
  if (SeqLocCheck (slp) == SEQLOCCHECK_WARNING) return FALSE;
  /*
  if (SeqLocId (slp) == NULL) return FALSE;
  */
  head = CollectRanges (bsp, slp);
  if (head == NULL) return FALSE;
  if (head->next == NULL) {
    SeqLocRangeFree (head);
    return FALSE;
  }
  last = head;
  strand = last->strand;
  curr = head->next;
  while (curr != NULL) {
    if (curr->strand == Seq_strand_minus) {
      if (strand == Seq_strand_plus || strand == Seq_strand_unknown) {
        SeqLocRangeFree (head);
        return TRUE;
      }
    } else {
      if (strand == Seq_strand_minus) {
        SeqLocRangeFree (head);
        return TRUE;
      }
    }
    last = curr;
    curr = curr->next;
  }
  SeqLocRangeFree (head);
  return FALSE;
}

static void ConvertToFeatsCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent, Boolean toProts)

{
  BioseqPtr   bsp;
  SeqFeatPtr  sfp;
  ValNodePtr  vnp;

  if (sep == NULL) return;
  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;
  if (ISA_aa (bsp->mol) && (! toProts)) return;
  vnp = (ValNodePtr) mydata;
  if (vnp == NULL) return;
  while (vnp != NULL) {
    switch (vnp->choice) {
      case Seq_descr_pub :
        sfp = CreateNewFeature (sep, NULL, SEQFEAT_PUB, NULL);
        if (sfp != NULL) {
          sfp->data.value.ptrvalue = AsnIoMemCopy ((Pointer) vnp->data.ptrvalue,
                                                   (AsnReadFunc) PubdescAsnRead,
                                                   (AsnWriteFunc) PubdescAsnWrite);
        }
        break;
      case Seq_descr_source :
        sfp = CreateNewFeature (sep, NULL, SEQFEAT_BIOSRC, NULL);
        if (sfp != NULL) {
          sfp->data.value.ptrvalue = AsnIoMemCopy ((Pointer) vnp->data.ptrvalue,
                                                   (AsnReadFunc) BioSourceAsnRead,
                                                   (AsnWriteFunc) BioSourceAsnWrite);
        }
        break;
      case Seq_descr_comment :
        sfp = CreateNewFeature (sep, NULL, SEQFEAT_COMMENT, NULL);
        if (sfp != NULL) {
          sfp->comment = StringSave ((CharPtr) vnp->data.ptrvalue);
        }
        break;
      default :
        break;
    }
    vnp = vnp->next;
  }
}

static void ConvertToFeatsOnNucsAndProts (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  ConvertToFeatsCallback (sep, mydata, index, indent, TRUE);
}

static void ConvertToFeatsOnNucsOnly (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  ConvertToFeatsCallback (sep, mydata, index, indent, FALSE);
}

static void ExtractPubSrcComDescs (SeqEntryPtr sep, ValNodePtr PNTR head, Boolean pub, Boolean src, Boolean com)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  ValNodePtr    nextsdp;
  Pointer PNTR  prevsdp;
  ValNodePtr    sdp;

  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sdp = bsp->descr;
    prevsdp = (Pointer PNTR) &(bsp->descr);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sdp = bssp->descr;
    prevsdp = (Pointer PNTR) &(bssp->descr);
  } else return;
  while (sdp != NULL) {
    nextsdp = sdp->next;
    if ((sdp->choice == Seq_descr_pub && pub) ||
        (sdp->choice == Seq_descr_source && src) ||
        (sdp->choice == Seq_descr_comment && com)) {
      *(prevsdp) = sdp->next;
      sdp->next = NULL;
      ValNodeLink (head, sdp);
    } else {
      prevsdp = (Pointer PNTR) &(sdp->next);
    }
    sdp = nextsdp;
  }
}

NLM_EXTERN Boolean ConvertPubSrcComDescsToFeats (SeqEntryPtr sep, Boolean pub, Boolean src, Boolean com, Boolean toProts)

{
  BioseqSetPtr  bssp;
  ValNodePtr    head;
  Boolean       rsult;
  ValNodePtr    sdp;

  rsult = FALSE;
  if (! (pub || src || com)) return FALSE;
  if (sep == NULL || sep->data.ptrvalue == NULL) return FALSE;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
      if (ConvertPubSrcComDescsToFeats (sep, pub, src, com, toProts)) {
        rsult = TRUE;
      }
    }
    return rsult;
  }
  head = NULL;
  ExtractPubSrcComDescs (sep, &head, pub, src, com);
  rsult = (head != NULL);
  if (toProts) {
    BioseqExplore (sep, head, ConvertToFeatsOnNucsAndProts);
  } else {
    BioseqExplore (sep, head, ConvertToFeatsOnNucsOnly);
  }
  for (sdp = head; sdp != NULL; sdp = sdp->next) {
    switch (sdp->choice) {
      case Seq_descr_pub :
        PubdescFree ((PubdescPtr) sdp->data.ptrvalue);
        break;
      case Seq_descr_source :
        BioSourceFree ((BioSourcePtr) sdp->data.ptrvalue);
        break;
      case Seq_descr_comment :
        MemFree (sdp->data.ptrvalue);
        break;
      default :
        break;
    }
  }
  ValNodeFree (head);
  return rsult;
}

/* from Colombe */

static CharPtr sqn_string_complement (CharPtr str)
{
  CharPtr strp;

  for (strp = str; *strp != '\0'; strp++) {
         if (*strp == 'A') *strp = 'T';
         else if (*strp == 'T') *strp = 'A';
         else if (*strp == 'C') *strp = 'G';
         else if (*strp == 'G') *strp = 'C';
  }
  *strp = '\0';
  return str;
}

static CharPtr sqn_string_reverse (CharPtr str)
{
  Char    car;
  Int4    j;
  Int4    k;

  j = 0;
  k = StringLen (str) - 1;
  while (j < k) {
    car = str[j]; str[j] = str[k]; str[k] = car;
    j++;
    k--;
  }
  return str;
}

static Int4 sqn_ReadBufferFromSep (SeqPortPtr spp, CharPtr buffer, Int4 from, Int4 to, Int4 buffsegstart)
{
  Uint1    residue;
  Int4     k;
  Int4     pos;

  SeqPortSeek (spp, from, SEEK_SET);
  k = buffsegstart;
  pos = from;
  residue = SeqPortGetResidue(spp);
  while (pos < to && residue != SEQPORT_EOF)
  {
    if ( ! IS_residue(residue)) {
      /*
      switch (residue)
      {  
           case SEQPORT_VIRT:
              Message(MSG_OK,"SEQPORT_VIRT [%d=%c] at %ld\n", (int)residue, (char)residue, (long)pos);
              break;
           case SEQPORT_EOS:
              Message(MSG_OK,"[EOS]\n");
              break;
           default:
              Message(MSG_OK,"unknown char\n");
              break;
      }  
      pos++;
      */
    } else {
      buffer[k] = (Char) residue;
      k++;  
      pos++;
    }
    residue = SeqPortGetResidue(spp);
  }
  buffer[k] = '\0';
  return k;
}
 
static CharPtr sqn_load_seq_data (SeqIdPtr sip, Int4 from, Int4 to, Boolean is_prot, Int4 *lenp)
{
  BioseqPtr        bsp;
  SeqLocPtr        slp;
  SeqPortPtr       spp;
  CharPtr          str = NULL;
  Int4             lens;

  if (from > -1 && to > -1 && from >= to)
     return NULL;
  bsp = BioseqLockById (sip);
  if (bsp != NULL) {
     if (from < 0 || from > bsp->length -1)
        from = 0;
     if (to < 0 || to > bsp->length -1)
        to = bsp->length -1;
     BioseqUnlock (bsp);
     slp = SeqLocIntNew (from, to, Seq_strand_plus, sip);
     if (is_prot)
        spp = SeqPortNewByLoc (slp, Seq_code_ncbistdaa);
     else
        spp = SeqPortNewByLoc (slp, Seq_code_iupacna);
     if (spp != NULL) {
        str = MemNew ((to-from+4) * sizeof(Char));
        lens = sqn_ReadBufferFromSep (spp, str, 0, to -from +1, 0);
        SeqPortFree (spp);
        if (lenp != NULL)
           *lenp = lens;
     }   
     SeqLocFree (slp);
  }
  return str;
}

static Int4 getlengthforid (SeqIdPtr sip)
{
  BioseqPtr        bsp;
  Int4             lens=0;

  if (sip==NULL)
     return 0;
  bsp = BioseqLockById (sip);
  if (bsp != NULL) {
     lens = bsp->length;
     BioseqUnlock (bsp);
  }
  return lens;
}

NLM_EXTERN SeqLocPtr StringSearchInBioseq (SeqIdPtr sip, CharPtr sub)
{
  SeqLocPtr slp=NULL;
  CharPtr   strdb,
            strtmp;
  Int4      lenbsp,
            fromp, top;
  Int4      lensub,
            lens,
            offset,
            shiftRange,
            maxRange;
  Boolean   firstpass=TRUE;

  lensub = StringLen (sub);
  maxRange = (Int4) MAX ((Int4)1000, lensub);
  lenbsp = getlengthforid (sip);
  while (slp == NULL) 
  {
   fromp = 0;
   top = MIN ((Int4)(fromp+maxRange), lenbsp) -1;
   while (fromp <= lenbsp && slp == NULL)
   {
     strdb = sqn_load_seq_data (sip, fromp, top, FALSE, &lens); 
     if (strdb != NULL)
     {
        offset = 0;
        strtmp = StringISearch (strdb, sub);
        if (strtmp != NULL) {
           offset =(Int4)abs(abs((long)strdb)-abs((long)strtmp));
           offset += fromp;
           if (offset > 0) {
              if (firstpass)
                 slp = SeqLocIntNew (offset, offset+lensub-1, Seq_strand_plus, sip);
              else 
                 slp = SeqLocIntNew (offset, offset+lensub-1, Seq_strand_minus, sip);
           }
        }
        MemFree (strdb);
     }
     shiftRange = maxRange - lensub;
     fromp = fromp + shiftRange;
     top = MIN ((Int4)(fromp+maxRange), lenbsp);
   }
   if (!firstpass) {
      sub = sqn_string_complement (sub);
      sub = sqn_string_reverse (sub);
      break;
   }
   firstpass=FALSE;
   sub = sqn_string_complement (sub);
   sub = sqn_string_reverse (sub);
  }
  return slp;
}

/*****************************************************************************
*
*   SequinEntryList (sep, mydata, mycallback, index, indent)
*       traverses all Seq-entry nodes beginning with sep
*       calls mycallback () at each node
*       Does enter BioseqSets of _class "parts", but ignores the
*       parts set itself
*
*****************************************************************************/

NLM_EXTERN Int4 SequinEntryList (SeqEntryPtr sep, Pointer mydata,
                                 SeqEntryFunc mycallback,
                                 Int4 index, Int2 indent)

{
  BioseqSetPtr  bssp;

  if (sep == NULL) return index;
  if (IS_Bioseq (sep)) {
    if (mycallback != NULL)
      (*mycallback) (sep, mydata, index, indent);
    return index + 1;
  }
  /*
  if (Bioseq_set_class (sep) == 4) return index;
  index++;
  */
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  sep = bssp->seq_set;
  indent++;
  while (sep != NULL) {
    index = SequinEntryList (sep, mydata, mycallback, index, indent);
    sep = sep->next;
  }
  return index;
}

/* functions to parse [org=Drosophila melanogaster] and [gene=lacZ] from titles */

NLM_EXTERN SqnTagPtr SqnTagParse (CharPtr ttl)

{
  Int2       num_tags;
  CharPtr    ptr;
  CharPtr    query;
  SqnTagPtr  stp;
  CharPtr    str;
  CharPtr    tmp;

  if (StringHasNoText (ttl)) return NULL;
  stp = (SqnTagPtr) MemNew (sizeof (SqnTag));
  if (stp == NULL) return NULL;
  query = StringSave (ttl);

  str = StringChr (query, '[');
  for (num_tags = 0; num_tags < MAX_SQN_TAGS && str != NULL; num_tags++) {
    str++;
    stp->tag [num_tags] = str;
    ptr = StringChr (str, '=');
    if (ptr != NULL) {
      *ptr = '\0';
      ptr++;
      stp->val [num_tags] = ptr;
      tmp = StringChr (ptr, ']');
      if (tmp != NULL) {
        *tmp = '\0';
        tmp++;
        str = StringChr (tmp, '[');
      }
      TrimSpacesAroundString (stp->tag [num_tags]);
      TrimSpacesAroundString (stp->val [num_tags]);
    }
  }

  stp->query = query;
  stp->num_tags = num_tags;

  return stp;
}

NLM_EXTERN SqnTagPtr SqnTagFree (SqnTagPtr stp)

{
  if (stp == NULL) return NULL;
  MemFree (stp->query);
  return MemFree (stp);
}

NLM_EXTERN CharPtr SqnTagFind (SqnTagPtr stp, CharPtr tag)

{
  Int2  i;

  if (stp == NULL || StringHasNoText (tag)) return NULL;
  for (i = 0; i < stp->num_tags; i++) {
    if (stp->tag [i] != NULL && StringICmp (stp->tag [i], tag) == 0) {
      return stp->val [i];
    }
  }
  return NULL;
}

/* functions to extract BioSource, MolInfo, and Bioseq information from parsed titles */

static CharPtr sqntag_biosrc_genome_list [] = {
  "?", "genomic", "chloroplast", "chromoplast", "kinetoplast",
  "mitochondrion", "plastid", "macronuclear", "extrachromosomal",
  "plasmid", "transposon", "insertion sequence", "cyanelle",
  "proviral", "virion", "nucleomorph", "apicoplast", "leucoplast",
  "proplastid", "endogenous-virus", NULL
};

static CharPtr sqntag_biosrc_orgmod_list [] = {
  "?", "?", "strain", "substrain", "type", "subtype", "variety",
  "serotype", "serogroup", "serovar", "cultivar", "pathovar", "chemovar",
  "biovar", "biotype", "group", "subgroup", "isolate", "common name",
  "acronym", "dosage", "natural host", "sub-species", "specimen-voucher",
  "authority", "forma", "forma-specialis", "ecotype", "synonym",
  "anamorph", "teleomorph", "breed", NULL
};

static CharPtr sqntag_biosrc_subsource_list [] = {
  "?", "chromosome", "map", "clone", "subclone", "haplotype",
  "genotype", "sex", "cell-line", "cell-type", "tissue-type",
  "clone-lib", "dev-stage", "frequency", "germline", "rearranged",
  "lab-host", "pop-variant", "tissue-lib", "plasmid-name", "transposon-name",
  "ins-seq-name", "plastid-name", "country", "segment", "endogenous-virus-name",
  "transgenic", "environmental-sample", "isolation-source", NULL
};

NLM_EXTERN BioSourcePtr ParseTitleIntoBioSource (
  SqnTagPtr stp,
  CharPtr organism,
  BioSourcePtr biop
)

{
  Int2          i;
  OrgModPtr     omp;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  SubSourcePtr  ssp;
  CharPtr       str;
  int           val;

  if ((stp == NULL || stp->num_tags == 0) && StringHasNoText (organism)) return biop;

  if (biop == NULL) {
    biop = BioSourceNew ();
    if (biop == NULL) return biop;
  }
  if (biop->org == NULL) {
    biop->org = OrgRefNew ();
  }
  orp = biop->org;
  if (orp->orgname == NULL) {
    orp->orgname = OrgNameNew ();
  }
  onp = orp->orgname;

  str = SqnTagFind (stp, "organism");
  if (str == NULL) {
    str = SqnTagFind (stp, "org");
  }
  if (organism == NULL) {
    organism = str;
  }
  if (! StringHasNoText (organism)) {
    if (StringICmp (orp->taxname, organism) != 0) {

      /* if command line or fasta defline organism doesn't match, clear template */

      biop->org = OrgRefFree (biop->org);
      biop->subtype = SubSourceFree (biop->subtype);

      /* then recreate orgref and orgname structures, save organism name */

      biop->org = OrgRefNew ();
      orp = biop->org;
      orp->orgname = OrgNameNew ();
      onp = orp->orgname;

      orp->taxname = StringSave (organism);
    }
  }

  if (stp == NULL) return biop;

  str = SqnTagFind (stp, "location");
  if (str != NULL) {
    if (StringICmp (str, "mitochondrial") == 0) {
      str = "mitochondrion";
    } else if (StringICmp (str, "provirus") == 0) {
      str = "proviral";
    }
    for (i = 0; sqntag_biosrc_genome_list [i] != NULL; i++) {
      if (StringICmp (str, sqntag_biosrc_genome_list [i]) == 0) {
        biop->genome = (Uint1) i;
      }
    }
  }

  for (i = 0; sqntag_biosrc_orgmod_list [i] != NULL; i++) {
    str = SqnTagFind (stp, sqntag_biosrc_orgmod_list [i]);
    if (str != NULL) {
      for (omp = onp->mod;
           omp != NULL && omp->subtype != (Uint1) i;
           omp = omp->next) continue;
      if (omp != NULL) {
        omp->subname = MemFree (omp->subname);
        omp->subname = StringSave (str);
      } else {
        omp = OrgModNew ();
        if (omp != NULL) {
          omp->subtype = (Uint1) i;
          omp->subname = StringSave (str);
          omp->next = onp->mod;
          onp->mod = omp;
        }
      }
    }
  }

  for (i = 0; sqntag_biosrc_subsource_list [i] != NULL; i++) {
    str = SqnTagFind (stp, sqntag_biosrc_subsource_list [i]);
    if (str != NULL) {
      for (ssp = biop->subtype;
           ssp != NULL && ssp->subtype != (Uint1) i;
           ssp = ssp->next) continue;
      if (ssp != NULL) {
        ssp->name = MemFree (ssp->name);
        ssp->name = StringSave (str);
      } else {
        ssp = SubSourceNew ();
        if (ssp != NULL) {
          ssp->subtype = (Uint1) i;
          ssp->name = StringSave (str);
          ssp->next = biop->subtype;
          biop->subtype = ssp;
        }
      }
    }
  }

  str = SqnTagFind (stp, "division");
  if (str == NULL) {
    str = SqnTagFind (stp, "div");
  }
  if (str != NULL) {
    onp->div = MemFree (onp->div);
    onp->div = StringSave (str);
  }

  str = SqnTagFind (stp, "lineage");
  if (str != NULL) {
    onp->lineage = MemFree (onp->lineage);
    onp->lineage = StringSave (str);
  }

  str = SqnTagFind (stp, "gcode");
  if (str != NULL && sscanf (str, "%d", &val) == 1) {
    onp->gcode = (Uint1) val; /* cytoplasmic */
  }

  str = SqnTagFind (stp, "mgcode");
  if (str != NULL && sscanf (str, "%d", &val) == 1) {
    onp->mgcode = (Uint1) val; /* mitochondrial */
  }

  str = SqnTagFind (stp, "note");
  if (str != NULL) {
    ssp = SubSourceNew ();
    if (ssp != NULL) {
      ssp->subtype = (Uint1) SUBSRC_other;
      ssp->name = StringSave (str);
      ssp->next = biop->subtype;
      biop->subtype = ssp;
    }
  }

  return biop;
}

static CharPtr molinfo_biomol_list [] = {
  "?", "genomic", "precursor RNA", "mRNA", "rRNA", "tRNA", "snRNA",
  "scRNA", "peptide", "other-genetic", "genomic-mRNA", "cRNA", "snoRNA", NULL
};

static CharPtr molinfo_tech_list [] = {
  "?", "standard", "EST", "STS", "survey", "genetic map", "physical map",
  "derived", "concept-trans", "seq-pept", "both", "seq-pept-overlap",
  "seq-pept-homol", "concept-trans-a", "htgs 1", "htgs 2", "htgs 3",
  "fli cDNA", "htgs 0", "htc", NULL
};

NLM_EXTERN MolInfoPtr ParseTitleIntoMolInfo (
  SqnTagPtr stp,
  MolInfoPtr mip
)

{
  Int2     i;
  CharPtr  str;

  if (stp == NULL) return mip;

  if (mip == NULL) {
    mip = MolInfoNew ();
    if (mip == NULL) return mip;
  }

  str = SqnTagFind (stp, "moltype");
  if (str != NULL) {
    for (i = 0; molinfo_biomol_list [i] != NULL; i++) {
      if (StringICmp (str, molinfo_biomol_list [i]) == 0) {
        mip->biomol = (Uint1) i;
      }
    }
  }

  str = SqnTagFind (stp, "tech");
  if (str != NULL) {
    for (i = 0; molinfo_tech_list [i] != NULL; i++) {
      if (StringICmp (str, molinfo_tech_list [i]) == 0) {
        mip->tech = (Uint1) i;
      }
    }
  }

  return mip;
}

NLM_EXTERN BioseqPtr ParseTitleIntoBioseq (
  SqnTagPtr stp,
  BioseqPtr bsp
)

{
  CharPtr  str;

  if (stp == NULL || bsp == NULL) return bsp;

  str = SqnTagFind (stp, "topology");
  if (str == NULL) {
    str = SqnTagFind (stp, "top");
  }
  if (str != NULL) {
    if (StringICmp (str, "linear") == 0) {
      bsp->topology = TOPOLOGY_LINEAR;
    } else if (StringICmp (str, "circular") == 0) {
      bsp->topology = TOPOLOGY_CIRCULAR;
    }
  }

  str = SqnTagFind (stp, "molecule");
  if (str == NULL) {
    str = SqnTagFind (stp, "mol");
  }
  if (str != NULL) {
    if (StringICmp (str, "dna") == 0) {
      bsp->mol = Seq_mol_dna;
    } else if (StringICmp (str, "rna") == 0) {
      bsp->mol = Seq_mol_rna;
    }
  }

  return bsp;
}

NLM_EXTERN GeneRefPtr ParseTitleIntoGeneRef (
  SqnTagPtr stp,
  GeneRefPtr grp
)

{
  CharPtr  str;

  if (stp == NULL || grp == NULL) return grp;

  str = SqnTagFind (stp, "gene");
  if (str != NULL) {
    grp->locus = StringSave (str);
  }

  str = SqnTagFind (stp, "allele");
  if (str != NULL) {
    grp->allele = StringSave (str);
  }

  str = SqnTagFind (stp, "gene_syn");
  if (str != NULL) {
    ValNodeCopyStr (&(grp->syn), 0, str);
  }

  return grp;
}

NLM_EXTERN ProtRefPtr ParseTitleIntoProtRef (
  SqnTagPtr stp,
  ProtRefPtr prp
)

{
  CharPtr  str;

  if (stp == NULL || prp == NULL) return prp;

  str = SqnTagFind (stp, "protein");
  if (str == NULL) {
    str = SqnTagFind (stp, "prot");
  }
  if (str != NULL) {
    ValNodeCopyStr (&(prp->name), 0, str);
  }

  str = SqnTagFind (stp, "EC_number");
  if (str != NULL) {
    ValNodeCopyStr (&(prp->ec), 0, str);
  }

  str = SqnTagFind (stp, "activity");
  if (str == NULL) {
    str = SqnTagFind (stp, "function");
  }
  if (str != NULL) {
    ValNodeCopyStr (&(prp->activity), 0, str);
  }

  return prp;
}

NLM_EXTERN GBBlockPtr ParseTitleIntoGenBank (
  SqnTagPtr stp,
  GBBlockPtr gbp
)

{
  Char     ch;
  CharPtr  last;
  CharPtr  ptr;
  CharPtr  str;
  CharPtr  tmp;

  if (stp == NULL) return gbp;

  if (gbp == NULL) {
    gbp = GBBlockNew ();
    if (gbp == NULL) return gbp;
  }

  str = SqnTagFind (stp, "secondary-accession");
  if (str == NULL) {
    str = SqnTagFind (stp, "secondary-accessions");
  }
  if (str != NULL) {
    tmp = StringSave (str);
    last = tmp;
    ptr = last;
    ch = *ptr;
    while (ch != '\0') {
      if (ch == ',') {
        *ptr = '\0';
        if (! StringHasNoText (last)) {
          TrimSpacesAroundString (last);
          ValNodeCopyStr (&(gbp->extra_accessions), 0, last);
        }
        ptr++;
        last = ptr;
        ch = *ptr;
      } else {
        ptr++;
        ch = *ptr;
      }
    }
    if (! StringHasNoText (last)) {
      TrimSpacesAroundString (last);
      ValNodeCopyStr (&(gbp->extra_accessions), 0, last);
    }
    MemFree (tmp);
  }

  return gbp;
}

static void AddStringToSeqHist (
  SeqHistPtr shp,
  CharPtr str
)

{
  Char          prefix [20];
  SeqIdPtr      sip;
  TextSeqIdPtr  tsip;
  Uint4         whichdb;

  if (shp == NULL || StringHasNoText (str)) return;
  sip = ValNodeAdd (&(shp->replace_ids));
  if (sip == NULL) return;
  tsip = TextSeqIdNew ();
  StringNCpy_0 (prefix, str, sizeof (prefix));
  whichdb = WHICH_db_accession (prefix);
  if (ACCN_IS_EMBL (whichdb)) {
    sip->choice = SEQID_EMBL;
  } else if (ACCN_IS_DDBJ (whichdb)) {
    sip->choice = SEQID_DDBJ;
  } else {
    sip->choice = SEQID_GENBANK;
  }
  sip->data.ptrvalue = (Pointer) tsip;
  tsip->accession = StringSave (str);
}

NLM_EXTERN SeqHistPtr ParseTitleIntoSeqHist (
  SqnTagPtr stp,
  SeqHistPtr shp
)

{
  Char     ch;
  CharPtr  last;
  CharPtr  ptr;
  CharPtr  str;
  CharPtr  tmp;

  if (stp == NULL) return shp;

  if (shp == NULL) {
    shp = SeqHistNew ();
    if (shp == NULL) return shp;
  }

  str = SqnTagFind (stp, "secondary-accession");
  if (str == NULL) {
    str = SqnTagFind (stp, "secondary-accessions");
  }
  if (str != NULL) {
    tmp = StringSave (str);
    last = tmp;
    ptr = last;
    ch = *ptr;
    while (ch != '\0') {
      if (ch == ',') {
        *ptr = '\0';
        if (! StringHasNoText (last)) {
          TrimSpacesAroundString (last);
          AddStringToSeqHist (shp, last);
        }
        ptr++;
        last = ptr;
        ch = *ptr;
      } else {
        ptr++;
        ch = *ptr;
      }
    }
    if (! StringHasNoText (last)) {
      TrimSpacesAroundString (last);
      AddStringToSeqHist (shp, last);
    }
    MemFree (tmp);
  }

  return shp;
}

/* PHRAP file reading functions */

static Boolean HasNoText (CharPtr str)

{
  Uchar  ch;	/* to use 8bit characters in multibyte languages */

  if (str != NULL) {
    ch = *str;
    while (ch != '\0') {
      if (ch > ' ') {
        return FALSE;
      }
      str++;
      ch = *str;
    }
  }
  return TRUE;
}

static CharPtr ReadALine (CharPtr str, size_t size, FILE *fp)

{
  Char     ch;
  CharPtr  ptr;
  CharPtr  rsult;

  if (str == NULL || size < 1 || fp == NULL) return NULL;
  *str = '\0';
  rsult = fgets (str, size, fp);
  if (rsult != NULL) {
    ptr = str;
    ch = *ptr;
    while (ch != '\0' && ch != '\n' && ch != '\r') {
      ptr++;
      ch = *ptr;
    }
    *ptr = '\0';
  }
  return rsult;
}

static CharPtr ReadALineOfScores (CharPtr str, size_t size, FILE *fp, BoolPtr nonewline)

{
  Char     ch;
  CharPtr  ptr;
  CharPtr  rsult;

  if (nonewline != NULL) {
    *nonewline = FALSE;
  }
  if (str == NULL || size < 1 || fp == NULL) return NULL;
  *str = '\0';
  rsult = fgets (str, size, fp);
  if (rsult != NULL) {
    ptr = str;
    ch = *ptr;
    while (ch != '\0' && ch != '\n' && ch != '\r') {
      ptr++;
      ch = *ptr;
    }
    *ptr = '\0';
    if (nonewline != NULL) {
      if (ch != '\n' && ch != '\r') {
        *nonewline = TRUE;
      } else {
        *nonewline = FALSE;
      }
    }
  }
  return rsult;
}

static SeqEntryPtr ReadPhrapDNA (FILE *fp, CharPtr id)

{
  ByteStorePtr  bs = NULL;
  BioseqPtr     bsp = NULL;
  Char          buf [256];
  Char          ch;
  Boolean       goOn = TRUE;
  CharPtr       p;
  CharPtr       q;
  SeqEntryPtr   sep = NULL;
  CharPtr       str;

  if (fp == NULL || HasNoText (id)) return NULL;
  sep = SeqEntryNew ();
  if (sep == NULL) return NULL;
  bsp = BioseqNew ();
  if (bsp == NULL) return NULL;
  bs = BSNew (1000);
  if (bs == NULL) return NULL;

  sep->choice = 1;
  sep->data.ptrvalue = (Pointer) bsp;
  SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, sep);

  bsp->mol = Seq_mol_na;
  bsp->seq_data_type = Seq_code_iupacna;
  bsp->repr = Seq_repr_raw;
  bsp->length = 0;
  bsp->id = MakeSeqID (id);
  SeqMgrAddToBioseqIndex (bsp);

  goOn = TRUE;
  while (goOn) {
    str = ReadALine (buf, sizeof (buf), fp);
    if (HasNoText (str)) {
      goOn = FALSE;
    } else {
      p = str;
      q = str;
      ch = *p;
      while (ch != '\0') {
        if (! (IS_ALPHA (ch))) {
          p++;
        } else {
          ch = TO_UPPER (ch);
          if (ch == 'X') {
            ch = 'N';
          }
          *q = ch;
          p++;
          q++;
        }
        ch = *p;
      }
      *q = '\0';
      BSWrite (bs, (VoidPtr) str, (Int4) StringLen (str));
    }
  }

  bsp->seq_data = bs;
  bsp->length = BSLen (bs);

  BioseqPack (bsp);
  return sep;
}

static SeqGraphPtr ReadPhrapQuality (FILE *fp, BioseqPtr bsp)

{
  ByteStorePtr  bs = NULL;
  Char          buf [256];
  Uint1         bytes [128];
  Char          ch;
  Boolean       goOn = TRUE;
  Int2          i;
  Int2          max = INT2_MIN;
  Int2          min = INT2_MAX;
  Boolean       nonewline;
  CharPtr       p;
  CharPtr       q;
  Char          prefix [256];
  size_t        prefixlen;
  SeqGraphPtr   sgp = NULL;
  SeqIntPtr     sintp;
  CharPtr       str;
  int           val;

  if (fp == NULL || bsp == NULL) return NULL;
  sgp = SeqGraphNew ();
  if (sgp == NULL) return NULL;
  bs = BSNew (1000);
  if (bs == NULL) return NULL;

  goOn = TRUE;
  buf [0] = '\0';
  prefix [0] = '\0';
  while (goOn) {
    StringCpy (buf, prefix);
    prefix [0] = '\0';
    prefixlen = StringLen (buf);
    ReadALineOfScores (buf + prefixlen, sizeof (buf) - prefixlen, fp, &nonewline);
    /* above function returned prefix characters past buf start */
    str = buf;
    if (HasNoText (str)) {
      goOn = FALSE;
    } else {
      i = 0;
      p = str;
      ch = *p;
      while (ch != '\0') {
        while (IS_WHITESP (ch)) {
          p++;
          ch = *p;
        }
        q = p;
        ch = *q;
        while (IS_DIGIT (ch)) {
          q++;
          ch = *q;
        }
        *q = '\0';
        q++;

        if (ch == '\0' && nonewline) {
          StringCpy (prefix, p);
        } else {
          if (*p != '\0') {
            if (sscanf (p, "%d", &val) == 1) {
              if (val < 0 || val > 255) {
                /* error */
                val = 0;
              }
              bytes [i] = (Uint1) val;
              i++;
              max = MAX (max, (Int2) val);
              min = MIN (min, (Int2) val);
            }
          }
          p = q;
          ch = *p;
        }
      }
      if (i > 0) {
        BSWrite (bs, (Pointer) bytes, (Int4) i);
      }
    }
  }

  sgp->numval = BSLen (bs);
  BSPutByte (bs, EOF);
  sgp->title = StringSave ("Phrap Quality");
  if (bsp->length != sgp->numval) {
    sgp->flags [0] = 1;
    sgp->compr = (bsp->length) / sgp->numval;
  } else {
    sgp->flags [0] = 0;
    sgp->compr = 1;
  }
  sgp->flags [1] = 0;
  sgp->flags [2] = 3;
  sgp->axis.intvalue = 0;
  sgp->min.intvalue = min;
  sgp->max.intvalue = max;
  sgp->a = 1.0;
  sgp->b = 0;
  sgp->values = (Pointer) bs;

  sintp = SeqIntNew ();
  sintp->from = 0;
  sintp->to = bsp->length - 1;
  sintp->id = SeqIdDup (bsp->id);
  ValNodeAddPointer (&(sgp->loc), SEQLOC_INT, (Pointer) sintp);

  return sgp;
}

static Boolean PhrapSequenceHasClipping (FILE *fp)

{
  Char     buf [256];
  Boolean  goOn = TRUE;
  Boolean  rsult = FALSE;
  CharPtr  str;

  if (fp == NULL) return FALSE;
  goOn = TRUE;
  while (goOn) {
    str = ReadALine (buf, sizeof (buf), fp);
    if (HasNoText (str)) {
      goOn = FALSE;
    } else {
      if (StringNCmp (str, "Clipping", 8) == 0) {
        rsult = TRUE;
      }
    }
  }
  return rsult;
}

static CharPtr BioseqGetLocalIdStr (BioseqPtr bsp)

{
  ObjectIdPtr  oip;
  SeqIdPtr     sip;

  if (bsp == NULL) return NULL;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_LOCAL) {
      oip = (ObjectIdPtr) sip->data.ptrvalue;
      if (oip != NULL && oip->str != NULL) {
        return oip->str;
      }
    }
  }
  return NULL;
}

static SeqAnnotPtr NewGraphSeqAnnot (CharPtr name, SeqGraphPtr sgp)

{
  SeqAnnotPtr  sap = NULL;

  if (sgp == NULL) return NULL;
  sap = SeqAnnotNew ();
  if (sap == NULL) return NULL;

  if (! HasNoText (name)) {
    ValNodeAddPointer (&(sap->desc), Annot_descr_name, StringSave (name));
  }
  sap->type = 3;
  sap->data = (Pointer) sgp;

  return sap;
}

static CharPtr taglist [] = {
  "", "DNA", "CO", "BaseQuality", "BQ", "Sequence", NULL
};

/* Phrap reading function based on sample code supplied by C. Magness */
NLM_EXTERN SeqEntryPtr ReadPhrapFile (FILE *fp)

{
  BioseqPtr    bsp;
  Char         buf [256];
  Boolean      goOn = TRUE;
  SeqEntryPtr  head = NULL;
  Int2         i;
  SeqEntryPtr  lastsep;
  SeqGraphPtr  lastsgp;
  CharPtr      p;
  CharPtr      q;
  SeqAnnotPtr  sap;
  SeqEntryPtr  sep = NULL;
  SeqGraphPtr  sgp;
  CharPtr      str;
  Int2         tag;

  if (fp == NULL) return NULL;
  goOn = TRUE;
  while (goOn) {
    str = ReadALine (buf, sizeof (buf), fp);
    if (str == NULL) {
      goOn = FALSE;
    } else if (! HasNoText (str)) {
      p = StringChr (str, ' ');
      if (p != NULL) {
        *p = '\0';
        p++;
      }
      tag = 0;
      for (i = 0; taglist [i] != NULL; i++) {
        if (StringCmp (str, taglist [i]) == 0) {
          tag = i;
        }
      }
      if (tag != 0) {
        if (p != NULL) {
          q = StringChr (p, ' ');
          if (q != NULL) {
            *q = '\0';
          }
        }
        switch (tag) {
          case 1 :
          case 2 :
            if (p != NULL) {
              sep = ReadPhrapDNA (fp, p);
              ValNodeLink (&head, sep);
            }
            /* for new format, sep points to current sequence */
            break;
          case 3 :
            if (p != NULL) {
              sep = head;
              while (sep != NULL && StringCmp (p, BioseqGetLocalIdStr ((BioseqPtr) sep->data.ptrvalue)) != 0) {
                sep = sep->next;
              }
            }
            /* and flow through to case 4 */
          case 4 :
            if (sep != NULL) {
              bsp = (BioseqPtr) sep->data.ptrvalue;
              sgp = ReadPhrapQuality (fp, bsp);
              if (sgp != NULL) {
                for (sap = bsp->annot; sap != NULL; sap = sap->next) {
                  if (sap->type == 3) {
                    for (lastsgp = sap->data; lastsgp->next != NULL; lastsgp = lastsgp->next) {
                      continue;
                    }
                    lastsgp->next = sgp;
                    break;
                  }
                }
                if (sap == NULL) {
                  if (bsp->annot != NULL) {
                    for (sap = bsp->annot; sap->next != NULL; sap = sap->next) {
                      continue;
                    }
                    sap->next = NewGraphSeqAnnot ("Graphs", sgp);
                  } else {
                    bsp->annot = NewGraphSeqAnnot ("Graphs", sgp);
                  }
                }
              }
            }
            break;
          case 5 :
            /* unlinkes and removes sep if Clipping line present */
            if (p != NULL) {
              if (PhrapSequenceHasClipping (fp)) {
                sep = head;
                lastsep = NULL;
                while (sep != NULL && StringCmp (p, BioseqGetLocalIdStr ((BioseqPtr) sep->data.ptrvalue)) != 0) {
                  lastsep = sep;
                  sep = sep->next;
                }
                if (sep != NULL) {
                  if (lastsep != NULL) {
                    lastsep->next = sep->next;
                    sep->next = NULL;
                    SeqEntryFree (sep);
                  } else {
                    head = sep->next;
                    sep->next = NULL;
                    SeqEntryFree (sep);
                  }
                }
              }
            }
            break;
          default :
            break;
        }
      }
    }
  }
  return head;
}

static ValNodePtr ParseContigOrFeatureTableString (CharPtr contigs, Boolean tabDelimited)

{
  Char        ch;
  Int2        i, j, k;
  CharPtr     str;
  Char        tmp [1024];
  ValNodePtr  vnp;

  vnp = NULL;
  i = 0;
  while (StringLen (contigs + i) > 0) {
    str = contigs + i;
    k = 0;
    ch = str [k];
    while (ch == ' ') {
      k++;
      ch = str [k];
    }
    j = 0;
    if (tabDelimited) {
      while (ch != '\0' && ch != '\t') {
        j++;
        ch = str [j + k];
      }
    } else {
      while (ch != '\0' && ch != ',' && (! (IS_WHITESP (ch)))) {
        j++;
        ch = str [j + k];
      }
    }
    if (ch == '\0') {
      i += j + k;
    } else {
      str [j + k] = '\0';
      i += j + k + 1;
    }
    StringNCpy_0 (tmp, str + k, sizeof (tmp));
    SqnTrimSpacesAroundString (tmp);
    if (HasNoText (tmp)) {
      ValNodeAdd (&vnp);
    } else {
      ValNodeCopyStr (&vnp, 0, tmp);
    }
  }
  if (vnp != NULL) {
    vnp->choice = (Uint1) ValNodeLen (vnp);
  }
  return vnp;
}

/* ReversePhrap coerces BioseqReverse to work on the SeqGraph byte store */

static void ReversePhrap (SeqGraphPtr sgp, Pointer userdata)

{
  ByteStorePtr  bs;
  Bioseq        bsq;

  if (sgp == NULL || sgp->values == NULL) return;
  if (StringICmp (sgp->title, "Phrap Quality") != 0) return;
  if (sgp->flags [1] != 0 || sgp->flags [2] != 3) return;

  bs = (ByteStorePtr) sgp->values;

  MemSet ((Pointer) &bsq, 0, sizeof (Bioseq));
  bsq.repr = Seq_repr_raw;
  bsq.mol = Seq_mol_na;
  bsq.length = BSLen (bs);
  bsq.seq_data_type = Seq_code_iupacna;
  bsq.seq_data = bs;

  BioseqReverse (&bsq);
}

NLM_EXTERN SeqEntryPtr SetPhrapContigOrder (SeqEntryPtr head, CharPtr contigs)

{
  BioseqPtr    bsp;
  Char         ch;
  CharPtr      id;
  size_t       len;
  Boolean      minus;
  SeqEntryPtr  sep, lastsep, nextsep, newhead;
  ValNodePtr   vnphead, vnp;

  if (head == NULL || contigs == NULL) return head;
  vnphead = ParseContigOrFeatureTableString (contigs, FALSE);
  if (vnphead == NULL) return head;
  newhead = NULL;
  for (vnp = vnphead; vnp != NULL; vnp = vnp->next) {
    sep = head;
    lastsep = NULL;
    id = (CharPtr) vnp->data.ptrvalue;
    len = StringLen (id);
    minus = FALSE;

    /* look for + or - after accession, indicating orientation */

    if (len > 1) {
      ch = id [len - 1];
      if (ch == '+') {
        id [len - 1] = '\0';
      } else if (ch == '-') {
        id [len - 1] = '\0';
        minus = TRUE;
      }
    }
    while (sep != NULL &&
           StringCmp (id, BioseqGetLocalIdStr ((BioseqPtr) sep->data.ptrvalue)) != 0) {
      lastsep = sep;
      sep = sep->next;
    }
    if (sep != NULL) {
      if (lastsep != NULL) {
        lastsep->next = sep->next;
        sep->next = NULL;
        ValNodeLink (&newhead, sep);
      } else {
        head = sep->next;
        sep->next = NULL;
        ValNodeLink (&newhead, sep);
      }

      /* if - orientation, reverse complement sequence */

      if (minus) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        if (bsp != NULL) {
          BioseqRevComp (bsp);

          /* and then reverse phrap scores */

          VisitGraphsOnBsp (bsp, NULL, ReversePhrap);
        }
      }
    }
  }
  for (sep = head; sep != NULL; sep = nextsep) {
    nextsep = sep->next;
    sep->next = NULL;
    SeqEntryFree (sep);
    sep = nextsep;
  }
  ValNodeFreeData (vnphead);
  return newhead;
}

/* ReadAsnFastaOrFlatFile section */

/* GetSeqId skips past LOCUS or ID, or starts past >, skips any white space, then
takes the next token as the seqID.  The return value points to the remaining
copied text, which for feature tables may contain a desired Seq-annot name. */

static CharPtr GetSeqId (CharPtr seqid, CharPtr str, size_t max, Boolean skiptag, Boolean trimwhite)

{
  Char     ch;
  CharPtr  ptr;

  if (seqid != NULL) {
    *seqid = '\0';
  }
  if (str == NULL || seqid == NULL) return FALSE;
  if (skiptag) {
    ch = *str;
    while (ch != '\0' && (! IS_WHITESP (ch))) {
      str++;
      ch = *str;
    }
  }
  ch = *str;
  while (IS_WHITESP (ch)) {
    str++;
    ch = *str;
  }
  StringNCpy_0 (seqid, str, max);
  str = seqid;
  while (ch != '\0' && (! IS_WHITESP (ch))) {
    str++;
    ch = *str;
  }
  if (ch == '\0') return NULL;
  *str = '\0';
  str++;
  ch = *str;
  while (ch != '\0' && (IS_WHITESP (ch))) {
    str++;
    ch = *str;
  }
  if (trimwhite) {
    ptr = str;
    while (ch != '\0' && (! IS_WHITESP (ch))) {
      ptr++;
      ch = *ptr;
    }
    *ptr = '\0';
  }
  return str;
}

/* Build contig section */
static void  AddNucToContig (CharPtr accnString, Int4 from, Int4 to,
                             Int4 size, Int2 strand, BioseqPtr segseq,
                             BoolPtr hasgaps, Boolean isgap)

{
  Boolean       allDigits;
  Char          ch;
  DbtagPtr      dp;
  CharPtr       ptr;
  SeqIntPtr     sintp;
  SeqIdPtr      sip;
  SeqLocPtr     slp;
  TextSeqIdPtr  tsip;
  long int      val;

  slp = ValNodeNew ((ValNodePtr) segseq->seq_ext);
  if (slp == NULL) return;
  if (segseq->seq_ext == NULL) {
    segseq->seq_ext = (Pointer) slp;
  }

  sintp = SeqIntNew ();
  sintp->from = from;
  sintp->to = to;
  sintp->strand = (Uint1) strand;

  slp->choice = SEQLOC_INT;
  slp->data.ptrvalue = (Pointer) sintp;

  if (isgap) {
    sip = ValNodeNew (NULL);
    /* sip = MakeUniqueSeqID ("gap_"); */
    dp = DbtagNew ();
    dp->db = StringSave ("SeqLit");
    dp->tag = ObjectIdNew ();
    dp->tag->id = 0;
    dp->tag->str = NULL;
    sip->choice = SEQID_GENERAL;
    sip->data.ptrvalue = dp;
    if (hasgaps != NULL) {
      *hasgaps = TRUE;
    }
  } else {
    allDigits = TRUE;
    ptr = accnString;
    ch = *ptr;
    while (ch != '\0' && allDigits) {
      if (! IS_DIGIT (ch)) {
        allDigits = FALSE;
      }
      ptr++;
      ch = *ptr;
    }
    if (allDigits && sscanf (accnString, "%ld", &val) == 1) {
      sip = ValNodeNew (NULL);
      sip->choice = (Uint1) SEQID_GI;
      sip->data.intvalue = val;
    } else {
      sip = SeqIdFromAccession (accnString, 0, NULL);
      if (sip == NULL) {
        sip = ValNodeNew (NULL);
        tsip = TextSeqIdNew ();
        tsip->accession = StringSave (accnString);
        sip->choice = (Uint1) SEQID_GENBANK;
        sip->data.ptrvalue = tsip;
      }
    }
  }

  sintp->id = sip;

  segseq->length += size;
}

#define accnString field [0]
#define startString field [1]
#define stopString field [2]
#define sizeString field [3]
#define strandString field [4]

static void AdjustContigValues (ValNodePtr line)

{
  Int2        i;
  ValNodePtr  nextAccn;
  ValNodePtr  nextStart;
  long int    num;
  ValNodePtr  thisStop;
  Char        tmp [32];
  Int4        val;

  if (line == NULL) return;
  for (i = 0, thisStop = line->data.ptrvalue; i < 2 && thisStop != NULL; i++, thisStop = thisStop->next) {
    continue;
  }
  line = line->next;
  while (line != NULL && line->data.ptrvalue == NULL) {
    line = line->next;
  }
  if (line == NULL) {
    if (thisStop != NULL) {
      if (sscanf ((CharPtr) thisStop->data.ptrvalue, "%ld", &num) == 1) {
        val = (Int4) num;
        val++;
        sprintf (tmp, "%ld", (long) val);
        thisStop->data.ptrvalue = MemFree (thisStop->data.ptrvalue);
        thisStop->data.ptrvalue = StringSave (tmp);
      }
    }
    return;
  }
  nextAccn = line->data.ptrvalue;
  if (nextAccn != NULL && StringICmp (nextAccn->data.ptrvalue, "gap") == 0) return;
  for (i = 0, nextStart = line->data.ptrvalue; i < 1 && nextStart != NULL; i++, nextStart = nextStart->next) {
    continue;
  }
  if (thisStop != NULL && nextStart != NULL) {
    thisStop->data.ptrvalue = MemFree (thisStop->data.ptrvalue);
    thisStop->data.ptrvalue = StringSave ((CharPtr) nextStart->data.ptrvalue);
  }
}

static void ProcessOneContigLine (ValNodePtr line, BioseqPtr segseq, Int4 lineNum,
                                  BoolPtr hasgaps, Boolean coordsOnMaster)

{
  Boolean     badNumber;
  CharPtr     field [5];
  Int2        i;
  Boolean     isgap;
  long int    num;
  Int4        size;
  Int4        start;
  Int4        stop;
  Int2        strand = Seq_strand_unknown;
  Int4        tmp;
  ValNodePtr  vnp;

  if (line == NULL || segseq == NULL) return;
  vnp = line->data.ptrvalue;
  if (vnp != NULL) {
    for (i = 0; i < 5; i++) {
      field [i] = NULL;
    }
    start = -1;
    stop = -1;
    size = -1;
    for (i = 0, vnp = line->data.ptrvalue; i < 5 && vnp != NULL; i++, vnp = vnp->next) {
      if (field [i] == NULL && (! HasNoText ((CharPtr) vnp->data.ptrvalue))) {
        field [i] = (CharPtr) vnp->data.ptrvalue;
      }
    }
  }

  if (HasNoText (accnString)) return;

  badNumber = FALSE;
  if (sizeString != NULL && sscanf (sizeString, "%ld", &num) == 1) {
    size = num;
  } else {
    size = -1;
  }
  if (startString != NULL && sscanf (startString, "%ld", &num) == 1) {
    start = num;
  } else {
    start = -1;
    badNumber = TRUE;
  }
  if (stopString != NULL && sscanf (stopString, "%ld", &num) == 1) {
    stop = num;
  } else {
    stop = -1;
    badNumber = TRUE;
  }
  if (start < 1 || stop < 1) {
    badNumber = TRUE;
  }
  isgap = FALSE;
  if (StringICmp (accnString, "gap") == 0) {
    if (size >= 0) {
      isgap = TRUE;
      badNumber = FALSE;
      start = 1;
      stop = size;
    }
  }

  if (badNumber) {
    if (startString == NULL) startString = "";
    if (stopString == NULL) stopString = "";
    if (start < 1 && stop < 1) {
      Message (MSG_POST, "Bad number in line %ld - start '%s', stop '%s'",
               (long) lineNum, startString, stopString);
    } else if (start < 1) {
      Message (MSG_POST, "Bad number in line %ld - start '%s'", (long) lineNum, startString);
    } else if (stop < 1) {
      Message (MSG_POST, "Bad number in line %ld - stop '%s'", (long) lineNum, stopString);
    } else {
      Message (MSG_POST, "Bad number in line %ld", (long) lineNum);
    }
    return;
  }

  if (isgap) {
    start = 0;
    stop = size - 1;
  } else {
    if (coordsOnMaster && start == stop) {
      Message (MSG_POST, "Ignoring accession %s", accnString);
      return;
    }

    start--;
    stop--;

    strand = Seq_strand_plus;
    if (strandString != NULL) {
      if (StringStr (strandString, "minus") ||
          StringChr (strandString, '-') ||
          StringStr (strandString, "complement")) {
        strand = Seq_strand_minus;
      }
    }
    if (start > stop) {
      tmp = start;
      start = stop;
      stop = tmp;
      strand = Seq_strand_minus;
    }
    if (strandString != NULL) {
      if (StringStr (strandString, "plus") || StringChr (strandString, '+')) {
        strand = Seq_strand_plus;
      }
    }

    if (coordsOnMaster) {
      stop -= (start + 1);
      start = 0;
    }

    size = ABS (stop - start) + 1;
  }

  AddNucToContig (accnString, start, stop, size, strand, segseq, hasgaps, isgap);
}

static void FreeFeatureTable (ValNodePtr head)

{
  ValNodePtr  vnp;

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    vnp->data.ptrvalue = ValNodeFreeData (vnp->data.ptrvalue);
  }
  ValNodeFreeData (head);
}

NLM_EXTERN SeqEntryPtr ReadContigListEx (FILE *fp, Boolean coordinatesOnMaster, CharPtr seqid, CharPtr title)

{
  BioseqPtr    bsp;
  DeltaSeqPtr  dsp;
  Boolean      hasgaps;
  ValNodePtr   head = NULL;
  Char         line [1023];
  Int4         lineNum;
  Int4         pos;
  SeqEntryPtr  sep;
  SeqIdPtr     sip = NULL;
  CharPtr      str;
  Char         tmp [128];
  ValNodePtr   vnp;

  if (fp == NULL) return NULL;

  pos = ftell (fp);
  str = ReadALine (line, sizeof (line), fp);
  if (str != NULL && StringNICmp (line, ">Contig", 7) == 0) {
    if (seqid == NULL && title == NULL) {
      title = GetSeqId (tmp, line, sizeof (tmp), TRUE, FALSE);
      seqid = tmp;
    }
    str = ReadALine (line, sizeof (line), fp);
  }
  while (str != NULL) {
    if (! HasNoText (line)) {
      if (StringNCmp (line, ">", 1) == 0 ||
          StringNCmp (line, "LOCUS ", 6) == 0 ||
          StringNCmp (line, "ID ", 3) == 0 ||
          StringNCmp (line, "//", 2) == 0 ||
          StringStr (line, "::=") != NULL) {
        fseek (fp, pos, SEEK_SET);
        break;
      }
      vnp = ParseContigOrFeatureTableString (line, TRUE);
      if (vnp != NULL) {
        ValNodeAddPointer (&head, 0, (Pointer) vnp);
      }
    }
    pos = ftell (fp);
    str = ReadALine (line, sizeof (line), fp);
  }
  if (head == NULL) return NULL;

  bsp = BioseqNew ();
  if (bsp == NULL) {
    FreeFeatureTable (head);
    return NULL;
  }
  bsp->mol = Seq_mol_dna;
  bsp->repr = Seq_repr_seg;
  bsp->seq_ext_type = 1;
  bsp->length = 0;
  if (! StringHasNoText (seqid)) {
    sip = SeqIdFindBest (MakeSeqID (seqid), 0);
  }
  if (sip == NULL) {
    sip = MakeUniqueSeqID ("contig_");
  }
  bsp->id = sip;

  if (! StringHasNoText (title)) {
    str = StringSaveNoNull (title);
    if (str != NULL) {
      SeqDescrAddPointer (&(bsp->descr), Seq_descr_title, (Pointer) str);
    }
  }

  if (coordinatesOnMaster) {
    for (vnp = head; vnp != NULL; vnp = vnp->next) {
      if (vnp->data.ptrvalue != NULL) {
        AdjustContigValues (vnp);
      }
    }
  }

  lineNum = 0;
  hasgaps = FALSE;
  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    lineNum++;
    if (vnp->data.ptrvalue != NULL) {
      ProcessOneContigLine (vnp, bsp, lineNum, &hasgaps, coordinatesOnMaster);
    }
  }

  FreeFeatureTable (head);

  if (bsp->seq_ext == NULL) return NULL;

  sep = SeqEntryNew ();
  if (sep == NULL) return NULL;
  sep->choice = 1;
  sep->data.ptrvalue = (Pointer) bsp;
  SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, sep);

  if (hasgaps) {
    dsp = GappedSeqLocsToDeltaSeqs (bsp->seq_ext);
    if (dsp != NULL) {
      bsp->seq_ext = SeqLocSetFree ((ValNodePtr) bsp->seq_ext);
      bsp->repr = Seq_repr_delta;
      bsp->seq_ext_type = 4;
      bsp->seq_ext = (Pointer) dsp;
    }
  }

  return sep;
}

NLM_EXTERN SeqEntryPtr ReadContigList (FILE *fp, Boolean coordinatesOnMaster)

{
  return ReadContigListEx (fp, coordinatesOnMaster, NULL, NULL);
}

/* PreCheckSeqForProteinType saves the current file position, then reads lines of
sequence, checking each character for letters that appear only in proteins.  It then
restores the file position, and returns true if it thinks it found a protein. */

static Boolean PreCheckSeqForProteinType (FILE *fp)

{
  Char     ch;
  Boolean  isProt = FALSE;
  Char     line [1023];
  CharPtr  p;
  Int4     pos;
  CharPtr  str;

  if (fp == NULL) return FALSE;

  pos = ftell (fp);
  str = ReadALine (line, sizeof (line), fp);
  while (str != NULL) {

    if (! HasNoText (line)) {

      if (StringNCmp (line, ">", 1) == 0 ||
          StringNCmp (line, "LOCUS ", 6) == 0 ||
          StringNCmp (line, "ID ", 3) == 0 ||
          StringNCmp (line, "//", 2) == 0 ||
          StringStr (line, "::=") != NULL) {
        fseek (fp, pos, SEEK_SET);
        return isProt;
      }

      p = line;
      ch = *p;
      while (ch != '\0') {
        if (! (IS_ALPHA (ch))) {
          p++;
        } else {
          ch = TO_UPPER (ch);
          if (StringChr ("EFILPQZ", ch) != NULL) {
            isProt = TRUE;
          }
          p++;
        }
        ch = *p;
      }

    }

    str = ReadALine (line, sizeof (line), fp);
  }

  fseek (fp, pos, SEEK_SET);
  return isProt;
}

/* ReadFlatFileDNA reads lines of sequence into a byte store.  Unless it is forced to be
treated as a nucleotide or a protein, it first calls PreCheckSeqForProteinType to look at
the sequence in advance, checking for protein-specific letters. If it encounters a non-
printing character, it completes the read but returns NULL. */

static ByteStorePtr ReadFlatFileDNA (FILE *fp, BoolPtr protPtr, Boolean forceNuc,
                                     Boolean forceProt, Boolean fastaAsSimpleSeq)

{
  Char           ch;
  ByteStorePtr   bs = NULL;
  Boolean        isProt = FALSE;
  Char           line [1023];
  CharPtr        p;
  CharPtr        q;
  Int4           pos;
  CharPtr        str;

  if (fp == NULL) return NULL;
  bs = BSNew (1000);
  if (bs == NULL) return NULL;

  if (forceNuc) {
    isProt = FALSE;
  } else if (forceProt) {
    isProt = TRUE;
  } else if (protPtr != NULL) {
    isProt = PreCheckSeqForProteinType (fp);
  }
  if (protPtr != NULL) {
    *protPtr = isProt;
  }

  pos = ftell (fp);
  str = ReadALine (line, sizeof (line), fp);
  while (str != NULL) {

    if (! HasNoText (line)) {

      if (StringNCmp (line, ">", 1) == 0 ||
          StringNCmp (line, "LOCUS ", 6) == 0 ||
          StringNCmp (line, "ID ", 3) == 0 ||
          StringStr (line, "::=") != NULL) {
        fseek (fp, pos, SEEK_SET);
        return bs;
      } else if (StringNCmp (line, "//", 2) == 0) {
        return bs;
      }

      p = line;
      q = line;
      ch = *p;
      while (ch != '\0') {
        if (IS_WHITESP (ch)) {
        } else if (! (IS_ALPHA (ch))) {
          if (isProt && (ch == '*' || ch == '-')) {
            *q = ch;
            q++;
          } else if (! IS_PRINT (ch)) {
            bs = BSFree (bs);
          }
        } else {
          if (! fastaAsSimpleSeq) {
            ch = TO_UPPER (ch);
          }
          if (! isProt) {
            if (ch == 'U') {
              ch = 'T';
            } else if (ch == 'u') {
              ch = 't';
            }
          } else {
            if (ch == 'X') {
              ch = 'N';
            } else if (ch == 'x') {
              ch = 'n';
            }
          }
          *q = ch;
          q++;
        }
        p++;
        ch = *p;
      }
      *q = '\0';
      if (bs != NULL) {
        BSWrite (bs, (VoidPtr) line, (Int4) StringLen (line));
      }

    }

    pos = ftell (fp);
    str = ReadALine (line, sizeof (line), fp);
  }

  if (bs != NULL && BSLen (bs) < 1) {
    bs = BSFree (bs);
  }
  return bs;
}

static SimpleSeqPtr ByteStoreToSimpleSeq (ByteStorePtr bs, CharPtr seqid, CharPtr title)

{
  SimpleSeqPtr  ssp;

  if (bs == NULL) return NULL;
  ssp = SimpleSeqNew ();
  if (ssp == NULL) return NULL;

  ssp->seq = bs;
  ssp->seqlen = BSLen (bs);

  if (! HasNoText (seqid)) {
    ssp->id [0] = StringSave (seqid);
    ssp->numid = 1;
  }
  if (! HasNoText (title)) {
    ssp->title = StringSave (title);
  }

  return ssp;
}

/* ReadFeatureTable reads lines of feature intervals and qualifiers into a Seq-annot. */

#define NUM_FTABLE_COLUMNS  6

#define START_TAG           0
#define STOP_TAG            1
#define FEAT_TYPE_TAG       2
#define QUAL_TYPE_TAG       3
#define QUAL_VAL_TAG        4
#define STRAND_TAG          5

#define startStr   field [START_TAG]
#define stopStr    field [STOP_TAG]
#define featType   field [FEAT_TYPE_TAG]
#define qualType   field [QUAL_TYPE_TAG]
#define qualVal    field [QUAL_VAL_TAG]
#define strandStr  field [STRAND_TAG]

static Boolean ParseFeatTableLine (CharPtr line, Int4Ptr startP, Int4Ptr stopP,
                                   BoolPtr partial5P, BoolPtr partial3P,
                                   CharPtr PNTR featP, CharPtr PNTR qualP,
                                   CharPtr PNTR valP, Int4 offset)

{
  Boolean     badNumber;
  CharPtr     field [NUM_FTABLE_COLUMNS];
  Int2        i;
  ValNodePtr  parsed;
  Boolean     partial5 = FALSE;
  Boolean     partial3 = FALSE;
  Int4        start;
  Int4        stop;
  CharPtr     str;
  Int4        tmp;
  long int    val;
  ValNodePtr  vnp;

  if (line == NULL || HasNoText (line)) return FALSE;
  if (*line == '[') return FALSE; /* offset and other instructions encoded in brackets */
  parsed = ParseContigOrFeatureTableString (line, TRUE);
  if (parsed == NULL) return FALSE;

  for (i = 0; i < NUM_FTABLE_COLUMNS; i++) {
    field [i] = NULL;
  }
  start = -1;
  stop = -1;
  vnp = parsed;
  for (i = 0; i < NUM_FTABLE_COLUMNS && vnp != NULL; i++) {
    if (field [i] == NULL) {
      if (! HasNoText ((CharPtr) vnp->data.ptrvalue)) {
        field [i] = (CharPtr) vnp->data.ptrvalue;
      }
    }
    vnp = vnp->next;
  }

  badNumber = FALSE;
  str = startStr;
  if (str != NULL && *str == '<') {
    partial5 = TRUE;
    str++;
  }
  if (str != NULL && sscanf (str, "%ld", &val) == 1) {
    start = val;
  } else {
    start = -1;
    badNumber = TRUE;
  }
  str = stopStr;
  if (str != NULL && *str == '>') {
    partial3 = TRUE;
    str++;
  }
  if (str != NULL && sscanf (str, "%ld", &val) == 1) {
    stop = val;
  } else {
    stop = -1;
    badNumber = TRUE;
  }

  if (badNumber) {
    start = -1;
    stop = -1;
  } else {
    start--;
    stop--;
    if (strandStr != NULL) {
      if (StringStr (strandStr, "minus") ||
          StringChr (strandStr, '-') ||
          StringStr (strandStr, "complement")) {
        if (start < stop) {
          tmp = start;
          start = stop;
          stop = tmp;
        }
      }
    }
  }

  *startP = start + offset;
  *stopP = stop + offset;
  *partial5P = partial5;
  *partial3P = partial3;
  *featP = featType;
  *qualP = qualType;
  *valP = qualVal;

  ValNodeFree (parsed);
  return TRUE;
}

static CharPtr aaList [] = {
  "-", "Gap", "Gap",        /* cannot be recognized because we split tRNA-xxx */
  "A", "Ala", "Alanine",
  "B", "Asx", "Asp or Asn",
  "C", "Cys", "Cysteine",
  "D", "Asp", "Aspartic Acid",
  "E", "Glu", "Glutamic Acid",
  "F", "Phe", "Phenylalanine",
  "G", "Gly", "Glycine",
  "H", "His", "Histidine",
  "I", "Ile", "Isoleucine",
  "K", "Lys", "Lysine",
  "L", "Leu", "Leucine",
  "M", "Met", "Methionine",
  "N", "Asn", "Asparagine",
  "P", "Pro", "Proline",
  "Q", "Gln", "Glutamine",
  "R", "Arg", "Arginine",
  "S", "Ser", "Serine",
  "T", "Thr", "Threonine",
  "V", "Val", "Valine",
  "W", "Trp", "Tryptophan",
  "X", "Xxx", "Undetermined or atypical",
  "Y", "Tyr", "Tyrosine",
  "Z", "Glx", "Glu or Gln",
  "U", "Sec", "Selenocysteine",
  "*", "Ter", "Termination",
  NULL, NULL, NULL
};

NLM_EXTERN Uint1 FindTrnaAA3 (CharPtr str)

{
  Uint1    aa;
  Int2     i;
  CharPtr  ptr;
  Char     tmp [128];

  if (HasNoText (str)) return 0;
  StringNCpy_0 (tmp, str, sizeof (tmp));
  SqnTrimSpacesAroundString (tmp);
  for (i = 0; aaList [i] != NULL; i += 3) {
    if (StringICmp (aaList [i + 1], tmp) == 0) {
      ptr = aaList [i];
      aa = (Uint1) ptr [0];
      return aa;
    }
  }
  if (StringICmp ("fMet", tmp) == 0) return (Uint1) 'M';
  if (StringICmp ("OTHER", tmp) == 0) return (Uint1) 'X';
  return 0;
}

NLM_EXTERN Uint1 FindTrnaAA (CharPtr str)

{
  Uint1    aa;
  Int2     i;
  Int2     j;
  CharPtr  ptr;
  Char     tmp [128];

  if (HasNoText (str)) return 0;
  StringNCpy_0 (tmp, str, sizeof (tmp));
  SqnTrimSpacesAroundString (tmp);
  for (i = 0; aaList [i] != NULL; i += 3) {
    for (j = 0; j < 3; j++) {
      if (StringICmp (aaList [i + j], tmp) == 0) {
        ptr = aaList [i];
        aa = (Uint1) ptr [0];
        return aa;
      }
    }
  }
  if (StringICmp ("fMet", tmp) == 0) return (Uint1) 'M';
  if (StringICmp ("OTHER", tmp) == 0) return (Uint1) 'X';
  return 0;
}

NLM_EXTERN CharPtr FindTrnaAAIndex (CharPtr str)

{
  Int2  i;
  Int2  j;
  Char  tmp [128];

  if (StringHasNoText (str)) return 0;
  StringNCpy_0 (tmp, str, sizeof (tmp));
  TrimSpacesAroundString (tmp);
  for (i = 0; aaList [i] != NULL; i += 3) {
    for (j = 0; j < 3; j++) {
      if (StringICmp (aaList [i + j], tmp) == 0) {
        return aaList [i + 1];
      }
    }
  }
  if (StringICmp ("fMet", tmp) == 0) return aaList [35];
  if (StringICmp ("OTHER", tmp) == 0) return aaList [52];
  return NULL;
}

NLM_EXTERN Uint1 ParseTRnaString (CharPtr strx, BoolPtr justTrnaText, Uint1Ptr cdP)

{
  Uint1       aa;
  Char        ch;
  Char        codon [16];
  Uint1       curraa;
  ValNodePtr  head;
  Int2        i;
  Boolean     justt = TRUE;
  CharPtr     str;
  tRNA        tr;
  ValNodePtr  vnp;

  if (justTrnaText != NULL) {
    *justTrnaText = FALSE;
  }
  if (cdP != NULL) {
    for (i = 0; i < 6; i++) { 
      cdP [i] = 255;
    }
  }
  if (StringHasNoText (strx)) return 0;

  MemSet ((Pointer) &tr, 0, sizeof (tRNA));
  for (i = 0; i < 6; i++) {
    tr.codon [i] = 255;
  }

  aa = 0;
  head = TokenizeTRnaString (strx);
  for (vnp = head; (aa == 0 || aa == 'A') && vnp != NULL; vnp = vnp->next) {
    str = (CharPtr) vnp->data.ptrvalue;
    curraa = FindTrnaAA (str);
    if (curraa != 0) {
      if (aa == 0) {
        aa = curraa;
      }
    } else if (StringICmp ("tRNA", str) != 0 &&
               StringICmp ("transfer", str) != 0 &&
               StringICmp ("RNA", str) != 0) {
      if (cdP != NULL && StringLen (str) == 3) {
        StringCpy (codon, str);
        for (i = 0; i < 3; i++) {
          if (codon [i] == 'U') {
            codon [i] = 'T';
          }
        }
        if (ParseDegenerateCodon (&tr, (Uint1Ptr) codon)) {
          for (i = 0; i < 6; i++) { 
            cdP [i] = tr.codon [i];
          }
        } else {
          justt = FALSE;
        }
      } else {
        justt = FALSE;
      }
    }
  }
  for (; vnp != NULL; vnp = vnp->next) {
    str = (CharPtr) vnp->data.ptrvalue;
    curraa = FindTrnaAA (str);
    if (curraa != 0) {
    } else if (StringICmp ("tRNA", str) != 0 &&
               StringICmp ("transfer", str) != 0 &&
               StringICmp ("RNA", str) != 0) {
      if (cdP != NULL && StringLen (str) == 3) {
        StringCpy (codon, str);
        for (i = 0; i < 3; i++) {
          if (codon [i] == 'U') {
            codon [i] = 'T';
          }
        }
        if (ParseDegenerateCodon (&tr, (Uint1Ptr) codon)) {
          for (i = 0; i < 6; i++) { 
            cdP [i] = tr.codon [i];
          }
        } else {
          justt = FALSE;
        }
      } else {
        justt = FALSE;
      }
    }
  }
  ValNodeFreeData (head);
  if (justt) {
    str = strx;
    ch = *str;
    while (ch != '\0') {
      if (IS_DIGIT (ch)) {
        justt = FALSE;
      }
      str++;
      ch = *str;
    }
  }
  if (justTrnaText != NULL) {
    *justTrnaText = justt;
  }
  return aa;
}

NLM_EXTERN ValNodePtr TokenizeTRnaString (CharPtr strx)

{
  Char        ch;
  ValNodePtr  head;
  Int2        i, j, k;
  size_t      len;
  CharPtr     ptr;
  Char        str [256];
  CharPtr     strs;
  Char        tmp [128];

  if (HasNoText (strx)) return NULL;
  strs = StringSave (strx);
  head = NULL;
  /* SGD Tx(NNN)c or Tx(NNN)c#, where x is the amino acid, c is the chromosome (A-P, Q for mito),
     and optional # is presumably for individual tRNAs with different anticodons and the same
     amino acid */
  len = StringLen (strs);
  if (len >= 8 && len <= 10) {
    if (strs [0] == 'T' || strs [0] == 't') {
      if (IS_ALPHA (strs [1]) && strs [2] == '('
          && strs [6] == ')' && IS_ALPHA (strs [7])) {
        if (len == 8 ||
            (len == 9 && IS_DIGIT (strs [8])) ||
            (len == 10 && IS_DIGIT (strs [8]) && IS_DIGIT (strs [9]))) {
          tmp [0] = '('; /* parse SGD tRNA anticodon */
          tmp [1] = strs [5]; /* reverse */
          tmp [2] = strs [4];
          tmp [3] = strs [3];
          tmp [4] = ')';
          tmp [5] = '\0';
          for (i = 1; i < 4; i++) {
            ch = tmp [i];
            ch = TO_UPPER (ch);
            switch (ch) {
              case 'A' :
                ch = 'T';
                break;
              case 'C' :
                ch = 'G';
                break;
              case 'G' :
                ch = 'C';
                break;
              case 'T' :
                ch = 'A';
                break;
              case 'U' :
                ch = 'A';
                break;
              default :
                ch = '?';
                break;
            }
            tmp [i] = ch; /* and complement */
          }
          ValNodeCopyStr (&head, 0, tmp);
          tmp [0] = strs [1]; /* parse SGD tRNA amino acid */
          tmp [1] = '\0';
          ValNodeCopyStr (&head, 0, tmp);
          MemFree (strs);
          return head;
        }
      }
    }
  }
  ptr = strs;
  ch = *ptr;
  while (ch != '\0') {
    if (ch == '*') {  /* keep possible terminator tRNA symbol */
    } else if (! IS_ALPHA (ch)) {
     *ptr = ' ';
    }
    ptr++;
    ch = *ptr;
  }
  i = 0;
  while (StringLen (strs + i) > 0) {
    StringNCpy_0 (str, strs + i, sizeof (str));
    k = 0;
    ch = str [k];
    while (ch == ' ') {
      k++;
      ch = str [k];
    }
    j = 0;
    while (ch != '\0' && ch != ' ') {
      j++;
      ch = str [j + k];
    }
    if (ch == ' ') {
      str [j + k] = '\0';
      i += j + k + 1;
    } else {
      i += j + k;
    }
    StringNCpy_0 (tmp, str + k, sizeof (tmp));
    if (StringNICmp (tmp, "tRNA", 4) == 0) {
      tmp [0] = ' ';
      tmp [1] = ' ';
      tmp [2] = ' ';
      tmp [3] = ' ';
    }
    SqnTrimSpacesAroundString (tmp);
    if (! HasNoText (tmp)) {
      ValNodeCopyStr (&head, 0, tmp);
    }
  }
  MemFree (strs);
  return head;
}

static CharPtr bondList [] = {
  "", "disulfide", "thiolester", "xlink", "thioether", NULL
};

static CharPtr siteList [] = {
  "", "active", "binding", "cleavage", "inhibit", "modified",
  "glycosylation", "myristoylation", "mutagenized", "metal binding",
  "phosphorylation", "acetylation", "amidation", "methylation",
  "hydroxylation", "sulfatation", "oxidative deamination",
  "pyrrolidone carboxylic acid", "gamma carboxyglutamic acid",
  "blocked", "lipid binding", "np binding", "DNA binding",
  "signal peptide", "transit peptide", "transmembrane region", NULL
};

static void StripHyphens (CharPtr str)

{
  Char     ch;
  CharPtr  ptr;

  if (str == NULL) return;
  ptr = str;
  ch = *ptr;
  while (ch != '\0') {
    if (ch == '-') {
      *ptr = ' ';
    }
    ptr++;
    ch = *ptr;
  }
}

static Uint1 ParseCodon (CharPtr str)

{
  Char   ch;
  Uint1  codon [4];
  Int2   i, j;

  if (str == NULL) return 255;
  for (i = 0, j = 1; i < 3; i++, j++) {
    ch = TO_UPPER (str [j]);
    codon [i] = (Uint1) ch;
  }
  codon [3] = 0;
  return IndexForCodon (codon, Seq_code_iupacna);
}

extern Boolean ParseCodeBreak (SeqFeatPtr sfp, CharPtr val);

static CharPtr orgRefList [] = {
  "", "organism", "mitochondrion", "div", "lineage", "gcode", "mgcode", NULL
};

static CharPtr orgModList [] = {
  "", "", "Strain", "Substrain", "Type", "Subtype", "Variety",
  "Serotype", "Serogroup", "Serovar", "Cultivar", "Pathovar",
  "Chemovar", "Biovar", "Biotype", "Group", "Subgroup", "Isolate",
  "Common", "Acronym", "Dosage", "Natural-host", "Sub-species",
  "Specimen-voucher", "Authority", "Forma", "Forma-specialis",
  "Ecotype", "Synonym", "Anamorph", "Teleomorph", "Breed",
  "Old Lineage", "Old Name", "Note", NULL
};

static CharPtr subSourceList [] = {
  "", "Chromosome", "Map", "Clone", "Subclone", "Haplotype",
  "Genotype", "Sex", "Cell-line", "Cell-type", "Tissue-type",
  "Clone-lib", "Dev-stage", "Frequency", "Germline", "Rearranged",
  "Lab-host", "Pop-variant", "Tissue-lib", "Plasmid-name", "Transposon-name",
  "Ins-seq-name", "Plastid-name", "Country", "Segment", "Endogenous-virus-name",
  "Transgenic", "Environmental-sample", "Isolation-source", NULL
};

static OrgNamePtr GetOrMakeOnp (OrgRefPtr orp)

{
  OrgNamePtr  onp;

  if (orp == NULL) return NULL;
  if (orp->orgname != NULL) return orp->orgname;
  onp = OrgNameNew ();
  orp->orgname = onp;
  return onp;
}

static Boolean ParseQualIntoBioSource (SeqFeatPtr sfp, CharPtr qual, CharPtr val)

{
  BioSourcePtr  biop;
  Int2          found, j;
  int           num;
  OrgModPtr     omp;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  SubSourcePtr  ssp;
  CharPtr       str;

  if (sfp == NULL || sfp->data.choice != SEQFEAT_BIOSRC) return FALSE;
  biop = (BioSourcePtr) sfp->data.value.ptrvalue;
  if (biop == NULL) return FALSE;
  orp = biop->org;
  if (orp == NULL) return FALSE;

  found = 0;
  for (j = 0, str = orgRefList [j]; str != NULL; j++, str = orgRefList [j]) {
    if (StringICmp (qual, str) == 0) {
      found = j;
    }
  }
  if (found > 0) {
    switch (found) {
      case 1 :
        orp->taxname = MemFree (orp->taxname);
        orp->taxname = StringSave (val);
        break;
      case 2 :
        biop->genome = GENOME_mitochondrion;
        break;
      case 3 :
        onp = GetOrMakeOnp (orp);
        if (onp == NULL) return FALSE;
        onp->div = MemFree (onp->div);
        onp->div = StringSave (val);
        break;
      case 4 :
        onp = GetOrMakeOnp (orp);
        if (onp == NULL) return FALSE;
        onp->lineage = MemFree (onp->lineage);
        onp->lineage = StringSave (val);
        break;
      case 5 :
        onp = GetOrMakeOnp (orp);
        if (onp == NULL) return FALSE;
        if (sscanf (val, "%d", &num) == 1) {
          onp->gcode = (Uint1) num;
        }
        break;
      case 6 :
        onp = GetOrMakeOnp (orp);
        if (onp == NULL) return FALSE;
        if (sscanf (val, "%d", &num) == 1) {
          onp->mgcode = (Uint1) num;
        }
        break;
      default :
        break;
    }
    return TRUE;
  }

  found = 0;
  for (j = 0, str = orgModList [j]; str != NULL; j++, str = orgModList [j]) {
    if (StringICmp (qual, str) == 0) {
      found = j;
    }
  }
  if (found > 0) {
    if (found == 32) {
      found = 253;
    } else if (found == 33) {
      found = 254;
    } else if (found == 34) {
      found = 255;
    }
    onp = GetOrMakeOnp (orp);
    if (onp == NULL) return FALSE;
    omp = OrgModNew ();
    if (omp == NULL) return FALSE;
    omp->subtype = (Uint1) found;
    omp->subname = StringSave (val);
    omp->next = onp->mod;
    onp->mod = omp;
    return TRUE;
  }

  found = 0;
  for (j = 0, str = subSourceList [j]; str != NULL; j++, str = subSourceList [j]) {
    if (StringICmp (qual, str) == 0) {
      found = j;
    }
  }
  if (found > 0) {
    ssp = SubSourceNew ();
    if (ssp == NULL) return FALSE;
    ssp->subtype = (Uint1) found;
    ssp->name = StringSave (val);
    ssp->next = biop->subtype;
    biop->subtype = ssp;
    return TRUE;
  }

  return FALSE;
}

static Boolean IS_real (CharPtr str)

{
  Char     ch;
  Boolean  nodigits = TRUE;
  Boolean  isinteger = TRUE;

  if (StringHasNoText (str)) return FALSE;
  ch = *str;
  while (ch != '\0') {
    if (ch == '+' || ch == '-' || ch == '.' || ch == 'E' || ch == 'e') {
      isinteger = FALSE;
    } else if (ch < '0' || ch > '9') {
      return FALSE;
    } else {
      nodigits = FALSE;
    }
    str++;
    ch = *str;
  }
  if (nodigits) return FALSE;
  if (isinteger) return FALSE;
  return TRUE;
}

static void AddFieldToSnpStsCloneUserObject (UserObjectPtr uop, CharPtr qual, CharPtr val)

{
  UserFieldPtr  curr;
  UserFieldPtr  prev = NULL;
  long int      num;
  ObjectIdPtr   oip;
  double        dbl;

  if (uop == NULL || StringHasNoText (qual) || StringHasNoText (val)) return;

  for (curr = uop->data; curr != NULL; curr = curr->next) {
    oip = curr->label;
    if (oip != NULL && StringICmp (oip->str, qual) == 0) {
      break;
    }
    prev = curr;
  }

  if (curr == NULL) {
    curr = UserFieldNew ();
    oip = ObjectIdNew ();
    oip->str = StringSave (qual);
    curr->label = oip;
    if (IS_real (val) && sscanf (val, "%lf", &dbl) == 1) {
      curr->choice = 3; /* real */
      curr->data.realvalue = (FloatHi) dbl;
    } else if (sscanf (val, "%ld", &num) == 1) {
      curr->choice = 2; /* integer */
      curr->data.intvalue = (Int4) num;
    } else {
      curr->choice = 1; /* visible string */
      curr->data.ptrvalue = StringSave (val);
    }

    /* link at end of list */

    if (prev != NULL) {
      prev->next = curr;
    } else {
      uop->data = curr;
    }
  }
}

static CharPtr snpQualList [] = {
  "", "snp_class", "weight", "chrcnt", "ctgcnt", "loccnt", "snp_het", "snp_het_se",
  "snp_maxrate", "snp_gtype", "snp_linkout", "snp_valid", NULL
};

static UserObjectPtr CreateSnpUserObject (void)

{
  ObjectIdPtr    oip;
  UserObjectPtr  uop;

  uop = UserObjectNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("dbSnpSynonymyData");
  uop->type = oip;

  return uop;
}

static UserObjectPtr GetSnpUserObject (SeqFeatPtr sfp)

{
  ObjectIdPtr    oip;
  UserObjectPtr  uop;

  if (sfp == NULL) return NULL;
  if (sfp->ext == NULL) {
    sfp->ext = CreateSnpUserObject ();
  }
  uop = sfp->ext;
  if (uop == NULL) return NULL;
  oip = uop->type;
  if (oip == NULL || StringICmp (oip->str, "dbSnpSynonymyData") != 0) return NULL;
  return uop;
}

static Boolean ParseQualIntoSnpUserObject (SeqFeatPtr sfp, CharPtr qual, CharPtr val)

{
  Int2           found, j;
  CharPtr        str;
  UserObjectPtr  uop;

  found = 0;
  for (j = 0, str = snpQualList [j]; str != NULL; j++, str = snpQualList [j]) {
    if (StringICmp (qual, str) == 0) {
      found = j;
    }
  }

  if (found > 0) {
    uop = GetSnpUserObject (sfp);
    if (uop == NULL) return FALSE;
    AddFieldToSnpStsCloneUserObject (uop, qual, val);
    return TRUE;
  }

  return FALSE;
}

static CharPtr stsQualList [] = {
  "", "sts_dsegs", "sts_aliases", "weight", NULL
};

static UserObjectPtr CreateStsUserObject (void)

{
  ObjectIdPtr    oip;
  UserObjectPtr  uop;

  uop = UserObjectNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("stsUserObject");
  uop->type = oip;

  return uop;
}

static UserObjectPtr GetStsUserObject (SeqFeatPtr sfp)

{
  ObjectIdPtr    oip;
  UserObjectPtr  uop;

  if (sfp == NULL) return NULL;
  if (sfp->ext == NULL) {
    sfp->ext = CreateStsUserObject ();
  }
  uop = sfp->ext;
  if (uop == NULL) return NULL;
  oip = uop->type;
  if (oip == NULL || StringICmp (oip->str, "stsUserObject") != 0) return NULL;
  return uop;
}

static Boolean ParseQualIntoStsUserObject (SeqFeatPtr sfp, CharPtr qual, CharPtr val)

{
  Int2           found, j;
  CharPtr        str;
  UserObjectPtr  uop;

  found = 0;
  for (j = 0, str = stsQualList [j]; str != NULL; j++, str = stsQualList [j]) {
    if (StringICmp (qual, str) == 0) {
      found = j;
    }
  }

  if (found > 0) {
    uop = GetStsUserObject (sfp);
    if (uop == NULL) return FALSE;
    AddFieldToSnpStsCloneUserObject (uop, qual, val);
    return TRUE;
  }
  return FALSE;
}

static CharPtr cloneQualList [] = {
  "", "clone_id", "method", "sequence", "bac-ends", "STS", "weight", NULL
};

static UserObjectPtr CreateCloneUserObject (void)

{
  ObjectIdPtr    oip;
  UserObjectPtr  uop;

  uop = UserObjectNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("cloneUserObject");
  uop->type = oip;

  return uop;
}

static UserObjectPtr GetCloneUserObject (SeqFeatPtr sfp)

{
  ObjectIdPtr    oip;
  UserObjectPtr  uop;

  if (sfp == NULL) return NULL;
  if (sfp->ext == NULL) {
    sfp->ext = CreateCloneUserObject ();
  }
  uop = sfp->ext;
  if (uop == NULL) return NULL;
  oip = uop->type;
  if (oip == NULL || StringICmp (oip->str, "cloneUserObject") != 0) return NULL;
  return uop;
}

static Boolean ParseQualIntoCloneUserObject (SeqFeatPtr sfp, CharPtr qual, CharPtr val)

{
  Int2           found, j;
  CharPtr        str;
  UserObjectPtr  uop;

  found = 0;
  for (j = 0, str = cloneQualList [j]; str != NULL; j++, str = cloneQualList [j]) {
    if (StringICmp (qual, str) == 0) {
      found = j;
    }
  }

  if (found > 0) {
    uop = GetCloneUserObject (sfp);
    if (uop == NULL) return FALSE;
    AddFieldToSnpStsCloneUserObject (uop, qual, val);
    return TRUE;
  }
  return FALSE;
}

/* gene ontology user object parsing */

static CharPtr goQualList [] = {
  "", "go_process", "go_component", "go_function", NULL
};

static CharPtr goQualType [] = {
  "", "Process", "Component", "Function", NULL
};

/* later will need to be able to deal with CombinedFeatureUserObjects */

static UserObjectPtr GetGeneOntologyUserObject (SeqFeatPtr sfp)

{
  ObjectIdPtr    oip;
  UserObjectPtr  uop;

  if (sfp == NULL) return NULL;
  if (sfp->ext == NULL) {
    sfp->ext = CreateGeneOntologyUserObject ();
  }
  uop = sfp->ext;
  if (uop == NULL) return NULL;
  oip = uop->type;
  if (oip == NULL || StringICmp (oip->str, "GeneOntology") != 0) return NULL;
  return uop;
}

static Boolean ParseQualIntoGeneOntologyUserObject (SeqFeatPtr sfp, CharPtr qual, CharPtr val)

{
  CharPtr        fields [4];
  Int2           found, j;
  Int4           goid = 0, pmid = 0;
  long int       num;
  CharPtr        str, ptr;
  UserObjectPtr  uop;

  found = 0;
  for (j = 0, str = goQualList [j]; str != NULL; j++, str = goQualList [j]) {
    if (StringICmp (qual, str) == 0) {
      found = j;
    }
  }

  if (found > 0) {
    uop = GetGeneOntologyUserObject (sfp);
    if (uop == NULL) return FALSE;
    str = StringSave (val);
    for (j = 0; j < 4; j++) {
      fields [j] = NULL;
    }
    ptr = str;
    for (j = 0; j < 4 && ptr != NULL; j++) {
      fields [j] = ptr;
      TrimSpacesAroundString (ptr);
      ptr = StringChr (ptr, '|');
      if (ptr != NULL) {
        *ptr = '\0';
        ptr++;
      }
    }
    if (fields [1] != NULL && sscanf (fields [1], "%ld", &num) == 1) {
      goid = (Int4) num;
    }
    if (fields [2] != NULL && sscanf (fields [2], "%ld", &num) == 1) {
      pmid = (Int4) num;
    }
    AddToGeneOntologyUserObject (uop, goQualType [found], fields [0], goid, pmid, fields [3]);
    MemFree (val);
    return TRUE;
  }
  return FALSE;
}

NLM_EXTERN void AddQualifierToFeature (SeqFeatPtr sfp, CharPtr qual, CharPtr val)

{
  Uint1           aa;
  Uint1           codon [6];
  CdRegionPtr     crp;
  DbtagPtr        db;
  GBQualPtr       gbq;
  GeneRefPtr      grp;
  ImpFeatPtr      ifp = NULL;
  Boolean         isGeneSyn = FALSE;
  Int2            j;
  Boolean         justTrnaText;
  GBQualPtr       last;
  size_t          len;
  int             num;
  ObjectIdPtr     oip;
  PubdescPtr      pdp;
  ProtRefPtr      prp = NULL;
  CharPtr         ptr;
  Int2            qnum;
  RnaRefPtr       rrp;
  CharPtr         str;
  CharPtr         tag;
  tRNAPtr         trna;
  long            uid;
  ValNodePtr      vnp;
  SeqFeatXrefPtr  xref;

  if (sfp == NULL || HasNoText (qual) ||
      (HasNoText (val) &&
       StringCmp (qual, "pseudo") != 0 &&
       StringCmp (qual, "mitochondrion") != 0)) return;
  qnum = GBQualNameValid (qual);
  if (qnum <= -1) {
    if (StringNCmp (qual, "gene_syn", 8) == 0) {
      qnum = GBQUAL_gene;
      isGeneSyn = TRUE;
    }
  }
  if (qnum <= -1) {
    if (sfp->data.choice == SEQFEAT_IMP) {
      ifp = (ImpFeatPtr) sfp->data.value.ptrvalue; /* for variation user object */
    }
    if (sfp->data.choice == SEQFEAT_REGION && StringCmp (qual, "region_name") == 0) {
      sfp->data.value.ptrvalue = MemFree (sfp->data.value.ptrvalue);
      sfp->data.value.ptrvalue = StringSave (val);
    } else if (sfp->data.choice == SEQFEAT_BOND && StringCmp (qual, "bond_type") == 0) {
      StripHyphens (val);
      sfp->data.value.intvalue = 255;
      for (j = 0; bondList [j] != NULL; j++) {
        if (StringNICmp (val, bondList [j], StringLen (bondList [j])) == 0) {
          sfp->data.value.intvalue = j;
        }
      }
    } else if (sfp->data.choice == SEQFEAT_SITE && StringCmp (qual, "site_type") == 0) {
      StripHyphens (val);
      sfp->data.value.intvalue = 255;
      for (j = 0; siteList [j] != NULL; j++) {
        if (StringNICmp (val, siteList [j], StringLen (siteList [j])) == 0) {
          sfp->data.value.intvalue = j;
        }
      }
    } else if (sfp->data.choice == SEQFEAT_PUB && (StringICmp (qual, "pmid") == 0 || StringICmp (qual, "PubMed") == 0)) {
      if (sscanf (val, "%ld", &uid) == 1) {
        pdp = (PubdescPtr) sfp->data.value.ptrvalue;
        if (pdp != NULL) {
          ValNodeAddInt (&(pdp->pub), PUB_PMid, (Int4) uid);
        }
      }
    } else if (sfp->data.choice == SEQFEAT_PUB && (StringICmp (qual, "muid") == 0 || StringICmp (qual, "MEDLINE") == 0)) {
      if (sscanf (val, "%ld", &uid) == 1) {
        pdp = (PubdescPtr) sfp->data.value.ptrvalue;
        if (pdp != NULL) {
          ValNodeAddInt (&(pdp->pub), PUB_Muid, (Int4) uid);
        }
      }
    } else if (sfp->data.choice == SEQFEAT_BIOSRC && ParseQualIntoBioSource (sfp, qual, val)) {
    } else if (sfp->data.choice == SEQFEAT_CDREGION && StringCmp (qual, "prot_desc") == 0) {
      xref = sfp->xref;
      while (xref != NULL && xref->data.choice != SEQFEAT_PROT) {
        xref = xref->next;
      }
      if (xref == NULL) {
        prp = ProtRefNew ();
        xref = SeqFeatXrefNew ();
        if (xref != NULL) {
          xref->data.choice = SEQFEAT_PROT;
          xref->data.value.ptrvalue = (Pointer) prp;
          xref->next = sfp->xref;
          sfp->xref = xref;
        }
      }
      if (xref != NULL) {
        prp = (ProtRefPtr) xref->data.value.ptrvalue;
      }
      if (prp == NULL) return;
      prp->desc = MemFree (prp->desc);
      prp->desc = StringSaveNoNull (val);
    } else if (ifp != NULL && StringICmp (ifp->key, "variation") == 0 && ParseQualIntoSnpUserObject (sfp, qual, val)) {
    } else if (ifp != NULL && StringICmp (ifp->key, "STS") == 0 && ParseQualIntoStsUserObject (sfp, qual, val)) {
    } else if (ifp != NULL && StringICmp (ifp->key, "misc_feature") == 0 && ParseQualIntoCloneUserObject (sfp, qual, val)) {
    } else if ((sfp->data.choice == SEQFEAT_GENE ||
                sfp->data.choice == SEQFEAT_CDREGION ||
                sfp->data.choice == SEQFEAT_RNA) &&
               ParseQualIntoGeneOntologyUserObject (sfp, qual, val)) {
    } else {
      ErrPostEx (SEV_ERROR, ERR_SEQ_FEAT_UnknownImpFeatQual, "Unknown qualifier %s", qual);
    }
    return;
  }
  if (qnum == GBQUAL_note) {
    if (sfp->comment == NULL) {
      sfp->comment = StringSave (val);
    } else {
      len = StringLen (sfp->comment) + StringLen (val) + 5;
      str = MemNew (sizeof (Char) * len);
      StringCpy (str, sfp->comment);
      StringCat (str, "; ");
      StringCat (str, val);
      sfp->comment = MemFree (sfp->comment);
      sfp->comment = str;
    }
    return;
  } else if (qnum == GBQUAL_gene && sfp->data.choice != SEQFEAT_GENE) {
    if (StringCmp (val, "-") == 0) {
      val = NULL;
    }
    xref = sfp->xref;
    while (xref != NULL && xref->data.choice != SEQFEAT_GENE) {
      xref = xref->next;
    }
    if (xref == NULL) {
      grp = GeneRefNew ();
      xref = SeqFeatXrefNew ();
      if (xref != NULL) {
        xref->data.choice = SEQFEAT_GENE;
        xref->data.value.ptrvalue = (Pointer) grp;
        xref->next = sfp->xref;
        sfp->xref = xref;
      }
    }
    if (xref != NULL) {
      grp = (GeneRefPtr) xref->data.value.ptrvalue;
      if (grp == NULL) return;
      if (isGeneSyn) {
        ValNodeCopyStr (&(grp->syn), 0, val);
      } else if (grp->locus == NULL) {
        grp->locus = StringSave (val);
      } else {
        ValNodeCopyStr (&(grp->syn), 0, val);
      }
    }
    return;
  } else if (qnum == GBQUAL_db_xref) {
    vnp = ValNodeNew (NULL);
    db = DbtagNew ();
    vnp->data.ptrvalue = db;
    tag = val;
    ptr = StringChr (tag, ':');
    if (ptr != NULL) {
      *ptr = '\0';
      ptr++;
      db->db = StringSave (tag);
      oip = ObjectIdNew ();
      oip->str = StringSave (ptr);
      db->tag = oip;
    } else {
      db->db = StringSave ("?");
      oip = ObjectIdNew ();
      oip->str = StringSave (tag);
      db->tag = oip;
    }
    vnp->next = sfp->dbxref;
    sfp->dbxref = vnp;
    return;
  } else if (qnum == GBQUAL_replace && StringCmp (val, "-") == 0) {
    val = "";
  }

  if (sfp->data.choice == SEQFEAT_GENE) {
    if (qnum == GBQUAL_gene) {
      grp = (GeneRefPtr) sfp->data.value.ptrvalue;
      if (grp != NULL) {
        if (isGeneSyn) {
          ValNodeCopyStr (&(grp->syn), 0, val);
        } else if (grp->locus == NULL) {
          grp->locus = StringSave (val);
        } else {
          ValNodeCopyStr (&(grp->syn), 0, val);
        }
      }
    } else if (qnum == GBQUAL_pseudo) {
      grp = (GeneRefPtr) sfp->data.value.ptrvalue;
      if (grp != NULL) {
        grp->pseudo = TRUE;
      }
    } else if (qnum == GBQUAL_allele) {
      grp = (GeneRefPtr) sfp->data.value.ptrvalue;
      if (grp != NULL) {
        grp->allele = StringSave (val);
      }
    }
    return;
  } else if (sfp->data.choice == SEQFEAT_CDREGION) {
    if (qnum == GBQUAL_function || qnum == GBQUAL_EC_number || qnum == GBQUAL_product) {
      xref = sfp->xref;
      while (xref != NULL && xref->data.choice != SEQFEAT_PROT) {
        xref = xref->next;
      }
      if (xref == NULL) {
        prp = ProtRefNew ();
        xref = SeqFeatXrefNew ();
        if (xref != NULL) {
          xref->data.choice = SEQFEAT_PROT;
          xref->data.value.ptrvalue = (Pointer) prp;
          xref->next = sfp->xref;
          sfp->xref = xref;
        }
      }
      if (xref != NULL) {
        prp = (ProtRefPtr) xref->data.value.ptrvalue;
      }
      if (prp == NULL) return;
      if (qnum == GBQUAL_function) {
        ValNodeCopyStr (&(prp->activity), 0, val);
      } else if (qnum == GBQUAL_EC_number) {
        ValNodeCopyStr (&(prp->ec), 0, val);
      } else if (qnum == GBQUAL_product) {
        ValNodeCopyStr (&(prp->name), 0, val);
      }
      return;
    } else if (qnum == GBQUAL_transl_except) {
      if (ParseCodeBreak (sfp, val)) return;
    } else if (qnum == GBQUAL_codon_start) {
      crp = (CdRegionPtr) sfp->data.value.ptrvalue;
      if (sscanf (val, "%d", &num) == 1 && crp != NULL) {
        if (num > 0 || num < 4) {
          crp->frame = (Uint1) num;
        }
      }
    }
  } else if (sfp->data.choice == SEQFEAT_RNA) {
    if (qnum == GBQUAL_product) {
      rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
      if (rrp == NULL) return;
      if (rrp->type == 3) {
        rrp->ext.choice = 2;
        trna = (tRNAPtr) MemNew (sizeof (tRNA));
        rrp->ext.value.ptrvalue = (Pointer) trna;
        if (trna != NULL) {
          trna->aatype = 2;
          for (j = 0; j < 6; j++) {
            trna->codon [j] = 255;
          }
          aa = ParseTRnaString (val, &justTrnaText, codon);
          if (aa != 0) {
            if (justTrnaText) {
              for (j = 0; j < 6; j++) {
                trna->codon [j] = codon [j];
              }
            }
            trna->aa = aa;
            if (aa == 'M') {
              if (StringStr (val, "fMet") != NULL) {
                if (sfp->comment == NULL) {
                  sfp->comment = StringSave ("fMet");
                } else {
                  len = StringLen (sfp->comment) + StringLen ("fMet") + 5;
                  str = MemNew (sizeof (Char) * len);
                  StringCpy (str, sfp->comment);
                  StringCat (str, "; ");
                  StringCat (str, "fMet");
                  sfp->comment = MemFree (sfp->comment);
                  sfp->comment = str;
                }
              }
            }
          }
        }
      } else {
        if (rrp->ext.choice == 1) {
          rrp->ext.value.ptrvalue = MemFree (rrp->ext.value.ptrvalue);
        }
        rrp->ext.choice = 1;
        rrp->ext.value.ptrvalue = StringSave (val);
      }
      return;
    }
  } else if (sfp->data.choice == SEQFEAT_BIOSRC) {
    if (ParseQualIntoBioSource (sfp, qual, val)) return;
  }

  gbq = GBQualNew ();
  if (gbq == NULL) return;
  gbq->qual = StringSave (qual);
  gbq->val = StringSave (val);
  if (sfp->qual == NULL) {
    sfp->qual = gbq;
  } else {
    last = sfp->qual;
    while (last->next != NULL) {
      last = last->next;
    }
    last->next = gbq;
  }
}

NLM_EXTERN SeqLocPtr AddIntervalToLocation (SeqLocPtr loc, SeqIdPtr sip,
                                            Int4 start, Int4 stop,
                                            Boolean partial5, Boolean partial3)

{
  Int4        flip;
  IntFuzzPtr  ifp;
  SeqLocPtr   rsult = NULL;
  SeqIntPtr   sintp;
  SeqLocPtr   slp;
  Int2        strand;
  SeqLocPtr   tmp;

  if (sip == NULL) return NULL;

  sintp = SeqIntNew ();
  strand = Seq_strand_plus;
  if (start > stop) {
    flip = start;
    start = stop;
    stop = flip;
    strand = Seq_strand_minus;
  }
  sintp->from = start;
  sintp->to = stop;
  sintp->strand = strand;
  sintp->id = SeqIdDup (sip);

  if (partial5) {
    ifp = IntFuzzNew ();
    if (ifp != NULL) {
      ifp->choice = 4;
      if (strand == Seq_strand_minus || strand == Seq_strand_both_rev) {
        sintp->if_to = ifp;
        ifp->a = 1;
      } else {
        sintp->if_from = ifp;
        ifp->a = 2;
      }
    }
  }

  if (partial3) {
    ifp = IntFuzzNew ();
    if (ifp != NULL) {
      ifp->choice = 4;
      if (strand == Seq_strand_minus || strand == Seq_strand_both_rev) {
        sintp->if_from = ifp;
        ifp->a = 2;
      } else {
        sintp->if_to = ifp;
        ifp->a = 1;
      }
    }
  }

  slp = ValNodeAddPointer (NULL, SEQLOC_INT, (Pointer) sintp);

  if (loc == NULL) return slp;

  if (loc->choice == SEQLOC_MIX) {
    tmp = (ValNodePtr) (loc->data.ptrvalue);
    while (tmp->next != NULL) {
      tmp = tmp->next;
    }
    tmp->next = slp;
    rsult = loc;
  } else {
    tmp = ValNodeNew (NULL);
    tmp->choice = SEQLOC_MIX;
    tmp->data.ptrvalue = (Pointer) loc;
    loc->next = slp;
    rsult = tmp;
  }

  if (SeqLocStrand (rsult) == Seq_strand_other) {
    slp = SeqLocFindNext (rsult, NULL);
    while (slp != NULL) {
      if (slp->choice == SEQLOC_INT) {
        sintp = (SeqIntPtr) slp->data.ptrvalue;
        if (sintp != NULL) {
          /* exon of one base in feature */
          if (sintp->from == sintp->to) {
            sintp->strand = Seq_strand_minus;
          }
        }
      }
      slp = SeqLocFindNext (rsult, slp);
    }
  }

  return rsult;
}

static SeqAnnotPtr ReadFeatureTable (FILE *fp, CharPtr seqid, CharPtr annotname)

{
  BioSourcePtr   biop;
  CdRegionPtr    crp;
  AnnotDescrPtr  desc;
  CharPtr        feat;
  GeneRefPtr     grp;
  Int2           idx;
  ImpFeatPtr     ifp;
  Int2           j;
  CharPtr        label;
  Char           line [2047];
  CharPtr        loc;
  Boolean        nonewline;
  long int       num;
  Int4           offset = 0;
  OrgRefPtr      orp;
  Boolean        partial5;
  Boolean        partial3;
  PubdescPtr     pdp;
  Int4           pos;
  SeqFeatPtr     prev;
  CharPtr        qual;
  Uint1          rnatype;
  RnaRefPtr      rrp;
  SeqAnnotPtr    sap = NULL;
  SeqFeatPtr     sfp = NULL;
  SeqIdPtr       sip;
  Int4           start;
  Int4           stop;
  SqnTagPtr      stp;
  CharPtr        str;
  CharPtr        tmp;
  CharPtr        val;

  if (fp == NULL || seqid == NULL) return NULL;
  sip = SeqIdFindBest (MakeSeqID (seqid), 0);
  if (sip == NULL) return NULL;

  pos = ftell (fp);
  str = ReadALineOfScores (line, sizeof (line), fp, &nonewline);
  while (str != NULL) {

    if (! HasNoText (line)) {

      if (StringNCmp (line, ">", 1) == 0 ||
          StringNCmp (line, "LOCUS ", 6) == 0 ||
          StringNCmp (line, "ID ", 3) == 0 ||
          StringStr (line, "::=") != NULL) {
        fseek (fp, pos, SEEK_SET);
        SeqIdFree (sip);
        return sap;
      } else if (StringNCmp (line, "//", 2) == 0) {
        SeqIdFree (sip);
        return sap;
      }

      if (ParseFeatTableLine (line, &start, &stop, &partial5, &partial3, &feat, &qual, &val, offset)) {
        if (feat != NULL && start >= 0 && stop >= 0) {

          if (sap == NULL) {
            sap = SeqAnnotNew ();
            if (sap != NULL) {
              sap->type = 1;
              if (! HasNoText (annotname)) {
                desc = ValNodeNew (NULL);
                if (desc != NULL) {
                  desc->choice = Annot_descr_name;
                  desc->data.ptrvalue = StringSave (annotname);
                  sap->desc = desc;
                }
              }
            }
          }

          sfp = SeqFeatNew ();
          if (sfp != NULL && sap != NULL) {
            if (sap->data != NULL) {
              prev = sap->data;
              while (prev->next != NULL) {
                prev = prev->next;
              }
              prev->next = sfp;
            } else {
              sap->data = (Pointer) sfp;
            }

            if (StringCmp (feat, "gene") == 0) {

              sfp->data.choice = SEQFEAT_GENE;
              grp = GeneRefNew ();
              if (grp != NULL) {
                sfp->data.value.ptrvalue = (Pointer) grp;
              }

            } else if (StringCmp (feat, "CDS") == 0) {

              sfp->data.choice = SEQFEAT_CDREGION;
              crp = CreateNewCdRgn (1, FALSE, 0);
              if (crp != NULL) {
                sfp->data.value.ptrvalue = (Pointer) crp;
              }

            } else if (StringStr (feat, "RNA") != NULL) {

              sfp->data.choice = SEQFEAT_RNA;
              rrp = RnaRefNew ();
              if (rrp != NULL) {
                sfp->data.value.ptrvalue = (Pointer) rrp;
                rnatype = 255;
                if (StringCmp (feat, "precursor_RNA") == 0) {
                  rnatype = 1;
                } else if (StringCmp (feat, "mRNA") == 0) {
                  rnatype = 2;
                } else if (StringCmp (feat, "tRNA") == 0) {
                  rnatype = 3;
                } else if (StringCmp (feat, "rRNA") == 0) {
                  rnatype = 4;
                } else if (StringCmp (feat, "snRNA") == 0) {
                  rnatype = 5;
                } else if (StringCmp (feat, "scRNA") == 0) {
                  rnatype = 6;
                } else if (StringCmp (feat, "snoRNA") == 0) {
                  rnatype = 7;
                } else if (StringCmp (feat, "misc_RNA") == 0) {
                  rnatype = 255;
                }
                rrp->type = rnatype;
              }

            } else if (StringCmp (feat, "source") == 0) {

              sfp->data.choice = SEQFEAT_BIOSRC;
              biop = BioSourceNew ();
              if (biop != NULL) {
                orp = OrgRefNew ();
                biop->org = orp;
                sfp->data.value.ptrvalue = (Pointer) biop;
              }

            } else if (StringCmp (feat, "Region") == 0) {

              sfp->data.choice = SEQFEAT_REGION;
              sfp->data.value.ptrvalue = StringSave ("?");

            } else if (StringCmp (feat, "Bond") == 0) {

              sfp->data.choice = SEQFEAT_BOND;
              sfp->data.value.intvalue = 255;

            } else if (StringCmp (feat, "Site") == 0) {

              sfp->data.choice = SEQFEAT_SITE;
              sfp->data.value.intvalue = 255;

            } else if (StringICmp (feat, "REFERENCE") == 0) {

              sfp->data.choice = SEQFEAT_PUB;
              pdp = PubdescNew ();
              if (pdp != NULL) {
                sfp->data.value.ptrvalue = (Pointer) pdp;
              }

            } else {
              sfp->data.choice = SEQFEAT_IMP;
              ifp = ImpFeatNew ();
              if (ifp != NULL) {
                ifp->key = StringSave (feat);
                sfp->data.value.ptrvalue = (Pointer) ifp;
              }

              idx = -1;
              for (j = 0; j < ParFlat_TOTAL_GBFEAT; j++) {
                if (StringCmp (ParFlat_GBFeat [j].key, feat) == 0) {
                  idx = j;
                }
              }
              if (idx == -1) {
                ErrPostEx (SEV_ERROR, ERR_SEQ_FEAT_UnknownImpFeatKey, "Unknown feature %s", feat);
              }
            }

            sfp->location = AddIntervalToLocation (NULL, sip, start, stop, partial5, partial3);

            if (partial5 || partial3) {
              sfp->partial = TRUE;
            }
          }

        } else if (start >= 0 && stop >= 0 && feat == NULL && qual == NULL && val == NULL && sfp != NULL) {

          sfp->location = AddIntervalToLocation (sfp->location, sip, start, stop, partial5, partial3);

          if (partial5 || partial3) {
            sfp->partial = TRUE;
          }

        } else if (sfp != NULL && qual != NULL &&
                   (val != NULL ||
                    StringCmp (qual, "pseudo") == 0 ||
                    StringCmp (qual, "mitochondrion") == 0)) {

          AddQualifierToFeature (sfp, qual, val);

        } else if (sfp != NULL && qual != NULL && val == NULL) {

          label = (CharPtr) FeatDefTypeLabel (sfp);
          if (label == NULL) {
            label = "?";
          }
          loc = SeqLocPrint (sfp->location);
          if (loc == NULL) {
            loc = StringSave ("?");
          }
          ErrPostEx (SEV_ERROR, ERR_SEQ_FEAT_WrongQualOnImpFeat, "Qualifier '%s' has no value on %s feature at %s", qual, label, loc);
          MemFree (loc);
        }

      } else if (*line == '[') {

        stp = SqnTagParse (line);
        if (stp != NULL) {
          tmp = SqnTagFind (stp, "offset");
          if (tmp != NULL) {
            if (sscanf (tmp, "%ld", &num) == 1) {
              offset = (Int4) num;
            }
          }
        }
        SqnTagFree (stp);
      }

/* Uncomment next lines if Purify reports memory leaks here. */

      /*
      MemFree (feat);
      MemFree (qual);
      MemFree (val);
      */

    }

    /* if humongously long line (e.g., /note), truncate by ignoring */

    while (nonewline && str != NULL) {
      str = ReadALineOfScores (line, sizeof (line), fp, &nonewline);
    }

    pos = ftell (fp);
    str = ReadALineOfScores (line, sizeof (line), fp, &nonewline);
  }

  SeqIdFree (sip);
  return sap;
}

/* ReadVecScreenTable reads lines of vector screen output into a Seq-annot. */

static SeqAnnotPtr ReadVecScreenTable (FILE *fp, CharPtr seqid, CharPtr annotname)

{
  Char            ch;
  CharPtr         database = NULL;
  Char            date [32];
  DatePtr         dp;
  AnnotDescrPtr   desc;
  GeneRefPtr      grp;
  ImpFeatPtr      ifp;
  Char            line [1023];
  Char            matchtype [64];
  Char            note [128];
  Int4            pos;
  SeqFeatPtr      prev;
  CharPtr         ptr;
  SeqAnnotPtr     sap = NULL;
  CharPtr         screen = NULL;
  SeqFeatPtr      sfp = NULL;
  SeqIdPtr        sip;
  long int        start;
  long int        stop;
  CharPtr         str;
  SeqFeatXrefPtr  xref;

  if (fp == NULL || seqid == NULL) return NULL;
  sip = SeqIdFindBest (MakeSeqID (seqid), 0);
  if (sip == NULL) return NULL;
  matchtype [0] = '\0';

  date [0] = '\0';
  dp = DateCurr ();
  DatePrint (dp, date);
  DateFree (dp);

  ptr = StringStr (annotname, "Database:");
  if (ptr != NULL) {
    ptr += 9;
    ch = *ptr;
    while (ch == ' ') {
      ptr++;
      ch = *ptr;
    }
    database = ptr;
  }

  ptr = StringStr (annotname, "Screen:");
  if (ptr != NULL) {
    ptr += 7;
    ch = *ptr;
    while (ch == ' ') {
      ptr++;
      ch = *ptr;
    }
    screen = ptr;
    while (ch != '\0' && ch != ' ') {
      ptr++;
      ch = *ptr;
    }
    *ptr = '\0';
  }

  pos = ftell (fp);
  str = ReadALine (line, sizeof (line), fp);
  while (str != NULL) {

    if (! HasNoText (line)) {

      if (StringNCmp (line, ">", 1) == 0 ||
          StringNCmp (line, "LOCUS ", 6) == 0 ||
          StringNCmp (line, "ID ", 3) == 0 ||
          StringStr (line, "::=") != NULL) {
        fseek (fp, pos, SEEK_SET);
        SeqIdFree (sip);
        return sap;
      } else if (StringNCmp (line, "//", 2) == 0) {
        SeqIdFree (sip);
        return sap;
      }

      if (sscanf (line, "%ld\t%ld", &start, &stop) == 2) {
        start--;
        stop--;

        if (start >= 0 && stop >= 0) {
          if (! HasNoText (matchtype)) {

            if (sap == NULL) {
              sap = SeqAnnotNew ();
              if (sap != NULL) {
                sap->type = 1;
                if (! HasNoText (annotname)) {
                  desc = ValNodeNew (NULL);
                  if (desc != NULL) {
                    desc->choice = Annot_descr_name;
                    desc->data.ptrvalue = StringSave ("VecScreen");
                    sap->desc = desc;
                  }
                }
              }
            }

            if (sfp == NULL) {
              sfp = SeqFeatNew ();
              if (sfp != NULL) {

                /* make misc_feature for now */

                sfp->data.choice = SEQFEAT_IMP;
                ifp = ImpFeatNew ();
                if (ifp != NULL) {
                  ifp->key = StringSave ("misc_feature");
                }
                AddQualifierToFeature (sfp, "standard_name", "Vector Contamination");
                AddQualifierToFeature (sfp, "phenotype", matchtype);

                if ((! StringHasNoText (database)) && (! StringHasNoText (screen))) {
                  sprintf (note, "Screened against %s using %s on %s", database, screen, date);
                  sfp->comment = StringSave (note);
                }

                /* suppress /gene */

                grp = GeneRefNew ();
                if (grp != NULL) {
                  xref = SeqFeatXrefNew ();
                  sfp->xref = xref;
                  if (xref != NULL) {
                    xref->data.choice = SEQFEAT_GENE;
                    xref->data.value.ptrvalue = (Pointer) grp;
                  }
                }

                sfp->data.value.ptrvalue = (Pointer) ifp;

                if (sap != NULL) {
                  if (sap->data != NULL) {
                    prev = sap->data;
                    while (prev->next != NULL) {
                      prev = prev->next;
                    }
                    prev->next = sfp;
                  } else {
                    sap->data = (Pointer) sfp;
                  }
                }

                sfp->location = AddIntervalToLocation (NULL, sip, (Int4) start, (Int4) stop, FALSE, FALSE);
              }

            } else {

              sfp->location = AddIntervalToLocation (sfp->location, sip, (Int4) start, (Int4) stop, FALSE, FALSE);

            }
          }
        }

      } else {
        StringNCpy_0 (matchtype, line, sizeof (matchtype));
        sfp = NULL;
        if (StringCmp (matchtype, "No hits found") == 0) {
          sprintf (note, "No vector hits found for %s", seqid);
          Message (MSG_POST, "%s\n", note);
        }
      }

    }

    pos = ftell (fp);
    str = ReadALine (line, sizeof (line), fp);
  }

  SeqIdFree (sip);
  return sap;
}

/* ReadRestrictionSiteTable reads lines of restriction enzyme names or cut sites into a Seq-annot. */

static SeqLocPtr AddPointToLocation (SeqLocPtr loc, SeqIdPtr sip, Int4 pt)

{
  PackSeqPntPtr  pspp;
  SeqLocPtr      slp;

  if (sip == NULL) return NULL;

  if (loc == NULL) {
    pspp = PackSeqPntNew ();
    pspp->id = SeqIdDup (sip);
    slp = ValNodeNew (NULL);
    slp->choice = SEQLOC_PACKED_PNT;
    slp->data.ptrvalue = (Pointer) pspp;
    loc = slp;
  }

  if (loc != NULL && loc->choice == SEQLOC_PACKED_PNT) {
    pspp = (PackSeqPntPtr) loc->data.ptrvalue;
    if (pspp != NULL) {
      PackSeqPntPut (pspp, pt);
    }
  }

  return loc;
}

static SeqAnnotPtr ReadRestrictionSiteTable (FILE *fp, CharPtr seqid, CharPtr annotname)

{
  DbtagPtr       dbt;
  AnnotDescrPtr  desc;
  Char           line [1023];
  Char           name [64];
  ObjectIdPtr    oip;
  Int4           pos;
  SeqFeatPtr     prev;
  Int4           pt;
  RsiteRefPtr    rrp;
  SeqAnnotPtr    sap = NULL;
  SeqFeatPtr     sfp = NULL;
  SeqIdPtr       sip;
  CharPtr        str;
  long int       val;

  if (fp == NULL || seqid == NULL) return NULL;
  sip = SeqIdFindBest (MakeSeqID (seqid), 0);
  if (sip == NULL) return NULL;
  name [0] = '\0';

  pos = ftell (fp);
  str = ReadALine (line, sizeof (line), fp);
  while (str != NULL) {

    if (! HasNoText (line)) {

      if (StringNCmp (line, ">", 1) == 0 ||
          StringNCmp (line, "LOCUS ", 6) == 0 ||
          StringNCmp (line, "ID ", 3) == 0 ||
          StringStr (line, "::=") != NULL) {
        fseek (fp, pos, SEEK_SET);
        SeqIdFree (sip);
        return sap;
      } else if (StringNCmp (line, "//", 2) == 0) {
        SeqIdFree (sip);
        return sap;
      }

      if (sscanf (line, "%ld", &val) == 1) {
        pt = (Int4) val;

        if (! HasNoText (name)) {

          if (sap == NULL) {
            sap = SeqAnnotNew ();
            if (sap != NULL) {
              sap->type = 1;
              if (! HasNoText (annotname)) {
                desc = ValNodeNew (NULL);
                if (desc != NULL) {
                  desc->choice = Annot_descr_name;
                  desc->data.ptrvalue = StringSave (annotname);
                  sap->desc = desc;
                }
              }
            }
          }

          if (sfp == NULL) {
            sfp = SeqFeatNew ();
            if (sfp != NULL) {
              sfp->data.choice = SEQFEAT_RSITE;
              dbt = DbtagNew ();
              if (dbt != NULL) {
                dbt->db = StringSave ("REBASE");
                oip = ObjectIdNew ();
                if (oip != NULL) {
                  oip->str = StringSave (name);
                }
                dbt->tag = oip;
              }
              rrp = ValNodeNew (NULL);
              if (rrp != NULL) {
                rrp->choice = 2;
                rrp->data.ptrvalue = dbt;
              }
              sfp->data.value.ptrvalue = (Pointer) rrp;

              if (sap != NULL) {
                if (sap->data != NULL) {
                  prev = sap->data;
                  while (prev->next != NULL) {
                    prev = prev->next;
                  }
                  prev->next = sfp;
                } else {
                  sap->data = (Pointer) sfp;
                }
              }
            }
          }

          sfp->location = AddPointToLocation (sfp->location, sip, pt);

        }

      } else {
        StringNCpy_0 (name, line, sizeof (name));
        sfp = NULL;
      }

    }

    pos = ftell (fp);
    str = ReadALine (line, sizeof (line), fp);
  }

  SeqIdFree (sip);
  return sap;
}

/* ReadMessageStrings allows retired services to announce replacement URLs. */

static void ReadMessageStrings (FILE *fp)

{
  Boolean     done = FALSE;
  ValNodePtr  head = NULL;
  size_t      len;
  Char        line [1023];
  Int4        pos;
  CharPtr     ptr;
  CharPtr     str;
  CharPtr     tmp;
  ValNodePtr  vnp;

  if (fp == NULL) return;

  pos = ftell (fp);
  str = ReadALine (line, sizeof (line), fp);
  while (str != NULL && (! done)) {

    if (! HasNoText (line)) {

      if (StringNCmp (line, ">", 1) == 0 ||
          StringNCmp (line, "LOCUS ", 6) == 0 ||
          StringNCmp (line, "ID ", 3) == 0 ||
          StringStr (line, "::=") != NULL) {
        fseek (fp, pos, SEEK_SET);
        done = TRUE;
      } else if (StringNCmp (line, "//", 2) == 0) {
        done = TRUE;
      }

      if (! done) {
        ValNodeCopyStr (&head, 0, line);
      }
      /* Message (MSG_POST, "%s\n", line); */
    }

    if (! done) {
      pos = ftell (fp);
      str = ReadALine (line, sizeof (line), fp);
    }
  }

  for (vnp = head, len = 0; vnp != NULL; vnp = vnp->next) {
    str = (CharPtr) vnp->data.ptrvalue;
    if (str != NULL) {
      len += StringLen (str) + 1;
    }
  }
  if (len > 0) {
    ptr = MemNew (sizeof (Char) * (len + 2));
    if (ptr != NULL) {
      for (vnp = head, tmp = NULL; vnp != NULL; vnp = vnp->next) {
        str = (CharPtr) vnp->data.ptrvalue;
        if (str != NULL) {
          if (tmp == NULL) {
            tmp = ptr;
          } else {
            tmp = StringMove (tmp, "\n");
          }
          tmp = StringMove (tmp, str);
        }
      }
      Message (MSG_POST, "%s\n", ptr);
      MemFree (ptr);
    }
  }

  ValNodeFreeData (head);
}

/* ReadUidList reads lines of uids (or accessions) into a byte store. */

static ByteStorePtr ReadUidList (FILE *fp, Boolean nucdb, Boolean lastResortSeqIDs)

{
  Boolean       allDigits;
  Boolean       abort = FALSE;
  ByteStorePtr  bs;
  Char          ch;
  Char          line [1023];
  Int4          pos;
  CharPtr       ptr;
  CharPtr       str;
  TextSeqId     tsid;
  Int4          uid;
  long int      val;
  ValNode       vn;

  if (fp == NULL) return NULL;
  bs = BSNew (128);
  if (bs == NULL) return NULL;

  pos = ftell (fp);
  str = ReadALine (line, sizeof (line), fp);
  while (str != NULL) {

    if (! HasNoText (line)) {

      if (StringNCmp (line, ">", 1) == 0 ||
          StringNCmp (line, "LOCUS ", 6) == 0 ||
          StringNCmp (line, "ID ", 3) == 0 ||
          StringStr (line, "::=") != NULL) {
        fseek (fp, pos, SEEK_SET);
        if (abort) {
          bs = BSFree (bs);
        }
        return bs;
      } else if (StringNCmp (line, "//", 2) == 0) {
        if (abort) {
          bs = BSFree (bs);
        }
        return bs;
      }

      allDigits = TRUE;
      ptr = line;
      ch = *ptr;
      while (ch != '\0' && allDigits) {
        if (! IS_DIGIT (ch)) {
          allDigits = FALSE;
        }
        ptr++;
        ch = *ptr;
      }
      if (allDigits && sscanf (line, "%ld", &val) == 1) {
        uid = (Int4) val;
        BSWrite (bs, &uid, sizeof (Int4));
      } else if (nucdb) {
        tsid.name = NULL;
        tsid.accession = line;
        tsid.release = NULL;
        tsid.version = INT2_MIN;
        vn.choice = (Uint1) SEQID_GENBANK;
        vn.data.ptrvalue = (Pointer) (&tsid);
        uid = GetGIForSeqId (&vn);
        if (uid > 0) {
          BSWrite (bs, &uid, sizeof (Int4));
        } else if (lastResortSeqIDs) {
          abort = TRUE;
        }
      }

    }

    pos = ftell (fp);
    str = ReadALine (line, sizeof (line), fp);
  }

  if (abort) {
    bs = BSFree (bs);
  }
  return bs;
}

/* ReadAsnFastaOrFlatFile reads lines, looking for starts of ASN.1, FASTA, GenBank, EMBL,
or GenPept files.  It then calls the appropriate read function, which is responsible for
reading the sequence (or object) and restoring the file pointer to the beginning of the
next record. */

NLM_EXTERN Pointer ReadAsnFastaOrFlatFile (FILE *fp, Uint2Ptr datatypeptr, Uint2Ptr entityIDptr,
                                           Boolean forceNuc, Boolean forceProt,
                                           Boolean parseFastaSeqId, Boolean fastaAsSimpleSeq)

{
  AsnIoPtr       aip;
  CharPtr        annotname;
  Int4           begin;
  ByteStorePtr   bs = NULL;
  BioseqPtr      bsp = NULL;
  BioseqSetPtr   bssp;
  Char           ch;
  Uint1          choice = 0;
  Boolean        coordinatesOnMaster;
  Uint2          datatype;
  Int2           db = -1;
  Boolean        inLetters;
  Boolean        isProt = FALSE;
  Int4           j;
  Char           line [1023];
  Boolean        mayBeAccessionList = TRUE;
  Boolean        mayBePlainFasta = TRUE;
  SeqFeatPtr     nextsfp;
  Int2           numDigits;
  Int2           numLetters;
  Int4           numLinks;
  ObjMgrDataPtr  omdp;
  ObjMgrPtr      omp;
  ObjMgrTypePtr  omtp = NULL;
  PubdescPtr     pdp;
  Int4           pos;
  ValNodePtr     pip;
  Pointer PNTR   prevsfp;
  ProjectPtr     proj = NULL;
  BoolPtr        protPtr;
  Pointer        ptr = NULL;
  SeqAnnotPtr    sap = NULL;
  SeqEntryPtr    sep;
  SeqFeatPtr     sfp;
  Char           seqid [128];
  SimpleSeqPtr   ssp = NULL;
  CharPtr        str;
  CharPtr        tag;
  CharPtr        title = NULL;
  CharPtr        tmp;
  Int4           uid;
  long int       val;
  ValNodePtr     vnp;
  ObjectIdPtr    oip;
  UserFieldPtr   ufp;
  UserObjectPtr  uop;

  if (fp == NULL) return NULL;

  if (datatypeptr != NULL) *datatypeptr = 0;
  if (entityIDptr != NULL) *entityIDptr = 0;

  if (forceNuc) {
    isProt = FALSE;
    protPtr = NULL;
  } else if (forceProt) {
    isProt = TRUE;
    protPtr = NULL;
  } else {
    protPtr = &isProt;
  }

  seqid [0] = '\0';
  pos = ftell (fp);
  begin = pos;
  str = ReadALine (line, sizeof (line), fp);
  while (str != NULL) {

    if (! HasNoText (line)) {

      if (StringStr (line, "::=") != NULL) {

        mayBePlainFasta = FALSE;
        mayBeAccessionList = FALSE;

        /* first skip past empty space at start of line */

        tag = line;
        ch = *tag;
        while (ch != '\0' && IS_WHITESP (ch)) {
          tag++;
          ch = *tag;
        }

        /* now find ASN tag */

        tmp = tag;
        ch = *tmp;
        while (ch != '\0' && (! IS_WHITESP (ch))) {
          tmp++;
          ch = *tmp;
        }
        *tmp = '\0';

        omp = ObjMgrReadLock ();
        omtp = ObjMgrTypeFind (omp, 0, tag, NULL);
        ObjMgrUnlock ();

        if (omtp != NULL) {
          fseek (fp, pos, SEEK_SET);
          aip = AsnIoNew (ASNIO_TEXT_IN, fp, NULL, NULL, NULL);
          aip->scan_for_start = TRUE;
          ptr = (*(omtp->asnread)) (aip, NULL);
          pos = AsnIoTell (aip);
          AsnIoFree (aip, FALSE);
          fseek (fp, pos, SEEK_SET);

          if (ptr == NULL) {
            ErrPostEx (SEV_ERROR, 0, 0, "Couldn't read type [%s]", omtp->asnname);
          } else {
            datatype = omtp->datatype;
            if (datatypeptr != NULL) {
              *datatypeptr = datatype;
            }
            if (entityIDptr != NULL) {
              *entityIDptr = ObjMgrRegister (datatype, ptr);
            }
            if (datatype == OBJ_BIOSEQ || datatype == OBJ_BIOSEQSET) {
              omp = ObjMgrReadLock ();
              omdp = ObjMgrFindByData (omp, ptr);
              ObjMgrUnlock ();
              if (omdp != NULL && omdp->choice == NULL) {
                /* always want sep above bsp or bssp */
                sep = SeqEntryNew ();
                if (sep != NULL) {
                  if (datatype == OBJ_BIOSEQ) {
                    bsp = (BioseqPtr) ptr;
                    sep->choice = 1;
                    sep->data.ptrvalue = bsp;
                    SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, sep);
                  } else if (datatype == OBJ_BIOSEQSET) {
                    bssp = (BioseqSetPtr) ptr;
                    sep->choice = 2;
                    sep->data.ptrvalue = bssp;
                    SeqMgrSeqEntry (SM_BIOSEQSET, (Pointer) bssp, sep);
                  } else {
                    sep = SeqEntryFree (sep);
                  }
                }
              }
            }
          }
        } else {
          ErrPostEx (SEV_ERROR, 0, 0, "Couldn't read type [%s]", line);
        }
        return ptr;

      } else if (line [0] == '>') {

        mayBePlainFasta = FALSE;
        mayBeAccessionList = FALSE;
        db = -1;
        if (StringNCmp (line, ">PubMed", 7) == 0) {
          db = 0;
        } else if (StringNCmp (line, ">Protein", 8) == 0) {
          db = 1;
        } else if (StringNCmp (line, ">Nucleotide", 11) == 0) {
          db = 2;
        } else if (StringNCmp (line, ">Structure", 10) == 0) {
          db = 3;
        } else if (StringNCmp (line, ">Genome", 7) == 0) {
          db = 4;
        }
        if (db != -1) {
          bs = ReadUidList (fp, (Boolean) (db == 2), FALSE);
          if (bs != NULL) {
            proj = ProjectNew ();
            if (proj != NULL) {
              pip = ValNodeNew (NULL);
              if (pip != NULL) {
                switch (db) {
                  case 0 :
                    choice = ProjectItem_pmuid;
                    break;
                  case 1 :
                    choice = ProjectItem_protuid;
                    break;
                  case 2 :
                    choice = ProjectItem_nucuid;
                    break;
                  case 3 :
                    choice = ProjectItem_genomeuid;
                    break;
                  case 4 :
                    choice = ProjectItem_structuid;
                    break;
                  default :
                    choice = 0;
                    break;
                }
                pip->choice = choice;
                proj->data = pip;
                numLinks = BSLen (bs) / sizeof (Int4);
                BSSeek (bs, 0L, 0);
                for (j = 0; j < numLinks; j++) {
                  BSRead (bs, &uid, sizeof (Int4));
                  ValNodeAddInt ((ValNodePtr PNTR) &(pip->data.ptrvalue), choice, uid);
                  /*
                  switch (db) {
                    case 0 :
                      ValNodeAddInt (&(pip->data.ptrvalue), ProjectItem_pmid, uid);
                      break;
                    case 1 :
                    case 2 :
                    case 3 :
                      sip = ValNodeNew (NULL);
                      if (sip != NULL) {
                        sip->choice = SEQID_GI;
                        sip->data.intvalue = uid;
                      }
                      break;
                    case 4 :
                      break;
                    default :
                      break;
                  }
                  */
                }
              }
            }
            bs = BSFree (bs);

            if (datatypeptr != NULL) {
              *datatypeptr = OBJ_PROJECT;
            }
            if (entityIDptr != NULL) {
              *entityIDptr = ObjMgrRegister (OBJ_PROJECT, (Pointer) proj);
            }

            return (Pointer) proj;
          }

        } else if (StringNICmp (line, ">Feature", 8) == 0) {

          annotname = GetSeqId (seqid, line, sizeof (seqid), TRUE, FALSE);
          if (! HasNoText (seqid)) {
            sap = ReadFeatureTable (fp, seqid, annotname);
            if (sap != NULL && sap->type == 1) {
              sfp = (SeqFeatPtr) sap->data;
              prevsfp = (Pointer PNTR) &(sap->data);
              while (sfp != NULL) {
                nextsfp = sfp->next;
                if (sfp->data.choice == SEQFEAT_PUB) {
                  pdp = (PubdescPtr) sfp->data.value.ptrvalue;
                  if (pdp != NULL && pdp->pub == NULL) {
                    *(prevsfp) = sfp->next;
                    sfp->next = NULL;
                    SeqFeatFree (sfp);
                  } else {
                    prevsfp = (Pointer PNTR) &(sfp->next);
                  }
                } else {
                  prevsfp = (Pointer PNTR) &(sfp->next);
                }
                sfp = nextsfp;
              }
              if (sap->data == NULL) {
                sap = SeqAnnotFree (sap);
              }
            }
            if (sap != NULL) {
              if (datatypeptr != NULL) {
                *datatypeptr = OBJ_SEQANNOT;
              }
              if (entityIDptr != NULL) {
                *entityIDptr = ObjMgrRegister (OBJ_SEQANNOT, (Pointer) sap);
              }
              return (Pointer) sap;
            }
          }

        } else if (StringNICmp (line, ">Vector", 7) == 0) {

          annotname = GetSeqId (seqid, line, sizeof (seqid), TRUE, FALSE);
          if (! HasNoText (seqid)) {
            sap = ReadVecScreenTable (fp, seqid, annotname);
            if (sap != NULL) {
              if (datatypeptr != NULL) {
                *datatypeptr = OBJ_SEQANNOT;
              }
              if (entityIDptr != NULL) {
                *entityIDptr = ObjMgrRegister (OBJ_SEQANNOT, (Pointer) sap);
              }
              return (Pointer) sap;
            }
          }

        } else if (StringNICmp (line, ">Restriction", 12) == 0) {

          annotname = GetSeqId (seqid, line, sizeof (seqid), TRUE, TRUE);
          if (! HasNoText (seqid)) {
            sap = ReadRestrictionSiteTable (fp, seqid, annotname);
            if (sap != NULL) {
              if (datatypeptr != NULL) {
                *datatypeptr = OBJ_SEQANNOT;
              }
              if (entityIDptr != NULL) {
                *entityIDptr = ObjMgrRegister (OBJ_SEQANNOT, (Pointer) sap);
              }
              return (Pointer) sap;
            }
          }

        } else if (StringNICmp (line, ">Contig", 7) == 0) {

          coordinatesOnMaster = FALSE;
          if (StringISearch (line, "Master") != NULL) {
            coordinatesOnMaster = TRUE;
          }
          annotname = GetSeqId (seqid, line, sizeof (seqid), TRUE, FALSE);
          sep = ReadContigListEx (fp, coordinatesOnMaster, seqid, annotname);
          if (sep != NULL && IS_Bioseq (sep)) {
            bsp = (BioseqPtr) sep->data.ptrvalue;
            if (bsp != NULL) {

              oip = ObjectIdNew ();
              oip->str = StringSave ("info");
              uop = UserObjectNew ();
              uop->type = oip;
              uop->_class = StringSave ("Genomes");

              oip = ObjectIdNew ();
              oip->id = 0;
              ufp = UserFieldNew ();
              ufp->choice = 2;
              ufp->data.intvalue = 0;
              ufp->label = oip;

              uop->data = ufp;

              vnp = SeqDescrNew (NULL);
              vnp->choice = Seq_descr_user;
              vnp->data.ptrvalue = (Pointer) uop;
              vnp->next = bsp->descr;
              bsp->descr = vnp;

              if (datatypeptr != NULL) {
                *datatypeptr = OBJ_BIOSEQ;
              }
              if (entityIDptr != NULL) {
                *entityIDptr = ObjMgrRegister (OBJ_BIOSEQ, (Pointer) bsp);
              }
            }
            return (Pointer) bsp;
          }

        } else if (StringNICmp (line, ">Virtual", 8) == 0) {

          tmp = GetSeqId (seqid, line, sizeof (seqid), TRUE, TRUE);
          if (! HasNoText (seqid)) {
            TrimSpacesAroundString (tmp);
            if (tmp != NULL && sscanf (tmp, "%ld", &val) == 1) {
              sep = SeqEntryNew ();
              if (sep != NULL) {
                bsp = BioseqNew ();
                if (bsp != NULL) {
                  sep->choice = 1;
                  sep->data.ptrvalue = (Pointer) bsp;
                  SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, sep);

                  bsp->mol = Seq_mol_na;

                  bsp->repr = Seq_repr_virtual;
                  bsp->length = (Int4) val;
                  bsp->id = MakeSeqID (seqid);
                  SeqMgrAddToBioseqIndex (bsp);

                  if (datatypeptr != NULL) {
                    *datatypeptr = OBJ_BIOSEQ;
                  }
                  if (entityIDptr != NULL) {
                    *entityIDptr = ObjMgrRegister (OBJ_BIOSEQ, (Pointer) bsp);
                  }
                }
              }
              return (Pointer) bsp;
            }
          }

        } else if (StringNICmp (line, ">Message", 8) == 0) {

          ReadMessageStrings (fp);

        } else {

          title = NULL;
          tmp = StringChr (line + 1, '[');
          if (tmp != NULL) {
            if (StringStr (tmp, "[") != NULL && StringStr (tmp, "=") != NULL) {
              title = StringSave (tmp);
            }
          } else if (fastaAsSimpleSeq) {
            tmp = StringChr (line + 1, ' ');
            if (tmp != NULL) {
              tmp++;
              TrimSpacesAroundString (tmp);
              title = StringSaveNoNull (tmp);
            }
          }
          if (parseFastaSeqId) {
            tmp = line + 1;
            ch = *tmp;
            while (IS_WHITESP (ch)) {
              tmp++;
              ch = *tmp;
            }
            if (ch == '[') {
              parseFastaSeqId = FALSE;
            }
          }
          if (parseFastaSeqId) {
            GetSeqId (seqid, line + 1, sizeof (seqid), FALSE, TRUE);
            if (! HasNoText (seqid)) {
              tmp = StringStr (line + 1, seqid);
              if (tmp != NULL) {
                tmp += StringLen (seqid);
                if (! StringHasNoText (tmp)) {
                  TrimSpacesAroundString (tmp);
                  title = StringSaveNoNull (tmp);
                }
              }
              bs = ReadFlatFileDNA (fp, protPtr, forceNuc, forceProt, fastaAsSimpleSeq);
            }
          } else {
            bs = ReadFlatFileDNA (fp, protPtr, forceNuc, forceProt, fastaAsSimpleSeq);
          }
          if (bs == NULL && title != NULL) {
            title = MemFree (title);
          }
        }

      } else if (StringNCmp (line, "LOCUS ", 6) == 0 || StringNCmp (line, "ID ", 3) == 0) {

        mayBePlainFasta = FALSE;
        mayBeAccessionList = FALSE;
        GetSeqId (seqid, line, sizeof (seqid), TRUE, TRUE);

      } else if (StringNCmp (line, "ORIGIN ", 7) == 0 || StringNCmp (line, "SQ ", 3) == 0) {

        mayBePlainFasta = FALSE;
        mayBeAccessionList = FALSE;
        if (! HasNoText (seqid)) {
          bs = ReadFlatFileDNA (fp, protPtr, forceNuc, forceProt, fastaAsSimpleSeq);
        }

      } else {
        if (mayBePlainFasta) {
          tmp = line;
          ch = *tmp;
          while (ch != '\0') {
            if (IS_WHITESP (ch)) {
            } else if (! (IS_ALPHA (ch) || IS_DIGIT (ch) || ch == '*' || ch == '-')) {
              mayBePlainFasta = FALSE;
            } else if (protPtr != NULL) {
              ch = TO_UPPER (ch);
              if (StringChr ("EFILPQZ", ch) != NULL) {
                isProt = TRUE;
              }
            }
            tmp++;
            ch = *tmp;
          }
        }
        if (mayBeAccessionList) {
          inLetters = TRUE;
          numLetters = 0;
          numDigits = 0;
          tmp = line;
          ch = *tmp;
          while (ch != '\0') {
            if (IS_WHITESP (ch)) {
            } else if (IS_ALPHA (ch)) {
              if (! inLetters) {
                mayBeAccessionList = FALSE;
                numLetters++;
              }
            } else if (IS_DIGIT (ch)) {
              inLetters = FALSE;
              numDigits++;
            } else {
              mayBeAccessionList = FALSE;
            }
            tmp++;
            ch = *tmp;
          }
          if (numLetters == 1 && numDigits == 5) {
          } else if (numLetters == 2 && numDigits == 6) {
          } else {
            mayBeAccessionList = FALSE;
          }
        }
      }

      if (bs != NULL) {
        if (fastaAsSimpleSeq) {
          ssp = ByteStoreToSimpleSeq (bs, seqid, title);
          if (ssp != NULL) {
            if (datatypeptr != NULL) {
              *datatypeptr = OBJ_FASTA;
            }
            if (entityIDptr != NULL) {
              *entityIDptr = ObjMgrRegister (OBJ_FASTA, (Pointer) ssp);
            }
          }
          return (Pointer) ssp;
        }

        sep = SeqEntryNew ();
        if (sep != NULL) {
          bsp = BioseqNew ();
          if (bsp != NULL) {
            sep->choice = 1;
            sep->data.ptrvalue = (Pointer) bsp;
            SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, sep);

            if (isProt) {
              bsp->mol = Seq_mol_aa;
              bsp->seq_data_type = Seq_code_ncbieaa;
            } else {
              bsp->mol = Seq_mol_na;
              bsp->seq_data_type = Seq_code_iupacna;
            }

            bsp->repr = Seq_repr_raw;
            bsp->length = 0;
            if (parseFastaSeqId) {
              bsp->id = MakeSeqID (seqid);
            } else {
              bsp->id = MakeNewProteinSeqId (NULL, NULL);
            }
            SeqMgrAddToBioseqIndex (bsp);

            bsp->seq_data = bs;
            bsp->length = BSLen (bs);

            BioseqPack (bsp);

            if (title != NULL) {
              vnp = CreateNewDescriptor (sep, Seq_descr_title);
              if (vnp != NULL) {
                vnp->data.ptrvalue = (Pointer) title;
                title = NULL;
              }
              bsp->descr = vnp;
            }

            if (datatypeptr != NULL) {
              *datatypeptr = OBJ_BIOSEQ;
            }
            if (entityIDptr != NULL) {
              *entityIDptr = ObjMgrRegister (OBJ_BIOSEQ, (Pointer) bsp);
            }
          }
        }
        return (Pointer) bsp;
      }

    }

    pos = ftell (fp);
    str = ReadALine (line, sizeof (line), fp);
  }

  if (mayBePlainFasta) {

    fseek (fp, begin, SEEK_SET);
    if (fastaAsSimpleSeq) {
      bs = ReadFlatFileDNA (fp, FALSE, (Boolean) (! isProt), (Boolean) (isProt), fastaAsSimpleSeq);
      if (bs != NULL) {
        ssp = ByteStoreToSimpleSeq (bs, NULL, NULL);
        if (ssp != NULL) {
          if (datatypeptr != NULL) {
            *datatypeptr = OBJ_FASTA;
          }
          if (entityIDptr != NULL) {
            *entityIDptr = ObjMgrRegister (OBJ_FASTA, (Pointer) ssp);
          }
        }
        return (Pointer) ssp;
      }
    }

    sep = FastaToSeqEntryEx (fp, (Boolean) (! isProt), NULL, FALSE);
    if (sep != NULL && IS_Bioseq (sep)) {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      if (bsp != NULL) {
        if (datatypeptr != NULL) {
          *datatypeptr = OBJ_BIOSEQ;
        }
        if (entityIDptr != NULL) {
          *entityIDptr = ObjMgrRegister (OBJ_BIOSEQ, (Pointer) bsp);
        }
      }
      return (Pointer) bsp;
    }

  }

  if (mayBeAccessionList) {

    fseek (fp, begin, SEEK_SET);
    bs = ReadUidList (fp, TRUE, TRUE);
    if (bs != NULL) {
      numLinks = BSLen (bs) / sizeof (Int4);
      if (numLinks < 1) {
        bs = BSFree (bs);
        return NULL;
      }
      proj = ProjectNew ();
      if (proj != NULL) {
        pip = ValNodeNew (NULL);
        if (pip != NULL) {
          choice = ProjectItem_nucuid;
          pip->choice = choice;
          proj->data = pip;
          BSSeek (bs, 0L, 0);
          for (j = 0; j < numLinks; j++) {
            BSRead (bs, &uid, sizeof (Int4));
            ValNodeAddInt ((ValNodePtr PNTR) &(pip->data.ptrvalue), choice, uid);
          }
        }
      }
      bs = BSFree (bs);

      if (datatypeptr != NULL) {
        *datatypeptr = OBJ_PROJECT;
      }
      if (entityIDptr != NULL) {
        *entityIDptr = ObjMgrRegister (OBJ_PROJECT, (Pointer) proj);
      }

      return (Pointer) proj;
    }

  }

  return NULL;
}

/* general purpose text finite state machine */
/* based on Practical Algorithms for Programmers by Binstock and Rex */

typedef struct fsagoto {
  Char             ch;
  Int2             newstate;
  struct fsagoto * next;
} GotoItem, PNTR GotoPtr;

typedef struct fsastate {
  GotoPtr       transition;
  ValNodePtr    matchfound;
  Int2          onfailure;
} StateItem, PNTR StatePtr;

#define FAIL_STATE -1

static Int2 GotoState (StatePtr stateTable, Int2 state,
                       Char ch, Boolean zeroFailureReturnsZero)

{
  GotoPtr  gp;

  for (gp = stateTable [state].transition; gp != NULL; gp = gp->next) {
    if (gp->ch == ch) return gp->newstate;
  }

  if (state == 0 && zeroFailureReturnsZero) return 0;

  return FAIL_STATE;
}

#define FailState(stateTable,state) stateTable [state].onfailure

static void AddTransition (StatePtr stateTable, Int2 oldState,
                           Char ch, Int2 newState)

{
  GotoPtr  gp;
  GotoPtr  prev;

  gp = (GotoPtr) MemNew (sizeof (GotoItem));
  if (gp == NULL) return;

  gp->ch = ch;
  gp->newstate = newState;

  prev = stateTable [oldState].transition;
  if (prev == NULL) {
    stateTable [oldState].transition = gp;
  } else {
    while (prev->next != NULL) {
      prev = prev->next;
    }
    prev->next = gp;
  }
}

static void AddOutput (StatePtr stateTable, Int2 state, CharPtr word)

{
  ValNodePtr  vnp;

  for (vnp = stateTable [state].matchfound; vnp != NULL; vnp = vnp->next) {
    if (StringCmp (word, (CharPtr) vnp->data.ptrvalue) == 0) return;
  }

  ValNodeCopyStr (&(stateTable [state].matchfound), 0, word);
}

static Int2 EnterWord (StatePtr stateTable, CharPtr word,
                       Int2 highState, Int2 maxState)

{
  Char     ch;
  Int2     next;
  CharPtr  ptr;
  Int2     state;

  state = 0;
  next = 0;

  /* try to overlay beginning of word onto existing table */

  for (ptr = word, ch = *ptr; ch != '\0'; ptr++, ch = *ptr) {
    next = GotoState (stateTable, state, ch, FALSE);
    if (next == FAIL_STATE) break;
    state = next;
  }

  /* now create new states for remaining characters in word */

  for ( ; ch != '\0'; ptr++, ch = *ptr) {
    highState++;
    AddTransition (stateTable, state, ch, highState);
    state = highState;
  }

  /* at end of word record match information */

  AddOutput (stateTable, state, word);

  return highState;
}

static void QueueAdd (Int2Ptr queue, Int2 qbeg, Int2 val)

{
  Int2  q;

  q = queue [qbeg];
  if (q == 0) {
    queue [qbeg] = val;
  } else {
    for ( ; queue [q] != 0; q = queue [q]) continue;
    queue [q] = val;
  }
  queue [val] = 0;
}

static void FindFail (StatePtr stateTable, Int2 state,
                      Int2 newState, Char ch)

{
  Int2        next;
  ValNodePtr  vnp;

  /* traverse existing failure path */

  while ((next = GotoState (stateTable, state, ch, TRUE)) == FAIL_STATE) {
    state = FailState (stateTable, state);
  }

  /* add new failure state */

  stateTable [newState].onfailure = next;

  /* add matches of substring at new state */

  for (vnp = stateTable [next].matchfound; vnp != NULL; vnp = vnp->next) {
    AddOutput (stateTable, newState, (CharPtr) vnp->data.ptrvalue);
  }
}

static void ComputeFail (StatePtr stateTable, Int2Ptr queue, Int2 highState)

{
  GotoPtr  gp;
  Int2     qbeg, r, s, state;

  qbeg = 0;
  queue [0] = 0;

  /* queue up states reached directly from state 0 (depth 1) */

  for (gp = stateTable [0].transition; gp != NULL; gp = gp->next) {
    s = gp->newstate;
    stateTable [s].onfailure = 0;
    QueueAdd (queue, qbeg, s);
  }

  while (queue [qbeg] != 0) {
    r = queue [qbeg];
    qbeg = r;

    /* depth 1 states beget depth 2 states, etc. */

    for (gp = stateTable [r].transition; gp != NULL; gp = gp->next) {
      s = gp->newstate;
      QueueAdd (queue, qbeg, s);

      /*
         State   Substring   Transitions   Failure
           2       st          a ->   3       6
           3       sta         l ->   4
           6       t           a ->   7       0
           7       ta          p ->   8

         For example, r = 2 (st), if 'a' would go to s = 3 (sta).
         From previous computation, 2 (st) fails to 6 (t).
         Thus, check state 6 (t) for any transitions using 'a'.
         Since 6 (t) 'a' -> 7 (ta), therefore set fail [3] -> 7.
      */

      state = FailState (stateTable, r);
      FindFail (stateTable, state, s, gp->ch);
    }
  }
}

typedef struct TextFsa {
  StatePtr      statePtr;
  ValNodePtr    siteList;
  Int2          maxState;
  Boolean       primed;
} TextFsaData;

static void PrimeStateTable (TextFsaPtr tbl)

{
  Int2        highState, maxState;
  Int2Ptr     queue;
  StatePtr    stateTable;
  ValNodePtr  vnp;
  CharPtr     word;

  if (tbl == NULL || tbl->siteList == NULL || tbl->primed) return;

  for (maxState = 1, vnp = tbl->siteList; vnp != NULL; vnp = vnp->next) {
    word = (CharPtr) vnp->data.ptrvalue;
    maxState += StringLen (word);
  }

  if (maxState > 4000) {
    Message (MSG_POST, "FiniteStateSearch cannot handle %d states", (int) maxState);
    return;
  }

  stateTable = (StatePtr) MemNew (sizeof (StateItem) * (size_t) maxState);
  queue = (Int2Ptr) MemNew (sizeof (Int2) * maxState);

  if (stateTable == NULL || queue == NULL) {
    MemFree (stateTable);
    MemFree (queue);
    Message (MSG_POST, "FiniteStateSearch unable to allocate buffers");
    return;
  }

  for (highState = 0, vnp = tbl->siteList; vnp != NULL; vnp = vnp->next) {
    word = (CharPtr) vnp->data.ptrvalue;
    highState = EnterWord (stateTable, word, highState, maxState);
  }

  ComputeFail (stateTable, queue, highState);

  MemFree (queue);

  tbl->statePtr = stateTable;
  tbl->maxState = maxState;
  tbl->primed = TRUE;
}

NLM_EXTERN TextFsaPtr TextFsaNew (void)

{
  TextFsaPtr  tbl;

  tbl = (TextFsaPtr) MemNew (sizeof (TextFsaData));
  if (tbl == NULL) return NULL;
  tbl->statePtr = NULL;
  tbl->siteList = NULL;
  tbl->primed = FALSE;
  return tbl;
}

NLM_EXTERN void TextFsaAdd (TextFsaPtr tbl, CharPtr word)

{
  if (tbl == NULL) return;
  ValNodeCopyStr (&(tbl->siteList), 0, word);
}

NLM_EXTERN Int2 TextFsaNext (TextFsaPtr tbl, Int2 currState,
                             Char ch, ValNodePtr PNTR matches)

{
  Int2      next;
  StatePtr  statePtr;

  if (matches != NULL) {
    *matches = NULL;
  }
  if (tbl == NULL) return 0;
  if (! tbl->primed) {
    PrimeStateTable (tbl);
  }
  statePtr = tbl->statePtr;
  if (statePtr == NULL) return 0;

  while ((next = GotoState (statePtr, currState, ch, TRUE)) == FAIL_STATE) {
    currState = FailState (statePtr, currState);
  }

  if (matches != NULL) {
    *matches = statePtr [next].matchfound;
  }

  return next;
}

NLM_EXTERN TextFsaPtr TextFsaFree (TextFsaPtr tbl)

{
  GotoPtr   gp;
  Int2      maxState;
  GotoPtr   nxtgp;
  Int2      state;
  StatePtr  statePtr;

  if (tbl == NULL) return NULL;
  statePtr = tbl->statePtr;
  if (statePtr == NULL) return NULL;
  maxState = tbl->maxState;

  for (state = 0; state < maxState; state++) {
    gp = statePtr [state].transition;
    while (gp != NULL) {
      nxtgp = gp->next;
      MemFree (gp);
      gp = nxtgp;
    }
    ValNodeFreeData (statePtr [state].matchfound);
  }

  MemFree (statePtr);
  return MemFree (tbl);
}

/* sequence quality exchange */

typedef struct gphgetdata {
  ValNodePtr  vnp;
  BioseqPtr   bsp;
} GphGetData, PNTR GphGetPtr;

typedef struct gphitem {
  SeqGraphPtr  sgp;
  Int4         left;
  Int4         right;
  Int2         index;
} GphItem, PNTR GphItemPtr;

static Boolean GetGraphsProc (GatherObjectPtr gop)

{
  GphGetPtr    ggp;
  GphItemPtr   gip;
  SeqGraphPtr  sgp;

  if (gop == NULL || gop->itemtype != OBJ_SEQGRAPH) return TRUE;
  ggp = (GphGetPtr) gop->userdata;
  sgp = (SeqGraphPtr) gop->dataptr;
  if (ggp == NULL || sgp == NULL) return TRUE;
  /* only phrap or gap4 currently allowed */
  if (StringICmp (sgp->title, "Phrap Quality") == 0 ||
      StringICmp (sgp->title, "Phred Quality") == 0 ||
      StringICmp (sgp->title, "Gap4") == 0) {
    /* data type must be bytes */
    if (sgp->flags[2] == 3) {
      if (SeqIdIn (SeqLocId (sgp->loc), ggp->bsp->id)) {
        gip = (GphItemPtr) MemNew (sizeof (GphItem));
        if (gip == NULL) return TRUE;
        gip->sgp = sgp;
        gip->left = GetOffsetInBioseq (sgp->loc, ggp->bsp, SEQLOC_LEFT_END);
        gip->right = GetOffsetInBioseq (sgp->loc, ggp->bsp, SEQLOC_RIGHT_END);
        ValNodeAddPointer (&(ggp->vnp), 0, (Pointer) gip);
      }
    }
  }
  return TRUE;
}

static int LIBCALLBACK SortSeqGraphProc (VoidPtr ptr1, VoidPtr ptr2)

{
  GphItemPtr  gip1, gip2;
  ValNodePtr  vnp1, vnp2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);
  if (vnp1 == NULL || vnp2 == NULL) return 0;
  gip1 = (GphItemPtr) vnp1->data.ptrvalue;
  gip2 = (GphItemPtr) vnp2->data.ptrvalue;
  if (gip1 == NULL || gip2 == NULL) return 0;
  if (gip1->left > gip2->left) {
    return 1;
  } else if (gip1->left < gip2->left) {
    return -1;
  } else if (gip1->right > gip2->right) {
    return -1;
  } else if (gip2->right < gip2->right) {
    return 1;
  }
  return 0;
}

/* gets valnode list of sorted graphs in GphItem structures */

static ValNodePtr GetSeqGraphsOnBioseq (Uint2 entityID, BioseqPtr bsp)

{
  GphGetData  ggd;
  Boolean     objMgrFilter [OBJ_MAX];

  ggd.vnp = NULL;
  ggd.bsp = bsp;
  MemSet ((Pointer) &objMgrFilter, 0, sizeof (objMgrFilter));
  objMgrFilter [OBJ_SEQGRAPH] = TRUE;
  GatherObjectsInEntity (entityID, 0, NULL, GetGraphsProc, (Pointer) &ggd, objMgrFilter);
  ggd.vnp = ValNodeSort (ggd.vnp, SortSeqGraphProc);
  return ggd.vnp;
}

/*
NLM_EXTERN void PrintQualityScores (BioseqPtr bsp, FILE *fp)

{
  ByteStorePtr  bs;
  Char          buf [41];
  Int4          curpos = 0, c80, i;
  Uint2         entityID;
  GphItemPtr    gip;
  ValNodePtr    head, vnp;
  SeqGraphPtr   sgp;
  Int2          val;

  if (bsp == NULL || fp == NULL) return;
  entityID = ObjMgrGetEntityIDForPointer (bsp);
  SeqIdWrite (bsp->id, buf, PRINTID_TEXTID_ACC_VER, sizeof (buf) - 1);
  head = GetSeqGraphsOnBioseq (entityID, bsp);
  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    gip = (GphItemPtr) vnp->data.ptrvalue;
    if (gip == NULL) continue;
    sgp = gip->sgp;
    fprintf (fp, ">%s %s (Length:%ld, Min: %ld, Max: %ld)\n", buf, sgp->title,
             (long) sgp->numval, (long) sgp->min.intvalue, (long) sgp->max.intvalue);
    bs = (ByteStorePtr) sgp->values;
    BSSeek (bs, 0, SEEK_SET);
    c80 = 0;
    for (i = 0; i < sgp->numval; i++) {
      val = (Int2) BSGetByte (bs);
      if (c80 == 20) {
        c80 = 0;
        fprintf (fp, "\n");
      }
      fprintf (fp, "%4d", (int) val);
      c80++;
    }
    fprintf (fp, "\n");
  }
  ValNodeFreeData (head);
}
*/

/*
NLM_EXTERN void PrintQualityScores (BioseqPtr bsp, FILE *fp)

{
  ByteStorePtr  bs;
  Char          buf [41];
  Int4          curpos = 0, c80, i, len = 0, min = INT4_MAX, max = INT4_MIN;
  Uint2         entityID;
  GphItemPtr    gip;
  ValNodePtr    head, vnp;
  SeqGraphPtr   sgp;
  CharPtr       title = NULL;
  Int2          val;

  if (bsp == NULL || fp == NULL) return;
  entityID = ObjMgrGetEntityIDForPointer (bsp);
  SeqIdWrite (bsp->id, buf, PRINTID_TEXTID_ACC_VER, sizeof (buf) - 1);
  head = GetSeqGraphsOnBioseq (entityID, bsp);

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    gip = (GphItemPtr) vnp->data.ptrvalue;
    if (gip == NULL) continue;
    sgp = gip->sgp;
    min = MIN ((Int4) min, (Int4) sgp->min.intvalue);
    max = MAX ((Int4) max, (Int4) sgp->max.intvalue);
    len += sgp->numval;
    if (title == NULL) {
      title = sgp->title;
    }
  }
  if (title == NULL) {
    title = "?";
  }
  len = bsp->length;
  fprintf (fp, ">%s %s (Length:%ld, Min: %ld, Max: %ld)\n", buf, title,
           (long) len, (long) min, (long) max);

  if (head == NULL) return;

  c80 = 0;

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    gip = (GphItemPtr) vnp->data.ptrvalue;
    if (gip == NULL) continue;
    sgp = gip->sgp;

    while (curpos < gip->left) {
      if (c80 == 20) {
        c80 = 0;
        fprintf (fp, "\n");
      }
      fprintf (fp, "%4d", (int) 0);
      curpos++;
      c80++;
    }

    bs = (ByteStorePtr) sgp->values;
    BSSeek (bs, 0, SEEK_SET);
    for (i = 0; i < sgp->numval; i++) {
      val = (Int2) BSGetByte (bs);
      if (c80 == 20) {
        c80 = 0;
        fprintf (fp, "\n");
      }
      fprintf (fp, "%4d", (int) val);
      curpos++;
      c80++;
    }
  }

  while (curpos < bsp->length) {
    if (c80 == 20) {
      c80 = 0;
      fprintf (fp, "\n");
    }
    fprintf (fp, "%4d", (int) 0);
    curpos++;
    c80++;
  }

  fprintf (fp, "\n");

  ValNodeFreeData (head);
}
*/

static void PrintQualProc (CharPtr buf, Uint4 buflen, Pointer userdata)

{
  FILE  *fp;

  fp = (FILE*) userdata;
  fprintf (fp, "%s", buf);
}

NLM_EXTERN void PrintQualityScores (BioseqPtr bsp, FILE *fp)

{
  PrintQualityScoresToBuffer (bsp, TRUE, (Pointer) fp, PrintQualProc);
}

NLM_EXTERN void PrintQualityScoresToBuffer (BioseqPtr bsp, Boolean gapIsZero, Pointer userdata, QualityWriteFunc callback)

{
  ByteStorePtr  bs;
  Char          id [41], buf [84], tmp [16];
  Int4          curpos = 0, c80, i, len = 0, min = INT4_MAX, max = INT4_MIN;
  Uint2         entityID;
  Int2          gap;
  GphItemPtr    gip;
  ValNodePtr    head, vnp;
  SeqGraphPtr   sgp;
  SeqIdPtr      sip;
  CharPtr       title = NULL, ptr;
  Int2          val;

  if (bsp == NULL || callback == NULL) return;
  entityID = ObjMgrGetEntityIDForPointer (bsp);
  head = GetSeqGraphsOnBioseq (entityID, bsp);

  /* skip bioseqs with no quality graphs */

  if (head == NULL) return;

  /* find accession */

  for (sip = bsp->id; sip != NULL &&
       sip->choice != SEQID_GENBANK && sip->choice != SEQID_EMBL &&
       sip->choice != SEQID_DDBJ && sip->choice != SEQID_OTHER;
       sip = sip->next) continue;
  if (sip == NULL) {
    sip = bsp->id;
  }
  SeqIdWrite (sip, id, PRINTID_TEXTID_ACC_VER, sizeof (id) - 1);

  if (gapIsZero) {
    gap = 0;
  } else {
    gap = -1;
  }

  /* get min, max, title, but currently won't use cumulative length */

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    gip = (GphItemPtr) vnp->data.ptrvalue;
    if (gip == NULL) continue;
    sgp = gip->sgp;
    min = MIN ((Int4) min, (Int4) sgp->min.intvalue);
    max = MAX ((Int4) max, (Int4) sgp->max.intvalue);
    len += sgp->numval;
    if (title == NULL) {
      title = sgp->title;
    }
  }
  if (title == NULL) {
    title = "?";
  }
  len = bsp->length; /* report full length of bioseq */
  if (min == INT4_MAX) {
    min = 0;
  }
  if (max == INT4_MIN) {
    max = 0;
  }
  sprintf (buf, ">%s %s (Length:%ld, Min: %ld, Max: %ld)\n", id, title,
           (long) len, (long) min, (long) max);
  callback (buf, sizeof (buf), userdata);

  c80 = 0;
  ptr = buf;
  buf [0] = '\0';

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    gip = (GphItemPtr) vnp->data.ptrvalue;
    if (gip == NULL) continue;
    sgp = gip->sgp;

    /* expand gaps by padding with 0s (optionally -1) */

    while (curpos < gip->left) {
      if (c80 == 20) {
        c80 = 0;
        ptr = StringMove (ptr, "\n");
        callback (buf, sizeof (buf), userdata);
        ptr = buf;
        buf [0] = '\0';
      }
      sprintf (tmp, "%3d", (int) gap);
      ptr = StringMove (ptr, tmp);
      curpos++;
      c80++;
    }

    /* now at proper position, write actual scores */

    bs = (ByteStorePtr) sgp->values;
    BSSeek (bs, 0, SEEK_SET);
    for (i = 0; i < sgp->numval; i++) {
      val = (Int2) BSGetByte (bs);
      if (c80 == 20) {
        c80 = 0;
        ptr = StringMove (ptr, "\n");
        callback (buf, sizeof (buf), userdata);
        ptr = buf;
        buf [0] = '\0';
      }
      if (val < 100) {
        sprintf (tmp, "%3d", (int) val);
      } else {
        sprintf (tmp, "%4d", (int) val);
      }
      ptr = StringMove (ptr, tmp);
      curpos++;
      c80++;
    }
  }

  /* expand any remaining space at end by padding with 0s (optionally -1) */

  while (curpos < bsp->length) {
    if (c80 == 20) {
      c80 = 0;
      ptr = StringMove (ptr, "\n");
      callback (buf, sizeof (buf), userdata);
      ptr = buf;
      buf [0] = '\0';
    }
    sprintf (tmp, "%3d", (int) gap);
    ptr = StringMove (ptr, tmp);
    curpos++;
    c80++;
  }

  ptr = StringMove (ptr, "\n");
  callback (buf, sizeof (buf), userdata);

  ValNodeFreeData (head);
}

NLM_EXTERN BytePtr GetScoresbySeqId (SeqIdPtr sip, Int4Ptr bsplength)

{
  ByteStorePtr  bs;
  BioseqPtr     bsp;
  Int4          curpos = 0, i;
  Uint2         entityID;
  GphItemPtr    gip;
  ValNodePtr    head, vnp;
  Int4          len;
  SeqGraphPtr   sgp;
  BytePtr       str = NULL;

  if (bsplength != NULL) {
    *bsplength = 0;
  }
  if (sip == NULL) return NULL;

  bsp = BioseqLockById (sip);
  if (bsp == NULL) return NULL;

  entityID = ObjMgrGetEntityIDForPointer (bsp);
  head = GetSeqGraphsOnBioseq (entityID, bsp);

  if (head != NULL && ISA_na (bsp->mol) && bsp->length < MAXALLOC) {
    str = MemNew (sizeof (Byte) * (bsp->length + 2));
    if (str != NULL) {

      len = bsp->length;
      if (bsplength != NULL) {
        *bsplength = len;
      }

      for (vnp = head; vnp != NULL; vnp = vnp->next) {
        gip = (GphItemPtr) vnp->data.ptrvalue;
        if (gip == NULL) continue;
        sgp = gip->sgp;

        /* expand gaps by padding with 0s (now 255) */

        while (curpos < gip->left && curpos < len) {
          str [curpos] = 255;
          curpos++;
        }

        /* now at proper position, write actual scores */

        bs = (ByteStorePtr) sgp->values;
        BSSeek (bs, 0, SEEK_SET);
        for (i = 0; i < sgp->numval && curpos < len; i++) {
          str [curpos] = (Byte) BSGetByte (bs);
          curpos++;
        }
      }

      /* expand any remaining space at end by padding with 0s (now 255) */

      while (curpos < len) {
        str [curpos] = 255;
        curpos++;
      }

    }
  }

  ValNodeFreeData (head);
  BioseqUnlock (bsp);
  return str;
}

NLM_EXTERN BytePtr GetScoresbyAccessionDotVersion (CharPtr accession, Int4Ptr bsplength)

{
  BytePtr   bs;
  SeqIdPtr  sip;

  if (bsplength != NULL) {
    *bsplength = 0;
  }
  if (StringHasNoText (accession)) return NULL;
  sip = SeqIdFromAccessionDotVersion (accession);
  if (sip == NULL) return NULL;

  bs = GetScoresbySeqId (sip, bsplength);
  sip = SeqIdFree (sip);
  return bs;
}

static void PrintAScore (
  FILE* fp,
  Int2 val,
  Int2Ptr linepos
)

{
  if (*linepos >= 20) {
    fprintf (fp, "\n");
    *linepos = 0;
  }
  if (val == 255) {
    val = -1;
  }
  fprintf (fp, "%3d", (int) val);
  (*linepos)++;
}

NLM_EXTERN void PrintQualityScoresForContig (
  BioseqPtr bsp,
  Boolean gapIsZero,
  FILE* fp
)

{
  Char         accn [41];
  BytePtr      bp;
  DeltaSeqPtr  dsp;
  Int2         gap;
  Int4         i;
  Int4         len;
  Int2         linepos = 0;
  SeqIdPtr     sip;
  SeqLitPtr    slitp;
  SeqLocPtr    slp;
  Int4         tstart, tstop;

  if (bsp == NULL || fp == NULL) return;
  if (bsp->repr != Seq_repr_delta || bsp->seq_ext_type != 4 || bsp->seq_ext == NULL) return;

  /* find accession */

  for (sip = bsp->id; sip != NULL &&
       sip->choice != SEQID_GENBANK && sip->choice != SEQID_EMBL &&
       sip->choice != SEQID_DDBJ && sip->choice != SEQID_OTHER;
       sip = sip->next) continue;
  if (sip == NULL) {
    sip = bsp->id;
  }
  SeqIdWrite (sip, accn, PRINTID_TEXTID_ACC_VER, sizeof (accn) - 1);
  fprintf (fp, ">%s\n", accn);

  if (gapIsZero) {
    gap = 0;
  } else {
    gap = -1;
  }

  for (dsp = (DeltaSeqPtr) bsp->seq_ext; dsp != NULL; dsp = dsp->next) {
    if (dsp->choice == 1) {

      slp = (SeqLocPtr) dsp->data.ptrvalue;
      if (slp == NULL || slp->choice == SEQLOC_NULL) continue;

      sip = SeqLocId (slp);
      if (sip == NULL) continue;

      /*
      if (sip->choice == SEQID_GI) {
        gi = sip->data.intvalue;
        accn [0] = '\0';
      } else {
        SeqIdWrite (sip, accn, PRINTID_TEXTID_ACC_VER, sizeof (accn) - 1);
        gi = 0;
      }
      */
      bp = GetScoresbySeqId (sip, &len);
      if (bp == NULL) {
        len = SeqLocLen (slp);
        for (i = 0; i < len; i++) {
          PrintAScore (fp, gap, &linepos);
        }
        continue;
      }

      tstart = SeqLocStart (slp);
      tstop = SeqLocStop (slp);

      len = tstop - tstart + 1;
      if (len == SeqLocLen (slp)) {
        if (SeqLocStrand (slp) == Seq_strand_minus) {
          for (i = tstop; i >= tstart; i--) {
            PrintAScore (fp, bp [i], &linepos);
          }
        } else {
          for (i = tstart; i <= tstop; i++) {
            PrintAScore (fp, bp [i], &linepos);
          }
        }
      }

      MemFree (bp);

    } else if (dsp->choice == 2) {

      slitp = (SeqLitPtr) dsp->data.ptrvalue;
      if (slitp == NULL || slitp->seq_data != NULL) continue;
      for (i = 0; i < slitp->length; i++) {
        PrintAScore (fp, gap, &linepos);
      }
    }
  }

  fprintf (fp, "\n");
}

typedef struct phrapdata {
  BioseqPtr  bsp;
  Int4       length;
  BytePtr    scores;
} PhrapData, PNTR PhrapDataPtr;

NLM_EXTERN SeqAnnotPtr PhrapGraphForContig (
  BioseqPtr bsp
)

{
  ByteStorePtr  bs;
  BytePtr       bp, str = NULL, ptr, tmp;
  Byte          by;
  DeltaSeqPtr   dsp;
  Int4          i, len, tstart, tstop;
  Int2          max = INT2_MIN;
  Int2          min = INT2_MAX;
  BioseqPtr     pbsp;
  PhrapDataPtr  pdp;
  ValNodePtr    phplist = NULL, vnp;
  SeqAnnotPtr   sap = NULL;
  SeqGraphPtr   sgp, lastsgp = NULL;
  SeqIntPtr     sintp;
  SeqIdPtr      sip;
  SeqLitPtr     slitp;
  SeqLocPtr     slp;

  if (bsp == NULL) return NULL;
  if (bsp->repr != Seq_repr_delta || bsp->seq_ext_type != 4 || bsp->seq_ext == NULL) return NULL;
  if ((! ISA_na (bsp->mol)) || bsp->length >= MAXALLOC) return NULL;

  str = MemNew (sizeof (Byte) * (bsp->length + 2));
  if (str == NULL) return NULL;
  ptr = str;

  /* lock all components once, get uniqued list of component Bioseqs and scores */

  for (dsp = (DeltaSeqPtr) bsp->seq_ext; dsp != NULL; dsp = dsp->next) {
    if (dsp->choice == 1) {

      slp = (SeqLocPtr) dsp->data.ptrvalue;
      if (slp == NULL || slp->choice == SEQLOC_NULL) continue;

      sip = SeqLocId (slp);
      if (sip == NULL) continue;

      pbsp = BioseqLockById (sip);
      if (pbsp == NULL) continue;

      for (vnp = phplist; vnp != NULL; vnp = vnp->next) {
        pdp = (PhrapDataPtr) vnp->data.ptrvalue;
        if (SeqIdIn (sip, pdp->bsp->id)) break;
      }
      if (vnp == NULL) {
        pdp = (PhrapDataPtr) MemNew (sizeof (PhrapData));
        if (pdp == NULL) continue;
        pdp->bsp = pbsp;
        pdp->scores = GetScoresbySeqId (pbsp->id, &(pdp->length));
        ValNodeAddPointer (&phplist, 0, (Pointer) pdp);
      } else {
        BioseqUnlock (pbsp);
      }
    }
  }

  /* build master byte array of scores */

  for (dsp = (DeltaSeqPtr) bsp->seq_ext; dsp != NULL; dsp = dsp->next) {
    if (dsp->choice == 1) {

      slp = (SeqLocPtr) dsp->data.ptrvalue;
      if (slp == NULL || slp->choice == SEQLOC_NULL) continue;

      sip = SeqLocId (slp);
      if (sip == NULL) continue;

      bp = NULL;
      for (vnp = phplist; vnp != NULL; vnp = vnp->next) {
        pdp = (PhrapDataPtr) vnp->data.ptrvalue;
        if (SeqIdIn (sip, pdp->bsp->id)) {
          bp = pdp->scores;
          break;
        }
      }
      if (bp == NULL) {
        len = SeqLocLen (slp);
        for (i = 0; i < len; i++) {
          *ptr = 255;
          ptr++;
        }
        continue;
      }

      tstart = SeqLocStart (slp);
      tstop = SeqLocStop (slp);

      len = tstop - tstart + 1;
      if (len == SeqLocLen (slp)) {
        if (SeqLocStrand (slp) == Seq_strand_minus) {
          for (i = tstop; i >= tstart; i--) {
            *ptr = bp [i];
            ptr++;
          }
        } else {
          for (i = tstart; i <= tstop; i++) {
            *ptr = bp [i];
            ptr++;
          }
        }
      }

      MemFree (bp);

    } else if (dsp->choice == 2) {

      slitp = (SeqLitPtr) dsp->data.ptrvalue;
      if (slitp == NULL || slitp->seq_data != NULL) continue;
      for (i = 0; i < slitp->length; i++) {
        *ptr = 255;
        ptr++;
      }
    }
  }

  /* now make graphs */

  i = 0;
  ptr = str;
  while (i < bsp->length) {
    by = *ptr;
    while (by == 255 && i < bsp->length) {
      i++;
      ptr++;
      by = *ptr;
    }
    if (i < bsp->length) {
      tstart = i;
      tmp = ptr;
      len = 0;
      max = INT2_MIN;
      min = INT2_MAX;
      while (by != 255 && i < bsp->length) {
        max = MAX (max, (Int2) by);
        min = MIN (min, (Int2) by);
        len++;
        i++;
        ptr++;
        by = *ptr;
      }
      tstop = i;
      sgp = SeqGraphNew ();
      if (sgp != NULL) {
        bs = BSNew (len + 1);
        if (bs != NULL) {
          BSWrite (bs, (Pointer) tmp, (Int4) len);
          sgp->numval = BSLen (bs);
          BSPutByte (bs, EOF);
          sgp->title = StringSave ("Phrap Quality");
          if (bsp->length != sgp->numval) {
            sgp->flags [0] = 1;
            sgp->compr = (len) / sgp->numval;
          } else {
            sgp->flags [0] = 0;
            sgp->compr = 1;
          }
          sgp->flags [1] = 0;
          sgp->flags [2] = 3;
          sgp->axis.intvalue = 0;
          sgp->min.intvalue = min;
          sgp->max.intvalue = max;
          sgp->a = 1.0;
          sgp->b = 0;
          sgp->values = (Pointer) bs;

          sintp = SeqIntNew ();
          sintp->from = tstart;
          sintp->to = tstop - 1;
          sintp->id = SeqIdDup (bsp->id);
          ValNodeAddPointer (&(sgp->loc), SEQLOC_INT, (Pointer) sintp);

          if (lastsgp != NULL) {
            lastsgp->next = sgp;
          }
          lastsgp = sgp;

          if (sap == NULL) {
            sap = SeqAnnotNew ();
            if (sap != NULL) {
              ValNodeAddPointer (&(sap->desc), Annot_descr_name, StringSave ("Graphs"));
              sap->type = 3;
              sap->data = (Pointer) sgp;
            }
          }
        }
      }
    }
  }

  /*
  sgp = SeqGraphNew ();
  if (sgp != NULL) {
    bs = BSNew (bsp->length);
    if (bs != NULL) {
      BSWrite (bs, (Pointer) str, (Int4) bsp->length);
      sgp->numval = BSLen (bs);
      BSPutByte (bs, EOF);
      sgp->title = StringSave ("Phrap Quality");
      if (bsp->length != sgp->numval) {
        sgp->flags [0] = 1;
        sgp->compr = (bsp->length) / sgp->numval;
      } else {
        sgp->flags [0] = 0;
        sgp->compr = 1;
      }
      sgp->flags [1] = 0;
      sgp->flags [2] = 3;
      sgp->axis.intvalue = 0;
      sgp->min.intvalue = min;
      sgp->max.intvalue = max;
      sgp->a = 1.0;
      sgp->b = 0;
      sgp->values = (Pointer) bs;

      sintp = SeqIntNew ();
      sintp->from = 0;
      sintp->to = bsp->length - 1;
      sintp->id = SeqIdDup (bsp->id);
      ValNodeAddPointer (&(sgp->loc), SEQLOC_INT, (Pointer) sintp);

      if (lastsgp != NULL) {
        lastsgp->next = sgp;
      }
      lastsgp = sgp;

      if (sap == NULL) {
        sap = SeqAnnotNew ();
        if (sap != NULL) {
          ValNodeAddPointer (&(sap->desc), Annot_descr_name, StringSave ("Graphs"));
          sap->type = 3;
          sap->data = (Pointer) sgp;
        }
      }
    }
  }
  */

  /* remove remaining single lock from components */

  for (vnp = phplist; vnp != NULL; vnp = vnp->next) {
    pdp = (PhrapDataPtr) vnp->data.ptrvalue;
    BioseqUnlock (pdp->bsp);
  }
  ValNodeFreeData (phplist);

  /* free master byte array */

  MemFree (str);

  return sap;
}

