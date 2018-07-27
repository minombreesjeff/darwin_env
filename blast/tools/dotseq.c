/* dotSeq.c */
/* ===========================================================================
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
*  any work or product based on this material.
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
* File Name:  dotseq.c
*
* Author:  Fasika Aklilu
*
* Version Creation Date:   8/9/01
*
* $Revision: 6.8 $
*
* File Description: computes local alignments for dot matrix
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------

$Revision: 6.8 $
$Log: dotseq.c,v $
Revision 6.8  2001/08/09 17:21:08  kans
include alignmgr.h to avoid Mac compile error

Revision 6.7  2001/08/09 16:31:49  aklilu
added revision

Revision 6.6  2001/01/19 20:11:31  kans
minor changes to work with MacOS X compiler (contributed by William Van Etten)

Revision 6.5  2000/09/19 21:36:09  kans
commented out printf

Revision 6.4  2000/08/07 13:29:23  sicotte
Fix printf (long) casts

Revision 6.3  2000/07/26 18:23:09  aklilu
added DOT_SPI_FindBestAlnByDotPlotEx, to return best alignments


*/

#include <dotseq.h>
#include <alignmgr.h>

/****************************************************************************

      GLOBAL VARIABLES                                                               
 ***************************************************************************/

 
  
static Int4  DOTblosum62[24][24] = {
  4, -1, -2, -2,  0, -1, -1,  0, -2, -1, -1, -1, -1, -2, -1,  1,  0, -3, -2,  0, -2, -1,  0, -4,
 -1,  5,  0, -2, -3,  1,  0, -2,  0, -3, -2,  2, -1, -3, -2, -1, -1, -3, -2, -3, -1,  0, -1, -4,
 -2,  0,  6,  1, -3,  0,  0,  0,  1, -3, -3,  0, -2, -3, -2,  1,  0, -4, -2, -3,  3,  0, -1, -4,
 -2, -2,  1,  6, -3,  0,  2, -1, -1, -3, -4, -1, -3, -3, -1,  0, -1, -4, -3, -3,  4,  1, -1, -4,
  0, -3, -3, -3,  9, -3, -4, -3, -3, -1, -1, -3, -1, -2, -3, -1, -1, -2, -2, -1, -3, -3, -2, -4,
 -1,  1,  0,  0, -3,  5,  2, -2,  0, -3, -2,  1,  0, -3, -1,  0, -1, -2, -1, -2,  0,  3, -1, -4,
 -1,  0,  0,  2, -4,  2,  5, -2,  0, -3, -3,  1, -2, -3, -1,  0, -1, -3, -2, -2,  1,  4, -1, -4,
  0, -2,  0, -1, -3, -2, -2,  6, -2, -4, -4, -2, -3, -3, -2,  0, -2, -2, -3, -3, -1, -2, -1, -4,
 -2,  0,  1, -1, -3,  0,  0, -2,  8, -3, -3, -1, -2, -1, -2, -1, -2, -2,  2, -3,  0,  0, -1, -4,
 -1, -3, -3, -3, -1, -3, -3, -4, -3,  4,  2, -3,  1,  0, -3, -2, -1, -3, -1,  3, -3, -3, -1, -4,
 -1, -2, -3, -4, -1, -2, -3, -4, -3,  2,  4, -2,  2,  0, -3, -2, -1, -2, -1,  1, -4, -3, -1, -4,
 -1,  2,  0, -1, -3,  1,  1, -2, -1, -3, -2,  5, -1, -3, -1,  0, -1, -3, -2, -2,  0,  1, -1, -4,
 -1, -1, -2, -3, -1,  0, -2, -3, -2,  1,  2, -1,  5,  0, -2, -1, -1, -1, -1,  1, -3, -1, -1, -4,
 -2, -3, -3, -3, -2, -3, -3, -3, -1,  0,  0, -3,  0,  6, -4, -2, -2,  1,  3, -1, -3, -3, -1, -4,
 -1, -2, -2, -1, -3, -1, -1, -2, -2, -3, -3, -1, -2, -4,  7, -1, -1, -4, -3, -2, -2, -1, -2, -4,
  1, -1,  1,  0, -1,  0,  0,  0, -1, -2, -2,  0, -1, -2, -1,  4,  1, -3, -2, -2,  0,  0,  0, -4,
  0, -1,  0, -1, -1, -1, -1, -2, -2, -1, -1, -1, -1, -2, -1,  1,  5, -2, -2,  0, -1, -1,  0, -4,
 -3, -3, -4, -4, -2, -2, -3, -2, -2, -3, -2, -3, -1,  1, -4, -3, -2, 11,  2, -3, -4, -3, -2, -4,
 -2, -2, -2, -3, -2, -1, -2, -3,  2, -1, -1, -2, -1,  3, -3, -2, -2,  2,  7, -1, -3, -2, -1, -4,
  0, -3, -3, -3, -1, -2, -2, -3, -3,  3,  1, -2,  1, -1, -2, -2,  0, -3, -1,  4, -3, -2, -1, -4,
 -2, -1,  3,  4, -3,  0,  1, -1,  0, -3, -4,  0, -3, -3, -2,  0, -1, -4, -3, -3,  4,  1, -1, -4,
 -1,  0,  0,  1, -3,  3,  4, -2,  0, -3, -3,  1, -1, -3, -1,  0, -1, -3, -2, -2,  1,  4, -1, -4,
  0, -1, -1, -1, -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2,  0,  0, -2, -1, -1, -1, -1, -1, -4,
 -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4,  1
 };
 
static Int4  DOTNCBIstdaaToBLOSUM [] =  {23, 0, 20, 4, 3, 6, 13, 7, 8, 9, 11, 10, 12, 2, 14, 5, 1, 15, 16, 19, 17, 22, 19, 21, 22 , 23};

static Int4        trim_size=0;
static Boolean     is_na;

/****************************************************************************

      STATIC FUNCTIONS                                                               
 ***************************************************************************/


static Boolean DOT_GetSeqsbyLoc(DOTMainDataPtr mip);
static Boolean DOT_ComputeDotPlot (DOTMainDataPtr mip);




/*_____________________________________(DOT_DNAScoringMatrix)_______


  Purpose : Create DNA scoring matrix.

____________________________________________________________________*/

extern Int4Ptr PNTR   DOT_DNAScoringMatrix(Int4 mismatch, Int4 reward,Int4 alsize){
  Int4Ptr PNTR alignMatrix;
  Int4 i,j,k;
  Int4 nbase1,nbase2;
  Int4 base1[4],base2[4];
  Int4 nmatches;
  /* alsize = alphabet size.. should be a 4 or 16. */
  /* If "ncbi'ized" the codes.. could make matrix this way,
     then use the ncbi translation tools 
     */
  if(alsize !=4 && alsize!=16) {
    ErrPostEx(SEV_WARNING,0,0,"DNAScoringMatrix: No ambiguity codes for alphabet size=%d\n",alsize);
  }
  alignMatrix= (Int4Ptr PNTR) Calloc(alsize*(alsize+1),
                                     MAX(sizeof(Int4),sizeof(Int4Ptr)));
  for(i=0;i<alsize;i++) {
    *(alignMatrix+i)=(Int4Ptr) (alignMatrix+(i+1)*alsize);
  }

  for(i=0;i<alsize;i++) {
    for(j=0;j<alsize;j++) {
      alignMatrix[i][j]=mismatch;
    }
  }
  for(i=0;i<alsize;i++)
      alignMatrix[i][i]=reward;
  if(alsize!=16) 
      return alignMatrix;

  /* For ncbi4na. "presence: of bases is Bit-encoded A=1,C=2,G=4,T=8"*/
  /* Score gaps as mismatches... should not be there */
  /*   for(i=0;i<alsize;i++) alignMatrix[0][i]=alignMatrix[i][0]=mismatch; */

  for(i=0;i<alsize;i++) {
    base1[0] = i&1;
    base1[1] = (i>>1)&1;
    base1[2] = (i>>2)&1;
    base1[3] = (i>>3)&1;
    nbase1 = base1[0]+base1[1]+base1[2]+base1[3];
    for(j=0;j<alsize;j++) {
      base2[0] = j&1;
      base2[1] = (j>>1)&1;
      base2[2] = (j>>2)&1;
      base2[3] = (j>>3)&1;
      nbase2 = base2[0]+base2[1]+base2[2]+base2[3];
      nmatches=0;
      for(k=0;k<4;k++) nmatches += (base1[k]&base2[k]);  /* matches */
      if(nbase1==0 || nbase2==0) {
        alignMatrix[i][j]=mismatch; /*gaps */
      } else {
        alignMatrix[i][j] =(reward*nmatches+(nbase1*nbase2-nmatches)*mismatch)/(nbase1*nbase2);
      }
    }
  }
  return alignMatrix;
}


