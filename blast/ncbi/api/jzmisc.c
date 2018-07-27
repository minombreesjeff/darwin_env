
/*   jzmisc.c
*
* File Name:  jzmisc.c
*
* Author: Jinghui Zhang
*
* Version Creation Date:   2/10/93
*
* File Description:
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 11/2/93		A suite of general functions used in pup and
*			chromoscope
*
* ==========================================================================
*/


#include <jzmisc.h>
#include <edutil.h>
#include <gather.h>
/*************************************************************************
***
*	gb_id_make(): make the Seq-id for Genbank with tsip->name = name
*	tsip->acc.
*
*************************************************************************
***/
NLM_EXTERN SeqIdPtr gb_id_make(CharPtr name, CharPtr acc)
{
   SeqIdPtr new_id;
   TextSeqIdPtr tsip;

	   new_id= (SeqIdPtr)ValNodeNew(NULL);
	   new_id->choice = SEQID_GENBANK;
	   tsip = TextSeqIdNew();
	   if(name)
	   {
	   if(StringLen(name) >3 && IS_ALPHA(name[0]))
	     tsip->name = StringSave(name);
	   }
	   if(acc)
	   {
	   if(StringLen(acc) >3 && IS_ALPHA(acc[0]))
	     tsip->accession = StringSave(acc);
	   }
	   new_id->data.ptrvalue = tsip;
	   return new_id;
}




/**************************************************************************
***
*	bb_sort(): bubble sort to the accending order of starts
*
***************************************************************************
***/
NLM_EXTERN void bb_sort(Int4Ptr start, Int2 num)
{
    Int4 hold;
    Int2 j, pass;
    Boolean swithed = TRUE;

	for(pass = 0; pass < num-1 && swithed == TRUE; ++pass){
	   swithed = FALSE;
	   for(j=0; j<num - pass -1; ++j){
	     if(start[j] > start[j+1]){
		swithed = TRUE;
		hold = start[j];
		start[j] = start[j+1];
		start[j+1] = hold;
	      }/**end of if**/
	    }
	}
}


NLM_EXTERN Boolean get_synonym(ValNodePtr head, CharPtr syn)
{

	if(head == NULL)
	  return FALSE;

	StringCpy(syn, head->data.ptrvalue);
	head = head->next;

	while(head){
	   StringCat(syn, "  ");
	   StringCat(syn, head->data.ptrvalue);
	   head = head->next;
	}

	return TRUE;

}

/******************************************************************
***
*	general_id_make(): make SeqIdPtr with SEQID_LOCAL choice
*
*******************************************************************
***/
NLM_EXTERN SeqIdPtr general_id_make(CharPtr name, CharPtr dbase)
{
   SeqIdPtr new_id;
   ObjectIdPtr obj_id;
   DbtagPtr db_tag;


	new_id=(SeqIdPtr)ValNodeNew(NULL);
	new_id->choice = SEQID_GENERAL;
	db_tag = DbtagNew();
	db_tag->db = StringSave(dbase);

	obj_id = ObjectIdNew();
	obj_id->str = StringSave(name);
	db_tag->tag = obj_id;

	new_id->data.ptrvalue = db_tag;

	return new_id;
}



NLM_EXTERN Boolean find_synonym(SeqFeatPtr sfp, CharPtr syn)
{
   GeneRefPtr grp;

	if(sfp->data.choice != 1)
	   return FALSE;

	grp = sfp->data.value.ptrvalue;
	return get_synonym(grp->syn, syn);

}

NLM_EXTERN Boolean check_syn(ValNodePtr syn, CharPtr symbol)
{

	if(syn == NULL)
	   return FALSE;

	while(syn){
	  if(StringICmp(syn->data.ptrvalue, symbol)==0)
	    return TRUE;
	  syn = syn->next;
	}

	return FALSE;

}



NLM_EXTERN SeqIdPtr find_sip(SeqIdPtr sip)
{
  Uint1  order [NUM_SEQID];

  SeqIdBestRank (order, NUM_SEQID);
  order [SEQID_LOCAL] = 2;
  order [SEQID_GENBANK] = 5;
  order [SEQID_EMBL] = 5;
  order [SEQID_PIR] = 5;
  order [SEQID_SWISSPROT] = 5;
  order [SEQID_DDBJ] = 5;
  order [SEQID_PRF] = 5;
  order [SEQID_PDB] = 5;
  order [SEQID_TPG] = 5;
  order [SEQID_TPE] = 5;
  order [SEQID_TPD] = 5;
  order [SEQID_GPIPE] = 9;
  order [SEQID_PATENT] = 10;
  order [SEQID_OTHER] = 1;
  order [SEQID_GENERAL] = 10;
  order [SEQID_GIBBSQ] = 15;
  order [SEQID_GIBBMT] = 15;
  order [SEQID_GIIM] = 20;
  order [SEQID_GI] = 20;
  return SeqIdSelect (sip, order, NUM_SEQID);
}

NLM_EXTERN Boolean  get_seq_name(SeqIdPtr hsip, CharPtr name, CharPtr acc, CharPtr dbase, Boolean check_chain)
{
  TextSeqIdPtr tsip;
  ObjectIdPtr obj_id;
  DbtagPtr db_tag;
  GiimPtr giim;
  SeqIdPtr sip;


	if(check_chain)
		sip = find_sip(hsip);
	else
		sip = hsip;
	if(sip == NULL)
		return FALSE;
	switch(sip->choice){
	   case 11:
	      db_tag = sip->data.ptrvalue;
	      obj_id = db_tag->tag;
	      StringCpy(acc, obj_id->str);
	      StringCpy(name, obj_id->str);
	      StringCpy(dbase, db_tag->db);
	      return TRUE;
	    case 5:
	      StringCpy(dbase, "GenBank");
	       tsip = sip->data.ptrvalue;
	       StringCpy(acc, tsip->accession);
	       if(tsip->name == NULL)
			StringCpy(name, acc);
	       else
	       		StringCpy(name, tsip->name);
	       return TRUE;
	    case 6:
	      StringCpy(dbase, "EMBL");
	       tsip = sip->data.ptrvalue;
	       StringCpy(acc, tsip->accession);
	       StringCpy(name, tsip->name);
	       return TRUE;
	    case 8:
	      StringCpy(dbase, "SwissProt");
	       tsip = sip->data.ptrvalue;
	       StringCpy(acc, tsip->accession);
	       StringCpy(name, tsip->name);
	       return TRUE;
	    case 13:
	       StringCpy(dbase, "DDBJ");
	       tsip = sip->data.ptrvalue;
	       StringCpy(acc, tsip->accession);
	       StringCpy(name, tsip->name);
	       return TRUE;
	    case 4:
	       giim = sip->data.ptrvalue;
	       if(giim->db)
		 StringCpy(dbase, giim->db);
	       else
		 StringCpy(dbase, "NCBI");
	       sprintf(acc, "giim%ld", (long)(giim->id));
	       sprintf(name, "giim%ld", (long)(giim->id));
	       return TRUE;
	    default:
	       return FALSE;
	  }
}


NLM_EXTERN Boolean get_sip_comment(SeqIdPtr sip, CharPtr comment)
{
  Char dbase[20], name[20], acc[20];

	if(get_seq_name(sip, name, acc, dbase, TRUE)){
	   if(sip->choice == SEQID_GENERAL)
	  	sprintf(comment, "Dbase %s; Name %s", dbase, acc);
	   else
		sprintf(comment, "Dbase %s; Name %s; Accession %s", dbase, name, acc);
	   return TRUE;
	}
	else
	   return FALSE;
}


/**make num empty space in the array**/
NLM_EXTERN void make_empty(CharPtr empty, Int2 num)
{
	Int2 i;

	for(i=0; i<(num); ++i)
	{
	   *empty = ' ';
	   ++empty;
	}
	*empty = '\0';
}


NLM_EXTERN Boolean is_sequenced(SeqLocPtr slp)
{
	IntFuzzPtr ifp;
	SeqPntPtr spp;

	if(slp->choice == SEQLOC_PNT)
	{
		spp = slp->data.ptrvalue;
		ifp = spp->fuzz;
		if(ifp)
		{
			if(ifp->choice ==4 && ifp->a ==0)
				return FALSE;
		}
	}

	return TRUE;

}


