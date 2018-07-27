/*  taxcs.c
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
* File Name:  taxcs.c
*
* Author:  Vladimir Soussov
*   
* File Description:  taxonomy server/client common utilities
*
*
* $Log: taxcs.c,v $
* Revision 1.3  1998/09/01 15:41:39  soussov
* removing warnings
*
* Revision 1.2  1998/04/01 17:34:06  soussov
* changed tp include <>
*
* Revision 1.1  1998/02/10 20:11:59  soussov
* taxon2 related soft
*
* Revision 1.2  1997/05/12 18:32:33  soussov
* 05/12/97
*
 * Revision 1.1.1.1  1997/04/30  21:29:41  soussov
 * initial tree for taxon2
 *
*
*/

#include <time.h>
#include <stdlib.h>
#include <txcommon.h>

/*
typedef struct t_nameClass {
    Int2 priority;
    char class_txt[34];
} TaxNameClass, PNTR TaxNameClassPtr;
 */
static Int2 class_alloced= 16;
static TaxNameClassPtr name_class= NULL;

Int4 tax_getBaseTime(void)
{
    static Int4 basetime= 0;
    static Boolean baseTime_set= FALSE;

    if(!baseTime_set) {
	Int4 yy, vv, dd= 0;
	time_t t= 0;
	
	vv= gmtime(&t)->tm_year;

	if(vv > 0) {
	    for(yy= 1900; yy < (1900 + vv); yy++) {
		if(yy%4 == 0) dd+= 366;
		else dd+= 365;
	    }

	    dd--;

	    basetime= (dd*24)*60;
	}
	baseTime_set= TRUE;

    }

    return basetime;
}

CharPtr tax_prntTime(Int4 t)
{
    time_t ttt= (t - tax_getBaseTime())* 60;
    
    return asctime(localtime(&ttt));
}

Int4 tax_getTime(void)
{
    time_t ttt= time(NULL);
    Int4 t= ttt/60 + tax_getBaseTime();
    return t;
}

Boolean tax_addNameClass(Int4 class_cde, CharPtr class_txt, Int4 priority)
{
    Int2 i;

    if(name_class == NULL) {
	name_class= MemNew(sizeof(TaxNameClass)*class_alloced);
	if(name_class == NULL) return FALSE;
	
	for(i= 0; i < class_alloced; i++) {
	    name_class[i].priority= 1000;
	    name_class[i].class_txt[0]= name_class[i].class_txt[33]= '\0';	    
	}
    }
    
    if(class_cde < 0) return FALSE;

    if(class_cde >= class_alloced) {
	TaxNameClassPtr tmp= MemMore(name_class, sizeof(TaxNameClass)*(class_cde + 8));

	if(tmp == NULL) return FALSE;
	
	name_class= tmp;
	for(i= class_alloced; i < class_cde + 8; i++) {
	    name_class[i].priority= 1000;
	    name_class[i].class_txt[0]= name_class[i].class_txt[33]= '\0';	    
	}
	class_alloced= class_cde + 8;
    }

    name_class[class_cde].priority= priority;
    StringNCpy(name_class[class_cde].class_txt, class_txt, 33);

    return TRUE;
}

CharPtr tax_getNameClass(Int4 class_cde, Int4Ptr priority)
{
    if((class_cde < 0) || (class_cde >= class_alloced)) {
	if(priority != NULL) *priority= 1000;
	return "";
    }

    if(priority != NULL) *priority= name_class[class_cde].priority;
    return name_class[class_cde].class_txt;
}

Int4 tax_getClass_cde(CharPtr class_txt)
{
    Int2 i;

    if(class_txt == NULL) return -1;

    for(i= 0; i < class_alloced; i++) {
	if(StringICmp(name_class[i].class_txt, class_txt) == 0) return (Int4)i;
    }

    return -1;
}

Boolean tax_dumpNameClasses(void (*dmpFunc)(VoidPtr, Int2, Int2, CharPtr), VoidPtr usrData)
{
    Int2 i;

    if((name_class == NULL) || (dmpFunc == NULL)) return FALSE;
    
    for(i= 0; i < class_alloced; i++) {
	if(name_class[i].class_txt[0] != '\0') {
	    (*dmpFunc)(usrData, i, name_class[i].priority, name_class[i].class_txt);
	}
    }
    return TRUE;
}

/*
typedef struct t_rank {
    char rank_txt[64];
} TaxRank, PNTR TaxRankPtr;
 */

static Int2 rank_alloced= 48, d_rank= 1;
static TaxRankPtr tax_rank= NULL;

