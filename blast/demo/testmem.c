/*****************************************************************************
*
*   testmem.c
*   	test transforms of asn.1 to and from memory buffer
*
*****************************************************************************/
#include <objsset.h>
#include <sequtil.h>

#define NUMARG 3
Args myargs[NUMARG] = {
	{"Filename for asn.1 input","bbone.prt",NULL,NULL,TRUE,'a',ARG_FILE_IN,0.0,0,NULL},
	{"Input asnfile in binary mode","F",NULL,NULL,TRUE,'b',ARG_BOOLEAN,0.0,0,NULL},
	{"Output Filename","stdout", NULL,NULL,TRUE,'f',ARG_FILE_OUT,0.0,0,NULL}};

Int2 Main(void)
{
	AsnIoPtr aip;
	SeqEntryPtr sep;
	BytePtr buf;
	Uint2 size = 32000, count;
	AsnIoMemPtr aimp;

					/* check command line arguments */

	if ( ! GetArgs("testmem",NUMARG, myargs))
		return 1;

					/* load the sequence alphabets  */
					/* (and sequence parse trees)   */
	if (! SeqEntryLoad())
		ErrShow();

					/* open the ASN.1 input file in the right mode */

	if ((aip = AsnIoOpen (myargs[0].strvalue, myargs[1].intvalue?"rb":"r"))
          == NULL)
		ErrShow();
	sep = SeqEntryAsnRead(aip, NULL);   	/* read the entry */
	AsnIoClose(aip);

	SeqEntryPack(sep);						/* pack it */

	buf = MemNew(size);                     /* allocate a buffer */
	aimp = AsnIoMemOpen("wb", buf, size);	/* open to write asn1 to it */
	SeqEntryAsnWrite(sep, aimp->aip, NULL);	/* write it */
	AsnIoFlush(aimp->aip);					/* flush it */
	count = aimp->count;					/* record how many bytes in it */
	AsnIoMemClose(aimp);					/* close it */

	SeqEntryFree(sep);						/* release the object */

	aimp = AsnIoMemOpen("rb", buf, count);	/* open to read from buffer */
	sep = SeqEntryAsnRead(aimp->aip, NULL);	/* read it */
	AsnIoMemClose(aimp);					/* close it */
	MemFree(buf);							/* free the buffer */

				  				/* open the output file */

	aip = AsnIoOpen (myargs[2].strvalue, "w");
	SeqEntryAsnWrite(sep, aip, NULL);	/* print it */
	AsnIoClose(aip);

	SeqEntryFree(sep);

	return(0);
}

