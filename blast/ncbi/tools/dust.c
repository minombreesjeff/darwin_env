static char const rcsid[] = "$Id: dust.c,v 6.5 2003/05/30 17:25:36 coulouri Exp $";

/* dust.c
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
* File Name:  dust.c
*
* Author(s):	Roma Tatusov, John Kuzio
*   
* Version Creation Date: 5/26/95
*
* $Revision: 6.5 $
*
* File Description:  a utility to find low complexity NA regions
*
* Modifications:  
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: dust.c,v $
* Revision 6.5  2003/05/30 17:25:36  coulouri
* add rcsid
*
* Revision 6.4  1999/08/18 17:50:47  kans
* include dust.h only after other headers are included to provide prototypes on Mac
*
* Revision 6.3  1999/08/18 16:26:17  sicotte
* Optimized by Moving a Malloc/Free outside of calling functions and other misc optimizations
*
* Revision 6.2  1999/03/12 17:03:29  kuzio
* cast
*
* Revision 6.1  1998/09/24 15:26:39  egorov
* Fix lint complaints
*
* Revision 6.0  1997/08/25 18:53:04  madden
* Revision changed to 6.0
*
* Revision 5.5  1997/04/24 20:50:41  kuzio
* quantify speed up -- remove useless loop
*
 * Revision 5.4  1997/03/12  21:43:48  kuzio
 * for interactive dusting: FilterSeq  from: ,Int4 window,  to: ,Int4Ptr exval,
 *
 * Revision 5.3  1997/02/26  15:59:55  kuzio
 * cast: wo1 ((Int2) (flen-i), seq+i, i, cloc);
 *
 * Revision 5.2  1997/02/14  22:29:48  kuzio
 * filter function updates
 *
 * Revision 5.1  1997/02/11  00:20:45  kuzio
 * support for graphs added
 *
 * Revision 5.0  1996/05/28  13:43:15  ostell
 * Set to revision 5.0
 *
 * Revision 4.3  1996/01/22  18:25:58  kuzio
 * function info in err code levels
 *
 * Revision 4.2  1995/12/07  18:56:19  kuzio
 * Trimmed error reports for invalid residues
 *
 * Revision 4.1  1995/11/01  12:31:59  kuzio
 * fixed nested comment line 491
 *
 * Revision 4.0  1995/07/26  13:50:15  ostell
 * force revision to 4.0
 *
 * Revision 1.2  1995/07/05  14:06:22  kuzio
 * fix EOS typo in dusttripfind array setup
 *
 * Revision 1.1  1995/05/26  18:47:47  kuzio
 * Initial revision
 *
*
* ==========================================================================
*/

#include <sequtil.h>
#include <objmgr.h>
#include <seqport.h>
#include <dust.h>

static char _this_module[] = "dust";
#undef  THIS_MODULE
#define THIS_MODULE _this_module
static char _this_file[] = __FILE__;
#undef  THIS_FILE
#define THIS_FILE _this_file

/* local, file scope, structures and variables */

typedef struct endpoints {
	struct	endpoints PNTR	next;
	Int4	from, to;
} DREGION;

typedef struct localcurrents {
	Int4	curlevel, curstart, curend;
} DCURLOC;

/* local functions */

static Int4 dust_segs PROTO ((Int4, SeqPortPtr, Int4, DREGION PNTR,
		       Int4, Int4, Int4, Int4));
static Int4 wo PROTO ((Int4, SeqPortPtr, Int4, DCURLOC PNTR, Int4 PNTR, UcharPtr seq));
static void wo1 PROTO ((Int4, UcharPtr, Int4, DCURLOC PNTR));
static Int4 dusttripfind PROTO ((SeqPortPtr, UcharPtr, Int4, Int4,
				 Int4 PNTR));
static SeqLocPtr slpDust PROTO ((SeqPortPtr, SeqLocPtr, SeqIdPtr,
				 ValNodePtr PNTR, DREGION PNTR,
				 Int4, Int4));

/* a simple bioseq */

