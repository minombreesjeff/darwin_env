/*   wprint.c
* ===========================================================================
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
*  any work or product based on this material
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
* File Name:  wprint.c
*
* Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov
*
* Version Creation Date:   7/15/95
*
* $Revision: 6.71 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

/*************************************
*
 * $Log: wprint.c,v $
 * Revision 6.71  2002/08/08 14:00:37  kans
 * added FANTOM_DB hyperlink
 *
 * Revision 6.70  2002/07/24 21:08:47  kans
 * reverted ncbi URL
 *
 * Revision 6.69  2002/07/23 16:44:35  kans
 * changed www.ncbi.nlm.nih.gov to www.ncbi.nih.gov
 *
 * Revision 6.68  2002/06/21 15:38:34  kans
 * added GABI db_xref
 *
 * Revision 6.67  2002/06/18 21:00:00  kans
 * added ISFinder as legal db_xref with hotlink
 *
 * Revision 6.66  2002/05/06 22:20:49  kans
 * added IFO and JCM db_xref hotlinks
 *
 * Revision 6.65  2002/04/18 22:24:16  kans
 * added dbEST and dbSTS links
 *
 * Revision 6.64  2002/02/20 21:59:44  tatiana
 * IMGT/LIGM dbxref added
 *
 * Revision 6.63  2002/02/01 19:40:01  kans
 * MGD uses id=MGI: URL prefix, strips MGI: if in db_xref text
 *
 * Revision 6.62  2002/01/02 13:21:16  kans
 * added WormBase links
 *
 * Revision 6.61  2001/12/06 17:00:41  kans
 * TextSave takes size_t, not Int2, otherwise titin protein tries to allocate negative number
 *
 * Revision 6.60  2001/12/06 12:36:41  kans
 * switch from viewer.cgi to viewer.fcgi
 *
 * Revision 6.59  2001/11/27 18:45:16  kans
 * fixed FLYBASE capitalization, added niaEST
 *
 * Revision 6.58  2001/10/02 17:39:29  yaschenk
 * Removing memory leaks
 *
 * Revision 6.57  2001/08/06 23:59:45  kans
 * added third party annotation SeqID support
 *
 * Revision 6.56  2001/08/06 22:13:13  kans
 * using NUM_SEQID, added TPA ids to arrays
 *
 * Revision 6.55  2001/04/23 22:40:04  tatiana
 * estimated size increased in www_featkey()
 *
 * Revision 6.54  2001/04/12 22:31:37  yaschenk
 * fixing string size estimates in www_accession
 *
 * Revision 6.53  2001/02/01 16:23:09  tatiana
 * typo fixed in RGD link
 *
 * Revision 6.52  2001/01/03 17:01:44  tatiana
 * a bug fixed in LocGenomePrint, complement added
 *
 * Revision 6.51  2000/12/06 20:56:17  tatiana
 * AceView link added
 *
 * Revision 6.50  2000/12/05 23:04:37  tatiana
 * NT_ COMMENT formatted
 *
 * Revision 6.49  2000/12/05 01:22:18  tatiana
 * COG added
 *
 * Revision 6.47  2000/10/24 20:33:39  tatiana
 * added link to UniSTS and InterimID
 *
 * Revision 6.46  2000/10/18 21:19:59  kans
 * changed omim link
 *
 * Revision 6.45  2000/10/18 12:29:37  tatiana
 * rice link changed
 *
 * Revision 6.44  2000/09/05 18:22:48  kans
 * added CDD link
 *
 * Revision 6.43  2000/08/07 14:03:03  tatiana
 * printf format fix
 *
 * Revision 6.42  2000/07/25 14:40:37  tatiana
 * changed SGD link
 *
 * Revision 6.41  2000/07/20 21:22:36  tatiana
 * refseq hot link corrected
 *
 * Revision 6.40  2000/07/14 20:24:27  kans
 * added RGD as dbxref with web link
 *
 * Revision 6.39  2000/07/11 22:17:12  tatiana
 * fixed hot link for refseq
 *
 * Revision 6.38  2000/07/10 21:02:17  tatiana
 * added link to refseq
 *
 * Revision 6.37  2000/06/21 18:31:04  tatiana
 * fixed qmap.cgi syntax for dbsource link
 *
 * Revision 6.36  2000/03/23 14:45:53  tatiana
 * added links for MGD, FBgn, FBan
 *
 * Revision 6.35  2000/03/20 17:47:51  kans
 * changed rice link
 *
 * Revision 6.34  2000/03/09 22:16:56  kans
 * changed rice link
 *
 * Revision 6.33  2000/02/29 15:33:15  tatiana
 * RiceGenes URL changed
 *
 * Revision 6.32  2000/02/25 21:52:03  kans
 * changed flybase link to indiana university url
 *
 * Revision 6.31  2000/02/24 17:53:56  kans
 * added dbSNP and RATMAP to dbxref
 *
 * Revision 6.30  2000/01/11 17:14:13  tatiana
 * check for web symbols added in PrintComment
 *
 * Revision 6.29  1999/12/21 14:27:10  tatiana
 * version added to CONTIG line
 *
 * Revision 6.28  1999/11/02 19:57:04  tatiana
 * a typo fixed in PrintGenome
 *
 * Revision 6.27  1999/11/02 16:31:20  tatiana
 * fixed PrintGenome()
 *
 * Revision 6.26  1999/10/01 16:40:24  vakatov
 * Fixed a stack memory overrun bug in PrintDate()
 *
 * Revision 6.25  1999/09/29 17:20:38  tatiana
 * SeqIdFindBest changed to SeqIdSelect for CONTIG line
 *
 * Revision 6.24  1999/08/31 18:31:24  tatiana
 * OLDQUERY ifdef added
 *
 * Revision 6.23  1999/08/30 17:07:01  tatiana
 * minor changes for html version printing
 *
 * Revision 6.22  1999/05/13 23:00:44  tatiana
 * hot link added to REFSEQ in COMMENT
 *
 * Revision 6.21  1999/04/07 22:17:58  tatiana
 * added BR in web GARPHIK_FMT
 *
 * Revision 6.20  1999/04/07 20:49:18  tatiana
 * fix compiler warning www_protein_id()
 *
 * Revision 6.19  1999/04/06 22:37:14  tatiana
 * www_protein_id() added
 *
 * Revision 6.18  1999/04/06 14:56:57  tatiana
 * SWISS-PROT link disabled
 *
 * Revision 6.17  1999/04/06 14:41:44  tatiana
 * LocusLink corrections
 *
 * Revision 6.16  1999/03/30 21:02:18  tatiana
 * www_accession www_taxid added
 *
 * Revision 6.15  1999/03/29 18:15:33  tatiana
 * OMIM and LocusLink hyper-links added to dbxref
 *
 * Revision 6.14  1999/03/12 17:33:47  tatiana
 * www_featkey() added and type casting fixed
 *
 * Revision 6.13  1998/11/23 17:05:35  tatiana
 * a bug fixed in SP link in www_db_xref()
 *
 * Revision 6.12  1998/10/08 15:14:52  tatiana
 * link to SWISSPROT - expasy.hcuge.ch removed
 *
 * Revision 6.11  1998/08/19 18:42:08  tatiana
 * hot link to RiceGenes added
 *
 * Revision 6.10  1998/08/04 14:58:30  tatiana
 * ff_AddString changed to ff_AddStringWithTildes in www_PrintComment()
 *
 * Revision 6.9  1998/07/24 16:51:06  vakatov
 * added LIBCALL modifier to "www_PrintComment()"
 *
 * Revision 6.8  1998/07/23 22:41:52  tatiana
 * added www_PrintComment()
 *
 * Revision 6.7  1998/05/28 18:30:27  tatiana
 * typedef for HeadTailProc moved to ffprint.h
 *
 * Revision 6.6  1998/04/30 21:52:48  tatiana
 * *** empty log message ***
 *
 * Revision 6.5  1998/04/29 16:21:48  tatiana
 * www_dbsource(): db=n changed to db=s in query link
 *
 * Revision 6.4  1998/02/11 19:37:05  tatiana
 * check for NULL added in LocPrintGenome()
 *
 * Revision 6.3  1998/02/06 21:21:08  tatiana
 * db changed to s in query URL
 *
 * Revision 6.0  1997/08/25 18:08:38  madden
 * Revision changed to 6.0
 *
 * Revision 5.21  1997/08/19 22:34:18  tatiana
 * bug fixed in PrintGenome()
 *
 * Revision 5.19  1997/08/07 22:18:13  tatiana
 * PrintGenome() fix for the WWW
 *
 * Revision 5.18  1997/08/06 22:38:03  tatiana
 * minor fixes in PrintGenome()
 *
 * Revision 5.17  1997/07/28 14:31:31  vakatov
 * Keep the head_tail_ff() proto in-sync with its header-located declaration
 *
 * Revision 5.16  1997/07/28 13:29:38  ostell
 * Moved GetUniGeneIDForSeqId() to seqmgr.c
 *
 * Revision 5.15  1997/07/25 15:51:37  tatiana
 * sition counts in PrintGenome
 *
 * Revision 5.14  1997/07/16 21:26:32  tatiana
 * PrintGenome() added
 *
 * Revision 5.13  1997/06/26 20:56:43  tatiana
 * a bug fixed in PrintSPBlock
 *
 * Revision 5.12  1997/06/23 19:18:53  tatiana
 * www_featloc() fixed
 *
 * Revision 5.11  1997/06/19 18:39:57  vakatov
 * [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
 *
 * Revision 5.10  1997/03/13 21:37:37  tatiana
 * query?-guid changed to query?uid in all Entrez limks
 *
 * Revision 5.9  1997/03/11  20:17:25  tatiana
 * OMIM hot links added to DBSOURCE
 *
 * Revision 5.8  1997/01/10  19:51:39  epstein
 * add missing LIBCALLs
 *
 * Revision 5.7  1997/01/06  21:38:52  vakatov
 * Removed local(static) NCBI_months[12] -- use that from the "ncbitime.[ch]"
 *
 * Revision 5.6  1996/12/03  15:50:43  tatiana
 * hot link to CK in db_xref added
 *
 * Revision 5.5  1996/08/16  20:35:44  tatiana
 * www_coded_by added
 *
 * Revision 5.4  1996/07/19  21:36:29  tatiana
 * protection for NULL acc added to PrintSPBlock()
 *
 * Revision 5.3  1996/07/19  18:13:16  tatiana
 * ddbj hot link bug fixed (gbj -> dbj)
 *
 * Revision 5.2  1996/07/12  19:49:52  tatiana
 * SP hot link added to db_xref
 *
 * Revision 5.1  1996/06/25  22:22:09  tatiana
 * new GDB hot list
 *
 * Revision 4.25  1996/04/29  19:54:11  tatiana
 * new SGD URL
 *
 * Revision 4.24  1996/04/15  16:08:36  tatiana
 * new line in DBSOURCE
 *
 * Revision 4.23  1996/04/15  14:37:43  tatiana
 * old_www_featloc removed
 *
 * Revision 4.21  1996/04/09  16:12:19  tatiana
 * *** empty log message ***
 *
 * Revision 4.20  1996/04/09  14:05:15  tatiana
 * DescrStructPtr changes
 *
 * Revision 4.18  1996/04/08  21:53:17  tatiana
 * change in www_featloc
 *
 * Revision 4.17  1996/03/27  15:40:55  epstein
 * re-retro previous 'referer' changes, and change swissprot output to point to protein DB
 *
 * Revision 4.16  1996/03/25  15:21:46  tatiana
 * www_featloc added
 *
 * Revision 4.15  1996/03/20  16:13:58  epstein
 * change print-logic to accomodate 'referer' script which logs references to external databases
 *
 * Revision 4.14  1996/02/28  04:53:06  ostell
 * minor fixes
 *
 * Revision 4.13  1996/02/26  03:45:24  ostell
 * fixed usage of GatherDescrListByChoice and changed function to allocate
 * first DescrStruct instead using static storage
 *
 * Revision 4.12  1996/02/26  00:46:18  ostell
 * removed unused local variables and integer size mismatch fusses
 *
 * Revision 4.11  1996/02/21  20:08:03  tatiana
 * a bug fixed n www_dbsource
 *
 * Revision 4.9  1995/12/20  22:43:19  tatiana
 * Int2 changed to Int4 in www_organism()
 *
 * Revision 4.8  1995/12/19  23:34:20  tatiana
 * hot link to SGD added
 *
 * Revision 4.7  1995/12/13  16:36:15  tatiana
 * hot link to db_xref added
 *
 * Revision 4.6  1995/11/24  15:38:44  tatiana
 * GetAppParam moved to init_www
 *
 * Revision 4.5  1995/11/22  18:58:01  tatiana
 * turn spaces to + in hotlink
 *
 * Revision 4.4  1995/11/17  21:48:28  tatiana
 * hot link to genetic code added
 *
 * Revision 4.3  1995/11/17  21:28:35  kans
 * asn2ff now uses gather (Tatiana)
 *
 * Revision 1.10  1995/07/17  19:33:20  kans
 * parameters combined into Asn2ffJobPtr structure
 *
*
**************************************/


