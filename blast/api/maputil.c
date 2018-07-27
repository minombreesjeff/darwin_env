#include <maputil.h>
#include <objsub.h>

/**************************************************
*
*	free_enzyme_list(enp)
*	Free a list of EnzDataPtr
*
**************************************************/
NLM_EXTERN EnzPtr free_enzyme_list (EnzPtr enp)
{
	EnzDataPtr edp;
	EnzPtr next;
 
	while(enp != NULL)
	{
		next = enp->next;
		edp = enp->data.ptrvalue;
		MemFree(edp->name);
		MemFree(edp->pattern);
		MemFree(edp);
 
		MemFree(enp);
		enp = next;
	}
 
	return enp;
 
}

/****************************************************
*
*	make_enzyme_list(file_name)
*	build a ValNodeList of EnzDataPtr from teh 
*	input file
*
****************************************************/
NLM_EXTERN EnzPtr make_enzyme_list(CharPtr file_name)
{
  Char buff[100];
  Char name[100];
  Char pattern[100];
  long cut_pos;
  EnzPtr head = NULL, e_new;
  EnzDataPtr data;
  Uint1 order =0;
  FILE *ifp;

	if(file_name == NULL)
	   return NULL;
	if((ifp = FileOpen(file_name, "r")) == NULL){
	  Message(MSG_ERROR, "fail to open Enzyme file %s", file_name);
	  return NULL;
	}

	while(FileGets(buff, 99, ifp) != NULL){
	      sscanf(buff, "%s\t%s\t%ld\n", name, pattern, &cut_pos);
	      ++order;
	      data = (EnzDataPtr)MemNew(sizeof(EnzData));
	      data->name = StringSave(name);
	      data->pattern = StringSave(pattern);
	      data->cut_pos = 1;
	      /**data->cut_pos = cut_pos;**/

	      e_new = ValNodeNew(head);
	      e_new->choice = order;
	      e_new->data.ptrvalue = data;
	      if(head == NULL)
		 head = e_new;
	}/**end of while**/

	FileClose(ifp);
	return head;
}


/**********************************************************************
*
*	FreeEquivAlign(ealign_list)
*	Free a list of Seq-annot that is of type Seq-align
*	mostly, those are the externally loaded Seq-align for showing
*	the Equiv map
*
***********************************************************************/
NLM_EXTERN ValNodePtr FreeEquivAlign(ValNodePtr ealign_list)
{
	SeqAnnotPtr annot;
	ValNodePtr next;

	while(ealign_list)
	{
		if(ealign_list->choice == 1)
		{
			annot = (SeqAnnotPtr)(ealign_list->data.ptrvalue);
			SeqAnnotFree(annot);
		}
		next = ealign_list->next;
		MemFree(ealign_list);
		ealign_list = next;
	}

	return NULL;
}
		

/*******************************************************************
*
*	GetEquivAlignType(annot)
*	annot stores the alignments of markers mapped by more than 
*	one groups. if return 1, the markers are the consistent markers
*	if return 2, the markers are inconsistent markers
*	if return 0, the alignment simply records the mapping to 
*	the sequence map
*	if return -1, unknown status. Will be treated the same as 1
*
*******************************************************************/
NLM_EXTERN Int2 GetEquivAlignType(SeqAnnotPtr annot)
{
	ValNodePtr vnp;
	UserObjectPtr uop;
	UserFieldPtr ufp;
	ObjectIdPtr oip;

	if(annot == NULL)
		return -1;
	for(vnp = annot->desc; vnp != NULL; vnp = vnp->next)
	{
		if(vnp->choice == Annot_descr_user)
		{
			uop = vnp->data.ptrvalue;
			if(uop != NULL)
			{
				oip = uop->type;
				if(oip->str && StringCmp(oip->str, 
					"Equiv Alignment") == 0)
				{
					ufp = uop->data;
					if(ufp->choice == 2)
						return (Int2)(ufp->data.intvalue);
				}
			}
		}
	}
	return -1;
}


/***********************************************************************
*
*	FreeMuskSep(sep_list)
*	Free the manually loaded Seq-entries
*	sep_list: a list of MuskSepPtr
*
***********************************************************************/
NLM_EXTERN ValNodePtr FreeMuskSep(ValNodePtr sep_list)
{
	ValNodePtr next;
	MuskSepPtr msp;

	while(sep_list)
	{
		next = sep_list->next;
		msp = (MuskSepPtr)(sep_list->data.ptrvalue);
		switch(msp->datatype)
		{
			case OBJ_SEQENTRY:
				SeqEntryFree((SeqEntryPtr)(msp->dataptr));
				break;
			case OBJ_SEQSUB :
				SeqSubmitFree(msp->dataptr);
				break;
			case OBJ_BIOSEQ :
				BioseqFree((BioseqPtr)(msp->dataptr));
				break;
			case OBJ_BIOSEQSET:
				BioseqSetFree((BioseqSetPtr)(msp->dataptr));
				break;
			default:
				break;
		}

		MemFree(msp);
		MemFree(sep_list);
		sep_list = next;
	}
	return NULL;
}


/**********************************************************************
*
*	get_Bioseq_type(bsp)
*	return the type of Bioseq, such as the genetic or physical map
*	depending on its bsp->repr type and bsp->seq_ext type.
*
**********************************************************************/
NLM_EXTERN Uint1 get_Bioseq_type(BioseqPtr bsp)
{
	SeqFeatPtr sfp;
	SeqAnnotPtr annot;

	switch (bsp->repr)
	{
		case Seq_repr_map:
			sfp = bsp->seq_ext;
			if(sfp == NULL)
			{
				annot = bsp->annot;
				while(annot)
				{
					if(annot->type == 1)
						sfp = annot->data;
					if(sfp != NULL)
						break;
				}
			}
			if(sfp == NULL)
				return PHYSICAL_MAP; 
			if(sfp->data.choice == 1)
				return GENETIC_MAP;
			if(sfp->data.choice == 13)
				return RESTRICTION_MAP;
			if(sfp->data.choice == 14)
				return CYTO_MAP;
			return PHYSICAL_MAP;

		case Seq_repr_virtual:
			return PHYSICAL_MAP;
		case Seq_repr_seg:
		case Seq_repr_ref:
		case Seq_repr_delta:
			return SEG_SEQ;
		default:
			return RAW_SEQ;
	}
}


/********************************************************************
*
*	MapLayoutFree(head)
*	Free the list of MapPosPtr
*
********************************************************************/
NLM_EXTERN MapLayoutPtr MapLayoutFree(MapLayoutPtr head)
{
	ValNodeFreeData(head);
	return NULL;
}


/***************************************************************************
*
*       getBioseqNumbering(bsp)
*       get the numbering object from Seq_descr. If no numbering, return NULL
*
****************************************************************************/
NLM_EXTERN NumberingPtr getBioseqNumbering (BioseqPtr bsp)
{
	NumberingPtr np = NULL;
	ValNodePtr anp;

                anp = BioseqGetSeqDescr(bsp, Seq_descr_num, NULL);
                if(anp != NULL)
                        np = (NumberingPtr)anp->data.ptrvalue;
		if(np != NULL)
		{
			if(np->choice != Numbering_real)
				return NULL;
			if(np->data.ptrvalue == NULL)
				return NULL;
		}
			

		return np;
}

/**********************************************************************
*
*   IS_NUM_GENE(gene_label): kludge function
*	determine if the gene_label is used as a map unit, such 
*	as the case for C.elegans physical map
*
**********************************************************************/
NLM_EXTERN Boolean IS_NUM_GENE(CharPtr gene_label)	
{
	return (gene_label[0]=='0' || atol(gene_label) != 0 );
}

/***********************************************************************
*
*       map_unit_label(): create a label for the map unit
*
***********************************************************************/
NLM_EXTERN Boolean map_unit_label(Int4 pos, NumberingPtr np, CharPtr label, Boolean use_kb)
{
  DataVal num_val;
  FloatHi m_val;

                if(np == NULL)
                {
		  if(use_kb)
                  	pos = pos/1000;
		  if(use_kb && pos != 0)
                  	sprintf(label, "%ldK", (long) pos);
		  else
			sprintf(label, "%ld", (long) pos);
                  return FALSE;
                }
                else
                {
                   NumberingValue(np, pos, &num_val);  /**Add the last line**/
                   m_val = num_val.realvalue;
		   sprintf(label, "%2.2Lf", (long double) m_val);
                   return TRUE;
                }
}


NLM_EXTERN Boolean start_new_stack(Int4 pre_pos, Int4 pos, Int4 scale, Int2Ptr label_width, Int2 c_width)
{
	Int2 max_width;
	Int4 space;

	max_width = MAX(c_width, *label_width);
	if(pre_pos != -1 &&  (*label_width > 0))  /*not the first mark*/
	{
		space = (ABS(pre_pos - pos))/scale;
		if(space < (max_width/2 +2))
			return FALSE;
	}

	*label_width = c_width;
	return TRUE;
}

/*************************************************************************
*
*	is_label_match(obj_id, label)
*	return TRUE if obj_id->str matches with label
*
**************************************************************************/

NLM_EXTERN Boolean is_label_match(ObjectIdPtr obj_id, CharPtr label)
{
        if(obj_id !=NULL && obj_id->str !=NULL)
                return (StringCmp(obj_id->str, label) == 0);
        return FALSE;
}