/*____________________________________________(DOT_CalculateScoreMeans)_____

  Purpose : Calculate max and mean values for matrix vectors.

____________________________________________________________________*/

static void DOT_CalculateMatrix_MinMax (DOTMainDataPtr mip)
{
  Int4             temp, sum, q, s, max;
  Int2             maxscore, minscore;
  Int4Ptr PNTR     matrix; 


  matrix = mip->matrix;
  max=(is_na)?4:24;
      
      for (maxscore = minscore = sum = s = 0; s < max; s++)
          for (q=0; q < max; q++)
            {
              if (is_na)
              temp = matrix[q][s];
              else
                temp = DOTblosum62[DOTNCBIstdaaToBLOSUM[q]][DOTNCBIstdaaToBLOSUM[s]];
              
              if (temp < minscore)
                minscore = temp;
            if (temp > maxscore) 
              maxscore = temp;
            
            sum += temp;
            }

      mip->maxscore=maxscore;
      mip->minscore=minscore;

  return;
}

/*_______________________________________________________(CompareFunc)_____


  Purpose : Compare function for hits binary tree.

____________________________________________________________________*/

static Int4 CompareFunc(Pointer n1, Pointer n2)
{
  DOTDiagPtr   nd1, nd2;

  nd1 = (DOTDiagPtr)n1;
  nd2 = (DOTDiagPtr)n2;

  if (nd1->score > nd2->score) 
    return 1;
  else if (nd1->score < nd2->score)
    return -1;
 /*  else if (nd1->length > nd2->length) */
/*     return 1; */
/*   else if (nd1->length < nd2->length) */
/*     return -1; */
  else if (nd1->rdmKey > nd2->rdmKey)
    return 1;
  else if (nd1->rdmKey < nd2->rdmKey)
    return -1;
  else
    return 0;
}

Pointer   avl_List[MAX_TRIM]={""};
Int4      avl_counter1=0;

/*___________________________________________________________(DOT_AddToList)

  Purpose : Add to a temporary array for deletion from binary tree.

____________________________________________________________________*/
static Boolean DOT_AddToList (Pointer item)
{

  avl_List[avl_counter1]=item;

  avl_counter1++;

  if (avl_counter1>=trim_size)
    {
      avl_counter1=0;
      return FALSE; 
    }
  else 
    return TRUE; 
}
/*___________________________________________________________(DOT_TrimBinTree)
  Purpose : Trim hits binary tree if grows > mip->tree_limit.

____________________________________________________________________*/
static void DOT_TrimBinTree (Avl_TreePtr tree, Int4Ptr cutoff)
{
  Int2  i;
  DOTDiagPtr node;
  
  Avl_Traverse(tree, DOT_AddToList);

  for (i=0; i<trim_size; i++)
    {
      node=(DOTDiagPtr)avl_List[i];
      *cutoff=MAX(*cutoff, node->score);
      node=(DOTDiagPtr)Avl_Delete(tree, (Pointer)node); 
      if (node) MemFree(node);
    }
}
/*_________________________________________(DOT_NewHitNode)_____


  Purpose : Create new hit node.

____________________________________________________________________*/
static DOTDiagPtr DOT_NewHitNode(DOTMainDataPtr mip, Int4 q_left, Int4 s_left, Int4 length, Int4 score)
{
  DOTDiagPtr          node;

  if (!(node =(DOTDiagPtr)MemNew (sizeof(DOTDiag)))) 
    {
      ErrPostEx (SEV_ERROR, 0, 0, "%s", "HIT node allocation failed");
      return NULL;
    }
  node->score=score;
  node->q_start = mip->q_start + q_left;  
  node->s_start = mip->s_start + s_left;

  node->length = length;
  node->rdmKey=RandomNum();
  
  return node;
}

/*___________________________________________________________(DOT_SaveHit)_____


  Purpose : Save extended hit.

____________________________________________________________________*/
static void DOT_SaveHit (DOTMainDataPtr mip, Int4 score, Int4 q_offset, Int4 s_offset, Int4 length)
{
  DOTDiagPtr          Node;
  Int4                unique=0;
  


  Node=DOT_NewHitNode(mip, q_offset, s_offset, length, score);

  if (Node == NULL) return;

  if (mip->first_pass == FALSE) 
    {
      if ((int)mip->tree->totalNodes>=mip->tree_limit)
        DOT_TrimBinTree(mip->tree, &(mip->cutoff_score));
  
     if(!Avl_Insert(mip->tree, (Pointer)Node, &unique))
        if (Node) MemFree(Node);
    }
  else /* first pass */
    {
      
      mip->tree =  (Avl_TreePtr)MemNew(sizeof(Avl_Tree));
      Avl_Initialize(mip->tree, CompareFunc, NULL);
      mip->tree->root=(Avl_NodePtr)MemNew(sizeof(Avl_Node));
      mip->tree->root->avl_p=(Pointer)Node;
      mip->tree->totalNodes++;
      mip->first_pass=FALSE;
      if (mip->tree_limit>1000)
        trim_size=200;
      else
        trim_size=(mip->tree_limit*20)/100;
    }
  
}

/*_______________________________________________________(CompareDiags)_____

  Purpose : Compare function for history binary tree.

____________________________________________________________________*/

static Int4 CompareDiags(Pointer n1, Pointer n2)
{
  Int4 diag2, diag1;
  DOTHistPtr node1, node2;

  node1 = (DOTHistPtr)n1;
  node2 = (DOTHistPtr)n2;

  diag1 = node1->diag_constant;
  diag2 = node2->diag_constant;

  if (diag1 == diag2) /* found diag match */
    return 0;/* found match */
  else if (diag1 > diag2) 
    return 1; /* greater */
  else
    return -1; /* smaller */  
}

/*_________________________________________(DOT_NewHistNode)_____


  Purpose : Create new history node.

____________________________________________________________________*/
DOTHistPtr DOT_NewHistNode(Int4 diag, Int4 q_pos)
{
  DOTHistPtr node;

  if (!(node =(DOTHistPtr)MemNew (sizeof(DOTHist)))) 
    {
      ErrPostEx (SEV_WARNING, 0, 0, "%s", "HIST node allocation failed");
      return NULL;
    }
  node->diag_constant = diag;
  node->q_stop = q_pos;
  
  return node;  
}


/*____________________________________________________________(DOT_ExtendProt)_____


  Purpose : Protein extend hit function.

____________________________________________________________________*/
static Int2 DOT_ExtendProt (DOTMainDataPtr mip, Uint1Ptr queryseq, Uint1Ptr subjectseq, Int4 q_off, Int4 s_off, Int4 diag, Int4 wordsize, DOTHistPtr node)
{
  Uint1Ptr         q_end,s, q, end, start, q_beg;
  Int4             score, sum, length;
  Int4             s_left, q_left, s_right, q_right, s_diff;
  Int4             q_start, s_start;
  Int4             threshold = 0, x, X; /* test default */
  Int4             report_len;
  

  q_left=q_off;
  s_left=s_off;
  x=X=threshold;
  report_len=1; /*  -arbitrary*/

  /* initialize positions for left extension */
  q =queryseq+(q_left-1); /* -1 b/c queryseq & subjectseq are =1 */
  s = subjectseq+(s_left-1);

  if (q_off<s_off)
    start=subjectseq+(s_off-q_off);
  else
    start=subjectseq;
  
  
  score = 0;
  sum = 0;

  /* extend to the left*/
  
  while (s >start) 
    {
      if (*q >24 || *s >24 || *q<1 || *s<1) 
        {
          q++;
          s++;
          break;/* ambiguity found */
        }
           
      if ((sum = DOTblosum62[DOTNCBIstdaaToBLOSUM[*s]][DOTNCBIstdaaToBLOSUM[*q]])>= threshold) 
        {
          score += sum;
          sum=0;
          if ((x=-score)<X)
            x=X;
        }
      else 
        if (sum<x)
          {
            q++;
            s++;
            break;
          }
      q--;
      s--;
    }
  
  q_beg=q;
  score-=mip->maxscore*wordsize;
  
  /* reinitialize positions for right extention */
  q=queryseq+(q_off-wordsize);
  s=subjectseq+(s_off-wordsize);
  q_right = mip->qlen- (q_off-wordsize)-1;
  s_right = mip->slen- (s_off-wordsize)-1;

  /* assign ends */

  if (q_right<s_right)
    end = s + q_right;
  else
    end = s + s_right;
 
  /* extend to the right only */
  
  while (s < end) 
    {
      q++;
      s++;
      if (*q > 24 || *s > 24 || *s<1 || *q<1) 
        {
          break;/* ambiguity found */
        }
      
      if ((sum = DOTblosum62[DOTNCBIstdaaToBLOSUM[*s]][DOTNCBIstdaaToBLOSUM[*q]])>= threshold) 
        {
          score += sum;
          sum=0;
          if ((x=-score)<X)
            x=X;
        }
      else 
        if (sum<x)
          break;
    }
  
  q_end = q;    

  length = q_end - q_beg;
  if (length>=report_len)
    { 
      /* check against the lowest score in tree */
      if (score<mip->cutoff_score)
        return -1; 
      
      s_diff = s_off-q_off;
      q_start = q_beg- queryseq;
      length = q_end - q_beg;
      s_start = q_start+s_diff;
      
      /* update node */
     if (node!=NULL)
       {
         node->diag_constant = diag;
         node->q_stop= q_start+length; 
       }
     
     DOT_SaveHit (mip, score, q_start, s_start, length);
     return 0;
    }
  else 
    return -1;  /* hit not stored */
}