NLM_EXTERN SeqLocPtr fuzz_loc(Int4 start, Int4 stop, Uint1 strand, SeqIdPtr sip, Boolean is_from, Boolean is_to)
{
   SeqLocPtr new;
   IntFuzzPtr fuzz;
   SeqIntPtr sint;

	   new = SeqLocIntNew(start, stop, strand, sip);

	   sint = (SeqIntPtr)new->data.ptrvalue;
	   if(is_from){
	   	sint->if_from = IntFuzzNew();
	   	fuzz = sint->if_from;
	   	fuzz->choice = 4;
	   	fuzz->a =2;
	   }
	   if(is_to){
	     	sint->if_to = IntFuzzNew();
	     	fuzz = sint->if_to;
	     	fuzz->choice =4;
	     	fuzz->a =1;
	   }
	   return new;
}



NLM_EXTERN SeqLocPtr link_loc(SeqLocPtr new, SeqLocPtr head)
{
  Int4 start, stop;
  SeqLocPtr update_seq_loc(Int4, Int4, Uint1, SeqLocPtr );

	if(new == NULL)
	   return head;
	if(head == NULL)
	   return new;

	while(head->next != NULL)
	   head = head->next;
	if(SeqIdForSameBioseq(SeqLocId(new), SeqLocId(head))){
	   if(SeqLocStop(head) == (SeqLocStart(new) -1)){
	       if(SeqLocStrand(new) == SeqLocStrand(head)){
		  start = SeqLocStart(head);
		  stop = SeqLocStop(new);
		  update_seq_loc(start, stop, SeqLocStrand(head), head);
		  SeqLocFree(new);
		  return head;
		}
	     }
	}

	head->next = new;
	return new;
}


NLM_EXTERN void s_witch(Int4Ptr x, Int4Ptr y)
{
	Int4 temp;

	temp = *x;
	*x = *y;
	*y=temp;
}

NLM_EXTERN void swap(Int4Ptr x, Int4Ptr y)
{
  Int4 temp;

	if((*x) > (*y)){
	  temp = *x;
	  *x = *y;
	  *y = temp;
	}
}


NLM_EXTERN Uint1 get_r_strand(SeqLocPtr seq_loc, SeqLocPtr f_loc)
{
	if(SeqLocStrand(seq_loc) != Seq_strand_minus)
	   return SeqLocStrand(f_loc);
	else{
	   if(SeqLocStrand(f_loc) == Seq_strand_minus)
	      return Seq_strand_plus;
	   else
	      return Seq_strand_minus;
	}

}

static Boolean fit_this_line(Int4 start, Int4 stop, Int4Ptr p_pos, Int2 i, Int4 space)
{
	if(p_pos[2*i] == 0 && p_pos[2*i+1] == 0)
		return TRUE;

	if(start > (p_pos[2*i+1] + space))
		return TRUE;

	if(stop < (p_pos[2*i] - space))
		return TRUE;

	return FALSE;
}

NLM_EXTERN Int2 find_f_pos(Int4 start, Int4 stop, Int4Ptr p_pos, Int4 space, Int2 num)
{

  	Int2 i =0;

	for(i =0; i<num; ++i)
	{
		if(fit_this_line(start, stop, p_pos, i, space))
		{
			if(p_pos[2*i] == 0 && p_pos[2*i+1] == 0)
			{
				p_pos[2*i] = start;
				p_pos[2*i+1] = stop;
			}
			else
			{
				p_pos[2*i] = MIN(start, p_pos[2*i]);
				p_pos[2*i +1] = MAX(stop, p_pos[2*i+1]);
			}
			return i;
		}
	}

	return -1;
}

NLM_EXTERN Int2 find_group_pos(Int4Ptr p_pos, Int4 space, Int2 num, Int2 group_size, Int4Ptr group_value)
{

  Int2 i =0, j;
  Boolean found;
  Int4 start, stop;

	if(num < group_size)
		return -1;
	for(i =0; i<= (num - group_size); ++i)
	{
	   found = TRUE;
	   for(j =0; j<group_size; ++j)
	   {
		start = group_value[2*j];
		stop = group_value[2*j+1];
		if(!fit_this_line(start, stop, p_pos, (Int2)(i+j), space))
		{
			found = FALSE;
			break;
		}
	   }
	   if(found)
	   {
		for(j =0; j<group_size; ++j)
		{
			start = group_value[2*j];
			stop = group_value[2*j+1];
			if(p_pos[2* (i+j)] == 0 && p_pos[2*(i+j)+1] == 0)
			{
				p_pos[2* (i+j)] = start;
				p_pos[2*(i+j)+1] = stop;
			}
			else
			{
				p_pos[2* (i+j)] = MIN(start, p_pos[2*(i+j)]);
				p_pos[2*(i+j)+1] = MAX(stop, p_pos[2*(i+j)+1]);
			}
		}
		return i;
	   }
	}

	Message(MSG_ERROR, "Fail in find_group_pos");
	return -1;
}
		

static void add_name_partial(CharPtr name, Boolean trunc5, Boolean trunc3)
{
   Char temp[100];

	if(!trunc5 && !trunc3)
	    return;

	if(trunc5)
	  sprintf(temp, "\'%s", name);
	else
	  StringCpy(temp, name);

	if(trunc3)
	   StringCat(temp, "'");

	StringCpy(name, temp);
	return;

}


/************************************************************************
***
*	get the citation label from a Pub-equiv
*
************************************************************************
***/
static Boolean get_cit_edit(CharPtr fmark, Int2 order, ValNodePtr head)
{

  ValNodePtr vnp, tmp;
  CitArtPtr cap;
  CitBookPtr cbp;
  CitSubPtr csp;
  CitGenPtr cgp;
  AuthListPtr alp = NULL;

	       vnp = head;
	       while(vnp){
		 if(vnp->choice == 4){
		   sprintf(fmark, "muid %ld", (long)(vnp->data.intvalue));
		   return TRUE;
		  }
		  vnp = vnp->next;
	        }
		vnp = head;

		alp = NULL;
		while(vnp){
		   if(vnp->choice ==5){
		     cap = vnp->data.ptrvalue;
		     if(cap)
			alp = cap->authors;
		   }
		   if(vnp->choice == 2){
		      csp = vnp->data.ptrvalue;
		      if(csp)
			 alp = csp->authors;
		   }
		   if(vnp->choice ==7){
		      cbp = vnp->data.ptrvalue;
		      if(cbp)
			 alp = cbp->authors;
		    }
		    if(vnp->choice ==1){
		       cgp = (CitGenPtr)vnp->data.ptrvalue;
		       if(cgp)
			  alp = cgp->authors;
		    }

		    if(alp){
			   if(alp->names){
			      tmp= alp->names;
			      if(tmp->choice ==3){
			      sprintf(fmark, "%s", (CharPtr) tmp->data.ptrvalue);
			      return TRUE;
			      }
			   }
		     }/*alp*/

		   vnp = vnp->next;
		}

		StringCpy(fmark, "Pub");
		return TRUE;
}

static void myStringNCpy(CharPtr target, CharPtr source, Int2 n)
{
	Int2 i;

	n = MIN(n, (Int2)StringLen(source));
	for(i = 0; i<n; ++i)
		target[i] = source[i];
	target[i] = '\0';
}

