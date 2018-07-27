/*  idfetch.c
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
 * Author Karl Sirotkin
 *
 $Log: idfetch.c,v $
 Revision 1.21  2001/11/02 14:24:44  kans
 made Fasta style SeqId args multi-line for Mac window

 Revision 1.20  2001/11/02 12:36:20  kans
 now using public Entrez2 server

 Revision 1.19  2001/09/28 15:56:04  kans
 look for extra and title fields in Entrez2 docsum

 Revision 1.18  2001/09/10 21:09:36  kans
 changed to use new Entrez2DocsumDataPtr - still need to get example of field_name keys

 Revision 1.17  2001/02/12 21:57:11  butanaev
 Made 3 retries to EntrezSynchronousQuery() when the NULL is returned.

 Revision 1.16  2001/02/08 16:13:46  yaschenk
 fixing wrong check for missing version in _PIR and SP

 Revision 1.15  2000/10/06 22:59:44  yaschenk
 strncpy not setting \0 bug

 Revision 1.14  2000/08/10 15:17:38  butanaev
 Updated -t 7 mode: strings like 'gi|3|emb|A00003.1|A00003' retreived from
 Entrez2DocsumPtr->caption.

 Revision 1.13  2000/08/03 17:01:23  kans
 included ni_lib.h for Mac, removed Mac compiler warnings

 Revision 1.12  2000/08/02 16:55:28  yaschenk
 increasing buffer size to 1000

 Revision 1.11  2000/08/02 16:17:00  butanaev
 Added:
 -t 7 - to retrieve Entrez DocSums
 -Q filename - to read Entrez query from the file

 Revision 1.9  2000/07/13 16:46:54  yaschenk
 adding ObjMgrFreeCache(0) to avoid hitting the limit in ObrMgr

 Revision 1.2  2000/06/01 18:05:35  butanaev
 Fixed numerous bugs with control flow...

 Revision 1.1  2000/06/01 16:48:22  butanaev
 New functionality:
 -G parameter, which previously accepted the list of gi's,
 now accepts gi,accession,accession.version,fasta seqid,
 which can be mixed.

 -q parameter generates the list out of Entrez query
   when -q is used -d has special meaning:
   -d n - run query against Nucleotide database
   -d p - run query against Protein database

 -n parameter limits the output to the list of gi's

 Revision 1.6  2000/05/24 17:30:42  yaschenk
 make parameter list look better

 Revision 1.5  2000/05/23 15:42:08  yaschenk
 adding quality score display

 Revision 1.4  2000/03/31 18:35:58  yaschenk
 Adding Jonathan's logic for FF and FASTA

 Revision 1.3  2000/03/30 20:43:51  yaschenk
 adding AsnIoReset between Entries

 Revision 1.2  1999/11/02 18:27:43  yaschenk
 adding -G parameter to idfetch

 Revision 1.1  1998/12/28 17:56:29  yaschenk
 preparing idfetch to go to production

 Revision 1.1  1997/05/29 14:34:07  sirotkin
 syncing sampson from mutant for procs. taking source from sampson. this is now current

 * Revision 4.0  1995/07/26  13:55:55  ostell
 * force revision to 4.0
 *
 * Revision 1.3  1995/06/21  14:14:29  kans
 * replaced asn2ff_entrez with SeqEntryToFlat
 *
 * Revision 1.2  1995/05/17  17:59:15  epstein
 * add RCS log revision history
 *
 * Revision 1.1  94/08/11  13:26:31  ostell
 * Initial revision
 *
 * Revision 1.3  1993/12/02  10:12:41  kans
 * Includes <ncbi.h> instead of <sys/types.h>
 *
 * Revision 1.2  93/11/24  13:25:56  sirotkin
 * First working version
 *
 * Revision 1.1  93/11/23  16:01:51  sirotkin
 * Initial revision
 *
 revised by OStell for public use.
 *
 * Modified by Eugene Yaschenko for ID1 Server
 *
 */

#include <ncbi.h>
#include <objsset.h>
#include <accid1.h>
#include <asn2ff.h>
#include <tofasta.h>
#include <ni_types.h>
#include <ni_lib.h>
#include <sqnutils.h>
#include <sequtil.h>
#include <ffprint.h>
#include <ent2api.h>

static Boolean ProcessOneDocSum (Int4 num, Int4Ptr uids);
static void EntrezQuery(char *query);

static Int4 BEGetUidsFromQuery(CharPtr query, Uint4Ptr PNTR uids,
                               Boolean is_na, Boolean count_only);
