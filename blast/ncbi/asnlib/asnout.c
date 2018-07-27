/* asnout.c
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
* File Name:  asnout.c
*
* Author:  James Ostell
*
* Version Creation Date: 3/4/91
*
* $Revision: 6.2 $
*
* File Description:
*   Routines for outputing ASN.1 parse trees from asntool
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 3/4/91   Kans        Stricter typecasting for GNU C and C++
*
* $Log: asnout.c,v $
* Revision 6.2  2001/01/10 17:42:47  beloslyu
* fix to resolve the problem with buggy optimizer of Forte compiler on Solaris/Intel
*
* Revision 6.1  1999/03/11 23:43:17  kans
* fprintf casts
*
* Revision 6.0  1997/08/25 18:10:14  madden
* Revision changed to 6.0
*
* Revision 5.1  1996/12/03 21:43:48  vakatov
* Adopted for 32-bit MS-Windows DLLs
*
 * Revision 5.0  1996/05/28  14:00:29  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:47:38  ostell
 * force revision to 4.0
 *
 * Revision 2.3  1995/05/15  18:38:28  ostell
 * added Log line
 *
*
* ==========================================================================
*/

/*****************************************************************************
*
*   asnout.c
*   	routines to output a set of modules to a header file
*
*****************************************************************************/

#include "asnbuild.h"

