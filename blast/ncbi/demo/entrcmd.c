/*   entrcmd.c
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
* File Name:  entrcmd.c
*
* Author:  Epstein
*
* Version Creation Date:   1/4/94
*
* $Revision: 6.4 $
*
* File Description: 
*       non-interactive command line interface for Entrez
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: entrcmd.c,v $
* Revision 6.4  1999/08/11 18:58:09  kans
* changed FindNuc and FindProt to avoid collision with sequtil functions
*
* Revision 6.3  1998/08/24 20:43:42  kans
* fixed -v -fd warnings
*
* Revision 6.2  1997/12/10 13:48:44  kans
* removed call to SeqEntryToFile
*
* Revision 6.1  1997/11/04 21:02:44  epstein
* change stray stdout to master_fp
*
* Revision 6.0  1997/08/25 18:19:34  madden
* Revision changed to 6.0
*
* Revision 5.7  1997/07/21 16:12:39  epstein
* CONVERT to new format for exported list of identifiers
*
* Revision 5.6  1997/07/14 18:24:20  epstein
* add complexity argument 'y'
*
* Revision 5.5  1997/03/21 18:41:46  epstein
* retrieve correct genome sequences
*
 * Revision 5.4  1997/03/10  19:33:45  epstein
 * add Genomes support
 *
 * Revision 5.3  1996/10/24  15:49:27  epstein
 * add -r option to fetch entries from ID
 *
 * Revision 5.2  1996/06/11  15:16:54  epstein
 * remove another artificial 32K boundary
 *
 * Revision 5.1  1996/05/31  19:27:46  epstein
 * eradicate 32K UID limitations, as much as possible
 *
 * Revision 4.4  1996/03/19  17:08:41  epstein
 * remove stray printfs
 *
 * Revision 4.3  1996/02/21  22:09:16  epstein
 * add EntrezBioseqFetchEnable/Disable() to fix GBFF outputs
 *
 * Revision 4.2  1995/09/18  18:18:52  epstein
 * add GenPept format
 *
 * Revision 4.1  1995/08/21  19:41:14  epstein
 * add cluster analysis
 *
 * Revision 4.0  1995/07/26  13:54:26  ostell
 * force revision to 4.0
 *
 * Revision 1.30  1995/07/20  18:58:15  epstein
 * use new SeqIdWrite function
 *
 * Revision 1.29  1995/06/19  21:42:11  kans
 * changed asn2ff_entrez to SeqEntryToFlat
 *
 * Revision 1.28  1995/05/15  01:29:58  ostell
 * added newline to end of file
 *
 * Revision 1.27  1995/05/15  01:28:44  ostell
 * Fixed Callbacks prototypes to SeqEntryExplore
 *
*
* ==========================================================================
*/

#include <ncbi.h>
#include <accentr.h>
#include <accutils.h>
#include <tofasta.h>
#include <tomedlin.h>
#include <asn2ff.h>

Args myargs[] = {
        {"Initial database", "m",NULL, NULL, TRUE,'d',ARG_STRING,0.0,0,NULL},
        {"Boolean expression", NULL, NULL, NULL, TRUE, 'e', ARG_STRING, 0.0,0,NULL},
        {"Comma-delimited list of UIDs", NULL, NULL, NULL, TRUE, 'u', ARG_STRING, 0.0,0,NULL},
        {"Program of commands", NULL, NULL, NULL, FALSE, 'p', ARG_STRING, 0.0,0,NULL},
        {"Display status report", "F", NULL, NULL, TRUE, 's', ARG_BOOLEAN, 0.0,0,NULL},
        {"Produce WWW/HTML formatted output (recommended value is /htbin)", NULL, NULL, NULL, TRUE, 'w', ARG_STRING, 0.0,0,NULL},
        {"Detailed help", "F", NULL, NULL, TRUE, 'h', ARG_BOOLEAN, 0.0,0,NULL},
        {"For WWW output, use Forms", "F", NULL, NULL, TRUE, 'f', ARG_BOOLEAN, 0.0,0,NULL},
        {"'Check' WWW output Forms", "F", NULL, NULL, TRUE, 'c', ARG_BOOLEAN, 0.0,0,NULL},
        {"Name of export file for named UID list", NULL, NULL, NULL, TRUE, 'x', ARG_STRING,0.0,0,NULL},
        {"Comma-delimited list of files to import for named UID list", NULL, NULL, NULL, TRUE, 'i', ARG_STRING,0.0,0,NULL},
        {"Produce a list of terms (term)", NULL, NULL, NULL, TRUE, 't', ARG_STRING, 0.0,0,NULL},
        {"Taxonomy lookup", NULL, NULL, NULL, TRUE, 'l', ARG_STRING, 0.0,0,NULL},
        {"On-the-fly neighboring", NULL, NULL, NULL, TRUE, 'n', ARG_FILE_IN, 0.0,0,NULL},
        {"Output file", "stdout", NULL, NULL, FALSE, 'o', ARG_FILE_OUT, 0.0,0,NULL},
        {"Use WWW-style encoding for special input characters", "T", NULL, NULL, TRUE, 'g', ARG_BOOLEAN, 0.0,0,NULL},
        {"Get sequences from ID Repository", "F", NULL, NULL, TRUE, 'r', ARG_BOOLEAN, 0.0,0,NULL},
        {"Complexity (1=bioseq only, 2=bioseq set, 3=nuc-prot set)", "3", NULL, NULL, TRUE, 'y', ARG_INT, 0.0,0,NULL}
};

#define ENTREZ_FLD_MNEMONIC_LENGTH 4
#define DEFAULT_TERMLIST_LEN      40

#define DISPLAY_SPECIAL_AND_TOTAL 1
#define DISPLAY_TOTAL_ONLY        2
#define DISPLAY_TERM_ONLY         3

typedef struct savlist {
    CharPtr name;
    Int4Ptr uids;
    DocType db;
    Int2 num;
} SavList, PNTR SavListPtr;

typedef struct term_and_counts {
    CharPtr term;
    Int4 special;
    Int4 total;
} TermAndCounts, PNTR TermAndCountsPtr;

typedef struct {
    Uint4 num;
    DocUidPtr uids;
    Int4Ptr weights;
} * LocalLinkSetPtr;
    

static CharPtr wwwPrefix = NULL;
static CharPtr theTerm = NULL;
static Boolean useForms = FALSE;
static Boolean checkForms = FALSE;
static Int2    numTerms;
static Int2    termsBefore;
static FILE *  exportFilePtr = NULL;
static LocalLinkSetPtr pubLsp = NULL;
static Int2    termDisplay = DISPLAY_TERM_ONLY;
static FILE *  master_fp = NULL;
static Int2    seqEntryRetval = 3;

static LocalLinkSetPtr LocalLinkSetNew(void)
{
    LocalLinkSetPtr lsp;

    lsp = MemNew(sizeof(*lsp));
    lsp->num = 0;
    lsp->uids = NULL;
    lsp->weights = NULL;

    return lsp;
}

static LocalLinkSetPtr LocalLinkSetFree(LocalLinkSetPtr lsp)
{
    MemFree(lsp->uids);
    MemFree(lsp->weights);
    MemFree(lsp);

    return NULL;
}

static LocalLinkSetPtr LinkSetToLocalLinkSet(LinkSetPtr newlsp)
{
    LocalLinkSetPtr lsp;

    if (newlsp != NULL)
    {
        lsp = LocalLinkSetNew();
        lsp->num = (Uint4) newlsp->num;
        lsp->uids = (DocUidPtr) MemDup(newlsp->uids, sizeof(DocUid) * lsp->num);
        lsp->weights = (DocUidPtr) MemDup(newlsp->weights, sizeof(Int4) * lsp->num);
    }
    return lsp;
}

static void
DoOutput(CharPtr term, Int2 depth, Boolean showTerminal)
{
    Int2 i;

    for (i = 1; i <= depth; i++)
        fprintf(master_fp, i < depth ? "-" : ( showTerminal ? "*" : "-"));
    fprintf (master_fp, "%s\n", term);
}

static void
PreOrderTaxTraversal(EntrezHierarchyPtr ehp, Int2 depth, DocType db, DocField fld, Int2 maxDepth)
{
    Int2 i;
    EntrezHierarchyPtr child;

    DoOutput(ehp->term, depth, depth >= maxDepth);

    if (depth >= maxDepth)
        return;

    for (i = 0; i < ehp->numChildren; i++)
    {
        if (ehp->children[i].isLeafNode)
        { /* no need to move down tree, since all information is here */
            DoOutput(ehp->children[i].name, depth + 1, FALSE);
        } else {
            child = EntrezHierarchyGet(ehp->children[i].name, db,
                                       fld);
            if (child != NULL)
            {
                PreOrderTaxTraversal(child, depth + 1, db, fld, maxDepth);
                EntrezHierarchyFree(child);
            }
        }
    }
}
    

/* find the last nucleotide bioseq in the bioseqset */
static void FindANuc(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
  BioseqPtr PNTR bp;
  BioseqPtr local_bsp;

  bp = (BioseqPtr PNTR) data;
  if (IS_Bioseq(sep))
  {
    local_bsp = (BioseqPtr) sep->data.ptrvalue;
    if (ISA_na(local_bsp->mol))
      *bp = local_bsp;
  }
}

/* find the last protein bioseq in the bioseqset */
static void FindAProt(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
  BioseqPtr PNTR bp;
  BioseqPtr local_bsp;

  bp = (BioseqPtr PNTR) data;
  if (IS_Bioseq(sep))
  {
    local_bsp = (BioseqPtr) sep->data.ptrvalue;
    if (ISA_aa(local_bsp->mol))
      *bp = local_bsp;
  }
}