#include <ncbi.h>
#include <objsset.h>
#include <prtutil.h>
#include <seqport.h>
#include <sequtil.h>
#include <asn2ffp.h>
#include <ffprint.h>

#ifdef ENABLE_ENTREZ
#include <accentr.h>
#endif

#define MAX_WWWBUF 328
#define MAX_WWWLOC 2

static CharPtr lim_str [MAX_WWWLOC] = {">","<"};
static CharPtr www_lim_str [MAX_WWWLOC] = {"&gt;","&lt;"};

static Char link_ff[MAX_WWWBUF];
static Char link_muid[MAX_WWWBUF];
static Char link_seq[MAX_WWWBUF];
static Char link_ace[MAX_WWWBUF];
static Char link_tax[MAX_WWWBUF];
static Char link_code[MAX_WWWBUF];
static Char link_fly[MAX_WWWBUF];
static Char link_cog[MAX_WWWBUF];
static Char link_sgd[MAX_WWWBUF];
static Char link_gdb[MAX_WWWBUF];
static Char link_ck[MAX_WWWBUF];
static Char link_rice[MAX_WWWBUF];
static Char link_sp[MAX_WWWBUF];
static Char link_pir[MAX_WWWBUF];
static Char link_pdb[MAX_WWWBUF];
static Char link_gdb_map[MAX_WWWBUF];
static Char link_UniSTS[MAX_WWWBUF];
static Char link_dbSTS[MAX_WWWBUF];
static Char link_dbEST[MAX_WWWBUF];
static Char link_omim[MAX_WWWBUF];
static Char link_locus[MAX_WWWBUF];
static Char link_snp[MAX_WWWBUF];
static Char link_ratmap[MAX_WWWBUF];
static Char link_rgd[MAX_WWWBUF];
static Char link_mgd[MAX_WWWBUF];
static Char link_fly_fban[MAX_WWWBUF];
static Char link_fly_fbgn[MAX_WWWBUF];
static Char link_cdd[MAX_WWWBUF];
static Char link_niaest[MAX_WWWBUF];
static Char link_worm_sequence[MAX_WWWBUF];
static Char link_worm_locus[MAX_WWWBUF];
static Char link_imgt[MAX_WWWBUF];
static Char link_ifo[MAX_WWWBUF];
static Char link_jcm[MAX_WWWBUF];
static Char link_isfinder[MAX_WWWBUF];
static Char link_gabi[MAX_WWWBUF];
static Char link_fantom[MAX_WWWBUF];

#define DEF_LINK_FF  "/cgi-bin/Entrez/getfeat?"

#define DEF_LINK_ACE  "http://www.ncbi.nlm.nih.gov/AceView/hs.cgi?"
#define DEF_LINK_SEQ  "http://www.ncbi.nlm.nih.gov/entrez/viewer.fcgi?"
#define DEF_LINK_MUID  "/entrez/utils/qmap.cgi?"

#define DEF_LINK_TAX "/htbin-post/Taxonomy/wgetorg?"
#define DEF_LINK_CODE "/htbin-post/Taxonomy/wprintgc?"

#define DEF_LINK_COG "http://www.ncbi.nlm.nih.gov/cgi-bin/COG/palox?"
#define DEF_LINK_MGD "http://www.informatics.jax.org/searches/accession_report.cgi?id=MGI:"
#define DEF_LINK_FBGN "http://flybase.bio.indiana.edu/.bin/fbidq.html?"
#define DEF_LINK_FBAN "http://www.fruitfly.org/cgi-bin/annot/fban?"

/*
#define DEF_LINK_FLY "/cgi-bin/Entrez/referer?http://morgan.harvard.edu/htbin-post/gene.script%3f"
*/
#define DEF_LINK_FLY "http://flybase.bio.indiana.edu/.bin/fbidq.html?"
/*
#define DEF_LINK_SGD "/cgi-bin/Entrez/referer?http://genome-www.stanford.edu/cgi-bin/dbrun/SacchDB%3ffind+SGDID+"
*/

#define DEF_LINK_SGD "/cgi-bin/Entrez/referer?http://genome-www4.stanford.edu/cgi-bin/SGD/locus.pl?locus="

#define DEF_LINK_GDB "http://gdbwww.gdb.org/gdb-bin/genera/genera/hgd/DBObject/GDB:"
#define DEF_LINK_CK "http://flybane.berkeley.edu/cgi-bin/cDNA/CK_clone.pl?db=CK&dbid="
/*
#define DEF_LINK_RICE "http://genome.cornell.edu:80/cgi-bin/webace?db=ricegenes&class=Probe&object="
*/
#define DEF_LINK_RICE "http://ars-genome.cornell.edu/cgi-bin/WebAce/webace?db=ricegenes&class=Marker&object="
#define DEF_LINK_SP "/cgi-bin/Entrez/referer?http://expasy.hcuge.ch/cgi-bin/sprot-search-ac%3f"
#define DEF_LINK_PDB "/cgi-bin/Entrez/referer?http://expasy.hcuge.ch/cgi-bin/get-pdb-entry%3f"
#define DEF_LINK_GDB_PREFIX "http://gdbwww.gdb.org/gdb-bin/gdb/browser/bin/locq?ACTION=query&cyto="

#define DEF_LINK_GDB_SUFFIX "&match=Inclusive&order=Locus+Location"

