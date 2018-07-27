/* ===========================================================================
 *
 *                            PUBLIC DOMAIN NOTICE
 *               National Center for Biotechnology Information
 *
 *  This software/database is a "United States Government Work" under the
 *  terms of the United States Copyright Act.  It was written as part of
 *  the author's official duties as a United States Government employee and
 *  thus cannot be copyrighted.  This software/database is freely available
 *  to the public for use. The National Library of Medicine and the U.S.
 *  Government have not placed any restriction on its use or reproduction.
 *
 *  Although all reasonable efforts have been taken to ensure the accuracy
 *  and reliability of the software and data, the NLM and the U.S.
 *  Government do not and cannot warrant the performance or results that
 *  may be obtained by using this software or data. The NLM and the U.S.
 *  Government disclaim all warranties, express or implied, including
 *  warranties of performance, merchantability or fitness for any particular
 *  purpose.
 *
 *  Please cite the author in any work or product based on this material.
 *
 * ===========================================================================
 *
 * File Name: vastsubs.c
 *
 * Author: Tom Madej
 *
 * Version Creation Date: 25 March 1998
 *
 * $Log: vastsubs.c,v $
 * Revision 6.1  1998/03/30 19:10:12  madej
 * Initial version of subset filtering routines.
 *
 */



/* This code implements subset filtering for the VAST structure neighbor hit lists. */

#include <stdio.h>
#include <ncbi.h>
#include <ncbimisc.h>



/* Coefficients (bases) for hash and increment functions. */

#define HBASE0		131	/* base for hash function */
#define HBASE1		3373	/* base for increment function */
#define MAXN_SUB	128	/* max number of subsets */
#define MAX_SUBNAME	64	/* max length of a subset name */
#define DOMID_SIZE	6	/* number of chars in a domain identifier */
#define NPRIMES		10	/* assortment of hash table sizes */



/* hash table nodes */

typedef struct ht_Node {
	Int4 num;		/* domain id array index for header, subset number for others */
	Int4 grpnum;		/* group number, i.e. cluster number within the subset */
	Int4 grank;		/* rank within the group */
	struct ht_Node *next;
} HT_NODE;



static Int4 TableSize;		/* size of the hash table (a prime) */
static HT_NODE *HashTable;	/* the actual hash table */
static Char *DomIdArray;	/* aux array for storing domain identifiers */
static Int4 NumberOfDomains;	/* number of domains read in */
static Int4 NumberOfSubsets;	/* number of subsets used */
static Int4 *SubsetList;	/* indices for subset names */
static Char *SubsetArray;	/* array for storing subset names */
static Int4 PrimeTable[NPRIMES] = { 100003, 200003, 300007, 400009, 500009, 600011, 700001,
					800011, 900001, 999983 };



/* Hash function used to identify chains/domains that belong to a given subset. */

static Int4
HashDomId(Char *domid, Int4 mod0)
{
	Int4 sum;
	Char *p;

	for (sum = 0, p = domid; *p; p++)
		sum = (HBASE0*sum + (Int4) *p) % mod0;

	return sum;

} /* end HashDomId */



/* Increment function used in double hashing scheme. */

static Int4
IncrDomId(Char *domid, Int4 mod0)
{
	Int4 sum;
	Char *p;

	for (sum = 0, p = domid; *p; p++)
		sum = (HBASE1*sum + (Int4) *p) % mod0;

	return sum % (mod0 - 1) + 1;

} /* end IncrDomId */



/* Insert an entry into the hash table. */

