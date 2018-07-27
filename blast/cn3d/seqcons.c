/*   $Id: seqcons.c,v 6.23 2000/05/05 13:28:59 thiessen Exp $
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* File Name:  $Id: seqcons.c,v 6.23 2000/05/05 13:28:59 thiessen Exp $
*
* Authors:  Paul Thiessen
*
* Version Creation Date: 2/10/2000
*
* File Description: to calculate colors for columns to visualize
*                   sequence conservation
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: seqcons.c,v $
* Revision 6.23  2000/05/05 13:28:59  thiessen
* fix to handle updated profiler
*
* Revision 6.22  2000/04/27 13:36:19  thiessen
* change algorithm GUI names
*
* Revision 6.21  2000/04/11 17:14:20  thiessen
* remove warning for zero-length alignment
*
* Revision 6.20  2000/04/06 19:03:20  thiessen
* new BLOSUM62 conservation coloring
*
* Revision 6.19  2000/04/06 14:49:46  thiessen
* minor optimizations
*
* Revision 6.18  2000/04/04 15:50:45  thiessen
* fix coloring when aligned gaps present
*
* Revision 6.17  2000/03/21 14:16:11  thiessen
* improved color ramp for variety scheme
*
* Revision 6.16  2000/03/17 22:49:00  thiessen
* fix for multi-chain / multi-model features ; added feature-move ; misc bug fixes
*
* Revision 6.15  2000/03/16 20:31:45  thiessen
* fixes to color; new color-by-variety algorithm
*
* Revision 6.14  2000/03/14 18:03:14  thiessen
* add target row to MMD; remove rowmgr
*
* Revision 6.13  2000/03/03 20:05:17  thiessen
* removal of palette-building pass if in 24-bit color
*
* Revision 6.12  2000/03/03 14:22:29  thiessen
* moved back into ncbicn3d
*
* Revision 1.1  2000/03/02 21:20:41  thiessen
* many improvements, moved into object library
*
* Revision 6.10  2000/02/25 02:53:15  thiessen
* check AlignMgr rather than bAligned flag to see if a residue is aligned
*
* Revision 6.9  2000/02/23 18:56:30  thiessen
* move to 1-based row numbers
*
* Revision 6.8  2000/02/19 21:25:58  thiessen
* split of cn3dmodl into cn3dmodl and cn3dstyl
*
* Revision 6.7  2000/02/17 15:41:11  thiessen
* added CSC algorithm selection submenu
*
* Revision 6.6  2000/02/16 21:25:31  thiessen
* new row manager module; made Cn3D use row-wise color storage for aligned strucs
*
* Revision 6.5  2000/02/16 14:02:00  thiessen
* further progress on seqcons module
*
* Revision 6.4  2000/02/14 21:22:00  kans
* include seqcons.h after other includes, to pick up SeqAlign, SeqId, etc., otherwise types undefined at first use
*
* Revision 6.3  2000/02/14 20:04:23  thiessen
* another fix for solvent; more filling in of seqcons
*
* Revision 6.2  2000/02/14 12:32:31  thiessen
* fix solvent identification; progress on seqcons module
*
* Revision 6.1  2000/02/11 01:12:35  thiessen
* new module for sequence conservation coloring
*
*
*
* ==========================================================================
*/

#include <vibrant.h>
#include <objseq.h>
#include <mmdbapi.h>
#include <alignmgr.h>
#include <actutils.h>
#include <seqcons.h>
#include <seqmgr.h>
#include <viewmgr.h>


#define MESSAGE_TYPE MSG_POST

#define ERR_RETURN(msg) do { \
    Message(MESSAGE_TYPE, "Color-by-conservation module error: %s", (msg)); \
    return; \
} while (0)

#define ERR_RETURN_VAL(msg, val) do { \
    Message(MESSAGE_TYPE, "Color-by-conservation module error: %s", (msg)); \
    return (val); \
} while (0)


/**** data structures to hold column colors ****/

static Nlm_UcharPtr CSC_CurrentColors = NULL;
static Nlm_Int4 CSC_CurrentNColumns = 0;

static int CSC_CurrentProfileRows;


