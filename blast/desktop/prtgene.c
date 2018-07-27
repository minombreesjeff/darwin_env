#include <prtgene.h>

/*functions to print everything between two genes */
#include <tofile.h>


#define GENE_MARKER	1
#define SEQ_SEGMENT	2
typedef struct print_gene {
	GeneDataPtr g_data;
	FILE *fp;
	Boolean html;
	Boolean found;
	Uint1 print_loc;
	NumberingPtr np;
	Uint1 prev_type;	/*the type of the previously recorded marker*/
}PrintGene, PNTR PrintGenePtr;


static 	ColData prt_col = {0, 0, 0, 'l', TRUE, TRUE, FALSE};
static  ParData prt_par = {FALSE};

#define MAX_HTML_SIZE 1000
static Char HTML_buffer[MAX_HTML_SIZE];

static void print_store_for_html PROTO((CharPtr temp, CharPtr html_buf, Int4Ptr c_pos, Int4 width, FILE *fp));

static void print_store_buf(CharPtr temp, CharPtr buf, Int4 buf_size, Int4Ptr cur_len, 
							FILE *fp)
{
	Int4 len;

	len = StringLen(temp);
	if(len + (*cur_len) > buf_size)
	{
		StringCat(buf, "\n");
		SendTextToFile (fp, buf, &prt_par, &prt_col);
		*cur_len = 0;
		StringCpy(buf, temp);
	}
	else
	{
		if(*cur_len == 0)
			StringCpy(buf, temp);
		else
			StringCat(buf, temp);
	}
	*cur_len += len;
}



/**********************************************************
*
*	functions to print a feature to a FILE
*
**********************************************************/
static Boolean PrintQual(GBQualPtr qdata, FILE *fp, CharPtr buf, Int4 buf_size)
{
	Int4 len;
	Char temp[101];


	if(qdata == NULL)
		return FALSE;
	sprintf(buf, "Qualifier ");
	len = StringLen(buf);
	while(qdata)
	{
		sprintf (temp, "%s=%s ", qdata->qual, qdata->val);
		print_store_buf(temp, buf, buf_size, &len, fp);
	    qdata = qdata->next;
	}

	if(len > 0)
	{
		StringCat(buf, "\n");
		SendTextToFile (fp, buf, &prt_par, &prt_col);
	}

	return TRUE;
}

static Boolean print_sfp_ext (SeqFeatPtr sfp, FILE *fp, CharPtr buf, Int4 buf_size)
{
	ObjectIdPtr oip;
	UserObjectPtr uop;
	UserFieldPtr ufp;
	Char temp[101];
	Int4 len;

	if(sfp->ext == NULL)
		return FALSE;
	uop = sfp->ext;
	len = 0;
	buf[0] = '\0';
	while(uop)
	{
		oip = uop->type;
		if(StringCmp(oip->str, "MapMarkerInfo") == 0)
		{
			ufp = uop->data;
			while (ufp)
			{
				oip = ufp->label;
				if(StringCmp(oip->str, "Bin Order") == 0)
				{	/* the order of the 1000:1 bin */
					if(ufp->choice == 2)	/*integer */
						sprintf(temp, "Bin Order is %ld\n", (long) ufp->data.intvalue);
						print_store_buf(temp, buf, buf_size, &len, fp);
				}
				else if(StringCmp(oip->str, "Marker Type" ) == 0)
				{
					if(ufp->choice == 2)
					{
						switch (ufp->data.intvalue)
						{
						case 1:
							sprintf(temp, "Framework Marker\n");
							break;
						case 2:
							sprintf(temp, "Recombination Mimimum Marker\n");
							break;
						case 3:
							sprintf(temp, "Likely Loci Marker\n");
							break;
						case 4:
							sprintf(temp, "Duplicated Marker\n");
							break;
						case 5:
							sprintf(temp, "Multiple Duplicated Marker\n");
							break;
						case 6:
							sprintf(temp, "STS Marker on YAC Contig\n");
							break;
						default:
							break;
						}
						print_store_buf(temp, buf, buf_size, &len, fp);
					}
				}

				else if(StringCmp(oip->str, "Marker Category" ) == 0)
				{
					if(ufp->choice == 2)
					{
						switch (ufp->data.intvalue)
						{
						case 1:
							sprintf(temp, "This is a YAC End\n");
							break;
						case 2:
							sprintf(temp, "This is a Random Marker\n");
							break;
						case 3:
							sprintf(temp, "This is a Genetic Marker\n");
							break;
						case 4:
							sprintf(temp, "This is a Gene Marker\n");
							break;
						case 5:
							sprintf(temp, "This is a EST Marker\n");
							break;
						case 6:
							sprintf(temp, "This is a Misc Marker\n");
							break;
						default:
							break;
						}
					}
					print_store_buf(temp, buf, buf_size, &len, fp);
				}
				ufp = ufp->next;
			}
		}
		if(StringCmp(oip->str, "MIT RH map") == 0)
		{
			ufp = uop->data;
			while(ufp)
			{
				oip = ufp->label;
				if(oip && StringCmp(oip->str, "STS probability") == 0)
				{
					if(ufp->choice == 1)
						sprintf(temp, "STS probability is %s \n", 
						(CharPtr)(ufp->data.ptrvalue));
					print_store_buf(temp, buf, buf_size, &len, fp);
				}
				ufp = ufp->next;
			}
		}

		if(StringCmp(oip->str, "Marker Category" ) == 0)
		{
			ufp = uop->data;
			if(ufp->choice == 2)
			{
				switch (ufp->data.intvalue)
				{
				case 1:
					sprintf(temp, "This is a YAC End\n");
					break;
				case 2:
					sprintf(temp, "This is a Random Marker\n");
					break;
				case 3:
					sprintf(temp, "This is a Genetic Marker\n");
					break;
				case 4:
					sprintf(temp, "This is a Gene Marker\n");
					break;
				case 5:
					sprintf(temp, "This is a EST Marker\n");
					break;
				case 6:
					sprintf(temp, "This is a Misc Marker\n");
					break;
				default:
					break;
				}
			}
			print_store_buf(temp, buf, buf_size, &len, fp);
		}
		uop = uop->next;
	}
	if(len > 0)
		SendTextToFile (fp, buf, &prt_par, &prt_col);
		
	return TRUE;
}



