MODULE valid
$$ SEQ_INST, 1
$^   ExtNotAllowed, 1
# This is a comment
A Bioseq "extension" is used for special classes of Bioseq. This class of
Bioseq should not have one but it does. This is probably a software error.

$^   ExtBadOrMissing, 2
This class of Bioseq requires an "extension" but it is missing or of the
wrong type. This is probably a software error.

$^   SeqDataNotFound, 3
No actual sequence data was found on this Bioseq. This is probably a
software problem.

$^   SeqDataNotAllowed, 4
The wrong type of sequence data was found on this Bioseq. This is probably a
software problem.

$^   ReprInvalid, 5
This Bioseq has an invalid representation class. This is probably a software
error.

$^   CircularProtein, 6
This protein Bioseq is represented as circular. Circular topology is
normally used only for certain DNA molecules, for example, plasmids.

$^   DSProtein, 7
This protein Bioseq has strandedness indicated. Strandedness is normally a
property only of DNA sequences. Please unset the strandedness.

$^   MolNotSet, 8
It is not clear whether this sequence is nucleic acid or protein. Please set
the appropriate molecule type (Bioseq.mol).

$^   MolOther, 9
Most sequences are either nucleic acid or protein. However, the molecule
type (Bioseq.mol) is set to "other". It should probably be set to nucleic
acid or a protein.

$^   FuzzyLen, 10
This sequence is marked as having an uncertain length, but the length is
known exactly.

$^   InvalidLen, 11
The length indicated for this sequence is invalid. This is probably a
software error.

$^   InvalidAlphabet, 12
This Bioseq has an invalid alphabet (e.g. protein codes on a nucleic acid or
vice versa). This is probably a software error.

$^   SeqDataLenWrong, 13
The length of this Bioseq does not agree with the length of the actual data.
This is probably a software error.

$^   SeqPortFail, 14
Something is very wrong with this entry. The validator cannot open a SeqPort
on the Bioseq. Further testing cannot be done.

$^   InvalidResidue, 15
Invalid residue codes were found in this Bioseq.

$^   StopInProtein, 16
Stop codon symbols were found in this protein Bioseq.

$^   PartialInconsistent, 17
This segmented sequence is described as complete or incomplete in several
places, but these settings are inconsistent.

$^   ShortSeq, 18
This Bioseq is unusually short (less than 4 amino acids or less than 11
nucleic acids). GenBank does not usually accept such short sequences.

$^   NoIdOnBioseq, 19
No SeqIds were found on this Bioseq. This is probably a software error.

$^   BadDeltaSeq, 20
Delta sequences should only be HTGS-1 or HTGS-2.

$^   LongHtgsSequence, 21
HTGS-1 or HTGS-2 sequences must be < 350 KB in length.

$^   LongLiteralSequence, 22
Delta literals must be < 350 KB in length.

$^   SequenceExceeds350kbp, 23
Individual sequences must be < 350 KB in length, unless they represent a single gene.

$^   ConflictingIdsOnBioseq, 24
Two SeqIds of the same class was found on this Bioseq. This is probably a software error.

$^   MolNuclAcid, 25
The specific type of this nucleic acid (DNA or RNA) is not set.

$^   ConflictingBiomolTech, 26
HTGS/STS/GSS records should be genomic DNA. There is a conflict between the
technique and expected molecule type.

$^   SeqIdNameHasSpace, 27
The Seq-id.name field should be a single word without any whitespace. This should be
fixed by the database staff.

$^   IdOnMultipleBioseqs, 28
There are multiple occurrences of the same Seq-id in this record. Sequence
identifiers must be unique within a record.

$^   DuplicateSegmentReferences, 29
The segmented sequence refers multiple times to the same Seq-id. This may be due
to a software error. Please consult with the database staff to fix this record.

$^   TrailingX, 30
The protein sequence ends with one or more X (unknown) amino acids.

$^   BadSeqIdFormat, 31
A nucleotide sequence identifier should be 1 letter plus 5 digits or 2 letters
plus 6 digits, and a protein sequence identifer should be 3 letters plus 5 digits.

$^   PartsOutOfOrder, 32
The parts inside a segmented set should correspond to the seq_ext of the segmented
bioseq.  A difference will affect how the flatfile is displayed.

$^   BadSecondaryAccn, 33
A secondary accession usually indicates a record replaced or subsumed by the current
record.  In this case, the current accession and secondary are the same.

