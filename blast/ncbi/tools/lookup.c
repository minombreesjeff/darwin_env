static char const rcsid[] = "$Id: lookup.c,v 6.58 2004/11/15 22:28:41 kans Exp $";

/*
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
*/
/*****************************************************************************

File name: lookup.c

Author: Tom Madden

Contents: Functions to store "hashed" word hits and their positions and
	in arrays and look these up again.

Detailed Contents:

        - the number of bits required is given by ln(alphabet size)/ln(2).

	- the number of bits times the wordsize must be less than 32.

        - contiguous words can be of any length, discontiguous words can
	only have the patten XX0X and be of length three, or word-width of
	four.

******************************************************************************/

/*******************************************************************************
* File Name: lookup.c
*
* Author: Tom Madden
*
* Version Creation Date:   10/26/95
*
* $Revision: 6.58 $
*
* File Description: 
*       Functions to store "words" from a query and perform lookups against
*	database sequences. 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* ==========================================================================
*
*
* RCS Modification History:
* $Log: lookup.c,v $
* Revision 6.58  2004/11/15 22:28:41  kans
* in MegaBlastBuildLookupTable, changed total_len (et al.) to Int8 and total_num (et al.) to Int4, from unsigned, which conflicted with pointer arguments to readdb_get_totals_ex
*
* Revision 6.57  2004/11/15 21:35:35  coulouri
*
* Manually overriding the effective database length can cause av_len to overflow an Int4; use Uint8 instead.
*
* Revision 6.56  2003/05/30 17:25:36  coulouri
* add rcsid
*
* Revision 6.55  2002/10/03 14:43:44  dondosha
* Bug fix for 2 sequences megablast
*
* Revision 6.54  2002/08/30 15:49:01  dondosha
* MegaBlastWordFinderDeallocate moved from mblast.c
*
* Revision 6.53  2002/06/07 18:50:43  dondosha
* Bug fix for Mega BLAST with two simultaneoue discontiguous word templates of length 21
*
* Revision 6.52  2002/05/17 21:40:13  dondosha
* Added 2 optimal Mega BLAST word templates for length 21
*
* Revision 6.51  2002/05/14 22:20:20  dondosha
* Renamed maximal discontiguous template type into optimal
*
* Revision 6.50  2002/04/09 18:19:47  dondosha
* Changed #ifdefs to conditionals, allowing different discontiguous templates in a single binary
*
* Revision 6.49  2002/04/01 22:28:29  dondosha
* Corrected syntax error for megablast with two-templates case
*
* Revision 6.48  2002/03/26 21:15:54  dondosha
* Added macro definitions to allow template length 21
*
* Revision 6.47  2002/03/05 17:39:39  dondosha
* Use reduced_wordsize instead of wordsize in lookup_find_init
*
* Revision 6.46  2002/02/19 22:35:06  dondosha
* Fixed uninitialized variable bug introduced by the two-template code
*
* Revision 6.45  2002/02/15 23:35:01  dondosha
* Allow hash table and two templates for megablast
*
* Revision 6.44  2001/12/28 20:46:21  dondosha
* 1. Mega BLAST related parameters moved into a separate structure
* 2. Environment variables for discontiguous words, etc. changed to options
* 3. Extension from discontiguous word with length 18 implemented
*
* Revision 6.43  2001/12/27 18:12:33  dondosha
* Fixed a bug caused by lack of Int4 to Int8 cast
*
* Revision 6.42  2001/12/10 22:54:27  dondosha
* Correction for determining the number of stacks to use in megablast
*
* Revision 6.41  2001/12/04 17:13:08  dondosha
* Made number of stacks for megablast word processing depend on query and database
*
* Revision 6.40  2001/10/12 21:32:46  dondosha
* Added discontiguous word capability to megablast
*
* Revision 6.39  2001/07/20 19:22:37  dondosha
* Added pv_array for megablast
*
* Revision 6.38  2000/09/29 22:17:42  dondosha
* Check if memory allocation fails in MegaBlastBuildLookupTable
*
* Revision 6.37  2000/08/28 17:13:56  dondosha
* Changed the bit indicating mask at hash only for megablast
*
* Revision 6.36  2000/07/10 17:18:58  dondosha
* Use several stacks in MegaBlast for speed up
*
* Revision 6.35  2000/05/31 14:06:11  dondosha
* Added warning messages when 12mers are masked in megablast
*
* Revision 6.34  2000/05/22 18:06:54  dondosha
* Fixed bug in masking at hash for megablast
*
* Revision 6.33  2000/05/17 17:38:26  dondosha
* Removed several debugging statements, introduced by previous revision
*
* Revision 6.32  2000/05/17 17:15:56  dondosha
* In megablast, clean the hash table from patterns with too many positions in query
*
* Revision 6.31  2000/04/10 17:27:15  madden
* Deallocate old lookup table memory before search
*
* Revision 6.30  2000/04/03 13:07:51  madden
* Fixed memory leak
*
* Revision 6.29  2000/03/07 21:59:53  madden
* Add return value of NULL for MegaBlastLookupTableDestruct
*
* Revision 6.28  2000/03/03 18:07:34  dondosha
* Added routine MegaBlastLookupTableDup plus cosmetic changes related to MegaBlast
*
* Revision 6.27  2000/03/02 17:24:34  dondosha
* Fixed handling of lower case masking in MegaBlast
*
* Revision 6.26  2000/02/23 20:41:39  dondosha
* Removed extern declaration for MegaBlastBuildLookupTable - now included from blast.h
*
* Revision 6.25  2000/02/17 19:02:10  shavirin
* Removed all references to absolete theCacheSize variable.
*
* Revision 6.24  2000/02/12 21:18:57  kans
* added prototype for MegaBlastBuildLookupTable - implemented in lookup.c, called from mblast.c
*
* Revision 6.23  2000/02/11 20:54:28  dondosha
* Added functions related to Webb Miller s lookup table
*
* Revision 6.22  2000/02/03 21:17:03  dondosha
* Fixed bug in lookup_new
*
* Revision 6.21  2000/02/01 21:08:48  dondosha
* Added mb_make_mod_lt and mb_lookup_position_aux_destruct - modifications for megablast
*
* Revision 6.20  2000/01/11 18:37:08  shavirin
* Added define DYNAMIC_CACHE to distinduish dynamic and static lookup.
*
* Revision 6.19  2000/01/11 17:58:50  shavirin
* Added new function make_mod_lt_new, which created dynamic lookup table
*
* Revision 6.18  2000/01/11 17:33:11  shavirin
* Added handling of the parameter theCacheSize.
*
* Revision 6.17  1999/12/21 21:33:23  shavirin
* Added parameter mod_lookup_table_memory to the lookup structure.
*
* Revision 6.15  1999/11/08 21:54:54  madden
* Fix typo
*
* Revision 6.14  1999/10/12 18:00:13  madden
* Add one to array lengths for purify errors.
*
* Revision 6.13  1999/10/06 14:25:20  madden
* Only allocate pv_array if needed
*
* Revision 6.12  1999/09/29 17:20:12  madden
* Deallocate ModLookupPositionPtr memory
*
* Revision 6.11  1999/09/29 13:28:44  madden
* Fill in pv_array correctly
*
* Revision 6.10  1999/09/28 20:14:33  madden
* Joerg changes to mimize cache misses
*
* Revision 6.9  1999/09/16 16:56:39  madden
* Changes to lookup_new for long words
*
* Revision 6.8  1999/09/16 14:16:27  madden
* lookup_find_init returns Uint1Ptr instead of CharPtr
*
* Revision 6.7  1999/04/27 15:40:12  madden
* Use aux lookup only for short words
*
* Revision 6.6  1998/09/22 16:27:47  madden
* Added function lookup_position_aux_destruct
*
* Revision 6.5  1998/07/27 18:14:17  madden
* lookup_get_memory replaces call to MemNew
*
* Revision 6.4  1998/04/15 20:25:58  madden
* Auxillary structure added to speed-up saving words for queries
*
* Revision 6.3  1998/02/26 22:34:39  madden
* Changes for 16 bit windows
*
* Revision 6.2  1998/01/05 20:32:38  kans
* AddPositionToLookupTable checks for NULL lookup parameter
*
* Revision 6.1  1998/01/05 17:38:05  madden
* Check that position is non-NULL in lookup_destruct
*
* Revision 6.0  1997/08/25 18:53:23  madden
* Revision changed to 6.0
*
* Revision 1.7  1997/08/19 18:19:23  madden
* Cast arg of log to Nlm_FloatHi
*
* Revision 1.6  1997/04/03 19:58:27  madden
* Added check for NULL lookup->position.
*
 * Revision 1.5  1996/09/25  14:17:30  madden
 * removed discontiguous options.
 *
 * Revision 1.4  1996/09/12  21:12:59  madden
 * Added new function to save an already computed index, lookup_add_index.
 *
 * Revision 1.3  1996/08/21  21:28:31  madden
 * Added casts to quiet NT compiler warning.s
 *
 * Revision 1.2  1996/08/15  18:33:35  madden
 * Changed Int2 to Int1.
 *
 * Revision 1.1  1996/08/05  19:48:50  madden
 * Initial revision
 *
 * Revision 1.11  1996/07/24  12:01:28  madden
 * Changes for blastx
 *
 * Revision 1.10  1996/07/18  22:00:02  madden
 * Changes for multiple contexts.
 *
 * Revision 1.9  1996/06/20  16:52:23  madden
 * Changed "pow" to "Nlm_Powi".
 *
 * Revision 1.8  1996/06/20  16:15:57  madden
 * Replaced int's with Int4's.
 *
 * Revision 1.7  1996/05/22  20:22:01  madden
 * Removed unused variable lookup_find_discontig.
 *
 * Revision 1.6  1996/05/16  19:50:15  madden
 * Added documentation block.
 *
 * Revision 1.5  1996/05/03  19:55:24  madden
 * Combined two functions.
 *
 * Revision 1.4  1996/04/04  20:47:55  madden
 * Made lookup_find into two functions, that are statics.
 *
 * Revision 1.3  1996/02/28  21:37:14  madden
 * changes for discontiguous words.
 *
 * Revision 1.2  1995/12/26  20:28:36  madden
 * removed lookup_add_word function.
 *
 * Revision 1.1  1995/12/08  15:48:23  madden
 * Initial revision
 *
*/