static Boolean PrintCommonFeature(SeqFeatPtr sfp, FILE *fp, CharPtr buf, Int4 buf_size)
{
	if(sfp->title)
	{
		sprintf(buf, "Title: %s\n", sfp->title);
		SendTextToFile (fp, buf, &prt_par, &prt_col);
	}

	if(sfp->partial)
	{
		sprintf(buf, "Partial\n");
		SendTextToFile (fp, buf, &prt_par, &prt_col);
	}

	if(sfp->excpt)
	{
		sprintf(buf, "Exception\n");
		SendTextToFile (fp, buf, &prt_par, &prt_col);
	}

	if(sfp->comment)
	{
		sprintf(buf, "Comment: %s\n", sfp->comment);
		SendTextToFile (fp, buf, &prt_par, &prt_col);
	}


	PrintQual(sfp->qual, fp, buf, buf_size);

	if(sfp->exp_ev ==1)
	{
		sprintf(buf, "Experimental Result\n");
		SendTextToFile (fp, buf, &prt_par, &prt_col);
	}

	if(sfp->exp_ev ==2)
	{
		sprintf(buf, "Not-Experimental\n");
		SendTextToFile (fp, buf, &prt_par, &prt_col);
	}


	return TRUE;

}

static Boolean print_citation (SeqFeatPtr sfp, FILE *fp, CharPtr buf, Int4 buf_size, Boolean is_html)
{
	ValNodePtr cit, pub;
	Char temp[101];
	Boolean found;
	Int4 muid;
	Int4 len;
	Int4 index;

	if(sfp->cit == NULL)
		return FALSE;
	cit = sfp->cit;
	found = FALSE;
	len = 0;
	index = 0;
	while(cit)
	{
		if(cit->choice == 3)
		{
			muid = cit->data.intvalue;
			if(muid != 0)
			{
				if(found == FALSE)
				{
					StringCpy(buf, "MEDLINE: ");
					found = TRUE;
					len = StringLen(buf);
					if(is_html)
						print_store_for_html(buf, NULL, &index, prt_col.width, fp);
				}
				if(is_html)
				{
					sprintf(temp, "%ld", (long) muid);
					sprintf(HTML_buffer, "<a href=\"http://www3.ncbi.nlm.nih.gov:80/htbin-post/Entrez/query?-guid=%ld&form=6&db=m&Dopt=r\">", (long) muid);
					print_store_for_html(temp, HTML_buffer, &index, prt_col.width, fp);
					print_store_for_html(" ", NULL, &index, prt_col.width, fp);
				}
				else
				{
					sprintf(temp, "%ld ", (long) muid);
					print_store_buf(temp, buf, buf_size, &len, fp);
				}
			}
		}
		else if(cit->choice == 1)
		{
			pub = (ValNodePtr)(cit->data.ptrvalue);
			while(pub)
			{
				if(pub->choice == PUB_Muid)
				{
					muid = pub->data.intvalue;
					if(muid != 0)
					{
						if(found == FALSE)
						{
							StringCpy(buf, "MEDLINE: ");
							found = TRUE;
							len = StringLen(buf);
							if(is_html)
								print_store_for_html(buf, NULL, &index, prt_col.width, fp);
						}
						if(is_html)
						{
							sprintf(temp, "%ld", (long) muid);
							sprintf(HTML_buffer, "<a href=\"http://www3.ncbi.nlm.nih.gov:80/htbin-post/Entrez/query?-guid=%ld&form=6&db=m&Dopt=r\">", (long) muid);
							print_store_for_html(temp, HTML_buffer, &index, prt_col.width, fp);
							print_store_for_html(" ", NULL, &index, prt_col.width, fp);
						}
						else
						{
							sprintf(temp, "%ld ", (long) muid);
							print_store_buf(temp, buf, buf_size, &len, fp);
						}
					}
				}
				pub = pub->next;
			}
		}
		cit = cit->next;
	}

	if(found)
	{
		if(is_html)
		{
			if(index > 0)
				fprintf(fp, "\n");
		}
		else if(len > 0)
		{
			StringCat(buf, "\n");
			SendTextToFile (fp, buf, &prt_par, &prt_col);
		}
	}
	return found;
}


