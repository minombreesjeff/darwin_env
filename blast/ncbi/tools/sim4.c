static char const rcsid[] = "$Id: sim4.c,v 6.5 2003/05/30 17:25:38 coulouri Exp $";

/* sim4.c - an algorithm to align a cDNA sequence with a genomic sequence. 

   *Note:  
         - when compiled with flag -DDEBUG, it gives information on the 
           (intermediate and final) block decomposition of  the sequences
         - when compiled with flag -DSTATS, it generates the actual
           alignment and gives the alignment scores on exon segments 
 */


/* #include <stdio.h> */
/* #include <string.h> */
/* #include <ctype.h> */
/* #include <math.h> */

#include <ncbi.h>
#include <simutil.h>

#define DEFAULT_W       8 
#define D_MASK          65535	/* distribution mask : 2^16-1 */ 
#define MIN_EXON_SIZE   W/2 
#define DIST_CUTOFF     4
#define PERCENT         .2 
#define CUTOFF          .2  
#define LL              60
#define M1              M-W+1
#define N1              N-W+1
#define NW              5	/* number of words used in the heuristic phase from sim3 */

#define MAXINT          65535
#define ALPHA           .5
#define BETA            .5


#define MININT   -99999
#define DEF_LIMIT 1000

typedef struct block {
  Int4 init_pos1;
  Int4 init_pos2;
  Int4 end_pos1;
  Int4 end_pos2;
  Int4 next;
} Block;



typedef struct table {
  Uint4 ecode;
  Int4 pos;
  struct table *next;
} Table;


typedef struct matchelt {
  Int4   mblock;
  Table *mref;
  struct matchelt *mnext;
} Matchelt;


typedef struct links {
  Matchelt *lptr;
  struct links *lnext;
} Links;

   
typedef struct threshelt {
  Int4   tblock;		/* block in seq2; a value between 1..M-W+1 */
  Table *tref;
} Threshelt;


struct edit {
  struct edit *link;		/* previous edit instruction */
  Int4 op;			/* INS or DEL */
  Int4 line1;			/* line number in file1 */
  Int4 line2;			/* line number in file2 */
};

typedef struct {
  Int1Ptr seq1, seq2;
  Int4 M, N, W;
  Int4 encoding[128];
  Int4 decoding[5];
  Int4 b_offset;
  Int4 limit;
} data_t;

#define MAX_BLOCKS      10000
typedef struct sim4global {	/*the global variables used in SIM4*/
	Block *Blist;
	Table ***pos_tbl;
	Table **lists;
	Links **Link;
	Matchelt **Matchlist;
}SIM4Global, PNTR SIM4GlobalPtr;


static Int4 segment(Int1Ptr, Int1Ptr, Int4, Int4, Int4, Int4, Int4, data_t *, SIM4GlobalPtr);
static void add_word(Uint4, Int4, SIM4GlobalPtr);
static Boolean init_tbl(Int4, SIM4GlobalPtr);
static void bld_table(Int1Ptr, Int4, Int4, data_t *, SIM4GlobalPtr);
static void bld_lists(Int1Ptr, Int4, Int4, data_t *, SIM4GlobalPtr);
static void create_mlists(Int4, Int4, Int4, SIM4GlobalPtr);
static void lisseq(Int4, Int4, Int4, Links **, SIM4GlobalPtr, ValNodePtr PNTR);
static Int4 btfind(Int4, Threshelt *, Int4);
static Int4 greedy(Int1Ptr, Int1Ptr, Int4, Int4, Int4, Int4,
		   Int4Ptr PNTR, Int4Ptr PNTR, Int4Ptr, data_t *, SIM4GlobalPtr);
static Int4 bgreedy(Int1Ptr, Int1Ptr, Int4, Int4, Int4, Int4, data_t *, SIM4GlobalPtr);
static Int4 bput_scr(struct edit *, Int4, Int4, Int4, Int4, Int4, Int4, Int4, SIM4GlobalPtr);
static Int4 fgreedy(Int1Ptr, Int1Ptr, Int4, Int4, Int4, Int4, data_t *, SIM4GlobalPtr);
static Int4 fput_scr(struct edit *, Int4, Int4, Int4, Int4, Int4, SIM4GlobalPtr);
static Int4 find_blocks PROTO((Int4 W, Links *lk, Int4 offset1, Int4 offset2, 
							  data_t *data, SIM4GlobalPtr sgp));
static Int4 power2(Int4 n); 
static Boolean sim4x(data_t *, SIM4GlobalPtr );
static Int4 snake(Int4, Int4, Int4, Int4, data_t *);
static Int4 rsnake(Int4, Int4, Int4, Int4, data_t *);
static void path(Int4, Int4, Int1, Int4, Int4, Int1, Int4,
		 EditScriptPtr PNTR, EditScriptPtr PNTR, data_t *);

static Int1 locate(Int4Ptr, Int4Ptr, Int4Ptr, Int4Ptr, Int1Ptr, Int1Ptr,
		   Int4, Int4, Int4, data_t *data);
static Int4 get_dist(Int4, Int4, Int4, Int4, data_t *data);
static void Condense_script(EditScriptPtr);
static void Reverse_Script(EditScriptPtr);

static EditScriptPtr SIM4 PROTO((Int1Ptr A, Int1Ptr B, Int4 len1, Int4 len2,
                    Int4 INPUT_W, Int4 LIMIT, Int4Ptr dist_ptr));
static void S2A(EditScriptPtr, Int4Ptr);
static void DISPLAY(Int1Ptr, Int1Ptr, Int4, Int4, Int4Ptr, Int4, Int4);
static void Print_Script(EditScriptPtr head, Int4 M, Int4 N, data_t *data);


SeqAlignPtr SIM4ALN_choice(SeqLocPtr cslp1, SeqLocPtr cslp2, Int4 limit, Int4 word_size)
{
	SeqAlignPtr sap = NULL;
	Boolean both_strand, is_dna;
	Uint1 strand_2, strand_1;
	CharPtr A, B;
	Int4 len1, len2;
	EditScriptPtr esp, c_esp;
	Int4 dist = -1, c_dist;
	Char buf[21];

 
	if(cslp1 == NULL || cslp2 == NULL)
	{
		ErrPostEx(SEV_WARNING, 0, 0, "Insufficient input");
		return NULL;
	}

	if(cslp1->choice != SEQLOC_INT || cslp2->choice !=SEQLOC_INT)
	{
		ErrPostEx(SEV_WARNING, 0, 0, "Incorrect type of Seq-loc. Only take Seq-int");
		return NULL;
	}
	strand_1 = SeqLocStrand(cslp1);
	strand_2 = SeqLocStrand(cslp2);
	both_strand = check_strand_mol(cslp2, &is_dna);
	if(!is_dna)
	{
		ErrPostEx(SEV_WARNING, 0, 0, "Sorry, SIM4 can only compare two DNA sequences");
		return NULL;
	}

	if(both_strand)
	{
		Change_Loc_Strand(cslp1, Seq_strand_plus);
		Change_Loc_Strand(cslp2, Seq_strand_plus);
	}

	A= make_sim_seq(cslp1, TRUE, NULL);
	if(A == NULL)
	{
		Change_Loc_Strand(cslp1, strand_1);
		Change_Loc_Strand(cslp2, strand_2);
		SeqIdWrite(SeqLocId(cslp1), buf, PRINTID_FASTA_LONG, 20);
      	ErrPostEx(SEV_WARNING, 0, 0, "Fail to make the first sequence %s", buf);
		return NULL;
	}

	B = make_sim_seq(cslp2, FALSE, NULL);
	if(B == NULL)
	{
		MemFree(A);
		Change_Loc_Strand(cslp1, strand_1);
		Change_Loc_Strand(cslp2, strand_2);
		SeqIdWrite(SeqLocId(cslp2), buf, PRINTID_FASTA_LONG, 20);
      	ErrPostEx(SEV_WARNING, 0, 0, "Fail to make the second sequence %s", buf);
		return NULL;
	}

	len1 = SeqLocLen(cslp1);
	len2 = SeqLocLen(cslp2);

	if(limit <= 0)
		limit = DEF_LIMIT;
	word_size = MAX(DEFAULT_W, word_size);
	esp = SIM4((Int1Ptr)(A+1), (Int1Ptr)(B+1), len1, len2,word_size, limit,&dist);
	if(both_strand)
	{
		Change_Loc_Strand(cslp2,(Uint1)2);
		make_sim_seq(cslp2, FALSE, B);
		c_esp = SIM4((Int1Ptr)(A+1), (Int1Ptr)(B+1), len1, len2,word_size, limit,&c_dist);
		if(c_esp != NULL)
		{
			if(esp == NULL || c_dist < dist)
			{
				Script_free(esp);
				esp = c_esp;
				dist = c_dist;
			}
			else
			{
				Change_Loc_Strand (cslp2, Seq_strand_plus);
				Script_free(c_esp);
			}
		}
		else
			Change_Loc_Strand (cslp2, Seq_strand_plus);
	}


	if(esp != NULL)
	{
		sap = ConvertEditScript(esp, cslp1, cslp2, 0, 0, FALSE);
		Script_free(esp);
	}
	else
		sap = NULL;
	Change_Loc_Strand(cslp1, strand_1);
	Change_Loc_Strand(cslp2, strand_2);
	MemFree(A);
	MemFree(B);
	return sap;

}


static Int4 power2(Int4 n)
{ 
  if(n < 0) {
     ErrPostEx(SEV_FATAL, 1, 0,
	       "Positive integer power only. Perhaps invalid data.");
     exit(1);
  }

  if(!n) return 1;
  else return 2 * power2(n - 1);
}

static Boolean init_tbl(Int4 W, SIM4GlobalPtr sgp)
{
  Int4 i, j, No_branches;

  sgp->Blist = MemGet((size_t)MAX_BLOCKS * sizeof(Block), MGET_ERRPOST);
  if(sgp->Blist == NULL)
	  return FALSE;
  MemSet((Pointer)(sgp->Blist), 0, MAX_BLOCKS * sizeof(Block));
  No_branches = power2(MAX(0, 2 * W - 16));
  sgp->pos_tbl = (Table ***)MemNew(No_branches * sizeof(Table **));

  for(i = 0; i < No_branches; i++) {
    sgp->pos_tbl[i] = (Table **)MemNew((D_MASK + 1) * sizeof(Table *));
    for(j = 0; j <= D_MASK; j++)
      sgp->pos_tbl[i][j] = NULL;
  }
  return TRUE;
}

static void bld_table(Int1Ptr s1, Int4 M, Int4 W, data_t *data, SIM4GlobalPtr sgp)
{
  Uint4 ecode;
  Int4 i, si, temp, mask;
  Int1Ptr q;

  /* initializing ... */

  for(i = 0; i < 128; i++) data->encoding[i] = 0;

  data->encoding['A'] = data->encoding['a'] = 0;
  data->encoding['C'] = data->encoding['c'] = 1;
  data->encoding['G'] = data->encoding['g'] = 2;
  data->encoding['T'] = data->encoding['t'] = 3;
  data->decoding[0] = 'A';
  data->decoding[1] = 'C';
  data->decoding[2] = 'G';
  data->decoding[3] = 'T';

  mask = (1 << (W + W - 2)) - 1;

  q = s1 - 1;
  ecode = 0L;
  for(si = i = 1; i < W; ++si)
    if((temp = data->encoding[*++q]) != 4) {
      ecode = (ecode << 2) + temp;
      i++;
    }
  for(; (*++q) && (si <= M); si++)  
    if((temp = data->encoding[*q]) != 4) {
      ecode = ((ecode & mask) << 2) + temp;
      add_word(ecode, (Int4)(q - s1 + 1), sgp);
    }
}

static void add_word(Uint4 ecode, Int4 pos, SIM4GlobalPtr sgp)
{
  Table *ptr;

  ptr = (Table *)MemNew(sizeof(Table));
  ptr->ecode = ecode;
  ptr->pos = pos;
  ptr->next = sgp->pos_tbl[ecode >> 16][ecode&D_MASK];
  sgp->pos_tbl[ecode >> 16][ecode&D_MASK] = ptr;
}

static void clean_up_global_val(SIM4GlobalPtr sgp)
{

	MemFree(sgp->Link);
	MemFree(sgp->lists);
	MemFree(sgp->Blist);

}