Boolean tax_addRank(Int2 rank_id, CharPtr rank_txt)
{
    Int2 i;

    rank_id+= d_rank;

    if(tax_rank == NULL) {
	tax_rank= MemNew(sizeof(TaxRank)*rank_alloced);
	if(tax_rank == NULL) return FALSE;
	
	for(i= 0; i < rank_alloced; i++) {
	    tax_rank[i].rank_txt[0]= tax_rank[i].rank_txt[63]= '\0';	    
	}
    }
    
    if(rank_id < 0) return FALSE;

    if(rank_id >= rank_alloced) {
	TaxRankPtr tmp= MemMore(tax_rank, sizeof(TaxRank)*(rank_id + 8));

	if(tmp == NULL) return FALSE;
	
	tax_rank= tmp;
	for(i= rank_alloced; i < rank_id + 8; i++) {
	    tax_rank[i].rank_txt[0]= tax_rank[i].rank_txt[63]= '\0';	    
	}
	rank_alloced= rank_id + 8;
    }

    StringNCpy(tax_rank[rank_id].rank_txt, rank_txt, 63);

    return TRUE;
}
    
CharPtr tax_getRank(Int2 rank_id)
{
    rank_id+= d_rank;

    if((rank_id < 0) || (rank_id >= rank_alloced)) return "";

    return tax_rank[rank_id].rank_txt;
}

Int2 tax_getRankId(CharPtr rank_txt)
{
    Int2 i;

    if(rank_txt == NULL) return -d_rank;

    for(i= 0; i < rank_alloced; i++) {
	if(StringICmp(tax_rank[i].rank_txt, rank_txt) == 0) return i - d_rank;
    }

    return -d_rank;
}


Boolean tax_dumpRanks(void (*dmpFunc)(VoidPtr, Int2, CharPtr), VoidPtr usrData)
{
    Int2 i;

    if((tax_rank == NULL) || (dmpFunc == NULL)) return FALSE;
    
    for(i= 0; i < rank_alloced; i++) {
	if(tax_rank[i].rank_txt[0] != '\0') {
	    (*dmpFunc)(usrData, i - d_rank, tax_rank[i].rank_txt);
	}
    }
    return TRUE;
}

/*
typedef struct t_division {
    char div_cde[4];
    char div_txt[64];
} TaxDivision, PNTR TaxDivisionPtr;
 */

static Int2 div_alloced= 16;
static TaxDivisionPtr tax_div= NULL;

Boolean tax_addDivision(Int4 div_id, CharPtr div_cde, CharPtr div_txt)
{
    Int2 i;

    if(tax_div == NULL) {
	tax_div= MemNew(sizeof(TaxDivision)*div_alloced);
	if(tax_div == NULL) return FALSE;
	
	for(i= 0; i < div_alloced; i++) {
	    tax_div[i].div_cde[0]= tax_div[i].div_cde[3]= 
		tax_div[i].div_txt[0]= tax_div[i].div_txt[63]= '\0';
	}
    }
    
    if(div_id < 0) return FALSE;

    if(div_id >= div_alloced) {
	TaxDivisionPtr tmp= MemMore(tax_div, sizeof(TaxDivision)*(div_id + 8));

	if(tmp == NULL) return FALSE;
	
	tax_div= tmp;
	for(i= div_alloced; i < div_id + 8; i++) {
	    tax_div[i].div_cde[0]= tax_div[i].div_cde[3]= 
		tax_div[i].div_txt[0]= tax_div[i].div_txt[63]= '\0';
	}
	div_alloced= div_id + 8;
    }

    StringNCpy(tax_div[div_id].div_cde, div_cde, 3);
    StringNCpy(tax_div[div_id].div_txt, div_txt, 63);

    return TRUE;
}

Boolean tax_getDivision(Int2 div_id, CharPtr* div_cde, CharPtr* div_txt)
{
    if((div_id < 0) || (div_id >= div_alloced)) return FALSE;

    if(div_cde != NULL) *div_cde= tax_div[div_id].div_cde;
    if(div_txt != NULL) *div_txt= tax_div[div_id].div_txt;
    return TRUE;
}

Int2 tax_getDivisionId(CharPtr div_cde, CharPtr div_txt)
{
    Int2 i;

    if(div_cde != NULL) {
	for(i= 0; i < div_alloced; i++) {
	    if(StringICmp(tax_div[i].div_cde, div_cde) == 0) return i;
	}
    }

    if(div_txt != NULL) {
	for(i= 0; i < div_alloced; i++) {
	    if(StringICmp(tax_div[i].div_txt, div_txt) == 0) return i;
	}
    }

    return -1;
}

Boolean tax_dumpDivisions(void (*dmpFunc)(VoidPtr, Int2, CharPtr, CharPtr), VoidPtr usrData)
{
    Int2 i;

    if((tax_div == NULL) || (dmpFunc == NULL)) return FALSE;
    
    for(i= 0; i < div_alloced; i++) {
	if(tax_div[i].div_txt[0] != '\0') {
	    (*dmpFunc)(usrData, i, tax_div[i].div_cde, tax_div[i].div_txt);
	}
    }
    return TRUE;
}

/*
typedef struct t_genCode {
    char gc_name[128];
} TaxGenCode, PNTR TaxGenCodePtr;
 */
static Int2 gc_alloced= 20;
static TaxGenCodePtr tax_gc= NULL;

