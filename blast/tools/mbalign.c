/* $Id: mbalign.c,v 6.35 2001/12/28 20:38:41 dondosha Exp $
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
* File Name:  $RCSfile: mbalign.c,v $
*
* Author:  Webb Miller and Co.
* Adopted for NCBI standard libraries by Sergey Shavirin
*
* Initial Creation Date: 10/27/1999
*
* $Revision: 6.35 $
*
* File Description:
*        Alignment functions for Mega Blast program
*
* $Log: mbalign.c,v $
* Revision 6.35  2001/12/28 20:38:41  dondosha
* Moved Mega BLAST related parameters into a separate structure
*
* Revision 6.34  2001/11/14 23:43:40  dondosha
* Subject sequence now already in blastna encoding, no need to convert
*
* Revision 6.33  2001/09/18 16:49:24  dondosha
* Removed unneeded functions, eliminated mbutils.h header
*
* Revision 6.32  2001/07/20 19:20:47  dondosha
* Allow packed or unpacked subject sequence
*
* Revision 6.31  2001/05/23 20:14:47  dondosha
* Corrected the row shift in non-affine greedy algorithm
*
* Revision 6.30  2001/05/07 22:05:24  dondosha
* Small correction in affine extension
*
* Revision 6.29  2001/05/03 21:48:28  dondosha
* Handle some cases when memory allocation fails
*
* Revision 6.28  2001/05/02 21:32:16  dondosha
* Return null from GreedyAlignMemFree
*
* Revision 6.27  2001/03/30 22:12:11  dondosha
* Improved protection from moving beyond sequence boundaries
*
* Revision 6.26  2001/02/12 19:47:49  dondosha
* Make space for the greedy algorithm a linked list, to allow its extention
*
* Revision 6.25  2001/01/25 18:50:53  dondosha
* Check for row going out of bounds in greedy extension routines
*
* Revision 6.24  2001/01/11 20:22:16  dondosha
* Allow for expansion of the space array for gapped extensions
*
* Revision 6.23  2000/12/07 17:45:14  dondosha
* Use actual subject sequence length in GreedyAlignMemAlloc for 2 Sequences engine
*
* Revision 6.22  2000/11/29 16:31:20  dondosha
* Database sequence length cannot be more than MAX_DBSEQ_LEN
*
* Revision 6.21  2000/11/03 20:58:35  dondosha
* Changed parameter one_line_results to no_traceback
*
* Revision 6.20  2000/10/13 16:57:16  dondosha
* Corrected behavior at the border between queries
*
* Revision 6.19  2000/10/02 14:48:49  dondosha
* Bug in previous change
*
* Revision 6.18  2000/09/29 22:16:48  dondosha
* Check if memory allocation failed in GreedyAlignMemAlloc
*
* Revision 6.17  2000/09/28 14:26:34  dondosha
* Moved all memory allocation away from the low level greedy algorithm
*
* Revision 6.16  2000/08/28 17:15:36  dondosha
* The sentinel value between queries changed to gap value
*
* Revision 6.15  2000/08/03 22:15:29  dondosha
* Use correct default gap extension parameter when allocating memory for affine case
*
* Revision 6.14  2000/05/24 19:51:28  dondosha
* Do not use readdb when megablast is used for two sequences
*
* Revision 6.13  2000/05/17 17:12:39  dondosha
* Removed unused variable
*
* Revision 6.12  2000/05/12 19:45:32  dondosha
* Subject sequence is now passed in ncbi4na encoding
*
* Revision 6.11  2000/05/01 21:23:40  dondosha
* Changed greedy_align_basic to MegaBlastGreedyAlign, added MegaBlastAffineGreedyAlign
*
* Revision 6.10  2000/04/19 18:53:31  dondosha
* Bug fix: allow to extend through gap and masked characters, but not end-points
*
* Revision 6.9  2000/04/07 18:52:32  dondosha
* Do not allocate new space for flast_d[0,1] every time
*
* Revision 6.8  2000/04/05 16:30:07  dondosha
* Fixed a bug that allowed crossing the end of a strand or query
*
* Revision 6.7  2000/03/30 20:38:33  dondosha
* Returned one modification from 6.5 that was lost in 6.6
*
* Revision 6.6  2000/03/29 22:03:14  dondosha
* Made edit_script_new and edit_script_append public for use in MegaBlastNtWordExtend
*
* Revision 6.5  2000/02/01 22:42:11  dondosha
* Moved some definitions to mbalign.h; added 3 new routines
*
* Revision 6.4  2000/01/26 16:39:18  beloslyu
* change space_t to space_type because HP-UX compiler got confused with ANSI C space_t
*
* Revision 6.3  1999/11/24 20:38:09  shavirin
* Added possibility to produce Traditional Blast Output.
*
* Revision 6.2  1999/11/03 19:53:57  shavirin
* Fixed problem with Realloc() function.
*
* Revision 6.1  1999/10/29 15:37:12  shavirin
* Initial revision.
*
*
* Initial revision.
*
* ==========================================================================
*/