static Boolean IsGenBank (SeqEntryPtr sep)
{
    BioseqPtr  bsp;
    Uint1      repr;
    Boolean    rsult;

    rsult = FALSE;
    if (sep->choice == 1) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        repr = Bioseq_repr (bsp);
        if (repr == Seq_repr_raw || repr == Seq_repr_const) {
            if (ISA_na (bsp->mol)) {
                rsult = TRUE;
            } else {
                Message (MSG_ERROR, "Protein record cannot be viewed in GenBank form.");
            }
        } else {
            Message (MSG_ERROR, "Bad sequence repr %d", (int) repr);
        }
    } else {
        rsult = TRUE;
    }
    return rsult;
}


static void
PrintGenbank(SeqEntryPtr sep, Boolean isprot)
{
    SeqEntryToFlat(sep, master_fp, isprot ? GENPEPT_FMT : GENBANK_FMT,
                   RELEASE_MODE);

    FilePuts ("\n\n", master_fp);
}

static Boolean PrintDSP(DocSumPtr dsp, DocUid uid)
{

    Int2 titleLen;
    Int2 size;
    CharPtr pEnd;
    CharPtr pStart;
    
    if (dsp == NULL)
        return TRUE;

    pStart = dsp->title;
    titleLen = StrLen(pStart);
    
    if ( titleLen <= 55 ) {
      size = titleLen;
    } else {
        pEnd = pStart + 55;
        while (IS_WHITESP(*pEnd) == FALSE)
            pEnd -=1;
        size = pEnd - pStart;
    }

    fprintf (master_fp, "%-20.20s %-*.*s\n",dsp->caption,size,size,pStart);
    fprintf (master_fp, "%c",(dsp->no_abstract ? ' ' : '*'));
    fprintf (master_fp, "                    ");

    pStart = pStart + size;
  
    while ((titleLen = StrLen(pStart)) > 0 ) {
        if ( titleLen <= 55 ) {
            size = titleLen;
        } else {
            pEnd = pStart + 55;
            while (IS_WHITESP(*pEnd) == FALSE)
                pEnd -=1;
            size = pEnd - pStart;
        }

        fprintf (master_fp, "%-*.*s\n",size,size,pStart+1);
        fprintf (master_fp, "%-21.21s","");
        pStart = pStart + size;
    }

    fprintf (master_fp, "\n");

    DocSumFree(dsp);
    return TRUE;
}

static Boolean PrintDSPMwww(DocSumPtr dsp, DocUid uid)
{
    CharPtr p;
    Boolean noNeighbors = FALSE;
    LocalLinkSetPtr lsp;
    Int2 medNeighbors;
    Int2 protNeighbors;
    Int2 nucNeighbors;
    
    if (dsp == NULL)
        return TRUE;

    fprintf (master_fp, "<DL>\n<DT>\n");
    if (useForms)
    {
        fprintf (master_fp, "<inPUT TYPE=\"checkbox\" NAME=\"nei\" VALUE=\"%d\"%s>\n", uid,
                checkForms ? " CHECKED" : "");
    }
    if (TRUE /* used to be non-FORMS only */ )
    {
        LinkSetPtr lsp;

        lsp = NULL;
        EntrezLinkUidList(&lsp, TYP_ML, TYP_ML, 1, &uid, FALSE);
        if (lsp != NULL)
        {
            medNeighbors = lsp->num;
            LinkSetFree(lsp);
            lsp = NULL;
        } else {
            medNeighbors = 0;
        }
        EntrezLinkUidList(&lsp, TYP_ML, TYP_AA, 1, &uid, FALSE);
        if (lsp != NULL)
        {
            protNeighbors = lsp->num;
            LinkSetFree(lsp);
            lsp = NULL;
        } else {
            protNeighbors = 0;
        }
        EntrezLinkUidList(&lsp, TYP_ML, TYP_NT, 1, &uid, FALSE);
        if (lsp != NULL)
        {
            nucNeighbors = lsp->num;
            LinkSetFree(lsp);
            lsp = NULL;
        } else {
            nucNeighbors = 0;
        }
        noNeighbors = !medNeighbors && !protNeighbors && !nucNeighbors;
    }

    fprintf (master_fp, "%s\n<BR><DD>\n", dsp->caption);
    for (p = dsp->title; *p; p++)
    {
        switch (*p)
        {
        case '&': fprintf (master_fp, "&amp;"); break;
        case '<': fprintf (master_fp, "&lt;"); break;
        case '>': fprintf (master_fp, "&gt;"); break;
        default: fprintf (master_fp, "%c", *p);
        }
    }
    fprintf (master_fp, "<I>");
    if (dsp->no_abstract)
    {
        fprintf (master_fp, " (no abstract available)");
    }
    fprintf (master_fp, " (View ");
    fprintf (master_fp, "<A HREF=\"%s/entrezmr?%d\">Report format</A>,\n", wwwPrefix, uid);
    fprintf (master_fp, "<A HREF=\"%s/entrezml?%d\">MEDLARS format</A>,\n", wwwPrefix, uid);
    if (noNeighbors)
    {
        fprintf (master_fp, "or ");
    }
    fprintf (master_fp, "<A HREF=\"%s/entrezma?%d\">ASN.1 format</A>", wwwPrefix, uid);
    if (! noNeighbors)
    {
        if (medNeighbors != 0)
            fprintf (master_fp, ", %s<A HREF=\"%s/entrezmmnei?%d\">%d MEDLINE neighbor%s</A>\n", !protNeighbors && !nucNeighbors ? "or " : "", wwwPrefix, uid, medNeighbors, medNeighbors == 1 ? "" : "s");
        if (protNeighbors != 0)
            fprintf (master_fp, ", %s<A HREF=\"%s/entrezmpnei?%d\">%d Protein link%s</A>\n", !nucNeighbors ? "or " : "", wwwPrefix, uid, protNeighbors, protNeighbors == 1 ? "" : "s");
        if (nucNeighbors != 0)
            fprintf (master_fp, ", or <A HREF=\"%s/entrezmnnei?%d\">%d Nucleotide link%s</A>\n", wwwPrefix, uid, nucNeighbors, nucNeighbors == 1 ? "" : "s");
    }
    fprintf (master_fp, ")\n<P></I></DL>\n");
    
    DocSumFree(dsp);
    return TRUE;
}

static Boolean PrintDSPNwww(DocSumPtr dsp, DocUid uid)
{
    CharPtr p;
    Boolean noNeighbors = FALSE;
    LocalLinkSetPtr lsp;
    Int2 medNeighbors;
    Int2 protNeighbors;
    Int2 nucNeighbors;
    Int4 weight;
    Int2 i;
    
    if (dsp == NULL)
        return TRUE;

    fprintf (master_fp, "<DL>\n<DT>\n");
    if (useForms)
    {
        fprintf (master_fp, "<inPUT TYPE=\"checkbox\" NAME=\"nei\" VALUE=\"%d\"%s>\n", uid,
                checkForms ? " CHECKED" : "");
    }
    if (TRUE /* used to be non-FORMS only */ )
    {
        LinkSetPtr lsp;

        lsp = NULL;
        EntrezLinkUidList(&lsp, TYP_NT, TYP_ML, 1, &uid, FALSE);
        if (lsp != NULL)
        {
            medNeighbors = lsp->num;
            LinkSetFree(lsp);
            lsp = NULL;
        } else {
            medNeighbors = 0;
        }
        EntrezLinkUidList(&lsp, TYP_NT, TYP_AA, 1, &uid, FALSE);
        if (lsp != NULL)
        {
            protNeighbors = lsp->num;
            LinkSetFree(lsp);
            lsp = NULL;
        } else {
            protNeighbors = 0;
        }
        EntrezLinkUidList(&lsp, TYP_NT, TYP_NT, 1, &uid, FALSE);
        if (lsp != NULL)
        {
            nucNeighbors = lsp->num;
            LinkSetFree(lsp);
            lsp = NULL;
        } else {
            nucNeighbors = 0;
        }
        noNeighbors = !medNeighbors && !protNeighbors && !nucNeighbors;
    }

    fprintf (master_fp, "%s\n<BR><DD>\n", dsp->caption);
    for (p = dsp->title; *p; p++)
    {
        switch (*p)
        {
        case '&': fprintf (master_fp, "&amp;"); break;
        case '<': fprintf (master_fp, "&lt;"); break;
        case '>': fprintf (master_fp, "&gt;"); break;
        default: fprintf (master_fp, "%c", *p);
        }
    }
    fprintf (master_fp, "<I>");
    weight = -1;
    if (pubLsp != NULL && pubLsp->weights != NULL)
    {
        for (i = 0; i < pubLsp->num; i++)
        {
            if (pubLsp->uids[i] == uid)
                weight = pubLsp->weights[i];
        }
    }
    if (weight > 1)
    {
        fprintf (master_fp, " (Similarity score %d)", weight);
    }
    fprintf (master_fp, " (View ");
    fprintf (master_fp, "<A HREF=\"%s/entreznr?%d\">Report format</A>,\n", wwwPrefix, uid);
    fprintf (master_fp, "<A HREF=\"%s/entrezng?%d\">GenBank format</A>,\n", wwwPrefix, uid);
    fprintf (master_fp, "<A HREF=\"%s/entreznf?%d\">FASTA format</A>,\n", wwwPrefix, uid);
    if (noNeighbors)
    {
        fprintf (master_fp, "or ");
    }
    fprintf (master_fp, "<A HREF=\"%s/entrezna?%d\">ASN.1 format</A>", wwwPrefix, uid);
    if (! noNeighbors)
    {
        if (medNeighbors != 0)
            fprintf (master_fp, ", %s<A HREF=\"%s/entreznmnei?%d\">%d MEDLINE link%s</A>\n", !protNeighbors && !nucNeighbors ? "or " : "", wwwPrefix, uid, medNeighbors, medNeighbors == 1 ? "" : "s");
        if (protNeighbors != 0)
            fprintf (master_fp, ", %s<A HREF=\"%s/entreznpnei?%d\">%d Protein link%s</A>\n", !nucNeighbors ? "or " : "", wwwPrefix, uid, protNeighbors, protNeighbors == 1 ? "" : "s");
        if (nucNeighbors != 0)
            fprintf (master_fp, ", or <A HREF=\"%s/entreznnnei?%d\">%d Nucleotide neighbor%s</A>\n", wwwPrefix, uid, nucNeighbors, nucNeighbors == 1 ? "" : "s");
    }
    fprintf (master_fp, ")<P></I></DL>\n");
    
    DocSumFree(dsp);
    return TRUE;
}

