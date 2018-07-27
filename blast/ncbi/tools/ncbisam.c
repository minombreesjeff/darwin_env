static char const rcsid[] = "$Id: ncbisam.c,v 6.30 2003/05/30 17:25:37 coulouri Exp $";

/* $Id: ncbisam.c,v 6.30 2003/05/30 17:25:37 coulouri Exp $
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
* File Name:  $RCSfile: ncbisam.c,v $
*
* Author:  Sergei Shavirin
*
* Initial Version Creation Date: 02/24/1997
*
* $Revision: 6.30 $
*
* File Description:
*         Main file for ISAM library
*
* $Log: ncbisam.c,v $
* Revision 6.30  2003/05/30 17:25:37  coulouri
* add rcsid
*
* Revision 6.29  2003/04/14 19:52:31  camacho
* Added ISAMUninitSearch
*
* Revision 6.28  2002/09/23 19:48:09  camacho
* Avoid the use of data->db_fd when searching the ISAM databases
*
* Revision 6.27  2002/09/20 15:17:12  camacho
* Fixed file descriptor leak
*
* Revision 6.26  2002/04/04 17:57:10  camacho
* Fixed binary search implementation in NISAMSearch
*
* Revision 6.25  2002/04/02 20:51:21  camacho
* Fixed bug in NISAMSearch
*
* Revision 6.24  2002/01/18 18:53:13  madden
* Changes to research the last page if appropriate
*
* Revision 6.23  2001/07/09 14:17:24  madden
* Fix PC-lint complaints from R. Williams
*
* Revision 6.22  2001/06/08 20:31:01  madden
* Fix memory leaks
*
* Revision 6.21  2000/08/04 19:54:17  shavirin
* Fixed problem with counting line when data not tested to be non- unique.
*
* Revision 6.20  2000/07/18 19:29:27  shavirin
* Added new parameter test_non_unique to suppress check for non-unique
* strings ids in the database - default - TRUE.
*
* Revision 6.19  2000/07/13 16:43:48  shavirin
* Fixed checking of order in String index creation.
*
* Revision 6.17  2000/02/11 21:14:07  madden
* Allocate MasterPos of correct (smaller) size
*
* Revision 6.16  1999/12/18 15:27:50  egorov
* Fix NT compilation problem
*
* Revision 6.15  1999/12/17 20:47:05  egorov
* Fix 'gcc -Wall' warnings
*
* Revision 6.14  1999/12/06 20:56:12  egorov
* fwrite() writes two bytes for '\n' if file is open not in binary mode,
* so MakeISAMIndex worked incorrectly.
* What to Blast programs, formatdb now works correctly on NT machine.
*
* Revision 6.13  1999/11/08 19:05:21  shavirin
* Fixed minor SGI warning.
*
* Revision 6.12  1999/08/25 20:18:49  shavirin
* Added possibility to store user-specified Int4 options in the index
* header.
*
* Revision 6.11  1999/03/17 21:38:04  kans
* Int4Ptr argument must point to Int4
*
* Revision 6.10  1999/03/17 20:56:24  shavirin
* Fixed warning "long int format"
*
* Revision 6.9  1999/02/19 22:01:14  madden
* Use memory-mapping and binary search on second numerical index
*
* Revision 6.8  1998/07/13 15:31:14  egorov
* make error message more understandable
*
* Revision 6.7  1998/05/28 17:18:04  shavirin
* Fixed non-intialized variable warning.
*
* Revision 6.6  1998/02/23 17:45:28  shavirin
* Fixed problem in sorted file checkup.
*
* Revision 6.5  1997/12/02 20:06:31  shavirin
* Fixed Macintosh warnings
*
* Revision 6.4  1997/12/02 19:38:17  shavirin
* Added variables
*
* Revision 6.3  1997/12/02 18:05:12  shavirin
* Removed typecast warnings in sprintf and sscanf
*
* Revision 6.2  1997/11/28 15:50:10  shavirin
* Added default returned value in the function ISAMGetDataNumber()
*
* Revision 6.1  1997/11/07 16:17:43  shavirin
* Added new function SISAMFindAllData() returned info about redundant keys
*
* Revision 6.0  1997/08/25 18:53:27  madden
* Revision changed to 6.0
*
* Revision 1.17  1997/05/16 17:08:44  shavirin
* Removed printf()
*
* Revision 1.16  1997/05/16 16:16:00  shavirin
* Added LIBCALLBACK to definition of ISAMUidCompare()
*
* Revision 1.15  1997/05/12 19:55:05  shavirin
* Some fixes type-changes to support ISAMCreateDatabase() API
*
* Revision 1.13  1997/05/09 14:12:06  shavirin
* Fixed memory leakage and added ErrLogPrintf()
*
* Revision 1.12  1997/05/08 21:18:08  shavirin
* Added generic function ISAMSearchTerm(), that will search complete ISAM
* string database created by ISAMCreateDatabase() function. Returns array of
* found uids corresponing given string term and gived bit-field mask
*
* Revision 1.10  1997/05/06 21:36:15  shavirin
* Added set of function for Coded Array compression implementation
*
 * Revision 1.9  1997/05/05  18:17:22  shavirin
 * Added support for platforms without memory mapping
 *
 * Revision 1.8  1997/05/05  14:37:54  shavirin
 * Fixed usage of Numeric ISAM with Windows NT platform
 *
 * Revision 1.7  1997/05/01  20:10:57  shavirin
 * Fixed few errors discovered on Macintoch
 *
 * Revision 1.6  1997/05/01  17:24:33  shavirin
 * Added String ISAM index functionality
 *
 * Revision 1.5  1997/02/26  01:28:11  shavirin
 * Fixed difference in definitions of SISAMSearch() function
 *
 * Revision 1.4  1997/02/25  22:16:49  shavirin
 * Changed general API of ISAM library .
 *
 * Revision 1.3  1997/02/25  19:38:12  shavirin
 * Added defence aginst little-big endian platforms
 *
 * Revision 1.2  1997/02/25  15:33:56  shavirin
 * Return value will be ISAMNoError if number is not found,
 * byt Data and Index will be set to ISAMNotFound
 *
 * Revision 1.1  1997/02/24  21:06:52  shavirin
 * Initial revision
 *
*
* ==========================================================================
*/
#include <ncbi.h>
#include <readdb.h>
#include <ncbisami.h>

/****************************************************************************/
/* INTERNAL FINCTION DEFINITIONS  */
/****************************************************************************/

#ifdef NONO
static Int4 GetPageNumElements(ISAMDataPtr data, Int4 SampleNum,
			       Int4Ptr Start);

/* ---------------------- ISAMInitSearch --------------------------
   Purpose:     Initialize ISAM Search. Checks for any errors

   Parameters:  ISAM Object
   Returns:     ISAM Error Code
   NOTE:        No need to call this function first.
  ------------------------------------------------------------------*/
static ISAMErrorCode ISAMInitSearch(ISAMObjectPtr object);

static ISAMErrorCode ISAMMakeNumericIndex(
                               ISAMDataPtr data,
                               Int4 page_size
                               );

static ISAMErrorCode ISAMMakeStringIndex(
                                 ISAMDataPtr data,
                                 Int4 page_size
                                 );
static Boolean ISAMCheckIfSorted(ISAMDataPtr data);
ISAMErrorCode ISAMCountLines(ISAMDataPtr data);
static Int4 ISAMReadLine(ISAMDataPtr data);
static Int4 ISAMDiffChar(CharPtr Term, CharPtr Key, Boolean IgnoreCase);
static void ISAMExtractData(CharPtr KeyData, 
                            CharPtr PNTR Key, CharPtr PNTR Data);
static CharPtr ISAMReadFileInMemory(CharPtr filename);

/* Coded Array - Field Array handling functions */

static Boolean ISAMWriteNBits10(ISAMTmpCAPtr cap, Int4 number);
static Boolean ISAMWriteBitNumber(ISAMTmpCAPtr cap, Int4 number);
static Uint4Ptr ISAMDecompressCA(Uint1Ptr buffer, Int4 length,
                                 Int4 num_bits, Int4 num_uids);
static Boolean ISAMCreateCA(ISAMTmpCAPtr cap, ISAMUidFieldPtr data, 
                            Int4 num_uids);
static ISAMTmpCAPtr ISAMTmpCANew(void);
static void ISAMTmpCAFree(ISAMTmpCAPtr cap);
static Boolean ISAMCreateFA(ISAMTmpCAPtr fcap, 
                             ISAMUidFieldPtr data, Int4 num_uids);
static Uint4Ptr ISAMDecompressFA(Uint1Ptr buffer, Int4 num_uids);
static ISAMErrorCode ISAMDumpTermEntry(ISAMTmpCAPtr cap, FILE *off_fd, 
                                       FILE *db_fd, ISAMUidFieldPtr uidf, 
                                       Int4 count, Int4Ptr offset);
static int LIBCALLBACK ISAMUidCompare(VoidPtr i, VoidPtr j);

#endif

/****************************************************************************/
/* EXTERNAL FINCTIONS  */
/****************************************************************************/

/* ----------------------  ISAMObjectNew --------------------------
   Purpose:     Creates ISAM object structure with default parameters
                
   Parameters:  type - Type of ISAM (Numeric, String etc. )
   Returns:     Poiner to created object structure
   NOTE:        Page size is set to default value if 0

  ------------------------------------------------------------------*/
ISAMObjectPtr ISAMObjectNew(ISAMType type,       /* Type of ISAM */
                            CharPtr DBFile,      /* ISAM Database file */
                            CharPtr IndexFile   /* ISAM Index file */
                            )
{
    ISAMDataPtr data;
    Char name[MAX_FILENAME_LEN];
	CharPtr ch, ch1;

    if(DBFile == NULL)
        return NULL;
    
    if((data = (ISAMDataPtr)MemNew(sizeof(ISAMData))) == NULL)
        return NULL;
    
    data->type = type;
    data->DBFileName = StringSave(DBFile);
    
    
    if(IndexFile == NULL) {
#ifdef OS_UNIX
        if(type == ISAMNumeric || type == ISAMNumericNoData)
            sprintf(name, "%s.nisam", DBFile);
        else
            sprintf(name, "%s.isam", DBFile);
        
        data->IndexFileName = StringSave(name);
#else
        sprintf(name, "%s", DBFile);
        
        /* Looking for last  '.' in the filename */
        for(ch = name; (ch = StringChr(ch, '.')) != NULL; ch1=ch)
            continue;
        
        if(ch1 != NULL) {
            if(type == ISAMNumeric || type == ISAMNumericNoData)
                sprintf(ch1, ".nsm");
            else
                sprintf(ch1, ".ism");                
        } else {
            if(type == ISAMNumeric || type == ISAMNumericNoData)            
                sprintf(name, "%s.nsm", DBFile);
            else
                sprintf(name, "%s.ism", DBFile);
        }
        
        data->IndexFileName = StringSave(name);
#endif
    } else {
        data->IndexFileName = StringSave(IndexFile);
    }

    if(type == ISAMNumeric || type == ISAMNumericNoData)
        data->PageSize = DEFAULT_NISAM_SIZE;
    else
        data->PageSize = DEFAULT_SISAM_SIZE;

    data->initialized = FALSE;
    data->KeySamples = NULL;
    data->KeyDataSamples = NULL;
    data->test_non_unique = TRUE; /* default - to check non-unique data */

    return (ISAMObjectPtr) data;
}
/* ----------------------  ISAMSetUpCAInfo --------------------------
   Purpose:     Added toISAM object Coded Array filenames information
                
   Parameters:  CAName - Common name for all CA/FA DB and offset files
                CADBExt - exetntio for CA/FA DB files
                CAOffExt - extention for CA/FA offset files
                MaxOffset - threshhold offset for starting write new
                            file

   Returns:     ISAM Error Code
   NOTE:        MaxOffset is set to default value if 10.000.000
  ------------------------------------------------------------------*/
