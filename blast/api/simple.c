/*  simple.c
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
* File Name:  simple.c
*
* Author:  James Ostell
*   
* Version Creation Date: 7/12/91
*
* $Revision: 6.2 $
*
* File Description:  Simple sequence loader
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: simple.c,v $
* Revision 6.2  1999/03/11 23:32:07  kans
* sprintf casts
*
* Revision 6.1  1999/01/11 16:07:00  kans
* obj mgr type load OBJ_FASTA as SimpleSeq
*
* Revision 6.0  1997/08/25 18:07:30  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/06/19 18:39:06  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.0  1996/05/28 13:23:23  ostell
* Set to revision 5.0
*
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 2.2  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/
#include <simple.h>		   /* the pub interface */
#include <all.h>        /* the AsnTool header */
#include <objmgr.h>

/*****************************************************************************
*
*   Bioseq ObjMgr Routines
*
*****************************************************************************/
static CharPtr simplename = "Simple";

static Pointer LIBCALLBACK SimpleSeqNewFunc (void)
{
	return (Pointer) SimpleSeqNew();
}

static Pointer LIBCALLBACK SimpleSeqFreeFunc (Pointer data)
{
	return (Pointer) SimpleSeqFree ((SimpleSeqPtr) data);
}

static Pointer LIBCALLBACK SimpleSeqAsnReadFunc (AsnIoPtr aip, AsnTypePtr atp)
{
	return (Pointer) SimpleSeqAsnRead (aip);
}


/*****************************************************************************
*
*   AllAsnLoad()
*
*****************************************************************************/
NLM_EXTERN Boolean AllAsnLoad (void)

{
    static Boolean loaded = FALSE;
    AsnModulePtr  amp;
    AsnTypePtr    atp;

    if (loaded)
        return TRUE;
    loaded = TRUE;

    if (! AsnLoad()) {
        loaded = FALSE;
        return FALSE;
    }

    amp = AsnAllModPtr ();
    atp = AsnTypeFind (amp, "Bioseq");

    ObjMgrTypeLoad(OBJ_FASTA, "Bioseq", simplename, "Simple Sequence",
        atp, SimpleSeqNewFunc, SimpleSeqAsnReadFunc, NULL,
        SimpleSeqFreeFunc, NULL, NULL);

    return TRUE;
}

/*****************************************************************************
*
*   SimpleSeqNew()
*
*****************************************************************************/
NLM_EXTERN SimpleSeqPtr SimpleSeqNew (void)

{
    return MemNew(sizeof(SimpleSeq));
}

/*****************************************************************************
*
*   SimpleSeqFree(ssp)
*
*****************************************************************************/
NLM_EXTERN SimpleSeqPtr SimpleSeqFree (SimpleSeqPtr ssp)

{
    Int2 i;

    if (ssp == NULL)
        return NULL;

    for (i = 0; i < ssp->numid; i++)
        MemFree(ssp->id[i]);
    MemFree(ssp->title);
    BSFree(ssp->seq);
    return MemFree(ssp);
}

/*****************************************************************************
*
*   ConvertToIupac(atp, ptr, len)
*
*****************************************************************************/
static ByteStorePtr ConvertToIupac (AsnTypePtr atp, ByteStorePtr bsp, Int4 seqlen)

{
    static Char twobit[4] = {'A','C','G','T'};
    static Char fourbit[16] = { '-','A','C','M','G','R','S','V','T','W','Y',
        'H','K','D','B','N'};
    ByteStorePtr bsnew;
    Int2 bitctr;
    Uint1 base, b;

    if ((atp == SEQ_DATA_iupacna) || (atp == SEQ_DATA_iupacaa))
        return bsp;              /* all set */

    bsnew = BSNew((Uint4)seqlen);
    if (bsnew == NULL)
        return NULL;

    BSSeek(bsp, 0, SEEK_SET);
    BSSeek(bsnew, 0, SEEK_SET);
    
    if (atp == SEQ_DATA_ncbi2na)
    {
        bitctr = 0;
        while (seqlen)
        {
            if (! bitctr)
            {
                b = (Uint1) BSGetByte(bsp);
                bitctr = 4;
            }
            base = b & (Uint1) 192;
            base >>= 6;
            b <<= 2;
            BSPutByte(bsnew, (Uint1)twobit[base]);
            bitctr--;
            seqlen--;
        }
    }
    else if (atp == SEQ_DATA_ncbi4na)
    {
        bitctr = 0;
        while (seqlen)
        {
            if (! bitctr)
            {
                b = (Uint1) BSGetByte(bsp);
                bitctr = 2;
            }
            base = b & (Uint1) 240;
            base >>= 4;
            b <<= 4;
            BSPutByte(bsnew, (Uint1)fourbit[base]);
            bitctr--;
            seqlen--;
        }
    }
    BSFree(bsp);
    return bsnew;
}

/*****************************************************************************
*
*   SimpleSeqAsnRead(aip)
*       assumes aip is positioned at the start of a bioseq
*
*****************************************************************************/
NLM_EXTERN SimpleSeqPtr SimpleSeqAsnRead (AsnIoPtr aip)

