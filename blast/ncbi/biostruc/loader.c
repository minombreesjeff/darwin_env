/*   $Id: loader.c,v 6.14 1999/05/25 16:21:20 kimelman Exp $
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
 * PubStruct DB Loader.
 *                   
 * Modifications:  
 * --------------------------------------------------------------------------
 * $Log: loader.c,v $
 * Revision 6.14  1999/05/25 16:21:20  kimelman
 * bugfix : --checks & --load / didn't generate error message
 *
 * Revision 6.13  1998/11/20 22:46:00  kimelman
 * fix loader return code
 *
 * Revision 6.12  1998/11/06 18:59:07  kimelman
 * PubStruct loading transaction granularity changed
 *
 * Revision 6.11  1998/10/22 17:01:20  kimelman
 * fix default dump directory
 *
 * Revision 6.10  1998/10/22  15:24:28  kimelman
 * fixed path to check_loader & default dump area
 *
 * Revision 6.9  1998/10/05  17:50:20  kimelman
 * processing fixed for "file not found" on load case
 *
 * Revision 6.8  1998/09/03 22:05:26  kimelman
 * added cycles to retry on failes and
 * wait for unhappy server to recover
 *
 * Revision 6.7  1998/08/05 21:12:35  kimelman
 * skip/load bugfix
 *
 * Revision 6.6  1998/07/22 22:08:28  kimelman
 * bugfix : check&create  (./mmdb ==> $outdir)
 *
 * Revision 6.5  1998/07/16 20:00:33  kimelman
 * pack & enforce options added
 *
 * Revision 6.4  1998/07/14 20:24:51  kimelman
 * FT schema & smart load
 *
 * Revision 6.3  1998/05/14 16:11:17  kimelman
 * Compression stuff added in debug mode.
 * few bugs fixed, related to OpenServer/SQL Server switching
 *
 * Revision 6.2  1998/05/08 03:03:47  kimelman
 * Open Server fix
 *
 * Revision 6.1  1998/04/03 20:25:21  kimelman
 * PubStruct access code added to mmdbsrv
 *
 *
 * ==========================================================================
 */


#include "PubStructAsn.h"
#include <assert.h>

#define LOAD_IN_STATE 1

static char  MMDB[PATH_MAX];
static char  outdir[PATH_MAX];
static char  bindir[PATH_MAX];
static char *server=NULL;
static int   state = -1;
static int   enforce = 0;

int
remove_struct(int uid)
{
  char path[1024];
  int acc;
  
  acc = PubStruct_lookup(server,uid,state);
  if(acc)
    acc = PubStruct_removeasn(server,acc);
  return acc;
}

int
load(int uid)
{
  char path[1024];
  char fname[1024];
  AsnIoPtr aip;
  FILE *p;
  int acc = 0;
  int pipe = 1;
  
  sprintf(fname, "%s%ld.val.gz",MMDB,uid);
  p = fopen(fname,"r");
  if(p)
    {
      fclose(p);
      sprintf(path, "gzip -dc %s%ld.val.gz",MMDB,uid);
      /* sprintf(path, "gzip -dc %ld.val.gz",uid); */
      p = popen(path,"rb");
    }
  if(!p)
    {
      sprintf(fname, "%s%ld.val",MMDB,uid);
      p = fopen(fname,"r");
      pipe = 0;
    }
  if(p)
    {
      acc = PubStruct_load(p,(enforce?-state-1:state),server);
      if(pipe)
        pclose(p);
      else
        fclose(p);
      return acc;
    }
#if 1
  fprintf(stderr, "Can't open file: %s%d.val(.gz) ",MMDB,uid);
  return -2;
#else
  sprintf(fname, "%s%ld.txt",MMDB,uid);
  p = fopen(fname,"r");
  if(!p)
    return 0;

  fclose(p);
  
  {
    BiostrucPtr bsp = NULL;
    Boolean     commit;
          
    aip = AsnIoOpen(fname,"r");
    bsp=BiostrucAsnRead(aip,NULL);
    AsnIoClose(aip);
          
    aip = PubStruct_newasn(server,state,&acc);
    commit = BiostrucAsnWrite(bsp,aip,NULL);
    commit = PubStruct_closeasn(aip,commit);
    
    if (!commit)  acc = 0;
  }
  return acc;
#endif
}

int
download(int uid, int pack)
{
  char path[1024];
  FILE *p;
  int acc;

  if(!pack)
    {
      sprintf(path,"%s/%ld.val",outdir,uid);
      p = fopen(path,"wb");
    }
  else
    {
      sprintf(path, "gzip -c >%s/%ld.val.gz",outdir,uid);
      p = popen(path,"wb");
    }
  
  if(!p)
    return 0;
  if (state==0)
    acc = PubStruct_download(server,0,uid,p); /* can go throuth the open server */
  else
    {
      acc = PubStruct_lookup(server,uid,state);
      if(acc)
        acc = PubStruct_download(server,acc,uid,p);
    }
  if(pack)
    pclose(p);
  else
    fclose(p);
  return acc;
}

