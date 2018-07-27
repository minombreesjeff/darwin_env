/*****************************************************************************
*
*   dosimple.c
*       test program for simple sequence (fasta format)
*
*****************************************************************************/
#include <simple.h>

Int2 Main(void)
{
	AsnIoPtr aip;
    SimpleSeqPtr ssp;
    FILE * fp;

	if ((aip = AsnIoOpen("bioseq.ent", "r")) == NULL)
		return 1;

    ssp = SimpleSeqAsnRead(aip);

	aip = AsnIoClose(aip);

    fp = FileOpen("simple.out", "w");

    fprintf(fp, "fasta format:\n\n");
    SimpleSeqPrint(ssp, fp, TRUE);

    fprintf(fp, "\n\nreport format:\n\n");
    SimpleSeqPrint(ssp, fp, FALSE);

    FileClose(fp);

	return 0;
}