#define DEF_LINK_UniSTS "http://www.ncbi.nlm.nih.gov/genome/sts/sts.cgi?uid="
#define DEF_LINK_dbSTS "http://www.ncbi.nlm.nih.gov/entrez/viewer.fcgi?"
#define DEF_LINK_dbEST "http://www.ncbi.nlm.nih.gov/entrez/viewer.fcgi?"
#define DEF_LINK_OMIM "http://www.ncbi.nlm.nih.gov/entrez/dispomim.cgi?id="
#define DEF_LINK_LOCUS "http://www.ncbi.nlm.nih.gov/LocusLink/LocRpt.cgi?l="
#define DEF_LINK_SNP "http://www.ncbi.nlm.nih.gov/SNP/snp_ref.cgi?type=rs&rs="
#define DEF_LINK_RATMAP "http://ratmap.gen.gu.se/action.lasso?-database=RATMAPfmPro&-layout=Detail&-response=/RM/Detail+Format.html&-search&-recid="
#define DEF_LINK_RGD "http://rgd.mcw.edu/query/query.cgi?id="

#define DEF_LINK_CDD "http://www.ncbi.nlm.nih.gov/Structure/cdd/cddsrv.cgi?uid="
#define DEF_LINK_NIAEST "http://lgsun.grc.nia.nih.gov/cgi-bin/pro3?sname1="
#define DEF_LINK_WORM_SEQUENCE "http://www.wormbase.org/db/seq/sequence?name="
#define DEF_LINK_WORM_LOCUS "http://www.wormbase.org/db/gene/locus?name="
#define DEF_LINK_IMGT "http://imgt.cines.fr:8104/cgi-bin/IMGTlect.jv?query=202+"
#define DEF_LINK_IFO "http://www.ifo.or.jp/index_e.html"
#define DEF_LINK_JCM "http://www.jcm.riken.go.jp/cgi-bin/jcm/jcm_number?JCM="
#define DEF_LINK_ISFINDER "http://www-is.biotoul.fr/scripts/is/is_spec.idc?name="
#define DEF_LINK_GABI "https://gabi.rzpd.de/cgi-bin-protected/GreenCards.pl.cgi?Mode=ShowBioObject&BioObjectName="
#define DEF_LINK_FANTOM "http://fantom.gsc.riken.go.jp/db/view/main.cgi?masterid="

/* now other data bases are linked to Entrez. may be changed later 
static char *link_epd = 
"/htbin-post/Entrez/query";
static char *link_tfd = 
"/htbin-post/Entrez/query";
*/
static Boolean www = FALSE;
static Pointer data = NULL;
static HeadTailProc head = NULL;
static HeadTailProc tail = NULL;

NLM_EXTERN CharPtr PrintDate(NCBI_DatePtr date)
{
	CharPtr retval = NULL;
	char month[4], year[5], day[3];
	char result[15];

	if ( date -> data[0] == 0){
/*---string---*/
		if (StringICmp(date -> str,"Not given") != 0){
			retval = StringSave(date -> str);
		}
	} else {
/*---standard---*/
		if (date->data[1] && date->data[2] && date->data[3]) {
			if ((int) (date -> data[1]) < 30) {
				sprintf(year, "%4d", (int) (date -> data[1] + 2000));
			} else {
				sprintf(year, "%4d", (int) (date -> data[1] + 1900));
			}
			sprintf(day, "%d", (int) (date -> data[3] ));
			StringCpy(month, NCBI_months[date->data[2] -1 ]);
			sprintf(result,"%s %s, %s.\n", month, day, year);
			retval = StringSave(result);
		}
	}
	return retval;
}

void PrintXrefButton PROTO((FILE *fp, SeqEntryPtr sep));

NLM_EXTERN void LIBCALL init_www(void)
{
	www = TRUE;
	GetAppParam("NCBI", "WWWENTREZ", "LINK_FF", DEF_LINK_FF, 
		link_ff, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_MUID", DEF_LINK_MUID, 
		link_muid, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_ACE", DEF_LINK_ACE, 
		link_ace, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_SEQ", DEF_LINK_SEQ, 
		link_seq, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_TAX", DEF_LINK_TAX, 
		link_tax, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_CODE", DEF_LINK_CODE, 
		link_code, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_FLY", DEF_LINK_FLY, 
		link_fly, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_COG", DEF_LINK_COG, 
		link_cog, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_SGD", DEF_LINK_SGD, 
		link_sgd, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_SGD", DEF_LINK_GDB, 
		link_gdb, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_CK", DEF_LINK_CK, 
		link_ck, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_RICE", DEF_LINK_RICE, 
		link_rice, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_SP", DEF_LINK_SP, 
		link_sp, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_PDB", DEF_LINK_PDB, 
		link_pdb, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_OMIM", DEF_LINK_OMIM, 
		link_omim, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_UniSTS", DEF_LINK_UniSTS, 
		link_UniSTS, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_dbSTS", DEF_LINK_dbSTS, 
		link_dbSTS, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_dbEST", DEF_LINK_dbEST, 
		link_dbEST, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_LOCUS", DEF_LINK_LOCUS, 
		link_locus, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_SNP", DEF_LINK_SNP, 
		link_snp, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_RATMAP", DEF_LINK_RATMAP, 
		link_ratmap, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_RGD", DEF_LINK_RGD, 
		link_rgd, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_MGD", DEF_LINK_MGD, 
		link_mgd, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_FBGN", DEF_LINK_FBGN, 
		link_fly_fbgn, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_FBAN", DEF_LINK_FBAN, 
		link_fly_fban, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_CDD", DEF_LINK_CDD, 
		link_cdd, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_NIAEST", DEF_LINK_NIAEST, 
		link_niaest, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_WORM_SEQUENCE", DEF_LINK_WORM_SEQUENCE, 
		link_worm_sequence, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_WORM_LOCUS", DEF_LINK_WORM_LOCUS, 
		link_worm_locus, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_IMGT", DEF_LINK_IMGT, 
		link_imgt, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_IFO", DEF_LINK_IFO, 
		link_ifo, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_JCM", DEF_LINK_JCM, 
		link_jcm, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_ISFINDER", DEF_LINK_ISFINDER,
		link_isfinder, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_GABI", DEF_LINK_GABI,
		link_gabi, MAX_WWWBUF);
	GetAppParam("NCBI", "WWWENTREZ", "LINK_FANTOM", DEF_LINK_FANTOM,
		link_fantom, MAX_WWWBUF);

}

NLM_EXTERN void LIBCALL fini_www(void)
{
	www = FALSE;
}

NLM_EXTERN void LIBCALL head_tail_ff(Pointer mydata, HeadTailProc headfun, HeadTailProc tailfun)
{
	data = mydata;
	head = headfun;
	tail = tailfun;
}

NLM_EXTERN void LIBCALL head_www(FILE *fp, SeqEntryPtr sep)
{

	if (head != NULL) {
		head(data, fp);
		return;
	}
	if (!www) {
		return;
	}
	fprintf(fp, "Content-type: text/html\n\n");
	fprintf(fp, "<HTML>\n");
	fprintf(fp, "<HEAD><TITLE>%s", "GenBank entry");
	fprintf(fp, "</TITLE></HEAD>\n");
	fprintf(fp, "<BODY>\n");
	fprintf(fp, "<hr>\n");
	fprintf(fp, "<pre>");
} 

NLM_EXTERN void LIBCALL tail_www(FILE *fp)
{
	if (tail != NULL) {
		tail(data, fp);
		return;
	}
	if (!www) {
		return;
	}
	fprintf(fp, "</pre>\n");
	fprintf(fp, "<hr>\n");
	fprintf(fp, "</BODY>\n");
	fprintf(fp, "</HTML>\n");
}

NLM_EXTERN Boolean LIBCALL get_www(void)
{
	return www;
}

/**************************************************************************
*	source is coming from GBBlock.source so we have to find real 
*	organism name for the hot link 
***************************************************************************/
NLM_EXTERN Boolean LIBCALL www_source(CharPtr orgname, OrgRefPtr orp)
{
	Int2	l, ll, lll;
	CharPtr	s;
	
	if (www && orp) {
		l = StringLen(link_tax);
		lll = StringLen(orgname);
		ll = StringLen("<a href=%sname=%s>");
		s = (CharPtr)MemNew(l+ ll + lll);
		sprintf(s, "<a href=%sname=%s>", link_tax, 
					orp->taxname?orp->taxname:orp->common);
		AddLink(s);
		MemFree(s);
		ff_AddString(orgname);
		AddLink("</a>");
	} else {
		ff_AddString(orgname);
	}
		return TRUE;

}			
NLM_EXTERN Boolean LIBCALL www_organism(CharPtr orgname, Int4 id)
{
	Int2	l, ll, lll;
	CharPtr	s, linkname, ss;
	
	if (www) {
		l = StringLen(link_tax);
		lll = StringLen(orgname);
		linkname = StringSave(orgname);
		for (ss = linkname; *ss != '\0'; ss++) {
			if (*ss == ' ') {
				*ss = '+';
			}
		}
		if (id != -1) {
			ll = StringLen("<a href=%sid=%d>");
		} else {
			ll = StringLen("<a href=%sname=%s>");
		}
		s = (CharPtr)MemNew(l+ ll + lll);
		if (id != -1) {
			sprintf(s, "<a href=%sid=%d>", link_tax, id);
		} else {
			sprintf(s, "<a href=%sname=%s>", link_tax, linkname);
		}
		AddLink(s);
		MemFree(s);
		ff_AddString(orgname);
		AddLink("</a>");
		MemFree(linkname);
	} else {
		ff_AddString(orgname);
	}
		return TRUE;
}			

