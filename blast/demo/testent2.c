#include <ncbi.h>
#include <asn.h>
#include <objall.h>
#include <objsub.h>
#include <objfdef.h>
#include <sqnutils.h>
#include <ent2api.h>
#include <urlquery.h>

static void SaveEntrezRequest (Entrez2RequestPtr e2rq)

{
  AsnIoPtr  aip;

  if (e2rq == NULL) return;
  aip = AsnIoOpen ("stdout", "w");
  if (aip == NULL) return;
  Entrez2RequestAsnWrite (e2rq, aip, NULL);
  AsnIoFlush (aip);
  AsnIoClose (aip);
}

static void SaveEntrezReply (Entrez2ReplyPtr e2ry)

{
  AsnIoPtr  aip;

  if (e2ry == NULL) {
    printf ("Entrez2ReplyPtr is NULL\n");
    return;
  }
  aip = AsnIoOpen ("stdout", "w");
  if (aip == NULL) return;
  Entrez2ReplyAsnWrite (e2ry, aip, NULL);
  AsnIoFlush (aip);
  AsnIoClose (aip);
}

static void SaveBooleanIdList (Entrez2ReplyPtr e2ry)

{
  Entrez2BooleanReplyPtr  e2br;
  Entrez2IdListPtr        e2lp;
  Int4                    i;
  E2ReplyPtr              reply;
  Uint4                   uid;

  if (e2ry == NULL) return;
  reply = e2ry->reply;
  if (reply == NULL || reply->choice != E2Reply_eval_boolean) return;
  e2br = EntrezExtractBooleanReply (e2ry);
  if (e2br == NULL) return;

  printf ("Boolean reply - count is %ld\n", (long) e2br->count);
  e2lp = e2br->uids;
  if (e2lp != NULL) {
    BSSeek (e2lp->uids, 0, SEEK_SET);
    for (i = 0; i < e2lp->num; i++) {
      uid = Nlm_BSGetUint4 (e2lp->uids);
      printf ("%ld\n", (long) uid);
    }
  }
  printf ("\n");

  Entrez2BooleanReplyFree (e2br);
}

static Int4 pmuidlist [] = {
  3038955,
  2539356
};

static Int4 nucuidlist [] = {
  14993658,
  14971124
};

static Int4 protuidlist [] = {
  131784,
  172345
};

static Int4 taxuidlist [] = {
  40674
};

static void TestE2 (Boolean dohuge, Boolean showuids, Boolean showinfo)

