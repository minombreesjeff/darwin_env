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
* File Name:  interface.h 
*
* Author:  Yanli Wang
*
* Initial Version Creation Date: 03/09/1997
*
* File Description:
*         Header for an interactive tool for taxonomy assignment to structure sequence
*
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: interface.h,v $
* Revision 1.1  2002/12/17 15:13:43  ywang
* pdbeast related tools updated
*
*
* ==========================================================================
*/
FILE *seq_list;
FILE *file_index;
FILE *log_pdbeast;
Char files[10][120] = {"all.OCT96", "bioseq.lis"};
Char seq_entry[MaxEntryNum][12], seq_entry_TaxState[MaxEntryNum];
Int4 entry_num;
Int4 current_seq_entry_num;
Int4 index_num[MaxEntryNum];

#define dir_index "/net/keystone/usr/people/ywang/work/taxonomy/index/"
#define dir_pdb "/usr/ncbi/db/pdb/Current/"

    GrouP g;
    IteM  i;
    MenU  m;
    MenU  edit;
    PrompT p;
    WindoW w;
    WindoW wpdb;
    Int2 val;
    ChoicE c;
    LisT lchain;

TexT    pdb_source;
DoC     org;
DoC     chain_panel;
DoC     pdbfile_panel;
Boolean redo = FALSE;
Boolean EntryListOn = FALSE;
