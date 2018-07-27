/* 
 * ===========================================================================
 *
 *                             COPYRIGHT NOTICE
 *               National Center for Biotechnology Information
 *
 *  This software/database is a "United States Government Work" under the
 *  terms of the United States Copyright Act.  It was written as part of
 *  the author's official duties as a Government employee and thus cannot
 *  be copyrighted.  This software/database is freely available to the
 *  public for use without a copyright notice.  Restrictions cannot be
 *  placed on its present or future use.
 *
 *  Although all reasonable efforts have been taken to ensure the accuracy
 *  and reliability of the software and data, the National Library of
 *  Medicine (NLM) and the U. S. Government do not and cannot warrant the
 *  performance or results that may be obtained by using this software or
 *  data.  The NLM and the U. S. Government disclaim all warranties as to
 *  performance, merchantability or fitness for any particular purpose.
 *
 *  Please see that the author is suitably cited in any work or product
 *  based on this material.
 *
 * ===========================================================================
 *
 * RCS $Id: objentr.c,v 6.4 1998/08/24 18:42:17 kans Exp $
 *
 * Author:   Greg Schuler, Jim Ostell, Jonathan Epstein
 *
 * Version Creation Date:   6/28/94
 *
 * File Description:  ASN.1 object support for EntrezInfo data block & docsums.
 *
 * Modifications:  
 * --------------------------------------------------------------------------
 * Rev   Date      Name        Description of modification
 * ----  --------  ----------  ----------------------------------------------
 * 1.1   06-28-94  Schuler     This file created from definitions extracted
 *                             from accentr.h
 * 1.2   08-22-94  Schuler     Added handling for new fields.  
 *
 * 05-19-95 Schuler     Added rcs Log directive for automatic insertion of
 *                      modification comments.
 *
 * Revision $Log: objentr.c,v $
 * Revision Revision 6.4  1998/08/24 18:42:17  kans
 * Revision fixed -v -fd warnings
 * Revision
 * Revision Revision 6.3  1998/03/26 23:23:16  yaschenk
 * Revision adding CdRomAsnLoad() as extern function to load Asn tree
 * Revision
 * Revision Revision 6.2  1997/10/10 20:19:13  kans
 * Revision div->tag now filled by StringNCpy_0
 * Revision
 * Revision Revision 6.1  1997/10/07 22:09:48  volodya
 * Revision new data fields (entrezDate, pStatus, pLanguage, pPubType) have added to DocSum structure
 * Revision
 * Revision Revision 6.0  1997/08/25 18:13:16  madden
 * Revision Revision changed to 6.0
 * Revision
 * Revision Revision 5.4  1997/06/26 21:55:46  vakatov
 * Revision [PC] DLL'd "ncbicdr.lib", "ncbiacc.lib", "ncbinacc.lib" and "ncbicacc.lib"
 * Revision
 * Revision Revision 5.3  1997/05/01 21:19:41  epstein
 * Revision fix leak detected by Purify
 * Revision
 * Revision 5.2  1997/03/05  19:23:43  brandon
 * minor fix.
 *
 * Revision 5.1  1997/03/05  19:00:09  brandon
 * added objet loaders for expanded EntrezInfo structure (including link data)
 *
 * Revision 5.0  1996/05/28  13:55:34  ostell
 * Set to revision 5.0
 *
 * Revision 4.1  1995/10/17  20:26:29  epstein
 * remove to free dsp->extra in DocSumFree
 *
 * Revision 4.0  1995/07/26  13:50:32  ostell
 * force revision to 4.0
 *
 * Revision 1.8  1995/07/11  14:54:53  epstein
 * move DocSumFree from accentr.c and fix docsum object loaders
 *
 * Revision 1.7  1995/07/11  12:25:14  epstein
 * add DocSum object loaders
 *
 * Revision 1.6  1995/05/16  14:36:20  schuler
 * Automatic comment insertion enabled
 *
 *
 * ==========================================================================
 */

#define REVISION_STR "$Revision: 6.4 $"



/*** #define WRITE_NEW_STUFF ***/


#include <cdrom.h>
#include <objentr.h>
#include <accentr.h>

static Boolean NEAR FieldDataAsnWrite PROTO((EntrezFieldDataPtr p, AsnIoPtr stream, AsnType *type_start));
static Boolean NEAR TypeDataAsnWrite PROTO((EntrezTypeDataPtr p, AsnIoPtr stream, AsnType *type_start, int num_fields));

static EntrezTypeInfo* EntrezTypeInfo_AsnRead (EntrezTypeInfo *tinf, AsnIo *aio, AsnType *type);
static Boolean EntrezTypeInfo_AsnWrite (const EntrezTypeInfo *tinf, AsnIo *aio, AsnType *type);

static EntrezFieldInfo* EntrezFieldInfo_AsnRead (EntrezFieldInfo *fld, AsnIo *stream, AsnType *type0);
static Boolean EntrezFieldInfo_AsnWrite (const EntrezFieldInfo *fld, AsnIo *stream, AsnType *type);

