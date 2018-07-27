------------------------------------------------------------------------


    Introduction to the Standalone WWW Blast server

------------------------------------------------------------------------


      Index

    * Introduction <#Inroduction>
    * What's new in this revision? <#Whatsnew>
    * Installation of the Standalone WWW server <#Installation>
    * Description of files in the distribution <#Description_of_files>
    * Configuration of BLAST databases <#Configuration_of_databases>
    * PSI/PHI Blast notes <#PSI_PHI_Notes>
    * Client/server versions for Entrez lookup and taxonomy reports
      <#CS_Notes>
    * Blast 2 sequences <#Bl2Seq>
    * XML output <#XML_Output>
    * Out of Frame BLASTX <#OOF_blastx>
    * RPS Blast <#RPS_blast>
    * Description of tags for main BLAST input page <#Description_of_tags>
    * Server configuration file and logfile <#Configuration_file>
    * How to debug WWW Blast programs <#Debugging>


      Introduction

This standalone WWW BLAST server suite of programs was designed similar
to the regular NCBI BLAST server and such command-line NCBI BLAST
programs like "blastall", "blastpgp", "rpsblast" and "megablast". It
incorporates most features, which exist in NCBI BLAST programs and
should be relatively easy to use. This server does not support any
request queuing and load balancing. As soon as the user hits a "Search"
button, BLAST starts immediately if entered information is valid. So,
this server is not intended to handle large load, which may exist in
public service. Such queueing and loadbalancing however may be
implemented using such products as Load Sharing Facility - "LSF" from
Platform Computing Corporation <http://www.platform.com>. Interface to
"LSF" was implemented in NCBI, however this was not included in this
suite. Standalone server assumes that users have their own BLAST or
RPS-BLAST database(s), that should be searched and want to have a simple
WWW interface to such search. It is STRONLY recommended that user have
experience in installation and running standalone NCBI BLAST programs.

After files are uncompressed, server is ready to be used immediately.
Any customizations to the program are welcomed. The source code for WWW
BLAST is now part of the NCBI C toolkit, which can be downloaded from
the NCBI FTP web site: ftp://ftp.ncbi.nih.gov
<ftp://ftp.ncbi.nih.gov/toolbox/ncbi_tools/README.htm>
The compilation of the NCBI C toolkit includes the WWW BLAST
executables. The files in the C toolkit most relevant for the WWW BLAST
executables are: wwwblast.c, wwwbutl.c, psiblast.c, wblast2.c.


      What's new in this revision?

May 10, 2004

    * XML output brought in sync with the text output, masking filtered
locations in query sequences.

February 2, 2004

    * All source code moved to the NCBI C toolkit.
    * Mouseover feature fixed in graphical overview.
    * All binaries synchronized with the latest NCBI C toolkit release.
      All future releases will be automatically synchronized with future
      toolkit releases.
    * Recompilation procedure changed. WWW BLAST binaries are now
compiled as part of the C toolkit.

November 21, 2003

    * Fixed a bug with absent images on PSI-BLAST iterations.
    * Removed header and progress messages from XML output.
    * All binaries recompiled with the newest version of the NCBI C
toolkit libra ries.

May 9, 2003

    * Cleaned the Makefile for recompiling the sources.
    * Added discontiguous Mega BLAST options.
    * Added option to choose a subsequence location for query.
    * Added option to choose database genetic code.
* All binaries recompiled.

April 21, 2003

    * Added instructions for recompilation of binaries from the source
      code to this readme file.
    * Fixed a bug in processing of the configuration file that limited
      allowed number of databases.
    * Added images for the Linkout and UniGene links from the BLAST output.
    * Corrected paths to the graphical overview for PSI/PHI BLAST output.
    * All binaries recompiled with the newest version of the NCBI C
toolkit libraries.

February 10, 2003

    * Added support for multi-query XML output.
* All binaries recompiled.

December 2, 2002

* All binaries recompiled with the newest version of the toolkit.