/*____________________________________________________________(DOT_ExtendNuc)_____


  Purpose : Nucleotide extend hit function.

____________________________________________________________________*/
static Int2 DOT_ExtendNuc (DOTMainDataPtr mip, Uint1Ptr queryseq, Uint1Ptr subjectseq, Int4 q_off, Int4 s_off, Int4 diag, Int4 wordsize, DOTHistPtr node)
{
  Uint1Ptr         q_end, s_end, s, q, end, start, q_beg;
  Int4             score, sum, length;
  Int4             s_left, q_left, s_right, s_diff, q_right;
  Int4Ptr PNTR     matrix;
  Int4             ex_threshold=0,x,X;
 
  matrix = mip->matrix;
  
  s_left = s_off/* -wordsize */;
  q_left=q_off/* -wordsize */;
  x=X=ex_threshold;
  /* reportable=4*maxscore;  *//* below lowest possible -store everything*/

  /* initialize positions for left extension */
  q=queryseq+ (q_left-1);
  s=subjectseq+ (s_left-1);

  /* find the beginning */
  if (q_off < s_off)
    start = subjectseq+(s_off-q_off);
  else
    start = subjectseq;
    
  score=0;
  sum = 0;

  /* extend to the left*/
  
  while (s >start)   
    {

      if (*q > 3 || *s > 3) /* ambiguity found */
        {
          q++;
          s++;
          break;
        }
      if ((sum+= matrix[*s][*q])>0)
        {
          score+=sum;
          sum=0;
          if ((x=-score)<X)
            x=X;
        }
      else
        if (sum<x)
          {
            q++;
            s++;
            break;
          }
      q--;
      s--;
    }

  q_beg=q;
  score-=mip->maxscore*(wordsize-2);

/* initialize positions for right extention */

  q=queryseq+(q_off-wordsize);
  s=subjectseq+(s_off-wordsize);

  q_right = mip->qlen- (q_off-wordsize)-1;
  s_right = mip->slen- (s_off-wordsize)-1;

  /* assign ends */

  if (q_right<s_right)
    end = s + q_right;
  else
    end = s + s_right;
  
  
  /* extend to the right */
  sum = 0;
  
  while (s < end) 
    {
      q++;
      s++;
      if (*q > 3 || *s > 3) /* ambiguity found */
        {
          break;
        }
      
      if ((sum+= matrix[*s][*q])>0)
        {
          score+=sum;
          sum=0;
          if ((x=-score)<X)
            x=X;
        }
      else
        if (sum<x)
          break;
    }
  
  q_end =q; 

  length = q_end - q_beg;

/*   if (score>= reportable) */ /* storing everything -reportable is the lowest possible anyway*/
   /*  {  */
      /* check against the lowest score in tree */
      if (score<mip->cutoff_score)
        return -1; 
      
      s_diff = s_off-q_off;

      q_left = q_beg- queryseq;
      length = q_end - q_beg;
      s_left = q_left+s_diff;
      
      /* update node */
     if (node!=NULL)
       {
         node->diag_constant = diag;
         node->q_stop= q_left + length; 
       }
     
     DOT_SaveHit (mip, score, q_left, s_left, length);
     return 0;
   /*  } */
/*   else  */
/*     return -1; */  /* hit not stored */
}



/*____________________________________________________________(ExtendHit)_____


  Purpose : Calls extend function for new hit.

____________________________________________________________________*/
static Int2 ExtendHit (DOTMainDataPtr mip, Uint1Ptr queryseq, Uint1Ptr subjectseq, Int4 q_off, Int4 s_off, Int4 diag_constant, Int4 wordsize, DOTHistPtr node)
{
  if (is_na)
    {
      return DOT_ExtendNuc (mip, queryseq, subjectseq, q_off, s_off, diag_constant,  wordsize, node);
    }
  else 
    { 
      return DOT_ExtendProt (mip, queryseq, subjectseq, q_off, s_off, diag_constant,  wordsize, node);
    }  
    
}
 
/*_________________________________________(DOT_HistBTree)_____


  Purpose : Check new hit against a previous hit in history binary tree.

____________________________________________________________________*/

static void DOT_HistBTree (DOTInfoPtr info, Int4 diag, Int4 q_pos)/* rtn 1=found match, rtn 0=new hit*/
{
  Avl_TreePtr tree=NULL;
  Pointer     tnode=NULL;
  DOTHistPtr     node=NULL;

  tree = info->tree;
  node=DOT_NewHistNode(diag,q_pos);
  
  if (node == NULL) return;

  /*  check diag in tree */    
  if ((tnode=Avl_Search (tree, (Pointer)node)) != NULL) /* diag found */
    {
      if ( ((DOTHistPtr)tnode)->q_stop < node->q_stop) /*old diag, new hit */
        ExtendHit (info->mip, info->qseq, info->sseq, info->q_pos, info->s_pos, node->diag_constant, info->wordsize, (DOTHistPtr)tnode);

      if (node) MemFree(node);

    }  
  else /* no diag, new hit */
    {
      if (ExtendHit (info->mip, info->qseq, info->sseq, info->q_pos, info->s_pos, node->diag_constant, info->wordsize, node) == 0)
        {
          if (!Avl_Insert(tree, (Pointer)node, NULL))
            if (node) MemFree(node); /* insert new node returned FALSE */
        }
      else
        if (node) MemFree(node);
    }
  

}

DOTDiagPtr PNTR avl_hitlist=NULL;
DOTHistPtr PNTR    avl_hist=NULL;
Int4            avl_counter2;


/*___________________________________________________________(CopyFunc)_____*/
static Boolean DOT_CopyFunc1 (Pointer item)
{

  avl_counter2--;
  if (!(avl_counter2<0))
    {
      avl_hist[avl_counter2]=item;
    }
  return TRUE;

}

/*_________________________________________(FreeBTree)_____

  Purpose : Free binary tree structure.

____________________________________________________________________*/
static void DOT_FreeBTree(Avl_TreePtr tree)
{
  if (tree)
    {
      Avl_Clear (tree, CompareDiags, NULL);
      if (tree->root!=NULL)
        {
          ErrPostEx (SEV_WARNING, 0, 0, "%s", "HistTree->root free failed");
        }
      else if (Avl_TotalNodes(tree)!=0)
        {
          ErrPostEx (SEV_WARNING, 0, 0, "%s", "HistTree counter != 0");
        }
    }
  
}

/*_________________________________________(FreeHistBTree)_____


  Purpose : Free memory function for elements in history binary tree.

____________________________________________________________________*/
static void DOT_FreeHistBTree(Avl_TreePtr tree)
{
  Int4  index, i;
  DOTHistPtr node;

  if (tree==NULL)
    {
      return;
    }
  
  index=tree->totalNodes;
  avl_counter2=index;

  avl_hist=(DOTHistPtr PNTR)MemNew(sizeof(DOTHistPtr)*index);
  Avl_Traverse(tree, DOT_CopyFunc1);
  
   for (i=0; i<index; i++)
    {
      node= avl_hist[i];
      node=(DOTHistPtr)Avl_Delete(tree, (Pointer)node); 
      if (node) MemFree(node);
    }

  if (avl_hist) MemFree(avl_hist);
  DOT_FreeBTree(tree);

  return;
}

