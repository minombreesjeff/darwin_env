/*   $Id: PubStructAsn.c,v 6.36 2004/04/01 13:43:05 lavr Exp $
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
 * Author:  Michael Kimelman
 *
 * File Description: PubStruct DB Asn (down)loader.
 *                   
 * Modifications:  
 * --------------------------------------------------------------------------
 * $Log: PubStructAsn.c,v $
 * Revision 6.36  2004/04/01 13:43:05  lavr
 * Spell "occurred", "occurrence", and "occurring"
 *
 * Revision 6.35  2001/05/29 17:15:12  kimelman
 * bugfix
 *
 * Revision 6.34  2001/05/25 01:42:10  kimelman
 * mmdb2livemmdb
 *
 * Revision 6.33  2000/12/29 17:26:28  kimelman
 * PDQBACH -> BACH
 *
 * Revision 6.32  2000/10/18 18:45:18  kimelman
 * bugfix
 *
 * Revision 6.31  2000/08/08 20:03:49  kimelman
 * BACH10 -> PDQBACH
 *
 * Revision 6.30  1999/12/01 23:25:35  kimelman
 * cleanup handle on no data
 *
 * Revision 6.29  1999/11/29 23:29:47  kimelman
 * bugfix: pending & eos processign at db_close on permanent connection
 *
 * Revision 6.28  1999/11/09 17:44:35  kimelman
 * recovery added for DB timeout promlems
 *
 * Revision 6.27  1999/08/02 21:53:12  kimelman
 * on fail postprocessing
 *
 * Revision 6.26  1999/08/02 19:50:50  kimelman
 * keep connection alive foor mmdbsrv & vastsrv sessions
 *
 * Revision 6.25  1999/06/29 19:03:41  kimelman
 * cleanup - prending results cancellation
 *
 * Revision 6.24  1999/06/15 01:32:16  kimelman
 * bugfix: Int4 casting : 2nd pass :)
 *
 * Revision 6.23  1999/06/14 18:16:39  kimelman
 * Int4 to CS_INT castings
 *
 * Revision 6.22  1999/06/11 21:56:36  kimelman
 * get rid of one more stupid message
 *
 * Revision 6.21  1999/06/11 17:56:31  kimelman
 * fixed conneciton close. no cancel if no idle command
 *
 * Revision 6.20  1999/05/25 16:20:40  kimelman
 * brakets misplacing fixed
 *
 * Revision 6.19  1999/04/28 18:23:42  kimelman
 * bugfix: reinit dbserver in case of connection failure & retry
 *
 * Revision 6.18  1998/11/19 23:53:38  kimelman
 * ct_cancel problem workaround
 *
 * Revision 6.17  1998/11/06 18:59:05  kimelman
 * PubStruct loading transaction granularity changed
 *
 * Revision 6.16  1998/10/22 15:23:21  kimelman
 * Parallel loading fixes: common resourse access extracted to tiny independent
 * transaction.
 *
 * Revision 6.15  1998/10/15  16:04:48  kimelman
 * switch to public ctutils library
 *
 * Revision 6.14  1998/09/03 21:30:11  kimelman
 * added number of retries to connect to DB server
 * added softer processing to server connection failure
 * added transmission timeouts.
 *
 * Revision 6.13  1998/08/08 04:52:26  kimelman
 * bugfixes:
 * 1. enforced loading mode.
 * 2. memory  leaks
 * 3. 'pos' value in "check on load" processing
 *
 * Revision 6.12  1998/08/05 21:12:33  kimelman
 * skip/load bugfix
 *
 * Revision 6.11  1998/07/16 20:02:51  kimelman
 * enforce option added
 *
 * Revision 6.10  1998/07/14 20:24:45  kimelman
 * FT schema & smart load
 *
 * Revision 6.9  1998/06/25 19:05:49  kimelman
 * move context statics from ctlibutils to PubStructAsn
 *
 * Revision 6.8  1998/06/17 15:33:52  kimelman
 * added commit transaction at the end of removeasn.
 *
 * Revision 6.7  1998/05/28 17:33:39  kimelman
 * throw away obsolete code
 *
 * Revision 6.6  1998/05/27 18:09:16  kimelman
 * put compression stuff into production
 *
 * Revision 6.5  1998/05/15 20:20:01  kimelman
 * compr -> nlmzip
 *
 * Revision 6.4  1998/05/14 16:11:12  kimelman
 * Compression stuff added in debug mode.
 * few bugs fixed, related to OpenServer/SQL Server switching
 *
 * Revision 6.3  1998/05/08 03:03:41  kimelman
 * Open Server fix
 *
 * Revision 6.2  1998/04/15 14:53:54  kimelman
 * 1. Make retrieval unifirm from open server and sql server.
 * 2. mmdbsrv retrival performance tuning:
 * 	- size of read-in buffers
 * 	- direct pdb2mmdb DB lookup instead of full loading pdb to mmdb translataion
 * 	  table
 * 3. cleaning mmdblocl.* and make.mmdbsrv in order to remove obsolete code
 *
 * Revision 6.1  1998/04/03 20:25:15  kimelman
 * PubStruct access code added to mmdbsrv
 *
 *
 * ==========================================================================
 */

#ifdef DEBUG_MODE
# define CT_DEBUG_MODE
#endif