static EditScriptPtr SIM4(Int1Ptr in_seq1, Int1Ptr in_seq2,
		  Int4 in_M, Int4 in_N,
		  Int4 in_W, Int4 LIMIT, Int4Ptr dist_ptr)
{
  data_t data;
  Int4 sflag = FALSE;
  Int4 i, i1, diff;
  Int1Ptr aux;
  EditScriptPtr head, tail, left, right, new;
   SIM4Global sg;


  data.limit = LIMIT;

  /* initialize global variables ... */
  data.seq1 = in_seq1;
  data.seq2 = in_seq2;
  data.M = in_M;
  data.N = in_N;
  data.W = in_W;
  data.b_offset = 0;

  if(data.M > data.N) {
    aux = data.seq1;
    data.seq1 = data.seq2;
    data.seq2 = aux;

    i = data.N;
    data.N = data.M;
    data.M = i;

    sflag = TRUE;
  }
  
  /* Compute the distance between two sequences A and B */
  
  /* printf("A heuristic phase 1 is tried.\n");	*/

  if(!data.limit)
	  data.limit = DEF_LIMIT;
  /* printf("distance limit = %d\n", data.limit); */
  *dist_ptr = 0;

  if(!sim4x(&data, &sg))
	  return NULL;
  i = 0; head = NULL;

  while((i1 = sg.Blist[i].next) && sg.Blist[i1].end_pos1) {
    diff = get_dist(sg.Blist[i1].init_pos1 - 1,
		    sg.Blist[i1].init_pos2 - 1,
		    sg.Blist[i1].end_pos1,
		    sg.Blist[i1].end_pos2,
		    &data);
	if(diff == -1)
	{
		clean_up_global_val(&sg);
		Script_free(head);
		return NULL;
	}

    /* if(diff > MAX((Int4)(PERCENT * (sg.Blist[i1].end_pos1 - sg.Blist[i1].init_pos1 + 1)), 2 * data.W))
	  ErrPostEx(SEV_WARNING, 0, 0,
	       "Distance threshold on segment exceeded."); */

      /* printf("Warning: Distance threshold on segment exceeded.\n"); */

    path(sg.Blist[i1].init_pos1 - 1, sg.Blist[i1].init_pos2 - 1, SUB,
	 sg.Blist[i1].end_pos1, sg.Blist[i1].end_pos2, SUB,
	 diff, &left, &right, &data);
    *dist_ptr += diff;

    if((diff = sg.Blist[i].init_pos1 - sg.Blist[i1].end_pos1 - 1) != 0) {
      new = (EditScriptPtr) MemNew(sizeof(EditScript));
      new->op_type = DEL;
      new->num = diff;
      new->next = head;
      head = new;
      *dist_ptr += diff; 
    }
    if((diff = sg.Blist[i].init_pos2 - sg.Blist[i1].end_pos2 - 1) != 0) {
      new = (EditScriptPtr) MemNew(sizeof(EditScript));
      new->op_type = INS;
      new->num = diff;
      new->next = head;
      head = new;
    }
    right->next = head;
    head = left;
    i = i1;
  }
  if((diff = sg.Blist[i].init_pos2 - sg.Blist[i1].end_pos2 - 1) != 0) {
    new = (EditScriptPtr) MemNew(sizeof(EditScript));
    new->op_type = INS;
    new->num = diff;
    new->next = head;
    head = new;
  }
  if((diff = sg.Blist[i].init_pos1 - sg.Blist[i1].end_pos1 - 1) != 0) {
    new = (EditScriptPtr) MemNew(sizeof(EditScript));
    new->op_type = DEL;
    new->num = diff;
    new->next = head;
    head = new;
    *dist_ptr += diff;
  }

  /* Merge the contiguous same operations together */
  Condense_script(head);

  /* Reset tail (due to the condensation) */
  tail = head;
  while(tail->next != NULL) tail = tail->next;

#ifdef STATS
  /*printf("distance=%d\n", *dist_ptr); */
  if(*dist_ptr > limit)
	  ErrPostEx(SEV_WARNING, 0, 0, "distance exceeds limit!");
    /* printf("\nWarning: distance exceeds limit!\n\n"); */
#endif 

  if(sflag == TRUE) {    /* Switch back the sequences */
    aux = data.seq1;
    data.seq1 = data.seq2;
    data.seq2 = aux;
    i = data.M;
    data.M = data.N;
    data.N = i;
    Reverse_Script(head);
  }
  
  /* Print_Script(head, data.M, data.N, &data); */
  clean_up_global_val(&sg);

  return head;
}

/* locate possible exons and form block list */
static Boolean sim4x(data_t *data, SIM4GlobalPtr sgp)
{
  Int4 bno, i, i1, diff, dist;
  Int4Ptr last, first;

  Int4 I1, I2, J1, J2; 
  Int4 offset1, offset2;
  Int1 hflag;
  Int4 p1, p2, length1, length2;

#ifdef XFLAG
  Int4 ulcell, u, l;
  Int4 j, mcell, mismatch; 
  Int1 temp[50];
#endif

  if(data->M < data->W)  data->W = data->M;
  if(!init_tbl(data->W, sgp))
	  return FALSE;

  bno = segment(data->seq1, data->seq2, data->M, data->N, data->W, 0, 0, data, sgp);

  sgp->Blist[0].init_pos1 = data->M + 1;
  sgp->Blist[0].init_pos2 = data->N + 1;
  sgp->Blist[bno + 1].end_pos1 = sgp->Blist[bno + 1].end_pos2 = 0;
  sgp->Blist[bno].next = bno + 1;
  sgp->Blist[bno + 1].next = 0;
  data->b_offset = bno + 2;

  /* skip small blocks, as potential error source */  
  /* external thresholding: begin sequence */
  i = data->b_offset - 1;
  while(i && (sgp->Blist[i].end_pos1 - sgp->Blist[i].init_pos1 + 1 < 2.0 * data->W))
    i--;
  sgp->Blist[i].next = data->b_offset - 1;

  /* external thresholding: end sequence */
  i = sgp->Blist[0].next;
  while(sgp->Blist[i].end_pos1 && (sgp->Blist[i].end_pos1 - sgp->Blist[i].init_pos1 + 1 < 2 * data->W))
    i++;
  sgp->Blist[0].next = i; 

  /* internal thresholding */ 
  i = 0;  
  while((i1 = sgp->Blist[i].next) && (sgp->Blist[i1].end_pos1)) {
    if(sgp->Blist[i1].end_pos1 - sgp->Blist[i1].init_pos1 + 1 < data->W)
      sgp->Blist[i].next = sgp->Blist[i1].next;
/* =========   HERE STARTS THE NEW CODE =========== */

      else if (sgp->Blist[i1].end_pos1-sgp->Blist[i1].init_pos1+1<2*data->W) {

               Int4  tmp = sgp->Blist[i1].next;

               /* extend block i to the left */
               p1 = sgp->Blist[i].init_pos1-1;
               p2 = sgp->Blist[i].init_pos2-1;
               while ((p1>0) && (p2>0) &&
                      (p1>sgp->Blist[tmp].end_pos1-1) &&
                      (data->seq1[p1-1] == data->seq2[p2-1])) {
                          p1--; p2--;
               }
               sgp->Blist[i].init_pos1 = p1+1;
               sgp->Blist[i].init_pos2 = p2+1;
                       
               /* if it exists, extend sgp->Blist[i1].next to the right */ 
               p1 = sgp->Blist[tmp].end_pos1;
               p2 = sgp->Blist[tmp].end_pos2; 
               while ((p1<sgp->Blist[i].init_pos1-1) &&
                      (data->seq1[p1] == data->seq2[p2])) {
                          p1++; p2++;
               }
               sgp->Blist[tmp].end_pos1 = p1;
               sgp->Blist[tmp].end_pos2 = p2;
                 
               sgp->Blist[i1].init_pos2 += MAX(sgp->Blist[tmp].end_pos1+1,sgp->Blist[i1].init_pos1)
                                      -sgp->Blist[i1].init_pos1;
               sgp->Blist[i1].init_pos1 = MAX(sgp->Blist[tmp].end_pos1+1,sgp->Blist[i1].init_pos1);

               sgp->Blist[i1].end_pos2 -= sgp->Blist[i1].end_pos1-
                                     MIN(sgp->Blist[i].init_pos1-1,sgp->Blist[i1].end_pos1);
               sgp->Blist[i1].end_pos1 = MIN(sgp->Blist[i].init_pos1-1,sgp->Blist[i1].end_pos1);
               if (sgp->Blist[i1].end_pos1-sgp->Blist[i1].init_pos1+1<data->W) {

                   if (sgp->Blist[tmp].end_pos1>=sgp->Blist[i].init_pos1) {
 
                        /* i and next[i1] overlap */
                        length1 = sgp->Blist[tmp].end_pos1-sgp->Blist[tmp].init_pos1+1;
                        length2 = sgp->Blist[i].end_pos1-sgp->Blist[i].init_pos1+1;

                        if (length2 >= length1) {
                            sgp->Blist[tmp].end_pos2 -= sgp->Blist[tmp].end_pos1-sgp->Blist[i].init_pos1+1;
                            sgp->Blist[tmp].end_pos1 = sgp->Blist[i].init_pos1-1; 
                        } else {
                            sgp->Blist[i].init_pos2 += sgp->Blist[tmp].end_pos1+1-
                                                  sgp->Blist[i].init_pos1; 
                            sgp->Blist[i].init_pos1 = sgp->Blist[tmp].end_pos1+1; 
                        }
                   }
                   sgp->Blist[i].next = sgp->Blist[i1].next;
               } else i = i1;
              
      /* ===========   HERE ENDS  THE NEW CODE =========== */

      }

    else i = i1;
  }

  /* separate forward greedy code for the right end */

  i1 = sgp->Blist[i=0].next;
  diff = sgp->Blist[i].init_pos1-sgp->Blist[i1].end_pos1-1;
  if(i1 && diff) {

    bno = fgreedy(data->seq1 + sgp->Blist[i1].end_pos1,
		  data->seq2 + sgp->Blist[i1].end_pos2,
		  diff,
		  sgp->Blist[i].init_pos2 - sgp->Blist[i1].end_pos2 - 1,
		  sgp->Blist[i1].end_pos1,sgp->Blist[i1].end_pos2,
		  data, sgp);

    if(bno) {
      sgp->Blist[i].next = data->b_offset;
      sgp->Blist[data->b_offset + bno - 1].next = i1;
      data->b_offset += bno;
    }
    else if(diff <= sgp->Blist[i].init_pos2 - sgp->Blist[i1].end_pos2 - 1) {
      /*  attempt the sim3 matching strategy  */
      hflag = locate(&I1, &J1, &I2, &J2,
		     data->seq1 + sgp->Blist[i1].end_pos1,
		     data->seq2 + sgp->Blist[i1].end_pos2,
		     diff,
		     sgp->Blist[i].init_pos2 - sgp->Blist[i1].end_pos2 - 1,
		     (Int4)(BETA * data->W), data);
      if(hflag == TRUE) {
	/*
	  printf("I1=%d, I2=%d,J1=%d,J2=%d\n",
	  I1+Blist[i1].end_pos1,
	  I2+Blist[i1].end_pos1,
	  J1+Blist[i1].end_pos2,
	  J2+Blist[i1].end_pos2);
	  */
	offset1 = sgp->Blist[i1].end_pos1;
	offset2 = sgp->Blist[i1].end_pos2;

	sgp->Blist[data->b_offset].init_pos1 = offset1 + I1 + 1;
	sgp->Blist[data->b_offset].init_pos2 = offset2 + J1 + 1;
	sgp->Blist[data->b_offset].end_pos1 = offset1 + I2;
	sgp->Blist[data->b_offset].end_pos2 = offset2 + J2;

	sgp->Blist[i].next = data->b_offset;
	sgp->Blist[data->b_offset].next = i1;
	data->b_offset++;
      }
      /* else printf("Right end of the cDNA sequence does not match the genome.\n"); */
    }

    i = i1;
  }

  
  /* step 2: greedy method on unmatched subsequences */

  while(i1 = sgp->Blist[i].next) {
    diff = sgp->Blist[i].init_pos1-sgp->Blist[i1].end_pos1-1;
    if (diff)
#ifdef XFLAG
      if ((diff<=MIN_EXON_SIZE) && sgp->Blist[i1].end_pos1){
	if (diff<=sgp->Blist[i].init_pos2-sgp->Blist[i1].end_pos2-1)
	  {
	    strncpy(temp,seq1+sgp->Blist[i1].end_pos1,diff);
	    temp[diff] = '\0';
	    ulcell = -1; mcell = MAXINT;
	    l = sgp->Blist[i1].end_pos2;
	    u = sgp->Blist[i].init_pos2-diff-1;
	    while (l<=u)
	      { mismatch = 0;
	      for (j=0; j<diff; j++)
		if (seq2[u+j]!=temp[j]) mismatch++;
	      if (((sgp->Blist[i].init_pos2-u-diff<=ALPHA*W) && (mismatch<=1)) ||
		  ((sgp->Blist[i].init_pos2-u-diff>ALPHA*W) && !mismatch))
		{ ulcell = l = u;
		mcell = mismatch;
		break; }

	      if (mismatch<mcell)
		{ ulcell=u; mcell=mismatch; }
	      u--;

	      mismatch = 0;
	      for (j=0; j<diff; j++)
		if (seq2[l+j]!=temp[j]) mismatch++;
	      if (((l+1-sgp->Blist[i1].end_pos2<=ALPHA*W) && (mismatch<=1)) ||
		  ((l+1-sgp->Blist[i1].end_pos2>ALPHA*W) && !mismatch))
		{ ulcell = u = l;
		mcell = mismatch;
		break; }
	      if (mismatch<mcell)
		{ ulcell=l; mcell=mismatch; }
	      l++;
              }

	    /* introduce block */
	    sgp->Blist[b_offset].init_pos1 = sgp->Blist[i1].end_pos1+1;
	    sgp->Blist[b_offset].init_pos2 = ulcell+1;
	    sgp->Blist[b_offset].end_pos1 = sgp->Blist[i].init_pos1-1; 
	    sgp->Blist[b_offset].end_pos2 = sgp->Blist[b_offset].init_pos2+diff-1;
	    sgp->Blist[i].next = b_offset;
	    sgp->Blist[b_offset].next = i1;
	    b_offset++;
	  }
	else { sgp->Blist[b_offset].init_pos1 = sgp->Blist[i1].end_pos1+1;
	sgp->Blist[b_offset].init_pos2 = sgp->Blist[i1].end_pos2+1;
	sgp->Blist[b_offset].end_pos1 = sgp->Blist[i].init_pos1-1;
	sgp->Blist[b_offset].end_pos2 = sgp->Blist[i].init_pos2-1;
	sgp->Blist[i].next = b_offset;
	sgp->Blist[b_offset].next = i1;
	b_offset++;
	}

      }
      else 
#endif
	{ 
	  /* gap is bigger than min-exon-size or begin sequence */
	  /*
	    print_format(diff, seq1+Blist[i1].end_pos1,LL,"d1");
	    print_format(Blist[i].init_pos2-Blist[i1].end_pos2-1,
	    seq2+Blist[i1].end_pos2,LL,"d2");  
	    */

	  if(sgp->Blist[i1].end_pos1) {

	    last  = (Int4Ptr )MemNew(sizeof(Int4));
	    first = (Int4Ptr )MemNew(sizeof(Int4));

	    bno = greedy(data->seq1 + sgp->Blist[i1].end_pos1, data->seq2 + sgp->Blist[i1].end_pos2,
			 diff, sgp->Blist[i].init_pos2 - sgp->Blist[i1].end_pos2 - 1,
			 sgp->Blist[i1].end_pos1, sgp->Blist[i1].end_pos2,
			 &first, &last, &dist, data, sgp);
	    if(dist > DIST_CUTOFF) {  
	      if((hflag = locate(&I1, &J1, &I2, &J2,
				 data->seq1 + sgp->Blist[i1].end_pos1,
				 data->seq2 + sgp->Blist[i1].end_pos2,
				 diff,
				 sgp->Blist[i].init_pos2 - sgp->Blist[i1].end_pos2 - 1,
				 (Int4)(BETA * data->W), data)) == TRUE) {
		offset1 = sgp->Blist[i1].end_pos1;
		offset2 = sgp->Blist[i1].end_pos2;
		
		sgp->Blist[data->b_offset].init_pos1 = offset1+I1+1;
		sgp->Blist[data->b_offset].init_pos2 = offset2+J1+1;
		sgp->Blist[data->b_offset].end_pos1 = offset1+I2;
		sgp->Blist[data->b_offset].end_pos2 = offset2+J2;

		sgp->Blist[i].next = data->b_offset;
		sgp->Blist[data->b_offset].next = i1;
		data->b_offset++;
	      }
	      else {     /* else if hflag */
		/* heuristic sim3 fails; use result produced by greedy */
		sgp->Blist[i].next = *first;
		sgp->Blist[*last].next = i1;
		data->b_offset += bno;
	      }
	    }
	    else {
	      sgp->Blist[i].next = *first;
	      sgp->Blist[*last].next = i1;
	      data->b_offset += bno;
	    }
            MemFree(last);
            MemFree(first);
	  }
	  else {
	    bno = bgreedy(data->seq1 + sgp->Blist[i1].end_pos1,
			  data->seq2 + sgp->Blist[i1].end_pos2,
			  diff,
			  sgp->Blist[i].init_pos2 - sgp->Blist[i1].end_pos2 - 1,
			  sgp->Blist[i1].end_pos1,
			  sgp->Blist[i1].end_pos2,
			  data, sgp);
	    if(bno) {
	      sgp->Blist[i].next = bno - 1 + data->b_offset;
	      sgp->Blist[data->b_offset].next = i1;
	      data->b_offset += bno;
	    }
	    else if(diff <= sgp->Blist[i].init_pos2 - sgp->Blist[i1].end_pos2 - 1) {
	      /* attempt the matching strategy of sim3  */
	      hflag = locate(&I1, &J1, &I2, &J2,
			     data->seq1 + sgp->Blist[i1].end_pos1,
			     data->seq2 + sgp->Blist[i1].end_pos2,
			     diff,
			     sgp->Blist[i].init_pos2 - sgp->Blist[i1].end_pos2 - 1,
			     (Int4)(BETA * data->W), data);
	      if(hflag == TRUE) {
		/*
		  printf("I1=%d, I2=%d,J1=%d,J2=%d\n",
		  I1+Blist[i1].end_pos1,
		  I2+Blist[i1].end_pos1,
		  J1+Blist[i1].end_pos2,
		  J2+Blist[i1].end_pos2);
		  */
		offset1 = sgp->Blist[i1].end_pos1;
		offset2 = sgp->Blist[i1].end_pos2;

		sgp->Blist[data->b_offset].init_pos1 = offset1+I1+1;
		sgp->Blist[data->b_offset].init_pos2 = offset2+J1+1;
		sgp->Blist[data->b_offset].end_pos1 = offset1+I2;
		sgp->Blist[data->b_offset].end_pos2 = offset2+J2;

		sgp->Blist[i].next = data->b_offset;
		sgp->Blist[data->b_offset].next = i1;
		data->b_offset++;
	      }
	      /* else printf("Left end of the cDNA sequence does not match the genome.\n");	*/
	    }
	  }
	}
    i = i1;
    }

  /* just printing ...*/
#ifdef DEBUG
  i=0;
  while (i=sgp->Blist[i].next)
    {
      fprintf (stderr,"B[%2d]:  %5d  %5d  %5d  %5d\n",i,sgp->Blist[i].init_pos1,
	       sgp->Blist[i].init_pos2,sgp->Blist[i].end_pos1,sgp->Blist[i].end_pos2);
    }

  fprintf(stderr,"\n**********************************\n\n");
  fprintf(stderr,"NORMALIZATION:\n\n");
#endif


  /* second cross - normalize blocks; compaction step  */

  i = sgp->Blist[0].next;
  while((i1 = sgp->Blist[i].next) && (sgp->Blist[i1].init_pos1)) {
    /*
      if (Blist[i].init_pos2-Blist[i1].end_pos2<=
      Blist[i].init_pos1-Blist[i1].end_pos1+ALPHA*W)
      */
    if(ABS(sgp->Blist[i].init_pos2 - sgp->Blist[i].init_pos1 -
	   sgp->Blist[i1].end_pos2 + sgp->Blist[i1].end_pos1) <= ALPHA * data->W)
      /* discard block */
      {
	sgp->Blist[i].init_pos1 = sgp->Blist[i1].init_pos1;
	sgp->Blist[i].init_pos2 = sgp->Blist[i1].init_pos2;
	sgp->Blist[i].next = sgp->Blist[i1].next;
      }
    else i = i1;
  }
  
#ifdef DEBUG
  /* just printing ... */
  printf("\n*******************************************\n");
  printf ("\nBlock decomposition:\n\n");
  i=0;
  while ((i=sgp->Blist[i].next) && (sgp->Blist[i].end_pos1))
    {
      printf ("Block:  I1=%5d,  I2=%5d,  J1=%5d,  J2=%5d\n", sgp->Blist[i].init_pos1,
	      sgp->Blist[i].init_pos2,sgp->Blist[i].end_pos1,sgp->Blist[i].end_pos2);
    }
  printf("\n*******************************************\n\n");
#endif
  return TRUE;
}