/*************************************************************************
*
*       get_band_type, get_band_name(): kludge functions associated with
*       the cytogenetic map, i.e., the band pattern and the name of the
*       cytogenetic map
*
**************************************************************************/
static Uint1 get_uop_type(UserObjectPtr uop)
{
	ObjectIdPtr obj_id;

	obj_id = uop->type;
        if(is_label_match(obj_id, "BND"))
                return BND;
        if(is_label_match(obj_id, "HET"))
                return HET;
        if(is_label_match(obj_id, "TEL"))
                return TEL;
        if(is_label_match(obj_id, "CEN"))
                return CEN;
        return 0;
}

NLM_EXTERN Uint1 get_band_type(UserObjectPtr uop)
{
   	UserFieldPtr ufp;
	Uint1 type;

	type = get_uop_type(uop);
	if(type !=0)
		return type;

        while(uop)
        {
           if(uop->data)
           {
               ufp = uop->data;
               while(ufp)
               { 
                   if(is_label_match(ufp->label, "BandType"))
		   {
                       if(StringCmp(ufp->data.ptrvalue, "GiemsaPos") ==0)
                                return GIEMSA_POS;
                       if(StringCmp(ufp->data.ptrvalue, "GiemsaNeg") ==0)
                                return GIEMSA_NEG;
                       if(StringCmp(ufp->data.ptrvalue, "Acrocentric") ==0)
                                return ACRO_CENTRIC;
                       if(StringCmp(ufp->data.ptrvalue, "Point") ==0)
                                return BAND_POINT;
                       if(StringCmp(ufp->data.ptrvalue, "VariableReg") ==0)
                                return VARIABLE_REG;
                       return 0;
                   }
                   ufp = ufp->next;
                }   
             }
 
             uop = uop->next;
        }
        return 0;
}



/*********************************************************************
*
*	get_band_name(uop)
*	parse the band name from a cytogenetic map
*
*********************************************************************/
NLM_EXTERN CharPtr get_band_name(UserObjectPtr uop)
{
	UserFieldPtr ufp;

        while(uop)
        {
		for(ufp = uop->data; ufp!=NULL; ufp=ufp->next)
		{
			if(is_label_match(ufp->label, "BandName"))
				return (CharPtr)(ufp->data.ptrvalue);
		}
		uop = uop->next;
	}
	return NULL;
}


/************************************************************************
*
*	is_map_segment(slp) 
*	return TRUE if slp is a Seq-loc from a amp
*	return FALSE if it is not a map or the Bioseq is not loaded to 
*	the memory yet
*
************************************************************************/
NLM_EXTERN Boolean is_map_segment(SeqLocPtr slp)
{
	BioseqPtr bsp=NULL;
	SeqLocPtr loc;
	SeqIdPtr sip;
	ObjectIdPtr oip;

	if(slp->choice == SEQLOC_NULL || slp->choice == SEQLOC_EMPTY)
		return TRUE;
	sip = SeqLocId(slp);
	if(sip == NULL)
		return TRUE;
	if(sip->choice == SEQID_LOCAL)
	{
		oip = sip->data.ptrvalue;
		if(oip && oip->str)
		{
			if(StringNCmp(oip->str, "virtual", 7) == 0)
				return TRUE;
			if(StringNCmp(oip->str, "virtural", 7) == 0)
				return TRUE;
		}
	}
	bsp = BioseqFindCore(sip);
	if(bsp !=NULL)
	{
		if(bsp->repr == Seq_repr_map)
			return TRUE;
		if(bsp->repr == Seq_repr_virtual)
			return TRUE;
		if(bsp->repr == Seq_repr_seg)
		{
			loc = bsp->seq_ext;
			return is_map_segment(loc);
		}
				
	}

	return FALSE;
}

/***************************************************************
*
*	figure_map_seqid(ext_loc)
*	a very unreliable way to figure out the if there is a 
*	Seq-id for the map. It is dependent on the frequency of 
*	the Seq-id in a  segmented sequence
*
****************************************************************/
typedef struct mapseqid_count{
	SeqIdPtr sip;
	Int4 count;
}MapSeqIdCount, PNTR MapSeqIdCountPtr;
#define COUNT_NUM 20

NLM_EXTERN SeqIdPtr figure_map_seqid(SeqLocPtr ext_loc)
{
	MapSeqIdCount id_count[COUNT_NUM];
	Int4 i, j, maxcount, total, c_total;
	Int4 n_maxcount;
	SeqIdPtr sip;
	BioseqPtr bsp;

	MemSet((Pointer)id_count, 0, (size_t)COUNT_NUM * sizeof(MapSeqIdCount));
	i =0; j= 0;
	while(ext_loc)
	{
		++i;
		sip = SeqLocId(ext_loc);
		for(j =0; j<COUNT_NUM; ++j)
		{
			if(id_count[j].sip == NULL)
			{
				id_count[j].sip = sip;
				id_count[j].count = 1;
				break;
			}
			else
			{
				if(SeqIdMatch(id_count[j].sip, sip))
				{
					++(id_count[j].count);
					break;
				}
			}
		}
		ext_loc = ext_loc->next;
	}
	total = i;

	/* if(total < COUNT_NUM)
		return NULL; */
	c_total = MIN(total, COUNT_NUM);
	maxcount = 0;
	n_maxcount = 0;
	j = 0;
	for(i = 0; i<c_total; ++i)
	{
		if(id_count[i].count > maxcount)
		{
			j = i;
			n_maxcount = maxcount;
			maxcount = id_count[i].count;
		}
	}
	sip = id_count[j].sip;
	if(sip == NULL)
		return NULL;
	if(sip->choice != SEQID_LOCAL && sip->choice != SEQID_GENERAL)
		return NULL;
	bsp = BioseqFind(sip);
	if(bsp != NULL)
	{
		if(bsp->repr == Seq_repr_raw || bsp->repr == Seq_repr_const)
			return NULL;
		if(bsp->repr == Seq_repr_map || bsp->repr == Seq_repr_virtual)
			return sip;
	}

	
	if((FloatHi)maxcount/(FloatHi)total > 0.3) 
		return (id_count[j].sip); 
	if(maxcount>n_maxcount) 
	{
		if(sip->choice == SEQID_LOCAL || sip->choice == SEQID_GENERAL)
			return sip;
		else
			return NULL;
	}
		
	return NULL;
}


NLM_EXTERN Uint1 ck_cyto_type(SeqFeatPtr sfp)
{
	UserObjectPtr uop;
	Uint1 band;

	if(sfp->data.choice == 14)
	{
		uop = sfp->data.value.ptrvalue;
		band = get_band_type(uop);
		if(band >= BND && band <= CEN)
			return FLY_CYTO;
		if(band >= BAND_POINT && band <= VARIABLE_REG)
			return HUMAN_CYTO;
	}

	return 0;
}
		
	

/********************************************************************
*
*	SortAlignPosition(app, dim)
*	Sort out the order of a multiple alignment in the vertical 
*	display mode. It is sorted to the descending order of 
*	app->top. one app correspond to one aligned segment. It can 
*	be of multiple dimensions
*	app: alignment position
*	dim: dimention of alignment
*
*********************************************************************/
NLM_EXTERN void SortAlignPosition(AlignPosPtr app, Int2 dim)
{
	Int4 tmp_left, tmp_right, tmp_top, tmp_bottom;
	Boolean s_witch = TRUE;
	Int2 i, j;

	for(i =0; i<dim-1 && s_witch; ++i)	/*sort the order*/
	{
		s_witch = FALSE;
		for(j = 0; j<dim-i-1; ++j)
		{
			if(app->top[j] > app->top[j+1])
			{
				s_witch = TRUE;
				tmp_left = app->left[j];
				tmp_right = app->right[j];
				tmp_top = app->top[j];
				tmp_bottom = app->bottom[j];

				app->left[j] = app->left[j+1];
				app->right[j] = app->right[j+1];
				app->top[j] = app->top[j+1];
				app->bottom[j] = app->bottom[j+1];
			
				app->left[j+1] = tmp_left;
				app->right[j+1] = tmp_right;
				app->top[j+1] = tmp_top;
				app->bottom[j+1] = tmp_bottom;
			}
		}
	}
}

NLM_EXTERN ValNodePtr free_slp_list(ValNodePtr slp_list)
{
	ValNodePtr next;
	SeqLocPtr slp;
	
	while(slp_list)
	{
		next = slp_list->next;
		slp = (SeqLocPtr)(slp_list->data.ptrvalue);
		if(slp != NULL)
			SeqLocSetFree(slp);
		slp_list->next = NULL;
		MemFree(slp_list);
		slp_list = next;
	}
	
	return NULL;
}

static void load_one_label(CharPtr label, ValNodePtr PNTR list, ValNodePtr PNTR prev)
{
	ValNodePtr n_vnp;

	if(label != NULL && label[0] != '\0')
	{
		n_vnp = ValNodeNew(NULL);
		n_vnp->data.ptrvalue = StringSave(label);
		if(*prev == NULL)
			*list = n_vnp;
		else
			(*prev)->next = n_vnp;
		*prev = n_vnp;
	}
}


