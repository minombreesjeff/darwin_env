/***************************************************************************
*   utilpars.c:
*   -- all common routines for main programs in this directory
*
* $Log: utilpars.c,v $
* Revision 6.2  2001/12/06 17:00:41  kans
* TextSave takes size_t, not Int2, otherwise titin protein tries to allocate negative number
*
* Revision 6.1  2000/10/26 16:45:23  kans
* ValidAminoAcid also checks single letter abbreviation
*
* Revision 6.0  1997/08/25 18:08:16  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/06/19 18:39:44  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.0  1996/05/28 13:23:23  ostell
* Set to revision 5.0
*
 * Revision 4.1  1995/11/17  21:28:35  kans
 * asn2ff now uses gather (Tatiana)
 *
 * Revision 1.4  1995/06/08  14:31:50  tatiana
 * change TERM to '*' in ParFlat_AA_array
 *
 * Revision 1.3  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*                                                                  10-12-93
****************************************************************************/

#include "utilpars.h"

/*-----------------
 *
 *  The following associative array is to be used only
 *  until the method for accessing the seqcode tables
 *  is finalized.
 *      -Karl  2/10/94

The tables now are:
                { symbol "A", name "Alanine" },
                { symbol "B" , name "Asp or Asn" },
                { symbol "C", name "Cysteine" },
                { symbol "D", name "Aspartic Acid" },
                { symbol "E", name "Glutamic Acid" },
                { symbol "F", name "Phenylalanine" },
                { symbol "G", name "Glycine" },
                { symbol "H", name "Histidine" } ,
                { symbol "I", name "Isoleucine" },
                { symbol "", name "" },
                { symbol "K", name "Lysine" },
                { symbol "L", name "Leucine" },
                { symbol "M", name "Methionine" },
                { symbol "N", name "Asparagine" } ,
                { symbol "", name "" },
                { symbol "P", name "Proline" },
                { symbol "Q", name "Glutamine"},
                { symbol "R", name "Arginine"},
                { symbol "S", name "Serine"},
                { symbol "T", name "Threoine"},
                { symbol "", name ""},
                { symbol "V", name "Valine"},
                { symbol "W", name "Tryptophan" },
                { symbol "X", name "Undetermined or atypical"},
                { symbol "Y", name "Tyrosine"},
                { symbol "Z", name "Glu or Gln" }
 
            code iupacaa3 ,
            num 25 ,                     -- continuous 0-23
            one-letter FALSE ,            -- all 3 letter codes
            table {
                { symbol "Ter", name "Termination" } ,
                { symbol "Ala", name "Alanine" },
                { symbol "Asx" , name "Asp or Asn" },
                { symbol "Cys", name "Cysteine" },
                { symbol "Asp", name "Aspartic Acid" },
                { symbol "Glu", name "Glutamic Acid" },
                { symbol "Phe", name "Phenylalanine" },
                { symbol "Gly", name "Glycine" },
                { symbol "His", name "Histidine" } ,
                { symbol "Ile", name "Isoleucine" },
                { symbol "Lys", name "Lysine" },
                { symbol "Leu", name "Leucine" },
                { symbol "Met", name "Methionine" },
                { symbol "Asn", name "Asparagine" } ,
                { symbol "Pro", name "Proline" },
                { symbol "Gln", name "Glutamine"},
                { symbol "Arg", name "Arginine"},
                { symbol "Ser", name "Serine"},
                { symbol "Thr", name "Threoine"},
                { symbol "Sec", name "Selenocysteine"},
                { symbol "Val", name "Valine"},
                { symbol "Trp", name "Tryptophan" },
                { symbol " X ", name "Undetermined or atypical"},
                { symbol "Tyr", name "Tyrosine"},
                { symbol "Glx", name "Glu or Gln" }
 *---------------*/
