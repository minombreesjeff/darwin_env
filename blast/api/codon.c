	      
#include <codon.h>


/******************************************************************
*
*	aa_to_codon(sfp, aa_start, aa_stop)
*	generate a list of CodonVecotr to show the codons of an 
*	amino acid sequence
*	sfp: the Seq-feat for cds
*	aa_start: the start position of protein sequence
*	aa_stop the stop position of protein sequence
*
******************************************************************/
NLM_EXTERN ValNodePtr aa_to_codon(SeqFeatPtr sfp, Int4 aa_start, Int4 aa_stop)
{
  BioseqPtr bsp;

  Int4 frame_offset, start_offset;
  SeqLocPtr slp = NULL;
  SeqLocPtr cdloc;
  CdRegionPtr crp;
  Uint1 frame;

  Boolean is_end;			/**is the end for process reached?**/
  Int4 p_start=0, p_stop=0;		/**protein start & stop in defined
					corresponding CdRegion Seq-loc**/

  Int4 line_len;
  Int4 cur_pos;			/**current protein position in process**/
  Int4 cd_len;		/**length of the cDNA for the coding region**/

  Int2 i, j;
  Int2 k, n;
  CharPtr PNTR buf;

  Boolean is_new;		/**Is cur_pos at the begin of new Seq-loc?**/
  CharPtr temp;

  SeqPortPtr spp;
  Uint1 residue;

  Boolean end_partial;
  Int4 d_start, seq_pos;
  Int2 pos;

  ValNodePtr head= NULL;
  CodonVectorPtr cvp;
  Boolean prt_stop_codon;
  Uint2 exon;




   if(sfp->data.choice !=3)
	return NULL;

   crp = sfp->data.value.ptrvalue;
   if(!crp)
	return NULL;
   frame = crp->frame;
   cdloc = sfp->location;
   if(cdloc == NULL )
	return NULL;

   if(frame>0)
	frame_offset = frame-1;
   else
	frame_offset = 0;
   start_offset = frame_offset;

   prt_stop_codon = (aa_stop == SeqLocStop(sfp->product));
   line_len = (aa_stop - aa_start + 1) + 1;
					/* +1 for the possible partial start codon*/
   if(prt_stop_codon)/*can be either as a stop codon or partial stop*/
	++line_len;
   buf = MemNew((size_t)3 * sizeof(CharPtr));
   for(i =0; i<3; ++i)
	buf[i] = MemNew((size_t)(line_len + 1) * sizeof (Char));
		

   cur_pos= aa_start;
   cd_len = 0;
   is_end = FALSE;
   p_start = 0;
   slp = NULL;
   exon = 0;
   while(!is_end && ((slp = SeqLocFindNext(cdloc, slp))!=NULL))
   {
	++exon;
	cd_len += SeqLocLen(slp);
	end_partial = ((cd_len - start_offset)%3 != 0);
	p_stop = (cd_len - start_offset)/3 -1;
	if(end_partial)
	   ++p_stop;
	if(p_stop > aa_stop || (p_stop == aa_stop && !end_partial))
	{
	   p_stop = aa_stop;		/**check if the end is reached**/
	   is_end = TRUE;
	}

	if(p_stop >= cur_pos)	/*get the exon*/
	{
	   bsp = BioseqLockById(SeqLocId(slp));
	   if(bsp)
	   {
		is_new = (p_start == cur_pos);	/*start a new exon?*/
		cvp = MemNew(sizeof(CodonVector));
		cvp->sip = SeqIdDup(find_sip(bsp->id));
		cvp->strand = SeqLocStrand(slp);
		cvp->exonCount = exon;
		if(is_new)
		{
			if(frame_offset == 0)
				cvp->frame = 0;
			else
				cvp->frame = 3- (Uint1)frame_offset;
		}
		else
			cvp->frame = 0;
		if(cur_pos==0 && frame_offset > 0)	/*partial start codon*/
			cvp->aa_index = 0;
		else
			cvp->aa_index = 1;
		if(is_new)	/**special case of the first partial**/
		   d_start = SeqLocStart(slp);
		else
		{
		   if(frame_offset && p_start >0)
			++p_start;
		   d_start = SeqLocStart(slp) + 3*(cur_pos - p_start) + frame_offset;
		}
	    /**p_start is the start position of aa in the current Seq-loc
	       cur_pos is the current aa that is in process. The offset will
	       help to located the position on the DNA Seq-loc for translation
	       d_start is the position of the starting DNA in the coordinates
	       of DNA segment, used for mark the sequence
	       **/

		seq_pos = d_start - SeqLocStart(slp);	/**the pos in spp**/
		if(SeqLocStrand(slp)== Seq_strand_minus)
		   d_start = SeqLocStop(slp) - seq_pos;
		cvp->dna_pos = d_start;

		n = (Int2)cur_pos - (Int2)aa_start + cvp->aa_index;	/*position in buffer*/
		for(i =0; i<3; ++i)
			make_empty(buf[i], (Int2)line_len);
		spp = SeqPortNewByLoc(slp, Seq_code_iupacna);
		SeqPortSeek(spp, seq_pos, SEEK_SET);
		/**store the partial codons**/
		if(is_new && frame_offset > 0)
		{
		   k = (Int2)frame_offset;
		   while(k > 0)
		   {
			residue = SeqPortGetResidue(spp);
			temp = buf[3-k];	/**the position**/
			pos = n;
			temp[pos] = TO_LOWER(residue);
			--k;
		   }
		   ++n;
		   if(cur_pos!=0)
			++cur_pos;
		}


	     	/**load  the codons**/
		k =0;
		while((residue = SeqPortGetResidue(spp)) != SEQPORT_EOF && cur_pos <= p_stop)
		{
		   j= (Uint1)k%3;
		   temp = buf[j];
		   temp[n] = TO_LOWER(residue);
		   if(j ==2)
		   {		/**the last base**/
			++n;
		 	if(!prt_stop_codon|| !is_end) /*for the last codon*/
			/**prt_end controls to print the whole loc**/
		   	   ++cur_pos;
		   }
		   ++k;
		}	/**end of while**/

		SeqPortFree(spp);

		for(i =0; i<3; ++i)
		   cvp->buf[i] = StringSave(buf[i]);
		ValNodeAddPointer(&head, 0, (Pointer)cvp);

		BioseqUnlock(bsp);
	   }/*end of if(bsp)*/
	}/**end of if for matched intervals**/

	if(end_partial)
	    p_start = p_stop;
	else
	    p_start = p_stop +1;

	frame_offset = (cd_len - start_offset)%3;
	 if(frame_offset >0)
	    frame_offset = 3-frame_offset;

   }/**end of while(slp && !is_end) **/

   for(i=0; i<3; ++i)
	MemFree(buf[i]);
   MemFree(buf);

   return head;
}