static Boolean IdFetch_func1(CharPtr data, Int2 maxplex);
static Boolean IdFetch_func(Int4 gi,CharPtr db, Int4 ent,Int2 maxplex);

Args myargs[] = {
	{"Filename for output ","stdout", NULL,NULL,FALSE,'o',ARG_FILE_OUT, 0.0,0,NULL},
	{"Output type:\t\
1=text asn.1\n\t\t\t\
2=Binary asn.1\n\t\t\t\
3=Genbank (Seq-entry only)\n\t\t\t\
4=genpept (Seq-entry only)\n\t\t\t\
5=fasta (table for history)\n\t\t\t\
6=quality scores (Seq-entry only)\n\t\t\t\
7=Entrez DocSums\n","1", "1", "7", FALSE, 't', ARG_INT, 0.0, 0, NULL } ,
{"Database to use (special meaning for -q flag: n - nucleotide, p - protein)",NULL,NULL,NULL,TRUE,'d',ARG_STRING,0.0,0,NULL},
	{"Entity number (retrieval number) to dump" ,"0","0","99999999",TRUE,'e',ARG_INT,0.0,0,NULL},
        {"Type of lookup:\t\
0 - get Seq-entry\n\t\t\t\
1 - get gi state (output to stderr)\n\t\t\t\
2 - get SeqIds\n\t\t\t\
3 - get gi historyn (sequence change only)\n\t\t\t\
4 - get gi revision history (any change to asn.1)\n", "0","0","4",TRUE,'i',ARG_INT,0.0,0,NULL},
	{"GI id for single Entity to dump" ,"0","0","99999999",TRUE,'g',ARG_INT,0.0,0,NULL},
	{"File with list of gi's, accessions, accession.version's, fasta seqid's to dump",NULL,NULL,NULL,TRUE,'G',ARG_FILE_IN,0.0,0,NULL},
	{"Max complexity:\t\
0 - get the whole blob\n\t\t\t\
1 - get the bioseq of interest\n\t\t\t\
2 - get the minimal bioseq-set containing the bioseq of interest\n\t\t\t\
3 - get the minimal nuc-prot containing the bioseq of interest\n\t\t\t\
4 - get the minimal pub-set containing the bioseq of interest\n" ,"0","0","4",TRUE,'c',ARG_INT,0.0,0,NULL},
 	{"flaTtened SeqId, format: \n		\'type(name,accession,release,version)\'\n			as \'5(HUMHBB)\' or \n		type=accession, or \n		type:number ",
		NULL,NULL,NULL,TRUE,'f',ARG_STRING,0.0,0,NULL},
 	{"Fasta style SeqId ENCLOSED IN QUOTES:\n\t\t\t\
lcl|int or str bbs|int bbm|int gb|acc|loc\n\t\t\t\
emb|acc|loc pir|acc|name sp|acc|name\n\t\t\t\
pat|country|patent|seq gi|int dbj|acc|loc\n\t\t\t\
prf|acc|name pdb|entry|chain",
	NULL,NULL,NULL,TRUE,'s',ARG_STRING,0.0,0,NULL},
        {"Log file", NULL,NULL,NULL,TRUE,'l',ARG_FILE_OUT,0.0,0,NULL},
        {"Generate gi list by entrez query", NULL,NULL,NULL,TRUE,'q',ARG_STRING,0.0,0,NULL},
        {"Generate gi list by entrez query", NULL,NULL,NULL,TRUE,'Q',ARG_FILE_IN,0.0,0,NULL},
        {"Output only the list of gis, used with -q", NULL,NULL,NULL,TRUE,'n',ARG_BOOLEAN,0.0,0,NULL}
};
int Numarg = sizeof(myargs)/sizeof(myargs[0]);

#define MACRO_SETARG(TAG,P) \
   {\
      P = Nlm_WhichArg (TAG, Numarg, myargs);\
      if( P < 0){\
         ErrPost(CTX_NCBIIDRETRIEVE,10,\
         "Program error looking for arg %c\n", TAG);\
         has_trouble = TRUE;\
      }\
   }

static Nlm_Int2 Nlm_WhichArg PROTO(( Nlm_Char which, Nlm_Int2 numargs, Nlm_ArgPtr ap));

Int4 giBuffer[1000];
int giBufferPos = 0;

DataVal Val;
Int2 fileoutarg, logarg, outtypearg,maxplexarg,seqidarg,
  giarg, fastaarg, infotypearg, entarg, dbarg, gifilearg,
  entrezqueryarg, entrezqueryfilearg, onlylistarg;

FILE * fp = NULL;
AsnIoPtr asnout=NULL;

