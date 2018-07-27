#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <strimprt.h>
#include <objmmdb3.h>

static Boolean loaded = FALSE;

#include <mmdb3.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
objmmdb3AsnLoad(void)
{

   if ( ! loaded) {
      NLM_EXTERN_LOADS

      if ( ! AsnLoad ())
      return FALSE;
      loaded = TRUE;
   }

   return TRUE;
}



/**************************************************
*    Generated object loaders for Module MMDB-Features
*    Generated using ASNCODE Revision: 6.8 at Mar 6, 2000  9:42 AM
*
**************************************************/


/**************************************************
*
*    BiostrucFeatureSetNew()
*
**************************************************/
NLM_EXTERN 
BiostrucFeatureSetPtr LIBCALL
BiostrucFeatureSetNew(void)
{
   BiostrucFeatureSetPtr ptr = MemNew((size_t) sizeof(BiostrucFeatureSet));

   return ptr;

}


/**************************************************
*
*    BiostrucFeatureSetFree()
*
**************************************************/
NLM_EXTERN 
BiostrucFeatureSetPtr LIBCALL
BiostrucFeatureSetFree(BiostrucFeatureSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr -> descr, (AsnOptFreeFunc) BiostrucFeatureSetDescrFree);
   AsnGenericUserSeqOfFree(ptr -> features, (AsnOptFreeFunc) BiostrucFeatureFree);
   return MemFree(ptr);
}


/**************************************************
*
*    BiostrucFeatureSetAsnRead()
*
**************************************************/
NLM_EXTERN 
BiostrucFeatureSetPtr LIBCALL
BiostrucFeatureSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BiostrucFeatureSetPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BiostrucFeatureSet ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC_FEATURE_SET);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC_FEATURE_SET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BiostrucFeatureSetNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BIOSTRUC_FEATURE_SET_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> id = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_FEATURE_SET_descr) {
      ptr -> descr = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BiostrucFeatureSetDescrAsnRead, (AsnOptFreeFunc) BiostrucFeatureSetDescrFree);
      if (isError && ptr -> descr == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_FEATURE_SET_features) {
      ptr -> features = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BiostrucFeatureAsnRead, (AsnOptFreeFunc) BiostrucFeatureFree);
      if (isError && ptr -> features == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = BiostrucFeatureSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    BiostrucFeatureSetAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BiostrucFeatureSetAsnWrite(BiostrucFeatureSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BIOSTRUC_FEATURE_SET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> id;
   retval = AsnWrite(aip, BIOSTRUC_FEATURE_SET_id,  &av);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> descr, (AsnWriteFunc) BiostrucFeatureSetDescrAsnWrite, aip, BIOSTRUC_FEATURE_SET_descr, BIOSTRUC_FEATURE_SET_descr_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> features, (AsnWriteFunc) BiostrucFeatureAsnWrite, aip, BIOSTRUC_FEATURE_SET_features, BIOSTRUC_FEATURE_SET_features_E);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    ChemGraphPntrsFree()
*
**************************************************/
NLM_EXTERN 
ChemGraphPntrsPtr LIBCALL
ChemGraphPntrsFree(ValNodePtr anp)
{
   Pointer pnt;

   if (anp == NULL) {
      return NULL;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   default:
      break;
   case ChemGraphPntrs_atoms:
      AtomPntrsFree(anp -> data.ptrvalue);
      break;
   case ChemGraphPntrs_residues:
      ResiduePntrsFree(anp -> data.ptrvalue);
      break;
   case ChemGraphPntrs_molecules:
      MoleculePntrsFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    ChemGraphPntrsAsnRead()
*
**************************************************/
NLM_EXTERN 
ChemGraphPntrsPtr LIBCALL
ChemGraphPntrsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   ValNodePtr anp;
   Uint1 choice;
   Boolean isError = FALSE;
   Boolean nullIsError = FALSE;
   AsnReadFunc func;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ChemGraphPntrs ::= (self contained) */
      atp = AsnReadId(aip, amp, CHEM_GRAPH_PNTRS);
   } else {
      atp = AsnLinkType(orig, CHEM_GRAPH_PNTRS);    /* link in local tree */
   }
   if (atp == NULL) {
      return NULL;
   }

   anp = ValNodeNew(NULL);
   if (anp == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the CHOICE or OpenStruct value (nothing) */
      goto erret;
   }

   func = NULL;

   atp = AsnReadId(aip, amp, atp);  /* find the choice */
   if (atp == NULL) {
      goto erret;
   }
   if (atp == CHEM_GRAPH_PNTRS_atoms) {
      choice = ChemGraphPntrs_atoms;
      func = (AsnReadFunc) AtomPntrsAsnRead;
   }
   else if (atp == CHEM_GRAPH_PNTRS_residues) {
      choice = ChemGraphPntrs_residues;
      func = (AsnReadFunc) ResiduePntrsAsnRead;
   }
   else if (atp == CHEM_GRAPH_PNTRS_molecules) {
      choice = ChemGraphPntrs_molecules;
      func = (AsnReadFunc) MoleculePntrsAsnRead;
   }
   anp->choice = choice;
   if (func != NULL)
   {
      anp->data.ptrvalue = (* func)(aip, atp);
      if (aip -> io_failure) goto erret;

      if (nullIsError && anp->data.ptrvalue == NULL) {
         goto erret;
      }
   }

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return anp;

erret:
   anp = MemFree(anp);
   aip -> io_failure = TRUE;
   goto ret;
}


/**************************************************
*
*    ChemGraphPntrsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ChemGraphPntrsAsnWrite(ChemGraphPntrsPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, CHEM_GRAPH_PNTRS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

   av.ptrvalue = (Pointer)anp;
   if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, &av)) {
      goto erret;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   case ChemGraphPntrs_atoms:
      writetype = CHEM_GRAPH_PNTRS_atoms;
      func = (AsnWriteFunc) AtomPntrsAsnWrite;
      break;
   case ChemGraphPntrs_residues:
      writetype = CHEM_GRAPH_PNTRS_residues;
      func = (AsnWriteFunc) ResiduePntrsAsnWrite;
      break;
   case ChemGraphPntrs_molecules:
      writetype = CHEM_GRAPH_PNTRS_molecules;
      func = (AsnWriteFunc) MoleculePntrsAsnWrite;
      break;
   }
   if (writetype != NULL) {
      retval = (* func)(pnt, aip, writetype);   /* write it out */
   }
   if (!retval) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}


/**************************************************
*
*    AtomPntrsNew()
*
**************************************************/
NLM_EXTERN 
AtomPntrsPtr LIBCALL
AtomPntrsNew(void)
{
   AtomPntrsPtr ptr = MemNew((size_t) sizeof(AtomPntrs));

   return ptr;

}


/**************************************************
*
*    AtomPntrsFree()
*
**************************************************/
NLM_EXTERN 
AtomPntrsPtr LIBCALL
AtomPntrsFree(AtomPntrsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericBaseSeqOfFree(ptr -> molecule_ids ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> residue_ids ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> atom_ids ,ASNCODE_INTVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    AtomPntrsAsnRead()
*
**************************************************/
NLM_EXTERN 
AtomPntrsPtr LIBCALL
AtomPntrsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AtomPntrsPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AtomPntrs ::= (self contained) */
      atp = AsnReadId(aip, amp, ATOM_PNTRS);
   } else {
      atp = AsnLinkType(orig, ATOM_PNTRS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = AtomPntrsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ATOM_PNTRS_number_of_ptrs) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> number_of_ptrs = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ATOM_PNTRS_molecule_ids) {
      ptr -> molecule_ids = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> molecule_ids == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ATOM_PNTRS_residue_ids) {
      ptr -> residue_ids = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> residue_ids == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ATOM_PNTRS_atom_ids) {
      ptr -> atom_ids = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> atom_ids == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = AtomPntrsFree(ptr);
   goto ret;
}



/**************************************************
*
*    AtomPntrsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AtomPntrsAsnWrite(AtomPntrsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ATOM_PNTRS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> number_of_ptrs;
   retval = AsnWrite(aip, ATOM_PNTRS_number_of_ptrs,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> molecule_ids ,ASNCODE_INTVAL_SLOT, aip, ATOM_PNTRS_molecule_ids, ATOM_PNTRS_molecule_ids_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> residue_ids ,ASNCODE_INTVAL_SLOT, aip, ATOM_PNTRS_residue_ids, ATOM_PNTRS_residue_ids_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> atom_ids ,ASNCODE_INTVAL_SLOT, aip, ATOM_PNTRS_atom_ids, ATOM_PNTRS_atom_ids_E);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    ChemGraphAlignmentNew()
*
**************************************************/
NLM_EXTERN 
ChemGraphAlignmentPtr LIBCALL
ChemGraphAlignmentNew(void)
{
   ChemGraphAlignmentPtr ptr = MemNew((size_t) sizeof(ChemGraphAlignment));

   ptr -> dimension = 2;
   return ptr;

}


/**************************************************
*
*    ChemGraphAlignmentFree()
*
**************************************************/
NLM_EXTERN 
ChemGraphAlignmentPtr LIBCALL
ChemGraphAlignmentFree(ChemGraphAlignmentPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr -> biostruc_ids, (AsnOptFreeFunc) BiostrucIdFree);
   AsnGenericChoiceSeqOfFree(ptr -> alignment, (AsnOptFreeFunc) ChemGraphPntrsFree);
   AsnGenericChoiceSeqOfFree(ptr -> domain, (AsnOptFreeFunc) ChemGraphPntrsFree);
   AsnGenericUserSeqOfFree(ptr -> transform, (AsnOptFreeFunc) TransformFree);
   AsnGenericUserSeqOfFree(ptr -> aligndata, (AsnOptFreeFunc) AlignStatsFree);
   return MemFree(ptr);
}


/**************************************************
*
*    ChemGraphAlignmentAsnRead()
*
**************************************************/
NLM_EXTERN 
ChemGraphAlignmentPtr LIBCALL
ChemGraphAlignmentAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ChemGraphAlignmentPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ChemGraphAlignment ::= (self contained) */
      atp = AsnReadId(aip, amp, CHEM_GRAPH_ALIGNMENT);
   } else {
      atp = AsnLinkType(orig, CHEM_GRAPH_ALIGNMENT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ChemGraphAlignmentNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CHEM_GRAPH_ALIGNMENT_dimension) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> dimension = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CHEM_GRAPH_ALIGNMENT_biostruc_ids) {
      ptr -> biostruc_ids = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BiostrucIdAsnRead, (AsnOptFreeFunc) BiostrucIdFree);
      if (isError && ptr -> biostruc_ids == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CHEM_GRAPH_ALIGNMENT_alignment) {
      ptr -> alignment = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ChemGraphPntrsAsnRead, (AsnOptFreeFunc) ChemGraphPntrsFree);
      if (isError && ptr -> alignment == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CHEM_GRAPH_ALIGNMENT_domain) {
      ptr -> domain = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ChemGraphPntrsAsnRead, (AsnOptFreeFunc) ChemGraphPntrsFree);
      if (isError && ptr -> domain == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CHEM_GRAPH_ALIGNMENT_transform) {
      ptr -> transform = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) TransformAsnRead, (AsnOptFreeFunc) TransformFree);
      if (isError && ptr -> transform == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CHEM_GRAPH_ALIGNMENT_aligndata) {
      ptr -> aligndata = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) AlignStatsAsnRead, (AsnOptFreeFunc) AlignStatsFree);
      if (isError && ptr -> aligndata == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = ChemGraphAlignmentFree(ptr);
   goto ret;
}