/******************************************************************
*
*	free_cvp_list(cvp_list)
*	free a list of CodonVectorPtr
*
******************************************************************/
NLM_EXTERN ValNodePtr free_cvp_list(ValNodePtr cvp_list)
{
	ValNodePtr next;
	CodonVectorPtr cvp;
	Int2 i;

	while(cvp_list)
	{
		next = cvp_list->next;
		cvp_list->next = NULL;
		cvp = cvp_list->data.ptrvalue;
		for(i=0; i<3; ++i)
			MemFree(cvp->buf[i]);
		SeqIdFree(cvp->sip);
		ValNodeFreeData(cvp_list);
		cvp_list = next;
	}

	return NULL;
}


/*********************************************************************
*
*	make_cds_paragraph(sfp, aa_start, aa_stop)
*	return a buffer for the display of 3-codon under one amino 
*	acid format. It also includes the new line characters 
*	This is what Jonathan K. desires to have for the sequin 
*	doc object
*	aa_start, aa_stop: start and stop in the amino acid sequence
*
*********************************************************************/
NLM_EXTERN CharPtr make_cds_paragraph(SeqFeatPtr sfp, Int4 aa_start, Int4 aa_stop)
{
	BioseqPtr pbsp;
	SeqPortPtr spp;
	ValNodePtr cvp_node, curr;
	CodonVectorPtr cvp;
	CharPtr docbuf = NULL;
	Int4 num, buf_size;
	Uint1 residue;
	Char p_name[30];
	Int4 space_len, i;
	CharPtr buf;
	Int4 pos;
	Int4 max_len = 150;
	Boolean extra_space;

	if(sfp == NULL || sfp->data.choice !=3)
		return NULL;
	if(sfp->product == NULL)
		return NULL;
	pbsp = BioseqLockById(SeqLocId(sfp->product));
	if(pbsp == NULL)
		return NULL;

	cvp_node = aa_to_codon(sfp, aa_start, aa_stop);
	num = 1;
	for(curr = cvp_node; curr !=NULL; curr = curr->next)
		num +=3;
	buf_size = num * max_len;
	/*  #ifdef WIN_16
		if(buf_size > 10000)
		{
			Message(MSG_ERROR, "Can not allocate enough space ");
			return NULL;
		}
	#endif
	*/

	docbuf = MemNew((size_t)(buf_size) * sizeof(Char));

	MuskSeqIdWrite(pbsp->id, p_name, B_SPACE, PRINTID_TEXTID_ACCESSION, TRUE, FALSE);
	/*SeqIdWrite (pbsp->id, p_name, PRINTID_FASTA_SHORT, 10);*/
	pos = 0;
	pos+= print_label_to_buffer(docbuf+pos, p_name, (aa_start+1), 0, FALSE, 
		FALSE, B_SPACE, POS_SPACE);

	/*print the amino acid sequence into buffer*/
	spp = SeqPortNew(pbsp, aa_start, aa_stop, Seq_strand_plus, Seq_code_ncbieaa);
	while((residue = SeqPortGetResidue(spp)) != SEQPORT_EOF )
		docbuf[pos++] = residue;
	docbuf[pos++] = '\n';
	SeqPortFree(spp);

	for(curr = cvp_node; curr !=NULL; curr = curr->next)
	{
		cvp = curr->data.ptrvalue;
		SeqIdWrite (cvp->sip, p_name, PRINTID_FASTA_SHORT, 10);
		extra_space = (cvp->aa_index == 0);
		for(i=0; i<3; ++i)
		{
			space_len = cvp->aa_index;
			buf = cvp->buf[i] + cvp->aa_index;
			if(i == cvp->frame)
			{
				pos+= print_label_to_buffer(docbuf+pos, p_name, 
					cvp->dna_pos, cvp->strand, extra_space, FALSE, B_SPACE, POS_SPACE);
			}
			else
				pos+= print_label_to_buffer(docbuf+pos, NULL, -1, 
				0, extra_space, FALSE, B_SPACE, POS_SPACE);
			sprintf(docbuf+pos, "%s\n", buf);
			pos += (StringLen(buf) +1);

		}
	}

	docbuf[pos++] = '\n';
	docbuf[pos] = '\0';
	
	free_cvp_list(cvp_node);
	BioseqUnlock(pbsp);

	return docbuf;
}