#include <mbalign.h>
#include <blastdef.h>

/* -------- From original file edit.c ------------- */

static Uint4 edit_val_get(edit_op_t op)
{
    return op >> 2;
}
static Uint4 edit_opc_get(edit_op_t op)
{
    return op & EDIT_OP_MASK;
}

static edit_op_t edit_op_cons(Uint4 op, Uint4 val)
{
    return (val << 2) | (op & EDIT_OP_MASK);
}

static edit_op_t edit_op_inc(edit_op_t op, Uint4 n)
{
    return edit_op_cons(edit_opc_get(op), edit_val_get(op) + n);
}

static edit_op_t edit_op_inc_last(edit_script_t *es, Uint4 n)
{
    edit_op_t *last;
    ASSERT (es->num > 0);
    last = &(es->op[es->num-1]);
    *last = edit_op_inc(*last, n);
    return *last;
}

static Int4 edit_script_ready(edit_script_t *es, Uint4 n)
{
    edit_op_t *p;
    Uint4 m = n + n/2;
    
    if (es->size <= n) {
        p = Realloc(es->op, m*sizeof(edit_op_t));
        if (p == 0) {
            return 0;
        } else {
            es->op = p;
            es->size = m;
        }
    }
    return 1;
}

static Int4 edit_script_readyplus(edit_script_t *es, Uint4 n)
{
    if (es->size - es->num <= n)
        return edit_script_ready(es, n + es->num);
    return 1;
}

static Int4 edit_script_put(edit_script_t *es, Uint4 op, Uint4 n)
{
    if (!edit_script_readyplus(es, 2))
        return 0;
    es->last = op;
    ASSERT(op != 0);
    es->op[es->num] = edit_op_cons(op, n);
    es->num += 1;
    es->op[es->num] = 0; /* sentinal */

    return 1;
}

static edit_script_t *edit_script_init(edit_script_t *es)
{
	es->op = 0;
	es->size = es->num = 0;
	es->last = 0;
	edit_script_ready(es, 8);
	return es;
}
static edit_op_t *edit_script_first(edit_script_t *es)
{
    return es->num > 0 ? &es->op[0] : 0;
}

static edit_op_t *edit_script_next(edit_script_t *es, edit_op_t *op)
{
    /* XXX - assumes flat address space */
    if (&es->op[0] <= op && op < &es->op[es->num-1])
        return op+1;
    else
        return 0;
}
static Int4 edit_script_more(edit_script_t *data, Uint4 op, Uint4 k)
{
    if (op == EDIT_OP_ERR) {
        ErrPostEx(SEV_FATAL, 0, 0, 
                  "edit_script_more: bad opcode %d:%d", op, k);
        return -1;
    }
    
    if (edit_opc_get(data->last) == op)
        edit_op_inc_last(data, k);
    else
        edit_script_put(data, op, k);

    return 0;
}
/* External */
edit_script_t *edit_script_append(edit_script_t *es, edit_script_t *et)
{
    edit_op_t *op;
    
    for (op = edit_script_first(et); op; op = edit_script_next(et, op))
        edit_script_more(es, edit_opc_get(*op), edit_val_get(*op));

    return es;
}

/* External */
edit_script_t *edit_script_new(void)
{
    edit_script_t *es = MemNew(sizeof(*es));
    if (!es)
        return 0;

    return edit_script_init(es);
}

/* External */
edit_script_t *edit_script_free(edit_script_t *es)
{
    if (es) {
        if (es->op)
            MemFree(es->op);
        MemSet(es, 0, sizeof(*es));
        MemFree(es);
    }
    return 0;
}

Int4 edit_script_del(edit_script_t *data, Uint4 k)
{
    return edit_script_more(data, EDIT_OP_DEL, k);
}

Int4 edit_script_ins(edit_script_t *data, Uint4 k)
{
    return edit_script_more(data, EDIT_OP_INS, k);
}
Int4 edit_script_rep(edit_script_t *data, Uint4 k)
{
    return edit_script_more(data, EDIT_OP_REP, k);
}

edit_script_t *edit_script_reverse_inplace(edit_script_t *es)
{
    Uint4 i;
    const Uint4 num = es->num;
    const Uint4 mid = num/2;
    const Uint4 end = num-1;
    
    for (i = 0; i < mid; ++i) {
        const edit_op_t t = es->op[i];
        es->op[i] = es->op[end-i];
        es->op[end-i] = t;
    }
    return es;
}

MBSpacePtr new_mb_space()
{
    MBSpacePtr p;
    Int4 amount;
    
    p = Nlm_Malloc(sizeof(MBSpace));
    amount = MAX_SPACE;
    p->space_array = Nlm_Malloc(sizeof(ThreeVal)*amount);
    if (p->space_array == NULL) {
       p = MemFree(p);
       return NULL;
    }
    p->used = 0; 
    p->size = amount;
    p->next = NULL;

    return p;
}

