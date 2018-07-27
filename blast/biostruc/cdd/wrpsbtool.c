/* $Id: wrpsbtool.c,v 1.11 2001/03/09 22:52:01 bauer Exp $
*===========================================================================
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
* File Name:  wrpsbtool.c
*
* Author:  Aron Marchler-Bauer
*
* Initial Version Creation Date: 4/19/2000
*
* $Revision: 1.11 $
*
* File Description:
*         tools for WWW-RPS BLAST 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: wrpsbtool.c,v $
* Revision 1.11  2001/03/09 22:52:01  bauer
* use JavaScript to embed graphics in results page
*
* Revision 1.10  2001/03/02 23:36:59  bauer
* more verbose Error messages
*
* Revision 1.9  2001/02/22 15:53:22  bauer
* support for jagged edge blocks
*
* Revision 1.8  2001/01/11 21:52:21  bauer
* fixed typo
*
* Revision 1.7  2000/12/29 20:32:40  bauer
* Changed header string in hitlist table
*
* Revision 1.6  2000/10/12 20:48:26  bauer
* added absolute URLs for graphics elements on the CD-search results pages
*
* Revision 1.5  2000/08/03 22:02:24  bauer
* added support for 3D-structure link highlighting
*
* Revision 1.4  2000/08/01 21:23:03  bauer
* added Entrez-Hotlink for Query
*
* Revision 1.3  2000/07/24 15:43:37  bauer
* fixed problem with pairwise alignment displays involvinb profile-only CDs
*
* Revision 1.2  2000/07/19 19:01:05  bauer
* removed debugging constructs from WRPSBCl3DisplayBlastPairList
*
* ==========================================================================
*/