/*___________________________________________________________(CopyFunc2)_____*/
static Boolean CopyFunc2 (Pointer item)
{
  avl_counter2--;
  if (!(avl_counter2<0))
    {
      avl_hitlist[avl_counter2]=item;
    }

  return TRUE;
}

/*_________________________________________(CopyHits)_____


  Purpose : Copy stored hits from binary tree to array.

____________________________________________________________________*/
static void DOT_CopyHits(DOTMainDataPtr mip)
{
  Int4      index;

  if (mip->tree==NULL)
    {
/*       Message(MSG_ERROR, "No hits"); */
      return; /* no hits */
    }

  index=mip->tree->totalNodes;
  avl_counter2=index;
  mip->index=index;

  mip->hitlist=(DOTDiagPtr PNTR)MemNew(sizeof(DOTDiagPtr)*index);
  avl_hitlist=mip->hitlist;
  Avl_Traverse(mip->tree, CopyFunc2);


}


 /*_________________________________________(DOT_BuildPLookup)_____


  Purpose : Build protein lookup table.

____________________________________________________________________*/
static LookupTablePtr DOT_BuildPLookup(Uint1Ptr queryseq, Int4 qlen, Int4 word_size, Int4 alphabet_size, Int4 compression_ratio) 
{
  BlastAllWordPtr all_words;
  LookupTablePtr  lookup;
  Int4            num_cols;
  Int4            index = 0;
  Int4            stop, start, offset, index1, index2;
  Int4            threshold=13; /* one-hit heuristics T must be at least 13*/
  Uint1Ptr        str, PNTR array, word;
  Boolean         EXACTMATCH, found_ambig, saved_index = FALSE;
  Int4                    end;
  Int4                    counter;


  lookup = lookup_new(alphabet_size, (Int2) word_size, (Int2) (word_size/compression_ratio));
  all_words = BlastPopulateAllWordArrays(word_size+1, alphabet_size);
  if (!all_words)
    return NULL;
  
  str = queryseq;
  num_cols = all_words->num_of_cols;
  array = all_words->array;
  stop = qlen - (word_size-1);
  start = 0;
  counter=0;
  
  for (offset = start; offset<stop; offset++, str++)
    {
      /* for every word - this part takes long -sol'n wordsize = 2!!!*/
      for (index1 = 0; index1<num_cols; index1++)
        {
          found_ambig = FALSE;
          EXACTMATCH=TRUE;
          word = array[index1];
          for (index2=0; index2<word_size; index2++)
            {

              if (*(str+index2)>25 
                  || *(str+index2)<1 ||*(word+index2)>25 
                  || *(word+index2)<1)   
                { 
                  found_ambig = TRUE; 
                  EXACTMATCH=FALSE;
                  break;
                }

              if (*(str+index2) != *(word+index2))
                {
                  EXACTMATCH = FALSE;
                  break; /* go on to the next word*/
                }
            }
          /* if str is exact match to the word */
          if (EXACTMATCH)
            {
              counter++;
              lookup_add(lookup, (CharPtr)word, offset+word_size, 0);
              saved_index = TRUE;
            }
        }
    }
  if (!saved_index)
        return NULL;

  /* deallocate */
  for (index=0; index<num_cols; index++)
    {
      if (array[index]) MemFree(array[index]);
    }
  if (array) MemFree(array);
  if (all_words) MemFree(all_words);


  return lookup;
}

 /*_________________________________________(DOT_BuildNLookup)_____


  Purpose : Build nucleotide lookup table.

____________________________________________________________________*/
static LookupTablePtr DOT_BuildNLookup(Uint1Ptr queryseq, Int4 qlen, Int2 wordsize, Int2 alphabet_size, Int2 compression_ratio) 
{

  Boolean          found_ambig, saved_index=FALSE;
  Int4             offset, end, reduced_wordsize, stop;
  Int4             lookup_index, index, char_size, index_addition;
  Uint1Ptr         str;
  LookupTablePtr   lookup;
  
  
  reduced_wordsize = wordsize/compression_ratio;
  lookup = lookup_new(alphabet_size, (Int2) wordsize, (Int2) wordsize);

  str = queryseq;
  end = (qlen-wordsize-1); /* use the shorter sequence for hash table */
  stop = reduced_wordsize;



  /*  create hash table */
  for (offset = 0; offset<end; offset++, str++)
    {
      found_ambig = FALSE;
      lookup_index = 0;
      index_addition = 0;
      
      for (index=0; index<stop; index++)
        {
          
          if (*(str+index_addition) > 3 || *(str+index_addition+1)> 3 || *(str+index_addition+2) > 3 || *(str+index_addition+3) > 3)
            {
              found_ambig = TRUE;
              break;
            }
          
          lookup_index += (*(str+index_addition)   << 6);
          lookup_index += (*(str+1+index_addition) << 4);
          lookup_index += (*(str+2+index_addition) << 2);
          lookup_index += *(str+3+index_addition);
          
          if (index != stop-1)
            {       /* 8 bits/byte */
              lookup_index <<= 8;
              index_addition += 4;
            }
        }
      
      if (found_ambig == FALSE)
        {
          lookup_add_index(lookup, (Int4) lookup_index, offset+ (reduced_wordsize*compression_ratio), 0/* context_index */);
          saved_index = TRUE;
        }
    }
  
  if (saved_index == FALSE)
    return NULL;
  /* convert lookup table to (mod_lt) table.*/


  return lookup;
}



 /*_________________________________________(DOT_ComputeHits)_____


  Purpose : Create hash table, find hits, store hits in hitlist array.

____________________________________________________________________*/
static Int2 DOT_ComputeHits(DOTMainDataPtr mip) 
{

  BlastAllWordPtr all_words;
  LookupTablePtr  lookup;
  ModLAEntry      *mod_lt;
  Int4            wordsize, alphabet_size, num_cols;
  Int4            char_size, lookup_index, mask, stop, start, compression_ratio;
  Int4            qlen, slen, offset, index1, index2;
  Int4            threshold=13;
  Uint1Ptr        queryseq, subjectseq, str, word, PNTR array;
  Boolean         EXACTMATCH, found_ambig, saved_index = FALSE;
  Int4                    end;
  Uint1Ptr                s, subject0, s_end, q_end, end_of_str;
  ModLookupPositionPtr    lookup_pos;
  ModLookupPosition       hit_info;
  Int4                    num_hits, num_hitsH, s_off, s_pos, q_off, q_pos;
  Int4                    diag, i, code_limit;
  DOTInfoPtr                 info;
  DOTHistPtr                 node=NULL;
  
  

  queryseq=mip->qseq;
  subjectseq=mip->sseq;
  qlen=mip->qlen;
  slen=mip->slen;

  if (is_na)
    {
      alphabet_size = 4; /*ncbi2na*/
      wordsize = mip->word_size; /* test default */
      compression_ratio = 4;
      if (!(lookup=DOT_BuildNLookup(queryseq, qlen, wordsize, alphabet_size, compression_ratio)))
        {
          return -1;	
        }
    }
  else
    {
      wordsize =2; /*test default*/
      alphabet_size = 25; /* ncbi stdaa */
      compression_ratio = 1; /* ??*/
      if (!(lookup=DOT_BuildPLookup(queryseq, qlen, wordsize, alphabet_size, compression_ratio)))
        {
          return -1;	
        }
    }
  
  code_limit=alphabet_size-1;

  /* convert lookup table to (mod_lt) table.*/
  lookup_position_aux_destruct(lookup);
  mod_lt = lookup->mod_lt;
  mask = lookup->mask;  
  char_size = lookup->char_size;
  s = subjectseq;
  subject0 = subjectseq;

  end = slen-wordsize-1;
  /* The subject sequence is too short, exit this function now. */
  if (wordsize > end)
    return -1;

  s_end = subjectseq+slen-1;
  q_end = queryseq+qlen-1;

  /* history btree structure */
  info = (DOTInfoPtr) MemNew (sizeof(DOTInfo));
  info->mip = mip;
  info->wordsize = wordsize;
  info->qseq = queryseq;
  info->sseq = subjectseq;
  info->first_pass = TRUE;


 /* scan subject sequence against lookup table */
  s = lookup_find_init (lookup, &lookup_index, s);

  for (;;) /* loops until end of subject sequence */
    {
      do 
        {
          /* lookup a contiguous word. */
          s++;
          if (s > s_end)  /*  end of subject sequence */
            goto  fin;

          if (*s > code_limit) /* ambiguity found */
            {
              num_hits = 0;
              continue; /*skip loop */
            }

          lookup_index = (((lookup_index) & mask) << char_size)  + *s;

        } while ((num_hits = mod_lt[lookup_index].num_used) == 0);
      
      lookup_pos = mod_lt[lookup_index].entries;
      hit_info = *((Uint4 *) lookup_pos);
      lookup_pos++;
      
      if (num_hits>3)
        {
          lookup_pos=*((ModLookupPositionPtr PNTR) lookup_pos);
        }
      
      s_pos = s_off = s-subject0+1;
      info->s_pos = s_pos;
              

      /* extend each hit in the linked list */
      do 
        {
          q_pos = hinfo_get_pos (hit_info);
          num_hits --;
          hit_info = *((Uint4 *) lookup_pos);
          lookup_pos++;

          diag = s_pos - q_pos; /* diag */
          info->q_pos = q_pos;
         
          if (info->first_pass)
            {
              info->tree = (Avl_TreePtr)MemNew(sizeof(Avl_Tree));
              Avl_Initialize(info->tree, CompareDiags, NULL);
              info->tree->root = (Avl_NodePtr) MemNew (sizeof(Avl_Node));
              node=DOT_NewHistNode(diag, q_pos);
              if(ExtendHit (mip, queryseq, subjectseq, q_pos, s_pos, diag, wordsize, node)==0)
                {
                  info->tree->root->avl_p = (Pointer)node;
                  info->tree->totalNodes++;
                  info->first_pass = FALSE;
                }
              else
                {
                  if (node) MemFree(node);
                  if (info->tree->root) MemFree(info->tree->root);
                  if (info->tree) MemFree(info->tree);
                }
              
            }
          else
            {
              DOT_HistBTree (info, diag, q_pos);
            }
                 
        } while (num_hits>0);
      
    }
 fin:

  lookup_destruct(lookup); /* deallocate lookup table */
  DOT_FreeHistBTree(info->tree);/* Free history btree */
  if (info->tree) MemFree(info->tree);
  if (info) MemFree(info);
  
  DOT_CopyHits(mip); /* copy bintree to array */
  if (mip->tree)
    {
      DOT_FreeBTree(mip->tree); /* free hits btree */
      if (mip->tree) MemFree(mip->tree);
    }
  return 0;

}


