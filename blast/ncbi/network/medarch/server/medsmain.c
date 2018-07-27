/*
*
*
* RCS Modification History:
* $Log: medsmain.c,v $
* Revision 6.0  1997/08/25 18:36:38  madden
* Revision changed to 6.0
*
* Revision 1.5  1996/03/01 16:08:41  grisha
* add default clause to request switch, now we will
* return error to the client if request code cannot
* be proceed by server
*
 * Revision 1.4  1995/12/07  17:22:08  tatiana
 * bulletproofing in main()
 *
 * Revision 1.3  1995/05/30  18:01:23  tatiana
 * *** empty log message ***
 *
 * Revision 1.2  1995/05/17  17:55:17  epstein
 * add RCS log revision history
 *
*/

#include <ncbi.h>
#include <objmedli.h>
#include <objpub.h>
#include <ncbinet.h>
#include <accentr.h>
#include "mappings.h"
#include "objmla.h"

FILE * Id_timing_file = NULL;

extern MedlineEntryPtr SybaseMedlineEntryGet( Int4 ui );
extern CitArtPtr SybaseMedlinePubGet( Int4 ui );
extern TitleMsgListPtr SybaseMedlineGetTitle( TitleMsgPtr tmsg );

main(int argc, char *argv[])
{
    MlaRequestPtr mlarp;
    MlaBackPtr mlabp;
    Boolean debug = FALSE;
    static char envset[PATH_MAX+5];
    extern char *getenv();
    short erract;
    ErrDesc err;
    TitleMsgPtr tmsgp;
    ValNodePtr pub;
    TitleMsgListPtr tmlp;
    int arg;
    NI_HandPtr hp;
    static Char buf[100];
    AsnIoPtr asnin;
    AsnIoPtr asnout;
    int read_timeout;
    Boolean done = FALSE;
    MedlineEntryPtr meddata;
    CitArtPtr pubdata;
    ValNodePtr pbp;
    int n, ui;
    ValNodePtr head;

    if (argc > 1) {
        arg = 1;
        if (StrCmp(argv[1], "-d") == 0) {
            arg++;
            debug = TRUE;
        }
    }

    if (! InitMedlineDB() ) {
        if (!debug)
            NI_ServerNACK("Medline service: Unable to initialize Sybase");
		return( -1 );
    }


    if (!debug) {

        NI_ServerACK();

        hp = NI_OpenASNIO();

        /* this read-timeout is effectively an idle timeout for */
        /* the server process; the process will terminate upon  */
        /* read-timeout                                         */
        GetAppParam("NCBI", "NET_SERV", "SERV_INACT_TIMER", "10",
                    buf, sizeof buf);
        read_timeout = atoi(buf) * 60; /* param is minutes */
        MsgSetReadTimeout(hp, read_timeout);

        asnin = hp->raip;
        asnout = hp->waip;
    } else {
    	if (argc > 2) {
    		asnin = AsnIoOpen(argv[2], "r");
    	} else {
        	asnin = AsnIoOpen("medserv.inp", "r");
        }
        asnout = AsnIoOpen("medserv.out", "w");
        Id_timing_file = FileOpen("medserv.tmt", "w");
    }

    while (!done) {
        /* encountering EOF on reading is a "normal" occurrence, */
        /* and does not merit an error message                   */
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err); /* clear any pending error, which can be ignored */

        mlarp = MlaRequestAsnRead(asnin, NULL);
        if (ErrFetch(&err))
        {
            done = TRUE;
            Message(MSG_POST, "Error encountered on AsnReadId %d", err);
            break; /* client terminated */
        }
        ErrSetOpts(erract, 0);

        if (mlarp == NULL) {
            done = TRUE;
            Message(MSG_POST, "Null AsnReadId");
            break; /* client terminated */
        }

        switch (mlarp->choice) {
        case MlaRequest_init:
            mlabp = ValNodeNew(NULL);
            mlabp->choice = MlaBack_init;
            mlabp->data.ptrvalue = NULL;
            MlaBackAsnWrite (mlabp, asnout, NULL);
            MlaBackFree (mlabp);
	    	mlarp->data.ptrvalue = NULL;
            break;

        case MlaRequest_getmle:

            meddata = SybaseMedlineEntryGet((Int4) mlarp->data.intvalue);
            mlabp = ValNodeNew(NULL);
            if (meddata == NULL) {
                mlabp->choice = MlaBack_error;
                mlabp->data.intvalue = 0;
            } else {
                mlabp->choice = MlaBack_getmle;
                mlabp->data.ptrvalue = (Pointer) meddata;
            }

            MlaBackAsnWrite (mlabp, asnout, NULL);

	    if (meddata != NULL) mlabp->data.ptrvalue = NULL;

            MlaBackFree (mlabp);
            break;

        case MlaRequest_getpub:

            pubdata = SybaseMedlinePubGet((Int4) mlarp->data.intvalue);
            mlabp = ValNodeNew(NULL);
            if (pubdata == NULL) {
                mlabp->choice = MlaBack_error;
                mlabp->data.intvalue = 0;
                MlaBackAsnWrite (mlabp, asnout, NULL);
            } else {
            	mlabp->choice = MlaBack_getpub;
            	mlabp->data.ptrvalue = ValNodeNew(NULL);
            	pub = (ValNodePtr) mlabp->data.ptrvalue;
            	pub->choice = 5; /* article */
            	pub->data.ptrvalue = (Pointer) pubdata;
                MlaBackAsnWrite (mlabp, asnout, NULL);
            	pubdata = NULL; /* for clean free */
				pub->data.ptrvalue = NULL;
	    	}
            MlaBackFree (mlabp);
            break;

        case MlaRequest_gettitle:
            tmsgp = (TitleMsgPtr) mlarp->data.ptrvalue;
	    	mlarp->data.ptrvalue = NULL;
            mlabp = ValNodeNew(NULL);
	    	tmlp = SybaseMedlineGetTitle( tmsgp );

            if (tmlp == NULL) {
                mlabp->choice = MlaBack_error;
                mlabp->data.intvalue = 0;
            } else {
                mlabp->choice = MlaBack_gettitle;
                mlabp->data.ptrvalue = (Pointer) tmlp;
   	    	}

            MlaBackAsnWrite (mlabp, asnout, NULL);

	    	if (tmlp != NULL) mlabp->data.ptrvalue = NULL;
	    	if (tmlp != NULL && tmlp->titles->type == 0) tmlp->titles = NULL;

            MlaBackFree (mlabp);
	    	TitleMsgFree( tmsgp );
	    	TitleMsgListFree( tmlp );
            break;

        case MlaRequest_citmatch:
            mlabp = ValNodeNew(NULL);
	    	pub = ( ValNodePtr )mlarp->data.ptrvalue;
	    	if (pub == NULL) {
                mlabp->choice = MlaBack_error;
                mlabp->data.intvalue = 0;
	    	} else {
	    		ui = SybaseMedlineCitMatch( (CitArtPtr)pub->data.ptrvalue );

            	if ( ui == 0 ) {
                	mlabp->choice = MlaBack_error;
                	mlabp->data.intvalue = 0;
            	} else {
            		mlabp->choice = MlaBack_citmatch;
            		mlabp->data.intvalue = ui;
	    		}
	    	}

            MlaBackAsnWrite (mlabp, asnout, NULL);
            MlaBackFree (mlabp);
            break;

        case MlaRequest_fini:
            done = TRUE;
            mlabp = ValNodeNew(NULL);
            mlabp->choice = MlaBack_fini;
            mlabp->data.ptrvalue = NULL;

            MlaBackAsnWrite (mlabp, asnout, NULL);

            MlaBackFree (mlabp);
	    mlarp->data.ptrvalue = NULL;
            break;
        default:                    /* undefined request code */
            if ( (mlabp=ValNodeNew(NULL)) != NULL ) { /* create node */
                mlabp->choice = MlaBack_error;        /* set error back */
                mlabp->data.intvalue = 0;             /* clear value */
                MlaBackAsnWrite (mlabp, asnout, NULL);/* send to client */
                MlaBackFree (mlabp);                  /* free memory */
            }
            break;
        }

        AsnIoReset (asnout);
        MlaRequestFree (mlarp);
    }

    CloseMedlineDB();

    AsnIoClose (asnin);
    AsnIoClose (asnout);
}