#include <ncbi.h>
#include <blast.h> /* this already includes lookup.h */
/*#include <zlib.h> /* For the CRC32 hashing function */

/* mod_lt[] is a modified lookup table.  -cfj
 * It contains the same info that is accessible via lookup->position, but in a form that reduces the
 * number of cache misses.
 * Doing a lookup via the original lookup table cost one cache miss per access, and then one miss per 
 *   entry as we walk down the hit list.
 *
 * mod_lt replaces the linked list with an array of hits, to reduce the number of misses.
 * Also, if there are 3 or fewer hits, they are placed directly in the table.
 *       if there are more than 3 hits, the first one is placed directly in the table (followed by
 *       a pointer to a contiguous array of all the remaining hits).
 *
 * This should probably be allocated and deallocated along with the standard lookup table, but this 
 * is not currently done.
 *
 * The make_mod_lt function should be called to build the mod_lt table after all hits have been placed
 * into the orginal lookup table.
 */

void make_mod_lt(LookupTablePtr lookup)
{
    Int4 index, j, pv_size;
    Uint4 len;
    OrigLookupPositionPtr PNTR lookup_pos;
    OrigLookupPositionPtr list;
    ModLookupPositionPtr next_free;
    PV_ARRAY_TYPE *pv_array=NULL;
    
    
    ModLAEntry *mod_lt;
    mod_lt=lookup->mod_lt;
    
    len = lookup->array_size;
    lookup_pos = lookup->position;
    
    /* Add 1 to silence purify messages. */
    /* Initial allocation - more, than necessary though */

    next_free = MemNew((1+lookup->num_pos_added)*sizeof(ModLookupPosition));
    lookup->mod_lookup_table_size = 0;
    
    if (next_free == NULL){
        ErrPostEx(SEV_ERROR, 0, 0, "Unable to allocate lookup->position");
        return;
    }
    lookup->mod_lookup_table_memory = next_free;
    
    
    /* pv_array is an array of 'presence bits', one bit per entry 
       in the lookup table, packed 64bits per Uint8 (on a 64-bit binary); 
       otherwise a Uint4 is used. It can be used to quickly see if there 
       is an entry in the lookup table. It's used on short-to-medium 
       length queries since most lookups will be empty. -cfj
    */
    
    if (lookup->num_unique_pos_added < PV_ARRAY_FACTOR*lookup->array_size) {
	pv_size = (lookup->array_size+PV_ARRAY_MASK)/(PV_ARRAY_MASK+1);    /* Size measured in PV_ARRAY_TYPE's */
	pv_array = MemNew(pv_size*PV_ARRAY_BYTES);
    }
    
    /* Walk through table, copying info into mod_lt[] */
    for(index = 0; index < len; index++) {
        if(lookup_pos[index] == NULL){
            mod_lt[index].num_used = 0;
        } else {
            /* count num entries */
            int count=0;
            list = lookup_pos[index];
            
            if (pv_array)
                pv_array[(index>>PV_ARRAY_BTS)] |= (((PV_ARRAY_TYPE) 1)<<(index&PV_ARRAY_MASK));
            
            while (list != NULL){
                count++;
                list = list->next;
            }
            
            if (count<=3) {
                list = lookup_pos[index];
                mod_lt[index].num_used=count;
                for(j=0;j<count;j++){
		    hinfo_set(&mod_lt[index].entries[j],list->position,list->context);
                    list = list->next;
                }	    
            } else {
                ModLookupPositionPtr * lpp= (ModLookupPositionPtr *) &mod_lt[index].entries[1];
                list=lookup_pos[index];
                mod_lt[index].num_used=count;
                hinfo_set(&mod_lt[index].entries[0],list->position,list->context);
                list = list->next;
                *lpp  = next_free;
                for(j = 1; j < count; j++){
		    hinfo_set(next_free,list->position,list->context);
                    next_free++;
                    lookup->mod_lookup_table_size++;
                    list = list->next;
                }
            }
        }
    }

    lookup->pv_array = pv_array;
}
void mb_make_mod_lt(LookupTablePtr lookup)
{
    Int4 index, j, pv_size;
    Uint4 len;
    OrigLookupPositionPtr PNTR lookup_pos;
    OrigLookupPositionPtr list;
    ModLookupPositionPtr next_free;
    PV_ARRAY_TYPE *pv_array=NULL;
    
    
    ModLAEntry *mod_lt;
    mod_lt=lookup->mod_lt;
    
    len = lookup->array_size;
    lookup_pos = lookup->position;
    
    /* Add 1 to silence purify messages. */
    /* Initial allocation - more, than necessary though */

    next_free = MemNew((1+lookup->num_pos_added)*sizeof(ModLookupPosition));
    lookup->mod_lookup_table_size = 0;
    
    if (next_free == NULL){
        ErrPostEx(SEV_ERROR, 0, 0, "Unable to allocate lookup->position");
        return;
    }
    lookup->mod_lookup_table_memory = next_free;
    
    
    /* pv_array is an array of 'presence bits', one bit per entry 
       in the lookup table, packed 64bits per Uint8 (on a 64-bit binary); 
       otherwise a Uint4 is used. It can be used to quickly see if there 
       is an entry in the lookup table. It's used on short-to-medium 
       length queries since most lookups will be empty. -cfj
    */
    
    if (lookup->num_unique_pos_added < PV_ARRAY_FACTOR*lookup->array_size) {
	pv_size = (lookup->array_size+PV_ARRAY_MASK)/(PV_ARRAY_MASK+1);    /* Size measured in PV_ARRAY_TYPE's */
	pv_array = MemNew(pv_size*PV_ARRAY_BYTES);
    }
    
    /* Walk through table, copying info into mod_lt[] */
    for(index = 0; index < len; index++) {
        if(lookup_pos[index] == NULL){
            mod_lt[index].num_used = 0;
        } else {
            /* count num entries */
            int count=0;
            list = lookup_pos[index];
            
            if (pv_array)
                pv_array[(index>>PV_ARRAY_BTS)] |= (((PV_ARRAY_TYPE) 1)<<(index&PV_ARRAY_MASK));
            
            while (list != NULL){
                count++;
                list = list->next;
            }
            
            if (count<=3) {
                list = lookup_pos[index];
                mod_lt[index].num_used=count;
                for(j=0;j<count;j++){
		    mod_lt[index].entries[j] = list->position;
                    list = list->next;
                }	    
            } else {
                ModLookupPositionPtr * lpp= (ModLookupPositionPtr *) &mod_lt[index].entries[1];
                list=lookup_pos[index];
                mod_lt[index].num_used=count;
                mod_lt[index].entries[0] = list->position;
                list = list->next;
                *lpp  = next_free;
                for(j = 1; j < count; j++){
		   *next_free = list->position;
                    next_free++;
                    lookup->mod_lookup_table_size++;
                    list = list->next;
                }
            }
        }
    }

    lookup->pv_array = pv_array;
}