NLM_EXTERN Boolean LIBCALL www_taxid(CharPtr orgname, Int4 id)
{
	Int2	l, ll, lll;
	CharPtr	s, linkname, ss;
	
	if (www) {
		l = StringLen(link_tax);
		lll = StringLen(orgname);
		linkname = StringSave(orgname);
		for (ss = linkname; *ss != '\0'; ss++) {
			if (*ss == ' ') {
				*ss = '+';
			}
		}
		if (id != -1) {
			ll = StringLen("<a href=%sid=%d>");
		} else {
			ll = StringLen("<a href=%sname=%s>");
		}
		s = (CharPtr)MemNew(l+ ll + lll);
		if (id != -1) {
			sprintf(s, "<a href=%sid=%d>", link_tax, id);
		} else {
			sprintf(s, "<a href=%sname=%s>", link_tax, linkname);
		}
		AddLink(s);
		MemFree(s);
		ff_AddString(orgname);
	/*	ff_AddInteger(" (%d)", id);*/
		AddLink("</a>");
		MemFree(linkname);
	} else {
		ff_AddString(orgname);
	}
		return TRUE;
}			

NLM_EXTERN Boolean LIBCALL www_featkey(CharPtr key, Int4 gi, Int2 entityID, Int2 itemID)
{
	Int2	l, ll;
	CharPtr	s;
	
	if (www) {
		l = StringLen(link_ff);
		ll = StringLen("<a href=%sgi=%ld&id=%d&entity=%d>");
		s = (CharPtr)MemNew(l+ ll + 3*7);
		sprintf(s, "<a href=%sgi=%ld&id=%d&entity=%d>", 
							link_ff, (Int4) gi, itemID, entityID);
		AddLink(s);
		MemFree(s);
		ff_AddString(key);
		AddLink("</a>");
	} else {
		ff_AddString(key);
	}
		return TRUE;
}			

NLM_EXTERN Boolean LIBCALL www_gcode(CharPtr gcode)
{
	Int2	l, ll, gc, lll = 1;
	CharPtr	s;
	
	if (www) {
		gc = atoi(gcode);
		if (gc >= 10)
			lll = 2;
		l = StringLen(link_code);
			ll = StringLen("<a href=%smode=c#SG%d>");
		s = (CharPtr)MemNew(l+ ll + lll);
			sprintf(s, "<a href=%smode=c#SG%d>", link_code, gc);
		AddLink(s);
		MemFree(s);
		ff_AddInteger("%d", gc);
		AddLink("</a>");
	} else {
		ff_AddString(gcode);
	}
		return TRUE;
}			

NLM_EXTERN Boolean LIBCALL www_muid(Int4 muid)
{
	Int2	l, ll;
	CharPtr	s;
	
	if (www) {
		l = StringLen(link_muid);
		ll = StringLen("<a href=%suid=%ld&form=6&db=m&Dopt=r>");
		s = (CharPtr)MemNew(l+ ll + 10);
		sprintf(s, "<a href=%suid=%ld&form=6&db=m&Dopt=r>", 
												link_muid, (Int4) muid);
		AddLink(s);
		MemFree(s);
		ff_AddInteger("%ld", (long) muid);
		AddLink("</a>");
	} else {
		ff_AddInteger("%ld", (long) muid);
	}
		return TRUE;
}			

NLM_EXTERN Boolean LIBCALL www_extra_acc(CharPtr acc, Boolean ncbi)
{
	Int2	l, ll;
	CharPtr	s, prefix;
	
	if (www && !ncbi) {
		l = StringLen(link_seq);
		prefix = "<a href=%sval=%s>"; 
		ll = StringLen(prefix); 
		s = (CharPtr)MemNew(l+ ll + 10);
		sprintf(s, prefix, link_seq, acc);
		AddLink(s);
		MemFree(s);
		ff_AddString( acc);
		AddLink("</a>");
	} else {
		ff_AddString( acc);
	}
	return TRUE;
}

NLM_EXTERN Boolean LIBCALL www_genpept_gi(CharPtr str)
{
	Int2	l, ll;
	Int4 	gi;
	CharPtr	s, prefix;
	
	if(www) {
			l = StringLen(link_seq);
			prefix = "<a href=%sval=%ld>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l+ ll + 10);
		if (StringNCmp(str, "gi|", 3) == 0) {
			str += 3;
			gi = atoi(str);
			ff_AddString("gi|");
			sprintf(s, prefix, link_seq, gi);
			AddLink(s);
			MemFree(s);
			ff_AddInteger("%ld", (long) gi);
			AddLink("</a>");
			for(; IS_DIGIT(*str); str++);
			ff_AddString(str);
		}
	} else {
		ff_AddString(str);
	}
	return TRUE;
}

NLM_EXTERN Boolean LIBCALL www_dbsource(CharPtr str, Boolean first, Uint1 choice)
{
	Int2	l, ll;
	CharPtr	s, prefix=NULL, p, text, link=NULL;
	
	if(www) {
		if (choice == SEQID_PIR /*|| choice == SEQID_SWISSPROT*/) {
			link = link_seq;
			prefix = "<a href=%sval=%s>";
		} else if (choice == SEQID_PDB || choice == SEQID_PRF) {
			link = link_seq;
			prefix = "<a href=%sval=%s>";
		} else if (choice == SEQID_EMBL || choice == SEQID_GENBANK || 
				choice == SEQID_DDBJ || choice == SEQID_GIBBSQ || 
				choice == SEQID_GIBBMT || choice == SEQID_GI || 
				choice == SEQID_GIIM || choice == SEQID_OTHER ||
				choice == SEQID_TPG || choice == SEQID_TPE || choice == SEQID_TPD)  {
			link = link_seq;
			prefix = "<a href=%sval=%s>";
		} else {
			ff_AddStringWithTildes(str);
			return TRUE;
		}
		l = StringLen(link);
		ll = StringLen(prefix); 
		if ((p = StringStr(str, "accession")) != NULL) {
			p += 9;
			while (*p == ' ') {
				p++;
			}
			text = TextSave(str, p-str);
			if (first == FALSE) {
				ff_AddString(", ");
			}
			ff_AddString(text);
			MemFree(text);
			text = StringSave(p);
			if (text[StringLen(text)-1] == ';') {
				text[StringLen(text)-1] = '\0';
			}
			s = (CharPtr)MemNew(l+ ll + StringLen(text) + 1);
			sprintf(s, prefix, link, text);
			AddLink(s);
			MemFree(s);
			MemFree(text);
			ff_AddString(p);
			AddLink("</a>");
		} else {
			if (first == FALSE) {
				ff_AddString(", ");
			}
			ff_AddString(str);
		}
	} else {
		ff_AddStringWithTildes(str);
	}
	return TRUE;
}

NLM_EXTERN Boolean www_coded_by(CharPtr str)
{
	Int2	l, ll;
	CharPtr	s, ss, prefix, p, text, link;
	
	if(www) {
		link = link_seq;
		prefix = "<a href=%sval=%s>";
		p = StringChr(str, ':');
		while (p != NULL) {
			for (ss = p-1; ss > str && IS_ALPHANUM(*ss); ss--);
			if (ss > str) {
				ss++;
				text = TextSave(str, ss-str);
				ff_AddString(text);
				MemFree(text);
			}
			text = TextSave(ss, p-ss);
			l = StringLen(link);
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l+ ll + StringLen(text));
			sprintf(s, prefix, link, text);
			AddLink(s);
			MemFree(s);
			ff_AddString(text);
			AddLink("</a>");
			str = p;
			p = StringChr(str+1, ':');
		}
		ff_AddString(str);
	} else {
		ff_AddString(str);
	}
	return TRUE;
}

NLM_EXTERN Boolean LIBCALL www_map(CharPtr str)
{
	Int2	l, ll, lll, llll;
	CharPtr	s, prefix;

	if (www) {
			l = StringLen(DEF_LINK_GDB_PREFIX);
			prefix = "<a href=%s%s%s>"; 
			ll = StringLen(prefix); 
			lll = StringLen(str); 
			llll = StringLen(DEF_LINK_GDB_SUFFIX); 
			s = (CharPtr)MemNew(l+ ll + lll + llll);
			sprintf(s, prefix, DEF_LINK_GDB_PREFIX, str, DEF_LINK_GDB_SUFFIX);
			AddLink(s);
			MemFree(s);
			ff_AddString(str);
			AddLink("</a>");
	} else {
		ff_AddString(str);
	}
	return TRUE;

}
NLM_EXTERN Boolean LIBCALL www_protein_id(CharPtr str)
{
	Int2	l, ll;
	CharPtr	s, prefix;
	
	if (www) {
		l = StringLen(link_seq);
		prefix = "<a href=%sval=%s>"; 
		ll = StringLen(prefix) + StringLen(str); 
		s = (CharPtr)MemNew(l + ll);
		sprintf(s, prefix, link_seq, str);
		AddLink(s);
		MemFree(s);
		ff_AddString(str);
		AddLink("</a>");
	} else {
		ff_AddString(str);
	}
	return TRUE;
}