/**************************************************
*
*    ChemGraphAlignmentAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ChemGraphAlignmentAsnWrite(ChemGraphAlignmentPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CHEM_GRAPH_ALIGNMENT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> dimension;
   retval = AsnWrite(aip, CHEM_GRAPH_ALIGNMENT_dimension,  &av);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> biostruc_ids, (AsnWriteFunc) BiostrucIdAsnWrite, aip, CHEM_GRAPH_ALIGNMENT_biostruc_ids, CHEM_GRAPH_ALIGNMENT_biostruc_ids_E);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> alignment, (AsnWriteFunc) ChemGraphPntrsAsnWrite, aip, CHEM_GRAPH_ALIGNMENT_alignment, CHEM_GRAPH_ALIGNMENT_alignment_E);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> domain, (AsnWriteFunc) ChemGraphPntrsAsnWrite, aip, CHEM_GRAPH_ALIGNMENT_domain, CHEM_GRAPH_ALIGNMENT_domain_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> transform, (AsnWriteFunc) TransformAsnWrite, aip, CHEM_GRAPH_ALIGNMENT_transform, CHEM_GRAPH_ALIGNMENT_transform_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> aligndata, (AsnWriteFunc) AlignStatsAsnWrite, aip, CHEM_GRAPH_ALIGNMENT_aligndata, CHEM_GRAPH_ALIGNMENT_aligndata_E);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    SphereNew()
*
**************************************************/
NLM_EXTERN 
SpherePtr LIBCALL
SphereNew(void)
{
   SpherePtr ptr = MemNew((size_t) sizeof(Sphere));

   return ptr;

}


/**************************************************
*
*    SphereFree()
*
**************************************************/
NLM_EXTERN 
SpherePtr LIBCALL
SphereFree(SpherePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ModelSpacePointFree(ptr -> center);
   RealValueFree(ptr -> radius);
   return MemFree(ptr);
}


/**************************************************
*
*    SphereAsnRead()
*
**************************************************/
NLM_EXTERN 
SpherePtr LIBCALL
SphereAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   SpherePtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Sphere ::= (self contained) */
      atp = AsnReadId(aip, amp, SPHERE);
   } else {
      atp = AsnLinkType(orig, SPHERE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = SphereNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == SPHERE_center) {
      ptr -> center = ModelSpacePointAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SPHERE_radius) {
      ptr -> radius = RealValueAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = SphereFree(ptr);
   goto ret;
}



/**************************************************
*
*    SphereAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
SphereAsnWrite(SpherePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, SPHERE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> center != NULL) {
      if ( ! ModelSpacePointAsnWrite(ptr -> center, aip, SPHERE_center)) {
         goto erret;
      }
   }
   if (ptr -> radius != NULL) {
      if ( ! RealValueAsnWrite(ptr -> radius, aip, SPHERE_radius)) {
         goto erret;
      }
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    ConeNew()
*
**************************************************/
NLM_EXTERN 
ConePtr LIBCALL
ConeNew(void)
{
   ConePtr ptr = MemNew((size_t) sizeof(Cone));

   return ptr;

}


/**************************************************
*
*    ConeFree()
*
**************************************************/
NLM_EXTERN 
ConePtr LIBCALL
ConeFree(ConePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ModelSpacePointFree(ptr -> axis_top);
   ModelSpacePointFree(ptr -> axis_bottom);
   RealValueFree(ptr -> radius_bottom);
   return MemFree(ptr);
}


/**************************************************
*
*    ConeAsnRead()
*
**************************************************/
NLM_EXTERN 
ConePtr LIBCALL
ConeAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ConePtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cone ::= (self contained) */
      atp = AsnReadId(aip, amp, CONE);
   } else {
      atp = AsnLinkType(orig, CONE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ConeNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CONE_axis_top) {
      ptr -> axis_top = ModelSpacePointAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONE_axis_bottom) {
      ptr -> axis_bottom = ModelSpacePointAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONE_radius_bottom) {
      ptr -> radius_bottom = RealValueAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = ConeFree(ptr);
   goto ret;
}



/**************************************************
*
*    ConeAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ConeAsnWrite(ConePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CONE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> axis_top != NULL) {
      if ( ! ModelSpacePointAsnWrite(ptr -> axis_top, aip, CONE_axis_top)) {
         goto erret;
      }
   }
   if (ptr -> axis_bottom != NULL) {
      if ( ! ModelSpacePointAsnWrite(ptr -> axis_bottom, aip, CONE_axis_bottom)) {
         goto erret;
      }
   }
   if (ptr -> radius_bottom != NULL) {
      if ( ! RealValueAsnWrite(ptr -> radius_bottom, aip, CONE_radius_bottom)) {
         goto erret;
      }
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    CylinderNew()
*
**************************************************/
NLM_EXTERN 
CylinderPtr LIBCALL
CylinderNew(void)
{
   CylinderPtr ptr = MemNew((size_t) sizeof(Cylinder));

   return ptr;

}


/**************************************************
*
*    CylinderFree()
*
**************************************************/
NLM_EXTERN 
CylinderPtr LIBCALL
CylinderFree(CylinderPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ModelSpacePointFree(ptr -> axis_top);
   ModelSpacePointFree(ptr -> axis_bottom);
   RealValueFree(ptr -> radius);
   return MemFree(ptr);
}


/**************************************************
*
*    CylinderAsnRead()
*
**************************************************/
NLM_EXTERN 
CylinderPtr LIBCALL
CylinderAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CylinderPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cylinder ::= (self contained) */
      atp = AsnReadId(aip, amp, CYLINDER);
   } else {
      atp = AsnLinkType(orig, CYLINDER);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = CylinderNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CYLINDER_axis_top) {
      ptr -> axis_top = ModelSpacePointAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CYLINDER_axis_bottom) {
      ptr -> axis_bottom = ModelSpacePointAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CYLINDER_radius) {
      ptr -> radius = RealValueAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = CylinderFree(ptr);
   goto ret;
}



/**************************************************
*
*    CylinderAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CylinderAsnWrite(CylinderPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CYLINDER);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> axis_top != NULL) {
      if ( ! ModelSpacePointAsnWrite(ptr -> axis_top, aip, CYLINDER_axis_top)) {
         goto erret;
      }
   }
   if (ptr -> axis_bottom != NULL) {
      if ( ! ModelSpacePointAsnWrite(ptr -> axis_bottom, aip, CYLINDER_axis_bottom)) {
         goto erret;
      }
   }
   if (ptr -> radius != NULL) {
      if ( ! RealValueAsnWrite(ptr -> radius, aip, CYLINDER_radius)) {
         goto erret;
      }
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    BrickNew()
*
**************************************************/
NLM_EXTERN 
BrickPtr LIBCALL
BrickNew(void)
{
   BrickPtr ptr = MemNew((size_t) sizeof(Brick));

   return ptr;

}


/**************************************************
*
*    BrickFree()
*
**************************************************/
NLM_EXTERN 
BrickPtr LIBCALL
BrickFree(BrickPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ModelSpacePointFree(ptr -> corner_000);
   ModelSpacePointFree(ptr -> corner_001);
   ModelSpacePointFree(ptr -> corner_010);
   ModelSpacePointFree(ptr -> corner_011);
   ModelSpacePointFree(ptr -> corner_100);
   ModelSpacePointFree(ptr -> corner_101);
   ModelSpacePointFree(ptr -> corner_110);
   ModelSpacePointFree(ptr -> corner_111);
   return MemFree(ptr);
}


/**************************************************
*
*    BrickAsnRead()
*
**************************************************/
NLM_EXTERN 
BrickPtr LIBCALL
BrickAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BrickPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Brick ::= (self contained) */
      atp = AsnReadId(aip, amp, BRICK);
   } else {
      atp = AsnLinkType(orig, BRICK);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BrickNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BRICK_corner_000) {
      ptr -> corner_000 = ModelSpacePointAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BRICK_corner_001) {
      ptr -> corner_001 = ModelSpacePointAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BRICK_corner_010) {
      ptr -> corner_010 = ModelSpacePointAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BRICK_corner_011) {
      ptr -> corner_011 = ModelSpacePointAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BRICK_corner_100) {
      ptr -> corner_100 = ModelSpacePointAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BRICK_corner_101) {
      ptr -> corner_101 = ModelSpacePointAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BRICK_corner_110) {
      ptr -> corner_110 = ModelSpacePointAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BRICK_corner_111) {
      ptr -> corner_111 = ModelSpacePointAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = BrickFree(ptr);
   goto ret;
}



/**************************************************
*
*    BrickAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BrickAsnWrite(BrickPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BRICK);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> corner_000 != NULL) {
      if ( ! ModelSpacePointAsnWrite(ptr -> corner_000, aip, BRICK_corner_000)) {
         goto erret;
      }
   }
   if (ptr -> corner_001 != NULL) {
      if ( ! ModelSpacePointAsnWrite(ptr -> corner_001, aip, BRICK_corner_001)) {
         goto erret;
      }
   }
   if (ptr -> corner_010 != NULL) {
      if ( ! ModelSpacePointAsnWrite(ptr -> corner_010, aip, BRICK_corner_010)) {
         goto erret;
      }
   }
   if (ptr -> corner_011 != NULL) {
      if ( ! ModelSpacePointAsnWrite(ptr -> corner_011, aip, BRICK_corner_011)) {
         goto erret;
      }
   }
   if (ptr -> corner_100 != NULL) {
      if ( ! ModelSpacePointAsnWrite(ptr -> corner_100, aip, BRICK_corner_100)) {
         goto erret;
      }
   }
   if (ptr -> corner_101 != NULL) {
      if ( ! ModelSpacePointAsnWrite(ptr -> corner_101, aip, BRICK_corner_101)) {
         goto erret;
      }
   }
   if (ptr -> corner_110 != NULL) {
      if ( ! ModelSpacePointAsnWrite(ptr -> corner_110, aip, BRICK_corner_110)) {
         goto erret;
      }
   }
   if (ptr -> corner_111 != NULL) {
      if ( ! ModelSpacePointAsnWrite(ptr -> corner_111, aip, BRICK_corner_111)) {
         goto erret;
      }
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    TransformNew()
*
**************************************************/
NLM_EXTERN 
TransformPtr LIBCALL
TransformNew(void)
{
   TransformPtr ptr = MemNew((size_t) sizeof(Transform));

   return ptr;

}


