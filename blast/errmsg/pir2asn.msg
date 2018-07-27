MODULE pir2asn
$$ ENTRY, 1
$^   NumKeywordBlk, 1, SEV_ERROR
No or more than one ENTRY keyword found, making this entry suspect,
perhaps because of data corruption resulting in two records
begin combined in the middle.
$^   Parsed, 2, SEV_INFO
$^   Dropped, 3, SEV_ERROR

$$ TITLE, 2
$^   NumKeywordBlk, 1, SEV_ERROR
No or more than one TITLE keyword found, making this entry suspect,
perhaps because of data corruption resulting in two records
begin combined in the middle.

$$ DATE, 3
$^   NumKeywordBlk, 1, SEV_ERROR
No or more than one DATE keyword found, making this entry suspect,
perhaps because of data corruption resulting in two records
begin combined in the middle.
$^   IllegalDate, 2, SEV_WARNING
Date not in dd-mmm-yyy format

$$ SUMMARY, 4
$^   NumKeywordBlk, 1, SEV_ERROR
No or more than one SUMMARY keyword found, making this entry suspect,
perhaps because of data corruption resulting in two records
begin combined in the middle.

$$ SEQUENCE, 5
$^   NumKeywordBlk, 1, SEV_ERROR
No or more than one SEQUENCE keyword found, making this entry suspect,
perhaps because of data corruption resulting in two records
begin combined in the middle.
$^   SeqLenNotEq, 2, SEV_WARNING

$$ REFERENCE, 7
$^   NumKeywordBlk, 1, SEV_ERROR
No REFERENCE block found
$^   IllegalDate, 2, SEV_WARNING
$^   Illegalreference, 3, SEV_ERROR
$^   No_references, 4, SEV_ERROR
$^   Fail_to_parse, 5, SEV_ERROR
$^   Journal, 6, SEV_WARNING
$^   Book, 7, SEV_WARNING
$^   InPress, 8, SEV_WARNING
$^   MuidNotFound, 9, SEV_WARNING
$^   SuccessfulMuidLookup, 10, SEV_INFO
$^   IllegalAuthorName, 11, SEV_WARNING
$^   EtAlInAuthors, 12, SEV_WARNING

$$ FEATURE, 8
$^   UnknownFeatKey, 1, SEV_WARNING

$$ SERVER, 9
$^   NotUsed, 1, SEV_WARNING
Avoid Medline or Taxonomy lookup.
No protein translation sequence or organism name has been checked;
Could not guess the genetic code, standard code used.
$^   Failed, 2, SEV_ERROR
Call for Taxonomy or/and Medline service failed.
$^   TaxNameNotMatch, 3, SEV_WARNING
$^   TaxIdNotUnique, 4, SEV_WARNING
$^   Not_species_level, 5, SEV_WARNING

$$ FORMAT, 10
$^   Unknown, 1, SEV_WARNING

$$ ORGANISM, 11
$^   NewSynonym, 1, SEV_INFO
$^   NoFormalName, 2, SEV_WARNING
$^   Unclassified, 3, SEV_WARNING
$^   NoOrganism, 4, SEV_WARNING

