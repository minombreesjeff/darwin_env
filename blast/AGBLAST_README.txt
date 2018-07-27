		  Apple/Genentech BLAST 2.2.10
			June 22nd, 2005

Apple/Genentech (A/G) BLAST is an enhanced, platform-specific
optimization of NCBI's standalone Basic Local Alignment Search Tool
(BLAST) developed by Apple's Advanced Computation Group.  In
particular, we have enhanced BLASTn (which finds similarities between
nucleotide sequences) for the PowerPC G4/G5 processors and Mac OS X;
note that this means the optimized code will not build or run on other
platforms.  Additional details are available below.

CONTENTS

* Installing A/G BLAST
* Building A/G BLAST
* For More Information
* Changes from Previous Versions


Installing A/G BLAST
--------------------

A/G BLAST is available from developer.apple.com/darwin as both the compiled
binary, as well as the complete NCBI source to BLAST with Apple
modifications.  If you have already installed this version of NCBI
blast, you can simply unarchive the binary and change the permissions
to replace your current version of blastall.

	$ gunzip blastall-2.2.10.gz
	$ chmod a+x blastall
	$ mv -f blastall dest-dir # e.g., ncbi/builds

Note that installing A/G BLAST will not have any effect on BLAST
searches performed through a browser (WWW BLAST) or search queries
submitted to NCBI's servers (Network BLAST).  The changes only apply
to the standalone version of BLAST.


Building A/G BLAST
------------------

A/G BLAST is derived from NCBI's BLAST, and can be built in the same
manner. If you are building BLAST from the source archive, the build
script can be found at "ncbi/make/makedis.csh".  Assuming you have
downloaded the source into the "~/Desktop" directory, do the
following:

	Launch /Applications/Utilities/Terminal.app
	$ cd ~/Desktop
	$ gnutar -xzf agblast-2.2.10.tgz # if not already extracted
	$ cd agblast-2.2.10
	$ ./ncbi/make/makedis.csh

This will run the build shell script.  The build process takes several
minutes. When the build has finished, this message will appear:

     *********************************************************
     *The new binaries are located in ./ncbi/build/ directory*
     *********************************************************

As the above message indicates, blastall and all other build result
executables are in "ncbi/build".


For More Information
--------------------

For further instructions on building and using BLAST, see NCBI's BLAST
demo at:

	ftp://ftp.ncbi.nih.gov/blast/demo/

More general information about BLAST can be found at:

	http://www.ncbi.nlm.nih.gov/BLAST/

which also includes a tutorial on using BLAST.


Changes from Previous Versions
------------------------------

NEW IN VERSION 2.2.10:

- updated original source to NCBI version 2.2.10, enabling A/G BLAST to
  compile and run on OS X 10.4