/*************************************************************
*
*	creating the html pages to the other genome mapping center
*
**************************************************************/
static CharPtr find_db_html_pointer (CharPtr db, CharPtr str)
{
	Char temp[101];

	if(db == NULL || str == NULL)
		return NULL;

	if(StringNCmp(db, "JAX", 3) == 0)
	{
		sprintf(HTML_buffer, "<a href=\"http://www.informatics.jax.org/bin/get_marker_by_symbol?%s\">", str);
		return HTML_buffer;
	}

	if(StringCmp(db, "Stanford") == 0 || StringCmp(db, "SHGC") == 0)
	{
		if(StringNCmp(str, "SHGC", 4) == 0)
		{
			sprintf(HTML_buffer, "<a href=\"http://www-shgc.stanford.edu/cgi-bin/getSTSinfo?%s\">", str);
			return HTML_buffer;
		}
		if(StringNCmp(str, "STSG-", 5) == 0)
		{	/*old name format for the STSG*/
			StringCpy(temp, "SHGC-");
			StringCat(temp, str+5);
			sprintf(HTML_buffer, "<a href=\"http://www-shgc.stanford.edu/cgi-bin/getSTSinfo?%s\">", temp);
			return HTML_buffer;
		}

		return NULL;
	}

	if(StringCmp(db, "MIT") == 0 )
	{
		if(StringNCmp(str, "WI-", 3) == 0)
		{
			sprintf(HTML_buffer, "<a href=\"http://www-genome.wi.mit.edu/cgi-bin/"
			"contig/sts_info?sts=%s&database=release\">", str);
			return HTML_buffer;
		}
		return NULL;
	}

	if(StringCmp(db, "CHLC") == 0)
	{
		if(StringNCmp(str, "GATA", 4) == 0)
		{
			sprintf(HTML_buffer, "<a href=\"http://www.chlc.org/cgi-bin/"
			"MarkerSearch?%s\">", str);
			return HTML_buffer;
		}
		return NULL;
	}


	return NULL;
}