/* QA */
void
check(int uid)
{
  char cmd[1024];
  
  sprintf(cmd, "sh %s/loader_check.sh %s %ld.val.gz %s", bindir, MMDB, uid, outdir);
  system(cmd);
}


#ifdef PURIFY
static void
memory_leaks(void)
{
  CTLibUtils       clu;
  
  if(!CTLibInit(&clu,"MOZART_SYS10","PubStruct","kimelman","kmlmNu",0,NULL))
    return;
  CTLibDrop(&clu);
  purify_new_inuse();
  purify_new_leaks();
}
#endif


int
main(int argc, char**argv)
{
  int mode = 0;
  int pack = 0;
  int failed = 0; 
  int uid;
  unsigned long flags;
  setbuf(stdout,NULL);
  strcpy(MMDB,"/net/vaster/mmdb/mmdb/data/");
  strcpy(outdir,"/tmp");
  {
    FILE *f = fopen(argv[0],"r");
    char *p; 
    if (f) /* great! - we have a direct path to binary program */
      strcpy(bindir,argv[0]);
    else
      { /* we should find the binary in the path */
        char buf[1000];
        sprintf(buf,"which %s", argv[0]);
        f = popen(buf,"r");
        assert(f);
        fscanf(f,"%s",bindir);
        pclose(f);
        f = fopen(bindir,"r");
      }
    if (!f)
      {
        fprintf(stderr,"Can't determine the path to binary - tried :\n%s\n%s\n",argv[0],bindir);
        exit(1);
      }
    assert(f);
    fclose(f);
    /* strip file name from path to get the dir name */
    p = bindir + strlen(bindir);
    while (p != bindir && *p != '/')
      p--;
    assert( p == bindir || *p == '/');
    if (*p == '/')
      *p = 0;
  }
#ifdef PURIFY
  purify_all_leaks();
  purify_all_inuse();
#endif
  {
    int i;
    for (i = 1 ; i < argc ; i++)
      {
        if (strcmp(argv[i],"--remove")==0 || strcmp(argv[i],"-r")==0)
          mode |=8;
        else if (strcmp(argv[i],"--load")==0 || strcmp(argv[i],"-l")==0)
          mode |=1;
        else if (strcmp(argv[i],"--download")==0|| strcmp(argv[i],"-d")==0)
          mode |=2;
        else if (strcmp(argv[i],"--checks")==0|| strcmp(argv[i],"-c")==0)
          {
            if (mode & 2 )
              mode |= 4;
            else
              goto errexit;
          }
        else if (strcmp(argv[i],"--verbose")==0 || strcmp(argv[i],"-v")==0)
          {
            ErrSetLogLevel(0);
            ErrSetOptFlags(EO_LOGTO_STDERR);
          }
        else if (strncmp(argv[i],"--path=",strlen("--path="))==0)
          strcpy(MMDB,argv[i]+strlen("--path="));
        else if (strcmp(argv[i],"--pack")==0)
          pack = 1;
        else if (strcmp(argv[i],"--enforce")==0)
          enforce = 1;
        else if (strncmp(argv[i],"--downpath=",strlen("--downpath="))==0)
          strcpy(outdir,argv[i]+strlen("--downpath="));
        else if (strncmp(argv[i],"--dbpath=",strlen("--dbpath="))==0)
          server = argv[i]+strlen("--dbpath=");
        else if (strncmp(argv[i],"--state=",strlen("--state="))==0)
          state = atol(argv[i]+strlen("--state="));
        else
          goto errexit;
      }
  }
  if ( MMDB[strlen(MMDB)-1] != '/')
    strcat(MMDB,"/");
  if (state<0 )
    state = (mode & 1 ? LOAD_IN_STATE :0);
  if (mode & 2 )
    {
      char shcmd[1024];
      sprintf(shcmd," test -d %s || mkdir %s",outdir,outdir);
      system (shcmd);
    }
    
  while(scanf("%d",&uid) != EOF)
    {
      int rc, repeat=5;
      printf("%5d: ",uid);
      while(repeat--)
        {
          if(mode & 8)
            {
              printf("removing... ");
              rc = remove_struct(uid);
            }
          else
            {
              rc = 1;
              if(mode & 1)
                { printf("loading...     "); rc = load(uid);          }
              if(mode & 2 && rc )
                { printf("downloading... "); rc = download(uid,pack); }
              if(mode & 4 && rc )
                { check(uid); rc = -1 ;                               }
            }
          if(rc)
            break;
          printf("slee-e-ep...  ");
          sleep(60);
        }
      if (rc == -1 )
        printf(";\n");
      else
        {
          printf("%s ;\n",rc ?"ok":"FAILED");
          if (rc==0)
            failed = 1;
        }
      ErrShow();
    }
  return (failed?1:0);
errexit:
  printf("usage: %s [ --load | --remove | --download [ --checks | --pack ] ] \n"
         "          [ --path=mmdb_files_dir ] [ --downpath=dump_mmdb_files_dir  ] \n"
         "          [ --state=<to_state> ] [ --verbose | -v ] [ --enforce ] \n"
         "          [ --dbpath=servername:DBname=username:password ]\n",
         argv[0]);
  return 2;
}