August 6, 2002

* Recompiled all binaries to include recent bug fixes.

June 18, 2002

    * Removed limit on the total length of database names.
    * Removed xmlblast.cgi and other unneeded XML related files.
* Recompiled all programs.

January 11, 2002

    * Removed formatdb, makemat and copymat binaries from the archive -
      those should be taken from the NCBI BLAST executables archive
      <ftp://ncbi.nlm.nih.gov/blast/executables>
    * Added tabular output format
* Added option to mask lower case in query

May, 2 2001

    * No major changes. All programs have been recompiled and made
synchronous to the latest NCBI tookit release.

November 3, 2000

* Blast 2 sequences <#Bl2Seq> was added

October, 19 2000

    * RPS Blast <#RPS_blast> was added
    * Out of Frame BLASTX <#OOF_blastx> (OOF) now available for testing
and suggestions.

September, 28 2000

    * Added possibility to limit search to results of Entrez query
(Regular client/server BLAST)

September, 11 2000

    * Added MEGABLAST <docs/megablast_readme.html> search.
    * Added possibility to have multiple FASTA query input - batch
searches with multiple graphical overviews. (Regular BLAST)

August, 22 2000

    * Added new advanced statistics to PSI Blast and ability to produce
      Smith-Waterman alignments
* Added support for XML output.

May, 17 2000

    * PSI and PHI Blast were added to this distribution
    * Added support for client/server interface for gi/accesion lookups
      using Entrez
    * Added possibility to print Taxonomy reports
    * Added option to print alternative alignment with specific color
schema

March, 20 2000

* Initial revision


      Installation of the Standalone WWW server

After downloading the file wwwblast.Your_platform.tar.gz to your
computer, place it into document directory of HTTPD server and
uncompress it by

    gzip -d wwwblast.Your_platform.tar.gz
    tar -xvpf wwwblast.Your_platform.tar

Please note that parameter "p" in tar options is significant - it will
preserve file access options stored in the distribution. Temporary
directory for BLAST overview images (TmpGifs) should have 777
permission, and logfiles (wwwblast.log and psiblast.log) should have 666.

After you have uncompressed the distribution file, "blast" directory
will be created. You can access sample BLAST HTML input forms using URLs:

    * http://your_hostname/blast/blast.html
    * http://your_hostname/blast/blast_cs.html
    * http://your_hostname/blast/psiblast.html
    * http://your_hostname/blast/psiblast_cs.html
    * http://your_hostname/blast/megablast.html
    * http://your_hostname/blast/megablast_cs.html
    * http://your_hostname/blast/wblast2.html
* http://your_hostname/blast/wblast2_cs.html

This distribution comes with 2 BLAST databases: "test_aa_db" - sample
protein database and "test_na_db" - sample nucleotide database. These
databases are configured to be searchable immediately with compatible
BLAST programs.


      Description of files in the distribution

* Root directory (./blast):

Files with suffix "*_cs.*" are analogous to the files without such
suffix, with added capability to make client/server Entrez lookups for
sequence gis and accessions.

blast.cgi, blast_cs.cgi

- BLAST search start-up C-shell files

.nlmstmanrc

- Configuration file for the graphical overview image (do not edit!)

blast.html, blast_cs.html

- sample BLAST search input HTML forms

megablast.html, megablast_cs.html

- sample MEGABLAST search input HTML forms

rpsblast.html, rpsblast_cs.html

- sample RPS BLAST search input HTML forms

blast.rc

- Default configuration file <#Configuration_file> for the WWW BLAST server

psiblast.rc

- Default configuration file <#Configuration_file> for the PSI/PHI WWW
BLAST server

psiblast.cgi, psiblast_cs.cgi

- PSI/PHI BLAST search start-up C-shell files

psiblast.html, psiblast_cs.html

- sample PSI/PHI BLAST search input HTML forms

psiblast.REAL, psiblast_cs.REAL

- Main PSI/PHI BLAST server executables

wblast2.html, wblast2_cs.html

