/*   ncbimisc.c
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
* File Name:  ncbimisc.c
*
* Author:  Gish, Kans, Ostell, Schuler
*
* Version Creation Date:   10/23/91
*
* $Revision: 6.21 $
*
* File Description: 
*   	miscellaneous functions
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 04-15-93 Schuler     Changed _cdecl to LIBCALL
* 06-15-93 Schuler     SGML functions moved to ncbisgml.c
* 02-16-94 Epstein     Retired Gestalt functions and definitions
*
* $Log: ncbimisc.c,v $
* Revision 6.21  2001/04/05 03:03:28  juran
* Defined our own C2PStr() and P2Cstr() for use with Carbon (which omits them).
*
* Revision 6.20  2000/10/30 18:11:41  beloslyu
* FreeBSD was added
*
* Revision 6.19  2000/08/28 18:41:29  vakatov
* Added type cast in ValNodeSort() to pass C++ compilation
*
* Revision 6.18  2000/03/10 17:29:12  kans
* changed divray to Uint8 - fix supplied by DDBJ for Cray computer
*
* Revision 6.17  1999/12/21 17:52:40  kans
* removed MPW/THINKC conditional code, starting upgrade to Carbon compatibility - Churchill
*
* Revision 6.16  1999/07/29 15:58:48  kans
* added bigintvalue, ValNodeAddBigInt (PD)
*
* Revision 6.15  1999/06/07 18:22:20  beloslyu
* NetBSD port
*
* Revision 6.14  1999/04/23 18:00:15  beloslyu
* change Uint4 to Nlm_Uint4 and CharPtr to Nlm_CharPtr
*
* Revision 6.13  1999/04/23 17:28:50  shavirin
* Changed Uint4 to the Nlm_Uint4
*
* Revision 6.12  1999/04/23 16:10:26  shavirin
* Added new function Nlm_GetChecksum() which calculates
* checksum from given string
*
* Revision 6.11  1999/04/14 19:37:23  madden
* Add Nlm_ prefix to Int8
*
* Revision 6.10  1999/04/14 15:31:06  madden
* add Nlm_Int8tostr function
*
* Revision 6.9  1999/03/11 16:21:51  kans
* ValNodeSort is more visible copy of jzmisc SortValNode
*
* Revision 6.8  1999/01/29 19:23:05  kans
* use C2PStr and P2CStr from <TextUtils.h>
*
* Revision 6.7  1999/01/21 20:08:37  ostell
* added SwitchUint2 and 4, added integer bytestores
*
* Revision 6.6  1999/01/07 15:20:57  victorov
* added Nlm_ prefix to MD5 functions to avoid conflicts
*
* Revision 6.5  1999/01/06 22:49:26  victorov
* added MD5 hash calculator
*
* Revision 6.4  1998/10/13 20:49:38  vakatov
* + Nlm_PlatformName()
*
* Revision 6.3  1998/07/02 18:24:28  vakatov
* Cleaned the code & made it pass through the C++ compilation
*
* Revision 6.2  1998/04/08 16:49:56  kans
* ValNodeLen returns Int4
*
* Revision 6.1  1997/12/12 21:42:28  kans
* Strings.h dragged in QuickDraw.h, so it is now out of the header
*
* Revision 6.0  1997/08/25 18:16:45  madden
* Revision changed to 6.0
*
* Revision 5.4  1997/01/31 15:37:12  vakatov
* ListSort():  replaced Boolean --> Nlm_Boolean
*
 * Revision 5.3  1997/01/28  21:26:56  epstein
 * move doubly-linked list handling functions from ni_list.[ch]
 *
 * Revision 5.2  1996/12/10  16:09:16  kans
 * added ValNodeLen function
 *
 * Revision 5.1  1996/12/03  21:48:33  vakatov
 * Adopted for 32-bit MS-Windows DLLs
 *
 * Revision 5.0  1996/05/28  13:18:57  ostell
 * Set to revision 5.0
 *
 * Revision 4.2  1996/03/26  16:31:06  epstein
 * migrate byte-swapping functions to ncbimisc.[ch]
 *
 * Revision 4.1  1995/10/28  15:03:20  ostell
 * added casts to quiet DOS compile warnings
 *
 * Revision 4.0  1995/07/26  13:46:50  ostell
 * force revision to 4.0
 *
 * Revision 2.17  1995/05/15  18:45:58  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/
 
#include <ncbi.h>
#include <ncbiwin.h>

#ifdef OS_MAC
#include <TextUtils.h>
#endif

/*
TRIPLE_MARK is the character inserted before the thousands, millions,
billions, etc. digit positions.  Change TRIPLE_MARK to a period
for the International scene, or define it as '\0' if no magnitude
markers are desired.
*/
#define TRIPLE_MARK	','

#define MISC_COMMAS 1 /* insert commas only when |value| >= 10,000 */
#define MISC_ALLCOMMAS 2 /* insert commas for any |value| >= 1,000 */
#define MISC_ANYCOMMAS (MISC_COMMAS|MISC_ALLCOMMAS)
#define MISC_PLUSSIGNS 4 /* prepend a plus sign (+) to positive values */


/*
buf[NBUFS][] is a circularly-maintained list of buffers for storing
the results of calls to Nlm_Ltostr() and Nlm_Ultostr().  Up to NBUFS
usages of either function in a single printf() may be made.
NBUFS should be defined large enough to satisfy _all_ likely occurrences.
*/
#define NBUFS 10 /* No. of static buffers for the ASCII results */

static int	bufno = 0; /* current buffer marker in the circular list */
static char	buf[NBUFS][(CHAR_BIT)*sizeof(long)/2];

/* divray[] is a fixed array of power-of-10 divisors which must be initialized*/
static Uint8 divray[CHAR_BIT*sizeof(Uint8)/2];
/* divray_max is related to the maximum precision available in a long int */
static int	divray_max;

/* commaray[] is a fixed array that identifies positions where commas belong */
static char	commaray[DIM(divray)];

/* divray_init() initializes divray[] */
static void divray_init PROTO((void));

/* ncbi_ultostr() is the basic (unsigned) integer-to-ASCII conversion engine */
static void ncbi_ultostr PROTO((char *buff, Uint8 value, int commas));

/* ulwidth() is the basic length-determiner for integer-ASCII conversions */
static int ulwidth PROTO((unsigned long value, int commas));

/* heapify() is the basic heap-sort function used by Nlm_HeapSort() */
static void	heapify PROTO((Nlm_CharPtr b0, Nlm_CharPtr b, Nlm_CharPtr lim, Nlm_CharPtr last, size_t w, int (LIBCALLBACK *compar) (Nlm_VoidPtr, Nlm_VoidPtr) ));

/* divray_init -- initialize array of divisors and array of marker locations */
static void
divray_init (void)

{
	Uint8	j = UINT8_MAX, k = 1;

	for (divray_max=0; divray_max < DIM(divray) && j != 0; ++divray_max) {
		divray[divray_max] = k;
		if ((divray_max+1)%3 == 0)
			commaray[divray_max] = (TRIPLE_MARK != '\0');
		j /= 10;
		k *= 10;
	}
	--divray_max;
}