/**************************************************
*
*    TransformFree()
*
**************************************************/
NLM_EXTERN 
TransformPtr LIBCALL
TransformFree(TransformPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr -> moves, (AsnOptFreeFunc) MoveFree);
   return MemFree(ptr);
}


/**************************************************
*
*    TransformAsnRead()
*
**************************************************/
NLM_EXTERN 
TransformPtr LIBCALL
TransformAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   TransformPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Transform ::= (self contained) */
      atp = AsnReadId(aip, amp, TRANSFORM);
   } else {
      atp = AsnLinkType(orig, TRANSFORM);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = TransformNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == TRANSFORM_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> id = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TRANSFORM_moves) {
      ptr -> moves = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MoveAsnRead, (AsnOptFreeFunc) MoveFree);
      if (isError && ptr -> moves == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = TransformFree(ptr);
   goto ret;
}



/**************************************************
*
*    TransformAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
TransformAsnWrite(TransformPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TRANSFORM);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> id;
   retval = AsnWrite(aip, TRANSFORM_id,  &av);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> moves, (AsnWriteFunc) MoveAsnWrite, aip, TRANSFORM_moves, TRANSFORM_moves_E);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    BiostrucFeatureSetDescrFree()
*
**************************************************/
NLM_EXTERN 
BiostrucFeatureSetDescrPtr LIBCALL
BiostrucFeatureSetDescrFree(ValNodePtr anp)
{
   Pointer pnt;

   if (anp == NULL) {
      return NULL;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   default:
      break;
   case BiostrucFeatureSetDescr_name:
      MemFree(anp -> data.ptrvalue);
      break;
   case BiostrucFeatureSetDescr_pdb_comment:
      MemFree(anp -> data.ptrvalue);
      break;
   case BiostrucFeatureSetDescr_other_comment:
      MemFree(anp -> data.ptrvalue);
      break;
   case BiostrucFeatureSetDescr_attribution:
      PubFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    BiostrucFeatureSetDescrAsnRead()
*
**************************************************/
NLM_EXTERN 
BiostrucFeatureSetDescrPtr LIBCALL
BiostrucFeatureSetDescrAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   ValNodePtr anp;
   Uint1 choice;
   Boolean isError = FALSE;
   Boolean nullIsError = FALSE;
   AsnReadFunc func;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BiostrucFeatureSetDescr ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC_FEATURE_SET_DESCR);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC_FEATURE_SET_DESCR);    /* link in local tree */
   }
   if (atp == NULL) {
      return NULL;
   }

   anp = ValNodeNew(NULL);
   if (anp == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the CHOICE or OpenStruct value (nothing) */
      goto erret;
   }

   func = NULL;

   atp = AsnReadId(aip, amp, atp);  /* find the choice */
   if (atp == NULL) {
      goto erret;
   }
   if (atp == BIOSTRUC_FEATURE_SET_DESCR_name) {
      choice = BiostrucFeatureSetDescr_name;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == BIOSTRUC_FEATURE_SET_DESCR_pdb_comment) {
      choice = BiostrucFeatureSetDescr_pdb_comment;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == BIOSTRUC_FEATURE_SET_DESCR_other_comment) {
      choice = BiostrucFeatureSetDescr_other_comment;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == BIOSTRUC_FEATURE_SET_DESCR_attribution) {
      choice = BiostrucFeatureSetDescr_attribution;
      func = (AsnReadFunc) PubAsnRead;
   }
   anp->choice = choice;
   if (func != NULL)
   {
      anp->data.ptrvalue = (* func)(aip, atp);
      if (aip -> io_failure) goto erret;

      if (nullIsError && anp->data.ptrvalue == NULL) {
         goto erret;
      }
   }

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return anp;

erret:
   anp = MemFree(anp);
   aip -> io_failure = TRUE;
   goto ret;
}


/**************************************************
*
*    BiostrucFeatureSetDescrAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BiostrucFeatureSetDescrAsnWrite(BiostrucFeatureSetDescrPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, BIOSTRUC_FEATURE_SET_DESCR);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

   av.ptrvalue = (Pointer)anp;
   if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, &av)) {
      goto erret;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   case BiostrucFeatureSetDescr_name:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, BIOSTRUC_FEATURE_SET_DESCR_name, &av);
      break;
   case BiostrucFeatureSetDescr_pdb_comment:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, BIOSTRUC_FEATURE_SET_DESCR_pdb_comment, &av);
      break;
   case BiostrucFeatureSetDescr_other_comment:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, BIOSTRUC_FEATURE_SET_DESCR_other_comment, &av);
      break;
   case BiostrucFeatureSetDescr_attribution:
      writetype = BIOSTRUC_FEATURE_SET_DESCR_attribution;
      func = (AsnWriteFunc) PubAsnWrite;
      break;
   }
   if (writetype != NULL) {
      retval = (* func)(pnt, aip, writetype);   /* write it out */
   }
   if (!retval) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}


/**************************************************
*
*    BiostrucFeatureNew()
*
**************************************************/
NLM_EXTERN 
BiostrucFeaturePtr LIBCALL
BiostrucFeatureNew(void)
{
   BiostrucFeaturePtr ptr = MemNew((size_t) sizeof(BiostrucFeature));

   return ptr;

}


/**************************************************
*
*    BiostrucFeatureFree()
*
**************************************************/
NLM_EXTERN 
BiostrucFeaturePtr LIBCALL
BiostrucFeatureFree(BiostrucFeaturePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   Property_propertyFree(ptr -> Property_property);
   Location_locationFree(ptr -> Location_location);
   return MemFree(ptr);
}


/**************************************************
*
*    Location_locationFree()
*
**************************************************/
static 
Location_locationPtr LIBCALL
Location_locationFree(ValNodePtr anp)
{
   Pointer pnt;

   if (anp == NULL) {
      return NULL;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   default:
      break;
   case Location_location_subgraph:
      ChemGraphPntrsFree(anp -> data.ptrvalue);
      break;
   case Location_location_region:
      RegionPntrsFree(anp -> data.ptrvalue);
      break;
   case Location_location_alignment:
      ChemGraphAlignmentFree(anp -> data.ptrvalue);
      break;
   case Location_location_similarity:
      RegionSimilarityFree(anp -> data.ptrvalue);
      break;
   case Location_location_indirect:
      OtherFeatureFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    BiostrucFeatureAsnRead()
*
**************************************************/
NLM_EXTERN 
BiostrucFeaturePtr LIBCALL
BiostrucFeatureAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BiostrucFeaturePtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BiostrucFeature ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC_FEATURE);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC_FEATURE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BiostrucFeatureNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BIOSTRUC_FEATURE_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> id = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_FEATURE_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_FEATURE_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      ptr -> OBbits__ |= 1<<1;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_FEATURE_property) {
      ptr -> Property_property = Property_propertyAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_FEATURE_location) {
      ptr -> Location_location = Location_locationAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = BiostrucFeatureFree(ptr);
   goto ret;
}



/**************************************************
*
*    Location_locationAsnRead()
*
**************************************************/
static 
Location_locationPtr LIBCALL
Location_locationAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   ValNodePtr anp;
   Uint1 choice;
   Boolean isError = FALSE;
   Boolean nullIsError = FALSE;
   AsnReadFunc func;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Location_location ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC_FEATURE_location);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC_FEATURE_location);    /* link in local tree */
   }
   if (atp == NULL) {
      return NULL;
   }

   anp = ValNodeNew(NULL);
   if (anp == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the CHOICE or OpenStruct value (nothing) */
      goto erret;
   }

   func = NULL;

   atp = AsnReadId(aip, amp, atp);  /* find the choice */
   if (atp == NULL) {
      goto erret;
   }
   if (atp == BIOSTRUC_FEATURE_location_subgraph) {
      choice = Location_location_subgraph;
      func = (AsnReadFunc) ChemGraphPntrsAsnRead;
   }
   else if (atp == BIOSTRUC_FEATURE_location_region) {
      choice = Location_location_region;
      func = (AsnReadFunc) RegionPntrsAsnRead;
   }
   else if (atp == BIOSTRUC_FEATURE_location_alignment) {
      choice = Location_location_alignment;
      func = (AsnReadFunc) ChemGraphAlignmentAsnRead;
   }
   else if (atp == BIOSTRUC_FEATURE_location_similarity) {
      choice = Location_location_similarity;
      func = (AsnReadFunc) RegionSimilarityAsnRead;
   }
   else if (atp == BIOSTRUC_FEATURE_location_indirect) {
      choice = Location_location_indirect;
      func = (AsnReadFunc) OtherFeatureAsnRead;
   }
   anp->choice = choice;
   if (func != NULL)
   {
      anp->data.ptrvalue = (* func)(aip, atp);
      if (aip -> io_failure) goto erret;

      if (nullIsError && anp->data.ptrvalue == NULL) {
         goto erret;
      }
   }

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return anp;

erret:
   anp = MemFree(anp);
   aip -> io_failure = TRUE;
   goto ret;
}


/**************************************************
*
*    BiostrucFeatureAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BiostrucFeatureAsnWrite(BiostrucFeaturePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BIOSTRUC_FEATURE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> id || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> id;
      retval = AsnWrite(aip, BIOSTRUC_FEATURE_id,  &av);
   }
   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, BIOSTRUC_FEATURE_name,  &av);
   }
   if (ptr -> type || (ptr -> OBbits__ & (1<<1) )){   av.intvalue = ptr -> type;
      retval = AsnWrite(aip, BIOSTRUC_FEATURE_type,  &av);
   }
   if (ptr -> Property_property != NULL) {
      if ( ! Property_propertyAsnWrite(ptr -> Property_property, aip, BIOSTRUC_FEATURE_property)) {
         goto erret;
      }
   }
   if (ptr -> Location_location != NULL) {
      if ( ! Location_locationAsnWrite(ptr -> Location_location, aip, BIOSTRUC_FEATURE_location)) {
         goto erret;
      }
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    Location_locationAsnWrite()
*
**************************************************/
static Boolean LIBCALL 
Location_locationAsnWrite(Location_locationPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, BIOSTRUC_FEATURE_location);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

   av.ptrvalue = (Pointer)anp;
   if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, &av)) {
      goto erret;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   case Location_location_subgraph:
      writetype = BIOSTRUC_FEATURE_location_subgraph;
      func = (AsnWriteFunc) ChemGraphPntrsAsnWrite;
      break;
   case Location_location_region:
      writetype = BIOSTRUC_FEATURE_location_region;
      func = (AsnWriteFunc) RegionPntrsAsnWrite;
      break;
   case Location_location_alignment:
      writetype = BIOSTRUC_FEATURE_location_alignment;
      func = (AsnWriteFunc) ChemGraphAlignmentAsnWrite;
      break;
   case Location_location_similarity:
      writetype = BIOSTRUC_FEATURE_location_similarity;
      func = (AsnWriteFunc) RegionSimilarityAsnWrite;
      break;
   case Location_location_indirect:
      writetype = BIOSTRUC_FEATURE_location_indirect;
      func = (AsnWriteFunc) OtherFeatureAsnWrite;
      break;
   }
   if (writetype != NULL) {
      retval = (* func)(pnt, aip, writetype);   /* write it out */
   }
   if (!retval) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}