NLM_EXTERN Uint1 label_feature(SeqFeatPtr sfp, CharPtr fmark, Int2 order, Boolean use_locus)
{
  GeneRefPtr grp;
  SeqLocPtr slp;
  SeqIntPtr sint;
  Boolean trunc5, trunc3;
  OrgRefPtr org;

  CdRegionPtr crp;
  SeqIdPtr sip;

  ProtRefPtr prp;
  ValNodePtr vnp, dbase;
  Char temp[200];
  RnaRefPtr rrp;
  ImpFeatPtr ifp;
  RsiteRefPtr rrf;
  TxinitPtr tp;
  PubdescPtr pdp;

  Char name[100];
  BioseqPtr bsp;
  Uint1 val;
  Boolean is_done, is_end;

     slp = sfp->location;
     trunc5 = FALSE;
     trunc3 = FALSE;
     if(slp->choice == SEQLOC_INT){
	sint = slp->data.ptrvalue;
	if(sint->if_from){
	   if(SeqLocStrand(slp) == Seq_strand_minus)
	     trunc3 = TRUE;
	   else
	     trunc5 = TRUE;
	 }
	 if(sint->if_to){
	   if(SeqLocStrand(slp) == Seq_strand_minus)
	      trunc5 = TRUE;
	   else
	      trunc3 = TRUE;
	  }
      }

	switch(sfp->data.choice){
	  case 1:
	    grp = sfp->data.value.ptrvalue;
	    if(grp && grp->locus)
	       myStringNCpy(fmark, grp->locus, 20);
	    else
	       StringCpy(fmark, "gene");
	    add_name_partial(fmark, trunc5, trunc3);
	    return TRUE;

	  case 2:
	    org = sfp->data.value.ptrvalue;
	    if(org && org->taxname)
	      StringCpy(fmark, org->taxname);
	    else{
	      if(org && org->common)
	         StringCpy(fmark, org->common);
	      else
	         StringCpy(fmark, "Org");
	    }
	    add_name_partial(fmark, trunc5, trunc3);
	    return TRUE;

	  case 3:
	    val = 1;
	    crp = sfp->data.value.ptrvalue;
	    if(crp && sfp->product){
	      sip = SeqLocId(sfp->product);
	      if((bsp = BioseqFind(sip))!= NULL)
		 seqid_name(bsp->id, name, use_locus, TRUE);
	       else
		 seqid_name(sip, name, use_locus, TRUE);

	       if(crp->orf)
		  sprintf(fmark, " ORF %s", name);
		else
		  StringCpy(fmark, name);

	    }
	    else{
		StringCpy(fmark, "CDregion");
		val =2;
	    }

	    add_name_partial(fmark, trunc5, trunc3);
	    return val;

	   case 4:
	     prp = sfp->data.value.ptrvalue;
	     vnp = prp->name;
	     dbase = prp->db;
	     if(vnp && dbase){
	       sprintf(temp, "%s %s", (CharPtr) dbase->data.ptrvalue, (CharPtr) vnp->data.ptrvalue);
	       myStringNCpy(fmark, temp, 20);
	       add_name_partial(fmark, trunc5, trunc3);
	       return TRUE;
	     }
	     if(dbase){
	       StringCpy(fmark, dbase->data.ptrvalue);
	       add_name_partial(fmark, trunc5, trunc3);
	       return TRUE;
	     }
	     if(vnp){
	       StringCpy(fmark, vnp->data.ptrvalue);
	       add_name_partial(fmark, trunc5, trunc3);
	       return TRUE;
	     }

	     StringCpy(fmark, "ProRef");
	     add_name_partial(fmark, trunc5, trunc3);
	     return TRUE;
	   case 5:
	     rrp = sfp->data.value.ptrvalue;
	     if(rrp->type == 0){
	       StringCpy(fmark, "Unknown");
	       add_name_partial(fmark, trunc5, trunc3);
	       return TRUE;
	     }
	     if(rrp->type == 1){
	       StringCpy(fmark, "Premsg");
	       add_name_partial(fmark, trunc5, trunc3);
	       return TRUE;
	     }
	     if(rrp->type == 2){
	       StringCpy(fmark, "mRNA");
	       add_name_partial(fmark, trunc5, trunc3);
	       return TRUE;
	     }
	     if(rrp->type == 3){
	       StringCpy(fmark, "tRNA");
	       add_name_partial(fmark, trunc5, trunc3);
	       return TRUE;
	     }
	     if(rrp->type == 4){
	       StringCpy(fmark, "rRNA");
	       add_name_partial(fmark, trunc5, trunc3);
	       return TRUE;
	     }
	     if(rrp->type == 5){
	       StringCpy(fmark, "snRNA");
	       add_name_partial(fmark, trunc5, trunc3);
	       return TRUE;
	     }
	     if(rrp->type == 6){
	       StringCpy(fmark, "scRNA");
	       add_name_partial(fmark, trunc5, trunc3);
	       return TRUE;
	     }
	     if(rrp->type == 7){
	       StringCpy(fmark, "snoRNA");
	       add_name_partial(fmark, trunc5, trunc3);
	       return TRUE;
	     }
	     if(rrp->type == 255){
	       StringCpy(fmark, "Other");
	       add_name_partial(fmark, trunc5, trunc3);
	       return TRUE;
	     }
	     StringCpy(fmark, "RNA");
	     add_name_partial(fmark, trunc5, trunc3);
	     return TRUE;
	   case 6:
	     pdp = sfp->data.value.ptrvalue;
	     vnp = pdp->pub;
	     return get_cit_edit(fmark, order, vnp);

	   case 7:
	      sip = SeqLocId(sfp->data.value.ptrvalue);
	      seqid_name(sip, fmark, use_locus, TRUE);
	      add_name_partial(fmark, trunc5, trunc3);
	      return TRUE;

	   case 8:
	      ifp = sfp->data.value.ptrvalue;
	      is_done = FALSE;
	      if(ifp->descr){
		StringCpy(temp, ifp->descr);
		myStringNCpy(fmark, temp, 20);
		is_done = TRUE;
	       }
	      if(ifp->key && !is_done){
		StringCpy(temp, ifp->key);
		myStringNCpy(fmark, temp, 20);
		is_done = TRUE;
	      }
	      if(!is_done)
	       	StringCpy(fmark, "Import");
	      is_end = FALSE;
	      if(sfp->cit != NULL){
		 if (sfp->cit->choice ==1){
		      vnp = (sfp->cit->data.ptrvalue);
		      while(vnp && !is_end){
			 if(vnp->choice == PUB_Muid)
		       		is_end = get_cit_edit(temp, order, vnp);
			 if(vnp->choice == PUB_Equiv)
			    	is_end = get_cit_edit(temp, order, vnp->data.ptrvalue);
			  vnp = vnp->next;
		       }
		       if(is_end)
		           StringCpy(fmark, temp);
		   }
	       }
	       if(!is_end)
	          add_name_partial(fmark, trunc5, trunc3);
	       return TRUE;

	    case 9:
	      if(sfp->data.value.ptrvalue){
		StringCpy(temp, sfp->data.value.ptrvalue);
		if(StringLen(temp) > 20)
		   myStringNCpy(fmark, temp, 20);
	        else
		   StringCpy(fmark, temp);
	        add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }
	      StringCpy(fmark, "Region");
	      add_name_partial(fmark, trunc5, trunc3);
	      return TRUE;
	    case 10:
	       StringCpy(fmark, "Comment");
	       add_name_partial(fmark, trunc5, trunc3);
	       return TRUE;
	    case 11:
	      if(sfp->data.value.intvalue ==1){
		StringCpy(fmark, "Disulfide");
	        add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }
	      if(sfp->data.value.intvalue ==2){
		StringCpy(fmark, "Thiolester");
	        add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }
	      if(sfp->data.value.intvalue ==3){
		StringCpy(fmark, "Xlink");
	        add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }
	      if(sfp->data.value.intvalue ==255){
		StringCpy(fmark, "Disulfide");
	        add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }

	      StringCpy(fmark, "bond");
	       add_name_partial(fmark, trunc5, trunc3);
	      return TRUE;

	    case 12:
	      if(sfp->data.value.intvalue ==1){
		StringCpy(fmark, "Active");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }
	      if(sfp->data.value.intvalue ==2){
		StringCpy(fmark, "Binding");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }
	      if(sfp->data.value.intvalue ==3){
		StringCpy(fmark, "Cleavage");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }
	      if(sfp->data.value.intvalue ==4){
		StringCpy(fmark, "Inhibit");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }
	      if(sfp->data.value.intvalue ==5){
		StringCpy(fmark, "Modified");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }

	      if(sfp->data.value.intvalue ==6){
		StringCpy(fmark, "Glycosylation");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }


	      if(sfp->data.value.intvalue ==7){
		StringCpy(fmark, "Myristoylation");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }


	      if(sfp->data.value.intvalue ==8){
		StringCpy(fmark, "Mutagenized");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }


	      if(sfp->data.value.intvalue ==9){
		StringCpy(fmark, "Metal-binding");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }


	      if(sfp->data.value.intvalue ==10){
		StringCpy(fmark, "Phosphorylation");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }


	      if(sfp->data.value.intvalue ==11){
		StringCpy(fmark, "Aceylation");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }


	      if(sfp->data.value.intvalue ==12){
		StringCpy(fmark, "Amidation");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }


	      if(sfp->data.value.intvalue ==13){
		StringCpy(fmark, "Methylation");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }


	      if(sfp->data.value.intvalue ==14){
		StringCpy(fmark, "Hydroxylation");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }


	      if(sfp->data.value.intvalue ==16){
		StringCpy(fmark, "Oxidative-deamination");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }


	      if(sfp->data.value.intvalue ==15){
		StringCpy(fmark, "Sulfataion");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }


	      if(sfp->data.value.intvalue ==17){
		StringCpy(fmark, "Pyrrolidone-carbosylic-acid");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }


	      if(sfp->data.value.intvalue ==18){
		StringCpy(fmark, "Gamma-carboxyglutamic-acid");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }


	      if(sfp->data.value.intvalue ==19){
		StringCpy(fmark, "Blocked");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }


	      if(sfp->data.value.intvalue ==20){
		StringCpy(fmark, "Lipid-binding");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }


	      if(sfp->data.value.intvalue ==21){
		StringCpy(fmark, "np-binding");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }


	      if(sfp->data.value.intvalue ==22){
		StringCpy(fmark, "dna-binding");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }

	      if(sfp->data.value.intvalue ==255){
		StringCpy(fmark, "Other");
		add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }

	      StringCpy(fmark, "Site");
	       add_name_partial(fmark, trunc5, trunc3);
	      return TRUE;
	     case 13:
	       rrf = sfp->data.value.ptrvalue;
	       if(rrf->choice ==1){
		 StringCpy(fmark, rrf->data.ptrvalue);
	          add_name_partial(fmark, trunc5, trunc3);
		 return TRUE;
	       }
	      StringCpy(fmark, "Rsite");
	       add_name_partial(fmark, trunc5, trunc3);
	      return TRUE;

	     case 14:
	      StringCpy(fmark, "User");
	       add_name_partial(fmark, trunc5, trunc3);
	      return TRUE;

	     case 15:
	       tp = sfp->data.value.ptrvalue;
	       if(tp->name){
		 StringCpy(temp, tp->name);
		 myStringNCpy(fmark, temp, 20);
	       add_name_partial(fmark, trunc5, trunc3);
		 return TRUE;
	       }
	      StringCpy(fmark, "Txinit");
	       add_name_partial(fmark, trunc5, trunc3);
	      return TRUE;

	    case 16:
	      StringCpy(fmark, "Numbering");
	       add_name_partial(fmark, trunc5, trunc3);
	      return TRUE;

	    case 17:
	      if(sfp->data.value.intvalue ==1){
		StringCpy(fmark, "Helix");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }
	      if(sfp->data.value.intvalue ==2){
		StringCpy(fmark, "Sheet");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }
	      if(sfp->data.value.intvalue ==3){
		StringCpy(fmark, "Turn");
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }
	      StringCpy(fmark, "Psec");
	       add_name_partial(fmark, trunc5, trunc3);
	      return TRUE;

	     case 18:
	      if(sfp->data.value.ptrvalue){
		StringCpy(temp, sfp->data.value.ptrvalue);
		myStringNCpy(fmark, temp, 20);
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }
	      StringCpy(fmark, "NstdRes");
	       add_name_partial(fmark, trunc5, trunc3);
	      return TRUE;

	     case 19:
	      if(sfp->data.value.ptrvalue){
		StringCpy(temp, sfp->data.value.ptrvalue);
		myStringNCpy(fmark, temp, 20);
	       add_name_partial(fmark, trunc5, trunc3);
		return TRUE;
	      }
	      StringCpy(fmark, "Heterogen");
	       add_name_partial(fmark, trunc5, trunc3);
	      return TRUE;

	      default:
		return FALSE;

	     }

}