/**** data structures to hold row/SeqId index ****/

static SeqAlignPtr CSC_CurrentSeqAlign = NULL;
static ACTProfilePtr CSC_CurrentProfile = NULL;


/**** functions to access color data ****/

static Nlm_Int4 CSC_GetColumnColorByRowAndAlignLoc(Nlm_Int4 row, Nlm_Int4 alignLoc)
{
    if (alignLoc < 0 || alignLoc >= CSC_CurrentNColumns)
        ERR_RETURN_VAL("alignLoc out of range", CSC_COLOR_ERROR);
    else
        return ((CSC_CurrentColors[3*alignLoc] << 16) +
                (CSC_CurrentColors[3*alignLoc+1] << 8) +
                 CSC_CurrentColors[3*alignLoc+2]);
}

Nlm_Int4 CSC_GetColumnColorByRow(Nlm_Int4 row, Nlm_Int4 seqLoc)
{
    Nlm_Int4 alignLoc = -1;

    if (row <= 0)
        return CSC_COLOR_ERROR;
    
    /* determine alignment location (column) from bioseq location */
    alignLoc = AlnMgrMapBioseqToSeqAlign(CSC_CurrentSeqAlign, seqLoc, row, NULL);

    /* then return the appropriate column color */
    if (alignLoc < 0)
        ERR_RETURN_VAL("can't map seqLoc to alignLoc", CSC_COLOR_ERROR);
    if (alignLoc >= CSC_CurrentNColumns) {
#ifdef _DEBUG
        Message(MSG_POST,"row %i, seqloc %i, alignloc %i, align len %i",
            row, seqLoc, alignLoc, CSC_CurrentNColumns);
#endif
        ERR_RETURN_VAL("alignLoc out of range", CSC_COLOR_ERROR);
    }

    return CSC_GetColumnColorByRowAndAlignLoc(row, alignLoc);
}


/**** functions for various algorithms to assign colors based on profile ****/

typedef void (*CSC_AlgorithmFunc)(void); /* must all be of this type */

static Nlm_Uint1 CSC_FullIdentityColor[3] = { 255, 25, 25 },
                 CSC_ZeroIdentityColor[3] = { 100, 100, 255 };


/* ramp depending on how many different residue types are present */
static void CSC_Algorithm_ShowVariety(void)
{
    static int *variety = NULL, vSize = -1;
    ACTProfilePtr app;
    int c, pr, offset, maxResTypes = 1, nResTypes, nRows, i = 0;
    Nlm_Int4 foundFlags, typeBit;
    double scale;
    FloatHi freqSum;

    /* static cache for color varieties */
    if (vSize != CSC_CurrentNColumns) {
        if (variety) MemFree(variety);
        variety = (int *) MemNew(CSC_CurrentNColumns * sizeof(int));
        if (!variety) return;
        vSize = CSC_CurrentNColumns;
    }

    nRows = AlnMgrGetNumRows(CSC_CurrentSeqAlign);

    /* first determine column variation and max */
    for (app = CSC_CurrentProfile; app; app = app->next) {
        for (c = 0; c < app->len; c++) {
            foundFlags = nResTypes = 0;
            freqSum = 0.0;
            for (pr = 1; pr < CSC_CurrentProfileRows; pr++) {
                if (((int) (app->freq[pr][c] + 0.001)) > 0) {
                    freqSum += app->freq[pr][c];
                    typeBit = ((Nlm_Int4) 1) << pr;
                    if (!(foundFlags & typeBit)) {
                        nResTypes++;
                        foundFlags |= typeBit;
                    }
                }
            }
            /* each gap counts as an additional variety */
            nResTypes += nRows - app->numseq;
            nResTypes += app->numseq - ((int) (freqSum + 0.001));

            variety[i++] = nResTypes;
            if (nResTypes > maxResTypes)
                maxResTypes = nResTypes;
        }
    }

    /* then color according to variety in each column, normalized by max variety */
    for (i = 0; i < CSC_CurrentNColumns; i++) {
        nResTypes = variety[i];
        if (maxResTypes == 1)
            scale = 0.0;
        else
            /* scale [1..maxResTypes] onto [0..1] */
            scale = 1.0 * (nResTypes - 1) / (maxResTypes - 1);

        offset = 3 * i;
        CSC_CurrentColors[offset] = (Nlm_Uint1)
            (scale * (CSC_ZeroIdentityColor[0] - CSC_FullIdentityColor[0])
                + CSC_FullIdentityColor[0]);
        CSC_CurrentColors[offset+1] = (Nlm_Uint1)
            (scale * (CSC_ZeroIdentityColor[1] - CSC_FullIdentityColor[1])
                + CSC_FullIdentityColor[1]);
        CSC_CurrentColors[offset+2] = (Nlm_Uint1)
            (scale * (CSC_ZeroIdentityColor[2] - CSC_FullIdentityColor[2])
                + CSC_FullIdentityColor[2]);
    }
}