/*_________________________________________(DOT_SortProc)_____________

  Purpose : Sort proc -by score, by q_start, by s_start.

____________________________________________________________________*/
static int LIBCALLBACK DOT_SortProc (VoidPtr vp1, VoidPtr vp2)
{
  DOTDiagPtr hit1, hit2;
  DOTDiagPtr PNTR hp1, PNTR hp2;
  
  hp1 = (DOTDiagPtr PNTR) vp1;
  hp2 = (DOTDiagPtr PNTR) vp2;
  hit1 = *hp1;
  hit2 = *hp2;

  /*by descending score */
  if (hit1->score < hit2->score) 
    return 1;
  else if (hit1->score > hit2->score)
    return -1;
  /* by ascending q_start */
  else if (hit1->q_start > hit2->q_start)
    return 1;
  else if (hit1->q_start < hit2->q_start)
    return -1;
  /* by ascending s_start */
  else if (hit1->s_start > hit2->s_start)
    return 1;
  else if (hit1->s_start < hit2->s_start)
    return -1;
  
  
  return 0;
}



/*_______________________________________________(DOT_SortHitList)________________

  Purpose :Sort hits array.

____________________________________________________________________*/

static Int2 DOT_SortHitlist (DOTDiagPtr PNTR hitlist, Int4 index)
{
  /* sort by score */
    HeapSort ((VoidPtr)hitlist, index, sizeof(DOTDiagPtr), DOT_SortProc);

  return 0;
}

/*___________________________________(DOT_ScoreCount)____________

  Purpose : Rank scores.

____________________________________________________________________*/

static void DOT_ScoreCount (DOTMainDataPtr mip, DOTDiagPtr PNTR hitlist, Int4 index)
{
  Int4    PNTR score_array, Score, score, unique, i, j;

  /* count unique scores */
  score=hitlist[0]->score;
  i=1;
  unique=1;

  while (i<index)
    {
      if (score>(Score=hitlist[i]->score))
        {
          score=Score;
          unique++;
        }
      i++;
    }

  mip->unique=unique;

  if (unique<=1)
    return;

  score_array=(Int4Ptr)MemNew(sizeof(Int4)*unique);
  
  j=1;
  i=1;
  score=hitlist[0]->score;
  score_array[0]=0;
  
  while (i<index)
    {
      if (score>(Score=hitlist[i]->score))
        {
          score=Score;
          score_array[j]=i;
          j++;
        }
      i++;
    }
  
  mip->score_array=score_array;
  
}

 /*_________________________________________(DOT_BuildHitList)_____


  Purpose : Build and sort hitlist array, count number of unique scores for threshold ramp.

____________________________________________________________________*/
Int2 DOT_BuildHitList(DOTMainDataPtr mip, Boolean sort, Boolean countUniqueScores) 
{
  DOTDiagPtr PNTR hitlist;
  Int4            index;


  DOT_ComputeHits(mip);

  if (mip->hitlist==NULL)
    return -1; /* no hits */


  /* sort results */
  if (sort)
    {
      hitlist = mip->hitlist;
      index = mip->index;
      DOT_SortHitlist (hitlist, index);
    }
  if (countUniqueScores)
    DOT_ScoreCount(mip, hitlist, index);

  return 0;
}




/*_________________________________________(Get_ProtResidues)_____

  Purpose : Fill protein sequence buffers.

____________________________________________________________________*/
static Boolean DOT_GetPResidues (DOTMainDataPtr mip, Boolean is_byLoc)
{
 
  Int4             i, j, q, s,m, n, temp, sum;
  SeqPortPtr       qspp, sspp;
  Uint1Ptr         qseq, sseq, temp_seq1, temp_seq2;
  Int4             offset,  x;
  Boolean          found_ambig;
  Int4             index_addition, index, buf_len, mod_len;
  Int4             qlen, slen;
  Int2             ctr;
  DOTDiagPtr PNTR  hitlist;
  Uint1Ptr         sbuffer, qbuffer;


 /* initialize buffers */
  qbuffer=(Uint1Ptr) MemNew (sizeof(Uint1)*101); 
  sbuffer=(Uint1Ptr) MemNew (sizeof(Uint1)*101); 

  MemSet((Pointer)qbuffer, '\0', sizeof(Char)*101);
  MemSet((Pointer)qbuffer, '\0', sizeof(Char)*101);

  qlen=mip->qlen;
  slen=mip->slen;

  temp_seq1=NULL;
  temp_seq2=NULL;

 
  if (!(qseq = (Uint1Ptr) MemNew (sizeof(Uint1)*(qlen)))) return FALSE;

  if (!(sseq = (Uint1Ptr) MemNew (sizeof(Uint1)*(slen)))) return FALSE;

 if (is_byLoc)
    {
      qspp = SeqPortNewByLoc(mip->qslp, Seq_code_ncbistdaa);
      sspp = SeqPortNewByLoc(mip->sslp, Seq_code_ncbistdaa);
    }
 else
   {
     qspp = SeqPortNew (mip->qbsp, MIN(mip->q_start, mip->q_stop), MAX(mip->q_start, mip->q_stop), 0,  Seq_code_ncbistdaa);
     sspp = SeqPortNew (mip->sbsp, mip->s_start, mip->s_stop, 0, Seq_code_ncbistdaa);
   }
 
 if (qspp == NULL || sspp == NULL) 
   {
     ErrPostEx (SEV_ERROR, 0, 0, "%s", "DOT- Failed on SeqPortNew");
     return FALSE;
   }
 
 temp_seq2 = sseq;
 temp_seq1 =  qseq;

 do 
   {
      ctr = SeqPortRead(qspp, qbuffer, 100);
      
      if (ctr > 0) 
        {
          i = 0;
          buf_len = ctr;
          while (buf_len > 0)
            {
              *temp_seq1 = qbuffer[i];

              temp_seq1++;
              i++;
              buf_len--;
            }
        }
    } while (ctr != SEQPORT_EOF * -1);


  do 
    {
      ctr = SeqPortRead(sspp, sbuffer, 100);
      
      if (ctr > 0) 
        {
          i = 0;
          buf_len = ctr;
          while (buf_len > 0)
            {
              *temp_seq2 = sbuffer[i];
              temp_seq2++;
              i++;
              buf_len--;
            }
        }
    } while (ctr != SEQPORT_EOF * -1);
  
  SeqPortFree(sspp);
  SeqPortFree(qspp);

  
  mip->sseq=sseq;
  mip->qseq=qseq;
  return TRUE;
}