ISAMErrorCode ISAMSetUpCAInfo(ISAMObjectPtr object, Int4 MaxOffset,
                              CharPtr CAName, CharPtr CADBExt,
                              CharPtr CAOffExt)
{
    ISAMDataPtr data;
    
    if(object == NULL)
        return ISAMBadParameter;
    
    data = (ISAMDataPtr) object;     
    
    data->CAName = StringSave(CAName);
    data->CADBExt = StringSave(CADBExt);
    data->CAOffExt = StringSave(CAOffExt);
    if(MaxOffset != 0)
        data->CAMaxOffset = MaxOffset;
    else
        data->CAMaxOffset = DEFAULT_CA_MAX_OFFSET;
    
    return ISAMNoError;
}

/* ---------------------- ISAMUpdateDatabase ------------------------
   Purpose:     
   
   Parameters:  
                
   Returns:     
  ------------------------------------------------------------------*/
ISAMErrorCode ISAMUpdateDatabase(CharPtr InFile,
                                 CharPtr NewDBDir, 
                                 Int4 MaxOffset, 
                                 CharPtr BaseName, 
                                 CharPtr DBExt,
                                 CharPtr IndexExt,
                                 CharPtr OffExt,
                                 CharPtr CodeExt)
{
    
    return ISAMNotImplemented;
}

static ISAMTmpCAPtr ISAMTmpCANew(void)
{
    ISAMTmpCAPtr cap;

    cap = MemNew(sizeof(ISAMTmpCA));

    cap->allocated = CA_TMP_CHUNK;
    cap->buffer = (Uint1Ptr) MemNew(cap->allocated);
    
    return cap;
}

static void ISAMTmpCAFree(ISAMTmpCAPtr cap)
{
    if(cap == NULL)
        return;

    MemFree(cap->buffer);
    MemFree(cap);
}

/* ------------------------------------------------------------------
                This is handler for HeapSort function
   ------------------------------------------------------------------*/
static int LIBCALLBACK ISAMUidCompare(VoidPtr i, VoidPtr j)
{
  if (*(Int4Ptr)i > *(Int4Ptr)j)
    return (1);
  if (*(Int4Ptr)i < *(Int4Ptr)j)
    return (-1);
  return (0);
}

static Boolean ISAMWriteBitNumber(ISAMTmpCAPtr cap, Int4 number)
{
    Int4 template;

    if(cap->num_bits == 0)
        return TRUE;
    
    template = PowersOfTwo[cap->num_bits - 1];
    
    for(; template; template >>= 1) {
        
        if(number & template)
            cap->buffer[cap->byte_num] |= OneBit[cap->bit_num] ;
        
        if(++cap->bit_num > 7) {
            cap->bit_num = 0;
            if((++cap->byte_num >= cap->allocated)) {
                cap->allocated += CA_TMP_CHUNK;
                cap->buffer = Realloc(cap->buffer, cap->allocated);
            }
        }
    }
    return TRUE;
}

static Boolean ISAMWriteNBits10(ISAMTmpCAPtr cap, Int4 number)
{
    register Int4 i;

    for(i = 0; i < number; i++) {
        cap->buffer[cap->byte_num] |= OneBit[cap->bit_num];
        if(++cap->bit_num > 7) {
            cap->bit_num = 0;
            if((++cap->byte_num >= cap->allocated)) {
                cap->allocated += CA_TMP_CHUNK;
                cap->buffer = Realloc(cap->buffer, cap->allocated);
            }
        }
    }

    /* Now wriiting 0 bit in the end of set of 1's */
    
    if(++cap->bit_num > 7) {
        cap->bit_num = 0;
        if((++cap->byte_num >= cap->allocated)) {
            cap->allocated += CA_TMP_CHUNK;
            cap->buffer = Realloc(cap->buffer, cap->allocated);
        }
    }
    return TRUE;
}

static Boolean ISAMCreateCA(ISAMTmpCAPtr cap, 
                            ISAMUidFieldPtr data, 
                            Int4 num_uids)
{
    Nlm_FloatHi AverageDiff;
    Int4 base, number, prev_number;
    Int4 i, diff, dividend;

    if(cap == NULL || data == NULL)
        return FALSE;
        
    cap->byte_num = 0;
    cap->bit_num  = 0;
    cap->length   = 0;
    cap->num_uids = num_uids;
    MemSet(cap->buffer, 0, cap->allocated);
    
    
    if((AverageDiff = (Nlm_FloatHi)(data[num_uids-1].uid - num_uids + 1) / 
        (Nlm_FloatHi)num_uids ) < 1) {
	AverageDiff = 1;
    }

    cap->num_bits = Log2(AverageDiff);
    base = PowersOfTwo[cap->num_bits];

    prev_number = -1;
    
    for(i = 0; i < num_uids; i++) {
        number = data[i].uid;

        if (number <= prev_number) {
            ErrLogPrintf("%s\n%s%ld%s%ld\n",
                         "Bad record number in writing to coded array!",
                         "Number: ", number,
                         "  Previous Number: ", prev_number);
            ISAMTmpCAFree(cap);
            return FALSE;
        }
        
        diff = number - prev_number - 1;
        
        dividend = diff/base;

        ISAMWriteNBits10(cap, dividend);
        ISAMWriteBitNumber(cap, diff);
        prev_number = number;
    }

    cap->length = cap->byte_num + (cap->bit_num != 0 ? 1 : 0);
    
    return TRUE;
}

static Boolean ISAMCreateFA(ISAMTmpCAPtr cap, 
                             ISAMUidFieldPtr uidf, Int4 num_uids)
{
    Int4 i, j;
    Int4 byte_start;

    if(cap == NULL || uidf == NULL)
        return FALSE;
    
    cap->byte_num = 0;
    cap->length   = 0;
    cap->num_uids = num_uids;    
    MemSet(cap->buffer, 0, cap->allocated);

    for(i = 0; i < num_uids; i++) {

        byte_start = cap->byte_num;
        
        for(j = 0, cap->bit_num = 0; j < 32; j++) {
            if(uidf[i].field & PowersOfTwo[j]) {
                cap->buffer[cap->byte_num] |= j;
                
                if((++cap->byte_num >= cap->allocated)) {
                    cap->allocated += CA_TMP_CHUNK;
                    cap->buffer = Realloc(cap->buffer, cap->allocated);
                }
                
                cap->bit_num++;
            }
        }
        
        for(j = 0; j < cap->bit_num -1; j++) {
            cap->buffer[byte_start+j] |=PowersOfTwo[7];
        }
    }
    
    cap->length = cap->byte_num;
    return TRUE;
}

static ISAMErrorCode ISAMDumpTermEntry(ISAMTmpCAPtr cap, FILE *off_fd, 
                                       FILE *db_fd, 
                                       ISAMUidFieldPtr uidf, 
                                       Int4 count, Int4Ptr offset)
{
    Int4 i, j, offset_out, ca_offset, num_bits;
    Uint4 numbers[32];
    Uint4 bit_flag = 0;
    Int4 length;

    offset_out = ftell(off_fd);
    ca_offset  = ftell(db_fd);
    MemSet(numbers, 0, sizeof(numbers));

    HeapSort(uidf, count, sizeof(Uint4)*2, ISAMUidCompare);

    for(i = 0; i < count; i++) {
        for(j = 0; j < 32; j++) {
            if(uidf[i].field & PowersOfTwo[j]) {
                numbers[j]++;
                bit_flag |= PowersOfTwo[j];
            }
        }
    }

    /* Calculating and writting code and field arrays */

    if(!ISAMCreateCA(cap, uidf, count)) {
        ErrLogPrintf("Cannot create coded array. Formating failed.\n");
        return ISAMInvalidFormat;
    }
    num_bits = cap->num_bits;

    FileWrite(cap->buffer, 1, cap->length, db_fd);
    length = cap->length;

    if(!ISAMCreateFA(cap, uidf, count)) {
        ErrLogPrintf("Cannot create field array. Formating failed.");
        return ISAMInvalidFormat;
    }

    FileWrite(cap->buffer, 1, cap->length, db_fd);

    /* ------- Now writting header ---------- */

    FileWrite(&count, 1, sizeof(Uint4), off_fd);
    FileWrite(&ca_offset, 1, sizeof(Uint4), off_fd);
    FileWrite(&length, 1, sizeof(Uint4), off_fd);
    FileWrite(&num_bits, 1, sizeof(Uint4), off_fd);
    FileWrite(&bit_flag, 1, sizeof(Uint4), off_fd);

    for(j = 0; j < 32; j++) {
        if(numbers[j] > 0)
            FileWrite(&numbers[j], 1, sizeof(Uint4), off_fd);
    }

    *offset = offset_out;
    return ISAMNoError;
}

/* returns NULL terminated string \n\r are removed */

static Int4 ISAMReadLine(ISAMDataPtr data)
{
    Int4 i = 0;
    Int4 ch;
    Int4 MaxChars;
    FILE *fd = data->db_fd;

    MaxChars = data->max_line_size-1;

    for(i = 0; (( ch = getc(fd)) != EOF) ; i++)  {
        if((ch == '\n') || (ch == '\r'))
            break;
        data->line[i] = (Char) ch;
        
        if(i == MaxChars) { /* Reallocating line buffer */
            data->max_line_size += LINE_SIZE_CHUNK;
            data->line = Realloc(data->line,  data->max_line_size);
            MaxChars = data->max_line_size-1;
        }
    }
    data->line[i] = NULLB;

    /* Finding first character on new line */

    while((ch = getc(fd)) != EOF) {
        if(IS_WHITESP(ch)) {
            continue;
        } else {
            ungetc(ch, fd);
            break;
        }
    }
    
    return i;
}

