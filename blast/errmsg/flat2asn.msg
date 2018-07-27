MODULE flat2asn
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
for a variety of fields.  The following are the fields and
the columns that are expected:
      bp               31-32  literally, "bp"
      Strand           34-36
          This must be blank or ss-, ds-, ms-};
      Molecule type    37-40
          This must be blank or DNA , RNA , pre-mRNA, mRNA,
             rRNA, tRNA, uRNA, or scRNA.
      Topology          43-52
             Circular, may be specified or it may be blank
             ("Tandem???")
      Division          53-55
          Legal division codes are: PRI, ROD, MAM, VRT, INV, PLN,
          BCT, RNA, VRL, PHG, SYN, UNA, or EST.
        (blank???)
         New or mispelled Embl divisions will be reported with a
         DIVISION_NewDivCode error code.  Note that EMBL uses FUN.
      Date               63-73, and be in dd-mmm-yyyy format.
$^   DirSubMode, 5, SEV_WARNING
Not standard EMBL format, used in DirSubMode only
$^   LineTypeOrder, 6, SEV_WARNING
$^   MissingSequenceData, 7, SEV_ERROR
$^   ContigWithSequenceData, 8, SEV_ERROR
$^   MissingContigFeature, 9, SEV_ERROR
$^   MissingSourceFeature, 10, SEV_ERROR
$^   MultipleCopyright, 11, SEV_WARNING
$^   MissingCopyright, 12, SEV_ERROR
$^   MultiplePatRefs, 13, SEV_ERROR
$$ DATACLASS, 2
$^   UnKnownClass, 1, SEV_WARNING
Swissprot error, only standard and preliminary are allowed.
$$ ENTRY, 3
$^   ParsingComplete, 2, SEV_INFO
$^   Begin, 3, SEV_ERROR
Looking for valid begining according to -f arguement in command line:
LOCUS for GenBank; ID for EMBL; ENTRY for PIR;
$^   Skipped, 6, SEV_ERROR
$^   Repeated, 7, SEV_WARNING
$^   LongSequence, 8, SEV_ERROR
$^   THC_Sequence, 9, SEV_WARNING
$^   Parsed, 10, SEV_INFO
$^   ParsingSetup, 11, SEV_INFO
$^   GBBlock_not_Empty, 12, SEV_WARNING
$^   LongHTGSSequence, 13, SEV_WARNING
$$ DATE, 5
$^   IllegalDate, 2, SEV_WARNING
Date not in dd-mmm-yyy format
$$ SEQUENCE, 7
$^   UnknownBaseHTG3, 1, SEV_WARNING
$^   SeqLenNotEq, 2, SEV_WARNING
The declared length of the sequence in the record was not
equal to the acutal number of residues found.
$^   BadResidue, 3, SEV_ERROR
Depending upon whether nucleic acid or protein, there are
different single letter legal codes.
$^   BadData, 4, SEV_WARNING
Can't parse the entry because of bad sequence data.
$$ SEGMENT, 8
$^   MissSegEntry, 1, SEV_ERROR
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
$^   OnlyOneMember, 6, SEV_WARNING
$^   Rejected, 7, SEV_WARNING
$$ ACCESSION, 9
$^   BadAccessNum, 2, SEV_ERROR
Accession must be upper case letter followed by 5 digits.
$^   NoAccessNum, 3, SEV_ERROR
No accession number could be found for this entry. The line number
given is only approximate.
$^   MoreAccessLine, 4, SEV_INFO
More than one accession block was found.  Continuation lines with
the wrong format can cause this in GenBank format.
$^   ForeignAccessNum, 5, SEV_WARNING
$$ LOCUS, 10
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
$^   NonViralRNAMoltype, 9, SEV_ERROR
$$ ORGANISM, 11
$^   NoOrganism, 1, SEV_WARNING
EMBL: no OS line found
GenBank: No ORGANISM line in the SOURCE block.
This message might repeat which trying to guess genetic code.
$^  HybridOrganism, 2, SEV_WARNING
In EMBL format only, can have multiple organisms (OS blocks).
If the taxonomy changes (OC block), this warning is produced.
$^   Unclassified, 3, SEV_WARNING
$^   MissParen, 4, SEV_WARNING
In EMBL format missing parenthesis after common name
$^   UnknownReplace, 5, SEV_INFO
$^   DoesntMatchFeats, 6, SEV_ERROR
$$ KEYWORD, 12
$^   MultipleHTGPhases, 1, SEV_ERROR
$^   ESTSubstring, 2, SEV_WARNING
$^   STSSubstring, 3, SEV_WARNING
$^   GSSSubstring, 4, SEV_WARNING
$$ DIVISION, 13
$^   NewDivCode, 1, SEV_WARNING
EMBL only (GenBank format related error is a FORMAT.LocusLinePosition
error), and the legal codes are:
    FUN, INV, MAM, ORG, PHG, PLN, PRI, PRO, ROD, SYN,
    UNA, VRL, VRT, and UNC  (UNA == UNC)
