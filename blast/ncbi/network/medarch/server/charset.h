/*
	PROJECT: Medline database.
	MODULE:	 charset
	FILES:	 charset.c, charset.h (this one)

	This module contains the procedures needed to map from the Medline
	EBCDIC character set to the domestic (no accented characters) and
	internationalized ASCII character sets used in the Sybase database.

	Work started: 10 June 1991, Rand Huntzinger
	Original version completed: 24 July 1991, Rand S. Huntzinger
*
*
* RCS Modification History:
* $Log: charset.h,v $
* Revision 6.0  1997/08/25 18:36:01  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:54:26  epstein
* add RCS log revision history
*
*/

#ifndef	DEF_MODULE_CHARSET
#define	DEF_MODULE_CHARSET

/* The CHARSET_SIZE parameter gives the maximum number of characters in
   the input character set.  It is used to allocate conversion tables. */

#define	CHARSET_SIZE		256

/* The following declarations give the basic conversion types for the
   characters in the character set.  They indicate how to interpret the
   source character in teh conversion process. */

#define	CHAR_OMIT		(0)	/* Omit this character - pad, etc. */
#define	CHAR_SIMPLE		(1)	/* EBCDIC -> ASCII = ISO-8859 */
#define CHAR_INTERNATIONAL	(2)	/* ASCII char != ISO-8859 character */
#define	CHAR_DIACRITIC		(3)	/* Two byte source, mark + character */
#define	CHAR_SPECIAL		(4)	/* Generates a control character */
#define	CHAR_INVALID		(5)	/* The input character is not valid */

/* These codes are used to separate sections of the MeSH heading
   records.  They correspond to EBCDIC codes 0x51->0x53 respectively.
   They are coded in the conversion table as type CHAR_SPECIAL. */

#define	CHAR_SV			'\001'
#define	CHAR_EV			'\002'
#define	CHAR_SS			'\003'

/* These codes appear in the CvtChar cs_error field and determine how
   character set errors are to be handled. */

#define	CHAR_BAD_ERROR	'E'		/* Invalid character is an ERROR! */
#define	CHAR_BAD_WARN	'W'		/* Invalid character is a warning */
#define	CHAR_BAD_COUNT	'C'		/* Only count errors - don't flag */
#define	CHAR_BAD_SKIP	'S'		/* Simply skip bad characters */


/* The ConvTable structure describes the CvtChar cv_table rows.  Each
   character in the character set has one row of this type which gives
   the type of conversion to take place plus a datum, which may be used
   in the conversion.  How the datum is used depends upon the type field. */

typedef struct ConvTable {
	char	type;			/* Type of the conversion */
	char	datum;			/* Value is character */
} ConvTable;


/* The CharGroup structure is insed in the CvtChar in_table and diacritics
   fields.  It is used where the ASCII and ISO-8859 equivalents differ.  The
   CharGroup table has three tables, one which contains source tables, one
   with the ascii and one with the ISO-8859 equivalents.  The source character
   is matched in the source string and the offset into that table is the
   offset of the corresponding character in the ascii and iso tables.

   This structure is used for handling types CHAR_INTERNATIONAL and
   CHAR_DIACRITIC. */

typedef struct CharGroup {
	int		 n_set;		/* Number of characters in set */
	unsigned char	*source;	/* Source character (EBCDIC) */
	char		*ascii;		/* Ascii conversion equivalents */
	char		*iso;		/* ISO (international) equivalents */
} CharGroup;


/* The CvtChar structure describes how to convert the source character set
   (a form of EBCDIC) into both ASCII and ISO-8859.  It contains the
   conversion table (cv_table) with associated tables for handling the
   international tables (in_table and diacritics), the source character
   set name, and information on how to handle errors. */

typedef struct CvtChar {
	char		*name;		/* Character set name */
	int		 cs_size;	/* # rows in cv_table */
	char		 cs_error;	/* Character set error level */
	char		 mark_bad;	/* True => mark bad characters */
	char		 bad_ascii;	/* ASCII bad character mark */
	char		 bad_iso;	/* ISO bad character mark */
	ConvTable	*cv_table;	/* Base table */
	CharGroup	*in_table;	/* ISO/ASCII diffs */
	int		 n_diacritics;	/* # diacritic tables */
	CharGroup	*diacritics;	/* Diacritic tables */
} CvtChar;


/* The CvtCode structure is filled out by the ConvertChar() function to
   translate a single character in the source character set into both it's
   ASCII and ISO-8859 equivalents.  */

typedef struct CvtCode {
    short	in_bytes;		/* Number of source character bytes */
    short	type;			/* Conversion type (cvtable.type) */
    char	ascii;			/* The ASCII equivalent */
    char	iso;			/* The ISO equivalent */
    short	valid;			/* True if character is to be used */
} CvtCode;

/* The following declarations are externally available routines defined
   in the charset.c portion of this module */

void	InitializeCharacterSet( /* CvtChar *cs */ );
CvtChar	*GetCharacterSet();
char	*CharacterSetName();
int	 ConvertCharacter();
int	 ConvertString( /* char *in, int n, char *out, int size, int iso */ );
int	 ConvertPatchedString( /* char *in, char *out, int out_size,
		char *patch, int patch_size */ );

#endif /* DEF_MODULE_CHARSET */