static Boolean ISAMCheckIfSorted(ISAMDataPtr data) 
{
    CharPtr prevline;
    Int4 length;
    CharPtr chptr;

    if(data == NULL || data->db_fd == NULL || data->max_line_size == 0)
        return FALSE;
    
    prevline = MemNew(data->max_line_size);
    
    rewind(data->db_fd);
    data->NumTerms = 0;
    
    if(data->type == ISAMString || data->type == ISAMStringDatabase) {
        while(ISAMReadLine(data) > 0) {
            data->NumTerms++;
            
            /* If not testing data - lines eventually should be counted */
            if(data->test_non_unique) {
                if((chptr = StringChr(data->line, ISAM_DATA_CHAR)) != NULL)
                    *chptr = NULLB;
                
                if (StringCmp(data->line, prevline) <= 0) {
                    ErrPostEx(SEV_WARNING, 0, 0, "Non-unique or not-sorted string IDs found %d line: '%s' %d line: '%s'", data->NumTerms, data->line, data->NumTerms-1, prevline);
                }
                length = StringLen(data->line)+1;
                StringNCpy_0(prevline, data->line, 
                             length > LINE_SIZE_CHUNK ? LINE_SIZE_CHUNK : length);
            }
        }
    } else {
        return FALSE;
    }

    rewind(data->db_fd);
    MemFree(prevline);
    return(TRUE);
}

/* ---------------------- ISAMMakeStringIndex ---------------------
   Purpose:     To create String ISAM Intex file for Database file
   
   Parameters:  ISAM Data 
   Returns:     ISAM itemized error code 
   NOTE:        Special default rules for UNIX platform
   ------------------------------------------------------------------*/
static ISAMErrorCode ISAMMakeStringIndex(
                                 ISAMDataPtr data,
                                 Int4 page_size,   /* ISAM page size */
                                 Int4 idx_option   /* Option for upper layer */
                                 )
{
    Int4 TermCount, Pos, count, SampleCount;
    Int4Ptr MasterPos, SamplePos;
    Int4 OffsetPos;
    FILE *tf_fd;
    Int4 Version = ISAM_VERSION;
    Uint4 value;

    if(page_size != 0)
        data->PageSize = page_size;
    else
        data->PageSize = DEFAULT_SISAM_SIZE;
    
    if((data->db_fd = FileOpen(data->DBFileName, "r")) == NULL)
        return ISAMBadFileName;
    
    
    /* Temporary space for line initialy set to MAX_LINE_SIZE 
       byt will be realocated if some line exceed this limit */
    
    if(data->max_line_size == 0) {
        data->max_line_size = LINE_SIZE_CHUNK;
        data->line = MemNew(LINE_SIZE_CHUNK);
    }

    /* This function will also split data if strings are
       identical and finaly count lines*/

    if(!ISAMCheckIfSorted(data))
        return ISAMNoOrder;

    /* Obtain the term offsets; select the sample terms. */
    
    MasterPos = (Int4 *)Nlm_Malloc(sizeof(Int4) * (((data->NumTerms+1)/(data->PageSize))+2));
    
    Pos = TermCount = SampleCount = 0;
    
    while(ISAMReadLine(data) > 0) {
        if (TermCount++ % data->PageSize == 0) 
            	MasterPos[SampleCount++] = SwapUint4(Pos);
        
        Pos = ftell(data->db_fd);
    }
    
    MasterPos[SampleCount] = SwapUint4(Pos);
    
    
    /* Create the sample file. */
    
    if (!(tf_fd = FileOpen(data->IndexFileName, "wb")))
    {
	MemFree(MasterPos);
        return ISAMBadFileName;
    }
    
    /* Write the term counts and offsets to the sample file. */
    value = SwapUint4(Version);     
    FileWrite((CharPtr)&value, sizeof(Int4), 1, tf_fd);
    value = SwapUint4(data->type);
    FileWrite((CharPtr)&value, sizeof(Int4), 1, tf_fd);
    value = SwapUint4(FileLength(data->DBFileName)); /* Length of DB file */
    FileWrite((CharPtr)&value, sizeof(Int4), 1, tf_fd);
    value = SwapUint4(TermCount); 
    FileWrite((CharPtr)&value,    sizeof(Int4), 1, tf_fd);
    value = SwapUint4(SampleCount); 
    FileWrite((CharPtr)&value,  sizeof(Int4), 1, tf_fd);
    value = SwapUint4(data->PageSize); 
    FileWrite((CharPtr)&value,      sizeof(Int4), 1, tf_fd);
    value = SwapUint4(data->max_line_size); 
    FileWrite((CharPtr)&value, sizeof(Int4), 1, tf_fd);
    value = SwapUint4(idx_option);
    FileWrite(&value, sizeof(Int4), 1, tf_fd);
    value = SwapUint4(0);      /* This space reserved for future use */
    FileWrite(&value, sizeof(Int4), 1, tf_fd);

    if(data->PageSize != MEMORY_ONLY_PAGE_SIZE)
        FileWrite((CharPtr)MasterPos, sizeof(Int4), SampleCount+1, tf_fd);
    
    /* Leave space for the offsets of the selected terms. */
    
    OffsetPos = ftell(tf_fd);
    SamplePos = (Int4 *)MemNew((SampleCount + 1) * sizeof(Int4));
    FileWrite((CharPtr)SamplePos, sizeof(Int4), SampleCount+1, tf_fd);
    
    /* Copy the selected terms to the sample file. */
  
    for (count = 0; count < SampleCount; count++) {
        SamplePos[count] = SwapUint4(ftell(tf_fd));
        fseek(data->db_fd, SwapUint4(MasterPos[count]), SEEK_SET);
        ISAMReadLine(data);
        fprintf(tf_fd,"%s%c",data->line, NULLB);
    }
    
    SamplePos[SampleCount] = ftell(tf_fd);
    
    /* Replace the space-holding zeroes with the offsets of the selected 
       terms.*/
    
    fseek(tf_fd, OffsetPos, SEEK_SET);
    FileWrite((CharPtr)SamplePos, sizeof(Int4), SampleCount+1, tf_fd);

    FileClose(tf_fd);
    FileClose(data->db_fd);
    data->db_fd = NULL;

    MemFree(SamplePos);
    MemFree(MasterPos);
    
    MemFree(data->line);
    data->max_line_size = 0;

    return ISAMNoError;
}

/* ------------------- ISAMReadFileInMemory  -----------------------
   Purpose:     Function reads data from file into a buffer 

   Parameters:  filename -  Name of file to read file 

   Returns:     Pointer to allocated buffer.
  ------------------------------------------------------------------*/

static CharPtr ISAMReadFileInMemory(CharPtr filename)
{
    CharPtr  in_buff;
    Int4     new_size = BUFF_SIZE_CHUNK;
    Int4     bytes = 0, buff_len = 0;
    FILE *fd;
    
    if(filename == NULL)
        return NULL;
    
    if((fd = FileOpen(filename, "rb")) == NULL)
        return NULL;
    
    /* initial allocation of memory */
    
    if((in_buff = MemNew(BUFF_SIZE_CHUNK)) == NULL) {
        ErrLogPrintf("Error in allocating memory\n");
        FileClose(fd);
        return NULL;
    }
    
    while ((bytes = FileRead(in_buff + buff_len, 1,
                             BUFF_SIZE_CHUNK, fd)) > 0) {
        new_size += bytes;
        buff_len += bytes;
        
        if ((in_buff = Realloc(in_buff, new_size)) == NULL) {
            ErrLogPrintf("Error in reallocating memory\n");
            FileClose(fd);
            return NULL;
        }
    }

    FileClose(fd);
    return(in_buff);
}

/* ---------------------- ISAMMakeNumericIndex ---------------------
   Purpose:     To create Numeric ISAM Intex file for Database file
   
   Parameters:  ISAM Data 
   Returns:     ISAM itemized error code 
   NOTE:        Special default rules for UNIX platform
   ------------------------------------------------------------------*/