{
  CharPtr              cookie;
  Int4                 count;
  Entrez2DbInfoPtr     e2db;
  Entrez2FieldInfoPtr  e2fd;
  Entrez2InfoPtr       e2ip;
  Entrez2RequestPtr    e2rq;
  Entrez2ReplyPtr      e2ry;
  ValNodePtr           head = NULL;
  CharPtr              key1 = NULL;
  CharPtr              key2 = NULL;
  CharPtr              key3 = NULL;
  Int4                 orgtermcount = -1;
  Int4                 pos = -1;
  E2ReplyPtr           reply;
  CharPtr              str;
  ValNodePtr           vnp;

  e2rq = EntrezCreateGetInfoRequest ();
  SaveEntrezRequest (e2rq);
  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (e2ry != NULL) {
    cookie = StringSave (e2ry->cookie);
    reply = e2ry->reply;
    if (showinfo) {
      SaveEntrezReply (e2ry);
    }
    if (reply != NULL && reply->choice == E2Reply_get_info) {
      e2ip = EntrezExtractInfoReply (e2ry);
      if (ValidateEntrez2InfoPtr (e2ip, &head)) {
        printf ("GetInfo validation succeeded\n");
      } else {
        printf ("GetInfo validation failed\n");
        for (vnp = head; vnp != NULL; vnp = vnp->next) {
          str = (CharPtr) vnp->data.ptrvalue;
          if (str == NULL) continue;
          printf ("%s\n", str);
        }
        ValNodeFreeData (head);
      }
      if (e2ip != NULL) {
        for (e2db = e2ip->db_info; e2db != NULL; e2db = e2db->next) {
          if (StringICmp (e2db->db_name, "nucleotide") == 0) {
            for (e2fd = e2db->fields; e2fd != NULL; e2fd = e2fd->next) {
              if (StringICmp (e2fd->field_name, "ORGN") == 0) {
                orgtermcount = e2fd->term_count;
                printf ("Nucleotide ORGN term count is %ld\n", orgtermcount);
              }
            }
          }
        }
      }
      Entrez2InfoFree (e2ip);
    } else {
      if (reply != NULL) {
        printf ("GetInfo request returned reply choice %d\n", (int) reply->choice);
        SaveEntrezReply (e2ry);
      } else {
        printf ("GetInfo request returned NULL reply\n");
      }
      Entrez2ReplyFree (e2ry);
    }
    if (cookie == NULL) {
      cookie = "(null)";
    }
    printf ("initial returned cookie:\n%s\n", cookie);
  } else {
    printf ("GetInfo request failed\n");
  }

  e2rq = EntrezCreateBooleanRequest (FALSE, FALSE, "taxonomy", NULL, 0, 0, NULL, 0, 0);
  EntrezAddToBooleanRequest (e2rq, NULL, 0, NULL, NULL, NULL, 0, 1, taxuidlist, NULL, FALSE, FALSE);
  SaveEntrezRequest (e2rq);
  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (e2ry != NULL) {
    key3 = StringSave (e2ry->key);
    SaveEntrezReply (e2ry);
    if (showuids) {
      SaveBooleanIdList (e2ry); /* also frees e2ry */
    } else {
      Entrez2ReplyFree (e2ry);
    }
  } else {
    printf ("Boolean request failed\n");
  }

  e2rq = EntrezCreateBooleanRequest (FALSE, TRUE, "PubMed", "ras mutation binding", 0, 0, NULL, 0, 0);
  SaveEntrezRequest (e2rq);
  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (e2ry != NULL) {
    key1 = StringSave (e2ry->key);
    SaveEntrezReply (e2ry);
    if (showuids) {
      SaveBooleanIdList (e2ry); /* also frees e2ry */
    } else {
      Entrez2ReplyFree (e2ry);
    }
  } else {
    printf ("Boolean request failed\n");
  }

  e2rq = EntrezCreateGetTermPositionRequest ("nucleotide", "ORGN", "Zea");
  SaveEntrezRequest (e2rq);
  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (e2ry != NULL) {
    SaveEntrezReply (e2ry);
    pos = EntrezExtractTermPosReply (e2ry);
    printf ("Zea position is %ld\n", pos);
    Entrez2ReplyFree (e2ry);
  } else {
    printf ("GetTermPosition request failed\n");
  }

  if (pos >= 0 && orgtermcount >= 0) {
    count = 20;
    if (pos + count >= orgtermcount) {
      count = orgtermcount - pos - 1;
    }
    e2rq = EntrezCreateGetTermListRequest ("nucleotide", "ORGN", pos, count);
    SaveEntrezRequest (e2rq);
    e2ry = EntrezSynchronousQuery (e2rq);
    e2rq = Entrez2RequestFree (e2rq);
    if (e2ry != NULL) {
      SaveEntrezReply (e2ry);
      Entrez2ReplyFree (e2ry);
    } else {
      printf ("GetTermList request failed\n");
    }
  }

  e2rq = EntrezCreateDocSumRequest ("nucleotide", 1322283, 0, NULL, NULL);
  SaveEntrezRequest (e2rq);
  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (e2ry != NULL) {
    SaveEntrezReply (e2ry);
    Entrez2ReplyFree (e2ry);
  } else {
    printf ("DocSum request failed\n");
  }

  e2rq = EntrezCreateDocSumRequest ("Nucleotide", 0, 2, nucuidlist, NULL);
  SaveEntrezRequest (e2rq);
  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (e2ry != NULL) {
    SaveEntrezReply (e2ry);
    Entrez2ReplyFree (e2ry);
  } else {
    printf ("DocSum request failed\n");
  }

  e2rq = EntrezCreateGetLinksRequest ("Protein", 0, 1, protuidlist, NULL, "protein_protein", 30000, FALSE, TRUE);
  SaveEntrezRequest (e2rq);
  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (e2ry != NULL) {
    SaveEntrezReply (e2ry);
    Entrez2ReplyFree (e2ry);
  } else {
    printf ("GetLinks request failed\n");
  }

  e2rq = EntrezCreateGetLinksRequest ("Protein", 0, 2, protuidlist, NULL, "protein_protein", 30000, FALSE, TRUE);
  SaveEntrezRequest (e2rq);
  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (e2ry != NULL) {
    SaveEntrezReply (e2ry);
    Entrez2ReplyFree (e2ry);
  } else {
    printf ("GetLinks request failed\n");
  }

  str = "((\"Aedes albopictus\"[Organism] AND ((1900[MDAT] : 3000[MDAT]) NOT patent[PROP])) AND gene_in_genomic[PROP])";
  e2rq = EntrezCreateBooleanRequest (TRUE, TRUE, "nucleotide", str, 0, 0, NULL, 0, 0);
  SaveEntrezRequest (e2rq);
  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (e2ry != NULL) {
    SaveEntrezReply (e2ry);
    if (showuids) {
      SaveBooleanIdList (e2ry); /* also frees e2ry */
    } else {
      Entrez2ReplyFree (e2ry);
    }
  } else {
    printf ("Boolean request failed\n");
  }

  e2rq = EntrezCreateBooleanRequest (TRUE, TRUE, "PubMed", "Kans JA [AUTH]", 0, 0, NULL, 0, 0);
  SaveEntrezRequest (e2rq);
  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (e2ry != NULL) {
    key1 = StringSave (e2ry->key);
    SaveEntrezReply (e2ry);
    if (showuids) {
      SaveBooleanIdList (e2ry); /* also frees e2ry */
    } else {
      Entrez2ReplyFree (e2ry);
    }
  } else {
    printf ("Boolean request failed\n");
  }

  e2rq = EntrezCreateBooleanRequest (TRUE, TRUE, "PubMed", "Casadaban MJ [AUTH]", 0, 0, NULL, 0, 0);
  SaveEntrezRequest (e2rq);
  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (e2ry != NULL) {
    key2 = StringSave (e2ry->key);
    SaveEntrezReply (e2ry);
    if (showuids) {
      SaveBooleanIdList (e2ry); /* also frees e2ry */
    } else {
      Entrez2ReplyFree (e2ry);
    }
  } else {
    printf ("Boolean request failed\n");
  }

  e2rq = EntrezCreateBooleanRequest (TRUE, TRUE, "PubMed", NULL, 0, 0, NULL, 0, 0);
  EntrezAddToBooleanRequest (e2rq, NULL, 0, NULL, NULL, key1, 0, 0, NULL, NULL, FALSE, FALSE);
  EntrezAddToBooleanRequest (e2rq, NULL, ENTREZ_OP_AND, NULL, NULL, NULL, 0, 0, NULL, NULL, FALSE, FALSE);
  EntrezAddToBooleanRequest (e2rq, NULL, 0, NULL, NULL, key2, 0, 0, NULL, NULL, FALSE, FALSE);
  SaveEntrezRequest (e2rq);
  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (e2ry != NULL) {
    SaveEntrezReply (e2ry);
    if (showuids) {
      SaveBooleanIdList (e2ry); /* also frees e2ry */
    } else {
      Entrez2ReplyFree (e2ry);
    }
  } else {
    printf ("Boolean request failed\n");
  }

  e2rq = EntrezCreateBooleanRequest (TRUE, TRUE, "PubMed", NULL, 0, 0, NULL, 0, 0);
  EntrezAddToBooleanRequest (e2rq, NULL, 0, NULL, NULL, NULL, 0, 2, pmuidlist, NULL, FALSE, FALSE);
  SaveEntrezRequest (e2rq);
  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (e2ry != NULL) {
    key3 = StringSave (e2ry->key);
    SaveEntrezReply (e2ry);
    if (showuids) {
      SaveBooleanIdList (e2ry); /* also frees e2ry */
    } else {
      Entrez2ReplyFree (e2ry);
    }
  } else {
    printf ("Boolean request failed\n");
  }

  e2rq = EntrezCreateBooleanRequest (TRUE, TRUE, "PubMed", NULL, 0, 0, NULL, 0, 0);
  EntrezAddToBooleanRequest (e2rq, NULL, 0, "AUTH", "Kans JA", NULL, 0, 0, NULL, NULL, FALSE, FALSE);
  EntrezAddToBooleanRequest (e2rq, NULL, ENTREZ_OP_AND, NULL, NULL, NULL, 0, 0, NULL, NULL, FALSE, FALSE);
  EntrezAddToBooleanRequest (e2rq, NULL, 0, NULL, NULL, key3, 0, 0, NULL, NULL, FALSE, FALSE);
  SaveEntrezRequest (e2rq);
  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (e2ry != NULL) {
    SaveEntrezReply (e2ry);
    if (showuids) {
      SaveBooleanIdList (e2ry); /* also frees e2ry */
    } else {
      Entrez2ReplyFree (e2ry);
    }
  } else {
    printf ("Boolean request failed\n");
  }

  e2rq = EntrezCreateBooleanRequest (TRUE, TRUE, "PubMed", NULL, 0, 0, NULL, 20, 0);
  EntrezAddToBooleanRequest (e2rq, NULL, 0, "AUTH", "Kans JA", NULL, 0, 0, NULL, NULL, FALSE, FALSE);
  SaveEntrezRequest (e2rq);
  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (e2ry != NULL) {
    SaveEntrezReply (e2ry);
    if (showuids) {
      SaveBooleanIdList (e2ry); /* also frees e2ry */
    } else {
      Entrez2ReplyFree (e2ry);
    }
  } else {
    printf ("Boolean request failed\n");
  }

  /*
  e2rq = EntrezCreateBooleanRequest (TRUE, TRUE, "PubMed", NULL, 0, 0, NULL, 0, 0);
  EntrezAddToBooleanRequest (e2rq, NULL, ENTREZ_OP_LEFT_PAREN, NULL, NULL, NULL, 0, 0, NULL, NULL, FALSE, FALSE);
  EntrezAddToBooleanRequest (e2rq, NULL, 0, "WORD", "immunit", NULL, 0, 0, NULL, NULL, FALSE, FALSE);
  EntrezAddToBooleanRequest (e2rq, NULL, ENTREZ_OP_RANGE, NULL, NULL, NULL, 0, 0, NULL, NULL, FALSE, FALSE);
  EntrezAddToBooleanRequest (e2rq, NULL, 0, "WORD", "immunitarian", NULL, 0, 0, NULL, NULL, FALSE, FALSE);
  EntrezAddToBooleanRequest (e2rq, NULL, ENTREZ_OP_RIGHT_PAREN, NULL, NULL, NULL, 0, 0, NULL, NULL, FALSE, FALSE);
  */
  /*
  e2rq = EntrezCreateBooleanRequest (TRUE, TRUE, "PubMed", "transposase* [WORD]", 0, 0, NULL, 0, 0);
  */
  e2rq = EntrezCreateBooleanRequest (TRUE, TRUE, "PubMed", NULL, 0, 0, NULL, 0, 0);
  EntrezAddToBooleanRequest (e2rq, NULL, 0, "WORD", "transposase*", NULL, 0, 0, NULL, NULL, FALSE, FALSE);
  SaveEntrezRequest (e2rq);
  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (e2ry != NULL) {
    SaveEntrezReply (e2ry);
    if (showuids) {
      SaveBooleanIdList (e2ry); /* also frees e2ry */
    } else {
      Entrez2ReplyFree (e2ry);
    }
  } else {
    printf ("Boolean request failed\n");
  }

  if (dohuge) {
    str = "\"nucleotide all\"[filter] NOT ApoE";
    e2rq = EntrezCreateBooleanRequest (TRUE, TRUE, "nucleotide", str, 0, 0, NULL, 0, 0);
    SaveEntrezRequest (e2rq);
    e2ry = EntrezSynchronousQuery (e2rq);
    e2rq = Entrez2RequestFree (e2rq);
    if (e2ry != NULL) {
      SaveEntrezReply (e2ry);
      if (showuids) {
        SaveBooleanIdList (e2ry); /* also frees e2ry */
      } else {
        Entrez2ReplyFree (e2ry);
      }
    } else {
      printf ("Boolean request failed\n");
    }
  }
}

