/*  objgen.c
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
* File Name:  objgen.c
*
* Author:  James Ostell
*   
* Version Creation Date: 1/1/91
*
* $Revision: 6.8 $
*
* File Description:  Object manager for module NCBI-General
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 05-13-93 Schuler     All public functions are now declared LIBCALL.
* 06-21-93 Schuler     Made a static copy of NCBI_months[] in this file 
*                      (because it cannot be exported from ncbitime.c when 
*                      it is linked as a DLL).
*
* $Log: objgen.c,v $
* Revision 6.8  2000/01/20 17:04:27  beloslyu
* new function DateClean to properly clean the date was added
*
* Revision 6.7  2000/01/20 15:13:16  ostell
* added hour/minute/second fields to Date
* added DateTimeCurr()
*
* Revision 6.6  1999/11/03 17:58:26  kans
* added SeqDescrAddPointer
*
* Revision 6.5  1999/10/05 17:24:14  kans
* added SeqDescrAdd
*
* Revision 6.4  1999/09/28 14:56:00  kans
* rename ObjValNodeNew to SeqDescrNew
*
* Revision 6.3  1999/09/23 00:07:45  kans
* ObjValNodeNew implemented
*
* Revision 6.2  1998/08/24 18:28:04  kans
* removed solaris -v -fd warnings
*
* Revision 6.1  1998/08/17 23:05:00  kans
* user object asn read needed to set choices 11 and 12
*
* Revision 6.0  1997/08/25 18:49:56  madden
* Revision changed to 6.0
*
* Revision 4.3  1997/06/19 18:41:23  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 4.2  1997/03/18 15:27:53  ostell
* made DbtagMatch and ObjectIdMatch case insensitive
*
 * Revision 4.1  1997/01/06  21:38:30  vakatov
 * Removed local(static) NCBI_months[12] -- use that from the "ncbitime.[ch]"
 *
 * Revision 4.0  1995/07/26  13:48:06  ostell
 * force revision to 4.0
 *
 * Revision 3.4  1995/05/15  21:22:00  ostell
 * added Log line
 *
*
*
*
* ==========================================================================
*/
#include <asngen.h>        /* the AsnTool header */
#include <objgen.h>		   /* the general objects interface */


static Boolean loaded = FALSE;

/* ValNode equivalent functions that allocate ObjValNode size */

NLM_EXTERN ValNodePtr LIBCALL SeqDescrNew (ValNodePtr vnp)

{
	ValNodePtr newnode;

	newnode = (ValNodePtr) Nlm_MemNew(sizeof(ObjValNode));
	if (newnode == NULL) return NULL;
	newnode->extended = 1; /* extended flag indicates extra fields are present */
	if (vnp != NULL)
    {
        while (vnp->next != NULL)
            vnp = vnp->next;
		vnp->next = newnode;
    }
	return newnode;
}

NLM_EXTERN ValNodePtr LIBCALL SeqDescrAdd (ValNodePtr PNTR head)

{
	ValNodePtr newnode;

	if (head != NULL)
	{
		newnode = SeqDescrNew (*head);
		if (*head == NULL)
			*head = newnode;
	}
	else
		newnode = SeqDescrNew (NULL);

	return newnode;
}

NLM_EXTERN ValNodePtr LIBCALL SeqDescrAddPointer (ValNodePtr PNTR head, Int2 choice, VoidPtr value)

{
	ValNodePtr newnode;

	newnode = SeqDescrAdd (head);
	if (newnode != NULL)
	{
		newnode->choice = (Uint1) choice;
		newnode->data.ptrvalue = value;
	}

	return newnode;
}

/*****************************************************************************
*
*   GeneralAsnLoad()
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL GeneralAsnLoad (void)
{
    if (loaded)
        return TRUE;

    if (AsnLoad())
        loaded = TRUE;
    return loaded;
}

/*****************************************************************************
*
*   Date Routines
*
*****************************************************************************/
/*****************************************************************************
*
*   DateNew()
*
*****************************************************************************/
NLM_EXTERN NCBI_DatePtr LIBCALL DateNew (void)
{
	NCBI_DatePtr dp = NULL;
	
	dp = (NCBI_DatePtr)MemNew(sizeof(NCBI_Date));
        dp->data[4] = 255;  /* hour not set */
        dp->data[5] = 255;  /* minute not set */
        dp->data[6] = 255;  /* second not set */
	return dp;
}

/*****************************************************************************
*
*   DateFree(dp)
*
*****************************************************************************/
NLM_EXTERN NCBI_DatePtr LIBCALL DateFree (NCBI_DatePtr dp)
{
    if (dp == NULL)
        return dp;

	MemFree(dp->str);
	return (NCBI_DatePtr)MemFree(dp);
}

/*****************************************************************************
*
*   DateClean(dp)
*
*****************************************************************************/
NLM_EXTERN NCBI_DatePtr LIBCALL DateClean (NCBI_DatePtr dp)
{
    if (dp == NULL)
        return dp;

	memset(dp, 0, sizeof(NCBI_Date));
	dp->data[4] = 255;  /* hour not set */
	dp->data[5] = 255;  /* minute not set */
	dp->data[6] = 255;  /* second not set */
	return dp;
}

/*****************************************************************************
*
*   DateWrite(dp, year, month, day, season)
*   if dp->data[0] not set
*   	if year != 0, it's a std date
*   	else it's a str date
*   if a std date, season = season
*   if a str date, season = str
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL DateWrite (NCBI_DatePtr dp, Int2 year, Int2 month, Int2 day, CharPtr season)
{
	if (dp == NULL)
		return FALSE;

	if (dp->str != NULL)    /* remove previous string if any */
		MemFree(dp->str);

        dp->data[4] = 255;   /* set time fields to not-set */
        dp->data[5] = 255;
        dp->data[6] = 255;

	if (year == 0)       /* date-str */
	{
		if (season == NULL)     /* not optional */
			return FALSE;
		dp->data[0] = 0;        /* set type */
		dp->data[1] = 0;        /* clear the rest */
		dp->data[2] = 0;
		dp->data[3] = 0;
		dp->str = StringSave(season);
	}
	else
	{                     /* date-std */
		dp->data[0] = 1;	         /* set type */
		year -= 1900;
		if ((year < 1) || (year > 255))   /* year not optional */
			return FALSE;
		if ((month < 0) || (month > 12))
			return FALSE;
		if ((day < 0) || (day > 31))
			return FALSE;
		dp->data[1] = (Uint1)year;
		dp->data[2] = (Uint1)month;
		dp->data[3] = (Uint1)day;
		dp->str = StringSave(season);
	}
	return TRUE;
}

/*****************************************************************************
*
*   DateRead(dp, &year, &month, &day, season)
*       season is buffer to copy season/str into
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL DateRead (NCBI_DatePtr dp, Int2Ptr yearptr, Int2Ptr monthptr, Int2Ptr dayptr, CharPtr season)
{
	if (dp == NULL)
		return FALSE;

	if (dp->data[0] == 0)     /* str type */
	{
		if (season == NULL)
			return FALSE;
		StringCpy(season, dp->str);
	}
	else                      /* std type */
	{
		if (yearptr != NULL)
			*yearptr = (Int2)dp->data[1] + 1900;
		if (monthptr != NULL)
			*monthptr = (Int2)dp->data[2];
		if (dayptr != NULL)
			*dayptr = (Int2)dp->data[3];
		if (season != NULL)
		{
			if (dp->str == NULL)
				*season = '\0';
			else
				StringCpy(season, dp->str);
		}
	}
	return TRUE;
}