static Boolean PrintDSPPwww(DocSumPtr dsp, DocUid uid)
{
    CharPtr p;
    Boolean noNeighbors = FALSE;
    LocalLinkSetPtr lsp;
    Int2 medNeighbors;
    Int2 protNeighbors;
    Int2 nucNeighbors;
    Int4 weight;
    Int2 i;
    
    if (dsp == NULL)
        return TRUE;

    fprintf (master_fp, "<DL>\n<DT>\n");
    if (useForms)
    {
        fprintf (master_fp, "<inPUT TYPE=\"checkbox\" NAME=\"nei\" VALUE=\"%d\"%s>\n", uid,
                checkForms ? " CHECKED" : "");
    }
    if (TRUE /* used to be non-FORMS only */ )
    {
        LinkSetPtr lsp;

        lsp = NULL;
        EntrezLinkUidList(&lsp, TYP_AA, TYP_ML, 1, &uid, FALSE);
        if (lsp != NULL)
        {
            medNeighbors = lsp->num;
            LinkSetFree(lsp);
            lsp = NULL;
        } else {
            medNeighbors = 0;
        }
        EntrezLinkUidList(&lsp, TYP_AA, TYP_AA, 1, &uid, FALSE);
        if (lsp != NULL)
        {
            protNeighbors = lsp->num;
            LinkSetFree(lsp);
            lsp = NULL;
        } else {
            protNeighbors = 0;
        }
        EntrezLinkUidList(&lsp, TYP_AA, TYP_NT, 1, &uid, FALSE);
        if (lsp != NULL)
        {
            nucNeighbors = lsp->num;
            LinkSetFree(lsp);
            lsp = NULL;
        } else {
            nucNeighbors = 0;
        }
        noNeighbors = !medNeighbors && !protNeighbors && !nucNeighbors;
    }

    fprintf (master_fp, "%s\n<BR><DD>\n", dsp->caption);
    for (p = dsp->title; *p; p++)
    {
        switch (*p)
        {
        case '&': fprintf (master_fp, "&amp;"); break;
        case '<': fprintf (master_fp, "&lt;"); break;
        case '>': fprintf (master_fp, "&gt;"); break;
        default: fprintf (master_fp, "%c", *p);
        }
    }
    fprintf (master_fp, "<I>");
    weight = -1;
    if (pubLsp != NULL && pubLsp->weights != NULL)
    {
        for (i = 0; i < pubLsp->num; i++)
        {
            if (pubLsp->uids[i] == uid)
                weight = pubLsp->weights[i];
        }
    }
    if (weight > 1)
    {
        fprintf (master_fp, " (Similarity score %d)", weight);
    }
    fprintf (master_fp, " (View ");
    fprintf (master_fp, "<A HREF=\"%s/entrezpr?%d\">Report format</A>,\n", wwwPrefix, uid);
    fprintf (master_fp, "<A HREF=\"%s/entrezpf?%d\">FASTA format</A>,\n", wwwPrefix, uid);
    if (noNeighbors)
    {
        fprintf (master_fp, "or ");
    }
    fprintf (master_fp, "<A HREF=\"%s/entrezpa?%d\">ASN.1 format</A>", wwwPrefix, uid);
    if (! noNeighbors)
    {
        if (medNeighbors != 0)
            fprintf (master_fp, ", %s<A HREF=\"%s/entrezpmnei?%d\">%d MEDLINE link%s</A>\n", !protNeighbors && !nucNeighbors ? "or " : "", wwwPrefix, uid, medNeighbors, medNeighbors == 1 ? "" : "s");
        if (protNeighbors != 0)
            fprintf (master_fp, ", %s<A HREF=\"%s/entrezppnei?%d\">%d Protein neighbor%s</A>\n", !nucNeighbors ? "or " : "", wwwPrefix, uid, protNeighbors, protNeighbors == 1 ? "" : "s");
        if (nucNeighbors != 0)
            fprintf (master_fp, ", or <A HREF=\"%s/entrezpnnei?%d\">%d Nucleotide link%s</A>\n", wwwPrefix, uid, nucNeighbors, nucNeighbors == 1 ? "" : "s");
    }
    fprintf (master_fp, ")<P></I></DL>\n");
    
    DocSumFree(dsp);
    return TRUE;
}

static void
ReportBadType (DocType db, CharPtr outputSpec)
{
    Message(MSG_POST, "Invalid output format \"%s\" for database \"%s\"",
            outputSpec, db == TYP_ML ? "MEDLINE" : (db == TYP_NT ?
            "Nucleotide" : (db == TYP_AA ? "Protein" : (db == TYP_CH ?
            "Genome" : "unknown"))));
}