NLM_EXTERN Int1 loc_offset(SeqLocPtr seq_loc, SeqLocPtr f_loc, Int4Ptr left, Int4Ptr right, BoolPtr l_trunc, BoolPtr r_trunc)
{
	Boolean temp;


	if(SeqLocCompare(seq_loc, f_loc) == SLC_NO_MATCH)
		return -1;

	*l_trunc = FALSE;
	*r_trunc = FALSE;
	*left = GetOffsetInLoc(f_loc, seq_loc, SEQLOC_LEFT_END);
	if(*left == -1)
	{
	   *l_trunc = TRUE;
	   if(SeqLocStrand(seq_loc) == Seq_strand_minus)
	      *left = SeqLocLen(seq_loc) -1;
	   else
	     *left = 0;
	}
	
	*right = GetOffsetInLoc(f_loc, seq_loc, SEQLOC_RIGHT_END);
	if(*right == -1)
	{
	   *r_trunc = TRUE;
	   if(SeqLocStrand(seq_loc) == Seq_strand_minus)
	      *right = 0;
	   else
	     *right = SeqLocLen(seq_loc) -1;
	}

	if(*right < *left)
	{
	   temp = *l_trunc;
	   *l_trunc = *r_trunc;
	   *r_trunc = temp;
	   swap(left, right);
	}

	return (Int1)get_r_strand(seq_loc, f_loc);

}


NLM_EXTERN Int1 tloc_offset(SeqLocPtr seq_loc, SeqLocPtr f_loc, Int4Ptr left, Int4Ptr right)
{
  Int4 t_start, t_stop;


	if(SeqLocCompare(seq_loc, f_loc) == SLC_NO_MATCH)
	   return -1;

	t_start = MAX(SeqLocStart(f_loc), SeqLocStart(seq_loc));
	t_stop = MIN(SeqLocStop(f_loc), SeqLocStop(seq_loc));
	if(SeqLocStrand(seq_loc) != Seq_strand_minus){
	*left = t_start - SeqLocStart(seq_loc);
	*right = t_stop - SeqLocStart(seq_loc);
	}
	else{
	*left = SeqLocStop(seq_loc) - t_stop;
	*right = SeqLocStop(seq_loc) - t_start;
	}
	swap (left, right);

	return (Int1)get_r_strand(seq_loc, f_loc);

}

NLM_EXTERN Boolean lr_offset_in_slp(SeqLocPtr slp, Int4 t_start, Int4 t_stop, Int4Ptr l_offset, Int4Ptr r_offset)
{
	Int4 start, stop;

	start = SeqLocStart(slp);
	stop = SeqLocStop(slp);
	if(t_start> stop || t_stop < start)
		return FALSE;
	t_start = MAX(t_start, start);
	t_stop = MIN(t_stop, stop);
	
	if(SeqLocStrand(slp) == Seq_strand_minus)
	{
		*l_offset = stop - t_stop;
		*r_offset = stop - t_start;
	}
	else
	{
		*l_offset = t_start - start;
		*r_offset = t_stop - start;
	}

	return TRUE;
}