void refresh_mb_space(MBSpacePtr sp)
{
   while (sp) {
      sp->used = 0;
      sp = sp->next;
   }
}

void free_mb_space(MBSpacePtr sp)
{
   MBSpacePtr next_sp;

   while (sp) {
      next_sp = sp->next;
      sp->space_array = MemFree(sp->space_array);
      sp = MemFree(sp);
      sp = next_sp;
   }
}

ThreeValPtr get_mb_space(MBSpacePtr S, Int4 amount)
{
    ThreeValPtr s;
    if (amount < 0) 
        return NULL;  
    
    while (S->used+amount > S->size) {
       if (S->next == NULL)
          if ((S->next = new_mb_space()) == NULL) {
	     ErrPostEx(SEV_WARNING, 0, 0, "Cannot get new space for greedy extension");
	     return NULL;
          }
       S = S->next;
    }

    s = S->space_array+S->used;
    S->used += amount;
    
    return s;
}
/* ----- */

static Int4 gcd(Int4 a, Int4 b)
{
    Int4 c;
    if (a < b) {
        c = a;
        a = b; b = c;
    }
    while ((c = a % b) != 0) {
        a = b; b = c;
    }

    return b;
}


static Int4 gdb3(Int4Ptr a, Int4Ptr b, Int4Ptr c)
{
    Int4 g;
    if (*b == 0) g = gcd(*a, *c);
    else g = gcd(*a, gcd(*b, *c));
    if (g > 1) {
        *a /= g;
        *b /= g;
        *c /= g;
    }
    return g;
}

static Int4 get_lastC(ThreeValPtr PNTR flast_d, Int4Ptr lower, Int4Ptr upper, 
                      Int4Ptr d, Int4 diag, Int4 Mis_cost, Int4Ptr row1)
{
    Int4 row;
    
    if (diag >= lower[(*d)-Mis_cost] && diag <= upper[(*d)-Mis_cost]) {
        row = flast_d[(*d)-Mis_cost][diag].C;
        if (row >= MAX(flast_d[*d][diag].I, flast_d[*d][diag].D)) {
            *d = *d-Mis_cost;
            *row1 = row;
            return sC;
        }
    }
    if (flast_d[*d][diag].I > flast_d[*d][diag].D) {
        *row1 = flast_d[*d][diag].I;
        return sI;
    } else {
        *row1 = flast_d[*d][diag].D;
        return sD;
    }
}

static Int4 get_last_ID(ThreeValPtr PNTR flast_d, Int4Ptr lower, Int4Ptr upper, 
                        Int4Ptr d, Int4 diag, Int4 GO_cost, 
                        Int4 GE_cost, Int4 IorD)
{
    Int4 ndiag; 
    Int4 row;

    if (IorD == sI) { ndiag = diag -1;} else ndiag = diag+1;
    if (ndiag >= lower[(*d)-GE_cost] && ndiag <=upper[(*d)-GE_cost]) 
        row = (IorD == sI)? flast_d[(*d)-GE_cost][ndiag].I: flast_d[(*d)-GE_cost][ndiag].D;
    else row = -100;
    if (ndiag >= lower[(*d)-GO_cost-GE_cost] && ndiag <= upper[(*d)-GO_cost-GE_cost] && row < flast_d[(*d)-GO_cost-GE_cost][ndiag].C) {
        *d = (*d)-GO_cost-GE_cost;
        return sC;
    }
    *d = (*d)-GE_cost;
    return IorD;
}

static Int4 get_lastI(ThreeValPtr PNTR flast_d, Int4Ptr lower, Int4Ptr upper, 
                      Int4Ptr d, Int4 diag, Int4 GO_cost, Int4 GE_cost)
{
    return get_last_ID(flast_d, lower, upper, d, diag, GO_cost, GE_cost, sI);
}


static int get_lastD(ThreeValPtr PNTR flast_d, Int4Ptr lower, Int4Ptr upper, 
                     Int4Ptr d, Int4 diag, Int4 GO_cost, Int4 GE_cost)
{
    return get_last_ID(flast_d, lower, upper, d, diag, GO_cost, GE_cost, sD);
}

/* --- From file align.c --- */
/* ----- */

static Int4 get_last(Int4 **flast_d, Int4 d, Int4 diag, Int4 *row1)
{
    if (flast_d[d-1][diag-1] > MAX(flast_d[d-1][diag], flast_d[d-1][diag+1])) {
        *row1 = flast_d[d-1][diag-1];
        return diag-1;
    } 
    if (flast_d[d-1][diag] > flast_d[d-1][diag+1]) {
        *row1 = flast_d[d-1][diag];
        return diag;
    }
    *row1 = flast_d[d-1][diag+1];
    return diag+1;
}


