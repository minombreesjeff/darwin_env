/* $Id: wrpsbtool.c,v 1.23 2004/04/13 19:42:09 bauer Exp $
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
* $Revision: 1.23 $
*
* File Description:
*         tools for WWW-RPS BLAST 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: wrpsbtool.c,v $
* Revision 1.23  2004/04/13 19:42:09  bauer
* fix URL for Cn3D launching via cddsrv.cgi
*
* Revision 1.22  2004/02/17 18:04:50  bauer
* prepared for alignment resorting
*
* Revision 1.21  2003/11/19 14:34:31  bauer
* changes to support SeqAnnot export
*
* Revision 1.20  2003/10/07 21:16:06  bauer
* support generation of Sequence Annotation from CD-Search results
*
* Revision 1.19  2003/04/25 14:42:12  bauer
* changes to BLAST database interface
*
* Revision 1.18  2002/12/24 18:22:14  bauer
* changes for v1.60
*
* Revision 1.17  2002/10/22 14:55:20  bauer
* fixed URL for bottom help link
*
* Revision 1.16  2002/08/06 02:39:10  bauer
* changes to accomodate COGs
*
* Revision 1.15  2002/06/25 21:23:28  bauer
* revised Sequence retrieval interface
*
* Revision 1.14  2002/06/12 15:04:53  bauer
* 6/11/02 update
*
* Revision 1.13  2002/05/06 17:18:05  bauer
* switched to graphics on the fly
*
* Revision 1.12  2002/03/07 19:12:15  bauer
* major revisions to cgi-bins and the CD-dumper
*
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
*
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
#include "cddutil.h"
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
/* Header and Footer for the Pages                                           */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void WRPSBSearchHead(CharPtr title, CharPtr banner, Boolean bAnnotOnly,
                     Boolean bNoWrap)
{
  CharPtr cTitle;
  CharPtr cBanner;

  if (bAnnotOnly) {
    printf("Content-type: text/html\n\n");
    if (!bNoWrap) {
      printf("<html>\n");
      printf("  <body>\n");
    }
  } else {
    cTitle = MemNew(128 * sizeof (Char));
    if (!title) {
      StringCpy(cTitle,"NCBI CD-Search");
    } else (StringCpy(cTitle, title));
    cBanner = MemNew(128 * sizeof (Char));
    if (!banner) {
      StringCpy(cBanner,"NCBI Conserved Domain Search");
    } else (StringCpy(cBanner, banner));
    printf("Content-type: text/html\n\n");
    printf("<html>\n");
    printf("  <head>\n");
    printf("    <META name=\"keywords\" content=\"CDD, protein domains, domains, NCBI, National Center for Biotechnology Information, National Library of Medicine, NLM, rps-blast, rps\">\n");
    printf("    <title>%s</title>\n",cTitle);
    printf("    <link rel=\"stylesheet\" href=\"http://www.ncbi.nlm.nih.gov/corehtml/ncbi_test.css\">\n");
    printf("  </head>\n");
    printf("  <body bgcolor=\"#FFFFFF\" text=\"#000000\" link=\"#0033CC\" vlink=\"#CC3300\">\n");
    printf("<!--  the header   -->\n");
    printf("<!--start of first table -->\n");
    printf("    <table width=\"100%%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">\n");
    printf("      <tr>                  \n");
    printf("<!-- logo -->\n");
    printf("        <td>\n");
    printf("          <table width=\"100%%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">\n");
    printf("            <tr>\n");
    printf("              <td align=\"left\">\n");
    printf("                <A HREF=\"http://www.ncbi.nlm.nih.gov\">\n");
    printf("                  <img src=\"/Structure/invleft.GIF\" width=\"130\" height=\"45\" border=\"0\">\n");
    printf("                </a>\n");
    printf("              </td>\n");
    printf("              <td align=\"left\">\n");
    printf("                <span class=\"H2\">%s</span>\n",cBanner);
    printf("              </td>\n");
    printf("            </tr>\n");
    printf("          </table>\n");
    printf("        </td>\n");
    printf("      </tr>\n");
    printf("      <tr>\n");
    printf("<!--site map-->\n");
    printf("        <td>\n");
    printf("          <table class=\"TEXT\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\" bgcolor=\"#000000\" width=\"100%%\">\n");
    printf("            <tr class=\"TEXT\" align=\"CENTER\">\n");
    printf("              <td width=\"12.5%%\"><a href=\"wrpsb.cgi\" class=\"GUTTER3\"><FONT COLOR=\"#FFFF00\"><b>New Search</b></FONT></a></td>\n");
    printf("              <td width=\"12.5%%\"><a href=\"http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?db=PubMed\" class=\"GUTTER3\"><FONT COLOR=\"#FFFFFF\">PubMed</FONT></a></td>\n");
    printf("              <td width=\"12.5%%\"><a href=\"http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?db=Nucleotide\" class=\"GUTTER3\"><FONT COLOR=\"#FFFFFF\">Nucleotide</FONT></a></td>\n");
    printf("              <td width=\"12.5%%\"><a href=\"http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?db=Protein\" class=\"GUTTER3\"><FONT COLOR=\"#FFFFFF\">Protein</FONT></a></td>\n");
    printf("              <td width=\"12.5%%\"><a href=\"http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?db=Structure\" class=\"GUTTER3\"><FONT COLOR=\"#FFFFFF\">Structure</FONT></a></td>\n");
    printf("              <td width=\"12.5%%\"><a href=\"http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?db=cdd\" class=\"GUTTER3\"><FONT COLOR=\"#FFFF00\"><b>CDD</b></FONT></a></td>\n");
    printf("              <td width=\"12.5%%\"><a href=\"http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?db=Taxonomy\" class=\"GUTTER3\"><FONT COLOR=\"#FFFFFF\">Taxonomy</FONT></a></td>\n");
    printf("              <td width=\"12.5%%\"><a href=\"cdd_help.shtml\" class=\"GUTTER3\"><FONT COLOR=\"#FFFF00\"><b>Help?</b></FONT></a></td>\n");
    printf("            </tr>\n");
    printf("          </table>\n");
    printf("        </td>\n");
    printf("      </tr>\n");
    printf("    </table>\n");
    printf("<!--start of 2nd table -->              \n");
    printf("    <table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" width=\"100%%\">\n");
    printf("      <tr valign=\"TOP\"> \n");
    printf("<!-- right content column  --> \n");
    printf("         <td width=\"100%%\" bgcolor=\"#FFFFFF\">       \n");
    printf("<!-- -------- view --------- -->\n");
    MemFree(cBanner);
    MemFree(cTitle);
  }
}

/*---------------------------------------------------------------------------*/
/* lower part of the page - the "footer"                                     */
/*---------------------------------------------------------------------------*/
void WRPSBSearchFoot(Boolean bAnnotOnly, Boolean bNoWrap)
{
  if (!bAnnotOnly) {
    printf("<!-- -------- end of view --------- -->\n");
    printf("         </td>\n");
    printf("<!-- end of right content column  -->                         \n");
    printf("      </tr>\n");
    printf("      <tr valign=\"TOP\"> \n");
    printf("        <td width=\"100%%\"> \n");
    printf("          <br>\n");
    printf("          <div align=\"center\" class=\"medium1\">  \n");
    printf("            <p><b><a href=\"cdd_help.shtml\">Help</a></b> | <a href=\"http://www.ncbi.nlm.nih.gov/About/disclaimer.html\">Disclaimer</a> | <a href=\"mailto:info@ncbi.nlm.nih.gov\">Write to the Help Desk</a><BR><a href=\"http://www.ncbi.nlm.nih.gov\">NCBI</a> | <a href=\"http://www.nlm.nih.gov\">NLM</a> | <a href=\"http://www.nih.gov\">NIH</a> </p>\n");
    printf("            <p>&nbsp;</p>\n");
    printf("          </div>\n");
    printf("        </td>\n");
    printf("      </tr>\n");
    printf("    </table>\n");
    printf("<!--  end of content  -->\n");
  }
  if (!bNoWrap) {
    printf("  </body>\n");
    printf("</html>\n");
  }
  fflush(NULL);
  exit(0);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* output simple error message as HTML                                       */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void WRPSBHtmlError(CharPtr cErrTxt, Boolean bAnnotOnly) 
{
  AsnIoPtr      aip;
  SeqAnnotPtr   sap;
  AnnotDescPtr  adp;

  if (bAnnotOnly) {
    WRPSBSearchHead(NULL,NULL,TRUE,TRUE);
    sap = SeqAnnotNew();
    sap->type = 1;
    sap->data = NULL;
    adp = (AnnotDescPtr) ValNodeNew(NULL);
    adp->choice = Annot_descr_name;
    adp->data.ptrvalue = StringSave(cErrTxt);
    sap->desc = adp;
    strcpy(OutputName,GetTempName("wrpsbcl3"));
    aip = AsnIoOpen(OutputName, "w");
    SeqAnnotAsnWrite((SeqAnnotPtr) sap, aip, NULL);
    AsnIoClose(aip);
    PrintFile(OutputName);
    RemoveTempFiles();   
    exit(1);  
  }
  WRPSBSearchHead("CD-Search Error","CD-Search Error Message",FALSE,FALSE);
  printf("<BR><h3>%s</h3>\n",cErrTxt);
  WRPSBSearchFoot(FALSE, FALSE);
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
CharPtr WRPSBFixCn3DStr(CharPtr instr)
{
  CharPtr outstr;
  CharPtr tempcopy;
  
  tempcopy = StringSave(instr);
  outstr = MemNew(PATH_MAX * sizeof(Char));
  StrCpy(outstr, strtok(tempcopy,"?"));
  StrCat(outstr, "/cddsrv.cn3?");
  StrCat(outstr, strtok(NULL,"?"));
  MemFree(tempcopy);
  return (outstr);
}

void WRPSBPrintDefLinesFromSeqAlign(AlignmentAbstractPtr aap, FILE *table,
                                    Boolean bAnyPdb, BioseqPtr query_bsp,
				    CharPtr urlcgi, Int4 querygi)
{
  Char                 dupstr[PATH_MAX];
  CharPtr              cn3dstr;
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
  

  if (!querygi) {
    buf = MemNew((query_bsp->length+1)*sizeof(Char));
    spp = FastaSeqPort((BioseqPtr)query_bsp,FALSE,FALSE,Seq_code_ncbieaa);
    FastaSeqLine(spp,buf,query_bsp->length,FALSE);
  }

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
        cn3dstr = WRPSBFixCn3DStr(aapThis->cHtmlLink);
        fprintf(table,"%s",cn3dstr);
	cn3dstr = MemFree(cn3dstr);
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
	if (querygi) fprintf(table, "&querygi=%d",querygi); 
	else fprintf(table, "&query=%s",buf);
	fprintf(table, "&aln=%s",aln);
	fprintf(table, "&ascbin=6&maxaln=10&seltype=3");
	fprintf(table,"\"><IMG SRC=\"%spinkb.gif\" BORDER=0></A></TD>\n",urlcgi);
      } else {
        fprintf(table,"<TD>&nbsp;</TD>\n");
      }
    }
    strncpy(dupstr,aapThis->long_defline,PATH_MAX);
    part1 = strtok(dupstr," ");
    if (part1) {
      part2 = dupstr + strlen(part1) + 1;
      part2[min(strlen(part2),65)] = '\0';
      if (strlen(part2) == 65) {
        part2[64]='.'; part2[63]='.'; part2[62]='.';
      }
    } else {
      part1 = StringSave(" ");
      part2 = StringSave(" ");
    }
    fprintf(table,"<TD NOWRAP ALIGN=LEFT><a href=\"%s\">%s</a></TD>\n",
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
/*   if (!aap->bIsProfile) {
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
    */
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
        if (Nlm_StrCmp(aap->cDatabase,"CDD") == 0) {
          fprintf(fp,", %s, %s, %s\n",aap->cCDDid,aap->cGraphId,description->data.ptrvalue);
	} else if (Nlm_StrCmp(aap->cDatabase,"Cdd") == 0) {
          fprintf(fp,", %s, %s\n",aap->cGraphId,description->data.ptrvalue);
	} else if (Nlm_StrCmp(aap->cDatabase,"Cog") == 0) {
          fprintf(fp,", %s, %s\n",aap->cGraphId,description->data.ptrvalue);
	} else {
          fprintf(fp,", %s\n", description->data.ptrvalue);
        }
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
/*
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
*/    
    if (option&DISP_FULL_HTML) fprintf(fp,"<pre>\n");
/*---------------------------------------------------------------------------*/
/* need to explicitly get the subject sequence from a FASTA file (versions..)*/
/*---------------------------------------------------------------------------*/
    sfp = FileOpen(aap->cSeqFile,"r");
    if (!sfp) WRPSBHtmlError("Could not find subject sequence!", FALSE);
    sep = FastaToSeqEntry(sfp,FALSE);
    FileClose(sfp);
    if (!sep) WRPSBHtmlError("Could not read subject sequence!", FALSE);
    bsp = (BioseqPtr) sep->data.ptrvalue;

    /* bsp = BioseqLockById(new_id); */
    subject_length = bsp->length;
    if (maxsubjaln >= subject_length) {
      WRPSBHtmlError("Error in RPS-Blast alignment!", FALSE);
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

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* check whether a hit is unique, or to a CD hit somewhere else in the list  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean WRPSBHitIsNew(AlignmentAbstractPtr aapThis,
                             AlignmentAbstractPtr aapHead,
			     Dart_Connect *Connection,
			     Int4 *cDartFam, Int4 cDartFamNum)
{
  AlignmentAbstractPtr aap;
  int                  Size, i;
  unsigned             Gilist[DARTSIZELIMIT];
  char                 Accession[DARTSIZELIMIT][30];
  
  if (!aapThis) return FALSE;
  for (i=0;i<cDartFamNum;i++) {
    if (cDartFam[i] == aapThis->pssmid) {
     aapThis->bIsArch = TRUE;
     return(TRUE);    
    }
  }
  if (Connection) {
    if (aapThis->pssmid > 0) {
      if (Dart_SameSim(Connection,aapThis->pssmid,Gilist,DARTSIZELIMIT,&Size)) {
        for (i=0;i<Size;i++) {
          if (!Dart_CDGi2Acc(Connection,Gilist[i],Accession[i],30)) {
	    Accession[i][0] = '\0';
          }
	}
      }
    } else {
      if (Dart_Related(Connection,aapThis->cCDDid,Gilist,DARTSIZELIMIT,&Size,NULL)) {
        for (i=0;i<Size;i++) {
          if (!Dart_CDGi2Acc(Connection,Gilist[i],Accession[i],30)) {
	    Accession[i][0] = '\0';
          }
	}
      }
    }
  } else {
    Size = 0;
  }
  
  aap = aapHead; while (aap) {
    if (aapThis->pssmid > -1 && aap->pssmid > -1) {
      if (aapThis->pssmid == aap->pssmid) {
        aapThis->colcyc = aap->colcyc;
        return FALSE;
      }
    }
    for (i=0;i<Size;i++) {
      if (Nlm_StrCmp(Accession[i],aap->cCDDid) == 0) {
        aapThis->colcyc = aap->colcyc;
        return FALSE;
      }
    }
    if (Nlm_StrCmp(CDDlocat,"inhouse")==0) {   /* map names just for inhouse */
      if (aapThis->cCDDid && aap->cCDDid) {
        if (Nlm_StrCmp(aapThis->cCDDid,aap->cCDDid) == 0) {
          aapThis->colcyc = aap->colcyc;
          return FALSE;
        }
      }
      if (Nlm_StrICmp(aapThis->cGraphId,aap->cGraphId) == 0) {
        aapThis->colcyc = aap->colcyc;
        return FALSE;
      }
    }
    aap = aap->next;
  }
  return TRUE;			     
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* define the indents on a block containing repeats according to the alignmt */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void WRPSBIndentsViaSeqAlign(CddRepeatPtr pcdr,
                                    AlignmentAbstractPtr aap,
				    Int4 iGraphWidth,
				    Int4 length)
{
  SeqLocPtr      slp;
  SeqIntPtr      sintp;
  Int4Ptr        istarts;
  Int4           i = 0, j, s1, s2;
  CddExpAlignPtr pCDea;
  DenseSegPtr    dsp;
  
  istarts = MemNew(pcdr->count * sizeof (Int4));
  slp = (SeqLocPtr) pcdr->location->data.ptrvalue;
  while (slp) {
    sintp = (SeqIntPtr) slp->data.ptrvalue;
    istarts[i] = sintp->from; i++;
    slp = slp->next;
  }
  
  pCDea = CddExpAlignNew();
  CddExpAlignAlloc(pCDea,length);
  dsp = aap->salp->segs;
  for (i=0;i<dsp->numseg;i++) {
    s1 = dsp->starts[i*2];
    s2 = dsp->starts[i*2+1]; 
    if (s1 >=0 && s2>= 0) {
      for (j=0;j<dsp->lens[i];j++) {
        pCDea->adata[s1+j]=s2+j;
      }
    }
  }
  aap->indents = MemNew(pcdr->count * sizeof (Int4));
  aap->nindents = 0;
  for (i=0;i<pcdr->count;i++) {
    for (j=0;j<length;j++) {
      if (pCDea->adata[j] >= istarts[i]) {
        s1 = (j * iGraphWidth) / (length - 1);
	if (s1 > aap->gstart && s1 < aap->gstop) {
	  aap->indents[aap->nindents] = s1;
	  aap->nindents++;
	}
	break;
      }
    }
  }
  MemFree(istarts);
}

static CddDescPtr FindDescByPSSMid(CddDescPtr pcdd, Int4 pssmid)
{
  CddDescPtr pcddThis;
  pcddThis = pcdd;
  while(pcddThis) {
    if (pcddThis->iPssmId == pssmid) return (pcddThis);
    pcddThis = pcddThis->next;
  }
  return(NULL);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* prioritize curated CDs, enforce database order in displaying results      */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean SeqAlignIsTo(SeqAlignPtr salp, CharPtr template)
{
  DenseSegPtr  dsp;
  SeqIdPtr     sip;
  DbtagPtr     dbtp;
  BioseqPtr    bsp;
  CharPtr      buffer;
  
  
  dsp = salp->segs;
  sip = dsp->ids->next;
  bsp = BioseqLockById(sip);
  if (bsp) {
    buffer = BioseqGetTitle(bsp);
    if (Nlm_StrNCmp(buffer,template,2) == 0) return TRUE;
    BioseqUnlock(bsp);
  }
  
  return FALSE;
}

static SeqAlignPtr WRPSBCl3SortAlignment(SeqAlignPtr salpin)
{
  SeqAlignPtr salpHead, salpTail, salpThis, salp;
  
  salpHead = salpTail = NULL;
  salpThis = salpin;
  while (salpThis) {
    if (SeqAlignIsTo(salpThis,"cd")) {
      salp = CddSeqAlignDup(salpThis);
      if (!salpHead) {
        salpHead = salp;
      } else {
        salpTail->next = salp;
      }
      salpTail = salp;
    }
    salpThis = salpThis->next;
  } 
  salpThis = salpin;
  while (salpThis) {
    if (SeqAlignIsTo(salpThis,"sm")) {
      salp = CddSeqAlignDup(salpThis);
      if (!salpHead) {
        salpHead = salp;
      } else {
        salpTail->next = salp;
      }
      salpTail = salp;
    }
    salpThis = salpThis->next;
  } 
  salpThis = salpin;
  while (salpThis) {
    if (SeqAlignIsTo(salpThis,"pf")) {
      salp = CddSeqAlignDup(salpThis);
      if (!salpHead) {
        salpHead = salp;
      } else {
        salpTail->next = salp;
      }
      salpTail = salp;
    }
    salpThis = salpThis->next;
  } 
  salpThis = salpin;
  while (salpThis) {
    if (SeqAlignIsTo(salpThis,"CO")) {
      salp = CddSeqAlignDup(salpThis);
      if (!salpHead) {
        salpHead = salp;
      } else {
        salpTail->next = salp;
      }
      salpTail = salp;
    }
    salpThis = salpThis->next;
  } 
  salpThis = salpin;
  while (salpThis) {
    if (SeqAlignIsTo(salpThis,"KO")) {
      salp = CddSeqAlignDup(salpThis);
      if (!salpHead) {
        salpHead = salp;
      } else {
        salpTail->next = salp;
      }
      salpTail = salp;
    }
    salpThis = salpThis->next;
  } 
  salpThis = salpin;
  while (salpThis) {
    if (SeqAlignIsTo(salpThis,"LO")) {
      salp = CddSeqAlignDup(salpThis);
      if (!salpHead) {
        salpHead = salp;
      } else {
        salpTail->next = salp;
      }
      salpTail = salp;
    }
    salpThis = salpThis->next;
  } 
  return(salpHead);
}



/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* fill in the Alignment Abstract Data Structure                             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
AlignmentAbstractPtr WRPSBCl3AbstractAlignment(BlastPruneSapStructPtr prune,
                                               BioseqPtr query_bsp,
                                               Int4 iGraphWidth,
                                               Int4 *mxr,
                                               Int4 iGraphMode,
                                               CharPtr dbversion,
                                               Boolean *bAnyPdb,
					       Dart_Connect *Connection,
					       Boolean bFull,
					       CharPtr cCDDefault,
					       CharPtr cDATApath,
					       CharPtr cCDDPrefix,
					       CharPtr cCDDPost_C,
					       CharPtr cCDDPost_O,
					       CharPtr cTREextens,
					       CharPtr cSEQextens,
					       CharPtr cOASIScgi,
					       CharPtr cCDDcgi,
					       CddDescPtr pcdd,
					       Int4    *cDartFam,
					       Int4    cDartFamNum,
					       Boolean bAnnotOnly)
{
  Boolean              *bConflict;
  Boolean              bDbIsOasis = TRUE, found_score = FALSE;
  Int4                 maxrow = 1, i, pssmid;
  Int4                 lastcol = -1;
  Int4                 iCount = 0, replen;
  Int4                 iColValue, number, score, fullCDstart, fullCDstop, nindent;
  Int4                 *iOvrlap, *iMutual, isize;
  AlignmentAbstractPtr aap, aapThis, aapTmp, aapHead = NULL;
  CddDescPtr           pcddThis;
  BioseqPtr            bsp;
  CddTreePtr           pcddt;
  CddDescrPtr          description;
  CddRepeatPtr         pcdr;
  CharPtr              cTemp;
  CharPtr              cCurrDesc;
  DbtagPtr             dbtp;
  DenseSegPtr          dsp;
  ObjectIdPtr          oidp;
  ScorePtr             thisScorePtr;
  SeqAlignPtr          sap;
  SeqIdPtr             sip, gi_list;
  TxDfLineStructPtr    txsp;
  Nlm_FloatHi          evalue, bit_score;
  Char                 path[PATH_MAX], hpath[PATH_MAX];
  Char                 buffer[BUFFER_LENGTH+1];
  Char                 cDatabase[16];
  Char                 cCDDid[16];
  Char                 CDDidx[PATH_MAX];

  if (bFull) {
    Nlm_StrCpy(CDDidx,cDATApath);
    Nlm_StrCat(CDDidx,"/cdd.idx");
    *bAnyPdb = FALSE;
  }
/*  if (Nlm_StrCmp(myargs[1].strvalue,"cdd_prop")==0) bDbIsOasis = FALSE; */
  
  sap = prune->sap;
/*  sap = WRPSBCl3SortAlignment(prune->sap); */
  while (sap) {
    iCount++;
    aapThis = (AlignmentAbstractPtr)MemNew(sizeof(AlignmentAbstract));
    aapThis->salp = sap; aapThis->pssmid = -1;
    dsp = sap->segs;
    for (i=0;i<dsp->numseg;i++) {
      if (dsp->starts[2*i] > -1) {
        aapThis->mstart = dsp->starts[2*i];
	aapThis->nmissg = (Nlm_FloatHi) dsp->starts[2*i+1];
	fullCDstart = aapThis->mstart - aapThis->nmissg;
        break;
      }
    }
    for (i=0;i<dsp->numseg;i++) {
      if (dsp->starts[2*i] > -1) {
        aapThis->mstop = dsp->starts[2*i]+dsp->lens[i]-1;
	aapThis->cmissg = (Nlm_FloatHi) (dsp->starts[2*i+1]+dsp->lens[i]-1);
      }
    }
    aapThis->score = sap->score;
    sip = dsp->ids->next;
    if (bFull) {
      aapThis->gstart = (aapThis->mstart * iGraphWidth) / (query_bsp->length-1);
      aapThis->gstop =  (aapThis->mstop  * iGraphWidth) / (query_bsp->length-1);
    }
    thisScorePtr = sap->score;
    found_score = GetScoreAndEvalue(sap, &score, &bit_score, &evalue, &number);
    if      (evalue >  100.0   ) iColValue = 204;
    else if (evalue >    1.0   ) iColValue = 153;
    else if (evalue >    0.01  ) iColValue = 102;
    else if (evalue >    0.0001) iColValue =  51;
    else                         iColValue =   0;
    if (bFull) {
      bsp = BioseqLockById(sip);
      if (!bsp) {
        if (sip->choice == SEQID_GENERAL) {
          dbtp = sip->data.ptrvalue;
	  if (Nlm_StrCmp(dbtp->db,"Cdd") == 0) {
	    oidp = dbtp->tag;
	    CddPssmIdFromAcc(&pssmid, oidp->str, CDDidx);
            oidp = ObjectIdNew();
	    oidp->id = pssmid;
	    dbtp = DbtagNew();
	    dbtp->db = StringSave("CDD");
	    dbtp->tag = oidp;
	    sip->data.ptrvalue = dbtp;
            bsp = BioseqLockById(sip);
          }
	}
      }
    } else bsp = NULL;
    if (bsp) aapThis->nmissg = aapThis->nmissg/(Nlm_FloatHi)bsp->length;
    if (bsp) {
      fullCDstop = aapThis->mstop + bsp->length - 1 - aapThis->cmissg;
      aapThis->cmissg = ((Nlm_FloatHi)bsp->length-1.0-aapThis->cmissg)/(Nlm_FloatHi) bsp->length;
    }
    txsp = (TxDfLineStructPtr) MemNew(sizeof(TxDfLineStruct));
    txsp->segs_str = NULL;
    txsp->segs_buflen = 0;
    if(bsp != NULL) {
      gi_list = GetUseThisGi(sap);
      if (gi_list) {
        FilterTheDefline(bsp, gi_list, buffer, BUFFER_LENGTH, &(txsp->title));
        gi_list = SeqIdSetFree(gi_list);
        sip = SeqIdFree(sip);
        txsp->id = SeqIdParse(buffer);
      } else {
        SeqIdWrite(bsp->id, buffer, PRINTID_FASTA_LONG, BUFFER_LENGTH);
        txsp->title = StringSave(BioseqGetTitle(bsp));
        txsp->id = sip;
      }
      txsp->is_na = (bsp->mol != Seq_mol_aa);
    } else {
      SeqIdWrite(sip, buffer, PRINTID_FASTA_LONG, BUFFER_LENGTH);
      txsp->title = StringSave("Unknown");
      txsp->is_na = FALSE;
      txsp->id = sip;
    }
    txsp->seqalign = sap;
    txsp->buffer_id = StringSave(buffer);
    txsp->score = score;
    txsp->bit_score = bit_score;
    txsp->evalue = evalue;
    txsp->number = number;
    txsp->found_score = found_score;
    SeqAlignSegsStr(sap, 1, &txsp->segs_str, &txsp->segs_buflen, &txsp->segs_used);
    txsp->isnew = FALSE;
    txsp->waschecked = FALSE;
    if(NULL != bsp) BioseqUnlock(bsp);
    aapThis->defline = txsp;
    isize = 2+strlen(aapThis->defline->title)+strlen(aapThis->defline->buffer_id);
    if (isize < CDD_MAX_DESCR) isize = CDD_MAX_DESCR;
    aapThis->long_defline = MemNew(sizeof(Char)*isize);
    StrCpy(aapThis->long_defline, aapThis->defline->buffer_id);
    StrCat(aapThis->long_defline," ");
     cTemp = StringSave(txsp->buffer_id);
    if (strncmp(cTemp,"gnl|",4)==0) {
      strtok(cTemp,"|");
      aapThis->cDatabase = StringSave(strtok(NULL,"|"));
      Nlm_StrCpy(path,strtok(NULL,"|"));
      aapThis->cCDDid = StringSave(strtok(path," "));
      MemFree(cTemp);
    } else WRPSBHtmlError("Could not interpret subject defline!", bAnnotOnly);
   if (bFull) {
      StrCat(aapThis->long_defline,aapThis->defline->title);
    } else {
      pcddThis = FindDescByPSSMid(pcdd,atoi(aapThis->cCDDid));
      if (pcddThis) {
        StrCat(aapThis->long_defline,pcddThis->cCddId);     
        StrCat(aapThis->long_defline,", ");     
        StrCat(aapThis->long_defline,pcddThis->cSourc);     
        StrCat(aapThis->long_defline,", ");     
        StrCat(aapThis->long_defline,pcddThis->cDescr);
	aapThis->nmissg = aapThis->nmissg/(Nlm_FloatHi)pcddThis->iPssmLength;
        fullCDstop = aapThis->mstop + pcddThis->iPssmLength - 1 - aapThis->cmissg;
        aapThis->cmissg = ((Nlm_FloatHi)pcddThis->iPssmLength-1.0-aapThis->cmissg)/(Nlm_FloatHi)pcddThis->iPssmLength;
      } else {
        CddSevError("Failed to retrieve information for a PSSM-Id!");
      }    
    }
    aapThis->bIsProfile = FALSE;
    if (StringICmp(aapThis->cDatabase,"Smart")==0) {
      aapThis->red = 255;
      aapThis->green = aapThis->blue = iColValue;
      if (StrNCmp(aapThis->cCDDid,"smart0",6) == 0) {
        cTemp = StringSave(txsp->title);
        aapThis->cGraphId = StringSave(strtok(cTemp,","));
	MemFree(cTemp);
      } else {
        aapThis->cGraphId = aapThis->cCDDid;
      }
    } else if (StringICmp(aapThis->cDatabase,"Pfam") ==0) {
      aapThis->blue = 255;
      aapThis->red = aapThis->green = iColValue;
      cTemp = StringSave(txsp->title);
      aapThis->cGraphId = StringSave(strtok(cTemp,","));
      MemFree(cTemp);
    } else if (StringICmp(aapThis->cDatabase,"scop1.39") ==0) {
      aapThis->green = 255;
      aapThis->red = aapThis->blue = iColValue;
      aapThis->bIsProfile = TRUE;    
      aapThis->cGraphId = aapThis->cCDDid;
    } else if (StringICmp(aapThis->cDatabase,"Load") ==0) {
      aapThis->green = iColValue;
      aapThis->red = aapThis->blue = 255;    
      cTemp = StringSave(aapThis->cCDDid);
      if (strstr(cTemp,":")) {
        strtok(cTemp,":");
        aapThis->cGraphId = StringSave(strtok(NULL,":"));
      } else {
        strtok(cTemp,"_");
        aapThis->cGraphId = StringSave(strtok(NULL,"_"));
      }
      MemFree(cTemp);
    } else if (StringCmp(aapThis->cDatabase,"CDD") == 0) {
      aapThis->pssmid = atoi(aapThis->cCDDid);
      if (bFull) {
        cTemp = StringSave(txsp->title);
        aapThis->cCDDid = StringSave(strtok(cTemp,","));
        aapThis->cGraphId = StringSave(strtok(NULL,","));
        if (!aapThis->cGraphId) aapThis->cGraphId = StringSave("obsolete");
      } else {
        aapThis->cCDDid = StringSave(pcddThis->cCddId);
	aapThis->cGraphId = StringSave(pcddThis->cSourc);
      }
      if (Nlm_StrNCmp(aapThis->cGraphId," ",1) == 0) aapThis->cGraphId = aapThis->cGraphId+1;
      CddTruncStringAtFirstPunct(aapThis->cGraphId);
      CddFillBlanksInString(aapThis->cGraphId);
    } else if (StringCmp(aapThis->cDatabase,"Cdd") == 0) {
      cTemp = StringSave(txsp->title);
      if (Nlm_StrStr(cTemp,",") != NULL) {
        if (Nlm_StrStr(cTemp,";") == NULL ||
	    Nlm_StrStr(cTemp,",") < Nlm_StrStr(cTemp,";")) {
	  aapThis->cGraphId = StringSave(strtok(cTemp,","));
	} else aapThis->cGraphId = StringSave(strtok(cTemp,";"));
      } else aapThis->cGraphId = StringSave(strtok(cTemp,";"));
    } else if (StringCmp(aapThis->cDatabase,"Cog") == 0) {
      cTemp = StringSave(txsp->title);
      if (Nlm_StrStr(cTemp,",") != NULL) {
        if (Nlm_StrStr(cTemp,";") == NULL ||
	    Nlm_StrStr(cTemp,",") < Nlm_StrStr(cTemp,";")) {
	  aapThis->cGraphId = StringSave(strtok(cTemp,","));
	} else aapThis->cGraphId = StringSave(strtok(cTemp,";"));
      } else aapThis->cGraphId = StringSave(strtok(cTemp,";"));
    } else {
      aapThis->green = iColValue;
      aapThis->red = aapThis->blue = 255;    
      aapThis->cGraphId = aapThis->cCDDid;
    }
    aapThis->bIsArch = FALSE;
    aapThis->bIsArchComplete = FALSE;
    if (!bFull && NULL != cDartFam) {
      for (i=0;i<cDartFamNum;i++) {
        if (aapThis->pssmid == cDartFam[i]) {
	  aapThis->bIsArch = TRUE;
          aapThis->bIsArchComplete = (aapThis->nmissg + aapThis->cmissg < 0.1);
	  break;
	}
      }
    } else {
      if (WRPSBHitIsNew(aapThis,aapHead,Connection,cDartFam,cDartFamNum)) {
        if (!aapThis->bIsArch) {
          aapThis->colcyc = lastcol + 1;
          if (aapThis->colcyc >= iNcolors) {
            aapThis->colcyc -= iNcolors;
          }
          lastcol = aapThis->colcyc;
        } else {
          aapThis->bIsArchComplete = (aapThis->nmissg + aapThis->cmissg < 0.1);
        }
      }

/*
    if (evalue > 0.01) {
      aapThis->red   = iDartCol[aapThis->colcyc+iNcolors][0];
      aapThis->green = iDartCol[aapThis->colcyc+iNcolors][1];
      aapThis->blue  = iDartCol[aapThis->colcyc+iNcolors][2];
    
    } else {
      aapThis->red   = iDartCol[aapThis->colcyc][0];
      aapThis->green = iDartCol[aapThis->colcyc][1];
      aapThis->blue  = iDartCol[aapThis->colcyc][2];
    }
*/
      if (aapThis->bIsArch) {
        aapThis->colcyc = 255;
        aapThis->red    = 200;
        aapThis->green  = 200;
        aapThis->blue   = 200;
      } else {
        aapThis->red   = iDartCol[aapThis->colcyc][0];
        aapThis->green = iDartCol[aapThis->colcyc][1];
        aapThis->blue  = iDartCol[aapThis->colcyc][2];
      }
      sprintf(aapThis->name,"ali%d",(Int4)random());
      aapThis->bIsOasis = bDbIsOasis;
      if (bDbIsOasis) {
        Nlm_StrCpy(path,cCDDPrefix);
        if (Nlm_StrNCmp(aapThis->cCDDid,"COG",3) == 0 || Nlm_StrNCmp(aapThis->cCDDid,"KOG",3) == 0) {
          strcat(path,cCDDefault);
        } else strcat(path,dbversion);
        strcat(path,cCDDPost_O);
        Nlm_StrCpy(hpath,cOASIScgi);
      } else {
        Nlm_StrCpy(path,cCDDPrefix);
        strcat(path,dbversion);
        strcat(path,cCDDPost_C);
        Nlm_StrCpy(hpath,cCDDcgi);
      }
      strcat(path,"/"); strcat(path,aapThis->cCDDid); strcat(path,cTREextens);
      strcat(hpath,aapThis->cCDDid);
      strcat(hpath,"&version=");
      if (Nlm_StrNCmp(aapThis->cCDDid,"COG",3) == 0 || Nlm_StrNCmp(aapThis->cCDDid,"KOG",3) == 0) {
        strcat(hpath,CDDefault);
      } else strcat(hpath,dbversion);
      aapThis->cHtmlLink = StringSave(hpath);
/*---------------------------------------------------------------------------*/
/* Open Cdd tree file and add description to aapThis data structure          */
/* changed to binary read to get prepared for the v1.00 rollout. Aron 6/12/00*/
/*---------------------------------------------------------------------------*/
      pcddt = (CddTreePtr) CddTreeReadFromFile(path,TRUE);
      aapThis->bHasStructure = FALSE;
      if (pcddt) {
        aapThis->description = pcddt->description;
        description = aapThis->description;
        while (description) {
          if (description->choice == CddDescr_comment) {
            cCurrDesc = description->data.ptrvalue;
            if (Nlm_StrCmp(cCurrDesc,"linked to 3D-structure")==0) {
              aapThis->bHasStructure = TRUE;
              *bAnyPdb = TRUE;
            }
          }
	  if (description->choice == CddDescr_repeats) {
	    pcdr = (CddRepeatPtr) description->data.ptrvalue;
	    nindent = pcdr->count - 1;
	    if (NULL == pcdr->location) {
	      replen = (fullCDstop - fullCDstart + 1) / (nindent+1);
	      aapThis->nindents = 0;
	      aapThis->indents = MemNew(nindent * sizeof(Int4));
	      for (i=0;i<nindent;i++) {
	        fullCDstop = ((fullCDstart + (i+1)*replen) * iGraphWidth) / (query_bsp->length - 1);
                if (fullCDstop > aapThis->gstart && fullCDstop < aapThis->gstop) {
                  aapThis->indents[aapThis->nindents] = fullCDstop;
	          aapThis->nindents++;
	        }
	      }
	    } else {
	      WRPSBIndentsViaSeqAlign(pcdr,aapThis,iGraphWidth,query_bsp->length);
	    }
	  }
          description = description->next;
        }
      }
/*---------------------------------------------------------------------------*/
/* get the file name for the FASTA-sequence file which stores the subject..  */
/*---------------------------------------------------------------------------*/
      aapThis->cSeqFile = MemNew(PATH_MAX*sizeof(Char));
      if (bDbIsOasis) {
        Nlm_StrCpy(aapThis->cSeqFile,cCDDPrefix);
        if (Nlm_StrNCmp(aapThis->cCDDid,"COG",3) == 0 || Nlm_StrNCmp(aapThis->cCDDid,"KOG",3) == 0) {
          strcat(aapThis->cSeqFile,cCDDefault);
        } else {
          strcat(aapThis->cSeqFile,dbversion);
        }
        strcat(aapThis->cSeqFile,cCDDPost_O);
      } else {
        Nlm_StrCpy(aapThis->cSeqFile,CDDPrefix); strcat(aapThis->cSeqFile,dbversion);
        strcat(aapThis->cSeqFile,cCDDPost_C);
      }
      strcat(aapThis->cSeqFile,"/"); strcat(aapThis->cSeqFile,aapThis->cCDDid);
      strcat(aapThis->cSeqFile,cSEQextens);
    }   
/*---------------------------------------------------------------------------*/
/* Check for overlaps and determine row number                               */
/* Check for mutal overlap if condensed graphics is selected                 */
/*---------------------------------------------------------------------------*/
    iOvrlap = MemNew(maxrow*sizeof(Int4));
    iMutual = MemNew(maxrow*sizeof(Int4));
    for (i=0;i<maxrow;i++) iOvrlap[i]=0;
    aapThis->row = 1; aapThis->bDrawThisOne = TRUE;
    if (aapHead) {    
      aapTmp = aapHead;
      while (aapTmp) {
        if (aapTmp->bDrawThisOne) {
          if (OverlapInterval(aapTmp->mstart,aapTmp->mstop, aapThis->mstart,aapThis->mstop))
            iOvrlap[aapTmp->row-1] = 1;
          if (iGraphMode == 1 || iGraphMode == 3) {
            if (OverlapMutual(aapTmp->mstart,aapTmp->mstop,aapThis->mstart,aapThis->mstop)) {
              iMutual[aapTmp->row-1] = 1;
	      if (aapThis->bIsArch) {
	        if (aapThis->bIsArchComplete) {
		  if (!aapTmp->bIsArch || !aapTmp->bIsArchComplete) iMutual[aapTmp->row-1] = 0; 
		}
	      } else {
	        if (aapTmp->bIsArch) iMutual[aapTmp->row-1] = 0;
	      }
            }
          }
        }
        aapTmp = aapTmp->next;
      }
      aapThis->row = 0;
      for (i=0;i<maxrow;i++) {
        if (iOvrlap[i]==0) {
          aapThis->row = i+1;
          break;
        }
      }
      if (iGraphMode == 1 || iGraphMode == 3) for (i=0;i<maxrow;i++) {
        if (iMutual[i]==1) {
          aapThis->bDrawThisOne = FALSE;
          break;
        }
      }
      if (aapThis->row == 0) aapThis->row = maxrow+1;
    }
    MemFree(iMutual);
    MemFree(iOvrlap);
    if (aapThis->row > maxrow) maxrow = aapThis->row;

    aapThis->next = NULL;

    if (aapHead==NULL) {
       aapHead = aapThis;
       aap = aapHead;
    } else {
       aap->next = aapThis;
       aap = aapThis;
    }
/*
    if (txsp) {
      txsp->buffer_id = MemFree(txsp->buffer_id);
      txsp->title = MemFree(txsp->title);
      txsp->segs_str = MemFree(txsp->segs_str);
      txsp = MemFree(txsp);
    }
*/
    sap = sap->next;
  }
  if (bFull) *mxr = maxrow;
  return(aapHead);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
SeqAnnotPtr WRPSBCl3SeqAnnot(AlignmentAbstractPtr aapIn, FILE *table,
                             Boolean bSeqAlign, Boolean bRetSeqAnnot)
{
  AsnIoPtr              aip;
  AlignmentAbstractPtr  aap;
  SeqAnnotPtr           sap;
  SeqLocPtr             slp;
  SeqFeatPtr            sfp, sfpHead = NULL, sfpTail = NULL;
  DenseSegPtr           dsp;
  SeqIdPtr              sipQuery;
  SeqIntPtr             sintp;
  DbtagPtr              dbtp;
  ObjectIdPtr           oidp;
  Char                  dupstr[PATH_MAX];
  CharPtr               part1, rest, thispart;
  UserObjectPtr         uop;
  UserFieldPtr          ufp, ufpTail;
  Nlm_FloatHi           evalue, bit_score;
  Int4                  score, number, i, icnt;
  Boolean               found_score;
  AnnotDescPtr          adp, adp2;
  DatePtr               dp;
  SeqAlignPtr           salpTail = NULL, salpHead = NULL;

  sap = SeqAnnotNew();
  if (bSeqAlign) {
    sap->type = 2; /* Sequence Alignment */
  } else sap->type = 1; /* feature table */
  adp = (AnnotDescPtr) ValNodeNew(NULL);
  adp->choice = Annot_descr_name;
  adp->data.ptrvalue = StringSave("CDDSearch");
  sap->desc = adp;
  adp2 = (AnnotDescPtr) ValNodeNew(NULL);
  dp = DateCurr();
  adp2->choice = Annot_descr_create_date;
  adp2->data.ptrvalue = dp;
  adp->next = adp2;
  aap = aapIn;
  while (aap) {
    if (aap->bDrawThisOne) {
      sfp = SeqFeatNew();
      if (bSeqAlign) {
        if (!salpHead) {
	  salpHead = aap->salp;
	} else salpTail->next = aap->salp;
	  salpTail = aap->salp;
	  salpTail->next = NULL;
      } else {
        sfp->data.choice = 9;
        strncpy(dupstr,aap->long_defline,PATH_MAX);
        part1 = strtok(dupstr," ");
        if (part1) {
          rest = dupstr + strlen(part1) + 1;
        } else {
          rest = StringSave(aap->long_defline);
        }
        icnt = 0;
        for (i=0;i<Nlm_StrLen(rest);i++) {
          if (rest[i] == ',') icnt++;
	  if (icnt > 1) break;
        }
        if (icnt > 1) {
          i = i+2;
        } else i = 0;
        thispart = &rest[i];
        for (i=0;i<Nlm_StrLen(thispart);i++) {
          if (thispart[i] == '.' || thispart[i] == ';') {
	    thispart[i] = '\0';
	    break;
	  }
        }
        sfp->data.value.ptrvalue = StringSave(thispart);
        sfp->comment = StringSave(aap->cGraphId);   /* comment is short name */
        if (aap->nmissg >= 0.2 || aap->cmissg >= 0.2) sfp->partial = TRUE;
        dsp = aap->salp->segs;
        sipQuery = dsp->ids;
        slp = (SeqLocPtr) ValNodeNew(NULL); slp->choice = SEQLOC_INT;
        sintp=SeqIntNew(); sintp->from = aap->mstart; sintp->to = aap->mstop;
        sintp->id = SeqIdDup(sipQuery); slp->data.ptrvalue = sintp;
        sfp->location = slp;
        if (sfp->partial) {
          SetSeqLocPartial(sfp->location, aap->nmissg >= 0.2, aap->cmissg >= 0.2);
        }
        uop = UserObjectNew();
        oidp = ObjectIdNew();
        oidp->str = StringSave("cddScoreData");
        uop->type = oidp;
        ufp = UserFieldNew();
        oidp = ObjectIdNew();
        oidp->str = StringSave("definition");
        ufp->label = oidp;
        ufp->choice = 1;
        ufp->data.ptrvalue =  StringSave(aap->cCDDid);
        uop->data = ufp;
        ufpTail = ufp;
        ufp = UserFieldNew();
        oidp = ObjectIdNew();
        oidp->str = StringSave("short_name");
        ufp->label = oidp;
        ufp->choice = 1;
        ufp->data.ptrvalue = StringSave(aap->cGraphId);     
        ufpTail->next = ufp;
        ufpTail = ufp;
        found_score = GetScoreAndEvalue(aap->salp, &score, &bit_score, &evalue, &number);
        if (found_score) {
          ufp = UserFieldNew();
          oidp = ObjectIdNew();
          oidp->str = StringSave("score");
          ufp->label = oidp;
          ufp->choice = 2;
          ufp->data.intvalue = score;     
          ufpTail->next = ufp;
          ufpTail = ufp;
          ufp = UserFieldNew();
          oidp = ObjectIdNew();
          oidp->str = StringSave("evalue");
          ufp->label = oidp;
          ufp->choice = 3;
          ufp->data.realvalue = evalue;     
          ufpTail->next = ufp;
          ufpTail = ufp;
          ufp = UserFieldNew();
          oidp = ObjectIdNew();
          oidp->str = StringSave("bit_score");
          ufp->label = oidp;
          ufp->choice = 3;
          ufp->data.realvalue = bit_score;     
          ufpTail->next = ufp;
          ufpTail = ufp;
        }
        sfp->ext = uop;
        dbtp = DbtagNew();
        dbtp->db = StringSave(aap->cDatabase);
        oidp = ObjectIdNew();
        oidp->id = aap->pssmid;
        dbtp->tag = oidp;
        sfp->dbxref = ValNodeNew(NULL);
        sfp->dbxref->data.ptrvalue = dbtp;
        if (!sfpHead) {
          sfpHead = sfp;
        } else {
          sfpTail->next = sfp;
        }
        sfpTail = sfp;
      }
    }
    aap = aap->next;
  }
  if (bSeqAlign) {
    sap->data = salpHead;
  } else sap->data = sfpHead;
  if (bRetSeqAnnot) return(sap);
  strcpy(OutputName,GetTempName("wrpsbcl3"));
  aip = AsnIoOpen(OutputName, "w");
  SeqAnnotAsnWrite((SeqAnnotPtr) sap, aip, NULL);
  AsnIoClose(aip);
  PrintFile(OutputName);
  RemoveTempFiles();   
  return(NULL);
}

AlignmentAbstractPtr AlignmentAbstractSetDestruct(AlignmentAbstractPtr aap)
{
  AlignmentAbstractPtr  aapThis, aapNext, aapHead;

  aapThis = aap; aapHead = aap;
  while (aapThis) {
    aapNext = aapThis->next;
    aapThis->long_defline = MemFree(aapThis->long_defline);
    aapThis->cCDDid = MemFree(aapThis->cCDDid);
    aapThis->cGraphId = MemFree(aapThis->cGraphId);
    aapThis->cDatabase = MemFree(aapThis->cDatabase);
    if (aapThis->defline)  {
      aapThis->defline->buffer_id = MemFree(aapThis->defline->buffer_id);
      aapThis->defline->title = MemFree(aapThis->defline->title);
      aapThis->defline->segs_str = MemFree(aapThis->defline->segs_str);
      aapThis->defline = MemFree(aapThis->defline);
    }
    aapThis = (AlignmentAbstractPtr) MemFree(aapThis);
    aapThis = aapNext;
  }
  return(aapHead);
}