static ISAMErrorCode ISAMMakeNumericIndex(
                                  ISAMDataPtr data,
                                  Int4 page_size,  /* ISAM page size */
                                  Int4 idx_option  /* Option for upper layer */
                                  )
{
    Int4 i, NumTerms, value;
    Int4 MaxSamples, SampleCount;
    Uint4Ptr KeyInfo, KeySamples;
    NISAMKeyDataPtr KeyDataInfo, KeyDataSamples;
    Boolean NoData;
    FILE *fd;
    Int4 Version = ISAM_VERSION;

    NoData = (data->type == ISAMNumericNoData);
    
    NumTerms = FileLength(data->DBFileName) / 
        (NoData ? sizeof(Uint4) : sizeof(NISAMKeyData));
    
    if(!Nlm_MemMapAvailable()) {
        if((data->FileStart = 
            ISAMReadFileInMemory(data->DBFileName)) == NULL)
            return ISAMBadFileName;
        if (NoData)
            KeyInfo = (Uint4Ptr) data->FileStart;
        else
            KeyDataInfo = (NISAMKeyDataPtr)data->FileStart;
    } else {    
        if((data->mmp = Nlm_MemMapInit(data->DBFileName)) == NULL)
            return ISAMMemMap;
        if (NoData)
            KeyInfo = (Uint4Ptr) data->mmp->mmp_begin;
        else
            KeyDataInfo = (NISAMKeyDataPtr)data->mmp->mmp_begin;
    }

    if(page_size != 0)
        data->PageSize = page_size;
    else
        data->PageSize = DEFAULT_NISAM_SIZE;   
    
#ifndef CHECK_ORDER
    for (i = 1; i < NumTerms; i++) {
        if (NoData) {
            if (SwapUint4(KeyInfo[i]) <= SwapUint4(KeyInfo[i-1]))
                break;
        } else {
            if (SwapUint4(KeyDataInfo[i].key) <= SwapUint4(KeyDataInfo[i-1].key))
                break;
        }
    }
    
    if (i < NumTerms) {
        ErrLogPrintf("NIsam key file %s not in sorted order!\n", 
                     data->DBFileName);
        
        if (NoData) {
            ErrLogPrintf("unsorted or non-unique elements:"
                         "#%ld, #%ld :  %ld, %ld\n",
                         i-1, i, SwapUint4(KeyInfo[i-1]), 
                         SwapUint4(KeyInfo[i]));
        } else {
            ErrLogPrintf("unsorted or non-unique elements:"
                         "#%ld, #%ld : %ld, %ld\n",
                         i-1, i, SwapUint4(KeyDataInfo[i-1].key), 
                         SwapUint4(KeyDataInfo[i].key));
        } 
        return ISAMNoOrder;
    } 
#endif 
    
    /* Obtain the term offsets; select the sample terms. */
    
    MaxSamples = NumTerms/data->PageSize + 4;
    
    if (NoData)
        KeySamples = (Uint4Ptr)MemNew(sizeof(Uint4)*(MaxSamples+1));
    else
        KeyDataSamples = (NISAMKeyDataPtr) MemNew(sizeof(NISAMKeyData)*
                                                  (MaxSamples+1));
    SampleCount = 0;
    
    for (i = 0; i < NumTerms; i++) {
        if (i % data->PageSize == 0) {
            if (NoData)
                KeySamples[SampleCount] = KeyInfo[i];
            else
                KeyDataSamples[SampleCount] = KeyDataInfo[i];
            SampleCount++;
        }
    }
    
    if (NoData) {
        KeySamples[SampleCount] = SwapUint4(UINT4_MAX);
    } else {
        KeyDataSamples[SampleCount].key = SwapUint4(UINT4_MAX);
        KeyDataSamples[SampleCount].data = SwapUint4(0);
    }
    
    /* Create the sample file. */
    
    if((fd = FileOpen(data->IndexFileName, "wb")) == NULL)
      return ISAMBadFileName;
    
    /* Write the term counts and offsets to the sample file. */

    value = SwapUint4(Version);         /* Index version */
    FileWrite((CharPtr)&value, sizeof(Int4), 1, fd);
    value = SwapUint4(data->type);      /* Index type */
    FileWrite(&value, sizeof(Int4), 1, fd);
    value = SwapUint4(FileLength(data->DBFileName)); /* Length of DB file */
    FileWrite(&value, sizeof(Int4), 1, fd);   
    value = SwapUint4(NumTerms);        /* Number of terms in DB file */
    FileWrite(&value, sizeof(Int4), 1, fd);
    value = SwapUint4(SampleCount);     /* Number of elements in index file */
    FileWrite(&value, sizeof(Int4), 1, fd);
    value = SwapUint4(data->PageSize);  /* Page size of ISAM */
    FileWrite(&value, sizeof(Int4), 1, fd);
    value = SwapUint4(0);      /* 0  max_line-size for strings here */
    FileWrite(&value, sizeof(Int4), 1, fd);
    value = SwapUint4(idx_option);  /* Option for the upper layer */
    FileWrite(&value, sizeof(Int4), 1, fd);
    value = SwapUint4(0);      /* This space reserved for future use */
    FileWrite(&value, sizeof(Int4), 1, fd);
    
    if (NoData) /* No swaping neeeded here */
        FileWrite((VoidPtr)KeySamples, sizeof(Uint4), SampleCount+1, fd);
    else
        FileWrite((VoidPtr)KeyDataSamples, sizeof(NISAMKeyData),
                  SampleCount+1, fd);
    
    FileClose(fd);

    if(data->mmp != NULL) {
        Nlm_MemMapFini(data->mmp);
        data->mmp = NULL;
    } else {
        MemFree(data->FileStart);
        data->FileStart = NULL;
    }

    if (NoData) 
        MemFree(KeySamples);
    else 
        MemFree(KeyDataSamples);
    
    return ISAMNoError;
}

/* ---------------------- ISAMMakeIndex --------------------------
   Purpose:     To create ISAM Intex file for Database file
   
   Parameters:  ISAM Object 
   Returns:     ISAM itemized error code 
   NOTE:        Special default rules for UNIX platform
  ------------------------------------------------------------------*/
ISAMErrorCode ISAMMakeIndex(ISAMObjectPtr object,
                            Int4 page_size,       /* ISAM page size */
                            Int4 idx_option       /* Option for upper layer */
                            )
{
    ISAMDataPtr data;

    if(object == NULL)
        return ISAMBadParameter;
    
    data = (ISAMDataPtr) object;
    
    if(data->type == ISAMString || data->type == ISAMStringDatabase)
        return ISAMMakeStringIndex(data, page_size, idx_option);
    else if (data->type == ISAMNumeric || data->type == ISAMNumericNoData)
        return ISAMMakeNumericIndex(data, page_size, idx_option);
    else 
        return ISAMNotImplemented;
}

/* ---------------------- ISAMCreateDatabase ------------------------
   Purpose:     To create coded array/offsets and ISAM database files 
                from input files in special form:
                All files are in sorted order and sorted through
                format: <term><\2><uid><field-bit mask><CR>
   
   Parameters:  ISAM Object 
                files - list of sorted files to process
   Returns:     ISAM itemized error code 
  ------------------------------------------------------------------*/
ISAMErrorCode ISAMCreateDatabase(CharPtr PNTR files,
                                 Int4 num_files, 
                                 Int4 MaxOffset, 
                                 CharPtr BaseName, 
                                 CharPtr DBExt,
                                 CharPtr IndexExt,
                                 CharPtr OffExt,
                                 CharPtr CodeExt)

{
    ISAMDataPtr data;
    ISAMErrorCode error;
    Char DBName[MAX_FILENAME_LEN], filename[MAX_FILENAME_LEN];
    FILE *ca_fd, *off_fd, *out_fd;
    long count = 0, ca_count = 0, files_count;
    Int4 offset;
    unsigned long uid_in, uid_last, field_in;
    CharPtr chptr;
    CharPtr term, value;
    Int4 uidf_allocated;
    CharPtr prevterm;
    ISAMTmpCAPtr cap;
    ISAMUidFieldPtr uidf;
    long int lvalue1, lvalue2;

    if(BaseName == NULL || files == NULL)
      return ISAMBadParameter;

    if(DBExt != NULL)
      sprintf(DBName, "%s.%s", BaseName, DBExt);
    else
      sprintf(DBName, "%s", BaseName);

    if(IndexExt != NULL)
      sprintf(filename, "%s.%s", BaseName, IndexExt);
    
    if((data = (ISAMDataPtr) ISAMObjectNew(ISAMStringDatabase, DBName,
                                           IndexExt == NULL ? 
                                           NULL : filename)) == NULL) {
        ErrLogPrintf("Creating of ISAM object failed\n");
        return ISAMMiscError;
    }

    data->CAName = StringSave(BaseName);
    data->CADBExt = StringSave(CodeExt);
    data->CAOffExt = StringSave(OffExt);

    if(MaxOffset != 0)
        data->CAMaxOffset = MaxOffset;
    else
        data->CAMaxOffset = DEFAULT_CA_MAX_OFFSET;
    
    sprintf(filename, "%s%ld.%s",
            data->CAName, (long) ca_count, data->CADBExt);

    if((out_fd = FileOpen(data->DBFileName, "bw")) == NULL)
        return ISAMBadFileName;
    
    sprintf(filename, "%s%ld.%s", 
            data->CAName, (long) ca_count, data->CADBExt);
    
    if((ca_fd  = FileOpen(filename, "bw")) == NULL)
        return ISAMBadFileName;
    
    sprintf(filename, "%s%ld.%s", 
            data->CAName, (long) ca_count, data->CAOffExt);
    if((off_fd = FileOpen(filename, "bw")) == NULL)
        return ISAMBadFileName;

    if(data->max_line_size == 0) {
        data->max_line_size = LINE_SIZE_CHUNK;
        data->line = MemNew(LINE_SIZE_CHUNK);
    }

    uidf = MemNew(sizeof(ISAMUidField)*UID_NUM_CHUNK);
    uidf_allocated = UID_NUM_CHUNK;

    cap = ISAMTmpCANew();

    for(files_count = 0; files_count < num_files; files_count++) {

        if((data->db_fd = FileOpen(files[ca_count], "r")) == NULL)
            return ISAMBadFileName;
    
        /* Reading first entry */
        
        ISAMReadLine(data);
        if((chptr = StringChr(data->line, ISAM_DATA_CHAR)) == NULL) {
          ErrLogPrintf("No ISAM delimiter char present in input. \n"
                       "Line: \"%s\" \n", 
                       data->line);
            return ISAMMiscError;
        }

        *chptr = NULLB;
        term = data->line;
        value = chptr + 1;
        
        if((sscanf(value, "%ld %ld", 
                   &lvalue1, &lvalue2)) != 2 || lvalue2 == 0){
            ErrLogPrintf("Invalidly formatted input file\n");
            return ISAMMiscError;
        }
        
        uidf[count].uid = lvalue1;
        uidf[count].field = lvalue2;

        uid_last = uidf[count].uid;

        if(++count > UID_NUM_CHUNK) {
          uidf_allocated += UID_NUM_CHUNK;
          uidf = Realloc(uidf, sizeof(ISAMUidField)*uidf_allocated);
        }
        
        prevterm = MemNew(LINE_SIZE_CHUNK);
        StringNCpy(prevterm, term, LINE_SIZE_CHUNK);            
        
        /* Reading to the end of file */
        
        while(ISAMReadLine(data) > 0) {
          if(data->line[0] == NULLB)
            continue; 
          if((chptr = StringChr(data->line, ISAM_DATA_CHAR)) == NULL) {
            ErrLogPrintf("No ISAM delimiter precent in the input\n"
                         "Line: \"%s\" \nPrevterm: \"%s\"\n", 
                         data->line, prevterm);
            return ISAMMiscError;
          }

            *chptr = NULLB;
            term = data->line;
            value = ++chptr;
            
            /* Yes, we got new term, so closing information about
               previous */
            
            if(StringCmp(term, prevterm)) {
              if((error = ISAMDumpTermEntry(cap, off_fd, ca_fd, 
                                            uidf, count, &offset)) !=
                   ISAMNoError) {
                  ErrLogPrintf("Failed to dump entry. All failed!\n"
                               "Term: \"%s\"\n", prevterm);
                    return error;
                }

                if(offset > data->CAMaxOffset) {
                    FileClose(ca_fd);
                    FileClose(off_fd);
                    ca_count++;

                    sprintf(filename, "%s%ld.%s", 
                            data->CAName, (long) ca_count, data->CADBExt);
                    if((ca_fd  = FileOpen(filename, "bw")) == NULL)
                        return ISAMBadFileName;
                    
                    sprintf(filename, "%s%ld.%s", 
                            data->CAName, (long) ca_count, data->CAOffExt);
                    if((off_fd = FileOpen(filename, "bw")) == NULL)
                        return ISAMBadFileName;
                }

                fprintf(out_fd, "%s%c%ld %ld\n", prevterm, 
                        ISAM_DATA_CHAR, (long) offset, (long) ca_count);
                count = 0;
                StringNCpy(prevterm, term, LINE_SIZE_CHUNK);            
                uid_last = -1;
            }
            
            if((sscanf(value, 
                       "%ld %ld", &uid_in, &lvalue2)) != 2 || lvalue2 == 0) {
                ErrLogPrintf("Invalidly formatted database. Database creation "
                             "failed.\nValue = \"%s\", Field = %ld\n",
                             value, (long) field_in);
                return ISAMInvalidFormat;
            }

            uid_in = lvalue1;
            field_in = lvalue2;
            
            if(uid_last == uid_in) {
                uidf[count].field |= field_in;
            } else {
                uidf[count].uid = uid_in;
                uidf[count].field = field_in;
                uid_last = uid_in;
                count++;
            }

            if(count >= uidf_allocated) {
                uidf_allocated += UID_NUM_CHUNK;
                uidf = Realloc(uidf, sizeof(ISAMUidField)*uidf_allocated);
            }
        }
        /* Writting last entry */
        
        if((error = 
            ISAMDumpTermEntry(cap, off_fd, ca_fd, 
                              uidf, count, &offset)) != ISAMNoError) {
            ErrLogPrintf("Failed to dump entry. All failed!\n");
            return error;
        }

        fprintf(out_fd, "%s%c%ld %ld\n", prevterm, 
                ISAM_DATA_CHAR, (long) offset, (long) ca_count);

        FileClose(data->db_fd);
        data->db_fd = NULL;
    }

    ISAMTmpCAFree(cap);
    MemFree(uidf);
    MemFree(prevterm);

    FileClose(ca_fd);
    FileClose(out_fd);
    FileClose(off_fd);

    if((error = ISAMMakeIndex((VoidPtr)data, 0, 0)) != ISAMNoError) {
        ErrLogPrintf("Failed to create ISAM String Index All failed!\n");
        return error;
    }
    ISAMObjectFree((VoidPtr)data);
    return ISAMNoError;
}