Boolean tax_addGC(Int2 gc_id, CharPtr gc_txt)
{
    Int2 i;

    if(tax_gc == NULL) {
	tax_gc= MemNew(sizeof(TaxGenCode)*gc_alloced);
	if(tax_gc == NULL) return FALSE;
	
	for(i= 0; i < gc_alloced; i++) {
	    tax_gc[i].gc_name[0]= tax_gc[i].gc_name[127]= '\0';	    
	}
    }
    
    if(gc_id < 0) return FALSE;

    if(gc_id >= gc_alloced) {
	TaxGenCodePtr tmp= MemMore(tax_gc, sizeof(TaxGenCode)*(gc_id + 8));

	if(tmp == NULL) return FALSE;
	
	tax_gc= tmp;
	for(i= gc_alloced; i < gc_id + 8; i++) {
	    tax_gc[i].gc_name[0]= tax_gc[i].gc_name[127]= '\0';	    
	}
	gc_alloced= gc_id + 8;
    }

    StringNCpy(tax_gc[gc_id].gc_name, gc_txt, 127);

    return TRUE;
}
    
CharPtr tax_getGCName(Int2 gc_id)
{

    if((gc_id < 0) || (gc_id >= gc_alloced)) return "";

    return tax_gc[gc_id].gc_name;
}

Int2 tax_getGCId(CharPtr gc_txt)
{
    Int2 i;

    if(gc_txt == NULL) return -1;

    for(i= 0; i < gc_alloced; i++) {
	if(StringICmp(tax_gc[i].gc_name, gc_txt) == 0) return i;
    }

    return -1;
}

Boolean tax_dumpGCs(void (*dmpFunc)(VoidPtr, Int2, CharPtr), VoidPtr usrData)
{
    Int2 i;

    if((tax_gc == NULL) || (dmpFunc == NULL)) return FALSE;
    
    for(i= 0; i < gc_alloced; i++) {
	if(tax_gc[i].gc_name[0] != '\0') {
	    (*dmpFunc)(usrData, i, tax_gc[i].gc_name);
	}
    }
    return TRUE;
}


#ifdef TAX_LOG

#include <rex_util.h>

static CharPtr get_token(CharPtr str, CharPtr token)
{
  int i;

  token[2]= '\0';

  while(IS_WHITESP(*str)) {
    if(*str == '\0') return NULL;
    ++str;
  }

  if(*str == '\0') return NULL;
  
  for(i= 1; i < 250; i++) {
    if(IS_WHITESP(*str)) {
      token[i]= ' ';
      token[i+1]= '\0';
      return str;
    }

    if(*str == '\0') {
      token[i]= ' ';
      token[i+1]= '\0';
      return NULL;
    }

    token[i]= TO_UPPER(*str);
    str++;
  }

  token[i]= ' ';
  token[i+1]= '*';
  token[i+2]= '\0';
  return str;
}

Boolean tax_matchName(CharPtr orgName, CharPtr str, Int4 mode)
{
    if(StringICmp(orgName, str) == 0) return TRUE;
    
    if(mode == TAX_RE_SEARCH) {
	/* regular expression search */
	char nBuff[256];
	Int4 k;
	rex_handler rh;
	Boolean res;

	strncpy(&nBuff[1], str, 250);
	nBuff[0]= '@';
	k= strlen(nBuff);
	nBuff[k]= '@';
	nBuff[k+1]= '\0';
	rh= rex_setExpr(nBuff, REX_NO_CASE | REX_NO_SPACE);
	if(rh == NULL) return 0;

	nBuff[0]= '@';
	for(k= 0; (k < 250) && (orgName[k] != '\0'); k++) {
	    nBuff[k+1]= toupper(orgName[k]);
	}
	k++;
	nBuff[k]= '@';
	nBuff[k+1]= '\0';

	res= rex_cmp(rh, nBuff);

	free(rh);
	return res;
    }

    if(mode == TAX_TOKEN_SEARCH) {
	char nBuff[256];
	Int4 k;
	rex_handler rh[16];
	Int2 nof_rh, j, res;
	CharPtr tail= str;

	nBuff[0]= '*';
	nBuff[1]= ' ';
	for(nof_rh= 0; (nof_rh != 16) && (tail != NULL); nof_rh++) {
	    tail= get_token(tail, nBuff);
	    rh[nof_rh]= rex_setExpr(nBuff, REX_NO_CASE | REX_NO_SPACE);
	    if(rh[nof_rh] == NULL) nof_rh--;
	}
	if(nof_rh < 1) return FALSE;

	nBuff[0]= ' ';

	tail= orgName;
	for(k= 0; (k < 250) && (tail[k] != '\0'); k++) {
	    nBuff[k+1]= toupper(tail[k]);
	}
	k++;
	nBuff[k]= ' ';
	nBuff[k+1]= '\0';

	for(res= 1, j= 0; (j < nof_rh) && (res > 0); j++) {
	    res= rex_cmp(rh[j], nBuff);
	}

	for(j= 0; j < nof_rh; j++) {
	    free(rh[j]);
	}
	return (res != 0)? TRUE : FALSE; 
    }

    return FALSE;
}
#endif	