$^   ZeroGiNumber, 34
GI numbers are assigned to sequences by NCBI's sequence tracking database.  0 is not
a legal value for a gi number.

$^   RnaDnaConflict, 35
The MolInfo biomol field is inconsistent with the Bioseq molecule type field.

$^   HistoryGiCollision, 36
The Bioseq history gi refers to this Bioseq, not to its predecessor or successor.

$^   GiWithoutAccession, 37
The Bioseq has a gi identifier but no GenBank/EMBL/DDBJ accession identifier.

$^   MultipleAccessions, 38
The Bioseq has a gi identifier and more than one GenBank/EMBL/DDBJ accession identifier.

$^   HistAssemblyMissing, 39
The Bioseq has a TPA identifier but does not have a Seq-hist.assembly alignment.  This
should be annotated or calculated by the database, resulting in a PRIMARY block visible
in the flatfile.

$^   TerminalNs, 40
The Bioseq has one or more N bases at the end.

$^   UnexpectedIdentifierChange, 41
The set of sequence identifiers on a Bioseq are not consistent with the previous version
of the record in the database.

$^   InternalNsInSeqLit, 42
There are runs of many Ns inside the SeqLit component of a delta Bioseq.

$^   SeqLitGapLength0, 43
A SeqLit component of a delta Bioseq can specify a gap, but it should not be a gap
of 0 length.

$^   TpaAssmeblyProblem, 44
Third party annotation records should have a TpaAssembly user object and a
Seq-hist.assembly alignment for the PRIMARY block.

$^   SeqLocLength, 45
A SeqLoc component of a delta Bioseq is suspiciously small.

$$ SEQ_DESCR, 2

$^   BioSourceMissing, 1
The biological source of this sequence has not been described correctly.  A
Bioseq must have a BioSource descriptor that covers the entire molecule.
Additional BioSource features may also be added to recombinant molecules,
natural or otherwise, to designate the parts of the molecule. Please add the
source information.

$^   InvalidForType, 2
This descriptor cannot be used with this Bioseq. A descriptor placed at the
BioseqSet level applies to all of the Bioseqs in the set. Please make sure
the descriptor is consistent with every sequence to which it applies.

$^   FileOpenCollision, 3
FileOpen is unable to find a local file.  This is normal, and can be ignored.

$^   Unknown, 4
An unknown or "other" modifier was used.

$^   NoPubFound, 5
No publications were found in this entry which refer to this Bioseq. If a
publication descriptor is added to a BioseqSet, it will apply to all of the
Bioseqs in the set. A publication feature should be used if the publication
applies only to a subregion of a sequence.

$^   NoOrgFound, 6
This entry does not specify the organism that was the source of the sequence.
Please name the organism.

$^   MultipleBioSources, 7
There are multiple BioSource or OrgRef descriptors in the same chain with
the same taxonomic name. Their information should be combined into a single
BioSource descriptor.

$^   NoMolInfoFound, 8
This sequence does not have a Mol-info descriptor applying to it.  This indicates
genomic vs. message, sequencing technique, and whether the sequence is incomplete.

$^   BadCountryCode, 9
The country code (up to the first colon) is not on the approved list of countries.

$^   NoTaxonID, 10
The BioSource is missing a taxonID database identifier.  This will be inserted by
the automated taxonomy lookup called by Clean Up Record.

$^   InconsistentBioSources, 11
This population study has BioSource descriptors with different taxonomic names.
All members of a population study should be from the same organism.

$^   MissingLineage, 12
A BioSource should have a taxonomic lineage, which can be obtained from the
taxonomy network server.

$^   SerialInComment, 13
Comments that refer to the conclusions of a specific reference should not be
cited by a serial number inside brackets (e.g., [3]), but should instead be
attached as a REMARK on the reference itself.

$^   BioSourceNeedsFocus, 14
Focus must be set on a BioSource descriptor in records where there is a
BioSource feature with a different organism name.

$^   BadOrganelle, 15
Note that only Kinetoplastida have kinetoplasts, and that only Chlorarchniophyta
and Cryptophyta have nucleomorphs.

$^   MultipleChromosomes, 16
There are multiple chromosome qualifiers on this Bioseq.  With the exception of
some pseudoautosomal genes, this is likely to be a biological annotation error.

$^   BadSubSource, 17
Unassigned SubSource subtype.