- sample BLAST 2 sequences search input HTML forms

wblast2.REAL, wblast2_cs.REAL

- Main BLAST 2 sequences server executables

bl2bag.cgi

- CGI used to create 2 sequences alignment image on the fly

blast_form.map

- Auxiliary map file for the front BLAST image

nph-viewgif.cgi

- CGI program used to view and delete overview images

readme.html

- this documentation

wwwblast.log

- default logfile

psiblast.log

- default PSI/PHI Blast logfile

ncbi_blast.rc

- sample file for full NCBI set of databases

    * ./data
    * directory - matrices used in BLAST search ./db
    * directory. - Files of test BLAST and RPS-BLAST databases. ./docs:
    * - HTML pages used in sample BLAST search input pages ./images
    * - images used in the sample BLAST search input pages ./Src
    * - source directory for the WWW BLAST server. ./Src/XML
      - source directory for creation of files related to the XML output.
          o blstxml.asn - ASN.1 definition for Blast XML
    o blstxml.dtd - corresponding DTD
    * ./TmpGifs
- storage for temporary BLAST overview gif files


      Configuration of BLAST databases

To set up databases for the standalone WWW BLAST server, it is necessary
to follow these steps:

   1. Put a file with concatenated FASTA entries in the "./db" directory
   2. Run "formatdb" program, available from the NCBI ftp site
      <ftp://ncbi.nlm.nih.gov/blast/executables> to format the database.
   3. Add name of the database into server configuration file
      <#Configuration_file>
   4. Add name of the database into (PSI/PHI) WWW BLAST search form 


      PSI/PHI Blast notes

There is one significant feature of the PSI/PHI Blast server: FASTA
files from which the BLAST databases are formatted should have GI
numbers in all sequence ids. This is almost always true for FASTA files
from the NCBI FTP site. Local databases may not be used with this
version of PSI/PHI Blast unless they have ">gi|12345..." prefix in the
definition line.

Databases for the PSI/PHI Blast should always be created with formatdb
using the "-o T" flag. Test database "test_aa_db" was created using this
flag and database "test_na_db" was created without this flag.

If this distribution is installed not in the "/blast" directory under
HTTPD documents root directory, than path to the distribution should be
set by an environment variable WWW_ROOT_PATH in the file psiblast.cgi or
psiblast_cs.cgi


      Client/server version for Entrez lookup and taxonomy reports

Regular Blast, PSI/PHI Blast, MegaBlast and Blast 2 Sequences have
client/server versions for Entrez gi/accession lookups and printing
Taxonomy reports. Configuration of client/server interface from the user
to NCBI should be done the same way as for any other client/server
program to NCBI. If program "blastcl3" works without problems, this
server should work OK as well. If user has firewall - default
configuration will definitely fail to work properly and this case will
require special configuration. If user has problems with such
configuration, one should write to info@ncbi.nlm.nih.gov
<mailto:info@ncbi.nlm.nih.gov> for further assistance.


      XML output

Possibility to produce XML output was added to this server. XML
definition of BLAST output is tied to the simple ASN.1 specification
designed for this case. These definitions may be found in the directory
./Src/XML. Any recommendations on improvements to this (possible)
standard may be sent to blast-help@ncbi.nlm.nih.gov
<mailto:blast-help@ncbi.nlm.nih.gov> XML may be printed by setting
"Alignment view" in blast.html or (blast_cs.html) page to "BLAST XML".
Resulting page will look empty - but if you open the page source (in
Netscape - View -> Page source) - you will see the complete XML document.


      Blast 2 sequences

Blast 2 sequences program was initially written by Tatiana Tatusova and
Tom Madden and was presented in the article: Tatiana A. Tatusova, Thomas
L. Madden (1999), "Blast 2 sequences - a new tool for comparing protein
and nucleotide sequences", FEMS Microbiol Lett. 174:247-250
<http://www.ncbi.nlm.nih.gov/htbin-post/Entrez/query?uid=10339815&form=6&db=m&Dopt=b>.
The standalone WWW version of the program mirrors the NCBI Blast 2
Sequences <http://www.ncbi.nlm.nih.gov/blast/bl2seq/bl2.html> web page.


      RPS Blast