/* ---------------------- ISAMInitSearch --------------------------
   Purpose:     Initialize ISAM Numeric Search. Checks for any errors
   
   Parameters:  ISAM Object
   Returns:     ISAM Error Code
   NOTE:        None
   ------------------------------------------------------------------*/
static ISAMErrorCode ISAMInitSearch(ISAMObjectPtr object)
{
    
    Int4Ptr FileInfo;
    Int4 Version, IsamType, DBFileLength;
    ISAMDataPtr data;
    Int4 reserved2;

    if(object == NULL)
        return ISAMBadParameter;
    
    data = (ISAMDataPtr) object;
    
    if(data->initialized == TRUE)
        return ISAMNoError;
    
    if(!Nlm_MemMapAvailable()) {
        if((data->FileStart = 
            ISAMReadFileInMemory(data->IndexFileName)) == NULL)
            return ISAMBadFileName;
        FileInfo = (Int4Ptr)data->FileStart;
    } else { 
        if((data->mmp = Nlm_MemMapInit(data->IndexFileName)) == NULL)
            return ISAMMemMap;

        FileInfo = (Int4Ptr)data->mmp->mmp_begin;
    }
    /* For numeric search. */
    data->mfp = NlmOpenMFILE(data->DBFileName);

    /* Check for consistence of files and parameters */

    if((Version   = SwapUint4(FileInfo[0])) != ISAM_VERSION) 
        return ISAMBadVersion;
    
    if((IsamType  = SwapUint4(FileInfo[1])) != data->type)
        return ISAMBadType;
    
    data->NumTerms      = SwapUint4(FileInfo[3]);
    data->NumSamples    = SwapUint4(FileInfo[4]);
    data->PageSize      = SwapUint4(FileInfo[5]);
    data->max_line_size = SwapUint4(FileInfo[6]);

    if(data->PageSize != MEMORY_ONLY_PAGE_SIZE) { 
        /* Special case of memory-only index */
        if((DBFileLength = 
            SwapUint4(FileInfo[2])) != FileLength(data->DBFileName))
            return ISAMWrongFile;
    }

    /* This space reserved for future use */

    data->idx_option    =  SwapUint4(FileInfo[7]);
    reserved2           =  SwapUint4(FileInfo[8]);

    if(data->max_line_size != 0)
        data->line = MemNew(data->max_line_size + 1);
    
    if(data->type == ISAMNumeric)
        data->KeyDataSamples = (NISAMKeyDataPtr)(FileInfo + 9);
    else
        data->KeySamples = (Uint4Ptr)(FileInfo + 9);

#if 0
    /* CC:This causes a file descriptor leak and it is not being used */
    if(data->PageSize != MEMORY_ONLY_PAGE_SIZE) { 
        /* Special case of memory-only index */
        if (!(data->db_fd = FileOpen(data->DBFileName, "rb"))) {
            ErrLogPrintf("Unable to open NISAM DB file %s for reading!\n",
                         data->DBFileName);
            return ISAMBadFileName;
        }
    }
#endif

    data->initialized = TRUE;
    return ISAMNoError;
}

/* ------------------------ ISAMGetIdxOption ------------------------
   Purpose:     Returns user specified option from ISAM database

   Parameters:  ISAM object
   Returns:     User specified option (set while formating)
   NOTE:        None
  ------------------------------------------------------------------*/
ISAMErrorCode ISAMGetIdxOption(ISAMObjectPtr object, Int4Ptr idx_option)
{
    ISAMDataPtr data;
    ISAMErrorCode error;
    
    if(object == NULL)
        return ISAMMiscError;
    
    data = (ISAMDataPtr) object;
    
    if(data->initialized == FALSE) {
        if((error = ISAMInitSearch(object)) != ISAMNoError)
            return error;
    }
    
    *idx_option = data->idx_option;
    
    return ISAMNoError;
}

/* ------------------------ ISAMGetIdxOption ------------------------
   Purpose:     To set option to check or not check for non-unique
                elements
   Parameters:  ISAM object
   Returns:     None
   NOTE:        None
  ------------------------------------------------------------------*/
void ISAMSetCheckForNonUnique(ISAMObjectPtr object, Boolean test_non_unique)
{
    ISAMDataPtr data;

    if(object == NULL)
        return;

    data = (ISAMDataPtr) object;

    data->test_non_unique = test_non_unique;

    return;
}

/* ---------------------- ISAMUninitSearch --------------------------
   Purpose:     Uninitialize an ISAM search (free all allocated and used 
                buffers and unmap and close all mapped/opened files).
                Undoes what the ISAMInitSearch function does.
   Parameters:  ISAM object
   Returns:     ISAM Error Code
   NOTE:        None
  ------------------------------------------------------------------*/
ISAMErrorCode ISAMUninitSearch(ISAMObjectPtr object)
{
    ISAMDataPtr data = NULL;
    
    if (!object)
        return ISAMBadParameter;

    if ( !(data = (ISAMDataPtr) object))
        return ISAMBadParameter;

    if (data->initialized == FALSE)
        return ISAMNoError;

    if (data->mmp != NULL) {
        Nlm_MemMapFini(data->mmp);
        data->mmp = NULL;
    } else {
        MemFree(data->FileStart);
        data->FileStart = NULL;
    }

    if (data->db_fd != NULL)
        FileClose(data->db_fd);  
    
    NlmCloseMFILE(data->mfp);

    if (data->max_line_size != 0) {
        data->max_line_size = 0;
        MemFree(data->line);
        data->line = NULL;
    }
    
    data->initialized = FALSE;

    return ISAMNoError;
}
/* ---------------------- ISAMObjectFree --------------------------
   Purpose:     To terminate all allocated and used buffers
                unmap and close all mapped/opened files
   Parameters:  ISAM object
   Returns:     None
   NOTE:        None
  ------------------------------------------------------------------*/

void ISAMObjectFree(ISAMObjectPtr object)
{
    ISAMDataPtr data = (ISAMDataPtr) object;

    if (ISAMUninitSearch(object) != ISAMNoError)
        return;

    if((data = (ISAMDataPtr) object) == NULL)
        return;

    MemFree(data->DBFileName);
    MemFree(data->IndexFileName);
    MemFree(data->CAName);
    MemFree(data->CADBExt);
    MemFree(data->CAOffExt);

    MemFree(data);
    
    return;
}

static Int4 GetPageNumElements(ISAMDataPtr data, Int4 SampleNum,
			       Int4Ptr Start)
{
    Int4 NumElements;
    
    *Start = SampleNum * data->PageSize;
    NumElements = (SampleNum + 1 == data->NumSamples) ? 
        data->NumTerms - *Start : data->PageSize;
    
    return NumElements;
}

#define NCBISAM_ITER_MAX 30
/* ------------------------ NISAMSearch ----------------------------
   Purpose:     Main search function of Numeric ISAM
   
   Parameters:  Key - interer to search
                Data - returned value (for NIASM with data)
                Index - internal index in database
   Returns:     ISAM Error Code
   NOTE:        None
  ------------------------------------------------------------------*/