$^   BadOrgMod, 18
Unassigned OrgMod subtype.

$^   InconsistentProteinTitle, 19
An instantiated protein title descriptor should normally be the same as the
automatically generated title.  This may be a curated exception, or it may
be out of synch with the current annotation.

$^   Inconsistent, 20
There are two descriptors of the same type which are inconsistent with each
other. Please make them consistent.

$^   ObsoleteSourceLocation, 21
There is a source location that is no longer legal for use in GenBank records.

$^   ObsoleteSourceQual, 22
There is a source qualifier that is no longer legal for use in GenBank records.

$^   StructuredSourceNote, 23
The name of a structured source field is present as text in a note.  The data
should probably be put into the appropriate field instead.

$^   UnnecessaryBioSourceFocus, 24
Focus should not be set on a BioSource descriptor in records where there is no
BioSource feature.

$^   RefGeneTrackingWithoutStatus, 25
The RefGeneTracking user object does not have the required Status field set.

$^   UnwantedCompleteFlag, 26
The Mol-info.completeness flag should not be set on a genomic sequence unless
the title also says it is a complete sequence or complete genome.

$$ GENERIC, 3

$^   NonAsciiAsn, 1
There is a non-ASCII type character in this entry.

$^   Spell, 2
There is a potentially misspelled word in this entry.

$^   AuthorListHasEtAl, 3
The author list contains et al, which should be replaced with the
remaining author names.

$^   MissingPubInfo, 4
The publication is missing essential information, such as title or
authors.

$^   UnnecessaryPubEquiv, 5
A nested Pub-equiv is not normally expected in a publication.  This may prevent
proper display of all publication information.

$^   BadPageNumbering, 6
The publication page numbering is suspect.

$$ SEQ_PKG, 4

$^   NoCdRegionPtr, 1
A protein is found in this entry, but the coding region has not been
described. Please add a CdRegion feature to the nucleotide Bioseq.

$^   NucProtProblem, 2
Both DNA and protein sequences were expected, but one of the two seems to be
missing. Perhaps this is the wrong package to use.

$^   SegSetProblem, 3
A segmented sequence was expected, but it was not found. Perhaps this is the
wrong package to use.

$^   EmptySet, 4
No Bioseqs were found in this BioseqSet. Is that what was intended?

$^   NucProtNotSegSet, 5
A nuc-prot set should not contain any other BioseqSet except segset.

$^   SegSetNotParts, 6
A segset should not contain any other BioseqSet except parts.

$^   SegSetMixedBioseqs, 7
A segset should not contain both nucleotide and protein Bioseqs.

$^   PartsSetMixedBioseqs, 8
A parts set should not contain both nucleotide and protein Bioseqs.

$^   PartsSetHasSets, 9
A parts set should not contain BioseqSets.

$^   FeaturePackagingProblem, 10
A feature should be packaged on its bioseq, or on a set containing the Bioseq.

$^   GenomicProductPackagingProblem, 11
The product of an mRNA feature in a genomic product set should point to a cDNA
Bioseq packaged in the set, perhaps within a nuc-prot set.  RefSeq records may
however be referenced remotely.

$^   InconsistentMolInfoBiomols, 12
Mol-info.biomol is inconsistent within a segset or parts set.

$^   ArchaicFeatureLocation, 13
A feature location should refer to the accession or gi number, not a local or general ID.

$^   ArchaicFeatureProduct, 14
A feature product should refer to the accession or gi number, not a local or general ID.

$^   GraphPackagingProblem, 15
A graph should be packaged on its bioseq, or on a set containing the Bioseq.

$$ SEQ_FEAT, 5

$^   InvalidForType, 1
This feature type is illegal on this type of Bioseq.

$^   PartialProblem, 2
There are several places in an entry where a sequence can be described as
either partial or complete. In this entry, these settings are inconsistent.
Make sure that the location and product Seq-locs, the Bioseqs, and the
SeqFeat partial flag all agree in describing this SeqFeat as partial or
complete.

$^   InvalidType, 3
A feature with an invalid type has been detected. This is most likely a
software problem.

$^   Range, 4
The coordinates describing the location of a feature do not fall within the
sequence itself. A feature location or a product Seq-loc is out of range of
the Bioseq it points to.

$^   MixedStrand, 5
Mixed strands (plus and minus) have been found in the same location. While
this is biologically possible, it is very unusual. Please check that this is
really what you mean.