/*****************************************************************************
*
*   DatePrint(dp, buf)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL DatePrint (NCBI_DatePtr dp, CharPtr to)

{
	Char tbuf[10];
	
	*to = '\0';

	if (dp->data[0] == 0)    /* str type */
	{
		StringCpy(to, dp->str);    /* just copy string */
	}
	else
	{
		if (dp->data[2] > 0)
		{
			to = StringMove(to, NCBI_months[dp->data[2] - 1]);
			*to = ' ';
			to++;
			*to = '\0';
		}
		if (dp->data[3] > 0)
		{
			sprintf(tbuf, "%d, ", dp->data[3]);   /* for MS windows sprintf */
            to = StringMove(to, tbuf);
		}
		if (dp->data[1] == 0)
			to = StringMove(to, "????");
		else
		{
			sprintf(tbuf, "%d", (int)(dp->data[1] + 1900));
        	to = StringMove(to, tbuf);
		}
		if (dp->str != NULL)
	    {
			*to = ';';
			to++;
			*to = ' ';
			to++;
			to = StringMove(to, dp->str);
		}
	}
	return TRUE;
}

/*****************************************************************************
*
*   DateCurr()
*
*****************************************************************************/
NLM_EXTERN NCBI_DatePtr LIBCALL DateCurr (void)
{
	NCBI_DatePtr dp;
	Nlm_DayTime dt;

	dp = DateNew();
	if (dp == NULL) return dp;
	GetDayTime(&dt);
	dp->data[0] = (Uint1)1;    /* date.std */
	dp->data[1] = (Uint1)dt.tm_year;
	dp->data[2] = (Uint1)(dt.tm_mon + 1);
	dp->data[3] = (Uint1)dt.tm_mday;

	return dp;
}

/*****************************************************************************
*
*   DateTimeCurr()
*
*****************************************************************************/
NLM_EXTERN NCBI_DatePtr LIBCALL DateTimeCurr (void)
{
	NCBI_DatePtr dp;
	Nlm_DayTime dt;

	dp = DateNew();
	if (dp == NULL) return dp;
	GetDayTime(&dt);
	dp->data[0] = (Uint1)1;    /* date.std */
	dp->data[1] = (Uint1)dt.tm_year;
	dp->data[2] = (Uint1)(dt.tm_mon + 1);
	dp->data[3] = (Uint1)dt.tm_mday;
        dp->data[4] = (Uint1)(dt.tm_hour);
        dp->data[5] = (Uint1)(dt.tm_min);
        dp->data[6] = (Uint1)(dt.tm_sec);

	return dp;
}

/*****************************************************************************
*
*   DateDup(dp)
*
*****************************************************************************/
NLM_EXTERN NCBI_DatePtr LIBCALL DateDup (NCBI_DatePtr dp)
{
	NCBI_DatePtr np;

	if (dp == NULL)
		return dp;

	np = DateNew();
	if (np == NULL) return np;
	MemCopy(np, dp, sizeof(NCBI_Date));
	if (dp->str != NULL)
		np->str = StringSave(dp->str);
	return np;
}

/*****************************************************************************
*
*   DateAsnWrite(dp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (Date ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL DateAsnWrite (NCBI_DatePtr dp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! GeneralAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, DATE);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (dp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

	av.ptrvalue = (Pointer)dp;

	if (dp->data[0] == 0)      /* str type */
	{
		if (! AsnWriteChoice(aip, atp, (Int2)0, &av)) goto erret;
		av.ptrvalue = dp->str;
		if (! AsnWrite(aip, DATE_str, &av)) goto erret;    /* write the choice */
	}
	else                       /* std type */
	{
		if (! AsnWriteChoice(aip, atp, (Int2)1, &av)) goto erret;

		if (! AsnOpenStruct(aip, DATE_std, (Pointer)dp))
            goto erret;

		if (dp->data[1] == 0)   /* not set properly */
			av.intvalue = 0;    /* carry it through */
		else
			av.intvalue = (Int4)dp->data[1] + 1900;
		if (! AsnWrite(aip, DATE_STD_year, &av)) goto erret;
		if (dp->data[2] != 0)
		{
			av.intvalue = dp->data[2];
			if (! AsnWrite(aip, DATE_STD_month, &av)) goto erret;
		}
		if (dp->data[3] != 0)
		{
			av.intvalue = dp->data[3];
			if (! AsnWrite(aip, DATE_STD_day, &av)) goto erret;
		}
		if (dp->str != NULL)
		{
			av.ptrvalue = dp->str;
			if (! AsnWrite(aip, DATE_STD_season, &av)) goto erret;
		}
		if (dp->data[4] < 24)
		{
			av.intvalue = dp->data[4];
			if (! AsnWrite(aip, DATE_STD_hour, &av)) goto erret;
		}
		if (dp->data[5] < 60)
		{
			av.intvalue = dp->data[5];
			if (! AsnWrite(aip, DATE_STD_minute, &av)) goto erret;
		}
		if (dp->data[6] < 60)
		{
			av.intvalue = dp->data[6];
			if (! AsnWrite(aip, DATE_STD_second, &av)) goto erret;
		}

		if (! AsnCloseStruct(aip, DATE_std, (Pointer)dp))
            goto erret;
	}
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   DateAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN NCBI_DatePtr LIBCALL DateAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    NCBI_DatePtr dp=NULL;

	if (! loaded)
	{
		if (! GeneralAsnLoad())
			return dp;
	}

	if (aip == NULL)
		return dp;

	if (orig == NULL)           /* Date ::= (self contained) */
		atp = AsnReadId(aip, amp, DATE);
	else
		atp = AsnLinkType(orig, DATE);    /* link in local tree */
    if (atp == NULL)
        return dp;

	dp = DateNew();
    if (dp == NULL)
        goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the CHOICE value (nothing) */
	atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;  /* find the choice */
	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;     /* get the first value */

	if (atp == DATE_str)      /* str type */
	{
		dp->str = (CharPtr)av.ptrvalue;         /* save the string */
	}
	else if (atp == DATE_std)    /* std type */
	{
		dp->data[0] = 1;         /* set type */
		atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* get the year */
		if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
		if (av.intvalue == 0)   /* year not set */
			dp->data[1] = 0;    /* don't blow it */
		else
			dp->data[1] = (Uint1)(av.intvalue - 1900);
		                                  /* check optionals */
		while ((atp = AsnReadId(aip, amp, atp)) != DATE_std)
		{
            if (atp == NULL)
                goto erret;
			if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
			if (atp == DATE_STD_month)
				dp->data[2] = (Uint1)av.intvalue;
			else if (atp == DATE_STD_day)
				dp->data[3] = (Uint1)av.intvalue;
			else if (atp == DATE_STD_season)
				dp->str = (CharPtr)av.ptrvalue;
			else if (atp == DATE_STD_hour)
				dp->data[4] = (Uint1)av.intvalue;
			else if (atp == DATE_STD_minute)
				dp->data[5] = (Uint1)av.intvalue;
			else if (atp == DATE_STD_second)
				dp->data[6] = (Uint1)av.intvalue;
		}
        if (atp == NULL) goto erret;
		if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read last END_STRUCT */
	}
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return dp;
erret:
    dp = DateFree(dp);
    goto ret;
}