/* ------ Functions, that create SeqAlignPtr from gap_align_ptr */
BlastSearchBlkPtr GreedyAlignMemAlloc(BlastSearchBlkPtr search)
{
   Int4 max_d, max_d_1, Xdrop, d_diff, max_len, max_cost, gd, i;
   Int4 reward, penalty, gap_open, gap_extend;
   Int4 Mis_cost, GE_cost;
   
   if (search == NULL) 
      return search;
   
   if (search->sbp->reward % 2 == 1) {
      reward = 2*search->sbp->reward;
      penalty = -2*search->sbp->penalty;
      Xdrop = 2*search->pbp->gap_x_dropoff;
      gap_open = 2*search->pbp->gap_open;
      gap_extend = 2*search->pbp->gap_extend;
   } else {
      reward = search->sbp->reward;
      penalty = -search->sbp->penalty;
      Xdrop = search->pbp->gap_x_dropoff;
      gap_open = search->pbp->gap_open;
      gap_extend = search->pbp->gap_extend;
   }

   if (gap_open == 0 && gap_extend == 0)
      gap_extend = reward / 2 + penalty;
   else
      gap_open += gap_extend;

   if (search->rdfp)
      max_len = readdb_get_maxlen(search->rdfp);
   else
      max_len = search->subject->length;

   max_len = MIN(max_len, MAX_DBSEQ_LEN);
 
   max_d = (Int4) (max_len / ERROR_FRACTION + 1);

   search->abmp = (GreedyAlignMemPtr) MemNew(sizeof(GreedyAlignMem));

   if (search->pbp->gap_open==0 && search->pbp->gap_extend==0) {
      d_diff = ICEIL(Xdrop+reward/2, penalty+reward);
   
      search->abmp->flast_d = (Int4Ptr PNTR) Malloc((max_d + 2) * sizeof(Int4Ptr));
      if (search->abmp->flast_d == NULL) {
         search->abmp = MemFree(search->abmp);
         return search;
      }
      search->abmp->flast_d[0] = Malloc((max_d + max_d + 6) * sizeof(Int4) * 2);
      if (search->abmp->flast_d[0] == NULL) {
	 ErrPostEx(SEV_WARNING, 0, 0, "Failed to allocate %ld bytes for abmp", (max_d + max_d + 6) * sizeof(Int4) * 2);
         search->abmp = GreedyAlignMemFree(search->abmp);
         return search;
      }

      search->abmp->flast_d[1] = search->abmp->flast_d[0] + max_d + max_d + 6;
      search->abmp->flast_d_affine = NULL;
      search->abmp->uplow_free = NULL;
   } else {
      search->abmp->flast_d = NULL;
      Mis_cost = reward + penalty;
      GE_cost = gap_extend+reward/2;
      max_d_1 = max_d;
      max_d *= GE_cost;
      max_cost = MAX(Mis_cost, gap_open+GE_cost);
      gd = gdb3(&Mis_cost, &gap_open, &GE_cost);
      d_diff = ICEIL(Xdrop+reward/2, gd);
      search->abmp->uplow_free = MemNew(sizeof(Int4)*2*(max_d+1+max_cost));
      search->abmp->flast_d_affine = (ThreeValPtr PNTR) 
	 Malloc((MAX(max_d, max_cost) + 2) * sizeof(ThreeValPtr));
      if (!search->abmp->uplow_free || !search->abmp->flast_d_affine) {
         search->abmp = GreedyAlignMemFree(search->abmp);
         return search;
      }
      search->abmp->flast_d_affine[0] = 
	 MemNew((2*max_d_1 + 6) * sizeof(ThreeVal) * (max_cost+1));
      for (i = 1; i <= max_cost; i++)
	 search->abmp->flast_d_affine[i] = 
	    search->abmp->flast_d_affine[i-1] + 2*max_d_1 + 6;
      if (!search->abmp->flast_d_affine || !search->abmp->flast_d_affine[0])
         search->abmp = GreedyAlignMemFree(search->abmp);
   }
   search->abmp->max_row_free = Malloc(sizeof(Int4) * (max_d + 1 + d_diff));
   if (!search->pbp->mb_params->no_traceback)
     search->abmp->space = new_mb_space();
   if (!search->abmp->max_row_free ||
       (!search->pbp->mb_params->no_traceback && !search->abmp->space))
      /* Failure in one of the memory allocations */
      search->abmp = GreedyAlignMemFree(search->abmp);

   return search;
}

GreedyAlignMemPtr GreedyAlignMemFree(GreedyAlignMemPtr abmp)
{
   if (abmp->flast_d) {
      MemFree(abmp->flast_d[0]);
      MemFree(abmp->flast_d);
   } else {
      MemFree(abmp->flast_d_affine[0]);
      MemFree(abmp->flast_d_affine);
      MemFree(abmp->uplow_free);
   }
   MemFree(abmp->max_row_free);
   if (abmp->space)
      free_mb_space(abmp->space);
   abmp = MemFree(abmp);
   return abmp;
}