ISAMErrorCode NISAMSearch(ISAMObjectPtr object, 
                          Uint4    Number, 
                          Uint4Ptr Data,
                          Uint4Ptr Index
                          )
{
    Boolean found;
    ISAMDataPtr data;
    Int4 Start = 0, Stop, SampleNum;
    Int4 NumElements, *KeyPage, *KeyPageStart;
    Int4 first, last, current, index, type;
    Boolean NoData;
    NISAMKeyDataPtr KeyDataPage=NULL, KeyDataSamples, KeyDataPageStart;
    Uint4Ptr KeySamples;
    Uint4 Key;
    ISAMErrorCode error;

    if((data = (ISAMDataPtr) object) == NULL)
        return ISAMBadParameter;
    
    if(data->initialized == FALSE) {
        if((error = ISAMInitSearch(object)) != ISAMNoError)
            return error;
    }

    NoData = (data->type == ISAMNumericNoData);
    KeyDataSamples = data->KeyDataSamples;
    KeySamples = data->KeySamples;
    type = data->type;
  
    /* search the sample file. */
    
    Stop = data->NumSamples -1;
    
    if (!data->lastKeyDataPage || Number <= data->first_gi || Number >= data->last_gi)
    {
       while(Stop >= Start) {
        SampleNum = ((Uint4)(Stop + Start)) >> 1;
        if (type == ISAMNumericNoData)
            Key = SwapUint4(KeySamples[SampleNum]);
        else
            Key = SwapUint4(KeyDataSamples[SampleNum].key);
        
        /* If this is an exact match, return the master term number. */
        
        if (Key == Number) {
            if (Data != NULL) {
                if (NoData) {
                    *Data = SampleNum * data->PageSize;
                } else {
                    *Data = SwapUint4(data->KeyDataSamples[SampleNum].data);
                }
            }
            if(Index != NULL)
                *Index = SampleNum * data->PageSize;
            
	    /* NULL this out so we don't confuse the next lookup. */
	    data->lastKeyDataPage = NULL;

            return ISAMNoError;
         }
        
         /* Otherwise, search for the next sample. */
         if ( Number < Key )
            Stop = --SampleNum;
         else
            Start = SampleNum +1;
       }
    
       /* If the term is out of range altogether, report not finding it. */
    
       if ( (SampleNum < 0) || (SampleNum >= data->NumSamples)) {
        
        if (Data != NULL)
            *Data = ISAMNotFound;
        
        if(Index != NULL)
            *Index = ISAMNotFound;
        
        return ISAMNotFound;
      }
    
      /* load the appropriate page of numbers into memory. */
    
      NumElements = GetPageNumElements(data, SampleNum, &Start);
      first = Start;
      last = Start + NumElements - 1;

      if (NoData) {
          if (data->mfp->mfile_true)
          {
              NlmSeekInMFILE(data->mfp, Start*sizeof(Int4), SEEK_SET);
              KeyPageStart = (Int4Ptr) data->mfp->mmp;
              KeyPage = KeyPageStart - Start;
          }
          else
          {
              KeyPageStart = (Int4Ptr) MemNew((NumElements + 1) * sizeof(Int4));
              NlmSeekInMFILE(data->mfp, Start*sizeof(Int4), SEEK_SET);
              NlmReadMFILE((Uint1Ptr)KeyPageStart, sizeof(Int4), NumElements,
                      data->mfp);
              KeyPage = KeyPageStart - Start;
          }
      } else {
          if (data->mfp->mfile_true)
          {
              NlmSeekInMFILE(data->mfp, Start*sizeof(NISAMKeyData), SEEK_SET);
              KeyDataPageStart = (NISAMKeyDataPtr) data->mfp->mmp;
              KeyDataPage = KeyDataPageStart - Start;
              /* The following data is used if the next lookup is on the same page. */
              data->first_gi = SwapUint4(KeyDataPage[first].key);
              data->last_gi = SwapUint4(KeyDataPage[last].key);
              data->first = first;
              data->last = last;
              data->lastKeyDataPage = KeyDataPage;
          }
          else
          {
              KeyDataPageStart = (NISAMKeyDataPtr) MemNew((NumElements + 1) * 
                                                 sizeof(NISAMKeyData));
              NlmSeekInMFILE(data->mfp, Start*sizeof(NISAMKeyData), SEEK_SET);
              NlmReadMFILE((Uint1Ptr)KeyDataPageStart, sizeof(NISAMKeyData), NumElements,
                      data->mfp);
              KeyDataPage = KeyDataPageStart - Start;
          }
      }
    }
    else
    {
        first = data->first;
        last = data->last;
        KeyDataPage = data->lastKeyDataPage;
    }
    
    found = FALSE;
    /* Search the page for the number. */
    if (NoData) {
        while (first <= last)
        {
            current = (first+last)/2;
            Key = SwapUint4(KeyPage[current]);
            if (Key > Number)
                last = --current;
            else if (Key < Number)
                first = ++current;
            else
            {
                found = TRUE;
                break;
            }
        }
    } else {
        while (first <= last)
        {
            current = (first+last)/2;
            Key = SwapUint4(KeyDataPage[current].key);
            if (Key > Number)
                last = --current;
            else if (Key < Number)
                first = ++current;
            else
            {
                found = TRUE;
                break;
            }
        }
    }

    
    if (found == FALSE) /* not found. */
    {
        
        if (Data != NULL)
            *Data = ISAMNotFound;
        
        if(Index != NULL)
            *Index = ISAMNotFound;
        
	if (data->mfp->mfile_true == FALSE)
	{
    	    if (NoData)
       	     	KeyPageStart = MemFree(KeyPageStart);
       	    else
       	     	KeyDataPageStart = MemFree(KeyDataPageStart);
	}
        
        return ISAMNotFound;
    }
    
    if (Data != NULL) {
        if (NoData)
            *Data = Start + current;
        else
            *Data =  SwapUint4(KeyDataPage[current].data);
    }
    
    if(Index != NULL)
        *Index = Start + current;
    
    if (data->mfp->mfile_true == FALSE)
    {
    	    if (NoData)
       	     	KeyPageStart = MemFree(KeyPageStart);
       	    else
       	     	KeyDataPageStart = MemFree(KeyDataPageStart);
    }
    
    return ISAMNoError;
}

/* ---------------------- NISAMSearchList --------------------------
   Purpose:       Perform search of multiple Keys
   
   Parameters:    NumKeys - number of input keys
                  Keys - array of keys
   Returns:       Data - array of returned values
                  Index - array of internal indexes
   NOTE:          None
  ------------------------------------------------------------------*/
ISAMErrorCode NISAMSearchList(ISAMObjectPtr object, 
                              Int4     NumKeys, 
                              Uint4Ptr Keys, 
                              Uint4Ptr Data,
                              Uint4Ptr Index
                              )
{
    Int4 count;
    ISAMErrorCode error;
    
    if (object == NULL || Data == NULL)
        return ISAMBadParameter;
    
    for (count = 0; count < NumKeys; count++) {
        if((error = NISAMSearch(object, Keys[count], 
                                Data + count, Index + count)) < 0)
            return error;
    }
    
    return ISAMNoError;
}

static Uint4Ptr ISAMDecompressCA(Uint1Ptr buffer, Int4 length, 
                                 Int4 num_bits, Int4 num_uids)
{
    Uint4Ptr data;
    Int4 diff, dividend;
    Int4 i, template, base;
    Int4 byte_num = 0, bit_num = 0;

    if(buffer == NULL || num_uids == 0)
        return NULL;
    
    data = MemNew(sizeof(Uint4)*num_uids);

    base = PowersOfTwo[num_bits];

    for(i = 0; i < num_uids; i++) {

        diff = dividend = 0;

        if(num_bits != 0)
            template = PowersOfTwo[num_bits - 1];
        else
            template = 0;

        /* Reading dividend first */
        
        while(buffer[byte_num] & OneBit[bit_num]) {
            dividend++;            
            if(++bit_num > 7) {
                bit_num = 0;
                byte_num++;
            }
        }

        /* And skipping following 0 bit */

        if(++bit_num > 7) {
            bit_num = 0;
            byte_num++;
        }

        for(; template; template >>= 1) {
            if(buffer[byte_num] & OneBit[bit_num])
                diff |= template;
            
            if(++bit_num > 7) {
                bit_num = 0;
                byte_num++;
            }        
        }
        data[i] = dividend*base + diff + (i == 0 ? 0 : (data[i-1] + 1));

    }  /* Over all uids */

    return data;
}

static Uint4Ptr ISAMDecompressFA(Uint1Ptr buffer, Int4 num_uids)
{
    Uint4Ptr fields;
    Int4 i, j;
    
    if(buffer == NULL || num_uids == 0)
        return NULL;
    
    fields = MemNew(sizeof(Uint4)*num_uids);
    
    for(i = 0, j =0; j < num_uids; i++) {
        
        fields[j] |= PowersOfTwo[buffer[i] & FA_Mask]; 
        
        if(!(buffer[i] & 0x80)) {
            j++;
        }
    }

    return fields;
}

/* ------------------------ ISAMSearchTerm -------------------------
   Purpose:     Main search function of complete String ISAM

   Parameters:  object - ISAM Object
                term_in - input string
                field_mask - fields to search in 0 and -1 mean search
                all fields
                uid - array of returned uids
                count number of returned uids
   Returns:     ISAM Error Code
   NOTE:        Initialization done with first call to this function
  ------------------------------------------------------------------*/
ISAMErrorCode ISAMSearchTerm(ISAMObjectPtr object, CharPtr term_in, 
                             Uint4 field_mask, Uint4Ptr PNTR uid_out, 
                             Int4Ptr count)
{
    ISAMErrorCode error;
    ISAMDataPtr data;
    CharPtr term = NULL, value = NULL;
    Uint1Ptr buffer;
    Int4 ca_count  = 0, num_uids = 0, num_bits, field_len=0;
    Int4 offset, i, j, length;
    Char filename[MAX_FILENAME_LEN];
    FILE *ca_fd, *off_fd;
    Uint4 bit_flag = 0;
    Uint4 numbers[32];
    Uint4 index;
    Uint4Ptr field, uid;
    long int lvalue1, lvalue2;

    if((data = (ISAMDataPtr) object) == NULL)
        return ISAMBadParameter;

    /* First searching for term in database */

    if((error = SISAMSearch(object, term_in, 
                            0, &term, &value, &index)) != ISAMNoError) {
        *count = 0;
        *uid_out = NULL;
        return error;
    }

    MemFree(term);

    /* Now retriving information about uids and fields */

    if((sscanf(value, "%ld %ld", &lvalue1, &lvalue2)) != 2) {
        ErrLogPrintf("Error in database formating (%s)\n", value);
        return error;
    }

    offset = lvalue1;
    ca_count = lvalue2;
    
    MemFree(value);

    /* Opening corresponding files */
    
    sprintf(filename, "%s%ld.%s", 
            data->CAName, (long) ca_count, data->CAOffExt);

    if((off_fd = FileOpen(filename, "r")) == NULL)
        return ISAMBadFileName;

    sprintf(filename, "%s%ld.%s", 
            data->CAName, (long) ca_count, data->CADBExt);
    
    if((ca_fd  = FileOpen(filename, "r")) == NULL)
        return ISAMBadFileName;
    
    /* Getting header/offset information */
    
    MemSet(numbers, 0, sizeof(numbers));
    
    if((fseek(off_fd, offset, SEEK_SET)) != 0)
        return ISAMFseekFailed;
    

    FileRead(&num_uids, 1, sizeof(Uint4), off_fd);
    FileRead(&offset, 1, sizeof(Uint4), off_fd);
    FileRead(&length, 1, sizeof(Uint4), off_fd);
    FileRead(&num_bits, 1, sizeof(Uint4), off_fd);
    FileRead(&bit_flag, 1, sizeof(Uint4), off_fd);
    
    if(field_mask == 0)
        field_mask = (Uint4)(-1);

    if(!(bit_flag & field_mask)) { /* Do not satisfy given bitmask */
        *count = 0;
        *uid_out = NULL;
        return ISAMNotFound;
    }

    for(j = 0; j < 32; j++) {
        if(bit_flag & PowersOfTwo[j]) {
            FileRead(&numbers[j], 1, sizeof(Uint4), off_fd);
            field_len += numbers[j];
        }
    }
    
    /* Now reading uids and fields from CA/FA file */
    
    fseek(ca_fd, offset, SEEK_SET);
    buffer = MemNew(length);
    FileRead(buffer, 1, length, ca_fd);
    
    if((uid = ISAMDecompressCA(buffer, length, 
                                num_bits, num_uids)) == NULL) {
        ErrLogPrintf("Cannot decompress coded array. Retrieve failed.");
        return ISAMMiscError;
    }
    
    MemFree(buffer);
    buffer = MemNew(field_len);

    FileRead(buffer, 1, field_len, ca_fd);    

    if((field = ISAMDecompressFA(buffer, num_uids)) == NULL) {
        ErrLogPrintf("Cannot decompress fields array. Retrieve failed.");
        return ISAMMiscError;
    }

    /* Now filtering returned uids by field_mask */
    
    for(i = 0, j = 0; i < num_uids; i++) {
        if(field[i] & field_mask) {
            uid[j] = uid[i];
            j++;
        }
    }

    *count = j;
    *uid_out = uid;

    MemFree(field);
    MemFree(buffer);
    FileClose(ca_fd);
    FileClose(off_fd);

    return ISAMNoError;
}