NLM_EXTERN Boolean LIBCALL www_db_xref(CharPtr str)
{
	Int2	l, ll;
	Int4 	gi;
	CharPtr	s, prefix, ss, p, pp;
	Boolean nothing = TRUE;
	Char id[10];
	Int2 id1, id2;
	
	if (www) {
		while ((p= StringStr(str, "FLYBASE:")) != NULL) {
			nothing = FALSE;
			p += StringLen("FlyBase:");
			l = StringLen(link_fly);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l+ ll + 20);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			for (pp = p; *pp != '\0' && *pp != ';'; pp++);
			ss = (CharPtr)MemNew(pp-p+1);
			StringNCpy(ss, p, pp-p);
			sprintf(s, prefix, link_fly, ss);
			AddLink(s);
			MemFree(s);
			ff_AddString(ss);			
			AddLink("</a>");
			ff_AddChar(';');
			str = pp+1;
			
		}
		if (( p = StringStr(str, "COG:")) != NULL) {
			nothing = FALSE;
			p += StringLen("COG:");
			l = StringLen(link_cog) + StringLen(p);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_cog, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "FLYBASE:FBa")) != NULL) {
			nothing = FALSE;
			p += StringLen("FLYBASE:");
			l = StringLen(link_fly_fban) + StringLen(p);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_fly_fban, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "FLYBASE:FBgn")) != NULL) {
			nothing = FALSE;
			p += StringLen("FLYBASE:");
			l = StringLen(link_fly_fbgn) + StringLen(p);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_fly_fbgn, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "PID:g")) != NULL) {
			nothing = FALSE;
			p += StringLen("PID:g");
			l = StringLen(link_seq);
			prefix = "<a href=%sval=%ld>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l+ ll + 10);
			gi = atoi(p);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			sprintf(s, prefix, link_seq, gi);
			AddLink(s);
			MemFree(s);
			ff_AddInteger("%ld", (long) gi);
			AddLink("</a>");
		} 
/*
		if (( p = StringStr(str, "SWISS-PROT:")) != NULL) {
			nothing = FALSE;
			p += StringLen("SWISS-PROT:");
			l = StringLen(link_seq) + StringLen(p);
			prefix = "<a href=%sval=%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_seq, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		}
*/		
		if (( p = StringStr(str, "UniSTS:")) != NULL) {
			nothing = FALSE;
			p += StringLen("UniSTS:");
			l = StringLen(link_UniSTS) + StringLen(p);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_UniSTS, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "dbEST:")) != NULL) {
			nothing = FALSE;
			p += StringLen("dbEST:");
			l = StringLen(link_dbEST) + StringLen(p);
			prefix = "<a href=%sval=gnl|dbest|%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_dbEST, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "dbSTS:")) != NULL) {
			nothing = FALSE;
			p += StringLen("dbSTS:");
			l = StringLen(link_dbSTS) + StringLen(p);
			prefix = "<a href=%sval=gnl|dbsts|%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_dbSTS, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "LocusID:")) != NULL) {
			nothing = FALSE;
			p += StringLen("LocusID:");
			l = StringLen(link_locus) + StringLen(p);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_locus, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "InterimID:")) != NULL) {
			nothing = FALSE;
			p += StringLen("InterimID:");
			l = StringLen(link_locus) + StringLen(p);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_locus, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "MIM:")) != NULL) {
			nothing = FALSE;
			p += StringLen("MIM:");
			l = StringLen(link_omim) + StringLen(p);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_omim, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "SGD:")) != NULL) {
			nothing = FALSE;
			p += StringLen("SGD:");
			l = StringLen(link_sgd) + StringLen(p);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_sgd, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "IMGT/LIGM:")) != NULL) {
			nothing = FALSE;
			p += StringLen("IMGT/LIGM:");
			l = StringLen(link_imgt) + StringLen(p);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_imgt, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "GDB:")) != NULL) {
			p += StringLen("GDB:");
			if (( pp = StringStr(p, "G00-")) != NULL) {
				pp += StringLen("G00-");
				id1 = atoi(pp);
				for (; *pp != '\0' && *pp != '-'; pp++);
				id2 = atoi(pp+1);
				sprintf(id, "%d%d", id1, id2);
				l = StringLen(link_gdb) + StringLen(p);
				prefix = "<a href=%s%s>"; 
				ll = StringLen(prefix); 
				s = (CharPtr)MemNew(l + ll);
				ss = (CharPtr)MemNew(p-str+1);
				StringNCpy(ss, str, p-str);
				ff_AddString(ss);
				MemFree(ss);
				sprintf(s, prefix, link_gdb, id);
				AddLink(s);
				MemFree(s);
				ff_AddString(p);
				AddLink("</a>");
				nothing = FALSE;
			} else if (IS_DIGIT(*p)) {
				l = StringLen(link_gdb) + StringLen(p);
				prefix = "<a href=%s%s>"; 
				ll = StringLen(prefix); 
				s = (CharPtr)MemNew(l + ll);
				ss = (CharPtr)MemNew(p-str+1);
				StringNCpy(ss, str, p-str);
				ff_AddString(ss);
				MemFree(ss);
				sprintf(s, prefix, link_gdb, p);
				AddLink(s);
				MemFree(s);
				ff_AddString(p);
				AddLink("</a>");
				nothing = FALSE;
			}
		} 
		if (( p = StringStr(str, "CK:")) != NULL) {
			nothing = FALSE;
			p += StringLen("CK:");
			l = StringLen(link_ck) + StringLen(p);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			sprintf(s, prefix, link_ck, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "RiceGenes:")) != NULL) {
			nothing = FALSE;
			p += StringLen("RiceGenes:");
			l = StringLen(link_rice) + StringLen(p);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			sprintf(s, prefix, link_rice, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "dbSNP:")) != NULL) {
			nothing = FALSE;
			p += StringLen("dbSNP:");
			l = StringLen(link_snp) + StringLen(p);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_snp, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "RATMAP:")) != NULL) {
			nothing = FALSE;
			p += StringLen("RATMAP:");
			l = StringLen(link_ratmap) + StringLen(p);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_ratmap, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "RGD:")) != NULL) {
			nothing = FALSE;
			p += StringLen("RGD:");
			l = StringLen(link_rgd) + StringLen(p);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_rgd, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "MGD:")) != NULL) {
			nothing = FALSE;
			p += StringLen("MGD:");
			l = StringLen(link_mgd) + StringLen(p);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			if (StringNICmp (p, "MGI:", 4) == 0) {
				p += 4;
			}
			sprintf(s, prefix, link_mgd, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "CDD:")) != NULL) {
			nothing = FALSE;
			p += StringLen("CDD:");
			l = StringLen(link_cdd) + StringLen(p);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_cdd, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "niaEST:")) != NULL) {
			nothing = FALSE;
			p += StringLen("niaEST:");
			l = StringLen(link_niaest) + StringLen(p);
			prefix = "<a href=%s%s&val=1>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_niaest, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "WormBase:")) != NULL) {
			CharPtr worm_link = NULL;
			nothing = FALSE;
			p += StringLen("WormBase:");
			if (StringStr (str, "unc") != NULL) {
				worm_link = link_worm_locus;
				l = StringLen(worm_link) + StringLen(p);
				prefix = "<a href=%s%s;class=Locus>"; 
			} else {
				worm_link = link_worm_sequence;
				l = StringLen(worm_link) + StringLen(p);
				prefix = "<a href=%s%s;class=Sequence>"; 
			}
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, worm_link, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "IFO:")) != NULL) {
			nothing = FALSE;
			p += StringLen("IFO:");
			l = StringLen(link_ifo) + StringLen(p);
			prefix = "<a href=%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_ifo);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "JCM:")) != NULL) {
			nothing = FALSE;
			p += StringLen("JCM:");
			l = StringLen(link_jcm) + StringLen(p);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_jcm, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		}
		if (( p = StringStr(str, "ISFinder:")) != NULL) {
			nothing = FALSE;
			p += StringLen("ISFinder:");
			l = StringLen(link_isfinder) + StringLen(p);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_isfinder, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		}
		if (( p = StringStr(str, "GABI:")) != NULL) {
			nothing = FALSE;
			p += StringLen("GABI:");
			l = StringLen(link_gabi) + StringLen(p);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_gabi, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		}
		if (( p = StringStr(str, "FANTOM_DB:")) != NULL) {
			nothing = FALSE;
			p += StringLen("FANTOM_DB:");
			l = StringLen(link_fantom) + StringLen(p);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			sprintf(s, prefix, link_fantom, p);
			AddLink(s);
			MemFree(s);
			ff_AddString(p);
			AddLink("</a>");
		}
		if (nothing) {
			ff_AddString(str);
		}
	} else {
		ff_AddString(str);
	}
	return TRUE;
}