static void AddPositionToLookupTable PROTO((LookupTablePtr lookup, Int4 index, Int4 position, Int1 context));

#define LOOKUP_MEMORY_MIN 128*1024	/* Minimum block of memory (in bytes) for LookupMemoryPtr. */

static VoidPtr lookup_deallocate_memory (LookupTablePtr lookup)
{
    LookupMemoryPtr next=NULL, mem_struct;
    
    if (lookup == NULL)
        return NULL;
    
    mem_struct = lookup->mem_struct_start;
    
    while (mem_struct) {
        next = mem_struct->next;
        MemFree(mem_struct->start);
        MemFree(mem_struct);
        mem_struct = next;
    }

    lookup->mem_struct_start = NULL;

    return NULL;
}

/*
  returns the requested amount of memory.  Large blocks are obtained at
  once and then passed out. 
  
  LookupTablePtr lookup:	contains all information on Lookup.
  size_t required: how much memory has been requested.
*/

static VoidPtr lookup_get_memory (LookupTablePtr lookup, size_t required)
{
    LookupMemoryPtr last, mem_struct;
    size_t memory_requested;
    Uint1Ptr new;
    
    mem_struct = lookup->mem_struct;
    
    if (!mem_struct || required > mem_struct->remaining) {
        if (mem_struct) {
            last = mem_struct;
        } else {
            last = NULL;
        }
        
        memory_requested = MAX(lookup->memory_chunk, LOOKUP_MEMORY_MIN);
        mem_struct = (LookupMemoryPtr) MemNew(sizeof(LookupMemory));
        mem_struct->start = (Uint1Ptr) MemNew(memory_requested);
        mem_struct->current = mem_struct->start;
        mem_struct->remaining = memory_requested;
        
        /* align to 64Byte block. -cfj */
        if(1){
            Uint8 extra = ((Uint8)mem_struct->current) & ((Uint8) 63);
            if (extra>0){
                extra=64-extra;
                mem_struct->current += extra;
                mem_struct->remaining -= extra;
            }
        }
        
        if (last) {
            last->next = mem_struct;	/* used at end for deallocation. */
            lookup->mem_struct = mem_struct; /* Used as a shortcut around a long while loop. */
        } else {
            lookup->mem_struct = mem_struct;
            lookup->mem_struct_start = mem_struct;
        }
    }
    
    new = mem_struct->current;
    mem_struct->current += required;
    mem_struct->remaining -= required;
    
    return (VoidPtr) new;	/* return pointer to new memory. */
}