Int2 Main()
{
  Boolean has_trouble = FALSE;
  Int4 entity_spec_count = 0;
  CharPtr outmode;
  Int4 ent = 0;
  Int4 gi = 0;
  FILE * fp_in = NULL;
  SeqIdPtr sip;
  Char tbuf[1024];

  /* check command line arguments */

  if( ! GetArgs("idfetch.c",Numarg, myargs))
    return 1;


  /********************************************************************
   ****                                                            ****
   ****  Map Args So Can be Accessed in order independent fashion  ****
   ****                                                            ****
   *******************************************************************/

  MACRO_SETARG('o', fileoutarg)
  MACRO_SETARG('t', outtypearg)
  MACRO_SETARG('i', infotypearg)
  MACRO_SETARG('c', maxplexarg)
  MACRO_SETARG('e', entarg)
  MACRO_SETARG('d', dbarg)
  MACRO_SETARG('g', giarg)
  MACRO_SETARG('G', gifilearg)
  MACRO_SETARG('f', seqidarg)
  MACRO_SETARG('l', logarg)
  MACRO_SETARG('s', fastaarg)
  MACRO_SETARG('q', entrezqueryarg)
  MACRO_SETARG('Q', entrezqueryfilearg)
  MACRO_SETARG('n', onlylistarg)

  if(! SeqEntryLoad())
    ErrShow();

  EntrezSetProgramName("IDFETCH");
  /* EntrezSetServer("www.ncbi.nlm.nih.gov", 80,
                  "/entrez/utils/entrez2server.fcgi");
  */

  if(myargs[entrezqueryarg].strvalue || myargs[entrezqueryfilearg].strvalue)
  {
    if(! myargs[dbarg].strvalue ||
       (0 != strcmp(myargs[dbarg].strvalue, "n") &&
        0 != strcmp(myargs[dbarg].strvalue, "p")))
    {
      ErrPost(CTX_NCBIIDRETRIEVE, 10, "-q should be with protein (-dp) or nucleotide (-dn)\n");
      exit(1);
    }
  }

  if(myargs[logarg].strvalue != NULL)
  {
    if(! ErrSetLog (myargs[logarg].strvalue))
    {
      ErrShow();
      has_trouble = TRUE;
    }
    else
    {
      ErrSetOpts (ERR_TEE, ERR_LOG_ON);
    }
  }
  if(myargs[infotypearg].intvalue>1
     && (myargs[outtypearg].intvalue == 3
         || myargs[outtypearg].intvalue == 4
         || myargs[outtypearg].intvalue == 6
        ))
  {
    ErrPostEx(SEV_ERROR,0,0,"-t 3,4,6 can be used only with -i 0");
    has_trouble=TRUE;
    goto FATAL;
  }

  if(myargs[outtypearg].intvalue == 7 && ! myargs[dbarg].strvalue)
  {
    ErrPostEx(SEV_ERROR,0,0,"-t 7 can be used only with protein (-dp) or nucleotide (-dn)");
    has_trouble=TRUE;
    goto FATAL;
  }

  if(myargs[outtypearg].intvalue == 7)
    myargs[infotypearg].intvalue = 2;

  if(myargs[entrezqueryarg].strvalue || myargs[entrezqueryfilearg].strvalue)
    entity_spec_count++;

  if(myargs[giarg].intvalue)
    entity_spec_count++;

  if(myargs[seqidarg].strvalue)
    entity_spec_count++;

  if(myargs[fastaarg].strvalue)
    entity_spec_count++;

  if(myargs[gifilearg].strvalue)
  {
    fp_in=FileOpen(myargs[gifilearg].strvalue,"r");
    if(fp_in==NULL)
    {
      ErrPostEx(SEV_ERROR, 0, 0, "couldn't open file %s", myargs[gifilearg].strvalue);
      has_trouble=TRUE;
      goto FATAL;
    }
    entity_spec_count ++;
  }

  if(entity_spec_count != 1)
  {
    ErrPostEx(SEV_ERROR,0,0, "One and only one parameters may be used: -g,-G,-f,-s -q");
    has_trouble=TRUE;
    goto FATAL;
  }
  if(myargs[infotypearg].intvalue != 1)
  {
    outmode = "w";
    switch(myargs[outtypearg].intvalue)
    {
    case 2:
      outmode = "wb";
    case 1:
      asnout = AsnIoOpen((CharPtr)myargs[fileoutarg].strvalue, outmode);
      if(asnout == NULL)
      {
        ErrPost(CTX_NCBIIDRETRIEVE,10,
                "Could not open %s for asn output\n",
                myargs[fileoutarg].strvalue);
        has_trouble = TRUE;
      }
      break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      fp = FileOpen((CharPtr)myargs[fileoutarg].strvalue, outmode);
      if(fp == NULL)
      {
        ErrPost(CTX_NCBIIDRETRIEVE,10,
                "Could not open %s for output\n",
                myargs[fileoutarg].strvalue);
        has_trouble = TRUE;
      }
      break;
    }
  }

  if( has_trouble )
    exit (1);

  if(fp_in || myargs[entrezqueryarg].strvalue || myargs[entrezqueryfilearg].strvalue)
  { /*** Statefull mode ***/
    NI_SetInterface(eNII_WWW);
  }
  else
  { /*** Stateless mode ***/
    NI_SetInterface(eNII_WWWDirect);
  }

  if(!ID1BioseqFetchEnable("idfetch",TRUE))
  {
    ErrPost(CTX_NCBIIDRETRIEVE, 20, "Could not open ID1 service");
    exit(1);
  }

  if(myargs[giarg].intvalue)
  {
    gi = myargs[giarg].intvalue;
  }
  else if(myargs[entrezqueryarg].strvalue || myargs[entrezqueryfilearg].strvalue)
  {}
  else if(fp_in)
  {}
  else if(myargs[fastaarg].strvalue != NULL)
  {
    sip = SeqIdParse((CharPtr)myargs[fastaarg].strvalue);
    if(sip == NULL)
    {
      ErrPostEx(SEV_ERROR,0,0,"Couldn't parse [%s]", myargs[fastaarg].strvalue);
      exit(1);
    }
  }
  else
  {
    /*
     "flaTtened SeqId, format:
     type(name,accession,release,version) or type=accession",
     */
    static CharPtr name = NULL, accession = NULL, release = NULL, version = NULL, number = NULL;
    CharPtr p;
    int type_int;
    static CharPtr PNTR fields [] = {&name, &accession, &release, &number};
    Boolean found_equals = FALSE, found_left = FALSE,
      found_colon = FALSE, flat_seqid_err = FALSE,
    dna_type = FALSE, any_type = FALSE;
    int dex;
    TextSeqIdPtr tsip;

    sip = ValNodeNew(NULL);
    type_int = atoi(myargs[seqidarg].strvalue);
    for(p = myargs[seqidarg].strvalue; *p; p++ )
    {
      if( *p == '(' || *p == '='  || *p == ':')
      {  /* ) match */

        if( *p == '('  )
        {  /* ) match */
          found_left = TRUE;
          if(p == myargs[seqidarg].strvalue)
          {
            any_type = TRUE;
            ErrPost(CTX_NCBIIDRETRIEVE,10,
                    "Sorry, any type is not allowed for ID1service");
            exit(1);
          }
          else if( p - myargs[seqidarg].strvalue == 1)
          {
            if(*myargs[seqidarg].strvalue == '0')
            {
              dna_type = TRUE;
              ErrPost(CTX_NCBIIDRETRIEVE,10,
                      "Sorry, 0== nucroe3 type is not allowed for ID1service");
              exit(1);
            }
          }
        }
        else if( *p == '=')
        {
          found_equals = TRUE;
          if(p == myargs[seqidarg].strvalue)
          {
            any_type = TRUE;
          }
          else if( p - myargs[seqidarg].strvalue == 1)
          {
            if(*myargs[seqidarg].strvalue == '0')
            {
              dna_type = TRUE;
            }
          }
        }
        else if( *p == ':')
        {
          found_colon = TRUE;
          if(p == myargs[seqidarg].strvalue)
          {
            any_type = TRUE;
          }
          else if( p - myargs[seqidarg].strvalue == 1)
          {
            if(*myargs[seqidarg].strvalue == '0')
            {
              dna_type = TRUE;
            }
          }
        }
        *p = '\0';
        p++;
        break;
      }
    }
    if( found_left)
    {
      for( * (fields[0]) = p, dex = 0; *p && dex < 4; p++)
      {
        if( *p == ',' || *p == ')' )
        {
          *p = '\0';
          dex ++;
          *(fields[dex]) = p + 1;
        }
      }
    }
    else if(found_equals)
    {
      accession = p;
    }
    else if(found_colon)
    {
      number = p;
    }
    else
    {
      ErrPost(CTX_NCBIIDRETRIEVE, 10,
              "id1test: could not find \'(\' or \'=\' or \':\' in flattened seqid=%s",myargs[seqidarg].strvalue);  /* ) match */
      exit(1);
    }
    sip->choice = type_int;
    switch(type_int)
    {
    case SEQID_GIBBSQ :
    case SEQID_GIBBMT :
    case SEQID_GI :
      sip->data.intvalue = atoi(number);
      break;
    case SEQID_GENBANK :
    case SEQID_EMBL :
    case SEQID_DDBJ :
    case SEQID_PIR :
    case SEQID_SWISSPROT :
    case SEQID_OTHER :
    case SEQID_PRF :
      tsip = TextSeqIdNew();
      sip->data.ptrvalue = tsip;
      if(accession)
        if(!*accession)
          accession = NULL;
      if(release)
        if(!*release)
          release = NULL;
      if(name)
        if(!*name)
          name = NULL;
      tsip->name = StringSave(name);
      tsip->accession = StringSave(accession);
      tsip->release = StringSave(release);
      break;
    case SEQID_PATENT :
    case SEQID_GENERAL :
    case SEQID_PDB :
    case SEQID_LOCAL :
      ErrPost(CTX_NCBIIDRETRIEVE,30,
              "Sorry, this test program does not support %d patent, general, pdb, or local, try id2asn ",
              type_int);
      exit(1);
      break;
    }
  }

  if(! fp_in && ! gi && ! (myargs[entrezqueryarg].strvalue || myargs[entrezqueryfilearg].strvalue))
  {
    gi = ID1ArchGIGet (sip);
    if(gi <= 0)
    {
      SeqIdPrint(sip, tbuf, PRINTID_FASTA_SHORT);
      ErrPostEx(SEV_ERROR, 0, 0, "Couldn't find SeqId [%s]", tbuf);
      goto FATAL;
    }
  }
  else if(fp_in)
  {
    while(fgets(tbuf,sizeof(tbuf)-1,fp_in))
      IdFetch_func1(tbuf, myargs[maxplexarg].intvalue);
  }
  else if(myargs[entrezqueryarg].strvalue || myargs[entrezqueryfilearg].strvalue)
  {
    if(myargs[entrezqueryarg].strvalue)
      EntrezQuery(myargs[entrezqueryarg].strvalue);
    else if(myargs[entrezqueryfilearg].strvalue)
    {
      FILE *fp_query=FileOpen(myargs[entrezqueryfilearg].strvalue,"r");
      if(fp_query==NULL)
      {
        ErrPostEx(SEV_ERROR, 0, 0, "couldn't open file %s", myargs[entrezqueryfilearg].strvalue);
        has_trouble=TRUE;
        goto FATAL;
      }
      {
        char buffer[2000];
        while(fgets(buffer, sizeof(buffer) - 1,fp_query))
        {
          int len = strlen(buffer);
          if(buffer[len - 1] == '\n')
            buffer[len - 1] = 0;
          EntrezQuery(buffer);
        }
      }
    }
  }

  if(gi>0 && !IdFetch_func(gi,myargs[dbarg].strvalue, myargs[entarg].intvalue,myargs[maxplexarg].intvalue))
  {
    has_trouble=TRUE;
    goto FATAL;
  }

  if(giBufferPos != 0)
  {
    if(! ProcessOneDocSum(giBufferPos, giBuffer))
    {
      has_trouble=TRUE;
      goto FATAL;
    }
  }

FATAL:
  if(asnout)
    AsnIoClose(asnout);
  if(fp)
    FileClose(fp);
  if(fp_in)
    FileClose(fp_in);

  /*ID1ArchFini();*/

  return(has_trouble?1:0);
}