NLM_EXTERN Boolean LIBCALL www_note_gi(CharPtr str)
{
	Int2	l, ll;
	Int4 	gi;
	CharPtr	s, prefix, ss, p, pp;
	Boolean nothing = TRUE;
	
	if (www) {
		while ((p= StringStr(str, "FlyBase: ")) != NULL) {
			nothing = FALSE;
			p += StringLen("FlyBase: ");
			l = StringLen(link_fly);
			prefix = "<a href=%s%s>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l+ ll + 20);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			while (*p == ' ')
				p++;
			for (pp = p; *pp != '\0' && *pp != ';'; pp++);
			ss = (CharPtr)MemNew(pp-p+1);
			StringNCpy(ss, p, pp-p);
			sprintf(s, prefix, link_fly, ss);
			AddLink(s);
			MemFree(s);
			ff_AddString(ss);			
			AddLink("</a>");
			ff_AddChar(';');
			str = pp+1;
			
		}
		if (( p = StringStr(str, "NCBI gi: ")) != NULL) {
			nothing = FALSE;
			p += StringLen("NCBI gi: ");
			l = StringLen(link_seq);
			prefix = "<a href=%sval=%ld>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l+ ll + 10);
			gi = atoi(p);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			sprintf(s, prefix, link_seq, gi);
			AddLink(s);
			MemFree(s);
			ff_AddInteger("%ld", (long) gi);
			AddLink("</a>");
		} 
		if (( p = StringStr(str, "AceView:")) != NULL) {
			nothing = FALSE;
		/*	p += StringLen("AceView:");*/
			gi = atoi(p + StringLen("AceView:"));
			l = StringLen(link_ace) + StringLen(p);
			prefix = "<a href=%sl=%ld>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-str+1);
			StringNCpy(ss, str, p-str);
			ff_AddString(ss);
			MemFree(ss);
			sprintf(s, prefix, link_ace, gi);
			AddLink(s);
			MemFree(s);
			ff_AddString("AceView");
		/*	ff_AddInteger("%ld", (long) gi);*/
			AddLink("</a>");
			
		} 
		if (nothing) {
			ff_AddString(str);
		}
	} else {
		ff_AddString(str);
	}
	return TRUE;
}

NLM_EXTERN Boolean LIBCALL www_xref(CharPtr str, Uint1 xref_class)
{
	Int2	l, ll;
	CharPtr	s, link, prefix, ss;
	
	if (www) {
		ss = (CharPtr)MemNew(StringLen(str) + 1);
		StringCpy(ss, str);
		if (xref_class == 5) {
			link = link_seq;    /*link_sp*/
			prefix = "<a href=%sval=%s>"; 
		} else if (xref_class == 8) {
			link = link_seq;  /*link_epd*/
			prefix = "";
		} else if (xref_class == 10) {
			link = link_seq;  /* link_tfd */
			prefix = "";
		} else if (xref_class == 11) {
			link = link_fly;
			prefix = "<a href=%s%s>";
		}
		if (link && prefix) {
			l = StringLen(link);
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l+ ll + 10);
			if (*(ss + StringLen(ss)  - 1) == '.') 
				*(ss + StringLen(ss)  - 1) = '\0';
				
			sprintf(s, prefix, link, ss);
			AddLink(s);
			MemFree(s);
		}
		ff_AddString( str);
		AddLink("</a>");
		MemFree(ss);
	} else {
		ff_AddString( str);
	}
	return TRUE;
}
NLM_EXTERN Boolean LIBCALL www_xref_button(FILE *fp, CharPtr str, Uint1 xref_class, Uint1 db)
{
	Int2	l, ll;
	CharPtr	s, link, prefix, ss;
	
	if (www) {
		ss = (CharPtr)MemNew(StringLen(str) + 1);
		StringCpy(ss, str);
		if (xref_class == 5) {
			link = link_seq;  /* link_sp */
			prefix = "<a href=%sval=%s>"; 
		} else if (xref_class == 8) {
			link = link_seq;  /* link_epd */
			prefix = "";
		} else if (xref_class == 10) {
			link = link_seq;   /*link_tfd*/
			prefix = "";
		} else if (xref_class == 11) {
			link = link_fly;
			prefix = "<a href=%s%s>";
		} if (xref_class == 255) {    /*for PIR and S-P  use db */
			link = link_seq;
			prefix = "<a href=%sval=%s>"; 
		}
		if (link && prefix) {
			l = StringLen(link);
			ll = StringLen(prefix); 
			if (db == SEQID_PIR) {
				s = (CharPtr)MemNew(l+ ll + 20);
				sprintf(s, prefix, link, str, str);
			} else {
				s = (CharPtr)MemNew(l+ ll + 10);
				if (*(ss + StringLen(ss)  - 1) == '.') {
					*(ss + StringLen(ss)  - 1) = '\0';
				}
				sprintf(s, prefix, link, ss);
			}
			fprintf(fp, "%s<img src=http://www.ncbi.nlm.nih.gov/cgi-bin/gorf/butt?%s align=middle border=0></a>", s, str);
			MemFree(s);
		}
		MemFree(ss);
	}
	return TRUE;
}

NLM_EXTERN Boolean LIBCALL PrintSPBlock (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	Int2	l, ll;
	CharPtr	link, prefix, s;
	ValNodePtr vnp=NULL, v;
	CharPtr string, acc = NULL;
	ValNodePtr ds_vnp, tvnp;
	DescrStructPtr dsp;
	SPBlockPtr spb;
	SeqIdPtr sid;
	TextSeqIdPtr tid;
	Boolean has_link = FALSE, first;
	DbtagPtr	db;

	tvnp = GatherDescrListByChoice(ajp, gbp, Seq_descr_sp); 
	for (ds_vnp= tvnp; ds_vnp; ds_vnp=ds_vnp->next) {
		dsp = (DescrStructPtr) ds_vnp->data.ptrvalue;
		vnp = dsp->vnp;
		gbp->descr = dsp;
		spb = (SPBlockPtr)vnp->data.ptrvalue;
		MemFree(vnp);
		if (spb->_class == 1) {
			ff_AddString("class: standard.");
			NewContLine();
		} else if (spb->_class == 2) {
			ff_AddString("class: preliminary.");
			NewContLine();
		}
		if (spb->extra_acc) {
			ff_AddString("extra accessions:");
			for (v = spb->extra_acc; v; v= v->next) {
				ff_AddString((CharPtr)v->data.ptrvalue);
				ff_AddString(",");
			}
		}
		if (spb->imeth) {
			ff_AddString("seq starts with Met");
		}
		if (spb->plasnm) {
			ff_AddString("plasmid:");
			for (v = spb->plasnm; v; v= v->next) {
				ff_AddString((CharPtr)v->data.ptrvalue);
				ff_AddString(",");
			}
		}
		if (spb->created) {
			string = PrintDate(spb->created);
			ff_AddString("created: ");
			ff_AddString(string);
		}
		if (spb->sequpd) {
			string = PrintDate(spb->sequpd);
			ff_AddString("sequence updated: ");
			ff_AddString(string);
		}
		if (spb->annotupd) {
			string = PrintDate(spb->annotupd);
			ff_AddString("annotation updated: ");
			ff_AddString(string);
		}
		if (spb->seqref) {
			ff_AddString("xrefs: ");
			first = TRUE;
			for (sid = spb->seqref; sid; sid= sid->next) {
				link = link_seq;
				if (first == FALSE) {
					ff_AddString(", ");
				}
				has_link = TRUE;
				first = FALSE;
				if(sid->choice == SEQID_GI) {
					prefix = "<a href=%sval=%ld>"; 
					ff_AddString("gi: ");
					acc = (CharPtr)MemNew(10);
					sprintf(acc, "%ld", (long) sid->data.intvalue);
				} else {
					prefix = "<a href=%sval=%s>";
					tid = (TextSeqIdPtr)sid->data.ptrvalue;
					acc = tid->accession;
				}
				switch(sid->choice) {
				case SEQID_GENBANK:
					ff_AddString("genbank accession ");
					break; 
				case SEQID_EMBL:
					ff_AddString("embl accession ");
					break; 
				case SEQID_PIR:
					ff_AddString("pir locus ");
					break; 
				case SEQID_SWISSPROT:
					ff_AddString("swissprot accession ");
					break; 
				case SEQID_DDBJ:
					ff_AddString("ddbj accession ");
					break; 
				case SEQID_PRF:
					ff_AddString("prf accession ");
					break; 
				case SEQID_GI:
					ff_AddString("gi: ");
					break; 
				case SEQID_TPG:
					ff_AddString("genbank third party accession ");
					break; 
				case SEQID_TPE:
					ff_AddString("embl third party accession ");
					break; 
				case SEQID_TPD:
					ff_AddString("ddbj third party accession ");
					break; 
				default:
					acc = NULL;
					break; 
				}
				if (www && has_link) {
					if (acc && link && prefix) {
						l = StringLen(link);
						ll = StringLen(prefix); 
						if (sid->choice == SEQID_PIR) {
							s = (CharPtr)MemNew(l+ ll + 40);
							sprintf(s, prefix, link, acc, acc);
						} else if (sid->choice == SEQID_GI) {
							s = (CharPtr)MemNew(l+ ll + 10);
							sprintf(s, prefix, link, sid->data.intvalue);
						} else {
							s = (CharPtr)MemNew(l+ ll + 10);
							sprintf(s, prefix, link, acc);
						}
					}
					AddLink(s);
				/*	MemFree(s); */
					ff_AddString( acc);
					AddLink("</a>");
				} else if (acc) {
					ff_AddString( acc);
				}
			}
		}
		first = TRUE;
		for (vnp = spb->dbref; vnp; vnp=vnp->next) {
			db = (DbtagPtr)vnp->data.ptrvalue;
			has_link = FALSE;
			if (first) {
				NewContLine();
				ff_AddString("xrefs (non-sequence databases): ");
				first = FALSE;
			} else {
				ff_AddString(", ");
			}
			ff_AddString(db->db);
			if (StringCmp(db->db, "MIM") == 0) {
				prefix = "<a href=%s%s>";
				l = StringLen(link_omim);
				ll = StringLen(prefix); 
				s = (CharPtr)MemNew(l+ ll + 10);
				has_link = TRUE;
			} 
			if (db->tag && db->tag->str) {
				ff_AddString(" ");				
				if (www && has_link) {
					sprintf(s, prefix, link_omim, db->tag->str);
					AddLink(s);
					MemFree(s);
					ff_AddString(db->tag->str);
					AddLink("</a>");
				} else {				
					ff_AddString(db->tag->str);
				}
			} else if (db->tag && db->tag->id) {
				ff_AddString(" ");
				if (www && has_link) {
					sprintf(s, "<a href=%s%d>", link_omim, db->tag->id);
					AddLink(s);
					MemFree(s);
					ff_AddInteger("%d", db->tag->id);
					AddLink("</a>");
				} else {				
					ff_AddInteger("%d", db->tag->id);
				}
			}
		}
	}
	return TRUE;
}