static Int4 ISAMGetDataNumber(CharPtr KeyData)
{
    CharPtr chptr;
    Int4 count, value;
    long int lvalue;

    if((chptr = StringChr(KeyData, ISAM_DATA_CHAR)) != NULL) {
        chptr++;
        if((count  = sscanf(chptr, "%ld", &lvalue)) != 1)
            return -1;
        else {
            value = lvalue;
            return value;
        }
    }
    return -1;
}

/*
  This returns the position of the first character that differs
   between the query Term and the Isam Key, or -1 if they are identical. 
*/
static Int4 ISAMDiffChar(CharPtr Term, CharPtr Key, Boolean IgnoreCase)

{
    CharPtr Start = Term;
  
    if(IgnoreCase) {
        while(*Term && (TO_UPPER(*Term) == TO_UPPER(*Key))) {
            Term++;
            Key++;
        }
    } else {
        while(*Term && (*Term == *Key)) {
            Term++;
            Key++;
        }
    }
    
    if(*Term != NULLB)
        return((Int4)(Term - Start));
    
    for(;;) {
        if (ENDS_ISAM_KEY(Key))
            return(-1);
        
        if (*Key != ' ')
            break;
        
        Key++;
    }
    
    return((Int4)(Term - Start));
}

#define ID_DATA_CHUNK 16
ISAMErrorCode SISAMFindAllData(ISAMObjectPtr object, 
                               CharPtr term_in,
                               Int4Ptr PNTR ids_out,
                               Int4Ptr count_out)
{
    ISAMDataPtr   data;
    ISAMErrorCode error;
    Int4          index, Start, Stop;
    Int4          i, Diff = 0, SampleNum, Pos;
    Int4          TermNum, count, NumBytes, allocated;
    Int4Ptr       ids;
    Uint4Ptr      SamplePos;
    CharPtr       Page, Key, FileStart, Ptr, chptr;
    CharPtr       value, key_out;
    
    if((data = (ISAMDataPtr) object) == NULL)
        return ISAMBadParameter;
    
    *count_out = 0;
    *ids_out = NULL;

    if(data->initialized == FALSE) {
        if((error = ISAMInitSearch(object)) != ISAMNoError)
            return error;
    }
    
    if((error = SISAMSearch(object, term_in, 0, &key_out,
                            &value, (Uint4Ptr) &index)) != ISAMNoError) {
        return error;
    }

    MemFree(key_out);
    MemFree(value);
    
    if(data->mmp != NULL)
        FileStart = (CharPtr)data->mmp->mmp_begin;
    else
        FileStart = data->FileStart;
    
    if(data->PageSize != MEMORY_ONLY_PAGE_SIZE) 
        SamplePos = data->KeySamples + data->NumSamples + 1;
    else
        SamplePos = data->KeySamples;
    
    SampleNum = index / data->PageSize;
    TermNum   = index % data->PageSize;
    
    Start = SampleNum; Stop = SampleNum;
    if(TermNum == 0) { /* Exact match. Borders must be checked */
        for(i = 1; Diff == -1 && (SampleNum - i) >= 0; i++) {
            Key = FileStart + SwapUint4(SamplePos[SampleNum-i]);
            if((Diff = ISAMDiffChar(term_in, Key, TRUE)) == -1)
                Start = SampleNum - i;
        }
        for(i = 1; Diff == -1 && (SampleNum + i) < data->NumSamples; i++) {
            Key = FileStart + SwapUint4(SamplePos[SampleNum + i]);
            if((Diff = ISAMDiffChar(term_in, Key, TRUE)) == -1)
                Stop = SampleNum + i;
        }
        if(Start == Stop) { /* We have to load 2 pages */
            if(Start-- < 0) Start = 0;
            if(Stop++ > data->NumSamples -1)
                Stop = data->NumSamples -1;   
        }
    } else {
        Stop++;
    }

    /* Reading all in memory */
    
    Pos = SwapUint4(data->KeySamples[Start]);
    NumBytes = SwapUint4(data->KeySamples[Stop]) - Pos;
    Page = (CharPtr) MemNew(NumBytes + 1);
    NlmSeekInMFILE(data->mfp, Pos, SEEK_SET);
    NlmReadMFILE((Uint1Ptr)Page, sizeof(Char), NumBytes, data->mfp);
    Page[NumBytes] = NULLB;
    
    /* Now removing all \n and \r characters */
    
    for(chptr = Page; *chptr != NULLB; chptr++) {
        if(*chptr == '\n' || *chptr == '\r')
            *chptr = NULLB;
    }
    
    /* Search the page for the term. */
    
    allocated = ID_DATA_CHUNK;
    ids = MemNew(sizeof(Int4) * allocated);
    count = 0;
    
    Ptr = Page;
    while (Ptr - Page < NumBytes) {
        Diff = ISAMDiffChar(term_in, Ptr, TRUE);
        
        if (Diff == -1) {
            if(count >= allocated) {
                allocated += ID_DATA_CHUNK;
                ids = Realloc(ids, allocated);
            }
            ids[count] = ISAMGetDataNumber(Ptr);
            count++;
        }
        Ptr += StringLen(Ptr);
        
        while(Ptr - Page < NumBytes && *Ptr == NULLB)
            Ptr++;
    }

    *count_out = count;
    *ids_out   = ids;

    MemFree(Page);

    return ISAMNoError;
}

static void ISAMExtractData(CharPtr KeyData, 
                            CharPtr PNTR Key, CharPtr PNTR Data)
{
    CharPtr chptr, nkey;

    if (KeyData == NULL)
        return;

    nkey = StringSave(KeyData);

    if((chptr = StringChr(nkey, ISAM_DATA_CHAR)) != NULL) {
        *chptr = NULLB;
        if(Data != NULL)
            *Data = StringSave(chptr+1);
    } else if(Data != NULL) {
        *Data = StringSave("");
    }
    
    if(Key != NULL)
        *Key = StringSave(nkey);
    
    MemFree(nkey);

    return;
}

ISAMErrorCode SISAMSearch(ISAMObjectPtr object, 
                          CharPtr term_in,
                          Int4 flags,
                          CharPtr PNTR term_out,
                          CharPtr PNTR value,
                          Uint4Ptr index)
{
    ISAMDataPtr   data;
    ISAMErrorCode error;

    Int4 Diff, Start, Stop, SampleNum, Length, Pos;
    Int4 TermNum, NumBytes, FoundShort = -1;
    CharPtr Page, Key, FileStart, Ptr, ShortTerm, chptr;
    Uint4Ptr SamplePos;
    Boolean IgnoreCase, Short, Follow;

    if((data = (ISAMDataPtr) object) == NULL)
        return ISAMBadParameter;
    
    if(data->initialized == FALSE) {
        if((error = ISAMInitSearch(object)) != ISAMNoError)
            return error;
    }

    IgnoreCase = TRUE;  /* We will set this option to avoid more
                           complications */
    
    /* search the sample file first */
    
    Start = 0; 
    Stop = data->NumSamples -1;
    Length = StringLen(term_in);
    ShortTerm= MemNew(data->max_line_size);

    Follow     = (Boolean)(flags & ISAM_FOLLOW_KEY);
    Short      = (Boolean)((flags & ISAM_SHORT_KEY) || Follow);

    if(data->mmp != NULL)
        FileStart = (CharPtr)data->mmp->mmp_begin;
    else
        FileStart = data->FileStart;

    if(data->PageSize != MEMORY_ONLY_PAGE_SIZE) 
        SamplePos = data->KeySamples + data->NumSamples + 1;
    else
        SamplePos = data->KeySamples;

    while(Stop >= Start) {
        SampleNum = ((Uint4)(Stop + Start)) >> 1;
        
        Key = FileStart + SwapUint4(SamplePos[SampleNum]);
        Diff = ISAMDiffChar(term_in, Key, IgnoreCase);
        
        /* If this is an exact match, return the master term number. */
        if (Diff == -1) {
            ISAMExtractData(Key, term_out, value);
            *index = data->PageSize * SampleNum;
            MemFree(ShortTerm);
            return ISAMNoError;
        }
        
        /* If the key is a superset of the sample term, backup until just
           before the term. */
        if (Short && (Diff >= Length)) {
            if (SampleNum > 0)
              SampleNum--;
            
            if (IgnoreCase) {
	      while((SampleNum > 0) &&
		    (StrNICmp(term_in, 
                              FileStart+SwapUint4(SamplePos[SampleNum]), 
                              Length) == 0))
                  SampleNum--; 
            } else {
                while((SampleNum > 0) &&
                      (StrNCmp(term_in, 
                               FileStart + SwapUint4(SamplePos[SampleNum]), 
                               Length) == 0))
                    SampleNum--; 
            }
            
            FoundShort = SampleNum + 1;
            Ptr = FileStart + SwapUint4(SamplePos[SampleNum+1]);
            StringCpy(ShortTerm, Ptr);
            break;
        } else
            /* If preceding is desired, note the key.  */

            if (Follow) {
                FoundShort = SampleNum;
                StringCpy(ShortTerm, Key);
            }
        
        /* Otherwise, search for the next sample. */
        if (IgnoreCase ? TO_LOWER(term_in[Diff]) < TO_LOWER(Key[Diff]) :
            term_in[Diff] < Key[Diff])
            Stop = --SampleNum;
        else
            Start = SampleNum +1;
    }
    
    /* If the term is out of range altogether, report not finding it. */

    if ( (SampleNum < 0) || (SampleNum >= data->NumSamples)) {
        MemFree(ShortTerm);
        return ISAMNotFound;
    }

    /* load the appropriate page of terms into memory. */
    
    Pos = SwapUint4(data->KeySamples[SampleNum]);
    
    NumBytes = SwapUint4(data->KeySamples[SampleNum + 1]) - Pos;
    Page = (CharPtr) MemNew(NumBytes + 1);
    NlmSeekInMFILE(data->mfp, Pos, SEEK_SET);
    NlmReadMFILE((Uint1Ptr)Page, sizeof(Char), NumBytes, data->mfp);
    Page[NumBytes] = NULLB;

    /* Now removing all \n and \r characters */

    for(chptr = Page; *chptr != NULLB; chptr++) {
        if(*chptr == '\n' || *chptr == '\r')
            *chptr = NULLB;
    }
    
    /* Search the page for the term. */
    TermNum = 0;
    Ptr = Page;
    while (Ptr - Page < NumBytes) {
        Diff = ISAMDiffChar(term_in, Ptr, IgnoreCase);

        if (Diff == -1) /* Complete match */
            break;
        
        if (Short && (Diff >= Length)) /* Partialy complete */
            break;
        
        /* Just next available term accepted */
        
        if (Follow && (IgnoreCase ? 
                       TO_UPPER(term_in[Diff]) < TO_UPPER(Ptr [Diff]) : 
                       term_in[Diff] < Ptr [Diff]))
            break;
        
        Ptr += StringLen(Ptr);
        
        while(Ptr - Page < NumBytes && *Ptr == NULLB)
            Ptr++;

        TermNum++;
    }
    
    /* If we didn't find a match in the page, then we failed, unless the
       items that begins the next page is a match (only possible if 
       ISAM_SHORT_KEY or ISAM_FOLLOW_KEY was specified. */
    if (Ptr - Page == NumBytes) {

        MemFree(Page);
  
        if (FoundShort >= 0) {
            ISAMExtractData(ShortTerm, term_out, value);
            *index = data->PageSize * FoundShort;
            MemFree(ShortTerm);
            return ISAMNoError;
        } else {
            *index = (Uint4) -1;
            MemFree(ShortTerm);
            return ISAMNotFound;
        }
    }
    
    /* Otherwise, we found a match. */
    ISAMExtractData(Ptr, term_out, value);

    *index = (data->PageSize * SampleNum) + TermNum;

    MemFree(Page);
    MemFree(ShortTerm);

    return ISAMNoError;
}