#include <PubStructAsn.h>
#include <ctlibutils.h>
#include <mmdbapi.h>
#include <nlmzip.h>

#include <assert.h>

#define DEF_SRV "BACH:PubStruct=anyone,allowed"

typedef enum {
  PS_NEW,
  PS_READ,
  PS_UPDATE,
  PS_STORE,
  PS_DELETE
} ps_action_t;

struct ps_chunk {
  struct ps_chunk  *next;
  int                 len;
  int                 size;
  int                 start;
  char               *data;
};

typedef struct pubstruct_t {
  CTLibUtils       clu;
  CS_IODESC        iodesc;
  struct ps_chunk *top;
  struct ps_chunk *bottom;
  ps_action_t      action;
  int              close_disabled;
  char            *srv;
  int              acc;
  int              eos;
  int              pending;
  int              cache_size;
  int              open_server;
} DB_stream_t ;

/****************************************************************************/

static struct ps_chunk *
new_piece(int min_size, int max_size)
{
  struct ps_chunk *piece;
  piece = MemNew(sizeof(*piece));
  assert(piece);
  piece->start = 0;
  piece->len = 0;
  piece->next = NULL;
  piece->size = max_size;
  while ((piece->data = MemNew(piece->size)) == NULL)
    {
      if (piece->size == min_size)
        {
          MemFree(piece);
          ErrPostEx(SEV_ERROR, ERR_SYBASE,0,"\n%s:%d: memory exhausted '%d' ",
                    __FILE__,__LINE__,min_size);
          return NULL;
        }
      else
        {
          piece->size /= 2;
          if (piece->size < min_size)
            piece->size = min_size;
        }
    } 
  return piece;
}

/*****************************************************************************
*   db handler interface  
*****************************************************************************/
static void
pubstruct_db_close(DB_stream_t *db)
{
  CS_INT     restype;
  CS_RETCODE retcode;
  
  if (!db)
    return; /*???? kind of assert */
  if (db->clu.ctcmd && db->action == PS_READ && db->pending)
    {
      do
        {
          CTRUN1( ct_cancel(NULL,db->clu.ctcmd, CS_CANCEL_CURRENT),0);
          CTRUN1( ct_results(db->clu.ctcmd,&restype),0);
          CTlib_TYPERES(restype);
        }
      while ( retcode == CS_SUCCEED );
      db->pending=0;
      db->eos=0;
    }
  if(db->close_disabled)
    return;

 errexit:
  db->clu.context = NULL; /* avoid cleaning context */
  CTLibDrop(&db->clu);
  while (db->top)
    {
      struct ps_chunk *piece = db->top ;
      db->top = db->top->next;
      MemFree(piece->data);
      MemFree(piece);
    }
  if (db->srv)
    MemFree(db->srv);
  MemFree(db);
  ErrShow();
}

static DB_stream_t *
pubstruct_db_open(char *server,ps_action_t action)
{
  static int               done    = 0   ;
  static CS_CONTEXT PNTR   context = NULL;
         DB_stream_t      *db ;
         int               retries = 0   ;
  
  if (!done)
    {
      if (getenv("DB_DEBUG"))
        {
          ErrSetLogLevel(0);
          ErrSetOptFlags(EO_LOGTO_STDERR);
        }
      done = 1;
    }
  
  assert ( action == PS_READ || action == PS_NEW || action == PS_UPDATE);
  
  db = MemNew(sizeof(*db));
  db->top = db->bottom = NULL;
  if (server == NULL)
    server = DEF_SRV;
  db->pending=0;
  db->eos=0;
  db->srv = MemNew(strlen(server)+1);
  {
    char *os = strstr(server,"_OS");
    if (os)
      {
        db->open_server = 1;
        db->clu.used4os = TRUE;
      }
  }
  db->clu.context = context;
  db->action = action;

  while(retries++<20)
    {
      strcpy(db->srv,server);
      if(!CTLibInit(&db->clu,db->srv,NULL,NULL,NULL,0,NULL))
        {
          sleep(10);
          continue;
        }
      if(context==NULL)
        {
          CS_INT to = 60;
          context=db->clu.context;
          ct_config(context,CS_SET,CS_TIMEOUT,(CS_VOID*)&to,CS_UNUSED,NULL);
        }
      return db;
    }
  pubstruct_db_close(db);
  db = NULL;
  ErrPostEx(SEV_ERROR,  ERR_SYBASE, 0,"Connection to %s - failed",server);
  ErrShow();
  return db;
}

/*
 *  DB Asn callbacks
 */

static Int4 LIBCALLBACK
dbio_read(Pointer ptr, CharPtr obuf, Int4 count)
{
  DB_stream_t     *db = (DB_stream_t*)ptr;
  CS_COMMAND PNTR  cmd = db->clu.ctcmd;
  CS_INT           bytes = 0;
  CS_RETCODE       retcode;

  assert(db->action == PS_READ);
  if (db->eos)
    {
      ErrPostEx(SEV_INFO,0,0,"Attempt to read after EOS");
      return 0;
    }
  CTRUN_POST (ct_get_data(cmd, 1,(CS_TEXT*)obuf,(CS_INT)count, &bytes));
  CTRUN_TYPECODE (retcode,2); /* print result code in debug mode */
  if (bytes < count )
    db->eos = 1 ;
  return bytes;
}