static Boolean print_gene_dblst(ValNodePtr db, FILE *fp, CharPtr buf, Int4 buf_size, Uint1 match_type, Boolean is_html)
{
	DbtagPtr dbtag;
	ObjectIdPtr oip;
	ValNodePtr curr;
	Int4 len;
	Char temp[101];
	Char separator[3];
	CharPtr html_buf;
	Int4 index;
	SeqIdPtr t_sip;
	Int4 gi;

	if(db == NULL)
		return FALSE;

	StringCpy(buf, "Other Databases: ");
	len = StringLen(buf);
	if(is_html)
	{
		fprintf(fp, "%s", buf);
		index = len;
	}


	for(curr = db; curr != NULL; curr = curr->next)
	{
		html_buf = NULL;
		dbtag = curr->data.ptrvalue;
		oip = dbtag->tag;
		if(is_html)
		{
			if(StringICmp(dbtag->db, "GDB") == 0 && oip->id > 0)
			{
				sprintf(HTML_buffer, "<a href=\"http://gdbwww.gdb.org/"
				"gdb-bin/genera/genera/hgd/DBObject/"
				"GDB:%ld\">", (long) oip->id);

				html_buf = HTML_buffer;
			}
			else if(StringICmp(dbtag->db, "MIM") == 0 && oip->id > 0)
			{
				sprintf(HTML_buffer, "<a href=\"http://www3.ncbi.nlm.nih.gov/htbin-post/Omim/dispmim?"
				"%ld\">",(long) oip->id);
				html_buf = HTML_buffer;
			}
			else if(StringICmp(dbtag->db, "GenBank") == 0 && oip->str != NULL)
			{
				t_sip = gb_id_make(NULL, oip->str);
				gi = GetGIForSeqId (t_sip);
				if(gi > 0)
				{
					sprintf(HTML_buffer, "<a href=\"http://www3.ncbi.nlm.nih.gov/"
                				"htbin-post/Entrez/query?form=6&dopt=g&db=n&"
                 				"uid=%08ld\">",(long) gi);
					html_buf = HTML_buffer;
				}
				SeqIdFree(t_sip);
			}
			else if(oip->str != NULL)
				html_buf = find_db_html_pointer (dbtag->db, oip->str);
		}
		if(curr->next == NULL)
			separator[0] = '\0';
		else
			StringCpy(separator, " ");
		if(oip->str)
		{
			if(StringICmp(dbtag->db, "GenBank") == 0 || 
				match_type == MATCH_NONE)
				sprintf(temp, "%s %s%s", dbtag->db, oip->str, separator);
			else
			{
				if(match_type == MATCH_QUERY)
					sprintf(temp, "%s [%s]%s", dbtag->db, oip->str, separator);
				else if(match_type == MATCH_ALIGN)
					sprintf(temp, "%s (%s)%s", dbtag->db, oip->str, separator);
			}
		}
		else
			sprintf(temp, "%s %ld%s", dbtag->db, (long) oip->id, separator);
		if(is_html)
		{
			if(curr->next == NULL)
				StringCat(temp, "\n");
			print_store_for_html(temp, html_buf, &index, prt_col.width, fp);
			print_store_for_html(" ", NULL, &index, prt_col.width, fp);
		}
		else
			print_store_buf(temp, buf, buf_size, &len, fp);
	}
	if(len > 0 && !is_html)
	{
		StringCat(buf, "\n");
		SendTextToFile (fp, buf, &prt_par, &prt_col);
	}
	return TRUE;
}

static Boolean print_gene_synonym(ValNodePtr syn, FILE *fp, CharPtr buf, Int4 buf_size, Uint1 match_type)
{
	ValNodePtr curr;
	CharPtr str;
	Char temp[101];
	Int4 len;

	if(syn == NULL)
		return FALSE;
	StringCpy(buf, "Synonym: ");
	len = StringLen(buf);
	for(curr = syn; curr != NULL; curr = curr->next)
	{
		str = curr->data.ptrvalue;
		if(match_type == MATCH_QUERY)
			sprintf(temp, "[%s] ", str);
		else if(match_type == MATCH_ALIGN)
			sprintf(temp, "(%s) ", str);
		else
			sprintf(temp, "%s ", str);
		print_store_buf(temp, buf, buf_size, &len, fp);
	}
	if(len > 0)
	{
		StringCat(buf, "\n");
		SendTextToFile(fp, buf, &prt_par, &prt_col);
	}
	return TRUE;
}


/************************************************************
*
* PrintGeneRefToFile(sfp, fp, match_type, print_loc, np)
*	print all the data associated with sfp to a file
*	sfp: the Seqfeat of type Gene-ref
*	fp: the FILE
*	match_type: the marker matches any query gene. If 
*	it matches directly (MATCH_QUERY), will be shown as [marker], 
*	if it matches via alignment (MATCH_ALIGN), will be shown 
*	as (marker). Otherwise,nothing will be shown
*	print_loc: print the location of the marker. PRINT_LOC_BOGO
*	directly print out the bogo base pair. PRINT_LOC_NUM, 
*	print out the location as the Numbering system
*	np the numbering system
*
***********************************************************/ 
static Boolean PrintGeneRefToFile(SeqFeatPtr sfp, FILE *fp, Uint1 match_type, Uint1 print_loc, NumberingPtr np, Boolean is_html)
{
	GeneRefPtr grp;
	Char buf[1001];
	SeqIdPtr sip;
	DbtagPtr db_tag;
	CharPtr html_buf;

	PrintCommonFeature(sfp, fp, buf, 1000);
	grp = sfp->data.value.ptrvalue;
	if(grp)
	{
		/* print out the locus name */
		if(grp->locus)
		{
			html_buf = NULL;
			if(is_html)
			{
				html_buf = NULL;
				sip = SeqLocId(sfp->location);
				if(sip->choice == SEQID_GENERAL)
				{
					db_tag = sip->data.ptrvalue;
					if(db_tag->db != NULL)
						html_buf = find_db_html_pointer (db_tag->db, grp->locus);
				}
			}


			if(match_type == MATCH_QUERY)
			{
				if(is_html)
				{
					if(html_buf != NULL)
						fprintf(fp, "Locus: %s<B>%s</B></a>\n", html_buf, grp->locus);
					else
						fprintf(fp, "Locus: <B>%s</B>\n", grp->locus);
				}
				else
					fprintf(fp, "Locus: [%s]\n", grp->locus);
			}
			else
			{
				if(is_html && html_buf != NULL)
					fprintf(fp, "Locus: %s%s</a>\n", html_buf, grp->locus);
				else
					fprintf(fp, "Locus: %s\n",  grp->locus);
			}
		}
		if(grp->desc)
		{
			sprintf(buf, "Description: %s\n", grp->desc);
			SendTextToFile (fp, buf, &prt_par, &prt_col);
		}

		if(grp->syn)
			print_gene_synonym(grp->syn, fp, buf, 1000, match_type);

		if(grp->db)
			print_gene_dblst(grp->db, fp, buf, 1000, match_type, is_html);
		if(grp->allele)
			fprintf(fp, "Allele: %s\n", grp->allele);
		if(grp->maploc)
			fprintf(fp, "MapLoc: %s\n", grp->maploc);

		if(grp->pseudo)
			fprintf(fp, "Pseudo Gene\n");

	}
	print_citation (sfp, fp, buf, 1000, is_html);
	print_sfp_ext (sfp, fp, buf, 1000);

	if(print_loc == PRINT_LOC_BOGO || np == NULL)
	{
		fprintf(fp, "Region %ld-%ld\n", (long) SeqLocStart(sfp->location), 
			(long) SeqLocStop(sfp->location));
	}
	else if(print_loc == PRINT_LOC_NUM)
	{
		if(np != NULL)
		{
			map_unit_label(SeqLocStart(sfp->location), np, buf, FALSE);
			fprintf(fp, "Region %s\n", buf);
		}
	}
	fprintf(fp, "\n");
	return TRUE;

}