static EntrezLinkInfo* EntrezLinkInfo_AsnRead (EntrezLinkInfo *fld, AsnIo *stream, AsnType *type0);
static Boolean EntrezLinkInfo_AsnWrite (const EntrezLinkInfo *fld, AsnIo *stream, AsnType *type);

#define EntrezDivInfo_New(a)   EntrezDivInfo_Construct(MemNew(sizeof(EntrezDivInfo),a))
#define EntrezDivInfo_Free(a)  (void)MemFree(EntrezDivInfo_Desctuct(a))
static  EntrezDivInfo* EntrezDivInfo_Construct (EntrezDivInfo *div, int type_ct);
static  EntrezDivInfo* EntrezDivInfo_Destruct (EntrezDivInfo *div);
static  EntrezDivInfo* EntrezDivInfo_AsnRead (EntrezDivInfo *div,  AsnIo *ain, AsnType *type_start);
static  Boolean EntrezDivInfo_AsnWrite (const EntrezDivInfo *div,  AsnIo *ain, AsnType *type_start);

static CdDate * CdDate_AsnRead (CdDate *date, AsnIo *ain, AsnType *type_start);
static Boolean CdDate_AsnWrite (const CdDate *date, AsnIo *aout, AsnType *atp);


#define Boolean_AsnWrite(a,b,c,d)  \
	{ if (d) 	\
		{ DataVal val;  val.boolvalue = (a);  \
		(d) = AsnWrite(b,c,&val); } }
	
#define Integer_AsnWrite(a,b,c,d)  \
	{ if (d) 	\
		{ DataVal val;  val.intvalue = (a);  \
		(d) = AsnWrite(b,c,&val); } } \
	
#define String_AsnWrite(a,b,c,d)  \
	{ if ((d) && ((a)!=NULL) )		\
		{ DataVal val;  val.ptrvalue = (void*)(a); \
		(d) = AsnWrite(b,c,&val); } } \
		 
	

static int _type_ct;
static int _fld_ct;
static int _div_ct;

static void LIBCALL EntrezLinkInfoFree(EntrezLinkInfo *LinkInfo)
{
  MemFree(LinkInfo -> tag);
  MemFree(LinkInfo -> name);
  MemFree(LinkInfo -> descr);
  MemFree(LinkInfo);
}


/*****************************************************************************
*
*   NetEntAsnLoad()
*
*****************************************************************************/

static Boolean loaded = FALSE;

NLM_EXTERN Boolean LIBCALL
CdRomAsnLoad(void)
{
    if (loaded)
        return TRUE;
        loaded = TRUE;

        if (! AllObjLoad()) {
                loaded = FALSE;
                return FALSE;
        }
        if (! AsnLoad()) {
                loaded = FALSE;
                return FALSE;
        }

        return TRUE;
}



/*****************************************************************************
*
*   ASN.1 object support:
*
*
*	EntrezInfoFree
*	EntrezInfoAsnRead
*	EntrezInfoAsnWrite
*
*****************************************************************************/

NLM_EXTERN EntrezInfoPtr LIBCALL EntrezInfoFree (EntrezInfoPtr info)
{
	Int2 i;

	if (info == NULL)
		return NULL;

	MemFree(info->volume_label);
	for (i = 0; i < info->type_count; i++)
		MemFree(info->types[i].fields);
	MemFree(info->types);
	MemFree(info->descr);
	MemFree(info->huff_left);
	MemFree(info->huff_right);
	if (info->type_names != NULL)
	{
		for (i = 0; i < info->type_count; i++)
			MemFree(info->type_names[i]);
		MemFree(info->type_names);
	}
	if (info->field_names != NULL)
	{
		for (i = 0; i < info->field_count; i++)
			MemFree(info->field_names[i]);
		MemFree(info->field_names);
	}
	for (i = 0; i < info -> link_count; i++)
	  EntrezLinkInfoFree(info -> link_info + i);
	return MemFree(info);
}