void EntrezQuery(char *query)
{
  Uint4 *ids;
  int count;
  int i;
  count = BEGetUidsFromQuery(query,
                             &ids,
                             0 == strcmp(myargs[dbarg].strvalue, "n"), FALSE);
  for(i = 0; i < count; ++i)
  {
    if(myargs[onlylistarg].intvalue)
      printf("%d\n", ids[i]);
    else
      IdFetch_func(ids[i],
                   myargs[dbarg].strvalue,
                   myargs[entarg].intvalue,
                   myargs[maxplexarg].intvalue);
  }
}

static void PrintQualScores (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
  BioseqPtr  bsp;
  FILE       *fp;

  if(IS_Bioseq (sep))
  {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    fp = (FILE*) data;
    PrintQualityScores (bsp, fp);
  }
}

static int TryGetGi(int choice, char *accession, char *name, int version)
{
  TextSeqIdPtr tsip;
  SeqIdPtr sip;

  sip = ValNodeNew(NULL);
  tsip = TextSeqIdNew();
  sip->choice = choice;
  sip->data.ptrvalue = tsip;
  tsip->name = NULL;
  tsip->accession = NULL;
  tsip->release = NULL;

  if(name != NULL)
    tsip->name = StringSave(name);
  if(accession != NULL)
    tsip->accession = StringSave(accession);
  tsip->version = version;
  return ID1ArchGIGet(sip);
}