/* ncbi_ultostr is the basic (unsigned) integer->ASCII conversion engine */
static void
ncbi_ultostr (char *buff, Uint8 value, int commas)
{
	Uint8 value_orig = value;
	int	i, quotient;

	if (divray_max == 0)
		divray_init();

	/*
	Insert commas when value_orig >= 10000 (the Macintosh Way),
	unless MISC_ALLCOMMAS is set.
	*/
	commas = ((commas&MISC_ALLCOMMAS) && value >= 1000)
				|| (commas && value > (10*1000));

	for (i=divray_max; i > 0 && divray[i] > value; --i)
		;

	for (; i >= 0; --i) {
		if (commas && commaray[i] != NULLB && value != value_orig)
			*buff++ = TRIPLE_MARK;
		quotient = (int)(value / divray[i]);
		*buff++ = (char) '0' + (char) quotient;
		switch (quotient) {
		case 0: break;
		case 1: value -=   divray[i]; break;
		case 2: value -= 2*divray[i]; break;
		case 3: value -= 3*divray[i]; break;
		case 4: value -= 4*divray[i]; break;
		case 5: value -= 5*divray[i]; break;
		case 6: value -= 6*divray[i]; break;
		case 7: value -= 7*divray[i]; break;
		case 8: value -= 8*divray[i]; break;
		case 9: value -= 9*divray[i]; break;
		default: value -= quotient*divray[i]; break; /* shouldn't be taken */
		}
	}
	*buff = NULLB; /* tack on a NUL terminator */
	return;
}
/* Nlm_Int8tostr -- convert a signed long integer to ASCII */
NLM_EXTERN char * LIBCALL Nlm_Int8tostr (Nlm_Int8 value, int opts)

{
	char	*bp0, *bp;

    bp0 = bp = &buf[bufno][0];
	if (++bufno >= NBUFS)
		bufno = 0;

	if (value < 0) {
		*bp++ = '-';
		value = -value;
	}
	else
		if (opts&MISC_PLUSSIGNS && value > 0)
			*bp++ = '+';

	ncbi_ultostr(bp, (Uint8)value, opts&MISC_ANYCOMMAS);

    return bp0;
}

/* Nlm_Ltostr -- convert a signed long integer to ASCII */
NLM_EXTERN char * LIBCALL Nlm_Ltostr (long value, int opts)

{
	char	*bp0, *bp;

    bp0 = bp = &buf[bufno][0];
	if (++bufno >= NBUFS)
		bufno = 0;

	if (value < 0) {
		*bp++ = '-';
		value = -value;
	}
	else
		if (opts&MISC_PLUSSIGNS && value > 0)
			*bp++ = '+';

	ncbi_ultostr(bp, (Uint8)value, opts&MISC_ANYCOMMAS);

    return bp0;
}

/* Nlm_Ultostr convert an unsigned long integer to ASCII */
NLM_EXTERN char * LIBCALL Nlm_Ultostr (unsigned long value, int opts)

{
	char	*bp0, *bp;

    bp = bp0 = &buf[bufno][0];
	if (++bufno >= NBUFS)
		bufno = 0;

	if (opts&MISC_PLUSSIGNS && value > 0)
		*bp++ = '+';

	ncbi_ultostr(bp, (Uint8) value, opts&MISC_ANYCOMMAS);

    return bp0;
}

/*
Return the length (in characters) of the ASCII base 10 representation
of the specified integer.

If "opts&MISC_COMMAS" is non-zero, consider the additional length required
for commas before the thousands, millions, billions, etc.  positions.

If "opts&MISC_ALLCOMMAS" is non-zero, insert commas even when the value
of the integer is less than 10,000.

If "opts&MISC_PLUSSIGNS" is non-zero, consider the length of a plus sign
in front of any positive value, as well as the standard minus sign in front
of negative values.
*/
NLM_EXTERN int LIBCALL Nlm_Lwidth (long value, int opts)

{
	int	len;

	if (value < 0) {
		len = 1; /* account for the minus sign */
		value = -value;
	}
	else
		/* account for a plus sign */
		len = (opts&MISC_PLUSSIGNS) && (value > 0);
			
	return len + ulwidth(value, opts&MISC_ANYCOMMAS);
}

/*
Return the length (in characters) of the ASCII base 10 representation
of the specified unsigned integer.
*/
NLM_EXTERN int LIBCALL Nlm_Ulwidth (unsigned long value, int opts)

{
	int	len;

	len = ulwidth(value, opts&MISC_ANYCOMMAS);
	return len + ((opts&MISC_PLUSSIGNS) && (value > 0));
}

static int
ulwidth (unsigned long value, int commas)

{
	int	j, len;

	if (divray_max == 0)
		divray_init();

	for (len=divray_max; len > 0 && divray[len] > value; --len)
		;

	if ((commas&MISC_ALLCOMMAS) || (commas && value >= (10*1000)) ) {
		for (j = len-1; j > 1; --j)
			len += (commaray[j] != 0);
	}
	return len+1;
}

/*
	Nlm_HeapSort -- sort a list using an heap sort algorithm

	Performance is guaranteed O(NlogN).  Compared to BSD UNIX(TM) qsort,
	Nlm_HeapSort averages about half as fast--which may be acceptable
	for a portable, public domain function which qsort is not.

	This code was derived from original work by Professor Webb Miller
	(Penn. State University), but don't blame him for this mess or any errors.

	7/31/90 WRG
	6/18/92 Modified for segmented memory safety.  JMO
*/

NLM_EXTERN void LIBCALL Nlm_HeapSort (Nlm_VoidPtr b, size_t nel, size_t width, int (LIBCALLBACK *compar )PROTO ((Nlm_VoidPtr, Nlm_VoidPtr )))	/* Element comparison routine */

{
	register Nlm_CharPtr	base = (Nlm_CharPtr)b;
	register size_t	i;
	register char	ch;
	register Nlm_CharPtr 	base0 = (Nlm_CharPtr)base, lim, basef;

	if (nel < 2)
		return;

	lim = &base[((nel-2)/2)*width];
	basef = &base[(nel-1)*width];
	i = nel/2;
	for (base = &base0[(i - 1)*width]; i > 0; base = base - width) {
		heapify(base0, base, lim, basef, width, compar);
		i--;
	}

	for (base = &base0[(nel-1)*width]; base > base0; base -= width) {
		for (i=0; i<width; ++i) {
			ch = base0[i];
			base0[i] = base[i];
			base[i] = ch;
		}
		lim = base0 + ((base-base0)/2 - width);
		if (base > base0+width)
			heapify(base0, base0, lim, base-width, width, compar);
	}
}

static void
heapify (Nlm_CharPtr base0, Nlm_CharPtr base, Nlm_CharPtr lim, Nlm_CharPtr last, size_t width, int (LIBCALLBACK *compar )PROTO ((Nlm_VoidPtr, Nlm_VoidPtr )))

{
	register size_t	i;
	register char	ch;
	register Nlm_CharPtr left_son, large_son;

	left_son = base0 + 2*(base-base0) + width;
	while (base <= lim) {
		if (left_son == last)
			large_son = left_son;
		else
			large_son = (*compar)(left_son, left_son+width) >= 0 ?
							left_son : left_son+width;
		if ((*compar)(base, large_son) < 0) {
			for (i=0; i<width; ++i) {
				ch = base[i];
				base[i] = large_son[i];
				large_son[i] = ch;
			}
			base = large_son;
			left_son = base0 + 2*(base-base0) + width;
		} else
			break;
	}
}

/*****************************************************************************
*
*   ValNodeNew(vnp)
*      adds after last node in list if vnp not NULL
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ValNodeNew (ValNodePtr vnp)

{
	ValNodePtr newnode;

	newnode = (ValNodePtr) Nlm_MemNew(sizeof(ValNode));
	if (vnp != NULL)
    {
        while (vnp->next != NULL)
            vnp = vnp->next;
		vnp->next = newnode;
    }
	return newnode;
}

/*****************************************************************************
*
*   ValNodeLen(vnp)
*      returns the number of nodes in the linked list
*
*****************************************************************************/
NLM_EXTERN Nlm_Int4 LIBCALL ValNodeLen (ValNodePtr vnp)