static void free_match_list_chain (Matchelt *curr)
{
	Matchelt *next;

	while(curr)
	{
		next = curr->mnext;
		MemFree(curr);
		curr = next;
	}

}

static void free_table_chain (Table *curr)
{
	Table *next;

	while(curr)
	{
		next = curr->next;
		MemFree(curr);
		curr = next;
	}
}

static Int4 segment(Int1 *s1,Int1 *s2,Int4 M,Int4 N,Int4 W,
		    Int4 offset1, Int4 offset2, data_t *data, SIM4GlobalPtr sgp)
{
  Int4 No_branches, bno;
  Int4 i, j;
  Links *Lkptr;
  ValNodePtr data_list;

  bld_table(s1, M, W, data, sgp);
  
  /* attach to each position in seq2 the list of 
     positions in seq1 having the same content */
  bld_lists(s2, N, W, data, sgp);

  /* create matching lists */
  create_mlists(M,N,W, sgp); 

  /* find longest increasing subsequence of positions */
  lisseq(M,N,W,&Lkptr, sgp, &data_list);

  bno = find_blocks(W, Lkptr, offset1, offset2, data, sgp);

  No_branches = power2(MAX(0,2*W-16));
  for (i=0; i<No_branches; i++)
  {
	for(j = 0; j <= D_MASK; j++)
	{
		if(sgp->pos_tbl[i][j] != NULL)
		{
			free_table_chain (sgp->pos_tbl[i][j]);
			sgp->pos_tbl[i][j] = NULL;
		}
	}
  	MemFree(sgp->pos_tbl[i]);  
  }
  MemFree(sgp->pos_tbl);

  
  for(i = 0; i <= M - W + 1; i++) 
  {
	if(sgp->Matchlist[i] != NULL)
	{
		free_match_list_chain (sgp->Matchlist[i]);
		sgp->Matchlist[i] = NULL;
	}
  }
  MemFree(sgp->Matchlist);

  ValNodeFreeData(data_list);
  return bno;
}
  
static void lisseq(Int4 M, Int4 N, Int4 W, Links **Lkptr, SIM4GlobalPtr sgp, ValNodePtr PNTR data_list)
{ 
  Threshelt *Thresh;
  Matchelt  *mptr;
  Links     *new;
  Int4 count, k, thr_size;  /* k an index of Thresh[.]; */
  ValNodePtr curr, prev;
  /* so k in 1,...MIN(M1,N1) */ 

  *data_list = NULL;
  prev = NULL;
  thr_size = MIN(N1, M1);    /* initially N1; modified */
  Thresh = (Threshelt *)MemNew((thr_size + 1) * sizeof(Threshelt));
  for(count = 1; count <= thr_size; count++) {
    Thresh[count].tblock = N1 + 1;
    Thresh[count].tref = NULL;
  }

  Thresh[0].tblock = 0;
  Thresh[0].tref = NULL;

  sgp->Link = (Links **)MemNew((thr_size + 1) * sizeof(Links *));
  sgp->Link[0] = NULL;

  for(count = 1; count <= M1; count++) {
    /* Link[count] = NULL; */
    /* for j in Matchlist[i] */
    mptr = sgp->Matchlist[count];
    while(mptr != NULL) {

      /* find k */
      if(!(k = btfind(mptr->mblock, Thresh, thr_size))) {
	ErrPostEx(SEV_FATAL, 1, 0, "Invalid data!");
	exit(1);
      }
      if((mptr->mblock<Thresh[k].tblock) ||
	 ((mptr->mblock==Thresh[k].tblock) &&
	  (count>(Thresh[k].tref)->pos))) {
	Thresh[k].tblock = mptr->mblock;
	Thresh[k].tref = mptr->mref;
	new = (Links *)MemNew(sizeof(Links));
	new->lptr = mptr;
	new->lnext = sgp->Link[k-1];
	sgp->Link[k] = new;

	/*store the data into a chain. for freeing in the future */
	curr = ValNodeNew(NULL);
	curr->data.ptrvalue = new;
	if(prev == NULL)
		*data_list = curr;
	else
		prev->next = curr;
	prev = curr;

      }
      mptr = mptr->mnext;
    }
  }
  k = MIN(M1, N1);
  while(Thresh[k].tblock == N1 + 1) k--;
  *Lkptr = sgp->Link[k];
  MemFree(Thresh);
}

  
static Int4 btfind(Int4 key, Threshelt *Thr, Int4 thr_size)
{
  Int4 l, u, index;

  l = 1; u = thr_size;
  
  while(l <= u) {
    index = (Int4)(l + u)/2;
    if((Thr[index - 1].tblock<key) && (key <= Thr[index].tblock))
      return index; 
    else
      if(key <= Thr[index].tblock) u = index - 1;      
      else l = index + 1;
  }
  return 0;
}
  
/* static void code2str(Uint4 ecode, Int1 *temp, int W) */
/* { int i, mask = 3; */

/*   temp[W] = '\0'; */
/*   for (i=W-1; i>=0; i--) */
/*      { temp[i] = decoding[mask&ecode]; */
/*        ecode = ecode>>2; */
/*      } */
/* } */

/* Int4 get_seq(Int1 *filename, Int1 **seq) */
/* {  */
/*   FILE *fp; */
/*   Int1 buffer[1000], *buf, *s; */
/*   int nlines=0; */

/*   fp = ckopen(filename,"r"); */
/*   while (fgets(buffer,1000,fp)!=NULL) */
/*         if (buffer[0]!='>') nlines++; */
/*   s = *seq = (Int1 *)MemNew(nlines*LL+1); */
/*   rewind(fp); */
/*   while (fgets(buffer,1000,fp)!=NULL) */
/*       if (buffer[0]!='>') { */
/*             buf = buffer; */
/*             while ((*buf) && (*buf!='\n')) */
/*                *s++ = *buf++; */
/*       } */
/*   *s = '\0'; */
/*   return s-*seq; */
/* } */

