static char const rcsid[] = "$Id: taxblast.c,v 6.23 2004/09/08 11:40:09 bollin Exp $";

/* $Id: taxblast.c,v 6.23 2004/09/08 11:40:09 bollin Exp $
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
* File Name:  $RCSfile: taxblast.c,v $
*
* Authors:  Scott Federhen, Sergei Shavirin
*
* Initial Version Creation Date: 04/04/2000
*
* $Revision: 6.23 $
*
* File Description:
*        Utilities and functions for Tax-Blast program
*
* $Log: taxblast.c,v $
* Revision 6.23  2004/09/08 11:40:09  bollin
* removed network/taxon1 dependencies
*
* Revision 6.22  2004/08/30 13:43:54  madden
* Change from htbin-post URL
*
* Revision 6.21  2003/09/16 16:03:19  rsmith
* guard include of unistd.h to only those platforms that need it and can use it.
*
* Revision 6.20  2003/09/15 18:46:29  rsmith
* added unistd.h include for sleep
*
* Revision 6.19  2003/05/30 17:25:38  coulouri
* add rcsid
*
* Revision 6.18  2003/05/13 16:02:54  coulouri
* make ErrPostEx(SEV_FATAL, ...) exit with nonzero status
*
* Revision 6.17  2002/11/06 21:31:47  ucko
* TXBGetGiFromSeqId: Compare oip->id to 0 rather than NULL, as it is an integer.
*
* Revision 6.16  2002/10/07 19:52:07  camacho
* Added RDTaxLookupReset
*
* Revision 6.15  2002/07/02 16:38:04  camacho
* Moved increment of number of taxids in db to end of FDBTaxCallback
*
* Revision 6.14  2002/05/23 22:22:15  camacho
* Do not keep entries with taxid 0 in blast taxonomy database
*
* Revision 6.13  2002/01/11 16:31:13  camacho
* Verify if taxonomy service is properly initialized in RDTaxLookupInit
*
* Revision 6.12  2001/12/13 21:48:51  camacho
* Fixed retrieval of maximum taxonomy id to properly populate the
* RDBTaxLookup structure.
*
* Revision 6.11  2001/07/11 15:24:02  shavirin
* Fixed minor problem when tax_id is unrecognizable.
*
* Revision 6.10  2001/06/21 19:34:28  shavirin
* Fixed problem in the function FDBTaxCallback().
*
* Revision 6.9  2001/05/18 21:02:30  shavirin
* Added possibility to extract taxonomy id information from SEQID_GENERAL
* seqids in the form NAME_taxid.
*
* Revision 6.8  2001/05/16 19:32:47  shavirin
* Added possibility to create taxonomy reports from Blast databases with
* SEQID_GENERAL types of SeqIds and tax_ids as database name.
*
* Revision 6.7  2001/05/08 21:58:28  shavirin
* Added possibility to generate tax_id for every definition in Blast FASTA
* definition set in ASN.1 structured definition lines.
*
* Revision 6.6  2001/02/06 22:28:00  shavirin
* Default output was set to "stdout".
*
* Revision 6.5  2000/12/12 23:13:11  shavirin
* Fixed some memory leaks.
*
* Revision 6.4  2000/12/08 22:30:57  shavirin
* Added set of functions for creation of Taxonomy lookup database using
* formatdb API.
*
* Revision 6.3  2000/08/10 14:52:44  shavirin
* Fixed typo.
*
* Revision 6.2  2000/07/20 20:55:21  shavirin
* Added TAX_BLAST_MAIN module, that may turn this file into
* standalone executable.
*
* Revision 6.1  2000/05/17 15:54:38  shavirin
* Initial revision in new location.
*
* Revision 1.6  2000/04/28 15:05:35  shavirin
* Added link to help, like to alignment moved to bit-score.
*
* Revision 1.5  2000/04/27 19:54:56  shavirin
* Changed "(())" to "[]" by request of Scott.
*
* Revision 1.4  2000/04/27 13:10:33  shavirin
* Increased font size for organism report.
*
* Revision 1.3  2000/04/26 19:54:26  shavirin
* Changed formation of Organism report.
*
* Revision 1.2  2000/04/24 13:50:14  shavirin
* Added index to BLAST reports. Added message "unrecognized taxid" for taxid == 0
*
* Revision 1.1  2000/04/20 14:01:52  shavirin
* Initial revision
*
*
* ==========================================================================
*/

#include <ncbi.h>
#if defined(OS_MAC)  ||  defined(COMP_METRO)
#include <unistd.h> /* for sleep()  */
#endif
#include <sequtil.h>
#include <treemgr.h>
#include <taxext.h>
#include <txclient.h>
#include <objseq.h>
#include <objgen.h>
#include <readdb.h>
#include <blastdef.h>
#include <taxblast.h>

typedef struct hitobj {
    Boolean    query_is_na;
    Boolean    db_is_na;
    Int4       numhits;
    Int4Ptr    gis;
    Int4Ptr    scores;
    FloatHiPtr bit_scores;
    FloatHiPtr e_values;
    Int4Ptr    taxoffs;
    CharPtr    *accs;
} HitObj, PNTR HitObjPtr;

typedef struct cnames {
    Boolean    preferred;
    Int4       count;
    ValNodePtr names;
} Cnames, PNTR CnamesPtr;

typedef struct bname {
    Int4       taxid;
    CharPtr    name;
} Bname, PNTR BnamePtr;

typedef struct orgobj {
    Int4       numorgs;
    Int4Ptr    taxids;
    CharPtr    PNTR names;
    CnamesPtr  PNTR cnames;
    BnamePtr   PNTR bnames;
    ValNodePtr PNTR hitlists;
} OrgObj, PNTR OrgObjPtr;

typedef struct nodeobj {
    Int4        numhits, numorgs;
    ValNodePtr  orglist;
    Boolean     joinnode, hitnode;
} NodeObj, PNTR NodeObjPtr;

typedef struct linobj {
    CharPtr     name;
    Int4        taxid;
    ValNodePtr  orglist;
    struct linobj PNTR last;
    struct linobj PNTR next;
} LinObj, PNTR LinObjPtr;

static HitObjPtr HitObjNew (void)
{
    HitObjPtr hitobj;
    
    hitobj = (HitObjPtr) MemNew(sizeof(HitObj));
    hitobj->numhits = 0;
    hitobj->gis = (Int4Ptr) NULL;
    hitobj->scores = (Int4Ptr) NULL;
    hitobj->e_values = (FloatHiPtr) NULL;
    hitobj->taxoffs = (Int4Ptr) NULL;
    return(hitobj);
}

static void HitObjFree (HitObjPtr hitobj)
{
    Int4 i, count;
    
    if (hitobj == NULL) 
        return;
    
    MemFree(hitobj->gis);
    MemFree(hitobj->scores);
    MemFree(hitobj->e_values);
    MemFree(hitobj->bit_scores);
    MemFree(hitobj->taxoffs);
    
    count = hitobj->numhits;
    
    for(i = 0; i < count; i++)
        MemFree(hitobj->accs[i]);
    
    MemFree(hitobj->accs);    
    MemFree(hitobj);

    return;
}

static OrgObjPtr OrgObjNew (void)
{
    OrgObjPtr orgobj;
    
    orgobj = (OrgObjPtr) MemNew(sizeof(OrgObj));
    orgobj->numorgs = 0;
    orgobj->taxids = (Int4Ptr) NULL;
    orgobj->names = (CharPtr *) NULL;
    orgobj->cnames = (CnamesPtr *) NULL;
    orgobj->bnames = (BnamePtr *) NULL;
    orgobj->hitlists = (ValNodePtr *) NULL;
    return(orgobj);
}

static void CnamesFree (CnamesPtr cnames)
{
    ValNodePtr vnp, vnp_next;
    
    ValNodeFreeData(cnames->names);

    MemFree(cnames);
    
    return;
}
static void BnameFree (BnamePtr bname)
{
    if (bname == NULL) 
        return;
    
    if (bname->name) 
        MemFree(bname->name);
    
    MemFree(bname);
    return;
}