CharPtr WWW_PrintGeneRef(SeqFeatPtr sfp, Uint1 match_type, Uint1 print_loc, NumberingPtr np, Boolean is_html)
{
	GeneRefPtr grp = NULL;
	SeqIdPtr sip;
	DbtagPtr db_tag;
	CharPtr html_buf, text = NULL;

	if (sfp == NULL) {
		return NULL;
	}
	if (sfp->data.choice == SEQFEAT_GENE) {
		grp = sfp->data.value.ptrvalue;
	}
	if(grp == NULL) {
		return NULL;
	}
	if(grp->locus)
	{
		html_buf = NULL;
		if(is_html)
		{
			html_buf = NULL;
			sip = SeqLocId(sfp->location);
			if(sip->choice == SEQID_GENERAL)
			{
				db_tag = sip->data.ptrvalue;
				if(db_tag->db != NULL)
					html_buf = find_db_html_pointer (db_tag->db, grp->locus);
			}
		}


		if(match_type == MATCH_QUERY)
		{
			if(is_html)
			{
				if(html_buf != NULL) {
					text = MemNew(StringLen(html_buf) + StringLen(grp->locus) + 20);
					sprintf(text, "Locus: %s<B>%s</B></a>\n", html_buf, grp->locus);
				} else {
					text = MemNew(StringLen(grp->locus) + 20);
					sprintf(text, "Locus: <B>%s</B>", grp->locus);
				}
			} else {
				text = MemNew(StringLen(grp->locus) + 20);
				sprintf(text, "Locus: [%s]\n", grp->locus);
			}
		} else {
			if(is_html && html_buf != NULL) {
				text = MemNew(StringLen(html_buf) + StringLen(grp->locus) + 20);
				sprintf(text, "Locus: %s%s</a>\n", html_buf, grp->locus);
			} else {
				text = MemNew(StringLen(grp->locus) + 20);
				sprintf(text, "Locus: %s",  grp->locus);
			}
		}
	}
	if(grp->desc)
	{
		text = MemNew(StringLen(grp->desc) + 30);
		sprintf(text, "Description: %s", grp->desc);
	}
	return text;

}

