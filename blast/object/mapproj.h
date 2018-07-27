/*  mapproj.h
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
* File Name:  mapproj.h
*
* Author:  Jonathan Kans
*   
* Version Creation Date: 3/23/97
*
* $Revision: 6.6 $
*
* File Description:  Mappings file for automatic ASN.1 code generation
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* ==========================================================================
*/

#include <sequtil.h>
#include <mmdbapi.h>
#include <objpubme.h>

#if defined(NLM_EXTERN_LOADS)
#undef NLM_EXTERN_LOADS
#endif
#define NLM_EXTERN_LOADS { if (! AllObjLoad()) return FALSE; \
                         if (! objpubmeAsnLoad()) return FALSE; \
                         if (! objmmdb1AsnLoad()) return FALSE; \
                         if (! objmmdb2AsnLoad()) return FALSE; \
                         if (! objmmdb3AsnLoad()) return FALSE; }

#include <objproj.h> /* needed for ProjectPtr definition */

static Int2 LIBCALL ProjectLabel (ProjectPtr proj, CharPtr buffer, Int2 buflen, Uint1 content)
{
	CharPtr name;
	Int2 diff = 0;
	static CharPtr proj_types[22] = {
		"Project..",
		"PubMed UID",
		"Protein UID",
		"Nucleotide UID" ,
		"Sequence UID" ,
		"Genome UID" ,
		"Structure UID" ,
		"PubMed ID",
		"Protein ID",
		"Nucleotide ID" ,
		"Sequence ID" ,
		"Genome ID" ,
		"Structure ID" ,
		"PubMed Entry",
		"Protein Entry",
		"Nucleotide Entry" ,
		"Sequence Entry" ,
		"Genome Entry" ,
		"Structure Entry" ,
		"Annotation" ,
		"Location" ,
		"Subproject" };
	ValNodePtr pip;

	if ((proj == NULL) || (buflen < 1))
		return 0;

	pip = proj->data;
	if (pip == NULL)
		return 0;

	if (pip->choice <= 21)
		name = proj_types[pip->choice];
	else
		name = proj_types[0];

	switch (content)
	{
		case OM_LABEL_BOTH:
		case OM_LABEL_CONTENT:
		case OM_LABEL_SUMMARY:
		case OM_LABEL_TYPE:
		default:
			diff = LabelCopy(buffer, name, buflen);
	}
	return diff;
}

static Uint2 LIBCALLBACK ProjectSubTypeFunc (Pointer ptr)
{
	ValNodePtr pip;

	if (ptr == NULL)
		return 0;
	pip = ((ProjectPtr)ptr)->data;
	if (pip == NULL)
		return 0;
	return (Uint2)pip->choice;
}