/*****************************************************************************
*
*   Int2 DateMatch(a, b, all)
*   	returns 0 = same
*   	1, -1 = different (b is after a = -1)
*       2, -2 = couldn't match, arbitrary ordering
*   	if (all) then all fields in one must be in the other
*   		else, matches only fields found in both
*   	string dates just come in alphabetical order
*
*     thus, for 0,1,-1 return values are like strcmp()
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL DateMatch (DatePtr a, DatePtr b, Boolean all)
{
	Int2 retval = 2;

	if ((a != NULL) && (b != NULL))
	{
		if (a->data[0] == b->data[0])  /* same type */
		{
			if (a->data[0] == 0)   /* string date */
			{
				retval = (Int2)StringICmp(a->str, b->str);
				if (retval > 0)
					return retval;
				else if (retval < 0)
					return (Int2) -1;
			}
			else                    /* std date */
			{
				if (a->data[1] > b->data[1])  /* year */
					return (Int2)1;
				else if (a->data[1] < b->data[1])
					return (Int2)-1;
				if (((a->data[2] != 0) && (b->data[2] != 0)) || all)
				{
					if (a->data[2] > b->data[2])  /* month */
						return (Int2)1;
					else if (a->data[2] < b->data[2])
						return (Int2)-1;
				}
				if (((a->data[3] != 0) && (b->data[3] != 0)) || all)
				{
					if (a->data[3] > b->data[3])  /* day */
						return (Int2)1;
					else if (a->data[3] < b->data[3])
						return (Int2)-1;
				}
				if (((a->str != NULL) && (b->str != NULL)) || all)
				{
					if ((a->str == NULL) && (b->str != NULL))
						return (Int2) -1;
					else if ((a->str != NULL) && (b->str == NULL))
						return (Int2) 1;
					retval = StringICmp(a->str, b->str);
					if (retval > 0)
						return (Int2)1;
					else if (retval < 0)
						return (Int2)-1;
				}
			}
			retval = 0;
		}
		else
		{
			if (a->data[0] == 1)
				return (Int2) -2;
			else
				return retval;
		}
	}
	else if (all)
	{
		if (a == NULL)
			return (Int2) -2;
		else
			return retval;
	}
	return retval;
}

/*****************************************************************************
*
*   DateCheck (dp)
*   	Checks the date and month values in a date structure
*       returns:
*      -4 = NULL pointer passed in
*      -3 = string date, can't be checked
*      -2 = month not set (but otherwise ok)
*      -1 = day not set	 (but otherwise ok)
*       0 = date ok, month,day,year all set
*       1 = day invalid
*       2 = month invalid
*   	3 = year not set (required for date)
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL DateCheck (DatePtr dp)
{
	Int2 	day, month, year, last;
	static Uint1 days[12] = { 31, 28, 31, 30, 31,
	  30, 31, 31, 30, 31, 30, 31 };
	  	
	if (dp == NULL) {
		return -4;
	}
	if (dp->data[0] == 1) {
		if (! dp->data[1])
			return 3;
		year = (Int2)dp->data[1] + 1900;
		month = dp->data[2];
		day = dp->data[3];
		if (! month)    /* month not set */
			return -2;
		if (month > 12)
			return 2;
		if (! day)
			return -1;
		last = days[month-1];
		if ((month == 2) && (year%4 == 0) && (year != 2000)) {
			last = 29;
		}
		if (day > last) {
			return 1;
		}
	}
	else
		return -3;

	return 0;
}

/*****************************************************************************
*
*   DbtagNew()
*
*****************************************************************************/
NLM_EXTERN DbtagPtr LIBCALL DbtagNew (void)
{
	DbtagPtr dbt;

	dbt = (DbtagPtr)MemNew(sizeof(Dbtag));
	return dbt;
}

/*****************************************************************************
*
*   DbtagFree()
*
*****************************************************************************/
NLM_EXTERN DbtagPtr LIBCALL DbtagFree (DbtagPtr dbt)
{
    if (dbt == NULL)
        return dbt;

	dbt->tag = ObjectIdFree(dbt->tag);
	MemFree(dbt->db);
	return (DbtagPtr)MemFree(dbt);
}

/*****************************************************************************
*
*   DbtagAsnRead(aip, atp)
*
*****************************************************************************/
NLM_EXTERN DbtagPtr LIBCALL DbtagAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DbtagPtr dbt=NULL;
	DataVal av;
	AsnTypePtr atp;

	if (! loaded)
	{
		if (! GeneralAsnLoad())
			return dbt;
	}

	if (aip == NULL)
		return dbt;

	if (orig == NULL)           /* Dbtag ::= */
		atp = AsnReadId(aip, amp, DBTAG);
	else
		atp = AsnLinkType(orig, DBTAG);
    if (atp == NULL)
        return dbt;

	dbt = DbtagNew();
    if (dbt == NULL)
        goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the START_STRUCT */

	atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;  /* read the db */
	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
	dbt->db = (CharPtr)av.ptrvalue;

	atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;  /* read tag */
	dbt->tag = ObjectIdAsnRead(aip, atp);
    if (dbt->tag == NULL)
        goto erret;

	atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* END_STRUCT */
	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
ret:
	AsnUnlinkType(orig);
	return dbt;
erret:
    dbt = DbtagFree(dbt);
    goto ret;
}

/*****************************************************************************
*
*   DbtagAsnWrite(dbt, aip, atp)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL DbtagAsnWrite (DbtagPtr dbt, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! GeneralAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, DBTAG);
    if (atp == NULL)
        return FALSE;

	if (dbt == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

	if (! AsnOpenStruct(aip, atp, (Pointer)dbt)) goto erret;

	av.ptrvalue = dbt->db;
	if (! AsnWrite(aip, DBTAG_db, &av)) goto erret;
	if (! ObjectIdAsnWrite(dbt->tag, aip, DBTAG_tag)) goto erret;

	if (! AsnCloseStruct(aip, atp, (Pointer)dbt)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);
	return retval;
}

/*****************************************************************************
*
*   DbtagMatch(a, b)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL DbtagMatch (DbtagPtr a, DbtagPtr b)
{
	if (a == b)
		return TRUE;

	if ((a == NULL) || (b == NULL))
		return FALSE;

	if (StringICmp(a->db, b->db))
		return FALSE;

	return ObjectIdMatch(a->tag, b->tag);
}

/*****************************************************************************
*
*   DbtagDup(oldtag)
*
*****************************************************************************/
NLM_EXTERN DbtagPtr LIBCALL DbtagDup (DbtagPtr oldtag)
{
	DbtagPtr newtag;

	if (oldtag == NULL)
		return oldtag;

	newtag = DbtagNew();
	if (newtag == NULL) return newtag;
	newtag->db = StringSave(oldtag->db);
	newtag->tag = ObjectIdDup(oldtag->tag);
	return newtag;
}