/* stuff for BLOSUM coloring */
#define CSC_BLOSUMSIZE 24
static const char CSC_Blosum62Fields[CSC_BLOSUMSIZE] =
   { 'A', 'R', 'N', 'D', 'C', 'Q', 'E', 'G', 'H', 'I', 'L', 'K', 'M', 
     'F', 'P', 'S', 'T', 'W', 'Y', 'V', 'B', 'Z', 'X', '*' };
static const Nlm_Int1 CSC_Blosum62Matrix[CSC_BLOSUMSIZE][CSC_BLOSUMSIZE] = {
/*       A,  R,  N,  D,  C,  Q,  E,  G,  H,  I,  L,  K,  M,  F,  P,  S,  T,  W,  Y,  V,  B,  Z,  X,  * */
/*A*/ {  4, -1, -2, -2,  0, -1, -1,  0, -2, -1, -1, -1, -1, -2, -1,  1,  0, -3, -2,  0, -2, -1,  0, -4 },
/*R*/ { -1,  5,  0, -2, -3,  1,  0, -2,  0, -3, -2,  2, -1, -3, -2, -1, -1, -3, -2, -3, -1,  0, -1, -4 },
/*N*/ { -2,  0,  6,  1, -3,  0,  0,  0,  1, -3, -3,  0, -2, -3, -2,  1,  0, -4, -2, -3,  3,  0, -1, -4 },
/*D*/ { -2, -2,  1,  6, -3,  0,  2, -1, -1, -3, -4, -1, -3, -3, -1,  0, -1, -4, -3, -3,  4,  1, -1, -4 },
/*C*/ {  0, -3, -3, -3,  9, -3, -4, -3, -3, -1, -1, -3, -1, -2, -3, -1, -1, -2, -2, -1, -3, -3, -2, -4 },
/*Q*/ { -1,  1,  0,  0, -3,  5,  2, -2,  0, -3, -2,  1,  0, -3, -1,  0, -1, -2, -1, -2,  0,  3, -1, -4 },
/*E*/ { -1,  0,  0,  2, -4,  2,  5, -2,  0, -3, -3,  1, -2, -3, -1,  0, -1, -3, -2, -2,  1,  4, -1, -4 },
/*G*/ {  0, -2,  0, -1, -3, -2, -2,  6, -2, -4, -4, -2, -3, -3, -2,  0, -2, -2, -3, -3, -1, -2, -1, -4 },
/*H*/ { -2,  0,  1, -1, -3,  0,  0, -2,  8, -3, -3, -1, -2, -1, -2, -1, -2, -2,  2, -3,  0,  0, -1, -4 },
/*I*/ { -1, -3, -3, -3, -1, -3, -3, -4, -3,  4,  2, -3,  1,  0, -3, -2, -1, -3, -1,  3, -3, -3, -1, -4 },
/*L*/ { -1, -2, -3, -4, -1, -2, -3, -4, -3,  2,  4, -2,  2,  0, -3, -2, -1, -2, -1,  1, -4, -3, -1, -4 },
/*K*/ { -1,  2,  0, -1, -3,  1,  1, -2, -1, -3, -2,  5, -1, -3, -1,  0, -1, -3, -2, -2,  0,  1, -1, -4 },
/*M*/ { -1, -1, -2, -3, -1,  0, -2, -3, -2,  1,  2, -1,  5,  0, -2, -1, -1, -1, -1,  1, -3, -1, -1, -4 },
/*F*/ { -2, -3, -3, -3, -2, -3, -3, -3, -1,  0,  0, -3,  0,  6, -4, -2, -2,  1,  3, -1, -3, -3, -1, -4 },
/*P*/ { -1, -2, -2, -1, -3, -1, -1, -2, -2, -3, -3, -1, -2, -4,  7, -1, -1, -4, -3, -2, -2, -1, -2, -4 },
/*S*/ {  1, -1,  1,  0, -1,  0,  0,  0, -1, -2, -2,  0, -1, -2, -1,  4,  1, -3, -2, -2,  0,  0,  0, -4 },
/*T*/ {  0, -1,  0, -1, -1, -1, -1, -2, -2, -1, -1, -1, -1, -2, -1,  1,  5, -2, -2,  0, -1, -1,  0, -4 },
/*W*/ { -3, -3, -4, -4, -2, -2, -3, -2, -2, -3, -2, -3, -1,  1, -4, -3, -2, 11,  2, -3, -4, -3, -2, -4 },
/*Y*/ { -2, -2, -2, -3, -2, -1, -2, -3,  2, -1, -1, -2, -1,  3, -3, -2, -2,  2,  7, -1, -3, -2, -1, -4 },
/*V*/ {  0, -3, -3, -3, -1, -2, -2, -3, -3,  3,  1, -2,  1, -1, -2, -2,  0, -3, -1,  4, -3, -2, -1, -4 },
/*B*/ { -2, -1,  3,  4, -3,  0,  1, -1,  0, -3, -4,  0, -3, -3, -2,  0, -1, -4, -3, -3,  4,  1, -1, -4 },
/*Z*/ { -1,  0,  0,  1, -3,  3,  4, -2,  0, -3, -3,  1, -1, -3, -1,  0, -1, -3, -2, -2,  1,  4, -1, -4 },
/*X*/ {  0, -1, -1, -1, -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2,  0,  0, -2, -1, -1, -1, -1, -1, -4 },
/***/ { -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4,  1 }
};