static Boolean
ProcessOutput(LocalLinkSetPtr lsp, DocType db, CharPtr outputSpec, long processingCount, long totalCount, Boolean parseOnly)
{
    long i;
    AsnIoPtr aip;
    MedlineEntryPtr mep;
    SeqEntryPtr sep;
    SeqIdPtr sip;
    Char seqIdBuf[256];

    if (StringCmp(outputSpec, "") == 0 || StringCmp(outputSpec, "no") == 0)
        return TRUE;
    if (StringCmp(outputSpec, "mc") == 0)
    {
        if (db != TYP_ML)
        {
            ReportBadType(db, outputSpec);
            return FALSE;
        }
        if (! parseOnly)
        {
           fprintf (master_fp, "%ld\n", totalCount);
        }
        return TRUE;
    }
    if (StringCmp(outputSpec, "mu") == 0)
    {
        if (db != TYP_ML)
        {
            ReportBadType(db, outputSpec);
            return FALSE;
        }
        if (! parseOnly && lsp != NULL)
        {
            if (exportFilePtr != NULL)
            {
	        CharPtr str = "garbage";
		
		switch(db) {
		case TYP_ML: str = "MEDLINE"; break;
		case TYP_AA: str = "protein"; break;
		case TYP_NT: str = "nucleotide"; break;
		case TYP_CH: str = "genome"; break;
		}
                fprintf(exportFilePtr, ">%s\n", str);		  
		  
                for (i = 0; i < processingCount; i++)
                {
                    fprintf(exportFilePtr, "%ld\n", lsp->uids[i]);
                }
                FileClose(exportFilePtr);
                exportFilePtr = NULL;
            } else {
                fprintf (master_fp, "\n");
                for (i = 0; i < processingCount; i++)
                {
                    fprintf (master_fp, "%ld\n", lsp->uids[i]);
                }
                fprintf (master_fp, "\n");
                fflush(master_fp);
            }
        }
        return TRUE;
    }
    if (StringCmp(outputSpec, "mz") == 0)
    { /* analyze */
        if (db != TYP_ML)
        {
            ReportBadType(db, outputSpec);
            return FALSE;
        }
        if (! parseOnly && lsp != NULL)
        {
            CharPtr terms[20];
            Int4 termTotals[20];
            Int4 count;

            count = EntrezClusterAnalysis(lsp->uids, lsp->num, FLD_WORD, 0, INT2_MAX, 20, terms, termTotals);
            fprintf (master_fp, "Analysis resulted in %d terms\n\n", (int) count);
            for (i = 0; i < count; i++)
            {
                fprintf (master_fp, "%s %ld\n", terms[i], (long) termTotals[i]);
                MemFree (terms[i]);
            }
        }
        return TRUE;
    }
    if (StringCmp(outputSpec, "md") == 0)
    {
        if (db != TYP_ML)
        {
            ReportBadType(db, outputSpec);
            return FALSE;
        }
        if (! parseOnly && lsp != NULL)
        {
            if (wwwPrefix != NULL && processingCount < totalCount)
            {
                fprintf (master_fp, "Warning: only %ld document summaries are being displayed\n", processingCount);
                fprintf (master_fp, "out of %d total entries.<P>\n", totalCount);
            }
            EntrezDocSumListGet((Int2) processingCount, db, lsp->uids,
                                wwwPrefix == NULL ? PrintDSP : PrintDSPMwww);
        }
        return TRUE;
    }
    if (StringCmp(outputSpec, "mr") == 0)
    {
        if (db != TYP_ML)
        {
            ReportBadType(db, outputSpec);
            return FALSE;
        }
        if (! parseOnly && lsp != NULL)
        {
            for (i = 0; i < processingCount; i++)
            {
                mep = EntrezMedlineEntryGet(lsp->uids[i]);
                if (mep != NULL)
                {
                    MedlineEntryToDocFile(mep, master_fp);
                    MedlineEntryFree(mep);
                    fprintf (master_fp, "\n\n");
                }
            }
            fflush(master_fp);
        }
        return TRUE;
    }
    if (StringCmp(outputSpec, "ma") == 0)
    {
        if (db != TYP_ML)
        {
            ReportBadType(db, outputSpec);
            return FALSE;
        }
        if (! parseOnly && lsp != NULL)
        {
            aip = AsnIoNew(ASNIO_TEXT_OUT, master_fp, NULL, NULL, NULL);
            for (i = 0; i < processingCount; i++)
            {
                mep = EntrezMedlineEntryGet(lsp->uids[i]);
                if (mep != NULL)
                {
                    MedlineEntryAsnWrite(mep, aip, NULL);
                    AsnIoReset(aip);
                    MedlineEntryFree(mep);
                }
            }
            AsnIoClose(aip);
        }
        return TRUE;
    }
    if (StringCmp(outputSpec, "ml") == 0)
    {
        if (db != TYP_ML)
        {
            ReportBadType(db, outputSpec);
            return FALSE;
        }
        if (! parseOnly && lsp != NULL)
        {
            for (i = 0; i < processingCount; i++)
            {
                mep = EntrezMedlineEntryGet(lsp->uids[i]);
                if (mep != NULL)
                {
                    MedlineEntryToDataFile(mep, master_fp);
                    fprintf (master_fp, "\n");
                    MedlineEntryFree(mep);
                }
            }
        }
        return TRUE;
    }


    if (StringCmp(outputSpec, "sc") == 0)
    {
        if (db != TYP_NT && db != TYP_AA && db != TYP_CH)
        {
            ReportBadType(db, outputSpec);
            return FALSE;
        }
        if (! parseOnly)
        {
            fprintf (master_fp, "%ld\n", totalCount);
        }
        return TRUE;
    }
    if (StringCmp(outputSpec, "su") == 0)
    {
        if (db != TYP_NT && db != TYP_AA && db != TYP_CH)
        {
            ReportBadType(db, outputSpec);
            return FALSE;
        }
        if (! parseOnly && lsp != NULL)
        {
            if (exportFilePtr != NULL)
            {
	        CharPtr str = "garbage";
		
		switch(db) {
		case TYP_ML: str = "MEDLINE"; break;
		case TYP_AA: str = "protein"; break;
		case TYP_NT: str = "nucleotide"; break;
		case TYP_CH: str = "genome"; break;
		}
                fprintf(exportFilePtr, ">%s\n", str);		  
		
                for (i = 0; i < processingCount; i++)
                {
                    fprintf(exportFilePtr, "%ld\n", lsp->uids[i]);
                }
                FileClose(exportFilePtr);
                exportFilePtr = NULL;
            } else {
                fprintf (master_fp, "\n");
                for (i = 0; i < processingCount; i++)
                {
                    fprintf (master_fp, "%ld\n", lsp->uids[i]);
                }
                fprintf (master_fp, "\n");
                fflush(master_fp);
            }
        }
        return TRUE;
    }
    if (StringCmp(outputSpec, "sd") == 0)
    {
        if (db != TYP_NT && db != TYP_AA)
        {
            ReportBadType(db, outputSpec);
            return FALSE;
        }
        if (! parseOnly && lsp != NULL)
        {
            if (wwwPrefix != NULL && processingCount < totalCount)
            {
                fprintf (master_fp, "Warning: only %ld document summaries are being displayed\n", processingCount);
                fprintf (master_fp, "out of %d total entries.<P>\n", totalCount);
            }
            pubLsp = lsp;
            EntrezDocSumListGet((Int2) processingCount, db, lsp->uids,
                                wwwPrefix == NULL ? PrintDSP : (db == TYP_NT ?
                                PrintDSPNwww : PrintDSPPwww));
            pubLsp = NULL;
        }
        return TRUE;
    }
    if (StringCmp(outputSpec, "sa") == 0)
    {
        if (db != TYP_NT && db != TYP_AA && db != TYP_CH)
        {
            ReportBadType(db, outputSpec);
            return FALSE;
        }
        if (! parseOnly && lsp != NULL)
        {
            aip = AsnIoNew(ASNIO_TEXT_OUT, master_fp, NULL, NULL, NULL);
            for (i = 0; i < processingCount; i++)
            {
                sep = EntrezSeqEntryGet(lsp->uids[i], db == TYP_CH ? -1 : seqEntryRetval);
                if (sep != NULL)
                {
                    SeqEntryAsnWrite(sep, aip, NULL);
                    AsnIoReset(aip);
                    SeqEntryFree(sep);
                }
            }
            AsnIoClose(aip);
        }
        return TRUE;
    }
    if (StringCmp(outputSpec, "sg") == 0 || StringCmp(outputSpec, "sr") == 0)
    {
        if (db != TYP_NT && db != TYP_AA && db != TYP_CH)
        {
            ReportBadType(db, outputSpec);
            return FALSE;
        }
        if (! parseOnly && lsp != NULL)
        {
            for (i = 0; i < processingCount; i++)
            {
                sep = EntrezSeqEntryGet(lsp->uids[i], db == TYP_CH ? -1 : seqEntryRetval);
                if (sep != NULL)
                {
                    PrintGenbank(sep, db == TYP_AA);
                    SeqEntryFree(sep);
                }
            }
        }
        return TRUE;
    }
    if (StringCmp(outputSpec, "sf") == 0)
    {
        if (db != TYP_NT && db != TYP_AA && db != TYP_CH)
        {
            ReportBadType(db, outputSpec);
            return FALSE;
        }
        if (! parseOnly && lsp != NULL)
        {
            for (i = 0; i < processingCount; i++)
            {
                sep = EntrezSeqEntryGet(lsp->uids[i], db == TYP_CH ? -1 : seqEntryRetval);
                if (sep != NULL)
                {
                    SeqEntryConvert (sep, Seq_code_iupacna);
                    SeqEntryToFasta (sep, master_fp, db == TYP_NT);
                    SeqEntryFree(sep);
                }
                fprintf (master_fp, "\n");
            }

        }
        return TRUE;
    }
    if (StringCmp(outputSpec, "si") == 0)
    {
        if (db != TYP_NT && db != TYP_AA && db != TYP_CH)
        {
            ReportBadType(db, outputSpec);
            return FALSE;
        }
        if (! parseOnly && lsp != NULL)
        {
            for (i = 0; i < processingCount; i++)
            {
                sip = EntrezSeqIdForGI(lsp->uids[i]);
                if (sip != NULL)
                {
                    SeqIdWrite(sip, seqIdBuf, PRINTID_FASTA_LONG, sizeof seqIdBuf);

                    fprintf (master_fp, "%s\n", seqIdBuf);
                }
            }
        }
        return TRUE;
    }


    Message(MSG_POST, "Unknown output format \"%s\"", outputSpec);
    return FALSE;
}

static Int2
RunProgram(CharPtr programStr, LocalLinkSetPtr lsp, DocType db, Boolean parseOnly)
{
    Int1 wrongDelim = '.';
    DocType newdb;
    Int2 len;
    CharPtr c;
    Char outputSpec[3];
    long processingCount;
    Char numStr[12];
    Int2 count;
    Int2 numToCopy;
    LinkSetPtr newlsp;

    if (programStr == NULL)
    {
        return -1;
    }

    if (lsp == NULL && !parseOnly)
    {
        return -1;
    }

    len = StrLen(programStr);
    outputSpec[2] = '\0';
    c = programStr;

    for (c = programStr; c < programStr + len; c += count + 1)
    {
        count = StrCSpn(c, ",.");
        if (c[count] == wrongDelim)
        {
            Message(MSG_POST, "Invalid delimiter");
            /* offset to offending delimeter */
            return (count + 1 + c - programStr);
        }

        processingCount = INT4_MAX;

        if (wrongDelim == '.')
        { /* process output */
            switch (count) {
            case 0:
                outputSpec[0] = '\0';
                break;
            case 1:
                Message(MSG_POST, "Invalid output specification \"%c\"", c[1]);
                return ( 2 + c - programStr);
            case 2:
                outputSpec[0] = c[0];
                outputSpec[1] = c[1];
                break;
            default:
                outputSpec[0] = c[0];
                outputSpec[1] = c[1];
                numToCopy = MIN(count - 2, sizeof(numStr) - 1);
                StrNCpy(numStr, c + 2, numToCopy);
                numStr[numToCopy] = '\0';
                if ((int) StrSpn(numStr, "0123456789") != (int) numToCopy)
                {
                  Message(MSG_POST, "Non-numeric character detected");
                  return ( count + c - programStr);
                }
                sscanf(numStr, "%ld", &processingCount);
                break;
            }
            if (lsp != NULL && !parseOnly)
            {
                processingCount = MIN(processingCount, lsp->num);
            }
            if (! ProcessOutput(lsp, db, outputSpec, processingCount,
                                lsp != NULL ? lsp->num : processingCount,
                                parseOnly))
            {
                /* note that error will be posted by ProcessOutput() */
                return ( 3 + c - programStr);
            }
        } else { /* process neighboring */
            if (count == 0)
            {
                Message(MSG_POST, "Null neighboring specification");
                return ( 1 + c - programStr);
            }
            if (count > 1)
            {
                numToCopy = MIN(count - 1, sizeof(numStr) - 1);
                StrNCpy(numStr, c + 1, numToCopy);
                numStr[numToCopy] = '\0';
                if ((int) StrSpn(numStr, "0123456789") != (int) numToCopy)
                {
                  Message(MSG_POST, "Non-numeric character detected");
                  return ( count + c - programStr);
                }
                sscanf(numStr, "%ld", &processingCount);
            }
            switch (*c) {
            case 'p':
                newdb = TYP_AA;
                break;
            case 'm':
                newdb = TYP_ML;
                break;
            case 'n':
                newdb = TYP_NT;
                break;
            case 'g':
                newdb = TYP_CH;
                break;
            default:
                Message(MSG_POST, "Invalid neighboring specification <%s>", *c);
                return ( 1 + c - programStr);
            }

            if (lsp != NULL && !parseOnly)
            {
                processingCount = MIN(processingCount, lsp->num);
                newlsp = NULL;
                EntrezLinkUidList(&newlsp, db, newdb, (Int2) processingCount, lsp->uids, FALSE);
                LocalLinkSetFree(lsp);
                lsp = LinkSetToLocalLinkSet(newlsp);
                LinkSetFree(newlsp);
            }
            db = newdb;
        }

        wrongDelim = wrongDelim == '.' ? ',' : '.';
    }

    if (lsp != NULL && !parseOnly)
        LocalLinkSetFree(lsp);

    return 0;
}