/*****************************************************************************
*
*   DbtagLabel(dbt, buf, buflen)
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL DbtagLabel(DbtagPtr dbt, CharPtr buf, Int2 buflen)
{
	Char numbuf[15];
	CharPtr tp = NULL;

	if (dbt == NULL) return 0;

	if (dbt->tag != NULL)
	{
		if (dbt->tag->str != NULL)
			tp = dbt->tag->str;
		else
		{
			sprintf(numbuf, "%ld", (long)(dbt->tag->id));
			tp = numbuf;
		}
	}

	return LabelCopyExtra(buf, ": ", buflen, dbt->db, tp);
}

/*****************************************************************************
*
*   ObjectIdNew()
*
*****************************************************************************/
NLM_EXTERN ObjectIdPtr LIBCALL ObjectIdNew (void)
{
	ObjectIdPtr oid;

	oid = (ObjectIdPtr)MemNew(sizeof(ObjectId));
	return oid;
}

/*****************************************************************************
*
*   ObjectIdFree()
*
*****************************************************************************/
NLM_EXTERN ObjectIdPtr LIBCALL ObjectIdFree (ObjectIdPtr oid)
{
    if (oid == NULL)
        return oid;

	MemFree(oid->str);
	return (ObjectIdPtr)MemFree(oid);
}

/*****************************************************************************
*
*   ObjectIdAsnRead(aip, atp)
*
*****************************************************************************/
NLM_EXTERN ObjectIdPtr LIBCALL ObjectIdAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	ObjectIdPtr oid = NULL;
	DataVal av;
	AsnTypePtr atp;

	if (! loaded)
	{
		if (! GeneralAsnLoad())
			return oid;
	}

	if (aip == NULL)
		return oid;

	if (orig == NULL)           /* ObjectId ::= */
		atp = AsnReadId(aip, amp, OBJECT_ID);
	else
		atp = AsnLinkType(orig, OBJECT_ID);
    if (atp == NULL)
        return oid;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the CHOICE value (nothing) */

	atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;  /* read the CHOICE type */
	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;		 /* and the value */
	
	oid = ObjectIdNew();
    if (oid == NULL)
        goto erret;
	if (atp == OBJECT_ID_id)
		oid->id = av.intvalue;
	else
		oid->str = (CharPtr)av.ptrvalue;
ret:
	AsnUnlinkType(orig);
	return oid;
erret:
    oid = ObjectIdFree(oid);
    goto ret;
}

/*****************************************************************************
*
*   ObjectIdAsnWrite(oid, aip, atp)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjectIdAsnWrite (ObjectIdPtr oid, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! GeneralAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, OBJECT_ID);
    if (atp == NULL)
        return FALSE;

	if (oid == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

	if (! AsnWrite(aip, atp, &av)) goto erret;     /* write the CHOICE (no value) */

	if (oid->str == NULL)    /* id used */
	{
		av.intvalue = oid->id;
		atp = OBJECT_ID_id;
	}
	else                    /* str used */
	{
		av.ptrvalue = oid->str;
		atp = OBJECT_ID_str;
	}

	if (! AsnWrite(aip, atp, &av)) goto erret;
	retval = TRUE;
erret:
	AsnUnlinkType(orig);
	return retval;
}

/*****************************************************************************
*
*   Boolean ObjectIdMatch(a, b)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjectIdMatch (ObjectIdPtr a, ObjectIdPtr b)
{
	if (a == b)
		return TRUE;

    if ((a == NULL) || (b == NULL))   /* only one is null */
        return FALSE;

	if ((a->str != NULL) && (b->str != NULL))  /* same type */
	{
	    if (StringICmp(a->str, b->str))
    	    return FALSE;
		else
			return TRUE;
	}
    else if ((a->str == NULL) && (b->str == NULL))  /* must be same kind */
    {
        if (a->id == b->id)
            return TRUE;
        else
            return FALSE;
    }
    else                   /* different kinds */
        return FALSE;
}

/*****************************************************************************
*
*   ObjectIdDup(oldid)
*
*****************************************************************************/
NLM_EXTERN ObjectIdPtr LIBCALL ObjectIdDup (ObjectIdPtr oldid)
{
	ObjectIdPtr newid;

	if (oldid == NULL)
		return oldid;
	
	newid = ObjectIdNew();
	newid->id = oldid->id;
	newid->str = StringSave(oldid->str);
	return newid;
}

/*****************************************************************************
*
*   NameStdNew()
*
*****************************************************************************/
NLM_EXTERN NameStdPtr LIBCALL NameStdNew (void)
{
	NameStdPtr nsp;

	nsp = (NameStdPtr)MemNew(sizeof(NameStd));
	return nsp;
}

/*****************************************************************************
*
*   NameStdFree()
*
*****************************************************************************/
NLM_EXTERN NameStdPtr LIBCALL NameStdFree (NameStdPtr nsp)
{
	Int2 i;

    if (nsp == NULL)
        return nsp;

	for (i = 0; i < 7; i++)
		MemFree(nsp->names[i]);
	return (NameStdPtr)MemFree(nsp);
}

/*****************************************************************************
*
*   NameStdAsnRead(aip, atp)
*
*****************************************************************************/
NLM_EXTERN NameStdPtr LIBCALL NameStdAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	NameStdPtr nsp=NULL;
	DataVal av;
	AsnTypePtr atp, start;
	Int2 i;

	if (! loaded)
	{
		if (! GeneralAsnLoad())
			return nsp;
	}

	if (aip == NULL)
		return nsp;

	if (orig == NULL)           /* NameStd ::= */
		atp = AsnReadId(aip, amp, NAME_STD);
	else
		atp = AsnLinkType(orig, NAME_STD);
	start = atp;
    if (atp == NULL)
        return nsp;

	nsp = NameStdNew();
    if (nsp == NULL)
        goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the START_STRUCT */

	do
	{
		atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
		if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
		if (atp == NAME_STD_last)
			i = 0;
		else if (atp == NAME_STD_first)
			i = 1;
		else if (atp == NAME_STD_middle)
			i = 2;
		else if (atp == NAME_STD_full)
			i = 3;
		else if (atp == NAME_STD_initials)
			i = 4;
		else if (atp == NAME_STD_suffix)
			i = 5;
		else if (atp == NAME_STD_title)
			i = 6;
		else
			i = 10;

		if (i < 10)
			nsp->names[i] = (CharPtr)av.ptrvalue;
	} while (atp != start);
ret:
	AsnUnlinkType(orig);
	return nsp;
erret:
    nsp = NameStdFree(nsp);
    goto ret;
}

/*****************************************************************************
*
*   NameStdAsnWrite(nsp, aip, atp)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL NameStdAsnWrite (NameStdPtr nsp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, ptr;
	Int2 i;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! GeneralAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, NAME_STD);
    if (atp == NULL)
        return FALSE;

	if (nsp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

	if (! AsnOpenStruct(aip, atp, (Pointer)nsp)) goto erret;

	for (i = 0; i < 7; i++)
	{
		if (nsp->names[i] != NULL)
		{
			av.ptrvalue = nsp->names[i];
			switch (i)
			{
				case 0:
					ptr = NAME_STD_last;
					break;
				case 1:
					ptr = NAME_STD_first;
					break;
				case 2:
					ptr = NAME_STD_middle;
					break;
				case 3:
					ptr = NAME_STD_full;
					break;
				case 4:
					ptr = NAME_STD_initials;
					break;
				case 5:
					ptr = NAME_STD_suffix;
					break;
				case 6:
					ptr = NAME_STD_title;
					break;
			}
			if (! AsnWrite(aip, ptr, &av)) goto erret;
		}
	}

	if (! AsnCloseStruct(aip, atp, (Pointer)nsp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);
	return retval;
}

/*****************************************************************************
*
*   PersonIdNew()
*
*****************************************************************************/
NLM_EXTERN PersonIdPtr LIBCALL PersonIdNew (void)
{
	PersonIdPtr pid;

	pid = (PersonIdPtr)MemNew(sizeof(PersonId));
	return pid;
}