/**************************************************************************
***
*	make_range(): initiate the range on the genome for (start, stop)
*	if(cross 0), have two slp, otherwise have only one segment
*
***************************************************************************
***/
NLM_EXTERN SeqLocPtr make_range(Int4 start, Int4 stop, Int4 end, SeqIdPtr sip, ValNodePtr PNTR g_start)
{
   SeqLocPtr slp;
   ValNodePtr vnp, next;

	if(start <0)
		start += end;
	if(stop <0)
		stop += end;

	if(start <= stop){
	  slp = SeqLocIntNew(start, stop, Seq_strand_plus, sip);
	  slp->next = NULL;

	  vnp = ValNodeNew(NULL);
	  vnp->next = NULL;
	  vnp->choice =0;
	  vnp->data.intvalue =0;
	}

	else{		/*for circular molecules which cross the circle*/
	  slp = SeqLocIntNew(start, end, Seq_strand_plus, sip);
	  slp->next = SeqLocIntNew(0, stop, Seq_strand_plus, sip);
	  vnp = ValNodeNew(NULL);
	  vnp->choice =1;
	  vnp->data.intvalue =0;

	  next = ValNodeNew(vnp);
	  next->choice =1;
	  next->data.intvalue = SeqLocLen(slp);

	}

	*g_start = vnp;
	return slp;
}


/******************************************************************************
***
*	make_ext_feat(): return Seq-feat from bsp->seq_ext
*
*******************************************************************************
***/
NLM_EXTERN SeqFeatPtr make_ext_feat(BioseqPtr biosp)
{


	   if(biosp == NULL)
	      return NULL;

	   if(biosp->repr != Seq_repr_map)/**No physical Map**/
	     return NULL;

	   if(biosp->seq_ext_type ==3)
	     return (SeqFeatPtr)(biosp->seq_ext);

	   return NULL;
}

/*****************************************************************************
***
*	get_bsp_feat(): make Seq-feat from Bioseq from bsp->annot
*
******************************************************************************
***/
NLM_EXTERN SeqFeatPtr get_bsp_feat(BioseqPtr bsp)
{
  SeqAnnotPtr annot;

	if(bsp == NULL)
	  return NULL;

	annot = bsp->annot;
	if(annot == NULL)
	  return NULL;

	while(annot){
	  if(annot->type == 1)
	    return (SeqFeatPtr)annot->data;
	  annot = annot->next;
	}

	return NULL;
}



NLM_EXTERN SeqAlignPtr find_nth_align(SeqAlignPtr align, Int2 num)
{
   Int2 i;

	i =0;
	while(align){
	  ++i;
	  if(num == i)
	     return align;
	   align = align->next;
	}

	return NULL;


}


/******************************************************************************
***
*	get the Seq-align from Seq-annot in a Bioseq
*
*******************************************************************************
***/
NLM_EXTERN SeqAlignPtr get_bsp_align(BioseqPtr bsp)
{
  SeqAnnotPtr annot;

	if(bsp == NULL)
	  return NULL;

	annot = bsp->annot;
	if(annot == NULL)
	  return NULL;

	while(annot){
	  if(annot->type == 2)
	    return (SeqAlignPtr)annot->data;
	  annot = annot->next;
	}

	return NULL;
}

NLM_EXTERN SeqAlignPtr get_sep_align(SeqEntryPtr sep)
{
  BioseqSetPtr bssp;
  BioseqPtr bsp;
  SeqAnnotPtr annot;

	if(sep->choice ==1){
	  bsp = sep->data.ptrvalue;
	  return get_bsp_align(bsp);
	}

	else{
	  bssp = sep->data.ptrvalue;
	  annot = bssp->annot;
	}

	if(annot == NULL)
	   return NULL;

	while(annot){
	  if(annot->type == 2)
	    return (SeqAlignPtr)annot->data;
	  annot = annot->next;
	}

	return NULL;

}


/*************************************************************************
****
*	get_align_id(): get SeqIdPtr from SeqAlignPtr.
*	order is the order of the aligned seq in SeqAlignPtr
*	in SeqIdPtr, can be 0 or 1 for 2-D alignment.
*
**************************************************************************
****/
NLM_EXTERN SeqIdPtr get_align_id(SeqAlignPtr sap_p, Int2 order)
{
    DenseSegPtr  dsp;
    StdSegPtr    ssp;
    DenseDiagPtr ddp;
    SeqAlignPtr  sap;
    Int2         i;
    SeqIdPtr     sip;
    SeqLocPtr    loc;
    
    i =0;
    switch(sap_p->segtype) {
    case 1:		/*Dense Diag*/
        ddp = sap_p->segs;
        sip = ddp->id;
        for(; sip!=NULL; sip = sip->next, ++i)
            if(order == i)
                return sip;
        return NULL;
    case 2:		/*Dense Seg*/
        dsp = sap_p->segs;
        sip = dsp->ids;
        for(; sip!=NULL; sip = sip->next, ++i)
            if(order == i)
                return sip;
        return NULL;
    case 3:
        ssp = sap_p->segs;
        loc = ssp->loc;
        for(; loc != NULL; loc = loc->next, ++i)
            if(order == i)
                return SeqLocId(loc);
        return NULL;
    case 5: /* Discontinuous alignment */
        sap = (SeqAlignPtr) sap_p->segs;
        sip = get_align_id(sap, order);
        return  sip;

    default:
        return NULL;
    }
}
			

/**************************************************************************
***
*	get_align_strand(): get the strand of an aligned sequence
*
**************************************************************************
***/
NLM_EXTERN Uint1 get_align_strand(SeqAlignPtr sap_p, Int2 order)
{
    DenseSegPtr dsp;
    StdSegPtr   ssp;
    DenseDiagPtr ddp;
    SeqLocPtr loc;
    Int2 i;
    SeqAlignPtr sap;

    switch(sap_p->segtype) {
    case 1:		/*Dense Diag*/
        ddp = sap_p->segs;
        order = order%(ddp->dim);
        if(ddp->strands)
            return ddp->strands[order];
        return 0;
    case 2:		/*Dense Seg*/
        dsp = sap_p->segs;
        order = order%(dsp->dim);
        if(dsp->strands)
            return dsp->strands[order];
        return 0;
    case 3:
        ssp = sap_p->segs;
        loc = ssp->loc;
        for(i = 0; loc != NULL; loc = loc->next, ++i)
            if(order == i)
                return SeqLocStrand(loc);
        return 0;
    case 5: /* Discontinuous alignment */
        
        sap = (SeqAlignPtr) sap_p->segs;
        return get_align_strand(sap, order);

    default:
        return 0;
    }
}
			
/************************************************************************
****
*	m_id_match(): check if the SeqAlign contains the sequence of 
*	match_id, return the order of the sequence in the alignment
*************************************************************************
****/
NLM_EXTERN Int2 m_id_match(SeqAlignPtr sap_p, SeqIdPtr match_id)
{
    DenseSegPtr dsp;
    StdSegPtr   ssp;
    DenseDiagPtr ddp;
    
    Int2 i;
    SeqIdPtr sip;
    SeqLocPtr loc;
    SeqAlignPtr sap;

    i =0;
    switch(sap_p->segtype) {
    case 1:		/*Dense Diag*/
        ddp = sap_p->segs;
        sip = ddp->id;
        for(; sip!=NULL; sip = sip->next, ++i)
            if(SeqIdForSameBioseq(sip, match_id))
                return i;
        return -1;
    case 2:		/*Dense Seg*/
        dsp = sap_p->segs;
        sip = dsp->ids;
        for(; sip!=NULL; sip = sip->next, ++i)
            if(SeqIdForSameBioseq(sip, match_id))
                return i;
        return -1;
    case 3:
        ssp = sap_p->segs;
        loc = ssp->loc;
        for(; loc != NULL; loc = loc->next, ++i)
            if(SeqIdForSameBioseq(SeqLocId(loc), match_id))
                return i;
        return -1;
    case 5: /* Discontinuous alignment */

        sap = (SeqAlignPtr) sap_p->segs;
        return m_id_match(sap, match_id);
        
    default:
        return -1;
    }   
}



/*****************************************************************************
***
*	get_clone_type(): determine if a clone is a YAC, a SEQUENCE or a CLONE
*	(cosmid library)
*
******************************************************************************
***/
NLM_EXTERN Uint1 get_clone_type(SeqIdPtr sip)
{
   Char acc[100], name[100], dbase[100];


	get_seq_name(sip, name, acc, dbase, TRUE);
	if(StringNICmp(dbase, "Yac", 3) ==0)
	  return IS_YAC;

	if(strstr(dbase, "clone") )
	  return IS_CLONE;

	if(StringCmp(dbase, "GenBank") ==0)
	  return  IS_SEQ;

	if(StringCmp(dbase, "EMBL") ==0)
	  return  IS_SEQ;

	if(StringCmp(dbase, "DDBJ") ==0)
	  return  IS_SEQ;


	if(StringCmp(dbase, "ACEDB") == 0)	/**it is not a clone**/
	  return IS_SEQ;

	return IS_CLONE;


}