static void load_gene_list(SeqFeatPtr sfp, ValNodePtr PNTR list, ValNodePtr PNTR prev)
{
	ObjMgrPtr omp;
	ObjMgrTypePtr omtp;
	Char label[21];
	GeneRefPtr grp;
	ValNodePtr syn;
	CharPtr str;
	Uint2 subtype;
	Uint1 val;
	ValNodePtr vnp;
	DbtagPtr db_tag;
	ObjectIdPtr oip;
	
	
	omp = ObjMgrGet();
	omtp = ObjMgrTypeFind (omp, OBJ_SEQFEAT, NULL, NULL);
	if(omp == NULL || omtp->labelfunc == NULL)
		return;
	if(omtp->subtypefunc == NULL)
		return;
		

	while(sfp)
	{
		subtype =  (*(omtp->subtypefunc)) (sfp);
		if(subtype == FEATDEF_GENE || subtype == FEATDEF_tRNA || subtype == FEATDEF_rep_origin || subtype == FEATDEF_CDS)
		{
			if(subtype == FEATDEF_CDS)
				val = CDS_MARK;
			else
				val = GENE_MARK;
			(*(omtp->labelfunc)) (sfp, label, 20, OM_LABEL_CONTENT);
			if(label[0] != '\0')
				load_one_label(label, list, prev);
				
			/*add synonym for the Gene-ref*/
			if(sfp->data.choice == 1)
			{
				grp = sfp->data.value.ptrvalue;
				if(grp != NULL)
				{
					for(syn = grp->syn; syn != NULL; syn = syn->next)
					{
						str = syn->data.ptrvalue;
						if(str != NULL)
							load_one_label(str, list, prev);
					}
					for(vnp = grp->db; vnp != NULL; vnp = vnp->next)
					{
						db_tag = vnp->data.ptrvalue;
						if(db_tag->db == NULL || StringICmp(db_tag->db, "GenBank"))
						{
							oip = db_tag->tag;
							if(oip != NULL && oip->str)
								load_one_label(oip->str, list, prev);
						}
					}
				}
			}
		}
		sfp = sfp->next;
	}
	
}


NLM_EXTERN Int2 get_seg_num(SeqLocPtr slp)
{
	Int2 i =0;
	
	while(slp)
	{
		++i;
		slp = slp->next;
	}
	
	return i;
}

#define MAX_SEG_NUM	4	/*maximum segment number to search for features in  segments*/
typedef struct findgene {    /* used by FindGeneCallback */
	SeqEntryPtr top;         /* top seqentry for explore.. used to prevent recursion */
	ValNodePtr PNTR list;    /* the list of feature labels */
	ValNodePtr PNTR prev;
} FindGeneStruct, PNTR FindGeneStructPtr;
			
static void FindGeneCallback(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	ValNodePtr PNTR list, PNTR prev;
	BioseqPtr bsp;
	BioseqSetPtr bssp;
	Int2 segnum;
	Boolean check_seg = FALSE;
	SeqEntryPtr s_sep;
	BioseqPtr s_bsp;
	SeqLocPtr slp;
	SeqIdPtr sip;
	SeqFeatPtr sfp;
	SeqAnnotPtr annot = NULL;
	FindGeneStructPtr fgsp;
	FindGeneStruct fgs;
	SeqEntryPtr last = NULL;

	fgsp = (FindGeneStructPtr)data;
	list = fgsp->list;
	prev = fgsp->prev;
	if(sep == NULL || fgsp == NULL || list == NULL || prev == NULL)
		return;

	if(sep->choice == 1)
	{
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		if(bsp == NULL)
			return;
		if(bsp->repr == Seq_repr_seg)
		{
			segnum = get_seg_num((SeqLocPtr)(bsp->seq_ext));
			if(segnum <= MAX_SEG_NUM || !BioseqHasFeature(bsp))
			{
				check_seg = TRUE;
				for(slp = bsp->seq_ext; slp != NULL; slp = slp->next)
				{
					sip = SeqLocId(slp);
					if(sip != NULL)
					{
						s_bsp = BioseqLockById(sip);
						if(s_bsp != NULL)
						{
							s_sep = SeqEntryFind(s_bsp->id);
							if((s_sep != NULL) && (s_sep != fgsp->top) && (s_sep != last))
							{
								fgs.top = s_sep;
								fgs.list = list;
								fgs.prev = prev;
								last = s_sep;
								SeqEntryExplore(s_sep, (Pointer) (&fgs), FindGeneCallback);
							}
							BioseqUnlock(s_bsp);
						}
					}
				}
			}
		}
		if(bsp->repr == Seq_repr_map)
		{
			sfp = bsp->seq_ext;
			load_gene_list(sfp, list, prev);
		}
		if(check_seg == FALSE)
			annot = bsp->annot;
	}
		
	else
	{
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		if(bssp == NULL)
			return;
		annot = bssp->annot;
	}
	
	while(annot)
	{
		if(annot->type == 1)
		{
			sfp = (SeqFeatPtr)(annot->data);
			load_gene_list(sfp, list, prev);
		}
		annot = annot->next;
	}

}


/*****************************************************************
*
*	Build a list of gene symbols to supply the Find Gene option in 
*   the global view
*
******************************************************************/
NLM_EXTERN ValNodePtr BuildGeneList(SeqEntryPtr sep)
{
	ValNodePtr list = NULL;
	ValNodePtr prev = NULL;
	FindGeneStruct fgs;
	
	if(sep == NULL)
		return FALSE;

	fgs.top = sep;
	fgs.list = &list;
	fgs.prev = &prev;
		
	SeqEntryExplore(sep, (Pointer) (&fgs), FindGeneCallback);
	
	return list;
	
}



typedef struct querydata {
	ValNodePtr query_list;
	GeneDataPtr gdp;
	SeqLocPtr target;
	
	ObjMgrPtr omp;
	ObjMgrTypePtr omtp;
	Char label[21];	
}QueryData, PNTR QueryDataPtr;

/***************************************************************
*
*	all the landmark genes need to be found ONLY ONCE. The user 
*	input data need to be found multiple times
*
****************************************************************/
static Boolean gene_is_loaded(GeneDataPtr gdp, CharPtr symbol)
{	
	if(symbol == NULL || gdp == NULL)
		return FALSE;
	
	while(gdp)
	{
		if(gdp->symbol)
			if(StringCmp(gdp->symbol, symbol) == 0)
				return TRUE;
		gdp = gdp->next;
	}
	
	return FALSE;
}

static SeqLocPtr dup_seq_loc(SeqLocPtr slp)
{
	Int4 start, stop;
	Uint1 strand;

	if(slp == NULL)
		return NULL;

	start = SeqLocStart(slp);
	stop = SeqLocStop(slp);
	strand = SeqLocStrand(slp);

	return SeqLocIntNew(start, stop, strand, SeqLocId(slp));
}

	

/***************************************************************** 
*
*       if sfp is a Gene-ref and contains the gene in g_list,   
*       return the string in g_list
*       else return NULL 
* 
*****************************************************************/ 
NLM_EXTERN Boolean check_landmark(SeqFeatPtr sfp, CharPtr mark)
{
	GeneRefPtr grp;
	ValNodePtr curr;
	DbtagPtr db_tag;
	ObjectIdPtr oip;

	if(sfp == NULL || sfp->data.choice != 1)
		return FALSE;
	grp = sfp->data.value.ptrvalue;
	if(grp != NULL)
	{
		if(grp->locus)
		{
			if(StringICmp(grp->locus, mark) == 0)
				return TRUE;
		}
		if(grp->syn)
		{
			if(check_syn(grp->syn, mark))
				return TRUE;
		}
		for(curr = grp->db; curr != NULL; curr = curr->next)
		{
			db_tag = curr->data.ptrvalue;
			if(db_tag)
			{
				oip = db_tag->tag;
				if(oip && oip->str)
				{
					if(StringICmp(oip->str, mark) == 0)
						return TRUE;
				}
			}
		}
	}

	return FALSE;
}
	
static Boolean gmarkfunc(GatherContextPtr gcp)
{
	QueryDataPtr qdp;
	
	GeneDataPtr gdp;
	GeneRefPtr grp;
	SeqFeatPtr sfp;
	ValNodePtr syn = NULL;
	Uint2 subtype;
	ValNodePtr curr;
	Boolean is_landmark;
	CharPtr match_str, str;
	Int4 start, stop;
	Uint1 strand;

	if(gcp == NULL)
		return FALSE;
	qdp = (QueryDataPtr)(gcp->userdata);
	if(qdp == NULL)
		return FALSE;
		
	if(gcp->thistype != OBJ_SEQFEAT && gcp->thistype != OBJ_BIOSEQ_MAPFEAT)
		return TRUE;
		
	sfp = (SeqFeatPtr)(gcp->thisitem);
	if(sfp == NULL)
		return TRUE;

	qdp->label[0] = '\0';		
	subtype =  (*(qdp->omtp->subtypefunc)) (sfp);
	if(subtype == FEATDEF_GENE || subtype == FEATDEF_tRNA || subtype == FEATDEF_rep_origin || subtype == FEATDEF_CDS)
	{
		if(subtype == FEATDEF_GENE)	/*load the synonom info*/
		{
			grp = sfp->data.value.ptrvalue;
			if(grp != NULL)
				syn = grp->syn;
		}
		
		(*(qdp->omtp->labelfunc)) (sfp, qdp->label, 20, OM_LABEL_CONTENT);
		for(curr = qdp->query_list; curr != NULL; curr = curr->next)
		{
			str = curr->data.ptrvalue;
			match_str = NULL;
			if(StringICmp(qdp->label, str) == 0)
				match_str = str;
			else if(check_landmark(sfp, str))
				match_str = str;
			if(match_str)
			{
				is_landmark = (curr->choice != 0);
				if(!is_landmark || !gene_is_loaded(qdp->gdp, match_str))
				{
					gdp = MemNew(sizeof (GeneData));
					gdp->landmark = is_landmark;
					gdp->symbol = StringSave(match_str);
					gdp->entityID = gcp->entityID;
					gdp->itemID = gcp->itemID;
					gdp->itemType = gcp->thistype;
					gdp->subtype = subtype;
					gdp->sfp = sfp;
					if(qdp->target == NULL)
						gdp->location = dup_seq_loc(sfp->location);
					else
					{
						start = gcp->extremes.left;
						stop = gcp->extremes.right;
						strand = gcp->extremes.strand;
						gdp->location = SeqLocIntNew(start, stop, strand, SeqLocId(qdp->target));
					}
					LinkGeneData(&(qdp->gdp), gdp);
				}
			}
		}
	}
	
	return TRUE;
	
}
		

