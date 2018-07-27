/* gbparint.c
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
* File Name:  gbparint.c
*
* Author:  Karl Sirotkin
*
* $Log: gbparint.c,v $
* Revision 6.8  2004/07/22 16:08:35  bazhin
* Changes to parse gaps of unknown lengths (like "gap(unk100)")
* within location strings.
*
* Revision 6.7  2004/03/03 17:32:19  kans
* Nlm_gbparselex checks against NULL input
*
* Revision 6.6  2003/12/05 16:42:11  bazhin
* Nlm_gbparselex() and Nlm_gbparselex_ver() functions now can handle
* RefSeq and WGS accessions.
*
* Revision 6.5  2001/06/07 17:00:54  tatiana
* added gi option in Nlm_gbparselex()
*
* Revision 6.4  2000/03/20 23:38:39  aleksey
* Finally submitted the changes which have been made by serge bazhin
* and been kept in my local directory.
*
* These changes allow to establish user callback functions
* in 'Asn2ffJobPtr' structure which are called within
* 'SeqEntryToFlatAjp' function call.
* The new members are:
* user_data       - pointer to a user context for passing data
* ajp_count_index - user defined function
* ajp_print_data  - user defined function
* ajp_print_index - user defined function
*
* Revision 6.3  1999/04/06 19:42:55  bazhin
* Changes, related to flat2asn's ACCESSION.VERSION parsing.
*
* Revision 6.2  1999/04/02 21:15:07  tatiana
* accession.version added
*
* Revision 6.1  1997/10/24 21:28:39  bazhin
* Is able to distinguish and process "gap(...)" tokens inside
* of location entries. Made for CONTIG line join contents.
*
* Revision 6.0  1997/08/25 18:06:05  madden
* Revision changed to 6.0
*
* Revision 5.3  1997/06/19 18:38:01  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.2  1997/02/06 00:16:14  tatiana
* dealing with 2+6 accession
*
 * Revision 5.1  1997/01/27  19:16:17  tatiana
 * accept two-letter prefix in accession number
 *
 * Revision 5.0  1996/05/28  13:23:23  ostell
 * Set to revision 5.0
 *
 * Revision 4.2  1996/05/21  21:12:05  tatiana
 * bullet proof in gbparseint()
 *
 * Revision 4.1  1995/07/31  19:02:10  tatiana
 * fix seq_id->choice
 *
 * Revision 1.8  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
*/

#include "parsegb.h"
#include "gbparlex.h"
#include "errdefn.h"
#include <sequtil.h>
#include <edutil.h>

#define TAKE_FIRST 1
#define TAKE_SECOND 2

void Nlm_gbgap PROTO((ValNodePtr PNTR currentPt, ValNodePtr PNTR retval,
                      Boolean unknown));

/*--------- do_Nlm_gbparse_error () ---------------*/

NLM_EXTERN void
do_Nlm_gbparse_error (CharPtr msg, CharPtr details)
{
	Int4 len = StringLen(msg) +7;
	CharPtr errmsg, temp;

	len += StringLen(details);
	temp = errmsg= MemNew((size_t)len);
	temp = StringMove(temp, msg);
	temp = StringMove(temp, " at ");
	temp = StringMove(temp, details);

	ErrPostStr(SEV_ERROR, ERR_FEATURE_LocationParsing, errmsg);

	MemFree(errmsg);
}
#define MAKE_THREAD_SAFE
#ifndef MAKE_THREAD_SAFE
static Nlm_gbparse_errfunc Err_func = do_Nlm_gbparse_error;
static Nlm_gbparse_rangefunc Range_func = NULL;
static Pointer Nlm_gbparse_range_data = NULL;
#define MACRO_THREAD_SAVE_STATIC

#else

#include <ncbithr.h>

static TNlmTls Err_func_tls=NULL;
static TNlmTls Range_func_tls=NULL;
static TNlmTls Nlm_gbparse_range_data_tls=NULL;

#define MACRO_THREAD_SAVE_STATIC \
        Nlm_gbparse_errfunc Err_func = NULL; \
        Nlm_gbparse_rangefunc Range_func = NULL; \
        Pointer Nlm_gbparse_range_data = NULL; \
        if(Err_func_tls) NlmTlsGetValue(Err_func_tls,(VoidPtr PNTR)&Err_func); \
	if(!Err_func) Err_func = do_Nlm_gbparse_error; \
        if(Range_func_tls) NlmTlsGetValue(Range_func_tls,(VoidPtr PNTR)&Range_func); \
        if(Nlm_gbparse_range_data_tls) NlmTlsGetValue(Nlm_gbparse_range_data_tls,(VoidPtr PNTR)&Nlm_gbparse_range_data); 
#endif

/*------------------ Nlm_gbcheck_range()-------------*/
static void
Nlm_gbcheck_range(Int4 num, SeqIdPtr idp, Boolean PNTR keep_rawPt, int PNTR num_errsPt, ValNodePtr head, ValNodePtr current)
{
	Int4 len;
	MACRO_THREAD_SAVE_STATIC;
	if (Range_func != NULL){
		len = (*Range_func)(Nlm_gbparse_range_data, idp);
		if (len > 0)
		if (num <0 || num >= len){
			Nlm_gbparse_error("range error",  head, current);
			* keep_rawPt = TRUE;
			(*num_errsPt) ++;
		}
	}
}

/*----------- Nlm_install_gbparse_error_handler ()-------------*/

NLM_EXTERN void
Nlm_install_gbparse_error_handler(Nlm_gbparse_errfunc new_func)
{
#ifdef MAKE_THREAD_SAFE
	NlmTlsSetValue(&Err_func_tls, (VoidPtr PNTR) new_func, NULL);
#else
	Err_func = new_func;
#endif
}

/*----------- Nlm_install_gbparse_range_func ()-------------*/

NLM_EXTERN void
Nlm_install_gbparse_range_func(Pointer data, Nlm_gbparse_rangefunc new_func)
{
#ifdef MAKE_THREAD_SAFE
	NlmTlsSetValue(&Range_func_tls, (VoidPtr PNTR) new_func,NULL);
	NlmTlsSetValue(&Nlm_gbparse_range_data_tls,data,NULL);
#else
	Range_func = new_func;
	Nlm_gbparse_range_data = data;
#endif

}

/*--------- Nlm_gbparse_error()-----------*/

NLM_EXTERN void
Nlm_gbparse_error(CharPtr front, ValNodePtr head, ValNodePtr current)
{
	CharPtr details;

	MACRO_THREAD_SAVE_STATIC;

	details = Nlm_gbparse_point (head, current);
	Err_func (front,details); 
	MemFree(details);
}

/*------ Nlm_gbparse_point ()----*/

NLM_EXTERN CharPtr 
Nlm_gbparse_point (ValNodePtr head, ValNodePtr current)
{
	CharPtr temp, retval = NULL;
	int len = 0;
	ValNodePtr now;

	for ( now = head; now ; now = now -> next){
		switch ( now-> choice){
			case GBPARSE_INT_JOIN :
				len += 4;
				break;
			case GBPARSE_INT_COMPL :
				len += 10;
				break;
			case GBPARSE_INT_LEFT :
			case GBPARSE_INT_RIGHT :
			case GBPARSE_INT_CARET :
			case GBPARSE_INT_GT :
			case GBPARSE_INT_LT :
			case GBPARSE_INT_COMMA :
			case GBPARSE_INT_SINGLE_DOT :
				len ++;
				break;
			case GBPARSE_INT_DOT_DOT :
				len += 2;
				break;
			case GBPARSE_INT_ACCESION :
			case GBPARSE_INT_NUMBER :
				len += StringLen ( now -> data.ptrvalue);
				break;
			case GBPARSE_INT_ORDER :
			case GBPARSE_INT_GROUP :
				len += 5;
				break;
			case GBPARSE_INT_ONE_OF :
			case GBPARSE_INT_ONE_OF_NUM:
				len += 6;
				break;
			case GBPARSE_INT_REPLACE :
				len += 7;
				break;
			case GBPARSE_INT_STRING:
				len += StringLen(now ->data.ptrvalue) + 1;
				break;
			case GBPARSE_INT_UNKNOWN :
			default:
				break;
		}
		len ++; /* for space */
		
		
		if ( now == current)
			break;
	}
	

	if (len > 0){
		temp = retval = MemNew(len+1);
		for ( now = head; now ; now = now -> next){
			switch ( now-> choice){
				case GBPARSE_INT_JOIN :
					temp = StringMove(temp,"join");
					break;
				case GBPARSE_INT_COMPL :
					temp = StringMove(temp,"complement");
					break;
				case GBPARSE_INT_LEFT :
					temp = StringMove(temp,"(");
					break;
				case GBPARSE_INT_RIGHT :
					temp = StringMove(temp,")");
					break;
				case GBPARSE_INT_CARET :
					temp = StringMove(temp,"^");
					break;
				case GBPARSE_INT_DOT_DOT :
					temp = StringMove(temp,"..");
					break;
				case GBPARSE_INT_ACCESION :
				case GBPARSE_INT_NUMBER :
				case GBPARSE_INT_STRING:
					temp = StringMove(temp,now -> data.ptrvalue);
					break;
				case GBPARSE_INT_GT :
					temp = StringMove(temp,">");
					break;
				case GBPARSE_INT_LT :
					temp = StringMove(temp,"<");
					break;
				case GBPARSE_INT_COMMA :
					temp = StringMove(temp,",");
					break;
				case GBPARSE_INT_ORDER :
					temp = StringMove(temp,"order");
					break;
				case GBPARSE_INT_SINGLE_DOT :
					temp = StringMove(temp,".");
					break;
				case GBPARSE_INT_GROUP :
					temp = StringMove(temp,"group");
					break;
				case GBPARSE_INT_ONE_OF :
				case GBPARSE_INT_ONE_OF_NUM:
					temp = StringMove(temp,"one-of");
					break;
				case GBPARSE_INT_REPLACE :
					temp = StringMove(temp,"replace");
					break;
				case GBPARSE_INT_UNKNOWN :
				default:
					break;
			}
				temp = StringMove(temp," ");
			if ( now == current)
				break;
		}
	}

	return retval;
}

/*--------- Nlm_find_one_of_num()------------*/
/*
 
Consider these for locations:
	     misc_signal     join(57..one-of(67,75),one-of(100,110)..200)
     misc_signal     join(57..one-of(67,75),one-of(100,110..120),200)
     misc_signal     join(57..one-of(67,75),one-of(100,110..115)..200)

     misc_signal     join(57..one-of(67,75),one-of(100,110),200)

In the first three, the one-of() is functioning as an alternative set
of numbers, in the last, as an alternative set of locations (even
though the locations are points).  
[yes the one-of(100,110..115).. is illegal]

  here is one more case:one-of(18,30)..470 so if the location
  starts with a one-of, it also needs to be checked.

To deal with this, the GBPARSE_INT_ONE_OF token type will be changed
by the following function to GBPARSE_INT_ONE_OF_NUM, in the three cases.

note that this change is not necessary in this case:
		join(100..200,300..one-of(400,500)), as after a ".." token,
    it has to be a number.

*/