static Int4
SubsetInsert(Int4 idloc, Int4 mod0, Int4 sublen, Int4 *subset, Int4 *grps, Int4 *ranks)
{
	Int4 h, i, j, cnt, incr, h0;
	HT_NODE *htn;

	h0 = HashDomId(&DomIdArray[idloc], mod0);
	incr = IncrDomId(&DomIdArray[idloc], mod0);

	/* be sure to count the number of probes */
	for (i = cnt = 0; cnt < mod0; i += incr, cnt++) {
		h = (h0 + i) % mod0;

		if (HashTable[h].num < 0) {
			/* insert the entry */
			HashTable[h].num = idloc;

			for (j = 0; j < sublen; j++) {
				htn = (HT_NODE *) MemNew(sizeof(HT_NODE));
				htn->num = subset[j];
				htn->grpnum = grps[j];
				htn->grank = ranks[j];
				htn->next = HashTable[h].next;
				HashTable[h].next = htn;
			}

			break;
		}
	}

	/* since the table shouldn't overflow this return will signal a bug */
	if (cnt >= mod0)
		return -1;

	return h;

} /* end SubsetInsert */



/* Return the smallest prime from PrimeTable[] that is larger than or equal to the input.
 *
 * Warning: I've selected a small assortment of large primes for table sizes.  If the PDB
 * should ever grow so that we have more than about 250,000 domains, then we'll need to
 * add some larger table sizes!
 */

static Int4
GetPrime(Int4 n)
{
	Int4 i;

	for (i = 0; i < NPRIMES; i++)
		if (PrimeTable[i] >= n)
			return PrimeTable[i];

	return 0;

} /* end GetPrime */



/* Allocates space for the subset table and domain identifier array.  Reads in the subset lists.
 *
 * Here is the format for the file that contains the subset table.
 *
 *	1. The first line is an integer that gives the total number of entries (domains).
 *	2. The second line is an integer that gives the total number of subsets.
 *	3. The the next few lines are the subset entries.  There are 'k' of these lines,
 *	   where 'k' is the integer in 2).  Each subset entry consists of a subset number,
 *	   i.e. 1 thru k, and a subset name.  The subset name begins with the first non-blank
 *	   character and ends with a newline (so blanks may appear within the name), e.g.
 *	   "All of PDB".
 *	4. The remainder of the lines are the domain entries.  There are 'n' of these lines,
 *	   where 'n' is the integer in 1).  Each domain entry consists of:
 *	   a) an 8-character domain identifier (code);
 *	   b) an integer giving the number of subsets it belongs to;
 *	   c) a sequence 'm' triples of integers, where 'm' is the integer in 4b);
 *	   d) each triple in 4c) consists of a subset number, group number, and rank within
 *	      the group.
 */