$^   MappedtoEST, 2, SEV_INFO
KW line maps one of these words:
     "EST", "EST PROTO((expressed sequence tag)", "expressed sequence tag",
     "partial cDNA sequence", "transcribed sequence fragment", "TSR",
     "putatively transcribed partial sequence", "UK putts" };
GB-block.div becomes "EST"
$^   MappedtoPAT, 3, SEV_WARNING
$^   MappedtoSTS, 4, SEV_WARNING
$^   Mismatch, 5, SEV_WARNING
$^   MissingESTKeywords, 6, SEV_WARNING
$^   MissingSTSKeywords, 7, SEV_WARNING
$^   MissingPatentRef, 8, SEV_WARNING
$^   PATHasESTKeywords, 9, SEV_WARNING
$^   PATHasSTSKeywords, 10, SEV_WARNING
$^   PATHasCDSFeature, 11, SEV_INFO
$^   STSHasCDSFeature, 12, SEV_WARNING
$^   NotMappedtoSTS, 13, SEV_WARNING
$^   ESTHasSTSKeywords, 14, SEV_INFO
$^   ESTHasCDSFeature, 15, SEV_WARNING
$^   NotMappedtoEST, 16, SEV_WARNING
$^   ShouldBeHTG, 17, SEV_ERROR
$^   MissingGSSKeywords, 18, SEV_INFO
$^   GSSHasCDSFeature, 19, SEV_WARNING
$^   NotMappedtoGSS, 20, SEV_WARNING
$^   MappedtoGSS, 21, SEV_WARNING
$^   PATHasGSSKeywords, 22, SEV_WARNING
$^   LongESTSequence, 23, SEV_WARNING
$^   LongSTSSequence, 24, SEV_WARNING
$^   LongGSSSequence, 25, SEV_WARNING
$^   GBBlockDivision, 26, SEV_WARNING
$^   MappedtoCON, 27, SEV_WARNING
$^   MissingHTGKeywords, 28, SEV_WARNING
$^   ShouldNotBeHTG, 29, SEV_ERROR
$^   ConDivInSegset, 30, SEV_ERROR
$^   ConDivLacksContig, 31, SEV_WARNING
$^   WrongHTGKeyword, 32, SEV_ERROR
$$ DEFINITION, 15
$^   HTGNotInProgress, 1, SEV_WARNING
$^   DifferingRnaTokens, 2, SEV_WARNING
$^   HTGShouldBeComplete, 3, SEV_ERROR
$$ REFERENCE, 16
$^   IllegPageRange, 3, SEV_WARNING
There are many classes of problems that can give this error message, some
of which are really warnings to have a human take a closer look.
$^   UnkRefRcToken, 4, SEV_WARNING
There are a limited number of valid Swiss-Prot Reference Comment
   (RC line) tokens.  This one is not one.
$^   UnkRefSubType, 5, SEV_WARNING
Illegal Swiss-prot Reference SubType.
$^   IllegalFormat, 6, SEV_WARNING
$^   IllegalAuthorName, 7, SEV_WARNING
$^   YearEquZero, 8, SEV_WARNING
$^   IllegalDate, 9, SEV_WARNING
$^   Patent, 10, SEV_WARNING
$^   Thesis, 12, SEV_WARNING
$^   Book, 14, SEV_WARNING
$^   DirectSubmission, 15, SEV_WARNING
$^   Illegalreference, 16, SEV_ERROR
$^   Fail_to_parse, 17
$^   No_references, 18, SEV_ERROR
$^   Xtratext, 19, SEV_WARNING
$^   InvalidInPress, 22, SEV_WARNING
$^   EtAlInAuthors, 24, SEV_WARNING
$^   NonDigitInPages, 25, SEV_WARNING
$^   LargePageRange, 26, SEV_WARNING
  Total pages more than ....   means that the total number in the
                               article is greater than normal.  This
                               is usually caused by a typographical error.
$^   InvertPageRange, 27, SEV_WARNING
  Page number may invert . . .  it looks like the first page is greater
                               that the last page.