/*
  LookupTablePtr LIBCALL lookup_new(Int2 alphabet_size, Int2 wordsize)
  
  alphabet_size: number of symbols in alphabet
  
  wordsize: size of initial word hit (often three)
  
  Perform the initial setup of the lookup table.  "lookup" 
  expects to work with a "zero-offset" alphabet such as
  NCBIstdaa or NCBI2na.
  
  Each letter (residue or basepair) in the word goes into it's
  own set of bits (calculated as num_of_bits below).  This is
  to allow masking and shifting of bits.  As an example consider
  a three-letter word with an alphabet containing 20 residues.  
  The minimum num_of_bits is five (able to represent up to 31)
  and the total "array_size" is 15.
  "mask" is used to mask the wordsize-1 lower bits (i.e., to set the
  bits the highest num_of_bits to zero).
*/
LookupTablePtr LIBCALL
lookup_new(Int2 alphabet_size, Int2 wordsize, Int2 reduced_wordsize)
     
{
    Int4 num_of_bits;
    LookupTablePtr lookup;
    
    /* How many bits are needed to hold the alphabet? */
    num_of_bits = Nlm_Nint(log((Nlm_FloatHi)alphabet_size)/NCBIMATH_LN2);

    /* 32 bits is 4 bytes */ 
    if (num_of_bits*reduced_wordsize > 32) {
        ErrPostEx(SEV_ERROR, 0, 0, "alphabet times wordsize > 32");
        return NULL;
    }
    
    lookup = (LookupTablePtr) MemNew(sizeof(LookupTable));
    if (lookup == NULL)
        return NULL;
    
    lookup->char_size = num_of_bits;
    lookup->wordsize = (Int4) wordsize;
    if (reduced_wordsize <= 0)
        reduced_wordsize = wordsize;
    lookup->reduced_wordsize = (Int4) reduced_wordsize;
    lookup->array_size = (Int4) Nlm_Powi(2.0, (num_of_bits*reduced_wordsize));
    lookup->mask = (Int4) Nlm_Powi(2.0, (num_of_bits*(reduced_wordsize-1))) - 1;
    lookup->mod_lt = NULL;    /* will be allocated when built */
    lookup->theTable = NULL;  /* will be allocated when built */
    
    lookup->position = (OrigLookupPositionPtr PNTR) MemNew((lookup->array_size)*sizeof(OrigLookupPositionPtr)); 
    if (lookup->position == NULL) {
        lookup = lookup_destruct(lookup);
        ErrPostEx(SEV_ERROR, 0, 0, "Unable to allocate lookup->position");
    }
    
    /* Only allocate the auxillary structure if it's less than 1 Meg. */
    if ((lookup->array_size)*sizeof(OrigLookupPositionPtr) <= (1<<20)) {
        lookup->position_aux =
            (OrigLookupPositionPtr PNTR) MemNew((lookup->array_size)*sizeof(OrigLookupPositionPtr));
    }
    
    return lookup;
}

LookupTablePtr LIBCALL
lookup_destruct(LookupTablePtr lookup)

{
    if (lookup == NULL)
        return lookup;
    
    if(lookup->position)
        MemFree(lookup->position);
    if(lookup->position_aux) 
        MemFree(lookup->position_aux);
    if(lookup->pv_array) 
        MemFree(lookup->pv_array);
    if(lookup->mod_lt) 
        MemFree(lookup->mod_lt);
    if(lookup->theTable)
        MemFree(lookup->theTable);
    if (lookup->mod_lookup_table_memory)
        MemFree(lookup->mod_lookup_table_memory);
    lookup_deallocate_memory(lookup);
    lookup = MegaBlastLookupTableDestruct(lookup);
    lookup = MemFree(lookup);
    
    return lookup;
}

/*
  Deallocated position_aux, which can be large for
  large word-sizes, and is no longer needed.
  
  This now does more than just dealloc lookup->position_aux.
  This is called afeter the original lookup table is fully built.
  So this is where we convert it to the new (mod_lt) table.
  -cfj
*/

Boolean
lookup_position_aux_destruct(LookupTablePtr lookup)
     
{
    if (lookup == NULL)
        return FALSE;
    
    if(lookup->position_aux) 
        lookup->position_aux = MemFree(lookup->position_aux);
    
    
    /* create the new lookup table, Add 1 to silence purify messages. */
    
    lookup->mod_lt = (ModLAEntry PNTR) MemNew((1+lookup->array_size)*sizeof(ModLAEntry)); 
    
    make_mod_lt(lookup);
    
    /* deallocate parts of the old table no longer needed */
    if(lookup->position) 
	lookup->position = MemFree(lookup->position);
    lookup_deallocate_memory(lookup);
    
    return TRUE;
}

Boolean
mb_lookup_position_aux_destruct(LookupTablePtr lookup)
     
{
    if (lookup == NULL)
        return FALSE;
    
    if(lookup->position_aux) 
        lookup->position_aux = MemFree(lookup->position_aux);
    
    
    /* create the new lookup table, Add 1 to silence purify messages. */
    
    lookup->mod_lt = (ModLAEntry PNTR) MemNew((1+lookup->array_size)*sizeof(ModLAEntry)); 
    
    mb_make_mod_lt(lookup);
    
    
    /* deallocate parts of the old table no longer needed */
    if(lookup->position) lookup->position = MemFree(lookup->position);
    
    return TRUE;
}

/*
  This function adds an (already computed) index to the lookup table 
  for some set of residues.  The most likely use of this is for
  compressed residues, as in blastn.
*/
void LIBCALL
lookup_add_index(LookupTablePtr lookup, Int4 lookup_index, Int4 position, Int1 context)
     
