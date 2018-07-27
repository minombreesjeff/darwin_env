
/*****************************************************************************
*
*   testobj.c
*       test program for object loaders
*
*****************************************************************************/
#include <objmedli.h>
#include <objsset.h>

Int2 Main(void)
{
	AsnIoPtr aip, aipout, aipnull;
    MedlineEntryPtr mep;
    BioseqSetPtr bsp;
	DatePtr tmp;

    if (! MedlineAsnLoad())
    {
        Message(MSG_ERROR, "MedlineLoad failed");
        return 1;
    }

	if ((aip = AsnIoOpen("medline.ent", "r")) == NULL)
		return 1;

    mep = MedlineEntryAsnRead(aip, NULL);
	if (mep == NULL) ErrShow();

      /**** this is a demonstration of error checking using a null device
		    before actually writing the object **************************/

	aipnull = AsnIoNullOpen();     /* open a NULL device for testing */
	aipout = AsnIoOpen("medline.out", "w");   /* open a real output stream */

	ErrSetOpts(ERR_CONTINUE, 0);   /* set to continue on an error */

	tmp = mep->em;           /* Entry Month is required for Medline Entry */
	mep->em = NULL;          /* so we create an error */
							 
	                         /* this will fail, and ErrPost() */
	if (! MedlineEntryAsnWrite(mep, aipnull, NULL))
		ErrShow();

	mep->em = tmp;           /* put it back */
	AsnIoReset(aipnull);     /* reset the null device */

							 /* now check it again */
							 /* normally this is all the code you need */

    if (! MedlineEntryAsnWrite(mep, aipnull, NULL))     /* if not ok  */
		ErrShow();               						/*    show error */
	else if (! MedlineEntryAsnWrite(mep, aipout, NULL))	/* else write it */
		ErrShow();										/*  with fail-safe */

							 /* that's all  */

    MedlineEntryFree(mep);

	aip = AsnIoClose(aip);
    aipout = AsnIoClose(aipout);

    if (! SeqSetAsnLoad())
    {
        Message(MSG_ERROR, "SeqSetLoad failed");
        return 1;
    }

	if ((aip = AsnIoOpen("bioseq.set", "r")) == NULL)
		return 1;

	aipout = AsnIoOpen("seqset.out", "w");
    bsp = BioseqSetAsnRead(aip, NULL);
    BioseqSetAsnWrite(bsp, aipout, NULL);
    BioseqSetFree(bsp);
	aip = AsnIoClose(aip);
    aipout = AsnIoClose(aipout);

	return 0;
}