NLM_EXTERN CharPtr LIBCALL www_featloc(CharPtr loc)
{
	Int2 i, n, k, l1, l2;
	CharPtr buf, tmp, ptr, s, eptr;
	
	if (loc == NULL) {
		return NULL;
	}
	n = StringLen(loc);
	eptr = loc + n - 1;
	for (i = 0, k = 0; i < MAX_WWWLOC; i++) {
		s = loc;
		while (s < eptr) {
			if ((ptr = StringStr(s, lim_str[i])) != NULL) {
				n += StringLen(www_lim_str[i]) - StringLen(lim_str[i]);
				k++;
				s = ptr + StringLen(lim_str[i]);
			} else {
				break;
			}
		}
	}
	if (k == 0) {
		return StringSave(loc);
	}
	buf = (CharPtr)MemNew(n + 1);
	StringCpy(buf, loc);
	for (i = 0; i < MAX_WWWLOC; i++) {
		s = buf;
		while ((ptr = StringStr(s, lim_str[i])) != NULL) {
			l1 = StringLen(www_lim_str[i]);
			l2 = StringLen(lim_str[i]);
			if (l1 != l2) {
				MemMove(ptr+l1, ptr+l2, StringLen(ptr+l2));
			}
			MemCpy(ptr, www_lim_str[i], l1);
			s = ptr + l1;
		}
	}
	
	return buf;
}

static void LitPrintGenome(SeqLitPtr slp)
{
	static Char		val[166];

	if (slp->seq_data != NULL)         /* not a gap */
	{
		if (slp->length == 0)  /* unknown length */
		{
			sprintf(val, "gap(%d)", slp->length);
			ff_AddString(val);
		} else {
/* don't know what to do here */
		}
	} else {                  /* gap length was set */
			sprintf(val, ",gap(%d)", slp->length);
			ff_AddString(val);
	}
}

static void LocPrintGenome(Asn2ffJobPtr ajp, GBEntryPtr gbp, SeqLocPtr slp_head)
{
	SeqLocPtr	slp, sslp;
	Boolean		first = TRUE;
	static Char		buf[14], val[166], temp[166];
	SeqIdPtr	sid, newid;
	Int4 		from, to, start, stop, beg, end, lcur, lprev;
	SeqIntPtr 	sint;
	BioseqPtr 	bsp = NULL, b = NULL;
	SeqEntryPtr sep = NULL;
	Int4		uid;
	Boolean		is_network, is_link = FALSE;
	Int2 p1=0, p2=0;
	DeltaSeqPtr dsp;
	static Uint1 fasta_order[NUM_SEQID] = { 
 	33, /* 0 = not set */
	20, /* 1 = local Object-id */
	15,  /* 2 = gibbsq */
	16,  /* 3 = gibbmt */
	30, /* 4 = giim Giimport-id */
	10, /* 5 = genbank */
	10, /* 6 = embl */
	10, /* 7 = pir */
	10, /* 8 = swissprot */
	15,  /* 9 = patent */
	20, /* 10 = other TextSeqId */
	20, /* 11 = general Dbtag */
	255,  /* 12 = gi */
	10, /* 13 = ddbj */
	10, /* 14 = prf */
	12, /* 15 = pdb */
        10,  /* 16 = tpg */
        10,  /* 17 = tpe */
        10   /* 18 = tpd */
    };
	
	Int2	l, ll;
	CharPtr	s, prefix;
	
#ifdef ENABLE_ENTREZ
	if ( !EntrezInit("asn2ff", FALSE, &is_network) ) {
		return;
	}
#endif
	for (slp = slp_head; slp; slp = slp->next) {
		is_link = FALSE;
		from = to = 0;
		sid = SeqLocId(slp);
		if (slp->choice == SEQLOC_INT || slp->choice == SEQLOC_WHOLE) {
			start = from = SeqLocStart(slp);
			stop = to = SeqLocStop(slp);
		} else if (slp->choice == SEQLOC_NULL){
			sprintf(val, ",%s", "gap()");
			ff_AddString(val);
			continue;
		} else {
			continue;
		}
		if (sid == NULL) {
			continue;
		}
		if (sid->choice == SEQID_GI) {
			newid = GetSeqIdForGI(sid->data.intvalue);
		} else if (sid->choice == SEQID_GENERAL) {
#ifdef ENABLE_ENTREZ
			if ((uid = GetUniGeneIDForSeqId(sid)) != 0)
				sep = EntrezSeqEntryGet(uid, -1);
			if (sep && IS_Bioseq(sep)){
			 bsp = (BioseqPtr) sep -> data.ptrvalue;
			}
			if (bsp && (bsp->seq_ext_type == 1 || bsp->seq_ext_type == 4)) {
				lcur = lprev = 0;
				for (sslp = slp_head; sslp; sslp = sslp->next) {
					is_link = FALSE;
					lprev = lcur;
					lcur += SeqLocLen(sslp);
					beg = SeqLocStart(sslp);
					end = SeqLocStop(sslp);
					sid = SeqLocId(sslp);
					if (from > lcur || to < lprev) {
						continue;
					}
					if (from > lprev) { /* first */
						start = beg + from - lprev;
					} else {
						start = beg;  /* middle */
					}
					if (to > lcur) {  /* middle */
						stop = end;
					} else {
						stop = beg + to - lprev;
					}
					if (first) {
						first = FALSE;
					} else {
						ff_AddChar(',');
					}
					if (sid->choice == SEQID_GI) {
						newid = GetSeqIdForGI(sid->data.intvalue);
						if (newid == NULL) {
							newid = sid;
						}
					} else {
						newid = sid;
					}
					if (ajp->show_version) {
					SeqIdWrite(SeqIdSelect(newid, fasta_order, NUM_SEQID), buf, PRINTID_TEXTID_ACC_VER, 13);
					} else {
					SeqIdWrite(SeqIdSelect(newid, fasta_order, NUM_SEQID), buf, PRINTID_TEXTID_ACCESSION, 10);
					}
					if (www && newid) {
						l = StringLen(link_seq);
						if (newid->choice == SEQID_GENERAL) {
							uid = GetUniGeneIDForSeqId(newid);
							if (uid != 0) {
								prefix =
								"<a href=%sval=%d>"; 
								ll = StringLen(prefix); 
								s = (CharPtr)MemNew(l+ ll + 10);
								sprintf(s, prefix, link_seq, uid);
								if (SeqLocStrand(slp) == Seq_strand_minus) {
									ff_AddString("complement(");
								}
								ff_AddString(buf);
								p1 = StringLen(buf);
								p2 = 0;
								is_link = TRUE;
							} else {
								if (SeqLocStrand(slp) == Seq_strand_minus) {
									ff_AddString("complement(");
								}
								ff_AddString(buf);
							}
						} else {
							prefix = 
							"<a href=%sval=%s>";
							ll = StringLen(prefix); 
							s = (CharPtr)MemNew(l+ ll + 10);
							sprintf(s, prefix, link_seq, buf);
							if (SeqLocStrand(slp) == Seq_strand_minus) {
								ff_AddString("complement(");
							}
							ff_AddString(buf);
							p1 = StringLen(buf);
							p2 = 0;
							is_link = TRUE;
						}
					} else {
						if (SeqLocStrand(slp) == Seq_strand_minus) {
							ff_AddString("complement(");
						}
						ff_AddString(buf);
					}
					if (SeqLocStrand(slp) == Seq_strand_minus) {
					sprintf(val,":%ld..%ld)",(long) start+1, (long) stop+1 );
					} else {
					sprintf(val,":%ld..%ld",(long) start+1, (long) stop+1 );
					}
					ff_AddString(val);
					p1 += StringLen(val);
					p2 += StringLen(val);
					if (is_link) {
						AddLinkLater(s, p1);
						MemFree(s);
						AddLinkLater("</a>", p2);
					}
				}
				continue;
			}
			newid = sid;
#else
			newid = sid;
#endif
		} else {
			newid = sid;
		}
		if (first) {
			first = FALSE;
		} else {
			ff_AddChar(',');
		}
		if (ajp->show_version) {
			SeqIdWrite(SeqIdSelect(newid, fasta_order, NUM_SEQID),
				buf, PRINTID_TEXTID_ACC_VER, 13);
		} else {
			SeqIdWrite(SeqIdSelect(newid, fasta_order, NUM_SEQID),
				buf, PRINTID_TEXTID_ACCESSION, 10);
		}
		if (www && newid) {
			l = StringLen(link_seq);
			if (newid->choice == SEQID_GENERAL) {
				uid = GetUniGeneIDForSeqId(newid);
				if (uid != 0) {
					prefix = "<a href=%sval=%ld>";
					ll = StringLen(prefix); 
					s = (CharPtr)MemNew(l+ ll + 10);
					sprintf(s, prefix, link_seq, uid);
					if (SeqLocStrand(slp) == Seq_strand_minus) {
						ff_AddString("complement(");
					}
					ff_AddString( buf);
					p1 = StringLen(buf);
					p2 = 0;
					is_link = TRUE;
				} else {
					ff_AddString( buf);
				}
			} else {
				prefix = "<a href=%sval=%s>";
				ll = StringLen(prefix); 
				s = (CharPtr)MemNew(l+ ll + 10);
				sprintf(s, prefix, link_seq, buf);
				if (SeqLocStrand(slp) == Seq_strand_minus) {
					ff_AddString("complement(");
				}
				ff_AddString( buf);
				p1 = StringLen(buf);
				p2 = 0;
				is_link = TRUE;
			} 
		} else {
			if (SeqLocStrand(slp) == Seq_strand_minus) {
				ff_AddString("complement(");
			}
			ff_AddString( buf);
		}
		if (SeqLocStrand(slp) == Seq_strand_minus) {
			sprintf(val,":%ld..%ld)",(long) start+1, (long) stop+1 );
		} else {
			sprintf(val,":%ld..%ld",(long) start+1, (long) stop+1 );
		}
		ff_AddString(val);
		p1 += StringLen(val);
		p2 += StringLen(val);
		if (is_link) {
			AddLinkLater(s, p1);
			MemFree(s);
			AddLinkLater("</a>", p2);
		}
	}
}