/* Args structure contains command-line arguments */

#define v_argService 0
#define s_argServer  1
#define r_argPort    2
#define p_argPath    3
#define h_argHuge    4
#define u_argUids    5
#define g_argInfo    6

Args myargs [] = {
  {"Service", NULL, NULL, NULL,
    TRUE, 'v', ARG_STRING, 0.0, 0, NULL},
  {"Server", "www.ncbi.nlm.nih.gov", NULL, NULL,
    TRUE, 's', ARG_STRING, 0.0, 0, NULL},
  {"Port", "80", "0", "7777",
    TRUE, 'r', ARG_INT, 0.0, 0, NULL},
  {"Path", "/entrez/utils/entrez2server.fcgi", NULL, NULL,
    TRUE, 'p', ARG_STRING, 0.0, 0, NULL},
  {"Do Huge Boolean Query", "F", NULL, NULL,
    TRUE, 'h', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Show UID List", "F", NULL, NULL,
    TRUE, 'u', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Show Get-Info Reply", "F", NULL, NULL,
    TRUE, 'g', ARG_BOOLEAN, 0.0, 0, NULL},
};

Int2 Main (void)

{
  Boolean  dohuge, showinfo, showuids;
  CharPtr  server, service, path;
  Uint2    port;

  /* standard setup */

  ErrSetFatalLevel (SEV_MAX);
  ErrClearOptFlags (EO_SHOW_USERSTR);
  UseLocalAsnloadDataAndErrMsg ();
  ErrPathReset ();

  if (! AllObjLoad ()) {
    Message (MSG_FATAL, "AllObjLoad failed");
    return 1;
  }
  if (! SubmitAsnLoad ()) {
    Message (MSG_FATAL, "SubmitAsnLoad failed");
    return 1;
  }
  if (! FeatDefSetLoad ()) {
    Message (MSG_FATAL, "FeatDefSetLoad failed");
    return 1;
  }
  if (! SeqCodeSetLoad ()) {
    Message (MSG_FATAL, "SeqCodeSetLoad failed");
    return 1;
  }
  if (! GeneticCodeTableLoad ()) {
    Message (MSG_FATAL, "GeneticCodeTableLoad failed");
    return 1;
  }

  /* process command line arguments */

  if (! GetArgs ("testent2", sizeof (myargs) / sizeof (Args), myargs)) {
    return 0;
  }

  service = myargs [v_argService].strvalue;
  server = myargs [s_argServer].strvalue;
  port = (Uint2) myargs [r_argPort].intvalue;
  path = myargs [p_argPath].strvalue;
  dohuge = (Boolean) myargs [h_argHuge].intvalue;
  showuids = (Boolean) myargs [u_argUids].intvalue;
  showinfo = (Boolean) myargs [g_argInfo].intvalue;

  if (! StringHasNoText (service)) {
    EntrezSetService (service);
    printf ("testent2 %s\n", service);
  } else {
    EntrezSetServer (server, port, path);
    printf ("testent2 %s:%d%s\n", server, (int) port, path);
  }

  TestE2 (dohuge, showuids, showinfo);
  printf ("testent2 finished\n");

  return 0;
}