/*****************************************************************************
*
*   PersonIdFree()
*
*****************************************************************************/
NLM_EXTERN PersonIdPtr LIBCALL PersonIdFree (PersonIdPtr pid)
{
    if (pid == NULL)
        return pid;

	if (pid->choice == 1)          /* Dbtag */
		DbtagFree((DbtagPtr)pid->data);
	else if (pid->choice == 2)     /* Name-std */
		NameStdFree((NameStdPtr)pid->data);
	else
		MemFree(pid->data);
	return (PersonIdPtr)MemFree(pid);
}

/*****************************************************************************
*
*   PersonIdAsnRead(aip, atp)
*
*****************************************************************************/
NLM_EXTERN PersonIdPtr LIBCALL PersonIdAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	PersonIdPtr pid=NULL;
	DataVal av;
	AsnTypePtr atp;

	if (! loaded)
	{
		if (! GeneralAsnLoad())
			return pid;
	}

	if (aip == NULL)
		return pid;

	if (orig == NULL)           /* PersonId ::= */
		atp = AsnReadId(aip, amp, PERSON_ID);
	else
		atp = AsnLinkType(orig, PERSON_ID);
    if (atp == NULL) return pid;

	pid = PersonIdNew();
    if (pid == NULL) goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the CHOICE (nothing) */

	atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* identify the CHOICE type */
	if (atp == PERSON_ID_dbtag)
	{
		pid->choice = 1;
		pid->data = DbtagAsnRead(aip, atp);
	}
	else if (atp == PERSON_ID_name)
	{
		pid->choice = 2;
		pid->data = NameStdAsnRead(aip, atp);
	}
	else
	{
		if (AsnReadVal(aip, atp, &av) <= 0) goto erret;        /* a string type */
		pid->data = av.ptrvalue;
		if (atp == PERSON_ID_ml)
			pid->choice = 3;
		else
			pid->choice = 4;       /* str */
	}
    if (pid->data == NULL)
        goto erret;
ret:
	AsnUnlinkType(orig);
	return pid;
erret:
    pid = PersonIdFree(pid);
    goto ret;
}

/*****************************************************************************
*
*   PersonIdAsnWrite(pid, aip, atp)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL PersonIdAsnWrite (PersonIdPtr pid, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! GeneralAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, PERSON_ID);
    if (atp == NULL)
        goto erret;

	if (pid == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

	if (! AsnWrite(aip, atp, &av)) goto erret;          /* write the CHOICE, no value */

	av.ptrvalue = pid->data;
	switch (pid->choice)
	{
		case 1:
			if (! DbtagAsnWrite((DbtagPtr)(pid->data), aip, PERSON_ID_dbtag))
                goto erret;
			break;
		case 2:
			if (! NameStdAsnWrite((NameStdPtr)(pid->data), aip, PERSON_ID_name))
                goto erret;
			break;
		case 3:
			if (! AsnWrite(aip, PERSON_ID_ml, &av)) goto erret;
			break;
		case 4:
			if (! AsnWrite(aip, PERSON_ID_str, &av)) goto erret;
			break;
	}
    retval = TRUE;
erret:
	AsnUnlinkType(orig);
	return retval;
}

/*****************************************************************************
*
*   PersonIdLabel(pid, buf, buflen, format)
*   	Makes a short label, lastname then initials if it can
*   	format = PIDLABEL_GENBANK   last,initials
*   		   PIDLABEL_EMBL      last initials
*
*       Modeled from GBGetAuthNames in asn2ff
*   	returns number of bytes in buf
*   	buf MUST be at least (buflen + 1) long
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL PersonIdLabel (PersonIdPtr pid, CharPtr buf, Int2 buflen, Int2 format)
{
	NameStdPtr nsp;
	Int2 len, diff;
	Char tbuf[2];
	CharPtr tmp;

	if ((pid == NULL) || (buflen < 0) || (buf == NULL))
		return 0;
    
    if (buflen == 0)    /* on a boundary */
    {
    	*(buf-1) = '>';
    	return 0;
    }
    
	*buf = '?';
	*(buf+1) = '\0';
	len = buflen;
	if (format == PIDLABEL_GENBANK)
		tbuf[0] = ',';
	else
		tbuf[0] = ' ';
	tbuf[1] = '\0';

	if (pid->choice == 2)  /* structured name */
	{
		nsp = (NameStdPtr)(pid->data);
		if (nsp->names[0] != NULL)   /* last name */
		{
			diff = LabelCopy(buf, nsp->names[0], buflen);
			buflen -= diff; buf += diff;
			if (nsp->names[4])
			{
				diff = LabelCopyExtra(buf, nsp->names[4], buflen, tbuf, NULL);
				buflen -= diff; buf += diff;
			}
			if (nsp->names[5]) /* suffix */
			{
				tbuf[0] = ' ';
				diff = LabelCopyExtra(buf, nsp->names[5], buflen, tbuf, NULL);
				buflen -= diff; buf += diff;
			}
		}
		else if (nsp->names[3] != NULL) /* full name */
		{
			diff = LabelCopy(buf, nsp->names[3], buflen);
			buflen -= diff; buf += diff;
		}
	}
	else if ((pid->choice == 3) || (pid->choice == 4))
	{
		diff = LabelCopy(buf, (CharPtr)(pid->data), buflen);
		if (format == PIDLABEL_EMBL)
		{
			for (tmp = buf; *tmp != '\0'; tmp ++)
			{
				if (*tmp == ',')
				{
					*tmp = ' ';
					break;
				}
			}
		}
		buflen -= diff; buf += diff;
	}
	else
		return LabelCopy(buf, "Unsupported PersonID", buflen);
	
	return (len - buflen);
}

/*****************************************************************************
*
*   IntFuzzNew()
*
*****************************************************************************/
NLM_EXTERN IntFuzzPtr LIBCALL IntFuzzNew (void)
{
	IntFuzzPtr ifp;

	ifp = (IntFuzzPtr)MemNew(sizeof(IntFuzz));
	return ifp;
}

/*****************************************************************************
*
*   IntFuzzFree()
*
*****************************************************************************/
NLM_EXTERN IntFuzzPtr LIBCALL IntFuzzFree (IntFuzzPtr ifp)
{
	if (ifp == NULL)
		return ifp;
	MemFree(ifp->alt);
	return (IntFuzzPtr)MemFree(ifp);
}