/******************************************************************
*
*	load_gdata_marks(slp, gene_list, seglevels, sep, gdp)
*	Gather the current Seq-entry to create the corresponding list of 
*	GeneDataPtr for the list of gene symbols
*	slp: the target Seq-local. can be set to NULL
*	gene_list: a list of query symbols
*	seglevels: levels of gather
*	sep: the Seq-entry
*	gdp: the header of GeneDataPtr
*
********************************************************************/	
NLM_EXTERN Boolean load_gdata_marks(SeqLocPtr slp, ValNodePtr gene_list, Int2 seglevels, SeqEntryPtr sep, GeneDataPtr PNTR pgdp)
{
	GatherScope gs;
	QueryData qd;
	ObjMgrPtr omp;
	ObjMgrTypePtr omtp;
	
	if(gene_list == NULL || sep == NULL || pgdp == NULL)
		return FALSE;
		
	omp = ObjMgrGet();
	if(omp == NULL)
		return FALSE;
	omtp = ObjMgrTypeFind(omp, OBJ_SEQFEAT, NULL, NULL);
	if(omtp == NULL)
		return FALSE;
		
	qd.omp = omp;
	qd.omtp = omtp;
	qd.label[0] = '\0';
	qd.query_list = gene_list;
	qd.gdp = *pgdp;
	qd.target = slp;

	
	MemSet((Pointer)(&gs), 0, sizeof(GatherScope));
	MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)OBJ_MAX * sizeof(Boolean));
	gs.ignore[OBJ_SEQFEAT] = FALSE;
	gs.ignore[OBJ_SEQANNOT] = FALSE;
	gs.ignore[OBJ_BIOSEQ_MAPFEAT] = FALSE;
	gs.nointervals = TRUE;
	gs.target = slp;
	if(slp != NULL)
		gs.get_feats_location = TRUE;
	gs.seglevels = seglevels;

	GatherSeqEntry(sep, (Pointer)(&qd), gmarkfunc, &gs);
	
	if(*pgdp == NULL)
		*pgdp = qd.gdp;
	return TRUE;
}

typedef struct bsp_order {
	SeqIdPtr sip;
	Int4 order;
	Boolean found;
}BspOrder, PNTR BspOrderPtr;

static void FindBspOrder(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	BspOrderPtr bop;
	BioseqPtr bsp;

	if(sep->choice != 1)
		return;
	if(data == NULL)
		return;

	bop = (BspOrderPtr)data;
	if(bop->found)
		return ;

	bsp = sep->data.ptrvalue;
	++(bop->order);
	bop->found = BioseqMatch(bsp, bop->sip);

}


static Boolean get_current_priority(SeqIdPtr sip, SeqEntryPtr sep, Int4Ptr order)
{
	BspOrder bo;

	bo.sip = sip;
	bo.order = 0;
	bo.found = FALSE;


	if(sep != NULL && sip != NULL)
		BioseqExplore(sep, (Pointer)&bo, FindBspOrder);
	*order = bo.order;

	return bo.found;

}

NLM_EXTERN Int2 get_seglevels (BioseqPtr bsp)
{
	SeqLocPtr slp;
	

	if(bsp && bsp->repr == Seq_repr_seg)
	{
		if(!BioseqHasFeature(bsp))
			return 1;
		slp = (SeqLocPtr)(bsp->seq_ext);
		if(get_seg_num(slp) <= 4)	/*4 is the cut off for searching at the second level*/
			return 1;
	}
	
	return 0;
}

/*************************************************************************
*
*	return the best location of the gene from sep.
*	best is defined as the Bioseq with the highest searching prioirity
*	e_start, e_stop record the extremes of the all the presence of 
*	the gene
*
*************************************************************************/
NLM_EXTERN SeqLocPtr get_location_for_query(SeqEntryPtr sep, CharPtr gene, Int4Ptr e_start, Int4Ptr e_stop)
{
	ValNode vn;
	GeneDataPtr gdp = NULL, cgdp, best;
	Int2 seglevels;
	BioseqPtr bsp;
	SeqLocPtr slp;
	SeqLoc sl;
	Int4Ptr priority_list;
	Int4 num;
	Int4 left = -1, right = -1;
	Int4 order;

	if(sep == NULL || gene == NULL)
		return NULL;

	bsp = find_big_bioseq(sep);
	if(bsp == NULL)
		return NULL;
	seglevels = get_seglevels(bsp);

	vn.choice = 0;
	vn.data.ptrvalue = gene;
	vn.next = NULL;


	if(seglevels == 1)
	{
		sl.choice = SEQLOC_WHOLE;
		sl.data.ptrvalue = bsp->id;
		sl.next = NULL;
		slp = &sl;
	}
	else
		slp = NULL;

	if(!load_gdata_marks(slp, &vn, seglevels, sep, &gdp))
		return NULL;

	priority_list = get_priority_order(sep, &num);
	best = NULL;
	for(cgdp = gdp; cgdp != NULL; cgdp = cgdp->next)
	{
		cgdp->priority = 10000;
		if(cgdp->location != NULL)
		{
			if(left == -1)
				left = SeqLocStart(cgdp->location);
			else
				left = MIN(left, SeqLocStart(cgdp->location));
			if(right == -1)
				right = SeqLocStop(cgdp->location);
			else
				right = MAX(right, SeqLocStop(cgdp->location));
		}

		if(num > 0 && priority_list != NULL)
		{
			if(get_current_priority(SeqLocId(cgdp->location), sep, &order))
			{
				if(order >=0 && order <= num)
					cgdp->priority = (Uint2)(priority_list[order-1]);
			}
		}
		if(best == NULL)
			best = cgdp;
		else
		{
			if(cgdp->priority < best->priority)
				best = cgdp;
		}
	}

	if(best != NULL)
	{
		slp = best->location;
		best->location = NULL;
	}
	*e_start = left;
	*e_stop = right;
	GeneDataFree(gdp);
	return slp;
}	

				
	

typedef struct ra_store{
	ValNodePtr rrp_list;	/*a collected list for repeats*/
	ValNodePtr arp_list;	/*a collected list for alignment*/
	ObjMgrPtr omp;
	ObjMgrTypePtr omtp;
	Char annotDB[21];	/*for storing the Seq-annot info*/
	Boolean load_align;
	Uint1 displayOrder;
}RAStore, PNTR RAStorePtr;

static void add_repeats_to_list(RepeatRegionPtr r_new, ValNodePtr PNTR list)
{
	Uint1 max_order = 0;
	ValNodePtr curr, prev = NULL;
	RepeatRegionPtr rrp;
	Boolean found = FALSE;
	ValNodePtr vnp;
	
	
	if(*list == NULL)
		ValNodeAddPointer(list, 1, r_new);
	else
	{
		curr = *list;
		while(curr)
		{
			rrp = curr->data.ptrvalue;
			if(rrp->rep_name[0] == '\0' && r_new->rep_name[0] == '\0')
				found = TRUE;
			if(StringICmp(rrp->rep_name, r_new->rep_name) == 0)
				found = TRUE;
			if(found)
			{
				vnp = ValNodeNew(NULL);
				vnp->choice = curr->choice;
				vnp->data.ptrvalue = r_new;
				vnp->next = curr->next;
				curr->next = vnp;
				return;
			}
			else
			{
				max_order = MAX(max_order, curr->choice);
				prev = curr;
			}
			curr = curr->next;
		}
		vnp = ValNodeNew(prev);
		vnp->choice = max_order + 1;
		vnp->data.ptrvalue = r_new;
	}
}

/*trying to parse the descriptor in Seq-annot for the alignment display*/
NLM_EXTERN Uint1 get_align_annot_qual(SeqAnnotPtr annot, CharPtr annotDB, Int4 buf_size, Uint1Ptr annot_type)
{
	UserObjectPtr uop;
	ValNodePtr desc;
	ObjectIdPtr oip;
	UserFieldPtr ufp;
	Boolean match;

	if(annot == NULL || annot->type !=2)
		return 0;
	desc = annot->desc;
	match = FALSE;
	*annot_type = 0;
	while(desc)
	{
		if(desc->choice == Annot_descr_user)
		{
			uop = desc->data.ptrvalue;
			while(uop)
			{
				if(uop->type)
				{
					oip = uop->type;
					if(StringCmp(oip->str, "Align Consist?") == 0)
					{
						*annot_type = ANNOT_CONSIST;
						match = TRUE;
					}
					else if(StringCmp(oip->str, "Blast Type") == 0)
					{
						match = TRUE;
						*annot_type = ANNOT_BLAST;
					}
					else if(StringCmp(oip->str, "FISH Align") == 0)
						*annot_type = ANNOT_FISH;
					if(match)
					{
						ufp = uop->data;
						if(ufp && ufp->choice == 2)
						{
							oip = ufp->label;
							if(oip->str && annotDB != NULL)
							{
								StringNCpy_0(annotDB, oip->str, buf_size);
							}
							return (Uint1)(ufp->data.intvalue);
						}
					}	
				}	
				uop = uop->next;
			}
		}
		desc = desc->next;
	}


	desc = annot->desc;
	while(desc)
	{
		if(desc->choice == Annot_descr_user)
		{
			uop = desc->data.ptrvalue;
			while(uop)
			{
				if(uop->type)
				{
					oip = uop->type;
					if(StringCmp(oip->str, "Hist Seqalign") == 0)
					{
						ufp = uop->data;
						if(ufp->choice == 4 && ufp->data.boolvalue)
						{
							oip = ufp->label;
							if(oip && oip->str && annotDB)
							{
								StringNCpy_0(annotDB, oip->str, buf_size);
								return 0;
							}
						}
					}
				}
				uop = uop->next;
			}
		}
		desc = desc->next;
	}
	return 0;
}