{
	Nlm_Int4 len;

	len = 0;
	while (vnp != NULL) {
		len++;
		vnp = vnp->next;
	}
	return len;
}

/*****************************************************************************
*
*   ValNodeAdd(head)
*      adds after last node in list if *head not NULL
*      If *head is NULL, sets it to the new ValNode
*      returns pointer to the NEW node added
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ValNodeAdd (ValNodePtr PNTR head)

{
	ValNodePtr newnode;

	if (head != NULL)
	{
		newnode = ValNodeNew(*head);
		if (*head == NULL)
			*head = newnode;
	}
	else
		newnode = ValNodeNew(NULL);

	return newnode;
}

/*****************************************************************************
*
*   ValNodeLink(head, newnode)
*      adds newnode at end of chain
*      if (*head == NULL) *head = newnode
*      ALWAYS returns pointer to START of chain
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ValNodeLink (ValNodePtr PNTR head, ValNodePtr newnode)
{
   ValNodePtr vnp;

   if (head == NULL)
	 return newnode;

   vnp = *head;
   
   if (vnp != NULL )
   {
        while (vnp->next != NULL)
            vnp = vnp->next;
        vnp->next = newnode;
    }
    else
		*head = newnode;

   return *head;
}

/*****************************************************************************
*
*   ValNodeAddStr (head, choice, str)
*      adds like ValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.ptrvalue = str
*         does NOT copy str
*      if str == NULL, does not add a ValNode
*   
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ValNodeAddStr (ValNodePtr PNTR head, Nlm_Int2 choice, Nlm_CharPtr str)
{
	ValNodePtr newnode;

	if (str == NULL) return NULL;

	newnode = ValNodeAdd(head);
	if (newnode != NULL)
	{
		newnode->choice = (Nlm_Uint1)choice;
		newnode->data.ptrvalue = (Nlm_VoidPtr)str;
	}

	return newnode;
}

/*****************************************************************************
*
*   ValNodeCopyStr (head, choice, str)
*      adds like ValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.ptrvalue = str
*         makes a COPY of str
*      if str == NULL, does not add a ValNode
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ValNodeCopyStr (ValNodePtr PNTR head, Nlm_Int2 choice, Nlm_CharPtr str)
{
	ValNodePtr newnode;

	if (str == NULL) return NULL;

	newnode = ValNodeAdd(head);
	if (newnode != NULL)
	{
		newnode->choice = (Nlm_Uint1)choice;
		newnode->data.ptrvalue = StringSave(str);
	}

	return newnode;
}

/*****************************************************************************
*
*   ValNodeAddInt (head, choice, value)
*      adds like ValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.intvalue = value
*   
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ValNodeAddInt (ValNodePtr PNTR head, Nlm_Int2 choice, Nlm_Int4 value)
{
	ValNodePtr newnode;

	newnode = ValNodeAdd(head);
	if (newnode != NULL)
	{
		newnode->choice = (Nlm_Uint1)choice;
		newnode->data.intvalue = value;
	}

	return newnode;
}

/*****************************************************************************
*
*   ValNodeAddBigInt (head, choice, value)
*      adds like ValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.bigintvalue = value
*   
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ValNodeAddBigInt (ValNodePtr PNTR head, Nlm_Int2 choice, Nlm_Int8 value)
{
	ValNodePtr newnode;

	newnode = ValNodeAdd(head);
	if (newnode != NULL)
	{
		newnode->choice = (Nlm_Uint1)choice;
		newnode->data.bigintvalue = value;
	}

	return newnode;
}

/*****************************************************************************
*
*   ValNodeAddBoolean (head, choice, value)
*      adds like ValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.boolvalue = value
*   
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ValNodeAddBoolean (ValNodePtr PNTR head, Nlm_Int2 choice, Nlm_Boolean value)
{
	ValNodePtr newnode;

	newnode = ValNodeAdd(head);
	if (newnode != NULL)
	{
		newnode->choice = (Nlm_Uint1)choice;
		newnode->data.boolvalue = value;
	}

	return newnode;
}

/*****************************************************************************
*
*   ValNodeAddFloat (head, choice, value)
*      adds like ValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.realvalue = value
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ValNodeAddFloat (ValNodePtr PNTR head, Nlm_Int2 choice, Nlm_FloatHi value)
{
	ValNodePtr newnode;

	newnode = ValNodeAdd(head);
	if (newnode != NULL)
	{
		newnode->choice = (Nlm_Uint1)choice;
		newnode->data.realvalue = value;
	}

	return newnode;
}

/*****************************************************************************
*
*   ValNodeAddPointer (head, choice, value)
*      adds like ValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.ptrvalue = value
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ValNodeAddPointer (ValNodePtr PNTR head, Nlm_Int2 choice, Nlm_VoidPtr value)
{
	ValNodePtr newnode;

	newnode = ValNodeAdd(head);
	if (newnode != NULL)
	{
		newnode->choice = (Nlm_Uint1)choice;
		newnode->data.ptrvalue = value;
	}

	return newnode;
}

/*****************************************************************************
*
*   ValNodeAddFunction (head, choice, value)
*      adds like ValNodeAdd()
*      sets newnode->choice = choice (if choice does not matter, use 0)
*      sets newnode->data.funcvalue = value
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ValNodeAddFunction (ValNodePtr PNTR head, Nlm_Int2 choice, Nlm_FnPtr value)
{
	ValNodePtr newnode;

	newnode = ValNodeAdd(head);
	if (newnode != NULL)
	{
		newnode->choice = (Nlm_Uint1)choice;
		newnode->data.funcvalue = value;
	}

	return newnode;
}

/*****************************************************************************
*
*   ValNodeFree(vnp)
*   	frees whole chain of ValNodes
*       Does NOT free associated data pointers
*           see ValNodeFreeData()
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ValNodeFree (ValNodePtr vnp)
{
	ValNodePtr next;

	while (vnp != NULL)
	{
		next = vnp->next;
		Nlm_MemFree(vnp);
		vnp = next;
	}
	return NULL;
}

/*****************************************************************************
*
*   ValNodeFreeData(vnp)
*   	frees whole chain of ValNodes
*       frees associated data pointers - BEWARE of this if these are not
*           allocated single memory block structures.
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ValNodeFreeData (ValNodePtr vnp)
{
	ValNodePtr next;

	while (vnp != NULL)
	{
		Nlm_MemFree(vnp->data.ptrvalue);
		next = vnp->next;
		Nlm_MemFree(vnp);
		vnp = next;
	}
	return NULL;
}

/*****************************************************************************
*
*   ValNodePtr ValNodeExtract(headptr, choice)
*       removes first node in chain where ->choice == choice
*       rejoins chain after removing the node
*       sets node->next to NULL
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ValNodeExtract (ValNodePtr PNTR headptr, Nlm_Int2 choice)
{
    ValNodePtr last = NULL,
        vnp = * headptr;

    while (vnp != NULL)
    {
        if (vnp->choice == (Nlm_Uint1)choice)
        {
            if (last == NULL)    /* first one */
                * headptr = vnp->next;
            else
                last->next = vnp->next;

            vnp->next = NULL;
            return vnp;
        }
        else
        {
            last = vnp;
            vnp = vnp->next;
        }
    }

    return NULL;    /* not found */
}