/*_________________________________________(GetNResidues)_____

  Purpose : Fill nucleotide sequence buffers.

____________________________________________________________________*/

static Boolean DOT_GetNResidues (DOTMainDataPtr mip, Boolean is_byLoc)
{
  Int4             i, j, q, s,m, n, temp, sum;
  SeqPortPtr       qspp, sspp;
  Uint1Ptr         qseq, sseq, temp_seq1, temp_seq2;
  Int4             offset, x;  
  Int4             qlen, slen;
  Boolean          found_ambig;
  Int4             index_addition, index, buf_len, mod_len;
  Int2             ctr;    
  DOTDiagPtr PNTR  hitlist;
  Uint1Ptr         sbuffer, qbuffer;

 /* initialize buffers */
  qbuffer=(Uint1Ptr) MemNew (sizeof(Uint1)*101); 
  sbuffer=(Uint1Ptr) MemNew (sizeof(Uint1)*101); 
  
  MemSet((Pointer)qbuffer, '\0', sizeof(Char)*101); 
  MemSet((Pointer)sbuffer, '\0', sizeof(Char)*101);


  mip->matrix = DOT_DNAScoringMatrix(-3, 1, 4);
  qlen=mip->qlen;
  slen=mip->slen;

  temp_seq1 = NULL;
  temp_seq2 = NULL;

  if (!(qseq = (Uint1Ptr) MemNew (sizeof(Uint1)*(qlen)))) return FALSE;
  if (!(sseq = (Uint1Ptr) MemNew (sizeof(Uint1)*(slen)))) return FALSE;

  if (is_byLoc)
    {
      qspp = SeqPortNewByLoc(mip->qslp, Seq_code_ncbi2na);
      sspp = SeqPortNewByLoc(mip->sslp, Seq_code_ncbi2na);
    }
  else
    {
      qspp = SeqPortNew (mip->qbsp, MIN(mip->q_start, mip->q_stop), MAX(mip->q_start, mip->q_stop), 0, Seq_code_ncbi2na); 
      sspp = SeqPortNew (mip->sbsp, mip->s_start, mip->s_stop, 0, Seq_code_ncbi2na);
    }

  if (qspp == NULL || sspp == NULL)
    {
      ErrPostEx (SEV_ERROR, 0, 0, "%s", "DOT- Failed on SeqPortNew");
      return FALSE;
    }
  temp_seq1 =  qseq;
  do 
    {
      ctr = SeqPortRead(qspp, qbuffer, 100);
      
      if (ctr > 0) 
        {  
          i = 0;
          buf_len = ctr;
          while (buf_len > 0)
            {
              *temp_seq1 = (Uint1)qbuffer[i];
              temp_seq1++;
              i++;  
              buf_len--;
            }
        }
    } while (ctr != SEQPORT_EOF * -1);
 
  temp_seq2 = sseq;
  do 
    {
      ctr = SeqPortRead(sspp, sbuffer, 100);
      
      if (ctr > 0) 
        {
          i = 0;
          buf_len = ctr;
          while (buf_len > 0)
            {
              *temp_seq2 = (Uint1)sbuffer[i];
              temp_seq2++;
              i++;
              buf_len--;
            }
        }
    } while (ctr != SEQPORT_EOF * -1);  


  SeqPortFree(sspp);
  SeqPortFree(qspp);
  if (qbuffer) MemFree(qbuffer);
  if (sbuffer) MemFree(sbuffer);
  mip->qseq = qseq;
  mip->sseq = sseq;
  return TRUE;
}


/*_________________________________________(DOT_GetSeqsbyLoc)_____

  Purpose : Calls functions to fill protein or nucleotide buffers.

____________________________________________________________________*/
static Boolean DOT_GetSeqsbyLoc (DOTMainDataPtr mip)
{
   SeqIdPtr         ssip, qsip, sId, qId; 
  Char         q_idbuf[42]={""}, s_idbuf[42]={""};

/*   mip->sname = (CharPtr)MemNew(42*sizeof(Char)); */
/*   mip->qname = (CharPtr)MemNew(42*sizeof(Char)); */
/*
  FastaId (mip->qbsp, idbuf, 32);
  sprintf (mip->qname, "%s",idbuf);
  FastaId (mip->sbsp, idbuf, 32);
  sprintf (mip->sname, "%s",idbuf);
  */
/*   qsip = SeqLocId(mip->qslp); */
/*   qId = SeqIdFindBestAccession(qsip); */
/*   ssip = SeqLocId(mip->sslp); */
/*   sId = SeqIdFindBestAccession(ssip); */
  SeqIdWrite(mip->qbsp->id, q_idbuf,PRINTID_FASTA_SHORT, 41);
  SeqIdWrite(mip->sbsp->id, s_idbuf,PRINTID_FASTA_SHORT, 41);
  mip->qname=StringSave(q_idbuf);
  mip->sname=StringSave(s_idbuf);

 /*  printf ("\nLengths: \n %s(query_seq):%d\n %s(subject_seq):%d\n", mip->qname, mip->qlen, mip->sname, mip->slen); */


 
 if (is_na)
   {
     if (!DOT_GetNResidues(mip, TRUE))
       return FALSE;
   }
 else
   {
     if (!DOT_GetPResidues(mip, TRUE))
       return FALSE;
   }
 return TRUE;
}

/*_________________________________________(DOT_GetSeqs)_____

  Purpose : Calls functions to fill protein or nucleotide buffers.

____________________________________________________________________*/
Boolean DOT_GetSeqs (DOTMainDataPtr mip, Boolean is_zoom)
{
  SeqIdPtr         ssip, qsip;
  Char             q_idbuf[42]={""}, s_idbuf[42]={""};


 if (is_zoom)
   {
     if (mip->qseq) mip->qseq=MemFree(mip->qseq);
     if (mip->sseq) mip->sseq=MemFree(mip->sseq);
   }
 else
   {
/*      mip->sname = (CharPtr)MemNew(42*sizeof(Char)); */
/*      mip->qname = (CharPtr)MemNew(42*sizeof(Char)); */
     SeqIdWrite(mip->qbsp->id, q_idbuf,PRINTID_FASTA_SHORT, 41); 
     SeqIdWrite(mip->sbsp->id, s_idbuf,PRINTID_FASTA_SHORT, 41); 
     mip->qname=StringSave(q_idbuf);
     mip->sname=StringSave(s_idbuf);

    /*  FastaId (mip->qbsp, idbuf, 32); */
/*      sprintf (mip->qname, "%s",idbuf); */
/*      FastaId (mip->sbsp, idbuf, 32); */
/*      sprintf (mip->sname, "%s",idbuf); */
   }
 
 if (is_na)
   {
     if (!DOT_GetNResidues(mip, FALSE))
       return FALSE;
   }
 else
   {
     if (!DOT_GetPResidues(mip, FALSE))
       return FALSE;
   }

 return TRUE;
 
}


/*_______________________________________________(DOT_FreeHitPtrArray)___

  Purpose : Free memory function for hits array.

____________________________________________________________________*/
Int2 DOT_FreeHitsArray (DOTDiagPtr PNTR hitlist, Int4 index)
{
  Int4   i;
  DOTDiagPtr  hit;

  for(i = 0; i < index; i++) 
    {
      hit=hitlist[i];
      if (hit) MemFree(hit);
    }

  if (hitlist) hitlist=MemFree(hitlist);
 
  if (hitlist!=NULL)
    {
      ErrPostEx (SEV_WARNING, 0, 0, "%s", "hitlist not empty");
      return -1;
    } 
  return 0;
}