NLM_EXTERN Boolean  check_match(Char a, Char b)
{

	if(a == b)
	   return TRUE;

	switch (a){
	  case 'A':
	    if(StringChr("DHMNRVW", b))
	       return TRUE;
	    else
	      return FALSE;
	   case 'B':
	     if(StringChr("GTC", b))
		return TRUE;
	     else
		return FALSE;
	   case 'C':
	     if(StringChr("BHMNSVY", b))
		return TRUE;
	     else
		return FALSE;
	   case 'D':
	     if(StringChr("GAT", b))
		return TRUE;
	     else
		return FALSE;
	   case 'G':
	     if(StringChr("BDKNRSV", b))
		return TRUE;
	     else
		return FALSE;
	   case 'H':
	     if(StringChr("ACT", b))
		return TRUE;
	     else
		return FALSE;
	   case 'K':
	     if(StringChr("GT", b))
		return TRUE;
	     else
		return FALSE;
	   case 'M':
	     if(StringChr("AC", b))
		return TRUE;
	     else
		return FALSE;
	   case 'N':
	     if(StringChr("AGTC", b))
		return TRUE;
	     else
		return FALSE;
	   case 'R':
	     if(StringChr("GA", b))
		return TRUE;
	     else
		return FALSE;
	   case 'S':
	     if(StringChr("GC", b))
		return TRUE;
	     else
		return FALSE;
	   case 'T':
	     if(StringChr("BDHKNWY", b))
		return TRUE;
	     else
		return FALSE;
	   case 'V':
	     if(StringChr("GCA", b))
		return TRUE;
	     else
		return FALSE;
	   case 'W':
	     if(StringChr("AT", b))
		return TRUE;
	     else
		return FALSE;
	   case 'Y':
	     if(StringChr("TC", b))
		return TRUE;
	     else
		return FALSE;
	   default :
	      return FALSE;
	}

}


static Boolean ReComputerNumObj(ValNodePtr desc, Int4 old_len, Int4 new_len)
{
   NumberingPtr num;
   NumRealPtr nrp;
   FloatHi val;

	if(old_len == new_len)
		return FALSE;

	if(desc == NULL)
		return FALSE;

	if(desc->choice != Seq_descr_num)
		return FALSE;

	num = (NumberingPtr)(desc->data.ptrvalue);
	if(num->choice != Numbering_real)
		return FALSE;

	if(new_len == 0)
		return FALSE;
	nrp = (NumRealPtr)(num->data.ptrvalue);
	val = (FloatHi) old_len * (nrp->a) /(FloatHi)new_len;
	nrp->a = val;
	num->data.ptrvalue = nrp;
	return TRUE;
}

static ValNodePtr get_numbering_desc(ValNodePtr desc, Uint1 type)
{
	while(desc)
	{
		if(desc->choice == type)
			return desc;
		desc = desc->next;
	}
	return NULL;
}


NLM_EXTERN Boolean SeqLocReplace(SeqIdPtr s_id, Int4 s_start, Int4 s_stop, Uint1 s_strand, SeqIdPtr t_id, Int4 t_start, Int4 t_stop, Uint1 t_strand)
{
  BioseqPtr source_bsp, target_bsp;
  Int4 pos;
  Boolean adj_feat;
  Int4 old_len, new_len;
  ValNodePtr num_desc;

	source_bsp = BioseqFind(s_id);
	target_bsp = BioseqFind(t_id);
	old_len = BioseqGetLen(target_bsp);

	if(target_bsp->repr == Seq_repr_map && source_bsp->repr != Seq_repr_map)
	{
		Message(MSG_ERROR, "Incorrect type of bioseq for map insertion");
		return FALSE;
	}
	else{
		pos = t_start;
		adj_feat = FALSE;
		if(s_stop - s_start != t_stop - t_start)
			adj_feat = TRUE;
		s_id = find_sip(source_bsp->id);
		if(s_id->choice == SEQID_OTHER)
			s_id = source_bsp->id;
		if(BioseqDelete(t_id, t_start, t_stop, adj_feat, FALSE))
		{

			if(BioseqInsert(s_id, s_start, s_stop, s_strand, t_id, pos, FALSE, adj_feat, FALSE))
			{
				new_len = BioseqGetLen(target_bsp);
				if(old_len != new_len)
				{
					num_desc = get_numbering_desc(target_bsp->descr, Seq_descr_num);
					ReComputerNumObj(num_desc, old_len, new_len);
				}
				return TRUE;
			 }


		}
		return FALSE;
	}


}


NLM_EXTERN Boolean get_pnt_val(SeqLocPtr slp, Int4Ptr pos, Uint1Ptr strand, BoolPtr fuzz)
{
	SeqPntPtr spp;
	if(slp->choice != SEQLOC_PNT)
		return FALSE;

	*pos = SeqLocStart(slp);
	*strand = SeqLocStrand(slp);
	spp = (SeqPntPtr)(slp->data.ptrvalue);
	if(spp->fuzz != NULL)
		*fuzz = TRUE;
	else
		*fuzz = FALSE;
	return TRUE;
}

NLM_EXTERN Boolean get_gene_syn(SeqFeatPtr sfp, CharPtr name, BoolPtr has_name, CharPtr syn, BoolPtr has_syn)
{
	GeneRefPtr grp;
	ValNodePtr vnp;
	Boolean has_prev;
	Char tmp[20];

	if(sfp->data.choice != 1)
		return FALSE;

	*has_name = FALSE;
	*has_syn = FALSE;
	grp = sfp->data.value.ptrvalue;
	if(grp->locus)
	{
		*has_name = TRUE;
		StringCpy(name, grp->locus);
	}

	if(grp->syn)
	{
		*has_syn = TRUE;
		vnp= grp->syn;
		has_prev = FALSE;
		while(vnp)
		{
			if(has_prev)
			{
				sprintf(tmp, " %s", (CharPtr) vnp->data.ptrvalue);
				StringCat(syn, tmp);
			}
			else
				StringCpy(syn, vnp->data.ptrvalue);
			vnp = vnp->next;
			has_prev = TRUE;
		}
	}

	return TRUE;

}


NLM_EXTERN Boolean get_feat_id(SeqFeatPtr sfp, CharPtr featId, BoolPtr has_fid, CharPtr dbase, BoolPtr has_db)
{
	DbtagPtr db_tag;
	ObjectIdPtr oip;


	*has_fid = FALSE;
	*has_db = FALSE;
	if(sfp->id.choice != 4 && sfp->id.choice != 3)
		return FALSE;

	if(sfp->id.choice == 3)
	{
		*has_db = FALSE;
		oip = sfp->id.value.ptrvalue;
		*has_fid = TRUE;
		StringCpy(featId, oip->str);
	}

	if(sfp->id.choice == 4)
	{
		db_tag = sfp->id.value.ptrvalue;
		oip = db_tag->tag;
		*has_db = TRUE;
		StringCpy(dbase, db_tag->db);
		*has_fid = TRUE;
		StringCpy(featId, oip->str);
	}

	return TRUE;

}

NLM_EXTERN Boolean DelNthFeat(SeqFeatPtr PNTR head, Int2 num)
{
	SeqFeatPtr curr, prev;
	Int2 i;

	i =0;
	curr = *head;
	prev = NULL;

	while(curr)
	{
		++i;
		if(num ==i)
		{
			if(prev == NULL)
				*head = curr->next;
			else
				prev->next = curr->next;
			curr->next = NULL;
			SeqFeatFree(curr);
			return TRUE;
		}
		prev = curr;
		curr = curr->next;
	}

	return FALSE;

}


