/*
	PROJECT: Medline database.
	MODULE:	 cspatch
	FILES:	 cspatch.c, cstopatch.c, patchtoiso.c, cspatch.h (this one)

	This module contains the definitions needed for building and
	interpreting patch commands used to convert the Medline character
	set (EBCDIC) into the ASCII character set with a ISO patch
	instruction string to indicate how to translate the ASCII into
	ISO-8859 characters.  (Ie. instructions on how to add the various
	accent marks which aren't available in ASCII).

	Work started: 10 June 1991, Rand Huntzinger

	TODO:	Consider using error logging package for usage error.
	Original version completed: 24 July 1991, Rand S. Huntzinger
*
*
* RCS Modification History:
* $Log: patchtoiso.h,v $
* Revision 6.0  1997/08/25 18:36:54  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:55:51  epstein
* add RCS log revision history
*
*/

#ifndef	DEF_MODULE_CSPATCH
#define	DEF_MODULE_CSPATCH

#include "charset.h"

/*
	Basic OPCODE interpretations.

	76543210
	========
	00xxxxxx	PATCH opcode.
	01xxxxxx	SPECIAL opcode group.
	10xxxxxx	OFFSET1 opcode (6 bit offset)
	11xxxxxx	OFFSET2 opcode (14 bit offset)
*/

#define	POP_BASIC_MASK		(0xc0)	/* Mask basic opcode - bits 7&6 */
#define	POP_BASIC_ARGMASK	(0x3f)	/* Basic opcode argument (6 bits) */
#define	POP_PATCH_CODE		(0x00)	/* PATCH opcode */
#define	POP_SPECIAL_GRP		(0x40)	/* SPECIAL opcode group */
#define	POP_OFFSET1_CODE	(0x80)	/* OFFSET1 opcode */
#define	POP_OFFSET2_CODE	(0xc0)	/* OFFSET2 opcode */

/*
	Special opcode group interpretation

	76543210
	========
	010xxxxx	MISC code group.
	011xxxxx	CHARSET (32 character set codes)
*/

#define	POP_SPEC_MASK		(0xe0)	/* SPECIAL opcodes */
#define	POP_MISC_GRP		(0x40)	/* MISC opcode group */
#define	POP_CSET_CODE		(0x60)	/* CHARSET opcode (not implemented) */
#define	POP_SPEC_ARGMASK	(0x1f)	/* Arguments for SPEC codes */

/*
	Misc opcode group interpretation

	76543210
	========
	01000xxx	SIMPLE opcode group.
	01001xxx	OMIT opcode (original codes, next 1..8 bytes).
	01010xxx	INVALID opcode (original codes, next 1..8 bytes)
	01011xxx	NOISO opcode (next 1..8 -> next code)
*/
#define	POP_MISC_MASK		(0xf8)	/* MISC opcode mask */
#define	POP_SIMPLE_GRP		(0x40)	/* SIMPLE opcode group */
#define	POP_OMIT_CODE		(0x48)	/* OMIT opcode */
#define	POP_INVALID_CODE	(0x50)	/* INVALID opcode */
#define	POP_NOISO_CODE		(0x58)	/* NOISO opcode */
#define	POP_MISC_ARGMASK	(0x07)	/* Arguments for MISC codes */

/*
	Simple opcode group interpretation
*/

#define	POP_SIMPLE_MASK		(0xff)	/* In case we need to add groups */
#define	POP_STOP_CODE		(0x40)	/* STOP opcode (end of patch list) */
/* NOTE: Codes 0x41..0x47 are undefined - will be treated as NOP's */
#define	POP_NOP_CODE		(0x00)	/* Official NOP - a variant of PATCH */

/*
	PATCH opcode interpretation

	00dddiii		Where ddd is the 3-bit delete count and
				iii is the 3-bit insertion count.
*/

#define	POP_PATCH_DELETE(C)	(((C) >> 3) & 0x7)	/* Bits 3..5 */
#define	POP_PATCH_INSERT(C)	((C) & 0x7)		/* Bits 0..2 */

/*
	Opcode construction macros.
*/

#define	POP_MAKE_PATCH(D,I) \
	( POP_PATCH_CODE | (((D) & 0x7) << 3) | ((D) & 7) )
#define	POP_MAKE_BASIC(C,O) \
	( (C) | ((O) & POP_BASIC_ARGMASK) )
#define	POP_MAKE_CHARSET(C) \
	( POP_CSET_CODE | ((C) & POP_SPEC_ARGMASK ) )
#define	POP_MAKE_MISC(C,A) \
	((C) | ((A) & POP_MISC_ARGMASK))

/*
	Define the Opcode classification codes.  Used to determine the
	grouping of opcodes.  The most important distinction is class ISO
	(which actually does a patch) and the others.
*/

#define	POP_CLASS_ISO		(1 << 0)	/* PATCH */
#define	POP_CLASS_CONTROL	(1 << 1)	/* OFFSET1, OFFSET2, STOP */
#define	POP_CLASS_INVALID	(1 << 2)	/* INVALID */
#define	POP_CLASS_OMIT		(1 << 3)	/* OMIT */
#define	POP_CLASS_ERROR		(1 << 4)	/* Error in the patch string */

/* Define the bits which we need if we're going to store a patch in the
   database */

#define	POP_PATCH_REQUIRED (POP_CLASS_ISO | POP_CLASS_INVALID | POP_CLASS_OMIT)

/* Externals */

int	ApplyIsoPatches( char *in, int in_size,
		char *out, int out_size, unsigned char *patches );

#endif /* DEF_MODULE_CSPATCH */