NLM_EXTERN EntrezInfoPtr LIBCALL EntrezInfoAsnRead (AsnIo *stream, AsnType *type_start)
{
	DataVal val;
	AsnType *atp, *oldtype;
    EntrezInfoPtr info;
	EntrezTypeDataPtr currtdp = NULL;
	EntrezFieldDataPtr currfdp = NULL;
	EntrezTypeInfo *tinf;
	EntrezFieldInfo *fld;
	EntrezLinkInfo *link;
	EntrezDivInfo *div;
	Int2 typectr, fieldctr;
	Int2 huff_left_ctr, huff_right_ctr;
	int i;
	size_t bytes;

	if ((stream == NULL) || (! AsnLoad()))
		return NULL;

	if (type_start == NULL)           /* Cdrom-inf ::= (self contained) */
		atp = AsnReadId(stream, amp, CDROM_INF);
	else
		atp = AsnLinkType(type_start, CDROM_INF);    /* link in local tree */
	oldtype = atp;

	AsnReadVal(stream, atp, &val);    /* read the START_STRUCT */

	info = (EntrezInfoPtr) MemNew(sizeof(EntrezInfo));
	typectr = 0;
	fieldctr = 0;
	huff_left_ctr = 0;
	huff_right_ctr = 0;

	while ((atp = AsnReadId(stream, amp, atp)) != oldtype)
	{
		AsnReadVal(stream, atp, &val);

		if (atp == CDROM_INF_volume_label)
			info->volume_label = (CharPtr)val.ptrvalue;
		else if (atp == CDROM_INF_version)
			info->version = (short)val.intvalue;
		else if (atp == CDROM_INF_issue)
			info->issue = (short)val.intvalue;
		else if (atp == CDROM_INF_format)
			info->format = (short)val.intvalue;
		else if (atp == CDROM_INF_descr)
			info->descr = (char*)val.ptrvalue;
		else if (atp == CDROM_INF_no_compression)
			info->no_compression = val.boolvalue;
		else if (atp == CDROM_INF_huff_count)
		{
			info->huff_count = (Int2)val.intvalue;
			info->huff_left = (Int2Ptr) MemNew(sizeof(Int2) * info->huff_count);
			info->huff_right = (Int2Ptr) MemNew(sizeof(Int2) * info->huff_count);
		}
		else if (atp == CDROM_INF_huff_left_E)
		{
			info->huff_left[huff_left_ctr] = (short)val.intvalue;
			huff_left_ctr++;
		}
		else if (atp == CDROM_INF_huff_right_E)
		{
			info->huff_right[huff_right_ctr] = (short)val.intvalue;
			huff_right_ctr++;
		}
		else if (atp == CDROM_INF_type_count)
		{
			info->type_count = (short)val.intvalue;
			info->type_names = (char**) MemNew(sizeof(char*) * info->type_count);
			info->types = (EntrezTypeData*) MemNew(sizeof(EntrezTypeData) * (size_t)info->type_count);
			bytes = (size_t)info->type_count * sizeof(EntrezTypeInfo);
			info->type_info = tinf = (EntrezTypeInfo*) MemNew(bytes);
		}
		else if (atp == CDROM_INF_type_names_E)
		{
			info->type_names[typectr] = (CharPtr)val.ptrvalue;
			typectr++;
		}
		else if (atp == CDROM_INF_type_bucket_size)
			info->type_bucket_size = (Int2)val.intvalue;
		else if (atp == CDROM_INF_field_count)
		{
			info->field_count = (short)val.intvalue;
		 	info->field_names = (char **) MemNew(sizeof(char*) * info->field_count);
		 	bytes = (size_t)info->field_count * sizeof(EntrezFieldInfo);
		 	info->field_info = fld = (EntrezFieldInfo*) MemNew(bytes);
		}
		else if (atp == CDROM_INF_link_count)
		{
			info->link_count = val.intvalue;
		 	bytes = (size_t)info->link_count * sizeof(EntrezLinkInfo);
		 	info->link_info = link = (EntrezLinkInfo*) MemNew(bytes);
		}
		else if (atp == CDROM_INF_field_names_E)
		{
			info->field_names[fieldctr] = (char*)val.ptrvalue;
			fieldctr++;
		}
		else if (atp == CDROM_INF_field_bucket_size)
			info->field_bucket_size = (short)val.intvalue;


		else if (atp == CDROM_INF_types_E)
		{
			if (val.intvalue == START_STRUCT)
			{
				if (currtdp == NULL)
					currtdp = info->types;
				else
					currtdp++;
				currtdp->fields = (EntrezFieldDataPtr) MemNew(sizeof(EntrezFieldData) * info->field_count);
				currfdp = NULL;   /* reset field data chain */
			}
		}
		else if (atp == TYPEDATA_fields_E)
		{
			if (val.intvalue == START_STRUCT)
			{
				if (currfdp == NULL)
					currfdp = currtdp->fields;
				else
					currfdp++;
			}
		}
		else if (atp == TYPEDATA_num)
			currtdp->num = val.intvalue;
		else if (atp == TYPEDATA_num_uids)
			currtdp->num_uids = val.intvalue;
		else if (atp == TYPEDATA_minuid)
			currtdp->minuid = val.intvalue;
		else if (atp == TYPEDATA_maxuid)
			currtdp->maxuid = val.intvalue;
		else if (atp == TYPEDATA_num_bucket)
			currtdp->num_bucket = (Int2)val.intvalue;
		else if (atp == FIELDDATA_num_terms)
			currfdp->num_terms = val.intvalue;
		else if (atp == FIELDDATA_num_bucket)
			currfdp->num_bucket = (Int2) val.intvalue;

		else if (atp == CDROM_INF_release_date)
			CdDate_AsnRead(&(info->release_date),stream,atp);
		else if (atp == CDROM_INF_close_date)
			CdDate_AsnRead(&(info->close_date),stream,atp);
		
		else if (atp == CDROM_INF_type_info_E)
			EntrezTypeInfo_AsnRead(tinf++,stream,atp);
		else if (atp == CDROM_INF_field_info_E)
			EntrezFieldInfo_AsnRead(fld++,stream,atp);
		else if (atp == CDROM_INF_link_info_E)
			EntrezLinkInfo_AsnRead(link++,stream,atp);

		else if (atp == CDROM_INF_div_count)
		{
			size_t bytes = (size_t)val.intvalue * sizeof(EntrezDivInfo);
			info->div_info = div = (EntrezDivInfo*) MemNew(bytes);
			info->div_count = (short)val.intvalue;
			for (i=0; i<val.intvalue; ++i)
				EntrezDivInfo_Construct(div++,info->type_count);
			div = info->div_info;
		}
		else if (atp == CDROM_INF_div_info_E)
			EntrezDivInfo_AsnRead(div++,stream,atp);

		else
		{
			AsnKillValue(atp,&val);
		}
						
	}
	AsnReadVal(stream, atp, &val);    /* read the END_STRUCT */
	AsnUnlinkType(type_start);       /* unlink local tree */
	return info;
}