{
    AsnTypePtr atp;
    DataVal av;
    SimpleSeqPtr ssp;
    static char * ids[7] = {
        "gibbsq",
        "gibbmt",
        "giim",
        "genbank",
        "embl",
        "pir",
        "swissprot" };
    Int2 bestid=50, idnum=0, i;
    Char buf[60];
    CharPtr tmp;
    Boolean is_num, got_one;

    if (aip == NULL)
        return NULL;

    if (! AllAsnLoad())
    {
        Message(MSG_ERROR, "Cannot load asnall.ld");
    }

    AsnIoReset(aip);    /* make sure its at initial state */
    ssp = SimpleSeqNew();

    atp = BIOSEQ;
    atp = AsnReadId(aip, amp, atp);
    AsnReadVal(aip, atp, &av);    /* start struct */
    while (AsnGetLevel(aip))    /* while not done with struct */
    {
        atp = AsnReadId(aip, amp, atp);
        if (atp == BIOSEQ_id)   /* get best sequence identifier */
        {
            i = 0;
            AsnReadVal(aip, atp, &av);   /* start SET OF */
            while ((atp = AsnReadId(aip, amp, atp)) != BIOSEQ_id)
            {
                is_num = FALSE;
                got_one = FALSE;
                if (atp == SEQ_ID_gibbsq)
                {
                    i = 0;
                    is_num = TRUE;
                    got_one = TRUE;
                }
                else if (atp == SEQ_ID_gibbmt)
                {
                    i = 1;
                    is_num = TRUE;
                    got_one = TRUE;
                }
                else if (atp == GIIMPORT_ID_id)
                {
                    i = 2;
                    is_num = TRUE;
                    got_one = TRUE;
                }
                else if (atp == SEQ_ID_genbank)
                    i = 3;
                else if (atp == SEQ_ID_embl)
                    i = 4;
                else if (atp == SEQ_ID_pir)
                    i = 5;
                else if (atp == SEQ_ID_swissprot)
                    i = 6;
                else if (atp == TEXTSEQ_ID_name)
                    got_one = TRUE;
                else if (atp == TEXTSEQ_ID_accession)
                    got_one = TRUE;

                if (got_one)
                {
                    AsnReadVal(aip, atp, &av);
                    if (is_num)
                        sprintf(buf, "%s_%ld", ids[i], (long) av.intvalue);
                    else
                    {
                        tmp = StringMove(buf, ids[i]);
                        tmp = StringMove(tmp, "_");
                        tmp = StringMove(tmp, (CharPtr)av.ptrvalue);
                        MemFree(av.ptrvalue);
                    }
                    ssp->id[idnum] = StringSave(buf);
                    if (i <= bestid)
                    {
                        ssp->bestid = idnum;
                        bestid = i;
                    }
                    idnum++;
                }
                else
                    AsnReadVal(aip, atp, NULL);
            }
            AsnReadVal(aip, atp, NULL);
        }  /* end processing Bioseq-ids */
        else if (atp == SEQDESC_title)
        {
            if (ssp->title == NULL)
            {
                AsnReadVal(aip, atp, &av);
                ssp->title = (CharPtr)av.ptrvalue;
            }
            else
                AsnReadVal(aip, atp, NULL);
        }
        else if (atp == SEQ_INST_repr)
        {
            AsnReadVal(aip, atp, &av);
            if (! ((av.intvalue == 2) || (av.intvalue == 4)))  /* raw only */
            {
                Message(MSG_ERROR, "SimpleSeq can only read raw or const Bioseqs");
                SimpleSeqFree(ssp);
                return NULL;
            }
        }
        else if (atp == SEQ_INST_length)
        {
            AsnReadVal(aip, atp, &av);
            ssp->seqlen = av.intvalue;
        }
        else if (atp == SEQ_INST_seq_data)
        {
            AsnReadVal(aip, atp, NULL);   /* CHOICE */
            atp = AsnReadId(aip, amp, atp);
            AsnReadVal(aip, atp, &av);
            ssp->seq = ConvertToIupac(atp, (ByteStorePtr) av.ptrvalue, ssp->seqlen);
            if (ssp->seq == NULL)
            {
                Message(MSG_ERROR, "Cannot allocate memory for %ld residues",
                    ssp->seqlen);
                SimpleSeqFree(ssp);
                return NULL;
            }
        }
        else
            AsnReadVal(aip, atp, NULL);   /* skip everything else */
    }
    ssp->numid = idnum;
    return ssp;
}

/*****************************************************************************
*
*   SimpleSeqPrint(ssp, fp, as_fasta)
*
*****************************************************************************/
NLM_EXTERN Boolean SimpleSeqPrint (SimpleSeqPtr ssp, FILE *fp, Boolean as_fasta)

{
    char localbuf[255];    /* for MS windows */
    Int2 i, numid;
    Int4 seqlen;
    CharPtr tmp;
    ByteStorePtr bsp;

    if ((ssp == NULL) || (fp == NULL))
        return FALSE;

    if (as_fasta)
    {
        tmp = StringMove(localbuf, ">");
        tmp = StringMove(tmp, ssp->id[ssp->bestid]);
        tmp = StringMove(tmp, " ");
        tmp = StringNCpy(tmp, ssp->title, (size_t)200);
        fprintf(fp, "%s\n", localbuf);
    }
    else
    {
        numid = ssp->numid;
        for (i = 0; i < numid; i++)
        {
            StringMove(localbuf, ssp->id[i]);
            fprintf(fp, "%s\n", localbuf);
        }
        StringNCpy(localbuf, ssp->title, (size_t)250);
        fprintf(fp, "%s\n", localbuf);
    }

    i = 0;
    bsp = ssp->seq;
    BSSeek(bsp, 0, SEEK_SET);
    seqlen = ssp->seqlen;
    while (seqlen)
    {
        if (i == 60)
        {
            localbuf[i] = '\0';
            fprintf(fp, "%s\n", localbuf);
            i = 0;
        }
        localbuf[i] = (char) BSGetByte(bsp);
        i++;
        seqlen--;
    }
    if (i)
    {
        localbuf[i] = '\0';
        fprintf(fp, "%s\n", localbuf);
    }
    return TRUE;
}