static void OrgObjFree (OrgObjPtr orgobj)
{
    Int4 numorgs, i;
    
    if (orgobj == NULL) 
        return;

    numorgs = orgobj->numorgs;

    MemFree(orgobj->taxids);

    if (orgobj->names) {
        for (i=0; i<numorgs; i++) 
            MemFree(orgobj->names[i]);
        MemFree(orgobj->names);
    }

    if (orgobj->cnames) {
        for (i=0; i<numorgs; i++) 
            CnamesFree(orgobj->cnames[i]);

        MemFree(orgobj->cnames);
    }

    if (orgobj->bnames) {
        for (i=0; i<numorgs; i++) 
            BnameFree(orgobj->bnames[i]);
        MemFree(orgobj->bnames);
    }
    if (orgobj->hitlists) {
        for (i=0; i<numorgs; i++) 
            ValNodeFree(orgobj->hitlists[i]);
        MemFree(orgobj->hitlists);
    }
    MemFree(orgobj);
    return;
}

static CnamesPtr CnamesNew (void)
{
    CnamesPtr cnames;
    
    cnames = (CnamesPtr) MemNew(sizeof(Cnames));
    cnames->preferred = FALSE;
    cnames->count = 0;
    cnames->names = (ValNodePtr) NULL;
    
    return(cnames);
}

static BnamePtr BnameNew (void)
{
    BnamePtr bname;
    
    bname = (BnamePtr) MemNew(sizeof(Bname));
    bname->taxid = 0;
    bname->name = (CharPtr) NULL;
    return(bname);
}

static NodeObjPtr NodeObjNew (void)
{
    NodeObjPtr nodeobj;
    
    nodeobj = (NodeObjPtr) MemNew(sizeof(NodeObj));
    nodeobj->numhits = 0;
    nodeobj->numorgs = 0;
    nodeobj->orglist = (ValNodePtr) NULL;
    nodeobj->joinnode = FALSE;
    nodeobj->hitnode = FALSE;
    return (nodeobj);
}

static void NodeObjFree (NodeObjPtr nodeobj)
{
    ValNodeFree (nodeobj->orglist);
    MemFree (nodeobj);
    return;
}

static LinObjPtr LinObjNew (void)
{
    LinObjPtr linobj;
    
    linobj = (LinObjPtr) MemNew(sizeof(LinObj));
    linobj->name = (CharPtr) NULL;
    linobj->taxid = 0;
    linobj->orglist = (ValNodePtr) NULL;
    linobj->last = (LinObjPtr) NULL;
    linobj->next = (LinObjPtr) NULL;
    
    return (linobj);
}

static void LinObjFree (LinObjPtr linobj)
{
    LinObjPtr linobj_next;

    if (linobj == NULL) 
        return;
    
    MemFree (linobj->name);
    ValNodeFree (linobj->orglist);
    linobj_next = linobj->next;
    MemFree (linobj);
    
    LinObjFree (linobj_next);
}

static Int4 CountAligns (SeqAlignPtr sap)
{
    Int4 count = 0;
    
    while (sap) {
        count++;
        sap = sap->next;
    }
    
    return(count);
}
static Int4 TXBGetGiFromSeqId(SeqIdPtr sip, Int4Ptr tax_idp, 
                              CharPtr * accessionp)
{
    SeqIdPtr sip_tmp;
    Int4 gi, value;
    DbtagPtr dbtag;
    ObjectIdPtr oip;
    Char buffer[256];
    CharPtr chptr;

    gi = -1; *tax_idp = 0;
    for(sip_tmp = sip; sip_tmp != NULL; sip_tmp = sip_tmp->next) {
        switch(sip_tmp->choice) {

        case SEQID_GI:
            gi = sip_tmp->data.intvalue;
            break;
            
        case SEQID_GENERAL:
            *tax_idp = 0;
            dbtag = sip_tmp->data.ptrvalue;
            if((value = atol(dbtag->db)) != 0) {
                *tax_idp = value;
            } else if((chptr = StringChr(dbtag->db, '_')) != NULL) {
                chptr++;
                if((value = atol(chptr)) != 0) {
                    *tax_idp = value;
                }
            }
            oip = (ObjectIdPtr) dbtag->tag;

            if (oip != NULL) {
                if (oip->id != 0) {
                    sprintf(buffer, "%d", oip->id);
                    *accessionp = StringSave(buffer);
                } else if (oip->str)
                    *accessionp = StringSave(oip->str);
            }
            break;
        }
    }
    
    return gi;
}

static Int4 TXBGetTargetGi(SeqAlignPtr sap, Int4Ptr tax_idp, 
                           CharPtr PNTR accessionp)
{
    Int4 gi;
    SeqIdPtr sip;
    DenseSegPtr dsp;
    DenseDiagPtr ddp;
    StdSegPtr ssp;
    
    switch(sap->segtype) {
    case SAS_DENDIAG:
        ddp = (DenseDiagPtr)(sap->segs);
        if(ddp != NULL && ddp->id != NULL) {
            sip = ddp->id->next; /* target sequence is the second id */
            gi = TXBGetGiFromSeqId(sip, tax_idp, accessionp);
        }
        break;
        
    case SAS_DENSEG:
        dsp = (DenseSegPtr)(sap->segs);
        if(dsp != NULL && dsp->ids != NULL) {
            sip = dsp->ids->next; /* target sequence is the second id */
            gi = TXBGetGiFromSeqId(sip, tax_idp, accessionp);
        }
        break;
        
    case SAS_STD:
        ssp = (StdSegPtr)(sap->segs);
        if(ssp != NULL && ssp->ids != NULL) {
            sip = ssp->ids->next; /* target sequence is the second id */
            gi = TXBGetGiFromSeqId(sip, tax_idp, accessionp);
        }
        break;
    case SAS_DISC:
        for(sap = sap->segs; sap != NULL; sap = sap->next) {
            gi =  TXBGetTargetGi(sap, tax_idp, accessionp);
        }
        break;
        
    default:
        ErrPostEx(SEV_ERROR, 0, 0, "Unusual seqalign type found %d\n", 
                  sap->segtype);
        gi = -1;
        break;
    }
    return gi;
}

static HitObjPtr GetAlignData (SeqAlignPtr sap)
{
    Int4 count = 0;
    ScorePtr score;
    DenseSegPtr seg;
    SeqIdPtr ids;
    Int4 numhits, tax_id;
    Int4Ptr gis, taxoffs, scores;
    FloatHiPtr e_values, bit_scores;
    HitObjPtr hitobj;
    CharPtr accession;

    numhits = CountAligns (sap);
    
    hitobj = HitObjNew();
    hitobj->numhits = numhits;
    gis = hitobj->gis = (Int4Ptr) MemNew (numhits*sizeof(Int4));
    taxoffs = hitobj->taxoffs = (Int4Ptr) MemNew (numhits*sizeof(Int4));
    scores = hitobj->scores = (Int4Ptr) MemNew (numhits*sizeof(Int4));
    e_values = hitobj->e_values = 
        (FloatHiPtr) MemNew (numhits*sizeof(FloatHi));
    bit_scores = hitobj->bit_scores = 
        (FloatHiPtr) MemNew (numhits*sizeof(FloatHi));

    /* Array of accessions for gnl processing */
    hitobj->accs = (CharPtr PNTR)  MemNew (numhits*sizeof(CharPtr));
    
    while (sap) {
        for(score = (ScorePtr) sap->score; score != NULL; 
            score = score->next) {
            if (strcmp("score", score->id->str)==0) {
                scores[0] = score->value.intvalue;
	    } else if (strcmp("e_value", score->id->str)==0) {
                e_values[0] = score->value.realvalue;
            } else if (strcmp("bit_score", score->id->str)==0) {
                bit_scores[0] = score->value.realvalue;
            } 
	}
        
        tax_id = 0;
        if((gis[0] = TXBGetTargetGi(sap, &tax_id, &accession)) == -1) {
            if(tax_id == 0) {
                ErrPostEx(SEV_WARNING, 0, 0,"Failure to extract gi/tax_id "
                          "from SeqAlign. Accession =%s", 
                          accession == NULL? "unknown" : accession);
            } else {
                taxoffs[count] = tax_id;
                hitobj->accs[count] = accession; /* Memory was allocated */
            }
        }
        
        gis++;
        scores++;
        e_values++;
        bit_scores++;
        sap = sap->next;
        count++;
    }
    
    return(hitobj);
}