/*_______________________________________________(DOT_FreeMainInfo)___

  Purpose : Free memory function for main info ptr. Clears ONLY hitlist and sequence buffers.

____________________________________________________________________*/
Int2 DOT_FreeMainInfo(DOTMainDataPtr mip)
{
  if (mip->hitlist) DOT_FreeHitsArray(mip->hitlist, mip->index);
  if (mip->score_array) mip->score_array=MemFree(mip->score_array);
  if (mip->qseq) mip->qseq=MemFree(mip->qseq);
  if (mip->sseq) mip->sseq=MemFree(mip->sseq);

  return 0;
}

/*_______________________________________________(DOT_FreeMainInfoPtrEx)___

  Purpose : Free memory function for main info ptr. Clears all.

____________________________________________________________________*/
Int2 DOT_FreeMainInfoPtrEx (DOTMainDataPtr mip)
{

  DOT_FreeMainInfo(mip);

  if (mip->is_na && mip->matrix) 
    Free(mip->matrix);
  if (mip->qslp) SeqLocFree(mip->qslp);
  if (mip->sslp) SeqLocFree(mip->sslp);
/*   if (mip->qbsp) BioseqFree(mip->qbsp); */
/*   if (mip->sbsp) BioseqFree(mip->sbsp); */

  if (mip) MemFree(mip);

  return 0;
}

/*____________________________________________(DOT_InitTheRest)____________


  Purpose : Initialize function for DOTMainDataPtr used by InitMainInfo and InitMainInfobyLoc.

____________________________________________________________________*/


static void DOT_InitTheRest(DOTMainDataPtr mip, Int4 word_size,Int4 tree_limit)
{

 if (ISA_aa (mip->qbsp->mol) && ISA_aa (mip->sbsp->mol)) 
    {
      mip->is_na = FALSE;
      is_na=FALSE;
      if (word_size < 4 || word_size > 11)
        word_size=2;
    }
  else if (ISA_na(mip->qbsp->mol) && ISA_na (mip->sbsp->mol)) 
    {
      mip->is_na = TRUE;
      is_na=TRUE;
      if (word_size < 1 || word_size > 2)
        word_size=8;
    }

  mip->word_size=word_size;
 
  if (tree_limit==0)
    mip->tree_limit=10200; /* default */
  else
    mip->tree_limit=tree_limit;

  mip->cutoff_score=0;  
  mip->first_pass=TRUE;
  mip->unique=0;
  mip->index=0;
}


/*____________________________________________(DOT_InitMainInfo)____________


  Purpose : Initialize DOTMainDataPtr for main window.
  **** use bsp with the shorter seq length as 'qbsp' for max speed ****

____________________________________________________________________*/

extern DOTMainDataPtr DOT_InitMainInfo (DOTMainDataPtr mip, BioseqPtr qbsp, BioseqPtr sbsp, Int4 word_size, Int4 tree_limit, Int4 q_left, Int4 q_right, Int4 s_left, Int4 s_right)
{
  Int4   slen;
  Int4   qlen;
  
  mip->q_start=q_left;
  mip->q_stop=q_right;
  mip->s_start=s_left;
  mip->s_stop=s_right;
  mip->qbsp=qbsp;
  mip->sbsp=sbsp;
  mip->sstrand = Seq_strand_plus;
  mip->qstrand = Seq_strand_plus;

   if (!((ISA_aa (mip->qbsp->mol) && ISA_aa (mip->sbsp->mol))||(ISA_na(mip->qbsp->mol) && ISA_na (mip->sbsp->mol))))
    {
      ErrPostEx(SEV_ERROR, 0, 0, "DOT -missmatched sequence types");
      return MemFree(mip);
    }

  DOT_InitTheRest(mip, word_size, tree_limit);
  
  return mip;
}

static BioseqPtr DOT_GetBioseqGivenSeqLoc (SeqLocPtr slp, Uint2 entityID)

{
  BioseqPtr    bsp=NULL;
  SeqEntryPtr  sep=NULL;
  SeqIdPtr     sip=NULL;
  SeqLocPtr    tmp=NULL;

  if (slp == NULL) return NULL;
  bsp = NULL;
  sip = SeqLocId (slp);
  if (sip != NULL) {
    bsp = BioseqFind (sip);
  } else {
    tmp = SeqLocFindNext (slp, NULL);
    if (tmp != NULL) {
      sip = SeqLocId (tmp);
      if (sip != NULL) {
        bsp = BioseqFind (sip);
        if (bsp != NULL) {
          sep = SeqMgrGetSeqEntryForData (bsp);
          entityID = ObjMgrGetEntityIDForChoice (sep);
          bsp = GetBioseqGivenSeqLoc (slp, entityID);
        }
      }
    }
  }
  return bsp;
}


static BioseqPtr DOT_GetBioseqReferencedByAnnot (SeqAnnotPtr sap, Uint2 entityID)

{
  SeqAlignPtr   align;
  BioseqPtr     bsp;
  DenseDiagPtr  ddp;
  DenseSegPtr   dsp;
  SeqFeatPtr    feat;
  SeqGraphPtr   graph;
  SeqIdPtr      sip;
  SeqLocPtr     slp;
  StdSegPtr     ssp;
  SeqLocPtr     tloc;

  if (sap == NULL) return NULL;
  switch (sap->type) {
    case 1 :
      feat = (SeqFeatPtr) sap->data;
      while (feat != NULL) {
        slp = feat->location;
        if (slp != NULL) {
          bsp = DOT_GetBioseqGivenSeqLoc (slp, entityID);
          if (bsp != NULL) return bsp;
        }
        feat = feat->next;
      }
      break;
    case 2 :
      align = (SeqAlignPtr) sap->data;
      while (align != NULL) {
        if (align->segtype == 1) {
          ddp = (DenseDiagPtr) align->segs;
          if (ddp != NULL) {
            for (sip = ddp->id; sip != NULL; sip = sip->next) {
              bsp = BioseqFind (sip);
              if (bsp != NULL) return bsp;
            }
          }
        } else if (align->segtype == 2) {
          dsp = (DenseSegPtr) align->segs;
          if (dsp != NULL) {
            for (sip = dsp->ids; sip != NULL; sip = sip->next) {
              bsp = BioseqFind (sip);
              if (bsp != NULL) return bsp;
            }
          }
        } else if (align->segtype == 3) {
          ssp = (StdSegPtr) align->segs;
          if (ssp != NULL && ssp->loc != NULL) {
            for (tloc = ssp->loc; tloc != NULL; tloc = tloc->next) {
              bsp = BioseqFind (SeqLocId (tloc));
              if (bsp != NULL) return bsp;
            }
          }
        }
        align = align->next;
      }
      break;
    case 3 :
      graph = (SeqGraphPtr) sap->data;
      while (graph != NULL) {
        slp = graph->loc;
        if (slp != NULL) {
          bsp = DOT_GetBioseqGivenSeqLoc (slp, entityID);
          if (bsp != NULL) return bsp;
        }
        graph = graph->next;
      }
      break;
    default :
      break;
  }
  return NULL;
}

/*____________________________(DOT_AttachSeqAnnotToSeqEntry)_____


  Purpose : Attach SeqAnnot structure to SeqEntry 
________________________________________________________________*/

extern Uint2 DOT_AttachSeqAnnotToSeqEntry (Uint2 entityID, SeqAnnotPtr sap, BioseqPtr bsp)

{
  Int2           genCode;
  SeqEntryPtr    oldscope;
  OMProcControl  ompc;
  SeqEntryPtr    sep;
  SeqFeatPtr     sfp = NULL;

  if (sap == NULL) return entityID;
  if (bsp==NULL)
    bsp = DOT_GetBioseqReferencedByAnnot (sap, entityID);

  if (bsp == NULL) return entityID;
  
  
  sep = SeqMgrGetSeqEntryForData (bsp);
  entityID = ObjMgrGetEntityIDForChoice (sep);
  if (sap->type == 1) {
    sfp = (SeqFeatPtr) sap->data;
    sep = SeqMgrGetSeqEntryForData(bsp);
/*     sep = GetBestTopParentForData (entityID, bsp); */
    genCode = SeqEntryToGeneticCode (sep, NULL, NULL, 0);
    SetEmptyGeneticCodes (sap, genCode);
  } 
  MemSet ((Pointer) &ompc, 0, sizeof (OMProcControl));
  ompc.input_entityID = entityID;
  ompc.input_itemID = GetItemIDGivenPointer (entityID, OBJ_BIOSEQ, (Pointer) bsp);
  ompc.input_itemtype = OBJ_BIOSEQ;
  ompc.output_itemtype = OBJ_SEQANNOT;
  ompc.output_data = (Pointer) sap;
  if (! AttachDataForProc (&ompc, FALSE)) {
    Message (MSG_ERROR, "DOT_AttachSeqAnnotToSeqEntry failed");
  } else if (sfp != NULL) {
    PromoteXrefs (sfp, bsp, entityID);
  }

return entityID;
}