/*****************************************************************************
*
*   ValNodePtr ValNodeExtractList(headptr, choice)
*       removes ALL nodes in chain where ->choice == choice
*       rejoins chain after removing the nodes
*       returns independent chain of extracted nodes
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ValNodeExtractList (ValNodePtr PNTR headptr, Nlm_Int2 choice)
{
    ValNodePtr last = NULL, first = NULL, vnp;

    while ((vnp = ValNodeExtract(headptr, choice)) != NULL)
    {
		if (last == NULL)
		{
			last = vnp;
			first = vnp;
		}
		else
			last->next = vnp;
		last = vnp;
	}

    return first;
}

/*****************************************************************************
*
*   ValNodeFindNext (head, curr, choice)
*   	Finds next ValNode with vnp->choice == choice after curr
*       If curr == NULL, starts at head of list
*       If choice < 0 , returns all ValNodes
*       Returns NULL, when no more found
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ValNodeFindNext (ValNodePtr head, ValNodePtr curr, Nlm_Int2 choice)
{
	if (head == NULL) return NULL;

	if (curr == NULL)
		curr = head;
	else
		curr = curr->next;

	while (curr != NULL)
	{
		if ((choice < 0) || (curr->choice == (Nlm_Uint1)choice))
			return curr;
		curr = curr->next;
	}

	return curr;
}

/*****************************************************************************
*
*   ValNodeSort(list, compar)
*   	Copied from SortValNode in jzcoll, renamed, for more general access
*   	Makes array from ValNode list, calls HeapSort, reconnects ValNode list
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ValNodeSort (ValNodePtr list, int (LIBCALLBACK *compar )PROTO ((Nlm_VoidPtr, Nlm_VoidPtr )))
{
	ValNodePtr tmp, PNTR head;
	Nlm_Int4 count, i;

	if (list == NULL) return NULL;
	
	count = ValNodeLen (list);
	head = (ValNodePtr *) MemNew (((size_t) count + 1) * sizeof (ValNodePtr));
	for (tmp = list, i = 0; tmp != NULL && i < count; i++) {
		head [i] = tmp;
		tmp = tmp->next;
	}

	HeapSort (head, (size_t) count, sizeof (ValNodePtr), compar);
	for (i = 0; i < count; i++) {
		tmp = head [i];
		tmp->next = head [i + 1];
	}
	list = head [0];
	MemFree (head);

	return list;
}

/*****************************************************************************
*
*   Start Of Node List Functions
*
*****************************************************************************/

NLM_EXTERN ValNodePtr LIBCALL NodeListNew (void)

{
  ValNodePtr  vnp;

  vnp = ValNodeNew (NULL);
  return vnp;
}

NLM_EXTERN ValNodePtr LIBCALL NodeListFree (ValNodePtr head)

{
  if (head != NULL) {
    ValNodeFreeData (head);
  }
  return NULL;
}

NLM_EXTERN Nlm_Int2 LIBCALL NodeListLen (ValNodePtr head)

{
  Nlm_Int2  item;

  item = 0;
  if (head != NULL) {
    while (head->next != NULL) {
      head = head->next;
      item++;
    }
  }
  return item;
}

NLM_EXTERN ValNodePtr LIBCALL NodeListFind (ValNodePtr head, Nlm_Int2 item, Nlm_Boolean extend)

{
  ValNodePtr  vnp;

  vnp = NULL;
  if (head != NULL && item > 0) {
    vnp = head;
    while (vnp->next != NULL && item > 0) {
      vnp = vnp->next;
      item--;
    }
    if (extend) {
      while (item > 0) {
        vnp = ValNodeNew (vnp);
        item--;
      }
    } else if (item > 0) {
      vnp = NULL;
    }
  }
  return vnp;
}

NLM_EXTERN Nlm_Boolean LIBCALL NodeListRead (ValNodePtr head, Nlm_Int2 item, Nlm_VoidPtr ptr, size_t size)

{
  Nlm_Boolean  copied;
  Nlm_BytePtr  dst;
  Nlm_BytePtr  src;
  ValNodePtr   vnp;

  copied = FALSE;
  if (head != NULL && item > 0 && ptr != NULL) {
    vnp = NodeListFind (head, item, FALSE);
    if (vnp != NULL && vnp->data.ptrvalue != NULL) {
      dst = (Nlm_BytePtr) ptr;
      src = (Nlm_BytePtr) (vnp->data.ptrvalue);
      while (size > 0) {
        *dst = *src;
        dst++;
        src++;
        size--;
      }
      copied = TRUE;
    } else {
      Nlm_MemFill (ptr, 0, size);
    }
  }
  return copied;
}

static Nlm_Boolean LIBCALL Nlm_WriteToNode (ValNodePtr vnp, Nlm_VoidPtr ptr, size_t size)

{
  Nlm_Boolean  copied;
  Nlm_BytePtr  dst;
  Nlm_BytePtr  src;

  copied = FALSE;
  if (vnp != NULL) {
    vnp->data.ptrvalue = MemFree (vnp->data.ptrvalue);
    if (ptr != NULL) {
      vnp->data.ptrvalue = MemNew (size);
      if (vnp->data.ptrvalue != NULL) {
        dst = (Nlm_BytePtr) (vnp->data.ptrvalue);
        src = (Nlm_BytePtr) ptr;
        while (size > 0) {
          *dst = *src;
          dst++;
          src++;
          size--;
        }
        copied = TRUE;
      }
    }
  }
  return copied;
}

NLM_EXTERN Nlm_Boolean LIBCALL NodeListWrite (ValNodePtr head, Nlm_Int2 item, Nlm_VoidPtr ptr, size_t size)

{
  Nlm_Boolean  copied;
  ValNodePtr   vnp;

  copied = FALSE;
  if (head != NULL && item > 0 && ptr != NULL) {
    vnp = NodeListFind (head, item, TRUE);
    copied = Nlm_WriteToNode (vnp, ptr, size);
  }
  return copied;
}

NLM_EXTERN Nlm_Boolean LIBCALL NodeListAppend (ValNodePtr head, Nlm_VoidPtr ptr, size_t size)

{
  Nlm_Boolean  copied;
  ValNodePtr   vnp;

  copied = FALSE;
  if (head != NULL && ptr != NULL) {
    vnp = ValNodeNew (head);
    copied = Nlm_WriteToNode (vnp, ptr, size);
  }
  return copied;
}

NLM_EXTERN Nlm_Boolean LIBCALL NodeListInsert (ValNodePtr head, Nlm_Int2 item, Nlm_VoidPtr ptr, size_t size)

{
  Nlm_Boolean  copied;
  ValNodePtr   prev;
  ValNodePtr   vnp;

  copied = FALSE;
  if (head != NULL && item > 0 && ptr != NULL) {
    if (item > 1) {
      prev = NodeListFind (head, (Nlm_Int2)(item - 1), FALSE);
    } else {
      prev = head;
    }
    if (prev != NULL) {
      vnp = ValNodeNew (NULL);
      if (vnp != NULL) {
        vnp->next = prev->next;
        prev->next = vnp;
        copied = Nlm_WriteToNode (vnp, ptr, size);
      }
    }
  }
  return copied;
}

NLM_EXTERN Nlm_Boolean LIBCALL NodeListReplace (ValNodePtr head, Nlm_Int2 item, Nlm_VoidPtr ptr, size_t size)

{
  Nlm_Boolean  copied;
  ValNodePtr   vnp;

  copied = FALSE;
  if (head != NULL && item > 0 && ptr != NULL) {
    vnp = NodeListFind (head, item, FALSE);
    copied = Nlm_WriteToNode (vnp, ptr, size);
  }
  return copied;
}

