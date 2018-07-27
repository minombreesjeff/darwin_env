static char const rcsid[] = "$Id: csim.c,v 6.2 2003/05/30 17:25:36 coulouri Exp $";

/*  csim.c
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
* File Name:  csim.c
*
* Author:  Jinghui Zhang
*
* Version Creation Date: 5/24/95
*
*
* File Description:  functions for using the sim/sim2 algorithm
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
*
* $Log: csim.c,v $
* Revision 6.2  2003/05/30 17:25:36  coulouri
* add rcsid
*
* Revision 6.1  1998/09/09 11:47:43  sirotkin
* MAXSEG to NLM_MAX_SEG to avoid RS6000 occasional conflict
*
* Revision 6.0  1997/08/25 18:52:57  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/03/14 15:45:14  kans
* undefine DEL and INS if previously defined in simutil.h
*
 * Revision 5.0  1996/05/28  13:43:15  ostell
 * Set to revision 5.0
 *
 * Revision 4.1  1995/09/27  19:46:44  zjing
 * protection for the default gap value
 *
 * Revision 4.0  1995/07/26  13:50:15  ostell
 * force revision to 4.0
 *
 * Revision 1.2  1995/07/17  20:36:21  kans
 * prototypes for Mac compilers
 *
 * Revision 1.1  1995/07/17  19:01:41  zjing
 * Initial revision
 *
*
*
* ==========================================================================
*/

#include <simutil.h>


typedef struct ONE 
	{ 
	  Int4 COL ;  
	  struct ONE  PNTR NEXT ;
	} pair, PNTR pairptr;

typedef struct NODE
	{ 
	  Int4  SCORE;
	  Int4  STARI;
	  Int4  STARJ;
	  Int4  ENDI;
	  Int4  ENDJ;
	  Int4  TOP;
	  Int4  BOT;
	  Int4  LEFT;
	  Int4  RIGHT; 
	 }  vertex, PNTR vertexptr;
		
typedef struct NODE_LIST
  	{
	  vertexptr  PNTR LIST;			/* an array for saving k best scores */
	  vertexptr  low;			/* lowest score node in LIST */
	  vertexptr  most;		/* latestly accessed node in LIST */
	  Int4  numnode;	 		/* the number of nodes in LIST */
	  Int4   min;				/* minimum score in LIST */
	  Int4   m1, mm, n1, nn;		/* boundaries of recomputed area */
  	}node_list, PNTR listptr;

typedef struct VEC
   	{
	  Int4Ptr  CC, DD;			/* saving matrix scores */
	  Int4Ptr  RR, SS, EE, FF; 		/* saving start-points */
	  Int4Ptr  HH, WW;		 	/* saving matrix scores */
	  Int4Ptr  II, JJ, XX, YY; 		/* saving start-points */
   	}vec, PNTR vecptr;

typedef struct MATRIX
	{
	  Int2Ptr PNTR v;
	  Int4 q, r, qr;
	}matrix;

typedef struct SCRIPT
  	{
     	  Int4  PNTR sapp;				/* Current script append ptr */
     	  Int4   last;				/* Last script op appended */
     	  Int4  I, J;				/* current positions of A ,B */
     	  Int4  no_mat; 			/* number of matches */ 
     	  Int4  no_mis; 			/* number of mismatches */ 
     	  Int4  al_len; 			/* length of alignment */
  	}script, PNTR scriptr;


static pairptr z, PNTR row;			/*for saving used aligned paires*/

static SeqAlignPtr SIM(SeqLocPtr loc1, SeqLocPtr loc2, CharPtr A, CharPtr B, Int4 M, Int4 N, Int4 K, Int4 nseq, FloatHi cut_off, Int2Ptr PNTR V, Int4 Q, Int4 R);
static void big_pass(CharPtr A, CharPtr B, Int4 M, Int4 N, Int4 K, Int4 nseq, matrix value, vecptr s_vec, listptr plist, pairptr PNTR row) ;
static void diff(CharPtr A, CharPtr B, Int4 M, Int4 N, Int4 tb, Int4 te, matrix value, scriptr s_ptr, pairptr PNTR row) ;
static Int4 diff1(CharPtr A, CharPtr B, Int4 M, Int4 N, Int4 tb, Int4 te, Int4Ptr CC, Int4Ptr DD, Int4Ptr RR, Int4Ptr SS, matrix value, scriptr s_ptr, pairptr PNTR row);
static void locate(CharPtr A, CharPtr B, Int4 nseq, matrix value, vecptr s_vec, BoolPtr flag, listptr plist, pairptr PNTR row) ;
static void small_pass(CharPtr A, CharPtr B, Int4 count, Int4 nseq, matrix value, vecptr s_vec, listptr plist, pairptr PNTR row) ;
static Int4 addnode(Int4 c, Int4 ci, Int4 cj, Int4 i, Int4 j, Int4 K, listptr plist)  ;
static vertexptr findmax(listptr plist);
static Boolean no_cross(BoolPtr flag, listptr plist, Int4Ptr rl, Int4Ptr cl);
static void display(CharPtr A, CharPtr B, Int4 M, Int4 N, Int4Ptr S, Int4 AP, Int4 BP, Int4Ptr rec_a1, Int4Ptr rec_a2, Int2Ptr POS) ;

/*****************************************************************************
***
*	CSIM() return the Seq-align objects for the two Seq-locs aligned with
*	the sim algorithm. 
*	for protein sequences, if pam_file == NULL, use the default matrix
*	PAM 200
*
*****************************************************************************
***/