static void
DumpTerm (CharPtr term, Int4 special, Int4 total)
{
    switch (termDisplay) {
    case DISPLAY_SPECIAL_AND_TOTAL:
        fprintf (master_fp, "%s\t%ld\t%ld\n", term, (long) special, (long) total);
        break;
    case DISPLAY_TOTAL_ONLY:
        fprintf (master_fp, "%s\t%ld\n", term, (long) total);
        break;
    case DISPLAY_TERM_ONLY:
        fprintf (master_fp, "%s\n", term);
        break;
    }
}

static Boolean
beginTermProc(CharPtr term, Int4 special, Int4 total)
{
    if (term != NULL)
    {
        DumpTerm(term, special, total);
        MemFree (term);
        return TRUE;
    } else {
        return FALSE;
    }
}

static Boolean
findOneTermProc(CharPtr term, Int4 special, Int4 total)
{
    if (term != NULL)
    {
        MemFree (term);
        return TRUE;
    } else {
        return FALSE;
    }
}

static Boolean
collectNumTermsProc(CharPtr term, Int4 special, Int4 total)
{
    static Boolean inited = FALSE;
    static TermAndCounts PNTR arrayOfTerm = NULL;
    static Int2 head;
    static Boolean sawOurTerm;
    Int4 i;

    if (special == -1)
    { /* flag indicating reset */
        for (i = 0; i < termsBefore; i++)
        {
            MemFree(arrayOfTerm[i].term);
        }
        MemFree(arrayOfTerm);
        arrayOfTerm = NULL;
        inited = FALSE;
        return TRUE;
    }

    if (term == NULL)
    {
        return FALSE;
    }

    if (! inited)
    {
        inited = TRUE;
        arrayOfTerm = MemNew(termsBefore * sizeof(TermAndCounts));
        for (i = 0; i < termsBefore; i++)
        {
            arrayOfTerm[i].term = NULL;
        }
        head = 0;
        sawOurTerm = FALSE;
    }

    if (sawOurTerm)
    {
        DumpTerm (term, special, total);
        MemFree (term);
        if (--head <= 0)
            return FALSE; /* no more terms, please */
        else
            return TRUE;
    } else {
        if (StringICmp(term, theTerm) >= 0)
        {
            sawOurTerm = TRUE;
            for (i = 0; i < termsBefore; i++)
            {
                if (arrayOfTerm[i].term == NULL)
                {
                    head = 0; /* didn't wrap around */
                    break;
                }
            }

            /* print out the queue */
            i = head;
            do {
                if (arrayOfTerm[i].term == NULL)
                    break;
                DumpTerm(arrayOfTerm[i].term, arrayOfTerm[i].special,
                         arrayOfTerm[i].total);
                MemFree (arrayOfTerm[i].term);
                arrayOfTerm[i].term = NULL;
                i = (i + 1) % termsBefore;
            } while (i != head);
            /* number of remaining records to be displayed after this one */
            head = numTerms - (termsBefore + 1);
            DumpTerm (term, special, total);
            MemFree (term);
            return TRUE;
        }
    }

    if (arrayOfTerm[head].term != NULL)
    {
        MemFree(arrayOfTerm[head].term);
    }
    arrayOfTerm[head].term = term;
    arrayOfTerm[head].special = special;
    arrayOfTerm[head].total = total;
    head = (head + 1) % termsBefore;
    return TRUE;
}


static Boolean
TermProcessing(CharPtr programStr, CharPtr termString, DocType db, Boolean parseOnly)
{
    Boolean centerOnTerm = FALSE;
    Boolean beginWithTerm = FALSE;
    Boolean endWithTerm = FALSE;
    Boolean inclusive;
    Char    fldStr[ENTREZ_FLD_MNEMONIC_LENGTH+1];
    DocField fld;
    Int2    firstPage;
    CharPtr countIndex;
    Int2    ratio;
    CharPtr localTermString;

    theTerm = termString;

    if (programStr == NULL || termString == NULL || (int) StrLen(programStr) <
        (3 + ENTREZ_FLD_MNEMONIC_LENGTH))
        return FALSE;

    switch (programStr[0])
    {
    case 's': /* output with special+total, tab-delimeted */
        termDisplay = DISPLAY_SPECIAL_AND_TOTAL; break;
    case 't':
        termDisplay = DISPLAY_TOTAL_ONLY; break;
    case 'o':
        termDisplay = DISPLAY_TERM_ONLY; break;
    default:
        return FALSE;
    }
    switch (programStr[1])
    {
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        ratio = programStr[1] - '0';
        centerOnTerm = TRUE; break;
    case 'c': /* center on the term */
        ratio = 4;
        centerOnTerm = TRUE; break;
    case 'b':
        beginWithTerm = TRUE; break;
    case 'e':
        endWithTerm = TRUE; break;
    default:
        return FALSE;
    }

    switch (programStr[2])
    { /* ignored for centerOnTerm cases, above */
    case 'n': /* non-inclusive */
        inclusive = FALSE; break;
    case 'i': /* inclusive */
        inclusive = TRUE; break;
    default:
        return FALSE;
    }

    StrNCpy (fldStr, &programStr[3], ENTREZ_FLD_MNEMONIC_LENGTH);
    fldStr[ENTREZ_FLD_MNEMONIC_LENGTH] = '\0';
    if ((fld = EntrezStringToField(db, fldStr)) < 0)
        return FALSE;
    countIndex = &programStr[3+ENTREZ_FLD_MNEMONIC_LENGTH];
    numTerms = DEFAULT_TERMLIST_LEN;
    if (*countIndex != '\0' &&
        StrSpn(countIndex, "0123456789") == StrLen(countIndex))
    {
        numTerms = atoi(countIndex);
    }
        
    if (parseOnly)
        return TRUE;

    if (termString[0] == '"' && termString[StrLen(termString)-1] == '"')
    {
        localTermString = MemNew(StrLen(termString));
        StrCpy (localTermString, &termString[1]);
        localTermString[StrLen(localTermString)-1] = '\0';
        theTerm = localTermString;
    } else {
        localTermString = termString;
    }


    if (centerOnTerm)
    {
        EntrezTermListByTerm(db, fld, localTermString, 1, findOneTermProc, &firstPage);
        if (firstPage > 0)
        {
            firstPage--;
        }
        termsBefore = (Int2) (numTerms / ((float) ratio / 2));
        EntrezTermListByPage(db, fld, firstPage, 4, collectNumTermsProc);
        collectNumTermsProc(NULL, -1, -1); /* reset */
    } else {
        EntrezTermListByTerm(db, fld, localTermString, numTerms, beginTermProc, &firstPage);
    }

    if (localTermString != termString)
    {
        MemFree (localTermString);
    }

    return TRUE;
}

static Boolean
TaxProcessing(CharPtr taxString, DocType db, CharPtr progString)
{
    EntrezHierarchyPtr ehp;
    EntrezHierarchyPtr ehp2;
    EntrezHierarchyChildPtr ecp;
    Int2 i;
    DocField fld;
    int maxDepth;

    if (db != TYP_AA && db != TYP_NT && db != TYP_ML && db != TYP_CH)
    {
        fprintf (master_fp, "Invalid database type %d\n", db);
        return FALSE;
    }
    fld = db == TYP_ML ? FLD_MESH_HIER : FLD_ORGN_HIER;
    ehp = EntrezHierarchyGet(taxString, db, fld);
    if (ehp == NULL)
    {
        fprintf (master_fp, "Term %s not found\n", taxString);
        return FALSE;
    }

    if (StrNCmp(progString, "dump", 4) == 0)
    {
        sscanf(&progString[4], "%d", &maxDepth);
        if (maxDepth <= 0)
            maxDepth = INT2_MAX;
        PreOrderTaxTraversal(ehp, 0, db, fld, (Int2) maxDepth);
        EntrezHierarchyFree(ehp);
    } else {
        fprintf (master_fp, "term %s\nLineage:\n", ehp->term);
        for (i = 0; i < ehp->numInLineage; i++)
            fprintf (master_fp, " %s\n", ehp->lineage[i]);
        fprintf (master_fp, " %s\n", taxString);
        if (ehp->numInLineage > 0)
        {
            ehp2 = EntrezHierarchyGet(ehp->lineage[ehp->numInLineage - 1], db,
                                      fld);
            if (ehp2 != NULL && ehp2->numChildren > 1)
            {
                fprintf (master_fp, "Siblings:\n");
                for (i = 0; i < ehp2->numChildren; i++)
                {
                    ecp = &ehp2->children[i];
                    if (StrICmp(ecp->name, taxString) != 0)
                        fprintf (master_fp, " %s\n", ecp->name);
                }
                EntrezHierarchyFree(ehp2);
            }
        }
             
        if (ehp->numChildren > 0)
        {
            fprintf (master_fp, "Children:\n");
            for (i = 0; i < ehp->numChildren; i++)
            {
                ecp = &ehp->children[i];
                fprintf (master_fp, " %s\n", ecp->name);
            }
        }
        EntrezHierarchyFree(ehp);
    }

    return TRUE;
}