$^   SeqLocOrder, 6
This location has intervals that are out of order. While whis is biologically
possible, it is very unusual. Please check that this is really what you mean.

$^   CdTransFail, 7
A fundamental error occurred in software while attempting to translate this
coding region. It is either a software problem or sever data corruption.

$^   StartCodon, 8
An illegal start codon was used. Some possible explanations are: (1) the
wrong genetic code may have been selected; (2) the wrong reading frame may
be in use; or (3) the coding region may be incomplete at the 5' end, in
which case a partial location should be indicated.

$^   InternalStop, 9
Internal stop codons are found in the protein sequence. Some possible
explanations are: (1) the wrong genetic code may have been selected; (2) the
wrong reading frame may be in use; (3) the coding region may be incomplete
at the 5' end, in which case a partial location should be indicated; or (4)
the CdRegion feature location is incorrect.

$^   NoProtein, 10
Normally a protein sequence is supplied. This sequence can then be compared
with the translation of the coding region. In this entry, no protein Bioseq
was found, and the comparison could not be made.

$^   MisMatchAA, 11
The protein sequence that was supplied is not identical to the translation
of the coding region. Mismatching amino acids are found between these two
sequences.

$^   TransLen, 12
The protein sequence that was supplied is not the same length as the
translation of the coding region. Please determine why they are different.

$^   NoStop, 13
A coding region that is complete should have a stop codon at the 3'end.  A
stop codon was not found on this sequence, although one was expected.

$^   TranslExcept, 14
An unparsed \transl_except qualifier was found. This indicates a parser
problem.

$^   NoProtRefFound, 15
The name and description of the protein is missing from this entry. Every
protein Bioseq must have one full-length Prot-ref feature to provide this
information.

$^   NotSpliceConsensus, 16
Splice junctions typically have GT as the first two bases of the intron
(splice donor) and AG as the last two bases of the intron (splice acceptor).
This intron does not conform to that pattern.

$^ OrfCdsHasProduct, 17
A coding region flagged as orf has a protein product.  There should be no
protein product bioseq on an orf.

$^ GeneRefHasNoData, 18
A gene feature exists with no locus name or other fields filled in.

$^ ExceptInconsistent, 19
A coding region has an exception gbqual but the excpt flag is not set.

$^ ProtRefHasNoData, 20
A protein feature exists with no name or other fields filled in.

$^ GenCodeMismatch, 21
The genetic code stored in the BioSource is different than that for this CDS.

$^ RNAtype0, 22
RNA type 0 (unknown RNA) should be type 255 (other).

$^ UnknownImpFeatKey, 23
An import feature has an unrecognized key.

$^ UnknownImpFeatQual, 24
An import feature has an unrecognized qualifier.

$^ WrongQualOnImpFeat, 25
This qualifier is not legal for this feature.

$^ MissingQualOnImpFeat, 26
An essential qualifier for this feature is missing.

$^ PsuedoCdsHasProduct, 27
A coding region flagged as pseudo has a protein product.  There should be no
protein product bioseq on a pseudo CDS.

$^ IllegalDbXref, 28
The database in a cross-reference is not on the list of officially
recognized database abbreviations.

$^ FarLocation, 29
The location has a reference to a bioseq that is not packaged in this record.

$^ DuplicateFeat, 30
The intervals on this feature are identical to another feature of the same type,
but the label or comment are different.

$^ UnnecessaryGeneXref, 31
This feature has a gene xref that is identical to the overlapping gene.  This is
redundant, and probably should be removed.

$^   TranslExceptPhase, 32
A \transl_except qualifier was not on a codon boundary.

$^   TrnaCodonWrong, 33
The tRNA codon recognized does not code for the indicated amino acid using the
specified genetic code.

$^   BothStrands, 34
Feature location indicates that it is on both strands. This is not biologically
possible for this kind of feature. Please indicate the correct strand (plus or
minus) for this feature.

$^   CDSgeneRange, 35
A CDS is overlapped by a gene feature, but is not completely contained by it.
This may be an annotation error.

$^   CDSmRNArange, 36
A CDS is overlapped by an mRNA feature, but the mRNA does not cover all
intervals (i.e., exons) on the CDS. This may be an annotation error.

$^ OverlappingPeptideFeat, 37
The intervals on this processed protein feature overlap another protein feature.
This may be caused by errors in originally annotating these features on DNA coordinates,
where start or stop positions do not occur in between codon boundaries.  These then
appear as errors when the features are converted to protein coordinates by mapping
through the CDS.