static Int4 LIBCALLBACK
dbio_write(Pointer ptr, CharPtr buf, Int4 count)
{
  DB_stream_t     *db    = (DB_stream_t*)ptr;
  CS_COMMAND PNTR  cmd   = db->clu.ctcmd;
  CS_RETCODE       retcode;
  struct ps_chunk *piece;
  int              desired_size = 100 * 1024;
  
  assert(db->action != PS_READ);
  assert(count >= 0);
  if(count==0)
    return 0;
  /*
   * because sybase required to say it the full length of data before
   * providing it - we can only collect data in memory until we got a
   * 'close' command. At that moment we will be able to calculate and store a whole
   * bunch of data -  a bit stupid activity - but...
   */
  if (db->bottom)
    piece = db->bottom;
  else
    db->bottom = db->top = piece = new_piece(count, desired_size);
  if (piece->size - piece->len < count)
    db->bottom = db->bottom->next = piece = new_piece(count, desired_size);
  assert (piece);
  assert (piece->size - piece->len >= count );
  memcpy (piece->data + piece->len,buf,count);
  piece->len += count;
  db->iodesc.total_txtlen += count;
  
  return count;
}

#define dbio_close pubstruct_closeasn

#define dbio_open(db) fci_open(db,(db->action==PS_READ?dbio_read:dbio_write),dbio_close)

/*****************************************************************************
*
* parse asn blob = extract mmdb_id, pdb_id, create_date & pubstruct_parseasn(char *postupdate_cmd, Int4 acc, char* buf, int buflen)
*****************************************************************************/

static int
pubstruct_parseasn(char *postupdate_cmd, Int4 acc, char* buf, int buflen)
{
  static AsnTypePtr atp, biostruc,mmdb_tp,bs;
  static AsnIoPtr aip;
  static AsnIoMemPtr aimp ;
  static AsnModulePtr amp = NULL;
  Int4  mmdb_id = 0;
  BiostrucSourcePtr bsp = NULL;
  
  if (amp == NULL)
    {
      /* initialization */
      
      if (! (objmmdb1AsnLoad() &&
             objmmdb2AsnLoad() &&
             objmmdb3AsnLoad()))   /* load Biostruc defintions */
        {
          ErrShow();
          return 0;
        }
      
      amp = AsnAllModPtr();
      biostruc = AsnFind("Biostruc");
      bs       = AsnFind("Biostruc-history.data-source");
      mmdb_tp  = AsnFind("Biostruc.id.E.mmdb-id");
      assert(bs);
    }
  
  aimp = AsnIoMemOpen("rb", (UcharPtr)buf, buflen);
  aip = aimp->aip;
  atp = biostruc;

  while (1)
    {
      atp = AsnReadId(aip, amp, atp);
      if (atp == bs)
        {
          assert (bsp == NULL);
          assert (mmdb_id != 0 );
          bsp = BiostrucSourceAsnRead(aip, atp);
          break;
        }
      else if (atp == mmdb_tp)
        {
          DataVal dv;
          AsnReadVal(aip, atp, &dv);
          if (mmdb_id == 0)
            mmdb_id = dv.intvalue;
          else
            if ( mmdb_id != dv.intvalue )
              {
                /* seems to be in replacement branch */
                ErrPostEx(SEV_INFO, 0, 0," %d replaced %d",
                          mmdb_id,dv.intvalue);
              }
            else
              ErrPostEx(SEV_ERROR, 0, 0,"PubStruct : second occurrence of mmdb_id ");
          if(postupdate_cmd==NULL)
            goto exit;
        }
      else if (atp == NULL)
        goto err;
      else
        AsnReadVal(aip, atp, NULL);  /* skip it */
    }
  assert (bsp != NULL);
  assert (mmdb_id != 0 );

  sprintf(postupdate_cmd,"exec new_struct1 %d,%d,",acc,mmdb_id);

  {  /*date*/
    DatePtr    d  = bsp->database_entry_date;
    ValNodePtr vn = bsp->VersionOfDatabase_version_of_database;

    if (vn->choice == VersionOfDatabase_version_of_database_release_date)
        d = (DatePtr)(vn->data.ptrvalue);
    
    if (d->data[0]==0)
      sprintf(postupdate_cmd+strlen(postupdate_cmd),"\'%s\',",d->str);
    else
      sprintf(postupdate_cmd+strlen(postupdate_cmd),"\'%d/%d/%d\',",
              (int)d->data[2],(int)d->data[3],1900+(int)d->data[1]);
  }
  {  /*pdb*/
    ValNodePtr vn =  bsp->database_entry_id;
    DbtagPtr   dbtag = (DbtagPtr)(vn->data.ptrvalue);
    assert(vn->choice == BiostrucId_other_database);
    assert( strcmp(dbtag->db,"PDB")==0);
    sprintf(postupdate_cmd+strlen(postupdate_cmd),"\'%s\'",dbtag->tag->str);
  }
#ifdef DEBUG_MODE
  ErrPostEx(SEV_INFO, 0, 0,"PubStruct : cmd generated \"%s\"",
            postupdate_cmd);
#endif
exit:
  BiostrucSourceFree(bsp);
  AsnIoMemClose(aimp);
  return mmdb_id;
err:
  ErrPostEx(SEV_FATAL, CTX_NCBIASN1, 81,
            "PubStruct : asn data error");
  return 0;
}

/*****************************************************************************
 *
 ****************************************************************************/