/* returns BLOSUM score, based values for an aa according to NCBIstdaa */
static Nlm_Int1 CSC_GetBlosumScore(int value1, int value2)
{
    int index1, index2, i, value, index;

    for (i = 0; i < 2; i++) {
        value = (i==0) ? value1 : value2;
        switch (value) { /* map NCBIstdaa "value" onto index in BLOSUM matrix */
            case 1: index = 0; break;
            case 2: index = 20; break;
            case 3: case 24: index = 4; break;
            case 4: index = 3; break;
            case 5: index = 6; break;
            case 6: index = 13; break;
            case 7: index = 7; break;
            case 8: index = 8; break;
            case 9: index = 9; break;
            case 10: index = 11; break;
            case 11: index = 10; break;
            case 12: index = 12; break;
            case 13: index = 2; break;
            case 14: index = 14; break;
            case 15: index = 5; break;
            case 16: index = 1; break;
            case 17: index = 15; break;
            case 18: index = 16; break;
            case 19: index = 19; break;
            case 20: index = 17; break;
            case 21: index = 22; break;
            case 22: index = 18; break;
            case 23: index = 21; break;
            default: index = -1; break;
        }
        if (i==0) index1 = index; else index2 = index;
    }
    if (index1 < 0 || index2 < 0)
        return -4;
    else
        return CSC_Blosum62Matrix[index1][index2];
}