static void
Nlm_find_one_of_num(ValNodePtr head_token)
{
	ValNodePtr current, scanner;

	current = head_token;
	if (current -> choice == GBPARSE_INT_ONE_OF){
			scanner= current -> next;
/*-------(is first token after ")" a ".."?----*/
			for (;scanner!=NULL; scanner = scanner -> next){
				if (scanner -> choice == GBPARSE_INT_RIGHT){
					scanner = scanner -> next;
					if (scanner != NULL){
						if (scanner -> choice == GBPARSE_INT_DOT_DOT){
/*---- this is it ! ! */
							current -> choice = GBPARSE_INT_ONE_OF_NUM;
						}
					}
					break;
				}
			}
	}
	for (current = head_token; current != NULL; current = current -> next){
		if ( current -> choice == GBPARSE_INT_COMMA || 
			current -> choice == GBPARSE_INT_LEFT ){
			scanner= current -> next;
			if ( scanner != NULL){
				if (scanner -> choice == GBPARSE_INT_ONE_OF){
/*-------(is first token after ")" a ".."?----*/
					for (;scanner!=NULL; scanner = scanner -> next){
						if (scanner -> choice == GBPARSE_INT_RIGHT){
							scanner = scanner -> next;
							if (scanner != NULL){
								if (scanner -> choice == GBPARSE_INT_DOT_DOT){
/*---- this is it ! ! */
									current -> next -> choice 
										= GBPARSE_INT_ONE_OF_NUM;
								}
							}
							break;
						}
					}
				}
			}
		}
	}

}

/*---------- Nlm_gbparseint()-----*/

NLM_EXTERN SeqLocPtr
Nlm_gbparseint(CharPtr raw_intervals, Boolean PNTR keep_rawPt, Boolean PNTR sitesPt, int PNTR num_errsPt, SeqIdPtr seq_id)
{
	SeqLocPtr retval = NULL;
	ValNodePtr head_token, current_token;
	int paren_count = 0;
	Boolean go_again;

	* keep_rawPt = FALSE;
	* sitesPt = FALSE;

	head_token = NULL;
	(*num_errsPt) = gbparselex(raw_intervals, & head_token);

	if (head_token == NULL) {
		*num_errsPt = 1;
		return NULL;
	}
	if ( ! (*num_errsPt)){
		current_token = head_token;
		Nlm_find_one_of_num(head_token);

	do {
		go_again= FALSE;
		if (current_token)
		switch ( current_token -> choice){
			case  GBPARSE_INT_JOIN : case  GBPARSE_INT_ORDER :
			case  GBPARSE_INT_GROUP : case  GBPARSE_INT_ONE_OF :
			case GBPARSE_INT_COMPL:
			retval = Nlm_gbloc(keep_rawPt,  & paren_count, sitesPt, & current_token,  
				head_token, (num_errsPt), seq_id);
/* need to check that out of tokens here */
			retval = Nlm_gbparse_better_be_done(num_errsPt, current_token, 
					head_token, retval, keep_rawPt,  paren_count);
				break;
			case GBPARSE_INT_STRING:
				Nlm_gbparse_error("string in loc", 
					head_token, current_token);
					* keep_rawPt = TRUE;  (* num_errsPt) ++;
/*  no break on purpose */
			case  GBPARSE_INT_UNKNOWN :
			default: 
			case  GBPARSE_INT_RIGHT :
			case  GBPARSE_INT_DOT_DOT :
			case  GBPARSE_INT_COMMA :
			case  GBPARSE_INT_SINGLE_DOT :
					
					Nlm_gbparse_error("illegal initial token", 
						head_token, current_token);
						* keep_rawPt = TRUE;  (* num_errsPt) ++;
				current_token = current_token -> next;
				break;

			case  GBPARSE_INT_ACCESION :
/*--- no warn, but strange ---*/
/*-- no break on purpose ---*/

			case  GBPARSE_INT_CARET : case  GBPARSE_INT_GT :
			case  GBPARSE_INT_LT : case  GBPARSE_INT_NUMBER :
			case  GBPARSE_INT_LEFT :

			case GBPARSE_INT_ONE_OF_NUM:

			retval = Nlm_gbint(keep_rawPt,  & current_token,
				head_token, (num_errsPt), seq_id);
/* need to check that out of tokens here */
			retval = Nlm_gbparse_better_be_done(num_errsPt, current_token, 
					head_token, retval, keep_rawPt,  paren_count);
				break;

			case  GBPARSE_INT_REPLACE :
			retval = Nlm_gbreplace(keep_rawPt,  & paren_count, sitesPt, & current_token,  
				head_token, (num_errsPt), seq_id);
				* keep_rawPt = TRUE;
/*---all errors handled within this function ---*/
				break;
			case GBPARSE_INT_SITES :
				* sitesPt = TRUE;
				go_again = TRUE;
				current_token = current_token -> next;
				break;
		}
	}while (go_again && current_token);
	}else{
		* keep_rawPt = TRUE;
	}
	
	if ( head_token)
		ValNodeFreeData(head_token);

	if ( (*num_errsPt)){
		SeqLocFree(retval);
		retval = NULL;
	}
	return retval;
}

/*---------- Nlm_gbparseint_ver()-----*/

NLM_EXTERN SeqLocPtr Nlm_gbparseint_ver(CharPtr raw_intervals,
                                        Boolean PNTR keep_rawPt,
                                        Boolean PNTR sitesPt,
                                        int PNTR num_errsPt,
                                        SeqIdPtr seq_id, Boolean accver)
{
	SeqLocPtr retval = NULL;
	ValNodePtr head_token, current_token;
	int paren_count = 0;
	Boolean go_again;

	* keep_rawPt = FALSE;
	* sitesPt = FALSE;

	head_token = NULL;
	(*num_errsPt) = Nlm_gbparselex_ver(raw_intervals, &head_token, accver);

	if (head_token == NULL) {
		*num_errsPt = 1;
		return NULL;
	}
	if ( ! (*num_errsPt)){
		current_token = head_token;
		Nlm_find_one_of_num(head_token);

	do {
		go_again= FALSE;
		if (current_token)
		switch ( current_token -> choice){
			case  GBPARSE_INT_JOIN : case  GBPARSE_INT_ORDER :
			case  GBPARSE_INT_GROUP : case  GBPARSE_INT_ONE_OF :
			case GBPARSE_INT_COMPL:
			retval = Nlm_gbloc_ver(keep_rawPt,  & paren_count, sitesPt, & current_token,  
				head_token, (num_errsPt), seq_id, accver);
/* need to check that out of tokens here */
			retval = Nlm_gbparse_better_be_done(num_errsPt, current_token, 
					head_token, retval, keep_rawPt,  paren_count);
				break;
			case GBPARSE_INT_STRING:
				Nlm_gbparse_error("string in loc", 
					head_token, current_token);
					* keep_rawPt = TRUE;  (* num_errsPt) ++;
/*  no break on purpose */
			case  GBPARSE_INT_UNKNOWN :
			default: 
			case  GBPARSE_INT_RIGHT :
			case  GBPARSE_INT_DOT_DOT :
			case  GBPARSE_INT_COMMA :
			case  GBPARSE_INT_SINGLE_DOT :
					
					Nlm_gbparse_error("illegal initial token", 
						head_token, current_token);
						* keep_rawPt = TRUE;  (* num_errsPt) ++;
				current_token = current_token -> next;
				break;

			case  GBPARSE_INT_ACCESION :
/*--- no warn, but strange ---*/
/*-- no break on purpose ---*/

			case  GBPARSE_INT_CARET : case  GBPARSE_INT_GT :
			case  GBPARSE_INT_LT : case  GBPARSE_INT_NUMBER :
			case  GBPARSE_INT_LEFT :

			case GBPARSE_INT_ONE_OF_NUM:

			retval = Nlm_gbint_ver(keep_rawPt,  & current_token,
				head_token, (num_errsPt), seq_id, accver);
/* need to check that out of tokens here */
			retval = Nlm_gbparse_better_be_done(num_errsPt, current_token, 
					head_token, retval, keep_rawPt,  paren_count);
				break;

			case  GBPARSE_INT_REPLACE :
			retval = Nlm_gbreplace_ver(keep_rawPt,  & paren_count, sitesPt, & current_token,  
				head_token, (num_errsPt), seq_id, accver);
				* keep_rawPt = TRUE;
/*---all errors handled within this function ---*/
				break;
			case GBPARSE_INT_SITES :
				* sitesPt = TRUE;
				go_again = TRUE;
				current_token = current_token -> next;
				break;
		}
	}while (go_again && current_token);
	}else{
		* keep_rawPt = TRUE;
	}
	
	if ( head_token)
		ValNodeFreeData(head_token);

	if ( (*num_errsPt)){
		SeqLocFree(retval);
		retval = NULL;
	}
	return retval;
}

/*---------- Nlm_gbloc()-----*/