/******************************************************************
*
*	aa_to_dnaloc(sfp, aa_start, aa_stop)
*	map the amino acid sequence to a list of Seq-locs in the 
*	DNA sequence
*
******************************************************************/
NLM_EXTERN SeqLocPtr aa_to_dnaloc(SeqFeatPtr sfp, Int4 aa_start, Int4 aa_stop)
{
  Int4 frame_offset, start_offset;	/*for determine the reading frame*/
  SeqLocPtr slp = NULL;
  CdRegionPtr crp;
  SeqLocPtr dna_loc, loc;			/*for the dna location*/

  Boolean is_end;			/**is the end for process reached?**/
  Int4 p_start=0, p_stop=0;		/**protein start & stop in defined
					corresponding CdRegion Seq-loc**/
  Int4 cur_pos;			/**current protein position in process**/
  Int4 cd_len;		/**length of the cDNA for the coding region**/

  Boolean is_new;		/**Is cur_pos at the begin of new exon?**/
  Boolean end_partial;		/*the end of aa is a partial codon*/
  Int4 d_start, d_stop;		/*the start and the stop of the DNA sequence*/
  Int4 offset;			/*offset from the start of the current exon*/
  Int4 aa_len;
  Uint1 strand;




   if(sfp->data.choice !=3)
	return NULL;


   crp = sfp->data.value.ptrvalue;
   if(!crp)
	return NULL;
   if(crp->frame>0)
	frame_offset = crp->frame-1;
   else
	frame_offset = 0;
   start_offset = frame_offset;


   cur_pos= aa_start;
   cd_len = 0;
   is_end = FALSE;
   p_start = 0;
   slp = NULL;
   dna_loc= NULL;
   while(!is_end && ((slp = SeqLocFindNext(sfp->location, slp))!=NULL))
   {
	cd_len += SeqLocLen(slp);
	end_partial = ((cd_len - start_offset)%3 != 0);
	p_stop = (cd_len - start_offset)/3 -1;
	if(end_partial)
	   ++p_stop;
	if(p_stop > aa_stop || (p_stop == aa_stop && !end_partial))
	{
	   p_stop = aa_stop;		/**check if the end is reached**/
	   is_end = TRUE;
	}

	if(p_stop >= cur_pos)	/*get the exon*/
	{
		is_new = (p_start == cur_pos);	/*start a new exon?*/
		if(is_new)	/**special case of the first partial**/
		   offset = 0;
		else
		{
		   if(frame_offset && p_start >0)
			++p_start;
		   offset = 3*(cur_pos - p_start) + frame_offset;
		}
		strand = SeqLocStrand(slp);
		if(strand == Seq_strand_minus)
		   d_start = SeqLocStop(slp) - offset;
		else
		   d_start = SeqLocStart(slp) + offset;

		d_stop = d_start;
		aa_len = MIN(p_stop, aa_stop) - cur_pos +1;
		if(strand == Seq_strand_minus)
		{
			d_stop -= 3*aa_len;
			d_stop = MAX(d_stop, SeqLocStart(slp));
			loc = SeqLocIntNew(d_stop, d_start, strand, SeqLocId(slp));
		}
		else
		{
			d_stop += 3*aa_len;
			d_stop = MIN(d_stop, SeqLocStop(slp));
			loc = SeqLocIntNew(d_start, d_stop, strand, SeqLocId(slp));
		}
		ValNodeLink(&dna_loc, loc);

		if(end_partial)
			cur_pos = p_stop;
		else
			cur_pos = p_stop+1;
	}



	if(end_partial)
	    p_start = p_stop;
	else
	    p_start = p_stop +1;

	frame_offset = (cd_len - start_offset)%3;
	 if(frame_offset >0)
	    frame_offset = 3-frame_offset;

   }/**end of while(slp && !is_end) **/
   return dna_loc;

}