/* ramp depending on sum of pairwise BLOSUM scores */
static void CSC_Algorithm_ByBlosum62(void)
{
    static int *score = NULL, vSize = -1;
    ACTProfilePtr app;
    int c, pr1, pr2, offset, maxScore, minScore, nRows, i = 0, sum, nGaps;
    double scale;
    FloatHi freqSum;

    if (CSC_CurrentProfile->nuc) {
        Message(MSG_POST, "Can't do BLOSUM coloring on nucleotides!\nWill do \"Variety\" coloring instead.");
        CSC_Algorithm_ShowVariety();
        return;
    }

    /* static cache for score totals */
    if (vSize != CSC_CurrentNColumns) {
        if (score) MemFree(score);
        score = (int *) MemNew(CSC_CurrentNColumns * sizeof(int));
        if (!score) return;
        vSize = CSC_CurrentNColumns;
    }

    nRows = AlnMgrGetNumRows(CSC_CurrentSeqAlign);

    /* first determine column sums, min and max */
    for (app = CSC_CurrentProfile; app; app = app->next) {
        for (c = 0; c < app->len; c++) {
            sum = 0;
            freqSum = 0.0;
            
            /* count all "identity" pair scores */
            for (pr1 = 1; pr1 < 26; pr1++) {
                sum += ((int) (app->freq[pr1][c] * (app->freq[pr1][c] - 1) / 2 + 0.001))
                    * CSC_GetBlosumScore(pr1, pr1);
                freqSum += app->freq[pr1][c];
            }

            /* then count all non-identical pairs */
            for (pr1 = 1; pr1 < 26 - 1; pr1++) {
                for (pr2 = pr1 + 1; pr2 < 26; pr2++) {
                    sum += ((int) (app->freq[pr1][c] * app->freq[pr2][c] + 0.001))
                        * CSC_GetBlosumScore(pr1, pr2);
                }
            }

            /* then count all pairs with (and amongst) gaps */
            nGaps = nRows - app->numseq;
            nGaps += app->numseq - ((int) (freqSum + 0.001));
            sum += (nGaps * (nGaps - 1) / 2 + nGaps * (nRows - nGaps))
                * CSC_GetBlosumScore(-1, -1);

            score[i] = sum;
            if (i == 0 || score[i] > maxScore)
                maxScore = score[i];
            if (i == 0 || score[i] < minScore)
                minScore = score[i];
            i++;
        }
    }

    /* then color according to score in each column, normalized by max score */
    for (i = 0; i < CSC_CurrentNColumns; i++) {
        if (minScore == maxScore)
            scale = 0.0;
        else
            /* scale [minScore..maxScore] onto [1..0] */
            scale = 1.0 - 1.0 * (score[i] - minScore) / (maxScore - minScore);

        offset = 3 * i;
        CSC_CurrentColors[offset] = (Nlm_Uint1)
            (scale * (CSC_ZeroIdentityColor[0] - CSC_FullIdentityColor[0])
                + CSC_FullIdentityColor[0]);
        CSC_CurrentColors[offset+1] = (Nlm_Uint1)
            (scale * (CSC_ZeroIdentityColor[1] - CSC_FullIdentityColor[1])
                + CSC_FullIdentityColor[1]);
        CSC_CurrentColors[offset+2] = (Nlm_Uint1)
            (scale * (CSC_ZeroIdentityColor[2] - CSC_FullIdentityColor[2])
                + CSC_FullIdentityColor[2]);
    }
}

/* simply one of two colors, for 100% column identity or not */
static void CSC_Algorithm_ShowIdentity(void)
{
    ACTProfilePtr app = CSC_CurrentProfile;
    int c, pr, offset = 0;
    int nRows = AlnMgrGetNumRows(CSC_CurrentSeqAlign);
    
    for (; app; app = app->next) {
        for (c = 0; c < app->len; offset += 3, c++) {
            for (pr = 1; pr < CSC_CurrentProfileRows; pr++) {
                if (((int) (app->freq[pr][c] + 0.001)) == 0) continue;
                else if (((int) (app->freq[pr][c] + 0.001)) == nRows) {
                    CSC_CurrentColors[offset] = CSC_FullIdentityColor[0];
                    CSC_CurrentColors[offset+1] = CSC_FullIdentityColor[1];
                    CSC_CurrentColors[offset+2] = CSC_FullIdentityColor[2];
                } else {
                    CSC_CurrentColors[offset] = CSC_ZeroIdentityColor[0];
                    CSC_CurrentColors[offset+1] = CSC_ZeroIdentityColor[1];
                    CSC_CurrentColors[offset+2] = CSC_ZeroIdentityColor[2];
                }
                break;
            }
        }
    }
}