static void load_new_interval(ValNodePtr PNTR intervals, Int4 left, Int4 right)
{
	GatherRangePtr grp, t_grp;
	ValNodePtr curr, prev, next, vnp;
	Int4 p_right;

	prev = NULL;
	/*overlaps the existing segment*/
	curr = *intervals;
	while(curr)
	{
		grp = curr->data.ptrvalue;
		if(!(left > (grp->right+1) || right < (grp->left -1)))
		{
			grp->left = MIN(left, grp->left);
			grp->right = MAX(right, grp->right);
			vnp = curr;
			/*check for the following segment*/
			curr = curr->next;
			while(curr)
			{
				next = curr->next;
				t_grp = curr->data.ptrvalue;
				if(t_grp->left <= grp->right +1)
				{
					grp->right = t_grp->right;
					vnp->next = next;
					curr->next = NULL;
					ValNodeFreeData(curr);
					curr = next;
				}
				else
					break;
			}
			return;
		}
		curr = curr->next;
	}
	
	/*No overlap*/
	grp = MemNew(sizeof(GatherRange));
	grp->left = left;
	grp->right = right;
	vnp = ValNodeNew(NULL);
	vnp->choice =  1;
	vnp->data.ptrvalue = grp;
	if(*intervals == NULL)
	{
		*intervals = vnp;
		return;
	}

	curr = *intervals;
	p_right = -1;
	prev = NULL;
	while(curr)
	{
		grp = curr->data.ptrvalue;
		if(p_right == -1)	/*it is the first segment*/
		{
			if(right < grp->left)
			{
				*intervals = vnp;
				vnp->next = curr;
				return;
			}
		}
		else if(left >= p_right+1 && right <= grp->left -1)
		{
			prev->next = vnp;
			vnp->next = curr;
			return;
		}

		p_right = grp->right;
		prev = curr;
		curr = curr->next;
	}

	prev->next = vnp;
}
		
static Boolean add_alignment_to_list(AlignRegionPtr a_new, ValNodePtr PNTR list)
{
	ValNodePtr curr;
	AlignRegionPtr arp;
	ValNodePtr vnp, prev;
	
	
	curr = *list;
	prev = NULL;
	while(curr)
	{
		arp = curr->data.ptrvalue;
		if(arp->displayOrder == a_new->displayOrder && 
			StringCmp(arp->seq_name, a_new->seq_name) == 0)
		{
			curr->choice += 1;
			arp->gr.left = MIN(arp->gr.left, a_new->gr.left);
			arp->gr.right = MAX(arp->gr.right, a_new->gr.right);
			load_new_interval(&(arp->intervals), a_new->gr.left, a_new->gr.right);
			if(a_new->score > arp->score)
			{
				arp->score = a_new->score;
				arp->p_val = a_new->p_val;
				arp->e_val = a_new->e_val;
			}
			MemFree(a_new);
			return FALSE;
		}
		prev = curr;
		curr = curr->next;
	}
	
	vnp = ValNodeNew(prev);
	vnp->choice = 1;
	vnp->data.ptrvalue = a_new;
	if(prev == NULL)
		*list = vnp;
	load_new_interval(&(a_new->intervals), a_new->gr.left, a_new->gr.right);
	return TRUE;
}
		
			
/*determine the status as absolute values*/
static Uint1 get_alignment_status(FloatHi score)
{
	if(score < 40.0)
		return 0;
	if(score <50.0)
		return 1;
	if(score < 80.0)
		return 2;
	if(score < 200)
		return 3;
	return 4;
}

static int LIBCALLBACK ArpNodeCompProc (VoidPtr ptr1, VoidPtr ptr2)
{
	AlignRegionPtr arp1, arp2;
	ValNodePtr   vnp1;
	ValNodePtr   vnp2;
	GatherRange   gr1, gr2;

	if (ptr1 != NULL && ptr2 != NULL)
	{
		vnp1 = *((ValNodePtr PNTR) ptr1);
		vnp2 = *((ValNodePtr PNTR) ptr2);
		if (vnp1 != NULL && vnp2 != NULL)
		{
			arp1 = (AlignRegionPtr) vnp1->data.ptrvalue;
			arp2 = (AlignRegionPtr) vnp2->data.ptrvalue;
			if (arp1 != NULL && arp2 != NULL)
			{
				if(arp2->score > arp1->score)
					return 1;
				if(arp1->score > arp2->score)
					return -1;
				
				gr1 = arp1->gr;
				gr2 = arp2->gr;
				if (gr1.left > gr2.left)
					 return 1;
				else if (gr1.left < gr2.left)
					return -1;
				else if (gr1.right < gr2.right)
					return 1;
				else if (gr1.right > gr2.right)
					return -1;
				return 0;
			}
		}
	}
	return 0;
}

static ValNodePtr sort_arp_list(ValNodePtr arp_list)
{
	return SortValNode(arp_list, ArpNodeCompProc);
}

static Boolean racollfunc (GatherContextPtr gcp)
{
	RAStorePtr rasp;
	Uint2 subtype;
	RepeatRegionPtr rrp;
	AlignRegionPtr arp;
	AlignDataPtr adp;
	SeqAnnotPtr annot;
	Uint1 annot_type;
	Int4 score, number;
	
	
	rasp = (RAStorePtr)(gcp->userdata);
	if(rasp == NULL)
		return FALSE;
		
	switch(gcp->thistype)
	{
		case OBJ_SEQANNOT:
			annot = (SeqAnnotPtr)(gcp->thisitem);
			if(annot->type == 2)	/*it is a Seq-annot for alignment*/
			{
				rasp->load_align = is_annot_for_hist_alignment(annot);
				if(rasp->load_align)
				{
					rasp->annotDB[0] = '\0';
					get_align_annot_qual(annot, rasp->annotDB, 20, &annot_type); 
					++(rasp->displayOrder);
				}
			}
			return TRUE;

		case OBJ_SEQFEAT:
			if(rasp->omtp == NULL)
				return TRUE;
			if (rasp->omtp->subtypefunc != NULL)
			{
				subtype = (*(rasp->omtp->subtypefunc)) (gcp->thisitem);
				if(subtype == FEATDEF_repeat_region || subtype == FEATDEF_repeat_unit)
				{
					rrp = MemNew(sizeof(RepeatRegion));
					MemCopy(&(rrp->gr), &(gcp->extremes), sizeof(GatherRange));
					(*(rasp->omtp->labelfunc)) (gcp->thisitem, rrp->rep_name, 19, OM_LABEL_CONTENT);
					add_repeats_to_list(rrp, &(rasp->rrp_list));
				}
			}
			return TRUE;
			
		case OBJ_SEQALIGN:
		case OBJ_SEQHIST_ALIGN:
			if(gcp->thistype == OBJ_SEQALIGN)
			{
				if(gcp->parenttype == OBJ_SEQANNOT)
				{
					if(rasp->load_align == FALSE)
						return TRUE;
				}
			}

			for(adp = gcp->adp; adp != NULL; adp = adp->next)
			{
				arp = MemNew(sizeof(AlignRegion));
				arp->annotDB[0] = '\0';
				arp->displayOrder = 0;
				score = 0;
				arp->score = -1.0;
				GetScoreAndEvalue((SeqAlignPtr)gcp->thisitem, &score, &(arp->score), &(arp->e_val), &number);
				if(arp->score <=0 && score > 0)
				{
					arp->score = (FloatHi)score;
				}

				if(gcp->thistype == OBJ_SEQALIGN)
				{
					if(gcp->parenttype == OBJ_SEQANNOT)
					{
						arp->displayOrder = rasp->displayOrder;
						StringCpy(arp->annotDB, rasp->annotDB);
					}
				}
				MuskSeqIdWrite(adp->sip, arp->seq_name, 19, PRINTID_TEXTID_ACCESSION, FALSE, FALSE); 
				MemCopy(&(arp->gr), &(adp->extremes), sizeof(GatherRange));
				add_alignment_to_list(arp, &(rasp->arp_list));
			}

			return TRUE;
		default:
			return TRUE;
	}
}

/* static Uint1 get_alignment_status(FloatHi score, FloatHi min_score, FloatHi max_score)
{
	FloatHi val;

	if(min_score == max_score)
		return 0;
	else
	{
		if(score == max_score)
			return 4;

		val = (score - min_score)/(max_score - min_score);
		return (Uint1)(val * 5.0);
	}
} */