#include <ncbi.h>
#include <objcode.h>
#include <objseq.h>
#include <sequtil.h>
#include <blastdef.h>
#include <ddvcreate.h>
#include <wrpsb.h>
#include <wrpsbtool.h>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Int4 max(Int4 i1, Int4 i2)
{ 
  if (i1 >= i2) return(i1);
  return(i2);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Int4 min(Int4 i1, Int4 i2)
{ 
  if (i1 <= i2) return(i1);
  return(i2);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* checks for general overlap (i.e. two intervals have at least one position */
/* in common                                                                 */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Boolean OverlapInterval(Int4 from1, Int4 to1, Int4 from2, Int4 to2)
{
  if (from1 <= to2 && to1 >= from2) return (TRUE);
  else return (FALSE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* checks for a mutal overlap exceeding a fraction defined in wrpsb.h        */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Boolean OverlapMutual(Int4 from1, Int4 to1, Int4 from2, Int4 to2)
{
  Int4    len1, from;
  Int4    len2, to;
  Int4    overlap;
  FloatHi mutual;

  if (from1 <= to2 && to1 >= from2) {
    len1 = to1-from1+1;
    len2 = to2-from2+1;
    from = max(from1, from2);
    to   = min(to1, to2);
    overlap = to - from + 1;
    mutual = .5*((FloatHi)overlap/(FloatHi)len1+(FloatHi)overlap/(FloatHi)len2);
    if (mutual >= MUTUAL_OVERLAP) return (TRUE);
  } 
  return (FALSE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void WRPSBHtmlError(CharPtr cErrTxt) 
{
  printf("Content-type: text/html\n\n");
  printf("<HTML>\n");
  printf("<TITLE>CD-Search Error</TITLE>\n");
  printf("<BODY BGCOLOR=#FFFFFF>\n");
  printf("<A HREF=\"blast_form.map\">\n");
  printf("<IMG SRC=\"%scdsearch.gif\" BORDER=0 ISMAP></A>\n",URLcgi);
  printf("<h2>WRPSB Error:</h2>\n");
  printf("<h3>%s</h3>\n",cErrTxt);
  printf("</BODY></HTML>\n");
  exit(1);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* formatted Printing of E-values and bit scores                             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Boolean print_score_eonly(FloatHi evalue, CharPtr buf)
{
  Char eval_buff[101];

  buf[0] = '\0';
  eval_buff[0] = '\0';
  if (evalue < 1.0e-180)     sprintf(eval_buff, "0.0");
  else if (evalue < 1.0e-99) sprintf(eval_buff, "%2.0e", evalue);
  else if (evalue < 0.0009)  sprintf(eval_buff, "%3.0e", evalue);
  else if (evalue < 0.1)     sprintf(eval_buff, "%4.3f", evalue);
  else if (evalue < 1.0)     sprintf(eval_buff, "%3.2f", evalue);
  else if (evalue < 10.0)    sprintf(eval_buff, "%2.1f", evalue);
  else                       sprintf(eval_buff, "%5.0f", evalue);
  if (eval_buff[0] != '\0') {
    sprintf(buf, "%s", eval_buff);
    return TRUE;
  }
  return FALSE;
}

Boolean print_score_sonly(FloatHi bit_score, CharPtr buf)
{
  Char bit_score_buff[101];

  buf[0] = '\0';
  bit_score_buff[0] = '\0';
  if (bit_score > 9999)      sprintf(bit_score_buff, "%4.3e", bit_score);
  else if (bit_score > 99.9) sprintf(bit_score_buff, "%4.0ld", (long)bit_score);
  else                       sprintf(bit_score_buff, "%4.1f", bit_score);
  if (bit_score_buff[0] != '\0') {
    sprintf(buf, "%s", bit_score_buff);
    return TRUE;
  }
  return FALSE;
}

Boolean print_score_evalue (FloatHi evalue, FloatHi bit_score, CharPtr buf)
{
  Char eval_buff[101];
  Char bit_score_buff[101];

  buf[0] = '\0';
  eval_buff[0] = '\0';
  bit_score_buff[0] = '\0';
  if (evalue < 1.0e-180)     sprintf(eval_buff, "0.0");
  else if (evalue < 1.0e-99) sprintf(eval_buff, "%2.0e", evalue);
  else if (evalue < 0.0009)  sprintf(eval_buff, "%3.0e", evalue);
  else if (evalue < 0.1)     sprintf(eval_buff, "%4.3f", evalue);
  else if (evalue < 1.0)     sprintf(eval_buff, "%3.2f", evalue);
  else if (evalue < 10.0)    sprintf(eval_buff, "%2.1f", evalue);
  else                       sprintf(eval_buff, "%5.0f", evalue);
  if (bit_score > 9999)      sprintf(bit_score_buff, "%4.3e", bit_score);
  else if (bit_score > 99.9) sprintf(bit_score_buff, "%4.0ld", (long)bit_score);
  else                       sprintf(bit_score_buff, "%4.1f", bit_score);
  if (eval_buff[0] != '\0' && bit_score_buff[0] != '\0') {
    sprintf(buf, "S=%s E=%s", bit_score_buff, eval_buff);
    return TRUE;
  }
  if (eval_buff[0] != '\0') {
    sprintf(buf, "E=%s", eval_buff);
    return TRUE;
  }
  if (bit_score_buff[0] != '\0') {
    sprintf(buf, "S=%s", bit_score_buff);
    return TRUE;
  }
  return FALSE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* print nicely formatted summary lines for the alignment display            */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void WRPSBPrintDefLinesFromSeqAlign(AlignmentAbstractPtr aap, FILE *table,
                                    Boolean bAnyPdb, BioseqPtr query_bsp,
				    CharPtr urlcgi)
{
  Char                 dupstr[PATH_MAX];
  Char                 cTmp[16];
  CharPtr              part1, part2;
  CharPtr              buf;
  AlignmentAbstractPtr aapThis;
  Int4                 i, k;
  Int4                 maxsubjaln, nsegments;
  SeqPortPtr           spp;
  SeqAlignPtr          salp4;
  DenseSegPtr          dsp;
  Char                 aln[CDD_MAX_DESCR];
  Char                 cTmp2[16];
  

  buf = MemNew((query_bsp->length+1)*sizeof(Char));
  spp = FastaSeqPort((BioseqPtr)query_bsp,FALSE,FALSE,Seq_code_ncbieaa);
  FastaSeqLine(spp,buf,query_bsp->length,FALSE);

  fprintf(table,"<TABLE CELLPADDING=1 CELLSPACING=1 BORDER=0 width=600>\n");
  fprintf(table,"<TR>\n");
  if (bAnyPdb) fprintf(table,"<TD NOWRAP VALIGN=BOTTOM ALIGN=CENTER>&nbsp;</TD>\n");
  fprintf(table,"<TD COLSPAN=2 NOWRAP VALIGN=BOTTOM ALIGN=LEFT>PSSMs producing significant alignments:</TD>\n");
  fprintf(table,"<TD NOWRAP VALIGN=BOTTOM ALIGN=CENTER>Score<br>(bits)</TD>\n");
  fprintf(table,"<TD NOWRAP VALIGN=BOTTOM ALIGN=CENTER>E<br>value</TD>\n");
  fprintf(table,"</TR>\n");
  fprintf(table,"<TR><TD COLSPAN=4>&nbsp</TD></TR>\n");
  aapThis = aap;
  while(aapThis) {
    fprintf(table,"<TR>\n");
    if (bAnyPdb) {
      if (aapThis->bHasStructure) {
        fprintf(table,"<TD><A HREF=\"");
        fprintf(table,"%s",aapThis->cHtmlLink);
        salp4 = aapThis->salp; salp4->next = NULL;
	dsp = salp4->segs;
	nsegments = 0;
        for (i=0;i<dsp->numseg;i++) {
          if (dsp->starts[i*2] != -1 && dsp->starts[i*2 + 1] != -1) nsegments++;
        }
        aln[0]='\0'; maxsubjaln = 0;
        sprintf(aln,"%d",nsegments);
        for (i=0;i<dsp->numseg;i++) {
          if (dsp->starts[i*2] != -1 && dsp->starts[i*2 + 1] != -1) {
            cTmp2[0]='\0';
            sprintf(cTmp2,",%d,%d,%d",dsp->starts[i*2+1],dsp->starts[i*2],dsp->lens[i]);
	    maxsubjaln = dsp->starts[i*2+1]+dsp->lens[i]-1;
            strcat(aln,cTmp2);
          }
        }
        aln[strlen(aln)]='\0';
	fprintf(table, "&query=%s",buf);
	fprintf(table, "&aln=%s",aln);
	fprintf(table, "&ascbin=0&maxaln=10&seltype=3");
	fprintf(table,"\"><IMG SRC=\"%spinkb.gif\" BORDER=0></A></TD>\n",urlcgi);
      } else {
        fprintf(table,"<TD>&nbsp;</TD>\n");
      }
    }
    strncpy(dupstr,aapThis->long_defline,PATH_MAX);
    part1 = strtok(dupstr," ");
    part2 = dupstr + strlen(part1) + 1;
    part2[min(strlen(part2),65)] = '\0';
    if (strlen(part2) == 65) {
      part2[64]='.'; part2[63]='.'; part2[62]='.';
    }
    fprintf(table,"<TD NOWRAP ALIGN=LEFT><a href=\"%s\" TARGET=\"_new\">%s</a></TD>\n",
            aapThis->cHtmlLink,part1);
    fprintf(table,"<TD NOWRAP ALIGN=LEFT>%s</TD>\n",part2);
    print_score_sonly(aapThis->defline->bit_score,cTmp);
    fprintf(table,"<TD NOWRAP ALIGN=CENTER><a href=\"#%s\">%s</a></TD>\n",aapThis->name,cTmp);
    print_score_eonly(aapThis->defline->evalue,cTmp);
    fprintf(table,"<TD NOWRAP ALIGN=CENTER>%s</TD>\n",cTmp);
    aapThis = aapThis->next;
    fprintf(table,"</TR>\n");
  }
  fprintf(table,"</TABLE>\n");
  fprintf(table,"<br>\n");
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Prints an acknowledgement of the Blast Query in the standard BLAST format.*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Boolean WRPSBAcknowledgeBlastQuery(BioseqPtr bsp, Int4 line_length, FILE *outfp, Boolean believe_query, Boolean html)

{
  Char buffer[BUFFER_LENGTH];

  if (bsp == NULL || outfp == NULL) return FALSE;
  asn2ff_set_output(outfp, NULL);

  ff_StartPrint(0, 0, line_length, NULL);
  if (html) ff_AddString("<b>Query=</b> ");
  else ff_AddString("Query= ");
  if (bsp->id && believe_query) {
    if (html) {
/*---------------------------------------------------------------------------*/
/* add hotlink to Entrez/Protein if query sequence is accessible there       */
/*---------------------------------------------------------------------------*/
      ff_AddString("<a href=\"http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?db=Protein&term=");
      SeqIdWrite(bsp->id,buffer,PRINTID_TEXTID_ACC_ONLY, BUFFER_LENGTH);
/*---------------------------------------------------------------------------*/
/* must fix links to pdb_derived sequences with chain-id's                   */
/*---------------------------------------------------------------------------*/
      if (strlen(buffer) == 6 && buffer[4]=='_') {
        buffer[4] = buffer[5];
	buffer[5] = buffer[6];
      }
      ff_AddString(buffer);
      ff_AddString("\">");
    }
    SeqIdWrite(bsp->id, buffer, PRINTID_FASTA_LONG, BUFFER_LENGTH);
    ff_AddString(buffer);
    if (html) {
      ff_AddString("</a>");
    }
    ff_AddChar(' ');
  } else {
    ff_AddString("local sequence:");
    ff_AddChar(' ');
  }
  ff_AddString(BioseqGetTitle(bsp));
  NewContLine();
  TabToColumn(10);
  ff_AddChar('(');
  ff_AddString(Ltostr((long) BioseqGetLen(bsp), 1));
  ff_AddString(" letters)");
/*  NewContLine(); */
  ff_EndPrint();
  return TRUE;
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Counts the number of SeqAligns present. Ignores identical subject-id's    */
/* and threats them like separate alignments                                 */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Int4 WRPSBGetSeqAlignCount(SeqAlignPtr sap)
{
  Int4     count = 0;

  while (sap) {
   count++;
    sap = sap->next;
  }
  return count;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Duplicates a SeqAlignPtr, up to the number of records specified.          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
SeqAlignPtr WRPSBGetPrivateSeqAlign(SeqAlignPtr sap, Int4 number, Int4Ptr number_returned)
{
  Int4        count=0;
  SeqAlignPtr new_head=NULL, var, thisSap, new_tail;


  for (var = sap, count=0; var != NULL; var=var->next, count++) {
    if (count >= number) break;
    thisSap = AsnIoMemCopy(var,(AsnReadFunc)SeqAlignAsnRead,(AsnWriteFunc)SeqAlignAsnWrite);
    if (new_head) {
      new_tail->next = thisSap;
      new_tail = thisSap;
      new_tail->next = NULL;  
    } else {
      new_head = thisSap;
      new_head->next = NULL;
      new_tail = new_head;
    }
  }
  *number_returned = count;
  return new_head;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Duplicate a SeqAlignPtr, keeping on the number of db hits specified.      */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
BlastPruneSapStructPtr WRPSBlastPruneHitsFromSeqAlign(SeqAlignPtr sap, Int4 number, Nlm_FloatHi ecutoff, BlastPruneSapStructPtr prune)
{
  SeqAlignPtr sapThis, sapLast = NULL, sapHead = NULL;
  Nlm_FloatHi bit_score, evalue;
  Int4        score, xnumber;

  sapThis = sap;
  while (sapThis) {
    if (GetScoreAndEvalue(sapThis, &score, &bit_score, &evalue, &xnumber)) {
      if (evalue <= ecutoff) {
        if (!sapHead) { 
          sapHead = sapThis; 
          sapLast = sapThis;
        } else {
          sapLast->next = sapThis;
          sapLast = sapThis;
        }
      }
    }
    sapThis=sapThis->next;
    if (sapLast) sapLast->next = NULL;
  }
  if (prune == NULL) {
    prune = MemNew(sizeof(BlastPruneSapStruct));
  } else {
    if (prune->number == number) return prune;
    if (prune->allocated) prune->sap = SeqAlignSetFree(prune->sap);
    prune->sap = NULL;
    prune->allocated = FALSE;
    prune->original_number = 0;
    prune->number = 0;
  }
  prune->original_number = WRPSBGetSeqAlignCount(sapHead);
  if (prune->original_number < number) {
    prune->number = prune->original_number;
    prune->sap = sapHead;
    prune->allocated = FALSE;
  } else {
    prune->sap = WRPSBGetPrivateSeqAlign(sapHead, number, &(prune->number));
    prune->allocated = TRUE;
  }
  return prune;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Get rid of the BlastPruneSapStruct again                                  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
BlastPruneSapStructPtr WRPSBlastPruneSapStructDestruct(BlastPruneSapStructPtr prune)
{
  if (prune == NULL) return NULL;

  if (prune->allocated) {
    prune->sap = SeqAlignSetFree(prune->sap);
  }
  prune = MemFree(prune);
  return prune;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static CharPtr StringAppend(CharPtr *dst, size_t *size, CharPtr src, size_t *used)
{
  size_t pos, len;

  if (*dst == NULL) {
    *size = 1;
    pos = 0;
  }
  else {
    pos = *used;
  }
  if (src == NULL) {
    return *dst;
  }
  len = StringLen(src);
  *used += len;
  if (*dst == NULL || pos + len + 1 > *size) {
    /**
     * extending destination buffer
     */
    CharPtr old = *dst;
    for (; pos + len + 1 > *size; *size *= 2);
    *dst = (CharPtr)MemNew(*size);
    **dst = '\0';
    if (old != NULL) {
      StringCpy(*dst, old);
      MemFree(old);
    }
  }
  StringCpy((*dst) + pos, src);
  return *dst;
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Boolean SeqAlignSegsStr(SeqAlignPtr sap, Int2 index, CharPtr *dst, size_t *size, size_t *used)
{
        Char buf[128];
        Int4 start, stop;

        start = SeqAlignStart(sap, 1);
        stop = SeqAlignStop(sap, 1);

        if (sap == NULL) {
                return FALSE;
        }

        sprintf(buf, "%ld-%ld", (long)(start), (long)(stop));
        StringAppend(dst, size, buf, used);

        return TRUE;
}

/*******************************************************************************

  Function : DDV_BlastGetColorSchemaZero()
  
  Purpose : BLAST color sheme 0 : standard b&w BLAST output
  
*******************************************************************************/
static void DDV_BlastGetColorSchemaZero(DDVOptionsBlockPtr dobp)
{
	memset(dobp,0,sizeof(DDVOptionsBlock));
	
	dobp->bUseLayout=FALSE;
}

/*******************************************************************************

  Function : DDV_BlastGetColorSchemaOne()
  
  Purpose : BLAST color sheme 1 :
              - masked regions in lowe case,
			  - everything else in upper case.
  
*******************************************************************************/
static void DDV_BlastGetColorSchemaOne(DDVOptionsBlockPtr dobp)
{
	memset(dobp,0,sizeof(DDVOptionsBlock));
	
	dobp->bUseLayout=TRUE;

	dobp->LayoutType=DDV_USE_MASKLAYOUT;

	dobp->MASKlayout.style=DDV_TXTSTYLE_LOWERCASE;
}

/*******************************************************************************

  Function : DDV_BlastGetColorSchemaTwo()
  
  Purpose : BLAST color sheme 2 :
              - masked regions in lower case, gray letters,
			  - UnAligned regions in italic,
			  - everything else in upper case.
  
*******************************************************************************/
static void DDV_BlastGetColorSchemaTwo(DDVOptionsBlockPtr dobp)
{
	memset(dobp,0,sizeof(DDVOptionsBlock));
	
	dobp->bUseLayout=TRUE;

	dobp->LayoutType=DDV_USE_MASKLAYOUT|DDV_USE_NOMLAYOUT;
	dobp->LayoutType|=DDV_USE_UALAYOUT;

	dobp->UAlayout.style=DDV_TXTSTYLE_ITALIC;
	dobp->normlayout.style=DDV_TXTSTYLE_BOLD;

	dobp->MASKlayout.style=DDV_TXTSTYLE_LOWERCASE|DDV_TXTSTYLE_COLOR;
	dobp->MASKlayout.rgb[0]=102;
	dobp->MASKlayout.rgb[1]=102;
	dobp->MASKlayout.rgb[2]=102;
}

/*******************************************************************************

  Function : DDV_BlastGetColorSchemaThree()
  
  Purpose : BLAST color sheme 3 :
              - masked regions in lower case, gray letters,
			  - UnAligned regions in italic,
			  - Identity/Similarity colors
			  - everything else in upper case.
  
*******************************************************************************/
static void DDV_BlastGetColorSchemaThree(DDVOptionsBlockPtr dobp)
{
	memset(dobp,0,sizeof(DDVOptionsBlock));
	
	dobp->bUseLayout=TRUE;

	dobp->LayoutType=DDV_USE_MASKLAYOUT;
	dobp->LayoutType|=DDV_USE_UALAYOUT;
	dobp->LayoutType|=DDV_USE_ISOLAYOUT;

	dobp->UAlayout.style=DDV_TXTSTYLE_ITALIC;

	dobp->MASKlayout.style=DDV_TXTSTYLE_LOWERCASE;/*|DDV_TXTSTYLE_COLOR;
	dobp->MASKlayout.rgb[0]=102;
	dobp->MASKlayout.rgb[1]=102;
	dobp->MASKlayout.rgb[2]=102;
*/
	dobp->ISOlayout.clr_ident=DDVCOL_ORANGE;
	dobp->ISOlayout.clr_simil=DDVCOL_BLUE;
	dobp->ISOlayout.clr_other=DDVCOL_BLACK;
}

/*******************************************************************************

  Function : DDV_BlastGetColorSchemaFour()
  
  Purpose : BLAST color sheme 4 = BLAST color schema 3 but with different colors
  
*******************************************************************************/
static void DDV_BlastGetColorSchemaFour(DDVOptionsBlockPtr dobp)
{
	memset(dobp,0,sizeof(DDVOptionsBlock));
	
	dobp->bUseLayout=TRUE;

	dobp->LayoutType=DDV_USE_MASKLAYOUT;
	dobp->LayoutType|=DDV_USE_UALAYOUT;
	dobp->LayoutType|=DDV_USE_ISOLAYOUT;

	dobp->UAlayout.style=DDV_TXTSTYLE_ITALIC;

	dobp->MASKlayout.style=DDV_TXTSTYLE_LOWERCASE|DDV_TXTSTYLE_COLOR;
	dobp->MASKlayout.rgb[0]=102;
	dobp->MASKlayout.rgb[1]=102;
	dobp->MASKlayout.rgb[2]=102;

	dobp->ISOlayout.clr_ident=DDVCOL_BLUE;
	dobp->ISOlayout.clr_simil=DDVCOL_BROWN;
	dobp->ISOlayout.clr_other=DDVCOL_RED;
}

/*******************************************************************************

  Function : DDV_BlastGetColorSchemaFive()
  
  Purpose : BLAST color sheme 5 = BLAST color schema 3 but with different colors
  
*******************************************************************************/
static void DDV_BlastGetColorSchemaFive(DDVOptionsBlockPtr dobp)
{
	memset(dobp,0,sizeof(DDVOptionsBlock));
	
	dobp->bUseLayout=TRUE;

	dobp->LayoutType=DDV_USE_MASKLAYOUT;
	dobp->LayoutType|=DDV_USE_UALAYOUT;
	dobp->LayoutType|=DDV_USE_ISOLAYOUT;

	dobp->UAlayout.style=DDV_TXTSTYLE_ITALIC;

	dobp->MASKlayout.style=DDV_TXTSTYLE_LOWERCASE|DDV_TXTSTYLE_COLOR;
	dobp->MASKlayout.rgb[0]=102;
	dobp->MASKlayout.rgb[1]=102;
	dobp->MASKlayout.rgb[2]=102;

	dobp->ISOlayout.clr_ident=DDVCOL_RED;
	dobp->ISOlayout.clr_simil=DDVCOL_BLUE;
	dobp->ISOlayout.clr_other=DDVCOL_BLACK;
}

/*******************************************************************************

  Function : DDV_BlastGetColorSchemaSix()
  
  Purpose : BLAST color sheme 6 = BLAST color schema 3 but with different colors
  
*******************************************************************************/
static void DDV_BlastGetColorSchemaSix(DDVOptionsBlockPtr dobp)
{
	memset(dobp,0,sizeof(DDVOptionsBlock));
	
	dobp->bUseLayout=TRUE;

	dobp->LayoutType=DDV_USE_MASKLAYOUT;
	dobp->LayoutType|=DDV_USE_UALAYOUT;
	dobp->LayoutType|=DDV_USE_ISOLAYOUT;

	dobp->UAlayout.style=DDV_TXTSTYLE_ITALIC;

	dobp->MASKlayout.style=DDV_TXTSTYLE_LOWERCASE;/*|DDV_TXTSTYLE_COLOR;
	dobp->MASKlayout.rgb[0]=102;
	dobp->MASKlayout.rgb[1]=102;
	dobp->MASKlayout.rgb[2]=102;
*/
	dobp->ISOlayout.clr_ident=DDVCOL_ORANGE;
	dobp->ISOlayout.clr_simil=DDVCOL_BLUE;
	dobp->ISOlayout.clr_other=DDVCOL_BLACK;

	dobp->ISOlayout.sty_ident=DDV_TXTSTYLE_BOLD;
}

/*******************************************************************************

  Function : DDV_InitDefSAPdispStyles_BLAST()
  
  Purpose : set the default styles for SeqAligns.
  
  Return value : -

*******************************************************************************/
static void DDV_InitDefSAPdispStyles_BLAST(DDV_Disp_OptPtr ddop)
{
	/*use colors*/
	ddop->bUseColors=FALSE;
	/*disc SAP styles*/
	ddop->ShowLeftTail=FALSE;
	ddop->ShowRightTail=FALSE;
	ddop->DispDiscStyle=MSA_TXT_STYLE_2;
	ddop->SpacerSize=SPACER_TXT_BLANK;
        ddop->DiscJustification=DISP_JUST_LEFT;

	/*ruler style*/
	ddop->RulerStyle=RulerStyle_Continue_Start;
}

/*******************************************************************************

  Function : DDV_DisplayNewBLAST()
  
  Purpose : build a default SeqAlign display for BLAST.
  
  Parameters : 	sap; SeqAlign
				disp_format; display formats (see pgppop.h)
				disp_data; for future implementation
				fp; to send the output

  NOTE : this function uses the alignmgr.[ch] instead of blocks.[ch]
  
  Return value : FALSE if failed 

*******************************************************************************/
static Boolean DDV_DisplayNewBLAST(SeqAlignPtr sap, ValNodePtr mask,
		Int4Ptr PNTR matrix,Uint4 disp_format,Pointer disp_data,FILE *fp)
{
  MsaParaGPopList    mppl;/*contains the data for the display*/
  Int2               LineSize;
  DDVOptionsBlockPtr dobp;
  ByteStorePtr       PNTR byteSpp;
  DDV_ColorGlobal    *layout;
  DDV_Disp_Opt       ddo;
  Boolean            bUseLayout;
  Uint1              what;
  
/*  DenseSegPtr        dsp;
  SeqIdPtr           sip;
  BioseqPtr          bsp;
  Int4               i;
  Int4               lastalign;
  
  dsp = sap->segs;
  sip = dsp->ids->next;
  bsp = BioseqLockById(sip);
  lastalign = dsp->lens[dsp->numseg-1]+dsp->starts[2*dsp->numseg-1]-1;  
  if (lastalign >= bsp->length) {
    WRPSBHtmlError("Bummer!");
  }
*/  

	MemSet(&mppl,0,sizeof(MsaParaGPopList));
	layout=NULL;

	/*get the optional data*/
	if (disp_data){
		dobp=(DDVOptionsBlockPtr)disp_data;
		LineSize=dobp->LineSize;
		byteSpp=dobp->byteSpp;
		bUseLayout=dobp->bUseLayout;
		what=dobp->LayoutType;
	}
	else{
		LineSize=ParaG_Size;
		byteSpp=NULL;
		bUseLayout=FALSE;
	}

	DDV_InitDefSAPdispStyles_BLAST(&ddo);
	
	/*build the indexed SeqAlign and the ParaG structure*/
	DDV_CreateDisplayFromIndex(sap, &mppl, LineSize,&ddo);
	
	/*create the layout*/
	if (bUseLayout){
		layout = DDV_CreateColorGlobal(TRUE, (void *)sap);
		if (layout){
			if (dobp->LayoutType&DDV_USE_STDCLR){
				DDV_InitColour_When_Start(sap,&mppl,&layout, FALSE);
			}
			if (dobp->LayoutType&DDV_USE_UALAYOUT || dobp->LayoutType&DDV_USE_NOMLAYOUT){
				DDV_LayoutUAregion(sap,&mppl,&layout,
					dobp->normlayout.rgb,dobp->normlayout.style,
					dobp->UAlayout.rgb,dobp->UAlayout.style);
			}
			if (dobp->LayoutType&DDV_USE_ISOLAYOUT){
				DDV_LayoutISOColors(layout,mppl.TableHead,mppl.nBsp,
					0,TRUE,matrix,dobp->ISOlayout.clr_ident,
					dobp->ISOlayout.clr_simil,dobp->ISOlayout.clr_other,
					dobp->ISOlayout.sty_ident,
					dobp->ISOlayout.sty_simil,dobp->ISOlayout.sty_other);
			}
			if (dobp->LayoutType&DDV_USE_GAPLAYOUT){
				DDV_LayoutGap(layout,mppl.TableHead[0],mppl.TableHead[1],1,
					dobp->GAPlayout.style,dobp->GAPlayout.rgb);
				DDV_LayoutGap(layout,mppl.TableHead[1],mppl.TableHead[0],0,
					dobp->GAPlayout.style,dobp->GAPlayout.rgb);
			}
			if (dobp->LayoutType&DDV_USE_MASKLAYOUT){
				DDV_LayoutMaskRegions(layout,mppl.TableHead[0],mask,0,
					dobp->MASKlayout.style,dobp->MASKlayout.rgb);
			}
		}
	}
	
	/*do the display */
	DDV_AffichageParaG(&mppl,0,0,mppl.LengthAli-1,mppl.LengthAli,0,disp_format,
		LineSize,fp,byteSpp,matrix,layout,mask);

	/*done... delete data*/
	DDV_DeleteDisplayList(&mppl);
	if (layout) DDV_DeleteColorGlobal(layout);

	
	return(TRUE);
}



/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*  function to display a BLAST output.                                      */
/*  Parameters :   sap; seqalign                                             */
/*                 mask; list of masked regions in the query                 */
/*                 fp; output file;                                          */
/*                 is_na; TRUE means nuc sequence                            */
/*                 tx_option; some display options                           */
/*                 ColorSchema; one of the available color schemes           */
/*  Return value : FALSE if failure                                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Boolean WRPSBDisplayBlastPairList(AlignmentAbstractPtr aap,
                                             ValNodePtr mask, FILE *fp,
                                             Boolean is_na, Uint4 tx_option,
                                             Uint1 ColorSchema,
                                             RPSBlastOptionsPtr rpsbop)
{
  DDVOptionsBlock dob;
  SeqAlignPtr     sap4;
  SeqIdPtr        new_id = NULL, old_id = NULL;    
  Int4Ptr PNTR    matrix;
  Uint4           option,i;
  Boolean         bRet, follower= FALSE;
  Char            dupstr[CDD_MAX_DESCR];
  CharPtr         part1;
  CddDescrPtr     description;
  DenseSegPtr     dsp;
  Int4            subject_length, nsegments;
  Nlm_FloatHi     aligned_fraction;
  BioseqPtr       bsp;
  Char            cTmp1[16], cTmp2[16], cTmp3[16];
  SeqPortPtr      spp;
  Char            aln[CDD_MAX_DESCR];
  CharPtr         buf;

  if (!aap || !fp) return(FALSE);
    
  if (is_na == FALSE){
      matrix=load_default_matrix();
      if (!matrix) return(FALSE);
  } else { matrix=NULL; }
    
  option =VIEW_FULLSEQ;
  if (ColorSchema==DDV_BLAST_COLOR0)    
    option|=DISP_FULL_TXT;
  else
  option|=DISP_FULL_HTML;
  option|=DISP_BSP_COORD;
  option|=SEQNAME_BLAST_STD;
  option|=DISP_NOLINKONNAME;
  option|=DISP_BLAST_STD;
  option|=DISPE_COLOR;
/*---------------------------------------------------------------------------*/
/* get the color scheme layout - note that the following functions will set  */
/* to 0 the entire dob structure, before filling in its content with the     */
/* layout information. All subsequent init. of dob should be done after the  */
/* following switch() statement                                              */
/*---------------------------------------------------------------------------*/
  switch(ColorSchema){
    case DDV_BLAST_COLOR0:
      option|=DISP_BLAST_MIDLINE;
      DDV_BlastGetColorSchemaZero(&dob);
      break;
    case DDV_BLAST_COLOR1:
      option|=DISP_BLAST_MIDLINE;
      DDV_BlastGetColorSchemaOne(&dob);
      break;
    case DDV_BLAST_COLOR2:
      option|=DISP_BLAST_MIDLINE;
      DDV_BlastGetColorSchemaTwo(&dob);
      break;
    case DDV_BLAST_COLOR3:
      DDV_BlastGetColorSchemaThree(&dob);
      break;
    case DDV_BLAST_COLOR4:
      DDV_BlastGetColorSchemaFour(&dob);
      break;
    case DDV_BLAST_COLOR5:
      DDV_BlastGetColorSchemaFive(&dob);
      break;
    case DDV_BLAST_COLOR6:
      DDV_BlastGetColorSchemaSix(&dob);
      break;
  }
  dob.LineSize=(Int2)60;
  dob.matrix=matrix;
  bRet=TRUE;
  buf = MemNew((rpsbop->query_bsp->length+1)*sizeof(Char));
  spp = FastaSeqPort((BioseqPtr)rpsbop->query_bsp,FALSE,FALSE,Seq_code_ncbieaa);
  FastaSeqLine(spp,buf,rpsbop->query_bsp->length,FALSE);
  while (aap) {
    sap4 = aap->salp;
/*---------------------------------------------------------------------------*/
/* build the Index                                                           */
/*---------------------------------------------------------------------------*/
    if (sap4->segtype == SAS_DISC){
      if (!sap4 || !AlnMgrIndexSingleSeqAlign(sap4)){
        bRet=FALSE;
        break;
      }
    } else {
      if (!sap4 || !AlnMgrIndexSingleChildSeqAlign(sap4)){
        bRet=FALSE;
        break;
      }
    }

/*---------------------------------------------------------------------------*/
/* Attempt to print score for the alignment                                  */
/*---------------------------------------------------------------------------*/
    if (option&DISP_FULL_HTML) fprintf(fp,"<HR WIDTH=\"400\">");
    new_id = TxGetSubjectIdFromSeqAlign(sap4);
    fprintf(fp, "<a name=\"%s\">",aap->name);
    strncpy(dupstr,aap->long_defline,CDD_MAX_DESCR);
    part1 = strtok(dupstr," ");
    fprintf(fp,"<a href=\"%s\" TARGET=\"_new\">%s</a>",aap->cHtmlLink,part1);
    description = aap->description;
    while (description) {
      if (description->choice == CddDescr_comment) {
        fprintf(fp,", %s\n", description->data.ptrvalue);
	break;
      }
      description = description->next;
    }
/*---------------------------------------------------------------------------*/
/* print form for displaying a multiple alignment                            */
/*---------------------------------------------------------------------------*/
   if (!aap->bIsProfile) {
      dsp = sap4->segs;
      nsegments = 0;
      for (i=0;i<dsp->numseg;i++) {
        if (dsp->starts[i*2] != -1 && dsp->starts[i*2 + 1] != -1) nsegments++;
      }
      aln[0]='\0';
      sprintf(aln,"%d",nsegments);
      for (i=0;i<dsp->numseg;i++) {
        if (dsp->starts[i*2] != -1 && dsp->starts[i*2 + 1] != -1) {
          cTmp2[0]='\0';
          sprintf(cTmp2,",%d,%d,%d",dsp->starts[i*2+1],dsp->starts[i*2],dsp->lens[i]);
          strcat(aln,cTmp2);
        }
      }
      aln[strlen(aln)]='\0';
      strcpy(dupstr,aap->cHtmlLink);
      part1 = strtok(dupstr,"?");
      fprintf(fp,"<FORM ACTION=\"%s\" METHOD=\"POST\" TARGET=\"_new\">\n",part1);
      fprintf(fp,"<INPUT TYPE=\"HIDDEN\" NAME=\"uid\" VALUE=\"%s\">\n",aap->cCDDid);
      fprintf(fp,"<INPUT TYPE=\"HIDDEN\" NAME=\"query\" VALUE=\"%s\">\n",buf);
      fprintf(fp,"<INPUT TYPE=\"HIDDEN\" NAME=\"aln\" VALUE=\"%s\">\n",aln);
      fprintf(fp,"<INPUT TYPE=\"SUBMIT\" NAME=\"submit\" VALUE=\"Add\"> query to multiple alignment, display \n");
      fprintf(fp,"<SELECT NAME=\"maxaln\">\n");
      fprintf(fp,"<OPTION SELECTED VALUE=\"10\">up to 10\n");
      fprintf(fp,"<OPTION VALUE=\"25\">up to 25\n");
      fprintf(fp,"<OPTION VALUE=\"50\">up to 50\n");
      fprintf(fp,"<OPTION VALUE=\"100\">up to 100\n");
      fprintf(fp,"<OPTION VALUE=\"0\">all\n");
      fprintf(fp,"</SELECT> aligned sequences\n");
      fprintf(fp,"</FORM>\n");
    }
    if (option&DISP_FULL_HTML) fprintf(fp,"<pre>\n");
    bsp = BioseqLockById(new_id);
    subject_length = bsp->length;
    aligned_fraction = 1.0 - aap->nmissg - aap->cmissg;
    fprintf(fp,"             CD-Length = %d residues, %5.1f%% aligned\n",subject_length,
            100.0 * aligned_fraction);
    print_score_sonly(aap->defline->bit_score,cTmp1);
    print_score_eonly(aap->defline->evalue,cTmp3);
    fprintf(fp,"             Score = %s bits (%d), Expect = %s\n",cTmp1, aap->defline->score, cTmp3);
    if (bsp!=NULL) BioseqUnlock(bsp);
    if (option&DISP_FULL_HTML) fprintf(fp,"</pre>\n");
/*---------------------------------------------------------------------------*/
/*display a SeqAlign                                                         */
/*---------------------------------------------------------------------------*/
    if (!DDV_DisplayNewBLAST(sap4, mask, matrix, option, (Pointer) &dob, fp)){
      bRet=FALSE;
      break;
    }
    aap = aap->next;
  }
  if (matrix){
    for(i = 0; i<TX_MATRIX_SIZE; ++i) MemFree(matrix[i]);
    MemFree(matrix);
  } 
  return(bRet);
}

/*******************************************************************************

  Function : WRPSB_BlastGetColorSchemaOne()
  
  Purpose : BLAST color sheme 1 :
              - masked regions in italic,
                          - everything else in upper case.
  
*******************************************************************************/
static void WRPSB_BlastGetColorSchemaOne(DDVOptionsBlockPtr dobp)
{
        memset(dobp,0,sizeof(DDVOptionsBlock));
        dobp->bUseLayout=TRUE;
        dobp->LayoutType=DDV_USE_MASKLAYOUT;
        dobp->MASKlayout.style=DDV_TXTSTYLE_ITALIC;
}

/*******************************************************************************

  Function : WRPSB_BlastGetColorSchemaTwo()
  
  Purpose : BLAST color sheme 2 :
              - masked regions in italic, gray letters,
                          - UnAligned regions in lowercase,
                          - everything else in upper case.
  
*******************************************************************************/
static void WRPSB_BlastGetColorSchemaTwo(DDVOptionsBlockPtr dobp)
{
        memset(dobp,0,sizeof(DDVOptionsBlock));
        
        dobp->bUseLayout=TRUE;

        dobp->LayoutType=DDV_USE_MASKLAYOUT|DDV_USE_NOMLAYOUT;
        dobp->LayoutType|=DDV_USE_UALAYOUT;

        dobp->UAlayout.style=DDV_TXTSTYLE_LOWERCASE;
        dobp->normlayout.style=DDV_TXTSTYLE_BOLD;

        dobp->MASKlayout.style=DDV_TXTSTYLE_ITALIC|DDV_TXTSTYLE_COLOR;
        dobp->MASKlayout.rgb[0]=102;
        dobp->MASKlayout.rgb[1]=102;
        dobp->MASKlayout.rgb[2]=102;
}
/*******************************************************************************

  Function : WRPSB_BlastGetColorSchemaThree()
  
  Purpose : BLAST color sheme 3 :
              - masked regions in italic,
                          - UnAligned regions in lowercase,
                          - Identity/Similarity colors
                          - everything else in upper case.
  
*******************************************************************************/
static void WRPSB_BlastGetColorSchemaThree(DDVOptionsBlockPtr dobp)
{
        memset(dobp,0,sizeof(DDVOptionsBlock));
        dobp->bUseLayout=TRUE;
        dobp->LayoutType=DDV_USE_MASKLAYOUT;
        dobp->LayoutType|=DDV_USE_UALAYOUT;
        dobp->LayoutType|=DDV_USE_ISOLAYOUT;
        dobp->UAlayout.style=DDV_TXTSTYLE_LOWERCASE;
        dobp->MASKlayout.style=DDV_TXTSTYLE_ITALIC;
        dobp->ISOlayout.clr_ident=DDVCOL_RED;
        dobp->ISOlayout.clr_simil=DDVCOL_BLUE;
        dobp->ISOlayout.clr_other=DDVCOL_BLACK;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*  function to display a BLAST output.                                      */
/*  Parameters :   sap; seqalign                                             */
/*                 mask; list of masked regions in the query                 */
/*                 fp; output file;                                          */
/*                 is_na; TRUE means nuc sequence                            */
/*                 tx_option; some display options                           */
/*                 ColorSchema; one of the available color schemes           */
/*  Return value : FALSE if failure                                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Boolean WRPSBCl3DisplayBlastPairList(AlignmentAbstractPtr aap,
                                     ValNodePtr mask, FILE *fp,
                                     Boolean is_na, Uint4 tx_option,
                                     Uint1 ColorSchema, BioseqPtr query_bsp,
				     CharPtr dbversion, CharPtr urlcgi)
{
  DDVOptionsBlock dob;
  SeqAlignPtr     sap4;
  SeqIdPtr        new_id = NULL, old_id = NULL;    
  SeqEntryPtr     sep;
  Int4Ptr PNTR    matrix;
  Uint4           option,i;
  Boolean         bRet, follower= FALSE;
  Char            dupstr[CDD_MAX_DESCR];
  CharPtr         part1;
  CddDescrPtr     description;
  DenseSegPtr     dsp;
  Int4            subject_length, nsegments, maxsubjaln;
  Nlm_FloatHi     aligned_fraction;
  BioseqPtr       bsp;
  Char            cTmp1[16], cTmp2[16], cTmp3[16];
  SeqPortPtr      spp;
  Char            aln[CDD_MAX_DESCR];
  CharPtr         buf;
  FILE            *sfp;

  if (!aap || !fp) return(FALSE);
    
  if (is_na == FALSE){
      matrix=load_default_matrix();
      if (!matrix) return(FALSE);
  } else { matrix=NULL; }
    
  option =VIEW_FULLSEQ;
  if (ColorSchema==DDV_BLAST_COLOR0)    
    option|=DISP_FULL_TXT;
  else
  option|=DISP_FULL_HTML;
  option|=DISP_BSP_COORD;
  option|=SEQNAME_BLAST_STD;
  option|=DISP_NOLINKONNAME;
  option|=DISP_BLAST_STD;
  option|=DISPE_COLOR;
/*---------------------------------------------------------------------------*/
/* get the color scheme layout - note that the following functions will set  */
/* to 0 the entire dob structure, before filling in its content with the     */
/* layout information. All subsequent init. of dob should be done after the  */
/* following switch() statement                                              */
/*---------------------------------------------------------------------------*/
  switch(ColorSchema){
    case DDV_BLAST_COLOR0:
      option|=DISP_BLAST_MIDLINE;
      DDV_BlastGetColorSchemaZero(&dob);
      break;
    case DDV_BLAST_COLOR1:
      option|=DISP_BLAST_MIDLINE;
      WRPSB_BlastGetColorSchemaOne(&dob);
      break;
    case DDV_BLAST_COLOR2:
      option|=DISP_BLAST_MIDLINE;
      WRPSB_BlastGetColorSchemaTwo(&dob);
      break;
    case DDV_BLAST_COLOR3:
      WRPSB_BlastGetColorSchemaThree(&dob);
      break;
    case DDV_BLAST_COLOR4:
      DDV_BlastGetColorSchemaFour(&dob);
      break;
    case DDV_BLAST_COLOR5:
      DDV_BlastGetColorSchemaFive(&dob);
      break;
    case DDV_BLAST_COLOR6:
      DDV_BlastGetColorSchemaSix(&dob);
      break;
  }
  dob.LineSize=(Int2)60;
  dob.matrix=matrix;
  bRet=TRUE;
  buf = MemNew((query_bsp->length+1)*sizeof(Char));
  spp = FastaSeqPort((BioseqPtr)query_bsp,FALSE,FALSE,Seq_code_ncbieaa);
  FastaSeqLine(spp,buf,query_bsp->length,FALSE);
  while (aap) {
    sap4 = aap->salp; sap4->next=NULL;
/*---------------------------------------------------------------------------*/
/* build the Index                                                           */
/*---------------------------------------------------------------------------*/
    if (sap4->segtype == SAS_DISC){
      if (!sap4 || !AlnMgrIndexSingleSeqAlign(sap4)){
        bRet=FALSE;
        break;
      }
    } else {
      if (!sap4 || !AlnMgrIndexSingleChildSeqAlign(sap4)){
        bRet=FALSE;
        break;
      }
    }

/*---------------------------------------------------------------------------*/
/* Attempt to print score for the alignment                                  */
/*---------------------------------------------------------------------------*/
    if (option&DISP_FULL_HTML) fprintf(fp,"<HR WIDTH=\"400\">");
    new_id = TxGetSubjectIdFromSeqAlign(sap4);
    fprintf(fp, "<a name=\"%s\">",aap->name);
    strncpy(dupstr,aap->long_defline,CDD_MAX_DESCR);
    part1 = strtok(dupstr," ");
    if (aap->bHasStructure) {
      fprintf(fp,"<TABLE><TR><TD VALIGN=TOP><IMG SRC=\"%spinkb.gif\"></TD><TD>\n",urlcgi);
    }

    fprintf(fp,"<a href=\"%s\" TARGET=\"_new\">%s</a>",aap->cHtmlLink,part1);
    description = aap->description;
    while (description) {
      if (description->choice == CddDescr_comment) {
        fprintf(fp,", %s\n", description->data.ptrvalue);
	break;
      }
      description = description->next;
    }
    if (aap->bHasStructure) {
      fprintf(fp,"</TD></TR></TABLE>\n");
    }

/*---------------------------------------------------------------------------*/
/* print form for displaying a multiple alignment                            */
/*---------------------------------------------------------------------------*/
    dsp = sap4->segs;
    nsegments = 0;
    for (i=0;i<dsp->numseg;i++) {
      if (dsp->starts[i*2] != -1 && dsp->starts[i*2 + 1] != -1) nsegments++;
    }
    aln[0]='\0'; maxsubjaln = 0;
    sprintf(aln,"%d",nsegments);
    for (i=0;i<dsp->numseg;i++) {
      if (dsp->starts[i*2] != -1 && dsp->starts[i*2 + 1] != -1) {
        cTmp2[0]='\0';
        sprintf(cTmp2,",%d,%d,%d",dsp->starts[i*2+1],dsp->starts[i*2],dsp->lens[i]);
	maxsubjaln = dsp->starts[i*2+1]+dsp->lens[i]-1;
        strcat(aln,cTmp2);
      }
    }
    aln[strlen(aln)]='\0';
    strcpy(dupstr,aap->cHtmlLink);
    part1 = strtok(dupstr,"?");
    if (!aap->bIsProfile) {
      fprintf(fp,"<FORM ACTION=\"%s\" METHOD=\"POST\" TARGET=\"_new\">\n",part1);
      fprintf(fp,"<INPUT TYPE=\"HIDDEN\" NAME=\"uid\" VALUE=\"%s\">\n",aap->cCDDid);
      fprintf(fp,"<INPUT TYPE=\"HIDDEN\" NAME=\"version\" VALUE=\"%s\">\n",dbversion);
      fprintf(fp,"<INPUT TYPE=\"HIDDEN\" NAME=\"query\" VALUE=\"%s\">\n",buf);
      fprintf(fp,"<INPUT TYPE=\"HIDDEN\" NAME=\"aln\" VALUE=\"%s\">\n",aln);
      fprintf(fp,"<INPUT TYPE=\"HIDDEN\" NAME=\"ascbin\" VALUE=2>\n");
      fprintf(fp,"<INPUT TYPE=\"SUBMIT\" NAME=\"submit\" VALUE=\"Add\"> query to multiple alignment, display \n");
      fprintf(fp,"<SELECT NAME=\"maxaln\">\n");
      fprintf(fp,"<OPTION VALUE=\"5\">up to 5\n");
      fprintf(fp,"<OPTION SELECTED VALUE=\"10\">up to 10\n");
      fprintf(fp,"<OPTION VALUE=\"25\">up to 25\n");
      fprintf(fp,"<OPTION VALUE=\"50\">up to 50\n");
      fprintf(fp,"<OPTION VALUE=\"100\">up to 100\n");
      fprintf(fp,"<OPTION VALUE=\"-1\">all\n");
      fprintf(fp,"</SELECT>&nbsp; sequences\n");
      fprintf(fp, "<SELECT NAME=\"seltype\">\n");
      fprintf(fp, "<OPTION VALUE=\"1\">from the top of the CD alignment\n"); 
      fprintf(fp, "<OPTION VALUE=\"2\">from the most diverse subset\n"); 
      fprintf(fp, "<OPTION SELECTED VALUE=\"3\">most similar to the query \n"); 
      fprintf(fp, "</SELECT>\n");

      fprintf(fp,"</FORM>\n");
    }
    if (option&DISP_FULL_HTML) fprintf(fp,"<pre>\n");
/*---------------------------------------------------------------------------*/
/* need to explicitly get the subject sequence from a FASTA file (versions..)*/
/*---------------------------------------------------------------------------*/
    sfp = FileOpen(aap->cSeqFile,"r");
    if (!sfp) WRPSBHtmlError("Could not find subject sequence!");
    sep = FastaToSeqEntry(sfp,FALSE);
    FileClose(sfp);
    if (!sep) WRPSBHtmlError("Could not read subject sequence!");
    bsp = (BioseqPtr) sep->data.ptrvalue;

    /* bsp = BioseqLockById(new_id); */
    subject_length = bsp->length;
    if (maxsubjaln >= subject_length) {
      WRPSBHtmlError("Error in RPS-Blast alignment!");
    }
    aligned_fraction = 1.0 - aap->nmissg - aap->cmissg;
    if (aap->nmissg >= 0.2 || aap->cmissg >= 0.2) {
      fprintf(fp,"             CD-Length = %d residues, <FONT COLOR=#CC0000>only %5.1f%% aligned</FONT>\n",subject_length,
              100.0 * aligned_fraction);
    } else {
      fprintf(fp,"             CD-Length = %d residues, %5.1f%% aligned\n",subject_length,
              100.0 * aligned_fraction);
    }
    print_score_sonly(aap->defline->bit_score,cTmp1);
    print_score_eonly(aap->defline->evalue,cTmp3);
    fprintf(fp,"             Score = %s bits (%d), Expect = %s\n",cTmp1, aap->defline->score, cTmp3);
    /* if (bsp!=NULL) BioseqUnlock(bsp); */
    if (option&DISP_FULL_HTML) fprintf(fp,"</pre>\n");
/*---------------------------------------------------------------------------*/
/*display a SeqAlign                                                         */
/*---------------------------------------------------------------------------*/
    if (!DDV_DisplayNewBLAST(sap4, mask, matrix, option, (Pointer) &dob, fp)){
      bRet=FALSE;
      break;
    }
    SeqEntryFree(sep);
    aap = aap->next;
  }
  if (matrix){
    for(i = 0; i<TX_MATRIX_SIZE; ++i) MemFree(matrix[i]);
    MemFree(matrix);
  } 
  return(bRet);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* return data as a dynamic pointer                                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
gdIOCtx* gdNewBorkCtx (void) {
  borkIOCtx 	*ctx;

  ctx = (borkIOCtx*) malloc(sizeof(borkIOCtx));
  if (ctx == NULL) {
    return NULL;
  }
  ctx->buffer = (char*) malloc(1024);
  ctx->len = 0;
  ctx->bufsz = 1024;
  ctx->ctx.putC = borkPutchar;
  ctx->ctx.putBuf = borkPutbuf;
  ctx->ctx.free = freeBorkCtx;
  return (gdIOCtx*)ctx;
}

static void freeBorkCtx(gdIOCtx *ctx)
{
  free(ctx);
}

static Int4 borkPutbuf( gdIOCtx* ctx, const void *buf, Int4 size )
{
  borkIOCtx          *bctx;
  bctx = (borkIOCtx*) ctx;
  if (bctx->len + size >= bctx->bufsz) {
      bctx->buffer = (Char*) realloc(bctx->buffer, (bctx->bufsz+size)*2);
      bctx->bufsz += size;
      bctx->bufsz *= 2;
  }
  memcpy(bctx->buffer + bctx->len, buf, size);
  bctx->len += size;
  return size;
}

static void borkPutchar( gdIOCtx* ctx, int a )
{
    char	*onechar = (char*) malloc(2);
    onechar[0] = a; 
    onechar[1] = '\0'; 
    borkPutbuf(ctx, onechar, 1);
}