NLM_EXTERN Nlm_Boolean LIBCALL NodeListDelete (ValNodePtr head, Nlm_Int2 item)

{
  Nlm_Boolean  deleted;
  ValNodePtr   prev;
  ValNodePtr   vnp;

  deleted = FALSE;
  if (head != NULL && item > 0) {
    if (item > 1) {
      prev = NodeListFind (head, (Nlm_Int2)(item - 1), FALSE);
    } else {
      prev = head;
    }
    if (prev != NULL) {
      vnp = prev->next;
      if (vnp != NULL) {
        prev->next = vnp->next;
        Nlm_MemFree (vnp->data.ptrvalue);
        Nlm_MemFree (vnp);
        deleted = TRUE;
      }
    }
  }
  return deleted;
}


/*****************************************************************************
*
*   End Of Node List Functions
*
*****************************************************************************/

#ifdef OS_MAC

// 2001-03-22:  Joshua Juran
// C2PStr() and P2CStr() do not exist in Carbon, so we roll our own.
# if TARGET_API_MAC_CARBON
static void C2PStr(char *ioStr)
{
	size_t len = strlen(ioStr);
	if (len > 255) {
		len = 255;
	}
	memmove(ioStr + 1, ioStr, len);
	ioStr[0] = len;
}

static void P2CStr(StringPtr ioStr)
{
	Byte len = ioStr[0];
	memmove(ioStr, ioStr + 1, len);
	ioStr[len] = '\0';
}
# endif

/*  p_churchill 12/99 removed conditional compilation support for 
 *  THINKC and MPW
 */
void Nlm_CtoPstr (Nlm_CharPtr str)

{
  C2PStr ((char *) str);
}

void Nlm_PtoCstr (Nlm_CharPtr str)

{
  P2CStr ((StringPtr) str);
}
#endif



NLM_EXTERN Nlm_Uint2 Nlm_SwitchUint2 (Nlm_Uint2 value)

{
	Nlm_Uint2  m;
	m  = ((value & (Nlm_Uint2)0xFF00) >> 8);
	m |= ((value & (Nlm_Uint2)0x00FF) << 8);
	return m;
}

NLM_EXTERN void Nlm_SwitchUint2Buff (Nlm_Uint2 *buff, int count)
{
	Nlm_Uint2 *ptr, n, m;
	
	for (ptr=buff; count >0; --count)
	{
		n = *ptr;
		m  = ((n & (Nlm_Uint2)0xFF00) >> 8);
		m |= ((n & (Nlm_Uint2)0x00FF) << 8);
		*ptr++ = m;
	}
}

NLM_EXTERN unsigned long  Nlm_SwitchLong (unsigned long value)
{
	unsigned long  m;
	m  = ((value & (unsigned long)0xFF000000) >> 24);
	m |= ((value & (unsigned long)0x00FF0000) >> 8);
	m |= ((value & (unsigned long)0x0000FF00) << 8);
	m |= ((value & (unsigned long)0x000000FF) << 24);
	return m;
}

NLM_EXTERN void Nlm_SwitchLongBuff (unsigned long *buff, int count)
{
	unsigned long *ptr, n, m;
	
	for (ptr=buff; count >0; --count)
	{
		n = *ptr;
		m  = ((n & (unsigned long)0xFF000000) >> 24);
		m |= ((n & (unsigned long)0x00FF0000) >> 8);
		m |= ((n & (unsigned long)0x0000FF00) << 8);
		m |= ((n & (unsigned long)0x000000FF) << 24);
		*ptr++ = m;
	}  
}

NLM_EXTERN Nlm_Uint4  Nlm_SwitchUint4 (Nlm_Uint4 value)
{
	Nlm_Uint4  m;
	m  = ((value & (Nlm_Uint4)0xFF000000) >> 24);
	m |= ((value & (Nlm_Uint4)0x00FF0000) >> 8);
	m |= ((value & (Nlm_Uint4)0x0000FF00) << 8);
	m |= ((value & (Nlm_Uint4)0x000000FF) << 24);
	return m;
}

NLM_EXTERN void Nlm_SwitchUint4Buff (Nlm_Uint4 *buff, int count)
{
	Nlm_Uint4 *ptr, n, m;
	
	for (ptr=buff; count >0; --count)
	{
		n = *ptr;
		m  = ((n & (Nlm_Uint4)0xFF000000) >> 24);
		m |= ((n & (Nlm_Uint4)0x00FF0000) >> 8);
		m |= ((n & (Nlm_Uint4)0x0000FF00) << 8);
		m |= ((n & (Nlm_Uint4)0x000000FF) << 24);
		*ptr++ = m;
	}  
}


/* the following ListXXX() functions previously resided in ni_list.c */

/*
 * Purpose:     Insert an item as the next element in a doubly linked list(ring)
 *
 * Parameters:
 *   elem           Next element to be inserted; this is data only,not a NodePtr
 *   ap             Insertion point
 *
 * Returns:
 *                The newly allocated NodePtr, containing forward and backward
 *                pointers and a pointer to elem
 *
 *
 * Description:
 *              Allocate the necessary memory for a "Node", attach the
 *              caller's data to that Node, and insert the Node after the
 *              specified node in the list, maintaining the integrity of
 *              a doubly-linked ring. If there are no other items in the
 *              ring, create a "minimal" ring which consists of the single
 *              Node pointing to itself in both directions.
 *
 * Note:
 *              Most "list" data is actually stored in a doubly-linked ring, as
 *              shown below. Furthermore, note that each node only contains a
 *              pointer to the actual data in the list, rather than the actual
 *              data itself.
 *
 *          +------------------------------------------------------------------+
 *          ^                                                                  |
 *          |       +-------------------------------------------------------+  |
 *          |       |                                                       ^  |
 *          |       V                                                       |  |
 *          |   +-------+       +-------+                       +-------+   |  |
 *          |   | next  |------>| next  |------> ...    ------->| next  |-->+  |
 *          |   +-------+       +-------+                       +-------+      |
 *          +<--| last  |<------| last  |<------ ...    <-------| last  |<-----+
 *              +-------+       +-------+                       +-------+
 *              | elem  |       | elem  |                       | elem  |
 *              +-------+       +-------+                       +-------+
 *                  |               |                               |
 *                  |               |                               |
 *                  V               V                               V
 *              +-------+       +-------+                       +-------+
 *              | actual|       | actual|                       | actual|
 *              | data  |       | data  |                       | data  |
 *              +-------+       +-------+                       +-------+
 */

NLM_EXTERN NodePtr  LIBCALL
ListInsert(Nlm_VoidPtr elem, NodePtr ap)                     /* ptr to node to insert after */
{
    NodePtr             np;
    
    if (elem == NULL)
        return NULL;
    
    np = (NodePtr) MemNew(sizeof(Node));
    np->elem = elem;
    
    if (ap == NULL) {           /* no nodes in list */
        np->last = np;
        np->next = np;
        return np;
    }
    else {                              /* 1 or more nodes in list */
        np->next = ap->next;
        ap->next = np;
        np->next->last = np;
        np->last = ap;
        return np;
    }
} /* ListInsert */



/*
 * Purpose:     Insert an item as the previous element in a doubly linked
 *              list(ring)
 *
 * Parameters:
 *   elem           Next element to be inserted; this is data only,not a NodePtr
 *   ap             Insertion point
 *
 * Returns:
 *                The newly allocated NodePtr, containing forward and backward
 *                pointers and a pointer to elem
 *
 *
 * Description:
 *              Insert the specified item into the ring, before the specified
 *              insertion point. In the case where the specified insertion
 *              point was NULL, this is equivalent to ListInsert().
 */