static void create_mlists(Int4 M, Int4 N, Int4 W, SIM4GlobalPtr sgp)
     /* Matchlist[i] - retains the list corresponding to position i+W-1 
   in sequence 2 (the long one ) */
{
  Int4 i;
  Matchelt *new;
  Table *ptr;

  sgp->Matchlist = (Matchelt **)MemNew((M - W + 2)*sizeof(Matchelt *));
  for(i = 0; i <= M - W + 1; i++) 
    sgp->Matchlist[i] = NULL; 

  /* traverse s2 */
  for(i = 1; i <= N - W + 1; i++) {
    ptr = sgp->lists[i];
    while(ptr != NULL) { 
      new = (Matchelt *)MemNew(sizeof(Matchelt));
      new->mblock = i;
      new->mnext = sgp->Matchlist[ptr->pos - W + 1];
      new->mref = ptr;
      sgp->Matchlist[ptr->pos - W + 1] = new; 
      ptr = ptr->next;
    }
  }
}

static void bld_lists(Int1 *s2, Int4 N, Int4 W, data_t *data, SIM4GlobalPtr sgp)
{
  Int4 i, si;
  Uint4 ecode, mask, temp;
  Int1 *s, *marker;

  sgp->lists = (Table **)MemNew((N - W + 2) * sizeof(Table *));
  for(i = 0; i <= N - W + 1; i++)   sgp->lists[i] = NULL;
  mask = (1 << (W + W - 2)) - 1;
  s = s2 - 1;

  ecode = 0L;
  for(i = si = 1; i < W; ++si)
    if((temp = data->encoding[*++s]) != 4) {
      ecode = (ecode << 2) + temp;
      i++;
    }
  marker = s+1;
  for(; (*++s) && (si <= N); si++)
    if((temp = data->encoding[*s]) != 4) {
      ecode = ((ecode & mask) << 2) + temp;
      sgp->lists[s - marker + 1] = sgp->pos_tbl[ecode >> 16][ecode&D_MASK];
    }
}


static Int4 find_blocks(Int4 W, Links *lk, Int4 offset1, Int4 offset2, data_t *data, SIM4GlobalPtr sgp)
{
  Links *lkptr;

  Int4 i, bno;  
  Int4 ppos1, ppos2, cpos2, cpos1;

  lkptr = lk;
  bno = 0;
  ppos2 = MAXINT;
  cpos1 = cpos2 = ppos1 = 0;
  while(lkptr != NULL) {
    cpos2 = lkptr->lptr->mblock+W-1;
    cpos1 = lkptr->lptr->mref->pos;

    if(abs(ppos2 - ppos1 - cpos2 + cpos1) > ALPHA * W)  {
      if((bno < 2) || (sgp->Blist[bno - 1 + data->b_offset].init_pos1 - offset1 - cpos1 > W)) {
	  sgp->Blist[bno + data->b_offset].init_pos1 = ppos1 + offset1;
	  sgp->Blist[bno + data->b_offset].init_pos2 = ppos2 + offset2;
	  bno++;
	}
      sgp->Blist[bno + data->b_offset].end_pos1 = cpos1 + offset1;
      sgp->Blist[bno + data->b_offset].end_pos2 = cpos2 + offset2;
    }
    ppos1 = cpos1; ppos2 = cpos2;
    lkptr = lkptr->lnext;
  }
  if((bno < 2) || (sgp->Blist[bno - 1 + data->b_offset].init_pos1 - offset1 - cpos1 > W)) {
    sgp->Blist[bno + data->b_offset].init_pos1 = cpos1 + offset1;
    sgp->Blist[bno + data->b_offset].init_pos2 = cpos2 + offset2;
  }
  else bno--;
              
  if((bno > 1) && (sgp->Blist[bno - 1 + data->b_offset].init_pos1 <= W + offset1)) bno--;

  sgp->Blist[data->b_offset].next = data->b_offset + 1;
  sgp->Blist[bno + data->b_offset + 1].end_pos1 = sgp->Blist[bno + data->b_offset + 1].end_pos2 = 0;

  for(i = 1; i <= bno; i++) {
    sgp->Blist[i + data->b_offset].next = i + data->b_offset + 1; 
    cpos1 = MAX(sgp->Blist[i + data->b_offset].init_pos1 - W + 1,
		sgp->Blist[i + 1 + data->b_offset].end_pos1 + 1);
    sgp->Blist[i + data->b_offset].init_pos2 =
      MAX(sgp->Blist[i + data->b_offset].init_pos2 - sgp->Blist[i + data->b_offset].init_pos1 + cpos1,
	  sgp->Blist[i + 1 + data->b_offset].end_pos2 + 1);
    sgp->Blist[i + data->b_offset].init_pos1 = cpos1;
  }

  sgp->Blist[bno + data->b_offset].next = 0;
 
  return bno;
}

static void link_to_data_list(Pointer data, ValNodePtr PNTR head, ValNodePtr PNTR prev)
{
	ValNodePtr curr;

             curr = ValNodeNew(NULL);
             curr->data.ptrvalue = data;
             if(*prev == NULL)
		*head = curr;
	     else
		(*prev)->next = curr;
	     *prev = curr;
}


static Int4 bgreedy(Int1 *s1, Int1 *s2, Int4 m, Int4 n,
		    Int4 offset1, Int4 offset2, data_t *data, SIM4GlobalPtr sgp)
{
  Int4 
    col,			/* column number */
    row,			/* row number */
    max_d,			/* bound on the length of edit script */
    d,				/* current distance */
    k,				/* current diagonal */
    bno=0,			/* returned number of blocks */
    DELTA,			/* n-m  */
    ORIGIN,
    lower,			/* boundaries for searching diagonals */
    upper;

  Int4     *last_d, *temp_d;	/* row containing the last d */
  struct edit **script,		/* corresponding edit script */
    **temp_script;		/* temporary edit script */
  struct edit *new;
  ValNodePtr data_list = NULL, prev = NULL;

  DELTA = n-m;
  max_d = DIST_CUTOFF;

  /*
    if ((DELTA<0) && 
    (abs(DELTA) > MAX((Int4)(ALPHA*DEFAULT_W),(Int4)(CUTOFF*n))))  
    fprintf(stderr,"\nWarning: data do not support proper segmentation!\n\n"); 
    */

  ORIGIN = m;
  for(row = m, col = n; row > 0 && col > 0 && (s1[row - 1] == s2[col - 1]); row--,col--);

  if(row == 0) {
    /* hit first row; stop search */
    sgp->Blist[data->b_offset].init_pos1 = offset1 + 1;
    sgp->Blist[data->b_offset].end_pos1 = offset1 + m;
    sgp->Blist[data->b_offset].init_pos2 = offset2 - m + n + 1;
    sgp->Blist[data->b_offset].end_pos2 = offset2 + n;

    return ++bno;
  }
  if(col == 0) {
    /* hit first column */
    sgp->Blist[data->b_offset].init_pos1 = offset1 + m - n + 1;
    sgp->Blist[data->b_offset].end_pos1 = offset1 + m;
    sgp->Blist[data->b_offset].init_pos2 = offset2 + 1;
    sgp->Blist[data->b_offset].end_pos2 = offset2 + n;

    return ++bno;
  }

  last_d = (Int4Ptr )MemNew((m + n + 1) * sizeof(Int4));
  temp_d = (Int4Ptr )MemNew((m + n + 1) * sizeof(Int4));
  script = (struct edit **)MemNew((m + n + 1)*sizeof(struct edit *));
  temp_script = (struct edit **)MemNew((m + n + 1)*sizeof(struct edit *));

  for(k = 0; k <= m + n; script[k] = NULL, last_d[k++] = m + 1);
  last_d[ORIGIN+DELTA] = row;

  lower = ORIGIN + DELTA - 1;
  upper = ORIGIN + DELTA + 1;

  for(d = 1; d <= max_d ; ++d)  {

    /* for each relevant diagonal ... */
    for(k = lower; k <= upper; k++) {
      /* get space for the next edit instruction */
      new = (struct edit *) MemNew(sizeof(struct edit));
	  link_to_data_list((Pointer)new, &data_list, &prev);

      /* find a d on diagonal k */
      if (k==-d+DELTA+ORIGIN) {

	/* move left from the last d-1 on diagonal k+1 */
	row = last_d[k+1];
	new->link = script[k+1];
	new->op = INS;
      } else if (k==d+DELTA+ORIGIN) {

	/* move up from the last d-1 on diagonal k-1 */
	row = last_d[k-1]-1;
	new->link = script[k-1];
	new->op = DEL;
      } else if ((last_d[k]-1<=last_d[k+1]) && 
		 (last_d[k]-1<=last_d[k-1]-1)) {
	/* substitution */
	row = last_d[k]-1;
	new->link = script[k];
	new->op = SUB;

      } else if ((last_d[k-1]-1<=last_d[k+1]) && 
		 (last_d[k-1]-1<=last_d[k]-1)) {
	/* move right from the last d-1 on diagonal k-1 */
	row = last_d[k-1]-1;
	new->link = script[k-1];
	new->op = DEL;
      } else  {

	/* move left from the last d-1 on diagonal k+1 */
	row = last_d[k+1];
	new->link = script[k+1];
	new->op = INS;
      }
      /* code common to the three cases */
      new->line1 = row;
      new->line2 = col = row + k - ORIGIN;
      temp_script[k] = new;

      /* slide up the diagonal */
      while (row > 0 && col > 0 && (s1[row-1]==s2[col-1])) {
	--row;
	--col;
      }
      temp_d[k] = row;

      if (row == 0 && col == 0) {
	/* hit northwest corner; have the answer */
	script[k] = temp_script[k];
		MemFree(last_d); MemFree(temp_d);
	bno = bput_scr(script[k],m,n,offset1,offset2,0,0,data->b_offset, sgp);
        MemFree(script);
        MemFree(temp_script);
        ValNodeFreeData(data_list);

	return bno;
      }
      if (row == 0) {

	/* hit last row, don't look to the right; 
	   record first and final insertions in the
	   list -- possibly identical. 
	   */ 
	script[k] = temp_script[k];

	/* record first INS */
	new = (struct edit *) MemNew(sizeof(struct edit));
	link_to_data_list((Pointer)new, &data_list, &prev);
	new->line1 = 0;
	new->line2 = col = --k - ORIGIN;
	new->op = INS;
	new->link = script[k+1];
	script[k] = new;

	/* record last INS */
	new = (struct edit *) MemNew(sizeof(struct edit));
	link_to_data_list((Pointer)new, &data_list, &prev);
	new->line1 = 0;
	new->line2 = col = 0;
	new->op = INS;
	new->link = script[k];
	script[ORIGIN] = new;

	MemFree(last_d); MemFree(temp_d);
	bno = bput_scr(script[ORIGIN],m,n,offset1,offset2,0,0,data->b_offset, sgp);
    MemFree(script);
    MemFree(temp_script);
    ValNodeFreeData(data_list);

	return bno;
      }

      if (col == 0) {

	/* hit last column, don't look forward;
	   record first and final deletions in the 
	   list, possibly identical.
	   */
	script[k] = temp_script[k];

	/* record first DEL */
	new = (struct edit *) MemNew(sizeof(struct edit));
	link_to_data_list((Pointer)new, &data_list, &prev);
	new->line1 = -(++k) + ORIGIN;
	new->line2 = 0;
	new->op = DEL;
	new->link = script[k-1];
	script[k] = new;

	/* record last DEL */
	new = (struct edit *) MemNew(sizeof(struct edit));
	link_to_data_list((Pointer)new, &data_list, &prev);
	new->line1 = 0;
	new->line2 = 0;
	new->op = DEL;
	new->link = script[k];
	script[ORIGIN] = new;


	bno = bput_scr(script[ORIGIN],m,n,offset1,offset2,0,0,data->b_offset, sgp);
	MemFree(last_d); MemFree(temp_d);

    MemFree(script);
    MemFree(temp_script);
    ValNodeFreeData(data_list);

	return bno;
      }

    }
    for (k=lower; k<=upper; k++) {
      script[k] = temp_script[k];
      last_d[k] = temp_d[k];
    }

    --lower;
    ++upper;
  }
  MemFree(last_d);
  MemFree(temp_d);
  MemFree(script);
  MemFree(temp_script);
  ValNodeFreeData(data_list);

  return bno=0;
}