/**************************************************
*
*    Property_propertyAsnWrite()
*
**************************************************/
static Boolean LIBCALL 
Property_propertyAsnWrite(Property_propertyPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, BIOSTRUC_FEATURE_property);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

   av.ptrvalue = (Pointer)anp;
   if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, &av)) {
      goto erret;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   case Property_property_color:
      writetype = BIOSTRUC_FEATURE_property_color;
      func = (AsnWriteFunc) ColorPropAsnWrite;
      break;
   case Property_property_render:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, BIOSTRUC_FEATURE_property_render, &av);
      break;
   case Property_property_transform:
      writetype = BIOSTRUC_FEATURE_property_transform;
      func = (AsnWriteFunc) TransformAsnWrite;
      break;
   case Property_property_camera:
      writetype = BIOSTRUC_FEATURE_property_camera;
      func = (AsnWriteFunc) CameraAsnWrite;
      break;
   case Property_property_script:
      writetype = BIOSTRUC_FEATURE_property_script;
      func = (AsnWriteFunc) BiostrucScriptAsnWrite;
      break;
   case Property_property_user:
      writetype = BIOSTRUC_FEATURE_property_user;
      func = (AsnWriteFunc) UserObjectAsnWrite;
      break;
   }
   if (writetype != NULL) {
      retval = (* func)(pnt, aip, writetype);   /* write it out */
   }
   if (!retval) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}


/**************************************************
*
*    Property_propertyAsnRead()
*
**************************************************/
static 
Property_propertyPtr LIBCALL
Property_propertyAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   ValNodePtr anp;
   Uint1 choice;
   Boolean isError = FALSE;
   Boolean nullIsError = FALSE;
   AsnReadFunc func;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Property_property ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC_FEATURE_property);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC_FEATURE_property);    /* link in local tree */
   }
   if (atp == NULL) {
      return NULL;
   }

   anp = ValNodeNew(NULL);
   if (anp == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the CHOICE or OpenStruct value (nothing) */
      goto erret;
   }

   func = NULL;

   atp = AsnReadId(aip, amp, atp);  /* find the choice */
   if (atp == NULL) {
      goto erret;
   }
   if (atp == BIOSTRUC_FEATURE_property_color) {
      choice = Property_property_color;
      func = (AsnReadFunc) ColorPropAsnRead;
   }
   else if (atp == BIOSTRUC_FEATURE_property_render) {
      choice = Property_property_render;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == BIOSTRUC_FEATURE_property_transform) {
      choice = Property_property_transform;
      func = (AsnReadFunc) TransformAsnRead;
   }
   else if (atp == BIOSTRUC_FEATURE_property_camera) {
      choice = Property_property_camera;
      func = (AsnReadFunc) CameraAsnRead;
   }
   else if (atp == BIOSTRUC_FEATURE_property_script) {
      choice = Property_property_script;
      func = (AsnReadFunc) BiostrucScriptAsnRead;
   }
   else if (atp == BIOSTRUC_FEATURE_property_user) {
      choice = Property_property_user;
      func = (AsnReadFunc) UserObjectAsnRead;
   }
   anp->choice = choice;
   if (func != NULL)
   {
      anp->data.ptrvalue = (* func)(aip, atp);
      if (aip -> io_failure) goto erret;

      if (nullIsError && anp->data.ptrvalue == NULL) {
         goto erret;
      }
   }

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return anp;

erret:
   anp = MemFree(anp);
   aip -> io_failure = TRUE;
   goto ret;
}


/**************************************************
*
*    Property_propertyFree()
*
**************************************************/
static 
Property_propertyPtr LIBCALL
Property_propertyFree(ValNodePtr anp)
{
   Pointer pnt;

   if (anp == NULL) {
      return NULL;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   default:
      break;
   case Property_property_color:
      ColorPropFree(anp -> data.ptrvalue);
      break;
   case Property_property_transform:
      TransformFree(anp -> data.ptrvalue);
      break;
   case Property_property_camera:
      CameraFree(anp -> data.ptrvalue);
      break;
   case Property_property_script:
      BiostrucScriptFree(anp -> data.ptrvalue);
      break;
   case Property_property_user:
      UserObjectFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    ColorPropNew()
*
**************************************************/
NLM_EXTERN 
ColorPropPtr LIBCALL
ColorPropNew(void)
{
   ColorPropPtr ptr = MemNew((size_t) sizeof(ColorProp));

   return ptr;

}


/**************************************************
*
*    ColorPropFree()
*
**************************************************/
NLM_EXTERN 
ColorPropPtr LIBCALL
ColorPropFree(ColorPropPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   return MemFree(ptr);
}


/**************************************************
*
*    ColorPropAsnRead()
*
**************************************************/
NLM_EXTERN 
ColorPropPtr LIBCALL
ColorPropAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ColorPropPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ColorProp ::= (self contained) */
      atp = AsnReadId(aip, amp, COLOR_PROP);
   } else {
      atp = AsnLinkType(orig, COLOR_PROP);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ColorPropNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == COLOR_PROP_r) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> r = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == COLOR_PROP_g) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> g = av.intvalue;
      ptr -> OBbits__ |= 1<<1;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == COLOR_PROP_b) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> b = av.intvalue;
      ptr -> OBbits__ |= 1<<2;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == COLOR_PROP_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = ColorPropFree(ptr);
   goto ret;
}



/**************************************************
*
*    ColorPropAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ColorPropAsnWrite(ColorPropPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, COLOR_PROP);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> r || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> r;
      retval = AsnWrite(aip, COLOR_PROP_r,  &av);
   }
   if (ptr -> g || (ptr -> OBbits__ & (1<<1) )){   av.intvalue = ptr -> g;
      retval = AsnWrite(aip, COLOR_PROP_g,  &av);
   }
   if (ptr -> b || (ptr -> OBbits__ & (1<<2) )){   av.intvalue = ptr -> b;
      retval = AsnWrite(aip, COLOR_PROP_b,  &av);
   }
   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, COLOR_PROP_name,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    CameraNew()
*
**************************************************/
NLM_EXTERN 
CameraPtr LIBCALL
CameraNew(void)
{
   CameraPtr ptr = MemNew((size_t) sizeof(Camera));

   return ptr;

}


/**************************************************
*
*    CameraFree()
*
**************************************************/
NLM_EXTERN 
CameraPtr LIBCALL
CameraFree(CameraPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   GLMatrixFree(ptr -> modelview);
   return MemFree(ptr);
}


/**************************************************
*
*    CameraAsnRead()
*
**************************************************/
NLM_EXTERN 
CameraPtr LIBCALL
CameraAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CameraPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Camera ::= (self contained) */
      atp = AsnReadId(aip, amp, CAMERA);
   } else {
      atp = AsnLinkType(orig, CAMERA);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = CameraNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CAMERA_x) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> x = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CAMERA_y) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> y = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CAMERA_distance) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> distance = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CAMERA_angle) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> angle = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CAMERA_scale) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> scale = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CAMERA_modelview) {
      ptr -> modelview = GLMatrixAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = CameraFree(ptr);
   goto ret;
}



/**************************************************
*
*    CameraAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CameraAsnWrite(CameraPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CAMERA);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> x;
   retval = AsnWrite(aip, CAMERA_x,  &av);
   av.intvalue = ptr -> y;
   retval = AsnWrite(aip, CAMERA_y,  &av);
   av.intvalue = ptr -> distance;
   retval = AsnWrite(aip, CAMERA_distance,  &av);
   av.intvalue = ptr -> angle;
   retval = AsnWrite(aip, CAMERA_angle,  &av);
   av.intvalue = ptr -> scale;
   retval = AsnWrite(aip, CAMERA_scale,  &av);
   if (ptr -> modelview != NULL) {
      if ( ! GLMatrixAsnWrite(ptr -> modelview, aip, CAMERA_modelview)) {
         goto erret;
      }
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    BiostrucScriptFree()
*
**************************************************/
NLM_EXTERN 
BiostrucScriptPtr LIBCALL
BiostrucScriptFree(BiostrucScriptPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) BiostrucScriptStepFree);
   return NULL;
}


/**************************************************
*
*    BiostrucScriptAsnRead()
*
**************************************************/
NLM_EXTERN 
BiostrucScriptPtr LIBCALL
BiostrucScriptAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BiostrucScriptPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BiostrucScript ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC_SCRIPT);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC_SCRIPT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BiostrucScriptStepAsnRead, (AsnOptFreeFunc) BiostrucScriptStepFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = BiostrucScriptFree(ptr);
   goto ret;
}



/**************************************************
*
*    BiostrucScriptAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BiostrucScriptAsnWrite(BiostrucScriptPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BIOSTRUC_SCRIPT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) BiostrucScriptStepAsnWrite, aip, atp, BIOSTRUC_SCRIPT_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    RegionPntrsNew()
*
**************************************************/
NLM_EXTERN 
RegionPntrsPtr LIBCALL
RegionPntrsNew(void)
{
   RegionPntrsPtr ptr = MemNew((size_t) sizeof(RegionPntrs));

   return ptr;

}


/**************************************************
*
*    RegionPntrsFree()
*
**************************************************/
NLM_EXTERN 
RegionPntrsPtr LIBCALL
RegionPntrsFree(RegionPntrsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   Region_regionFree(ptr -> Region_region);
   return MemFree(ptr);
}


/**************************************************
*
*    Region_regionFree()
*
**************************************************/
static 
Region_regionPtr LIBCALL
Region_regionFree(ValNodePtr anp)
{
   Pointer pnt;

   if (anp == NULL) {
      return NULL;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   default:
      break;
   case Region_region_site:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) RegionCoordinatesFree);
      break;
   case Region_region_boundary:
      AsnGenericChoiceSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) RegionBoundaryFree);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    RegionPntrsAsnRead()