NLM_EXTERN Boolean LIBCALL EntrezInfoAsnWrite (EntrezInfo *info, AsnIo *stream, AsnType *type_start)

{
	DataVal val;
	int i;
	AsnType *atp;
	Boolean retval = FALSE;

	if ((stream == NULL) || (! AsnLoad()))
		return FALSE;

	atp = AsnLinkType(type_start, CDROM_INF); /* link local tree */

	if (atp == NULL)
		return FALSE;

	if (info == NULL)
	{
		AsnNullValueMsg(stream, atp);
		goto erret;
	}
	
	_type_ct = info->type_count;
	_fld_ct = info->field_count;
	_div_ct = info->div_count;

	if (! AsnStartStruct(stream, atp))
		goto erret;
	val.ptrvalue = info->volume_label;
	AsnWrite (stream, CDROM_INF_volume_label, &val);
	val.intvalue = info->version;
	AsnWrite (stream, CDROM_INF_version, &val);
	val.intvalue = info->issue;
	AsnWrite (stream, CDROM_INF_issue, &val);
	val.intvalue = info->format;
	AsnWrite (stream, CDROM_INF_format, &val);
	val.ptrvalue = info->descr;
	AsnWrite (stream, CDROM_INF_descr, &val);
	val.boolvalue = info->no_compression;
	AsnWrite (stream, CDROM_INF_no_compression, &val);

	val.intvalue = info->huff_count;
	AsnWrite (stream, CDROM_INF_huff_count, &val);
	AsnStartStruct (stream, CDROM_INF_huff_left);
	for (i = 0; i < info->huff_count; i++)
	{
		val.intvalue = info->huff_left[i];
		AsnWrite(stream, CDROM_INF_huff_left_E, &val);
	}
	AsnEndStruct (stream, CDROM_INF_huff_left);
	AsnStartStruct (stream, CDROM_INF_huff_right);
	for (i = 0; i < info->huff_count; i++)
	{
		val.intvalue = info->huff_right[i];
		AsnWrite(stream, CDROM_INF_huff_right_E, &val);
	}
	AsnEndStruct (stream, CDROM_INF_huff_right);

	val.intvalue = info->type_count;
	AsnWrite (stream, CDROM_INF_type_count, &val);
	AsnStartStruct (stream, CDROM_INF_type_names);
	for (i = 0; i < info->type_count; i++)
	{
		val.ptrvalue = info->type_names[i];
		AsnWrite(stream, CDROM_INF_type_names_E, &val);
	}
	AsnEndStruct (stream, CDROM_INF_type_names);

	val.intvalue = info->type_bucket_size;
	AsnWrite (stream, CDROM_INF_type_bucket_size, &val);

	val.intvalue = info->field_count;
	AsnWrite (stream, CDROM_INF_field_count, &val);
	AsnStartStruct (stream, CDROM_INF_field_names);
	for (i = 0; i < info->field_count; i++)
	{
		val.ptrvalue = info->field_names[i];
		AsnWrite(stream, CDROM_INF_field_names_E, &val);
	}
	AsnEndStruct (stream, CDROM_INF_field_names);

	val.intvalue = info->field_bucket_size;
	AsnWrite (stream, CDROM_INF_field_bucket_size, &val);

	AsnStartStruct (stream, CDROM_INF_types);
	for (i = 0; i < info->type_count; i++)
	{
		TypeDataAsnWrite (&info->types[i], stream, CDROM_INF_types_E,
						  info->field_count);
	}
	AsnEndStruct (stream, CDROM_INF_types);
	
#define WRITE_NEW_STUFF
#ifdef WRITE_NEW_STUFF
	
	if (info->release_date.year != 0)
		CdDate_AsnWrite(&info->release_date,stream,CDROM_INF_release_date);
	if (info->close_date.year != 0)
		CdDate_AsnWrite(&info->close_date,stream,CDROM_INF_close_date);
	
	if (info->type_info != NULL)
	{
		AsnStartStruct(stream,CDROM_INF_type_info);
		for (i=0; i<info->type_count; ++i)
			EntrezTypeInfo_AsnWrite(&(info->type_info[i]),stream,CDROM_INF_type_info_E);
		AsnEndStruct(stream,CDROM_INF_type_info);
	}

	if (info->field_info != NULL)
	{
		AsnStartStruct(stream,CDROM_INF_field_info);
		for (i=0; i<info->field_count; ++i)
			EntrezFieldInfo_AsnWrite(&(info->field_info[i]),stream,CDROM_INF_field_info_E);
		AsnEndStruct(stream,CDROM_INF_field_info);
	}
	
	if (info->div_count > 0)
	{
		val.intvalue = info->div_count;
		AsnWrite(stream,CDROM_INF_div_count,&val);
		AsnStartStruct(stream,CDROM_INF_div_info);
		for (i=0; i<info->div_count; ++i)
			EntrezDivInfo_AsnWrite(&(info->div_info[i]),stream,CDROM_INF_div_info_E);
		AsnEndStruct(stream,CDROM_INF_div_info);
	}
	
#endif

	if (! AsnEndStruct(stream, atp))
		goto erret;

	retval = TRUE;

erret:
	AsnUnlinkType(type_start); /* unlink local tree */
	return retval;
}