static int
pubstruct_openblob (DB_stream_t *db, int mmdb_id)
{
  CS_COMMAND PNTR cmd;
  CS_INT          count,restype;
  Int4            status;
  CS_RETCODE      retcode;

  char            buf[1024];

  if (db->acc)
    {
      assert(db->acc> 0);
      if (db->action == PS_READ)
        sprintf(buf,"exec id_get_asn 0,%d,10,0,0 ",(int)db->acc);
      else
        {
          assert (!db->open_server);
          sprintf(buf,"select blob from Struct where acc = %d",(int)db->acc);
        }
    }
  else
    {
      assert (mmdb_id > 0);
      assert (db->action == PS_READ);
      sprintf(buf,"exec id_get_asn %d,0,10,0,0 ",mmdb_id);
    }
  cmd = db->clu.ctcmd;
#ifdef DEBUG_MODE
  ErrPostEx(SEV_INFO,  ERR_SYBASE, 0,"pubstruct_openblob: execute(%s)",buf);
#endif
  CTRUN ( ct_command(cmd,CS_LANG_CMD,(Pointer)buf,CS_NULLTERM,CS_UNUSED) );
  CTRUN ( ct_send(cmd) );
  CTRUN ( ct_results(cmd,&restype));
  CTlib_TYPERES(restype);
  db->pending=1;

  /* skip 'exec' status line */
  if (db->action == PS_READ)
    {
      count = 0;
      while(1)
        {
          if (restype == CS_ROW_RESULT || restype == CS_PARAM_RESULT)
            {
              count ++;
              switch (count)
                {
                case 1: /* get_asnprop */ break ;
                case 2: /* asn */
                  goto loopexit;
                }
            }
          if (restype == CS_STATUS_RESULT) {
            int da = db->close_disabled;
            db->close_disabled = 1;
            pubstruct_db_close(db);
            db->close_disabled = da;
            return -1; /* no data */
          }
          CTRUN1( ct_cancel(NULL,db->clu.ctcmd, CS_CANCEL_CURRENT),0);
          CTRUN1 ( ct_results(cmd,&restype),0);
          CTlib_TYPERES(restype);
        }
    }
loopexit:

  /* we are ready to read the blob */
  assert(restype == CS_ROW_RESULT || restype == CS_PARAM_RESULT);
  CTRUN ( ct_fetch(cmd,CS_UNUSED,CS_UNUSED,CS_UNUSED,&count)) ;

  CTRUN_POST (ct_get_data(cmd,1,buf,(CS_INT)0,NULL));
  if(!CTRUN_TYPECODE (retcode,0))
    goto errexit;

  CTRUN ( ct_data_info(cmd,CS_GET,1,&db->iodesc) );

  if ( db->action != PS_READ ) /* write case */
    {
      do
        {
          CTRUN1( ct_cancel(NULL,db->clu.ctcmd, CS_CANCEL_CURRENT),0);
          CTRUN1( ct_results(db->clu.ctcmd,&restype),0);
          CTlib_TYPERES(restype);
        }
      while ( retcode == CS_SUCCEED );
    }
  return 1;
 errexit:
  pubstruct_db_close(db);
  return 0;
}

static Int4 LIBCALLBACK pubstruct_closeasn(Pointer ptr,int commit);

/*
 * comress blob - stored in db->top...db->bottom chain
 */

static int
pack_blob(DB_stream_t    *db)
{
  struct ps_chunk *top; 
  fci_t            compr;
  int              cache_size       = 1024;
  Int4             expectation;
  struct ps_chunk *piece;
          
  top = db->top;
  db->top = db->bottom = NULL;
  expectation=db->iodesc.total_txtlen;
  db->iodesc.total_txtlen = 0;
          
  compr = compressor_open(dbio_open(db),30*1024,0);
          
  while (top)
    {
      Int4   len;
      Int4   len1;
      piece = top ;
      expectation -= piece->len;
      if ( expectation<0 )
        goto errexit;
              
      for (piece->start = 0; piece->start < piece->len; piece->start += len1)
        {
          len1 = piece->len - piece->start;
          if (len1>cache_size)
            len1 = cache_size;
          len = compr->proc_buf(compr->data, (CharPtr)piece->data+piece->start,len1);
          if (len!= len1)
            {
              compr->close(compr->data,0);
              MemFree(compr);
              goto errexit;
            }
          cache_size *=2;
          if (cache_size > piece->len)
            cache_size = piece->len;
        }
      top = top->next;
      MemFree(piece->data);
      MemFree(piece);
    }
  compr->close(compr->data,1);
  MemFree(compr);
  return 1;
 errexit:
  return 0;
}