RPS Blast or "Reversed Position Specific BLAST" is a very fast
alternative to the program IMPALA
<http://www.ncbi.nlm.nih.gov:80/entrez/query.fcgi?cmd=Retrieve&db=PubMed&list_uids=10745990&dopt=Abstract>.
It has the same general objective - to compare a sequence to a
collection of conserved domains (aka motifs, profiles, HMMs). RPS Blast
has a completely different implementation that has increased the speed
of profiles search 10 to 100 times depending on search conditions in
comarison to IMPALA. RPS Blast has an option to perform a translated
search of DNA sequences against these conserved domains. Currently RPS
Blast is one of the tools chosen to annotate human genome at NCBI and is
the basis for the CDD Blast search
<http://www.ncbi.nlm.nih.gov/Structure/cdd/wrpsb.cgi> page.

Databases for RPS blast are hardware dependent - for speed reasons.
Hence they are different for big/little endian platforms.

To build RPS database it is necessary to follow the procedure explained
in the file "README.rps", that comes with this distribution. There is a
small RPS database available for testing. This database is a part of a
real NCBI database used in CDD search page. Full NCBI database is
available in platform-independent form from FTP site.


      Out of Frame BLASTX

The OOF version of the blastx program finds alignments between a
nucleotide sequence translated into 3 frames and protein sequences, in
which query sequence frames can be shifted within one alignment. The
algorithm and low-level alignment functions were written by Zheng Zhang,
currently at Paracel Inc. <http://www.paracel.com>, and incorporated
into regular BLAST API by Sergei Shavirin.

XML and tabular output are not yet implemented for the OOF alignments.


      Description of tags for the main BLAST input page