NLM_EXTERN SeqLocPtr
Nlm_gbloc(Boolean PNTR keep_rawPt, int PNTR parenPt, Boolean PNTR sitesPt, ValNodePtr PNTR currentPt, ValNodePtr head_token, int PNTR num_errPt, SeqIdPtr seq_id)
{
	SeqLocPtr retval =NULL;
	Boolean add_nulls=FALSE;
	ValNodePtr current_token = * currentPt;
	Boolean did_complement= FALSE;
	Boolean go_again ;

	do {
		go_again= FALSE;
		switch ( current_token -> choice){
			case  GBPARSE_INT_COMPL :
			*currentPt = (* currentPt) -> next;
			if ( (*currentPt) == NULL){
				Nlm_gbparse_error("unexpected end of usable tokens",
					head_token, *currentPt);
				* keep_rawPt = TRUE;  (* num_errPt) ++;
				goto FATAL;
			}
			if ( (* currentPt) -> choice != GBPARSE_INT_LEFT){
					Nlm_gbparse_error("Missing \'(\'", /* paran match  ) */
						head_token, * currentPt);
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
			}else{
				(*parenPt) ++; *currentPt = (* currentPt) -> next;
				if ( ! * currentPt){
						Nlm_gbparse_error("illegal null contents",
							head_token, *currentPt);
						* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
				}else{
					if (  (* currentPt) -> choice == GBPARSE_INT_RIGHT){ /* paran match ( */
						Nlm_gbparse_error("Premature \')\'",
							head_token, *currentPt);
						* keep_rawPt = TRUE;  (* num_errPt) ++;
						goto FATAL;
					}else{
						retval =  Nlm_gbloc (keep_rawPt, parenPt, sitesPt, currentPt, 
							head_token, num_errPt,seq_id) ;
						SeqLocRevCmp ( retval);
						did_complement= TRUE;
						if ( * currentPt){
							if ( (* currentPt) -> choice != GBPARSE_INT_RIGHT){
									Nlm_gbparse_error("Missing \')\'",
										head_token, *currentPt);
									* keep_rawPt = TRUE;  (* num_errPt) ++;
									goto FATAL;
							}else{
								(*parenPt) --; *currentPt = (* currentPt) -> next;
							}
						}else{
							Nlm_gbparse_error("Missing \')\'",
								head_token, *currentPt);
							* keep_rawPt = TRUE;  (* num_errPt) ++;
							goto FATAL;
						}
					}
				}
			}
				break;
/* REAL LOCS */
			case GBPARSE_INT_JOIN : retval =ValNodeNew(NULL); retval -> choice = SEQLOC_MIX; break;
			case  GBPARSE_INT_ORDER : retval =ValNodeNew(NULL); retval -> choice = SEQLOC_MIX; add_nulls=TRUE;break;
			case  GBPARSE_INT_GROUP : * keep_rawPt = TRUE; retval =ValNodeNew(NULL); retval -> choice = SEQLOC_MIX;
				break;
			case  GBPARSE_INT_ONE_OF : retval =ValNodeNew(NULL); retval -> choice = SEQLOC_EQUIV; break;

/* ERROR */
			case GBPARSE_INT_STRING:
				Nlm_gbparse_error("string in loc", 
					head_token, current_token);
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
/*--- no break on purpose---*/
			case  GBPARSE_INT_UNKNOWN : default: 
			case  GBPARSE_INT_RIGHT : case  GBPARSE_INT_DOT_DOT:case  GBPARSE_INT_COMMA :
			case  GBPARSE_INT_SINGLE_DOT :
				Nlm_gbparse_error("illegal initial loc token",
					head_token, *currentPt);
				* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;

/* Interval, occurs on recursion */
			case GBPARSE_INT_GAP:
				Nlm_gbgap(currentPt, &retval, FALSE);
				break;
			case GBPARSE_INT_UNK_GAP:
				Nlm_gbgap(currentPt, &retval, TRUE);
				break;
			case  GBPARSE_INT_ACCESION :
			case  GBPARSE_INT_CARET : case  GBPARSE_INT_GT :
			case  GBPARSE_INT_LT : case  GBPARSE_INT_NUMBER :
			case  GBPARSE_INT_LEFT :

			case GBPARSE_INT_ONE_OF_NUM:

			retval = Nlm_gbint(keep_rawPt, currentPt, 
				head_token, num_errPt, seq_id);
				break;

			case  GBPARSE_INT_REPLACE :
/*-------illegal at this level --*/
				Nlm_gbparse_error("illegal replace",
					head_token, *currentPt);
				* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
			case GBPARSE_INT_SITES :
				* sitesPt = TRUE;
				go_again = TRUE;
				(*currentPt) = (*currentPt) -> next;
				break;
		}
	} while (go_again && *currentPt);

	if ( !  (* num_errPt)) if (retval && retval->choice != SEQLOC_NULL) 
	if ( retval -> choice != SEQLOC_INT && retval -> choice != SEQLOC_PNT
			&& ! did_complement){ 
/*--------
 * ONLY THE CHOICE has been set. the "join", etc. only has been noted
 *----*/
		*currentPt = (* currentPt) -> next;
		if ( ! * currentPt){
				Nlm_gbparse_error("unexpected end of interval tokens",
					head_token, *currentPt);
						* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
		}else{
			if ( (* currentPt) -> choice != GBPARSE_INT_LEFT){
					Nlm_gbparse_error("Missing \'(\'",
						head_token, *currentPt); /* paran match  ) */
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
			}else{
				(*parenPt) ++; *currentPt = (* currentPt) -> next;
				if ( ! * currentPt){
						Nlm_gbparse_error("illegal null contents",
							head_token, *currentPt);
						* keep_rawPt = TRUE;  (* num_errPt) ++;
						goto FATAL;
				}else{
					if (  (* currentPt) -> choice == GBPARSE_INT_RIGHT){ /* paran match ( */
						Nlm_gbparse_error("Premature \')\'" ,
							head_token, *currentPt);
						* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
					}else{

						ValNodePtr last= NULL, next_loc = NULL;

						while ( ! *num_errPt && * currentPt){
							if ( (* currentPt) -> choice == GBPARSE_INT_RIGHT){
								while  ((* currentPt) -> choice == GBPARSE_INT_RIGHT){ 
									(*parenPt) --;
									*currentPt = (* currentPt) -> next;
									if ( ! *currentPt)
										break;
								}
								break;
							}
							if ( ! * currentPt){
								break;
							}
							next_loc = Nlm_gbloc(keep_rawPt, parenPt,sitesPt,  
								currentPt, head_token, num_errPt,
								seq_id);
							if( retval -> data.ptrvalue == NULL)
								retval -> data.ptrvalue = next_loc;
							if ( last)
								last -> next = next_loc;
							last = next_loc;
							if ( ! * currentPt){
								break;
							}
							if ( ! * currentPt){
								break;
							}
							if ((* currentPt) -> choice == GBPARSE_INT_RIGHT){
								break;
							}
							if (  (* currentPt) -> choice == GBPARSE_INT_COMMA){
								*currentPt = (* currentPt) -> next;
								if(add_nulls){
									next_loc = ValNodeNew(last);
									next_loc -> choice = SEQLOC_NULL;
									last -> next = next_loc;
									last = next_loc;
								}
							}else{
								Nlm_gbparse_error("Illegal token after interval",
									head_token, *currentPt);
								* keep_rawPt = TRUE;  (* num_errPt) ++;
								goto FATAL;
							}
						}
					}
				}
				if ( (*currentPt) == NULL){
					Nlm_gbparse_error("unexpected end of usable tokens",
						head_token, *currentPt);
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
				}else{
					if ( (* currentPt) -> choice != GBPARSE_INT_RIGHT){
							Nlm_gbparse_error("Missing \')\'" /* paran match  ) */,
									 head_token, *currentPt);
							* keep_rawPt = TRUE;  (* num_errPt) ++;
						goto FATAL;
					}else{
						(*parenPt) --; *currentPt = (* currentPt) -> next;
					}
				}
			}
		}
	}

FATAL:
	if ( (* num_errPt)){
		if (retval){
			SeqLocFree(retval); 
			retval =ValNodeNew(NULL);
			retval -> choice = SEQLOC_WHOLE;
			retval -> data.ptrvalue = SeqIdDup(seq_id);
		}
	}
	
	return retval;
}

/*---------- Nlm_gbloc_ver()-----*/

NLM_EXTERN SeqLocPtr Nlm_gbloc_ver(Boolean PNTR keep_rawPt, int PNTR parenPt,
                               Boolean PNTR sitesPt, ValNodePtr PNTR currentPt,
                               ValNodePtr head_token, int PNTR num_errPt,
                               SeqIdPtr seq_id, Boolean accver)
{
	SeqLocPtr retval =NULL;
	Boolean add_nulls=FALSE;
	ValNodePtr current_token = * currentPt;
	Boolean did_complement= FALSE;
	Boolean go_again ;

	do {
		go_again= FALSE;
		switch ( current_token -> choice){
			case  GBPARSE_INT_COMPL :
			*currentPt = (* currentPt) -> next;
			if ( (*currentPt) == NULL){
				Nlm_gbparse_error("unexpected end of usable tokens",
					head_token, *currentPt);
				* keep_rawPt = TRUE;  (* num_errPt) ++;
				goto FATAL;
			}
			if ( (* currentPt) -> choice != GBPARSE_INT_LEFT){
					Nlm_gbparse_error("Missing \'(\'", /* paran match  ) */
						head_token, * currentPt);
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
			}else{
				(*parenPt) ++; *currentPt = (* currentPt) -> next;
				if ( ! * currentPt){
						Nlm_gbparse_error("illegal null contents",
							head_token, *currentPt);
						* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
				}else{
					if (  (* currentPt) -> choice == GBPARSE_INT_RIGHT){ /* paran match ( */
						Nlm_gbparse_error("Premature \')\'",
							head_token, *currentPt);
						* keep_rawPt = TRUE;  (* num_errPt) ++;
						goto FATAL;
					}else{
						retval =  Nlm_gbloc_ver (keep_rawPt, parenPt, sitesPt, currentPt, 
							head_token, num_errPt,seq_id, accver) ;
						SeqLocRevCmp ( retval);
						did_complement= TRUE;
						if ( * currentPt){
							if ( (* currentPt) -> choice != GBPARSE_INT_RIGHT){
									Nlm_gbparse_error("Missing \')\'",
										head_token, *currentPt);
									* keep_rawPt = TRUE;  (* num_errPt) ++;
									goto FATAL;
							}else{
								(*parenPt) --; *currentPt = (* currentPt) -> next;
							}
						}else{
							Nlm_gbparse_error("Missing \')\'",
								head_token, *currentPt);
							* keep_rawPt = TRUE;  (* num_errPt) ++;
							goto FATAL;
						}
					}
				}
			}
				break;
/* REAL LOCS */
			case GBPARSE_INT_JOIN : retval =ValNodeNew(NULL); retval -> choice = SEQLOC_MIX; break;
			case  GBPARSE_INT_ORDER : retval =ValNodeNew(NULL); retval -> choice = SEQLOC_MIX; add_nulls=TRUE;break;
			case  GBPARSE_INT_GROUP : * keep_rawPt = TRUE; retval =ValNodeNew(NULL); retval -> choice = SEQLOC_MIX;
				break;
			case  GBPARSE_INT_ONE_OF : retval =ValNodeNew(NULL); retval -> choice = SEQLOC_EQUIV; break;

/* ERROR */
			case GBPARSE_INT_STRING:
				Nlm_gbparse_error("string in loc", 
					head_token, current_token);
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
/*--- no break on purpose---*/
			case  GBPARSE_INT_UNKNOWN : default: 
			case  GBPARSE_INT_RIGHT : case  GBPARSE_INT_DOT_DOT:case  GBPARSE_INT_COMMA :
			case  GBPARSE_INT_SINGLE_DOT :
				Nlm_gbparse_error("illegal initial loc token",
					head_token, *currentPt);
				* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;

/* Interval, occurs on recursion */
			case GBPARSE_INT_GAP:
				Nlm_gbgap(currentPt, &retval, FALSE);
				break;
			case GBPARSE_INT_UNK_GAP:
				Nlm_gbgap(currentPt, &retval, TRUE);
				break;
			case  GBPARSE_INT_ACCESION :
			case  GBPARSE_INT_CARET : case  GBPARSE_INT_GT :
			case  GBPARSE_INT_LT : case  GBPARSE_INT_NUMBER :
			case  GBPARSE_INT_LEFT :

			case GBPARSE_INT_ONE_OF_NUM:

			retval = Nlm_gbint_ver(keep_rawPt, currentPt, 
				head_token, num_errPt, seq_id, accver);
				break;

			case  GBPARSE_INT_REPLACE :
/*-------illegal at this level --*/
				Nlm_gbparse_error("illegal replace",
					head_token, *currentPt);
				* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
			case GBPARSE_INT_SITES :
				* sitesPt = TRUE;
				go_again = TRUE;
				(*currentPt) = (*currentPt) -> next;
				break;
		}
	} while (go_again && *currentPt);

	if ( !  (* num_errPt)) if (retval && retval->choice != SEQLOC_NULL) 
	if ( retval -> choice != SEQLOC_INT && retval -> choice != SEQLOC_PNT
			&& ! did_complement){ 
/*--------
 * ONLY THE CHOICE has been set. the "join", etc. only has been noted
 *----*/
		*currentPt = (* currentPt) -> next;
		if ( ! * currentPt){
				Nlm_gbparse_error("unexpected end of interval tokens",
					head_token, *currentPt);
						* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
		}else{
			if ( (* currentPt) -> choice != GBPARSE_INT_LEFT){
					Nlm_gbparse_error("Missing \'(\'",
						head_token, *currentPt); /* paran match  ) */
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
			}else{
				(*parenPt) ++; *currentPt = (* currentPt) -> next;
				if ( ! * currentPt){
						Nlm_gbparse_error("illegal null contents",
							head_token, *currentPt);
						* keep_rawPt = TRUE;  (* num_errPt) ++;
						goto FATAL;
				}else{
					if (  (* currentPt) -> choice == GBPARSE_INT_RIGHT){ /* paran match ( */
						Nlm_gbparse_error("Premature \')\'" ,
							head_token, *currentPt);
						* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
					}else{

						ValNodePtr last= NULL, next_loc = NULL;

						while ( ! *num_errPt && * currentPt){
							if ( (* currentPt) -> choice == GBPARSE_INT_RIGHT){
								while  ((* currentPt) -> choice == GBPARSE_INT_RIGHT){ 
									(*parenPt) --;
									*currentPt = (* currentPt) -> next;
									if ( ! *currentPt)
										break;
								}
								break;
							}
							if ( ! * currentPt){
								break;
							}
							next_loc = Nlm_gbloc_ver(keep_rawPt, parenPt,sitesPt,  
								currentPt, head_token, num_errPt,
								seq_id, accver);
							if( retval -> data.ptrvalue == NULL)
								retval -> data.ptrvalue = next_loc;
							if ( last)
								last -> next = next_loc;
							last = next_loc;
							if ( ! * currentPt){
								break;
							}
							if ( ! * currentPt){
								break;
							}
							if ((* currentPt) -> choice == GBPARSE_INT_RIGHT){
								break;
							}
							if (  (* currentPt) -> choice == GBPARSE_INT_COMMA){
								*currentPt = (* currentPt) -> next;
								if(add_nulls){
									next_loc = ValNodeNew(last);
									next_loc -> choice = SEQLOC_NULL;
									last -> next = next_loc;
									last = next_loc;
								}
							}else{
								Nlm_gbparse_error("Illegal token after interval",
									head_token, *currentPt);
								* keep_rawPt = TRUE;  (* num_errPt) ++;
								goto FATAL;
							}
						}
					}
				}
				if ( (*currentPt) == NULL){
					Nlm_gbparse_error("unexpected end of usable tokens",
						head_token, *currentPt);
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
				}else{
					if ( (* currentPt) -> choice != GBPARSE_INT_RIGHT){
							Nlm_gbparse_error("Missing \')\'" /* paran match  ) */,
									 head_token, *currentPt);
							* keep_rawPt = TRUE;  (* num_errPt) ++;
						goto FATAL;
					}else{
						(*parenPt) --; *currentPt = (* currentPt) -> next;
					}
				}
			}
		}
	}

FATAL:
	if ( (* num_errPt)){
		if (retval){
			SeqLocFree(retval); 
			retval =ValNodeNew(NULL);
			retval -> choice = SEQLOC_WHOLE;
			retval -> data.ptrvalue = SeqIdDup(seq_id);
		}
	}
	
	return retval;
}