static Int4 fgreedy(Int1 *s1, Int1 *s2, Int4 m, Int4 n, Int4 offset1, Int4 offset2, data_t *data, SIM4GlobalPtr sgp)
{
  Int4     col,                    /* column number */
          row,                    /* row number */
          max_d,                  /* bound on the length of the edit script */
          d,                      /* current compressed distance */
          k,                      /* current diagonal */
          bno=0,                  /* returned number of blocks */
          DELTA,                  /* n-m  */
          ORIGIN,
          lower,
          upper;
  Int4     *last_d, *temp_d;       /* column containing the last p */
  struct edit **script,           /* corresponding edit script */
              **temp_script;
  struct edit *new;
  ValNodePtr data_list = NULL, prev = NULL;


  DELTA = n-m;
  max_d = DIST_CUTOFF;
/*
  if ((DELTA<0) && 
      (abs(DELTA) > MAX((int)(ALPHA*DEFAULT_W),(int)(CUTOFF*n)))) 
        fprintf(stderr,"\nWarning: data do not support proper segmentation!\n\n");    
*/

  ORIGIN = m;
  for (row=0, col=0; col<n && row<m && (s1[row]==s2[col]); row++, col++);

  if (row == m) {
       /* hit last row; stop search */

       sgp->Blist[data->b_offset].init_pos1 = offset1+1;
       sgp->Blist[data->b_offset].end_pos1 = offset1+m;
       sgp->Blist[data->b_offset].init_pos2 = offset2+1;
       sgp->Blist[data->b_offset].end_pos2 = offset2+m;

       return ++bno;
  }

  if (col == n) {
       /* hit last column */

       sgp->Blist[data->b_offset].init_pos1 = offset1+1;
       sgp->Blist[data->b_offset].end_pos1 = offset1+n;
       sgp->Blist[data->b_offset].init_pos2 = offset2+1;
       sgp->Blist[data->b_offset].end_pos2 = offset2+n;

       return ++bno;
  }


  last_d = (Int4Ptr )MemNew((m+n+1)*sizeof(Int4));
  temp_d = (Int4Ptr )MemNew((m+n+1)*sizeof(Int4));
  script = (struct edit **)MemNew((m+n+1)*sizeof(struct edit *));
  temp_script = (struct edit **)MemNew((m+n+1)*sizeof(struct edit *));

  for (k=0; k<=m+n; script[k] = NULL, last_d[k++]=-1);
  last_d[ORIGIN] = row;

  lower = ORIGIN - 1;
  upper = ORIGIN + 1;

  for (d = 1; d <= max_d ; ++d)  {

          /* for each relevant diagonal ... */
          for (k = lower; k <= upper; k++) {
               /* get space for the next edit instruction */
               new = (struct edit *) MemNew(sizeof(struct edit));
			   link_to_data_list((Pointer)new, &data_list, &prev);

               /* find a d on diagonal k */
               if (k==-d+ORIGIN) {

                        /* move down from the last d-1 on diagonal k+1 */
                        row = last_d[k+1]+1;
                        new->link = script[k+1];
                        new->op = DEL;
               } else if (k==d+ORIGIN) {

                        /* move right from the last d-1 on diagonal k-1 */
                        row = last_d[k-1];
                        new->link = script[k-1];
                        new->op = INS;
               } else if ((last_d[k]>=last_d[k+1]) && 
                          (last_d[k]+1>=last_d[k-1])) {

                        /* substitution */
                        row = last_d[k]+1;
                        new->link = script[k];
                        new->op = SUB;
               } else if ((last_d[k+1]+1>=last_d[k-1]) && 
                          (last_d[k+1]>=last_d[k])) {

                        /* move down from the last d-1 on diagonal k+1 */
                        row = last_d[k+1]+1;
                        new->link = script[k+1];
                        new->op = DEL;
               } else {

                        /* move right from the last d-1 on diagonal k-1 */
                        row = last_d[k-1];
                        new->link = script[k-1];
                        new->op = INS;
               }

               /* code common to the three cases */
               new->line1 = row;
               new->line2 = col = row + k - ORIGIN;
               temp_script[k] = new;

               /* slide down the diagonal */
               if (row>=0)
               while (row < m && col < n && (s1[row]==s2[col])) {
                                ++row;
                                ++col;
               }
               temp_d[k] = row;

               if (row == m && col == n) {
                   /* hit southeast corner; have the answer */
                   script[k] = temp_script[k];
                   bno = fput_scr(script[k],offset1,offset2,m,n,data->b_offset, sgp);
					MemFree(last_d); MemFree(temp_d);

					MemFree(script);
					MemFree(temp_script);
					ValNodeFreeData(data_list);

                   return bno;
               }
               if (row == m) {
                   /* hit last row; don't look further */
                   script[k] = temp_script[k];

                   /* record first INS */
                   new = (struct edit *) MemNew(sizeof(struct edit));
				   link_to_data_list((Pointer)new, &data_list, &prev);
                   new->line1 = m;
                   new->line2 = col = m + (++k) - ORIGIN;
                   new->op = INS;
                   new->link = script[k-1];
                   script[k] = new;

                   /* record last INS */
                   new = (struct edit *) MemNew(sizeof(struct edit));
				   link_to_data_list((Pointer)new, &data_list, &prev);
                   new->line1 = m;
                   new->line2 = n;
                   new->op = INS;
                   new->link = script[k];
                   script[ORIGIN + DELTA] = new;

                   MemFree(last_d); MemFree(temp_d);
                   bno = fput_scr(script[ORIGIN+DELTA],offset1,offset2,m,n,data->b_offset, sgp);

					MemFree(script);
					MemFree(temp_script);
					ValNodeFreeData(data_list);

                   return bno;
               }

               if (col == n) {
                   /* hit last column; don't look further */
                   script[k] = temp_script[k];

                   /* record first DEL */
                   new = (struct edit *) MemNew(sizeof(struct edit));
				   link_to_data_list((Pointer)new, &data_list, &prev);
                   new->line1 = n - (--k) + ORIGIN;
                   new->line2 = n;
                   new->op = DEL;
                   new->link = script[k+1];
                   script[k] = new;

                   /* record last DEL */
                   new = (struct edit *) MemNew(sizeof(struct edit));
				   link_to_data_list((Pointer)new, &data_list, &prev);
                   new->line1 = m;
                   new->line2 = n;
                   new->op = DEL;
                   new->link = script[k];
                   script[ORIGIN] = new;

                   MemFree(last_d); MemFree(temp_d);
                   bno = fput_scr(script[ORIGIN+DELTA],offset1,offset2,m,n,data->b_offset, sgp);
					MemFree(script);
					MemFree(temp_script);
					ValNodeFreeData(data_list);
                   return bno;
               }


          }
          for (k=lower; k<=upper; k++) {
               script[k] = temp_script[k];
               last_d[k] = temp_d[k];
          }

          --lower;
          ++upper;
     }
	 ValNodeFreeData(data_list);
     MemFree(last_d);
     MemFree(temp_d);
     MemFree(script);
     MemFree(temp_script);

     return bno=0;
}

static void free_2D_Int(Int4 ** val, Int4 size)
{
	Int4 i;

	for(i = 0; i<size; ++i)
		MemFree(val[i]);
	MemFree(val);
}

static void free_this_script (struct edit ***val, Int4 size)
{
	Int4 i;

	for(i = 0; i<size; ++i)
		MemFree(val[i]);
	MemFree(val);
}