typedef struct struct_AA1_toAA3 {
	CharPtr aa3;
 Uint1 aa1;
} ParFlat_AA1_to_AA3;
ParFlat_AA1_to_AA3
 ParFlat_AA_array[ParFlat_TOTAL_AA] = {
       "Ala", 'A',
        "Asx", 'B',
        "Cys", 'C',
        "Asp", 'D',
        "Glu", 'E',
        "Phe", 'F',
        "Gly", 'G',
        "His", 'H',
        "Ile", 'I',
        "Lys", 'K',  /* notice no 'J', breaks naive meaning of index -Karl */
        "Leu", 'L',
        "Met", 'M',
        "Asn", 'N',
        "Pro", 'P',  /* no 'O' */
        "Gln", 'Q',
        "Arg", 'R',
        "Ser", 'S',
        "Thr", 'T',
        "Val", 'V',
        "Trp", 'W', 
        "Xxx", 'X', /* no U */
        "Tyr", 'Y',
        "Glx", 'Z',
        "Sec", 'U', /* not in iupacaa */
        "TERM", '*', /* not in iupacaa */ /*changed by Tatiana 06.07.95?`*/
        "OTHER", 'X'};

/*--------------------- TextSave() -------------------------*/
/*****************************************************************************
*   TextSave(text, len)
*   -- allocates a null terminated string and copies len characters into
*      it starting at text.
*   -- returns a pointer to the allocated string. If len is 0 returns NULL
*
*                                                              3-26-93
*****************************************************************************/
NLM_EXTERN CharPtr TextSave(CharPtr text, size_t len)
{
   CharPtr str = NULL;

   if ((text == NULL) || (len == 0))
      return str;

   str = MemNew((size_t)(len + 1));
   MemCopy(str, text, (size_t)len);

   return (str);

} /* TextSave */

/*------------------------- DelTailBlank() ---------------------------*/
/***************************************************************************
*  DelTailBlank:
*  -- return a string without tail blanks
*                                                                  10-12-93
***************************************************************************/
NLM_EXTERN void DelTailBlank(CharPtr str)
{
   Int4 size;

   size = StringLen(str);
   while (size > 0 && str[size-1] == ' ') {
       str[size-1] = '\0';
       size--;
   }

} /* DelTailBlank */

/*-------------------- MatchArrayStringIcase() ----------------------------*/
/***************************************************************************
*  MatchArrayStringIcase:
*  -- return array position of the "str" (ignored case) in the array_string
*  -- return -1 if no match 
*                                                                 10-12-93
***************************************************************************/
NLM_EXTERN Int2 MatchArrayStringIcase(CharPtr array_string[], Int2 totalstr, CharPtr text)
{
   Int2 i;

   for (i = 0; i < totalstr && text != NULL; i++)
       if (StringICmp(text, array_string[i]) == 0)
          return (i);

   return (-1);

} /* MatchArrayStringIcase */

/*-------------------------- ValidAminoAcid() ------------------------*/
/***************************************************************************
*   ValidAminoAcid:
*   -- return a sequential code for the amino acid if "aa" is a valid
*      amino acid; otherwise return 255, unknown
*   -- using NCBIstdaa, IUPAC3aa
*                                                                 7-8-93
* for now, use iupacaa and return 'X' if no match found
*   -Karl 2/10/94
***************************************************************************/
NLM_EXTERN Uint1 ValidAminoAcid(CharPtr aa)
{
   Int2  i;

   if (aa == NULL) (Uint1) 'X';

   for (i = 0; i < ParFlat_TOTAL_AA && aa != NULL; i++)
       if (StringICmp(aa, ParFlat_AA_array[i].aa3) == 0) {
          return (ParFlat_AA_array[i].aa1);
       }

   if (aa [1] != '\0') return (Uint1) 'X';

   for (i = 0; i < ParFlat_TOTAL_AA && aa != NULL; i++)
       if (aa [0] == ParFlat_AA_array[i].aa1) {
          return (ParFlat_AA_array[i].aa1);
       }

   return (Uint1) 'X';

} /* ValidAminoAcid */