/**********************************************************/
void Nlm_gbgap(ValNodePtr PNTR currentPt, ValNodePtr PNTR retval,
               Boolean unknown)
{
    ValNodePtr vnp_first;
    ValNodePtr vnp_second;
    ValNodePtr vnp_third;
    SeqLocPtr  vvv;

    vnp_first = (*currentPt)->next;
    if(vnp_first == NULL || vnp_first->choice != GBPARSE_INT_LEFT)
        return;

    vnp_second = vnp_first->next;
    if(vnp_second == NULL || (vnp_second->choice != GBPARSE_INT_NUMBER &&
       vnp_second->choice != GBPARSE_INT_RIGHT))
        return;

    if(vnp_second->choice == GBPARSE_INT_RIGHT)
    {
        (*retval) = ValNodeNew(*retval);
        (*retval)->choice = SEQLOC_NULL;
    }
    else
    {
        vnp_third = vnp_second->next;
        if(vnp_third == NULL || vnp_third->choice != GBPARSE_INT_RIGHT)
            return;

        vvv = GapToSeqLocEx(atoi((CharPtr) vnp_second->data.ptrvalue), unknown);
        if(vvv == NULL)
            return;

        if(*retval == NULL)
            (*retval) = vvv;
        else
        {
            (*retval)->next = vvv;
            (*retval) = (*retval)->next;
        }

        (*currentPt) = (*currentPt)->next;
    }

    (*currentPt) = (*currentPt)->next;
    (*currentPt) = (*currentPt)->next;
    (*currentPt) = (*currentPt)->next;
}

/*--------------- Nlm_gbint ()--------------------*/

NLM_EXTERN SeqLocPtr /* sometimes returns points */

Nlm_gbint(Boolean PNTR keep_rawPt, ValNodePtr PNTR currentPt, ValNodePtr head_token, int PNTR num_errPt, SeqIdPtr seq_id)
{
	SeqLocPtr retnode = ValNodeNew(NULL);
	SeqIntPtr retint = SeqIntNew();
	TextSeqIdPtr tp;
	IntFuzzPtr fuzz=NULL;
	SeqIdPtr idp = NULL;
	Boolean took_choice=FALSE;

	retnode -> choice = SEQLOC_INT;

		if ( (* currentPt) -> choice == GBPARSE_INT_ACCESION){
			idp = ValNodeNew(NULL);
			if (seq_id){
				if (
					seq_id -> choice == SEQID_GENBANK
					 || seq_id -> choice == SEQID_EMBL
					 || seq_id -> choice == SEQID_DDBJ
				){
					idp -> choice = seq_id -> choice;
					took_choice = TRUE;
				}
			}
			if (! took_choice){
				idp -> choice = SEQID_GENBANK;
			}
			tp = TextSeqIdNew();
			idp -> data.ptrvalue = tp;
			tp -> accession = StringSave ( (* currentPt) ->data.ptrvalue);
			 *currentPt  =  (* currentPt)  -> next;
			if ( !  *currentPt ){
					Nlm_gbparse_error("Nothing after accession",
						head_token, *currentPt);
					* keep_rawPt = TRUE;  (* num_errPt) ++;

					SeqIdFree(idp);
					idp = NULL;

					goto FATAL;
			}
		}else{
			idp = SeqIdDup (seq_id);	
		}
		if ( (* currentPt) -> choice == GBPARSE_INT_LT){
				fuzz = IntFuzzNew();
				fuzz -> choice = 4;
				fuzz ->a = 2;
			 *currentPt  =  (* currentPt)  -> next;
			if ( !  *currentPt ){
					Nlm_gbparse_error("Nothing after \'<\'",
						head_token, *currentPt);
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
			}
		}
		if ( ! (* num_errPt))
		switch ( (*currentPt ) -> choice){
			case  GBPARSE_INT_ACCESION :
				if ( idp){
					Nlm_gbparse_error("duplicate accessions",
						head_token, *currentPt);
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
				}
				break;
			case  GBPARSE_INT_CARET : 
					Nlm_gbparse_error("caret (^) before number" ,
						head_token, *currentPt);
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
			case  GBPARSE_INT_LT : 
				if ( idp){
					Nlm_gbparse_error("duplicate \'<\'",
						head_token, *currentPt);
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
				}
				break;
			case  GBPARSE_INT_GT :
			case  GBPARSE_INT_NUMBER :
			case  GBPARSE_INT_LEFT :

			case GBPARSE_INT_ONE_OF_NUM:

				retint -> if_from = fuzz;
				retint -> id = idp;
				retnode -> data.ptrvalue = retint;
				Nlm_gbload_number (& ( retint -> from), & (retint -> if_from),
					keep_rawPt, currentPt, head_token, 
					num_errPt,TAKE_FIRST);
				Nlm_gbcheck_range(retint -> from, idp, keep_rawPt, num_errPt, head_token, *currentPt);
		if ( ! (* num_errPt) ){
		if ( * currentPt){
				Boolean in_caret = FALSE;
		switch ( (*currentPt ) -> choice){
				SeqPntPtr point;

				default: case GBPARSE_INT_JOIN: case GBPARSE_INT_COMPL: 
				case GBPARSE_INT_SINGLE_DOT:case GBPARSE_INT_ORDER: case GBPARSE_INT_GROUP:
				case GBPARSE_INT_ACCESION:
					Nlm_gbparse_error("problem with 2nd number",
						head_token, *currentPt);;
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
				case GBPARSE_INT_COMMA: case GBPARSE_INT_RIGHT: /* valid thing to leave on*/
/*--------------but have a point, not an interval----*/
					Nlm_gbpintpnt(retnode, & retint);
					break;
				case GBPARSE_INT_GT: case GBPARSE_INT_LT:
					Nlm_gbparse_error("Missing \'..\'",
						head_token, *currentPt);;
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
				case GBPARSE_INT_CARET:
				if (retint -> if_from){
					Nlm_gbparse_error("\'<\' then \'^\'",
					head_token, *currentPt);
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
				}
				retint -> if_from = IntFuzzNew();
				retint -> if_from -> choice = 4;
				retint -> if_from ->a = 4;
				retint -> if_to = IntFuzzNew();
				retint -> if_to -> choice = 4;
				retint -> if_to ->a = 4;
				in_caret = TRUE;
/*---no break on purpose ---*/
				case GBPARSE_INT_DOT_DOT:
			 *currentPt  =  (* currentPt)  -> next;
			if ( (*currentPt) == NULL){
				Nlm_gbparse_error("unexpected end of usable tokens",
					head_token, *currentPt);
				* keep_rawPt = TRUE;  (* num_errPt) ++;
				goto FATAL;
			}
/*--no break on purpose here ---*/
				case GBPARSE_INT_NUMBER:
				case GBPARSE_INT_LEFT:

				case GBPARSE_INT_ONE_OF_NUM:  /* unlikely, but ok */

				if ( (* currentPt) -> choice == GBPARSE_INT_RIGHT){
						if (retint -> if_from){
							Nlm_gbparse_error("\'^\' then \'>\'",
								head_token, *currentPt);
							* keep_rawPt = TRUE;  (* num_errPt) ++;
							goto FATAL;
						}
				}
				Nlm_gbload_number (& ( retint -> to), & (retint -> if_to),
					keep_rawPt, currentPt, head_token, 
					num_errPt, TAKE_SECOND);
				Nlm_gbcheck_range(retint -> to, idp, keep_rawPt, num_errPt, head_token, *currentPt);
/*----------
 *  The caret location implies a place (point) between two location.
 *  This is not exactly captured by the ASN.1, but pretty close
 *-------*/
				if (in_caret){
					Int4 to = retint -> to;
	
					point = Nlm_gbpintpnt(retnode, & retint);
					if ( point -> point +1 == to){
						point -> point = to; /* was essentailly correct */
					}else{
						point -> fuzz -> choice = 2; /* range */
						point -> fuzz -> a = to; /* max */
						point -> fuzz ->b = point -> point;
					}
				}
				if (retint != NULL)
				if (retint -> from == retint -> to &&
						! retint -> if_from &&
						! retint -> if_to){
/*-------if interval really a point, make is so ----*/
					Nlm_gbpintpnt(retnode, & retint);
				}
				} /* end switch */
				}else{
					Nlm_gbpintpnt(retnode, & retint);
				}
			}else{
				goto FATAL;
			}
				break;
			default:
					Nlm_gbparse_error("No number when expected",
						head_token, *currentPt);
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
			
		}


RETURN:
		return retnode;

FATAL:
		if (retint && (* num_errPt)){
			SeqIntFree(retint);
			retint = NULL;
		}
		ValNodeFree(retnode);
		retnode = NULL;
		goto RETURN;
}