static void print_store_for_html(CharPtr temp, CharPtr html_buf, Int4Ptr c_pos, Int4 width, FILE *fp)
{
	Int4 t_len;
	CharPtr str;
	Int4 from, to;
	Char white_sp;
	Char str_val[201];

	t_len = StringLen(temp);
	if(t_len + (*c_pos) <= width)
	{
		if(html_buf != NULL)
			fprintf(fp, "%s", html_buf);
		fprintf(fp, "%s", temp);
		if(html_buf != NULL)
			fprintf(fp, "</a>");

		*c_pos += t_len;
		if(*c_pos == width)
		{
			fprintf(fp, "<br>");
			fprintf(fp, "\n");
			*c_pos = 0;
		}
	}
	else
	{
		if(html_buf != NULL)
			fprintf(fp, "%s", html_buf);
		white_sp = '\0';
		t_len = 0;
		from = 0;
		to = -1;
		for(str = temp; str != NULL && *str !='\0'; ++str)
		{
			if(IS_WHITESP(*str))
			{
				if(t_len > 0)
				{
					if((to - from +1) + t_len + *c_pos > width)
					{
						StringNCpy(str_val, temp+from, to-from+1);
						str_val[to-from+1] = '\0';
						fprintf(fp, "%s\n", str_val);
						*c_pos = 0;
						from = to+1;
					}
					to += (t_len +1);
					if(*str == '\n')
					{
						StringNCpy(str_val, temp+from, to-from+1);
						str_val[to-from+1] = '\0';
						fprintf(fp, "%s", str_val);
						*c_pos = 0;
						from = to +1;
					}
					
				}
				else
				{
					to += 1;
				}
				t_len = 0;
				white_sp = *str;

			}
			else
				++t_len;
		}

		if(to - from +1 > 0)
		{
			*c_pos += to - from +1;
			StringNCpy(str_val, temp+from, to-from+1);
			str_val[to-from+1] = '\0';
			fprintf(fp, "%s", str_val);
			if(html_buf != NULL)
				fprintf(fp, "</a>");

			if(*c_pos == width)
			{
				fprintf(fp, "<br>");
				fprintf(fp, "\n");
				*c_pos =0;
			}
		}
		if(html_buf != NULL)
			fprintf(fp, "</a>");
	}
}



static void print_seqid PROTO((SeqIdPtr sip, CharPtr buf, Int2 buf_size));

static Boolean print_store_seqid(SeqIdPtr sip, CharPtr buf, Int4 buf_size, Int4Ptr len, FILE *fp, Boolean is_html, Int4Ptr index, Boolean first)
{
	Char temp[101];
	CharPtr html_buf;
	DbtagPtr db_tag;
	ObjectIdPtr oip;

	if(sip == NULL)
		return FALSE;
	if(!first)
	{
		StringCpy(temp, " ");
		if(is_html)
			print_store_for_html(temp, NULL, index, (Int4)(prt_col.width), fp);
		else
			print_store_buf(temp, buf, buf_size, len, fp);
	}
	html_buf = NULL;

	if(sip->choice == SEQID_GI)
	{
		MuskSeqIdWrite(sip, temp, 100, PRINTID_TEXTID_ACCESSION, TRUE, TRUE);
		if(is_html)
		{
			sprintf(HTML_buffer, "<a href=\"http://www3.ncbi.nlm.nih.gov/"
                "htbin-post/Entrez/query?form=6&dopt=g&db=n&"
                 "uid=%08ld\">", (long) sip->data.intvalue);
			html_buf = HTML_buffer;
		}
	}
	else 
	{
		if(sip->choice == SEQID_GENERAL)
		{
			db_tag = sip->data.ptrvalue;
			if(is_html && StringICmp(db_tag->db, "UNIGENE") == 0)
			{
				oip = db_tag->tag;
				sprintf(HTML_buffer, "<a href=\"http://www.ncbi.nlm.nih.gov/"
                "cgi-bin/Schuler/clust2html?Homo+sapiens+%ld\">",(long) oip->id);
				html_buf = HTML_buffer;
			}
			print_seqid (sip, temp, 100);
		}
		else
			MuskSeqIdWrite(sip, temp, 100, PRINTID_TEXTID_ACCESSION, FALSE, FALSE);
	}

	if(is_html)
		print_store_for_html(temp, html_buf, index, (Int4)(prt_col.width), fp);
	else
		print_store_buf(temp, buf, buf_size, len, fp);
	return TRUE;
}

static Boolean fetch_this_bioseq(SeqIdPtr sip)
{
	ObjectIdPtr oip;
	DbtagPtr db_tag;

	if(sip->choice == SEQID_LOCAL)
	{
		oip = sip->data.ptrvalue;
		if(oip->str)
			return (StringNCmp(oip->str, "HGM", 3) == 0);
	}
	else if(sip->choice == SEQID_GENERAL)
	{
		db_tag = sip->data.ptrvalue;
		if(StringCmp(db_tag->db, "HUMGEN") == 0)
		{
			oip = db_tag->tag;
			return (oip->id > 0);
		}
	}

	return FALSE;
}