static Int4 FindTaxid (Int4 taxid, OrgObjPtr orgobj)
{
    Int4 numorgs, i;
    Int4Ptr taxids;
    
    numorgs = orgobj->numorgs;
    taxids = orgobj->taxids;
    
    for (i=0; i<numorgs; i++) {
        if (taxid==taxids[i]) 
            return(i);
    }
    
    return(-1);
}

static void TaxNameFree(TaxNamePtr names, Int4 num_names)
{
    Int4 i;

    if(num_names == 0)
        return;
    
    for(i = 0; i < num_names; i++) {
        MemFree(names[i].name_txt);
        MemFree(names[i].unique_name);
    }
    MemFree(names);
}

static CnamesPtr GetCommonNames (Int4 taxid)
{
    CnamesPtr cnames;
    TaxNamePtr names;
    Int4 i, num_names;
    Uint1 name_code;
    
    cnames = CnamesNew();

    name_code = (Uint1) tax_getClass_cde ("preferred common name");

    num_names = tax_getOrgNames (taxid, &names);

    for (i=0; i < num_names; i++) {
        if (names[i].class_cde == name_code) {
            cnames->preferred = TRUE;
            cnames->count = 1;
            ValNodeAddPointer (&cnames->names, 0, 
                               StringSave (names[i].name_txt));
            break;
	}
    }
    name_code = (Uint1) tax_getClass_cde ("common name");

    for (i=0; i < num_names; i++) {
        if (names[i].class_cde == name_code) {
            cnames->count++;
            ValNodeAddPointer(&cnames->names, 0, 
                              StringSave (names[i].name_txt));
	}
    }
    
    TaxNameFree (names, num_names);
    
    return (cnames);
}

static OrgObjPtr GetOrgData (HitObjPtr hitobj)
{
    Int4 numhits, numorgs, taxid, taxoff, i, namelen;
    Int4Ptr taxids, gis, taxoffs;
    Taxon1DataPtr tdptr;
    OrgRefPtr orgref;
    OrgObjPtr orgobj;
    CharPtr *names;
    CnamesPtr *cnames;
    ValNodePtr *hitlists;
    Char buffer[128];

    if(hitobj == NULL)
        return NULL;
    
    numhits = hitobj->numhits;
    gis = hitobj->gis;
    taxoffs = hitobj->taxoffs;
    
    orgobj = OrgObjNew();
    numorgs = orgobj->numorgs = 0;
    taxids = orgobj->taxids = (Int4Ptr) MemNew(numhits*sizeof(Int4));
    names = orgobj->names = (CharPtr *) MemNew(numhits*sizeof(CharPtr *));
    cnames = orgobj->cnames = 
        (CnamesPtr *) MemNew(numhits*sizeof(CnamesPtr *));
    orgobj->bnames = (BnamePtr *) MemNew(numhits*sizeof(BnamePtr *));
    
    /* these orgobj vectors are all too big - numhits > numorgs */
    
    hitlists = orgobj->hitlists = 
        (ValNodePtr *) MemNew(numhits*sizeof(ValNodePtr *));
    
    for (i=0; i<numhits; i++) {

        /* If taxoffs[i] data is not 0 - it was used to store taxonomy ID
           taken directly from SeqAlign ASN in accordance to gnl||
           convention */
        
        if(taxoffs[i] == 0) {
            taxid = tax1_getTaxId4GI (gis[i]);
        } else {
            taxid = taxoffs[i];
        }
        
        taxoff = FindTaxid (taxid, orgobj);
        
        if (taxoff>=0) {
            taxoffs[i] = taxoff;
            ValNodeAddInt (&hitlists[taxoff], 0, i);
	} else {
            taxoffs[i] = numorgs;
            taxids[numorgs] = taxid;
            
            ValNodeAdd(&hitlists[numorgs]);
            hitlists[numorgs]->choice = 0;
            hitlists[numorgs]->data.intvalue = i;

            tdptr = tax1_getbyid(taxids[numorgs]);

            if (tdptr) {
                orgref = tdptr->org;
                namelen = StrLen(orgref->taxname);
                names[numorgs] = MemNew((namelen+1)*sizeof(Char));
                StrCpy(names[numorgs], orgref->taxname);

                Taxon1DataFree (tdptr);
                
                cnames[numorgs] = GetCommonNames (taxid);

                /* bname[numorgs] = GetBlastName (taxid, tree);    */
                /* get the blast names after building partial tree */
	    } else {
                taxids[numorgs] = -taxid;  /* flag taxid not found */
                /* sprintf (buffer, "taxid %d", taxid); */
                sprintf (buffer, "Unresolved taxid (%d)", taxid);
                names[numorgs] = StringSave (buffer);
                cnames[numorgs] = CnamesNew();
	    }

            numorgs++;
            orgobj->numorgs = numorgs;
	}
    }
    
    /*  for (i=0; i<numhits; i++)
    {
      fprintf(stdout, "%8d %4d %e %6d %s\n", 
      gis[i], hitobj->scores[i], hitobj->e_values[i], 
      taxids[taxoffs[i]], names[taxoffs[i]]);
      } */
    
    return(orgobj);
}

static BnamePtr GetBlastName (Int4 taxid, TreePtr tree)
{
    BnamePtr bname;
    TreeCursorPtr cursor;
    TaxNamePtr names;
    Int4 i, num_names;
    Uint1 name_code;
    
    bname = BnameNew ();
    
    name_code = (Uint1) tax_getClass_cde ("blast name");
    num_names = tax_getOrgNames (taxid, &names);
    
    for (i=0; i < num_names; i++) {
        if (names[i].class_cde == name_code) {
            bname->taxid = taxid;
            bname->name = StringSave (names[i].name_txt);
            TaxNameFree (names, num_names);
            return (bname);
	}
    }

    TaxNameFree(names, num_names);
    cursor = tree_openCursor (tree, NULL, NULL);
    tax_ptree_toTaxId (cursor, taxid, FALSE);

    while (tree_parent (cursor)) {
        taxid = tax1e_getTaxId (cursor);
        num_names = tax_getOrgNames (taxid, &names);
        
        for (i=0; i < num_names; i++) {
            if (names[i].class_cde == name_code) {
                bname->taxid = taxid;
                bname->name = StringSave (names[i].name_txt);
                tree_closeCursor (cursor);
                TaxNameFree(names, num_names);
                return (bname);
	    }
	}
        
        TaxNameFree(names, num_names);
    }
    
    tree_closeCursor (cursor);
    return (bname);
}