static Int4 greedy(Int1 *s1, Int1 *s2, Int4 m, Int4 n,
		   Int4 offset1, Int4 offset2,
		   Int4Ptr *first, Int4Ptr *last, Int4Ptr dist, data_t *data, SIM4GlobalPtr sgp)
{
  Int4     col,                    /* column number */
          d,                      /* current distance */
          k,                      /* current diagonal */
          max_d,                  /* bound on size of edit script */
          bno=0,                  /* returned number of blocks */
          Cost,
          blower,flower,          /* boundaries for searching diagonals */
          bupper,fupper,          
          row,                    /* row number */
          DELTA,                  /* n-m  */
          ORIGIN; 
  Int4     back, forth;            /* backward and forward limits at exit */

  Int4     **blast_d, **flast_d;   /* rows containing the last d */
  Int4     *min_row, *min_diag,    /* MIN (b)/ MAX (f) row (and diagonal) */ 
          *max_row, *max_diag;    /* reached for cost d=0, ... m.  */ 
 
  struct edit ***bscript,         /* corresponding edit scripts */
              ***fscript;
  struct edit *new;
  ValNodePtr data_list = NULL, prev = NULL;

  DELTA = n-m;
  max_d = m+1;
  if (DELTA<0)
      { /* if (abs(DELTA) > MAX((Int4)(ALPHA*DEFAULT_W),(Int4)(CUTOFF*n)))
            fprintf(stderr,"\nWarning: data do not support proper segmentation!\n\n");
*/

        /* record block, anyways */
        sgp->Blist[data->b_offset].init_pos1 = offset1+1;
        sgp->Blist[data->b_offset].init_pos2 = offset2+1;
        sgp->Blist[data->b_offset].end_pos1 = offset1+m;
        sgp->Blist[data->b_offset].end_pos2 = offset2+n;
        **last = **first = data->b_offset;

        *dist = -1;
        return ++bno;
      }

  ORIGIN = m;
  for (row=m, col=n; row>0 && col>0 && (s1[row-1]==s2[col-1]); row--,col--);

  if (row == 0) {
       /* hit last row; stop search */
       sgp->Blist[data->b_offset].init_pos1 = offset1+1;
       sgp->Blist[data->b_offset].end_pos1 = offset1+m;
       sgp->Blist[data->b_offset].init_pos2 = offset2-m+n+1;
       sgp->Blist[data->b_offset].end_pos2 = offset2+n;
       **last = **first = data->b_offset;

       *dist = 0;
       return ++bno;
  }

  blast_d = (Int4Ptr *)MemNew((m+1)*sizeof(Int4Ptr ));
  bscript = (struct edit ***)MemNew((m+1)*sizeof(struct edit **));
  for (d=0; d<=m; d++) {
       blast_d[d] = (Int4Ptr )MemNew((m+n+1)*sizeof(Int4));
       bscript[d] = (struct edit **)MemNew((m+n+1)*sizeof(struct edit *));
  }

  for (d=0; d<=m; d++)
       for (k=0; k<=m+n; blast_d[d][k++]=m+1);
  blast_d[0][ORIGIN+DELTA] = row;
  bscript[0][ORIGIN+DELTA] = NULL;

  blower = ORIGIN + DELTA - 1;
  bupper = ORIGIN + DELTA + 1;

  for (row=0; row<n && row<m && (s1[row]==s2[row]); row++);

  if (row == m) {
       /* hit last row; stop search */
       sgp->Blist[data->b_offset].init_pos1 = offset1+1;
       sgp->Blist[data->b_offset].end_pos1 = offset1+m;
       sgp->Blist[data->b_offset].init_pos2 = offset2+1;
       sgp->Blist[data->b_offset].end_pos2 = offset2+m;
       **last = **first = data->b_offset;

       *dist = 0;
	   free_this_script (bscript, m+1);
	   free_2D_Int(blast_d, m+1);

       return ++bno;
  }

  flast_d = (Int4Ptr *)MemNew((m+1)*sizeof(Int4Ptr ));
  fscript = (struct edit ***)MemNew((m+1)*sizeof(struct edit **));
  for (d=0; d<=m; d++) {
       flast_d[d] = (Int4Ptr )MemNew((m+n+1)*sizeof(Int4));
       fscript[d] = (struct edit **)MemNew((m+n+1)*sizeof(struct edit *));
  }


  for (d=0; d<=m; d++)
       for (k=0; k<=m+n; flast_d[d][k++]=-1);
  flast_d[0][ORIGIN] = row;
  fscript[0][ORIGIN] = NULL;

  flower = ORIGIN - 1;
  fupper = ORIGIN + 1;

  max_row = (Int4Ptr )MemNew((m+1)*sizeof(Int4));
  min_row = (Int4Ptr )MemNew((m+1)*sizeof(Int4));
  max_diag = (Int4Ptr )MemNew((m+1)*sizeof(Int4));
  min_diag = (Int4Ptr )MemNew((m+1)*sizeof(Int4));

  for (d=1; d<=m; d++) {
       min_row[d] = m+1;
       max_row[d] = -1;
  }
  min_row[0] = blast_d[0][ORIGIN+DELTA];
  min_diag[0] = ORIGIN+DELTA;
  max_row[0] = flast_d[0][ORIGIN];
  max_diag[0] = ORIGIN;	

  d = 1;
  while (d <= max_d) {

        /* for each relevant diagonal ... */
        for (k = blower; k <= bupper; k++) {
             /* get space for the next edit instruction */
             new = (struct edit *)MemNew(sizeof(struct edit));
			 link_to_data_list((Pointer)new, &data_list, &prev);

             /* find a d on diagonal k */
             if (k==-d+DELTA+ORIGIN) {

                      /* move left from the last d-1 on diagonal k+1 */
                      row = blast_d[d-1][k+1];
                      new->link = bscript[d-1][k+1];
                      new->op = INS;
             }
             else if (k==d+DELTA+ORIGIN) {

                      /* move up from the last d-1 on diagonal k-1 */
                      row = blast_d[d-1][k-1]-1;
                      new->link = bscript[d-1][k-1];
                      new->op = DEL;
             } else if ((blast_d[d-1][k]<=blast_d[d-1][k+1]) &&
                        (blast_d[d-1][k]-1<=blast_d[d-1][k-1])) {

                      /* substitution */
                      row = blast_d[d-1][k]-1;
                      new->link = bscript[d-1][k];
                      new->op = SUB;

             } else if ((blast_d[d-1][k-1]<=blast_d[d-1][k+1]-1) &&
                        (blast_d[d-1][k-1]<=blast_d[d-1][k]-1)) {
                      /* move right from the last d-1 on diagonal k-1 */
                      row = blast_d[d-1][k-1]-1;
                      new->link = bscript[d-1][k-1];
                      new->op = DEL;
             } else  {
                      /* move left from the last d-1 on diagonal k+1 */
                      row = blast_d[d-1][k+1];
                      new->link = bscript[d-1][k+1];
                      new->op = INS;
             }
             /* code common to the three cases */
             new->line1 = row;
             new->line2 = col = row + k - ORIGIN;
             bscript[d][k] = new;

             /* slide up the diagonal */
             while (row > 0 && col > 0 && (s1[row-1]==s2[col-1])) {
                             --row;
                             --col;
             }
             blast_d[d][k] = row;

             if (row == 0 && col == 0)
                        if (d <= max_d) {
                            /* hit northwest corner; have the answer */
                            bno = bput_scr(bscript[d][k],m,n,offset1,offset2,0,0,data->b_offset, sgp);
                            **first = data->b_offset+bno-1;
                            **last = data->b_offset;

                            /* for (Cost=0; Cost<=m; Cost++) { 
                                 MemFree(blast_d[Cost]); MemFree(flast_d[Cost]);
                                 MemFree(bscript[Cost]); MemFree(fscript[Cost]);
                            } */
							free_this_script (bscript, m+1);
							free_this_script (fscript, m+1);
							free_2D_Int(blast_d, m+1);
							free_2D_Int(flast_d, m+1);

                            MemFree(min_row); MemFree(min_diag);
                            MemFree(max_row); MemFree(max_diag);
							ValNodeFreeData(data_list);

                            *dist = d;
                            return bno;
             } else goto fin;

             if (row == 0)
                        /* hit last row; fill in insertions */
                        if (d <= max_d) {
                             new = (struct edit *) MemNew(sizeof(struct edit));
							 link_to_data_list((Pointer)new, &data_list, &prev);
                             new->line1 = 0;
                             new->line2 = col = --k - ORIGIN;
                             new->op = INS;
                             new->link = bscript[d][k+1];
                             bscript[d][k] = new;

                             /* record last INS */
                             new = (struct edit *)MemNew(sizeof(struct edit));
							 link_to_data_list((Pointer)new, &data_list, &prev);
                             new->line1 = 0;
                             new->line2 = col = 0;
                             new->op = INS;
                             new->link = bscript[d][k];

                             bno = bput_scr(new,m,n,offset1,offset2,0,0,data->b_offset, sgp);
                             **first = data->b_offset+bno-1;
                             **last = data->b_offset;

                             /* for (Cost = 0; Cost<=m; Cost++) { 
                                  MemFree(blast_d[Cost]); MemFree(flast_d[Cost]);
                                  MemFree(bscript[Cost]); MemFree(fscript[Cost]);
                             }
                             MemFree(min_row); MemFree(min_diag);
                             MemFree(max_row); MemFree(max_diag);*/
							free_this_script (bscript, m+1);
							free_this_script (fscript, m+1);
							free_2D_Int(blast_d, m+1);
							free_2D_Int(flast_d, m+1);

                            MemFree(min_row); MemFree(min_diag);
                            MemFree(max_row); MemFree(max_diag);
							ValNodeFreeData(data_list);


                             *dist = d;
                             return bno;

                        } else goto fin;
       
             if (col == 0)
                        if (d <= max_d) {
                             /* hit last column; fill in deletions */
                             /* record first DEL */
                             new = (struct edit *) MemNew(sizeof(struct edit));
							 link_to_data_list((Pointer)new, &data_list, &prev);
                             new->line1 = -(++k) + ORIGIN;
                             new->line2 = 0;
                             new->op = DEL;
                             new->link = bscript[d][k-1];
                             bscript[d][k] = new;

                             /* record last DEL */
                             new = (struct edit *)MemNew(sizeof(struct edit));
							 link_to_data_list((Pointer)new, &data_list, &prev);
                             new->line1 = 0;
                             new->line2 = 0;
                             new->op = DEL;
                             new->link = bscript[d][k];

                             bno = bput_scr(new,m,n,offset1,offset2,0,0,data->b_offset, sgp);
                             **first = data->b_offset+bno-1;
                             **last = data->b_offset;

                             /* for (Cost = 0; Cost<=m; Cost++) { 
                                  MemFree(blast_d[Cost]); MemFree(flast_d[Cost]);
                                  MemFree(bscript[Cost]); MemFree(fscript[Cost]);
                             }
                             MemFree(min_row); MemFree(min_diag);
                             MemFree(max_row); MemFree(max_diag);*/

							free_this_script (bscript, m+1);
							free_this_script (fscript, m+1);
							free_2D_Int(blast_d, m+1);
							free_2D_Int(flast_d, m+1);

                            MemFree(min_row); MemFree(min_diag);
                            MemFree(max_row); MemFree(max_diag);
							ValNodeFreeData(data_list);


                             *dist = d;
                             return bno;
                        } else goto fin;

        }     /* for k */

        min_row[d] = blast_d[d][DELTA+ORIGIN];
        min_diag[d] = DELTA+ORIGIN;
        for (k=blower; k<=bupper; ++k)
             if (blast_d[d][k]<min_row[d]) {
                       min_row[d] = blast_d[d][k];
                       min_diag[d] = k;
        }

        /* record cell, if paths overlap with minimum combined cost */
        /* obs: it suffices to search up to Cost=min(d-1,(max_d-d)) */
        for (Cost=0; Cost<d; Cost++)
             if ((min_row[d]<=max_row[Cost]) &&
                 (max_d > d+Cost)) {
                           max_d = d+Cost;
                           back = d;
                           forth = Cost;
                           break;
             }

        --blower;
        ++bupper;


        /* for each relevant diagonal ... */
        for (k = flower; k <= fupper; k++) {
               /* get space for the next edit instruction */
               new = (struct edit *) MemNew(sizeof(struct edit));
			   link_to_data_list((Pointer)new, &data_list, &prev);

               /* find a d on diagonal k */
               if (k==-d+ORIGIN) {
                        /* move down from the last d-1 on diagonal k+1 */
                        row = flast_d[d-1][k+1]+1;
                        new->link = fscript[d-1][k+1];
                        new->op = DEL;

               } else if (k==d+ORIGIN) {
                        /* move right from the last d-1 on diagonal k-1 */
                        row = flast_d[d-1][k-1];
                        new->link = fscript[d-1][k-1];
                        new->op = INS;

               } else if ((flast_d[d-1][k]>=flast_d[d-1][k+1]) &&
                          (flast_d[d-1][k]+1>=flast_d[d-1][k-1])) {

                        /* substitution */
                        row = flast_d[d-1][k]+1;
                        new->link = fscript[d-1][k];
                        new->op = SUB;

               } else if ((flast_d[d-1][k+1]+1>=flast_d[d-1][k-1]) &&
                          (flast_d[d-1][k+1]>=flast_d[d-1][k])) {

                        /* move left from the last d-1 on diagonal k+1 */
                        row = flast_d[d-1][k+1]+1;
                        new->link = fscript[d-1][k+1];
                        new->op = DEL;
               } else {
                        /* move right from the last d-1 on diagonal k-1 */
                        row = flast_d[d-1][k-1];
                        new->link = fscript[d-1][k-1];
                        new->op = INS;

               }
               /* code common to the three cases */
               new->line1 = row;
               new->line2 = col = row + k - ORIGIN;
               fscript[d][k] = new;

               /* slide down the diagonal */
               if (row>=0)
               while (row < m && col < n && (s1[row]==s2[col])) {
                                ++row;
                                ++col;
               }
               flast_d[d][k] = row;

               if (row == m && col == n)
                   if (d<=max_d) {
                       /* hit southeast corner; have the answer */
                       bno = fput_scr(fscript[d][k],offset1,offset2,m,n,data->b_offset, sgp);

                       **first = data->b_offset;
                       **last = data->b_offset+bno-1;

                      /*  for (Cost = 0; Cost<=m; Cost++) { 
                            MemFree(blast_d[Cost]); MemFree(flast_d[Cost]);
                            MemFree(bscript[Cost]); MemFree(fscript[Cost]);
                       }
                       MemFree(min_row); MemFree(min_diag);
                       MemFree(max_row); MemFree(max_diag);	*/

					   free_this_script (bscript, m+1);
					   free_this_script (fscript, m+1);
					   free_2D_Int(blast_d, m+1);
					   free_2D_Int(flast_d, m+1);

                       MemFree(min_row); MemFree(min_diag);
                       MemFree(max_row); MemFree(max_diag);
					   ValNodeFreeData(data_list);


                       *dist = d;
                       return bno;
                   } else goto fin;

               if (row == m)
                   if (d<=max_d) {
                       /* hit last row; don't look forward */
                       /* record first INS */
                       new = (struct edit *)MemNew(sizeof(struct edit));
					   link_to_data_list((Pointer)new, &data_list, &prev);
                       new->line1 = m;
                       new->line2 = col = m + (++k) - ORIGIN;
                       new->op = INS;
                       new->link = fscript[d][k-1];
                       fscript[d][k] = new;

                       /* record last INS */
                       new = (struct edit *)MemNew(sizeof(struct edit));
					   link_to_data_list((Pointer)new, &data_list, &prev);
                       new->line1 = m;
                       new->line2 = n;
                       new->op = INS;
                       new->link = fscript[d][k];

                       bno = fput_scr(new,offset1,offset2,m,n,data->b_offset, sgp);
                       **first = data->b_offset;
                       **last = data->b_offset+bno-1;

                       /* for (Cost = 0; Cost<=m; Cost++) { 
                            MemFree(blast_d[Cost]); MemFree(flast_d[Cost]);
                            MemFree(bscript[Cost]); MemFree(fscript[Cost]);
                       }
                       MemFree(min_row); MemFree(min_diag);
                       MemFree(max_row); MemFree(max_diag);	*/

					   free_this_script (bscript, m+1);
					   free_this_script (fscript, m+1);
					   free_2D_Int(blast_d, m+1);
					   free_2D_Int(flast_d, m+1);

                       MemFree(min_row); MemFree(min_diag);
                       MemFree(max_row); MemFree(max_diag);
					   ValNodeFreeData(data_list);

                       
                       *dist = d;
                       return bno;

                   } else goto fin;

               if (col == n)
                   /* hit last column; don't look forward */
                   if (d<=max_d) {
                       /* record first DEL */
                       new = (struct edit *)MemNew(sizeof(struct edit));
					   link_to_data_list((Pointer)new, &data_list, &prev);
                       new->line1 = n - (--k) + ORIGIN;
                       new->line2 = n;
                       new->op = DEL;
                       new->link = fscript[d][k+1];
                       fscript[d][k] = new;

                       /* record last DEL */
                       new = (struct edit *)MemNew(sizeof(struct edit));
					   link_to_data_list((Pointer)new, &data_list, &prev);
                       new->line1 = m;
                       new->line2 = n;
                       new->op = DEL;
                       new->link = fscript[d][k];

                       bno = fput_scr(new,offset1,offset2,m,n,data->b_offset, sgp);
                       **first = data->b_offset;
                       **last = data->b_offset+bno-1;

                       /* for (Cost = 0; Cost<=m; Cost++) { 
                            MemFree(blast_d[Cost]); MemFree(flast_d[Cost]);
                            MemFree(bscript[Cost]); MemFree(fscript[Cost]);
                       }
                       MemFree(min_row); MemFree(min_diag);
                       MemFree(max_row); MemFree(max_diag);	*/

					   free_this_script (bscript, m+1);
					   free_this_script (fscript, m+1);
					   free_2D_Int(blast_d, m+1);
					   free_2D_Int(flast_d, m+1);

                       MemFree(min_row); MemFree(min_diag);
                       MemFree(max_row); MemFree(max_diag);
					   ValNodeFreeData(data_list);


                       *dist = d;
                       return bno;
                   } else goto fin;
          }     /* for k */


          max_row[d] = flast_d[d][ORIGIN];
          max_diag[d] = ORIGIN;
          for (k=flower; k<=fupper; ++k)
                 if (flast_d[d][k]>max_row[d]) {
                     max_row[d] = flast_d[d][k];
                     max_diag[d] = k;
                 }

          /* record backward and forward limits, if minimum combined
           * cost in overlapping. Note: it suffices to search up to 
           * Cost=min(d,(max_d-d)).
           */
          for (Cost=0; Cost<=d; Cost++)
             if ((min_row[Cost]<=max_row[d]) &&
                 (max_d > d+Cost)) {
                      max_d = d+Cost;
                      back = Cost;
                      forth = d;
                      break;
             }
        --flower;
        ++fupper;

        ++d;  /* for d */
      }


  fin:
        if (m-min_row[back]>=max_row[forth]) {
            bno = bput_scr(bscript[back][min_diag[back]],m,n,
                           offset1,offset2,
                           min_row[back],
                           min_row[back]+min_diag[back]-ORIGIN,
                           data->b_offset, sgp);
            **first = (bno) ? data->b_offset+bno-1 : data->b_offset+bno;
            col = fput_scr(fscript[forth][max_diag[forth]],
                           offset1,offset2,
                           min_row[back],
                           min_row[back]+max_diag[forth]-ORIGIN,
                           data->b_offset+bno, sgp);
        } else {
            bno = bput_scr(bscript[back][min_diag[back]],m,n,
                           offset1,offset2,
                           max_row[forth],
                           max_row[forth]+min_diag[back]-ORIGIN,
                           data->b_offset, sgp);
            **first = (bno) ? data->b_offset+bno-1 : data->b_offset+bno;
            col = fput_scr(fscript[forth][max_diag[forth]],
                           offset1,offset2,
                           max_row[forth],
                           max_row[forth]+max_diag[forth]-ORIGIN,
                           data->b_offset+bno, sgp);
        }
        if (bno) sgp->Blist[data->b_offset].next = data->b_offset+bno;
        **last = data->b_offset+bno+col-1;

        /* for (Cost=0; Cost<=m; Cost++) {
                     MemFree(blast_d[Cost]); MemFree(flast_d[Cost]);
                     MemFree(bscript[Cost]); MemFree(fscript[Cost]);
        }
        MemFree(min_row); MemFree(min_diag);
        MemFree(max_row); MemFree(max_diag); */

		free_this_script (bscript, m+1);
		free_this_script (fscript, m+1);
		free_2D_Int(blast_d, m+1);
		free_2D_Int(flast_d, m+1);

        MemFree(min_row); MemFree(min_diag);
        MemFree(max_row); MemFree(max_diag);
		ValNodeFreeData(data_list);


        *dist = back+forth;
        return bno+col;

}