/*****************************************************************************
*
*   IntFuzzAsnRead(aip, atp)
*
*****************************************************************************/
NLM_EXTERN IntFuzzPtr LIBCALL IntFuzzAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	IntFuzzPtr ifp=NULL;
	DataVal av;
	AsnTypePtr atp;
	Int4Ptr tmp;

	if (! loaded)
	{
		if (! GeneralAsnLoad())
			return ifp;
	}

	if (aip == NULL)
		return ifp;

	if (orig == NULL)           /* IntFuzz ::= */
		atp = AsnReadId(aip, amp, INT_FUZZ);
	else
		atp = AsnLinkType(orig, INT_FUZZ);
    if (atp == NULL)
        return ifp;

	ifp = IntFuzzNew();
    if (ifp == NULL)
        goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the CHOICE (nothing) */

	atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* identify the CHOICE type */
	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;        /* read first value */
	ifp->a = av.intvalue;             /* ok in 3/4 of cases */

	if (atp == INT_FUZZ_p_m)   /* plus-minus */
	{
		ifp->choice = 1;
	}
	else if (atp == INT_FUZZ_pct)
	{
		ifp->choice = 3;
	}
	else if (atp == INT_FUZZ_lim)
	{
		ifp->choice = 4;
	}
	else if (atp == INT_FUZZ_range)     /* range, read a SEQUENCE */
	{
		ifp->choice = 2;
		atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;        /* max */
		if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
		ifp->a = av.intvalue;
		atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;        /* min */
		if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
		ifp->b = av.intvalue;
		atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;        /* end of structure */
		if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
	}
	else if (atp == INT_FUZZ_alt)
	{
		ifp->choice = 5;
		ifp->a = 0;
		ifp->b = 0;
		while ((atp = AsnReadId(aip, amp, atp)) == INT_FUZZ_alt_E)
		{
			if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
			if (ifp->a == ifp->b)   /* need more room */
			{
				tmp = ifp->alt;
				ifp->alt = MemNew((size_t)(ifp->b + 10) * sizeof(Int4));
				if (ifp->alt == NULL) goto erret;
				MemMove(ifp->alt, tmp, (size_t)(ifp->b * sizeof(Int4)));
				MemFree(tmp);
				ifp->b += 10;
			}
			ifp->alt[ifp->a] = av.intvalue;
			ifp->a++;
			
		}
		if (atp == NULL) goto erret;
		if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
	}
ret:
	AsnUnlinkType(orig);
	return ifp;
erret:
    ifp = IntFuzzFree(ifp);
    goto ret;
}

/*****************************************************************************
*
*   IntFuzzAsnWrite(ifp, aip, atp)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL IntFuzzAsnWrite (IntFuzzPtr ifp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, ptr;
    Boolean retval = FALSE;
	Int4 i;

	if (! loaded)
	{
		if (! GeneralAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, INT_FUZZ);
    if (atp == NULL) return FALSE;

	if (ifp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

	av.ptrvalue = (Pointer)ifp;
	if (! AsnWriteChoice(aip, atp, (Int2)ifp->choice, &av)) goto erret;

	av.intvalue = ifp->a;          /* ok in 3/4 choices */
	ptr = NULL;

	switch (ifp->choice)
	{
		case 1:
			ptr = INT_FUZZ_p_m;
			break;
		case 2:
			if (! AsnOpenStruct(aip, INT_FUZZ_range, (Pointer)ifp))
                goto erret;
			if (! AsnWrite(aip, INT_FUZZ_range_max, &av)) goto erret;
			av.intvalue = ifp->b;
			if (! AsnWrite(aip, INT_FUZZ_range_min, &av)) goto erret;
			if (! AsnCloseStruct(aip, INT_FUZZ_range, (Pointer)ifp))
                goto erret;
			break;
		case 3:
			ptr = INT_FUZZ_pct;
			break;
		case 4:
			ptr = INT_FUZZ_lim;
			break;
		case 5:
			if (! AsnOpenStruct(aip, INT_FUZZ_alt, (Pointer)ifp))
                goto erret;
			for (i = 0; i < ifp->a; i++)
			{
				av.intvalue = ifp->alt[i];
				if (! AsnWrite(aip, INT_FUZZ_alt_E, &av)) goto erret;
			}
			if (! AsnCloseStruct(aip, INT_FUZZ_alt, (Pointer)ifp))
                goto erret;
			break;
	}

	if (ptr != NULL)           /* 3/5 types */
		if (! AsnWrite(aip, ptr, &av)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);
	return retval;
}

/*****************************************************************************
*
*   UserFieldNew()
*
*****************************************************************************/
NLM_EXTERN UserFieldPtr LIBCALL UserFieldNew (void)
{
	UserFieldPtr ufp;

	ufp = (UserFieldPtr)MemNew(sizeof(UserField));
	return ufp;
}

/*****************************************************************************
*
*   UserFieldFree()
*
*****************************************************************************/
NLM_EXTERN UserFieldPtr LIBCALL UserFieldFree (UserFieldPtr ufp)
{
    Int4 i, num;
    CharPtr PNTR cpp;
    ByteStorePtr PNTR bpp;
    UserFieldPtr ufpa, ufpb;
    UserObjectPtr uopa, uopb;

    if (ufp == NULL)
        return ufp;

    num = ufp->num;

	if (ufp->label != NULL)
		ufp->label = ObjectIdFree(ufp->label);
		
    switch (ufp->choice)
    {
        case 1:             /* str */
        case 8:             /* ints */
        case 9:             /* reals */
            MemFree(ufp->data.ptrvalue);
            break;
        case 5:             /* os */
            BSFree((ByteStorePtr) ufp->data.ptrvalue);
            break;
        case 6:             /* object */
            UserObjectFree((UserObjectPtr) ufp->data.ptrvalue);
            break;
        case 7:             /* strs */
            cpp = (CharPtr PNTR) ufp->data.ptrvalue;
            for (i = 0; i < num; i++)
                MemFree(cpp[i]);
            MemFree(cpp);
            break;
        case 10:            /* oss */
            bpp = (ByteStorePtr PNTR) ufp->data.ptrvalue;
            for (i = 0; i < num; i++)
                BSFree(bpp[i]);
            MemFree(bpp);
            break;
        case 11:            /* fields */
            ufpa = (UserFieldPtr) ufp->data.ptrvalue;
            while (ufpa != NULL)
            {
                ufpb = ufpa->next;
                UserFieldFree(ufpa);
                ufpa = ufpb;
            }
            break;
        case 12:            /* objects */
            uopa = (UserObjectPtr) ufp->data.ptrvalue;
            while (uopa != NULL)
            {
                uopb = uopa->next;
                UserObjectFree(uopa);
                uopa = uopb;
            }
            break;

    }
	return (UserFieldPtr)MemFree(ufp);
}