static Boolean IdFetch_func1(CharPtr data, Int2 maxplex)
{
  int seqIdParse = 0, hasVersion = 0, accession = 0;
  int len = strlen(data), i;

  for(i = 0; i < len; ++i)
  {
    if(data[i] == '\n')
      data[i] = 0;
    if(data[i] == '|')
      seqIdParse = 1;
    if(data[i] == '.')
      hasVersion = i;
    if(isalpha(data[i]))
      accession = 1;
  }
  if(seqIdParse)
  {
    SeqIdPtr sip;
    Int4 gi;
    sip = SeqIdParse(data);
    if(sip == NULL)
    {
      ErrPostEx(SEV_ERROR,0,0,"Couldn't parse [%s]", data);
      exit(1);
    }
    gi = ID1ArchGIGet(sip);
    if(gi <= 0)
    {
      SeqIdPrint(sip, data, PRINTID_FASTA_SHORT);
      ErrPostEx(SEV_ERROR, 0, 0, "Couldn't find SeqId [%s]", data);
      exit(1);
    }
    return IdFetch_func(gi, NULL, 0, maxplex);
  }
  else if(hasVersion || accession)
  {
    char acc[100];
    int ver = 0;
    int gi;
    if(hasVersion)
    {
      strncpy(acc, data, hasVersion);
      acc[hasVersion]='\0';
      ver = atoi(data + hasVersion + 1);
    }
    else
      strcpy(acc, data);

    if(ver == 0)
      ver = INT2_MIN;

    if((gi = TryGetGi(SEQID_GENBANK, acc, NULL, ver)) ||
       (gi = TryGetGi(SEQID_OTHER, acc, NULL, ver)) ||
       (gi = TryGetGi(SEQID_GENBANK, NULL, acc, ver)) ||
       (gi = TryGetGi(SEQID_OTHER, NULL, acc, ver)))
      return IdFetch_func(gi,
                          myargs[dbarg].strvalue,
                          myargs[entarg].intvalue,
                          myargs[maxplexarg].intvalue);

    if(ver == INT2_MIN &&
       (
        (gi = TryGetGi(SEQID_PIR, acc, NULL, ver)) ||
        (gi = TryGetGi(SEQID_SWISSPROT, acc, NULL, ver)) ||
        (gi = TryGetGi(SEQID_PIR, NULL, acc, ver)) ||
        (gi = TryGetGi(SEQID_SWISSPROT, NULL, acc, ver))
       )
      )
      return IdFetch_func(gi,
                          myargs[dbarg].strvalue,
                          myargs[entarg].intvalue,
                          myargs[maxplexarg].intvalue);
    return 0;
  }
  else
    return IdFetch_func(atoi(data), NULL, 0, maxplex);
}

