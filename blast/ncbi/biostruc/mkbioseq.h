/* mkbioseq.h
 *
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
 * File Name: mkbioseq.h
 *
 * Author: Ken Addess
 *
 * $Log: mkbioseq.h,v $
 * Revision 6.1  1998/07/17 18:56:01  madej
 * Created by Ken Addess.
 *
 */

/*****************************************************************************
*
*   mkbioseq.h
*
*****************************************************************************/

#ifndef _MKBIOSEQ_
#define _MKBIOSEQ_

#include <ncbi.h>
#include <asn.h>
#include <mmdbapi.h>

#include "strimprt.h"
#define MAXNUM 50

SeqEntryPtr LIBCALL CreateSeqEntry PROTO 
((BiostrucSourcePtr bssp, BiostrucGraphPtr bsgp, BiostrucModelPtr bsmp, ValNodePtr descr, Int4 nchn));
SeqIdPtr LIBCALL MakePDBId PROTO((BiostrucSourcePtr bssp, MoleculeGraphPtr mgp, DbtagPtr dtp));
SeqIdPtr LIBCALL MakeLocalID PROTO((Int4 mid, MoleculeGraphPtr mgp, DbtagPtr dtp));
SeqIdPtr LIBCALL MakeGId PROTO((Int4 gi));
BiostrucPtr LIBCALL readBiostruc PROTO((CharPtr filename, Int2 mode, Int4 mdlLvl));
ValNodePtr LIBCALL MakeBioseqDescr PROTO((MoleculeGraphPtr mgp, ValNodePtr bioseq_descr));
Uint1 LIBCALL MakeBioseqMol PROTO((MoleculeGraphPtr mgp));
Int4 LIBCALL CountNumOfResidues PROTO((MoleculeGraphPtr mgp));
Boolean LIBCALL isBiopoly PROTO((Int4 molecule_id, MoleculeGraphPtr currentbp));
Boolean LIBCALL isHet PROTO((Int4 molecule_id, MoleculeGraphPtr currenthet));
Int4 LIBCALL getHetIdx PROTO((Int4 molecule_id, MoleculeGraphPtr currenthet));
ByteStorePtr LIBCALL AddSeqData PROTO((MoleculeGraphPtr mgp, Uint1 mol, Int4 length, BiostrucGraphPtr bsgp, BiostrucResidueGraphSetPtr stdDictionary));
CharPtr LIBCALL getStdOlcode PROTO
((Int4 standard_graphs_id, Int4 residue_graph_id, BiostrucResidueGraphSetPtr std));
CharPtr LIBCALL getNstdOlcode PROTO((ResidueGraphPtr nrg));
ResidueGraphPtr LIBCALL getNstdResGraph PROTO((Int4 rgid, BiostrucGraphPtr bsgp));
CharPtr LIBCALL rmvSpace PROTO((CharPtr str));
SeqAnnotPtr LIBCALL AddNstdSeqAnnot PROTO((MoleculeGraphPtr mgp, SeqIdPtr id, BiostrucGraphPtr bsgp));
Boolean LIBCALL isNstd PROTO((ResidueGraphPtr rgp));
SeqAnnotPtr LIBCALL AddSecDomToSeqAnnot PROTO((BiostrucFeaturePtr bsfp, CharPtr name, SeqAnnotPtr seq_annot, SeqIdPtr id, Int4 num));
Int4 LIBCALL findChnidx PROTO((Int4 mol_id, Int4 nbp, MoleculeGraphPtr bp));
ValNodePtr LIBCALL MakeHetValNode PROTO((MoleculeGraphPtr nhet, BiostrucResidueGraphSetPtr stdDictionary, ResidueGraphPtr rg));
CharPtr LIBCALL getResNam PROTO((ResidueGraphPntrPtr rgpp, BiostrucResidueGraphSetPtr stdDictionary, ResidueGraphPtr rg));
CharPtr LIBCALL getResComm PROTO((ResidueGraphPntrPtr rgpp, BiostrucResidueGraphSetPtr stdDictionary, ResidueGraphPtr rg));
SeqAnnotPtr LIBCALL AddHetToSeqAnnot 
PROTO((SeqAnnotPtr seq_annot, SeqIdPtr id, ValNodePtr hetval, ValNodePtr pvnThePoints, Int4 rescount));
SeqAnnotPtr LIBCALL AddDisulToSeqAnnot PROTO((SeqAnnotPtr seq_annot, Int4 residx1, Int4 residx2, SeqIdPtr id1, SeqIdPtr id2));
Int4 LIBCALL 
getAtomElementIdx PROTO ((Int4 molecule_id, Int4 residue_id, Int4 atom_id, BiostrucGraphPtr bsgp, BiostrucResidueGraphSetPtr stdDictionary));
ResidueGraphPtr LIBCALL getResGraph PROTO((ResidueGraphPntrPtr rgpp, BiostrucGraphPtr bsgp, BiostrucResidueGraphSetPtr stdDictionary));


#endif