/*****************************************************************************
*
*   UserFieldAsnRead(aip, atp)
*
*****************************************************************************/
NLM_EXTERN UserFieldPtr LIBCALL UserFieldAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	UserFieldPtr ufp=NULL;
	DataVal av;
	AsnTypePtr atp;
    Int4 num = 0, i = 0;
    CharPtr PNTR cpp;
    ByteStorePtr PNTR bpp;
    Int4Ptr ip;
    FloatHiPtr fp;
    UserFieldPtr ufpa, ufpb = NULL;
    UserObjectPtr uopa, uopb = NULL;
    static char * emsg1 = "Too many %s in UserField. line %ld",
        * emsg2 = "Too few %s in UserField. line %ld";

	if (! loaded)
	{
		if (! GeneralAsnLoad())
			return ufp;
	}

	if (aip == NULL)
		return ufp;

	if (orig == NULL)           /* UserField ::= */
		atp = AsnReadId(aip, amp, USER_FIELD);
	else
		atp = AsnLinkType(orig, USER_FIELD);
    if (atp == NULL)
        return ufp;

	ufp = UserFieldNew();
    if (ufp == NULL)
        goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read start struct */

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* object id */
    ufp->label = ObjectIdAsnRead(aip, atp);
    if (ufp->label == NULL) goto erret;

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* could be num */
    if (atp == USER_FIELD_num)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        num = av.intvalue;
        ufp->num = num;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the CHOICE (nothing) */

	atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* identify the CHOICE type */

    if (atp == USER_FIELD_data_object)
    {
        ufp->choice = 6;
        ufp->data.ptrvalue = (Pointer) UserObjectAsnRead(aip, atp);
        if (ufp->data.ptrvalue == NULL) goto erret;
    }
    else if (atp == USER_FIELD_data_strs)
    {
        ufp->choice = 7;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* start SEQUENCE OF */
        ufp->data.ptrvalue = MemNew((size_t)(sizeof(CharPtr) * num));
        if (ufp->data.ptrvalue == NULL)
            goto erret;
        cpp = (CharPtr PNTR) ufp->data.ptrvalue;
        while ((atp = AsnReadId(aip, amp, atp)) == USER_FIELD_data_strs_E)
        {
            if (i >= num)
            {
                ErrPost(CTX_NCBIOBJ,1, emsg1, "strs", aip->linenumber);
                goto erret;
            }
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
            cpp[i] = (CharPtr)av.ptrvalue;
            i++;
        }
        if (atp == NULL) goto erret;
        if (i != num)
        {
            ErrPost(CTX_NCBIOBJ,1, emsg2, "strs", aip->linenumber);
            goto erret;
        }
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end SEQUENCE OF */
    }
    else if (atp == USER_FIELD_data_ints)
    {
        ufp->choice = 8;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* start SEQUENCE OF */
        ufp->data.ptrvalue = MemNew((size_t)(sizeof(Int4) * num));
        ip = (Int4Ptr) ufp->data.ptrvalue;
        if (ip == NULL) goto erret;
        while ((atp = AsnReadId(aip, amp, atp)) == USER_FIELD_data_ints_E)
        {
            if (i >= num)
            {
                ErrPost(CTX_NCBIOBJ,1, emsg1, "ints", aip->linenumber);
                goto erret;
            }
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
            ip[i] = av.intvalue;
            i++;
        }
        if (atp == NULL) goto erret;
        if (i != num)
        {
            ErrPost(CTX_NCBIOBJ,1, emsg2, "ints", aip->linenumber);
            goto erret;
        }
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end SEQUENCE OF */
    }
    else if (atp == USER_FIELD_data_reals)
    {
        ufp->choice = 9;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* start SEQUENCE OF */
        ufp->data.ptrvalue = MemNew((size_t)(sizeof(FloatHi) * num));
        fp = (FloatHiPtr) ufp->data.ptrvalue;
        if (fp == NULL)
            goto erret;
        while ((atp = AsnReadId(aip, amp, atp)) == USER_FIELD_data_reals_E)
        {
            if (i >= num)
            {
                ErrPost(CTX_NCBIOBJ,1, emsg1, "reals", aip->linenumber);
                goto erret;
            }
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
            fp[i] = av.realvalue;
            i++;
        }
        if (atp == NULL) goto erret;
        if (i != num)
        {
            ErrPost(CTX_NCBIOBJ,1, emsg2, "reals", aip->linenumber);
            goto erret;
        }
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end SEQUENCE OF */
    }
    else if (atp == USER_FIELD_data_oss)
    {
        ufp->choice = 10;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* start SEQUENCE OF */
        ufp->data.ptrvalue = MemNew((size_t)(sizeof(ByteStorePtr) * num));
        bpp = (ByteStorePtr PNTR) ufp->data.ptrvalue;
        if (bpp == NULL)
            goto erret;
        while ((atp = AsnReadId(aip, amp, atp)) == USER_FIELD_data_oss_E)
        {
            if (i >= num)
            {
                ErrPost(CTX_NCBIOBJ,1, emsg1, "oss", aip->linenumber);
                goto erret;
            }
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
            bpp[i] = (ByteStorePtr)av.ptrvalue;
            i++;
        }
        if (atp == NULL) goto erret;
        if (i != num)
        {
            ErrPost(CTX_NCBIOBJ,1, emsg2, "oss", aip->linenumber);
            goto erret;
        }
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end SEQUENCE OF */
    }
    else if (atp == USER_FIELD_data_fields)
    {
        ufp->choice = 11;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* start SEQUENCE OF */
        while ((atp = AsnReadId(aip, amp, atp)) == USER_FIELD_data_fields_E)
        {
            ufpa = UserFieldAsnRead(aip, atp);
            if (ufpa == NULL) goto erret;
            if (ufpb == NULL)
                ufp->data.ptrvalue = (Pointer) ufpa;
            else
                ufpb->next = ufpa;
            ufpb = ufpa;
        }
        if (atp == NULL) goto erret;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end SEQUENCE OF */
    }
    else if (atp == USER_FIELD_data_objects)
    {
        ufp->choice = 12;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* start SEQUENCE OF */
        while ((atp = AsnReadId(aip, amp, atp)) == USER_FIELD_data_objects_E)
        {
            uopa = UserObjectAsnRead(aip, atp);
            if (uopa == NULL) goto erret;
            if (uopb == NULL)
                ufp->data.ptrvalue = (Pointer) uopa;
            else
                uopb->next = uopa;
            uopb = uopa;
        }
        if (atp == NULL) goto erret;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end SEQUENCE OF */
    }
    else 
    {
        if (AsnReadVal(aip, atp, &ufp->data) <= 0) goto erret;
        if (atp == USER_FIELD_data_str)
            ufp->choice = 1;
        else if (atp == USER_FIELD_data_int)
            ufp->choice = 2;
        else if (atp == USER_FIELD_data_real)
            ufp->choice = 3;
        else if (atp == USER_FIELD_data_bool)
            ufp->choice = 4;
        else if (atp == USER_FIELD_data_os)
            ufp->choice = 5;
    }

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* end struct */
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
ret:
	AsnUnlinkType(orig);
	return ufp;
erret:
    ufp = UserFieldFree(ufp);
    goto ret;
}