{
    AddPositionToLookupTable(lookup, lookup_index, position, context);
    
    return;
}

/*
  This function adds an index to the lookup table for any number of 
  contiguous resdiues. 
  
*/
void LIBCALL
lookup_add(LookupTablePtr lookup, CharPtr string, Int4 position, Int1 context)

{
    Int4 char_size, lookup_index=0, wordsize;
    
    char_size = lookup->char_size;
    wordsize = lookup->wordsize;
    
    lookup_index = *string;
    wordsize--;
    while (wordsize > 0) {
        lookup_index <<= char_size;
        string++;
        lookup_index += *string;
        wordsize--;
    }
    AddPositionToLookupTable(lookup, lookup_index, position, context);
    
    return;
}

static void 
AddPositionToLookupTable(LookupTablePtr lookup, Int4 index, Int4 position, Int1 context)
     
{
    OrigLookupPositionPtr new, last, PNTR lookup_pos, PNTR lookup_pos_aux;
    
    if (lookup == NULL) return;
    lookup->num_pos_added++;
    
    lookup_pos = lookup->position;
    lookup_pos_aux = lookup->position_aux;
    new = (OrigLookupPositionPtr) lookup_get_memory(lookup, sizeof(OrigLookupPosition));
    if (*(lookup_pos+index) == NULL) {
        lookup->num_unique_pos_added++;
        lookup_pos[index] = new;
        lookup_pos[index]->position = position;
        lookup_pos[index]->context = context;
        if (lookup_pos_aux) {
            lookup_pos_aux[index] = new;
        }
    } else {	/* Go to last link, use aux struct if it exists. */
        if (lookup_pos_aux) {
            last = lookup_pos_aux[index];
            lookup_pos_aux[index] = new;
        } else {
            last = lookup_pos[index];
            while (last->next)
                last = last->next;
        }
        last->next = new;
        new->position = position;
        new->context = context;
    }
    
    return;
}

/*
  Initializes the "lookup_index" for "string".  For contiguous word
  matching the first wordsize-1 letters of string are placed into the 
  lookup_index.  The "new" starting point of the string is then returned.
*/ 

Uint1Ptr LIBCALL
lookup_find_init(LookupTablePtr lookup, Int4 PNTR lookup_index, Uint1Ptr string)
     
{
    
    Int4 char_size, wordsize;
    
    char_size = lookup->char_size;
    wordsize = lookup->reduced_wordsize;
    
    /* Fill in wordsize-1 spaces. */
    *lookup_index = *string;
    
    wordsize -= 2;
    while (wordsize > 0) {
        string++;
        *lookup_index <<= char_size;
        *lookup_index += *string;
        wordsize--;
    }
    
    return string;
}
 
/* 
   Allocates memory for the BLAST_WordFinder, and lookup table.
*/

BLAST_WordFinderPtr
BLAST_WordFinderNew (Int2 alphabet_size, Int2 wordsize, Int2 compression_ratio, Boolean round_down)
     
{
    BLAST_WordFinderPtr wfp;
    Int2 reduced_wordsize;
    
    wfp = (BLAST_WordFinderPtr) MemNew(sizeof(BLAST_WordFinder));
    
    if (wfp != NULL) {
	/* If the compression_ratio is greater than one and round_down is TRUE, then we want to round 
           the wordsize down, as a multiple of compression_ratio should be three less.  round_down is only
           TRUE in blastn if we want to guarantee that a certain wordsize is used (i.e., 11). */
        if (compression_ratio > 1) {
            if (round_down) {

#if BLAST_ALTIVEC
				Int4	lookup_wordsize = (wordsize-3)/compression_ratio;
#endif				


                reduced_wordsize = (Int2) MIN(2, ((wordsize-3)/compression_ratio));

#if BLAST_ALTIVEC
				if (wordsize < 11 && wordsize > 4) {
					reduced_wordsize = 2;
					lookup_wordsize = reduced_wordsize;
				}
#endif								

                wfp->lookup = lookup_new(alphabet_size, (Int2) ((wordsize-3)/compression_ratio), (Int2) reduced_wordsize);

#if BLAST_ALTIVEC                
				if (wordsize < 8 && wordsize > 4) {
	                wfp->lookup->true_wordsize_bits = wordsize;
	            }
#endif                

            } else {
                reduced_wordsize = wordsize/compression_ratio;
                wfp->lookup = lookup_new(alphabet_size, (wordsize/compression_ratio), (Int2) reduced_wordsize);
            }
        } else {
            wfp->lookup = lookup_new(alphabet_size, (Int2) (wordsize/compression_ratio), 0);
        }
        wfp->compression_ratio = compression_ratio;
        wfp->wordsize = wordsize;
    }
    
    return wfp;
}

BLAST_WordFinderPtr
BLAST_WordFinderDestruct (BLAST_WordFinderPtr wfp)
     
{
    
    if (wfp != NULL) {
        wfp->lookup = lookup_destruct(wfp->lookup);
        wfp = MemFree(wfp);
    }
    
    return wfp;
}

BLAST_WordFinderPtr
MegaBlastWordFinderDeallocate(BLAST_WordFinderPtr wfp)
{
   LookupTablePtr lookup;
   
   if (wfp == NULL || (lookup = wfp->lookup) == NULL || 
       lookup->mb_lt == NULL) 
      return wfp;
   if (lookup->mb_lt->estack) {
      Int4 index;
      for (index=0; index<lookup->mb_lt->num_stacks; index++)
	 MemFree(lookup->mb_lt->estack[index]);
      MemFree(lookup->mb_lt->estack);
      MemFree(lookup->mb_lt->stack_size);
      MemFree(lookup->mb_lt->stack_index);
   }
   MemFree(lookup->mb_lt);
   MemFree(lookup);
   wfp = MemFree(wfp);
   return wfp;
}

#if 0
/* Given a 4-byte integer index, compute a 'size'-bit hash value */
Int4 MB_GetHashValue(Int4 index, Int4 size) 
{
   Int4 value;
   VoidPtr buffer = (VoidPtr) &(index);
   
   value = crc32(0, buffer, 4);
   value = value>>(32-size);
   return value;
}
#endif

