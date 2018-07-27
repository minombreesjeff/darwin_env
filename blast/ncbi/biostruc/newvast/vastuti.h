#ifndef _VASTUTI_
#define _VASTUTI_

#include <ncbi.h>
#include <accentr.h>
#include <netentr.h>
#include <ncbiwww.h>
#include <sys/resource.h>
#include <asn.h>
#include <accutils.h>
#include <mmdbapi.h>
#include "vastlocl.h"
#include "mmdblocl.h"
#include "mmdbdata.h"
#include "vastgenDB.h"
#include <objcdd.h>


void loadSubsetInfo();

CharPtr GetSubsetName_DB(Int4 subsetId);

void WWWPrintFileData(CharPtr FName,  FILE *pFile);

BiostrucAnnotSetPtr LocalGetBiostrucAnnotSet(Int4 mmdbid, CharPtr JobID);

BiostrucAnnotSetPtr LocalGetFeatureSet(Int4 mmdbid, Int4 feature_set_id, CharPtr JobID);

BiostrucAnnotSetPtr BiostrucAnnotSetGetByFid(BiostrucAnnotSetPtr basp, Int4 feature_id, Int4 feature_set_id);

Int2 Check_VastSearch_Password(CharPtr pcPassNew, CharPtr JobID);

BiostrucAnnotSetPtr PruneBiostrucAnnotHits(BiostrucAnnotSetPtr basp, Int4 FSID, ValNodePtr pvnFids);

void MakeNbrList(CharPtr nbrString,  CharPtr *(*SelNames), Int4Ptr *SelSds,
	Int4 *iCount, Int4 ischar);

void 
OrderCopyVpp(VastPageDataPtr vpptmp, VastPageDataPtr vpp, Int4 iKept, Int4Ptr KepBsfId);


#endif