static int
store_blob(DB_stream_t    *db)
{
  CS_INT           count,restype;
  CS_RETCODE       retcode;
  struct ps_chunk *piece;
  Int4             expectation;
  Int4             expectationR;
  CS_COMMAND PNTR  cmd = db->clu.ctcmd;
  int attempt=0;

  expectationR=expectation=db->iodesc.total_txtlen;
 retry_trans:
  if(!CTLibSimpleSQL_Ex(cmd,"begin transaction"))
    /* looks like either handler, connection or server died */
    return 0; /*come backj and retry the whole process */
  if(!pubstruct_openblob (db, 0))
    goto errexit;
  db->iodesc.total_txtlen = expectation = expectationR ;
  CTRUN ( ct_command(cmd,CS_SEND_DATA_CMD,NULL,CS_UNUSED,CS_COLUMN_DATA) );
  CTRUN ( ct_data_info(cmd,CS_SET,CS_UNUSED,&db->iodesc) );
  
  for(piece = db->top ;piece ; piece = piece->next)
    {
      expectation -= piece->len;
      if ( expectation<0 )
        goto errexit;
      CTRUN(ct_send_data(cmd, (CS_VOID*)piece->data,(CS_INT)piece->len));
    }
  CTRUN ( ct_send(cmd) );
  CTRUN ( ct_results(cmd,&restype));
  if (restype == CS_CMD_FAIL)
    goto errexit;
  assert(restype == CS_PARAM_RESULT);
  CTRUN (ct_cancel(NULL,cmd, CS_CANCEL_ALL));
  if(!CTLibSimpleSQL_Ex(cmd,"commit transaction"))
    goto errexit;
  /* free space */
  while(db->top)
    {
      piece = db->top;
      db->top = piece->next;
      MemFree(piece->data);
      MemFree(piece);
    }
  return 1;
 errexit:
  CTLibSimpleSQL_Ex(cmd,"rollback transaction");
  if (attempt++ <20)
    {
      sleep(60);
      goto retry_trans;
    }
  return 0;
}

static Int4 LIBCALLBACK
pubstruct_closeasn(Pointer ptr,int commit)
{
  DB_stream_t    *db = (DB_stream_t *)ptr;

  switch(db->action)
    { 
    case PS_READ:
      break;
    case PS_STORE:
      return commit;
    case PS_NEW:
    case PS_UPDATE:
      {
        char            postupdate_cmd[1024];
        
        if(!commit)                 goto errexit;

        postupdate_cmd[0]=0;
        if (db->action == PS_NEW)
          if(pubstruct_parseasn(postupdate_cmd,db->acc,db->top->data,db->top->len)==0)
            goto errexit;
        assert(strlen(postupdate_cmd)< sizeof(postupdate_cmd));

        db->action = PS_STORE;

        if(!pack_blob(db))          goto errexit;
        
        if(!store_blob(db))         goto errexit;
        
        if (strlen(postupdate_cmd)>0)
          {
            int retries=0;
            /* we should expect transaction boundaries inside */
            while(retries++<20 &&
                  !CTLibSimpleSQL_Ex(db->clu.ctcmd,postupdate_cmd))
              sleep(10);
            if(retries>=20)
              goto errexit;
          }
        break;
      }
    default:
      ErrPostEx(SEV_FATAL, 0, 0,"Internal error at %s:%d: action = %d",
                __FILE__,__LINE__,db->action);
    }
  pubstruct_db_close(db);
  return 1;
  
 errexit:
  pubstruct_db_close(db);
  ErrPostEx(SEV_FATAL, 0, 0,"PubStruct update unsuccessfull");
  return 0;
}

/*****************************************************************************
 *
 ****************************************************************************/

static AsnIoPtr
pubstruct_openasnio(DB_stream_t *db)
{
  AsnIoPtr        aip     = NULL;
  
  if ( db->action == PS_READ )
    {
      aip = asnio2fci_open(1,compressor_open(cacher_open(dbio_open(db),100*1024,1
                                                         ),100*1024,1
                                             )
                           );
    }
  else /* write case */
    {
      db->iodesc.total_txtlen = 0;
      aip = asnio2fci_open(0,dbio_open(db));
    }
  return aip;
}


/*****************************************************************************
 *
 ****************************************************************************/

static AsnIoPtr
pubstruct_openasn (DB_stream_t *db, Int4 *accp,int mmdb_id)
{
  db->acc = 0;
  if (accp)
    {
      assert(*accp> 0);
      db->acc = *accp;
    }
  if ( db->action == PS_READ )
    {
      int try=0;
      int rc;
      while (try++<10)
        {
          rc = pubstruct_openblob(db,mmdb_id);
          ErrPostEx(SEV_INFO,0,0,"#######PubStruct openblob(acc=%d;mmdb=%d) = %d (1-ok,-1-no data,0-fail)\n",
                    db->acc,mmdb_id,rc);
          if(rc==1) break; /* Normal exit */
          fprintf(stderr,"#######PubStruct openblob(acc=%d;mmdb=%d) = %d\n",db->acc,mmdb_id,rc);
          if(rc<0) return NULL;
          CTLibDisconnect(&db->clu);
          sleep(2);
          ErrPostEx(SEV_ERROR, 0, 0,"PubStruct reconnecting");
          fprintf(stderr,"PubStruct reconnecting");
          if(!CTLibConnect(&db->clu,NULL))
            return NULL;
        }
      if(rc==0)
        return NULL;
    }
  return pubstruct_openasnio(db);
}

/**
 * PubStruct_closeasn closes AsnIO stream, which was open by some of functions
 * above and does some "termination procedure" which determined by thw function,
 * which opens connection.
 */

int      LIBCALL
PubStruct_closeasn(AsnIoPtr aip,int commit)
{
      return asnio2fci_close(aip,commit);
}

/**
 * PubStruct_newasn opens AsnIo stream to created new entry in the database. When
 * this stream is closed, database table's fields will be populated by data,
 * extracted from written asn. state is the only data - which is absent in asn.
 * (beside DB accession)
 * accp argument is optional - side effect return of new accession number
 */
