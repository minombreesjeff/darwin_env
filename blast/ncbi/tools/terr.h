#ifndef __MODULE_toasn3__
#define __MODULE_toasn3__

#define ERR_ORGANISM  1,0
#define ERR_ORGANISM_Empty  1,1
#define ERR_ORGANISM_NotFound  1,2
#define ERR_ORGANISM_Diff  1,3
#define ERR_SOURCE  2,0
#define ERR_SOURCE_MultipleQualifiers  2,1
#define ERR_SOURCE_DiffQualifiers  2,2
#define ERR_SOURCE_Identical  2,3
#define ERR_SOURCE_NotFound  2,4
#define ERR_SOURCE_GeneticCode  2,5
#define ERR_SOURCE_UnwantedQualifiers  2,6
#define ERR_SOURCE_MissingOrganism  2,7
#define ERR_SOURCE_NotFoundWHole  2,9
#define ERR_SOURCE_Multiple  2,10
#define ERR_SOURCE_Diff  2,11
#define ERR_SOURCE_QualDiffValues  2,12
#define ERR_SOURCE_IllegalQual  2,13
#define ERR_SOURCE_QualUnknown  2,14
#define ERR_TAXONOMY  4,0
#define ERR_TAXONOMY_GeneticCode  4,1
#define ERR_FEATURE  5,0
#define ERR_FEATURE_CannotMapDnaLocToAALoc  5,1
#define ERR_FEATURE_BadLocation  5,2
#define ERR_FEATURE_CDSNotFound  5,3

#endif