/**************************************************************************
*
*	collect_repeats_and_align(slp, rrp_list, arp_list, seglevels, sep)
*
*	collect repeat features and alignment for global display
*	rrp_list: the list of the repeat features
*	arp_list: the list of the alignments
*
***************************************************************************/				
NLM_EXTERN Boolean collect_repeats_and_align(SeqLocPtr slp, ValNodePtr PNTR rrp_list, ValNodePtr PNTR arp_list, Int2 seglevels, SeqEntryPtr sep, Uint1Ptr align_has_status)
{
	GatherScope gs;
	RAStore ras;
	ObjMgrPtr omp;
	ObjMgrTypePtr omtp;
	AlignRegionPtr arp, n_arp;
	ValNodePtr prev, next, curr, p_last;
	
	*align_has_status = FALSE;
	if(slp == NULL || sep == NULL || (rrp_list == NULL && arp_list == NULL))
		return FALSE;
		
	omp = ObjMgrGet();
	if(omp == NULL)
		return FALSE;
	omtp = ObjMgrTypeFind(omp, OBJ_SEQFEAT, NULL, NULL);
	ras.omp = omp;
	ras.omtp = omtp;
	ras.rrp_list = NULL;
	ras.arp_list = NULL;
	ras.annotDB[0] = '\0';
	ras.displayOrder = 0;
	ras.load_align = TRUE;
		
	
	MemSet((Pointer)(&gs), 0, sizeof(GatherScope));
	MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)OBJ_MAX * sizeof(Boolean));
	if(rrp_list != NULL)
		gs.ignore[OBJ_SEQFEAT] = FALSE;
	if(arp_list != NULL)
		gs.ignore[OBJ_SEQHIST_ALIGN] = FALSE;
	gs.ignore[OBJ_SEQHIST] = FALSE;
	gs.ignore[OBJ_SEQANNOT] = FALSE;
	gs.ignore[OBJ_SEQALIGN] = FALSE;

	gs.nointervals = TRUE;
	gs.target = slp;
	gs.get_feats_location = TRUE;
	gs.seglevels = seglevels;
	gs.mapinsert = TRUE;

	GatherSeqEntry(sep, (Pointer)(&ras), racollfunc, &gs);
	if(rrp_list != NULL)
		*rrp_list = ras.rrp_list;
	if(arp_list != NULL)
	{
		*arp_list = ras.arp_list;
		if(*arp_list != NULL)
		{
			/*sort arp_list in groups according to the displayOrder*/
			curr = *arp_list;
			p_last = NULL;
			while(curr)
			{
				arp = curr->data.ptrvalue;
				next = curr->next;
				prev = curr;
				while(next)
				{
					n_arp = next->data.ptrvalue;
					if(n_arp->displayOrder == arp->displayOrder)
					{
						prev = next;
						next = next->next;
					}
					else
						break;
				}
				prev->next = NULL;
				curr = sort_arp_list(curr);
				if(p_last == NULL)
					*arp_list = curr;
				else
					p_last->next = curr;

				while(curr->next != NULL)
					curr = curr->next;
				p_last = curr;
				curr = next;
			}


			for(curr = *arp_list; curr != NULL; curr = curr->next)
			{
				arp = curr->data.ptrvalue;
				arp->status = get_alignment_status(arp->score);
				if(arp->status > 0)
					*align_has_status = TRUE;
			}
		}
	}
	/* *max_displayOrder = ras.displayOrder; */
	return TRUE;
}




/*###################################################################
#
#	functions related to make_Bioseq_list
#
####################################################################*/

static void FindBspCallback(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	ValNodePtr PNTR bsp_list;
	BioseqPtr bsp;
	Uint1 choice;

	if(sep->choice == 1)
	{
		bsp_list = (ValNodePtr PNTR)data;
		bsp = sep->data.ptrvalue;
		choice = 0;
		if(IsSeqIndexMap(bsp))
			choice = SEQINDEX_VAL;
		ValNodeAddPointer(bsp_list, choice, (Pointer)bsp);
	}
}

static Boolean is_EQUIV(SeqEntryPtr sep)
{
	BioseqSetPtr bssp;

	if(sep->choice == 2)
	{
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
			return (bssp->_class == 10);
	}
	return FALSE;
}

NLM_EXTERN Int4Ptr get_priority_order(SeqEntryPtr sep, Int4Ptr num)
{
	BioseqSetPtr bssp;
	SeqDescrPtr desc;
	UserObjectPtr uop;
	UserFieldPtr ufp;
	ObjectIdPtr oip;

	if(sep == NULL || sep->choice !=2)
		return NULL;
	bssp = sep->data.ptrvalue;
	if(bssp == NULL || bssp->_class != 10)
		return NULL;

	for(desc = bssp->descr; desc != NULL; desc = desc->next)
	{
		if(desc->choice == Seq_descr_user)
		{
			uop = desc->data.ptrvalue;
			oip = uop->type;
			if(oip != NULL && oip->str != NULL)
			{
				if(StringCmp(oip->str, "Equiv Search Priority") == 0)
				{
					ufp = uop->data;
					if(ufp->choice == 8)	
					{
						if(num != NULL)
							*num = ufp->num;
						return (Int4Ptr)(ufp->data.ptrvalue);
					}
				}
			}
		}
	}

	return NULL;
}
 

	
	


/*************************************************************
*
*	check the Seq-annot to see if it is designed to be displayed 
*	as a sequence history or not
*
**************************************************************/
NLM_EXTERN Boolean is_annot_for_hist_alignment(SeqAnnotPtr annot)
{
	UserObjectPtr uop;
	ValNodePtr desc;
	ObjectIdPtr oip;
	UserFieldPtr ufp;

	if(annot == NULL || annot->type !=2)
		return FALSE;
	desc = annot->desc;
	while(desc)
	{
		if(desc->choice == Annot_descr_user)
		{
			uop = desc->data.ptrvalue;
			while(uop)
			{
				if(uop->type)
				{
					oip = uop->type;
					if(StringCmp(oip->str, "Hist Seqalign") == 0)
					{
						ufp = uop->data;
						if(ufp && ufp->choice == 4)
							return (ufp->data.boolvalue);
					}	
				}	
				uop = uop->next;
			}
		}
		desc = desc->next;
	}

	return FALSE;
}


NLM_EXTERN ValNodePtr get_equiv_align(SeqEntryPtr sep)
{
	BioseqSetPtr bssp;
	SeqAnnotPtr annot;
	ValNodePtr ealign_list = NULL;
	SeqAlignPtr align;

	if(is_EQUIV(sep))
	{
		bssp = sep->data.ptrvalue;
		annot = bssp->annot;
		while(annot)
		{
			if(annot->type == 2)
			{
				align = annot->data;
				if(align && align->segtype == 3)
				{	/*has to be Std-seg */
				if(!is_annot_for_hist_alignment(annot))	/*protection for cases that might include the alignment shown as history*/
					ValNodeAddPointer(&ealign_list, 0, annot);
				}
			}
			annot = annot->next;
		}
	}

	return ealign_list;
}




NLM_EXTERN Boolean make_Bioseq_list(SeqEntryPtr sep, ValNodePtr PNTR bsp_list, ValNodePtr PNTR equiv_align)
{
	BioseqPtr bsp;
	ValNodePtr sep_align_list;
	ValNodePtr this_list;
	Int4Ptr priority_order;
	Int4 num, i;
	ValNodePtr curr;

	if(sep == NULL || bsp_list == NULL)
		return FALSE;
		
	if(is_EQUIV(sep))	/*is it an Equiv-seg of Bioseqs*/
	{
		priority_order = get_priority_order(sep, &num);
		this_list = NULL;
		SeqEntryExplore(sep, (Pointer) (&this_list), FindBspCallback);
		for(i= 0, curr = this_list; curr != NULL && i<num; curr = curr->next)
		{
			if(curr->choice != SEQINDEX_VAL)
			{	/*the sequence index map is excluded from 
				  search priority */
				if(priority_order != NULL)
					curr->choice = (Uint1)(priority_order[i++]);
				else
					curr->choice = SEQINDEX_VAL-1;
			}
		}
		ValNodeLink(bsp_list, this_list);

		if(equiv_align != NULL)
		{
			sep_align_list = get_equiv_align(sep);
			ValNodeLink(equiv_align, sep_align_list);
		}
	}
	else
	{
		bsp = find_big_bioseq(sep);
		if(bsp != NULL)
			ValNodeAddPointer(bsp_list, 1, (Pointer)bsp);
	}
	return TRUE;
}

#define MINDIST         100             /*for drawing the dynamic scaler*/

NLM_EXTERN Int4 calculate_ruler(Int4 scaleX)
{
  FloatHi logDist;
  FloatHi minDist;
  FloatHi nextPower;
  Int4 ruler;


	  minDist = (double) (MINDIST * scaleX);
	  logDist = log10 (minDist);
	  nextPower = (exp(ceil(logDist) * NCBIMATH_LN10));
	  if(minDist < nextPower /5.0)
	     ruler = (Int4)( (nextPower + 0.5)/5.0 );
	  else if (minDist <nextPower/4){
	     ruler = (Int4)( (nextPower + 0.5)/4.0 );
	   }
	   else if(minDist <nextPower/2){
	      ruler = (Int4)( (nextPower+0.5)/2.0 );
	   }
	   else {
	     ruler = (Int4) (nextPower+0.5);
	  }
	  return ruler;
}


NLM_EXTERN GeneDataPtr LinkGeneData(GeneDataPtr PNTR head, GeneDataPtr g_new)
{
	GeneDataPtr curr;

	if(*head == NULL)
		*head = g_new;
	else
	{
		curr = *head;
		while(curr->next != NULL)
			curr = curr->next;
		curr->next = g_new;
	}

	return (*head);
}