static TreePtr GetTreeData(OrgObjPtr orgobj)
{
    TreePtr tree;
    TreeCursorPtr cursor;
    NodeObjPtr nodeobj;
    Int4 i, taxid, numorgs, numhits;
    Uint2 data_size;
    CharPtr name;
    BnamePtr bname;
    
    if (orgobj == NULL) 
        return NULL;
    
    tree = tax_ptree_new();

    numorgs = orgobj->numorgs;
    
    cursor = tree_openCursor(tree, NULL, NULL);
    /* name = tax1e_getTaxName(cursor); for degugging */
    
    for (i=0; i<numorgs; i++) {  /* fill out user data objects */
        
        taxid = orgobj->taxids[i];
        
        if (taxid<=0)
            continue;     /* taxid not found */
        
        tax_ptree_addNode (tree, taxid);
        tax_ptree_toTaxId (cursor, taxid, FALSE);

        /* name = tax1e_getTaxName(cursor); for degugging */
        
        bname = GetBlastName (taxid, tree);
        orgobj->bnames[i] = bname;
        
        numhits = ValNodeLen (orgobj->hitlists[i]);
        
        nodeobj = (NodeObjPtr) cursor->node->sys_data;
        if (nodeobj) {
            /* internal hit - add code here */
            nodeobj->numhits += numhits;
            nodeobj->numorgs++;
            ValNodeAddInt (&nodeobj->orglist, 0, i);
            nodeobj->joinnode = TRUE;
            nodeobj->hitnode = TRUE;
	} else {
            nodeobj = NodeObjNew();
            nodeobj->numhits = numhits;
            nodeobj->numorgs = 1;
            ValNodeAdd (&nodeobj->orglist);
            nodeobj->orglist->choice = 0;
            nodeobj->orglist->data.intvalue = i;
            nodeobj->hitnode = TRUE;
            cursor->node->sys_data = (VoidPtr) nodeobj;
	}
        
        while (tree_parent(cursor)) {
            /* name = tax1e_getTaxName(cursor); for degugging */
            nodeobj = (NodeObjPtr) cursor->node->sys_data;
            
            if (nodeobj) {
                nodeobj->numhits += numhits;
                nodeobj->numorgs++;
                ValNodeAddInt (&nodeobj->orglist, 0, i);
                nodeobj->joinnode = TRUE;
                break;
            } else {
                nodeobj = NodeObjNew();
                nodeobj->numhits = numhits;
                nodeobj->numorgs = 1;
                ValNodeAdd (&nodeobj->orglist);
                nodeobj->orglist->choice = 0;
                nodeobj->orglist->data.intvalue = i;
                cursor->node->sys_data = (VoidPtr) nodeobj;
            }
        }
        
        while (tree_parent(cursor)) {
            /* name = tax1e_getTaxName(cursor); for degugging */	  
            nodeobj = (NodeObjPtr) cursor->node->sys_data;
            
            if (nodeobj) {
                nodeobj->numhits += numhits;
                nodeobj->numorgs++;
                ValNodeAddInt (&nodeobj->orglist, 0, i);
            } else {
                ErrPostEx(SEV_ERROR, 0,0, "Missing Node Object\n");
            }
        }
    }

    if(cursor->node->sys_data == NULL) {
        /* Here we have to clean all data allocated so far */
        tree_closeCursor(cursor);
        return NULL;
    }

    tree_closeCursor(cursor);

    return (tree);
}

static ValNodePtr ValNodeDiff (ValNodePtr vnp1, ValNodePtr vnp2)
{
    ValNodePtr vnp, vnp_ret;
    Int4 val1, val2;
    
    vnp = ValNodeNew(NULL);
    vnp->choice = 0;
    vnp->data.intvalue = 1;
    vnp->next = (ValNodePtr) NULL;
    
    while (vnp1) {
        if (!vnp2) {
            ValNodeAddInt (&vnp, 0, vnp1->data.intvalue);
            vnp1 = vnp1->next;
            continue;
	}
        
        val1 = vnp1->data.intvalue;
        val2 = vnp2->data.intvalue;
        
        if (val1<val2) {
            ValNodeAddInt (&vnp, 0, val1);
            vnp1 = vnp1->next;
	} else if (val1==val2) {
            vnp1 = vnp1->next;
            vnp2 = vnp2->next;
	} else {
            vnp1 = vnp1->next;
	}
    }
    
    vnp_ret = vnp->next;
    MemFree(vnp);
    return(vnp_ret);
}

static void ValNodeIntCpy (ValNodePtr PNTR to, ValNodePtr from)
{
    if (from == NULL || to == NULL) 
        return;

    if (!(*to)) {
        ValNodeAdd(to);
        (*to)->choice = 0;
        (*to)->data.intvalue = from->data.intvalue;
    } else {
        ValNodeAddInt (to, 0, from->data.intvalue);
    }
    
    from = from->next;
    while (from) {
        ValNodeAddInt (to, 0, from->data.intvalue);
        from = from->next;
    }
}

static LinObjPtr GetLinData (TreePtr tree, Int4 focus)
{
    LinObjPtr linobj, linobjlast;
    TreeCursorPtr cursor, focus_cursor;
    NodeObjPtr parent_nodeobj, nodeobj;
    CharPtr name, focus_name;   /* for debugging */
    Int4 taxid;
    
    focus_cursor = tree_openCursor (tree, NULL, NULL);
    tax_ptree_toTaxId (focus_cursor, focus, FALSE);
    
    /* focus_name = tax1e_getTaxName (focus_cursor); for debugging */
    
    cursor = tree_openCursor(tree, NULL, NULL);

    if((nodeobj = (NodeObjPtr) cursor->node->sys_data) == NULL)
        return NULL;
    
    linobj = linobjlast = (LinObjPtr) NULL;
    
    while (!nodeobj->joinnode) {
        if(tree_child (cursor) == FALSE)
            break;
        nodeobj = (NodeObjPtr) cursor->node->sys_data;
    }
    
    taxid = tax1e_getTaxId (cursor);
    /* name = tax1e_getTaxName (cursor); for debugging */
    
    while (taxid>0) {
        if (!linobj) {
            linobj = LinObjNew();
            linobj->name = tax1e_getTaxName (cursor);
            linobj->taxid = tax1e_getTaxId (cursor);
            linobjlast = linobj;
	} else {
            linobjlast->next = LinObjNew();
            linobjlast->next->last = linobjlast;   /* doubly linked */
            linobjlast = linobjlast->next;
            linobjlast->name = tax1e_getTaxName (cursor);
            linobjlast->taxid = tax1e_getTaxId (cursor);
	}
        
        parent_nodeobj = nodeobj;
        
        tree_child (cursor);
        /* name = tax1e_getTaxName (cursor); for debugging */
        nodeobj = (NodeObjPtr) cursor->node->sys_data;
        
        while (!tree_isDescendant (cursor, focus_cursor)) {
            if (!tree_sibling(cursor)) {
                break;
            }
            /* name = tax1e_getTaxName (cursor); for debugging */
            nodeobj = (NodeObjPtr) cursor->node->sys_data;
	}
        
        if (nodeobj->numorgs>1) {
            taxid = tax1e_getTaxId (cursor);
            linobjlast->orglist = ValNodeDiff (parent_nodeobj->orglist,
                                               nodeobj->orglist);
	} else {
            taxid = 0;
            ValNodeIntCpy (&linobjlast->orglist, parent_nodeobj->orglist);
	}
        
        tax_ptree_toTaxId (cursor, taxid, FALSE);
        nodeobj = (NodeObjPtr) cursor->node->sys_data;
        
        while (!nodeobj->joinnode) {
            if(tree_child (cursor) == FALSE)
                break;
            /* name = tax1e_getTaxName (cursor); for debugging */
            nodeobj = (NodeObjPtr) cursor->node->sys_data;
	}
    }
    
    tree_closeCursor(cursor);
    tree_closeCursor(focus_cursor);
    
    return (linobj);
}

static void taxreport_maxlen (TreeCursorPtr cursor, Int2 depth, Int2Ptr maxlen)
{
    NodeObjPtr nodeobj;
    CharPtr name;
    Int2 namelen;
    Int4 incoming_taxid;
    
    if (cursor == NULL) 
        return;

    incoming_taxid = tax1e_getTaxId (cursor);
    
    nodeobj = (NodeObjPtr) cursor->node->sys_data;

    while (!nodeobj->joinnode) {
        if (!tree_child (cursor)) 
            break; /* terminal node */
        else nodeobj = (NodeObjPtr) cursor->node->sys_data;
    }
    
    name = tax1e_getTaxName (cursor);
    namelen = 2*depth + StrLen(name);
    *maxlen = MAX(*maxlen, namelen);
    
    if (tree_child (cursor)) {
        depth++;
        taxreport_maxlen (cursor, depth, maxlen);
        while (tree_sibling (cursor))
            taxreport_maxlen (cursor, depth, maxlen);
    }
    
    tax_ptree_toTaxId (cursor, incoming_taxid, FALSE);

    MemFree(name);
    return;
}