NLM_EXTERN NodePtr  LIBCALL
ListInsertPrev(Nlm_VoidPtr elem, NodePtr ap)                     /* ptr to node to insert before */
{
    NodePtr             np;
    
    np = ap;
    if (ap != NULL)
        ap = ap->last;          /* previous node */
    
    ap = ListInsert(elem, ap);
    return (np == NULL) ? ap : np;
} /* ListInsertPrev */



/*
 * Purpose:     Delete a single node from a list or ring
 *
 * Parameters:
 *   np             Node to be deleted
 *
 * Returns:
 *                A pointer to the "next" node in the list/ring, after the
 *                deleted node.
 *
 *
 * Description:
 *              Delete the specified node from a list or ring. It is the
 *              responsibility of the caller to free the memory associated
 *              with the "elem" (data), if appropriate.
 */

NLM_EXTERN NodePtr  LIBCALL
ListDelete(NodePtr np)
{
    NodePtr             nextnode, lastnode;
    
    if (np == NULL)
        return NULL;
    
    nextnode = np->next;
    lastnode = np->last;
    
    if (nextnode == NULL && lastnode == NULL)   /* only node in a list */
        ;
    else if (nextnode == NULL) {                /* last in a list */
        np->last->next = NULL;
        nextnode = np->last;
    }
    else if (lastnode == NULL) {                /* first in a list */
        np->next->last = NULL;
        nextnode = np->next;
    }
    else if (np == nextnode)                    /* last in a ring */
        nextnode = NULL;
    else {                                      /* node with both neighbors */
        np->last->next = nextnode;
        np->next->last = np->last;
    }
    
    MemFree(np);                        /* assumes element memory has been freed */
    return nextnode;
} /* ListDelete */



/*
 * Purpose:     Get the next element from a list or ring (non-destructively)
 *
 * Parameters:
 *   np             Node before the node to be selected
 *
 * Returns:
 *                A pointer to the "next" node in the list/ring (or NULL
 *                if the list/ring was NULL). Note that for a list, the
 *                returned value can also be NULL.
 *
 *
 * Description:
 *              Return the "next" node in the list or rin.g
 */

NLM_EXTERN NodePtr  LIBCALL
ListGetNext(NodePtr np)
{
    if (np == NULL)
        return NULL;
    return np->next;
} /* ListGetNext */



/*
 * Purpose:     Swap two adjacent nodes in a list or ring
 *
 * Parameters:
 *   np1            "Prior" node
 *   np2            "Next" node
 *
 *
 * Description:
 *              Swap the two specified elements, provided that they are
 *              adjacent, and np1 precedes np2.
 */

NLM_EXTERN void  LIBCALL
ListSwapAdj(NodePtr np1, NodePtr np2)       /* priornode, nextnode */
{
    if (np1 == NULL || np2 == NULL || np1->next->last != np1) /* must be sane */
        return;

    if (np1->next != np2 || np2->last != np1)             /* must be in order */
        return;
    
    if (np1->last != NULL)
        np1->last->next = np2;
    
    if (np2->next != NULL)
        np2->next->last = np1;
    
    np1->next = np2->next;
    np2->last = np1->last;
    
    np1->last = np2;
    np2->next = np1;
} /* ListSwapAdj */



/*
 * Purpose:     Sort the specified ring/list
 *
 * Parameters:
 *   head           Head of the list to be sorted
 *   cmpfunc        Comparison function (return values are like memcmp())
 *   order          ASCEND or DESCEND
 *
 * Returns:
 *              A pointer to the first element of the sorted ring or list
 *
 *
 * Description:
 *              Sort the specified list, in place, using bubble sort, and
 *              the specified comparison function. Determine prior to sorting
 *              whether this is a list or a ring. If it's a ring, break the
 *              ring prior to sorting, and restore it to a ring topology
 *              after sorting has been completed.
 */

NLM_EXTERN NodePtr  LIBCALL
ListSort(NodePtr head, int (*cmpfunc )PROTO ((NodePtr, NodePtr )), int order)
                       /* 0 if equal, LT 0 if 1st element > 2nd element */
{
    NodePtr     np;
    Nlm_Boolean sorted = FALSE, ring;
    int         result;
    
    if (head == NULL)
        return NULL;
    if (head->last == NULL)
        ring = FALSE;
    else
        ring = TRUE;
    if (ring)
        ListBreakRing(head);
    
    /* just bubble sort for now */
    
    while (! sorted) {
        np = head;
        sorted = TRUE;
        
        while (np->next != NULL) {
            result = (*cmpfunc)(np, np->next);
            if ((result > 0 && order == ASCEND) || (result < 0 && order == DESCEND)) {
                sorted = FALSE;
                if (np == head)
                    head = np->next;    /* keep head pointing at 1st element */
                ListSwapAdj(np, np->next);
            }
            else
                np = np->next;
        }
    }
    
    if (ring)
        ListConnectRing(head);
    return head;        /* ptr to first element */
} /* ListSort */



/*
 * Purpose:     Break the specified ring into a non-circular (linear) list
 *
 * Parameters:
 *   np             Head of the ring to be broken
 *
 *
 * Description:
 *              Break the specified ring between its head and tail.
 *
 * Note:
 *              This function may be called safely (without effect) if the
 *              passed parameter is already a list, rather than a ring.
 */

NLM_EXTERN void  LIBCALL
ListBreakRing(NodePtr np)
{
    if (np == NULL)
        return;
    if (np->last == NULL)
        return;
    
    np->last->next = NULL;
    np->last = NULL;
} /* ListBreakRing */



/*
 * Purpose:     Convert a list into a ring.
 *
 * Parameters:
 *   head           Head of the list to be connected
 *
 *
 * Description:
 *              Connect the specified list between its head and tail, producing
 *              a ring.
 *
 * Note:
 *              This function may be called safely (without effect) if the
 *              passed parameter is already a ring, rather than a list.
 */

NLM_EXTERN void  LIBCALL
ListConnectRing(NodePtr head)
{
    NodePtr     np;
    
    if (head == NULL)
        return;
    
    np = head;
    
    while (np->next != NULL) {
        np = np->next;
        if (np == head)
            return;
    }
    
    np->next = head;
    head->last = np;
} /* ListConnectRing */


/*
 * Purpose:     Copy a list where the list elements are character strings
 *
 * Parameters:
 *   strlist        List to be copied
 *
 * Returns:
 *              A copy of the original list (which may be NULL)
 *
 *
 * Description:
 *              Create a list which is a copy of the original list, and
 *              also make copies of the strings.
 *
 * Note:
 *              There is no obvious way to make a generic list copying
 *              routine, because, in general, the length of each list
 *              element is unknown. This is a simple case where it is
 *              easy to copy a list.
 */

NLM_EXTERN NodePtr LIBCALL
ListStrCopy (NodePtr strlist)
{
    NodePtr newlist = NULL;
    NodePtr np = strlist;
    Nlm_CharPtr stringtext;

    if (strlist == NULL)
        return NULL;

    do {
        stringtext = StringSave((Nlm_CharPtr) np->elem);
        newlist = ListInsert(stringtext, newlist);
        np = ListGetNext(np);
    } while (np != NULL && np != strlist);

    return newlist->next; /* points to 1st element in new list */
}


/*
 * Purpose:     Delete a list where the list elements are character strings
 *
 * Parameters:
 *   np             List to be deleted
 *
 *
 * Description:
 *              Delete the list nodes and the character string data associated
 *              with each node.
 *
 * Note:
 *              This routine will work for any list element which is a single
 *              block of memory. However, it will not work in the more general
 *              case where a list element in turn references other memory
 *              which must also be freed.
 */