NLM_EXTERN Boolean PackSeqPntInsert(PackSeqPntPtr head, Int4 pos)
{
	Int4 c_pos;
	Uint1 i, cnt;
	PackSeqPntPtr pspp, new;

	pspp = head;
	while(pspp)
	{
		cnt = pspp->used;
		for(i =0; i<pspp->used; ++i)
		{
			if(pos < pspp->pnts[i])
			{
				c_pos = pspp->pnts[i];
				pspp->pnts[i] = pos;
				pspp->used = i+1;

				new = PackSeqPntNew();
				for(; i<cnt; ++i)
				{
					if(new->used ==0)
						new->pnts[new->used] = c_pos;
					else
						new->pnts[new->used] = pspp->pnts[i];
					++new->used;
				}
				new->next = pspp->next;
				pspp->next = new;
				return TRUE;
			}

		}
		pspp = pspp->next;
	}

	return PackSeqPntPut(head, pos);
}

NLM_EXTERN Boolean insert_new_rsite(BioseqPtr r_bsp, CharPtr enz, Int4 pos)
{
	SeqFeatPtr sfp;
	RsiteRefPtr rrp;
	CharPtr name;
	SeqLocPtr slp;
	PackSeqPntPtr pspp;


	if(r_bsp == NULL)
		return FALSE;

	sfp = make_ext_feat(r_bsp);
	if(sfp->data.choice != 13)
		return FALSE;

	while(sfp)
	{
		rrp = sfp->data.value.ptrvalue;
		name = rrp->data.ptrvalue;
		if(StringICmp(name, enz) ==0)
		{
			slp = sfp->location;
			pspp = (PackSeqPntPtr)(slp->data.ptrvalue);
			return PackSeqPntInsert(pspp, pos);
		}
		sfp = sfp->next;
	}

	return FALSE;

}


NLM_EXTERN Boolean PackSeqPntDelete(PackSeqPntPtr PNTR head, Int2 index)
{
	PackSeqPntPtr pspp, prev;
	Int2 j, i;

	if(index < 0)
		return FALSE;
	pspp = *head;
	j =0;
	prev = NULL;
	while(pspp)
	{
		if(index < (Int2)(j + (pspp->used)))
		{
			if(pspp->used == 1)
			{
				if(prev != NULL)
					prev->next = pspp->next;
				else
					*head = pspp->next;
				pspp->next = NULL;
				PackSeqPntFree(pspp);
				return TRUE;
			}
			else
			{
				for(i=index-j; i<(Int2)(pspp->used -1); ++i)
					pspp->pnts[i] = pspp->pnts[i+1];
				--(pspp->used);
				return TRUE;
			}
		}
		j += pspp->used;
		pspp = pspp->next;
	}

	return FALSE;
}




NLM_EXTERN Boolean delete_rsite(BioseqPtr r_bsp, Int2 order, Int2 index)
{
	SeqFeatPtr sfp, prev;
	SeqLocPtr slp;
	PackSeqPntPtr pspp;
	Int2 i;


	if(r_bsp == NULL)
		return FALSE;

	sfp = make_ext_feat(r_bsp);
	if(sfp->data.choice != 13)
		return FALSE;

	i =0;
	prev = NULL;
	while(sfp)
	{
		++i;
		if(i == order)
		{
			slp = sfp->location;
			if(slp->choice != SEQLOC_PACKED_PNT)
				return FALSE;
			pspp = slp->data.ptrvalue;
			if(PackSeqPntDelete(&pspp, index))
			{
				if(pspp == NULL)
				{
					if(prev)
						prev->next = sfp->next;
					else
						r_bsp->seq_ext = sfp->next;
					sfp->next = NULL;
					SeqFeatFree(sfp);
				}
				else
					slp->data.ptrvalue = pspp;
				return TRUE;
			}
		}
		prev = sfp;
		sfp = sfp->next;
	}
	return FALSE;

}

NLM_EXTERN Boolean DeleteNthAlign(SeqAlignPtr PNTR head, Int2 order)
{
	SeqAlignPtr curr, prev;
	Int2 num;

	curr = *head;
	num =0;
	prev = NULL;
	while(curr)
	{
		++num;
		if(order == num)
		{
			if(prev == NULL)
				*head = curr->next;
			else
				prev->next = curr->next;
			curr->next = NULL;
			SeqAlignFree(curr);
			return TRUE;
		}
		prev = curr;
		curr = curr->next;
	}
	return FALSE;
}


NLM_EXTERN Boolean Is_Local_Seq(SeqIdPtr hsip)
{

	SeqIdPtr sip;

	sip = find_sip(hsip);
	return (sip->choice == SEQID_LOCAL || sip->choice == SEQID_GENERAL || sip->choice == SEQID_OTHER);

}


NLM_EXTERN void write_out_put(SeqEntryPtr sep, CharPtr file_name, Boolean bin)
{
   AsnIoPtr aip;

        aip = AsnIoOpen(file_name, (bin? "wb" : "w"));
        SeqEntryAsnWrite(sep, aip, NULL);
        AsnIoClose(aip);
}


/***********************************************************************
***
*	functions for redrawing the Scaler for sequence
*
************************************************************************
***/
NLM_EXTERN Int4 slp_list_len(SeqLocPtr slp)
{
	Int4 seq_len;

	for(seq_len = 0; slp!= NULL; slp = slp->next)
		seq_len += SeqLocLen(slp);
	return seq_len;
}

NLM_EXTERN Int4 get_node_num(ValNodePtr vnp)
{
	Int4 i;
	
	for(i =0; vnp!=NULL; vnp = vnp->next)
		++i;
	return i;
}
	

NLM_EXTERN Boolean BioseqHasFeature(BioseqPtr bsp)
{
	SeqAnnotPtr annot;
	SeqFeatPtr sfp;
	SeqIdPtr sip;

	if(bsp == NULL || bsp->annot == NULL)
		return FALSE;
	for(annot = bsp->annot; annot != NULL; annot = annot->next)
		if(annot->type == 1)
			if(annot->data != NULL)
			{
				sfp = annot->data;
				if(sfp != NULL && sfp->location != NULL)
				{
					sip = SeqLocId(sfp->location);
					if(sip != NULL && BioseqMatch(bsp, sip))
						return TRUE;
				}
				return FALSE;
			}

	return FALSE;
}


NLM_EXTERN Int2 get_vnp_num(ValNodePtr vnp)
{
	Int2 num =0;
	
	while(vnp)
	{
		++num;
		vnp = vnp->next;
	}

	return num;
}



/*###################################################################
#
#	functions related to find a default master sequence in Seq-align
#	which will be used to display multiple pairwise alignment
#
###################################################################*/
typedef struct seqid_count{
	SeqIdPtr sip;
	Uint4 count;
}SeqIdCount, PNTR SeqIdCountPtr;

static ValNodePtr find_repeat(ValNodePtr head, SeqIdPtr sip)
{
	ValNodePtr vnp;
	SeqIdCountPtr sicp;

	for(vnp = head; vnp !=NULL; vnp = vnp->next)
	{
		sicp = vnp->data.ptrvalue;
		if(SeqIdMatch(sicp->sip, sip))
		{
			++(sicp->count);
			return head;
		}
	}
	sicp = MemNew(sizeof(SeqIdCount));
	sicp->sip = sip;
	sicp->count = 1;
	ValNodeAddPointer(&head, 0, (Pointer)sicp);
	return head;
}


static SeqIdPtr find_max(ValNodePtr vnp)
{
	Uint4 max;
	SeqIdPtr sip= NULL;
	SeqIdCountPtr sicp;


	max =0;
	while(vnp !=NULL)
	{
		sicp = vnp->data.ptrvalue;
		if(sicp->count >max)
		{
	    		max = sicp->count;
	    		sip = sicp->sip;
		}
	  	vnp = vnp->next;
	}

	return sip;
}