static Int4 bput_scr(struct edit *start, Int4 len1, Int4 len2, 
                    Int4 offset1, Int4 offset2, Int4 row, Int4 col,
                    Int4 offset, SIM4GlobalPtr sgp)
{       struct edit *eptr;
        Int4 bno=0, ppos1, ppos2, i;

        eptr = start;
        ppos1 = row; ppos2 = col-1;

        while (eptr != NULL) {
           if (ppos1>eptr->line1)        /* not used in this version */
                     eptr = eptr->link;
           else
           switch (eptr->op) {
                case DEL     :
                case SUB :
                     eptr = eptr->link;
                     break;
                case INS     :
                     if (ppos1==eptr->line1) {
                         ppos2 = eptr->line2;
                     } else {
                         sgp->Blist[bno+offset].init_pos1 = ppos1+offset1+1;
                         sgp->Blist[bno+offset].end_pos1 = eptr->line1-1+offset1+1;
                         sgp->Blist[bno+offset].init_pos2 = ppos2+1+offset2+1;
                         sgp->Blist[bno+offset].end_pos2 = eptr->line2-1+offset2+1;
                         ppos1 = eptr->line1;
                         ppos2 = eptr->line2;     
                         bno++;
                     }
                     eptr=eptr->link;
           }
        }
        if ((ppos1!=len1) || (ppos2+1!=len2)) {

                  sgp->Blist[bno+offset].init_pos1 = ppos1+offset1+1;
                  sgp->Blist[bno+offset].end_pos1 = len1-1+offset1+1;
                  sgp->Blist[bno+offset].init_pos2 = ppos2+1+offset2+1;
                  sgp->Blist[bno+offset].end_pos2 = len2-1+offset2+1;
                  bno++;
        } 
        for (i=offset; i<bno+offset; i++)
                  sgp->Blist[i+1].next = i;

        return bno;
}

static Int4 fput_scr(struct edit *start, Int4 offset1, Int4 offset2, 
                    Int4 row, Int4 col, Int4 offset, SIM4GlobalPtr sgp)
{       struct edit *eptr;
        Int4 bno=0, ppos1, ppos2, i;

        eptr = start;
        ppos1 = row; ppos2 = col+1;

        while (eptr != NULL) {
           if (ppos1 < eptr->line1)        /* not used in this version */ 
                     eptr = eptr->link;
           else 
           switch (eptr->op) {
                case DEL     :
                case SUB :
                     eptr = eptr->link;
                     break;
                case INS     :
                     if (ppos1==eptr->line1) {
                         ppos2 = eptr->line2;
                     } else {
                         sgp->Blist[bno+offset].init_pos1 = eptr->line1+1+offset1;
                         sgp->Blist[bno+offset].end_pos1 = ppos1+offset1;
                         sgp->Blist[bno+offset].init_pos2 = eptr->line2+1+offset2;
                         sgp->Blist[bno+offset].end_pos2 = ppos2-1+offset2;
                         ppos1 = eptr->line1;
                         ppos2 = eptr->line2;
                         bno++;
                     }
                     eptr=eptr->link;
           }
        }
        if (ppos1!=0) {  /* not a final insert */
                  sgp->Blist[bno+offset].init_pos1 = 1+offset1;
                  sgp->Blist[bno+offset].end_pos1 = ppos1+offset1;
                  sgp->Blist[bno+offset].init_pos2 = 1+offset2;
                  sgp->Blist[bno+offset].end_pos2 = ppos2-1+offset2;
                  bno++;
        }

        for (i=offset; i<bno+offset; i++)
           sgp->Blist[i].next = i+1;

        return bno;
}

static void path(Int4 i1, Int4 j1, Int1 type1, Int4 i2, Int4 j2, Int1 type2, Int4 dist,
		 EditScriptPtr PNTR head, EditScriptPtr PNTR tail, data_t *data)
{
  Int4Ptr SS, DD, II;       /* Forward vectors */
  Int4Ptr RS, RD, RI;       /* Backward vectors */

  EditScriptPtr head1, tail1, head2, tail2;
  Int4 midc, rmidc;
  Int4 start, lower, upper;
  Int4 rstart, rlower, rupper;
  Int4 c, k, t1, t2, t;
  Int4 maxint;
  Int4 mi, mj, mtype;
  Int1 flag;

  /* Boundary cases */
  if (i1 == i2) {
    if (j1 == j2) *head = NULL;
    else {
      head1 = (EditScriptPtr) MemNew(sizeof(EditScript));
      head1->op_type = INS;
      head1->num = j2-j1;
      head1->next = NULL;
      *head = *tail = head1;
    }
    return;
  }

  if (j1 == j2) {
    head1 = (EditScriptPtr) MemNew(sizeof(EditScript));
    head1->op_type = DEL;
    head1->num = i2-i1;
    head1->next = NULL;
    *head = *tail = head1;
    return;
  }

  if (dist <= 1) {
    if (j2-i2 == j1-i1) {
      head1 = (EditScriptPtr) MemNew(sizeof(EditScript));
      head1->op_type = SUB;
      head1->num = i2-i1;
      head1->next = NULL;
      *head = *tail = head1;
    } else if (j2-i2 > j1-i1) {
      if (type1 == INS) {
	head1 = (EditScriptPtr) MemNew(sizeof(EditScript));
	head1->op_type = INS;
	head1->num = 1;
	head2 = (EditScriptPtr) MemNew(sizeof(EditScript));
	head2->op_type = SUB;
	head2->num = i2-i1;
      } else {
	head1 = (EditScriptPtr) MemNew(sizeof(EditScript));
	head1->op_type = SUB;
	head1->num = i2-i1;
	head2 = (EditScriptPtr) MemNew(sizeof(EditScript));
	head2->op_type = INS;
	head2->num = 1;
      }
      head1->next = head2;
      head2->next = NULL;
      *head = head1;
      *tail = head2;
    } else if (j2-i2 < j1-i1) {
      if (type1 == DEL) {
	head1 = (EditScriptPtr) MemNew(sizeof(EditScript));
	head1->op_type = DEL;
	head1->num = 1;
	head2 = (EditScriptPtr) MemNew(sizeof(EditScript));
	head2->op_type = SUB;
	head2->num = j2-j1;
      } else {
	head1 = (EditScriptPtr) MemNew(sizeof(EditScript));
	head1->op_type = SUB;
	head1->num = j2-j1;
	head2 = (EditScriptPtr) MemNew(sizeof(EditScript));
	head2->op_type = DEL;
	head2->num = 1;
      }
      head1->next = head2;
      head2->next = NULL;
      *head = head1;
      *tail = head2;
    }
    return;
  }

  /* Divide the problem at the middle cost */
  midc = dist/2;
  rmidc = dist - midc;

  /* Compute the boundary diagonals */
  start = j1 - i1;
  lower = MAX(j1-i2, start-midc);
  upper = MIN(j2-i1, start+midc);
  rstart = j2-i2;
  rlower = MAX(j1-i2, rstart-rmidc);
  rupper = MIN(j2-i1, rstart+rmidc);

  /* Allocate space for forward vectors */
  SS = (Int4Ptr )MemNew((upper-lower+1)*sizeof(Int4)) - lower;
  DD = (Int4Ptr )MemNew((upper-lower+2)*sizeof(Int4)) - lower;
  II = (Int4Ptr )MemNew((upper-lower+2)*sizeof(Int4)) - lower + 1;

  /* Forward computation */
  for (k=lower; k<=upper; ++k) SS[k] = MININT;
  for (k=lower; k<=upper+1; ++k) DD[k] = MININT;
  for (k=lower-1; k<=upper; ++k) II[k] = MININT;
  if (type1 == SUB) SS[start] = snake(start, i1, i2, j2, data);
  else if (type1 == DEL) {
    DD[start] = i1;
    SS[start] = snake(start, i1, i2, j2, data);
  } else {
    II[start] = i1;
    SS[start] = snake(start, i1, i2, j2, data);
  }

  for (c = 1; c <= midc; ++c) {
    t = MAX(lower, start-c);
    t1 = II[t-1];
    for (k = t; k <= MIN(upper, start + c); ++k) {
      t2 = II[k];
      II[k] = MAX(t1, SS[k]);
      t1 = t2;
      DD[k] = MAX(DD[k + 1] + 1, SS[k]);
      SS[k] = snake(k, MIN(j2 - k, MAX(MAX(SS[k] + 1, II[k]), DD[k])), i2, j2, data);
    }
  }

  /* Allocate space for backward vectors */
  RS = (Int4Ptr )MemNew((rupper-rlower+1)*sizeof(Int4)) - rlower;
  RD = (Int4Ptr )MemNew((rupper-rlower+2)*sizeof(Int4)) - rlower + 1;
  RI = (Int4Ptr )MemNew((rupper-rlower+2)*sizeof(Int4)) - rlower;

  /* Backward computation */
  maxint = i2 + dist + data->N;
  for(k=rlower; k<=rupper; ++k) RS[k] = maxint;
  for(k=rlower-1; k<=rupper; ++k) RD[k] = maxint;
  for(k=rlower; k<=rupper+1; ++k) RI[k] = maxint;
  if(type2 == SUB)
    RI[rstart] = RD[rstart] = RS[rstart] = rsnake(rstart, i2, i1, j1, data);
  else if (type2 == DEL) RD[rstart] = i2;
  else RI[rstart] = i2;

  for(c = 1; c <= rmidc; ++c) {
    t = MAX(rlower, rstart - c);
    t1 = RD[t - 1];
    for(k = t; k <= MIN(rupper, rstart + c); ++k) {
      RS[k] = rsnake(k, MAX(j1 - k, MIN(MIN(RS[k] - 1, RD[k]), RI[k])), i1, j1, data);
      t2 = RD[k];
      RD[k] = MIN(t1 - 1, RS[k]);
      t1 = t2;
      RI[k] = MIN(RI[k + 1], RS[k]);
    }
  }

  /* Find (mi, mj, mtype) such that
     the distance from (i1, j1, type1) to (mi, mj, mtype) is midc
     and the distance from (mi, mj, mtype) to (i2, j2, type2) is rmidc.
     */

  flag = FALSE;
  for (k=MAX(lower,rlower); k<=MIN(upper,rupper);++k) {

    if (SS[k]>=RS[k] || DD[k]>=RD[k] || II[k]>=RI[k]) {
      if (DD[k]>=RD[k]) {
	mi = DD[k];
	mj = k+mi;
	mtype = DEL;
      } else if (II[k] >= RI[k]) {
	mi = II[k];
	mj = k+mi;
	mtype = INS;
      } else {
	mi = SS[k];
	mj = k+mi;
	mtype = SUB;
      }

      flag = TRUE;
      break;
    }
  }

  /* Free working vectors */
  MemFree(SS+lower);
  MemFree(DD+lower);
  MemFree(II+lower-1);
  MemFree(RS+rlower);
  MemFree(RD+rlower-1);
  MemFree(RI+rlower);

  if (flag) {
    /* Find a path from (i1,j1,type1) to (mi,mj,mtype) */
    path(i1, j1, type1, mi, mj, mtype, midc, &head1, &tail1, data);

    /* Find a path from (mi,mj,mtype) to (i2,j2,type2) */
    path(mi, mj, mtype, i2, j2, type2, rmidc, &head2, &tail2, data);

    /* Join these two paths together */
    if(head1) tail1->next = head2;
    else head1 = head2;
  }
  else {
    ErrPostEx(SEV_WARNING, 0, 0, "Something wrong when dividing");

    /* printf("Something wrong when dividing\n"); */
    head1 = NULL;
  }
  *head = head1;
  if (head2) *tail = tail2;
  else *tail = tail1;
}

/* Condense_script - merge contiguous operations of the same type together */
static void Condense_script(EditScriptPtr head)
{
        EditScriptPtr tp, tp1;

        tp = head;
        while (tp != NULL) {
           while (((tp1 = tp->next) != NULL) && (tp->op_type == tp1->op_type)) {
                 tp->num = tp->num + tp1->num;
                 tp->next = tp1->next;
                 MemFree(tp1);
           }
           tp = tp->next;
        }
}

static void Reverse_Script(EditScriptPtr head)
{
        EditScriptPtr tp;

        tp = head;
        while (tp != NULL) {
           if (tp->op_type == INS) tp->op_type = DEL;
           else if (tp->op_type == DEL) tp->op_type = INS;
           tp = tp->next;
        }
}