static void taxreport_fn (FILE* outfile, TreeCursorPtr cursor, Int2 depth, 
                          Int2 maxlen, Int2 maxhits, Int2 maxorgs)
{
    NodeObjPtr nodeobj;
    ValNodePtr intermediates, iname;
    ValNodePtr children, child;
    Int4 incoming_taxid, taxid;
    CharPtr name;
    Int2 namelen;
    Int2 i;
    
    if (cursor == NULL) 
        return;

    incoming_taxid = tax1e_getTaxId (cursor);
    intermediates = (ValNodePtr) NULL;
    children = (ValNodePtr) NULL;
    
    nodeobj = (NodeObjPtr) cursor->node->sys_data;
    while (!nodeobj->joinnode) {
        name = tax1e_getTaxName (cursor);
        
        if (!tree_child (cursor)) {
            MemFree(name);
            break; /* terminal node */
        }
        ValNodeAddPointer (&intermediates, 0, (VoidPtr) name);
        nodeobj = (NodeObjPtr) cursor->node->sys_data;
    }

    name = tax1e_getTaxName (cursor);
    namelen = 2*depth + StrLen(name);
    for (i=0; i<depth; i++) fputs(". ", outfile);
    fprintf (outfile, "%s", name);

    MemFree(name);

    fputc(' ', outfile);
    for (i=namelen; i<=maxlen; i++) fputc('.', outfile);
    fputc(' ', outfile);

    fprintf (outfile, "%5d hits %4d orgs ",
             nodeobj->numhits, nodeobj->numorgs);
    if (!intermediates)
        fputc ('\n', outfile);
    else  {
        fputs("[", outfile);
        for (iname=intermediates; iname->next; iname=iname->next) {
            fprintf (outfile, "%s; ", (CharPtr)iname->data.ptrvalue);
	}
        fprintf (outfile, "%s]\n", (CharPtr)iname->data.ptrvalue);
    }

    ValNodeFreeData (intermediates);

    if (tree_child (cursor)) {
        taxid = tax1e_getTaxId (cursor);
        children = ValNodeNew (NULL);
        children->choice = 0;
        children->data.intvalue = taxid;
        while (tree_sibling (cursor)) {     /* reverse order of children */
            taxid = tax1e_getTaxId (cursor);
            child = ValNodeNew (NULL);
            child->choice = 0;
            child->data.intvalue = taxid;
            child->next = children;
            children = child;
	}
        
        depth++;
        
        for (child=children; child; child=child->next) {
            taxid = child->data.intvalue;
            tax_ptree_toTaxId (cursor, taxid, FALSE);
            taxreport_fn (outfile, cursor, depth, maxlen, maxhits, maxorgs);
	}

        ValNodeFree (children);
    }
    
    tax_ptree_toTaxId (cursor, incoming_taxid, FALSE);

    return;
}

Boolean TXBGetDefLine(ReadDBFILEPtr rdfp, Int4 seqno, 
                      SeqIdPtr PNTR sip, CharPtr PNTR defline)
{
    Int4 count;
    CharPtr new_defline, defline_ptr, new_defline_ptr;
    
    readdb_get_descriptor(rdfp, seqno, sip, defline);
    
    count = 0;
    new_defline = NULL;
    if (*defline != NULL) {
        defline_ptr = *defline;
        
        while (*defline_ptr != NULLB) {
            count++;
            if (*defline_ptr == READDB_DEF_SEPARATOR) {
                /* Two spaces for every ctrl-A as it will be replaced by 2. */
                count++;
            }
            defline_ptr++;
        }
        
        if (count != 0) {
            new_defline = (CharPtr)Nlm_Malloc((count+1)*sizeof(Char));
            new_defline_ptr = new_defline;
            defline_ptr = *defline;
            while (*defline_ptr != NULLB) {
                if (*defline_ptr == READDB_DEF_SEPARATOR) { 	
                    *new_defline_ptr = ' ';
                    new_defline_ptr++;
                    *new_defline_ptr = '>';
                    new_defline_ptr++;
                } else {
                    *new_defline_ptr = *defline_ptr;
                    new_defline_ptr++;
                }
                defline_ptr++;
            }
            *new_defline_ptr = NULLB;
            MemFree(*defline);
        }
    }
    *defline = new_defline;

    return TRUE;
}
static void TXBRecursiveWalk(TreeCursorPtr cursor)
{
    Int2 s;
    TXC_TreeNodePtr node;
    NodeObjPtr nodeobj;

    if(tree_child(cursor)) {
	do {
	    TXBRecursiveWalk(cursor);
	}
	while(tree_sibling(cursor));
	tree_parent(cursor);
    }
    /* node= tree_getNodeData(cursor, &s); */

    /* if((node->tax_id & 0x1) != 0) ++nof_nodes; */

    if(cursor->node->sys_data != NULL) {
        nodeobj = (NodeObjPtr) cursor->node->sys_data;
        NodeObjFree(nodeobj);
    }
}

static void TXBFreeSysData(TreePtr tree)
{
    TreeCursorPtr cursor;
    
    cursor = tree_openCursor (tree, NULL, NULL);
    
    tax1e_toNode(cursor, 1);
    
    TXBRecursiveWalk(cursor);
    
    tree_closeCursor(cursor);
}

#define BUFFLEN 128
#define CNAMELEN 25
#define LINLINELEN 256