$^ SerialInComment, 38
Comments that refer to the conclusions of a specific reference should not be
cited by a serial number inside brackets (e.g., [3]), but should instead be
attached as a REMARK on the reference itself.

$^ MultipleCDSproducts, 39
More than one CDS feature points to the same protein product.  This can happen with
viral long terminal repeats (LTRs), but GenBank policy is to have each equivalent
CDS point to a separately accessioned protein Bioseq.

$^ FocusOnBioSourceFeature, 40
The /focus flag is only appropriate on BioSource descriptors, not BioSource features.

$^ PeptideFeatOutOfFrame, 41
The start or stop positions of this processed peptide feature do not occur in between
codon boundaries.  This may incorrectly overlap other peptides when the features are
converted to protein coordinates by mapping through the CDS.

$^ InvalidQualifierValue, 42
The value of this qualifier is constrained to a particular vocabulary of style.  This
value does not conform to those constraints.  Please see the feature table documentation
for more information.

$^ MultipleMRNAproducts, 43
More than one mRNA feature points to the same cDNA product.  This is an error in the
genomic product set.  Each mRNA feature should have a unique product Bioseq.

$^   mRNAgeneRange, 44
An mRNA is overlapped by a gene feature, but is not completely contained by it.
This may be an annotation error.

$^   TranscriptLen, 45
The mRNA sequence that was supplied is not the same length as the
transcription of the mRNA feature. Please determine why they are different.

$^   TranscriptMismatches, 46
The mRNA sequence and the transcription of the mRNA feature are different.
If the number is large, it may indicate incorrect intron/exon boundaries.

$^   CDSproductPackagingProblem, 47
The nucleotide location and protein product of the CDS are not packaged together
in the same nuc-prot set.  This may be an error in the software used to create
the record.

$^   DuplicateInterval, 48
The location has identical adjacent intervals, e.g., a duplicate exon reference.

$^   PolyAsiteNotPoint, 49
A polyA_site should be at a single nucleotide position.

$^   ImpFeatBadLoc, 50
An import feature loc field does not equal the feature location.  This should be
corrected, and then the loc field should be cleared.

$^   LocOnSegmentedBioseq, 51
Feature locations traditionally go on the individual parts of a segmented bioseq, not
on the segmented sequence itself.  These features are invisible in asn2ff reports, and
are now being flagged for correction.

$^   UnnecessaryCitPubEquiv, 52
A set of citations on a feature should not normally have a nested Pub-equiv construct.
This may prevent proper matching to the correct publication.

$^   ImpCDShasTranslation, 53
A CDS that has known translation errors cannot have a /translation qualifier.

$^   ImpCDSnotPseudo, 54
A CDS that has known translation errors must be marked as pseudo to suppress the
translation.

$^   MissingMRNAproduct, 55
The mRNA feature points to a cDNA product that is not packaged in the record.
This is an error in the genomic product set.

$^   AbuttingIntervals, 56
The start of one interval is next to the stop of another.  A single interval
may be desirable in this case.

$^   CollidingGeneNames, 57
Two gene features should not have the same name.

$^   MultiIntervalGene, 58
A gene feature on a single Bioseq should have a single interval spanning everything
considered to be under that gene.

$^ FeatContentDup, 59
The intervals on this feature are identical to another feature of the same type,
and the label and comment are also identical. This is likely to be an error in
annotating the record.  Note that GenBank format suppresses duplicate features,
so use of Graphic view is recommended.

$^   BadProductSeqId, 60
The feature product refers to a database ID that has a locus name but no accession.
This is probably an error in parsing of a submission.

$^   RnaProductMismatch, 61
The RNA feature product type does not correspond to the RNA feature type.  These
need to be consistent.

$^   MissingCDSproduct, 62
The CDS should have a product, but does not.  Pseudo or short CDSs (less than 6
amino acids), or those marked with a rearrangement required for product exception,
are exempt from needing a product.

$^   BadTrnaCodon, 63
The tRNA codon recognized is an illegal value.

$^   BadTrnaAA, 64
The tRNA encoded amino acid is an illegal value.

$^   OnlyGeneXrefs, 65
There are gene xrefs but no gene features.  Records should normally have single-interval
gene features covering other biological features.  Gene xrefs are used only to override
the inheritance by overlap.