static void Print_Script(EditScriptPtr head, Int4 M, Int4 N, data_t *data)
{
  EditScriptPtr tp;
  Int4 i, j, k, count;

  i = j = 0;
  tp = head;

  while(tp) {
    if(tp->op_type == SUB) {
      k = 0;
      while(k < tp->num) {
	count = 0;
	while((data->seq1[i] == data->seq2[j]) && (k < tp->num)) {
	  ++i;
	  ++j;
	  ++count;
	  ++k;
	}
	if(count > 0) fprintf(stdout, "copy %d\n", count);
	if(k < tp->num) {
	  fprintf(stdout, "replace %c by %c\n", data->seq1[i], data->seq2[j]);
	  ++i;
	  ++j;
	  ++k;
	}
      }
    }
    else if (tp->op_type == INS) {
      if((tp == head || tp->next == NULL) && (M <= N))
	fprintf(stdout, "skip (second sequence) %d\n", tp->num);
      else {
	/*
	  printf("insert %d\n", tp->num);
	  */
	fprintf(stdout, "insert ");
	if(tp->num <= 10)
	  for(k = j; k < j + tp->num; ++k) fprintf(stdout, "%c", data->seq2[k]);
	else fprintf(stdout, " %d ", tp->num);
	fprintf(stdout, "\n");
      }
      j += tp->num;
    }
    else {     /* DEL */
      if((tp == head || tp->next == NULL) && (M > N))
	fprintf(stdout, "skip (first sequence) %d\n", tp->num);
      else {
	/*
	  printf("delete %d\n", tp->num);
	  */
	fprintf(stdout, "delete ");
	if(tp->num <= 10)
	  for(k=i; k<i+tp->num; ++k)
	    fprintf(stdout, "%c", data->seq1[k]);
	else
	  fprintf(stdout, "%d ", tp->num);
	fprintf(stdout, "\n");
      }
      i += tp->num;
    }
    tp = tp->next;
  }
}

static void S2A(EditScriptPtr head, Int4Ptr S)
{
        EditScriptPtr tp;
        Int4Ptr lastS;
	Int4 i;

        tp = head;
        lastS = S;
        while (tp != NULL) {
/*
        printf("tp->op_type=%d, tp->num=%d\n",tp->op_type, tp->num);
*/
           if (tp->op_type == SUB) {
                for (i=0; i<tp->num; ++i) *lastS++ = 0;
           } else if (tp->op_type == INS) {
                *lastS++ = tp->num;
           } else {     /* DEL */
                *lastS++ = 0 - tp->num;
           }
           tp = tp->next;
        }
}



/* Alignment display routine */

static Int1 ALINE[51], BLINE[51], CLINE[51];

static void DISPLAY(Int1 A[], Int1 B[], Int4 M, Int4 N, Int4 S[], Int4 AP, Int4 BP)
{ register Int1 *a, *b, *c;
  register Int4   i,  j, op;
           Int4   lines, ap, bp;

  i = j = op = lines = 0;
  ap = AP;
  bp = BP;
  a = ALINE;
  b = BLINE;
  c = CLINE;
  while (i < M || j < N)
    { if (op == 0 && *S == 0)
        { op = *S++;
          *a = A[++i];
          *b = B[++j];
          *c++ = (*a++ == *b++) ? '|' : ' ';
        }
      else
        { if (op == 0)
            op = *S++;
          if (op > 0)
            { *a++ = ' ';
              *b++ = B[++j];
              op--;
            }
          else
            { *a++ = A[++i];
              *b++ = ' ';
              op++;
            }
          *c++ = '-';
        }
      if (a >= ALINE+50 || i >= M && j >= N)
        { *a = *b = *c = '\0';
          fprintf(stdout, "\n%6ld ",(long int)50*lines++);
          for (b = ALINE+10; b <= a; b += 10)
            fprintf(stdout, "    .    :");
          if (b <= a+5)
            fprintf(stdout, "    .");
          fprintf(stdout, "\n%6d %s\n       %s\n%6d %s\n",ap,ALINE,CLINE,bp,BLINE);
          ap = AP + i;
          bp = BP + j;
          a = ALINE;
          b = BLINE;
          c = CLINE;
        }
    }
}



static Int4 snake(Int4 k, Int4 x, Int4 endx, Int4 endy, data_t *data)
{
  Int4 y;

  if(x < 0) return x;
  y = x + k;

  while (x < endx && y < endy && data->seq1[x] == data->seq2[y]) {
    ++x; ++y;
  }

  return x;
}


static Int4 rsnake(Int4 k, Int4 x, Int4 startx, Int4 starty, data_t *data)
{
  Int4 y;

  if(x > data->M) return x;
  y = x + k;
  while(x > startx && y > starty && data->seq1[x - 1] == data->seq2[y - 1]) {
    --x; --y;
  }

  return x;
}

static Int4 get_dist(Int4 i1, Int4 j1, Int4 i2, Int4 j2, data_t *data)
{
  Int4Ptr SS, DD, II;
  Int4 goal_diag;
  Int4 c, k, t1, t2, t;
  Int4 start, lower, upper;

  /* Compute the boundary diagonals */
  /* printf("i1 = %ld, j1 = %ld, i2= %ld, j2=%ld, limit = %ld", i1, j1, i2, j2, data->limit); */
  start = j1 - i1;
  lower = MAX(j1 - i2, start - data->limit);
  upper = MIN(j2 - i1, start + data->limit);
  goal_diag = j2 - i2;

  if(goal_diag > upper || goal_diag < lower) {
	/* ErrPostEx(SEV_WARNING, 0, 0, "The two sequences are not really similar"); */
	return -1;

    /* printf("The two sequences are not really similar.\n");
    printf("Please try an exact aligning method.\n");
    exit(1); */
  }

  /* Allocate space for forward vectors */
  SS = (Int4Ptr)MemNew((upper - lower + 1) * sizeof(Int4)) - lower;
  DD = (Int4Ptr)MemNew((upper - lower + 2) * sizeof(Int4)) - lower;
  II = (Int4Ptr)MemNew((upper - lower + 2) * sizeof(Int4)) - lower + 1;

  /* Initialization */
  for(k = lower; k <= upper; ++k) SS[k] = MININT;
  for(k = lower; k <= upper + 1; ++k) DD[k] = MININT;
  for(k = lower - 1; k <= upper; ++k) II[k] = MININT;
  SS[start] = snake(start, i1, i2, j2, data);

  if(SS[goal_diag] >= i2) {
    /*
      #ifdef STATS
      printf("get_dist = %d\n", 0);
      #endif
      */

    /* Free working vectors */
    MemFree(SS+lower);
    MemFree(DD+lower);
    MemFree(II+lower-1);
    return 0;
  }

  for(c = 1; c <= data->limit; ++c) {
    t = MAX(lower, start-c);
    t1 = II[t - 1];
    for(k = t; k <= MIN(upper, start + c); ++k) {
      t2 = II[k];
      II[k] = MAX(t1, SS[k]);
      t1 = t2;
      DD[k] = MAX(DD[k + 1] + 1, SS[k]);
      SS[k] = snake(k, MIN(j2 - k,MAX(MAX(SS[k] + 1, II[k]), DD[k])), i2, j2, data);
    }

    if (SS[goal_diag] >= i2) {
#ifdef STATS
      printf("get_dist = %d\n",c);
#endif

      /* Free working vectors */
      MemFree(SS + lower);
      MemFree(DD + lower);
      MemFree(II + lower - 1);
      return c;
    }
  }

  /* Ran out of distance limit */
  /* ErrPostEx(SEV_WARNING, 0, 0, "The two sequences are not really similar"); */
  return -1;
  /* printf("The two sequences are not really similar.\n");
  printf("Please try an exact method.\n");
  exit(1); */
}


static Int1 locate(Int4Ptr i1, Int4Ptr j1, Int4Ptr i2, Int4Ptr j2,
                   Int1 *A, Int1 *B, Int4 m, Int4 n, Int4 W1,
		   data_t *data)
{
  Int4 mask;
  Int4 BUCKETS;
  Int1Ptr bucket;
  Int4 i, j;
  Int1Ptr p, q;
  Int4 ecode, ecode1;
  Int4 fcount, bcount, fmax, bmax;
  float trust_ratio;
  Int4 paired_fj, paired_bj, best_range, t;
  ValNodePtr prev = NULL, data_list = NULL;

  typedef struct max_chain {
    Int4 j;  /* j position */
    struct max_chain *next;
  } max_chain;
  max_chain *fj, *bj, *tj, *tj1, *tj2;

  if(m < W1 + NW - 1) {
    /*              printf("m=%d, The sequences are too short for the heuristic method.\n",m);
     */
    return FALSE;
  }

  /* Set up the hash table (derived from Sequence A) */

  mask = (1 << (W1 + W1 - 2)) - 1;
  BUCKETS = power2(2 * W1);
  bucket = (Int1Ptr) MemNew(BUCKETS * sizeof(Int1));
  for (i = 0; i<BUCKETS; ++i) bucket[i] = 0;

  /* front end */
  p = A;

  ecode = 0L;
  for(i = 1; i < W1; ++i) {
    ecode = (ecode << 2) + data->encoding[*p++];
  }

  for(i = 0; i < NW; ++i) {
    ecode = ((ecode & mask) << 2) + data->encoding[*p++];
    bucket[ecode] = 1;
  }
  /* back end */
  p = A + m - W1 - NW + 1;
  ecode = 0L;
  for(i = 1; i < W1; ++i)
    ecode = (ecode << 2) + data->encoding[*p++];
  for(i = 0; i < NW; ++i) {
    ecode = ((ecode & mask) << 2) + data->encoding[*p++];
    if (bucket[ecode] == 1 || bucket[ecode] == 3) bucket[ecode] = 3;
    else bucket[ecode] = 2;
  }

  /* Locate the interval with the maximum number of the matched 8-tuples */
  trust_ratio = 0.6;
  fmax = bmax = NW * trust_ratio;
  fj = bj = NULL;
  fcount = bcount = 0;
  p = B;
  ecode = 0L;
  for(j = 1; j < W1; ++j)
    ecode = (ecode << 2) + data->encoding[*p++];
  ecode1 = ecode;
  q = p;
  for(j = 0; j < NW; ++j) {
    ecode = ((ecode & mask) << 2) + data->encoding[*p++];
    if(bucket[ecode] == 1 || bucket[ecode] == 3) ++fcount;
    if(bucket[ecode] == 2 || bucket[ecode] == 3) ++bcount;
  }

  if (fcount >= fmax) {
    fj = (max_chain *) MemNew(sizeof(max_chain));
    link_to_data_list((Pointer)fj, &data_list, &prev);
    fj->j = 0;
    fj->next = NULL;
    fmax = fcount;
  }
  if (bcount >= bmax) {
    bj = (max_chain *) MemNew(sizeof(max_chain));
    link_to_data_list((Pointer)bj, &data_list, &prev);
    bj->j = W1 + NW - 2;
    bj->next = NULL;
    bmax = bcount;
  }

  for(j = 1; j <= n - W1 - NW + 1; ++j) {
    ecode1 = ((ecode1 & mask) << 2) + data->encoding[*q++];
    if (bucket[ecode1] == 1 || bucket[ecode1] == 3) --fcount;
    if (bucket[ecode1] == 2 || bucket[ecode1] == 3) --bcount;
    ecode = ((ecode & mask) << 2) + data->encoding[*p++];
    if (bucket[ecode] == 1 || bucket[ecode] == 3) ++fcount;
    if (bucket[ecode] == 2 || bucket[ecode] == 3) ++bcount;

    if(fcount > fmax) {
      while(fj != NULL) {
	tj = fj->next;
	/* MemFree(fj); */
	fj = tj;
      }
      fj = (max_chain *) MemNew(sizeof(max_chain));
      link_to_data_list((Pointer)fj, &data_list, &prev);
      fj->j = j;
      fj->next = NULL;
      fmax = fcount;
    }
    else if(fcount == fmax) {
      tj = (max_chain *) MemNew(sizeof(max_chain));
      link_to_data_list((Pointer)tj, &data_list, &prev);
      tj->j = j;
      tj->next = fj;
      fj = tj;
    }
    if(bcount > bmax) {
      while(bj != NULL) {
	tj = bj->next;
	/* MemFree(bj); */
	bj = tj;
      }
      bj = (max_chain *) MemNew(sizeof(max_chain));
      link_to_data_list((Pointer)bj, &data_list, &prev);
      bj->j = j + W1 + NW - 2;
      bj->next = NULL;
      bmax = bcount;
    }
    else if(bcount == bmax) {
      tj = (max_chain *) MemNew(sizeof(max_chain));
      link_to_data_list((Pointer)tj, &data_list, &prev);
      tj->j = j + W1 + NW - 2;
      tj->next = bj;
      bj = tj;
    }
  }

  MemFree(bucket);

  if(fj == NULL || bj == NULL)
  {
     if(data_list != NULL)
         ValNodeFreeData(data_list);
      return FALSE;
  }

  best_range = (Int4)(PERCENT * m) + 1;
  for(tj1 = fj; tj1 != NULL; tj1 = tj1->next) {
    for(tj2 = bj; tj2 != NULL; tj2 = tj2->next) {
      t = tj2->j - tj1->j + 1 - m;
      if(t < 0) t = -t;

      if(t < best_range) {
	best_range = t;
	paired_fj = tj1->j;
	paired_bj = tj2->j;
      }
    }
  }

  ValNodeFreeData(data_list);
  if(best_range > (Int4)(PERCENT * m)) return FALSE;

  *i1 = 0;
  *i2 = m;
  *j1 = paired_fj;
  *j2 = paired_bj + 1;

  return TRUE;
}