/* simply color all aligned residues a single color */
static void CSC_Algorithm_ShowAligned(void)
{
    int c, offset = 0;

    for (c = 0; c < CSC_CurrentNColumns; c++) {
        CSC_CurrentColors[offset] = CSC_FullIdentityColor[0];
        CSC_CurrentColors[offset+1] = CSC_FullIdentityColor[1];
        CSC_CurrentColors[offset+2] = CSC_FullIdentityColor[2];
        offset += 3;
    }
}


/**** to manage which algorithm is used ****/

typedef struct _CSC_AlgorithmFuncListItem {
    Nlm_Int2 type;
    Nlm_CharPtr name;
    CSC_AlgorithmFunc function;
} CSC_AlgorithmFuncListItem;

/* must be in the same order as the enum in seqcons.h */
static CSC_AlgorithmFuncListItem CSC_AlgorithmFuncList[] = {
    { CSC_BYVARIETY, "Variety", CSC_Algorithm_ShowVariety },
    { CSC_BYBLOSUM62, "Weighted Variety", CSC_Algorithm_ByBlosum62 },
    { CSC_SHOWIDENTITY, "Identity", CSC_Algorithm_ShowIdentity },
    { CSC_SHOWALIGNED, "Aligned", CSC_Algorithm_ShowAligned }
};

static Nlm_Int2 CSC_CurrentAlgorithm = -1;

const Nlm_Char * CSC_GetAlgorithmName(Nlm_Int2 which)
{
    if (which >= 0 && which < CSC_NUMALGORITHMS)
        return CSC_AlgorithmFuncList[which].name;
    ERR_RETURN_VAL("invalid algorithm identity", NULL);
}


/**** functions to calculate and access column colors ****/

/* calculate and store column colors for an alignment */
Nlm_Boolean CSC_CalculateColumnColors(SeqAlignPtr salp, Nlm_Int2 useAlgorithm)
{
    ACTProfilePtr app;
    int len;
    
    /* if passed a null pointer, then clear the current stores */
    if (!salp) {
        if (CSC_CurrentColors) MemFree(CSC_CurrentColors);
        if (CSC_CurrentProfile) ACT_ProfileSetFree(CSC_CurrentProfile);
        CSC_CurrentColors = NULL;
        CSC_CurrentSeqAlign = NULL;
        CSC_CurrentProfile = NULL;
        CSC_CurrentNColumns = 0;
        CSC_CurrentAlgorithm = -1;
        return FALSE;
    }

    CSC_CurrentAlgorithm = useAlgorithm;
    CSC_CurrentSeqAlign = salp;

    /* calculate and save alignment profile */
    if (!AlnMgrIndexSeqAlign(salp))
        ERR_RETURN_VAL("problem indexing SeqAlign", FALSE);
    if (AlnMgrGetAlnLength(salp, FALSE) <= 0)
        return TRUE;
    if (!(app = ACT_MakeProfileFromSA(salp)))
        ERR_RETURN_VAL("problem generating sequence profile", FALSE);
    if (CSC_CurrentProfile) ACT_ProfileSetFree(CSC_CurrentProfile);
    CSC_CurrentProfile = app;

    /* allocate/free new color storage as necessary */
    for (len = 0, app = CSC_CurrentProfile; app; app = app->next) len += app->len;
    if (len != AlnMgrGetAlnLength(salp, FALSE)) /* sanity check */
        ERR_RETURN_VAL("length mismatch between alignment and profile", FALSE);
    if (len != CSC_CurrentNColumns) {
        if (CSC_CurrentColors)
            MemFree(CSC_CurrentColors);
        CSC_CurrentColors = (Nlm_Uchar *) MemNew(3 * len * sizeof(Nlm_Uchar));
        if (!CSC_CurrentColors)
            ERR_RETURN_VAL("out of memory", FALSE);
        CSC_CurrentNColumns = len;
    }

    /* do the appropriate calculation */
    CSC_CurrentProfileRows = CSC_CurrentProfile->nuc ? 5 : 25;
    (*CSC_AlgorithmFuncList[CSC_CurrentAlgorithm].function)();

    return TRUE;
}