static Boolean PrintSequenceToFile(SeqLocPtr slp, FILE *fp, Uint1 print_loc, NumberingPtr np, GatherRangePtr grp, Boolean is_html)
{
	SeqIdPtr sip;
	BioseqPtr bsp;
	SeqAlignPtr align;
	Int4 len;
	Char temp[101];
	Char temp_1[41], temp_2[41];
	DenseSegPtr dsp;
	Char buf[1001];
	Int4 buf_size = 1000;
	Int4 index = 0;
	Boolean first;
	SeqLocPtr t_slp;

	if(slp == NULL)
		return FALSE;
	sip = SeqLocId(slp);
	len = 0;
	print_store_seqid(sip, buf, buf_size, &len, fp, is_html, &index, TRUE);


	if(print_loc != PRINT_LOC_NONE)
	{	/*print out the location on the genome */
		if(print_loc == PRINT_LOC_BOGO || np == NULL)
			sprintf(temp, ": %ld - %ld (bp)", (long) grp->left, (long) grp->right);
		else
		{

			map_unit_label(grp->left, np, temp_1, FALSE);
			map_unit_label(grp->right, np, temp_2, FALSE);

			sprintf(temp, ": %s - %s (bp) on the genome", temp_1, temp_2);
		}
		if(is_html)
		{
			StringCat(temp, "<br>");
			StringCat(temp, "\n");
			print_store_for_html(temp, NULL, &index, (Int4)prt_col.width, fp);
		}
		else
			print_store_buf(temp, buf, buf_size, &len, fp);
	}
	if(len > 0)
	{
		StringCat(buf, "\n");
		SendTextToFile (fp, buf, &prt_par, &prt_col);
	}

	if(fetch_this_bioseq(sip))
	{	/* fetch the hum-gen record */
		bsp = BioseqLockById(sip);
		if(bsp != NULL)
		{
			if(bsp->hist != NULL)
			{
				len = 0;
				index = 0;
				align = bsp->hist->assembly;
				first = TRUE;
				while(align)
				{
					if(align->segtype == 2)
					{
						dsp = align->segs;
						sip = dsp->ids->next;
						print_store_seqid(sip, buf, buf_size, &len, fp, is_html, &index, first);
						first = FALSE;
					}
					align = align->next;
				}
				if(!is_html)
				{
					if(len > 0)
					{
						StringCat(buf, "\n");
						SendTextToFile (fp, buf, &prt_par, &prt_col);
					}
				}
				else if(index > 0) {
					fprintf(fp, "<br>");
					fprintf(fp, "\n");
				}
			}
			else if(bsp->repr == Seq_repr_seg && bsp->seq_ext_type == 1)
			{
				t_slp = bsp->seq_ext;
				len = 0;
				index = 0;
				first = TRUE;
				while(t_slp)
				{
					if(SeqLocId(t_slp) != NULL)
						print_store_seqid(SeqLocId(t_slp), buf, buf_size, &len, fp, is_html, &index, first);
					first = FALSE;
					t_slp = t_slp->next;
				}
		
				if(!is_html)
				{
					if(len > 0)
					{
						StringCat(buf, "\n");
						SendTextToFile (fp, buf, &prt_par, &prt_col);
					}
				}
				else if(index > 0) {
					fprintf(fp, "<br>");
					fprintf(fp, "\n");
				}
			}
			BioseqUnlock(bsp);
		}
	}
	if(is_html)
		fprintf(fp, "<br>");
	fprintf(fp, "\n");

	return TRUE;
}





	




static Boolean is_sfp_query_gene (GeneDataPtr gdata, Uint2 itemID, 
								  Uint2 entityID, Uint2 itemType)
{
	while(gdata)
	{
		if(gdata->entityID == entityID && gdata->itemID == itemID && 
			gdata->itemType == itemType)
			return TRUE;
		gdata = gdata->next;
	}

	return FALSE;
}

static Boolean is_loc_match_align (GeneDataPtr gdata, SeqFeatPtr sfp)
{
	StdSegPtr ssp;
	SeqLocPtr slp;

	while(gdata)
	{
		if(gdata->align_seg != NULL)
		{
			ssp = gdata->align_seg->data.ptrvalue;
			for(slp = ssp->loc; slp != NULL; slp = slp->next)
			{
				if(SeqLocCompare (slp, sfp->location) == SLC_A_EQ_B)
					return TRUE;
			}
		}
		gdata = gdata->next;
	}

	return FALSE;
}



static void print_seqid (SeqIdPtr sip, CharPtr buf, Int2 buf_size)
{
	DbtagPtr db_tag;
	ObjectIdPtr oip;

	if(sip->choice == SEQID_GENERAL)
	{
		db_tag = sip->data.ptrvalue;
		oip = db_tag->tag;
		if(oip->str != NULL)
			sprintf(buf, "%s %s", db_tag->db, oip->str);
		else
			sprintf(buf, "%s %ld", db_tag->db, (long) oip->id);
	}
	else
		SeqIdWrite(sip, buf, PRINTID_TEXTID_ACCESSION, buf_size);
}