/*--------------- Nlm_gbint_ver ()--------------------*/

NLM_EXTERN SeqLocPtr /* sometimes returns points */

Nlm_gbint_ver(Boolean PNTR keep_rawPt, ValNodePtr PNTR currentPt,
          ValNodePtr head_token, int PNTR num_errPt, SeqIdPtr seq_id,
          Boolean accver)
{
	SeqLocPtr retnode = ValNodeNew(NULL);
	SeqIntPtr retint = SeqIntNew();
	TextSeqIdPtr tp;
	IntFuzzPtr fuzz=NULL;
	SeqIdPtr idp = NULL;
	Boolean took_choice=FALSE;
	CharPtr p;

	retnode -> choice = SEQLOC_INT;

		if ( (* currentPt) -> choice == GBPARSE_INT_ACCESION){
			idp = ValNodeNew(NULL);
			if (seq_id){
				if (
					seq_id -> choice == SEQID_GENBANK
					 || seq_id -> choice == SEQID_EMBL
					 || seq_id -> choice == SEQID_DDBJ
				){
					idp -> choice = seq_id -> choice;
					took_choice = TRUE;
				}
			}
			if (! took_choice){
				idp -> choice = SEQID_GENBANK;
			}
			tp = TextSeqIdNew();
			idp -> data.ptrvalue = tp;
			if(accver == FALSE)
			{
				tp->accession = StringSave((*currentPt)->data.ptrvalue);
			}
			else
			{
				p = StringChr((*currentPt)->data.ptrvalue, '.');
				if(p == NULL)
				{
					tp->accession = StringSave((*currentPt)->data.ptrvalue);
					Nlm_gbparse_error("Missing accession's version",
							  head_token, *currentPt);
				}
				else
				{
					*p = '\0';
					tp->accession = StringSave((*currentPt)->data.ptrvalue);
					tp->version = atoi(p + 1);
					*p = '.';
				}
			}
			*currentPt  =  (* currentPt)  -> next;
			if ( !  *currentPt ){
					Nlm_gbparse_error("Nothing after accession",
						head_token, *currentPt);
					* keep_rawPt = TRUE;  (* num_errPt) ++;

					SeqIdFree(idp);
					idp = NULL;

					goto FATAL;
			}
		}else{
			idp = SeqIdDup (seq_id);	
		}
		if ( (* currentPt) -> choice == GBPARSE_INT_LT){
				fuzz = IntFuzzNew();
				fuzz -> choice = 4;
				fuzz ->a = 2;
			 *currentPt  =  (* currentPt)  -> next;
			if ( !  *currentPt ){
					Nlm_gbparse_error("Nothing after \'<\'",
						head_token, *currentPt);
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
			}
		}
		if ( ! (* num_errPt))
		switch ( (*currentPt ) -> choice){
			case  GBPARSE_INT_ACCESION :
				if ( idp){
					Nlm_gbparse_error("duplicate accessions",
						head_token, *currentPt);
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
				}
				break;
			case  GBPARSE_INT_CARET : 
					Nlm_gbparse_error("caret (^) before number" ,
						head_token, *currentPt);
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
			case  GBPARSE_INT_LT : 
				if ( idp){
					Nlm_gbparse_error("duplicate \'<\'",
						head_token, *currentPt);
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
				}
				break;
			case  GBPARSE_INT_GT :
			case  GBPARSE_INT_NUMBER :
			case  GBPARSE_INT_LEFT :

			case GBPARSE_INT_ONE_OF_NUM:

				retint -> if_from = fuzz;
				retint -> id = idp;
				retnode -> data.ptrvalue = retint;
				Nlm_gbload_number (& ( retint -> from), & (retint -> if_from),
					keep_rawPt, currentPt, head_token, 
					num_errPt,TAKE_FIRST);
				Nlm_gbcheck_range(retint -> from, idp, keep_rawPt, num_errPt, head_token, *currentPt);
		if ( ! (* num_errPt) ){
		if ( * currentPt){
				Boolean in_caret = FALSE;
		switch ( (*currentPt ) -> choice){
				SeqPntPtr point;

				default: case GBPARSE_INT_JOIN: case GBPARSE_INT_COMPL: 
				case GBPARSE_INT_SINGLE_DOT:case GBPARSE_INT_ORDER: case GBPARSE_INT_GROUP:
				case GBPARSE_INT_ACCESION:
					Nlm_gbparse_error("problem with 2nd number",
						head_token, *currentPt);;
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
				case GBPARSE_INT_COMMA: case GBPARSE_INT_RIGHT: /* valid thing to leave on*/
/*--------------but have a point, not an interval----*/
					Nlm_gbpintpnt(retnode, & retint);
					break;
				case GBPARSE_INT_GT: case GBPARSE_INT_LT:
					Nlm_gbparse_error("Missing \'..\'",
						head_token, *currentPt);;
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
				case GBPARSE_INT_CARET:
				if (retint -> if_from){
					Nlm_gbparse_error("\'<\' then \'^\'",
					head_token, *currentPt);
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
				}
				retint -> if_from = IntFuzzNew();
				retint -> if_from -> choice = 4;
				retint -> if_from ->a = 4;
				retint -> if_to = IntFuzzNew();
				retint -> if_to -> choice = 4;
				retint -> if_to ->a = 4;
				in_caret = TRUE;
/*---no break on purpose ---*/
				case GBPARSE_INT_DOT_DOT:
			 *currentPt  =  (* currentPt)  -> next;
			if ( (*currentPt) == NULL){
				Nlm_gbparse_error("unexpected end of usable tokens",
					head_token, *currentPt);
				* keep_rawPt = TRUE;  (* num_errPt) ++;
				goto FATAL;
			}
/*--no break on purpose here ---*/
				case GBPARSE_INT_NUMBER:
				case GBPARSE_INT_LEFT:

				case GBPARSE_INT_ONE_OF_NUM:  /* unlikely, but ok */

				if ( (* currentPt) -> choice == GBPARSE_INT_RIGHT){
						if (retint -> if_from){
							Nlm_gbparse_error("\'^\' then \'>\'",
								head_token, *currentPt);
							* keep_rawPt = TRUE;  (* num_errPt) ++;
							goto FATAL;
						}
				}
				Nlm_gbload_number (& ( retint -> to), & (retint -> if_to),
					keep_rawPt, currentPt, head_token, 
					num_errPt, TAKE_SECOND);
				Nlm_gbcheck_range(retint -> to, idp, keep_rawPt, num_errPt, head_token, *currentPt);
/*----------
 *  The caret location implies a place (point) between two location.
 *  This is not exactly captured by the ASN.1, but pretty close
 *-------*/
				if (in_caret){
					Int4 to = retint -> to;
	
					point = Nlm_gbpintpnt(retnode, & retint);
					if ( point -> point +1 == to){
						point -> point = to; /* was essentailly correct */
					}else{
						point -> fuzz -> choice = 2; /* range */
						point -> fuzz -> a = to; /* max */
						point -> fuzz ->b = point -> point;
					}
				}
				if (retint != NULL)
				if (retint -> from == retint -> to &&
						! retint -> if_from &&
						! retint -> if_to){
/*-------if interval really a point, make is so ----*/
					Nlm_gbpintpnt(retnode, & retint);
				}
				} /* end switch */
				}else{
					Nlm_gbpintpnt(retnode, & retint);
				}
			}else{
				goto FATAL;
			}
				break;
			default:
					Nlm_gbparse_error("No number when expected",
						head_token, *currentPt);
					* keep_rawPt = TRUE;  (* num_errPt) ++;
					goto FATAL;
			
		}


RETURN:
		return retnode;

FATAL:
		if (retint && (* num_errPt)){
			SeqIntFree(retint);
			retint = NULL;
		}
		ValNodeFree(retnode);
		retnode = NULL;
		goto RETURN;
}

/*------------------- Nlm_gbpintpnt()-----------*/

NLM_EXTERN SeqPntPtr
Nlm_gbpintpnt(SeqLocPtr retnode, SeqIntPtr PNTR retintPt)
{
	SeqPntPtr point;
		point = SeqPntNew();
		point -> point = (*retintPt) -> from;
		point -> id = (*retintPt) -> id;
		(*retintPt) -> id = NULL;
		point -> fuzz = (*retintPt) -> if_from;
		(*retintPt) -> if_from = NULL;
		SeqIntFree((*retintPt));
		(*retintPt) = NULL;
		retnode -> choice = SEQLOC_PNT;
		retnode -> data.ptrvalue = point;
	return point;
}

/*----- Nlm_gbload_number() -----*/