static Boolean NEAR FieldDataAsnWrite (EntrezFieldData *data, AsnIo *stream, AsnType *type_start)
{
	DataVal val;
	AsnType *atp;
	Boolean retval = FALSE;

	ASSERT(stream != NULL);

	atp = AsnLinkType(type_start,FIELDDATA); /* link local tree */

	if (atp == NULL)
		return FALSE;

	if (data == NULL)
	{
		AsnNullValueMsg(stream, atp);
		goto erret;
	}

	if (! AsnStartStruct(stream, atp))
		goto erret;
	val.intvalue = data->num_terms;
	AsnWrite (stream, FIELDDATA_num_terms, &val);
	val.intvalue = data->num_bucket;
	AsnWrite (stream, FIELDDATA_num_bucket, &val);

	if (! AsnEndStruct(stream, atp))
		goto erret;

	retval = TRUE;

erret:
	AsnUnlinkType(type_start); /* unlink local tree */
	return retval;
}

static Boolean NEAR TypeDataAsnWrite (EntrezTypeData *data, AsnIoPtr stream,
                                       AsnType *type_start, int num_fields)
{
	DataVal val;
	AsnType *atp;
	int i;
	Boolean retval = FALSE;

	ASSERT(stream != NULL);

	atp = AsnLinkType(type_start, TYPEDATA); /* link local tree */

	if (atp == NULL)
		return FALSE;

	if (data == NULL)
	{
		AsnNullValueMsg(stream, atp);
		goto erret;
	}

	if (! AsnStartStruct(stream, atp))
		goto erret;
	val.intvalue = data->num;
	AsnWrite (stream, TYPEDATA_num, &val);
	val.intvalue = data->num_uids;
	AsnWrite (stream, TYPEDATA_num_uids, &val);
	val.intvalue = data->minuid;
	AsnWrite (stream, TYPEDATA_minuid, &val);
	val.intvalue = data->maxuid;
	AsnWrite (stream, TYPEDATA_maxuid, &val);
	val.intvalue = data->num_bucket;
	AsnWrite (stream, TYPEDATA_num_bucket, &val);
	AsnStartStruct(stream, TYPEDATA_fields);
	for (i=0; i<_fld_ct; ++i)
		FieldDataAsnWrite (&data->fields[i], stream, TYPEDATA_fields_E);
	AsnEndStruct(stream, TYPEDATA_fields);

	if (! AsnEndStruct(stream, atp))
		goto erret;

	retval = TRUE;

erret:
	AsnUnlinkType(type_start); /* unlink local tree */
	return retval;
}


/* ========== EntrezTypeInfo ========== */

static EntrezTypeInfo* EntrezTypeInfo_AsnRead (EntrezTypeInfo *tinf, AsnIo *stream, AsnType *type0)
{
	AsnType *type = type0;
	DataVal val;
	while ((type = AsnReadId(stream,amp,type)) != type0)
	{
		AsnReadVal(stream,type,&val);
		if (type == TYPE_INFO_id)
			tinf->id = (int) val.intvalue;
		else if (type == TYPE_INFO_tag) {
			strcpy(tinf->tag,(char*)val.ptrvalue);
			MemFree((char*)val.ptrvalue);
		} else if (type == TYPE_INFO_name)
			tinf->name = (char*) val.ptrvalue;
		else if (type == TYPE_INFO_descr)
			tinf->descr = (char*) val.ptrvalue;
		else if (type == TYPE_INFO_asntype)
			tinf->asntype = (char*) val.ptrvalue;
		else
			AsnKillValue(type,&val);
	}
	AsnReadVal(stream,type,NULL);   /* END STRUCT */
	return tinf;
}

static Boolean EntrezTypeInfo_AsnWrite (const EntrezTypeInfo *tinf, AsnIo *stream, AsnType *type)
{
	ASSERT(tinf != NULL);
	ASSERT(stream != NULL);
	
	if (AsnStartStruct(stream,type))
	{
		Boolean retval = TRUE;
		Integer_AsnWrite(tinf->id,stream,TYPE_INFO_id,retval);
		String_AsnWrite(tinf->tag,stream,TYPE_INFO_tag,retval);
		String_AsnWrite(tinf->name,stream,TYPE_INFO_name,retval);
		String_AsnWrite(tinf->descr,stream,TYPE_INFO_descr,retval);
		String_AsnWrite(tinf->asntype,stream,TYPE_INFO_asntype,retval);
		if (AsnEndStruct(stream,type))
			return retval;
	}
	return FALSE;
}