*
**************************************************/
NLM_EXTERN 
RegionPntrsPtr LIBCALL
RegionPntrsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   RegionPntrsPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* RegionPntrs ::= (self contained) */
      atp = AsnReadId(aip, amp, REGION_PNTRS);
   } else {
      atp = AsnLinkType(orig, REGION_PNTRS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = RegionPntrsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == REGION_PNTRS_model_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> model_id = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == REGION_PNTRS_region) {
      ptr -> Region_region = Region_regionAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = RegionPntrsFree(ptr);
   goto ret;
}



/**************************************************
*
*    Region_regionAsnRead()
*
**************************************************/
static 
Region_regionPtr LIBCALL
Region_regionAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   ValNodePtr anp;
   Uint1 choice;
   Boolean isError = FALSE;
   Boolean nullIsError = FALSE;
   AsnReadFunc func;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Region_region ::= (self contained) */
      atp = AsnReadId(aip, amp, REGION_PNTRS_region);
   } else {
      atp = AsnLinkType(orig, REGION_PNTRS_region);    /* link in local tree */
   }
   if (atp == NULL) {
      return NULL;
   }

   anp = ValNodeNew(NULL);
   if (anp == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the CHOICE or OpenStruct value (nothing) */
      goto erret;
   }

   func = NULL;

   atp = AsnReadId(aip, amp, atp);  /* find the choice */
   if (atp == NULL) {
      goto erret;
   }
   if (atp == REGION_PNTRS_region_site) {
      choice = Region_region_site;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) RegionCoordinatesAsnRead,             (AsnOptFreeFunc) RegionCoordinatesFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == REGION_PNTRS_region_boundary) {
      choice = Region_region_boundary;
      anp -> data.ptrvalue =
      AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) RegionBoundaryAsnRead,             (AsnOptFreeFunc) RegionBoundaryFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   anp->choice = choice;
   if (func != NULL)
   {
      anp->data.ptrvalue = (* func)(aip, atp);
      if (aip -> io_failure) goto erret;

      if (nullIsError && anp->data.ptrvalue == NULL) {
         goto erret;
      }
   }

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return anp;

erret:
   anp = MemFree(anp);
   aip -> io_failure = TRUE;
   goto ret;
}


/**************************************************
*
*    RegionPntrsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
RegionPntrsAsnWrite(RegionPntrsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, REGION_PNTRS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> model_id;
   retval = AsnWrite(aip, REGION_PNTRS_model_id,  &av);
   if (ptr -> Region_region != NULL) {
      if ( ! Region_regionAsnWrite(ptr -> Region_region, aip, REGION_PNTRS_region)) {
         goto erret;
      }
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    Region_regionAsnWrite()
*
**************************************************/
static Boolean LIBCALL 
Region_regionAsnWrite(Region_regionPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, REGION_PNTRS_region);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

   av.ptrvalue = (Pointer)anp;
   if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, &av)) {
      goto erret;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   case Region_region_site:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) RegionCoordinatesAsnWrite, aip, REGION_PNTRS_region_site, REGION_PNTRS_region_site_E);
      break;
   case Region_region_boundary:
      retval = AsnGenericChoiceSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) RegionBoundaryAsnWrite, aip, REGION_PNTRS_region_boundary, REGION_PNTRS_region_boundary_E);
      break;
   }
   if (writetype != NULL) {
      retval = (* func)(pnt, aip, writetype);   /* write it out */
   }
   if (!retval) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}


/**************************************************
*
*    RegionSimilarityNew()
*
**************************************************/
NLM_EXTERN 
RegionSimilarityPtr LIBCALL
RegionSimilarityNew(void)
{
   RegionSimilarityPtr ptr = MemNew((size_t) sizeof(RegionSimilarity));

   ptr -> dimension = 2;
   return ptr;

}


/**************************************************
*
*    RegionSimilarityFree()
*
**************************************************/
NLM_EXTERN 
RegionSimilarityPtr LIBCALL
RegionSimilarityFree(RegionSimilarityPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr -> biostruc_ids, (AsnOptFreeFunc) BiostrucIdFree);
   AsnGenericUserSeqOfFree(ptr -> similarity, (AsnOptFreeFunc) RegionPntrsFree);
   AsnGenericUserSeqOfFree(ptr -> transform, (AsnOptFreeFunc) TransformFree);
   return MemFree(ptr);
}


/**************************************************
*
*    RegionSimilarityAsnRead()
*
**************************************************/
NLM_EXTERN 
RegionSimilarityPtr LIBCALL
RegionSimilarityAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   RegionSimilarityPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* RegionSimilarity ::= (self contained) */
      atp = AsnReadId(aip, amp, REGION_SIMILARITY);
   } else {
      atp = AsnLinkType(orig, REGION_SIMILARITY);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = RegionSimilarityNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == REGION_SIMILARITY_dimension) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> dimension = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == REGION_SIMILARITY_biostruc_ids) {
      ptr -> biostruc_ids = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BiostrucIdAsnRead, (AsnOptFreeFunc) BiostrucIdFree);
      if (isError && ptr -> biostruc_ids == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == REGION_SIMILARITY_similarity) {
      ptr -> similarity = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) RegionPntrsAsnRead, (AsnOptFreeFunc) RegionPntrsFree);
      if (isError && ptr -> similarity == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == REGION_SIMILARITY_transform) {
      ptr -> transform = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) TransformAsnRead, (AsnOptFreeFunc) TransformFree);
      if (isError && ptr -> transform == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = RegionSimilarityFree(ptr);
   goto ret;
}



/**************************************************
*
*    RegionSimilarityAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
RegionSimilarityAsnWrite(RegionSimilarityPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, REGION_SIMILARITY);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> dimension;
   retval = AsnWrite(aip, REGION_SIMILARITY_dimension,  &av);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> biostruc_ids, (AsnWriteFunc) BiostrucIdAsnWrite, aip, REGION_SIMILARITY_biostruc_ids, REGION_SIMILARITY_biostruc_ids_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> similarity, (AsnWriteFunc) RegionPntrsAsnWrite, aip, REGION_SIMILARITY_similarity, REGION_SIMILARITY_similarity_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> transform, (AsnWriteFunc) TransformAsnWrite, aip, REGION_SIMILARITY_transform, REGION_SIMILARITY_transform_E);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    OtherFeatureNew()
*
**************************************************/
NLM_EXTERN 
OtherFeaturePtr LIBCALL
OtherFeatureNew(void)
{
   OtherFeaturePtr ptr = MemNew((size_t) sizeof(OtherFeature));

   return ptr;

}


/**************************************************
*
*    OtherFeatureFree()
*
**************************************************/
NLM_EXTERN 
OtherFeaturePtr LIBCALL
OtherFeatureFree(OtherFeaturePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   BiostrucIdFree(ptr -> biostruc_id);
   return MemFree(ptr);
}


/**************************************************
*
*    OtherFeatureAsnRead()
*
**************************************************/
NLM_EXTERN 
OtherFeaturePtr LIBCALL
OtherFeatureAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   OtherFeaturePtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* OtherFeature ::= (self contained) */
      atp = AsnReadId(aip, amp, OTHER_FEATURE);
   } else {
      atp = AsnLinkType(orig, OTHER_FEATURE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = OtherFeatureNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == OTHER_FEATURE_biostruc_id) {
      ptr -> biostruc_id = BiostrucIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == OTHER_FEATURE_set) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> set = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == OTHER_FEATURE_feature) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> feature = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = OtherFeatureFree(ptr);
   goto ret;
}



/**************************************************
*
*    OtherFeatureAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
OtherFeatureAsnWrite(OtherFeaturePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, OTHER_FEATURE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> biostruc_id != NULL) {
      if ( ! BiostrucIdAsnWrite(ptr -> biostruc_id, aip, OTHER_FEATURE_biostruc_id)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> set;
   retval = AsnWrite(aip, OTHER_FEATURE_set,  &av);
   av.intvalue = ptr -> feature;
   retval = AsnWrite(aip, OTHER_FEATURE_feature,  &av);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    ResiduePntrsFree()
*
**************************************************/
NLM_EXTERN 
ResiduePntrsPtr LIBCALL
ResiduePntrsFree(ValNodePtr anp)
{
   Pointer pnt;

   if (anp == NULL) {
      return NULL;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   default:
      break;
   case ResiduePntrs_explicit:
      ResidueExplicitPntrsFree(anp -> data.ptrvalue);
      break;
   case ResiduePntrs_interval:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) ResidueIntervalPntrFree);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    ResiduePntrsAsnRead()
*
**************************************************/
NLM_EXTERN 
ResiduePntrsPtr LIBCALL
ResiduePntrsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   ValNodePtr anp;
   Uint1 choice;
   Boolean isError = FALSE;
   Boolean nullIsError = FALSE;
   AsnReadFunc func;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ResiduePntrs ::= (self contained) */
      atp = AsnReadId(aip, amp, RESIDUE_PNTRS);
   } else {
      atp = AsnLinkType(orig, RESIDUE_PNTRS);    /* link in local tree */
   }
   if (atp == NULL) {
      return NULL;
   }

   anp = ValNodeNew(NULL);
   if (anp == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the CHOICE or OpenStruct value (nothing) */
      goto erret;
   }

   func = NULL;

   atp = AsnReadId(aip, amp, atp);  /* find the choice */
   if (atp == NULL) {
      goto erret;
   }
   if (atp == RESIDUE_PNTRS_explicit) {
      choice = ResiduePntrs_explicit;
      func = (AsnReadFunc) ResidueExplicitPntrsAsnRead;
   }
   else if (atp == RESIDUE_PNTRS_interval) {
      choice = ResiduePntrs_interval;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ResidueIntervalPntrAsnRead,             (AsnOptFreeFunc) ResidueIntervalPntrFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   anp->choice = choice;
   if (func != NULL)
   {
      anp->data.ptrvalue = (* func)(aip, atp);
      if (aip -> io_failure) goto erret;

      if (nullIsError && anp->data.ptrvalue == NULL) {
         goto erret;
      }
   }

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return anp;

erret:
   anp = MemFree(anp);
   aip -> io_failure = TRUE;
   goto ret;
}


/**************************************************
*
*    ResiduePntrsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ResiduePntrsAsnWrite(ResiduePntrsPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, RESIDUE_PNTRS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

   av.ptrvalue = (Pointer)anp;
   if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, &av)) {
      goto erret;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   case ResiduePntrs_explicit:
      writetype = RESIDUE_PNTRS_explicit;
      func = (AsnWriteFunc) ResidueExplicitPntrsAsnWrite;
      break;
   case ResiduePntrs_interval:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) ResidueIntervalPntrAsnWrite, aip, RESIDUE_PNTRS_interval, RESIDUE_PNTRS_interval_E);
      break;
   }
   if (writetype != NULL) {
      retval = (* func)(pnt, aip, writetype);   /* write it out */
   }
   if (!retval) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}


/**************************************************
*
*    MoleculePntrsNew()
*
**************************************************/
NLM_EXTERN 
MoleculePntrsPtr LIBCALL
MoleculePntrsNew(void)
{
   MoleculePntrsPtr ptr = MemNew((size_t) sizeof(MoleculePntrs));

   return ptr;

}