static ValNodePtr
ParseImportedFiles(CharPtr str)
{
    CharPtr localStr;
    CharPtr token;
    FILE *fp;
    Char s[100];
    DocType db;
    ValNodePtr head = NULL;
    ValNodePtr node;
    SavListPtr slp;
    Int2 linesread;
    Int4Ptr uids;
    CharPtr p;

    localStr = StringSave(str);
    token = StrTok(localStr, ", ");

    while (token != NULL)
    {
        if ((fp = FileOpen(token, "r")) == NULL)
        {
            Message(MSG_POST, "Error opening file %s", token);
            MemFree(localStr);
            return NULL;
        }
        linesread = 0;
        while (FileGets(s, (sizeof s) - 1, fp) != NULL)
        {
	    Boolean nonNumeric = StrSpn(s, "0123456789 \n\r") != StrLen(s);
	    
            linesread++;
            if (linesread == 1)
            {
                if(nonNumeric)
		  {
		    db = -1;

		    if(StrICmp(s,">MEDLINE") == 0)
		      db = TYP_ML;
		    else if (StrICmp(s,">protein") == 0)
		      db = TYP_AA;
		    else if (StrICmp(s,">nucleotide") == 0)
		      db = TYP_NT;
		    else if (StrICmp(s,">genome") == 0)
		      db = TYP_CH;
		}
		db = atoi(s);
                if (db != TYP_ML && db != TYP_AA && db != TYP_NT && db != TYP_CH)
                {
                    Message(MSG_POST, "Invalid database type %d in file %s", db, token);
                    FileClose(fp);
                    MemFree(localStr);
                    return NULL;
                }
		continue;
            }
            if (nonNumeric)
            {
                Message(MSG_POST, "Invalid character at line %d of file %s", linesread, token);
                FileClose(fp);
                MemFree(localStr);
                return NULL;
            }
        }
        fseek(fp, 0, SEEK_SET); /* rewind to beginning */
        uids = (Int4Ptr) MemNew(sizeof(Int4) * linesread);
        linesread = -1; /* skip over db this time */
        while (FileGets(s, (sizeof s) - 1, fp) != NULL)
        {
            if (linesread >= 0)
            {
                uids[linesread] = atoi(s);
            }
            linesread++;
        }
        FileClose(fp);
        slp = (SavListPtr) MemNew(sizeof(SavList));
        slp->uids = uids;
        slp->db = db;
        slp->num = linesread;
        if ((p = StringRChr(token, DIRDELIMCHR)) == NULL)
        {
            slp->name = MemNew(StrLen(token) + 2);
            StrCpy(&slp->name[1], token);
        } else {
            slp->name = StringSave(p);
        }
        slp->name[0] = '*'; /* to make the name unique, like in Entrez */
        if (head == NULL)
        {
            head = ValNodeNew(NULL);
            node = head;
        } else {
            node = ValNodeNew(head);
        }
        node->data.ptrvalue = (Pointer) slp;

        token = StrTok(NULL, ", ");
    }

    return head;
}
            

static LocalLinkSetPtr
ParseUidList(CharPtr str)
{
    CharPtr localStr;
    CharPtr token;
    long uid;
    int i;
    Int4 count = 0;
    Int4Ptr vector;
    LocalLinkSetPtr lsp;

    /* loop through twice ... the first time count, the second time, store values */
    for (i = 0; i < 2; i++)
    {
        localStr = StringSave(str);
        token = StrTok(localStr, ", ");
        count = 0;
        while (token != NULL)
        {
            if (StrSpn(token, "0123456789") != StrLen(token))
            {
                Message(MSG_POST, "parsing error at position %d", ((long) token) - ((long) localStr));
                MemFree(localStr);
                return NULL;
            }
            if (i == 1)
            {
                sscanf(token, "%ld", &uid);
                vector[count] = (Int4) uid;
            }
            count++;
            token = StrTok(NULL, ", ");
        }
        if (i == 0)
        {
            vector = MemNew(count * sizeof(Int4));
        }
        MemFree(localStr);
    }

    if (count == 0)
    {
        return NULL;
    }
    lsp = LocalLinkSetNew();
    lsp->num = count;
    lsp->uids = vector;
    return lsp;
}

static CharPtr
FormatPositionalErr(Int2 beginErr, Int2 endErr, Int2 startLen)
{
    int i;
    CharPtr str;

    /* prepare text describing where error occurred */
    str = MemNew(endErr + startLen + 2);
    for (i = 0; i < endErr + startLen - 1; i++)
    {
        str[i] = ' ';
    }
    str[i++] =  '^';
    str[beginErr + startLen] = '^';
    str[i] =  '\0';

    return str;
}

#define IS_HEX(x)   (IS_DIGIT(x) || ((x) >= 'a' && ((x) <= 'f')) || \
                     ((x) >= 'A' && ((x) <= 'F')))

static CharPtr
WWWStyleDecoding(CharPtr string, Boolean doEncoding)
{ /* decoding in-place, assuming that decoded string is always smaller than
     original */
    CharPtr p, q, maxchar;
    Char str[3];
    int newchar;

    if (! doEncoding)
        return string;

    maxchar = string + (int) StrLen(string);

    for (p = string; p < maxchar - 2; p++)
    {
        if (*p == '%' && IS_HEX(p[1]) && IS_HEX(p[2]))
        {
            str[0] = p[1];
            str[1] = p[2];
            str[2] = '\0';
            sscanf(str, "%x", &newchar);
            *p = (Char) newchar;
            maxchar -= 2;
            for (q = p + 1; q <= maxchar; q++)
                *q = q[2];
        }
    }

    return string;
}
static int LIBCALLBACK
compUidsDescending(VoidPtr a, VoidPtr b)
{
    Int4Ptr x = (Int4Ptr) a;
    Int4Ptr y = (Int4Ptr) b;

    return (*y - *x);  /* note descending order */
}

static void
SortUidsDescending(LocalLinkSetPtr lsp)
{
    Boolean sorted;
    int k;
    Int4 temp;

    if (lsp == NULL)
        return;

    /* try to sort uids in descending order */

    for (sorted = TRUE, k = 1; k < lsp->num; k++)
    {
        if (lsp->uids[k-1] < lsp->uids[k])
        {
            sorted = FALSE;
            break;
        }
    }

    if (! sorted)
    {   /* assume that the existing order is reversed */
        for (k = (lsp->num / 2) - 1; k >= 0; k--)
        {
            temp = lsp->uids[k];
            lsp->uids[k] = lsp->uids[lsp->num - 1 - k];
            lsp->uids[lsp->num - 1 - k] = temp;
        }

        /* now check that it's sorted */
        for (sorted = TRUE, k = 1; k < lsp->num; k++)
        {
            if (lsp->uids[k-1] < lsp->uids[k])
            {
                sorted = FALSE;
                break;
            }
        }

        if (! sorted)
        { /* as a last resort, sort them using heapsort */
            HeapSort(lsp->uids, lsp->num, sizeof(Int4), compUidsDescending);
        }
    }
}

