/**************************************************************************
*                                                                         *
*                             COPYRIGHT NOTICE                            *
*                                                                         *
* This software/database is categorized as "United States Government      *
* Work" under the terms of the United States Copyright Act.  It was       *
* produced as part of the author's official duties as a Government        *
* employee and thus can not be copyrighted.  This software/database is    *
* freely available to the public for use without a copyright notice.      *
* Restrictions can not be placed on its present or future use.            *
*                                                                         *
* Although all reasonable efforts have been taken to ensure the accuracy  *
* and reliability of the software and data, the National Library of       *
* Medicine (NLM) and the U.S. Government do not and can not warrant the   *
* performance or results that may be obtained by using this software,     *
* data, or derivative works thereof.  The NLM and the U.S. Government     *
* disclaim any and all warranties, expressed or implied, as to the        *
* performance, merchantability or fitness for any particular purpose or   *
* use.                                                                    *
*                                                                         *
* In any work or product derived from this material, proper attribution   *
* of the author(s) as the source of the software or data would be         *
* appreciated.                                                            *
*                                                                         *
**************************************************************************/
/* Author Hugues Sicotte 8/14/99 
   Stand-alone program to dust one sequence.
     future improvements are to take a whole fasta library as input.
*/
#include <ncbi.h>
#include <objseq.h>
#include <objsset.h>
#include <sequtil.h>
#include <seqport.h>
#include <tofasta.h>
#include <blast.h>
#include <blastpri.h>
#include <txalign.h>
#include <objloc.h>
#include <dust.h>

		
#define NUMARG 6

static Args myargs [NUMARG] = {
  { "sequence",
	NULL, NULL, NULL, FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  { "Lowercase Masked letters (default is N)",
        "F", NULL, NULL, FALSE, 'l', ARG_BOOLEAN, 0.0, 0, NULL},
  { "is nucleotide)",
        "T", NULL, NULL, FALSE, 'N', ARG_BOOLEAN, 0.0, 0, NULL},
  { "level",
        "20", NULL, NULL, FALSE, 'L', ARG_INT, 0.0, 0, NULL},
  { "window size",
        "16", NULL, NULL, FALSE, 'W', ARG_INT, 0.0, 0, NULL},
  { "Number of iterations of dusting (for timing purposes)",
        "1", NULL, NULL, FALSE, 'I', ARG_INT, 0.0, 0, NULL}

};

Int2 Main (void)
 
{
    CharPtr inputfile;
    FILE* infp, *outfp;
    Boolean is_na,UseN;
    BioseqPtr query;
    SeqEntryPtr sep;
    Int4 loop,i,start,stop,last;
    SeqLocPtr slp,slp_dust;
    Char c;
    SeqPortPtr spp;
    Int2 level,window;

    if (! GetArgs ("testdust", NUMARG, myargs))
        {
            return (1);
        }
    
    if (! SeqEntryLoad())
        return 1;
    
    ErrSetMessageLevel(SEV_WARNING);
    
    inputfile = myargs [0].strvalue;
    
    if ((infp = FileOpen(inputfile, "r")) == NULL)
	{
            ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to open input file %s\n", inputfile);
            return (1);
	}
    is_na = (Boolean) myargs [2].intvalue;
    UseN = (Boolean) myargs [1].intvalue;
    level = (Int2) myargs[3].intvalue;
    window = (Int2) myargs[4].intvalue;
    loop = (Int2) myargs[5].intvalue;
    sep = FastaToSeqEntry(infp, is_na);
    FileClose(infp);
    if (sep != NULL)
	{
            query = NULL;
            if (is_na)
		{
                    SeqEntryExplore(sep, &query, FindNuc);
		}
            else
		{
                    SeqEntryExplore(sep, &query, FindProt);
		}

		if (query == NULL)
		{
	  	 	ErrPostEx(SEV_FATAL, 1, 0, "Unable to obtain bioseq\n");
	   		return 2;
		}
	}
    for(i=0;i<loop;i++) {
        slp_dust = BioseqDust (query, (Int4)0, query->length-1,
                               level, window, (Int2)5, (Int2) 1);
        if(i!=loop-1 && slp_dust)
            slp_dust = SeqLocFree(slp_dust);
    }
    outfp = stdout;
    if(slp_dust==NULL) {
        BioseqRawToFasta(query, outfp, is_na);
    } else {
        SeqEntrysToDefline(sep,outfp,is_na,TRUE);
        start = SeqLocStart(slp_dust);
        stop = SeqLocStop(slp_dust);
        spp = SeqPortNew(query,0,query->length-1,Seq_strand_plus,Seq_code_iupacna);
        last =-1;
        slp=NULL;
        last = -1;
        while((slp=SeqLocFindNext(slp_dust,slp))!=NULL) {
            start = SeqLocStart(slp);
            stop = SeqLocStop(slp);
            for(i=last+1;i<start;) {
                c= SeqPortGetResidue(spp);
                fprintf(outfp,"%c",c);
                i++;                    
                if(!(i%80)) 
                    fprintf(outfp,"\n");
            }
            for(i=start;i<=stop;) {
                c= SeqPortGetResidue(spp);
                if(UseN)
                    c= 'N';
                else
                    c = TO_LOWER(c);
                fprintf(outfp,"%c",c);
                i++;                    
                if(!(i%80)) 
                    fprintf(outfp,"\n");
            }
            
            last = stop;
        }
        for(i=stop+1;i<query->length;) {
                c= SeqPortGetResidue(spp);
                fprintf(outfp,"%c",c);
                i++;                    
                if(!(i%80)) 
                    fprintf(outfp,"\n");
        }

        if(i%80)
            fprintf(outfp,"\n");
        SeqPortFree(spp);
        slp_dust = SeqLocFree(slp_dust);
    }

    SeqEntryFree(sep);

    return 0;
}
	