$^   UTRdoesNotAbutCDS, 66
The 5'UTR and 3'UTR features should exactly abut the CDS feature.

$^   BadConflictFlag, 67
The coding region conflict flag is set, but the translated product is the
same as the instantiated product Bioseq.

$^   ConflictFlagSet, 68
The coding region conflict flag is appropriately set, but this record should
be brought to the attention of the source database for possible correction.

$^   LocusTagProblem, 69
A gene locus_tag should be a single token, with no spaces.

$^   CollidingLocusTags, 70
Two gene features should not have the same locus_tag, which is supposed to be
a unique identifer.

$^   AltStartCodon, 71
An alternative start codon was used. This is rare, and it is expected that
confirmatory evidence will be cited.

$^   PartialsInconsistent, 72
There are several places in an entry where a sequence can be described as
either partial or complete. In this entry, these settings are inconsistent.
Make sure that the location and product Seq-locs, the Bioseqs, and the
SeqFeat partial flag all agree in describing this SeqFeat as partial or
complete.

$^   GenesInconsistent, 73
The gene on the genomic sequence of a genomic product set should be the
same as the gene on the cDNA product of the mRNA feature.

$$ SEQ_ALIGN, 6

$^   SeqIdProblem, 1
The seqence referenced by an alignment SeqID is not packaged in the record.

$^   StrandRev, 2
Please contact the sequence database for further help with this error.

$^   DensegLenStart, 3
Please contact the sequence database for further help with this error.

$^   StartLessthanZero, 4
Please contact the sequence database for further help with this error.

$^   StartMorethanBiolen, 5
Please contact the sequence database for further help with this error.

$^   EndLessthanZero, 6
Please contact the sequence database for further help with this error.

$^   EndMorethanBiolen, 7
Please contact the sequence database for further help with this error.

$^   LenLessthanZero, 8
Please contact the sequence database for further help with this error.

$^   LenMorethanBiolen, 9
Please contact the sequence database for further help with this error.

$^   SumLenStart, 10
Please contact the sequence database for further help with this error.

$^   AlignDimSeqIdNotMatch, 11
Please contact the sequence database for further help with this error.

$^   SegsDimSeqIdNotMatch, 12
Please contact the sequence database for further help with this error.

$^   FastaLike, 13
Please contact the sequence database for further help with this error.

$^   NullSegs, 14
Please contact the sequence database for further help with this error.

$^   SegmentGap, 15
Please contact the sequence database for further help with this error.

$^   SegsDimOne, 16
Please contact the sequence database for further help with this error.

$^   AlignDimOne, 17
Please contact the sequence database for further help with this error.

$^   Segtype, 18
Please contact the sequence database for further help with this error.

$^   BlastAligns, 19
BLAST alignments are not desired in records submitted to the sequence database.

$$ SEQ_GRAPH, 7

$^   GraphMin, 1
The graph minimum value is outside of the 0-100 range.

$^   GraphMax, 2
The graph maximum value is outside of the 0-100 range.

$^   GraphBelow, 3
Some quality scores are below the stated graph minimum value.

$^   GraphAbove, 4
Some quality scores are above the stated graph maximum value.

$^   GraphByteLen, 5
The number of bytes in the quality graph does not correspond to the
stated length of the graph.

$^   GraphOutOfOrder, 6
The quality graphs are not packaged in order - may be due to an old fa2htgs bug.

$^   GraphBioseqLen, 7
The length of the quality graph does not correspond to the length of the Bioseq.

$^   GraphSeqLitLen, 8
The length of the quality graph does not correspond to the length of the
delta Bioseq literal component.

$^   GraphSeqLocLen, 9
The length of the quality graph does not correspond to the length of the
delta Bioseq location component.

$^   GraphStartPhase, 10
The quality graph does not start or stop on a sequence segment boundary.

$^   GraphStopPhase, 11
The quality graph does not start or stop on a sequence segment boundary.

$^   GraphDiffNumber, 12
The number quality graph does not equal the number of sequence segments.

$^   GraphACGTScore, 13
Quality score values for known bases should be above 0.

$^   GraphNScore, 14
Quality score values for unknown bases should not be above 0.

$^   GraphGapScore, 15
Gap positions should not have quality scores above 0.

$^   GraphOverlap, 16
Quality graphs overlap - may be due to an old fa2htgs bug.

