MODULE a2ferr

$$ DATE, 1

$^   IllegalDate, 1, SEV_WARNING
Date not in dd-mmm-yyy format
$^   Create_after_update, 2, SEV_WARNING
Standard create date after update date.


$$ SEGMENT, 2
$^   LocalSegEntry, 1, SEV_WARNING
The Segmented set (GenBank) that is declared has some 
local members.  That is, if there 6 members declared,
The set might only have:
 1 of 6, 
 2 of 6, 
 4 of 6, 
 5 of 6 and
 6 of 6, and thus be missing 3 of 6.
   Another possible problem is there could be a line:
      "3 of 5", instad of "3 of 6"
$^   MoreThan1000Segs, 2, SEV_INFO
asn2ff is not equipped, as of 8/94, to handle more than 1000 segments.



$$ ACCESSION, 3
$^   BadAccessNum, 1, SEV_ERROR
Accession must be upper case letter followed by 5 digits.

$^   NoAccessNum, 2, SEV_ERROR
No accession number could be found for this entry. The line number
given is only approximate.

$$ LOCUS, 4
$^   ChangedLocusName, 1, SEV_ERROR
???Change this later on.
For GenBank in a segmented set, it is an error if the segment number 
can not be found at the end of the LOCUS name.  So a LOCUS name
in SEGMENT 2 of 10 must end in 02, as ABCD02.

$^   BadLocusName, 2, SEV_ERROR
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

$^   LocusNameCollision, 3, SEV_WARNING
The Locus name in this entry conflicts with anothe name in the database.

$^   NoLocusName, 8, SEV_ERROR
No token after 'LOCUS' found in GenBank format.

$$ REFERENCE, 5
$^   NoPageNumbering, 1, SEV_WARNING
$^   IllegalPageRange, 2, SEV_WARNING
There are many classes of problems that can give this error message, some
of which are really warnings to have a human take a closer look:
  Total pages more than ....   means that the total number in the
                               article is greater than normal.  This
                               is usually caused by a typographical error.
  Page number may invert . . .	it looks like the first page is greater
                               that the last page.
  First-page contains  . . .   Although this is possible in the 
  non-digit character          bibliographic world, it is often
    or last-page               caused by an error.
$^   YearEquZero, 3, SEV_WARNING
$^   IllegalDate, 4, SEV_WARNING
$^   Patent, 5, SEV_WARNING
$^   Journal, 6, SEV_WARNING
$^   Thesis, 7, SEV_WARNING
$^   Book, 8, SEV_WARNING
$^   DirectSubmission, 9, SEV_WARNING
$^   Illegalreference, 10, SEV_ERROR
$^   NoAuthorName, 11, SEV_ERROR
$^   NoJournalName, 12, SEV_ERROR
$^   No_references, 13, SEV_ERROR
$^   NoDateOnRef, 14, SEV_ERROR
$^   DirSubInCitGen, 15, SEV_INFO
$^   MuidZeroOnly, 16, SEV_WARNING
$^   VolHasSuppl, 17, SEV_INFO
$^   ParanInSupp, 18, SEV_INFO
$^   NoValidRefs, 19, SEV_WARNING

$$ FEATURE, 6
$^   UnknownFeatureKey, 1, SEV_WARNING
$^   non_pseudo, 2, SEV_INFO
$^   Bad_location, 3, SEV_INFO
$^   GcodeAndTTableClash, 4, SEV_WARNING
Two conflicting translation values: one in Cdregion.code and one as a qualifier.
$^   NoQualOnMiscFeat, 5, SEV_WARNING
$^   CodeBreakLoc, 6, SEV_WARNING
$^   Duplicated, 7, SEV_WARNING
$^   IdenticalGeneName, 8, SEV_WARNING
$^   NULLGeneLocus, 9, SEV_WARNING
$^   Dropped, 10, SEV_ERROR

$$ ENTRY, 7
$^   Partial_peptide, 1, SEV_INFO
$^   No_valid_ref, 2, SEV_ERROR
$^   No_source_line, 3, SEV_WARNING
$^   OriginTooLong, 4, SEV_WARNING
$^   DroppedFromFlatfile, 5, SEV_ERROR

$$ GI, 8
$^   No_GI_Number, 1, SEV_WARNING
No GI number was found.

$$ PRINT, 9
$^   NullString, 1, SEV_WARNING