/*
	Version to search a (possibly) packed nucl. sequence against
an unpacked sequence.  s2 is the packed nucl. sequence.
s1 can be packed or unpacked. If rem == 4, then s1 is unpacked.
len2 corresponds to the unpacked (true) length.

 * Basic O(ND) time, O(N) space, alignment function. 
 * Parameters:
 *   s1, len1        - first sequence and its length
 *   s2, len2        - second sequence and its length
 *   reverse         - direction of alignment
 *   xdrop_threshold - 
 *   mismatch_cost   -
 *   e1, e2          - endpoint of the computed alignment
 *   edit_script     -
 *   rem             - offset shift within a packed sequence
 */
Int4 MegaBlastGreedyAlign(const UcharPtr s1, Int4 len1,
			  const UcharPtr s2, Int4 len2,
			  Boolean reverse, Int4 xdrop_threshold, 
			  Int4 match_cost, Int4 mismatch_cost,
			  Int4Ptr e1, Int4Ptr e2, 
			  GreedyAlignMemPtr abmp, edit_script_t *S,
                          Uint1 rem)
{
    Int4 col,			/* column number */
        d,				/* current distance */
        k,				/* current diagonal */
        flower, fupper,            /* boundaries for searching diagonals */
        row,		        /* row number */
        MAX_D, 			/* maximum cost */
        ORIGIN,
        return_val = 0;
    Int4Ptr PNTR flast_d = abmp->flast_d; /* rows containing the last d */
    Int4Ptr max_row;		/* reached for cost d=0, ... len1.  */
    
    Int4 X_pen = xdrop_threshold;
    Int4 M_half = match_cost/2;
    Int4 Op_cost = mismatch_cost + M_half*2;
    Int4 D_diff = ICEIL(X_pen+M_half, Op_cost);
    
    Int4 x, cur_max, b_diag = 0, best_diag = INT_MAX/2;
    Int4Ptr max_row_free = abmp->max_row_free;
    Char nlower = 0, nupper = 0;
    MBSpacePtr space = abmp->space;
    Int4 max_len = len2;
 
    MAX_D = (Int4) (len1/ERROR_FRACTION + 1);
    ORIGIN = MAX_D + 2;
    *e1 = *e2 = 0;
    
    if (reverse) {
       if (!(rem & 4)) {
          for (row = 0; row < len2 && row < len1 && 
                  (s2[len2-1-row] ==
                   READDB_UNPACK_BASE_N(s1[(len1-1-row)/4], 
                                        3-(len1-1-row)%4)); 
               row++)
             /*empty*/ ;
       } else {
          for (row = 0; row < len2 && row < len1 && (s2[len2-1-row] == s1[len1-1-row]); row++)
             /*empty*/ ;
       }
    } else {
       if (!(rem & 4)) {
          for (row = 0; row < len2 && row < len1 && 
                  (s2[row] == 
                   READDB_UNPACK_BASE_N(s1[(row+rem)/4], 
                                        3-(row+rem)%4)); 
               row++)
             /*empty*/ ;
       } else {
          for (row = 0; row < len2 && row < len1 && (s2[row] == s1[row]); row++)
             /*empty*/ ;
       }
    }
    *e1 = row;
    *e2 = row;
    if (row == len1) {
        if (S != NULL)
            edit_script_rep(S, row);
	/* hit last row; stop search */
	return 0;
    }
    if (S==NULL) 
       space = 0;
    else 
       refresh_mb_space(space);
    
    max_row = max_row_free + D_diff;
    for (k = 0; k < D_diff; k++)
	max_row_free[k] = 0;
    
    flast_d[0][ORIGIN] = row;
    max_row[0] = (row + row)*M_half;
    
    flower = ORIGIN - 1;
    fupper = ORIGIN + 1;

    d = 1;
    while (d <= MAX_D) {
	Int4 fl0, fu0;
	flast_d[d - 1][flower - 1] = flast_d[d - 1][flower] = -1;
	flast_d[d - 1][fupper] = flast_d[d - 1][fupper + 1] = -1;
	x = max_row[d - D_diff] + Op_cost * d - X_pen;
	x = ICEIL(x, M_half);	
	cur_max = 0;
	fl0 = flower;
	fu0 = fupper;
	for (k = fl0; k <= fu0; k++) {
	    row = MAX(flast_d[d - 1][k + 1], flast_d[d - 1][k]) + 1;
	    row = MAX(row, flast_d[d - 1][k - 1]);
	    col = row + k - ORIGIN;
	    if (row + col >= x)
		fupper = k;
	    else {
		if (k == flower)
		    flower++;
		else
		    flast_d[d][k] = -1;
		continue;
	    }
            
            if (row > max_len || row < 0) {
                  flower = k+1; nlower = 1;
            } else {
               /* Slide down the diagonal. Don't do this if reached 
                  the end point, which has value 0x0f */
               if (reverse) {
                  if (s2[len2-row] != 0x0f) {
                     if (!(rem & 4))
                     while (row < len2 && col < len1 && s2[len2-1-row] == 
                            READDB_UNPACK_BASE_N(s1[(len1-1-col)/4],
                                                 3-(len1-1-col)%4)) {
                        ++row;
                        ++col;
                     }
                     else
                      while (row < len2 && col < len1 && s2[len2-1-row] == s1[len1-1-col]) {
                        ++row;
                        ++col;
                     }
                  } else {
                     max_len = row;
                     flower = k+1; nlower = 1;
                  }
               } else if (s2[row-1] != 0x0f) { 
                  if (!(rem & 4)) {
                     while (row < len2 && col < len1 && s2[row] == 
                            READDB_UNPACK_BASE_N(s1[(col+rem)/4],
                                                 3-(col+rem)%4)) {
                        ++row;
                        ++col;
                     }
                  } else {
                     while (row < len2 && col < len1 && s2[row] == s1[col]) {
                        ++row;
                        ++col;
                     }
                  }
               } else {
                  max_len = row;
                  flower = k+1; nlower = 1;
               }
            }
	    flast_d[d][k] = row;
	    if (row + col > cur_max) {
		cur_max = row + col;
		b_diag = k;
	    }
	    if (row == len2) {
		flower = k+1; nlower = 1;
	    }
	    if (col == len1) {
		fupper = k-1; nupper = 1;
	    }
	}
	k = cur_max*M_half - d * Op_cost;
	if (max_row[d - 1] < k) {
	    max_row[d] = k;
	    return_val = d;
	    best_diag = b_diag;
	    *e2 = flast_d[d][b_diag];
	    *e1 = (*e2)+b_diag-ORIGIN;
	} else {
	    max_row[d] = max_row[d - 1];
	}
	if (flower > fupper)
	    break;
	d++;
	if (!nlower) flower--; 
	if (!nupper) fupper++;
	if (S==NULL)
	   flast_d[d] = flast_d[d - 2];
	else {
           /* space array consists of ThreeVal structures which are 
              3 times larger than Int4, so divide requested amount by 3
           */
	   flast_d[d] = (Int4Ptr) get_mb_space(space, (fupper-flower+7)/3);
	   if (flast_d[d] != NULL)
              flast_d[d] = flast_d[d] - flower + 2;
           else
	      return return_val;
        }
    }
    
    if (S!=NULL) { /*trace back*/
        Int4 row1, col1, diag1, diag;
        d = return_val; diag = best_diag;
        row = *e2; col = *e1;
        while (d > 0) {
            diag1 = get_last(flast_d, d, diag, &row1);
            col1 = row1+diag1-ORIGIN;
            if (diag1 == diag) {
                if (row-row1 > 0) edit_script_rep(S, row-row1);
            } else if (diag1 < diag) {
                if (row-row1 > 0) edit_script_rep(S, row-row1);
                edit_script_ins(S,1);
            } else {
                if (row-row1-1> 0) edit_script_rep(S, row-row1-1);
                edit_script_del(S, 1);
            }
            d--; diag = diag1; col = col1; row = row1;
        }
        edit_script_rep(S, flast_d[0][ORIGIN]);
        if (!reverse) 
            edit_script_reverse_inplace(S);
    }
    return return_val;
}