static void TXBHtmlReportInternal (FILE *outfile, HitObjPtr hitobj, 
                                   OrgObjPtr orgobj, TreePtr tree, Int4 focus,
                                   CharPtr database, CharPtr link_href,
                                   CharPtr window_name, Boolean show_gi)
{
    LinObjPtr linobj, lo_next, lo_last;
    Int4 taxid, gi, gi_last, seqno, tx_length;
    Int2 numtaxa, numorgs, numhits, linlines, longname;
    Int2 i, j, k, depth, save_depth; 
    Int2 orgoff, hitoff;
    Int2Ptr taxa_name_len;
    Int2Ptr size_names, size_hits, size_bnames;
    Int2 max_names, max_hits, max_bnames;
    SeqIdPtr seqid;
    BioseqPtr bsp;
    ValNodePtr org, hitlist, valnode;
    CharPtr name, cname, blastname, title;
    CharPtr ptr, ptr_start, eval_buff_ptr;
    Boolean found_next_one;
    BnamePtr bname;
    Char padchar;
    Char buffer[BUFFLEN], tx_defline[256], eval_buff[16];
    TreeCursorPtr cursor;
    NodeObjPtr nodeobj;
    ReadDBFILEPtr rdfp;

    Nlm_FloatHi evalue;

    if (!outfile) return;
    if (!hitobj) return;
    if (!orgobj) return;
    if (!tree) return;
    
    if (focus <= 0) 
        focus = orgobj->taxids[0];

    if((linobj = GetLinData (tree, focus)) == NULL)
        return;

    if((rdfp = readdb_new(database, !hitobj->db_is_na)) == NULL)
        return;

    /* Index */

    fprintf(outfile, "<b>Index</b>\n"
            "<UL>\n"
            "<LI><a href=#lineage>Lineage Report</a></LI>\n"
            "<LI><a href=#organism_report>Organism Report</a></LI>\n"
            "<LI><a href=#taxonomy_report>Taxonomy Report</a></LI>\n"
            "<LI><a href=http://www.ncbi.nlm.nih.gov/blast/taxblasthelp.html>Help</a></LI>\n"
            "</UL>\n");

    /*---- print out the lineage report ----*/
    
    fprintf (outfile, "<B><A NAME=lineage>Lineage Report</A></B><BR>\n");
    
    fprintf (outfile, "<PRE>");
    fprintf (outfile, "<FONT SIZE=-1>");
    
    numorgs = orgobj->numorgs;
    numtaxa = 0;

    for (lo_next=linobj; lo_next; lo_next=lo_next->next) 
        numtaxa++;
    
    linlines = numtaxa + numorgs; 
    
    taxa_name_len = (Int2Ptr) MemNew (numtaxa * sizeof(Int2));
    longname = 0;
    for (depth=0, lo_next=linobj; lo_next; depth++, lo_next=lo_next->next) {
        taxa_name_len[depth] = StrLen(lo_next->name);
        longname = MAX (longname, taxa_name_len[depth]);
    }
    
    for (depth=0, lo_next=linobj; lo_next; 
         depth++, lo_last=lo_next, lo_next=lo_next->next) {

        for (i=0; i<depth; i++) 
            fputs (". ", outfile);

        fprintf (outfile, "<a href=http://www.ncbi.nlm.nih.gov/Taxonomy/Browser/wwwtax.cgi?id=%ld>%s</a>", 
           (long) lo_next->taxid, lo_next->name);
        
        bname = GetBlastName (lo_next->taxid, tree);

        if (bname && bname->name) {
            for (i=taxa_name_len[depth]; 
                 i<=longname; i++) fputc (' ', outfile);
            fprintf (outfile, "[<a href=http://www.ncbi.nlm.nih.gov/Taxonomy/Browser/wwwtax.cgi?id=%ld>%s</a>]", 
               (long) bname->taxid, bname->name);
	}
        fputc ('\n', outfile);

        BnameFree(bname);

    }

    MemFree(taxa_name_len);

    save_depth = depth;
    
    size_names = (Int2Ptr) MemNew (numorgs * sizeof(Int2));
    size_hits = (Int2Ptr) MemNew (numorgs * sizeof(Int2));
    size_bnames = (Int2Ptr) MemNew (numorgs * sizeof(Int2));
    max_names = max_hits = max_bnames = 0;
    
    for (lo_next=lo_last, i=0; lo_next;
         lo_next=lo_next->last, depth--) {
        for (org=lo_next->orglist; org; org=org->next, i++) {
            orgoff = org->data.intvalue;
            size_names[i] = 2*depth + StrLen(orgobj->names[orgoff]);
            if (orgobj->cnames[orgoff]->count > 0) {
                cname = (CharPtr)orgobj->cnames[orgoff]->names->data.ptrvalue;
                size_names[i] += MIN(CNAMELEN, StrLen(cname)) + 3;
	    }
            max_names = MAX(max_names, size_names[i]);
            
            hitlist = (ValNodePtr) orgobj->hitlists[orgoff];
            numhits = ValNodeLen (hitlist);
            size_hits[i] = (Int2) log10(numhits);
            max_hits = MAX(max_hits, size_hits[i]);
            
            blastname = orgobj->bnames[orgoff]->name;
            if (blastname) size_bnames[i] = StrLen(blastname);
            else size_bnames[i] = 0;
            max_bnames = MAX(max_bnames, size_bnames[i]);
	}
    }
    
    for (lo_next=lo_last, depth=save_depth, i=0, padchar='-';
         lo_next;
         lo_next=lo_next->last, depth--, padchar='-') {
        for (org=lo_next->orglist; org; org=org->next, i++, padchar='.') {
            orgoff = org->data.intvalue;
            taxid = orgobj->taxids[orgoff];
            for (j=0; j<depth; j++) fputs(". ", outfile);
            fprintf (outfile, "<A HREF=http://www.ncbi.nlm.nih.gov/Taxonomy/Browser/wwwtax.cgi?id=%ld>%s</A>", 
               (long) taxid, orgobj->names[orgoff]);
            if (orgobj->cnames[orgoff]->count > 0) {
                cname = (CharPtr)orgobj->cnames[orgoff]->names->data.ptrvalue;
                StrNCpy (buffer, cname, BUFFLEN);
                if (StrLen(buffer) > CNAMELEN) {
                    buffer[CNAMELEN] = '\0';
                    buffer[CNAMELEN-1] = '.';
                    buffer[CNAMELEN-2] = '.';
                    buffer[CNAMELEN-3] = '.';
		}
                fprintf (outfile, " (%s)", buffer);
	    }
            
            fputc (' ', outfile);
            for (j=size_names[i]; j<=max_names; j++) 
                fputc (padchar, outfile);
            fputc (' ', outfile);
            
            hitlist = (ValNodePtr) orgobj->hitlists[orgoff];
            numhits = ValNodeLen(hitlist);
            hitoff = hitlist->data.intvalue;
            
            fprintf (outfile, "%4.0ld ", (long) hitobj->bit_scores[hitoff]);
            /*	  fprintf (outfile, "%-9.2e ", hitobj->e_values[hitoff]); */
            
            for (j=size_hits[i]; j<max_hits; j++)
                fputc(' ', outfile);
            if (numhits==1) fprintf(outfile, "<a href=#%d>1 hit</a> ", taxid);
            else fprintf(outfile, "<a href=#%d>%d hits</a>", taxid, numhits);
            
            if (size_bnames[i]>0)
                fprintf (outfile, " [<a href=http://www.ncbi.nlm.nih.gov/Taxonomy/Browser/wwwtax.cgi?id=%ld>%s</a>]", 
                    (long) orgobj->bnames[orgoff]->taxid, orgobj->bnames[orgoff]->name);
            else fputs(" []", outfile);
            for (j=size_bnames[i]; j<=max_bnames; j++) fputc(' ', outfile);

            if((gi = hitobj->gis[hitoff]) > 0) {
                seqno = readdb_gi2seq(rdfp, gi, NULL);
            } else {
                seqno = readdb_acc2fasta(rdfp, hitobj->accs[hitoff]);
            }

            if(seqno < 0) {
                ErrPostEx(SEV_ERROR, 0, 0, "Gi %d was not found in the BLAST database", gi);
                title = StringSave("Title was not found");
            } else {
                TXBGetDefLine(rdfp, seqno, &seqid, &title);
                SeqIdSetFree(seqid);
            }

            if (StringLen(title) > 60) 
                title[59] = '\0';

            if(title == NULL)   /* gnl case may have no title */
                title = StringSave(hitobj->accs[hitoff]);

            if(gi > 0) {            
                if (hitobj->query_is_na) {
                    fprintf (outfile, " <a href=http://www.ncbi.nlm.nih.gov:80/entrez/query.fcgi?cmd=Retrieve&db=Nucleotide&list_uids=%d&dopt=GenBank>%s</a>", gi, title);
                } else {
                    fprintf (outfile, " <a href=http://www.ncbi.nlm.nih.gov:80/entrez/query.fcgi?cmd=Retrieve&db=Protein&list_uids=%d&dopt=GenPept>%s</a>", gi, title);
                }
            } else {
                fprintf (outfile, " %s", title);
            }

            MemFree (title);
            fputc('\n', outfile);
	}
    }

    fprintf (outfile, "</FONT>");
    fprintf (outfile, "</PRE>");
    
    MemFree (size_names);
    MemFree (size_hits);
    MemFree (size_bnames);
    
    /*---- print out the organism report ----*/
    
    fprintf (outfile, "<HR>\n");
    fprintf (outfile, "<B><A NAME=organism_report>Organism Report</A></B>\n");
    
    fprintf (outfile, "<PRE>");
    /*    fprintf (outfile, "<FONT SIZE=-1>"); */
    
    numorgs = orgobj->numorgs;
    for (i=0; i<numorgs; i++) {
        fprintf (outfile, "<a name=%d></a>", orgobj->taxids[i]);
        fprintf (outfile, "  <b><a href=http://www.ncbi.nlm.nih.gov/Taxonomy/Browser/wwwtax.cgi?id=%ld>%s</a></b>", 
               (long) orgobj->taxids[i], orgobj->names[i]);
        
        if (orgobj->cnames[i]->count > 0) {
            cname= (CharPtr) orgobj->cnames[i]->names->data.ptrvalue;
            if (orgobj->cnames[i]->preferred ||
                orgobj->cnames[i]->count==1)
                fprintf (outfile, " (%s)", cname);
            else fprintf (outfile, " (%s, ...)", cname);
	}
        
        if (orgobj->bnames[i] && orgobj->bnames[i]->name) {
            fprintf (outfile, " [<a href=http://www.ncbi.nlm.nih.gov/Taxonomy/Browser/wwwtax.cgi?id=%ld>%s</a>]", 
                 (long) orgobj->bnames[i]->taxid, orgobj->bnames[i]->name);
	}
        
        fprintf (outfile, " taxid %d", orgobj->taxids[i]);
        
        fputc('\n', outfile);
        
        gi_last = 0;
        for (valnode=orgobj->hitlists[i]; valnode; valnode=valnode->next) {
            hitoff = valnode->data.intvalue;

            if((gi = hitobj->gis[hitoff]) > 0) {
                if (gi == gi_last) 
                    continue;
                else 
                    gi_last = gi;

                seqno = readdb_gi2seq(rdfp, gi, NULL);
            } else {
                seqno = readdb_acc2fasta(rdfp, hitobj->accs[hitoff]);
            }
            if(seqno < 0) {
                ErrPostEx(SEV_ERROR, 0, 0, "Gi %d was not found in the BLAST database", gi);
                title = StringSave("Title was not found");
                sprintf(buffer, "SeqId not found");
            } else {
                TXBGetDefLine(rdfp, seqno, &seqid, &title);
                if(title == NULL)
                    title = StringSave(hitobj->accs[hitoff]);
                SeqIdWrite (seqid, buffer, PRINTID_FASTA_LONG, BUFFLEN);
                SeqIdSetFree(seqid);
            }

            /* At this point we have seqid printed into buffer and
               full defline printed into title - now we will format
               definition line analogos to BLAST definition lines */

            /* Adjusting seqid buffer to show only accession */

            found_next_one = FALSE;
            if (!show_gi) {
                if (StringNCmp(buffer, "gi|", 3) == 0) {
                    ptr = &buffer[3];
                    while (*ptr != NULLB && *ptr != ' ') { /* Is there another ID beside the GI? */
                        if (*ptr == '|') {
                            ptr++;
                            found_next_one = TRUE;
                            break;
                        }
                        ptr++;
                    }
                }
            }
            if (found_next_one == FALSE) {
                ptr = buffer;
                ptr_start = buffer;
            } else {
                ptr_start = ptr;
            }

            if((tx_length = 60 - StringLen(ptr_start)) < 0)
                tx_length = 50; /* impossible ! */

            if(StringLen(title) > tx_length) {

                title[tx_length-1] = '.';
                title[tx_length-2] = '.';
                title[tx_length-3] = '.';
                title[tx_length] = NULLB;
            }

            /*  if (strlen(title)>60) 
                title[59]='\0'; */

            if(gi > 0) {            
                if (hitobj->query_is_na) {
                    fprintf (outfile, " <a href=http://www.ncbi.nlm.nih.gov:80/entrez/query.fcgi?cmd=Retrieve&db=Nucleotide&list_uids=%d&dopt=GenBank>%s</a>", gi, ptr_start);
                } else {
                    fprintf (outfile, " <a href=http://www.ncbi.nlm.nih.gov:80/entrez/query.fcgi?cmd=Retrieve&db=Protein&list_uids=%d&dopt=GenPept>%s</a>", gi, ptr_start);
                }
            } else {
                fprintf (outfile, " %s", ptr_start);
            }
            
            fprintf(outfile, " %s", title);

            if((k = tx_length - StringLen(title)) > 0) {
                int l;
                for(l = 0; l < k; l++)
                    fputc(' ', outfile);
            }

            evalue = hitobj->e_values[hitoff];
            eval_buff_ptr = eval_buff;

            if (evalue < 1.0e-180) {
                sprintf(eval_buff, "0.0");
            } else if (evalue < 1.0e-99) {
                sprintf(eval_buff, "%2.0le", evalue);
                eval_buff_ptr++; 	/* Knock off digit. */
            } else if (evalue < 0.0009) {
                sprintf(eval_buff, "%3.0le", evalue);
            } else if (evalue < 0.1) {
                sprintf(eval_buff, "%4.3lf", evalue);
            } else if (evalue < 1.0) {
                sprintf(eval_buff, "%3.2lf", evalue);
            } else if (evalue < 10.0) {
                sprintf(eval_buff, "%2.1lf", evalue);
            } else {
                sprintf(eval_buff, "%5.0lf", evalue);
            }


            fprintf(outfile, "    <a href=%s#%d TARGET=\"%s\">%4.0ld</a>  %s", 
                    link_href == NULL? "" : link_href, 
                    hitobj->gis[hitoff],
                    window_name == NULL? "" : window_name,
                    (long) hitobj->bit_scores[hitoff],  
                    eval_buff);

            fputc('\n', outfile);
            
            MemFree(title);
	}
        fputc('\n', outfile);
    }
    
    /*   fprintf (outfile, "</FONT>"); */
    fprintf (outfile, "</PRE>");
    
    /*---- print out the taxonomy report ----*/
    
    fprintf (outfile, "<HR>\n");
    fprintf (outfile, "<B><A NAME=taxonomy_report>Taxonomy Report</A></B>\n");
    
    fprintf (outfile, "<PRE>");
    fprintf (outfile, "<FONT SIZE=-1>");

    cursor = tree_openCursor (tree, NULL, NULL);
    /* name = tax1e_getTaxName (cursor); */
    depth = 0;
    longname = 0;
    taxreport_maxlen (cursor, depth, &longname);

    nodeobj = cursor->node->sys_data;
    taxreport_fn (outfile, cursor, depth, longname,
                  nodeobj->numhits, nodeobj->numorgs);

    tree_closeCursor(cursor);

    fprintf (outfile, "</FONT>");
    fprintf (outfile, "</PRE>");
    
    /*---- end of reports ----*/
    
    fprintf (outfile, "</BODY>\n");
    fprintf (outfile, "</HTML>\n");

    TXBFreeSysData(tree);
    LinObjFree(linobj);
    readdb_destruct(rdfp);

    return;
}