static Boolean ck_reverse(Uint1 strand_1, Uint1 strand_2)
{
	if(strand_1 == strand_2)
		return FALSE;
	
	/* if(strand_1 == 0 || strand_2 == 0)
	{
		if(strand_1 == Seq_strand_plus)
			return FALSE;
		if(strand_2 == Seq_strand_plus)
			return FALSE;
	} */
	return (strand_1 == Seq_strand_minus || strand_2 == Seq_strand_minus);

}


NLM_EXTERN Int4 print_protein_for_cds(SeqFeatPtr sfp, CharPtr buf, SeqLocPtr loc, Boolean reverse_minus)
{
	CdRegionPtr crp;
	Int4 frame_offset, start_offset;
	Uint1 f_strand;
	Boolean reverse;
	Int4 cd_len;
	GatherRange gr;
	Int2 p_pos, buf_len;
	Int4 a_left, a_right;
	Int4 aa, val;
	SeqLocPtr slp;
	SeqPortPtr spp;
	ByteStorePtr p_data;
	Int4 end_pos, start_pos = -1;
	Uint1 residue;
	Boolean seal_ends = FALSE;
	Boolean reverse_order;

	if(sfp == NULL || sfp->data.choice != 3)
		return -1;
	if(buf == NULL || loc == NULL)
		return -1;
	crp = sfp->data.value.ptrvalue;
	if(crp == NULL)
		return -1;

	if(buf[0] == '\0')
		seal_ends = TRUE;
	spp = NULL;
	p_data = NULL;
	if(sfp->product !=NULL && !IS_BOGO_Product(sfp->ext))
	{
		spp = SeqPortNewByLoc(sfp->product, Seq_code_ncbieaa);
		if(spp !=NULL)
		{
    			SeqPortSeek(spp, 0, SEEK_SET);
			end_pos = spp->totlen-1;
		}
	}
	if(spp == NULL)
	{
		p_data = ProteinFromCdRegion(sfp, TRUE);
		/* p_data = ProteinFromCdRegion(sfp, FALSE); */
		if(p_data !=NULL)
		{
			BSSeek(p_data, 0, SEEK_SET);
			end_pos = BSLen(p_data)-1;
		}
	}

	if(spp == NULL && p_data == NULL)
		return -1;

	if(crp->frame == 0)
		frame_offset = 0;
	else
		frame_offset = (Int4)crp->frame-1;
	start_offset = frame_offset;

	f_strand = SeqLocStrand(sfp->location);
	reverse = ck_reverse(f_strand, SeqLocStrand(loc));
	/*if reverse == TRUE, the translated protein is written backwards*/
	if(reverse && reverse_minus)
		reverse_order = TRUE;
	else
		reverse_order = FALSE;
	
	slp = NULL;
	cd_len = 0;
	aa = 0;

        buf_len = SeqLocLen(loc);

	if(reverse_order)
	{
		p_pos = buf_len -1;
		if(seal_ends)
		{
			buf[p_pos+1] = '\0';
			seal_ends = FALSE;
		}
	}
	else
		p_pos = 0;

	while((slp = SeqLocFindNext(sfp->location, slp))!=NULL)
	{
	   if(SeqLocOffset(loc, slp, &gr, 0))
	   {
		if(reverse_order)
		{
			if(gr.right < p_pos)
				p_pos = (Int2)(gr.right);
		}
		else
		{
			if(p_pos < gr.left)
				p_pos = (Int2)(gr.left);
		}
		SeqLocOffset(slp, loc, &gr, 0);
		
		a_left = gr.left + cd_len;
		a_right = gr.right + cd_len;
		/* if(reverse_order)
		{
			temp = a_right;
			a_right = -a_left;
			a_left = -temp;
		} */
		for(; a_left<=a_right; ++a_left)
		{
			val = ABS(a_left) - start_offset;
			aa = val/3;
			if(aa < 0 || aa > end_pos)/*stop & partial codon*/
			{
				buf[p_pos] = '^';
			}
			else
			{
				if(val%3==1)/*label aa in the middle of 3-bp codon*/
				{
					if(start_pos == -1)
						start_pos = aa;
					if(spp !=NULL)
					{
						SeqPortSeek(spp, aa, SEEK_SET);
						residue = SeqPortGetResidue(spp); 
					}
					else
					{
						BSSeek(p_data, aa, SEEK_SET);
						residue = (Uint1)BSGetByte(p_data);
					}
					if(IS_ALPHA(residue) || residue == '*' || residue == '-')
					
						buf[p_pos] = residue;
					else
						buf[p_pos] = '?';
				}
				else
					buf[p_pos] = ' ';
			}
			if(reverse_order)
				-- p_pos;
			else {
				++p_pos;
                                if (p_pos > buf_len)
                                   break;
                        }
		}
	     }
	     cd_len += SeqLocLen(slp);
	     /*frame_offset = (cd_len - start_offset)%3;
	     if(frame_offset > 0)
		--frame_offset;*/
		
	}

	if(spp != NULL)
		SeqPortFree(spp);
	if(p_data != NULL)
		BSFree(p_data);

	if(p_pos  == 0)	/*all the residues are introns*/
	{
		if(seal_ends)
		{
			end_pos = buf_len;
			MemSet((Pointer)buf, '~', (size_t)(end_pos) * sizeof(Char));
			buf[end_pos] = '\0';
		}
	}
	else
	{
		if(seal_ends)
		{
			buf[p_pos] = '\0';
		}
		if(start_pos == -1)
			start_pos = aa;
	}

	return start_pos;
}