Int4 MegaBlastAffineGreedyAlign (const UcharPtr s1, Int4 len1, 
				 const UcharPtr s2, Int4 len2,
				 Boolean reverse, Int4 xdrop_threshold, 
				 Int4 match_score, Int4 mismatch_score, 
				 Int4 gap_open, Int4 gap_extend,
				 Int4Ptr e1, Int4Ptr e2, 
				 GreedyAlignMemPtr abmp, edit_script_t *S,
                                 Uint1 rem)
{
    Int4 col,			/* column number */
        d,			/* current distance */
        k,			/* current diagonal */
        flower, fupper,         /* boundaries for searching diagonals */
        row,		        /* row number */
        MAX_D, 			/* maximum cost */
        ORIGIN,
        return_val = 0;
    ThreeValPtr PNTR flast_d;	/* rows containing the last d */
    Int4Ptr max_row_free = abmp->max_row_free;
    Int4Ptr max_row;		/* reached for cost d=0, ... len1.  */
    Int4 Mis_cost, GO_cost, GE_cost;
    Int4 D_diff, gd;
    Int4 M_half;
    Int4 max_cost;
    Int4 *lower, *upper;
    
    Int4 x, cur_max, b_diag = 0, best_diag = INT_MAX/2;
    Char nlower = 0, nupper = 0;
    MBSpacePtr space = abmp->space;
    Int4 stop_condition;
    Int4 max_d;
    Int4Ptr uplow_free;
    Int4 max_len = len2;
 
    if (match_score % 2 == 1) {
        match_score *= 2;
        mismatch_score *= 2;
        xdrop_threshold *= 2;
        gap_open *= 2;
	gap_extend *= 2;
    }
    M_half = match_score/2;

    if (gap_open == 0 && gap_extend == 0) {
       return MegaBlastGreedyAlign(s1, len1, s2, len2, reverse, 
                                   xdrop_threshold, match_score, 
                                   mismatch_score, e1, e2, abmp, S, rem);
    }
    
    gap_open += gap_extend;
    
    Mis_cost = mismatch_score + match_score;
    GO_cost = gap_open;
    GE_cost = gap_extend+M_half;
    gd = gdb3(&Mis_cost, &GO_cost, &GE_cost);
    D_diff = ICEIL(xdrop_threshold+M_half, gd);
    
    
    MAX_D = (Int4) (len1/ERROR_FRACTION + 1);
    max_d = MAX_D*GE_cost;
    ORIGIN = MAX_D + 2;
    max_cost = MAX(Mis_cost, GO_cost+GE_cost);
    *e1 = *e2 = 0;
    
    if (reverse) {
       if (!(rem & 4)) {
          for (row = 0; row < len2 && row < len1 && 
                  (s2[len2-1-row] ==
                   READDB_UNPACK_BASE_N(s1[(len1-1-row)/4], 
                                        3-(len1-1-row)%4)); 
               row++)
             /*empty*/ ;
       } else {
          for (row = 0; row < len2 && row < len1 && (s2[len2-1-row] == s1[len1-1-row]); row++)
             /*empty*/ ;
       }
    } else {
       if (!(rem & 4)) {
          for (row = 0; row < len2 && row < len1 && 
                  (s2[row] == 
                   READDB_UNPACK_BASE_N(s1[(row+rem)/4], 
                                        3-(row+rem)%4)); 
               row++)
             /*empty*/ ;
       } else {
          for (row = 0; row < len2 && row < len1 && (s2[row] == s1[row]); row++)
             /*empty*/ ;
       }
    }
    *e1 = row;
    *e2 = row;
    if (row == len1 || row == len2) {
        if (S != NULL)
            edit_script_rep(S, row);
	/* hit last row; stop search */
	return row*match_score;
    }
    flast_d = abmp->flast_d_affine;
    if (S==NULL) {
        space = 0;
    } else { 
        refresh_mb_space(space);
    }
    max_row = max_row_free + D_diff;
    for (k = 0; k < D_diff; k++)
	max_row_free[k] = 0;
    uplow_free = abmp->uplow_free;
    lower = uplow_free;
    upper = uplow_free+max_d+1+max_cost;
    /* next 3 lines set boundary for -1,-2,...,-max_cost+1*/
    for (k = 0; k < max_cost; k++) {lower[k] =LARGE;  upper[k] = -LARGE;}
    lower += max_cost;
    upper += max_cost; 
    
    flast_d[0][ORIGIN].C = row;
    flast_d[0][ORIGIN].I = flast_d[0][ORIGIN].D = -2;
    max_row[0] = (row + row)*M_half;
    lower[0] = upper[0] = ORIGIN;
    
    flower = ORIGIN - 1;
    fupper = ORIGIN + 1;
    
    d = 1;
    stop_condition = 1;
    while (d <= max_d) {
	Int4 fl0, fu0;
	x = max_row[d - D_diff] + gd * d - xdrop_threshold;
	x = ICEIL(x, M_half);
	if (x < 0) x=0;
	cur_max = 0;
	fl0 = flower;
	fu0 = fupper;
	for (k = fl0; k <= fu0; k++) {
	    row = -2;
	    if (k+1 <= upper[d-GO_cost-GE_cost] && k+1 >= lower[d-GO_cost-GE_cost]) 
                row = flast_d[d-GO_cost-GE_cost][k+1].C;
	    if (k+1  <= upper[d-GE_cost] && k+1 >= lower[d-GE_cost] &&
		row < flast_d[d-GE_cost][k+1].D) 
                row = flast_d[d-GE_cost][k+1].D;
	    row++;
	    if (row+row+k-ORIGIN >= x) 
	      flast_d[d][k].D = row;
	    else flast_d[d][k].D = -2;
	    row = -1; 
	    if (k-1 <= upper[d-GO_cost-GE_cost] && k-1 >= lower[d-GO_cost-GE_cost]) 
                row = flast_d[d-GO_cost-GE_cost][k-1].C;
	    if (k-1  <= upper[d-GE_cost] && k-1 >= lower[d-GE_cost] &&
		row < flast_d[d-GE_cost][k-1].I) 
                row = flast_d[d-GE_cost][k-1].I;
	    if (row+row+k-ORIGIN >= x) 
                flast_d[d][k].I = row;
	    else flast_d[d][k].I = -2;
            
	    row = MAX(flast_d[d][k].I, flast_d[d][k].D);
	    if (k <= upper[d-Mis_cost] && k >= lower[d-Mis_cost]) 
                row = MAX(flast_d[d-Mis_cost][k].C+1,row);
            
	    col = row + k - ORIGIN;
	    if (row + col >= x)
		fupper = k;
	    else {
		if (k == flower)
		    flower++;
		else
		    flast_d[d][k].C = -2;
		continue;
	    }
            if (row > max_len || row < -2) {
               flower = k; nlower = k+1; 
            } else {
               /* slide down the diagonal */
               if (reverse) {
                  if (s2[len2 - row] != 0x0f) {
                     if (!(rem & 4)) {
                        while (row < len2 && col < len1 && s2[len2-1-row] == 
                               READDB_UNPACK_BASE_N(s1[(len1-1-col)/4],
                                                    3-(len1-1-col)%4)) {
                           ++row;
                           ++col;
                        }
                     } else {
                        while (row < len2 && col < len1 && s2[len2-1-row] ==
                               s1[len1-1-col]) {
                           ++row;
                           ++col;
                        }
                     }
                  } else {
                     max_len = row;
                     flower = k; nlower = k+1; 
                  }
               } else if (s2[row-1] != 0x0f) {
                  if (!(rem & 4)) {
                     while (row < len2 && col < len1 && s2[row] == 
                            READDB_UNPACK_BASE_N(s1[(col+rem)/4],
                                                 3-(col+rem)%4)) {
                        ++row;
                        ++col;
                     }
                  } else {
                     while (row < len2 && col < len1 && s2[row] == s1[col]) {
                        ++row;
                        ++col;
                     }
                  }
               } else {
                  max_len = row;
                  flower = k; nlower = k+1;
               }
            }
	    flast_d[d][k].C = row;
	    if (row + col > cur_max) {
		cur_max = row + col;
		b_diag = k;
	    }
	    if (row == len2) {
		flower = k; nlower = k+1;
	    }
	    if (col == len1) {
		fupper = k; nupper = k-1;
	    }
	}
	k = cur_max*M_half - d * gd;
	if (max_row[d - 1] < k) {
	    max_row[d] = k;
	    return_val = d;
	    best_diag = b_diag;
	    *e2 = flast_d[d][b_diag].C;
	    *e1 = (*e2)+b_diag-ORIGIN;
	} else {
	    max_row[d] = max_row[d - 1];
	}
	if (flower <= fupper) {
            stop_condition++;
            lower[d] = flower; upper[d] = fupper;
	} else {
            lower[d] = LARGE; upper[d] = -LARGE;
	}
	if (lower[d-max_cost] <= upper[d-max_cost]) stop_condition--;
	if (stop_condition == 0) break;
	d++;
	flower = MIN(lower[d-Mis_cost], MIN(lower[d-GO_cost-GE_cost], lower[d-GE_cost])-1);
	if (nlower) flower = MAX(flower, nlower);
	fupper = MAX(upper[d-Mis_cost], MAX(upper[d-GO_cost-GE_cost], upper[d-GE_cost])+1);
	if (nupper) fupper = MIN(fupper, nupper);
	if (d > max_cost) {
	   if (S==NULL) {
	      /*if (d > max_cost)*/
	      flast_d[d] = flast_d[d - max_cost-1];
	   } else {
	      flast_d[d] = get_mb_space(space, fupper-flower+1)-flower;
	      if (flast_d[d] == NULL)
		 return return_val;
           }
	}
    }
    
    if (S!=NULL) { /*trace back*/
        Int4 row1, diag, state;
        d = return_val; diag = best_diag;
        row = *e2; state = sC;
        while (d > 0) {
            if (state == sC) {
                /* diag will not be changed*/
                state = get_lastC(flast_d, lower, upper, &d, diag, Mis_cost, &row1);
                if (row-row1 > 0) edit_script_rep(S, row-row1);
                row = row1;
            } else {
                if (state == sI) {
                    /*row unchanged */
                    state = get_lastI(flast_d, lower, upper, &d, diag, GO_cost, GE_cost);
                    diag--;
                    edit_script_ins(S,1);
                } else {
                    edit_script_del(S,1);
                    state = get_lastD(flast_d, lower, upper, &d, diag, GO_cost, GE_cost);
                    diag++;
                    row--;
                }
            }
        }
        edit_script_rep(S, flast_d[0][ORIGIN].C);
        if (!reverse) 
            edit_script_reverse_inplace(S);
    }
    return_val = max_row[return_val];
    return return_val;
}



/* ------------------------------------------------------------ */