static Entrez2ReplyPtr MyEntrezSynchronousQuery(Entrez2RequestPtr e2rq)
{
  int i;
  for(i = 0; i < 3; ++i)
  {
    Entrez2ReplyPtr reply = EntrezSynchronousQuery(e2rq);
    if(reply != NULL)
      return reply;
  }
  return NULL;
}

static Boolean ProcessOneDocSum (Int4 num, Int4Ptr uids)

{
  Entrez2DocsumPtr      dsp;
  Entrez2DocsumListPtr  e2dl;
  Entrez2RequestPtr     e2rq;
  Entrez2ReplyPtr       e2ry;
  Entrez2DocsumDataPtr  e2ddp;
  CharPtr db;
  Boolean result;

  if(num == 0)
    return TRUE;

  db = strcmp(myargs[dbarg].strvalue, "n") == 0 ? "Nucleotide" : "Protein";
  e2rq = EntrezCreateDocSumRequest (db, 0, num, uids, NULL);
  if (e2rq == NULL) return FALSE;

  e2ry = MyEntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree(e2rq);
  e2dl = EntrezExtractDocsumReply (e2ry);

  if (e2dl == NULL)
    return FALSE;

  result = TRUE;
  for (dsp = e2dl->list; dsp != NULL; dsp = dsp->next)
  {
    char *title = "";
    char *extra = "";
    for (e2ddp = dsp->docsum_data; e2ddp != NULL; e2ddp = e2ddp->next) {
      if (StringHasNoText (e2ddp->field_value)) continue;
      if (StringICmp (e2ddp->field_name, "Title") == 0) {
        title = e2ddp->field_value;
      } else if (StringICmp (e2ddp->field_name, "Extra") == 0) {
        extra = e2ddp->field_value;
      }
    }

    fprintf(fp,">%s %s\n", extra, title);

  }

  Entrez2DocsumListFree (e2dl);
  return result;
}