static void
PrintHelp(void)
{
    fprintf (master_fp, "Entrcmd is a non-interactive command-line interface which allows a user to\n");
    fprintf (master_fp, "perform a series of neighboring and output operations, based upon an initial\n");
    fprintf (master_fp, "set of UIDs or a boolean expression which describes a set of UIDs.\n");
    fprintf (master_fp, "Alternatively, it can be used to display an alphabetically sorted list of\n");
    fprintf (master_fp, "terms near an initial term.\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "Type 'entrcmd' with no arguments for a brief summary of command-line options.\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "    EXPRESSION SYNTAX (-e option)\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "The following grammar is based upon Backus-Naur form.  Braces ({}) are used to\n");
    fprintf (master_fp, "specify optional fields, and ellipses (...) represents an arbitrary number\n");
    fprintf (master_fp, "of repititions.  In most Backus-Naur forms, the vertical bar (|) and brackets\n");
    fprintf (master_fp, "([]) are used as meta-symbols.  However, in the following grammar, the\n");
    fprintf (master_fp, "vertical bar and brackets are terminal symbols, and three stacked vertical\n");
    fprintf (master_fp, "bars are used to represent alternation.\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "expression ::= diff { - diff ... }\n");
    fprintf (master_fp, "diff ::= term { | term ... }\n");
    fprintf (master_fp, "term ::= factor { & factor ... }\n");
    fprintf (master_fp, "                     |\n");
    fprintf (master_fp, "factor ::= qualtoken | ( expression )\n");
    fprintf (master_fp, "                     |\n");
    fprintf (master_fp, "qualtoken ::= token { [ fld { ,S } ] }\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "token is a string of characters which either contains no special characters,\n");
    fprintf (master_fp, "or which is delimited by double-quotes (\").  Double-quote marks and\n");
    fprintf (master_fp, "backslashes (\\) which appear with a quoted token must be quoted by an\n");
    fprintf (master_fp, "additional backslash.\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "fld is an appropriate string describing a field.  The possible values are\n");
    fprintf (master_fp, "described in the following table.  For all databases, an asterisk(*) is a\n");
    fprintf (master_fp, "possible value for fld, signifying the union of all possible fields for that\n");
    fprintf (master_fp, "database.  '*' is also the default field, if no field qualifier is specified.\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "  | fld| Databases and descriptions\n");
    fprintf (master_fp, "  +----+--------------------------------------------------------------------\n");
    fprintf (master_fp, "  |WORD| For MEDLINE, \"Abstract or Title\"; for Sequences, \"Text Terms\"\n");
    fprintf (master_fp, "  |MESH| MEDLINE only, \"MeSH term\"\n");
    fprintf (master_fp, "  |AUTH| For all databases, \"Author Name\"\n");
    fprintf (master_fp, "  |JOUR| For all databases, \"Journal Title\"\n");
    fprintf (master_fp, "  |GENE| For all databases, \"Gene Name\"\n");
    fprintf (master_fp, "  |KYWD| For MEDLINE, \"Substance\", for Sequences \"Keyword\"\n");
    fprintf (master_fp, "  |ECNO| For MEDLINE and protein, \"E.C. number\"\n");
    fprintf (master_fp, "  |ORGN| For all databases, \"Organism\"\n");
    fprintf (master_fp, "  |ACCN| For Sequence databases, \"Accession\"\n");
    fprintf (master_fp, "  |PROT| For protein, \"Protein Name\"\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "The presence of \",S\"  after a field specifier implies the same semantics\n");
    fprintf (master_fp, "as \"special\" in Entrez.  Entrez \"total\" semantics are the default.\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "    PROGRAM OF COMMANDS (-p option)\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "For the \"-e\" and \"-u\" options, the program of commands consists of a sequence of\n");
    fprintf (master_fp, "neighboring operations alternated with optional output commands.  All output\n");
    fprintf (master_fp, "commands, except the first, must be preceded by a period (.), and all\n");
    fprintf (master_fp, "neighboring commands must be preceded by a comma (,).\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "The output commands are:\n");
    fprintf (master_fp, "   no    None (default)             sg    Sequence GenBank/GenPept flat file format\n");
    fprintf (master_fp, "   ma    MEDLINE ASN.1 format       sa    Sequence ASN.1 format\n");
    fprintf (master_fp, "   md    MEDLINE docsums            sd    Sequence docsums\n");
    fprintf (master_fp, "   ml    MEDLARS format             sf    Sequence FASTA format\n");
    fprintf (master_fp, "   mr    MEDLINE report format      sr    Sequence report format\n");
    fprintf (master_fp, "   mu    MEDLINE UIDs               su    Sequence UIDs\n");
    fprintf (master_fp, "                                    si    Sequence IDs\n");
    fprintf (master_fp, "Each output command may be followed by an optional count indicating how\n");
    fprintf (master_fp, "many articles to display.  The default is to display all the articles.\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "If the \"-x\" command line option appears (\"export to a saved UID list\"), then\n");
    fprintf (master_fp, "the first \"mu\" or \"su\" command results in those UIDs being written to that\n");
    fprintf (master_fp, "\"saved UID list\" file, rather than being written to the standard output.\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "Neighboring commands indicate the database to neighbor \"to\", and\n");
    fprintf (master_fp, "consists of the first letter of each of the possible databases:\n");
    fprintf (master_fp, "(medline, protein, nucleotide) followed by an optional count of\n");
    fprintf (master_fp, "how many of the current set of articles should be included in the\n");
    fprintf (master_fp, "neighboring operation.\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "Example:\n");
    fprintf (master_fp, "  Find the articles written by \"Kay LE\", but not by \"Forman-Kay JD\".  Find\n");
    fprintf (master_fp, "  their MEDLINE neighbors.  Print document summaries for all of these\n");
    fprintf (master_fp, "  neighbors.  Of these neighbors, neighbor the first 5 entries to the protein\n");
    fprintf (master_fp, "  database.  Print up to 10 of these sequences in Sequence Report format.\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "    entrcmd -e '\"Kay LE\" [AUTH] - \"Forman-Kay JD\" [AUTH]' -p ,m.md,p5.sr10\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "If the \"-t\" option is used, then the program of commands is different from\n");
    fprintf (master_fp, "what is described above.  Rather, it consists of a seven character string,\n");
    fprintf (master_fp, "optionally followed by the number of terms which should be displayed.\n");
    fprintf (master_fp, "The default number of terms is 40.\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "The string is of the form '123FLDD', where 1, 2, and 3 are as follows,\n");
    fprintf (master_fp, "and FLDD is one of the field specifications described above (AUTH, etc.).\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "1 - one of 't', 's', or 'o', where 't' means that the total term counts\n");
    fprintf (master_fp, "    should be displayed after the term, 's' means that the special and\n");
    fprintf (master_fp, "    total term counts should be displayed after the term, and 'o' means\n");
    fprintf (master_fp, "    that only the term itself should be displayed\n");
    fprintf (master_fp, "2 - one of 'b', 'c', 'e', or an integer from 3 to 9, where:\n");
    fprintf (master_fp, "    'b' - display terms beginning with the specified term\n");
    fprintf (master_fp, "    'c' - \"center\" terms; i.e., display half the terms before the specified\n");
    fprintf (master_fp, "          term, and half the terms after the specified term\n");
    fprintf (master_fp, "    'e' - display terms ending with the specified term\n");
    fprintf (master_fp, "    k   - an integer from 3 to 9, indicating that (2/k)ths of the terms\n");
    fprintf (master_fp, "          should be alphabetically before the specified term.  Note that\n");
    fprintf (master_fp, "          '4' is the same as 'c'.  The value '9' is recommended for\n");
    fprintf (master_fp, "          scrolled displays.\n");
    fprintf (master_fp, "3 - One of 'i' or 'n', indicating for the 'b' and 'e' options above whether\n");
    fprintf (master_fp, "    the specified term is to be included in the output, where 'i' means\n");
    fprintf (master_fp, "    inclusive, and 'n' means non-inclusive.  This value is ignored for\n");
    fprintf (master_fp, "    other values of the previous character, but must be present as a\n");
    fprintf (master_fp, "    place-holder.\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "[ WARNING: SOME OF THESE TERM SPECIFICATIONS OPTIONS (COMBINATIONS OF 1,\n");
    fprintf (master_fp, "2, AND 3 ABOVE) ARE CURRENTLY UNIMPLEMENTED ]\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "    WORLD WIDE WEB STYLE OUTPUT (-w option)\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "The entrcmd program can also generate output which is appropriate for\n");
    fprintf (master_fp, "display in an HTML document, to be \"served\" by a WWW server.  In particular,\n");
    fprintf (master_fp, "some output text contains HTML hypertext links to other data, as well as\n");
    fprintf (master_fp, "HTML formatting information.  The parameter to the -w option is the\n");
    fprintf (master_fp, "directory prefix for the linked hypertext items; \"/htbin\" is recommended.\n");
    fprintf (master_fp, "\n");
    fprintf (master_fp, "If the \"-w\" option is selected, then the \"-f\" option may also be selected.\n");
    fprintf (master_fp, "This indicates that the HTML output should be of a form which is\n");
    fprintf (master_fp, "appropriate for a HTML \"FORM\".  This output can only be processed by\n");
    fprintf (master_fp, "advanced WWW clients, but potentially provides a nicer interface, where\n");
    fprintf (master_fp, "each document summary has an associated checkbox, resulting in a display\n");
    fprintf (master_fp, "which is similar to the Entrez CD-ROM application.  The \"-c\" option, if used\n");
    fprintf (master_fp, "in conjunction with \"-f\", indicates that these checkboxes should be\n");
    fprintf (master_fp, "\"pre-checked\", i.e., selected.  This potentially provides the equivalent\n");
    fprintf (master_fp, "of the Entrez \"select all\" operation for neighboring.\n");
}


static LocalLinkSetPtr BSPtoLSP(ByteStorePtr bsp)
{
    LocalLinkSetPtr lsp;
    
    if (bsp == NULL)
        return NULL;

    lsp = LocalLinkSetNew();

    lsp->num = BSLen(bsp) / sizeof(DocUid);
    if ((lsp->uids = MemNew(BSLen(bsp))) == NULL)
    { /* platforms which can't allocate this are out of luck */
        lsp = LocalLinkSetFree(lsp);
    } else {
        BSSeek (bsp, 0L, 0);
        BSRead (bsp, lsp->uids, lsp->num * sizeof (DocUid));
    }

    return lsp;
}

Int2 Main(void)
{
    int Numarg = sizeof(myargs)/sizeof(Args);
    DocType db = TYP_ML;
    Boolean exprSpecified = FALSE;
    Boolean uidsSpecified = FALSE;
    Boolean termSpecified = FALSE;
    Boolean taxSpecified = FALSE;
    Boolean neighborSpecified = FALSE;
    CharPtr boolString;
    short erract;
    ErrDesc err;
    Int2 beginErr;
    Int2 endErr;
    CharPtr str;
    LocalLinkSetPtr lsp = NULL;
    LinkSetPtr oldstylelsp;
    ByteStorePtr bsp;
    CharPtr programStr;
    CharPtr termString;
    CharPtr taxString;
    CharPtr neighborString;
    CharPtr neighborFile;
    FILE *neighborFp;
    CharPtr exportFile;
    CharPtr importFileList;
    Int2 progErr;
    ValNodePtr savlist = NULL;
    ValNodePtr np;
    SavListPtr slp;
    Char param[6];
    EntrezNeighborTextPtr entp; 
    Boolean useWWWEncoding;

    if ( ! GetArgs("Entrez command-line $Revision: 6.4 $", Numarg, myargs))
        return 1;

    if (myargs[14].strvalue)
    {
        if ((master_fp = FileOpen(myargs[14].strvalue, "w")) == NULL)
        {
            Message(MSG_POST, "Unable to open output file <%s>", myargs[14].strvalue);
            return 9;
        }
    }

    if (myargs[6].intvalue)
    {
        PrintHelp();
        FileClose(master_fp);
        return 0;
    }

    if (myargs[0].strvalue != NULL)
    {
        switch(myargs[0].strvalue[0]) {
        case 'm': db = TYP_ML; break;
        case 'n': db = TYP_NT; break;
        case 'g': db = TYP_CH; break;
        case 'p': db = TYP_AA; break;
        default:
            Message(MSG_POST /* MSG_FATAL */, "Invalid database type <%s>", myargs[0].strvalue);
            FileClose(master_fp); 
            return 1;
        }
    }

    useWWWEncoding = myargs[15].intvalue;

    if (myargs[16].intvalue)
      seqEntryRetval = -2;
    else
      seqEntryRetval = myargs[17].intvalue;
    
    if (myargs[1].strvalue != NULL && myargs[1].strvalue[0] != '\0')
    {
        exprSpecified = TRUE;
        boolString = WWWStyleDecoding(myargs[1].strvalue, useWWWEncoding);
    }
    if (myargs[2].strvalue != NULL && myargs[2].strvalue[0] != '\0')
        uidsSpecified = TRUE;

    if (myargs[11].strvalue != NULL && myargs[11].strvalue[0] != '\0')
    {
        termSpecified = TRUE;
        termString = WWWStyleDecoding(myargs[11].strvalue, useWWWEncoding);
    }

    if (myargs[12].strvalue != NULL && myargs[12].strvalue[0] != '\0')
    {
        taxSpecified = TRUE;
        taxString = WWWStyleDecoding(myargs[12].strvalue, useWWWEncoding);
    }

    if (myargs[13].strvalue != NULL && myargs[13].strvalue[0] != '\0')
    {
        neighborSpecified = TRUE;
        neighborFile = myargs[13].strvalue;
        if ((neighborFp = FileOpen(neighborFile, "r")) == NULL)
        {
            Message(MSG_POST /* MSG_FATAL */, "Unable to open neighboring-file %s", neighborFile);
            FileClose(master_fp);
            return 1;
        }
        FileClose(neighborFp);
    }

    if (((exprSpecified != 0) + (uidsSpecified != 0) + (termSpecified != 0) +
         (taxSpecified != 0) + (neighborSpecified != 0)) != 1)
    {
        Message(MSG_POST /* MSG_FATAL */, "Exactly one of the -e, -l, -n, -t and -u options must be specified");
        FileClose(master_fp);
        return 1;
    }

    if (uidsSpecified)
    {
        lsp = ParseUidList(myargs[2].strvalue);
        if (lsp == NULL)
        {
            Message(MSG_POST /* MSG_FATAL */, "Syntax error on UID list");
            FileClose(master_fp);
            return 1;
        }
    }

    if (exprSpecified)
    {
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_CONTINUE, 0);
        ErrFetch(&err);
        if (! EntrezTLParseString(boolString, db, -1, &beginErr, &endErr))
        {
            ErrShow();
            Message(MSG_POST, "Syntax error: %s", boolString);
            if (endErr < 0)
            {
                endErr = 0;
            }

            str = FormatPositionalErr(beginErr, endErr, StrLen("Syntax error: "));

            Message(MSG_POST, str);
            MemFree(str);
            FileClose(master_fp);
            return 2;
        }
        ErrSetOpts(erract, 0);
    }

    wwwPrefix = myargs[5].strvalue;

    programStr = myargs[3].strvalue;

    useForms = myargs[7].intvalue;
    checkForms = myargs[8].intvalue;
    exportFile = myargs[9].strvalue;
    importFileList = myargs[10].strvalue;

    if (exportFile != NULL && exportFile[0] != '\0') {
        GetAppParam("ENTREZ", "ENTRCMD", "EXPORT_OK", "FALSE", param,
                    sizeof param);
        if (StrICmp(param, "TRUE") != 0)
        {
            Message(MSG_POST, "Export option is disabled");
            FileClose(master_fp);
            return 6;
        }
    }

    if (termSpecified)
    {
        if (! TermProcessing(programStr, termString, db, TRUE))
        {
            Message(MSG_POST, "Invalid term program specification %s", programStr);
            FileClose(master_fp);
            return 3;
        }
    } else if (taxSpecified) {
        /* no action */
    } else {
        if ((progErr = RunProgram(programStr, NULL, db, TRUE)) != 0)
        {
            Message(MSG_POST, "Program error: %s", programStr);
            if (progErr > 0)
                str = FormatPositionalErr(progErr - 1, progErr - 1,
                                          StrLen("Program error: "));
            else
                str = StringSave("Validation error");
    
            Message(MSG_POST, str);
            MemFree(str);
            FileClose(master_fp);
            return 4;
        }
        if (exportFile != NULL)
        {
            exportFilePtr = FileOpen(exportFile, "w");
        }
        if (importFileList != NULL)
        {
            if ((savlist = ParseImportedFiles(importFileList)) == NULL)
            {
                Message(MSG_POST, "Fatal error processing imported files");
            }
        }
    }

    /* note that we defer EntrezInit() until we're sure that there are no */
    /* parsing errors                                                     */
    if (! EntrezInit("entrcmd", FALSE, NULL))
    {
        Message(MSG_POST, "Unable to access Entrez dataset");
        FileClose(master_fp);
        return 5;
    }

    EntrezBioseqFetchEnable("entrcmd", TRUE);

    if (myargs[4].intvalue)
    {
        str = EntrezDetailedInfo();
        fprintf (master_fp, "                               STATUS REPORT\n\n\n%s\n\n", str);
        fflush(master_fp);
    }

    while (savlist != NULL)
    { /* create named UID lists, as needed */
        slp = (SavListPtr) savlist->data.ptrvalue;
        EntrezCreateNamedUidList(slp->name, slp->db, 0, slp->num, slp->uids);
        MemFree(slp->name);
        MemFree(slp->uids);
        MemFree(slp);
        np = savlist->next;
        MemFree(savlist);
        savlist = np;
    }

    if (exprSpecified)
    { /* note that we deferred evaluation until after EntrezInit() */
        if ((bsp = EntrezTLEvalXString(boolString, db, -1, NULL, NULL)) != NULL)
        {
            lsp = BSPtoLSP(bsp);
            BSFree(bsp);
        }
        if (db == TYP_ML && lsp != NULL)
        {
            SortUidsDescending(lsp);
        }
    }

    if (neighborSpecified)
    {
        if (db == TYP_ML)
        {
            size_t neighborLen;
            int k;
            Int4 temp;
    
            if (! EntrezCanNeighborText())
            {
                Message(MSG_POST, "Unable to perform on-the-fly neighboring\n");
                FileClose(master_fp);
                return 5;
            }
            /* create text object here . */
            neighborLen = FileLength(neighborFile);
            if ((neighborString = MemNew(neighborLen+1)) == NULL)
            {
                Message(MSG_POST, "Unable to allocate memory for on-the-fly neighboring\n");
                FileClose(master_fp);
                return 5;
            }
            neighborFp = FileOpen(neighborFile, "r");
            FileRead(neighborString, neighborLen, 1, neighborFp);
            FileClose (neighborFp);
            neighborString[neighborLen] = 0;
            entp = EntrezNeighborTextNew();
            entp->percent_terms_to_use = 100;
            entp->max_neighbors = 0;
            entp->min_score = 0;
            entp->fld = FLD_WORD;
            entp->normalText = neighborString;
            entp->specialText = StringSave("");
	    oldstylelsp = EntrezDoNeighborText(entp);
            lsp = LinkSetToLocalLinkSet(oldstylelsp);
	    LinkSetFree(oldstylelsp);
            EntrezNeighborTextFree(entp);
    
            /* reverse the order since they are received in backwards order */
            if (lsp != NULL)
            {
                for (k = (lsp->num / 2) - 1; k >= 0; k--)
                {
                    temp = lsp->uids[k];
                    lsp->uids[k] = lsp->uids[lsp->num - 1 - k];
                    lsp->uids[lsp->num - 1 - k] = temp;
                }
            }
        } else {
            Boolean isprot = db == TYP_AA;
            SeqEntryPtr sep;
            BioseqPtr bsp;

            if (! EntrezCanBlast())
            {
                Message(MSG_POST, "Unable to perform on-the-fly BLAST\n");
                FileClose(master_fp);
                return 5;
            }
            neighborFp = FileOpen(neighborFile, "r");
            sep = FastaToSeqEntry(neighborFp, !isprot);
            FileClose (neighborFp);
            if (sep == NULL) 
            {
                Message (MSG_OK, "Error encountered while parsing sequence data");
                return 8;
            }
            bsp = NULL;
            SeqEntryExplore(sep, &bsp, isprot? FindAProt : FindANuc);
            if (bsp == NULL)
            {
                Message (MSG_OK, "Error encountered while parsing sequence data for Bioseq");
                /* ? SeqEntryFree(sep); */
                return 9;
            }
            oldstylelsp = EntrezBlastBioseq(bsp, db, NULL, NULL, NULL, FALSE);
            lsp = LinkSetToLocalLinkSet(oldstylelsp);
	    LinkSetFree(oldstylelsp);
        }
    }

    if (termSpecified)
    {
        TermProcessing(programStr, termString, db, FALSE);
    } else if (taxSpecified) {
        TaxProcessing(taxString, db, programStr);
    } else {
        RunProgram(programStr, lsp, db, FALSE);
    }
    EntrezFini();
    EntrezBioseqFetchDisable();

    FileClose(master_fp);
    return 0;
}