/* ------------------------  NISAMFindKey ---------------------------
   Purpose:     Return Key value by absolute internal index
   
   Parameters:  Index - absolute internal index                
   Returns:     Key   - corresponding key value
   Data  - corresponding data value
   NOTE:        
   ------------------------------------------------------------------*/
ISAMErrorCode NISAMFindKey(ISAMObjectPtr object, 
                           Int4 Index,
                           Uint4Ptr Key,
                           Uint4Ptr Data
                           )
{
    return NISAMFindKeys(object, Index, Index, Key, Data);
}


/* ----------------------  NISAMFindKeys -------------------------
   Purpose:     Retuns set of Key/Data pairs from 
   First to Last internal index
                
   Parameters:  First - beginning of interval
                Last  - the end of interval
   Returns:     Keys - array of Keys
                Data - array of Data
   NOTE:        None
  ------------------------------------------------------------------*/
ISAMErrorCode NISAMFindKeys(ISAMObjectPtr object, 
                            Int4     First, 
                            Int4     Last, 
                            Uint4Ptr Keys, 
                            Uint4Ptr Data
                            )
{
    ISAMDataPtr data = (ISAMDataPtr)object;
    Int4 TotalNums, count;
    Int4Ptr KeyPage;
    NISAMKeyDataPtr KeyDataPage;
    Boolean NoData = (data->type == ISAMNumericNoData);
    ISAMErrorCode error;
    
    if(data == NULL)
        return ISAMBadParameter;
    
    if(data->initialized == FALSE) {
        if((error = ISAMInitSearch(object)) != ISAMNoError)
            return error;
    }
    
    if ((First < 0) || (Last >= data->NumTerms) || (First > Last))
        return ISAMBadParameter;
    
    TotalNums = Last-First + 1;
    
    if (NoData) {
        KeyPage = (Int4Ptr)MemNew((TotalNums + 1) * sizeof(Int4));
        NlmSeekInMFILE(data->mfp, First*sizeof(Int4), SEEK_SET);
        NlmReadMFILE((Uint1Ptr)KeyPage, sizeof(Int4), TotalNums, data->mfp);
    } else {
        KeyDataPage = (NISAMKeyDataPtr)MemNew((TotalNums + 1) * 
                                              sizeof(NISAMKeyData));
        NlmSeekInMFILE(data->mfp, First*sizeof(NISAMKeyData), SEEK_SET);
        NlmReadMFILE((Uint1Ptr)KeyDataPage, sizeof(NISAMKeyData), TotalNums, 
                data->mfp);
    }
    
    if (NoData) {  
        for (count = 0; count < TotalNums; count++) {
            if (Keys != NULL)
                Keys[count] = SwapUint4(KeyPage[count]);
            if (Data != NULL)
                Data[count] = First + count;
        }
    } else {
        for (count = 0; count < TotalNums; count++) {
            if (Keys != NULL)
                Keys[count] = SwapUint4(KeyDataPage[count].key);
            if (Data != NULL)
                Data[count] = SwapUint4(KeyDataPage[count].data);
        }
    }
    
    if (NoData)
        MemFree(KeyPage);
    else
        MemFree(KeyDataPage);
    
    return ISAMNoError;
}

/* ------------------------  ISAMNumTerms ---------------------------
   Purpose:     Returns total number of terms in ISAM database

   Parameters:  ISAM object
   Returns:     Number of terms
   NOTE:        None
  ------------------------------------------------------------------*/
ISAMErrorCode ISAMNumTerms(ISAMObjectPtr object, Int4Ptr terms)
{
    ISAMDataPtr data = (ISAMDataPtr) object;
    ISAMErrorCode error;
    
    if(data == NULL || terms == NULL)
        return ISAMBadParameter;
    
    if(data->initialized == FALSE) {
        if((error = ISAMInitSearch(object)) != ISAMNoError)
            return error;
    }
    
    *terms = data->NumTerms; 
    return ISAMNoError;
}

/****************************************************************************/
/* INTERNAL FINCTIONS  */
/****************************************************************************/

ISAMErrorCode ISAMCountLines(ISAMDataPtr data)
     /* this returns the number of lines in a file. */
{
    if(data == NULL)
        return ISAMBadParameter;
    
    data->NumTerms = 0;
    rewind(data->db_fd);
    while(ISAMReadLine(data) > 0)
        data->NumTerms++;
    
    rewind(data->db_fd);
    return ISAMNoError;
}

#ifdef NISAM_TEST_MODULE 
Int2 Main(void)
{
    ISAMObjectPtr object;
    Int4 i, terms, key_failed=0;
    Uint4Ptr Keys, Data;
    Uint4 Value;
    Uint4 Index;
    ISAMErrorCode error;
    CharPtr PNTR argv = GetArgv();
    Int4         argc = GetArgc();
    
    if(argc < 2) {

        printf("USAGE: %s <key file name> <index filename>\n", argv[0]);
        return 1;
    }
        
    if((object = ISAMObjectNew(ISAMNumeric, argv[1], argv[2])) == NULL) {
        printf("Failed to create ISAM object.\n");
        return 1;
    } 

    /*    if((error = ISAMMakeIndex(object, 0)) != ISAMNoError) {
        printf("Failed to create numerical index. "
               "Error code is %d\n", error);
        return 1;
    } */
    
    if((error = ISAMNumTerms(object, &terms)) != ISAMNoError) {
        printf("Failed to return number of terms. "
               "Error code is %d\n", error);
        return 1;
    }

    printf("Number of terms is %d\n", terms);  
    
    Keys = (Uint4Ptr) MemNew(terms*sizeof(Uint4));
    Data = (Uint4Ptr) MemNew(terms*sizeof(Uint4));
    
    if((error = NISAMFindKeys(object, 0, 
                              terms-1, Keys, Data)) != ISAMNoError) {
        printf("Failed to find keys. Error code is %d\n", error);
        return 1;
    }
    
    for(i=0; i < terms; i++) {
        if(i%1000 == 0)
            printf("Passed number %d\n", i);
        
        if((error = NISAMSearch(object, Keys[i], 
                                &Value, &Index)) != ISAMNoError) {
            printf("Failed to search. Error code is %d\n", error);
            return 1;
        }
        
        if(Value != Data[i] || Index != i) {
            printf("ISAM failed for key = %d\n "
                   "Index: %d expected %d\n"
                   "Value: %d expected %d\n", 
                   Keys[i], Index, i, Value, Data[i]
                   );  
            if(key_failed++ > 100)
                break;
        }
    }
    if(key_failed == 0)
        printf("Test succeeded\n");
    
    MemFree(Keys);
    MemFree(Data);
    ISAMObjectFree(object);
    return 0;
}
#endif
#ifdef SISAM_TEST_MODULE

Int2 Main(void)
{
    ISAMErrorCode error;
    CharPtr key, data, key_out, chptr;
    Uint4 index;    
    ISAMObjectPtr isamp;
    Char tmpbuff[1024];
    FILE *fd;
    register Int4 i;
    CharPtr PNTR argv = GetArgv();
    Int4         argc = GetArgc();
    
    if(argc < 2) {
        printf("USAGE: %s <key file name> [<index file name>]\n", argv[0]);
        return 1;
    }

    isamp = ISAMObjectNew(ISAMString, argv[1], argv[2]);

    if(argv[2] == NULL) {
        printf("Indexing file %s ...\n", argv[1]);
        
        if((error = ISAMMakeIndex(isamp, 1)) != ISAMNoError) {
            printf("Creating of index failed with error code %d\n", error);
            return 1;
        }
    }

    fd = FileOpen(argv[1], "r");
    
    for(i=0; fgets(tmpbuff, 1024, fd) != NULL; i++) {
        tmpbuff[StringLen(tmpbuff)-1] = NULLB; 
        if((chptr = StringChr(tmpbuff, ISAM_DATA_CHAR)) != NULL)
            *chptr = NULLB;

        if((error = SISAMSearch(isamp, tmpbuff, 0, &key_out,
                                &data, &index)) != ISAMNoError) {
            printf("Search failed with error code %d\n"
                   "String: %s\n", error, tmpbuff);
            return(1);
        } else {
            if(index != i)
                printf("Position mismatch:\n"
                       "String: %s\nData: %s\n"
                       "Position: %d (expected %d)\n", 
                       key_out, data, index, i);
            if(StringCmp(tmpbuff, key_out))
                printf("String mismatch:\n"
                       "String: %s\nExpected: %s\n"
                       "Data: %s\n"
                       "Position: %d (expected %d)\n", 
                       key_out, tmpbuff, data, index, i);
            if(i%100 == 0)
                printf("Passed index %d\n", i);

            MemFree(key_out);
            MemFree(data);
            if(i == 5000)
                break;
        }
    }
    ISAMObjectFree(isamp);
    printf("TEST SUCCESSFUL!!!\n");
    return 0;
}
#endif