NLM_EXTERN void LIBCALL
ListStrDel (NodePtr np)
{
    while (np != NULL)
    {
        MemFree (np->elem);
        np = ListDelete(np);
    }
}


NLM_EXTERN const Nlm_Char* Nlm_PlatformName(void)
{
/* Mac */
#if defined(OS_MAC)
#  if defined(PROC_PPC)
  return "PowerPC";
#  elif defined(PROC_MC680X0)
  return "MacOS_68K";
#  else
  return "MacOS";
#  endif

/* VMS */
#elif defined(OS_VMS)
#  if defined(OS_AXP_VMS)
  return "AXP/OpenVMS";
#  else
  return "VMS";
#  endif

/* UNIX */
#elif defined(OS_UNIX)
#  if defined(PROC_IBM370)
  return "IBM370_AIX";
#  elif defined(OS_UNIX_SUN)
  return "SunOS";
#  elif defined(OS_UNIX_SOL)
  return "Solaris";
#  elif defined(OS_UNIX_OSF1) && defined(PROC_ALPHA)
  return "Alpha_OSF1";
#  elif defined(COMP_AUX)
  return "Mac_AUX";
#  elif defined(COMP_CRAY) && defined(PROC_YMP)
  return "Cray";
#  elif defined(PROC_CONVEX)
  return "Convex";
#  elif defined(PROC_HPPA)
  return "HPUX";
#  elif defined(OS_UNIX_NEXT)
  return "NEXT";
#  elif defined(PROC_MIPS)
  return "SGI_MIPS";
#  elif defined(OS_UNIX_ULTRIX)
  return "ULTRIX";
#  elif defined(OS_UNIX_SYSV) && defined(PROC_SPARC)
  return "SystemV_SPARC";
#  elif defined(OS_UNIX_AIX)
  return "AIX";
#  elif defined(OS_UNIX_LINUX)
#    if defined(PROC_ALPHA)
  return "LINUX_ALPHA";
#    else
  return "LINUX";
#    endif
#  elif defined(OS_UNIX_NETBSD)
  return "NetBSD";
#  elif defined(OS_UNIX_FREEBSD)
  return "FreeBSD";
#  else
  return "UNIX";
#  endif

/* PC */
#elif defined(OS_MSWIN)
#  if defined(WIN16)
  return "WIN16";
#  elif defined(WIN32)
  return "WIN32";
#  else
  return "MSWIN";
#  endif

/* NT */
#elif defined(OS_WINNT)
  return "WIN_NT";

/* Unknown */
#else
  return "Unknown";

#endif
}


/**
 * MD5 stuff (used for security purposes)
 */

/*
 * Note: this code is harmless on little-endian machines.
 */
static
void
byteReverse(Nlm_UcharPtr buf, Nlm_Uint4 longs)
{
    Nlm_Uint4 t;
    do {
	t = (Nlm_Uint4) ((unsigned) buf[3] << 8 | buf[2]) << 16 |
	    ((unsigned) buf[1] << 8 | buf[0]);
	*(Nlm_Uint4Ptr) buf = t;
	buf += 4;
    } while (--longs);
}

/*
 * Start MD5 accumulation.  Set bit count to 0 and buffer to mysterious
 * initialization constants.
 */
NLM_EXTERN void LIBCALL
Nlm_MD5Init(Nlm_MD5ContextPtr ctx)
{
    ctx->buf[0] = 0x67452301;
    ctx->buf[1] = 0xefcdab89;
    ctx->buf[2] = 0x98badcfe;
    ctx->buf[3] = 0x10325476;

    ctx->bits[0] = 0;
    ctx->bits[1] = 0;
}

/*
 * Update context to reflect the concatenation of another buffer full
 * of bytes.
 */
NLM_EXTERN void LIBCALL
Nlm_MD5Update(Nlm_MD5ContextPtr ctx, Nlm_UcharPtr buf, Nlm_Uint4 len)
{
    Nlm_Uint4 t;

    /* Update bitcount */

    t = ctx->bits[0];
    if ((ctx->bits[0] = t + ((Nlm_Uint4) len << 3)) < t)
	ctx->bits[1]++;		/* Carry from low to high */
    ctx->bits[1] += len >> 29;

    t = (t >> 3) & 0x3f;	/* Bytes already in shsInfo->data */

    /* Handle any leading odd-sized chunks */

    if (t) {
	Nlm_UcharPtr p = (Nlm_UcharPtr) ctx->in + t;

	t = 64 - t;
	if (len < t) {
	    memcpy(p, buf, len);
	    return;
	}
	memcpy(p, buf, t);
#ifdef IS_BIG_ENDIAN
	byteReverse(ctx->in, 16);
#endif
	Nlm_MD5Transform(ctx->buf, (Nlm_Uint4Ptr) ctx->in);
	buf += t;
	len -= t;
    }
    /* Process data in 64-byte chunks */

    while (len >= 64) {
	memcpy(ctx->in, buf, 64);
#ifdef IS_BIG_ENDIAN
	byteReverse(ctx->in, 16);
#endif
	Nlm_MD5Transform(ctx->buf, (Nlm_Uint4Ptr) ctx->in);
	buf += 64;
	len -= 64;
    }

    /* Handle any remaining bytes of data. */
    memcpy(ctx->in, buf, len);
}

/*
 * Final wrapup - pad to 64-byte boundary with the bit pattern 
 * 1 0* (64-bit count of bits processed, MSB-first)
 */
NLM_EXTERN void LIBCALL
Nlm_MD5Final(Nlm_MD5ContextPtr ctx, Nlm_Uchar digest[16])
{
    Nlm_Uint4 count;
    Nlm_UcharPtr p;

    /* Compute number of bytes mod 64 */
    count = (ctx->bits[0] >> 3) & 0x3F;

    /* Set the first char of padding to 0x80.  This is safe since there is
       always at least one byte free */
    p = ctx->in + count;
    *p++ = 0x80;

    /* Bytes of padding needed to make 64 bytes */
    count = 64 - 1 - count;

    /* Pad out to 56 mod 64 */
    if (count < 8) {
	/* Two lots of padding:  Pad the first block to 64 bytes */
	memset(p, 0, count);
#ifdef IS_BIG_ENDIAN
	byteReverse(ctx->in, 16);
#endif
	Nlm_MD5Transform(ctx->buf, (Nlm_Uint4Ptr) ctx->in);

	/* Now fill the next block with 56 bytes */
	memset(ctx->in, 0, 56);
    } else {
	/* Pad block to 56 bytes */
	memset(p, 0, count - 8);
    }
#ifdef IS_BIG_ENDIAN
	byteReverse(ctx->in, 14);
#endif

    /* Append length in bits and transform */
    ((Nlm_Uint4Ptr) ctx->in)[14] = ctx->bits[0];
    ((Nlm_Uint4Ptr) ctx->in)[15] = ctx->bits[1];

    Nlm_MD5Transform(ctx->buf, (Nlm_Uint4Ptr) ctx->in);
#ifdef IS_BIG_ENDIAN
    byteReverse((Nlm_UcharPtr) ctx->buf, 4);
#endif
    memcpy(digest, ctx->buf, 16);
    memset(ctx, 0, sizeof(ctx));	/* In case it's sensitive */
}

/* The four core functions - F1 is optimized somewhat */

/* #define F1(x, y, z) (x & y | ~x & z) */
#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

/* This is the central step in the MD5 algorithm. */
#define MD5STEP(f, w, x, y, z, data, s) \
	( w += f(x, y, z) + data,  w = w<<s | w>>(32-s),  w += x )

