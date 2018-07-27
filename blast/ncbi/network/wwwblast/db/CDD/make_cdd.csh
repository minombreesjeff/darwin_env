#!/bin/csh -f

#
# First regular BLAST database should be creeated for the given concatenated list
# of profiles in order
#
formatdb -i test_CDD_db -p T 
#
# Now we have to prepare memory-mapped lookup table and PSI matrix for collection of
# profiles
#
copymat -P test_CDD_db -r T