SeqLocPtr BioseqDust (BioseqPtr bsp, Int4 start, Int4 end,
		      Int2 level, Int2 window, Int2 minwin, Int2 linker)
{
	SeqLocPtr	slp = NULL;	/* initialize */
	ValNodePtr	vnp = NULL;

	SeqPortPtr	spp;

	DREGION	PNTR reg, PNTR regold;
	Int4	nreg;
	Int4 l;
	Int4 loopDustMax = 1;

/* error msg stuff */
/*	ErrSetOptFlags (EO_MSG_CODES | EO_SHOW_FILELINE | EO_BEEP); */
	ErrSetOptFlags (EO_MSG_CODES);

/* make sure bioseq is there */
	if (!bsp)
	{
		ErrPostEx (SEV_ERROR, 1, 1,
			  "no bioseq");
                ErrShow ();
		return slp;
	}
	if (!ISA_na (bsp->mol))
	{
		ErrPostEx (SEV_WARNING, 1, 2,
			  "not nucleic acid");
                ErrShow ();
		return slp;
	}

/* place for dusted regions */
	reg = MemNew (sizeof (DREGION));
	if (!reg)
	{
		ErrPostEx (SEV_FATAL, 1, 3,
			   "memory allocation error");
                ErrShow ();
		return slp;
	}
	reg->from = 0;
	reg->to = 0;
	reg->next = NULL;

/* do it */
	spp = SeqPortNew (bsp, start, end, 0, Seq_code_ncbi2na);
	if (!spp)
	{
		ErrPostEx (SEV_ERROR, 1, 4,
			   "sequence port open failed");
                ErrShow ();
		MemFree (reg);
		return slp;
	}

	l = spp->totlen;
	nreg = dust_segs (l, spp, start, reg, (Int4)level, (Int4)window, (Int4)minwin, (Int4)linker);
	slp = slpDust (spp, NULL, bsp->id, &vnp, reg, nreg, loopDustMax);

/* clean up memory */
	SeqPortFree (spp);
	while (reg)
	{
		regold = reg;
		reg = reg->next;
		MemFree (regold);
	}

	return slp;
}

/* sequence location pointers */

SeqLocPtr SeqLocDust (SeqLocPtr this_slp,
		      Int2 level, Int2 window, Int2 minwin, Int2 linker)
{
	SeqLocPtr	next_slp, slp = NULL;
	ValNodePtr	vnp = NULL;

	SeqIdPtr	id;
	BioseqPtr	bsp;
	SeqPortPtr	spp;

	DREGION	PNTR reg, PNTR regold;
	Int4 nreg;
	Int4 start, end, l;
	Int2 loopDustMax = 0;

/* error msg stuff */
	ErrSetOptFlags (EO_MSG_CODES);

	if (!this_slp)
	{
		ErrPostEx (SEV_ERROR, 2, 1,
			  "no sequence location given for dusting");
                ErrShow ();
		return slp;
	}

/* place for dusted regions */
	regold = reg = MemNew (sizeof (DREGION));
	if (!reg)
	{
		ErrPostEx (SEV_FATAL, 2, 2,
			   "memory allocation error");
                ErrShow ();
		return slp;
	}
	reg->from = 0;
	reg->to = 0;
	reg->next = NULL;

/* count seqlocs */
	next_slp = NULL;
	while ((next_slp = SeqLocFindNext (this_slp, next_slp)) != NULL)
			loopDustMax++;
	if (!loopDustMax)
	{
		ErrPostEx (SEV_ERROR, 2, 3,
			   "can not find next seq loc");
                ErrShow ();
	}

/* loop for dusting as needed */
	next_slp = NULL;
	while ((next_slp = SeqLocFindNext (this_slp, next_slp)) != NULL)
	{
/* offsets into actual sequence */
		start = SeqLocStart (next_slp);
		end = SeqLocStop (next_slp);

/* if all goes okay should get a seqport pointer */
		id = SeqLocId (next_slp);
		if (!id)
		{
			ErrPostEx (SEV_ERROR, 2, 4,
				  "no bioseq id");
			ErrShow ();
			continue;
		}
		bsp = BioseqLockById (id);
		if (!bsp)
		{
			ErrPostEx (SEV_ERROR, 2, 5,
				  "no bioseq");
			ErrShow ();
			continue;
		}
		if (!ISA_na (bsp->mol))
		{
			ErrPostEx (SEV_WARNING, 2, 6,
				  "not nucleic acid");
			ErrShow ();
			BioseqUnlock (bsp);
			continue;
		}
		spp = SeqPortNew (bsp, start, end, 0, Seq_code_ncbi2na);
		BioseqUnlock (bsp);
		if (!spp)
		{
			ErrPostEx (SEV_ERROR, 2, 7,
				   "sequence port open failed");
			ErrShow ();
			continue;
		}

		l = spp->totlen;
		nreg = dust_segs (l, spp, start, reg,
				  (Int4)level, (Int4)window, (Int4)minwin, (Int4)linker);
		slp = slpDust (spp, slp, id, &vnp,
			       reg, nreg, loopDustMax);
/* find tail - this way avoids referencing the pointer */
		while (reg->next) reg = reg->next;

		SeqPortFree (spp);
	}

/* clean up memory */
	reg = regold;
	while (reg)
	{
		regold = reg;
		reg = reg->next;
		MemFree (regold);
	}

	return slp;
}