/*
 * The core of the MD5 algorithm, this alters an existing MD5 hash to
 * reflect the addition of 16 longwords of new data.  MD5Update blocks
 * the data and converts bytes into longwords for this routine.
 */
NLM_EXTERN void LIBCALL
Nlm_MD5Transform(Nlm_Uint4 buf[4], Nlm_Uint4 in[16])
{
    register Nlm_Uint4 a, b, c, d;

    a = buf[0];
    b = buf[1];
    c = buf[2];
    d = buf[3];

    MD5STEP(F1, a, b, c, d, in[0] + 0xd76aa478, 7);
    MD5STEP(F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
    MD5STEP(F1, c, d, a, b, in[2] + 0x242070db, 17);
    MD5STEP(F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
    MD5STEP(F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
    MD5STEP(F1, d, a, b, c, in[5] + 0x4787c62a, 12);
    MD5STEP(F1, c, d, a, b, in[6] + 0xa8304613, 17);
    MD5STEP(F1, b, c, d, a, in[7] + 0xfd469501, 22);
    MD5STEP(F1, a, b, c, d, in[8] + 0x698098d8, 7);
    MD5STEP(F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
    MD5STEP(F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
    MD5STEP(F1, b, c, d, a, in[11] + 0x895cd7be, 22);
    MD5STEP(F1, a, b, c, d, in[12] + 0x6b901122, 7);
    MD5STEP(F1, d, a, b, c, in[13] + 0xfd987193, 12);
    MD5STEP(F1, c, d, a, b, in[14] + 0xa679438e, 17);
    MD5STEP(F1, b, c, d, a, in[15] + 0x49b40821, 22);

    MD5STEP(F2, a, b, c, d, in[1] + 0xf61e2562, 5);
    MD5STEP(F2, d, a, b, c, in[6] + 0xc040b340, 9);
    MD5STEP(F2, c, d, a, b, in[11] + 0x265e5a51, 14);
    MD5STEP(F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
    MD5STEP(F2, a, b, c, d, in[5] + 0xd62f105d, 5);
    MD5STEP(F2, d, a, b, c, in[10] + 0x02441453, 9);
    MD5STEP(F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
    MD5STEP(F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
    MD5STEP(F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
    MD5STEP(F2, d, a, b, c, in[14] + 0xc33707d6, 9);
    MD5STEP(F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
    MD5STEP(F2, b, c, d, a, in[8] + 0x455a14ed, 20);
    MD5STEP(F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
    MD5STEP(F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
    MD5STEP(F2, c, d, a, b, in[7] + 0x676f02d9, 14);
    MD5STEP(F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

    MD5STEP(F3, a, b, c, d, in[5] + 0xfffa3942, 4);
    MD5STEP(F3, d, a, b, c, in[8] + 0x8771f681, 11);
    MD5STEP(F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
    MD5STEP(F3, b, c, d, a, in[14] + 0xfde5380c, 23);
    MD5STEP(F3, a, b, c, d, in[1] + 0xa4beea44, 4);
    MD5STEP(F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
    MD5STEP(F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
    MD5STEP(F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
    MD5STEP(F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
    MD5STEP(F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
    MD5STEP(F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
    MD5STEP(F3, b, c, d, a, in[6] + 0x04881d05, 23);
    MD5STEP(F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
    MD5STEP(F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
    MD5STEP(F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
    MD5STEP(F3, b, c, d, a, in[2] + 0xc4ac5665, 23);

    MD5STEP(F4, a, b, c, d, in[0] + 0xf4292244, 6);
    MD5STEP(F4, d, a, b, c, in[7] + 0x432aff97, 10);
    MD5STEP(F4, c, d, a, b, in[14] + 0xab9423a7, 15);
    MD5STEP(F4, b, c, d, a, in[5] + 0xfc93a039, 21);
    MD5STEP(F4, a, b, c, d, in[12] + 0x655b59c3, 6);
    MD5STEP(F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
    MD5STEP(F4, c, d, a, b, in[10] + 0xffeff47d, 15);
    MD5STEP(F4, b, c, d, a, in[1] + 0x85845dd1, 21);
    MD5STEP(F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
    MD5STEP(F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
    MD5STEP(F4, c, d, a, b, in[6] + 0xa3014314, 15);
    MD5STEP(F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
    MD5STEP(F4, a, b, c, d, in[4] + 0xf7537e82, 6);
    MD5STEP(F4, d, a, b, c, in[11] + 0xbd3af235, 10);
    MD5STEP(F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
    MD5STEP(F4, b, c, d, a, in[9] + 0xeb86d391, 21);

    buf[0] += a;
    buf[1] += b;
    buf[2] += c;
    buf[3] += d;
}

/*----- Here is simplified MD4 algorithm, that used in Blast E-mail server
  and formatdb ------ */

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a, b, c, d, x, s) { \
    (a) += F ((b), (c), (d)) + (x); \
	(a) = ROTATE_LEFT ((a), (s)); \
}
#define GG(a, b, c, d, x, s) { \
    (a) += G ((b), (c), (d)) + (x) + (Nlm_Uint4)0x5a827999; \
	(a) = ROTATE_LEFT ((a), (s)); \
}
#define HH(a, b, c, d, x, s) { \
    (a) += H ((b), (c), (d)) + (x) + (Nlm_Uint4)0x6ed9eba1; \
	(a) = ROTATE_LEFT ((a), (s)); \
}

/* This function calculates checksum from a buffer */
Nlm_Uint4 Nlm_GetChecksum(Nlm_CharPtr p)
{
    Nlm_Int4 len;
    struct Hash {
	Nlm_Uint4 x1;
	Nlm_Uint4 x2;
	Nlm_Uint4 x3;
	Nlm_Uint4 x4;
	Nlm_Uint4 x5;
	Nlm_Uint4 x6;
	Nlm_Uint4 x7;
	Nlm_Uint4 x8;
	Nlm_Uint4 x9;
	Nlm_Uint4 x10;
	Nlm_Uint4 x11;
	Nlm_Uint4 x12;
    } Hash; 

    Nlm_Uint4 a = 0x67452301;
    Nlm_Uint4 b = 0xefcdab89;
    Nlm_Uint4 c = 0x98badcfe;
    Nlm_Uint4 d = 0x10325476;

    if (p == NULL || p[0] == NULLB)
	return 0;

    MemSet(&Hash, 0, sizeof(Hash));
    MemCopy(&Hash, p, 
	    (len = StringLen(p)) > sizeof(Hash)? sizeof(Hash) : len);

    FF (a, b, c, d, Hash.x1, 3);   /* 1 */
    FF (d, a, b, c, Hash.x2, 7);   /* 2 */
    FF (c, d, a, b, Hash.x3, 11);  /* 3 */
    FF (b, c, d, a, Hash.x4, 19);  /* 4 */

    GG (a, b, c, d, Hash.x5, 3);   /* 17 */
    GG (d, a, b, c, Hash.x6, 5);   /* 18 */
    GG (c, d, a, b, Hash.x7, 9);   /* 19 */
    GG (b, c, d, a, Hash.x8, 13);  /* 20 */

    HH (b, c, d, a, Hash.x9, 15);  /* 40 */
    HH (a, b, c, d, Hash.x10, 3);  /* 41 */
    HH (d, a, b, c, Hash.x11, 9);  /* 42 */
    HH (c, d, a, b, Hash.x12, 11); /* 43 */

    return (a+b+c+d);
}

/*  ----- End of simplified MD4 algorithm ------ */