void TXBHtmlReport(SeqAlignPtr sap, FILE *outfile, Boolean query_is_na,
                   Boolean db_is_na, CharPtr database, CharPtr link_href, 
                   CharPtr window_name, Boolean show_gi)
{
    HitObjPtr hitobj = NULL;
    OrgObjPtr orgobj = NULL;
    TreePtr tree = NULL;
    
    if((hitobj = GetAlignData (sap)) != NULL) {
        hitobj->query_is_na = query_is_na;
        hitobj->db_is_na = db_is_na;
        
        tax1_init();
        if((orgobj = GetOrgData (hitobj)) != NULL) {
            tree = GetTreeData (orgobj);
        }
    }

    fprintf(outfile, "<HTML>"
            "<TITLE>Tax BLAST Report</TITLE>\n"
            "<BODY BGCOLOR=\"#FFFFFF\" LINK=\"#0000FF\""
            " VLINK=\"#660099\" ALINK=\"#660099\">\n");
    fprintf (outfile, "<a name=\"taxblast\"></a>"
             "<CENTER><H3>Tax BLAST Report</H3></CENTER>\n");

    if(tree == NULL) {

        fprintf(outfile, "No valid taxids found in the alignment<HR>\n");
        fflush(outfile);
        HitObjFree(hitobj);
        OrgObjFree(orgobj);
        tax1_fini();
        return;
    }
    
    TXBHtmlReportInternal(outfile, hitobj, orgobj, tree, 0 /*focus*/,
                          database, link_href, window_name, show_gi);
    
    tax1_fini();

    tree_delete(tree);
    HitObjFree(hitobj);
    OrgObjFree(orgobj);

    return;
}

/* -----------------------------------------------------------
   Here is list of functions used to create taxonomy names database as
   part of Blast database 
   ----------------------------------------------------------- */

#define CODE_NAME_SCI           0 
#define CODE_NAME_PREF_COMMON   7 
#define CODE_NAME_COMMON        8 
#define CODE_NAME_PREF_ACRONYM  11 
#define CODE_NAME_ACRONYM       3 
#define CODE_NAME_BLAST         13 

static Int2 SpeciesRank= 26;

typedef struct TAXData {
    TreePtr tax_tree;
    TreeCursorPtr cursor;
} TAXData, PNTR TAXDataPtr;


Boolean GetSuperKingdom(TreeCursorPtr cursor, Int4 tax_id, CharPtr s_king)
{
    TreeNodeId nid= tree_getId(cursor);
    TXC_TreeNodePtr tnp;
    CharPtr super_name;
    Uint2 s;
    Int2 rank;

    tax_ptree_toTaxId (cursor, tax_id, FALSE);
    super_name = NULL;
    s_king[0] = NULLB;

    while(tree_parent(cursor)) {
	if((tnp= tree_getNodeData(cursor, &s)) != NULL) {
	    rank= tnp->flags & 0xFF;

	    if(rank > SpeciesRank) {
		continue;
	    }
            
	    if(tnp->tax_id < 2) break;
            
	    if((tnp->flags & TXC_GBHIDE) == 0) {
		super_name = tnp->node_label;                
	    }
	}
    }

    if(super_name == NULL) {
        StringCpy(s_king, "-");
        return FALSE;
    }

    if(!StringICmp(super_name, "Eukaryota")) {
        StringCpy(s_king, "E");
    } else if (!StringICmp(super_name, "Bacteria")) {
        StringCpy(s_king, "B");
    } else if (!StringICmp(super_name, "Archaea")) {
        StringCpy(s_king, "A");
    } else if (!StringICmp(super_name, "Viruses") || 
               !StringICmp(super_name, "Viroids")) {
        StringCpy(s_king, "V");
    } else {
        StringCpy(s_king, "-");
    }

    return TRUE;
}