static AsnIoPtr
pubstruct_newasn (DB_stream_t *db, int state, Int4 *accp)
{
  CS_COMMAND PNTR cmd = db->clu.ctcmd;
  char buffer[1024];
  CS_INT acc;

  CTLibSimpleSQL_Ex(cmd,"begin transaction");
  
  sprintf(buffer,"exec new_struct %d",state);
  
  if (!CTlibSingleValueSelect(cmd,buffer,&acc,sizeof(acc)))
    goto FATAL;
  if (!accp)
    accp = (Int4*)&acc;
  else
    *accp = (Int4)acc;
  /* unlock parallel processing */
  CTLibSimpleSQL_Ex(cmd,"commit transaction");
  
  return pubstruct_openasn (db, accp,0) ;  /* SUCCESSFULL exit */

FATAL:
  /* FAILURE exit */
  ErrPostEx(SEV_FATAL, 0, 0,"PubStruct insert unsuccessfull");
  CTLibSimpleSQL_Ex(cmd,"roolback transaction");
  pubstruct_db_close(db);
  ErrShow();
  return NULL;
}


AsnIoPtr LIBCALL
PubStruct_newasn (char *server,int state, Int4 *accp)
{
  DB_stream_t     *db;
  db = pubstruct_db_open(server,PS_NEW);
  if(!db)
    return NULL;
  return pubstruct_newasn (db,state,accp);
}

/**
 * PubStruct_readasn opens AsnIo stream for reading asn found by accession number
 */

AsnIoPtr LIBCALL
PubStruct_readasn1   (ps_handle_t db,Int4 acc)
{
  db->action=PS_READ; db->pending=0;
#ifdef DEBUG_MODE
  ErrPostEx(SEV_INFO,  ERR_SYBASE, 0,"view(acc=%d)",acc);
#endif
  return pubstruct_openasn (db, &acc,0);
}

AsnIoPtr LIBCALL
PubStruct_readasn    (char *server,Int4 acc)
{
  DB_stream_t     *db = pubstruct_db_open(server,PS_READ);
  if (!db)
    return NULL;
  return pubstruct_openasn (db, &acc,0);
}

/**
 * PubStruct_viewasn opens AsnIo stream for reading indexed asn found by mmdb_id
 */

AsnIoPtr LIBCALL
PubStruct_viewasn1   (ps_handle_t db,Int4 mmdbid)
{
  db->action=PS_READ; db->pending=0;
#ifdef DEBUG_MODE
  ErrPostEx(SEV_INFO,  ERR_SYBASE, 0,"view(mmdb=%d)",mmdbid);
#endif
  return pubstruct_openasn (db, NULL,mmdbid);
}

AsnIoPtr LIBCALL
PubStruct_viewasn    (char *server,Int4 mmdbid)
{
  DB_stream_t     *db = pubstruct_db_open(server,PS_READ);
  if (!db)
    return NULL;
  return pubstruct_openasn (db, NULL,mmdbid);
}

/**
 * PubStruct_updateasn opens AsnIo stream for updating existing asn. asn identified
 * by accession number. after updating the 'state' of the data become 'newstate'. 
 */
AsnIoPtr LIBCALL
PubStruct_updateasn  (char *server,Int4 acc, int newstate)
{
  DB_stream_t     *db; 
  CS_COMMAND PNTR cmd; 
  char buf[1024];

  db  = pubstruct_db_open(server,PS_UPDATE);
  if(!db)
    return NULL;
  
  cmd = db->clu.ctcmd;
  sprintf(buf,"exec push_struct %d,%d",acc,newstate);
  if(CTLibSimpleSQL_Ex(cmd,buf))
    return pubstruct_openasn (db, &acc,0) ;

  /* FAIL way */
  pubstruct_db_close(db);
  ErrShow();
  return NULL;
}

/**
 * PubStruct_removeasn suppress given asn.
 */
int      LIBCALL
PubStruct_removeasn  (char *server,Int4 acc)
{
  DB_stream_t     *db;
  CS_COMMAND PNTR cmd;
  char buf[1024];
  int rc = 0;
  
  db = pubstruct_db_open(server,PS_UPDATE);
  if(!db)
    return 0;
  cmd= db->clu.ctcmd;
  sprintf(buf,"exec rm_struct %d",acc);
  if (CTLibSimpleSQL_Ex(cmd,buf))
    if(CTLibSimpleSQL_Ex(cmd,"commit transaction"))
      rc = 1;
  pubstruct_db_close(db);
  return rc;
}

/**
 * File reading wrappers. ( a bit optimized )
 *
 * PubStruct_load read given asn from file stream and put it in database
 * (using ..._newasn) returns accession number if everything ok. or 0 in
 * case of fail
 */