void PrintGenome(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	SeqLocPtr	slp_head=NULL;
	Boolean		first = TRUE;
	Boolean		is_network;
	DeltaSeqPtr dsp;
	SeqLitPtr 	litp;
	
	Int2	l, ll;
	CharPtr	s, prefix;
	
#ifdef ENABLE_ENTREZ
	if ( !EntrezInit("asn2ff", FALSE, &is_network) ) {
		return;
	}
#endif
	ff_StartPrint(0, 12, ASN2FF_GB_MAX, NULL);
	if (www && ajp->format == GRAPHIK_FMT) {
		if (gbp->bsp && gbp->bsp->seq_ext == NULL) {
			return;
		}
		ff_AddString("<b>CONTIG</b>&nbsp;&nbsp;&nbsp;");
	} else {
		ff_AddString("CONTIG");
	}
	TabToColumn(13);
	ff_AddString("join(");
	if (gbp->bsp->seq_ext_type == 1) {
		slp_head = (SeqLocPtr) gbp->bsp->seq_ext;
		LocPrintGenome(ajp, gbp, slp_head);
	} else if (gbp->bsp->seq_ext_type == 4) {
		for (dsp = (DeltaSeqPtr) gbp->bsp->seq_ext; dsp; dsp=dsp->next) {
			if (dsp->choice == 1) {  /* SeqLoc */
				slp_head = (SeqLocPtr)(dsp->data.ptrvalue);
				if (first == FALSE) {
					ff_AddString(",");
				}
				LocPrintGenome(ajp, gbp, slp_head);
			} else {
				litp = (SeqLitPtr)(dsp->data.ptrvalue);
				if (litp == NULL) continue;
				LitPrintGenome(litp);
			}
			if (first)
				first = FALSE;
		}
	}
	ff_AddChar(')');
	ff_EndPrint();
	/*PrintTerminator();*/

}

NLM_EXTERN void LIBCALL www_accession (CharPtr string)
{
	Int2	l, ll;
	CharPtr	s, prefix=NULL, p, link=NULL;

	if (string == NULL) {
		return;
	}
	if (!www) {
		ff_AddString(string);
	} else {
			link = link_seq;
			prefix = "<a href=%sval=%s>";
			s = (CharPtr)MemNew(StringLen(link_seq)+ StringLen(prefix) + StringLen(string) + 10);
			sprintf(s, prefix, link, string);
			AddLink(s);
			MemFree(s);
			ff_AddString(string);
			AddLink("</a>");
	}
	return;
}
static Boolean iscospa(Char c)
{
	return (isspace(c) || c == ','  || c == ' ') ;
}


NLM_EXTERN void LIBCALL www_PrintComment (CharPtr string, Boolean identifier, Uint1 format)
{
	Int2	lpref, l, ll;
	Int4	gi;
	CharPtr	s, prefix=NULL, p, pp, link=NULL, www_str, acc, ss;
	Boolean isfirst = TRUE;
	
	if (string == NULL) {
		return;
	}
	if (format == EMBL_FMT || format == PSEUDOEMBL_FMT ||
		format == EMBLPEPT_FMT) {
		if (identifier == TRUE)
			PrintXX();
		ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "CC");
	} else {
		ff_StartPrint(0, 12, ASN2FF_GB_MAX, NULL);
		if (identifier == TRUE) {
			if (format == GRAPHIK_FMT && www) {
				ff_AddString("<BR><b>COMMENT</b>&nbsp;&nbsp;&nbsp;");
			} else {
				ff_AddString("COMMENT");
			}
		}
		TabToColumn(13);
	}
	if (!www) {
		ff_AddStringWithTildes(string);
		ff_EndPrint();
		return;
	} 
	if ((p = StringStr(string, "REFSEQ")) != NULL) {
		acc = TextSave(string, p-string);
		ff_AddString(acc);
		MemFree(acc);
		p += 7;
		AddLink("<a href=http://www.ncbi.nlm.nih.gov/LocusLink/refseq.html>");
		ff_AddString("REFSEQ:");
		AddLink("</a>");
		string = p;
		if ((p = StringStr(string, "was derived from ")) != NULL) {
			p += StringLen("was derived from ");
			s = TextSave(string, p-string);
			ff_AddString(s);
			MemFree(s);
			prefix = "<a href=%sval=%s>";
			lpref = StringLen(link_seq)+ StringLen(prefix);
			for (; isspace(*p); p++) ;
			isfirst = TRUE;
			do {
				if (isfirst) {
					isfirst = FALSE;
				} else {
					ff_AddString(", ");
				}
				for (pp=p; *pp != '\0' && !iscospa(*pp); pp++) ;
				acc = TextSave(p, pp-p);
				if (acc[StringLen(acc)-1] == '.') {
					acc[StringLen(acc)-1] = '\0';
				}
				s = (CharPtr)MemNew(lpref + StringLen(acc)+1);
				sprintf(s, prefix, link_seq, acc);
				AddLink(s);
				MemFree(s);
				ff_AddString(acc);
				MemFree(acc);
				AddLink("</a>");
				for (p = pp; iscospa(*p); p++) ;
			} while (*p != '\0');
			ff_AddString(".");
		} else {
			ff_AddString(string);
		}
		ff_EndPrint();
		return;
	}
	if (( p = StringStr(string, "AceView:")) != NULL) {
		/*	p += StringLen("AceView:");*/
			gi = atoi(p + StringLen("AceView:"));
			l = StringLen(link_ace) + StringLen(p);
			prefix = "<a href=%sl=%ld>"; 
			ll = StringLen(prefix); 
			s = (CharPtr)MemNew(l + ll);
			ss = (CharPtr)MemNew(p-string+1);
			StringNCpy(ss, string, p-string);
			ff_AddString(ss);
			MemFree(ss);
			sprintf(s, prefix, link_ace, gi);
			AddLink(s);
			MemFree(s);
			ff_AddString("AceView");
		/*	ff_AddInteger("%ld", (long) gi);*/
			AddLink("</a>");
		ff_EndPrint();
		return;
	}
	while ((p = StringStr(string, "gi:")) != NULL) {
		p += 3;
		s = TextSave(string, p-string);
		ff_AddString(s);
		gi = atoi(p);
		link = link_seq;
		prefix = "<a href=%sval=%d>";
		s = (CharPtr)MemNew(StringLen(link_seq)+ StringLen(prefix) + 12);
		sprintf(s, prefix, link, gi);
		AddLink(s);
		MemFree(s);
		ff_AddInteger("%d", gi);
		AddLink("</a>");
		while (IS_DIGIT(*p)) 
			p++;
		string = p;
	}
	www_str = www_featloc(string);
	ff_AddStringWithTildes(www_str);
	MemFree(www_str);
	ff_EndPrint();
	return;
}	/* PrintComment */