/* determine whether a (view) row has a structure */
static Nlm_Boolean CSC_DoesRowHaveStructure(Nlm_Int4 row, PMSD pmsd)
{
    PDNMM pdnmm;
    PDNMS pdnms;

    /* convert viewed row that's passed into target row to search MMD's*/
    row = ViewMgr_VRow2TRow(CSC_CurrentSeqAlign, row);

    /* check master */
    pdnmm = pmsd->pdnmmHead;
    while (pdnmm) {
        if (((PMMD) pdnmm->data.ptrvalue)->iTargetRow == row)
            return TRUE;
        pdnmm = pdnmm->next;
    }

    /* go through the slave structures */
    pdnms = pmsd->pdnmsSlaves;
    while (pdnms) {
        pdnmm = ((PMSD) pdnms->data.ptrvalue)->pdnmmHead;
        while (pdnmm) {
            if (((PMMD) pdnmm->data.ptrvalue)->iTargetRow == row)
                return TRUE;
            pdnmm = pdnmm->next;
        }
        pdnms = pdnms->next;
    }

    return FALSE;
}

/* color a row */
void CSC_SetDDVRowCells(Nlm_Int4 row, Nlm_Boolean applyColumnColor, 
                        Nlm_Uint1 *unalignedColor, DDV_ColorGlobal *pDDVCG)
{
    BioseqPtr bsp;
    int seqLoc;
    DDV_ColorCell *pColorCell;
    Nlm_Int4 alignLoc, residueColorInt;
    Nlm_Uint1 residueColor[3];

    if (row <= 0 || !pDDVCG || !CSC_CurrentSeqAlign)
        return;

    bsp = BioseqLockById(AlnMgrGetNthSeqIdPtr(CSC_CurrentSeqAlign, row));
    if (!bsp) return;

    for (seqLoc = 0; seqLoc < bsp->length; seqLoc++) {

        pColorCell = DDV_GetColor(pDDVCG, NULL, row, seqLoc);
        if (pColorCell == NULL) return;

        alignLoc = AlnMgrMapBioseqToSeqAlign(CSC_CurrentSeqAlign, seqLoc, row, NULL);
        if (alignLoc >= 0 && applyColumnColor) {
            residueColorInt = CSC_GetColumnColorByRow(row, seqLoc);
            residueColor[0] = (residueColorInt >> 16) & 0xFF;
            residueColor[1] = (residueColorInt >> 8) & 0xFF;
            residueColor[2] = residueColorInt & 0xFF;
            DDV_SetColorInCell(pColorCell, residueColor);
        } else {
            DDV_SetColorInCell(pColorCell, unalignedColor);
        }

        DDV_RequestColor(&(pDDVCG->Palette), pColorCell);
        pColorCell->LowerCase = (alignLoc < 0);
        DDV_SetColor(pDDVCG, NULL, row, seqLoc, pColorCell);
    }

    BioseqUnlock(bsp);
}

/* color non-structure rows */
void CSC_SetNonStructureDDVRowCells(PMSD pmsd, Nlm_Boolean applyColumnColor, 
                                    Nlm_Uint1 *unalignedColor, DDV_ColorGlobal *pDDVCG)
{
    int row, nRows;

    if (!pmsd || !pDDVCG || !CSC_CurrentSeqAlign)
        return;

    nRows = AlnMgrGetNumRows(CSC_CurrentSeqAlign);
    for (row = 1; row <= nRows; row++) {
        if (!CSC_DoesRowHaveStructure(row, pmsd))
            CSC_SetDDVRowCells(row, applyColumnColor, unalignedColor, pDDVCG);
    }
}

/* do all rows */
void CSC_SetAllDDVRowCells(Nlm_Boolean applyColumnColor, Nlm_Uint1 *unalignedColor,
                           DDV_ColorGlobal *pDDVCG)
{
    int row, nRows;

    if (!pDDVCG || !CSC_CurrentSeqAlign)
        return;

    nRows = AlnMgrGetNumRows(CSC_CurrentSeqAlign);
    for (row = 1; row <= nRows; row++)
        CSC_SetDDVRowCells(row, applyColumnColor, unalignedColor, pDDVCG);
}