$^   SingleTokenPageRange, 28, SEV_WARNING
$^   MissingBookPages, 29, SEV_WARNING
$^   MissingBookAuthors, 30, SEV_WARNING
$^   DateCheck, 31, SEV_WARNING
$^   GsdbRefDropped, 32, SEV_WARNING
$^   UnusualBookFormat, 33, SEV_WARNING
$$ FEATURE, 17
$^   MultFocusedFeats, 1, SEV_ERROR
$^   ExpectEmptyComment, 6, SEV_WARNING
specific Swiss-Prot message for INIT_MET feature
$^   DiscardData, 7, SEV_WARNING
includes unbalance double quote
$^   InValidEndPoint, 9, SEV_WARNING
$^   MissManQual, 10, SEV_WARNING
$^   NoFeatData, 11, SEV_WARNING
$^   NoFragment, 12, SEV_WARNING
$^   NotSeqEndPoint, 13, SEV_WARNING
$^   OldNonExp, 15, SEV_WARNING
$^   PartialNoNonTer, 16, SEV_WARNING
$^   Pos, 17, SEV_WARNING
$^   TooManyInitMet, 20, SEV_WARNING
specific Swiss-Prot error message
$^   UnEqualEndPoint, 22, SEV_WARNING
$^   UnknownFeatKey, 23, SEV_WARNING
$^   UnknownQualSpelling, 24, SEV_WARNING
$^   LocationParsing, 30, SEV_ERROR
$^   FeatureKeyReplaced, 32, SEV_WARNING
$^   Dropped, 33, SEV_ERROR
$^   UnknownDBName, 36, SEV_WARNING
$^   Duplicated, 37, SEV_WARNING
$^   NoSource, 38, SEV_WARNING
$^   MultipleSource, 39, SEV_WARNING
$$ LOCATION, 18
$^   FailedCheck, 1, SEV_WARNING
$^   MixedStrand, 2, SEV_WARNING
$$ GENENAME, 19
$^   IllegalGeneName, 1, SEV_WARNING
$^   DELineGeneName, 2, SEV_WARNING
$$ BIOSEQSETCLASS, 20
$^   NewClass, 1, SEV_INFO
$$ CDREGION, 21
$^   FrameNotSet, 1, SEV_WARNING
$^   InternalStopCodonFound, 2, SEV_WARNING
$^   NoProteinSeq, 6, SEV_WARNING
$^   TerminalStopCodonMissing, 7, SEV_WARNING
$^   TranslationDiff, 8, SEV_WARNING
$^   TranslationsAgree, 9, SEV_INFO
$^   IllegalStart, 10, SEV_WARNING
This error is caused by the start of translation being an unrecognized
initiation codon when the CDS is not recognized as partial.  Usually,
one adds '<' or '>' on the locations to accurately reflect the biology
and remove this error.  On rare occasions, a /partial should be added.
$^   GeneticCodeDiff, 11, SEV_WARNING
Genetic code returned by Taxonomy server is different from /transl_table
$^   UnevenLocation, 12, SEV_WARNING
$^   ShortProtein, 13, SEV_WARNING
$^   GeneticCodeAssumed, 14, SEV_WARNING
$^   NoTranslationCompare, 15, SEV_WARNING
$^   TranslationAdded, 16, SEV_INFO
$^   InvalidGcodeTable, 17, SEV_WARNING
$^   ConvertToImpFeat, 18, SEV_ERROR
$^   BadLocForTranslation, 19, SEV_ERROR
$^   LocationLength, 20, SEV_WARNING
$^   TranslationOverride, 21, SEV_WARNING
$^   FoundBadPIDType, 22, SEV_ERROR
$^   FoundBadPIDNumber, 23, SEV_RROR
$^   InvalidDb_xref, 24, SEV_ERROR
$^   TooManyDbxref, 25, SEV_ERROR
$^   Multiple_PID, 28, SEV_WARNING
$$ GENEREF, 22
$^   GeneIntervalOverlap, 1, SEV_WARNING
$^   NoUniqMaploc, 2, SEV_WARNING
$$ PROTREF, 23
$^   NoNameForProtein, 1, SEV_WARNING
$$ SEQID, 24
$^   NoSeqId, 1, SEV_ERROR
$$ SERVER, 26
$^   NotUsed, 1, SEV_WARNING
No protein translation sequence or organism name has been checked;
Could not guess the genetic code, standard code used.
$^   Failed, 2, SEV_ERROR
Call for Taxonomy or/and Medline service failed.
$^   GcFromSuppliedLineage, 6, SEV_WARNING
$^   TaxNameWasFound, 7, SEV_INFO
$$ NCBI_GI, 27
$^   BadDataFormat, 1, SEV_ERROR
$$ SPROT, 28
$^   DRLine, 1, SEV_WARNING