#ifdef WIN_MSWIN
static char * headerlines[] = {     
#else
static CharPtr headerlines[] = {
#endif
	"/***********************************************************************\n",
	"*\n",
	"*",
	"*\n",
	"*        Automatic header module from ASNTOOL\n",
	"*\n",
	"************************************************************************/\n",
	"\n",
	"#ifndef _ASNTOOL_\n",
	"#include <asn.h>\n",
	"#endif\n",
	"\n"};
/*****************************************************************************
*
*   void AsnOutput(filename)
*       in loader file, pointers are repesented as numbers
*           -32000 = NULL
*           -32001 = &avn[0]
*           any other negative number &avn[positive number]
*           0 or any positive number = &at[number]
*   
*
*****************************************************************************/
NLM_EXTERN void AsnOutput (CharPtr filename, AsnModulePtr amp, Boolean loader, Int2 maxDefineLength)

{
	FILE *fp, *ld;
	int i, numvalue = 0, numtype = 0, nummod = 0, j;
	AsnProc p;
	AsnModulePtr curr_mod;
	AsnTypePtr curr_type, atp;
	AsnTmpTypePtr attp, attp2;
	AsnTmpValuePtr attvp;
	AsnValxNodePtr atvp;
	Int2 isa;
	char buf[200];
    char * pnt;
    CharPtr ptr;

	StringCpy(buf, filename);

	fp = fopen(buf, "w");
	
	if (fp == NULL)
	{
		AsnIoErrorMsg(NULL, 82, buf);
		return;
	}

    pnt = buf;
    while (* pnt != '\0')
        pnt++;
    while ((pnt > buf) && (*pnt != '.'))
        pnt--;
    pnt++;
	if (loader)
		*pnt = 'l';
	pnt++;

	if (amp->filename != NULL)    /* the $Revision extension */
	{
		if (StringLen(amp->filename) <= 1)
		{
			*pnt = '0';
			pnt++;
		}
		StringMove(pnt, amp->filename);
		MemFree(amp->filename);    /* get rid of the extension */
	}
	amp->filename = StringSave(buf);

    if (loader)
        ld = fopen(buf, "w");
    else
        ld = NULL;

	for (i = 0; i < 12; i++)
		fprintf(fp, headerlines[i]);
	fprintf(fp, "static char * asnfilename = \"%s\";\n", buf);

	curr_mod = amp;
	p.curr_tmptype = &p.root_tmptype;      /* fake node to start */
	p.curr_tmpvalue = &p.root_tmpvalue;
	p.root_tmptype.next = NULL;
	p.root_tmpvalue.next = NULL;

	while (curr_mod != NULL)
	{
		nummod++;
		curr_type = curr_mod->types;
		while (curr_type != NULL)
		{
			curr_type = AsnOutAddType(&p, curr_type);
		}
		curr_mod = curr_mod->next;
	}

	/******************** set index values ***************************/
	
	attvp = p.root_tmpvalue.next;
	while (attvp != NULL)
	{
		attvp->index = numvalue;
		numvalue++;
		attvp = attvp->next;
	}

	attp = p.root_tmptype.next;
	while (attp != NULL)
	{
		attp->index = numtype;
		numtype++;
		attp = attp->next;
	}

	/****************** write values *********************************/
    if (loader)     /* always put in all pointers and counts */
    {
        fprintf(ld, "%d %d %d\n", numvalue, numtype, nummod);

        fprintf(fp, "static AsnValxNodePtr avn;\n");
        fprintf(fp, "static AsnTypePtr at;\n");
        fprintf(fp, "static AsnModulePtr amp;\n");
    }

	if (numvalue)
	{
        if (! loader)   
    		fprintf(fp, "static AsnValxNode avnx[%d] = {\n", numvalue);

		attvp = p.root_tmpvalue.next;

		while (attvp != NULL)
		{
			atvp = attvp->val;
            if (! loader)
            {
    			fprintf(fp, "    {%d,", atvp->valueisa);
	    		if (atvp->name == NULL)
		    		fprintf(fp, "NULL,");
			    else
				    fprintf(fp, "\"%s\" ,", atvp->name);
    			fprintf(fp,"%ld,", (long) atvp->intvalue);
	    		if (atvp->realvalue == (FloatHi) 0.0)
		    		fprintf(fp,"0.0,");
			    else
				    fprintf(fp,"%f,", atvp->realvalue);
    			if (atvp->next == NULL)
	    			fprintf(fp, "NULL }");
		    	else
			    	fprintf(fp, "&avnx[%d] }", AsnOutFindValue(&p, atvp->next));
    			if (attvp->next == NULL)
	    			fprintf(fp, " };\n\n");
		    	else
			    	fprintf(fp, " ,\n");
            }
            else
            {
    			fprintf(ld, "%d ", atvp->valueisa);
	    		if (atvp->name == NULL)
		    		fprintf(ld, "-32000 ");
			    else
				    fprintf(ld, "%s ", atvp->name);
    			fprintf(ld,"%ld ", (long) atvp->intvalue);
	    		if (atvp->realvalue == (FloatHi) 0.0)
		    		fprintf(ld,"0.0 ");
			    else
				    fprintf(ld,"%f ", atvp->realvalue);
    			if (atvp->next == NULL)
	    			fprintf(ld, "-32000\n");
		    	else
                {
                    j = AsnOutFindValue(&p, atvp->next);
                    if (j == 0)
                       j = 32001; 
			    	fprintf(ld, "-%d\n", j);
                }
            }
			attvp = attvp->next;
		}
	}

	/****************** write types *********************************/

    if (! loader)
        fprintf(fp, "static AsnType atx[%d] = {\n", numtype);

	attp = p.root_tmptype.next;

	while (attp != NULL)
	{
		atp = attp->ptr;
		isa = atp->isa;
        if (! loader)
        {
    		fprintf(fp, "  {%d,", isa);
	    	if (atp->name == NULL)
		    	fprintf(fp, " NULL,");
    		else
	    		fprintf(fp, " \"%s\" ,", atp->name);
		    fprintf(fp, "%d,%d,", atp->tagclass, atp->tagnumber);

    		if (ISA_BASETYPE(isa))    /* primitive type, fake it */
	    		fprintf(fp, "0,0,0,0,0,0,NULL,NULL,NULL,0,NULL}");
		    else
    		{
	    		if (atp->implicit)
		    		i = 1;
			    else
				    i = 0;
    			fprintf(fp,"%d,", i);
	    		if (atp->optional)
		    		i = 1;
			    else
				    i = 0;
    			fprintf(fp,"%d,", i);
	    		if (atp->hasdefault)
		    		i = 1;
			    else
				    i = 0;
    			fprintf(fp,"%d,", i);
	    		if (atp->exported)
		    		i = 1;
			    else	
				    i = 0;
    			fprintf(fp,"%d,", i);
	    		if (atp->imported)
		    		i = 1;
			    else
				    i = 0;
    			fprintf(fp, "%d,", i);
	    		fprintf(fp, "0,");   /* resolved is FALSE */
		    	if (atp->defaultvalue == NULL)
			    	fprintf(fp, "NULL,");
    			else
	    			fprintf(fp, "&avnx[%d],", AsnOutFindValue(&p, atp->defaultvalue));
		    	attp2 = AsnOutFindType(&p, atp->type);
			    if (attp2 == NULL)    /* imported */
				    fprintf(fp, "NULL,");
    			else
	    			fprintf(fp, "&atx[%d],", attp2->index);
		    	if (atp->type != NULL)
			    	isa = atp->type->isa;    /* get base type */
    			else    
	    			isa = -32000;
		    	if ((atp->branch == NULL) || (atp->imported))
			    	fprintf(fp, "NULL,");
    			else if ((isa == INTEGER_TYPE) || (isa == ENUM_TYPE))
	    			fprintf(fp, "&avnx[%d],", AsnOutFindValue(&p, (AsnValxNodePtr) atp->branch));
		    	else
			    {
				    attp2 = AsnOutFindType(&p, (AsnTypePtr) atp->branch);
    				fprintf(fp, "&atx[%d],", attp2->index);
	    		}
		    	fprintf(fp, "0,");     /* ->tmp */
			    if (atp->next == NULL)
    				fprintf(fp, "NULL}");
	    		else
		    	{
			    	attp2 = AsnOutFindType(&p, atp->next);
				    fprintf(fp, "&atx[%d]}", attp2->index);
    			}
	        }
    		if (attp->next == NULL)
	    		fprintf(fp, " };\n\n");
		    else
			    fprintf(fp, " ,\n");
        }
        else
        {
    		fprintf(ld, "%d ", isa);
	    	if (atp->name == NULL)
		    	fprintf(ld, "-32000 ");
    		else
            {
                ptr = atp->name;
                pnt = buf;
                while (*ptr != '\0')  /* fix SEQ OF, SET OF */
                {
                    *pnt = *ptr;
                    if (*ptr == ' ')
                        *pnt = '!';
                    ptr++;  pnt++;
                }
                *pnt = '\0';
	    		fprintf(ld, "%s ", buf);
            }
		    fprintf(ld, "%d %d ", atp->tagclass, atp->tagnumber);

    		if (ISA_BASETYPE(isa))    /* primitive type, fake it */
	    		fprintf(ld, "0 0 0 0 0 -32000 -32000 -32000 -32000\n");
		    else
    		{
	    		if (atp->implicit)
		    		i = 1;
			    else
				    i = 0;
    			fprintf(ld,"%d ", i);
	    		if (atp->optional)
		    		i = 1;
			    else
				    i = 0;
    			fprintf(ld,"%d ", i);
	    		if (atp->hasdefault)
		    		i = 1;
			    else
				    i = 0;
    			fprintf(ld,"%d ", i);
	    		if (atp->exported)
		    		i = 1;
			    else	
				    i = 0;
    			fprintf(ld,"%d ", i);
	    		if (atp->imported)
		    		i = 1;
			    else
				    i = 0;
    			fprintf(ld, "%d ", i);
		    	if (atp->defaultvalue == NULL)
			    	fprintf(ld, "-32000 ");
    			else
                {
                    j = AsnOutFindValue(&p, atp->defaultvalue);
                    if (j == 0)
                        j = 32001;
	    			fprintf(ld, "-%d ", j);
                }
		    	attp2 = AsnOutFindType(&p, atp->type);
			    if (attp2 == NULL)    /* imported */
				    fprintf(ld, "-32000 ");
    			else
	    			fprintf(ld, "%d ", attp2->index);
		    	if (atp->type != NULL)
			    	isa = atp->type->isa;    /* get base type */
    			else    
	    			isa = -32000;
		    	if ((atp->branch == NULL) || (atp->imported))
			    	fprintf(ld, "-32000 ");
    			else if ((isa == INTEGER_TYPE) || (isa == ENUM_TYPE))
                {
	    			j = AsnOutFindValue(&p, (AsnValxNodePtr) atp->branch);
                    if (j == 0)
                        j = 32001;
                    fprintf(ld, "-%d ", j);
                }
		    	else
			    {
				    attp2 = AsnOutFindType(&p, (AsnTypePtr) atp->branch);
    				fprintf(ld, "%d ", attp2->index);
	    		}
			    if (atp->next == NULL)
    				fprintf(ld, "-32000\n");
	    		else
		    	{
			    	attp2 = AsnOutFindType(&p, atp->next);
				    fprintf(ld, "%d\n", attp2->index);
    			}
	        }
        }
		attp = attp->next;
	}

	/*********************** print the modules ***********************/

    if (! loader)
    	fprintf(fp, "static AsnModule ampx[%d] = {\n", nummod);

	i = 1;
	curr_mod = amp;
	while (curr_mod != NULL)
	{
        if (! loader)
        {
    		fprintf(fp, "  { \"%s\" ,", curr_mod->modulename);
	    	if (curr_mod->filename == NULL)
		    	fprintf(fp, " NULL,");
    		else
	    		fprintf(fp, " \"%s\",", curr_mod->filename);
		    attp = AsnOutFindType(&p, curr_mod->types);
    		fprintf(fp, "&atx[%d],", attp->index);
	    	fprintf(fp, "NULL,");
		    if (curr_mod->next == NULL)
    		{
	    		fprintf(fp, "NULL,0,0}");
		    	fprintf(fp, " };\n\n");
    		}
	    	else
		    	fprintf(fp, "&ampx[%d],0,0} ,\n", i);
        }
        else
        {
    		fprintf(ld, "%s ", curr_mod->modulename);
	    	if (curr_mod->filename == NULL)
		    	fprintf(ld, "-32000 ");
    		else
	    		fprintf(ld, "%s ", curr_mod->filename);
		    attp = AsnOutFindType(&p, curr_mod->types);
    		fprintf(ld, "%d ", attp->index);
		    if (curr_mod->next == NULL)
	    		fprintf(ld, "-32000\n");
	    	else
		    	fprintf(ld, "%d\n", i);
        }
    	i++;
	    curr_mod = curr_mod->next;
	}

    if (loader)
        fclose(ld);
    else
    {
        if (numvalue)
            fprintf(fp, "static AsnValxNodePtr avn = avnx;\n");
        else
            fprintf(fp, "static AsnValxNodePtr avn = NULL;\n");
        fprintf(fp, "static AsnTypePtr at = atx;\n");
        fprintf(fp, "static AsnModulePtr amp = ampx;\n\n");
    }

	curr_mod = amp;
	while (curr_mod != NULL)
	{
		atp = curr_mod->types;
		fprintf(fp, "\n\n/**************************************************\n");
		fprintf(fp, "*\n");
		fprintf(fp, "*    Defines for Module %s\n", curr_mod->modulename);
		fprintf(fp, "*\n");
		fprintf(fp, "**************************************************/\n");
		while (atp != NULL)
		{
			if (! atp->imported)    /* must be defined elsewhere */
				AsnOutDefineType(fp, &p, atp, maxDefineLength);
			atp = atp->next;
		}
		curr_mod = curr_mod->next;
	}

	fclose(fp);
	return;	
}

/*****************************************************************************
*
*   void AsnOutDefineType(fp, app, atp, maxDefineLength)
*
*****************************************************************************/
NLM_EXTERN void AsnOutDefineType (FILE *fp, AsnProcPtr app, AsnTypePtr atp, Int2 maxDefineLength)

{
	char buf[200];

	AsnOutDefineElement(fp, app, atp, buf, NULL, maxDefineLength);
	return;
}

/*****************************************************************************
*
*   void AsnOutDefineElement(fp, app, atp, buf, pnt, maxDefineLength)
*
*****************************************************************************/
NLM_EXTERN void AsnOutDefineElement (FILE *fp, AsnProcPtr app, AsnTypePtr atp, CharPtr buf, CharPtr pnt, Int2 maxDefineLength)

{
	CharPtr to, from, symbol;
	AsnTmpTypePtr attp;
	Boolean first;
	int i;

	if (pnt == NULL)
		first = TRUE;
	else
		first = FALSE;

	while (atp != NULL)
	{
		to = pnt;
		if (atp->name != NULL)
		{
			from = atp->name;
			if (first)
				to = buf;
			else
			{
				*to = '_';
				to++;
			}
			while (*from != '\0')
			{
				if (first)
					*to = TO_UPPER(*from);
				else
					*to = *from;
				if (*to == '-')
					*to = '_';
				to++; from++;
			}
			*to = '\0';
			attp = AsnOutFindType(app, atp);
			if (first)
				fprintf(fp, "\n");
			symbol = to;
			i = maxDefineLength;    /* max symbol length */
			while ((i) && (symbol > buf))
			{
				i--; symbol--;
			}
			if (symbol != buf && StrChr(symbol, '_') != NULL)   /* longer than maxDefineLength */
			{
				while (*symbol != '_')   /* end neatly */
					symbol++;
				symbol++;
			}
			fprintf(fp, "#define %s &at[%d]\n", symbol, attp->index);
		}
		switch(atp->type->isa)
		{
			case SEQ_TYPE:
			case SET_TYPE:
			case CHOICE_TYPE:
				AsnOutDefineElement(fp, app, (AsnTypePtr) atp->branch, buf, to, maxDefineLength);
				break;
			case SEQOF_TYPE:
			case SETOF_TYPE:
				StringCpy(to, "_E");
				symbol = to + 2;
				i = maxDefineLength;     /* max symbol length */
				while ((i) && (symbol > buf))
				{
					i--; symbol--;
				}
				if (symbol != buf)   /* longer than maxDefineLength */
				{
					while (*symbol != '_')   /* end neatly */
						symbol++;
					symbol++;
				}
				attp = AsnOutFindType(app, (AsnTypePtr) atp->branch);
				fprintf(fp, "#define %s &at[%d]\n", symbol, attp->index);
				switch (((AsnTypePtr)atp->branch)->type->isa)
				{
					case CHOICE_TYPE:
					case SET_TYPE:
					case SEQ_TYPE:
					case SEQOF_TYPE:
					case SETOF_TYPE:
						AsnOutDefineElement(fp, app, (AsnTypePtr) atp->branch, buf, to+2, maxDefineLength);
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		if (first)
			return;
		atp = atp->next;
	}
	return;
}

/*****************************************************************************
*
*   Int2 AsnOutFindValue(app, avnp)
*   	returns index of a value node ptr
*
*****************************************************************************/
NLM_EXTERN Int2 AsnOutFindValue (AsnProcPtr app, AsnValxNodePtr avnp)

{
	AsnTmpValuePtr attvp;

	attvp = app->root_tmpvalue.next;
	while (attvp != NULL)
	{
		if (attvp->val == avnp)
			return attvp->index;
		attvp = attvp->next;
	}
	return -32000;
}
/*****************************************************************************
*
*   AsnTypePtr AsnOutAddType(AsnProcPtr, AsnTypePtr)
*   	builds temporaries for this type
*   	returns the next type
*
*****************************************************************************/
NLM_EXTERN AsnTypePtr AsnOutAddType (AsnProcPtr app, AsnTypePtr atp)

{
	AsnTypePtr atp2;
	AsnTypePtr next;

	if (atp == NULL)        /* protection */
		return NULL;

	if (! AsnOutNewType(app, atp))      /* already have it */
	{
		if (ISA_BASETYPE(atp->isa))
			return NULL;
		return atp->next;
	}

	if (ISA_BASETYPE(atp->isa))     /* all done if primitive */
		return NULL;

	if (atp->type == NULL)       /* externally defined type */
		return atp->next;        /* no more to do */

	switch (atp->type->isa)        /* load any subvalues */
	{
		case INTEGER_TYPE:
			if (atp->branch == NULL)    /* not named */
				break;
		case ENUM_TYPE:
			AsnOutNewValueChain(app, (AsnValxNodePtr) atp->branch);
			break;
		case SETOF_TYPE:
		case SEQOF_TYPE:
		case SEQ_TYPE:
		case SET_TYPE:
		case CHOICE_TYPE:
			atp2 = (AsnTypePtr) atp->branch;
			while (atp2 != NULL)
				atp2 = AsnOutAddType(app, atp2);
			break;
		default:
			break;
	}

	if (atp->hasdefault)
		AsnOutNewValueChain(app, atp->defaultvalue);

	AsnOutAddType(app, atp->type);

	next = atp->next;
	return next;
}


/*****************************************************************************
*
*   Boolean AsnOutNewType(app, atp)
*   	if type is on tmp list already, returns FALSE
*   	if not, adds it, and returns TRUE
*
*****************************************************************************/
NLM_EXTERN Boolean AsnOutNewType (AsnProcPtr app, AsnTypePtr atp)

{
	AsnTmpTypePtr attp;

	attp = AsnOutFindType(app, atp);
	if (attp != NULL)      /* have it already */
		return FALSE;
		
	            /* not found, add it */
	attp = (AsnTmpTypePtr) MemNew(sizeof(AsnTmpType));
	app->curr_tmptype->next = attp;
	app->curr_tmptype = attp;
	attp->ptr = atp;
	return TRUE;
}

/*****************************************************************************
*
*   AsnTmpTypePtr AsnOutFindType(app, atp)
*   	returns a pointer to the tmptype matching atp
*
*****************************************************************************/
NLM_EXTERN AsnTmpTypePtr AsnOutFindType (AsnProcPtr app, AsnTypePtr atp)

{
 	AsnTmpTypePtr attp;

	if (atp == NULL)      /* imported, no type set */
		return NULL;

	attp = app->root_tmptype.next;
	while (attp != NULL)
	{
		if (attp->ptr == atp)     /* found it */
			return attp;
		attp = attp->next;
	}
	return attp;         /* didn't find it */
}

/*****************************************************************************
*
*   void AsnOutNewValueChain(app, avnp)
*   	values are always used by just one type
*
*****************************************************************************/
NLM_EXTERN void AsnOutNewValueChain (AsnProcPtr app, AsnValxNodePtr avnp)

{
	AsnTmpValuePtr curr;

	curr = app->curr_tmpvalue;

	while (avnp != NULL)
	{
		curr->next = (AsnTmpValuePtr) MemNew(sizeof(AsnTmpValue));
		curr = curr->next;
		curr->val = avnp;
		avnp = avnp->next;
	}
	app->curr_tmpvalue = curr;
	return;
}