static Boolean IdFetch_func(Int4 gi,CharPtr db, Int4 ent,Int2 maxplex)
{
  SeqEntryPtr	sep=NULL;
  Int4		status,gi_state;
  SeqIdPtr	sip_ret=NULL;
  SeqId		si={SEQID_GI,0,0};
  ID1SeqHistPtr	ishp=NULL;
  Char		buf[200],user_string[100];
  ErrStrId        utag;
  Boolean		retval=TRUE;
  BioseqPtr	bsp=NULL;
  Uint2		entityID;
  Uint1		group_segs;

  if(myargs[outtypearg].intvalue == 7)
  {
    Boolean result = TRUE;
    if(giBufferPos == sizeof(giBuffer) / sizeof(giBuffer[0]))
    {
      result =  ProcessOneDocSum(giBufferPos, giBuffer);
      giBufferPos = 0;
    }
    giBuffer[giBufferPos++] = gi;
    return result;
  }

  sprintf(user_string,"GI=%d|db=%s|ent=%d|",gi,db?db:"NULL",ent);
  utag=ErrUserInstall(user_string,0);

  switch(myargs[infotypearg].intvalue)
  {
  case 0:
    if(maxplex == 1 && myargs[outtypearg].intvalue != 1 && myargs[outtypearg].intvalue != 2)
    {
      si.data.intvalue=gi;
      if((bsp=BioseqLockById(&si)) != NULL)
      {
        sep = ObjMgrGetChoiceForData (bsp);
        switch(myargs[outtypearg].intvalue)
        {
        case 3:
        case 4:
          if(bsp->repr == Seq_repr_seg)
          {
            entityID = ObjMgrGetEntityIDForChoice (sep);
            sep = GetBestTopParentForData (entityID, bsp);
          }
          break;
        case 5:
          if(ISA_na (bsp->mol))
          {
            group_segs = 0;
            if(bsp->repr == Seq_repr_seg)
            {
              group_segs = 1;
            }
            else if(bsp->repr == Seq_repr_delta)
            {
              group_segs = 3;
            }
          }
          else
          {
            group_segs=0;
          }
          break;
        }
      }
    }
    else
    {
      sep = ID1ArchSeqEntryGet (gi,db,ent,&status,maxplex);
    }
    if(!sep)
    {
      switch(status)
      {
      case 1:
        ErrPostEx(SEV_WARNING,0,0,"Sequence has been withdrawn");
        break;
      case 2:
        ErrPostEx(SEV_WARNING,0,0,"Sequence is not yet available");
        break;
      default:
        ErrPostEx(SEV_WARNING,0,0,"Unable to read ASN.1");
        ID1ArchFini();
        ID1ArchInit();
      }
      retval=FALSE;
      goto DONE;
    }
    if(status==3)
      ErrPostEx(SEV_INFO,0,0,"IS_DEAD");
    break;
  case 1:
    gi_state = ID1ArcgGIStateGet(gi);
    break;
  case 2:
    sip_ret = ID1ArchSeqIdsGet(gi,asnout);
    break;
  case 3:
    ishp = ID1ArchGIHistGet(gi,FALSE,asnout);
    break;
  case 4:
    ishp = ID1ArchGIHistGet(gi,TRUE,asnout);
    break;
  }

  if(myargs[infotypearg].intvalue == 1)
  {
    Char	buf[200];
    id_print_gi_state(gi_state,buf,sizeof(buf));
    printf("gi= %d, states: %s\n",gi,buf);
  }
  else
  {
    switch(myargs[outtypearg].intvalue)
    {
    case 1:
    case 2:
      switch(myargs[infotypearg].intvalue)
      {
      case 0:
        SeqEntryAsnWrite(sep, asnout, NULL);
        AsnIoReset(asnout);
        break;
      }
      break;
    case 3:
      if(!SeqEntryToFlat(sep, fp, GENBANK_FMT, RELEASE_MODE)){
        ErrPostEx(SEV_WARNING,0,0,
                  "GenBank Format does not exist for this sequence ");
        retval=FALSE;
        goto DONE;
      }
      break;
    case 4:
      if(!SeqEntryToFlat(sep, fp, GENPEPT_FMT, RELEASE_MODE))
      {
        ErrPostEx(SEV_WARNING,0,0,
                  "GenPept Format does not exist for this sequence");
        retval=FALSE;
        goto DONE;
      }
      break;
    case 6:
      SeqEntryExplore (sep, (Pointer) fp, PrintQualScores);
      break;
    case 5:
      switch(myargs[infotypearg].intvalue){
      case 0:
        if(bsp){
          SeqEntrysToFasta (sep, fp, ISA_na (bsp->mol), group_segs);
        }
        else
        {
          SeqEntryToFasta(sep, fp, TRUE);  /* nuc acids */
          SeqEntryToFasta(sep, fp, FALSE); /* proteins */
        }
        break;
      case 2:
        SeqIdWrite(sip_ret, buf, PRINTID_FASTA_LONG, sizeof(buf) - 1);
        fprintf(fp, "%s\n", buf);
        break;

      case 3:
      case 4:
        SeqHistPrintTable(ishp,fp);
        break;
      }
      break;
    }
  }
DONE:
  if(bsp)
  {
    static Uint2  reap_cnt;
    BioseqUnlock(bsp);
    reap_cnt++;
    if(reap_cnt > 128){
      ObjMgrFreeCache(0);
      reap_cnt=0;
    }
  }
  else if(sep)
  {
    SeqEntryFree(sep);
  }
  if(sip_ret)	SeqIdFree(sip_ret);
  if(ishp)	ID1SeqHistFree(ishp);
  ErrUserDelete(utag);
  return retval;
}
/*****************************************************************************
 *
 *   Nlm_WhichArg(ap)
 *     returns array position for a tag
 *
 *****************************************************************************/