/**************************************************
*
*    MoleculePntrsFree()
*
**************************************************/
NLM_EXTERN 
MoleculePntrsPtr LIBCALL
MoleculePntrsFree(MoleculePntrsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericBaseSeqOfFree(ptr -> molecule_ids ,ASNCODE_INTVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    MoleculePntrsAsnRead()
*
**************************************************/
NLM_EXTERN 
MoleculePntrsPtr LIBCALL
MoleculePntrsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MoleculePntrsPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MoleculePntrs ::= (self contained) */
      atp = AsnReadId(aip, amp, MOLECULE_PNTRS);
   } else {
      atp = AsnLinkType(orig, MOLECULE_PNTRS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MoleculePntrsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MOLECULE_PNTRS_number_of_ptrs) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> number_of_ptrs = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MOLECULE_PNTRS_molecule_ids) {
      ptr -> molecule_ids = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> molecule_ids == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = MoleculePntrsFree(ptr);
   goto ret;
}



/**************************************************
*
*    MoleculePntrsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MoleculePntrsAsnWrite(MoleculePntrsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MOLECULE_PNTRS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> number_of_ptrs;
   retval = AsnWrite(aip, MOLECULE_PNTRS_number_of_ptrs,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> molecule_ids ,ASNCODE_INTVAL_SLOT, aip, MOLECULE_PNTRS_molecule_ids, MOLECULE_PNTRS_molecule_ids_E);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    ResidueExplicitPntrsNew()
*
**************************************************/
NLM_EXTERN 
ResidueExplicitPntrsPtr LIBCALL
ResidueExplicitPntrsNew(void)
{
   ResidueExplicitPntrsPtr ptr = MemNew((size_t) sizeof(ResidueExplicitPntrs));

   return ptr;

}


/**************************************************
*
*    ResidueExplicitPntrsFree()
*
**************************************************/
NLM_EXTERN 
ResidueExplicitPntrsPtr LIBCALL
ResidueExplicitPntrsFree(ResidueExplicitPntrsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericBaseSeqOfFree(ptr -> molecule_ids ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> residue_ids ,ASNCODE_INTVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    ResidueExplicitPntrsAsnRead()
*
**************************************************/
NLM_EXTERN 
ResidueExplicitPntrsPtr LIBCALL
ResidueExplicitPntrsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ResidueExplicitPntrsPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ResidueExplicitPntrs ::= (self contained) */
      atp = AsnReadId(aip, amp, RESIDUE_EXPLICIT_PNTRS);
   } else {
      atp = AsnLinkType(orig, RESIDUE_EXPLICIT_PNTRS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ResidueExplicitPntrsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == RESIDUE_EXPLICIT_PNTRS_number_of_ptrs) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> number_of_ptrs = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == RESIDUE_EXPLICIT_PNTRS_molecule_ids) {
      ptr -> molecule_ids = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> molecule_ids == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == RESIDUE_EXPLICIT_PNTRS_residue_ids) {
      ptr -> residue_ids = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> residue_ids == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = ResidueExplicitPntrsFree(ptr);
   goto ret;
}



/**************************************************
*
*    ResidueExplicitPntrsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ResidueExplicitPntrsAsnWrite(ResidueExplicitPntrsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, RESIDUE_EXPLICIT_PNTRS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> number_of_ptrs;
   retval = AsnWrite(aip, RESIDUE_EXPLICIT_PNTRS_number_of_ptrs,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> molecule_ids ,ASNCODE_INTVAL_SLOT, aip, RESIDUE_EXPLICIT_PNTRS_molecule_ids, RESIDUE_EXPLICIT_PNTRS_molecule_ids_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> residue_ids ,ASNCODE_INTVAL_SLOT, aip, RESIDUE_EXPLICIT_PNTRS_residue_ids, RESIDUE_EXPLICIT_PNTRS_residue_ids_E);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    ResidueIntervalPntrNew()
*
**************************************************/
NLM_EXTERN 
ResidueIntervalPntrPtr LIBCALL
ResidueIntervalPntrNew(void)
{
   ResidueIntervalPntrPtr ptr = MemNew((size_t) sizeof(ResidueIntervalPntr));

   return ptr;

}


/**************************************************
*
*    ResidueIntervalPntrFree()
*
**************************************************/
NLM_EXTERN 
ResidueIntervalPntrPtr LIBCALL
ResidueIntervalPntrFree(ResidueIntervalPntrPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    ResidueIntervalPntrAsnRead()
*
**************************************************/
NLM_EXTERN 
ResidueIntervalPntrPtr LIBCALL
ResidueIntervalPntrAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ResidueIntervalPntrPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ResidueIntervalPntr ::= (self contained) */
      atp = AsnReadId(aip, amp, RESIDUE_INTERVAL_PNTR);
   } else {
      atp = AsnLinkType(orig, RESIDUE_INTERVAL_PNTR);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ResidueIntervalPntrNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == RESIDUE_INTERVAL_PNTR_molecule_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> molecule_id = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == RESIDUE_INTERVAL_PNTR_from) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> from = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == RESIDUE_INTERVAL_PNTR_to) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> to = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = ResidueIntervalPntrFree(ptr);
   goto ret;
}



/**************************************************
*
*    ResidueIntervalPntrAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ResidueIntervalPntrAsnWrite(ResidueIntervalPntrPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, RESIDUE_INTERVAL_PNTR);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> molecule_id;
   retval = AsnWrite(aip, RESIDUE_INTERVAL_PNTR_molecule_id,  &av);
   av.intvalue = ptr -> from;
   retval = AsnWrite(aip, RESIDUE_INTERVAL_PNTR_from,  &av);
   av.intvalue = ptr -> to;
   retval = AsnWrite(aip, RESIDUE_INTERVAL_PNTR_to,  &av);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    RegionCoordinatesNew()
*
**************************************************/
NLM_EXTERN 
RegionCoordinatesPtr LIBCALL
RegionCoordinatesNew(void)
{
   RegionCoordinatesPtr ptr = MemNew((size_t) sizeof(RegionCoordinates));

   return ptr;

}


/**************************************************
*
*    RegionCoordinatesFree()
*
**************************************************/
NLM_EXTERN 
RegionCoordinatesPtr LIBCALL
RegionCoordinatesFree(RegionCoordinatesPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericBaseSeqOfFree(ptr -> coordinate_indices ,ASNCODE_INTVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    RegionCoordinatesAsnRead()
*
**************************************************/
NLM_EXTERN 
RegionCoordinatesPtr LIBCALL
RegionCoordinatesAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   RegionCoordinatesPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* RegionCoordinates ::= (self contained) */
      atp = AsnReadId(aip, amp, REGION_COORDINATES);
   } else {
      atp = AsnLinkType(orig, REGION_COORDINATES);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = RegionCoordinatesNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == REGION_COORDINATES_model_coord_set_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> model_coord_set_id = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == REGION_COORDINATES_number_of_coords) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> number_of_coords = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == REGION_COORDINATES_coordinate_indices) {
      ptr -> coordinate_indices = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> coordinate_indices == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = RegionCoordinatesFree(ptr);
   goto ret;
}



/**************************************************
*
*    RegionCoordinatesAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
RegionCoordinatesAsnWrite(RegionCoordinatesPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, REGION_COORDINATES);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> model_coord_set_id;
   retval = AsnWrite(aip, REGION_COORDINATES_model_coord_set_id,  &av);
   if (ptr -> number_of_coords || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> number_of_coords;
      retval = AsnWrite(aip, REGION_COORDINATES_number_of_coords,  &av);
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> coordinate_indices ,ASNCODE_INTVAL_SLOT, aip, REGION_COORDINATES_coordinate_indices, REGION_COORDINATES_coordinate_indices_E);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    RegionBoundaryFree()
*
**************************************************/
NLM_EXTERN 
RegionBoundaryPtr LIBCALL
RegionBoundaryFree(ValNodePtr anp)
{
   Pointer pnt;

   if (anp == NULL) {
      return NULL;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   default:
      break;
   case RegionBoundary_sphere:
      SphereFree(anp -> data.ptrvalue);
      break;
   case RegionBoundary_cone:
      ConeFree(anp -> data.ptrvalue);
      break;
   case RegionBoundary_cylinder:
      CylinderFree(anp -> data.ptrvalue);
      break;
   case RegionBoundary_brick:
      BrickFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    RegionBoundaryAsnRead()
*
**************************************************/
NLM_EXTERN 
RegionBoundaryPtr LIBCALL
RegionBoundaryAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   ValNodePtr anp;
   Uint1 choice;
   Boolean isError = FALSE;
   Boolean nullIsError = FALSE;
   AsnReadFunc func;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* RegionBoundary ::= (self contained) */
      atp = AsnReadId(aip, amp, REGION_BOUNDARY);
   } else {
      atp = AsnLinkType(orig, REGION_BOUNDARY);    /* link in local tree */
   }
   if (atp == NULL) {
      return NULL;
   }

   anp = ValNodeNew(NULL);
   if (anp == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the CHOICE or OpenStruct value (nothing) */
      goto erret;
   }

   func = NULL;

   atp = AsnReadId(aip, amp, atp);  /* find the choice */
   if (atp == NULL) {
      goto erret;
   }
   if (atp == REGION_BOUNDARY_sphere) {
      choice = RegionBoundary_sphere;
      func = (AsnReadFunc) SphereAsnRead;
   }
   else if (atp == REGION_BOUNDARY_cone) {
      choice = RegionBoundary_cone;
      func = (AsnReadFunc) ConeAsnRead;
   }
   else if (atp == REGION_BOUNDARY_cylinder) {
      choice = RegionBoundary_cylinder;
      func = (AsnReadFunc) CylinderAsnRead;
   }
   else if (atp == REGION_BOUNDARY_brick) {
      choice = RegionBoundary_brick;
      func = (AsnReadFunc) BrickAsnRead;
   }
   anp->choice = choice;
   if (func != NULL)
   {
      anp->data.ptrvalue = (* func)(aip, atp);
      if (aip -> io_failure) goto erret;

      if (nullIsError && anp->data.ptrvalue == NULL) {
         goto erret;
      }
   }

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return anp;

erret:
   anp = MemFree(anp);
   aip -> io_failure = TRUE;
   goto ret;
}


/**************************************************
*
*    RegionBoundaryAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
RegionBoundaryAsnWrite(RegionBoundaryPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, REGION_BOUNDARY);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

   av.ptrvalue = (Pointer)anp;
   if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, &av)) {
      goto erret;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   case RegionBoundary_sphere:
      writetype = REGION_BOUNDARY_sphere;
      func = (AsnWriteFunc) SphereAsnWrite;
      break;
   case RegionBoundary_cone:
      writetype = REGION_BOUNDARY_cone;
      func = (AsnWriteFunc) ConeAsnWrite;
      break;
   case RegionBoundary_cylinder:
      writetype = REGION_BOUNDARY_cylinder;
      func = (AsnWriteFunc) CylinderAsnWrite;
      break;
   case RegionBoundary_brick:
      writetype = REGION_BOUNDARY_brick;
      func = (AsnWriteFunc) BrickAsnWrite;
      break;
   }
   if (writetype != NULL) {
      retval = (* func)(pnt, aip, writetype);   /* write it out */
   }
   if (!retval) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}