/***************************************************************************
***
*	set_matrix: functions for set the matrix
*
****************************************************************************
***/
static void set_pam_200(Int2Ptr PNTR V)
{
 static Int2 wgts[23][23] = {	/* the PAM200 matrix */
 
 2,-2, 0, 0,-2,-1, 0, 1,-1,-1,-2,-1,-1,-3, 1, 1, 1,-5,-3, 0, 1, 1, 0,
 -2, 5, 0,-1,-3, 1,-1,-3, 1,-2,-2, 2, 0,-3, 0, 0,-1, 1,-4,-2, 0, 1, 0,
  0, 0, 2, 2,-3, 0, 1, 0, 1,-1,-2, 1,-2,-2, 0, 1, 0,-4,-1,-2, 3, 2, 0,
  0,-1, 2, 3,-4, 1, 3, 0, 0,-2,-3, 0,-2,-4,-1, 0, 0,-6,-3,-2, 4, 3, 0,
 -2,-3,-3,-4, 8,-4,-4,-3,-3,-2,-5,-4,-4,-4,-2, 0,-2,-6, 0,-2,-3,-3, 0,
 -1, 1, 0, 1,-4, 4, 2,-1, 2,-2,-1, 0,-1,-4, 0,-1,-1,-4,-3,-2, 2, 4, 0,
  0,-1, 1, 3,-4, 2, 3, 0, 0,-2,-3, 0,-2,-4, 0, 0, 0,-6,-3,-2, 3, 4, 0,
  1,-3, 0, 0,-3,-1, 0, 4,-2,-2,-3,-2,-3,-3,-1, 1, 0,-5,-4,-1, 1, 0, 0,
 -1, 1, 1, 0,-3, 2, 0,-2, 5,-2,-2, 0,-2,-1, 0,-1,-1,-3, 0,-2, 2, 2, 0,
 -1,-2,-1,-2,-2,-2,-2,-2,-2, 4, 2,-1, 2, 1,-2,-1, 0,-5,-1, 3,-1,-1, 0,
 -2,-2,-2,-3,-5,-1,-3,-3,-2, 2, 4,-2, 3, 1,-2,-2,-1,-4,-1, 1,-2,-1, 0,
 -1, 2, 1, 0,-4, 0, 0,-2, 0,-1,-2, 4, 1,-4,-1, 0, 0,-3,-4,-2, 1, 1, 0,
 -1, 0,-2,-2,-4,-1,-2,-3,-2, 2, 3, 1, 5, 0,-2,-1, 0,-4,-2, 1,-1, 0, 0,
 -3,-3,-2,-4,-4,-4,-4,-3,-1, 1, 1,-4, 0, 7,-4,-2,-2, 0, 5,-1,-2,-3, 0,
  1, 0, 0,-1,-2, 0, 0,-1, 0,-2,-2,-1,-2,-4, 5, 1, 0,-5,-4,-1, 0, 1, 0,
  1, 0, 1, 0, 0,-1, 0, 1,-1,-1,-2, 0,-1,-2, 1, 2, 1,-2,-2,-1, 1, 1, 0,
  1,-1, 0, 0,-2,-1, 0, 0,-1, 0,-1, 0, 0,-2, 0, 1, 3,-4,-2, 0, 1, 0, 0,
 -5, 1,-4,-6,-6,-4,-6,-5,-3,-5,-4,-3,-4, 0,-5,-2,-4,12, 0,-6,-3,-4, 0,
 -3,-4,-1,-3, 0,-3,-3,-4, 0,-1,-1,-4,-2, 5,-4,-2,-2, 0, 7,-2,-1,-2, 0,
  0,-2,-2,-2,-2,-2,-2,-1,-2, 3, 1,-2, 1,-1,-1,-1, 0,-6,-2, 4,-1,-1, 0,
  1, 0, 3, 4,-3, 2, 3, 1, 2,-1,-2, 1,-1,-2, 0, 1, 1,-3,-1,-1, 4, 4, 0,
  1, 1, 2, 3,-3, 4, 4, 0, 2,-1,-1, 1, 0,-3, 1, 1, 0,-4,-2,-1, 4, 5, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

  Int2 alpha=23;
  Int2 i, j;
  CharPtr achars="ARNDCQEGHILKMFPSTWYVBZX";

  for(i=0; i<alpha; ++i)
     for(j=0; j<alpha; ++j)
       V[achars[i]][achars[j]]=10*(wgts[i][j]);

}

static Boolean set_pam_file(Int2Ptr PNTR V, CharPtr pam_file)
{
  FILE *fp;
  FloatLo weight;
  Char buf[200], alph[SIZE_MATRIX]; 
  CharPtr p;
  Int2 alph_size;
  Int2 i, j;
 
  	if((fp=FileOpen(pam_file, "r")) == NULL)
	{
		Message(MSG_ERROR, "Fail to open matrix file %s, use PAM200 instead", pam_file);
		return FALSE;
	}

  	while(FileGets(buf, 199, fp)) /*get rid of empty lines*/
     		if (buf[0]==' ' || IS_UPPER(buf[0]))
     			break;
 
	/*check the first line to get the alpha-bet order and size*/
     	for(alph_size=0, p=buf; *p!='\0'; ++p)  
        	if(IS_UPPER(*p))
          		alph[alph_size++]=*p;
 
  	for(i=0; i<alph_size; ++i)
      		for (j =0; j<alph_size; ++j) 
		{
         		fscanf(fp, "%f", &weight);      /**get the matrix**/
         		V[alph[i]][alph[j]]=(Int2)(10*weight);
         	}
 
  	FileClose(fp);
	return TRUE;
}
  

static void DNA_matrix(Int2Ptr PNTR V, FloatHi M_val, FloatHi I_val, FloatHi V_val)
{

 FloatHi parm_M, parm_I, parm_V;


	/***set up values for parm_M, parm_I, parm_V**/
	   parm_M=M_val;
	   parm_M +=(parm_M > 0.0)? 0.05: -0.05;

	   parm_I=I_val;
	   parm_I +=(parm_I > 0.0)? 0.05: -0.05;
		
	   parm_V=V_val;
	   parm_V +=(parm_V > 0.0)? 0.05: -0.05;

	/**set up DNA matrix**/
	V['A']['A']=V['C']['C']=V['G']['G']=V['T']['T']=(Int2)(10*parm_M);
	V['A']['G']=V['G']['A']=V['C']['T']=V['T']['C']=(Int2)(10*parm_I);
	V['A']['C']=V['A']['T']=V['C']['A']=V['C']['G']=(Int2)(10*parm_V);
	V['G']['C']=V['G']['T']=V['T']['A']=V['T']['G']=(Int2)(10*parm_V);
}


/*****************************************************************
*
*	set_matrix(V, pspp, is_dna)
*	set up weight matrix for SIM_1
*
*	V: the matrix
*	pspp: the parameters used for SIM
*	is_dna: if TRUE, DNA sequences are compared
*	return TRUE if PAM200 is set up
*
********************************************************************/
static Boolean set_matrix(Int2Ptr PNTR V, PSimPamPtr pspp, Boolean is_dna)
{

	if(is_dna)
	{
	     DNA_matrix(V, pspp->M_val, pspp->I_val, pspp->V_val);
	     return FALSE;
	}
	else
	{
	    if(pspp->pam_file == NULL)
	    {
	    	set_pam_200(V);
	    	return TRUE;
	    }
	    else
	    {
		if(!set_pam_file(V, pspp->pam_file))
		{
			set_pam_200(V);
			return TRUE;
		}
		else
			return FALSE;
	    }
	}
}





/**get the values for open a gap and gap extension for each base**/
static void gap_val(Int4Ptr open_gap, Int4Ptr ext_gap, FloatHi O_val, FloatHi E_val, Boolean use_default, Boolean pam_200)
{

  FloatHi parm_O, parm_E;

	if(use_default)
	{
		if(pam_200) 	/**PAM200 is useed**/
		{
	    		parm_O=DEFAULT_PAM_O;
	    		parm_E=DEFAULT_PAM_E;
		}
		else
		{
			parm_O = DEFAULT_O;
			parm_E = DEFAULT_E;
		}
	}
	else
	{
	    parm_O=O_val;
	    parm_E=E_val;
	}	

	parm_O += (parm_O >0)? 0.05 : -0.05;
	*open_gap=(Int4)(10 * parm_O);

	parm_E +=(parm_E>0)? 0.05: -0.05;
	*ext_gap=(Int4)(10 * parm_E);
}


/***************************************************************************
***
*	CSIM(): compute K top alignment between two sequence locations (loc1, 
*	loc2). 
*	loc1, loc2: tow sequence locations. For DNA sequences, if the strand 
*		    of loc2 is unknown, it will search in both orientations
*	K:		the number of alignments to compute
*	cut_off:	the cut off score for each alignment
*	sim1_pam:	Parameters for SIM1. set to NULL for default
*
*****************************************************************************
***/
SeqAlignPtr CSIM(SeqLocPtr loc1, SeqLocPtr loc2, Int4 K, FloatHi cut_off, PSimPamPtr sim1_pam)
{
  
  Boolean both_strand;
  SeqAlignPtr out_sap, out_sapC;

  CharPtr seq1, seq2;
  Int4 seq1_len, seq2_len;
  Int4 nseq;
  Boolean is_dna, pam_flag;
  Int2Ptr PNTR V;
  Int4 Q, R;
  Int2 i;

  PSimPam psp;
  PSimPamPtr pspp;
  
	if(loc1 == NULL || loc2 == NULL)
		return NULL;
	if(loc1->choice != SEQLOC_INT || loc2->choice !=SEQLOC_INT)
		return NULL;

	if(sim1_pam == NULL)
	{
		DefaultPSimPam(&psp);
		pspp = &psp;
	}
	else
	{
		/*protect the default gap open and extension value*/
		if(sim1_pam->O != DEFAULT_O || sim1_pam->E != DEFAULT_E)
			sim1_pam->def_gap = FALSE;
		else
			sim1_pam->def_gap = TRUE;

		pspp = sim1_pam;
	}

	if(SeqLocCompare(loc1, loc2) == SLC_A_EQ_B)
	   nseq = 1;
	else
	   nseq = 2;

	both_strand = check_strand_mol (loc2, &is_dna);
	if(both_strand)
		Change_Loc_Strand(loc2, Seq_strand_plus);

	/**initiate the matrix space**/
   	V=MemNew((size_t)(SIZE_MATRIX) * sizeof(Int2Ptr));
   	for(i=0; i<SIZE_MATRIX; ++i)
     	  	V[i]=(Int2Ptr)MemNew((size_t)(SIZE_MATRIX)*sizeof(Int2));

	/**get the score  matrix and the gap values**/
	pam_flag = set_matrix(V, pspp, is_dna);
	gap_val(&Q, &R, pspp->O, pspp->E, pspp->def_gap, pam_flag);


	
	seq1_len = SeqLocLen(loc1);
	seq2_len = SeqLocLen(loc2);
	seq1 = make_sim_seq(loc1, FALSE, NULL);
	seq2 = make_sim_seq(loc2, FALSE, NULL);

	out_sap= SIM(loc1, loc2, seq1, seq2, seq1_len, seq2_len, K, nseq, cut_off, V, Q, R);
	if(both_strand){ 
	    	Change_Loc_Strand(loc2, Seq_strand_minus);
	    	seq2 = make_sim_seq(loc2, FALSE, seq2);
		out_sapC= SIM(loc1, loc2, seq1, seq2, seq1_len, seq2_len, K, nseq, cut_off, V, Q, R);
		out_sap= link_align(out_sapC, out_sap);
		out_sap = get_top_K_alignment(out_sap, K, cut_off);
	}

	MemFree(seq1);
	MemFree(seq2);
        for (i=0; i<SIZE_MATRIX; ++i)
           MemFree ( V[i]);
        MemFree(V);


	return out_sap;

	      
}/**end of CSIM**/



/* DIAG() assigns value to x if (ii,jj) is never used before */
#define DIAG(ii, jj, x, pvalue)				\
{ for ( tt = 1, z = row[(ii)]; z != 0; z = z->NEXT )	\
    if ( z->COL == (jj) )				\
      { tt = 0; break; }				\
  if ( tt )						\
    x = ( pvalue );					\
}

/* replace (ss1, xx1, yy1) by (ss2, xx2, yy2) if the latter is large */
#define ORDER(ss1, xx1, yy1, ss2, xx2, yy2)		\
{ if ( ss1 < ss2 )					\
    { ss1 = ss2; xx1 = xx2; yy1 = yy2; }		\
  else							\
    if ( ss1 == ss2 )					\
      { if ( xx1 < xx2 )				\
	  { xx1 = xx2; yy1 = yy2; }			\
	else						\
	  if ( xx1 == xx2 && yy1 < yy2 )		\
	    yy1 = yy2;					\
      }							\
}



/* SIM(A,B,M,N,K,is_dna,nseq,val) reports K best non-intersecting alignments of
   the segments of A and B in order of similarity scores. If the score 
   is below val, the alignments will be discarded*/

static SeqAlignPtr SIM(SeqLocPtr loc1, SeqLocPtr loc2, CharPtr A, CharPtr B, Int4 M, Int4 N, Int4 K, Int4 nseq, FloatHi cut_off, Int2Ptr PNTR V, Int4 Q, Int4 R)
{

  SeqAlignPtr new, head = NULL;
  Int4Ptr rec_a1, rec_a2;
  Int2  POS;

  Int4 endi, endj, stari, starj;	/* endpoint and startpoint */ 
  Int4  score;   			/* the max score in (plist->LIST) */
  Int4 count;				/* maximum size of list */	
  register  Int4  i;			/* row and column indices */
  Int4  PNTR S;				/* saving operations for diff */
  vertexptr cur; 			/* temporary pointer */
  vecptr s_vec;				/*space for processing*/
  matrix value;				/*values for the matrix and gap penalty*/
  Boolean flag;				/*indicate if recomputation is necessary*/
  Int4 rl, cl;				/*left and top boundaries*/
  scriptr s_ptr;			/* script for the alignment*/
  listptr plist;			/*list for saving the best scores*/
  Int4 q;

  FloatHi re_score; 



	/**allocate the space for recording the result**/
	rec_a1 = MemNew((size_t)NLM_MAXSEG * sizeof(Int4));
	rec_a2 = MemNew((size_t)NLM_MAXSEG * sizeof(Int4));

	/*get the matrix value and gap penalties*/
	value.v=V;
	value.q=q=Q;
	value.r=R;
	value.qr=Q+R;

	/*initiate the siace for process*/
	s_vec=(vecptr)MemNew(sizeof(vec));
	/* allocate space for all vectors */
	/*j =(size_t) (N + 1) * sizeof(Int4);*/
	s_vec->CC = ( Int4Ptr ) MemNew((size_t)(N+1)*sizeof(Int4));
	s_vec->DD = ( Int4Ptr ) MemNew((size_t)(N+1)*sizeof(Int4));
	s_vec->RR = ( Int4Ptr ) MemNew((size_t)(N+1)*sizeof(Int4));
	s_vec->SS = ( Int4Ptr ) MemNew((size_t)(N+1)*sizeof(Int4));
	s_vec->EE = ( Int4Ptr ) MemNew((size_t)(N+1)*sizeof(Int4));
	s_vec->FF = ( Int4Ptr ) MemNew((size_t)(N+1)*sizeof(Int4));
	/*i = (M + 1) * sizeof(Int4);*/
	s_vec->HH = ( Int4Ptr ) MemNew((size_t)(M+1)*sizeof(Int4));
	s_vec->WW = ( Int4Ptr ) MemNew((size_t)(M+1)*sizeof(Int4));
	s_vec->II = ( Int4Ptr ) MemNew((size_t)(M+1)*sizeof(Int4));
	s_vec->JJ = ( Int4Ptr ) MemNew((size_t)(M+1)*sizeof(Int4));
	s_vec->XX = ( Int4Ptr ) MemNew((size_t)(M+1)*sizeof(Int4));
	s_vec->YY = ( Int4Ptr ) MemNew((size_t)(M+1)*sizeof(Int4));
	S = ( Int4Ptr ) MemNew((size_t)(M+N+2)*sizeof(Int4));
	row = ( pairptr PNTR ) MemNew( (size_t)(M + 1) * sizeof(pairptr));

	/* set up list for each row */
	for ( i = 1; i <= M; i++ )
	  if ( nseq == 2 )
	     row[i] = 0;
	  else
	    { row[i] =  ( pairptr ) MemNew( sizeof(pair));
	      z=row[i];
              z->COL = i;			
              z->NEXT = 0;
	    }

	/*initiate the script*/
	plist=(listptr)MemNew(sizeof(node_list));
	plist->low=NULL;
	plist->most=NULL;

	(plist->LIST) = ( vertexptr PNTR ) MemNew((size_t) K * sizeof(vertexptr));
	for ( i = 0; i < K ; i++ )
	   (plist->LIST)[i] = ( vertexptr ) MemNew( sizeof(vertex));
	(plist->numnode) = (plist->min) = 0;
	    s_ptr=(scriptr)MemNew(sizeof(script));


	big_pass(A,B,M,N,K,nseq, value, s_vec, plist, row);

        /* Report the K best alignments one by one. After each alignment is
           output, recompute part of the matrix. First determinee the size
	   of the area to be recomputed, then do the recomputation         */

	for ( count = K - 1; count >= 0 ; count-- )
	  { 
	    POS=0;
	    if ( (plist->numnode) == 0 )
	      fatal("The number of alignments computed is too large");
            cur = findmax(plist); /*Return a pointer to a node with max score*/

	    score = cur->SCORE;
      	    stari = ++cur->STARI;
            starj = ++cur->STARJ;
            endi = cur->ENDI;
            endj = cur->ENDJ;
            (plist->m1) = cur->TOP;
            (plist->mm) = cur->BOT;
            (plist->n1) = cur->LEFT;
            (plist->nn) = cur->RIGHT;
            rl = endi - stari + 1;
            cl = endj - starj + 1;
            s_ptr->I = stari - 1;
            s_ptr->J = starj - 1;
            s_ptr->sapp = S;
            s_ptr->last = 0;
            (s_ptr->al_len) = 0;
            (s_ptr->no_mat) = 0;
	    (s_ptr->no_mis) = 0;

            diff(&A[stari]-1, &B[starj]-1,rl,cl,q,q, value, s_ptr, row);

            /* Output the best alignment */
		re_score = (FloatHi)(score/10.0);

     	   display(&A[stari]-1,&B[starj]-1,rl,cl,S,stari,starj, rec_a1, rec_a2, &POS);
	   if(re_score >= cut_off)	/*save the result to Seq-align*/
	   {
	   	new = make_align(rec_a1, rec_a2, POS, re_score, NULL, loc1, loc2);
	   	head = link_align(new, head);
	   }
	   
            if ( count )
	      { flag = FALSE;

                locate(A,B,nseq, value, s_vec, &flag, plist, row);

                if ( flag )
		   small_pass(A,B,count,nseq, value, s_vec, plist, row);
              }

	  }
	   MemFree(rec_a1);
	   MemFree(rec_a2);
	   MemFree(S);
	   MemFree(s_vec->CC);
	   MemFree(s_vec->DD);
	   MemFree(s_vec->RR);
	   MemFree(s_vec->SS);
	   MemFree(s_vec->EE);
	   MemFree(s_vec->FF);
	   MemFree(s_vec->HH);
	   MemFree(s_vec->WW);
	   MemFree(s_vec->II);
	   MemFree(s_vec->JJ);
	   MemFree(s_vec->XX);
	   MemFree(s_vec->YY);
	   MemFree(s_vec);
	   MemFree(s_ptr);

	     for (i=1; i<=M; i++)
	       MemFree(row[i]);
	     MemFree (row);


	  for(i=0; i<K; i++)
	  MemFree((plist->LIST)[i]);
	  MemFree(plist->LIST);
	  MemFree(plist);

	  return head;
}

/* A big pass to compute K best classes */

static void big_pass(CharPtr A, CharPtr B, Int4 M, Int4 N, Int4 K, Int4 nseq, matrix value, vecptr s_vec, listptr plist, pairptr PNTR row) 
{ 
  register  Int4  i, j;			/* row and column indices */
  register  Int4  c;			/* best score at current point */
  register  Int4  f;			/* best score ending with insertion */
  register  Int4  d;			/* best score ending with deletion */
  register  Int4  p;			/* best score at (i-1, j-1) */
  register  Int4  ci, cj;		/* end-point associated with c */ 
  register  Int4  di, dj;		/* end-point associated with d */
  register  Int4  fi, fj;		/* end-point associated with f */
  register  Int4  pi, pj;		/* end-point associated with p */
  Int2Ptr  va, PNTR v;			/* pointer to v(A[i], B[j]) */
  Int4 q, r, qr;
  Boolean tt;
  /**pairptr z;**/

  q=value.q;
  r=value.r;
  qr=value.qr;
  v=value.v;

	
	/* Compute the matrix and save the top K best scores in (plist->LIST)
	   s_vec->CC : the scores of the current row
	   s_vec->RR and s_vec->EE : the starting point that leads to score s_vec->CC
	   s_vec->DD : the scores of the current row, ending with deletion
	   s_vec->SS and s_vec->FF : the starting point that leads to score s_vec->DD        */
 	/* Initialize the 0 th row */
	for ( j = 1; j <= N ; j++ )
	  {  s_vec->CC[j] = 0;
	     s_vec->RR[j] = 0;
	     s_vec->EE[j] = j;
	     s_vec->DD[j] = - (q);
	     s_vec->SS[j] = 0;
	     s_vec->FF[j] = j;
	  }
	for ( i = 1; i <= M; i++) 
	  {  c = 0;				/* Initialize column 0 */
	     f = - (q);
	     ci = fi = i;
	     va = v[A[i]];
	     if ( nseq == 2 )
	       { p = 0;
	         pi = i - 1;
	         cj = fj = pj = 0;
	       }
	     else
	       { p = s_vec->CC[i];
		 pi = s_vec->RR[i];
		 pj = s_vec->EE[i];
	         cj = fj = i;
	       }
	     for ( j = (nseq == 2 ? 1 : (i+1)) ; j <= N ; j++ )  
	       {  f = f - r;
		  c = c - qr;
		  ORDER(f, fi, fj, c, ci, cj)
		  c = s_vec->CC[j] - qr; 
		  ci = s_vec->RR[j];
		  cj = s_vec->EE[j];
		  d = s_vec->DD[j] - r;
		  di = s_vec->SS[j];
		  dj = s_vec->FF[j];
		  ORDER(d, di, dj, c, ci, cj)
		  c = 0;
		  DIAG(i, j, c, p+va[B[j]])		/* diagonal */
		  if ( c <= 0 )
		    { c = 0; ci = i; cj = j; }
		  else
		    { ci = pi; cj = pj; }
		  ORDER(c, ci, cj, d, di, dj)
		  ORDER(c, ci, cj, f, fi, fj)
		  p = s_vec->CC[j];
		  s_vec->CC[j] = c;
		  pi = s_vec->RR[j];
		  pj = s_vec->EE[j];
		  s_vec->RR[j] = ci;
		  s_vec->EE[j] = cj;
		  s_vec->DD[j] = d;
		  s_vec->SS[j] = di;
		  s_vec->FF[j] = dj;
		  if ( c > (plist->min) )	/* add the score into list */
		    (plist->min) = addnode(c, ci, cj, i, j, K, plist);
	        }
	  }
}

#ifdef DEL
#undef DEL /* in simutil.h */
#endif
#ifdef INS
#undef INS /* in simutil.h */
#endif

#define GAP(k)  ((k) <= 0 ? 0 : q+r*(k))	/* k-symbol indel score */
#define DEL(s_ptr, k)				\
 { (s_ptr->I) += (k);				\
  (s_ptr->al_len) += (k);				\
  if ((s_ptr->last) < 0)				\
    (s_ptr->last) = s_ptr->sapp[-1] -= (k);		\
  else					\
    (s_ptr->last) = *(s_ptr->sapp)++ = -(k);		\
}
						/* Append "s_ptr->Insert k" op */
#define INS(k)				\
{ (s_ptr->J) += k;				\
  (s_ptr->al_len) += k;				\
  if ((s_ptr->last) < 0)				\
    { s_ptr->sapp[-1] = (k); *(s_ptr->sapp)++ = (s_ptr->last); }	\
  else					\
    (s_ptr->last) = *(s_ptr->sapp)++ = (k);		\
}

						/* Append "Replace" op */
#define REP 				\
{ (s_ptr->last) = *(s_ptr->sapp)++ = 0; 			\
  (s_ptr->al_len) += 1;				\
}

static void diff(CharPtr A, CharPtr B, Int4 M, Int4 N, Int4 tb, Int4 te, matrix value, scriptr s_ptr, pairptr PNTR row) 
{ 
  Int4Ptr CC, DD, RR, SS;

  /*j= (N+1) * sizeof (Int4);*/
  CC= (Int4Ptr) MemNew ((size_t)(N+1)*sizeof(Int4));
  DD= (Int4Ptr) MemNew ((size_t)(N+1)*sizeof(Int4));
  RR= (Int4Ptr) MemNew ((size_t)(N+1)*sizeof(Int4));
  SS= (Int4Ptr) MemNew ((size_t)(N+1)*sizeof(Int4));

  diff1(A, B, M, N, tb, te, CC, DD, RR, SS, value, s_ptr, row);
  MemFree(CC);
  MemFree(DD);
  MemFree(RR);
  MemFree(SS);

}

static Int4 diff1(CharPtr A, CharPtr B, Int4 M, Int4 N, Int4 tb, Int4 te, Int4Ptr CC, Int4Ptr DD, Int4Ptr RR, Int4Ptr SS, matrix value, scriptr s_ptr, pairptr PNTR row)
{
  Int4   midi, midj, type;	/* Midpoint, type, and cost */
  Int4 midc;
  Int4 q, r, qr;	/* matrix and the gap penalty value*/
  Int2Ptr PNTR v;
  Boolean tt;
  /**pairptr z;**/

  register Int4   i, j;
  register Int4 c, e, d, s;
  Int4 t;
  Int2 PNTR va;

  q=value.q;
  r=value.r;
  qr=value.qr;
  v=value.v;



/* Boundary cases: M <= 1 or N == 0 */

  if (N <= 0)
    { if (M > 0) DEL(s_ptr, M)
      return - GAP(M);
    }
  if (M <= 1)
    { if (M <= 0)
        { INS(N);
          return - GAP(N);
        }
      if (tb > te) tb = te;
      midc = - (tb + r + GAP(N) );
      midj = 0;
      va = v[A[1]];
      for (j = 1; j <= N; j++)
        {  for ( tt = 1, z = row[(s_ptr->I)+1]; z != 0; z = z->NEXT )	
              if ( z->COL == j+(s_ptr->J) )			
	         { tt = 0; break; }		
           if ( tt )			
            { c = va[B[j]] - ( GAP(j-1) + GAP(N-j) );
              if (c > midc)
               { midc = c;
                 midj = j;
               }
	    }
	}
      if (midj == 0)
        { INS(N) DEL(s_ptr, 1) }
      else
        { if (midj > 1) INS(midj-1)
          REP
	  if ( A[1] == B[midj] )
	     (s_ptr->no_mat) += 1;
	  else
	     (s_ptr->no_mis) += 1;
	  /* mark (A[(s_ptr->I)],B[J]) as used: put J into list row[(s_ptr->I)] */	
          (s_ptr->I)++; (s_ptr->J)++;
	  /**if(row[(s_ptr->I)]==NULL)
	    row[(s_ptr->I)] = ( pairptr ) MemNew( sizeof(pair));
          (row[(s_ptr->I)])->COL = s_ptr->J;**/			
	  z = (pairptr)MemNew(sizeof(pair));
	  z->COL = s_ptr->J;
	  z->NEXT = row[s_ptr->I];
	  row[s_ptr->I] = z;
          if (midj < N) INS(N-midj)
        }
      return midc;
    }

/* Divide: Find optimum midpoint (midi,midj) of cost midc */

  midi = M/2;			/* Forward phase:                          */
  CC[0] = 0;			/*   Compute C(M/2,k) & D(M/2,k) for all k */
  t = -q;
  for (j = 1; j <= N; j++)
    { CC[j] = t = t-r;
      DD[j] = t-q;
    }
  t = -tb;
  for (i = 1; i <= midi; i++)
    { s = CC[0];
      CC[0] = c = t = t-r;
      e = t-q;
      va = v[A[i]];
      for (j = 1; j <= N; j++)
        { if ((c = c - qr) > (e = e - r)) e = c;
          if ((c = CC[j] - qr) > (d = DD[j] - r)) d = c;
	  DIAG(i+(s_ptr->I), (s_ptr->J)+j, c, s+v[A[i]][B[j]])
          if (c < d) c = d;
          if (c < e) c = e;
          s = CC[j];
          CC[j] = c;
          DD[j] = d;
        }
    }
  DD[0] = CC[0];

  RR[N] = 0;			/* Reverse phase:                          */
  t = -q;			/*   Compute R(M/2,k) & S(M/2,k) for all k */
  for (j = N-1; j >= 0; j--)
    { RR[j] = t = t-r;
      SS[j] = t-q;
    }
  t = -te;
  for (i = M-1; i >= midi; i--)
    { s = RR[N];
      RR[N] = c = t = t-r;
      e = t-q;
      va = v[A[i+1]];
      for (j = N-1; j >= 0; j--)
        { if ((c = c - qr) > (e = e - r)) e = c;
          if ((c = RR[j] - qr) > (d = SS[j] - r)) d = c;
	  DIAG(i+1+s_ptr->I, j+1+s_ptr->J, c, s+v[A[i+1]][B[j+1]])
          if (c < d) c = d;
          if (c < e) c = e;
          s = RR[j];
          RR[j] = c;
          SS[j] = d;
        }
    }
  SS[N] = RR[N];

  midc = CC[0]+RR[0];		/* Find optimal midpoint */
  midj = 0;
  type = 1;
  for (j = 0; j <= N; j++)
    if ((c = CC[j] + RR[j]) >= midc)
      if (c > midc || CC[j] != DD[j] && RR[j] == SS[j])
        { midc = c;
          midj = j;
        }
  for (j = N; j >= 0; j--)
    if ((c = DD[j] + SS[j] + q) > midc)
      { midc = c;
        midj = j;
        type = 2;
      }

/* Conquer: recursively around midpoint */

  if (type == 1)
    { diff1(A,B,midi,midj,tb,q, CC, DD, RR, SS, value, s_ptr, row);
      diff1(A+midi,B+midj,M-midi,N-midj,q,te, CC, DD, RR, SS, value, s_ptr, row);
    }
  else
    { diff1(A,B,midi-1,midj,tb,(Int4)0, CC, DD, RR, SS, value, s_ptr, row);
      DEL(s_ptr, 2);
      diff1(A+midi+1,B+midj,M-midi-1,N-midj,(Int4)0,te, CC, DD, RR, SS, value, s_ptr, row);
    }
  return midc;
}


/* Determine the left and top boundaries of the recomputed area */
static void locate(CharPtr A, CharPtr B, Int4 nseq, matrix value, vecptr s_vec, BoolPtr flag, listptr plist, pairptr PNTR row) 
{ 
  register  Int4  i, j;			/* row and column indices */
  register  Int4  c;			/* best score at current point */
  register  Int4  f;			/* best score ending with insertion */
  register  Int4  d;			/* best score ending with deletion */
  register  Int4  p;			/* best score at (i-1, j-1) */
  register  Int4  ci, cj;		/* end-point associated with c */ 
  register  Int4  di, dj;		/* end-point associated with d */
  register  Int4  fi, fj;		/* end-point associated with f */
  register  Int4  pi, pj;		/* end-point associated with p */
  short  cflag, rflag;			/* for recomputation */
  Int2Ptr  va;				/* pointer to v(A[i], B[j]) */
  Int4  limit;				/* the bound on j */
  Int4 q, r, qr;			/*gap penalty*/
  Int2Ptr PNTR v; 
  Boolean tt;				/*flag for  DIAG()*/
  Int4 rl, cl;
  /*pairptr z;*/				/* for DIAG()*/

	q=value.q;
	r=value.r;
	qr=value.qr;
	v=value.v;
	/* Reverse pass
	   rows
	   s_vec->CC : the scores on the current row
	   s_vec->RR and s_vec->EE : the endpoints that lead to s_vec->CC
	   s_vec->DD : the deletion scores 
	   s_vec->SS and s_vec->FF : the endpoints that lead to s_vec->DD

	   columns
	   s_vec->HH : the scores on the current columns
	   s_vec->II and s_vec->JJ : the endpoints that lead to s_vec->HH
	   s_vec->WW : the deletion scores
	   s_vec->XX and s_vec->YY : the endpoints that lead to s_vec->WW
	*/
	for ( j = (plist->nn); j >= (plist->n1) ; j-- )
          {  s_vec->CC[j] = 0;
	     s_vec->EE[j] = j;
	     s_vec->DD[j] = - (q);
	     s_vec->FF[j] = j;
	     if ( nseq == 2 || j > (plist->mm) )
                s_vec->RR[j] = s_vec->SS[j] = (plist->mm) + 1;
	     else
                s_vec->RR[j] = s_vec->SS[j] = j;
	  }

        for ( i = (plist->mm); i >= (plist->m1); i-- )
	  {  c = p = 0;
	     f = - (q);
	     ci = fi = i;
	     pi = i + 1;
	     cj = fj = pj = (plist->nn) + 1;
	     va = v[A[i]];
	     if ( nseq == 2 || (plist->n1) > i )
		limit = (plist->n1);
	     else
		limit = i + 1;
	     for ( j = (plist->nn); j >= limit ; j-- )  
	       {  f = f - r;
		  c = c - qr;
		  ORDER(f, fi, fj, c, ci, cj)
		  c = s_vec->CC[j] - qr; 
		  ci = s_vec->RR[j];
		  cj = s_vec->EE[j];
		  d = s_vec->DD[j] - r;
		  di = s_vec->SS[j];
		  dj = s_vec->FF[j];
		  ORDER(d, di, dj, c, ci, cj)
		  c = 0;
		  DIAG(i, j, c, p+va[B[j]])		/* diagonal */
		  if ( c <= 0 )
		    { c = 0; ci = i; cj = j; }
		  else
		    { ci = pi; cj = pj; }
		  ORDER(c, ci, cj, d, di, dj)
		  ORDER(c, ci, cj, f, fi, fj)
		  p = s_vec->CC[j];
		  s_vec->CC[j] = c;
		  pi = s_vec->RR[j];
		  pj = s_vec->EE[j];
		  s_vec->RR[j] = ci;
		  s_vec->EE[j] = cj;
		  s_vec->DD[j] = d;
		  s_vec->SS[j] = di;
		  s_vec->FF[j] = dj;
		  if ( c > (plist->min) )
		    *flag = TRUE;
	        }
	     if ( nseq == 2 || i < (plist->n1) )
	       { s_vec->HH[i] = s_vec->CC[(plist->n1)];
	         s_vec->II[i] = s_vec->RR[(plist->n1)];
	         s_vec->JJ[i] = s_vec->EE[(plist->n1)];
	         s_vec->WW[i] = f;
	         s_vec->XX[i] = fi;
	         s_vec->YY[i] = fj;
	       }
	  }
      
  for ( rl = (plist->m1), cl = (plist->n1); ; )
    { for ( rflag = cflag = 1; ( rflag && (plist->m1) > 1 ) || ( cflag && (plist->n1) > 1 ) ;  )
        { if ( rflag && (plist->m1) > 1 )	/* Compute one row */
            { rflag = 0;
	      (plist->m1)--;
      	      c = p = 0;
	      f = - (q);
	      ci = fi = (plist->m1);
	      pi = (plist->m1) + 1;
	      cj = fj = pj = (plist->nn) + 1;
	      va = v[A[(plist->m1)]];
	      for ( j = (plist->nn); j >= (plist->n1) ; j-- )  
	        { f = f - r;
		  c = c - qr;
		  ORDER(f, fi, fj, c, ci, cj)
		  c = s_vec->CC[j] - qr; 
		  ci = s_vec->RR[j];
		  cj = s_vec->EE[j];
		  d = s_vec->DD[j] - r;
		  di = s_vec->SS[j];
		  dj = s_vec->FF[j];
		  ORDER(d, di, dj, c, ci, cj)
		  c = 0;
		  DIAG((plist->m1), j, c, p+va[B[j]])		/* diagonal */
		  if ( c <= 0 )
		    { c = 0; ci = (plist->m1); cj = j; }
		  else
		    { ci = pi; cj = pj; }
		  ORDER(c, ci, cj, d, di, dj)
		  ORDER(c, ci, cj, f, fi, fj)
		  p = s_vec->CC[j];
		  s_vec->CC[j] = c;
		  pi = s_vec->RR[j];
		  pj = s_vec->EE[j];
		  s_vec->RR[j] = ci;
		  s_vec->EE[j] = cj;
		  s_vec->DD[j] = d;
		  s_vec->SS[j] = di;
		  s_vec->FF[j] = dj;
		  if ( c > (plist->min) )
		     *flag = TRUE;
		  if ( ! rflag && ( ci > rl && cj > cl || di > rl && dj > cl
	 		                            || fi > rl && fj > cl ) )
		      rflag = 1;
	        }
	      s_vec->HH[(plist->m1)] = s_vec->CC[(plist->n1)];
	      s_vec->II[(plist->m1)] = s_vec->RR[(plist->n1)];
	      s_vec->JJ[(plist->m1)] = s_vec->EE[(plist->n1)];
	      s_vec->WW[(plist->m1)] = f;
	      s_vec->XX[(plist->m1)] = fi;
	      s_vec->YY[(plist->m1)] = fj;
	      if ( ! cflag && ( ci > rl && cj > cl || di > rl && dj > cl
			     || fi > rl && fj > cl ) )
	         cflag = 1;
	    }

	  if ( nseq == 1 && (plist->n1) == ((plist->m1) + 1) && ! rflag )
	     cflag = 0;
	  if ( cflag && (plist->n1) > 1 )	/* Compute one column */
	    { cflag = 0;
	      (plist->n1)--;
	      c = 0;
	      f = - (q);
	      cj = fj = (plist->n1);
	      va = v[B[(plist->n1)]];
	      if ( nseq == 2 || (plist->mm) < (plist->n1) )
		{ p = 0;
	          ci = fi = pi = (plist->mm) + 1;
	          pj = (plist->n1) + 1;
		  limit = (plist->mm);
		}
	      else
		{ p = s_vec->HH[(plist->n1)];
		  pi = s_vec->II[(plist->n1)];
		  pj = s_vec->JJ[(plist->n1)];
	          ci = fi = (plist->n1);
		  limit = (plist->n1) - 1;
		}
	      for ( i = limit; i >= (plist->m1) ; i-- )  
	        { f = f - r;
		  c = c - qr;
		  ORDER(f, fi, fj, c, ci, cj)
		  c = s_vec->HH[i] - qr; 
		  ci = s_vec->II[i];
		  cj = s_vec->JJ[i];
		  d = s_vec->WW[i] - r;
		  di = s_vec->XX[i];
		  dj = s_vec->YY[i];
		  ORDER(d, di, dj, c, ci, cj)
		  c = 0;
	          DIAG(i, (plist->n1), c, p+va[A[i]])
		  if ( c <= 0 )
		    { c = 0; ci = i; cj = (plist->n1); }
		  else
		    { ci = pi; cj = pj; }
		  ORDER(c, ci, cj, d, di, dj)
		  ORDER(c, ci, cj, f, fi, fj)
		  p = s_vec->HH[i];
		  s_vec->HH[i] = c;
		  pi = s_vec->II[i];
		  pj = s_vec->JJ[i];
		  s_vec->II[i] = ci;
		  s_vec->JJ[i] = cj;
		  s_vec->WW[i] = d;
		  s_vec->XX[i] = di;
		  s_vec->YY[i] = dj;
		  if ( c > (plist->min) )
		     *flag = TRUE;
	          if ( ! cflag && ( ci > rl && cj > cl || di > rl && dj > cl
		               || fi > rl && fj > cl ) )
		     cflag = 1;
	        }
	      s_vec->CC[(plist->n1)] = s_vec->HH[(plist->m1)];
	      s_vec->RR[(plist->n1)] = s_vec->II[(plist->m1)];
	      s_vec->EE[(plist->n1)] = s_vec->JJ[(plist->m1)];
	      s_vec->DD[(plist->n1)] = f;
	      s_vec->SS[(plist->n1)] = fi;
	      s_vec->FF[(plist->n1)] = fj;
	      if ( ! rflag && ( ci > rl && cj > cl || di > rl && dj > cl
		                                 || fi > rl && fj > cl ) )
	         rflag = 1;
	    }
	}

      if ( (plist->m1) == 1 && (plist->n1) == 1 || no_cross(flag, plist, &rl, &cl) )
	 break;
   }
  (plist->m1)--;
  (plist->n1)--;
}

/* recompute the area on forward pass */
static void small_pass(CharPtr A, CharPtr B, Int4 count, Int4 nseq, matrix value, vecptr s_vec, listptr plist, pairptr PNTR row) 
{ 
  register  Int4  i, j;			/* row and column indices */
  register  Int4  c;			/* best score at current point */
  register  Int4  f;			/* best score ending with insertion */
  register  Int4  d;			/* best score ending with deletion */
  register  Int4  p;			/* best score at (i-1, j-1) */
  register  Int4  ci, cj;		/* end-point associated with c */ 
  register  Int4  di, dj;		/* end-point associated with d */
  register  Int4  fi, fj;		/* end-point associated with f */
  register  Int4  pi, pj;		/* end-point associated with p */
  Int2Ptr  va;				/* pointer to v(A[i], B[j]) */
  Int4  limit;				/* lower bound on j */
  Int4 q, r, qr;			/* gap penalties*/
  Int2Ptr PNTR v;			/* matrix*/
  Boolean tt;				/*flag for DIAG() */
  /*pairptr z;*/				/*for process DIAG()*/

	q=value.q; 
	r=value.r;
	qr=value.qr;
	v=value.v;
	for ( j = (plist->n1) + 1; j <= (plist->nn) ; j++ )
	  {  s_vec->CC[j] = 0;
	     s_vec->RR[j] = (plist->m1);
	     s_vec->EE[j] = j;
	     s_vec->DD[j] = - (q);
	     s_vec->SS[j] = (plist->m1);
	     s_vec->FF[j] = j;
	  }
	for ( i = (plist->m1) + 1; i <= (plist->mm); i++) 
	  {  c = 0;				/* Initialize column 0 */
	     f = - (q);
	     ci = fi = i;
	     va = v[A[i]];
	     if ( nseq == 2 || i <= (plist->n1) )
	       { p = 0;
	         pi = i - 1;
	         cj = fj = pj = (plist->n1);
		 limit = (plist->n1) + 1;
	       }
	     else
	       { p = s_vec->CC[i];
		 pi = s_vec->RR[i];
		 pj = s_vec->EE[i];
	         cj = fj = i;
		 limit = i + 1;
	       }
	     for ( j = limit ; j <= (plist->nn) ; j++ )  
	       {  f = f - r;
		  c = c - qr;
		  ORDER(f, fi, fj, c, ci, cj)
		  c = s_vec->CC[j] - qr; 
		  ci = s_vec->RR[j];
		  cj = s_vec->EE[j];
		  d = s_vec->DD[j] - r;
		  di = s_vec->SS[j];
		  dj = s_vec->FF[j];
		  ORDER(d, di, dj, c, ci, cj)
		  c = 0;
		  DIAG(i, j, c, p+va[B[j]])		/* diagonal */
		  if ( c <= 0 )
		    { c = 0; ci = i; cj = j; }
		  else
		    { ci = pi; cj = pj; }
		  ORDER(c, ci, cj, d, di, dj)
		  ORDER(c, ci, cj, f, fi, fj)
		  p = s_vec->CC[j];
		  s_vec->CC[j] = c;
		  pi = s_vec->RR[j];
		  pj = s_vec->EE[j];
		  s_vec->RR[j] = ci;
		  s_vec->EE[j] = cj;
		  s_vec->DD[j] = d;
		  s_vec->SS[j] = di;
		  s_vec->FF[j] = dj;
		  if ( c > (plist->min) )	/* add the score into list */
		    (plist->min) = addnode(c, ci, cj, i, j, count, plist);
	        }
	  }
}

/* Add a new node into list.  */

static Int4 addnode(Int4 c, Int4 ci, Int4 cj, Int4 i, Int4 j, Int4 K, listptr plist)  
{ 
  Boolean found;				/* 1 if the node is in (plist->LIST) */
  register Int4 d;
  Int4 cost;

  found = FALSE;
  cost=plist->min;

  if ( (plist->most) != 0 && (plist->most)->STARI == ci && (plist->most)->STARJ == cj )
    found = TRUE;
  else
     for ( d = 0; d < (plist->numnode) ; d++ )
	{ (plist->most) = (plist->LIST)[d];
	  if ( (plist->most)->STARI == ci && (plist->most)->STARJ == cj )
	    { found = TRUE;
	      break;
	    }
        }
  if ( found )
    { if ( (plist->most)->SCORE < c )
        { (plist->most)->SCORE = c;
          (plist->most)->ENDI = i;
          (plist->most)->ENDJ = j;
        }
      if ( (plist->most)->TOP > i ) (plist->most)->TOP = i;
      if ( (plist->most)->BOT < i ) (plist->most)->BOT = i;
      if ( (plist->most)->LEFT > j ) (plist->most)->LEFT = j;
      if ( (plist->most)->RIGHT < j ) (plist->most)->RIGHT = j;
    }
  else
    { if ( (plist->numnode) == K )	/* list full */
	 (plist->most) = (plist->low);
      else
         (plist->most) = (plist->LIST)[(plist->numnode)++];
      (plist->most)->SCORE = c;
      (plist->most)->STARI = ci;
      (plist->most)->STARJ = cj;
      (plist->most)->ENDI = i;
      (plist->most)->ENDJ = j;
      (plist->most)->TOP = (plist->most)->BOT = i;
      (plist->most)->LEFT = (plist->most)->RIGHT = j;
    }
  if ( (plist->numnode) == K )
    { if ( (plist->low) == (plist->most) || ! (plist->low) ) 
        { for ( (plist->low) = (plist->LIST)[0], d = 1; d < (plist->numnode) ; d++ )
            if ( (plist->LIST)[d]->SCORE < (plist->low)->SCORE )
              (plist->low) = (plist->LIST)[d];
	}
      return ( (plist->low)->SCORE ) ;
    }
  else
    return cost;
}

/* Find and remove the largest score in list */

static vertexptr findmax(listptr plist)
{ 
  vertexptr  cur;
  register Int4 i, j;

  for ( j = 0, i = 1; i < (plist->numnode) ; i++ )
    if ( (plist->LIST)[i]->SCORE > (plist->LIST)[j]->SCORE )
       j = i;
  cur = (plist->LIST)[j];
  if ( j != --(plist->numnode) )
    { (plist->LIST)[j] = (plist->LIST)[(plist->numnode)];
      (plist->LIST)[(plist->numnode)] =  cur;
    }
  (plist->most) = (plist->LIST)[0];
  if ( (plist->low) == cur ) (plist->low) = (plist->LIST)[0];
  return ( cur );
}

/* return 1 if no node in (plist->LIST) share vertices with the area */

static Boolean no_cross(BoolPtr flag, listptr plist, Int4Ptr rl, Int4Ptr cl)
{ 
  vertexptr  cur;
  register Int4 i;

      for ( i = 0; i < (plist->numnode); i++ )
	{ cur = (plist->LIST)[i];
	  if ( cur->STARI <= (plist->mm) && cur->STARJ <= (plist->nn) && cur->BOT >= (plist->m1)-1 && 
	       cur->RIGHT >= (plist->n1)-1 && ( cur->STARI < *rl || cur->STARJ < *cl ))
	     { if ( cur->STARI < *rl ) *rl = cur->STARI;
	       if ( cur->STARJ < *cl ) *cl = cur->STARJ;
	       *flag = TRUE;
	       break;
	     }
	}
      if ( i == (plist->numnode) )
	return 1;
      else
	return 0;
}

/* Alignment display routine */

static void display(CharPtr A, CharPtr B, Int4 M, Int4 N, Int4Ptr S, Int4 AP, Int4 BP, Int4Ptr rec_a1, Int4Ptr rec_a2, Int2Ptr POS) 
{
  Int4   i, j, op, start_i, start_j, match, mis_match;

  for (i = j = 0; i < M || j < N; ) {
	start_i = i;
	start_j = j;
	match = mis_match = 0;
	while (i < M && j < N && *S == 0) {
		++i;
		++j;
		if (A[i] == B[j])
			++match;
		else
			++mis_match;
		S++;
	}
	rec_a1[*POS] = AP+start_i;
	rec_a2[*POS] = BP+start_j;
	 ++*POS;
	rec_a1[*POS] = AP+i-1;
	rec_a2[*POS] = BP+j-1;
	 ++*POS;

	if (i < M || j < N)
		if ((op = *S++) > 0)
			j += op;
		else
			i -= op;
    }
}