Boolean
MegaBlastBuildLookupTable(BlastSearchBlkPtr search)
{
   LookupTablePtr lookup = search->wfp->lookup;
   Int4 query_length = search->context[search->first_context].query->length;
   register Uint1Ptr seq, pos;
   register Int4 index;
   register Int4 ecode, extra_code;
   register Int4 mask;
   Int4 ecode1, ecode2;
   Uint1 val, nuc_mask = 0xfc, at_hash_mask = 0x10;
   MbLookupTablePtr mb_lt;
   Int4 masked_word_count = 0;
   PV_ARRAY_TYPE *pv_array=NULL;
   Int4 pv_shift, pv_array_bts, pv_size;
   Boolean discontiguous_word = FALSE;
   Int2 word_length, extra_length, word_weight;
   Int4 last_offset;
   Int4 length, size, hash_shift, hash_mask, crc;
   Uint1Ptr hash_buf;
   Int4 bit0, no_bit0;
   Int2 template_length;
   MegaBlastParameterBlkPtr mb_params = search->pbp->mb_params;
   MBTemplateType template_type = mb_params->template_type;
   Boolean amb_cond;
   Boolean use_two_templates = mb_params->use_two_templates;

   if (lookup == NULL)
      return FALSE;

   if(lookup->position_aux) 
      lookup->position_aux = MemFree(lookup->position_aux);
    
   mb_lt = (MbLookupTablePtr) MemNew(sizeof(MbLookupTable));
    
   /* Increment lookup->wordsize, since it is now equal to (wordsize - 3)/4 */
   if (++lookup->wordsize < 3) 
      mb_lt->width = 2;
   else
      mb_lt->width = 3;
   mb_lt->lpm = lookup->wordsize * READDB_COMPRESSION_RATIO;

   mb_lt->max_positions = mb_params->max_positions; 

#ifdef USE_HASH_TABLE
   /* Make hash table size the smallest power of 2 greater than query length */
   for (length=query_length,size=1; length>0; length=length>>1,size++);
   mb_lt->hashsize = 1<<size;
   hash_shift = (32 - size)/2;
   hash_mask = mb_lt->hashsize - 1;
#else
   mb_lt->hashsize = (1<<(8*mb_lt->width));
#endif

   if (mb_params->disc_word) {
      discontiguous_word = TRUE;
      word_length = READDB_COMPRESSION_RATIO*(mb_lt->width+1);
      template_length = mb_params->template_length;
      extra_length = template_length - word_length;
      word_weight = mb_params->word_weight;
      mask = (1 << (8*(mb_lt->width+1) - 2)) - 1;
   } else {
      word_length = READDB_COMPRESSION_RATIO*mb_lt->width;
      mask = mb_lt->mask = (1 << (8*mb_lt->width - 2)) - 1;
   }

   if ((mb_lt->hashtable = (Int4Ptr) 
        MemNew(mb_lt->hashsize*sizeof(Int4))) == NULL) {
      MegaBlastLookupTableDestruct(lookup);
      return FALSE;
   }

   if ((mb_lt->next_pos = (Int4Ptr) 
        MemNew(query_length*sizeof(Int4))) == NULL) {
      MegaBlastLookupTableDestruct(lookup);
      return FALSE;
   }
   if (use_two_templates) {
      if (word_weight >= 12) {
         if ((mb_lt->hashtable2 = (Int4Ptr) 
              MemNew(mb_lt->hashsize*sizeof(Int4))) == NULL) {
            MegaBlastLookupTableDestruct(lookup);
            return FALSE;
         }
      } else {/* For weight 11 no need for extra main table */
         mb_lt->hashtable2 = mb_lt->hashtable;
      }
      if ((mb_lt->next_pos2 = (Int4Ptr) 
           MemNew(query_length*sizeof(Int4))) == NULL) {
         MegaBlastLookupTableDestruct(lookup);
         return FALSE;
      }
   }
   seq = search->context[search->first_context].query->sequence_start;
   pos = seq + word_length;

   ecode = 0;

   if (discontiguous_word) {
      if (word_weight == 11) {
         no_bit0 = 0x007fffff;
         bit0 = 0x00800000;
      } else {
         no_bit0 = 0xffffffff;
         bit0 = 0x00000000;
      }
      if (/*template18*/extra_length) {
         last_offset = query_length - extra_length;
         for (index = 1; index <= last_offset; index++) {
            val = *++seq;
            if ((val & nuc_mask) != 0) { /* ambiguity, gap or masked residue */
               if (*seq & at_hash_mask) /* Will extend through this residue */
                  *seq &= 0x0f;
               ecode = 0;
               pos = seq + word_length;
               extra_code = 0;
            } else {
               /* get next base */
               ecode = ((ecode & mask) << 2) + val;
               if (seq >= pos) {
                  switch (template_type) {
                  case TEMPL_11_18: case TEMPL_12_18:
                     amb_cond = !GET_AMBIG_CONDITION_18(seq);
                     break;
                  case TEMPL_11_18_OPT: case TEMPL_12_18_OPT:
                     amb_cond = !GET_AMBIG_CONDITION_18_OPT(seq);
                     break;
                  case TEMPL_11_21: case TEMPL_12_21:
                     amb_cond = !GET_AMBIG_CONDITION_21(seq);
                     break;
                  case TEMPL_11_21_OPT: case TEMPL_12_21_OPT:
                     amb_cond = !GET_AMBIG_CONDITION_21_OPT(seq);
                     break;
                  default:
                     amb_cond = FALSE;
                     break;
                  }

                  if (amb_cond) {
                     switch (template_type) {
                     case TEMPL_11_18:
                        ecode1 = (GET_WORD_INDEX_11_18(ecode) |
                                  GET_EXTRA_CODE_18(seq)) & no_bit0;
                        break;
                     case TEMPL_12_18:
                        ecode1 = (GET_WORD_INDEX_12_18(ecode) |
                                  GET_EXTRA_CODE_18(seq)) & no_bit0;
                        break;
                     case TEMPL_11_18_OPT:
                        ecode1 = (GET_WORD_INDEX_11_18_OPT(ecode) |
                                  GET_EXTRA_CODE_18_OPT(seq)) & no_bit0;
                        break;
                     case TEMPL_12_18_OPT:
                        ecode1 = (GET_WORD_INDEX_12_18_OPT(ecode) |
                                  GET_EXTRA_CODE_18_OPT(seq)) & no_bit0;
                        break;
                     case TEMPL_11_21:
                        ecode1 = (GET_WORD_INDEX_11_21(ecode) |
                                  GET_EXTRA_CODE_21(seq)) & no_bit0;
                        break;
                     case TEMPL_12_21:
                        ecode1 = (GET_WORD_INDEX_12_21(ecode) |
                                  GET_EXTRA_CODE_21(seq)) & no_bit0;
                        break;
                     case TEMPL_11_21_OPT:
                        ecode1 = (GET_WORD_INDEX_11_21_OPT(ecode) |
                                  GET_EXTRA_CODE_21_OPT(seq)) & no_bit0;
                        break;
                     case TEMPL_12_21_OPT:
                        ecode1 = (GET_WORD_INDEX_12_21_OPT(ecode) |
                                  GET_EXTRA_CODE_21_OPT(seq)) & no_bit0;
                        break;
                     default: 
                        ecode1 = 0;
                        break;
                     }

#ifdef USE_HASH_TABLE                     
                     hash_buf = (Uint1Ptr)&ecode1;
                     CRC32(crc, hash_buf);
                     ecode1 = (crc>>hash_shift) & hash_mask;
#endif

                     if (use_two_templates) {
                        switch (template_type) {
                        case TEMPL_11_18:
                           ecode2 = GET_WORD_INDEX_11_18_OPT(ecode) |
                              GET_EXTRA_CODE_18_OPT(seq) | bit0;
                           break;
                        case TEMPL_12_18:
                           ecode2 = GET_WORD_INDEX_12_18_OPT(ecode) |
                              GET_EXTRA_CODE_18_OPT(seq) | bit0;
                           break;
                        case TEMPL_11_21:
                           ecode2 = GET_WORD_INDEX_11_21_OPT(ecode) |
                              GET_EXTRA_CODE_21_OPT(seq) | bit0;
                           break;
                        case TEMPL_12_21:
                           ecode2 = GET_WORD_INDEX_12_21_OPT(ecode) |
                              GET_EXTRA_CODE_21_OPT(seq) | bit0;
                           break;
                        default:
                           ecode2 = 0; break;
                        }
                           
#ifdef USE_HASH_TABLE                     
                        hash_buf = (Uint1Ptr)&ecode2;
                        CRC32(crc, hash_buf);
                        ecode2 = (crc>>hash_shift) & hash_mask;
#endif
                        if (mb_lt->hashtable[ecode1] == 0 || 
                            mb_lt->hashtable2[ecode2] == 0)
                           lookup->num_unique_pos_added++;
                        mb_lt->next_pos2[index] = mb_lt->hashtable2[ecode2];
                        mb_lt->hashtable2[ecode2] = index;
                     } else {
                        if (mb_lt->hashtable[ecode1] == 0)
                           lookup->num_unique_pos_added++;
                     }
                     mb_lt->next_pos[index] = mb_lt->hashtable[ecode1];
                     mb_lt->hashtable[ecode1] = index;
                  }
               }
            }
         }
         /* Modify the last 2 residues if they are masked only for lookup */
         for (index = 0; index < extra_length; index++) {
            if (*++seq & at_hash_mask)
               *seq &= 0x0f;
         }
      } else { /* Template length 16 */
         for (index = 1; index <= query_length; index++) {
            val = *++seq;
            if ((val & nuc_mask) != 0) { /* ambiguity, gap or masked residue */
               ecode = 0;
               pos = seq + word_length;
               if (*seq & at_hash_mask) /* Will extend through this residue */
                  *seq &= 0x0f;
            } else {
               /* get next base */
               ecode = ((ecode & mask) << 2) + val;
               if (seq >= pos) {
                  switch (template_type) {
                  case TEMPL_11_16:
                     ecode1 = GET_WORD_INDEX_11_16(ecode) & no_bit0;
                     break;
                  case TEMPL_12_16:
                     ecode1 = GET_WORD_INDEX_12_16(ecode) & no_bit0;
                     break;
                  case TEMPL_11_16_OPT:
                     ecode1 = GET_WORD_INDEX_11_16_OPT(ecode) & no_bit0;
                     break;
                  case TEMPL_12_16_OPT:
                     ecode1 = GET_WORD_INDEX_12_16_OPT(ecode) & no_bit0;
                     break;
                  default:
                     ecode1 = 0;
                     break;
                  }
                     
#ifdef USE_HASH_TABLE
                  hash_buf = (Uint1Ptr)&ecode1;
                  CRC32(crc, hash_buf);
                  ecode1 = (crc>>hash_shift) & hash_mask;
#endif
                  if (use_two_templates) {
                     switch (template_type) {
                     case TEMPL_11_16:
                        ecode2 = GET_WORD_INDEX_11_16_OPT(ecode) | bit0;
                        break;
                     case TEMPL_12_16:
                        ecode2 = GET_WORD_INDEX_12_16_OPT(ecode) | bit0;
                        break;
                     default:
                        ecode2 = 0; break;
                     }
#ifdef USE_HASH_TABLE                     
                     hash_buf = (Uint1Ptr)&ecode2;
                     CRC32(crc, hash_buf);
                     ecode2 = (crc>>hash_shift) & hash_mask;
#endif
                     if (mb_lt->hashtable[ecode1] == 0 || 
                         mb_lt->hashtable2[ecode2] == 0)
                        lookup->num_unique_pos_added++;
                     mb_lt->next_pos2[index] = mb_lt->hashtable2[ecode2];
                     mb_lt->hashtable2[ecode2] = index;
                  } else {
                     if (mb_lt->hashtable[ecode1] == 0)
                        lookup->num_unique_pos_added++;
                  }
                  mb_lt->next_pos[index] = mb_lt->hashtable[ecode1];
                  mb_lt->hashtable[ecode1] = index;
               }
            }
         }
      }
   } else { /* Contiguous word */
      for (index = 1; index <= query_length; index++) {
         val = *++seq;
         if ((val & nuc_mask) != 0) { /* ambiguity, gap or masked residue */
            ecode = 0;
            pos = seq + word_length;
            if (*seq & at_hash_mask) /* Will extend through this residue */
               *seq &= 0x0f;
         } else {
            /* get next base */
            ecode = ((ecode & mask) << 2) + val;
            if (seq >= pos) {
               if (mb_lt->hashtable[ecode] == 0)
                  lookup->num_unique_pos_added++;
               mb_lt->next_pos[index] = mb_lt->hashtable[ecode];
               mb_lt->hashtable[ecode] = index;
            }
         }
      }
   }

   /* Now remove the hash entries that have too many positions */
   if (mb_lt->max_positions>0) {
      for (ecode=0; ecode<mb_lt->hashsize; ecode++) {
	 Int4 pcount, pcount1=0;
	 for (index=mb_lt->hashtable[ecode], pcount=0; 
	      index>0; index=mb_lt->next_pos[index], pcount++);
         if (use_two_templates) {
            for (index=mb_lt->hashtable2[ecode], pcount1=0; 
                 index>0; index=mb_lt->next_pos2[index], pcount1++);
         }
	 if ((pcount=MAX(pcount,pcount1))>mb_lt->max_positions) {
	    mb_lt->hashtable[ecode] = 0;
            if (use_two_templates)
               mb_lt->hashtable2[ecode] = 0;
	    ErrPostEx(SEV_WARNING, 0, 0, "%lx - %d", ecode, pcount);
	    masked_word_count++;
	 }
      }
      if (masked_word_count)
	 ErrPostEx(SEV_WARNING, 0, 0, "Masked %d %dmers", masked_word_count,
		   4*mb_lt->width);
	 
   }

   if (discontiguous_word)
      mb_lt->mask = (1 << mb_lt->width*8) - 1;
   else
      mb_lt->mask = (1 << (mb_lt->width*8 - 8)) - 1;
      
   /* Stacks are allocated for word size >= 11, or if query length is too long,
      otherwise will use diagonal array (ewp and ewp_params structures) */
   if (query_length > MAX_DIAG_ARRAY || 
        search->pbp->mb_params->word_weight >= 11) {
      Int8 total_len=0, av_search_space=0, av_len=0;
      Int4 total_num=0, stack_size=0, num_stacks=0;
      if (search->rdfp) {
         if (search->dblen > 0 && search->dbseq_num > 0) {
            total_len = search->dblen;
            total_num = search->dbseq_num;
         } else {
            readdb_get_totals_ex(search->rdfp, &total_len, &total_num, TRUE);
         }
         if (total_num > 0)
            av_len = total_len / total_num;
         else
            av_len = 1000;
      } else {
         av_len = search->subject->length;
      }
      av_search_space = 
         ((Int8) search->context[search->first_context].query->length) * av_len;
      num_stacks = MIN(1 + (Int4) sqrt(av_search_space)/100, 500);
      stack_size = 5000/num_stacks;
      mb_lt->stack_index = (Int4Ptr) MemNew(num_stacks*sizeof(Int4));
      mb_lt->stack_size = (Int4Ptr) Malloc(num_stacks*sizeof(Int4));
      mb_lt->estack = (MbStackPtr PNTR) Malloc(num_stacks*sizeof(MbStackPtr));
      for (index=0; index<num_stacks; index++) {
         mb_lt->estack[index] = 
            (MbStackPtr) Malloc(stack_size*sizeof(MbStack));
         mb_lt->stack_size[index] = stack_size;
      }
      mb_lt->num_stacks = num_stacks;
   } 

#ifdef USE_HASH_TABLE
   /* If hash table is used instead of index table, no need for extra reduction
      of pv_array size */
   pv_shift = 0;
#else
   /* For 12-mer based lookup table need to make presense bit array much 
      smaller, so it stays in cache, even though this allows for collisions */
   pv_shift = (mb_lt->width < 3) ? 0 : 5;
#endif
   pv_array_bts = PV_ARRAY_BTS + pv_shift;

   if (lookup->num_unique_pos_added < 
       (PV_ARRAY_FACTOR*(mb_lt->hashsize>>pv_shift))) {
      pv_size = mb_lt->hashsize>>pv_array_bts;
      /* Size measured in PV_ARRAY_TYPE's */
      pv_array = MemNew(pv_size*PV_ARRAY_BYTES);

      for (index=0; index<mb_lt->hashsize; index++) {
         if (mb_lt->hashtable[index] != 0 ||
             (use_two_templates && mb_lt->hashtable2[index] != 0))
            pv_array[index>>pv_array_bts] |= 
               (((PV_ARRAY_TYPE) 1)<<(index&PV_ARRAY_MASK));
      }
      lookup->pv_array = pv_array;
   }
   lookup->mb_lt = mb_lt;
   return TRUE;
}