/* ========== EntrezFieldInfo ========== */


static EntrezFieldInfo* EntrezFieldInfo_AsnRead (EntrezFieldInfo *fld, AsnIo *stream, AsnType *type0)
{
	AsnType *type = type0;
	DataVal val;
	while ((type = AsnReadId(stream,amp,type)) != type0)
	{
		AsnReadVal(stream,type,&val);
		if (type == FIELD_INFO_id)
			fld->id = (int) val.intvalue;
		else if (type == FIELD_INFO_tag)
			strcpy(fld->tag,(char*)val.ptrvalue);
		else if (type == FIELD_INFO_name)
			fld->name = (char*) val.ptrvalue;
		else if (type == FIELD_INFO_descr)
			fld->descr = (char*) val.ptrvalue;
		else if (type == FIELD_INFO_single_token)
			fld->single_token = (unsigned) val.boolvalue;
		else if (type == FIELD_INFO_has_special)
			fld->has_special = (unsigned) val.boolvalue;
		else if (type == FIELD_INFO_hier_avail)
			fld->hier_avail = (unsigned) val.boolvalue;
		else if (type == FIELD_INFO_hier_id)
			fld->hier_id = (int) val.intvalue;
		else if (type == FIELD_INFO_post_type)
			fld->post_type = (PostType) val.intvalue;
		else
			AsnKillValue(type,&val);
	}
	AsnReadVal(stream,type,NULL);   /* END STRUCT */
	return fld;
}

static Boolean EntrezFieldInfo_AsnWrite (const EntrezFieldInfo *fld, AsnIo *stream, AsnType *type)
{
	ASSERT(fld != NULL);
	ASSERT(stream != NULL);
	
	if (AsnStartStruct(stream,type))
	{
		Boolean retval = TRUE;
		Integer_AsnWrite(fld->id,stream,FIELD_INFO_id,retval);
		String_AsnWrite(fld->tag,stream,FIELD_INFO_tag,retval);
		String_AsnWrite(fld->name,stream,FIELD_INFO_name,retval);
		String_AsnWrite(fld->descr,stream,FIELD_INFO_descr,retval);
		if (fld->single_token)
			Boolean_AsnWrite(fld->single_token,stream,FIELD_INFO_single_token,retval);
		if (fld->has_special)
			Boolean_AsnWrite(fld->has_special,stream,FIELD_INFO_has_special,retval);
		if (fld->hier_avail)
		{
			Boolean_AsnWrite(fld->hier_avail,stream,FIELD_INFO_hier_avail,retval);
			Integer_AsnWrite(fld->hier_id,stream,FIELD_INFO_hier_id,retval);
		}
		if (fld->post_type != PostType_sort)
			Integer_AsnWrite(fld->post_type,stream,FIELD_INFO_post_type,retval);
		if (AsnEndStruct(stream,type))
			return retval;
	}
	return FALSE;
}

static EntrezLinkInfo* EntrezLinkInfo_AsnRead (EntrezLinkInfo *link, AsnIo *stream, AsnType *type0)
{
  AsnType *type = type0;
  DataVal val;
  while ((type = AsnReadId(stream,amp,type)) != type0)
    {
      AsnReadVal(stream,type,&val);
      if (type == LINK_INFO_id)
	link->id = (int) val.intvalue;
      else if (type == LINK_INFO_tag)
	strcpy(link->tag,(char*)val.ptrvalue);
      else if (type == LINK_INFO_name)
	link->name = (char*) val.ptrvalue;
      else if (type == LINK_INFO_descr)
	link->descr = (char*) val.ptrvalue;
      else if (type == LINK_INFO_dbfrom)
	link->dbfrom = (int) val.intvalue;
      else if (type == LINK_INFO_dbto)
	link->dbto = (int) val.intvalue;
      else if (type == LINK_INFO_reciprocal)
	link->reciprocal = (int) val.intvalue;
      else if (type == LINK_INFO_datasize)
	link->datasize = (int) val.intvalue;
      else
	AsnKillValue(type,&val);
    }
  AsnReadVal(stream,type,NULL);   /* END STRUCT */
  return link;
}

static Boolean EntrezLinkInfo_AsnWrite (const EntrezLinkInfo *link, AsnIo *stream, AsnType *type)
{
  ASSERT(link != NULL);
  ASSERT(stream != NULL);
  
  if (AsnStartStruct(stream,type))
    {
      Boolean retval = TRUE;
      Integer_AsnWrite(link->id,stream,LINK_INFO_id,retval);
      String_AsnWrite(link->tag,stream,LINK_INFO_tag,retval);
      String_AsnWrite(link->name,stream,LINK_INFO_name,retval);
      String_AsnWrite(link->descr,stream,LINK_INFO_descr,retval);
      Integer_AsnWrite(link->dbfrom,stream,LINK_INFO_dbfrom,retval);
      Integer_AsnWrite(link->dbto,stream,LINK_INFO_dbto,retval);
      Integer_AsnWrite(link->datasize,stream,LINK_INFO_datasize,retval);
      Integer_AsnWrite(link->reciprocal,stream,LINK_INFO_reciprocal,retval);
      if (AsnEndStruct(stream,type))
	return retval;
    }
  return FALSE;
}