NLM_EXTERN void
Nlm_gbload_number (Int4 PNTR numPt, IntFuzzPtr PNTR fuzzPt, Boolean PNTR keep_rawPt, ValNodePtr PNTR currentPt, ValNodePtr head_token, int PNTR num_errPt, int take_which)
{
	int num_found=0;
	int fuzz_err =0;
	Boolean strange_sin_dot = FALSE;

		if ((*currentPt ) -> choice == GBPARSE_INT_CARET){
			Nlm_gbparse_error("duplicate carets",
					head_token, *currentPt);
			(*keep_rawPt) = TRUE; (*num_errPt) ++;
			 *currentPt  =  (* currentPt)  -> next;
			fuzz_err = 1;
		}else if ((*currentPt ) -> choice == GBPARSE_INT_GT ||
				(*currentPt ) -> choice == GBPARSE_INT_LT){
			if ( ! * fuzzPt){
				* fuzzPt = IntFuzzNew();
			}
			(* fuzzPt) -> choice = 4;
			if ((*currentPt ) -> choice == GBPARSE_INT_GT ){
				(* fuzzPt) -> a = 1; /* 'a' serves as "lim" for choice 4 */
			}else{
				(* fuzzPt) -> a = 2;
			}
			 *currentPt  =  (* currentPt)  -> next;
		}else if ((*currentPt ) -> choice == GBPARSE_INT_LEFT){
			strange_sin_dot = TRUE;
			 *currentPt  =  (* currentPt)  -> next;
				if ((*currentPt ) -> choice == GBPARSE_INT_NUMBER){
					if ( ! * fuzzPt){
						* fuzzPt = IntFuzzNew();
					}
					(* fuzzPt)  -> b  = atoi((*currentPt ) -> data.ptrvalue)-1;
					(* fuzzPt) -> choice = 2;
					if ( take_which == TAKE_FIRST ){
						* numPt = (* fuzzPt)  -> b;
					}
					 *currentPt  =  (* currentPt)  -> next;
					num_found=1;
				}else{
					fuzz_err =1;
				}
				if ((*currentPt ) -> choice != GBPARSE_INT_SINGLE_DOT ){
					fuzz_err =1;
				}else{
					 *currentPt  =  (* currentPt)  -> next;
					if ((*currentPt ) -> choice == GBPARSE_INT_NUMBER){
						(* fuzzPt)  -> a  = atoi((*currentPt ) -> data.ptrvalue)-1;
						if ( take_which ==  TAKE_SECOND ){
							* numPt = (* fuzzPt)  -> a;
						}
						 *currentPt  =  (* currentPt)  -> next;
					}else{
						fuzz_err =1;
					}
					if ((*currentPt ) -> choice == GBPARSE_INT_RIGHT){
						 *currentPt  =  (* currentPt)  -> next;
					}else{
						fuzz_err =1;
					}
				}
					
		}else if ((*currentPt ) -> choice != GBPARSE_INT_NUMBER) {
/* this prevents endless cycling, unconditionally */
		if ((*currentPt ) -> choice != GBPARSE_INT_ONE_OF 
			&& (*currentPt ) -> choice !=  GBPARSE_INT_ONE_OF_NUM)
			 *currentPt  =  (* currentPt)  -> next;
			num_found = -1;
		}

		if ( ! strange_sin_dot){
			if ( ! * currentPt){
					Nlm_gbparse_error("unexpected end of interval tokens",
						head_token, *currentPt);
							* keep_rawPt = TRUE;  (* num_errPt) ++;
			}else{
				 if ((*currentPt ) -> choice == GBPARSE_INT_NUMBER){
					* numPt = atoi((*currentPt ) -> data.ptrvalue)-1;
					 *currentPt  =  (* currentPt)  -> next;
					num_found=1;
				}
			}
		}

	if ( fuzz_err){
					Nlm_gbparse_error("Incorrect uncertainty",
						head_token, *currentPt);
					(*keep_rawPt) = TRUE; (*num_errPt) ++;
	}
	if ( num_found != 1){
					(*keep_rawPt) = TRUE; 
/****************
 *
 *  10..one-of(13,15) type syntax here
 *
 ***************/
		if ((*currentPt ) -> choice == GBPARSE_INT_ONE_OF 
				|| (*currentPt ) -> choice == GBPARSE_INT_ONE_OF_NUM){
			Boolean one_of_ok = TRUE;
			Boolean at_end_one_of = FALSE;

			*currentPt  =  (* currentPt)  -> next;
			if ((*currentPt ) -> choice != GBPARSE_INT_LEFT){
				one_of_ok = FALSE;
			}else{
				*currentPt  =  (* currentPt)  -> next;
			}
			if (one_of_ok &&  (*currentPt ) -> choice == GBPARSE_INT_NUMBER){
					* numPt = atoi((*currentPt ) -> data.ptrvalue)-1;
					 *currentPt  =  (* currentPt)  -> next;
			}else{
				one_of_ok = FALSE;
			}
			while  (one_of_ok && ! at_end_one_of &&  *currentPt != NULL){
				switch ( (*currentPt ) -> choice){
					default:
						one_of_ok = FALSE;
						break;
					case GBPARSE_INT_COMMA:
					case GBPARSE_INT_NUMBER:
					 *currentPt  =  (* currentPt)  -> next;
					break;
					case GBPARSE_INT_RIGHT:
					 *currentPt  =  (* currentPt)  -> next;
					at_end_one_of = TRUE;
					break;
				}
			}
			if ( ! one_of_ok && ! at_end_one_of){
				while (! at_end_one_of && *currentPt != NULL){
					if ((*currentPt ) -> choice == GBPARSE_INT_RIGHT){
						at_end_one_of = TRUE;
					}
				 *currentPt  =  (* currentPt)  -> next;
				}
			}

			if ( ! one_of_ok){
				Nlm_gbparse_error("bad one-of() syntax as number",
					head_token, *currentPt);
				(*num_errPt) ++;
			}
		}else{
			Nlm_gbparse_error("Number not found when expected",
				head_token, *currentPt);
			(*num_errPt) ++;
		}
	}
}

/*----------------- Nlm_gbparse_better_be_done()-------------*/
NLM_EXTERN SeqLocPtr
Nlm_gbparse_better_be_done(int PNTR num_errsPt, ValNodePtr current_token, ValNodePtr head_token, SeqLocPtr ret_so_far, Boolean PNTR keep_rawPt, int paren_count)
{
	SeqLocPtr retval = ret_so_far;

		if ( current_token)
		while (current_token -> choice == GBPARSE_INT_RIGHT){
			paren_count --; 
			current_token =  current_token -> next;
			if ( ! current_token){
				if ( paren_count){
					char par_msg[40];
					sprintf(par_msg, "mismatched parentheses (%d)", paren_count);
					Nlm_gbparse_error(par_msg,
					head_token, current_token);
					*keep_rawPt = TRUE;
					(*num_errsPt) ++;
				}
				break;
			}
		}
		if ( paren_count){
					Nlm_gbparse_error("text after last legal right parenthesis",
					head_token, current_token);
					*keep_rawPt = TRUE;
					(*num_errsPt) ++;
		}

		if (current_token){
			Nlm_gbparse_error("text after end",
					head_token, current_token);
			*keep_rawPt = TRUE;
			(*num_errsPt) ++;
		}
	return retval;
}

/*-------- Nlm_gbreplace() --------*/

NLM_EXTERN SeqLocPtr 
Nlm_gbreplace (Boolean PNTR keep_rawPt, int PNTR parenPt, Boolean PNTR sitesPt, ValNodePtr PNTR currentPt, ValNodePtr head_token, int PNTR num_errPt, SeqIdPtr seq_id)
{
	SeqLocPtr retval = NULL;

		* keep_rawPt = TRUE;
		 *currentPt  =  (* currentPt)  -> next;

		if ((*currentPt ) -> choice == GBPARSE_INT_LEFT){
			 *currentPt  =  (* currentPt)  -> next;
			retval = Nlm_gbloc (keep_rawPt, parenPt, sitesPt, currentPt, head_token, 
				num_errPt,seq_id);
			if ( ! * currentPt){
					Nlm_gbparse_error("unexpected end of interval tokens",
						head_token, *currentPt);
							* keep_rawPt = TRUE;  (* num_errPt) ++;
			}else{
				
				if ((*currentPt ) -> choice != GBPARSE_INT_COMMA){
						Nlm_gbparse_error("Missing comma after first location in replace",
						head_token, *currentPt);
						(* num_errPt) ++;
				}
			}
		}else{
					Nlm_gbparse_error("Missing \'(\'" /* paran match  ) */
						, head_token, *currentPt);
					(* num_errPt) ++;
		}
	return retval;
}



/*-------- Nlm_gbreplace_ver() --------*/

NLM_EXTERN SeqLocPtr Nlm_gbreplace_ver(Boolean PNTR keep_rawPt, int PNTR parenPt,
                                   Boolean PNTR sitesPt,
                                   ValNodePtr PNTR currentPt,
                                   ValNodePtr head_token, int PNTR num_errPt,
                                   SeqIdPtr seq_id, Boolean accver)
{
	SeqLocPtr retval = NULL;

		* keep_rawPt = TRUE;
		 *currentPt  =  (* currentPt)  -> next;

		if ((*currentPt ) -> choice == GBPARSE_INT_LEFT){
			 *currentPt  =  (* currentPt)  -> next;
			retval = Nlm_gbloc_ver (keep_rawPt, parenPt, sitesPt, currentPt, head_token, 
				num_errPt,seq_id, accver);
			if ( ! * currentPt){
					Nlm_gbparse_error("unexpected end of interval tokens",
						head_token, *currentPt);
							* keep_rawPt = TRUE;  (* num_errPt) ++;
			}else{
				
				if ((*currentPt ) -> choice != GBPARSE_INT_COMMA){
						Nlm_gbparse_error("Missing comma after first location in replace",
						head_token, *currentPt);
						(* num_errPt) ++;
				}
			}
		}else{
					Nlm_gbparse_error("Missing \'(\'" /* paran match  ) */
						, head_token, *currentPt);
					(* num_errPt) ++;
		}
	return retval;
}

/**********************************************************/
static int Nlm_gbparse_accprefix(CharPtr acc)
{
    CharPtr p;
    int     ret;

    if(acc == NULL || *acc == '\0')
        return(0);

    for(p = acc; IS_ALPHA(*p) != 0;)
        p++;
    ret = p - acc;
    if(*p == '_')
    {
        if(ret == 2)
        {
            for(p++; IS_ALPHA(*p) != 0;)
                p++;
            ret = p - acc;
            if(ret != 3 && ret != 7)
                ret = 1;
        }
        else
            ret = 1;
    }
    else if(ret != 1 && ret != 2 && ret != 4)
        ret = 1;
    return(ret);
}


char Saved_ch;

#define Nlm_lex_error_MACRO(msg)\
		if (*current_col){\
		Saved_ch = *(current_col +1);\
		*(current_col +1) = '\0';\
		}else{\
		Saved_ch='\0';\
		}\
		Nlm_gbparse_error(msg, & forerrmacro, & forerrmacro);\
		if (Saved_ch)\
		*(current_col +1) = Saved_ch;

/*------------- gbparselex()-----------------------*/