/****************************************************************
*
*	LoadLandMarkGene(sep)
*	get the landmark gene from the User-object in the descriptor
*	all the genes are linked to a ValNode and vnp->choice is set 
*	to 1 to indicate it is a landmark gene
*
******************************************************************/
NLM_EXTERN ValNodePtr LoadLandMarkGene(SeqEntryPtr sep)
{
	BioseqPtr bsp;
	BioseqSetPtr bssp;
	SeqDescrPtr descr = NULL;
	UserObjectPtr uop;
	UserFieldPtr ufp;
	CharPtr symbol;
	ValNodePtr list = NULL;

	if(sep == NULL)
		return NULL;

	if(sep->choice == 1)
	{
		bsp = sep->data.ptrvalue;
		descr = bsp->descr;
	}
	else
	{
		bssp = sep->data.ptrvalue;
		descr = bssp->descr;
	}

	while(descr)
	{
		if(descr->choice == Seq_descr_user)
		{
			uop = descr->data.ptrvalue;
			while(uop)
			{
				if(uop->type != NULL)
				{
					if(is_label_match(uop->type, "LandMark"))
					{
						ufp = uop->data;
						while(ufp)
						{
							if(ufp->choice == 1)
							{
								symbol = (CharPtr)(ufp->data.ptrvalue);
								if(symbol != NULL)
									ValNodeCopyStr(&list, 1, symbol);
									/*set choice =1 to indicate it is a landmark gene*/
							}
							ufp = ufp->next;
						}
					}
				}
				uop = uop->next;
			}
		}
		descr = descr->next;
	}
	return list;

}

NLM_EXTERN Boolean BioseqHasLandMark(BioseqPtr bsp)
{
	ValNodePtr descr;
	UserObjectPtr uop;

	if(bsp == NULL)
		return FALSE;

	for(descr = bsp->descr; descr != NULL; descr = descr->next)
	{
		if(descr->choice == Seq_descr_user)
		{
			uop = descr->data.ptrvalue;
			if(uop->type != NULL)
			{
				if(is_label_match(uop->type, "LandMark"))
					return TRUE;
			}
		}
	}

	return FALSE;
}


NLM_EXTERN GeneDataPtr GeneDataFree(GeneDataPtr head)
{
	GeneDataPtr next;

	while(head != NULL)
	{
		next = head->next;
		MemFree(head->symbol);
		SeqLocFree(head->location);
		ValNodeFree(head->align_seg);
		MemFree(head);
		head = next;
	}
	return head;
}


/***********************************************************************
*
*	for each sequence in alignment stored in Seq-hist, if the aligned 
*	sequence itself contains alignment, it is temporarily loaded as 
*	a user-object in the descriptor of the bioseq. This function extract 
*	the information from the descripor and store it as a list of gi's 
*	plus the kludge offset value
*
************************************************************************/
NLM_EXTERN ValNodePtr get_seqids_with_alignment(BioseqPtr mbsp)
{
	ValNodePtr descr;
	UserObjectPtr uop;
	UserFieldPtr ufp;
	ValNodePtr align_id_list = NULL;
	ObjectIdPtr oip;
	Int4 gi;
	
	if(mbsp == NULL)
		return NULL;
		
	for (descr = mbsp->descr; descr != NULL; descr = descr->next)
	{
		if(descr->choice == Seq_descr_user)
		{
			uop = descr->data.ptrvalue;
			if(uop->type != NULL)
			{
				if(is_label_match(uop->type, "History"))
				/*temporary criteria for storing the seq-id with alignment*/
 				{
 					ufp = uop->data;
 					while(ufp)
 					{
 						if(ufp->choice ==2)	/*it is an integer*/
 						{
 							gi = ufp->data.intvalue;
 							oip = ufp->label;
 							/*oip->id is the kludge offset factor*/
 							ValNodeAddInt(&align_id_list, (Uint1)(oip->id), gi);
 						}
 						ufp = ufp->next;
 					}
 				}
 			}
 		}
 	}
 	return align_id_list;
}


/***********************************************************************
*
*	map the kludge offet factor for Unigene, RICE, MOUSE ,FlyBase, etc
*
************************************************************************/
NLM_EXTERN Int4 get_kludge_factor(SeqIdPtr sip, Int4Ptr gi)
{
	DbtagPtr db_tag;
	ObjectIdPtr oip;

	*gi = -1;
	
	if(sip == NULL) 
		return 0;
	if(sip->choice == SEQID_GI)
		*gi = sip->data.intvalue;
	if(sip->choice != SEQID_GENERAL)
		return 0;
				
	db_tag = sip->data.ptrvalue;
	if(db_tag == NULL || db_tag->db == NULL)
		return 0L;
	oip = db_tag->tag;
	*gi = oip->id;
		
	if(StringCmp(db_tag->db, "UNIGENE") == 0)
		return 1L;
		
	if(StringICmp(db_tag->db, "FlyBase") == 0)
		return 2L;
		
	if(StringCmp(db_tag->db, "JACKSON") == 0)
		return 3L;

	 if(StringCmp(db_tag->db, "JRGP") == 0)
		return 4L;
		
	return 0L;
}

NLM_EXTERN GeneDataPtr make_gene_data(ValNodePtr gene_list)
{
	GeneDataPtr head, g_new;
	CharPtr str;

	head = NULL;
	while(gene_list)
	{
		str = gene_list->data.ptrvalue;
		g_new = MemNew(sizeof(GeneData));
		g_new->symbol = StringSave(str);
		g_new->priority = 0;
		g_new->landmark = (gene_list->choice != 0);
		LinkGeneData(&head, g_new);
		gene_list = gene_list->next;
	}

	return head;
}


NLM_EXTERN void RefreshGeneData(GeneDataPtr gdp)
{
	while(gdp)
	{
		gdp->priority = 0;
		gdp = gdp->next;
	}
}


/**************************************************************
*
*	get the alignment for the FISH map
*	for the Human Cytogenetic map, if there is a 
*	Seq-annot stored as Hist-align and the intervals are 
*	aligned to the FISH map, it will return the Seq-align
*	of the alignment to the FISH map
*
***************************************************************/
NLM_EXTERN SeqAlignPtr get_FISH_align (BioseqPtr bsp)
{
	SeqAnnotPtr annot;
	AnnotDescrPtr descr;
	Boolean is_hist, is_fish;
	UserObjectPtr uop;
	UserFieldPtr ufp;
	ObjectIdPtr oip;

	for(annot = bsp->annot; annot != NULL; annot = annot->next)
	{
		if(annot->type == 2)
		{
			descr = annot->desc;
			is_hist = FALSE;
			is_fish = FALSE;
			while(descr)
			{
				if(descr->choice == Annot_descr_user)
				{
					uop = descr->data.ptrvalue;
					oip = uop->type;
					if(oip && oip->str && 
						StringCmp(oip->str, "Hist Seqalign") == 0)
					{
						ufp = uop->data;
						while(ufp)
						{
							oip = ufp->label;
							if(StringCmp(oip->str, "Hist Seqalign") == 0)
							{
								if(ufp->choice == 4)
									is_hist = ufp->data.boolvalue;
							}
							if(StringCmp(oip->str, "FISH Align") == 0)
							{
								if(ufp->choice == 4)
									is_fish = ufp->data.boolvalue;
							}
							ufp = ufp->next;
						}
					}	
				}
				descr = descr->next;
			}
			if(is_hist && is_fish)
				return (SeqAlignPtr)(annot->data);
		}
	}

	return NULL;
}
		
	
/*******************************************************
*
*	annot_is_user_defined(annot)
*
*	determine if the Seq-annot contains the features 
*	that were defined by the user. This is to 
*	distinguish the local data from the public data 
*	set
*
********************************************************/
NLM_EXTERN Boolean annot_is_user_defined (SeqAnnotPtr annot)
{
	UserObjectPtr uop;
	ValNodePtr desc;

	if(annot == NULL || annot->desc == NULL)
		return FALSE;

	for(desc = annot->desc; desc != NULL; desc = desc->next)
	{
		if(desc->choice == Annot_descr_user)
		{
			uop = desc->data.ptrvalue;
			if(is_label_match(uop->type, "User Feature"))
				return TRUE;
		}
	}

	return FALSE;
}


/*
*
*	functions related to the map legend of a Bioseq
*
*/
NLM_EXTERN UserObjectPtr BioseqHasMapLegend (BioseqPtr bsp)
{
	ValNodePtr vnp;
	UserObjectPtr uop;
	ObjectIdPtr oip;


	if(bsp == NULL)
		return NULL;
	for (vnp = bsp->descr; vnp != NULL; vnp = vnp->next)
	{
		if(vnp->choice == Seq_descr_user)
		{
			uop = vnp->data.ptrvalue;
			if(uop && uop->type != NULL)
			{
				oip = uop->type;
				if(oip->str && StringCmp(oip->str, "MapLegend") == 0)
					return uop;
			}
		}
	}


	return NULL;
}
		
	 						
NLM_EXTERN Boolean SeqLocListHasLegend (ValNodePtr slp_list)
{
	SeqLocPtr slp;
	BioseqPtr bsp;

	while(slp_list)
	{
		slp = slp_list->data.ptrvalue;
		if(slp != NULL)
		{
			bsp = BioseqFind(SeqLocId(slp));
			if(bsp != NULL)
			{
				if(BioseqHasMapLegend(bsp) != NULL)
					return TRUE;
			}
		}
		slp_list = slp_list->next;
	}

	return FALSE;
}


/*
*
*	Is it a Sequence Index Map
*
*/
NLM_EXTERN Boolean IsSeqIndexMap (BioseqPtr bsp)
{
	ValNodePtr vnp;

	if(bsp == NULL)
		return FALSE;

	for(vnp = bsp->descr; vnp != NULL; vnp = vnp->next)
	{
		if(vnp->choice == Seq_descr_comment)
		{
			if(vnp->data.ptrvalue != NULL)
			{
				if(StringCmp(vnp->data.ptrvalue, 
					"Sequence Index Map") == 0)
					return TRUE;
			}
		}
	}


	return FALSE;
}


