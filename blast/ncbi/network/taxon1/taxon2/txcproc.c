/*  txcproc.c
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
* File Name:  txcproc.c
*
* Author:  Vladimir Soussov
*   
* File Description:  taxonomy client communication procedures
*
*
* $Log: txcproc.c,v $
* Revision 1.7  2003/03/05 21:32:00  soussov
* new lookup procedure
*
* Revision 1.6  1999/12/20 17:05:17  soussov
* taxid4gi added
*
* Revision 1.5  1998/07/27 16:55:08  soussov
* Subtree version
*
* Revision 1.4  1998/04/01 17:33:57  soussov
* changed tp include <>
*
* Revision 1.3  1998/03/13 19:18:54  soussov
* add CS_CANCELED case for ct_results
*
* Revision 1.2  1998/02/11 15:53:13  soussov
* ctpublic.h was added
*
* Revision 1.1  1998/02/10 20:12:06  soussov
* taxon2 related soft
*
*
*/

#include <ctpublic.h>
#include <txclient.h>

static CS_CONTEXT* te_context;
static CS_CONNECTION* te_link;
static CS_INT my_last_update= 0;

CS_RETCODE c_msg_proc(CS_CONTEXT* cp, CS_CONNECTION* chp, CS_CLIENTMSG* emsgp)
{
    char buff[512];

    if(emsgp->msgstringlen > 0) {
	sprintf(buff, "! ct error: %s\n", emsgp->msgstring);
	ErrPostEx(SEV_ERROR, 100, 1, buff);	
    }
    if((emsgp->osnumber != 0) && (emsgp->osstringlen > 0)) {
	sprintf(buff, "! os error: %s\n", emsgp->osstring);
	ErrPostEx(SEV_ERROR, 100, 2, buff);
    }
    if((emsgp->sqlstatelen > 0) && (strcmp(emsgp->sqlstate, "ZZZZZ") != 0)) {
	sprintf(buff, "! sql statement: %s\n", emsgp->sqlstate);
	ErrPostEx(SEV_ERROR, 100, 3, buff);
    }

    return CS_SUCCEED;
}

CS_RETCODE s_msg_proc(CS_CONTEXT* cp, CS_CONNECTION* chp, CS_SERVERMSG* msg)
{
    char buff[512];

    if((msg->textlen > 0) && (msg->severity > 0)) {
	sprintf(buff, "! server message: %s\t %s\t %s\t %s\n", msg->svrname, msg->proc, msg->text, msg->sqlstate);
	ErrPostEx(SEV_ERROR, 100, 4, buff);
    }
    return CS_SUCCEED;
}

Boolean txc_connect2Server(CharPtr srv_name, CharPtr usr, CharPtr passwd, CharPtr applic)
{
    CS_BOOL val= CS_TRUE;
    CS_INT t_out= 40;

    cs_ctx_global(CS_VERSION_110, &te_context);
    ct_init(te_context, CS_VERSION_110);
    ct_config(te_context, CS_SET, CS_TIMEOUT, &t_out, CS_UNUSED, NULL);

    ct_callback(te_context, NULL, CS_SET, CS_CLIENTMSG_CB, c_msg_proc);
    ct_callback(te_context, NULL, CS_SET, CS_SERVERMSG_CB, s_msg_proc);

    ct_con_alloc(te_context, &te_link);
    ct_con_props(te_link, CS_SET, CS_USERNAME, usr, CS_NULLTERM, NULL);
    ct_con_props(te_link, CS_SET, CS_PASSWORD, passwd, CS_NULLTERM, NULL);
    ct_con_props(te_link, CS_SET, CS_APPNAME, applic, CS_NULLTERM, NULL);
    if(ct_connect(te_link, srv_name, CS_NULLTERM) != CS_SUCCEED) {
	return FALSE;
    }
    return TRUE;
}