Boolean FDBTaxCallback (RDBTaxLookupPtr tax_lookup, Int4 tax_id)
{
    RDBTaxNamesPtr tnames;
    TreePtr tax_tree;
    TreeCursorPtr cursor;
    TAXDataPtr tdp;
    TaxNamePtr res_names, tnp;
    Int4 count, i;

    if(tax_lookup == NULL)
        return FALSE;

    /* If this tax id already exist just return OK */
    if(tax_lookup->tax_array[tax_id] != NULL)
        return TRUE;

    if(tax_id == 0)
        return TRUE;
    
    tdp = (TAXDataPtr) tax_lookup->tax_data;
    
    tax_tree= tdp->tax_tree;
    cursor= tdp->cursor;

    tnames = MemNew(sizeof(RDBTaxNames));

    count = tax_getOrgNames(tax_id, &res_names);

    for(i = 0; i < count; i++) {
        switch(res_names[i].class_cde) {
            
        case CODE_NAME_SCI:
            tnames->sci_name = StringSave(res_names[i].name_txt);
            break;
        case CODE_NAME_PREF_COMMON:
        case CODE_NAME_COMMON:
        case CODE_NAME_PREF_ACRONYM:
        case CODE_NAME_ACRONYM:
            if(tnames->common_name == NULL) /* Only first will be stored */
                tnames->common_name = StringSave(res_names[i].name_txt);
            break;
        case CODE_NAME_BLAST:
            tnames->blast_name = StringSave(res_names[i].name_txt);
            break;
        }
    }

    TaxNameFree(res_names, count);

    if(tnames->blast_name == NULL) {
        BnamePtr bname;

        if((bname = GetBlastName(tax_id, tax_tree)) != NULL) {
            tnames->blast_name = bname->name;
            MemFree(bname);
        }
    }

    if(tnames->sci_name == NULL) {
        /* MemFree(tnames);
           return FALSE; */
        tnames->sci_name = StringSave("Unknown");
    }
    
    if(tnames->common_name == NULL)
        tnames->common_name = StringSave(tnames->sci_name);
    if(tnames->blast_name == NULL)
        tnames->blast_name = StringSave("Unknown");

    GetSuperKingdom(cursor, tax_id, tnames->s_king);

    tnames->tax_id = tax_id;
    tax_lookup->tax_array[tax_id] = tnames;

#ifdef DEBUG
    printf("FDBTaxCallback(%d): %s,%s,%s,%c%c%c\n",
            tnames->tax_id,tnames->sci_name,tnames->common_name,
            tnames->blast_name,tnames->s_king[0],tnames->s_king[1],
            tnames->s_king[2]);
#endif
    tax_lookup->taxids_in_db++;

    return TRUE;
}
static Int4 getTotalTaxIdCount(TreeCursorPtr cursor)
{
    TreePtr tax_tree;
    Int4 total_taxids_count, count;
    
    if(cursor == NULL) {
        tax_tree = tax1e_getTaxTreePtr();  /* get pointer to taxonomy tree */
        cursor = tree_openCursor(tax_tree, NULL, NULL);  /* open cursor */
    }
    
    /* Total recursive walk with conting all nodes */
    total_taxids_count = 0;
    if(tree_child(cursor)) {

	do {
            total_taxids_count++;
	    count = getTotalTaxIdCount(cursor);
            total_taxids_count += count;
	} while(tree_sibling(cursor));

	tree_parent(cursor);
    }

    return total_taxids_count;
}

RDBTaxLookupPtr RDTaxLookupInit(void)
{
    RDBTaxLookupPtr tax_lookup;
    TAXDataPtr tax_data;
    Int2 retries = 3, secs2wait = 10;

    /* connect to taxonomy service */
    while (retries--) {
        if (tax1_init())
            break;
        else
            sleep(secs2wait);
    }

    if (!tax1_isAlive()) {
        ErrPostEx(SEV_FATAL, 1,0, 
                "Could not initialize taxonomy service: RDTaxLookupInit");
        return NULL;
    }
    
    /* download all the taxonomy tree from the server */
    tax1e_invokeChildren(-1); 
    
    tax_lookup = MemNew(sizeof(RDBTaxLookup));
    /*tax_lookup->all_taxid_count = getTotalTaxIdCount(NULL);*/
    tax_lookup->all_taxid_count = tax1e_maxTaxId();
    
    tax_lookup->tax_array = MemNew(sizeof(RDBTaxNamesPtr)*tax_lookup->all_taxid_count);

    tax_data = MemNew(sizeof(TAXData));
    tax_data->tax_tree = tax1e_getTaxTreePtr();  /* get pointer to taxonomy tree */
    tax_data->cursor = tree_openCursor(tax_data->tax_tree, NULL, NULL);  /* open cursor */
    tax_lookup->tax_data = (VoidPtr) tax_data;

    return tax_lookup;
}

/* Remove all entries from taxonomy lookup structure but don't close the 
 * connection to the taxonomy service */
RDBTaxLookupPtr RDTaxLookupReset(RDBTaxLookupPtr tax_lookup)
{
    RDBTaxNamesPtr tnames;
    Int4 i;

    for(i = 0; i < tax_lookup->all_taxid_count; i++) {        
        
        tnames = tax_lookup->tax_array[i];
        
        if (tnames != NULL) {
            MemFree(tnames->sci_name);
            MemFree(tnames->common_name);
            MemFree(tnames->blast_name);
            MemFree(tnames);
        }
        tax_lookup->tax_array[i] = tnames = NULL;
    }
   	tax_lookup->taxids_in_db = 0; 
}

void RDTaxLookupClose(RDBTaxLookupPtr tax_lookup)
{
	tax_lookup = RDTaxLookupReset(tax_lookup);

    MemFree(tax_lookup->tax_data);
    MemFree(tax_lookup->tax_array);
    MemFree(tax_lookup);
    
    tax1_fini();
    return;
}

#if defined (TAX_BLAST_MAIN)

#define NUMARG (sizeof(myargs)/sizeof(myargs[0]))

static Args myargs [] = {
    { "Input ASN.1 File (SeqAnnot)",             /* 0 */
      NULL, NULL, NULL, FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL },
    { "Sequence is DNA",                         /* 1 */
      "F", NULL, NULL, TRUE, 'p', ARG_BOOLEAN, 0.0, 0, NULL },
    { "Database used to get SeqAnnot ASN.1",     /* 2 */
      "nr", NULL, NULL, TRUE, 'd', ARG_STRING, 0.0, 0, NULL },
    { "Output file name",                        /* 3 */
      "stdout", NULL, NULL, TRUE, 'o', ARG_FILE_OUT, 0.0, 0, NULL }
};

Int2 Main (void)
{
    AsnIoPtr aip;
    SeqAnnotPtr sap;
    Boolean is_na = FALSE;
    FILE *outfile;
    Char ofile[128];
    
    if (!GetArgs("txblast", NUMARG, myargs)) {
        return 1;
    }
    
    if (myargs[1].intvalue) 
        is_na = TRUE;
    
    if((aip = AsnIoOpen(myargs[0].strvalue, "r")) == NULL) {
        ErrPostEx(SEV_FATAL, 1,0, "AsnIoOpen failure\n");
        return 1;
    }
    
    if((sap = SeqAnnotAsnRead (aip, NULL)) == NULL) {
        ErrPostEx(SEV_FATAL, 1,0,"SeqAlignAsnRead failure\n");
        return 1;
    }

    if(StringCmp(myargs[3].strvalue, "stdout")) {
        sprintf (ofile, "%s.html", myargs[0].strvalue);
        outfile = FileOpen(ofile, "w");
    } else {
        outfile = FileOpen(myargs[3].strvalue, "w");
    }
    
    TXBHtmlReport((SeqAlignPtr)sap->data, outfile, is_na, is_na, 
                  myargs[2].strvalue, NULL, NULL, FALSE);
    
    FileClose(outfile);
    
    AsnIoClose(aip);
    SeqAnnotFree(sap);
    
    return (0);
}
#endif