/*****************************************************************************
*
*   UserFieldAsnWrite(ufp, aip, atp)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL UserFieldAsnWrite (UserFieldPtr ufp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Int4 num, i;
    CharPtr PNTR cpp;
    ByteStorePtr PNTR bpp;
    Int4Ptr ip;
    FloatHiPtr fp;
    UserFieldPtr ufpa;
    UserObjectPtr uopa;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! GeneralAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, USER_FIELD);
    if (atp == NULL) return FALSE;

	if (ufp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)ufp)) goto erret;  /* start the struct */
    if (! ObjectIdAsnWrite(ufp->label, aip, USER_FIELD_label))
        goto erret;
    num = ufp->num;

    if (num > 0)
    {
        av.intvalue = num;
        if (! AsnWrite(aip, USER_FIELD_num, &av)) goto erret;
    }
    
	if (! AsnWriteChoice(aip, USER_FIELD_data, (Int2)ufp->choice, &ufp->data))
        goto erret;

    switch (ufp->choice)
    {
        case 1:
            if (! AsnWrite(aip, USER_FIELD_data_str, &ufp->data))
                goto erret;
            break;
        case 2:
            if (! AsnWrite(aip, USER_FIELD_data_int, &ufp->data))
                goto erret;
            break;
        case 3:
            if (! AsnWrite(aip, USER_FIELD_data_real, &ufp->data))
                goto erret;
            break;
        case 4:
            if (! AsnWrite(aip, USER_FIELD_data_bool, &ufp->data))
                goto erret;
            break;
        case 5:
            if (! AsnWrite(aip, USER_FIELD_data_os, &ufp->data))
                goto erret;
            break;
        case 6:
            if (! UserObjectAsnWrite((UserObjectPtr)ufp->data.ptrvalue, aip, USER_FIELD_data_object))
                goto erret;
            break;
        case 7:
            cpp = (CharPtr PNTR) ufp->data.ptrvalue;
            if (! AsnOpenStruct(aip, USER_FIELD_data_strs, (Pointer)cpp))
                goto erret;
            for (i = 0; i < num; i++)
            {
                av.ptrvalue = (Pointer) cpp[i];
                if (! AsnWrite(aip, USER_FIELD_data_strs_E, &av)) goto erret;
            }
            if (! AsnCloseStruct(aip, USER_FIELD_data_strs, (Pointer)cpp))
                goto erret;
            break;
        case 8:
            ip = (Int4Ptr) ufp->data.ptrvalue;
            if (! AsnOpenStruct(aip, USER_FIELD_data_ints, (Pointer)ip))
                goto erret;
            for (i = 0; i < num; i++)
            {
                av.intvalue = ip[i];
                if (! AsnWrite(aip, USER_FIELD_data_ints_E, &av)) goto erret;
            }
            if (! AsnCloseStruct(aip, USER_FIELD_data_ints, (Pointer)ip))
                goto erret;
            break;
        case 9:
            fp = (FloatHiPtr) ufp->data.ptrvalue;
            if (! AsnOpenStruct(aip, USER_FIELD_data_reals, (Pointer)fp))
                goto erret;
            for (i = 0; i < num; i++)
            {
                av.realvalue = fp[i];
                if (! AsnWrite(aip, USER_FIELD_data_reals_E, &av)) goto erret;
            }
            if (! AsnCloseStruct(aip, USER_FIELD_data_reals, (Pointer)fp))
                goto erret;
            break;
        case 10:
            bpp = (ByteStorePtr PNTR) ufp->data.ptrvalue;
            if (! AsnOpenStruct(aip, USER_FIELD_data_oss, (Pointer)bpp))
                goto erret;
            for (i = 0; i < num; i++)
            {
                av.ptrvalue = (Pointer) bpp[i];
                if (! AsnWrite(aip, USER_FIELD_data_oss_E, &av)) goto erret;
            }
            if (! AsnCloseStruct(aip, USER_FIELD_data_oss, (Pointer)bpp))
                goto erret;
            break;
        case 11:
            ufpa = (UserFieldPtr) ufp->data.ptrvalue;
            if (! AsnOpenStruct(aip, USER_FIELD_data_fields, ufp->data.ptrvalue))
                goto erret;
            while (ufpa != NULL)
            {
                if (! UserFieldAsnWrite(ufpa, aip, USER_FIELD_data_fields_E))
                    goto erret;
                ufpa = ufpa->next;
            }
            if (! AsnCloseStruct(aip, USER_FIELD_data_fields, ufp->data.ptrvalue))
                goto erret;
            break;
        case 12:
            uopa = (UserObjectPtr) ufp->data.ptrvalue;
            if (! AsnOpenStruct(aip, USER_FIELD_data_objects, ufp->data.ptrvalue))
                goto erret;
            while (uopa != NULL)
            {
                if (! UserObjectAsnWrite(uopa, aip, USER_FIELD_data_objects_E))
                    goto erret;
                uopa = uopa->next;
            }
            if (! AsnCloseStruct(aip, USER_FIELD_data_objects, ufp->data.ptrvalue))
                goto erret;
            break;
    }
    if (! AsnCloseStruct(aip, atp, (Pointer)ufp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);
	return retval;
}

/*****************************************************************************
*
*   UserObjectNew()
*
*****************************************************************************/
NLM_EXTERN UserObjectPtr LIBCALL UserObjectNew (void)
{
	UserObjectPtr uop;

	uop = (UserObjectPtr)MemNew(sizeof(UserObject));
	return uop;
}

/*****************************************************************************
*
*   UserObjectFree()
*
*****************************************************************************/
NLM_EXTERN UserObjectPtr LIBCALL UserObjectFree (UserObjectPtr uop)
{
    UserFieldPtr ufpa, ufpb;

    if (uop == NULL)
        return uop;

    MemFree(uop->_class);
    ObjectIdFree(uop->type);
    ufpa = uop->data;
    while (ufpa != NULL)
    {
        ufpb = ufpa->next;
        UserFieldFree(ufpa);
        ufpa = ufpb;
    }
	return (UserObjectPtr)MemFree(uop);
}

/*****************************************************************************
*
*   UserObjectAsnRead(aip, atp)
*
*****************************************************************************/
NLM_EXTERN UserObjectPtr LIBCALL UserObjectAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	UserObjectPtr uop=NULL;
	DataVal av;
	AsnTypePtr atp;
    UserFieldPtr ufpa, ufpb = NULL;

	if (! loaded)
	{
		if (! GeneralAsnLoad())
			return uop;
	}

	if (aip == NULL)
		return uop;

	if (orig == NULL)           /* UserObject ::= */
		atp = AsnReadId(aip, amp, USER_OBJECT);
	else
		atp = AsnLinkType(orig, USER_OBJECT);
    if (atp == NULL)
        return uop;

	uop = UserObjectNew();
    if (uop == NULL)
        goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the start struct */

	atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* class? */
    if (atp == USER_OBJECT_class)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        uop->_class = (CharPtr) av.ptrvalue;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }

    uop->type = ObjectIdAsnRead(aip, atp);
    if (uop->type == NULL) goto erret;

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* start SEQUENCE OF */
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    while ((atp = AsnReadId(aip, amp, atp)) == USER_OBJECT_data_E)
    {
        ufpa = UserFieldAsnRead(aip, atp);
        if (ufpa == NULL)
            goto erret;
        if (ufpb == NULL)
            uop->data = ufpa;
        else
            ufpb->next = ufpa;
        ufpb = ufpa;
    }
    if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end SEQUENCE OF */
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* end SEQUENCE */
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
ret:
	AsnUnlinkType(orig);
	return uop;
erret:
    uop = UserObjectFree(uop);
    goto ret;
}

/*****************************************************************************
*
*   UserObjectAsnWrite(uop, aip, atp)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL UserObjectAsnWrite (UserObjectPtr uop, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    UserFieldPtr ufp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! GeneralAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, USER_OBJECT);
    if (atp == NULL) return FALSE;

	if (uop == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)uop)) goto erret;

    if (uop->_class != NULL)
    {
        av.ptrvalue = (Pointer) uop->_class;
        if (! AsnWrite(aip, USER_OBJECT_class, &av)) goto erret;
    }

    if (! ObjectIdAsnWrite(uop->type, aip, USER_OBJECT_type))
        goto erret;

    if (! AsnOpenStruct(aip, USER_OBJECT_data, (Pointer)uop->data))
        goto erret;
    ufp = uop->data;
    while (ufp != NULL)
    {
        if (! UserFieldAsnWrite(ufp, aip, USER_OBJECT_data_E))
            goto erret;
        ufp = ufp->next;
    }
    if (! AsnCloseStruct(aip, USER_OBJECT_data, (Pointer)uop->data))
        goto erret;
    
    if (! AsnCloseStruct(aip, atp, (Pointer)uop)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);
	return retval;
}