/* entry point for dusting - from BioseqDust or SeqLocDust */

static Int4 dust_segs (Int4 length, SeqPortPtr spp, Int4 start,
		       DREGION PNTR reg,
		       Int4 level, Int4 windowsize, Int4 minwin, Int4 linker)
{
        Int4    len;
	Int4	i, invrescount;
        Int4 retlen;
        UcharPtr seq;
	DREGION	PNTR regold = NULL;
	DCURLOC	cloc;
	Int4	nreg, windowhalf;

/* defaults are more-or-less in keeping with original dust */
	if (level < 2 || level > 64) level = 20;
	if (windowsize < 8 || windowsize > 64) windowsize = 64;
	if (minwin < 4 || minwin > 128) minwin = 4;
	if (linker < 1 || linker > 32) linker = 1;
	windowhalf = windowsize / 2;

/* keep track of weird invalid residues */
	invrescount = 0;

	nreg = 0;
	seq = MemNew (windowsize);			/* triplets */
	if (!seq)
	{
		ErrPostEx (SEV_FATAL, 4, 1,
			   "failed to allocate triplet buffer");
                ErrShow ();
		return nreg;
	}

	for (i = 0; i < length; i += windowhalf)
	{
		len = (Int4) ((length > i+windowsize) ? windowsize : length-i);
		len -= 2;
		retlen = wo (len, spp, i, &cloc, &invrescount, seq);

/* get rid of itsy-bitsy's, dusttripfind aborts - move 1 triplet away */
		if ((cloc.curend - cloc.curstart + 1) < minwin)
		{
			if (retlen != len)
			{
				i += (retlen - windowhalf + 3);
			}
			continue;
		}

		if (cloc.curlevel > level)
		{
			if (nreg &&
			    regold->to + linker >= cloc.curstart+i+start &&
			    regold->from <= cloc.curstart + i + start)
			{
/* overlap windows nicely if needed */
				regold->to = cloc.curend + i + start;
			}
			else
			{
/* new window or dusted regions do not overlap				*/
				reg->from = cloc.curstart + i + start;
				reg->to = cloc.curend + i + start;
/* 5' edge effects - 3' edge effects - are best handled interactively	*/
/* it prbly would be good to put 'linker' as an interactive option	*/
/* interactive means wrapping stuff up in a graphics shell		*/
				regold = reg;
				reg = MemNew (sizeof (DREGION));
				if (!reg)
				{
					ErrPostEx (SEV_FATAL, 3, 1,
						   "memory allocation error");
					ErrShow ();
                                        MemFree(seq);
					return nreg;
				}
				reg->next = NULL;
				regold->next = reg;
				nreg++;
			}
/* kill virtually all 3' tiling anomalies */
			if (cloc.curend < windowhalf)
			{
				i += (cloc.curend - windowhalf);
			}
		}				/* end 'if' high score	*/
	}					/* end for */
	MemFree (seq);
	if (invrescount > 0)
	{
		ErrPostEx (SEV_INFO, 3, 2,
		   "Total invalid residues found: %ld", (long) invrescount);
                ErrShow ();
	}
	return nreg;
}