NLM_EXTERN int
Nlm_gbparselex(CharPtr linein, ValNodePtr PNTR lexed)
{	
	CharPtr current_col=0, points_at_term_null,spare, line_use = NULL;
	int dex;
	int retval = 0, len;
	ValNodePtr current_token = NULL, last_token = NULL;
	Boolean skip_new_token=FALSE;
	Boolean die_now=FALSE;
	ValNode forerrmacro;


	if (linein == NULL) return retval;

	forerrmacro.choice =GBPARSE_INT_ACCESION ;

	if (*linein){
		len = StringLen(linein);
		line_use = MemNew(len + 1);
		StringCpy(line_use, linein);
		if ( * lexed){
				Nlm_lex_error_MACRO( "Lex list not cleared on entry to Nlm_gbparselex")
			ValNodeFree( * lexed);
			* lexed = NULL;
		}
		current_col = line_use ;
		forerrmacro.data.ptrvalue = line_use;
/*---------
 *   Clear terminal white space
 *---------*/
		points_at_term_null = line_use + len;
		spare = points_at_term_null - 1;
		while (*spare == ' '  || *spare == '\n' || *spare == '\r' || *spare == '~') {
			*spare-- = '\0';
			points_at_term_null --;
		}


		while (current_col < points_at_term_null && ! die_now) {
			if ( ! skip_new_token){
				last_token = current_token;
				current_token = ValNodeNew(current_token);
				if ( ! * lexed)
					* lexed = current_token;
			}
			switch ( *current_col){

			case '\"':
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_STRING;
				for (spare = current_col +1; spare < points_at_term_null; 
						spare ++) {
					if ( *spare == '\"'){
						break;
					}
				}
				if (spare >= points_at_term_null){
						Nlm_lex_error_MACRO( "unterminated string")
						retval ++;
				}else{
					len = spare-current_col + 1;
					current_token -> data.ptrvalue = 
						MemNew(len +2);
					StringNCpy(current_token -> data.ptrvalue,
						current_col,len);
					current_col += len;
				}
					break;
/*------
 *  NUMBER
 *------*/
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_NUMBER;
				for (dex=0, spare = current_col; isdigit((int) *spare); spare ++){
					dex ++ ;
				}
				current_token -> data.ptrvalue = MemNew(dex+1);
				StringNCpy(current_token -> data.ptrvalue, current_col, dex);
				current_col += dex -1;
				break;
/*------
 *  JOIN
 *------*/
			case 'j':
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_JOIN;
				if (StringNCmp(current_col,"join",(unsigned) 4)!=0){
					Nlm_lex_error_MACRO( "\"join\" misspelled")
					retval += 10;
					for(;*current_col && *current_col != '('; current_col++)
						; /* vi match )   empty body*/ 
					current_col -- ;  /* back up 'cause ++ follows */
				}else{
					current_col += 3;
				}
				break;
			
/*------
 *  ORDER and ONE-OF
 *------*/
			case 'o':
				skip_new_token = FALSE;
				if (StringNCmp(current_col,"order",(unsigned) 5)!=0){
					if (StringNCmp(current_col,"one-of",(unsigned) 6)!=0){
					Nlm_lex_error_MACRO( "\"order\" or \"one-of\" misspelled")
						retval ++;
						for(;*current_col && *current_col != '('; current_col++)
							; /* vi match )   empty body*/ 
						current_col -- ;  /* back up 'cause ++ follows */
					}else{
						current_token -> choice = GBPARSE_INT_ONE_OF ;
						current_col += 5;
					}
				}else{
					current_token -> choice = GBPARSE_INT_ORDER;
					current_col += 4;
				}
				break;

/*------
 *  REPLACE
 *------*/
			case 'r' :
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_REPLACE ;
				if (StringNCmp(current_col,"replace",(unsigned) 6)!=0){
					Nlm_lex_error_MACRO( "\"replace\" misspelled")
					retval ++;
					for(;*current_col && *current_col != '('; current_col++)
						; /* vi match )   empty body*/ 
					current_col -- ;  /* back up 'cause ++ follows */
				}else{
					current_col += 6;
				}
				break;
			
/*------
 *  GAP or GROUP or GI
 *------*/
			case 'g':
				skip_new_token = FALSE;
				if(StringNCmp(current_col, "gap", 3) == 0 &&
				   (current_col[3] == '(' ||
				    current_col[3] == ' ' ||
				    current_col[3] == '\t' ||
				    current_col[3] == '\0'))
				{
				    current_token->choice = GBPARSE_INT_GAP;
				    current_token->data.ptrvalue = MemNew(4);
				    StringCpy(current_token->data.ptrvalue, "gap");
				    if(StringNICmp(current_col + 3, "(unk", 4) == 0)
				    {
					current_token->choice = GBPARSE_INT_UNK_GAP;
					last_token = current_token;
					current_token = ValNodeNew(current_token);
					current_token->choice = GBPARSE_INT_LEFT;
					current_col += 4;
				    }
				    current_col += 2;
				    break;
				}
				if(StringNCmp(current_col, "gi|", 3) == 0) {
				    current_token->choice = GBPARSE_INT_ACCESION;
				    current_col += 3;
					for (; IS_DIGIT(*current_col); current_col++) ;
				    break;
				}
				current_token -> choice = GBPARSE_INT_GROUP;
				if (StringNCmp(current_col,"group",(unsigned) 5)!=0){
					Nlm_lex_error_MACRO("\"group\" misspelled")
					retval ++;
					for(;*current_col && *current_col != '('; current_col++)
						; /* vi match )   empty body*/ 
					current_col -- ;  /* back up 'cause ++ follows */
				}else{
					current_col += 4;
				}
				break;
			
/*------
 *  COMPLEMENT
 *------*/
			case 'c':
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_COMPL;
				if (StringNCmp(current_col,"complement",(unsigned) 10)!=0){
					Nlm_lex_error_MACRO("\"complement\" misspelled")
					retval += 10;
					for(;*current_col && *current_col != '('; current_col++)
						; /* vi match )   empty body*/ 
					current_col -- ;  /* back up 'cause ++ follows */
				}else{
					current_col += 9;
				}
				break;

/*-------
 * internal bases ignored
 *---------*/
			case 'b':
			if (StringNCmp(current_col,"bases",(unsigned) 5)!=0){
				goto ACCESSION;
			}else{
				skip_new_token = TRUE;
				current_col += 4;
			}
				break;

/*------
 *  ()^.,<>  (bases (sites
 *------*/
			case '(':
				if (StringNCmp(current_col,"(base",(unsigned) 5)==0){
					skip_new_token = FALSE;
					current_token -> choice = GBPARSE_INT_JOIN;
					current_col += 4;
					if (*current_col != '\0')
					if ( * (current_col +1) == 's')
						current_col ++;
					last_token = current_token;
					current_token = ValNodeNew(current_token);
					current_token -> choice = GBPARSE_INT_LEFT;
				}else if (StringNCmp(current_col,"(sites",(unsigned) 5)==0){
					skip_new_token = FALSE;
					current_col += 5;
					if (*current_col != '\0')
					if ( * (current_col +1) == ')'){
						current_col ++;
						current_token -> choice = GBPARSE_INT_SITES;
					}else{
						current_token -> choice = GBPARSE_INT_SITES;
						last_token = current_token;
						current_token = ValNodeNew(current_token);
						current_token -> choice = GBPARSE_INT_JOIN;
						last_token = current_token;
						current_token = ValNodeNew(current_token);
						current_token -> choice = GBPARSE_INT_LEFT;
						if (*current_col != '\0'){
							if ( * (current_col +1) == ';'){
								current_col ++;
							}else if (StringNCmp(current_col +1," ;", (unsigned) 2) ==0){
								current_col += 2;
							}
						}
					}
				}else{
					skip_new_token = FALSE;
					current_token -> choice = GBPARSE_INT_LEFT;
				}
				break;

			case ')':
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_RIGHT;
			
				break;

			case '^':
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_CARET;
				break;

                        case '-':
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_DOT_DOT ;
				break;
			case '.':
				skip_new_token = FALSE;
				if (StringNCmp(current_col,"..",(unsigned) 2)!=0){
					current_token -> choice = GBPARSE_INT_SINGLE_DOT ;
				}else{
					current_token -> choice = GBPARSE_INT_DOT_DOT;
					current_col ++ ;
				}
				break;

			case '>':
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_GT;
				break;

			case '<':
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_LT;

				break;

			case ';':
			case ',':
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_COMMA;
				break;

			case ' ': case '\t': case '\n': case '\r': case '~':
				skip_new_token = TRUE;
				break;
	
			case 't' :
			if (StringNCmp(current_col,"to",(unsigned) 2)!=0){
				goto ACCESSION;
			}else{
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_DOT_DOT;
				current_col ++ ;
				break;
			}
				
			case 's' :
			if (StringNCmp(current_col,"site",(unsigned) 4)!=0){
				goto ACCESSION;
			}else{
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_SITES;
				current_col += 3 ;
				if (*current_col != '\0')
				if ( * (current_col +1) == 's')
					current_col ++;
				if (*current_col != '\0'){
					if ( * (current_col +1) == ';'){
						current_col ++;
					}else if (StringNCmp(current_col +1," ;", (unsigned) 2) ==0){
						current_col += 2;
					}
				}
				break;
			}
				

 ACCESSION:
			default:
	/*-------
 * all GenBank accessions start with a capital letter
 * and then have numbers
	------*/
/* new accessions start with 2 capital letters !!  1997 */
/* new accessions have .version !!  2/15/1999 */
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_ACCESION;
				dex = Nlm_gbparse_accprefix(current_col);
				spare = current_col + dex;
				for (; isdigit((int) *spare); spare ++){
					dex ++ ;
				}
				if (*spare == '.') {
					dex ++ ;
					for (spare++; isdigit((int) *spare); spare ++){
						dex ++ ;
					}
				}
				if (*spare != ':'){
					Nlm_lex_error_MACRO( "ACCESSION missing \":\"" )
					retval += 10;
					current_col --;
				}
				current_token -> data.ptrvalue = MemNew(dex+1);
				StringNCpy(current_token -> data.ptrvalue, current_col, dex);
				current_col += dex ;


		}
	/*--move to past last "good" character---*/
				current_col ++;
			}
			if ( ! * lexed && current_token){
				* lexed = current_token;
			}
			if (skip_new_token && current_token) { 
/*---------
 *   last node points to a null (blank or white space token)
 *-----------*/
				if (last_token){
					last_token -> next = NULL;
				}else{
					* lexed = NULL;
				}
				ValNodeFree(current_token);
			}
	}
	if ( line_use)
		MemFree(line_use);

	return retval;
}					


/*------------- Nlm_gbparselex_ver() -----------------------*/