static void print_bioseq_descriptor (BioseqPtr bsp, SeqLocPtr slp, FILE *fp, Int2 line_len)
{
	Int2 i;
	Char buf[1001];
	ValNodePtr descr;

	for(i =0; i<line_len; ++i)
		fprintf(fp, "*");
	fprintf(fp, "\n");
	print_seqid (bsp->id, buf, 1000);
	fprintf(fp, "%s\n", buf);

	for(descr = bsp->descr; descr != NULL; descr = descr->next)
	{
		if(descr->choice == Seq_descr_title || descr->choice == Seq_descr_name)
		{
			sprintf(buf, "%s\n", (CharPtr)(descr->data.ptrvalue));
			SendTextToFile(fp, buf, &prt_par, &prt_col);
		}
	}
	sprintf (buf, "Region: %ld-%ld(bp) on the Chromosome\n", (long) SeqLocStart(slp), (long) SeqLocStop(slp));
	SendTextToFile(fp, buf, &prt_par, &prt_col);
	for(i =0; i<line_len; ++i)
		fprintf(fp, "*");
	fprintf(fp, "\n");

}


static Boolean prtgenefunc(GatherContextPtr gcp)
{

	PrintGenePtr pgp;
	SeqFeatPtr sfp;
	Uint1 match_type;
	SeqLocPtr slp;

	if(gcp == NULL)
		return FALSE;

	pgp = (PrintGenePtr)(gcp->userdata);
	if(pgp == NULL)
		return FALSE;

	switch(gcp->thistype)
	{
	case OBJ_SEQFEAT:
	case OBJ_BIOSEQ_MAPFEAT:

		sfp = (SeqFeatPtr)(gcp->thisitem);
		if(sfp == NULL || sfp->data.choice != 1)
			return TRUE;

		match_type = MATCH_NONE;
		if(is_sfp_query_gene (pgp->g_data, gcp->itemID,
			gcp->entityID, gcp->thistype))
			match_type = MATCH_QUERY;
		else if (is_loc_match_align (pgp->g_data, sfp))
			match_type = MATCH_ALIGN;
		if(pgp->prev_type != GENE_MARKER)
		{
			pgp->prev_type = GENE_MARKER;
			fprintf(pgp->fp, "\nSTS Markers\n\n");
		}

		PrintGeneRefToFile(sfp, pgp->fp, match_type, pgp->print_loc, pgp->np, pgp->html);
		pgp->found = TRUE;
		break;

	case OBJ_BIOSEQ_SEG:	/*segments of a segmented sequence*/
		slp = (SeqLocPtr)(gcp->thisitem);
		if(!is_map_segment(slp))
		{
			if(pgp->prev_type != SEQ_SEGMENT)
			{
				pgp->prev_type = SEQ_SEGMENT;
				fprintf(pgp->fp, "\nSequence Data\n\n");
			}

			PrintSequenceToFile(slp, pgp->fp, pgp->print_loc, pgp->np, &(gcp->extremes), pgp->html);
			pgp->found = TRUE;
		}
		break;
	default:
		break;
	}
	return TRUE;
}


/******************************************************************
*
*	print the everything in the interval slp to a text file
*	slp: the interval
*	g_data: the query gene
*	is_html: make it a html page
*	line_len; the length of the file
*	fp: the output file
*
********************************************************************/	
Boolean print_genome_interval(SeqLocPtr slp, GeneDataPtr g_data, Boolean is_html, Int2 line_len, FILE *fp)
{
	GatherScope gs;
	PrintGene pg;
	SeqEntryPtr sep;
	BioseqPtr bsp;

	if(slp == NULL || fp == NULL)
		return FALSE;
	sep = SeqEntryFind(SeqLocId(slp));
	if(sep == NULL)
		return FALSE;
	bsp = BioseqFind(SeqLocId(slp));
	if(bsp == NULL)
		return FALSE;

	pg.g_data = g_data;
	pg.html = is_html;
	pg.fp = fp;
	pg.found = FALSE;
	pg.print_loc = PRINT_LOC_NUM;
	pg.np = getBioseqNumbering(bsp);
	pg.prev_type = 0;

	prt_col.width = line_len;
	print_bioseq_descriptor (bsp, slp, fp, line_len);

	
	MemSet((Pointer)(&gs), 0, sizeof(GatherScope));
	MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)OBJ_MAX * sizeof(Boolean));
	gs.ignore[OBJ_SEQFEAT] = FALSE;
	gs.ignore[OBJ_SEQANNOT] = FALSE;
	gs.ignore[OBJ_BIOSEQ_MAPFEAT] = FALSE;
	gs.ignore[OBJ_BIOSEQ] = FALSE;
	if(bsp->repr == Seq_repr_seg && bsp->seq_ext_type == 1)
		gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
	gs.nointervals = TRUE;
	gs.target = slp;
	gs.offset = SeqLocStart(slp);
	gs.get_feats_location = TRUE;
	GatherSeqEntry(sep, (Pointer)(&pg), prtgenefunc, &gs);

	if(pg.found == FALSE)
		fprintf(fp, "None\n");
	return (pg.found);
}