static Nlm_Int2 Nlm_WhichArg( Nlm_Char which, Nlm_Int2 numargs, Nlm_ArgPtr ap)
{
  Nlm_Boolean okay = FALSE;
  Nlm_Int2 i;
  Nlm_ArgPtr curarg;
  Nlm_Int2 retval = -1;

  if((ap == NULL) || (numargs == 0) )
    return okay;

  curarg = ap;                        /* set defaults */

  for(i = 0; i < numargs; i++, curarg++)
  {
    if(curarg->tag == which)
    {
      retval = i;
      break;
    }
  }

  return retval;
}

/* This function is interface to the Entrez2 engine. It may be used
   to get list of gis corresponding to the Entrez Boolean string or
   just number of such hits in the Entrez database */

static Int4 BEGetUidsFromQuery(CharPtr query, Uint4Ptr PNTR uids, 
                               Boolean is_na, Boolean count_only)
{
  Entrez2ReplyPtr e2ry;
  Entrez2RequestPtr  e2rq;
  E2ReplyPtr e2rp;
  Int4 count = 0;
  Entrez2BooleanReplyPtr e2br;
  Entrez2IdListPtr e2idlist;

  *uids = NULL;

  e2rq = EntrezCreateBooleanRequest(!count_only, FALSE,
                                    is_na? "Nucleotide" : "Protein",
                                    query, 0, 0, NULL, 0, 0);

  e2ry = MyEntrezSynchronousQuery(e2rq);

  if(e2ry == NULL)
  {
    ErrPostEx(SEV_ERROR, 0, 0, "NULL returned from EntrezSynchronousQuery()");
    return -1;
  }

  if((e2rp = e2ry->reply) == NULL)
  {
    ErrPostEx(SEV_ERROR, 0, 0, "Invalid ASN.1: E2ReplyPtr==NULL");
    return -1;
  }

  switch(e2rp->choice)
  {

  case E2Reply_error:
    ErrPostEx(SEV_ERROR, 0, 0, (CharPtr) e2rp->data.ptrvalue);
    count = -1;
    break;
  case E2Reply_eval_boolean:
    e2br = (Entrez2BooleanReplyPtr) e2rp->data.ptrvalue;
    count = e2br->count;
    if((e2idlist = e2br->uids) != NULL) {
      count = e2idlist->num;
      *uids = MemNew(sizeof(Int4)*count);
      BSSeek((ByteStorePtr) e2idlist->uids, 0, SEEK_SET);
      BSRead((ByteStorePtr) e2idlist->uids, *uids, sizeof(Int4)*count);

    }
    break;
  default:
    ErrPostEx(SEV_ERROR, 0, 0, "Invalid reply type from the server: %d", e2rp->choice);
    count = -1;
    break;
  }
  Entrez2ReplyFree(e2ry);
  Entrez2RequestFree(e2rq);
  return count;
}