/*find a list of the seqlocs on the contig that maps to the 
*current chromosome. For now, only the Whitehead map and the 
*Eric Green's map is considered. return a list of Seq-locs that 
*contains contigs within the region
*/
NLM_EXTERN Uint1 FindContigDB (SeqIdPtr sip)
{
	DbtagPtr db_tag;

	while(sip)
	{
		if(sip->choice == SEQID_GENERAL)
		{
			db_tag = sip->data.ptrvalue;
			if(db_tag->db)
			{
				if(StringCmp(db_tag->db, "MIT") == 0)
					return YAC_MIT;
				if(StringCmp(db_tag->db, "NHGRI") == 0)
					return YAC_NHGRI;
			}
		}
		sip = sip->next;
	}
	return 0;
}



/*
################################################################
#
#	functions related to mapping the location of the chromosome 
#	to a location on the YAC Contig
#
################################################################
*/


/*the positions on the contig sequence is calculated by the ratio*/
static FloatHi calculate_percent_pos(Int4 pos, Int4 start, Int4 len)
{
	return (FloatHi)(pos  - start)/(FloatHi)len;
}

static Int4 map_position_by_ratio(Int4 m_pos, SeqLocPtr m_loc, Int4 s_len)
{
	FloatHi ratio;
	FloatHi val;

	ratio = calculate_percent_pos(m_pos, SeqLocStart(m_loc), SeqLocLen(m_loc));
	val = ratio * (FloatHi)s_len;
	return MIN((Int4)val, s_len-1);
}


typedef struct contig_map {
	Int4 offset;	/*offset of the contig to the chromosome coordinates*/
	SeqLocPtr slp;	/*location of the contig*/
}ContigMap, PNTR ContigMapPtr;

static int LIBCALLBACK OffsetCompProc (VoidPtr ptr1, VoidPtr ptr2)
{
  ContigMapPtr  cmp1, cmp2;
  ValNodePtr   vnp1;
  ValNodePtr   vnp2;

  if (ptr1 != NULL && ptr2 != NULL) {
    vnp1 = *((ValNodePtr PNTR) ptr1);
    vnp2 = *((ValNodePtr PNTR) ptr2);
    if (vnp1 != NULL && vnp2 != NULL) {
      cmp1 = (ContigMapPtr) vnp1->data.ptrvalue;
      cmp2 = (ContigMapPtr) vnp2->data.ptrvalue;
      if (cmp1 != NULL && cmp2 != NULL) {
		  if(cmp1->offset > cmp2->offset)
			  return 1;
		  else
		  {
			  if(cmp1->offset < cmp2->offset)
				  return -1;
			  else
				  return 0;
		  }
	  }
	}
  }
  return 0;
}


/*
*
*	chr_slp is the position on the genome/chromosome. align is the alignment
*	between the contig and the chromosome. It will try to map the chromosome 
*	coordinates to the contig coordinates. Since more than one contig may be 
*	identified, the result will be chained to a list. The list of the contigs 
*	are sorted by their position on the chromosome
*/
static ValNodePtr MapContigPosition (SeqLocPtr chr_slp, SeqAlignPtr align)
{
	StdSegPtr ssp;
	SeqLocPtr slp;
	SeqLocPtr m_loc, s_loc;
	SeqIdPtr chr_sip;
	Int4 offset;
	Int4 start, stop;
	Int4 chr_start, chr_stop;
	Int4 ctg_start, ctg_stop;
	ContigMapPtr cmp;
	ValNodePtr list, curr, contig_list;
	BioseqPtr bsp;

	chr_start = SeqLocStart(chr_slp);
	chr_stop = SeqLocStop(chr_slp);
	chr_sip = SeqLocId(chr_slp);
	list = NULL;
	while(align)
	{
		if(align->segtype == 3)
		{
			ssp = align->segs;
			while(ssp)
			{
				m_loc = NULL;
				s_loc = NULL;
				for(slp = ssp->loc; slp != NULL; slp = slp->next)
				{
					if(SeqIdMatch(SeqLocId(slp), chr_sip))
					{
						if(SeqLocCompare(slp, chr_slp) != SLC_NO_MATCH)
							m_loc = slp;
					}
					else
						s_loc = slp;
				}
				if(m_loc != NULL && s_loc != NULL)
				{
					if(SeqLocStart(m_loc) > chr_start)
						offset = SeqLocStart(m_loc) - chr_start;
					else
						offset = 0;
					start = MAX(SeqLocStart(m_loc), chr_start);
					stop = MIN(SeqLocStop(m_loc), chr_stop);

					/*assuming the contig is mapped to the chromosome from 
					  head-to-toe. Use the original Bioseq to get the location 
					  on the contig
					 */
					bsp = BioseqLockById(SeqLocId(s_loc));
					if(bsp != NULL)
					{
						ctg_start = map_position_by_ratio(start, m_loc, bsp->length);
						ctg_stop = map_position_by_ratio(stop, m_loc, bsp->length);
						slp = SeqLocIntNew(ctg_start, ctg_stop, Seq_strand_plus, SeqLocId(s_loc));
						
						cmp = MemNew(sizeof(ContigMap));
						cmp->offset = offset;
						cmp->slp = slp;
						ValNodeAddPointer(&list, 0, cmp);
						BioseqUnlock(bsp);
					}
				}
				ssp = ssp->next;
			}
		}
		align = align->next;
	}

	if(list == NULL)
		return NULL;
	list = SortValNode(list, OffsetCompProc);
	/*extract the Seq-loc from cmp, and free the structure*/
	contig_list = NULL;
	for(curr = list; curr != NULL; curr = curr->next)
	{
		cmp = curr->data.ptrvalue;
		if(cmp && cmp->slp != NULL)
			ValNodeAddPointer(&contig_list, 0, cmp->slp);
		MemFree(cmp);
	}
	ValNodeFree(list);
	return contig_list;
}




/*return a list of Seq-locs which are the contigs mapped to 
*	the current location on the genome
*
*/
NLM_EXTERN ValNodePtr FindContigList (SeqLocPtr chr_slp)
{
	BioseqPtr bsp;
	SeqAlignPtr align;
	ValNodePtr list = NULL;
	SeqAnnotPtr annot;
	Uint1 type, annot_type;
	Char annotDB[21];


	bsp = BioseqLockById(SeqLocId(chr_slp));
	if(bsp == NULL)
		return NULL;

	if(FindContigDB (bsp->id) == 0)
	{
		BioseqUnlock(bsp);
		return NULL;
	}
	if(bsp->hist && bsp->hist->assembly)
	{
		align = bsp->hist->assembly;
		list = MapContigPosition (chr_slp, align);
	}
	if(list == NULL)
	{
		for(annot = bsp->annot; annot != NULL; annot = annot->next)
		{
			if(annot->type == 2)
			{
				if(is_annot_for_hist_alignment(annot))
				{
					type = get_align_annot_qual(annot, annotDB, 20, &annot_type);
					if(annot_type == ANNOT_CONSIST && type == 1)
						/*for anchored contigs only*/
					{
						align = annot->data;
						list = MapContigPosition(chr_slp, align);
						if(list != NULL)
							break;
					}
				}
			}
		}
	}

	BioseqUnlock(bsp);
	return list;
}

NLM_EXTERN Boolean is_lod_score_annot(SeqAnnotPtr annot)
{
	SeqFeatPtr sfp;
	ValNodePtr desc;

	if(annot == NULL || annot->type != 1)
		return FALSE;

	sfp = annot->data;
	if(sfp == NULL || sfp->data.choice != 14)
		return FALSE;
	for(desc = annot->desc; desc != NULL; desc = desc->next)
	{
		if(desc->choice == Annot_descr_name)
		{
			if(StringCmp(desc->data.ptrvalue, "LOD Score") == 0)
				return TRUE;
		}
	}

	return FALSE;
}

NLM_EXTERN CharPtr GetAnnotTitle(SeqAnnotPtr annot)
{
	ValNodePtr desc;

	if(annot == NULL)
		return NULL;

	for(desc = annot->desc; desc != NULL; desc = desc->next)
	{
		if(desc->choice == Annot_descr_title)
		{
			if(desc->data.ptrvalue != NULL)
				return (desc->data.ptrvalue);
		}
	}

	return NULL;
}


NLM_EXTERN Int2 GetLODScoreNumber (BioseqPtr bsp)
{
	Int2 num = 0;
	SeqAnnotPtr annot;

	if(bsp == NULL || bsp->annot == NULL)
		return 0;
	for(annot = bsp->annot; annot != NULL; annot = annot->next)
	{
		if(is_lod_score_annot(annot))
			++num;
	}

	return num;
}

NLM_EXTERN Uint1 GetLODScoreBitValue (SeqFeatPtr sfp)
{
	UserObjectPtr uop;
	ObjectIdPtr oip;
	UserFieldPtr ufp;

	if(sfp == NULL || sfp->data.choice != 14)
		return 0;

	uop = sfp->data.value.ptrvalue;
	if(uop != NULL)
	{
		oip = uop->type;
		if(oip && oip->str && StringCmp(oip->str, "LOD Score Value") == 0)
		{
			ufp = uop->data;
			while(ufp)
			{
				oip = ufp->label;
				if(oip->str && StringCmp(oip->str, "Bit Value") == 0)
				{
					if(ufp->choice == 2)
						return (Uint1)(ufp->data.intvalue);
				}
				ufp = ufp->next;
			}
		}
	}

	return 0;
}


