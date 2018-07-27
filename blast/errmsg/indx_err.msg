MODULE indx_err
$$ FORMAT, 1
$^   NonAsciiChar, 1, SEV_ERROR
# This is a comment REJECT(LANL,EMBL,DDBJ,PIR,SP)
No column of any block of the flat file in any of the formats
is allowed to have a nonASCII character.  NonASCII is defined
as anything greater than  decimal value 126 (`~`) or less than
decimal 32 (space) except decimal 10, which is a newline.
$^   MissingEnd, 2, SEV_ERROR
The // line was not found after sequence was found before a line
with an letter was found.  Likely to be a truncated entry.
$^   MissingField, 3, SEV_ERROR
 Required field was not found in the flat file
$^   LocusLinePosition, 4, SEV_ERROR
The GenBank flat file format requires fixed column positions
for a variety of fields.
$^   DirSubMode, 5, SEV_WARNING
Not standard EMBL format, used in DirSubMode only
$^   LineTypeOrder, 6, SEV_ERROR
$^   Multiple_NI, 8, SEV_ERROR
$^   ContigInSegset, 9, SEV_ERROR
$$ ENTRY, 2
$^   ParsingSetup, 2, SEV_INFO
$^   Begin, 3, SEV_ERROR
Looking for valid begining according to -f arguement in command line:
LOCUS for GenBank; ID for EMBL; ENTRY for PIR;
$^   InvalidLineType, 6, SEV_ERROR
$^   Parsed, 7, SEV_INFO
$^   ParsingComplete, 8, SEV_INFO
$$ ACCESSION, 3
$^   BadAccessNum, 2, SEV_ERROR
Accession must be upper case letter followed by 5 digits.
$^   NoAccessNum, 3, SEV_ERROR
No accession number could be found for this entry. The line number
given is only approximate.
$^   MoreAccessLine, 4, SEV_INFO
More than one accession block was found.  Continuation lines with
the wrong format can cause this in GenBank format.
$^   ForeignAccessNum, 5, SEV_WARNING
$^   DirSubTakeOver, 6, SEV_WARNING
$$ LOCUS, 4
$^   WrongTopology, 2, SEV_WARNING
This message occurs when looking for either Circular or 'RNA' or 'DNA' in
embl mode.  Anything other than this will cause this warning.
$^   NoGIBBModMolType, 3, SEV_WARNING
 In the flat files, the only legal values are blank, RNA, pre-mRNA, mRNA,
 rRNA, tRNA, uRNA, ss-RNA, ds-RNA, ms-RNA, scRNA, DNA, ds-DNA, and ss-DNA
$^   MayBeNewSpeciesCode, 4, SEV_WARNING
Swiss-Prot specific error.  The species is not in the list of
LOCUS name prefix-species pairs.
$^   NoSpeciesCode, 5, SEV_WARNING
Swiss-Prot error when no species can be found.
$^   NoMolType, 6, SEV_WARNING
Can't find Molecule type
$^   BadLocusName, 7, SEV_ERROR
There are multiple possible ways to get this error message.
In all formats, this identifier must have either digits or
uppercase letters.
For Swiss-Prot, the rules are more complicated:
Locus name consists of up to 10 uppercase alphanumeric characters
      rule: X_Y format
         X is a mnemonic code, up to 4 alphanumeric characters to represent
             the protein name.
         Y is a mnemonic species identification code of at most 5
             alphanumeric characters to representing the biological source of
             the protein
$^   NoLocusName, 8, SEV_ERROR
No token after 'LOCUS' found in GenBank format.
$$ SEGMENT, 5
$^   MissSegEntry, 1, SEV_WARNING
The Segmented set (GenBank) that is declared has some
missing members.  That is, if there 6 members declared,
The set might only have:
 1 of 6,
 2 of 6,
 4 of 6,
 5 of 6 and
 6 of 6, and thus be missing 3 of 6.
   Another possible problem is there could be a line:
      "3 of 5", instad of "3 of 6"
$^   DiffMolType, 2, SEV_WARNING
A segmented set is supposed to be from the same molecule,
but with some unknown regions.  It should therefore all be
of the same type of molecule.  For this error to occur, there
have to be different molecule types for different segments
within the set.
$^   BadLocusName, 3, SEV_ERROR
For GenBank in a segmented set, it is an error if the segment number
can not be found at the end of the LOCUS name.  So a LOCUS name
in SEGMENT 2 of 10 must end in 02, as ABCD02.
$^   IncompSeg, 4, SEV_ERROR
There were not three blank-separated token on the SEGMENT line, in
GenBank Flat File format, the segment line has to look like:
SEGMENT     4 of 14
 for example.
$^   PubMatch, 5, SEV_WARNING
There were matching reference with different serial numbers in
segments.
$$ REFERENCE, 8
$^   IllegalDate, 1, SEV_WARNING
$$ FEATURE, 9
$^   NoFeatData, 1, SEV_WARNING
$$ SEQID, 10
$^   NoSeqId, 1, SEV_ERROR
$$ ORGANISM, 11
$^   Multiple, 1, SEV_ERROR