Int4 tax_findByName(CharPtr sname, int mode, TaxNamePtr* res_name)
{
    CS_COMMAND *cmd;
    CS_DATAFMT parfmt;
    CS_TINYINT smode;
    CS_INT res_type, rows_read, tax_id;
    char nbuff[256], ubuff[256];
    Boolean more_results= TRUE;
    Boolean more_fetch;
    CS_SMALLINT has_unique;
    CS_TINYINT class_cde, designator;
    Int4 n= 0, alloc_room= 0;
    TaxNamePtr fnd_name= NULL;
    int nnn= 64;

    if(sname == NULL) return 0;
    
    if(ct_cmd_alloc(te_link, &cmd) != CS_SUCCEED) return 0;


    /* build command: tax_findByName @name_txt, @mode */

    if(ct_command(cmd, CS_RPC_CMD, "tax_findByName", CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }
    
    memset(&parfmt, 0, sizeof(CS_DATAFMT));
    strcpy(parfmt.name, "@name_txt");
    parfmt.namelen= CS_NULLTERM;
    parfmt.datatype= CS_CHAR_TYPE;
    parfmt.maxlength= strlen(sname);
    parfmt.status= CS_INPUTVALUE;
    parfmt.locale= NULL;
    if(ct_param(cmd, &parfmt, sname, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }

    strcpy(parfmt.name, "@mode");
    parfmt.datatype= CS_TINYINT_TYPE;
    parfmt.maxlength= 1;
    parfmt.locale= NULL;
    smode= mode;
    if(ct_param(cmd, &parfmt, &smode, 1, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }

    /* send this command to server */
    if(ct_send(cmd) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }
    
    while(more_results && (--nnn > 0)) {
	switch(ct_results(cmd, &res_type)) {
	case CS_FAIL :
	    if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
		ct_close(te_link, CS_FORCE_CLOSE);
	    }
	case CS_CANCELED :
	    ct_cmd_drop(cmd);
	    return 0;

	case CS_END_RESULTS :
	    more_results= FALSE;
	    break;

	case CS_SUCCEED :
	    if(res_type == CS_ROW_RESULT) {
		/* bind and fetch these data */
		more_fetch= TRUE;
		parfmt.datatype= CS_INT_TYPE;
		parfmt.format= CS_FMT_UNUSED;
		parfmt.maxlength= 4;
		parfmt.scale= CS_SRC_VALUE;
		parfmt.precision= CS_SRC_VALUE;
		parfmt.count= 1;
		parfmt.locale= NULL;
		if(ct_bind(cmd, 1, &parfmt, &tax_id, NULL, NULL) != CS_SUCCEED) break;

		parfmt.datatype= CS_CHAR_TYPE;
		parfmt.format= CS_FMT_NULLTERM;
		parfmt.maxlength= 255;
		if((ct_bind(cmd, 2, &parfmt, nbuff, NULL, NULL) != CS_SUCCEED) ||
		   (ct_bind(cmd, 3, &parfmt, ubuff, NULL, &has_unique) != CS_SUCCEED)) break;

		parfmt.datatype= CS_TINYINT_TYPE;
		parfmt.format= CS_FMT_UNUSED;
		parfmt.maxlength= 1;
		if((ct_bind(cmd, 4, &parfmt, &class_cde, NULL, NULL) != CS_SUCCEED) ||
		   (ct_bind(cmd, 5, &parfmt, &designator, NULL, NULL) != CS_SUCCEED)) break;

		while(more_fetch) {
		    switch(ct_fetch(cmd,CS_UNUSED,CS_UNUSED,CS_UNUSED,&rows_read)) {
		    case CS_SUCCEED :
			if(n == 0) {
			    *res_name= fnd_name= malloc(sizeof(TaxName));
			    alloc_room= 1;
			}
			else if(n >= alloc_room) {
			    
			    alloc_room+= (8 > (alloc_room/4))? 8 : (alloc_room/4);
			    *res_name= realloc(fnd_name, alloc_room*sizeof(TaxName));
			    if(*res_name == NULL) {
				alloc_room= n--;
				*res_name= fnd_name;
			    }
			    else fnd_name= *res_name;
			}

			fnd_name[n].tax_id= tax_id;
			fnd_name[n].class_cde= class_cde;
			fnd_name[n].designator= designator;
			fnd_name[n].name_txt= StringSave(nbuff);
			fnd_name[n].unique_name= (has_unique < 0)? NULL : StringSave(ubuff);
			n++;
			continue;

		    case CS_ROW_FAIL :
			break;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;

		    default :
			/* this is a error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return FALSE;
		    }
		}
	    }
	    continue;

	default : break;
	}
    }
    ct_cmd_drop(cmd);
    return n;
}    

Int4 tax_getDesignator(char* sname)
{
    CS_COMMAND *cmd;
    CS_DATAFMT parfmt;
    CS_INT res_type, rows_read, tax_id= 0;
    Boolean more_results= TRUE;
    Boolean more_fetch;
    int nnn= 64;

    if((sname == NULL) || (*sname == '\0')) return 0;
    
    if(ct_cmd_alloc(te_link, &cmd) != CS_SUCCEED) return 0;

    /* build command: tax_getDesignator @name_txt */

    if(ct_command(cmd, CS_RPC_CMD, "tax_getDesignator", CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }
        
    memset(&parfmt, 0, sizeof(CS_DATAFMT));
    strcpy(parfmt.name, "@name_txt");
    parfmt.namelen= CS_NULLTERM;
    parfmt.datatype= CS_CHAR_TYPE;
    parfmt.maxlength= strlen(sname);
    parfmt.status= CS_INPUTVALUE;
    parfmt.locale= NULL;
    if(ct_param(cmd, &parfmt, sname, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }

    /* send this command to server */
    if(ct_send(cmd) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }
        
    while(more_results && (--nnn > 0)) {
	switch(ct_results(cmd, &res_type)) {
	case CS_FAIL :
	    if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
		ct_close(te_link, CS_FORCE_CLOSE);
	    }
	case CS_CANCELED :
	    ct_cmd_drop(cmd);
	    return 0;

	case CS_END_RESULTS :
	    more_results= FALSE;
	    break;

	case CS_SUCCEED :
	    if(res_type == CS_STATUS_RESULT) {
		more_fetch= TRUE;
		parfmt.datatype= CS_INT_TYPE;
		parfmt.format= CS_FMT_UNUSED;
		parfmt.maxlength= 4;
		parfmt.scale= CS_SRC_VALUE;
		parfmt.precision= CS_SRC_VALUE;
		parfmt.count= 1;
		if(ct_bind(cmd, 1, &parfmt, &tax_id, NULL, NULL) != CS_SUCCEED) break;
		while(more_fetch) {
		    switch(ct_fetch(cmd,CS_UNUSED,CS_UNUSED,CS_UNUSED,&rows_read)) {
		    case CS_SUCCEED :
			continue;

		    case CS_ROW_FAIL :
			break;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;

		    default :
			/* this is a error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return 0;
		    }
		}
	    }
	    continue;

	default : break;
	}
    }
    ct_cmd_drop(cmd);
    return tax_id;		
}


Int4 tax_uniqueName(char* sname, Int4 id)
{
    CS_COMMAND *cmd;
    CS_DATAFMT parfmt;
    CS_INT res_type, rows_read, tax_id= 0;
    Boolean more_results= TRUE;
    Boolean more_fetch;
    int nnn= 64;

    if((sname == NULL) || (*sname == '\0')) return 0;
    
    if(ct_cmd_alloc(te_link, &cmd) != CS_SUCCEED) return 0;

    /* build command: tax_checkUnique @name_txt, @tax_id */

    if(ct_command(cmd, CS_RPC_CMD, "tax_checkUnique", CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }
        
    memset(&parfmt, 0, sizeof(CS_DATAFMT));
    strcpy(parfmt.name, "@name_txt");
    parfmt.namelen= CS_NULLTERM;
    parfmt.datatype= CS_CHAR_TYPE;
    parfmt.maxlength= strlen(sname);
    parfmt.status= CS_INPUTVALUE;
    parfmt.locale= NULL;
    if(ct_param(cmd, &parfmt, sname, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }

    strcpy(parfmt.name, "@tax_id");
    parfmt.datatype= CS_INT_TYPE;
    parfmt.maxlength= sizeof(CS_INT);
    if(ct_param(cmd, &parfmt, &id, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }

    /* send this command to server */
    if(ct_send(cmd) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }
        
    while(more_results && (--nnn > 0)) {
	switch(ct_results(cmd, &res_type)) {
	case CS_FAIL :
	    if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
		ct_close(te_link, CS_FORCE_CLOSE);
	    }
	case CS_CANCELED :
	    ct_cmd_drop(cmd);
	    return 0;

	case CS_END_RESULTS :
	    more_results= FALSE;
	    break;

	case CS_SUCCEED :
	    if(res_type == CS_STATUS_RESULT) {
		more_fetch= TRUE;
		parfmt.datatype= CS_INT_TYPE;
		parfmt.format= CS_FMT_UNUSED;
		parfmt.maxlength= 4;
		parfmt.scale= CS_SRC_VALUE;
		parfmt.precision= CS_SRC_VALUE;
		parfmt.count= 1;
		if(ct_bind(cmd, 1, &parfmt, &tax_id, NULL, NULL) != CS_SUCCEED) break;
		while(more_fetch) {
		    switch(ct_fetch(cmd,CS_UNUSED,CS_UNUSED,CS_UNUSED,&rows_read)) {
		    case CS_SUCCEED :
			continue;

		    case CS_ROW_FAIL :
			break;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;

		    default :
			/* this is a error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return 0;
		    }
		}
	    }
	    continue;

	default : break;
	}
    }
    ct_cmd_drop(cmd);
    return tax_id;		
}


Int4 tax_getIdByName(char* sname, char* qualif, Int1 id)
{
    CS_COMMAND *cmd;
    CS_DATAFMT parfmt;
    CS_INT res_type, rows_read, tax_id= 0;
    int nnn= 64;
    Boolean more_results= TRUE;
    Boolean more_fetch;
    char s_sname[100];
    char s_qualif[120];

    if((sname == NULL) || (*sname == '\0')) return 0;
    
    if(ct_cmd_alloc(te_link, &cmd) != CS_SUCCEED) return 0;

    /* build command: tax_getIdByName(@org_name, @qualif, @q_type) */

    if(ct_command(cmd, CS_RPC_CMD, "tax_getIdByName", CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }
        
    memset(&parfmt, 0, sizeof(CS_DATAFMT));
    strcpy(parfmt.name, "@org_name");
    parfmt.namelen= CS_NULLTERM;
    parfmt.datatype= CS_CHAR_TYPE;
    strncpy(s_sname, sname, 96);
    s_sname[96]= '\0';
    parfmt.maxlength= strlen(s_sname);
    parfmt.status= CS_INPUTVALUE;
    parfmt.locale= NULL;
    if(ct_param(cmd, &parfmt, s_sname, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }
    strcpy(parfmt.name, "@qualif");
    if(qualif == NULL) {
	s_qualif[0]= s_qualif[1]= '\0';
	parfmt.maxlength= 1;
    }
    else {
	strncpy(s_qualif, qualif, 116);
	s_qualif[116]= '\0';
	parfmt.maxlength= strlen(s_qualif);
    }

    if(ct_param(cmd, &parfmt, s_qualif, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }

    strcpy(parfmt.name, "@q_type");
    parfmt.datatype= CS_TINYINT_TYPE;
    parfmt.maxlength= sizeof(CS_TINYINT);
    if(ct_param(cmd, &parfmt, &id, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }

    /* send this command to server */
    if(ct_send(cmd) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }
        
    while(more_results && (--nnn > 0)) {
	switch(ct_results(cmd, &res_type)) {
	case CS_FAIL :
	    if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
		ct_close(te_link, CS_FORCE_CLOSE);
	    }
	case CS_CANCELED :
	    ct_cmd_drop(cmd);
	    return 0;

	case CS_END_RESULTS :
	    more_results= FALSE;
	    break;

	case CS_SUCCEED :
	    if(res_type == CS_STATUS_RESULT) {
		more_fetch= TRUE;
		parfmt.datatype= CS_INT_TYPE;
		parfmt.format= CS_FMT_UNUSED;
		parfmt.maxlength= 4;
		parfmt.scale= CS_SRC_VALUE;
		parfmt.precision= CS_SRC_VALUE;
		parfmt.count= 1;
		if(ct_bind(cmd, 1, &parfmt, &tax_id, NULL, NULL) != CS_SUCCEED) break;
		while(more_fetch) {
		    switch(ct_fetch(cmd,CS_UNUSED,CS_UNUSED,CS_UNUSED,&rows_read)) {
		    case CS_SUCCEED :
			continue;

		    case CS_ROW_FAIL :
			break;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;

		    default :
			/* this is a error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return 0;
		    }
		}
	    }
	    continue;

	default : break;
	}
    }
    ct_cmd_drop(cmd);
    return tax_id;		
}


Int4 tax_getOrgNames(Int4 tax_id, TaxNamePtr* res_name)
{
    CS_COMMAND *cmd;
    CS_DATAFMT parfmt;
    CS_TINYINT smode;
    CS_INT res_type, rows_read;
    Boolean more_results= TRUE;
    Boolean more_fetch;
    char nbuff[256], ubuff[256];
    CS_SMALLINT has_unique;
    CS_TINYINT class_cde, designator;
    Int4 n= 0, alloc_room= 0;
    TaxNamePtr fnd_name= NULL;
    int nnn= 64;

    if(tax_id <= 0) return 0;
    
    if(ct_cmd_alloc(te_link, &cmd) != CS_SUCCEED) return 0;


    /* build command: tax_getOrgNames @tax_id */

    if(ct_command(cmd, CS_RPC_CMD, "tax_getOrgNames", CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }
    
    memset(&parfmt, 0, sizeof(CS_DATAFMT));
    strcpy(parfmt.name, "@tax_id");
    parfmt.namelen= CS_NULLTERM;
    parfmt.datatype= CS_INT_TYPE;
    parfmt.maxlength= sizeof(Int4);
    parfmt.status= CS_INPUTVALUE;
    parfmt.locale= NULL;
    if(ct_param(cmd, &parfmt, &tax_id, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }

    /* send this command to server */
    if(ct_send(cmd) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }
    
    while(more_results && (--nnn > 0)) {
	switch(ct_results(cmd, &res_type)) {
	case CS_FAIL :
	    if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
		ct_close(te_link, CS_FORCE_CLOSE);
	    }
	case CS_CANCELED :
	    ct_cmd_drop(cmd);
	    return 0;

	case CS_END_RESULTS :
	    more_results= FALSE;
	    break;

	case CS_SUCCEED :
	    if(res_type == CS_ROW_RESULT) {
		/* bind and fetch these data */
		more_fetch= TRUE;
		parfmt.datatype= CS_INT_TYPE;
		parfmt.format= CS_FMT_UNUSED;
		parfmt.maxlength= 4;
		parfmt.scale= CS_SRC_VALUE;
		parfmt.precision= CS_SRC_VALUE;
		parfmt.count= 1;
		parfmt.locale= NULL;
		if(ct_bind(cmd, 1, &parfmt, &tax_id, NULL, NULL) != CS_SUCCEED) break;

		parfmt.datatype= CS_CHAR_TYPE;
		parfmt.format= CS_FMT_NULLTERM;
		parfmt.maxlength= 255;
		if((ct_bind(cmd, 2, &parfmt, nbuff, NULL, NULL) != CS_SUCCEED) ||
		   (ct_bind(cmd, 3, &parfmt, ubuff, NULL, &has_unique) != CS_SUCCEED)) break;

		parfmt.datatype= CS_TINYINT_TYPE;
		parfmt.format= CS_FMT_UNUSED;
		parfmt.maxlength= 1;
		if((ct_bind(cmd, 4, &parfmt, &class_cde, NULL, NULL) != CS_SUCCEED) ||
		   (ct_bind(cmd, 5, &parfmt, &designator, NULL, NULL) != CS_SUCCEED)) break;

		while(more_fetch) {
		    switch(ct_fetch(cmd,CS_UNUSED,CS_UNUSED,CS_UNUSED,&rows_read)) {
		    case CS_SUCCEED :
			if(n == 0) {
			    *res_name= fnd_name= malloc(sizeof(TaxName));
			    alloc_room= 1;
			}
			else if(n >= alloc_room) {
			    
			    alloc_room+= (8 > (alloc_room/4))? 8 : (alloc_room/4);
			    *res_name= realloc(fnd_name, alloc_room*sizeof(TaxName));
			    if(*res_name == NULL) {
				alloc_room= n--;
				*res_name= fnd_name;
			    }
			    else fnd_name= *res_name;
			}

			fnd_name[n].tax_id= tax_id;
			fnd_name[n].class_cde= class_cde;
			fnd_name[n].designator= designator;
			fnd_name[n].name_txt= StringSave(nbuff);
			fnd_name[n].unique_name= (has_unique < 0)? NULL : StringSave(ubuff);
			n++;
			continue;

		    case CS_ROW_FAIL :
			break;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;

		    default :
			/* this is an error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return FALSE;
		    }
		}
	    }
	    continue;

	default : break;
	}
    }
    ct_cmd_drop(cmd);
    return n;
}    


TXC_NodeDataPtr txc_getOrgData(Int4 tax_id)
{
    CS_COMMAND *cmd;
    CS_DATAFMT parfmt;
    CS_INT res_type, rows_read;
    CS_SMALLINT rank_id, gc_id, mgc_id, div_id;
    Boolean more_results= TRUE;
    Boolean more_fetch;
    TXC_NodeDataPtr node_data= NULL;
    int nnn= 64;
    char embl_buff[8];

    if(tax_id <= 0) return 0;
    
    if(ct_cmd_alloc(te_link, &cmd) != CS_SUCCEED) return 0;


    /* build command: tax_getOrgData @tax_id */

    if(ct_command(cmd, CS_RPC_CMD, "tax_getOrgData", CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }
    
    memset(&parfmt, 0, sizeof(CS_DATAFMT));
    strcpy(parfmt.name, "@tax_id");
    parfmt.namelen= CS_NULLTERM;
    parfmt.datatype= CS_INT_TYPE;
    parfmt.maxlength= sizeof(Int4);
    parfmt.status= CS_INPUTVALUE;
    parfmt.locale= NULL;
    if(ct_param(cmd, &parfmt, &tax_id, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }

    /* send this command to server */
    if(ct_send(cmd) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }
    
    while(more_results && (--nnn > 0)) {
	switch(ct_results(cmd, &res_type)) {
	case CS_FAIL :
	    if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
		ct_close(te_link, CS_FORCE_CLOSE);
	    }
	case CS_CANCELED :
	    ct_cmd_drop(cmd);
	    return NULL;

	case CS_END_RESULTS :
	    more_results= FALSE;
	    break;

	case CS_SUCCEED :
	    if(res_type == CS_ROW_RESULT) {
		/* bind and fetch these data */
		if(node_data == NULL) {
		    if((node_data= MemNew(sizeof(TXC_NodeData))) == NULL) {
			ct_cancel(NULL, cmd, CS_CANCEL_ALL);
			ct_cmd_drop(cmd);
			return NULL;
		    }
		    node_data->tax_id= tax_id;
		}
		    
		more_fetch= TRUE;
		parfmt.datatype= CS_INT_TYPE;
		parfmt.format= CS_FMT_UNUSED;
		parfmt.maxlength= 4;
		parfmt.scale= CS_SRC_VALUE;
		parfmt.precision= CS_SRC_VALUE;
		parfmt.count= 1;
		parfmt.locale= NULL;
		if((ct_bind(cmd, 1, &parfmt, &node_data->crt_date, NULL, NULL) != CS_SUCCEED) ||
		   (ct_bind(cmd, 2, &parfmt, &node_data->upd_date, NULL, NULL) != CS_SUCCEED)) break;

		parfmt.datatype= CS_SMALLINT_TYPE;
		parfmt.maxlength= 2;
		if((ct_bind(cmd, 3, &parfmt, &rank_id, NULL, NULL) != CS_SUCCEED) ||
		   (ct_bind(cmd, 4, &parfmt, &gc_id, NULL, NULL) != CS_SUCCEED) ||
		   (ct_bind(cmd, 5, &parfmt, &mgc_id, NULL, NULL) != CS_SUCCEED) ||
		   (ct_bind(cmd, 6, &parfmt, &div_id, NULL, NULL) != CS_SUCCEED) ||
		   (ct_bind(cmd, 7, &parfmt, &node_data->flags, NULL, NULL) != CS_SUCCEED)) break;

		parfmt.datatype= CS_CHAR_TYPE;
		parfmt.format= CS_FMT_NULLTERM;
		parfmt.maxlength= 8;
		if(ct_bind(cmd, 8, &parfmt, embl_buff, NULL, NULL) != CS_SUCCEED) break;

		while(more_fetch) {
		    switch(ct_fetch(cmd, CS_UNUSED, CS_UNUSED, CS_UNUSED, &rows_read)) {
		    case CS_SUCCEED :
			node_data->rank_id= rank_id;
			node_data->gc_id= gc_id;
			node_data->mgc_id= mgc_id;
			node_data->div_id= div_id;
			StringNCpy(node_data->embl_cde, embl_buff, 4);
			continue;

		    case CS_ROW_FAIL :
			break;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;

		    default :
			/* this is an error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return FALSE;
		    }
		}
	    }
	    continue;

	default : break;
	}
    }
    ct_cmd_drop(cmd);
    return node_data;
}    


Boolean txc_loadNameClasses()
{
    CS_COMMAND *cmd;
    CS_DATAFMT datafmt;
    CS_SMALLINT class_cde, priority;
    CS_INT res_type, rows_read;
    int nnn= 64;
    Boolean more_results= TRUE;
    Boolean more_fetch;
    char class_txt[128];
    
    if(ct_cmd_alloc(te_link, &cmd) != CS_SUCCEED) return 0;


    /* build command: tax_dumpClasses() */

    if(ct_command(cmd, CS_RPC_CMD, "tax_dumpClasses", CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return FALSE;
    }
    
    /* send this command to server */
    if(ct_send(cmd) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return FALSE;
    }
    
    memset(&datafmt, 0, sizeof(CS_DATAFMT));

    while(more_results && (--nnn > 0)) {
	switch(ct_results(cmd, &res_type)) {
	case CS_FAIL :
	    if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
		ct_close(te_link, CS_FORCE_CLOSE);
	    }
	case CS_CANCELED :
	    ct_cmd_drop(cmd);
	    return FALSE;

	case CS_END_RESULTS :
	    more_results= FALSE;
	    break;

	case CS_SUCCEED :
	    if(res_type == CS_ROW_RESULT) {
		/* bind and fetch these data */
		more_fetch= TRUE;
		datafmt.datatype= CS_SMALLINT_TYPE;
		datafmt.format= CS_FMT_UNUSED;
		datafmt.maxlength= sizeof(CS_SMALLINT);
		datafmt.scale= CS_SRC_VALUE;
		datafmt.precision= CS_SRC_VALUE;
		datafmt.count= 1;
		datafmt.locale= NULL;
		if((ct_bind(cmd, 1, &datafmt, &class_cde, NULL, NULL) != CS_SUCCEED) ||
		   (ct_bind(cmd, 2, &datafmt, &priority, NULL, NULL) != CS_SUCCEED)) break;

		datafmt.datatype= CS_CHAR_TYPE;
		datafmt.format= CS_FMT_NULLTERM;
		datafmt.maxlength= 128;
		if(ct_bind(cmd, 3, &datafmt, class_txt, NULL, NULL) != CS_SUCCEED) break;

		while(more_fetch) {
		    switch(ct_fetch(cmd, CS_UNUSED, CS_UNUSED, CS_UNUSED, &rows_read)) {
		    case CS_SUCCEED :
			tax_addNameClass(class_cde, class_txt, priority);
			continue;

		    case CS_ROW_FAIL :
			break;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;

		    default :
			/* this is an error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return FALSE;
		    }
		}
	    }
	    continue;

	default : break;
	}
    }
    ct_cmd_drop(cmd);
    return TRUE;
}    


Boolean txc_loadRanks()
{
    CS_COMMAND *cmd;
    CS_DATAFMT datafmt;
    CS_SMALLINT rank_id;
    CS_INT res_type, rows_read;
    int nnn= 64;
    Boolean more_results= TRUE;
    Boolean more_fetch;
    char rank_txt[128];
    
    if(ct_cmd_alloc(te_link, &cmd) != CS_SUCCEED) return 0;


    /* build command: tax_dumpRanks() */

    if(ct_command(cmd, CS_RPC_CMD, "tax_dumpRanks", CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return FALSE;
    }
    
    /* send this command to server */
    if(ct_send(cmd) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return FALSE;
    }
    
    memset(&datafmt, 0, sizeof(CS_DATAFMT));

    while(more_results && (--nnn > 0)) {
	switch(ct_results(cmd, &res_type)) {
	case CS_FAIL :
	    if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
		ct_close(te_link, CS_FORCE_CLOSE);
	    }
	case CS_CANCELED :
	    ct_cmd_drop(cmd);
	    return FALSE;

	case CS_END_RESULTS :
	    more_results= FALSE;
	    break;

	case CS_SUCCEED :
	    if(res_type == CS_ROW_RESULT) {
		/* bind and fetch these data */
		more_fetch= TRUE;
		datafmt.datatype= CS_SMALLINT_TYPE;
		datafmt.format= CS_FMT_UNUSED;
		datafmt.maxlength= sizeof(CS_SMALLINT);
		datafmt.scale= CS_SRC_VALUE;
		datafmt.precision= CS_SRC_VALUE;
		datafmt.count= 1;
		datafmt.locale= NULL;
		if(ct_bind(cmd, 1, &datafmt, &rank_id, NULL, NULL) != CS_SUCCEED) break;

		datafmt.datatype= CS_CHAR_TYPE;
		datafmt.format= CS_FMT_NULLTERM;
		datafmt.maxlength= 128;
		if(ct_bind(cmd, 2, &datafmt, rank_txt, NULL, NULL) != CS_SUCCEED) break;

		while(more_fetch) {
		    switch(ct_fetch(cmd, CS_UNUSED, CS_UNUSED, CS_UNUSED, &rows_read)) {
		    case CS_SUCCEED :
			tax_addRank(rank_id, rank_txt);
			continue;

		    case CS_ROW_FAIL :
			break;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;

		    default :
			/* this is an error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return FALSE;
		    }
		}
	    }
	    continue;

	default : break;
	}
    }
    ct_cmd_drop(cmd);
    return TRUE;
}    


Boolean txc_loadDivisions()
{
    CS_COMMAND *cmd;
    CS_DATAFMT datafmt;
    CS_SMALLINT div_id;
    CS_INT res_type, rows_read;
    int nnn= 64;
    Boolean more_results= TRUE;
    Boolean more_fetch;
    char div_cde[8], div_txt[128];
    
    if(ct_cmd_alloc(te_link, &cmd) != CS_SUCCEED) return 0;


    /* build command: tax_dumpDivisions() */

    if(ct_command(cmd, CS_RPC_CMD, "tax_dumpDivisions", CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return FALSE;
    }
    
    /* send this command to server */
    if(ct_send(cmd) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return FALSE;
    }
    
    memset(&datafmt, 0, sizeof(CS_DATAFMT));

    while(more_results && (--nnn > 0)) {
	switch(ct_results(cmd, &res_type)) {
	case CS_FAIL :
	    if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
		ct_close(te_link, CS_FORCE_CLOSE);
	    }
	case CS_CANCELED :
	    ct_cmd_drop(cmd);
	    return FALSE;

	case CS_END_RESULTS : 
	    more_results= FALSE;
	    break;

	case CS_SUCCEED :
	    if(res_type == CS_ROW_RESULT) {
		/* bind and fetch these data */
		more_fetch= TRUE;
		datafmt.datatype= CS_SMALLINT_TYPE;
		datafmt.format= CS_FMT_UNUSED;
		datafmt.maxlength= sizeof(CS_SMALLINT);
		datafmt.scale= CS_SRC_VALUE;
		datafmt.precision= CS_SRC_VALUE;
		datafmt.count= 1;
		datafmt.locale= NULL;
		if(ct_bind(cmd, 1, &datafmt, &div_id, NULL, NULL) != CS_SUCCEED) break;

		datafmt.datatype= CS_CHAR_TYPE;
		datafmt.format= CS_FMT_NULLTERM;
		datafmt.maxlength= 8;
		if(ct_bind(cmd, 2, &datafmt, div_cde, NULL, NULL) != CS_SUCCEED) break;

		datafmt.maxlength= 128;
		if(ct_bind(cmd, 3, &datafmt, div_txt, NULL, NULL) != CS_SUCCEED) break;		

		while(more_fetch) {
		    switch(ct_fetch(cmd, CS_UNUSED, CS_UNUSED, CS_UNUSED, &rows_read)) {
		    case CS_SUCCEED :
			tax_addDivision(div_id, div_cde, div_txt);
			continue;

		    case CS_ROW_FAIL :
			break;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;

		    default :
			/* this is an error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return FALSE;
		    }
		}
	    }
	    continue;

	default : break;
	}
    }
    ct_cmd_drop(cmd);
    return TRUE;
}    

Boolean txc_loadGCs()
{
    CS_COMMAND *cmd;
    CS_DATAFMT datafmt;
    CS_SMALLINT gc_id;
    CS_INT res_type, rows_read;
    int nnn= 64;
    Boolean more_results= TRUE;
    Boolean more_fetch;
    char gc_name[128];
    
    if(ct_cmd_alloc(te_link, &cmd) != CS_SUCCEED) return 0;


    /* build command: tax_dumpGCs() */

    if(ct_command(cmd, CS_RPC_CMD, "tax_dumpGCs", CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return FALSE;
    }
    
    /* send this command to server */
    if(ct_send(cmd) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return FALSE;
    }
    
    memset(&datafmt, 0, sizeof(CS_DATAFMT));

    while(more_results && (--nnn > 0)) {
	switch(ct_results(cmd, &res_type)) {
	case CS_FAIL :
	    if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
		ct_close(te_link, CS_FORCE_CLOSE);
	    }
	case CS_CANCELED :
	    ct_cmd_drop(cmd);
	    return FALSE;

	case CS_END_RESULTS :
	    more_results= FALSE;
	    break;

	case CS_SUCCEED :
	    if(res_type == CS_ROW_RESULT) {
		/* bind and fetch these data */
		more_fetch= TRUE;
		datafmt.datatype= CS_SMALLINT_TYPE;
		datafmt.format= CS_FMT_UNUSED;
		datafmt.maxlength= sizeof(CS_SMALLINT);
		datafmt.scale= CS_SRC_VALUE;
		datafmt.precision= CS_SRC_VALUE;
		datafmt.count= 1;
		datafmt.locale= NULL;
		if(ct_bind(cmd, 1, &datafmt, &gc_id, NULL, NULL) != CS_SUCCEED) break;

		datafmt.datatype= CS_CHAR_TYPE;
		datafmt.format= CS_FMT_NULLTERM;
		datafmt.maxlength= 128;
		if(ct_bind(cmd, 2, &datafmt, gc_name, NULL, NULL) != CS_SUCCEED) break;

		while(more_fetch) {
		    switch(ct_fetch(cmd, CS_UNUSED, CS_UNUSED, CS_UNUSED, &rows_read)) {
		    case CS_SUCCEED :
			tax_addGC(gc_id, gc_name);
			continue;

		    case CS_ROW_FAIL :
			break;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;

		    default :
			/* this is an error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return FALSE;
		    }
		}
	    }
	    continue;

	default : break;
	}
    }
    ct_cmd_drop(cmd);
    return TRUE;
}    


TXC_TreeNodePtr* txc_getLineage(Int4 lin_id, Int4Ptr lin_len)
{
    CS_COMMAND *cmd;
    CS_DATAFMT parfmt;
    CS_INT res_type, rows_read, tax_id, flags;
    int nnn= 64;
    Boolean more_results= TRUE;
    Boolean more_fetch;
    Int4 n, l, alloc_room= 48;
    char label[512];
    TXC_TreeNodePtr* lineage= NULL;
    CharPtr c;

    *lin_len= 0;
	
    if(ct_cmd_alloc(te_link, &cmd) != CS_SUCCEED) return FALSE;


    /* build command: tax_getLineage @tax_id=lin_id */

    if(ct_command(cmd, CS_RPC_CMD, "tax_getLineage", CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }
    
    memset(&parfmt, 0, sizeof(CS_DATAFMT));
    strcpy(parfmt.name, "@tax_id");
    parfmt.namelen= CS_NULLTERM;
    parfmt.datatype= CS_INT_TYPE;
    parfmt.maxlength= CS_UNUSED;
    parfmt.status= CS_INPUTVALUE;
    parfmt.locale= NULL;
    if(ct_param(cmd, &parfmt, &lin_id, 4, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }


    /* send this command to server */
    if(ct_send(cmd) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }


    while(more_results && (--nnn > 0)) {
	switch(ct_results(cmd, &res_type)) {
	case CS_FAIL :
	    if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
		ct_close(te_link, CS_FORCE_CLOSE);
	    }
	case CS_CANCELED :
	    ct_cmd_drop(cmd);
	    return NULL;

	case CS_END_RESULTS :
	    more_results= FALSE;
	    break;

	case CS_SUCCEED :
	    if(res_type == CS_ROW_RESULT) {
		/* bind and fetch these data */
		more_fetch= TRUE;
		parfmt.datatype= CS_INT_TYPE;
		parfmt.format= CS_FMT_UNUSED;
		parfmt.maxlength= 4;
		parfmt.scale= CS_SRC_VALUE;
		parfmt.precision= CS_SRC_VALUE;
		parfmt.count= 1;
		parfmt.locale= NULL;
		if((ct_bind(cmd, 1, &parfmt, &tax_id, NULL, NULL) != CS_SUCCEED) ||
		   (ct_bind(cmd, 2, &parfmt, &flags, NULL, NULL) != CS_SUCCEED)) {
		    break;
		}
		parfmt.datatype= CS_CHAR_TYPE;
		parfmt.format= CS_FMT_NULLTERM;
		parfmt.maxlength= 255;
		if(ct_bind(cmd, 3, &parfmt, label, NULL, NULL) != CS_SUCCEED) break;

		if(lineage == NULL) {
		    lineage= MemNew(alloc_room*sizeof(TXC_TreeNodePtr));
		    if(lineage == NULL) {
			ct_cancel(NULL, cmd, CS_CANCEL_ALL);
			ct_cmd_drop(cmd);
			return NULL;
		    }
		    n= 0;
		}

		while(more_fetch) {
		    switch(ct_fetch(cmd,CS_UNUSED,CS_UNUSED,CS_UNUSED,&rows_read)) {
		    case CS_SUCCEED :
			if(n >= alloc_room) {
			    /* need more memory */
			    TXC_TreeNodePtr* tmp= MemMore(lineage, (alloc_room + 8)*sizeof(TXC_TreeNodePtr));

			    if(tmp == NULL) continue;
			    lineage= tmp;
			    alloc_room+= 8;
			}
			l= StringLen(label) + 1;
			if((lineage[n]= MemNew(l + 8)) != NULL) {
			    lineage[n]->tax_id= tax_id;
			    lineage[n]->flags= flags;
			    StringCpy(lineage[n]->node_label, label);
			    if((flags & TXC_SUFFIX) != 0) {
				c= StringChr(lineage[n]->node_label, '~');
				if(c != NULL) *c= '\0';
				lineage[n]->flags^= TXC_SUFFIX; /* clear this flag */
			    }
			    n++;
			}
			continue;

		    case CS_ROW_FAIL :
			break;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;

		    default :
			/* this is a error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return NULL;
		    }
		}
	    }
	    continue;

	default : break;
	}
    }
    ct_cmd_drop(cmd);
    *lin_len= n;
    return lineage;
}    


TXC_TreeNodePtr* txc_getChildren(Int4 node_id, Int4Ptr nof_children)
{
    CS_COMMAND *cmd;
    CS_DATAFMT parfmt;
    CS_INT res_type, rows_read, tax_id, parent_id, flags, last_parent= -1;
    CS_SMALLINT levels= 1;
    int nnn= 64;
    Boolean more_results= TRUE;
    Boolean more_fetch;
    Int4 n, l, alloc_room= 16;
    char label[512];
    TXC_TreeNodePtr* child= NULL;
    CharPtr c;

    if(node_id < 0) {
	node_id= -node_id;
	levels= 64;
	alloc_room= 64*1024;
    }

    *nof_children= 0;
	
    if(ct_cmd_alloc(te_link, &cmd) != CS_SUCCEED) return NULL;


    /* build command: tax_getSubtree @root_id=root, @levels= levels */

    if(ct_command(cmd, CS_RPC_CMD, "tax_getSubtree", CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }
    
    memset(&parfmt, 0, sizeof(CS_DATAFMT));
    strcpy(parfmt.name, "@root_id");
    parfmt.namelen= CS_NULLTERM;
    parfmt.datatype= CS_INT_TYPE;
    parfmt.maxlength= CS_UNUSED;
    parfmt.status= CS_INPUTVALUE;
    parfmt.locale= NULL;
    if(ct_param(cmd, &parfmt, &node_id, 4, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }

    strcpy(parfmt.name, "@levels");
    parfmt.datatype= CS_SMALLINT_TYPE;
    parfmt.locale= NULL;
    if(ct_param(cmd, &parfmt, &levels, 2, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }

    /* send this command to server */
    if(ct_send(cmd) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }


    while(more_results && (--nnn > 0)) {
	switch(ct_results(cmd, &res_type)) {
	case CS_FAIL :
	    if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
		ct_close(te_link, CS_FORCE_CLOSE);
	    }
	case CS_CANCELED :
	    ct_cmd_drop(cmd);
	    return NULL;

	case CS_END_RESULTS :
	    more_results= FALSE;
	    break;

	case CS_SUCCEED :
	    if(res_type == CS_STATUS_RESULT) {
		more_fetch= TRUE;
		parfmt.datatype= CS_INT_TYPE;
		parfmt.format= CS_FMT_UNUSED;
		parfmt.maxlength= 4;
		parfmt.scale= CS_SRC_VALUE;
		parfmt.precision= CS_SRC_VALUE;
		parfmt.count= 1;
		if(ct_bind(cmd, 1, &parfmt, &tax_id, NULL, NULL) != CS_SUCCEED) break;
		while(more_fetch) {
		    switch(ct_fetch(cmd,CS_UNUSED,CS_UNUSED,CS_UNUSED,&rows_read)) {
		    case CS_SUCCEED :
			continue;

		    case CS_ROW_FAIL :
			break;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;

		    default :
			/* this is a error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return 0;
		    }
		}
	    }
	    else if(res_type == CS_ROW_RESULT) {
		/* bind and fetch these data */
		more_fetch= TRUE;
		parfmt.datatype= CS_INT_TYPE;
		parfmt.format= CS_FMT_UNUSED;
		parfmt.maxlength= 4;
		parfmt.scale= CS_SRC_VALUE;
		parfmt.precision= CS_SRC_VALUE;
		parfmt.count= 1;
		parfmt.locale= NULL;
		if((ct_bind(cmd, 1, &parfmt, &parent_id, NULL, NULL) != CS_SUCCEED) ||
		   (ct_bind(cmd, 2, &parfmt, &tax_id, NULL, NULL) != CS_SUCCEED) ||
		   (ct_bind(cmd, 3, &parfmt, &flags, NULL, NULL) != CS_SUCCEED)) {
		    break;
		}
		parfmt.datatype= CS_CHAR_TYPE;
		parfmt.format= CS_FMT_NULLTERM;
		parfmt.maxlength= 255;
		if(ct_bind(cmd, 4, &parfmt, label, NULL, NULL) != CS_SUCCEED) break;


		while(more_fetch) {
		    switch(ct_fetch(cmd,CS_UNUSED,CS_UNUSED,CS_UNUSED,&rows_read)) {
		    case CS_SUCCEED :
			if(((levels == 1) && (parent_id != node_id)) || (tax_id == node_id)) continue;
			
			if(child == NULL) {
			    child= MemNew(alloc_room*sizeof(TXC_TreeNodePtr));
			    if(child == NULL) {
				ct_cancel(NULL, cmd, CS_CANCEL_ALL);
				ct_cmd_drop(cmd);
				return NULL;
			    }
			    n= 0;
			}
			else if(n >= alloc_room-1) {
			    /* need more memory */
			    TXC_TreeNodePtr* tmp= 
				MemMore(child, (alloc_room + 8*levels)*sizeof(TXC_TreeNodePtr));

			    if(tmp == NULL) continue;
			    child= tmp;
			    alloc_room+= 8*levels;
			}
			if((levels > 1) && (parent_id != last_parent)) {
			    last_parent= parent_id;
			    child[n]= MemNew(8);
			    child[n]->tax_id= parent_id;
			    child[n++]->flags= TXC_SUFFIX;
			}
			l= StringLen(label) + 1;
			if((child[n]= MemNew(l + 8)) != NULL) {
			    child[n]->tax_id= tax_id;
			    child[n]->flags= flags;
			    StringCpy(child[n]->node_label, label);
			    if((flags & TXC_SUFFIX) != 0) {
				c= StringChr(child[n]->node_label, '~');
				if(c != NULL) *c= '\0';
				child[n]->flags^= TXC_SUFFIX; /* clear this flag */
			    }
			    n++;
			}
			continue;

		    case CS_ROW_FAIL :
			break;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;
		    default :
			/* this is a error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return NULL;
		    }
		}
	    }
	    continue;

	default : break;
	}
    }
    ct_cmd_drop(cmd);
    *nof_children= n;
    if(n == 0) {
	MemFree(child);
	child= NULL;
    }
    return child;
}    


TXC_CitListPtr txc_citGet4Node(Int4 tax_id, Int4Ptr nof_cit)
{
    CS_COMMAND *cmd;
    CS_DATAFMT parfmt;
    CS_INT res_type, rows_read;
    int nnn= 64;
    Boolean more_results= TRUE;
    Boolean more_fetch;
    char cit_key[132];
    CS_INT cit_id;
    Int4 n= 0, alloc_room= 0;
    TXC_CitListPtr refer= NULL, tmp;

    if(tax_id <= 0) return NULL;
    
    if(ct_cmd_alloc(te_link, &cmd) != CS_SUCCEED) return NULL;


    /* build command: tax_citGet4Node @tax_id */

    if(ct_command(cmd, CS_RPC_CMD, "tax_getCit4Node", CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }
    
    memset(&parfmt, 0, sizeof(CS_DATAFMT));
    strcpy(parfmt.name, "@tax_id");
    parfmt.namelen= CS_NULLTERM;
    parfmt.datatype= CS_INT_TYPE;
    parfmt.maxlength= sizeof(Int4);
    parfmt.status= CS_INPUTVALUE;
    parfmt.locale= NULL;
    if(ct_param(cmd, &parfmt, &tax_id, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }

    /* send this command to server */
    if(ct_send(cmd) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }
    
    while(more_results && (--nnn > 0)) {
	switch(ct_results(cmd, &res_type)) {
	case CS_FAIL :
	    if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
		ct_close(te_link, CS_FORCE_CLOSE);
	    }
	case CS_CANCELED :
	    ct_cmd_drop(cmd);
	    return NULL;

	case CS_END_RESULTS :
	    more_results= FALSE;
	    break;

	case CS_SUCCEED :
	    if(res_type == CS_ROW_RESULT) {
		/* bind and fetch these data */
		more_fetch= TRUE;
		parfmt.datatype= CS_INT_TYPE;
		parfmt.format= CS_FMT_UNUSED;
		parfmt.maxlength= 4;
		parfmt.scale= CS_SRC_VALUE;
		parfmt.precision= CS_SRC_VALUE;
		parfmt.count= 1;
		parfmt.locale= NULL;
		if(ct_bind(cmd, 1, &parfmt, &cit_id, NULL, NULL) != CS_SUCCEED) break;

		parfmt.datatype= CS_CHAR_TYPE;
		parfmt.format= CS_FMT_NULLTERM;
		parfmt.maxlength= 128;
		if(ct_bind(cmd, 2, &parfmt, cit_key, NULL, NULL) != CS_SUCCEED) break;

		while(more_fetch) {
		    switch(ct_fetch(cmd,CS_UNUSED,CS_UNUSED,CS_UNUSED,&rows_read)) {
		    case CS_SUCCEED :
			if(n == 0) {
			    refer= malloc(sizeof(TXC_CitList));
			    alloc_room= 1;
			}
			else if(n >= alloc_room) {
			    
			    alloc_room+= 2;
			    tmp= realloc(refer, alloc_room*sizeof(TXC_CitList));
			    if(tmp == NULL) {
				alloc_room= n--;
			    }
			    else refer= tmp;
			}

			refer[n].cit_id= cit_id;
			refer[n].cit_key= StringSave(cit_key);
			n++;
			continue;

		    case CS_ROW_FAIL :
			break;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;

		    default :
			/* this is an error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return refer;
		    }
		}
	    }
	    continue;
	default : break;
	}
    }
    ct_cmd_drop(cmd);

    *nof_cit= n;
    return refer;
}    


CharPtr txc_citGetKey(Int4 cit_id)
{
    CS_COMMAND *cmd;
    CS_DATAFMT parfmt;
    CS_INT res_type, rows_read;
    int nnn= 64;
    Boolean more_results= TRUE;
    Boolean more_fetch;
    char cit_key[132];

    if(cit_id <= 0) return NULL;
    
    if(ct_cmd_alloc(te_link, &cmd) != CS_SUCCEED) return NULL;


    /* build command: tax_citGet4Node @tax_id */

    if(ct_command(cmd, CS_RPC_CMD, "tax_getCitKey", CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }
    
    memset(&parfmt, 0, sizeof(CS_DATAFMT));
    strcpy(parfmt.name, "@cit_id");
    parfmt.namelen= CS_NULLTERM;
    parfmt.datatype= CS_INT_TYPE;
    parfmt.maxlength= sizeof(Int4);
    parfmt.status= CS_INPUTVALUE;
    parfmt.locale= NULL;
    if(ct_param(cmd, &parfmt, &cit_id, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }

    /* send this command to server */
    if(ct_send(cmd) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }
    
    while(more_results && (--nnn > 0)) {
	switch(ct_results(cmd, &res_type)) {
	case CS_FAIL :
	    if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
		ct_close(te_link, CS_FORCE_CLOSE);
	    }
	case CS_CANCELED :
	    ct_cmd_drop(cmd);
	    return NULL;

	case CS_END_RESULTS :
	    more_results= FALSE;
	    break;

	case CS_SUCCEED :
	    if(res_type == CS_ROW_RESULT) {
		/* bind and fetch these data */

		more_fetch= TRUE;
		parfmt.datatype= CS_CHAR_TYPE;
		parfmt.format= CS_FMT_NULLTERM;
		parfmt.maxlength= 128;
		parfmt.scale= CS_SRC_VALUE;
		parfmt.precision= CS_SRC_VALUE;
		parfmt.count= 1;
		parfmt.locale= NULL;
		if(ct_bind(cmd, 1, &parfmt, cit_key, NULL, NULL) != CS_SUCCEED) break;

		while(more_fetch) {
		    switch(ct_fetch(cmd,CS_UNUSED,CS_UNUSED,CS_UNUSED,&rows_read)) {
		    case CS_SUCCEED :
			cit_key[131]= '\0';
			continue;

		    case CS_ROW_FAIL :
			break;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;

		    default :
			/* this is an error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return StringSave(cit_key);
		    }
		}
	    }
	    continue;

	default : break;
	}
    }
    ct_cmd_drop(cmd);

    return StringSave(cit_key);
}    

TXC_CitListPtr txc_citFind(CharPtr key_str, Int4Ptr nof_cit)
{
    CS_COMMAND *cmd;
    CS_DATAFMT parfmt;
    CS_INT res_type, rows_read;
    int nnn= 64;
    Boolean more_results= TRUE;
    Boolean more_fetch;
    char cit_key[132];
    CS_INT cit_id;
    Int4 n= 0, alloc_room= 0;
    TXC_CitListPtr refer= NULL, tmp;

    if((key_str == NULL) || (*key_str == '\0')) return NULL;
    
    if(ct_cmd_alloc(te_link, &cmd) != CS_SUCCEED) return NULL;


    /* build command: tax_citGet4Node @tax_id */

    if(ct_command(cmd, CS_RPC_CMD, "tax_findCit", CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }
    
    memset(&parfmt, 0, sizeof(CS_DATAFMT));
    strcpy(parfmt.name, "@cit_key");
    parfmt.namelen= CS_NULLTERM;
    parfmt.datatype= CS_CHAR_TYPE;
    parfmt.maxlength= strlen(key_str);
    parfmt.status= CS_INPUTVALUE;
    parfmt.locale= NULL;
    if(ct_param(cmd, &parfmt, key_str, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }

    /* send this command to the server */
    if(ct_send(cmd) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }
    
    while(more_results && (--nnn > 0)) {
	switch(ct_results(cmd, &res_type)) {
	case CS_FAIL :
	    if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
		ct_close(te_link, CS_FORCE_CLOSE);
	    }
	case CS_CANCELED :
	    ct_cmd_drop(cmd);
	    return NULL;

	case CS_END_RESULTS :
	    more_results= FALSE;
	    break;

	case CS_SUCCEED :
	    if(res_type == CS_ROW_RESULT) {
		/* bind and fetch these data */
		more_fetch= TRUE;
		parfmt.datatype= CS_INT_TYPE;
		parfmt.format= CS_FMT_UNUSED;
		parfmt.maxlength= 4;
		parfmt.scale= CS_SRC_VALUE;
		parfmt.precision= CS_SRC_VALUE;
		parfmt.count= 1;
		parfmt.locale= NULL;
		if(ct_bind(cmd, 1, &parfmt, &cit_id, NULL, NULL) != CS_SUCCEED) break;

		parfmt.datatype= CS_CHAR_TYPE;
		parfmt.format= CS_FMT_NULLTERM;
		parfmt.maxlength= 128;
		if(ct_bind(cmd, 2, &parfmt, cit_key, NULL, NULL) != CS_SUCCEED) break;

		while(more_fetch) {
		    switch(ct_fetch(cmd,CS_UNUSED,CS_UNUSED,CS_UNUSED,&rows_read)) {
		    case CS_SUCCEED :
			if(n == 0) {
			    refer= malloc(sizeof(TXC_CitList));
			    alloc_room= 1;
			}
			else if(n >= alloc_room) {
			    
			    alloc_room+= 2;
			    tmp= realloc(refer, alloc_room*sizeof(TXC_CitList));
			    if(tmp == NULL) {
				alloc_room= n--;
			    }
			    else refer= tmp;
			}

			refer[n].cit_id= cit_id;
			refer[n].cit_key= StringSave(cit_key);
			n++;
			continue;

		    case CS_ROW_FAIL :
			break;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;

		    default :
			/* this is an error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return refer;
		    }
		}
	    }
	    continue;

	default : break;
	}
    }
    ct_cmd_drop(cmd);

    *nof_cit= n;
    return refer;
}    

_citation* txc_citGetByKey(CharPtr key_str)
{
    CS_COMMAND *cmd;
    CS_DATAFMT parfmt;
    CS_INT res_type, rows_read;
    int nnn= 64;
    Boolean more_results= TRUE;
    Boolean more_fetch;
    char cit_key[132], url[132], txt[6*256];
    CS_INT cit_key_ln, url_ln, txt_ln[6];
    CS_SMALLINT url_ind, txt_ind[6];
    CS_INT cit_id, flags, medline_id, pubmed_id;
    Int4 n, i;
    _citation* cit= NULL;

    if((key_str == NULL) || (*key_str == '\0')) return NULL;
    
    if(ct_cmd_alloc(te_link, &cmd) != CS_SUCCEED) return NULL;


    /* build command: tax_citGet4Node @tax_id */

    if(ct_command(cmd, CS_RPC_CMD, "tax_citGetByKey", CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }
    
    memset(&parfmt, 0, sizeof(CS_DATAFMT));
    strcpy(parfmt.name, "@cit_key");
    parfmt.namelen= CS_NULLTERM;
    parfmt.datatype= CS_CHAR_TYPE;
    parfmt.maxlength= strlen(key_str);
    parfmt.status= CS_INPUTVALUE;
    parfmt.locale= NULL;
    if(ct_param(cmd, &parfmt, key_str, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }

    /* send this command to the server */
    if(ct_send(cmd) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }
    
    while(more_results && (--nnn > 0)) {
	switch(ct_results(cmd, &res_type)) {
	case CS_FAIL :
	    if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
		ct_close(te_link, CS_FORCE_CLOSE);
	    }
	case CS_CANCELED :
	    ct_cmd_drop(cmd);
	    return NULL;

	case CS_END_RESULTS :
	    more_results= FALSE;
	    break;

	case CS_SUCCEED :
	    if(res_type == CS_ROW_RESULT) {
		/* bind and fetch these data */
		more_fetch= TRUE;
		parfmt.datatype= CS_INT_TYPE;
		parfmt.format= CS_FMT_UNUSED;
		parfmt.maxlength= 4;
		parfmt.scale= CS_SRC_VALUE;
		parfmt.precision= CS_SRC_VALUE;
		parfmt.count= 1;
		parfmt.locale= NULL;
		if((ct_bind(cmd, 1, &parfmt, &cit_id, NULL, NULL) != CS_SUCCEED) ||
		   (ct_bind(cmd, 3, &parfmt, &flags, NULL, NULL) != CS_SUCCEED)  ||
		   (ct_bind(cmd, 4, &parfmt, &medline_id, NULL, NULL) != CS_SUCCEED) ||
		   (ct_bind(cmd, 5, &parfmt, &pubmed_id, NULL, NULL) != CS_SUCCEED)) break;

		parfmt.datatype= CS_CHAR_TYPE;
		parfmt.format= CS_FMT_NULLTERM;
		parfmt.maxlength= 128;
		if((ct_bind(cmd, 2, &parfmt, cit_key, &cit_key_ln, NULL) != CS_SUCCEED) ||
		   (ct_bind(cmd, 6, &parfmt, url, &url_ln, &url_ind) != CS_SUCCEED)) break;

		parfmt.maxlength= 256;
		for(n= 0; n < 6; n++) {
		    if(ct_bind(cmd, 7+n, &parfmt, txt + 256*n, &txt_ln[n], &txt_ind[n]) != CS_SUCCEED)
			break;
		}

		while(more_fetch) {
		    switch(ct_fetch(cmd,CS_UNUSED,CS_UNUSED,CS_UNUSED,&rows_read)) {
		    case CS_SUCCEED :
			if(cit == NULL) {
			    cit= malloc(sizeof(_citation));
			}
			cit->id= cit_id;
			cit->key= StringSave(cit_key);
			cit->flags= flags;
			cit->mu_id= medline_id;
			cit->pm_id= pubmed_id;
			cit->url= ((url_ind == -1) || (url_ln < 1) || (url[0] == '\0'))? NULL :
			    StringSave(url);
			
			for(i= n= 0; i < 6; i++) {
			    n+= ((txt_ind[i] == -1) || (txt_ln[i] < 1) || (txt[256*i] == '\0'))? 0 : 
				(txt_ln[i] + 1);
			}
			if(n == 0) {
			    cit->txt= NULL;
			}
			else {
			    cit->txt= MemNew(n);
			    cit->txt[0]= '\0';
			    for(i= 0; i < 6; i++) {
				if((txt_ind[i] != -1) && (txt_ln[i] > 0) && (txt[256*i] != '\0')) {
				    strcat(cit->txt, &txt[256*i]);
				}
			    }
			}
			continue;

		    case CS_ROW_FAIL :
			break;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;

		    default :
			/* this is an error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return cit;
		    }
		}
	    }
	    continue;

	default : break;
	}
    }
    ct_cmd_drop(cmd);

    return cit;
}    

void txc_close()
{
    int i= 0;
    CS_RETCODE rc;

    while((rc= ct_close(te_link, CS_UNUSED)) == CS_FAIL) {
	++i;
	if(ct_cancel(te_link, NULL, CS_CANCEL_ALL) != CS_SUCCEED) break;
	if(i > 2) break;
    }
    if(rc == CS_FAIL) ct_close(te_link, CS_FORCE_CLOSE);

    if(ct_exit(te_context, CS_UNUSED) != CS_FAIL) {
	ct_exit(te_context, CS_FORCE_EXIT);
    }
}


/********************************/
/* retrive subspecies functions */
/********************************/

/* tax_SSget(@tax_id int, @subtype tinyint, @subname varchar(120)) proc */

_subspecPtr tax_SSget(Int4 tax_id, _subspecPtr ssrec)
{
    CS_COMMAND *cmd;
    CS_DATAFMT parfmt;
    char rname[132];
    CS_INT res_type, rows_read;
    CS_INT rname_ln;
    int nnn= 64;
    Boolean more_results= TRUE;
    Boolean more_fetch;

    if(tax_id < 2) return NULL;

    if((ssrec == NULL) || (ssrec->sname == NULL)) return NULL;

    if(ct_cmd_alloc(te_link, &cmd) != CS_SUCCEED) return NULL;

    /* build command: tax_checkUnique @name_txt, @tax_id */

    if(ct_command(cmd, CS_RPC_CMD, "tax_SSget", CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }
        
    memset(&parfmt, 0, sizeof(CS_DATAFMT));
    strcpy(parfmt.name, "@tax_id");
    parfmt.namelen= CS_NULLTERM;
    parfmt.status= CS_INPUTVALUE;
    parfmt.locale= NULL;
    parfmt.datatype= CS_INT_TYPE;
    parfmt.maxlength= sizeof(CS_INT);
    if(ct_param(cmd, &parfmt, &tax_id, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }

    strcpy(parfmt.name, "@subtype");
    parfmt.datatype= CS_TINYINT_TYPE;
    parfmt.maxlength= 1;
    if(ct_param(cmd, &parfmt, &ssrec->stype, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }

    strcpy(parfmt.name, "@subname");
    strncpy(rname, ssrec->sname, 116);
    rname[116]= '\0';
    parfmt.datatype= CS_CHAR_TYPE;
    parfmt.maxlength= strlen(rname);
    if(ct_param(cmd, &parfmt, rname, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }

    /* send this command to server */
    if(ct_send(cmd) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }

    if(ssrec->rname != NULL) ssrec->rname= MemFree(ssrec->rname);
        
    while(more_results && (--nnn > 0)) {
	switch(ct_results(cmd, &res_type)) {
	case CS_FAIL :
	    if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
		ct_close(te_link, CS_FORCE_CLOSE);
	    }
	case CS_CANCELED :
	    ct_cmd_drop(cmd);
	    return NULL;

	case CS_END_RESULTS :
	    more_results= FALSE;
	    break;

	case CS_SUCCEED :
	    if(res_type == CS_ROW_RESULT) {
		/* bind and fetch these data */
		more_fetch= TRUE;
		parfmt.datatype= CS_INT_TYPE;
		parfmt.format= CS_FMT_UNUSED;
		parfmt.maxlength= 4;
		parfmt.scale= CS_SRC_VALUE;
		parfmt.precision= CS_SRC_VALUE;
		parfmt.count= 1;
		parfmt.locale= NULL;
		if(ct_bind(cmd, 1, &parfmt, &ssrec->r_id, NULL, NULL) != CS_SUCCEED) break;

		parfmt.datatype= CS_TINYINT_TYPE;
		parfmt.maxlength= 1;
		if(ct_bind(cmd, 2, &parfmt, &ssrec->rtype, NULL, NULL) != CS_SUCCEED) break;
		

		parfmt.datatype= CS_CHAR_TYPE;
		parfmt.format= CS_FMT_NULLTERM;
		parfmt.maxlength= 132;
		if(ct_bind(cmd, 3, &parfmt, rname, &rname_ln, NULL) != CS_SUCCEED) break;

		while(more_fetch) {
		    switch(ct_fetch(cmd,CS_UNUSED,CS_UNUSED,CS_UNUSED,&rows_read)) {
		    case CS_SUCCEED :
			if(rname_ln >= 0) {
			    rname[rname_ln]= '\0';
			    if(ssrec->rname != NULL) MemFree(ssrec->rname);
			    ssrec->rname= StringSave(rname);
			}
			continue;

		    case CS_ROW_FAIL :
			continue;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;

		    default :
			/* this is an error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return NULL;
		    }
		}
	    }
	    continue;

	default : break;
	}
    }
    ct_cmd_drop(cmd);

    return (ssrec->rname == NULL)? NULL : ssrec;   
}

tax_OrgModPtr tax_SSgetLegal(Int4 tax_id)
{
    CS_COMMAND *cmd;
    CS_DATAFMT parfmt;
    CS_INT res_type, rows_read;
    CS_TINYINT subtype;
    int nnn= 64;
    Boolean more_results= TRUE;
    Boolean more_fetch;
    char subname[256];
    CS_INT subname_ln;
    tax_OrgModPtr first= NULL;
    tax_OrgModPtr last, tmp;

    if(tax_id < 2) return NULL;
    
    if(ct_cmd_alloc(te_link, &cmd) != CS_SUCCEED) return NULL;


    /* call  tax_SSgetLegal (@tax_id int) proc */

    if(ct_command(cmd, CS_RPC_CMD, "tax_SSgetLegal", CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }
    
    memset(&parfmt, 0, sizeof(CS_DATAFMT));
    strcpy(parfmt.name, "@tax_id");
    parfmt.namelen= CS_NULLTERM;
    parfmt.datatype= CS_INT_TYPE;
    parfmt.maxlength= sizeof(Int4);
    parfmt.status= CS_INPUTVALUE;
    parfmt.locale= NULL;
    if(ct_param(cmd, &parfmt, &tax_id, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }

    /* send this command to the server */
    if(ct_send(cmd) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return NULL;
    }
    
    while(more_results && (--nnn > 0)) {
	switch(ct_results(cmd, &res_type)) {
	case CS_FAIL :
	    if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
		ct_close(te_link, CS_FORCE_CLOSE);
	    }
	case CS_CANCELED :
	    ct_cmd_drop(cmd);
	    return first;

	case CS_END_RESULTS :
	    more_results= FALSE;
	    break;

	case CS_SUCCEED :
	    if(res_type == CS_ROW_RESULT) {
		/* bind and fetch these data */
		more_fetch= TRUE;
		parfmt.datatype= CS_TINYINT_TYPE;
		parfmt.format= CS_FMT_UNUSED;
		parfmt.maxlength= 1;
		parfmt.scale= CS_SRC_VALUE;
		parfmt.precision= CS_SRC_VALUE;
		parfmt.count= 1;
		parfmt.locale= NULL;
		if(ct_bind(cmd, 1, &parfmt, &subtype, NULL, NULL) != CS_SUCCEED) break;


		parfmt.datatype= CS_CHAR_TYPE;
		parfmt.format= CS_FMT_NULLTERM;
		parfmt.maxlength= 240;
		if(ct_bind(cmd, 2, &parfmt, subname, &subname_ln, NULL) != CS_SUCCEED) break;

		while(more_fetch) {
		    switch(ct_fetch(cmd,CS_UNUSED,CS_UNUSED,CS_UNUSED,&rows_read)) {
		    case CS_SUCCEED :
			if(subname_ln >= 0) subname[subname_ln]= '\0';
			tmp= MemNew(sizeof(tax_OrgMod));
			if(tmp == NULL) continue;
			tmp->next= NULL;
			tmp->subname= StringSave(subname);
			tmp->subtype= subtype;
			if(first == NULL) {
			    first= last= tmp;
			}
			else {
			    last->next= tmp;
			    last= tmp;
			}
			
			continue;

		    case CS_ROW_FAIL :
			continue;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;

		    default :
			/* this is an error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return first;
		    }
		}
	    }
	    continue;

	default : break;
	}
    }
    ct_cmd_drop(cmd);

    return first;
}    

/* tax_SSgetNodes(@tax_id int, @subtype tinyint, @subname varchar(120)) proc */

Int4 tax_SSgetNodes(Uint1 stype, CharPtr sname, Uint1 mode, Int4Ptr* ids)
{
    CS_COMMAND *cmd;
    CS_DATAFMT parfmt;
    CS_INT res_type, rows_read;
    int nnn= 64;
    Boolean more_results= TRUE;
    Boolean more_fetch;
    Int4Ptr p_id;
    Int4 nof_nodes= 0, nof_rooms;
    CS_INT tax_id;
    char tname[120];

    if((sname == NULL) || (*sname == '\0')) return 0;

    if(ct_cmd_alloc(te_link, &cmd) != CS_SUCCEED) return 0;

    /* build command: tax_SSgetNodes @subtype, @subname, @mode */

    if(ct_command(cmd, CS_RPC_CMD, "tax_SSgetNodes", CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }
        
    memset(&parfmt, 0, sizeof(CS_DATAFMT));
    strcpy(parfmt.name, "@subtype");
    parfmt.namelen= CS_NULLTERM;
    parfmt.status= CS_INPUTVALUE;
    parfmt.locale= NULL;
    parfmt.datatype= CS_TINYINT_TYPE;
    parfmt.maxlength= sizeof(CS_TINYINT);
    if(ct_param(cmd, &parfmt, &stype, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }

    strcpy(parfmt.name, "@subname");
    parfmt.datatype= CS_CHAR_TYPE;
    strncpy(tname, sname, 116);
    tname[116]= '\0';
    parfmt.maxlength= strlen(tname);
    if(ct_param(cmd, &parfmt, tname, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }

    strcpy(parfmt.name, "@mode");
    parfmt.datatype= CS_TINYINT_TYPE;
    parfmt.maxlength= 1;
    if(ct_param(cmd, &parfmt, &mode, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }

    /* send this command to server */
    if(ct_send(cmd) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }

    while(more_results && (--nnn > 0)) {
	switch(ct_results(cmd, &res_type)) {
	case CS_FAIL :
	    if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
		ct_close(te_link, CS_FORCE_CLOSE);
	    }
	case CS_CANCELED :
	    ct_cmd_drop(cmd);
	    return 0;

	case CS_END_RESULTS :
	    more_results= FALSE;
	    break;

	case CS_SUCCEED :
	    if(res_type == CS_ROW_RESULT) {
		/* bind and fetch these data */
		more_fetch= TRUE;
		parfmt.datatype= CS_INT_TYPE;
		parfmt.format= CS_FMT_UNUSED;
		parfmt.maxlength= 4;
		parfmt.scale= CS_SRC_VALUE;
		parfmt.precision= CS_SRC_VALUE;
		parfmt.count= 1;
		parfmt.locale= NULL;
		if(ct_bind(cmd, 1, &parfmt, &tax_id, NULL, NULL) != CS_SUCCEED) break;
		p_id= MemNew(4 * sizeof(Int4));
		if(p_id != NULL) {
		    nof_rooms= 4;
		}
		else break;

		while(more_fetch) {
		    switch(ct_fetch(cmd,CS_UNUSED,CS_UNUSED,CS_UNUSED,&rows_read)) {
		    case CS_SUCCEED :
			if(nof_nodes >= nof_rooms) {
			    nof_rooms+= 8;
			    *ids= p_id;
			    p_id= MemMore(p_id, nof_rooms*(sizeof(Int4)));
			    if(p_id == NULL) {
				p_id= *ids;
				nof_rooms-= 8;
				continue;
			    }
			}
			
			p_id[nof_nodes++]= tax_id;
			continue;

		    case CS_ROW_FAIL :
			continue;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;

		    default :
			/* this is an error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return 0;
		    }
		}
	    }
	    continue;

	default : break;
	}
    }
    ct_cmd_drop(cmd);

    *ids= p_id;
    return nof_nodes;   
}

Int4 tax_getTaxId4GI(Int4 gi)
{
    CS_COMMAND *cmd;
    CS_DATAFMT parfmt;
    CS_INT res_type, rows_read, tax_id= 0;
    Boolean more_results= TRUE;
    Boolean more_fetch;
    int nnn= 64;

    if(ct_cmd_alloc(te_link, &cmd) != CS_SUCCEED) return 0;

    /* build command: tax_getTaxId4GI @gi */

    if(ct_command(cmd, CS_RPC_CMD, "tax_getTaxId4GI", CS_NULLTERM, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }
        
    memset(&parfmt, 0, sizeof(CS_DATAFMT));
    strcpy(parfmt.name, "@gi");
    parfmt.namelen= CS_NULLTERM;
    parfmt.datatype= CS_INT_TYPE;
    parfmt.maxlength= sizeof(Int4);
    parfmt.status= CS_INPUTVALUE;
    parfmt.locale= NULL;
    if(ct_param(cmd, &parfmt, &gi, parfmt.maxlength, CS_UNUSED) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }

    /* send this command to server */
    if(ct_send(cmd) != CS_SUCCEED) {
	ct_cmd_drop(cmd);
	return 0;
    }
        
    while(more_results && (--nnn > 0)) {
	switch(ct_results(cmd, &res_type)) {
	case CS_FAIL :
	    if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
		ct_close(te_link, CS_FORCE_CLOSE);
	    }
	case CS_CANCELED :
	    ct_cmd_drop(cmd);
	    return 0;

	case CS_END_RESULTS :
	    more_results= FALSE;
	    break;

	case CS_SUCCEED :
	    if(res_type == CS_STATUS_RESULT) {
		more_fetch= TRUE;
		parfmt.datatype= CS_INT_TYPE;
		parfmt.format= CS_FMT_UNUSED;
		parfmt.maxlength= 4;
		parfmt.scale= CS_SRC_VALUE;
		parfmt.precision= CS_SRC_VALUE;
		parfmt.count= 1;
		if(ct_bind(cmd, 1, &parfmt, &tax_id, NULL, NULL) != CS_SUCCEED) break;
		while(more_fetch) {
		    switch(ct_fetch(cmd,CS_UNUSED,CS_UNUSED,CS_UNUSED,&rows_read)) {
		    case CS_SUCCEED :
			continue;

		    case CS_ROW_FAIL :
			break;

		    case CS_END_DATA :
			more_fetch= FALSE;
			continue;

		    default :
			/* this is a error */
			more_fetch= FALSE;
			if(ct_cancel(NULL, cmd, CS_CANCEL_ALL) != CS_SUCCEED) {
			    ct_close(te_link, CS_FORCE_CLOSE);
			}
			ct_cmd_drop(cmd);
			return 0;
		    }
		}
	    }
	    continue;

	default : break;
	}
    }
    ct_cmd_drop(cmd);
    return tax_id;		
}

Int4 txc_findByOrg(OrgRefPtr inp_orgRef, OrgModPtr* hitName)
{
    if(hitName) *hitName= NULL;
    return tax1_getTaxIdByOrgRef(inp_orgRef);
}