/* ========== EntrezDivInfo ========== */

static EntrezDivInfo* EntrezDivInfo_Construct (EntrezDivInfo *div, int type_ct)
{
	if (div != NULL)
	{
		memset((void*)div,0,sizeof(EntrezDivInfo));
		div->docs = (long*) MemNew(sizeof(long) * (size_t)type_ct);
	}
	return div;
}

static EntrezDivInfo* EntrezDivInfo_Destruct (EntrezDivInfo *div)
{
	if (div != NULL)
	{
		MemFree((void*)div->docs);
		div->docs = NULL;
	}
	return div;
}

static EntrezDivInfo* EntrezDivInfo_AsnRead (EntrezDivInfo *div,  AsnIo *ain, AsnType *type_start)
{
	if (div == NULL)
		div = (EntrezDivInfo*) MemNew(sizeof(EntrezDivInfo));
	if (div != NULL)
	{
		DataVal val;
		AsnType *atp = type_start;
		int i;
		while ((atp = AsnReadId(ain,amp,atp)) != type_start)
		{
			AsnReadVal(ain,atp,&val);
			
			if (atp == DIV_INFO_tag)
			{
				/* memcpy((void*)div->tag,val.ptrvalue,3); */
				Nlm_StringNCpy_0 (div->tag, val.ptrvalue, sizeof (div->tag));
				div->tag[3] = '\0';
			}                                         
			else if (atp == DIV_INFO_descr)
			{
				const char *str = (char*) val.ptrvalue;
				div->descr = (str && *str) ? StrSave(str) : NULL;
			}
			else if (atp == DIV_INFO_reldate)
			{
				const char *str = (char*) val.ptrvalue;
				div->reldate = (str && *str) ? StrSave(str) : NULL;
			}
			else if (atp == DIV_INFO_date)
			{
				CdDate_AsnRead(&div->date,ain,atp);
			}
			else if (atp == DIV_INFO_docs)
			{
				ASSERT(div->docs != NULL);
				i = 0;
			}
			else if (atp == DIV_INFO_docs_E)
			{
				div->docs[i++] = val.intvalue;
			}
			else
			{
				AsnKillValue(atp,&val);
			}
		}
		AsnReadVal(ain,atp,NULL);   /* END STRUCT */
	}
	return div;
}

static Boolean EntrezDivInfo_AsnWrite (const EntrezDivInfo *div,  AsnIo *stream, AsnType *type)
{
	ASSERT(div != NULL);
	ASSERT(stream != NULL);
	
	if (AsnStartStruct(stream,type))
	{
		Boolean retval = TRUE;
		String_AsnWrite(div->tag,stream,DIV_INFO_tag,retval);
		String_AsnWrite(div->descr,stream,DIV_INFO_descr,retval);
		String_AsnWrite(div->reldate,stream,DIV_INFO_reldate,retval);
		CdDate_AsnWrite(&(div->date),stream,DIV_INFO_date);
		if (div->docs != NULL)
		{
			int i;
			AsnStartStruct(stream,DIV_INFO_docs);
			for (i=0; i<_type_ct; ++i)
				Integer_AsnWrite(div->docs[i],stream,DIV_INFO_docs_E,retval);
			AsnEndStruct(stream,DIV_INFO_docs);
		}
		if (AsnEndStruct(stream,type))
			return TRUE;
	}
	return FALSE;
}


/* ========== CdDate ========== */

static CdDate * CdDate_AsnRead (CdDate *date, AsnIo *stream, AsnType *type_start)
{
	AsnType *type = type_start;
	DataVal val;
	while ((type = AsnReadId(stream,amp,type)) != type_start)            
	{
		AsnReadVal(stream,type,&val);
		if (type == CD_DATE_year)
			date->year = (unsigned) val.intvalue;
		else if (type == CD_DATE_month)
			date->month = (unsigned) val.intvalue;
		else if (type == CD_DATE_day)
			date->day = (unsigned) val.intvalue;
		else
			AsnKillValue(type,&val);
	}
	AsnReadVal(stream,type,NULL);   /* END STRUCT */
	return date;
}

static Boolean CdDate_AsnWrite (const CdDate *date, AsnIo *stream, AsnType *type)
{
	ASSERT(date != NULL);
	ASSERT(stream != NULL);
	
	if (AsnStartStruct(stream,type))
	{
		Boolean retval = TRUE;
		Integer_AsnWrite(date->year,stream,CD_DATE_year,retval);
		Integer_AsnWrite(date->month,stream,CD_DATE_month,retval);
		Integer_AsnWrite(date->day,stream,CD_DATE_day,retval);
		if (AsnEndStruct(stream,type))
			return retval;
	}
	return FALSE;
}



/**************************************************
*
*    DocSumNew()
*
**************************************************/