/********************************************************************
*
*	print_label_to_buffer(buf, label, pos, strand, extra_space)
*
*	print a label (with label_name=label, position=pos, 
*	orientation = strand, extra_space = extra_space for partial start)
*	into the current buffer
*	return the offset of the buffer pointer to the current buffer
*
*********************************************************************/
NLM_EXTERN Int4 print_label_to_buffer_all(CharPtr buf, CharPtr label, 
		Int4 pos, Uint1 strand, Boolean extra_space, Boolean is_html, 
		Int4 label_space, Int4 num_space, Boolean show_strand)
{

	return print_label_to_buffer_all_ex(buf, label, pos, strand, 
		extra_space, is_html, label_space, num_space, show_strand, 
		TRUE);
}

/********************************************************************
*
*	print_label_to_buffer_all_ex(buf, label, pos, strand, extra_space)
*
*	print a label (with label_name=label, position=pos, 
*	orientation = strand, extra_space = extra_space for partial start)
*	into the current buffer
*	return the offset of the buffer pointer to the current buffer
*	Has an option to strip/not-strip semicolons for hardline old blast
*	users.
*
*********************************************************************/
NLM_EXTERN Int4 print_label_to_buffer_all_ex(CharPtr buf, CharPtr label, 
		Int4 pos, Uint1 strand, Boolean extra_space, Boolean is_html, 
		Int4 label_space, Int4 num_space, Boolean show_strand, 
		Boolean strip_semicolon)
{
	Int4 len;
	Char symbol;
	Char temp[100];
	Int4 i = 0;
	CharPtr str;
	Int4 max_b_space;

	/*
	*	print the label to the buffer
	*/
	max_b_space = label_space + 1;
	len = 0;
	if(label)
	{
		if (strip_semicolon)
		{
			str= StrTok(label, ":");
			if(str == NULL)
				str = label;
			else
			{
				str = StrTok(NULL, ":");
				if(str == NULL)
					str = label;
			}
		}
		else
		{
			str = label;
		}
		len = MIN(max_b_space-1, (Int4)StringLen(str));
		StringNCpy(buf, str, len);
		i = len;
		if(is_html)
		{
			sprintf(buf+i, "</a>");
			i += 4;
		}
		buf[i++] = ' ';
		++len;
	}
	
	/*add the leftover empty space */
	for(; len<max_b_space; ++len)
		buf[i++] = ' ';

	if(show_strand)
	{
		symbol = ' ';
		if(strand == Seq_strand_plus)
			symbol = '>';
		if(strand == Seq_strand_minus)
			symbol = '<';
		buf[i++] = ' ';
		buf[i++] = symbol;
		buf[i++] = ' ';
	}

	len = 0;
	if(pos != -1)
	{
		sprintf(buf+i, "%ld", (long) pos);
		sprintf(temp, "%ld", (long) pos);
		len = StringLen(temp);
		i += len;
	}

	if(extra_space)	/*for partial codon*/
		++len;
	max_b_space = num_space + 1;
	for(; len <max_b_space; ++len)
		buf[i++] = ' ';
	return i;
}