int LIBCALL
PubStruct_load(FILE *infile, int state_out, char *server)
{
  AsnIoPtr aip;
  Int4 acc;
  DB_stream_t *db;
  int    old_asn = 1 ;

  if(!infile)
    return 0;
  db = pubstruct_db_open(server,PS_NEW);
  if(!db)
    return 0;
  assert(db->bottom==NULL);

  while (!feof(infile)) /* read data */
    {
      struct ps_chunk *piece;
      int len;

      if (db->bottom)
        piece = db->bottom;
      else
        db->bottom = db->top = piece = new_piece(0x4000, 1024L*1024L);
      if (piece->size == piece->len)
        db->bottom =  db->bottom->next = piece = new_piece(0x4000, 1024L*1024L);
      
      assert(piece);
      
      len = fread(piece->data + piece->len,1,piece->size - piece->len,infile);
      piece->len += len;
      db->iodesc.total_txtlen += len;
    }
  if ( state_out >=0 )
    { /* check on load */
      int  mmdb_id;
      Int4 acc1;
      
      mmdb_id = pubstruct_parseasn(NULL,0,db->top->data,db->top->len);
      if ( mmdb_id <= 0 )
        goto err;
      acc = PubStruct_lookup (server,mmdb_id,-state_out-1);
      if ( acc <= 0 )
        old_asn = 0;
      else
        {
          struct ps_chunk *piece = db->top;
          int    pos = 0;
          Int2   len;
          Int2   len1;
          
          aip = PubStruct_readasn ( server, acc );
          
          if(!aip)
            goto err;
          while(piece)
            {
              char buf[0x4000];
              
              assert(pos>=0 && pos <= piece->len);
              len1 = sizeof(buf);
              if ( piece->len - pos < len1)
                len1 = piece->len - pos;
              len = aip->readfunc(aip->iostruct, buf, len1);
              if (len <=0 )
                break;
              /* compare data */
              assert(pos>=0 && pos <= piece->len);
              assert(len <= piece->len - pos);
              if(memcmp(buf,piece->data+pos, len)!=0)
                {
                  old_asn = 0 ;
                  break;
                }
              pos+= len;
              if (piece->len == pos)
                {
                  piece = piece -> next ;
                  pos = 0;
                }
              if (len < len1)
                {
                  old_asn = 0;
                  break;
                }
            }
          PubStruct_closeasn (aip,0);
          aip = 0;
        }
    }
  if(old_asn && state_out >=0 )
    {
      acc = -acc ;
      printf("skipped...      ");
      pubstruct_db_close(db);
    }
  else
    {
      int txt = db->iodesc.total_txtlen;
      aip = pubstruct_newasn (db,(state_out>=0?state_out:-state_out-1),&acc);
      if(!aip) return 0;
      db->iodesc.total_txtlen = txt;
      if (!PubStruct_closeasn (aip,1))
        return 0;
    }
  return acc;
err:
  if(aip)
    PubStruct_closeasn (aip,0);
  pubstruct_db_close(db);
  return 0;
}

/**
 * PubStruct_download read given asn from DB and dump it to file stream
 */
int LIBCALL
PubStruct_download(char *server, Int4 acc, Int4 mmdb, FILE *outfile)
{
  char buf[0x4000];
  AsnIoPtr aip;

#ifdef PURIFY
  purify_new_inuse();
  purify_new_leaks();
#endif
  if(!outfile)
    return 0;
  if (acc>0)
    aip = PubStruct_readasn (server, acc);
  else
    aip = PubStruct_viewasn (server, mmdb);
  
  if(!aip)
    return 0;
  while(1)
    {
      int len;
      int len1;
      len = aip->readfunc(aip->iostruct, buf, sizeof(buf));
      if (len <=0 )
        break;
      len1 = fwrite(buf,1,len,outfile);
      assert(len == len1);
      if (len < sizeof(buf))
        break;
    }
  return PubStruct_closeasn (aip,1);
}

/**
 * PubStruct_lookup transforms mmdb and state into accession number (return value)
 * the meaning of state is as follows.
 * state = 0 : Production data
 * state > 0 : intermediate stages of asn assembling. "up to user"
 * state < 0 : request for Struct.state <= abs('state')
 */
NLM_EXTERN Int4     LIBCALL
PubStruct_lookup1(ps_handle_t db,Int4 mmdb,int state)
{
  CS_COMMAND PNTR cmd;
  CS_INT          count,restype;
  CS_RETCODE      retcode;
  Int4            status;
  char buf[1024];
  CS_INT acc;
  
  db->action=PS_READ; db->pending=0;
  cmd = db->clu.ctcmd;
  sprintf(buf,"exec id_find_gi %d,%d",(int)mmdb,(int)state);
  db->pending=1;
  
  ErrPostEx(SEV_INFO, 0, 0,buf);
#ifdef DEBUG_MODE
  ErrPostEx(SEV_INFO,  ERR_SYBASE, 0,"execute(%s)",buf);
#endif
  CTRUN ( ct_command(cmd,CS_LANG_CMD,(Pointer)buf,CS_NULLTERM,CS_UNUSED) );
  CTRUN ( ct_send(cmd) );
  CTRUN ( ct_results(cmd,&restype));

  /* skip 'exec' status line */
  CTlib_TYPERES(restype);
  if (restype == CS_STATUS_RESULT)
    {
      CTRUN(ct_cancel(NULL,db->clu.ctcmd, CS_CANCEL_CURRENT));
      CTRUN ( ct_results(cmd,&restype));
      CTlib_TYPERES(restype);
    }

  if (!(restype == CS_ROW_RESULT || restype == CS_PARAM_RESULT))
    goto errexit;
  count = 0;
  CTRUN1 ( ct_fetch(cmd,CS_UNUSED,CS_UNUSED,CS_UNUSED,&count),2) ;
  acc = 0;
  if (count==1)
    {    
      CTRUN1(ct_get_data(cmd,1,buf,(CS_INT)sizeof(buf),NULL),3);
      CTRUN1(ct_get_data(cmd,2,&acc,(CS_INT)sizeof(acc),NULL),3);
    }
  return acc;  /* SUCCESSFULL exit */

errexit:
  /* FAILURE exit */
  ErrPostEx(SEV_FATAL, 0, 0,"PubStruct lookup unsuccessfull");
  CTLibSimpleSQL_Ex(cmd,"roolback transaction");
  ErrShow();
  if(db->close_disabled)
    {
      pubstruct_db_close(db);
    }
  return 0;
}