static Int4 wo (Int4 len, SeqPortPtr spp, Int4 iseg, DCURLOC PNTR cloc,
			Int4 PNTR invrescount, UcharPtr seq)
{
	Int4 i, flen;

	cloc->curlevel = 0;
	cloc->curstart = 0;
	cloc->curend = 0;

/* get the chunk of sequence in triplets */

	SeqPortSeek (spp, iseg, SEEK_SET);
        MemSet (seq,0,len+2);        /* Zero the triplet buffer */
	flen = dusttripfind (spp, seq, iseg, len, invrescount);

/* dust the chunk */
	for (i = 0; i < flen; i++)
	{
		wo1 (flen-i, seq+i, i, cloc);
	}

	cloc->curend += cloc->curstart;

	return flen;
}

static void wo1 (Int4 len, UcharPtr seq, Int4 iwo, DCURLOC PNTR cloc)
{
        Uint4 sum;
	Int4 loop;
	Int4 newlevel;

	Int2 PNTR countsptr;
	Int2 counts[4*4*4];
	MemSet (counts, 0, sizeof (counts));
/* zero everything */
	sum = 0;
	newlevel = 0;

/* dust loop -- specific for triplets	*/
	for (loop = 0; loop < len; loop++)
	{
		countsptr = &counts[*seq++];
		if (*countsptr)
		{
			sum += (Uint8)(*countsptr);

			newlevel = 10 * sum / loop;

			if (cloc->curlevel < newlevel)
			{
				cloc->curlevel = newlevel;
				cloc->curstart = iwo;
				cloc->curend = loop + 2; /* triplets */
			}
		}
		(*countsptr)++;
	}
	return;
}

/* fill an array with 2-bit encoded triplets */

static Int4 dusttripfind (SeqPortPtr spp, UcharPtr s1, Int4 icur, Int4 max,
				Int4 PNTR invrescount)
{
        Int4 pos;
        Int4 n;
	UcharPtr s2, s3;
	Int2 c;
	Boolean flagVD;

	n = 0;

	s2 = s1 + 1;
	s3 = s1 + 2;

	SeqPortSeek (spp, icur, SEEK_SET);

/* set up needs streamlining */
/* start again at segment or virtual sequence bounderies */
/* set up 1 */
	if ((c = SeqPortGetResidue (spp)) == SEQPORT_EOF) 
            return n;
	if (c == SEQPORT_EOS || c == SEQPORT_VIRT) 
            return n;
	if (!IS_residue (c))
	{
		c = 0;				/* 255 it's 'A' */
                if (*invrescount < 3)
                {
            		pos = SeqPortTell (spp);
         		ErrPostEx (SEV_INFO, 5, 1,
			 "Invalid residue converted to 'A': %ld", (long) pos);
			ErrShow ();
		}
		(*invrescount)++;
	}
	*s1 |= c;
	*s1 <<= 2;

/* set up 2 */
	if ((c = SeqPortGetResidue (spp)) == SEQPORT_EOF)
            return n;
	if (c == SEQPORT_EOS || c == SEQPORT_VIRT)
            return n;
	if (!IS_residue (c))
	{
		c = 0;				/* 255 it's 'A' */
                if (*invrescount < 3)
                {
                        pos = SeqPortTell (spp);
         		ErrPostEx (SEV_INFO, 5, 1,
			 "Invalid residue converted to 'A': %ld", (long) pos);
			ErrShow ();
		}
		(*invrescount)++;
	}
	*s1 |= c;
	*s2 |= c;

/* triplet fill loop */
	flagVD = TRUE;
	while ((c = SeqPortGetResidue (spp)) != SEQPORT_EOF && n < max)
	{
		if (c == INVALID_RESIDUE)
		{
			c = 0;				/* 255 it's 'A' */
			if (*invrescount < 3)
			{
          			pos = SeqPortTell (spp);
				ErrPostEx (SEV_INFO, 5, 1,
				 "Invalid residue converted to 'A': %ld", (long) pos);
				ErrShow ();
			}
			(*invrescount)++;
		}
		if (IS_residue (c))
		{
				*s1 <<= 2;
				*s2 <<= 2;
				*s1 |= c;
				*s2 |= c;
				*s3 |= c;
				s1++;
				s2++;
				s3++;
				n++;
		}
		else
		{
			switch (c)
			{
				case SEQPORT_EOS:	/* 252 rare	*/
					break;
/* VIRT if there is an undetermined segment of sequence			*/
				case SEQPORT_VIRT:	/* 251 ignore ?	*/
				default:
/*					flagVD = TRUE;     dust across v-seg */
					flagVD = FALSE;  /* don't dust across */
					break;
			}
			if (!flagVD) break;
		}
	}		/* end while */

	return n;
}