/**************************************************
*
*    AlignStatsNew()
*
**************************************************/
NLM_EXTERN 
AlignStatsPtr LIBCALL
AlignStatsNew(void)
{
   AlignStatsPtr ptr = MemNew((size_t) sizeof(AlignStats));

   return ptr;

}


/**************************************************
*
*    AlignStatsFree()
*
**************************************************/
NLM_EXTERN 
AlignStatsPtr LIBCALL
AlignStatsFree(AlignStatsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> descr);
   return MemFree(ptr);
}


/**************************************************
*
*    AlignStatsAsnRead()
*
**************************************************/
NLM_EXTERN 
AlignStatsPtr LIBCALL
AlignStatsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AlignStatsPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AlignStats ::= (self contained) */
      atp = AsnReadId(aip, amp, ALIGN_STATS);
   } else {
      atp = AsnLinkType(orig, ALIGN_STATS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = AlignStatsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ALIGN_STATS_descr) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> descr = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALIGN_STATS_scale_factor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> scale_factor = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALIGN_STATS_vast_score) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> vast_score = av.intvalue;
      ptr -> OBbits__ |= 1<<1;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALIGN_STATS_vast_mlogp) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> vast_mlogp = av.intvalue;
      ptr -> OBbits__ |= 1<<2;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALIGN_STATS_align_res) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> align_res = av.intvalue;
      ptr -> OBbits__ |= 1<<3;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALIGN_STATS_rmsd) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> rmsd = av.intvalue;
      ptr -> OBbits__ |= 1<<4;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALIGN_STATS_blast_score) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> blast_score = av.intvalue;
      ptr -> OBbits__ |= 1<<5;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALIGN_STATS_blast_mlogp) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> blast_mlogp = av.intvalue;
      ptr -> OBbits__ |= 1<<6;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALIGN_STATS_other_score) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> other_score = av.intvalue;
      ptr -> OBbits__ |= 1<<7;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = AlignStatsFree(ptr);
   goto ret;
}



/**************************************************
*
*    AlignStatsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AlignStatsAsnWrite(AlignStatsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ALIGN_STATS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> descr != NULL) {
      av.ptrvalue = ptr -> descr;
      retval = AsnWrite(aip, ALIGN_STATS_descr,  &av);
   }
   if (ptr -> scale_factor || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> scale_factor;
      retval = AsnWrite(aip, ALIGN_STATS_scale_factor,  &av);
   }
   if (ptr -> vast_score || (ptr -> OBbits__ & (1<<1) )){   av.intvalue = ptr -> vast_score;
      retval = AsnWrite(aip, ALIGN_STATS_vast_score,  &av);
   }
   if (ptr -> vast_mlogp || (ptr -> OBbits__ & (1<<2) )){   av.intvalue = ptr -> vast_mlogp;
      retval = AsnWrite(aip, ALIGN_STATS_vast_mlogp,  &av);
   }
   if (ptr -> align_res || (ptr -> OBbits__ & (1<<3) )){   av.intvalue = ptr -> align_res;
      retval = AsnWrite(aip, ALIGN_STATS_align_res,  &av);
   }
   if (ptr -> rmsd || (ptr -> OBbits__ & (1<<4) )){   av.intvalue = ptr -> rmsd;
      retval = AsnWrite(aip, ALIGN_STATS_rmsd,  &av);
   }
   if (ptr -> blast_score || (ptr -> OBbits__ & (1<<5) )){   av.intvalue = ptr -> blast_score;
      retval = AsnWrite(aip, ALIGN_STATS_blast_score,  &av);
   }
   if (ptr -> blast_mlogp || (ptr -> OBbits__ & (1<<6) )){   av.intvalue = ptr -> blast_mlogp;
      retval = AsnWrite(aip, ALIGN_STATS_blast_mlogp,  &av);
   }
   if (ptr -> other_score || (ptr -> OBbits__ & (1<<7) )){   av.intvalue = ptr -> other_score;
      retval = AsnWrite(aip, ALIGN_STATS_other_score,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    ModelSpacePointNew()
*
**************************************************/
NLM_EXTERN 
ModelSpacePointPtr LIBCALL
ModelSpacePointNew(void)
{
   ModelSpacePointPtr ptr = MemNew((size_t) sizeof(ModelSpacePoint));

   return ptr;

}


/**************************************************
*
*    ModelSpacePointFree()
*
**************************************************/
NLM_EXTERN 
ModelSpacePointPtr LIBCALL
ModelSpacePointFree(ModelSpacePointPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    ModelSpacePointAsnRead()
*
**************************************************/
NLM_EXTERN 
ModelSpacePointPtr LIBCALL
ModelSpacePointAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ModelSpacePointPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ModelSpacePoint ::= (self contained) */
      atp = AsnReadId(aip, amp, MODEL_SPACE_POINT);
   } else {
      atp = AsnLinkType(orig, MODEL_SPACE_POINT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ModelSpacePointNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MODEL_SPACE_POINT_scale_factor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> scale_factor = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MODEL_SPACE_POINT_x) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> x = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MODEL_SPACE_POINT_y) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> y = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MODEL_SPACE_POINT_z) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> z = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = ModelSpacePointFree(ptr);
   goto ret;
}



/**************************************************
*
*    ModelSpacePointAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ModelSpacePointAsnWrite(ModelSpacePointPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MODEL_SPACE_POINT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> scale_factor;
   retval = AsnWrite(aip, MODEL_SPACE_POINT_scale_factor,  &av);
   av.intvalue = ptr -> x;
   retval = AsnWrite(aip, MODEL_SPACE_POINT_x,  &av);
   av.intvalue = ptr -> y;
   retval = AsnWrite(aip, MODEL_SPACE_POINT_y,  &av);
   av.intvalue = ptr -> z;
   retval = AsnWrite(aip, MODEL_SPACE_POINT_z,  &av);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    RealValueNew()
*
**************************************************/
NLM_EXTERN 
RealValuePtr LIBCALL
RealValueNew(void)
{
   RealValuePtr ptr = MemNew((size_t) sizeof(RealValue));

   return ptr;

}


/**************************************************
*
*    RealValueFree()
*
**************************************************/
NLM_EXTERN 
RealValuePtr LIBCALL
RealValueFree(RealValuePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    RealValueAsnRead()
*
**************************************************/
NLM_EXTERN 
RealValuePtr LIBCALL
RealValueAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   RealValuePtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* RealValue ::= (self contained) */
      atp = AsnReadId(aip, amp, REALVALUE);
   } else {
      atp = AsnLinkType(orig, REALVALUE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = RealValueNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == REALVALUE_scale_factor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> scale_factor = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == REALVALUE_scaled_integer_value) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> scaled_integer_value = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = RealValueFree(ptr);
   goto ret;
}



/**************************************************
*
*    RealValueAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
RealValueAsnWrite(RealValuePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, REALVALUE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> scale_factor;
   retval = AsnWrite(aip, REALVALUE_scale_factor,  &av);
   av.intvalue = ptr -> scaled_integer_value;
   retval = AsnWrite(aip, REALVALUE_scaled_integer_value,  &av);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    MoveFree()
*
**************************************************/
NLM_EXTERN 
MovePtr LIBCALL
MoveFree(ValNodePtr anp)
{
   Pointer pnt;

   if (anp == NULL) {
      return NULL;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   default:
      break;
   case Move_rotate:
      RotMatrixFree(anp -> data.ptrvalue);
      break;
   case Move_translate:
      TransMatrixFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    MoveAsnRead()
*
**************************************************/
NLM_EXTERN 
MovePtr LIBCALL
MoveAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   ValNodePtr anp;
   Uint1 choice;
   Boolean isError = FALSE;
   Boolean nullIsError = FALSE;
   AsnReadFunc func;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Move ::= (self contained) */
      atp = AsnReadId(aip, amp, MOVE);
   } else {
      atp = AsnLinkType(orig, MOVE);    /* link in local tree */
   }
   if (atp == NULL) {
      return NULL;
   }

   anp = ValNodeNew(NULL);
   if (anp == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the CHOICE or OpenStruct value (nothing) */
      goto erret;
   }

   func = NULL;

   atp = AsnReadId(aip, amp, atp);  /* find the choice */
   if (atp == NULL) {
      goto erret;
   }
   if (atp == MOVE_rotate) {
      choice = Move_rotate;
      func = (AsnReadFunc) RotMatrixAsnRead;
   }
   else if (atp == MOVE_translate) {
      choice = Move_translate;
      func = (AsnReadFunc) TransMatrixAsnRead;
   }
   anp->choice = choice;
   if (func != NULL)
   {
      anp->data.ptrvalue = (* func)(aip, atp);
      if (aip -> io_failure) goto erret;

      if (nullIsError && anp->data.ptrvalue == NULL) {
         goto erret;
      }
   }

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return anp;

erret:
   anp = MemFree(anp);
   aip -> io_failure = TRUE;
   goto ret;
}


/**************************************************
*
*    MoveAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MoveAsnWrite(MovePtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, MOVE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

   av.ptrvalue = (Pointer)anp;
   if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, &av)) {
      goto erret;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   case Move_rotate:
      writetype = MOVE_rotate;
      func = (AsnWriteFunc) RotMatrixAsnWrite;
      break;
   case Move_translate:
      writetype = MOVE_translate;
      func = (AsnWriteFunc) TransMatrixAsnWrite;
      break;
   }
   if (writetype != NULL) {
      retval = (* func)(pnt, aip, writetype);   /* write it out */
   }
   if (!retval) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}


/**************************************************
*
*    RotMatrixNew()
*
**************************************************/
NLM_EXTERN 
RotMatrixPtr LIBCALL
RotMatrixNew(void)
{
   RotMatrixPtr ptr = MemNew((size_t) sizeof(RotMatrix));

   return ptr;

}