LookupTablePtr
MegaBlastLookupTableDestruct(LookupTablePtr lookup)
{
   Int4 index;

   if (!lookup->mb_lt)
      return lookup;
   if (lookup->mb_lt->hashtable)
      MemFree(lookup->mb_lt->hashtable);
   if (lookup->mb_lt->next_pos)
      MemFree(lookup->mb_lt->next_pos);
   if (lookup->mb_lt->next_pos2)
      MemFree(lookup->mb_lt->next_pos2);
   if (lookup->mb_lt->estack) {
      for (index=0; index<lookup->mb_lt->num_stacks; index++)
	 MemFree(lookup->mb_lt->estack[index]);
      MemFree(lookup->mb_lt->estack);
      MemFree(lookup->mb_lt->stack_index);
      MemFree(lookup->mb_lt->stack_size);
   }
   lookup->mb_lt = MemFree(lookup->mb_lt);
   return lookup;
}


LookupTablePtr MegaBlastLookupTableDup(LookupTablePtr lookup)
{
   Int4 index;

   /* The only piece that actually needs to be duplicated is estack array */
   LookupTablePtr new_lookup = 
      (LookupTablePtr) MemDup(lookup, sizeof(LookupTable));

   new_lookup->mb_lt = 
      (MbLookupTablePtr) MemDup(lookup->mb_lt, sizeof(MbLookupTable));
   new_lookup->mb_lt->stack_index = (Int4Ptr) MemNew(lookup->mb_lt->num_stacks*sizeof(Int4));
   new_lookup->mb_lt->stack_size = (Int4Ptr) MemDup(lookup->mb_lt->stack_size,
						    lookup->mb_lt->num_stacks*sizeof(Int4));
   if (lookup->mb_lt->estack) {
      new_lookup->mb_lt->estack = (MbStackPtr PNTR) Malloc(lookup->mb_lt->num_stacks*sizeof(MbStackPtr));
      for (index=0; index<lookup->mb_lt->num_stacks; index++) 
         new_lookup->mb_lt->estack[index] = 
            (MbStackPtr) Malloc(lookup->mb_lt->stack_size[index]*sizeof(MbStack));
   }
   return new_lookup;
}