/*******************************************************************
*
*	make_master(sap_p)
*	return the Seq-id of the sequence with the highest appearance
*	frequency in a list of Seq-align
*	sap_p: a list of Seq-align
*	the return the Seq-id will be treated as the master in a 
*	multiple pairwise alignment
*
*******************************************************************/
NLM_EXTERN SeqIdPtr make_master( SeqAlignPtr sap_p)
{
    ValNodePtr vnp= NULL;   /*record the frequency of Seq-ids in alignment**/
    SeqIdPtr sip;
    DenseSegPtr dsp;
    DenseDiagPtr ddp;
    StdSegPtr ssp;
    SeqLocPtr slp;
    SeqAlignPtr sap;

    while(sap_p !=NULL) {
        switch(sap_p->segtype) {
        case 1:
            ddp = (DenseDiagPtr)(sap_p->segs);
            while(ddp) {
                for(sip = ddp->id; sip!=NULL; sip = sip->next)
                    vnp = find_repeat(vnp, sip);
                ddp = ddp->next;
            }
            break;
            
        case 2:
            dsp = (DenseSegPtr)(sap_p->segs);
            for(sip = dsp->ids; sip!=NULL; sip = sip->next)
                vnp = find_repeat(vnp, sip);
            break;
            
        case 3:
            ssp = (StdSegPtr)(sap_p->segs);
            for(slp = ssp->loc; slp != NULL; slp = slp->next) {
                sip = SeqLocId(slp);
                vnp = find_repeat(vnp, sip);
            }
            break;
        case 5:
            for(sap = sap_p->segs; sap != NULL; sap = sap->next) {
                sip = make_master(sap);
                vnp = find_repeat(vnp, sip);
            }
            break;
            
        default:
            break;
        }
        sap_p = sap_p->next;
    }
    
    sip = find_max(vnp);
    ValNodeFreeData(vnp);
    return sip;
}

NLM_EXTERN Boolean use_multiple_dimension(SeqAlignPtr align)
{
	DenseDiagPtr ddp;

	if(align->next != NULL)
		return FALSE;

	if(align->segtype == 1)
	{
		ddp = align->segs;
		return (ddp->next == NULL);
	}
	else
		return TRUE;
}


NLM_EXTERN void get_boundary(SeqIdPtr m_sip, Int4Ptr start, Int4Ptr stop, SeqAlignPtr align)
{
	Uint1 m_strand;
	Int4 m_start, m_stop;

	*start = -1;
	*stop = -1;
	while(align!=NULL)
	{
		get_align_ends(align, m_sip, &m_start, &m_stop, &m_strand);
		if(*start == -1)
		{
			*start = m_start;
			*stop = m_stop;
		}
		else
		{
			*start = MIN(m_start, (*start));
			*stop = MAX(m_stop, (*stop));
		}
		align = align->next;
	}
}


/*attach the information about the blast or the alignment type */
NLM_EXTERN void AddAlignInfoToSeqAnnotEx(SeqAnnotPtr annot, Uint1 align_type, CharPtr label)
{
	UserObjectPtr uop, b_uop;
	UserFieldPtr ufp;
	ObjectIdPtr oip;

	if(annot == NULL || annot->type != 2)
		return;

	oip = ObjectIdNew();
	oip->str = StringSave("Hist Seqalign");
	uop = UserObjectNew();
	uop->type = oip;

	oip = ObjectIdNew();
	oip->str = StringSave("Hist Seqalign");
	ufp = UserFieldNew();
	ufp->choice = 4;
	ufp->data.boolvalue = TRUE;
	ufp->label = oip;

	uop->data = ufp;


	ufp = UserFieldNew();
	oip = ObjectIdNew();
	if(label == NULL)
	{
		switch(align_type)
		{
			case 1:
				oip->str = StringSave("BLASTN");
				break;
			case 3:
				oip->str = StringSave("BLASTX");
				break;
			case 2:
				oip->str = StringSave("BLASTP");
				break;
			case 4:
				oip->str = StringSave("TBLASTN");
				break;
			default:
				break;
		}
	}
	else
		oip->str = StringSave(label);
	ufp->label = oip;
	ufp->choice = 2;
	ufp->data.intvalue = (Int4)align_type;

	oip = ObjectIdNew();
	oip->str = StringSave("Blast Type");
	b_uop = UserObjectNew();
	b_uop->type = oip;
	b_uop->data = ufp;

	/* uop->next = b_uop; */
	ValNodeAddPointer(&(annot->desc), Annot_descr_user, (Pointer)uop);
	ValNodeAddPointer(&(annot->desc), Annot_descr_user, (Pointer)b_uop);
}

NLM_EXTERN void AddAlignInfoToSeqAnnot(SeqAnnotPtr annot, Uint1 align_type)
{
	AddAlignInfoToSeqAnnotEx(annot, align_type, NULL);
}

NLM_EXTERN ValNodePtr SortValNode (ValNodePtr list, int (LIBCALLBACK *compar )PROTO ((Nlm_VoidPtr, Nlm_VoidPtr )))
{
	ValNodePtr tmp, PNTR head;
	Int4 count, i;

	if(list == NULL)
		return NULL;
	
	count = get_node_num(list);
	head = MemNew(((size_t)count+1) * sizeof (ValNodePtr));
	for(tmp = list, i=0; tmp !=NULL && i<count; ++i)
	{
		head[i] = tmp;
		tmp = tmp->next;
	}

	HeapSort(head, (size_t)count, sizeof(ValNodePtr), compar);
	for(i =0; i<count; ++i)
	{
		tmp = head[i];
		tmp->next = head[i+1];
	}
	list = head[0];
	MemFree(head);

	return list;
}

static Boolean get_align_score(SeqAlignPtr sap, FloatHiPtr score)
{

        ScorePtr scp;

        if(sap->score == NULL || score == NULL)
                return FALSE;
	*score = 0;
        scp = sap->score;
	if(scp->choice == 2)
	{
        	*score = scp->value.realvalue;
        	return TRUE;
	}
	else
		return FALSE;
}

typedef struct align_score {
	SeqAlignPtr align;
	FloatHi score;
}AlignScore, PNTR AlignScorePtr;

static int LIBCALLBACK AlignScoreCompProc(VoidPtr ptr1, VoidPtr ptr2)
{
  ValNodePtr   vnp1;
  ValNodePtr   vnp2;
  AlignScorePtr asp1, asp2;

  if (ptr1 != NULL && ptr2 != NULL) {
    vnp1 = *((ValNodePtr PNTR) ptr1);
    vnp2 = *((ValNodePtr PNTR) ptr2);
    if (vnp1 != NULL && vnp2 != NULL) {
      asp1 = (AlignScorePtr) vnp1->data.ptrvalue;
      asp2 = (AlignScorePtr) vnp2->data.ptrvalue;
      if (asp1 != NULL && asp2 != NULL) {
	if(asp1->score > asp2->score)
		return -1;
	else if(asp1->score < asp2->score)
		return 1;
	else
		return 0;
   }
  }
  }
  return 0;
}


NLM_EXTERN SeqAlignPtr sort_align_by_score(SeqAlignPtr PNTR halign)
{
	ValNodePtr list, curr;
	AlignScorePtr asp;
	Int4 score;
	FloatHi bit_score, evalue;
	Int4 number;
	Boolean comp = FALSE;
	SeqAlignPtr align, prev;

	list = NULL;
	align = *halign;
	while(align)
	{
		asp = MemNew(sizeof(AlignScore));
		asp->align = align;
		if(GetScoreAndEvalue(align, &score, &bit_score, &evalue, &number))
			asp->score = (FloatHi)score;
		else
			get_align_score(align, &(asp->score));
		if(asp->score > 0.0)
			comp = TRUE;
		ValNodeAddPointer(&list, 0, asp);
		align = align->next;
	}
	if(list == NULL)
		return NULL;
	if(list->next == NULL || !comp)
	{
		ValNodeFreeData(list);
		return (*halign);
	}

	list = SortValNode(list, AlignScoreCompProc);
	prev = NULL;
	for(curr = list; curr != NULL; curr = curr->next)
	{
		asp = curr->data.ptrvalue;
		asp->align->next = NULL;
		if(prev == NULL)
		{
			*halign = asp->align;
		}
		else
			prev->next = asp->align;
		prev = asp->align;
	}
	ValNodeFreeData(list);
	return (*halign);
}


