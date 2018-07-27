/*===========================================================================
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
* File Name:  pdbeast.h
*
* Author:  Yanli Wang
*
* Initial Version Creation Date: 03/09/1997
*
* File Description:
*         Header file for an interactive tool for taxonomy assignment to structure sequence
*
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: pdbeast.h,v $
* Revision 1.1  2002/12/17 15:13:43  ywang
* pdbeast related tools updated
*
*
* ==========================================================================
*/

#define seq_num 1
#define seq_set_num 2
#define org_choice_num 6
#define biosource_choice_num 23
#define pdbsource_choice_num 21

#define RawBioseqDir_text  "/net/keystone/usr/people/ywang/work/taxonomy/RawBioseq/"
#define RawBioseqDir_binary  "/net/clobber/usr/people8/bryant/MMDB.Bioseq/"
#define BioseqDir_text   "/net/keystone/usr/people/ywang/work/taxonomy/Bioseq_text/"
#define BioseqDir_binary   "/net/keystone/usr/people/ywang/work/taxonomy/Bioseq_binary/"

#define http_arg1 "http://www.ncbi.nlm.nih.gov/blast/Blast.cgi?QUERY_BELIEVE_DEFLINE=yes&QUERY="

#define http_arg2  "&DATABASE=nr&HITLIST_SIZE=10&FILTER=L&EXPECT=10&FORMAT_TYPE=HTML&PROGRAM=blastp&CLIENT=web&SERVICE=plain&NCBI_GI=on&PAGE=Proteins&CMD=Put&AUTO_FORMAT=Semiauto&FORMAT_OBJECT=Alignment&ALIGNMENT_VIEW=Pairwise&SHOW_OVERVIEW=on"

#define http_arg3 "&DATABASE=nr&HITLIST_SIZE=10&FILTER=L&EXPECT=10&FORMAT_TYPE=HTML&PROGRAM=blastn&CLIENT=web&SERVICE=plain&NCBI_GI=on&PAGE=Nucleotides&CMD=Web&AUTO_FORMAT=Semiauto&FORMAT_OBJECT=Alignment&ALIGNMENT_VIEW=Pairwise&SET_DEFAULTS.x=34&SET_DEFAULTS.y=8&SHOW_OVERVIEW=on&END_OF_HTTPGET=Yes"
 
Char RawSeqEntryName[120], SeqEntryName[120];
Char EntryCode[120];

AsnIoPtr aip1, aip2;
Char pdbsource_str[10240];

Boolean IsBioseq, IsBioseqSet, Pdbsource, Biosource, Biosource_chain[MaxChainNum], BioOrg, BioOrg_chain[MaxChainNum];
Boolean Bio_source, ReplaceSource;
Boolean OrgFound;
Boolean AllAssign, PartAssign;
Boolean SingleChain;
Boolean TextInEntry, TextOutEntry;
Boolean TaxAssign, EntryOpen;
Boolean chain_assign[MaxChainNum];
Boolean SingleEntry, EntryFromList;

Char chain_orgName[MaxChainNum][120];

FILE *IndexList;

SeqEntryPtr root_sep;
OrgRefPtr my_org;
OrgRefPtr my_org_chain[MaxChainNum];

Int4 nchain;
Char chain[MaxChainNum];
Char selected_chain, last_click_chain;
Int2 selected_chain_num, current_chain_num;
Char CurrentOrgName[120];