NLM_EXTERN Int4     LIBCALL
PubStruct_lookup(char *server,Int4 mmdb,int state)
{
  DB_stream_t     *db;
  Int4             acc=0;
  db = pubstruct_db_open(server,PS_READ);
  if(!db)
    return 0;
  acc = PubStruct_lookup1(db,mmdb,state);
  pubstruct_db_close(db);
  return acc;
}

/**
 * PubStruct_pdb2mmdb make a fast lookup for given pdb code
 */

Int4     LIBCALL
PubStruct_pdb2mmdb1(ps_handle_t db,CharPtr pdb)
{
  char   buf[1024];
  CS_INT mmdb_id = 0  ;

  db->action=PS_READ; db->pending=0;
  sprintf(buf,"exec pdb2mmdb '%s'",pdb);
#ifdef DEBUG_MODE
  ErrPostEx(SEV_INFO,  ERR_SYBASE, 0,"execute(%s)",buf);
#endif
  if (!CTlibSingleValueSelect(db->clu.ctcmd,buf,&mmdb_id,sizeof(mmdb_id)))
    {
      mmdb_id = 0;
      if(db->close_disabled)
        {
          db->pending=1;
          pubstruct_db_close(db);
          db->pending=0;
        }
    }
  return mmdb_id;
}

Boolean LIBCALL
PubStruct_mmdb2livemmdb(ps_handle_t db,Int4 ommdb,Int4Ptr newmmdbid,Int4Ptr live,CharPtr pdb)
{
  char   buf[1024];
  CS_COMMAND PNTR cmd;
  CS_INT          count,restype;
  CS_RETCODE      retcode;
  Int4            status;

  if(!db)            return FALSE;
  if(!db->clu.ctcmd) return FALSE;
  
  db->action=PS_READ; db->pending=0;
  cmd = db->clu.ctcmd;
  sprintf(buf,"exec livemmdb %d",ommdb);
#ifdef DEBUG_MODE
  ErrPostEx(SEV_INFO,  ERR_SYBASE, 0,"execute(%s)",buf);
#endif

  CTRUN ( ct_command(cmd,CS_LANG_CMD,(Pointer)buf,CS_NULLTERM,CS_UNUSED) );
  CTRUN ( ct_send(cmd) );
  CTRUN ( ct_results(cmd,&restype));

  /* skip 'exec' status line */
  CTlib_TYPERES(restype);
  if (restype == CS_STATUS_RESULT)
    {
      CTRUN(ct_cancel(NULL,cmd, CS_CANCEL_CURRENT));
      CTRUN ( ct_results(cmd,&restype));
      CTlib_TYPERES(restype);
    }
  
  if (!(restype == CS_ROW_RESULT || restype == CS_PARAM_RESULT))
    goto errexit;
  count = 0;
  CTRUN1 ( ct_fetch(cmd,CS_UNUSED,CS_UNUSED,CS_UNUSED,&count),2) ;
  if (count==1)
    {    
      CS_INT     val;
      CS_TINYINT val1;
      
      if(pdb) {
        CTRUN1(ct_get_data(cmd,1,buf,(CS_INT)sizeof(buf),&val),3);
        buf[val]=0;
        strcpy(pdb,buf);
      }
      CTRUN1(ct_get_data(cmd,2,&val,(CS_INT)sizeof(val),NULL),3);
      if(newmmdbid) *newmmdbid = val;
      CTRUN1(ct_get_data(cmd,3,&val1,(CS_INT)sizeof(val1),NULL),2);
      if(live) *live = val1;
      db->pending=1; 
    }
  pubstruct_db_close(db);
  return TRUE;  /* SUCCESSFULL exit */

errexit:
  /* FAILURE exit */
  ErrPostEx(SEV_FATAL, 0, 0,"PubStruct lookup unsuccessfull");
  ErrShow();
  pubstruct_db_close(db);
  return FALSE;
}

Int4     LIBCALL
PubStruct_pdb2mmdb(char *server,CharPtr pdb)
{
  DB_stream_t     *db;
  CS_INT           mmdb_id;
  
  db = pubstruct_db_open(server,PS_READ);
  if(!db) return 0;
  mmdb_id = PubStruct_pdb2mmdb1(db,pdb);
  
  pubstruct_db_close(db);
  return mmdb_id;
}

/**********************************************************************/
ps_handle_t
PubStruct_connect   (char *server)
{
  ps_handle_t db ;
  db = pubstruct_db_open(server,PS_READ);
  if(db)
    db->close_disabled = 1 ;
  return db;
}

NLM_EXTERN void
PubStruct_disconnect(ps_handle_t db)
{
  if(db)
    db->close_disabled=0;
  pubstruct_db_close(db);
}