/**************************************************
*
*    RotMatrixFree()
*
**************************************************/
NLM_EXTERN 
RotMatrixPtr LIBCALL
RotMatrixFree(RotMatrixPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    RotMatrixAsnRead()
*
**************************************************/
NLM_EXTERN 
RotMatrixPtr LIBCALL
RotMatrixAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   RotMatrixPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* RotMatrix ::= (self contained) */
      atp = AsnReadId(aip, amp, ROT_MATRIX);
   } else {
      atp = AsnLinkType(orig, ROT_MATRIX);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = RotMatrixNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ROT_MATRIX_scale_factor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> scale_factor = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ROT_MATRIX_rot_11) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> rot_11 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ROT_MATRIX_rot_12) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> rot_12 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ROT_MATRIX_rot_13) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> rot_13 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ROT_MATRIX_rot_21) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> rot_21 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ROT_MATRIX_rot_22) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> rot_22 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ROT_MATRIX_rot_23) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> rot_23 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ROT_MATRIX_rot_31) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> rot_31 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ROT_MATRIX_rot_32) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> rot_32 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ROT_MATRIX_rot_33) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> rot_33 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = RotMatrixFree(ptr);
   goto ret;
}



/**************************************************
*
*    RotMatrixAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
RotMatrixAsnWrite(RotMatrixPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ROT_MATRIX);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> scale_factor;
   retval = AsnWrite(aip, ROT_MATRIX_scale_factor,  &av);
   av.intvalue = ptr -> rot_11;
   retval = AsnWrite(aip, ROT_MATRIX_rot_11,  &av);
   av.intvalue = ptr -> rot_12;
   retval = AsnWrite(aip, ROT_MATRIX_rot_12,  &av);
   av.intvalue = ptr -> rot_13;
   retval = AsnWrite(aip, ROT_MATRIX_rot_13,  &av);
   av.intvalue = ptr -> rot_21;
   retval = AsnWrite(aip, ROT_MATRIX_rot_21,  &av);
   av.intvalue = ptr -> rot_22;
   retval = AsnWrite(aip, ROT_MATRIX_rot_22,  &av);
   av.intvalue = ptr -> rot_23;
   retval = AsnWrite(aip, ROT_MATRIX_rot_23,  &av);
   av.intvalue = ptr -> rot_31;
   retval = AsnWrite(aip, ROT_MATRIX_rot_31,  &av);
   av.intvalue = ptr -> rot_32;
   retval = AsnWrite(aip, ROT_MATRIX_rot_32,  &av);
   av.intvalue = ptr -> rot_33;
   retval = AsnWrite(aip, ROT_MATRIX_rot_33,  &av);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    TransMatrixNew()
*
**************************************************/
NLM_EXTERN 
TransMatrixPtr LIBCALL
TransMatrixNew(void)
{
   TransMatrixPtr ptr = MemNew((size_t) sizeof(TransMatrix));

   return ptr;

}


/**************************************************
*
*    TransMatrixFree()
*
**************************************************/
NLM_EXTERN 
TransMatrixPtr LIBCALL
TransMatrixFree(TransMatrixPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    TransMatrixAsnRead()
*
**************************************************/
NLM_EXTERN 
TransMatrixPtr LIBCALL
TransMatrixAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   TransMatrixPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* TransMatrix ::= (self contained) */
      atp = AsnReadId(aip, amp, TRANS_MATRIX);
   } else {
      atp = AsnLinkType(orig, TRANS_MATRIX);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = TransMatrixNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == TRANS_MATRIX_scale_factor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> scale_factor = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TRANS_MATRIX_tran_1) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> tran_1 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TRANS_MATRIX_tran_2) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> tran_2 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TRANS_MATRIX_tran_3) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> tran_3 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = TransMatrixFree(ptr);
   goto ret;
}



/**************************************************
*
*    TransMatrixAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
TransMatrixAsnWrite(TransMatrixPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TRANS_MATRIX);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> scale_factor;
   retval = AsnWrite(aip, TRANS_MATRIX_scale_factor,  &av);
   av.intvalue = ptr -> tran_1;
   retval = AsnWrite(aip, TRANS_MATRIX_tran_1,  &av);
   av.intvalue = ptr -> tran_2;
   retval = AsnWrite(aip, TRANS_MATRIX_tran_2,  &av);
   av.intvalue = ptr -> tran_3;
   retval = AsnWrite(aip, TRANS_MATRIX_tran_3,  &av);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    GLMatrixNew()
*
**************************************************/
NLM_EXTERN 
GLMatrixPtr LIBCALL
GLMatrixNew(void)
{
   GLMatrixPtr ptr = MemNew((size_t) sizeof(GLMatrix));

   return ptr;

}


/**************************************************
*
*    GLMatrixFree()
*
**************************************************/
NLM_EXTERN 
GLMatrixPtr LIBCALL
GLMatrixFree(GLMatrixPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    GLMatrixAsnRead()
*
**************************************************/
NLM_EXTERN 
GLMatrixPtr LIBCALL
GLMatrixAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   GLMatrixPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* GLMatrix ::= (self contained) */
      atp = AsnReadId(aip, amp, GL_MATRIX);
   } else {
      atp = AsnLinkType(orig, GL_MATRIX);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = GLMatrixNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == GL_MATRIX_scale) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> scale = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GL_MATRIX_m11) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m11 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GL_MATRIX_m12) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m12 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GL_MATRIX_m13) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m13 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GL_MATRIX_m14) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m14 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GL_MATRIX_m21) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m21 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GL_MATRIX_m22) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m22 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GL_MATRIX_m23) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m23 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GL_MATRIX_m24) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m24 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GL_MATRIX_m31) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m31 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GL_MATRIX_m32) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m32 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GL_MATRIX_m33) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m33 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GL_MATRIX_m34) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m34 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GL_MATRIX_m41) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m41 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GL_MATRIX_m42) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m42 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GL_MATRIX_m43) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m43 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GL_MATRIX_m44) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m44 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = GLMatrixFree(ptr);
   goto ret;
}



/**************************************************
*
*    GLMatrixAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
GLMatrixAsnWrite(GLMatrixPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, GL_MATRIX);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> scale;
   retval = AsnWrite(aip, GL_MATRIX_scale,  &av);
   av.intvalue = ptr -> m11;
   retval = AsnWrite(aip, GL_MATRIX_m11,  &av);
   av.intvalue = ptr -> m12;
   retval = AsnWrite(aip, GL_MATRIX_m12,  &av);
   av.intvalue = ptr -> m13;
   retval = AsnWrite(aip, GL_MATRIX_m13,  &av);
   av.intvalue = ptr -> m14;
   retval = AsnWrite(aip, GL_MATRIX_m14,  &av);
   av.intvalue = ptr -> m21;
   retval = AsnWrite(aip, GL_MATRIX_m21,  &av);
   av.intvalue = ptr -> m22;
   retval = AsnWrite(aip, GL_MATRIX_m22,  &av);
   av.intvalue = ptr -> m23;
   retval = AsnWrite(aip, GL_MATRIX_m23,  &av);
   av.intvalue = ptr -> m24;
   retval = AsnWrite(aip, GL_MATRIX_m24,  &av);
   av.intvalue = ptr -> m31;
   retval = AsnWrite(aip, GL_MATRIX_m31,  &av);
   av.intvalue = ptr -> m32;
   retval = AsnWrite(aip, GL_MATRIX_m32,  &av);
   av.intvalue = ptr -> m33;
   retval = AsnWrite(aip, GL_MATRIX_m33,  &av);
   av.intvalue = ptr -> m34;
   retval = AsnWrite(aip, GL_MATRIX_m34,  &av);
   av.intvalue = ptr -> m41;
   retval = AsnWrite(aip, GL_MATRIX_m41,  &av);
   av.intvalue = ptr -> m42;
   retval = AsnWrite(aip, GL_MATRIX_m42,  &av);
   av.intvalue = ptr -> m43;
   retval = AsnWrite(aip, GL_MATRIX_m43,  &av);
   av.intvalue = ptr -> m44;
   retval = AsnWrite(aip, GL_MATRIX_m44,  &av);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    BiostrucScriptStepNew()
*
**************************************************/
NLM_EXTERN 
BiostrucScriptStepPtr LIBCALL
BiostrucScriptStepNew(void)
{
   BiostrucScriptStepPtr ptr = MemNew((size_t) sizeof(BiostrucScriptStep));

   ptr -> pause = 10;
   return ptr;

}


/**************************************************
*
*    BiostrucScriptStepFree()
*
**************************************************/
NLM_EXTERN 
BiostrucScriptStepPtr LIBCALL
BiostrucScriptStepFree(BiostrucScriptStepPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> step_name);
   AsnGenericUserSeqOfFree(ptr -> feature_do, (AsnOptFreeFunc) OtherFeatureFree);
   TransformFree(ptr -> camera_move);
   return MemFree(ptr);
}


/**************************************************
*
*    BiostrucScriptStepAsnRead()
*
**************************************************/
NLM_EXTERN 
BiostrucScriptStepPtr LIBCALL
BiostrucScriptStepAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BiostrucScriptStepPtr ptr;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BiostrucScriptStep ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC_SCRIPT_STEP);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC_SCRIPT_STEP);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BiostrucScriptStepNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BIOSTRUC_SCRIPT_STEP_step_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> step_id = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_SCRIPT_STEP_step_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> step_name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_SCRIPT_STEP_feature_do) {
      ptr -> feature_do = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) OtherFeatureAsnRead, (AsnOptFreeFunc) OtherFeatureFree);
      if (isError && ptr -> feature_do == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_SCRIPT_STEP_camera_move) {
      ptr -> camera_move = TransformAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_SCRIPT_STEP_pause) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> pause = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_SCRIPT_STEP_waitevent) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> waitevent = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_SCRIPT_STEP_extra) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> extra = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_SCRIPT_STEP_jump) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> jump = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = BiostrucScriptStepFree(ptr);
   goto ret;
}



/**************************************************
*
*    BiostrucScriptStepAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BiostrucScriptStepAsnWrite(BiostrucScriptStepPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BIOSTRUC_SCRIPT_STEP);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> step_id;
   retval = AsnWrite(aip, BIOSTRUC_SCRIPT_STEP_step_id,  &av);
   if (ptr -> step_name != NULL) {
      av.ptrvalue = ptr -> step_name;
      retval = AsnWrite(aip, BIOSTRUC_SCRIPT_STEP_step_name,  &av);
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> feature_do, (AsnWriteFunc) OtherFeatureAsnWrite, aip, BIOSTRUC_SCRIPT_STEP_feature_do, BIOSTRUC_SCRIPT_STEP_feature_do_E);
   if (ptr -> camera_move != NULL) {
      if ( ! TransformAsnWrite(ptr -> camera_move, aip, BIOSTRUC_SCRIPT_STEP_camera_move)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> pause;
   retval = AsnWrite(aip, BIOSTRUC_SCRIPT_STEP_pause,  &av);
   av.boolvalue = ptr -> waitevent;
   retval = AsnWrite(aip, BIOSTRUC_SCRIPT_STEP_waitevent,  &av);
   av.intvalue = ptr -> extra;
   retval = AsnWrite(aip, BIOSTRUC_SCRIPT_STEP_extra,  &av);
   if (ptr -> jump || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> jump;
      retval = AsnWrite(aip, BIOSTRUC_SCRIPT_STEP_jump,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