Int4 LIBCALL
LoadSubsetTable(Char *fname)
{
	Int4 i, j, n, len, strindex, ndoms, nsubs;
	Int4 subset[MAXN_SUB], grps[MAXN_SUB], ranks[MAXN_SUB];
	FILE *fd;
	Char ch, domid[9];



	if ((fd = FileOpen(fname, "r")) == NULL)
		return 0;

	fscanf(fd, "%d", &ndoms);
	NumberOfDomains = ndoms;

	/* the table size should be a prime number >= 4*ndoms */
	if ((TableSize = GetPrime(4*ndoms)) == 0)
		return 0;

	HashTable = (HT_NODE *) MemNew(TableSize*sizeof(HT_NODE));
	DomIdArray = (Char *) MemNew((DOMID_SIZE + 1)*ndoms*sizeof(char));

	if ((HashTable == NULL) || (DomIdArray == NULL))
		return 0;

	/* zap the table just to be sure */
	for (i = 0; i < TableSize; i++) {
		HashTable[i].num = -1;
		HashTable[i].grpnum = 0;
		HashTable[i].next = NULL;
	}

	for (i = 0; i < (DOMID_SIZE + 1)*ndoms; i++)
		DomIdArray[i] = '\0';

	for (i = 0; i <= DOMID_SIZE; i++)
		domid[i] = '\0';

	/* read in the subset numbers and names */
	fscanf(fd, "%d", &nsubs);

	if (nsubs > MAXN_SUB)
		return 0;

	NumberOfSubsets = nsubs;
	SubsetList = (Int4 *) MemNew(nsubs*sizeof(Int4));
	SubsetArray = (Char *) MemNew(nsubs*MAX_SUBNAME*sizeof(Char));

	if ((SubsetList == NULL) || (SubsetArray == NULL))
		return 0;

	for (i = 0; i < nsubs; i++)
		SubsetList[i] = -1;

	for (i = 0, strindex = 0; i < nsubs; i++) {
		if (strindex > nsubs*MAX_SUBNAME)
			return 0;

		fscanf(fd, "%d", &j);

		if (j != i + 1)
			return 0;

		while (1) {
			ch = (Char) fgetc(fd);

			if ((ch == ' ') && (SubsetList[i] < 0))
				continue;

			if (SubsetList[i] < 0) {
				SubsetList[i] = strindex;
				len = 0;
			}

			if (ch != '\n')
				SubsetArray[strindex + len++] = ch;
			else {
				SubsetArray[strindex + len++] = '\0';
				strindex += len;
				break;
			}
		}
	}

	/* now read in the domain entries */
	for (i = strindex = 0; i < ndoms; i++) {
		/* read in the domain identifier and transfer it to DomIdArray[] */
		fscanf(fd, "%8c", domid);
		DomIdArray[strindex] = domid[0];
		DomIdArray[strindex + 1] = domid[1];
		DomIdArray[strindex + 2] = domid[2];
		DomIdArray[strindex + 3] = domid[3];
		DomIdArray[strindex + 4] = domid[5];
		DomIdArray[strindex + 5] = domid[7];

		/* read in the subset data triples */
		fscanf(fd, "%d", &n);

		if (n > MAXN_SUB)
			return 0;

		for (j = 0; j < n; j++)
			fscanf(fd, "%d %d %d", &subset[j], &grps[j], &ranks[j]);

		fscanf(fd, "\n");

		/* insert the entry into the hash table */
		if (SubsetInsert(strindex, TableSize, n, subset, grps, ranks) < 0)
			return 0;

		strindex += DOMID_SIZE + 1;
	}

	FileClose(fd);
	return 1;

} /* end LoadSubsetTable */



/* Returns the number of domains. */

Int4 LIBCALL
GetNumberOfDomains(void)
{
	return NumberOfDomains;

} /* end GetNumberOfDomains */



/* Returns the number of subsets. */

Int4 LIBCALL
GetNumberOfSubsets(void)
{
	return NumberOfSubsets;

} /* end GetNumberOfSubsets */



/* Returns the subset name corresponding to a subset number. */

CharPtr LIBCALL
GetSubsetName(Int4 num)
{
	if ((num < 1) || (num > NumberOfSubsets))
		return NULL;

	return &SubsetArray[SubsetList[num - 1]];

} /* end GetSubsetName */



/* Returns the subset number corresponding to a subset name. */

Int4 LIBCALL
GetSubsetNum(Char *subname)
{
	Int4 i;

	for (i = 0; i < NumberOfSubsets; i++)
		if (strcmp(subname, &SubsetArray[SubsetList[i]]) == 0)
			return i + 1;

	return 0;

} /* end GetSubsetNum */



/* This routine returns 0 if the given domain id does not belong to the specified subset.
 * If it does belong then it returns the group (cluster) number within that subset, and
 * also the rank within the cluster.
 */

Int4 LIBCALL
BelongsToSubset(Char *domid, Int4 sub, Int4 *grpnum, Int4 *grank)
{
	Int4 h, i, h0, incr, cnt;
	HT_NODE *htn;

	h0 = HashDomId(domid, TableSize);
	incr = IncrDomId(domid, TableSize);
	*grpnum = *grank = 0;

	for (i = cnt = 0; cnt < TableSize; i += incr, cnt++) {
		h = (h0 + i) % TableSize;

		if (HashTable[h].num < 0)
			return 0;

		if (strcmp(domid, &DomIdArray[HashTable[h].num]) == 0) {
			for (htn = HashTable[h].next; htn != NULL; htn = htn->next)
				if (htn->num == sub) {
					*grpnum = htn->grpnum;
					*grank = htn->grank;
					return 1;
				}

			return 0;
		}
	}

	/* if we get to here there's a problem */
	return -1;

} /* end BelongsToSubset */