NLM_EXTERN DocSumPtr LIBCALL DocSumNew(void)
{
   DocSumPtr ptr = MemNew((size_t) sizeof(DocSum));

   ptr -> no_abstract = 0;
   ptr -> translated_title = 0;
   ptr -> no_authors = 0;
   ptr -> non_document = 0;
   ptr -> is_segmented = 0;
   ptr -> is_partial = 0;
   ptr->entrezDate = 0;
   ptr->pStatus = ptr->pLanguage = ptr->pPubType = NULL;
   return ptr;

}


/**************************************************
*
*    DocSumAsnRead()
*
**************************************************/

NLM_EXTERN DocSumPtr LIBCALL DocSumAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   DocSumPtr ptr;

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* DocSum ::= (self contained) */
      atp = AsnReadId(aip, amp, DOCSUM);
   } else {
      atp = AsnLinkType(orig, DOCSUM);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = DocSumNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == DOCSUM_no_abstract) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> no_abstract = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DOCSUM_translated_title) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> translated_title = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DOCSUM_no_authors) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> no_authors = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DOCSUM_caption) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> caption = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DOCSUM_title) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> title = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DOCSUM_extra) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> extra = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DOCSUM_non_document) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> non_document = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DOCSUM_is_segmented) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> is_segmented = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DOCSUM_is_partial) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> is_partial = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DOCSUM_create) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      CdDate_AsnRead(&ptr->create, aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DOCSUM_modify) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      CdDate_AsnRead(&ptr->modify, aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DOCSUM_link_count) {
      Int2 index = 0;

      AsnReadVal(aip, atp, &av);
      while ((atp = AsnReadId(aip, amp, atp)) == DOCSUM_link_count_E)
      {
	  AsnReadVal(aip, atp, &av);
	  ptr -> link_count[index++] = (short) av.intvalue;
      }
      AsnReadVal(aip, atp, &av);
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DOCSUM_uid) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> uid = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DOCSUM_not_yet_neighbored) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> not_yet_neighbored = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = DocSumFree(ptr);
   goto ret;
}



/**************************************************
*
*    DocSumAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL DocSumAsnWrite(DocSumPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;
   Int2 index;

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, DOCSUM);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.boolvalue = ptr -> no_abstract;
   if (av.boolvalue) retval = AsnWrite(aip, DOCSUM_no_abstract,  &av);
   av.boolvalue = ptr -> translated_title;
   if (av.boolvalue) retval = AsnWrite(aip, DOCSUM_translated_title,  &av);
   av.boolvalue = ptr -> no_authors;
   if (av.boolvalue) retval = AsnWrite(aip, DOCSUM_no_authors,  &av);
   if (ptr -> caption != NULL) {
      av.ptrvalue = ptr -> caption;
      retval = AsnWrite(aip, DOCSUM_caption,  &av);
   }
   if (ptr -> title != NULL) {
      av.ptrvalue = ptr -> title;
      retval = AsnWrite(aip, DOCSUM_title,  &av);
   }
   if (ptr -> extra != NULL) {
      av.ptrvalue = ptr -> extra;
      retval = AsnWrite(aip, DOCSUM_extra,  &av);
   }
   av.boolvalue = ptr -> non_document;
   if (av.boolvalue) retval = AsnWrite(aip, DOCSUM_non_document,  &av);
   av.boolvalue = ptr -> is_segmented;
   if (av.boolvalue) retval = AsnWrite(aip, DOCSUM_is_segmented,  &av);
   av.boolvalue = ptr -> is_partial;
   if (av.boolvalue) retval = AsnWrite(aip, DOCSUM_is_partial,  &av);
   if ( ! CdDate_AsnWrite(&ptr -> create, aip, DOCSUM_create)) {
      goto erret;
   }
   if ( ! CdDate_AsnWrite(&ptr -> modify, aip, DOCSUM_modify)) {
      goto erret;
   }

   for (index = 0; index < DocType_MAX; index++)
   {
       if (ptr->link_count[index] != 0)
       {
	   break;
       }
   }

   if (index < DocType_MAX)
   { /* some non-zero links */
       AsnStartStruct(aip, DOCSUM_link_count);
       for (index = 0; index < DocType_MAX; index++)
       {
           av.intvalue = ptr->link_count[index];
           AsnWrite(aip, DOCSUM_link_count_E, &av);
       }
       AsnEndStruct(aip, DOCSUM_link_count);
   }

   av.intvalue = ptr -> uid;
   if (av.intvalue != 0) retval = AsnWrite(aip, DOCSUM_uid,  &av);

   av.boolvalue = ptr -> not_yet_neighbored;
   if (av.boolvalue) retval = AsnWrite(aip, DOCSUM_not_yet_neighbored,  &av);

   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}


/*****************************************************************************
*
*   DocSumFree(dsp)
*
*****************************************************************************/
NLM_EXTERN DocSumPtr LIBCALL DocSumFree (DocSumPtr dsp)
{
    if (dsp == NULL)
        return NULL;
    MemFree(dsp->caption);
    MemFree(dsp->title);
    MemFree(dsp->extra);
    MemFree(dsp->pStatus);
    MemFree(dsp->pLanguage);
    MemFree(dsp->pPubType);
    return ((DocSumPtr) MemFree(dsp));
}