NLM_EXTERN Int4 print_label_to_buffer(CharPtr buf, CharPtr label, Int4 pos, Uint1 strand, 
	Boolean extra_space, Boolean is_html, Int4 label_space, Int4 num_space)
{
	return print_label_to_buffer_all(buf, label, pos, strand, 
		extra_space, is_html, label_space, num_space, TRUE);
}


NLM_EXTERN void print_label(FILE *fp, CharPtr label, Int4 pos, Uint1 strand, Boolean extra_space)
{
	Int4 len;
	Char symbol;
	Char temp[100];

	len = 0;
	if(label)
	{
		len = StringLen(label);
		fprintf(fp, "%s", label);
	}
	for(; len<B_SPACE; ++len)
		fprintf(fp, " ");

	symbol = ' ';
	if(strand == Seq_strand_plus)
		symbol = '>';
	if(strand == Seq_strand_minus)
		symbol = '<';
	fprintf(fp, " %c ", symbol);

	len = 0;
	if(pos != -1)
	{
		sprintf(temp, "%ld", (long) pos);
		len = StringLen(temp);
		fprintf(fp, "%s", temp);
	}

	if(extra_space)	/*for partial codon*/
		++len;
	for(; len <POS_SPACE; ++len)
		fprintf(fp, " ");
}