/* look for dustable locations */

static SeqLocPtr slpDust (SeqPortPtr spp, SeqLocPtr slp, SeqIdPtr id,
			  ValNodePtr PNTR vnp, DREGION PNTR reg,
			  Int4 nreg, Int4 loopDustMax)
{
	SeqIntPtr	sintp;
        Int4            i;
        Boolean         flagNoPack;

/* point to dusted locations */
	if (nreg)
	{

/* loopDustMax == 1 forces PACKED_INT IN - PACKED_INT OUT as needed	*/
		flagNoPack = FALSE;
		if (nreg == 1 && loopDustMax == 1) flagNoPack = TRUE;

		if (!slp)
		{
			if ((slp = ValNodeNew (NULL)) == NULL)
			{
				ErrPostEx (SEV_ERROR, 6, 1,
					   "val node new failed");
				ErrShow ();
				return slp;
			}
		}

		if (flagNoPack)
		{
			slp->choice = SEQLOC_INT;
		}
		else
		{
			slp->choice = SEQLOC_PACKED_INT;
		}

		for (i = 0; i < nreg; i++)
		{
			sintp = SeqIntNew ();
			if (!sintp)
			{
				ErrPostEx (SEV_FATAL, 6, 2,
					   "memory allocation error");
				ErrShow ();
				return slp;
			}
			sintp->id = SeqIdDup (id);
			sintp->from = reg->from;
			sintp->to = reg->to;
			if (!flagNoPack) ValNodeAddPointer
					(vnp, SEQLOC_INT, sintp);
			reg = reg->next;
		}

		if (flagNoPack)
		{
			slp->data.ptrvalue = (Pointer) sintp;
		}
		else
		{
			slp->data.ptrvalue = *vnp;
		}
	}
	return slp;
}

extern FloatHiPtr DustGraph (SeqPortPtr spp, Int4 length, Int2 level,
                             Int2 window, Int2 minwin, Int2 linker)
{
  DCURLOC     cloc;
  Int2        windowhalf;
  Int2        retlen;
  Int4        len;
  Int4        i, n;
  Int4        invrescount;
  FloatHi     maxval, curval;
  FloatHiPtr  fhead, ftemp;
  FloatHiPtr  fptr;
  UcharPtr    seq;

  invrescount = 0;
  windowhalf = window / 2;

  maxval = level * 5.0;
  if (maxval > 100.0)
    maxval = 100.0;

  fptr = (FloatHiPtr) MemNew ((sizeof (FloatHi)) * length);
  fhead = fptr;
  for (i = 0; i < length; i++)
    *fptr++ = 0.0;

  fptr = fhead;
  seq = MemNew ((size_t)window);			/* triplets */
  if (!seq)
      {
          ErrPostEx (SEV_FATAL, 4, 1,
                     "failed to allocate triplet buffer");
          ErrShow ();
          return 0;
      }

  for (i = 0; i < length; i += windowhalf)
  {
    len = (length > i+window) ? window : (Int2) (length-i);
    len -= 2;
    retlen = wo (len, spp, i, &cloc, &invrescount, seq);

    if ((cloc.curend - cloc.curstart + 1) < minwin)
    {
      if (retlen != len)
        i += (retlen - windowhalf + 3);
      continue;
    }
    curval = (FloatHi) cloc.curlevel;
    if (curval > maxval)
      curval = maxval;
    fptr = fhead + cloc.curstart + i;
    for (n = cloc.curstart; n < cloc.curend+1; n++)
      *fptr++ = curval;
  }
  MemFree(seq);
  fptr = fhead;
  for (i = 0; i < length-((Int4)linker); i++)
  {
    if (*fptr != 0)
    {
      ftemp = fptr + linker;
      if (*ftemp != 0)
      {
        curval = (*ftemp + *fptr) / 2;
        fptr++;
        for (n = 1; n < linker; n++)
          *fptr++ = curval;
        i += (linker - 1);
      }
    }
    else
    {
      fptr++;
    }
  }

  return fhead;
}