/*____________________________________________(DOT_InitMainInfobyLoc)____________


  Purpose : Initialize DOTMainDataPtr for main window.
  **** use slp with the shorter seq length as 'slp1' for max speed ****
____________________________________________________________________*/

static DOTMainDataPtr DOT_InitMainInfobyLoc (DOTMainDataPtr mip, SeqLocPtr slp1, SeqLocPtr slp2, Int4 word_size, Int4 tree_limit)
{
  Int4   slen;
  Int4   qlen, temp;
  SeqLocPtr qslp, sslp;
  BioseqPtr qbsp, sbsp;


  qlen=SeqLocLen(slp1);
  slen=SeqLocLen(slp2);

  qslp=slp1;
  sslp=slp2;

  mip->qstrand = SeqLocStrand(qslp);
  mip->sstrand = SeqLocStrand(sslp);


  mip->qbsp = BioseqFind(SeqLocId(qslp));
  mip->sbsp = BioseqFind(SeqLocId(sslp));

   if (!((ISA_aa (mip->qbsp->mol) && ISA_aa (mip->sbsp->mol))||(ISA_na(mip->qbsp->mol) && ISA_na (mip->sbsp->mol))))
    {
      ErrPostEx(SEV_ERROR, 0, 0, "DOT -missmatched sequence types");
      return MemFree(mip);
    }

  
  /* mip start and stop are in bioseq coordinates */
  mip->q_start = GetOffsetInBioseq (qslp, mip->qbsp, SEQLOC_LEFT_END);
  mip->s_start = GetOffsetInBioseq (sslp, mip->sbsp, SEQLOC_LEFT_END);
  mip->q_stop = GetOffsetInBioseq (qslp, mip->qbsp, SEQLOC_RIGHT_END);
  mip->s_stop = GetOffsetInBioseq (sslp, mip->sbsp, SEQLOC_RIGHT_END);


  mip->qslp=qslp;
  mip->sslp=sslp;
  mip->qlen=qlen;
  mip->slen=slen;



  DOT_InitTheRest(mip, word_size, tree_limit);
  return mip;
}

/*____________________________________________(DOT_ComputeDotPlot)__________

  Purpose : Process input sequence information.

____________________________________________________________________*/
static Boolean DOT_ComputeDotPlot (DOTMainDataPtr mip)
{
  DOTDiagPtr    PNTR  hitlist;
  Int4                index;


  DOT_CalculateMatrix_MinMax(mip);

  if (DOT_BuildHitList(mip, TRUE, TRUE)<0) 
    {
      ErrPostEx (SEV_WARNING, 0, 0, "%s", "DOT- no matches found");
      return FALSE; /* no hits */
    }
  return TRUE;
}

/*____________________________________________(DOT_SPI_FindBestAlnByDotPlot)__________

  Purpose : Sarah's function to find the best alignment.

____________________________________________________________________*/

SeqAlignPtr DOT_SPI_FindBestAlnByDotPlot(SeqLocPtr slp1, SeqLocPtr slp2, Int4 wordsize, Int4 num_hits)
{
   DOTDiagPtr      ddp;
   DenseSegPtr     dsp;
   Int4            i;
   DOTMainDataPtr  mip;
   SeqAlignPtr     sap;
   SeqAlignPtr     sap_head;
   SeqAlignPtr     sap_prev;
   ScorePtr        scp;
   Int4            start1;
   Int4            start2;
   Uint1           strand;

   mip = DOT_CreateAndStorebyLoc (slp1, slp2, wordsize, num_hits);
   sap = sap_head = sap_prev = NULL;
   if (mip == NULL || mip->hitlist == NULL)
      return NULL;
   i = 0;
   ddp = *mip->hitlist;
   start1 = SeqLocStart(slp1);
   start2 = SeqLocStart(slp2);
   strand = SeqLocStrand(slp2);
   while (ddp != NULL && i < mip->index)
   {
      ddp = mip->hitlist[i];
      i++;
      sap = SeqAlignNew();
      dsp = DenseSegNew();
      sap->type = SAT_PARTIAL;
      sap->segtype = SAS_DENSEG;
      sap->dim = 2;
      dsp->dim = 2;
      dsp->numseg = 1;
      dsp->ids = SeqIdDup(SeqLocId(slp1));
      dsp->ids->next = SeqIdDup(SeqLocId(slp2));
      dsp->strands = (Uint1Ptr)MemNew(2*sizeof(Uint1));
      dsp->strands[0] = SeqLocStrand(slp1);
      dsp->strands[1] = SeqLocStrand(slp2);
      dsp->starts = (Int4Ptr)MemNew(2*sizeof(Int4));
      dsp->lens = (Int4Ptr)MemNew(sizeof(Int4));
      dsp->starts[0] = ddp->q_start;
      if (dsp->strands[1] == Seq_strand_minus)
         dsp->starts[1] = ddp->s_start - ddp->length + 1;
      else
         dsp->starts[1] = ddp->s_start;
      if (ddp->length > SeqLocLen(slp2))
         dsp->lens[0] = SeqLocLen(slp2);
      else
         dsp->lens[0] = ddp->length - 1;
      scp = ScoreNew();
      scp->id = ObjectIdNew();
      scp->id->str = StringSave("score");
      scp->choice = 1;
      scp->value.intvalue = ddp->score;
      dsp->scores = scp;
      sap->segs = (Pointer)(dsp);
      if (sap_head != NULL)
      {
         sap_prev->next = sap;
         sap_prev = sap;
      } else
         sap_head = sap_prev = sap;
   }
   if (sap_head == NULL)
      return NULL;
   AlnMgrIndexSeqAlign(sap_head);
   AlnMgrMakeMultipleByScore(sap_head);
   AlnMgrDeleteHidden(sap_head, FALSE);
   sap = (SeqAlignPtr)(sap_head->segs);
   sap_head->segs = NULL;
   SeqAlignFree(sap_head);
   return sap;
}

/*____________________________________________(DOT_CreateAndStorebyLoc)__________

  Purpose : Runs Dotplot functions, does plus - minus strands.

____________________________________________________________________*/
DOTMainDataPtr DOT_CreateAndStorebyLoc (SeqLocPtr slp1, SeqLocPtr slp2, Int4 word_size, Int4 num_hits)
{
  DOTMainDataPtr mip;
  
  if (slp1 == NULL || slp2 == NULL || word_size == 0 || num_hits == 0 || SeqLocLen(slp1)<word_size || SeqLocLen(slp2)<word_size )
    return NULL;

  mip=(DOTMainDataPtr)MemNew(sizeof(DOTMainData));
  if (!(mip=DOT_InitMainInfobyLoc(mip, slp1, slp2, word_size, num_hits)))
    return NULL;

  if (!DOT_GetSeqsbyLoc(mip))
    return NULL;
  if (!DOT_ComputeDotPlot(mip))
    return NULL;
  
  return mip;

}

/*____________________________________________(DOT_CreateAndStore)__________

  Purpose : Runs Dotplot functions.

____________________________________________________________________*/
DOTMainDataPtr DOT_CreateAndStore (DOTMainDataPtr mip, BioseqPtr qbsp, BioseqPtr sbsp, Int4 q_start, Int4 q_stop, Int4 s_start, Int4 s_stop, Int4 word_size, Int4 num_hits, Boolean initialize)
{
  if (mip==NULL) return NULL;

  if (mip==NULL || qbsp == NULL || sbsp == NULL || word_size == 0 || num_hits == 0 || qbsp->length<word_size || sbsp->length<word_size )
    return NULL;

  if (initialize)
    if (!(mip=DOT_InitMainInfo(mip, qbsp, sbsp, word_size, num_hits, 0, qbsp->length-1, 0, sbsp->length-1)))
      return NULL;
  if (!DOT_GetSeqs(mip, FALSE))
    return NULL;
  if (!DOT_ComputeDotPlot(mip))
    return NULL;
  
  return mip;

}