This standalone server has a tag convention analogous to the regular
NCBI BLAST server. Sample BLAST search forms may be changed to
accommodate particular needs of the user in the custom search. Here is
the list of these tags and their meaning. If some tag is missing from
the search input page, it will take a default value. Exceptions are tags
PROGRAM, DATALIB and SEQUENCE (or SEQFILE), that should always be set.

    * PROGRAM
      - name of the BLAST program. Supported values include programs:
    * blastn, blastp, blastx, tblastx and tblastn DATALIB
      - name of the database(s) to search. This implementation includes
      possibility to use multiple databases. To use multiple databases
      several "DATALIB" tags should be used on the page for example
      using checkboxes (look for example at Microbial Genomes Blast
      Databases BLAST
      <http://www.ncbi.nlm.nih.gov/Microb_blast/unfinishedgenome.html>
      at NCBI). Note, that all of these databases should be properly
    * written in the server configuration file. SEQUENCE
      and SEQFILE - these tags used to pass sequence. First SEQUENCE tag
      is used for the input sequence. If it is missing, the SEQFILE tag
    * is used instead. UNGAPPED_ALIGNMENT
      - default BLAST search is a gapped search; this tag, if set, will
    * turn gapped alignment off. MAT_PARAM
      used to set 3 parameters at the same time. Value for this tag
      should be in format " " where mat_name - string name of the matrix
      (BLOSUM62, etc), d1 - integer for cost to open gap and d2 - cost
    * to extend gap (-G and -E parameters in blastall respectably) GAP_OPEN
      - set value for cost to open gap - 0 or missing tag invoked
    * default behavior GAP_EXTEND
      - set value for cost to extend gap - 0 or missing tag invoked
    * default behavior X_DROPOFF
      - Dropoff (X) for blast extensions in bits (default if zero) (-y
    * parameter in "blastpgp" program) GENETIC_CODE
    * - Query Genetic code to use (for blastx only) THRESHOLD_2
      - Threshold for extending hits in second pass in multipass model
    * search MATRIX
    * - Matrix (default is BLOSUM62) (-M in blastall) EXPECT
    * - Expectation value (-e in blastall) NUM_OF_BITS
    * - Number of bits to trigger gapping (-N in blastpgp) NCBI_GI
      - If formated database use SeqIds in the NCBI format this option
    * will turn printing of gis together with accessions. FILTER
      - Multiple instances of values of this tag are concatenated and
      passed to the engine as "filter_string" ("L" for low complexity
      and "m" if filter should be set for lookup table only) - any
      letter will turn default filtering on - DUST for nucleotides and
    * SEG for proteins (-F in blastall) DESCRIPTIONS
    * - Number of one-line descriptions in the output (-v in blastall)
      ALIGNMENTS
    * - Number of alignments to show (-b in blastall) COLOR_SCHEMA
      - Color schema to use in printing of alternative alignment. This
      option is valid only for blastp and blastn programs. If set - it
    * will override option set by "ALIGNMENT_VIEW" TAX_BLAST
      - Print taxonomy reports. This option is valid only for
    * client/server version of regular Blast XML_OUTPUT
      - Print XML Blast output. All other alignment view options will be
    * disabled ENTREZ_QUERY
      - Limit search to results of Entrez query. Only for client/server
    * version RPSBLAST
      - This tag with turn "blastp" or "blastx" search into RPS Blast
    * search for the rps blast database. OOF_ALIGN
      -This flag if set to non-zero digit will turn on OOF alignment for
    * "blastx" and will set frame shift penalty to this value.
      OTHER_ADVANCED
      - this tag allows to input string analogous to the command line
      parameters of blastall. Setting parameter in OTHER_ADVANCED tag
      will override all other settings of this parameter. Supported
      options include:
          o -G
          o gap open cost -E
          o gap extend cost -q
          o penalty for nucleotide mismatch -r
          o reward for nucleotide match -e
          o expect value -W
          o wordsize -v
          o Number of descriptions to print -b
          o Number of alignments to show -K
          o Number of best hits from a region to keep -Y
    * effective search space ALIGNMENT_VIEW
      - will set type of alignment to show. Available options include:
          o 0 - Pairwise
          o 1 - query-anchored with identities
          o 2 - query-anchored without identities
          o 3 - flat query-anchored with identities
          o 4 - flat query-anchored without identities
          o 7 - BLAST XML (equivalent to setting the XML_OUTPUT tag)
    o 9 - Hit Table - to produce tabular output
    * OVERVIEW
    * - used to turn on or off printing of alignment overview image
      BLAST_TYPE
      - a special tag to distinguish different BLAST search types. See
the description of a configuration file.


      Server configuration file and logfile

Default configuration file is "blast.rc" and logfile "wwwblast.log".
Setting tag BLAST_TYPE to specific value may change these names. This is
useful if few different search input pages use the same CGI search
engine, but significantly different by content and priorities. A sample
configuration file comes with this distribution:

This file will set how many CPUs will be used in the BLAST search and
what databases may be used with what programs. Logfile currently stores
only limited information but also may be updated by programmers to store
more values in it. Please note, that usually HTTPD servers run by
accounts that do not have write access to disk, so the logfile
permission should be set to 666.


      How to debug WWW Blast programs

There is a way to debug these programs.

   1. Add line "setenv DEBUG_COMMAND_LINE TRUE" into the *.cgi file
      (uncomment it)
   2. Run search that results in the problem - this should create a file
      "/tmp/__web.in" in the "/tmp" directory.
   3. Set all necessary environment variables on the command line
      (BLASTDB at least) and run from command-line: "blast.REAL <
      /tmp/__web.in" 

This should do your problematic search without WWW. If this resulted in
coredump - you may look into the core file with:

dbx blast.REAL core

and then use command "where" to print stack.

------------------------------------------------------------------------

BLAST Help Desk <mailto:blast-help@ncbi.nlm.nih.gov>

Last modified: Fri Jan 11, 2002