NLM_EXTERN int
Nlm_gbparselex_ver(CharPtr linein, ValNodePtr PNTR lexed, Boolean accver)
{	
	CharPtr current_col=0, points_at_term_null,spare, line_use = NULL;
	int dex;
	int retval = 0, len;
	ValNodePtr current_token = NULL, last_token = NULL;
	Boolean skip_new_token=FALSE;
	Boolean die_now=FALSE;
	ValNode forerrmacro;

	forerrmacro.choice =GBPARSE_INT_ACCESION ;

	if (*linein	){
		len = StringLen(linein);
		line_use = MemNew(len + 1);
		StringCpy(line_use, linein);
		if ( * lexed){
				Nlm_lex_error_MACRO( "Lex list not cleared on entry to Nlm_gbparselex_ver")
			ValNodeFree( * lexed);
			* lexed = NULL;
		}
		current_col = line_use ;
		forerrmacro.data.ptrvalue = line_use;
/*---------
 *   Clear terminal white space
 *---------*/
		points_at_term_null = line_use + len;
		spare = points_at_term_null - 1;
		while (*spare == ' '  || *spare == '\n' || *spare == '\r' || *spare == '~') {
			*spare-- = '\0';
			points_at_term_null --;
		}


		while (current_col < points_at_term_null && ! die_now) {
			if ( ! skip_new_token){
				last_token = current_token;
				current_token = ValNodeNew(current_token);
				if ( ! * lexed)
					* lexed = current_token;
			}
			switch ( *current_col){

			case '\"':
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_STRING;
				for (spare = current_col +1; spare < points_at_term_null; 
						spare ++) {
					if ( *spare == '\"'){
						break;
					}
				}
				if (spare >= points_at_term_null){
						Nlm_lex_error_MACRO( "unterminated string")
						retval ++;
				}else{
					len = spare-current_col + 1;
					current_token -> data.ptrvalue = 
						MemNew(len +2);
					StringNCpy(current_token -> data.ptrvalue,
						current_col,len);
					current_col += len;
				}
					break;
/*------
 *  NUMBER
 *------*/
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_NUMBER;
				for (dex=0, spare = current_col; isdigit((int) *spare); spare ++){
					dex ++ ;
				}
				current_token -> data.ptrvalue = MemNew(dex+1);
				StringNCpy(current_token -> data.ptrvalue, current_col, dex);
				current_col += dex -1;
				break;
/*------
 *  JOIN
 *------*/
			case 'j':
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_JOIN;
				if (StringNCmp(current_col,"join",(unsigned) 4)!=0){
					Nlm_lex_error_MACRO( "\"join\" misspelled")
					retval += 10;
					for(;*current_col && *current_col != '('; current_col++)
						; /* vi match )   empty body*/ 
					current_col -- ;  /* back up 'cause ++ follows */
				}else{
					current_col += 3;
				}
				break;
			
/*------
 *  ORDER and ONE-OF
 *------*/
			case 'o':
				skip_new_token = FALSE;
				if (StringNCmp(current_col,"order",(unsigned) 5)!=0){
					if (StringNCmp(current_col,"one-of",(unsigned) 6)!=0){
					Nlm_lex_error_MACRO( "\"order\" or \"one-of\" misspelled")
						retval ++;
						for(;*current_col && *current_col != '('; current_col++)
							; /* vi match )   empty body*/ 
						current_col -- ;  /* back up 'cause ++ follows */
					}else{
						current_token -> choice = GBPARSE_INT_ONE_OF ;
						current_col += 5;
					}
				}else{
					current_token -> choice = GBPARSE_INT_ORDER;
					current_col += 4;
				}
				break;

/*------
 *  REPLACE
 *------*/
			case 'r' :
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_REPLACE ;
				if (StringNCmp(current_col,"replace",(unsigned) 6)!=0){
					Nlm_lex_error_MACRO( "\"replace\" misspelled")
					retval ++;
					for(;*current_col && *current_col != '('; current_col++)
						; /* vi match )   empty body*/ 
					current_col -- ;  /* back up 'cause ++ follows */
				}else{
					current_col += 6;
				}
				break;
			
/*------
 *  GAP or GROUP or GI
 *------*/
			case 'g':
				skip_new_token = FALSE;
				if(StringNCmp(current_col, "gap", 3) == 0 &&
				   (current_col[3] == '(' ||
				    current_col[3] == ' ' ||
				    current_col[3] == '\t' ||
				    current_col[3] == '\0'))
				{
				    current_token->choice = GBPARSE_INT_GAP;
				    current_token->data.ptrvalue = MemNew(4);
				    StringCpy(current_token->data.ptrvalue, "gap");
				    if(StringNICmp(current_col + 3, "(unk", 4) == 0)
				    {
					current_token->choice = GBPARSE_INT_UNK_GAP;
					last_token = current_token;
					current_token = ValNodeNew(current_token);
					current_token->choice = GBPARSE_INT_LEFT;
					current_col += 4;
				    }
				    current_col += 2;
				    break;
				}
				if(StringNCmp(current_col, "gi|", 3) == 0) {
				    current_token->choice = GBPARSE_INT_ACCESION;
				    current_col += 3;
					for (; IS_DIGIT(*current_col); current_col++) ;
				    break;
				}
				current_token -> choice = GBPARSE_INT_GROUP;
				if (StringNCmp(current_col,"group",(unsigned) 5)!=0){
					Nlm_lex_error_MACRO("\"group\" misspelled")
					retval ++;
					for(;*current_col && *current_col != '('; current_col++)
						; /* vi match )   empty body*/ 
					current_col -- ;  /* back up 'cause ++ follows */
				}else{
					current_col += 4;
				}
				break;
			
/*------
 *  COMPLEMENT
 *------*/
			case 'c':
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_COMPL;
				if (StringNCmp(current_col,"complement",(unsigned) 10)!=0){
					Nlm_lex_error_MACRO("\"complement\" misspelled")
					retval += 10;
					for(;*current_col && *current_col != '('; current_col++)
						; /* vi match )   empty body*/ 
					current_col -- ;  /* back up 'cause ++ follows */
				}else{
					current_col += 9;
				}
				break;

/*-------
 * internal bases ignored
 *---------*/
			case 'b':
			if (StringNCmp(current_col,"bases",(unsigned) 5)!=0){
				goto ACCESSION;
			}else{
				skip_new_token = TRUE;
				current_col += 4;
			}
				break;

/*------
 *  ()^.,<>  (bases (sites
 *------*/
			case '(':
				if (StringNCmp(current_col,"(base",(unsigned) 5)==0){
					skip_new_token = FALSE;
					current_token -> choice = GBPARSE_INT_JOIN;
					current_col += 4;
					if (*current_col != '\0')
					if ( * (current_col +1) == 's')
						current_col ++;
					last_token = current_token;
					current_token = ValNodeNew(current_token);
					current_token -> choice = GBPARSE_INT_LEFT;
				}else if (StringNCmp(current_col,"(sites",(unsigned) 5)==0){
					skip_new_token = FALSE;
					current_col += 5;
					if (*current_col != '\0')
					if ( * (current_col +1) == ')'){
						current_col ++;
						current_token -> choice = GBPARSE_INT_SITES;
					}else{
						current_token -> choice = GBPARSE_INT_SITES;
						last_token = current_token;
						current_token = ValNodeNew(current_token);
						current_token -> choice = GBPARSE_INT_JOIN;
						last_token = current_token;
						current_token = ValNodeNew(current_token);
						current_token -> choice = GBPARSE_INT_LEFT;
						if (*current_col != '\0'){
							if ( * (current_col +1) == ';'){
								current_col ++;
							}else if (StringNCmp(current_col +1," ;", (unsigned) 2) ==0){
								current_col += 2;
							}
						}
					}
				}else{
					skip_new_token = FALSE;
					current_token -> choice = GBPARSE_INT_LEFT;
				}
				break;

			case ')':
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_RIGHT;
			
				break;

			case '^':
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_CARET;
				break;

                        case '-':
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_DOT_DOT ;
				break;
			case '.':
				skip_new_token = FALSE;
				if (StringNCmp(current_col,"..",(unsigned) 2)!=0){
					current_token -> choice = GBPARSE_INT_SINGLE_DOT ;
				}else{
					current_token -> choice = GBPARSE_INT_DOT_DOT;
					current_col ++ ;
				}
				break;

			case '>':
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_GT;
				break;

			case '<':
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_LT;

				break;

			case ';':
			case ',':
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_COMMA;
				break;

			case ' ': case '\t': case '\n': case '\r': case '~':
				skip_new_token = TRUE;
				break;
	
			case 't' :
			if (StringNCmp(current_col,"to",(unsigned) 2)!=0){
				goto ACCESSION;
			}else{
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_DOT_DOT;
				current_col ++ ;
				break;
			}
				
			case 's' :
			if (StringNCmp(current_col,"site",(unsigned) 4)!=0){
				goto ACCESSION;
			}else{
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_SITES;
				current_col += 3 ;
				if (*current_col != '\0')
				if ( * (current_col +1) == 's')
					current_col ++;
				if (*current_col != '\0'){
					if ( * (current_col +1) == ';'){
						current_col ++;
					}else if (StringNCmp(current_col +1," ;", (unsigned) 2) ==0){
						current_col += 2;
					}
				}
				break;
			}
				

 ACCESSION:
			default:
	/*-------
 * all GenBank accessions start with a capital letter
 * and then have numbers
	------*/
/* new accessions start with 2 capital letters !!  1997 */
/* new accessions have .version !!  2/15/1999 */
				skip_new_token = FALSE;
				current_token -> choice = GBPARSE_INT_ACCESION;
				dex = Nlm_gbparse_accprefix(current_col);
				spare = current_col + dex;
				for (; isdigit((int) *spare); spare ++){
					dex ++ ;
				}
				if (accver != FALSE && *spare == '.') {
					dex ++ ;
					for (spare++; isdigit((int) *spare); spare ++){
						dex ++ ;
					}
				}
				if (*spare != ':'){
					Nlm_lex_error_MACRO( "ACCESSION missing \":\"" )
					retval += 10;
					current_col --;
				}
				current_token -> data.ptrvalue = MemNew(dex+1);
				StringNCpy(current_token -> data.ptrvalue, current_col, dex);
				current_col += dex ;


		}
	/*--move to past last "good" character---*/
				current_col ++;
			}
			if ( ! * lexed && current_token){
				* lexed = current_token;
			}
			if (skip_new_token && current_token) { 
/*---------
 *   last node points to a null (blank or white space token)
 *-----------*/
				if (last_token){
					last_token -> next = NULL;
				}else{
					* lexed = NULL;
				}
				ValNodeFree(current_token);
			}
	}
	if ( line_use)
		MemFree(line_use);

	return retval;
}					


/*---- non_white()----*/

NLM_EXTERN CharPtr
Nlm_non_white(CharPtr ch)
{
   while (isspace((int) *++ch))if (! *ch) break;
      ;
   return ch;
}

/*------ gbparse_lexfree()-------*/

NLM_EXTERN ValNodePtr
Nlm_gbparse_lexfree(ValNodePtr anp)
{
	ValNodePtr next;

   while (anp != NULL)
   {
      next = anp->next;
			if ( anp -> choice == GBPARSE_INT_NUMBER || 
					anp -> choice == GBPARSE_INT_ACCESION){
				MemFree(anp->data.ptrvalue);
			}
			MemFree(anp);
      anp = next;
   }

	return NULL;
}
