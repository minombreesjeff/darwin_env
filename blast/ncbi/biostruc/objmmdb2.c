#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <strimprt.h>
#include <objmmdb2.h>

static Boolean loaded = FALSE;

#include <mmdb2.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

Boolean LIBCALL
objmmdb2AsnLoad(void)
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
*    Generated object loaders for Module MMDB-Structural-model
*    Generated using ASNCODE Revision: 4.2 at Aug 1, 1996  4:27 PM
*
**************************************************/


/**************************************************
*
*    BiostrucModelNew()
*
**************************************************/

BiostrucModelPtr LIBCALL
BiostrucModelNew(void)
{
   BiostrucModelPtr ptr = MemNew((size_t) sizeof(BiostrucModel));

   return ptr;

}


/**************************************************
*
*    BiostrucModelFree()
*
**************************************************/

BiostrucModelPtr LIBCALL
BiostrucModelFree(BiostrucModelPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr -> descr, (AsnOptFreeFunc) ModelDescrFree);
   ModelSpaceFree(ptr -> model_space);
   AsnGenericUserSeqOfFree(ptr -> model_coordinates, (AsnOptFreeFunc) ModelCoordinateSetFree);
   return MemFree(ptr);
}


/**************************************************
*
*    BiostrucModelAsnRead()
*
**************************************************/

BiostrucModelPtr LIBCALL
BiostrucModelAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BiostrucModelPtr ptr;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BiostrucModel ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC_MODEL);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC_MODEL);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BiostrucModelNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BIOSTRUC_MODEL_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> id = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_MODEL_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_MODEL_descr) {
      ptr -> descr = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ModelDescrAsnRead, (AsnOptFreeFunc) ModelDescrFree);
      if (isError && ptr -> descr == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_MODEL_model_space) {
      ptr -> model_space = ModelSpaceAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_MODEL_model_coordinates) {
      ptr -> model_coordinates = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ModelCoordinateSetAsnRead, (AsnOptFreeFunc) ModelCoordinateSetFree);
      if (isError && ptr -> model_coordinates == NULL) {
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
   ptr = BiostrucModelFree(ptr);
   goto ret;
}



/**************************************************
*
*    BiostrucModelAsnWrite()
*
**************************************************/
Boolean LIBCALL 
BiostrucModelAsnWrite(BiostrucModelPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BIOSTRUC_MODEL);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> id;
   retval = AsnWrite(aip, BIOSTRUC_MODEL_id,  &av);
   av.intvalue = ptr -> type;
   retval = AsnWrite(aip, BIOSTRUC_MODEL_type,  &av);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> descr, (AsnWriteFunc) ModelDescrAsnWrite, aip, BIOSTRUC_MODEL_descr, BIOSTRUC_MODEL_descr_E);
   if (ptr -> model_space != NULL) {
      if ( ! ModelSpaceAsnWrite(ptr -> model_space, aip, BIOSTRUC_MODEL_model_space)) {
         goto erret;
      }
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> model_coordinates, (AsnWriteFunc) ModelCoordinateSetAsnWrite, aip, BIOSTRUC_MODEL_model_coordinates, BIOSTRUC_MODEL_model_coordinates_E);
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
*    ModelDescrFree()
*
**************************************************/

ModelDescrPtr LIBCALL
ModelDescrFree(ValNodePtr anp)
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
   case ModelDescr_name:
      MemFree(anp -> data.ptrvalue);
      break;
   case ModelDescr_pdb_reso:
      MemFree(anp -> data.ptrvalue);
      break;
   case ModelDescr_pdb_method:
      MemFree(anp -> data.ptrvalue);
      break;
   case ModelDescr_pdb_comment:
      MemFree(anp -> data.ptrvalue);
      break;
   case ModelDescr_other_comment:
      MemFree(anp -> data.ptrvalue);
      break;
   case ModelDescr_attribution:
      PubFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    ModelDescrAsnRead()
*
**************************************************/

ModelDescrPtr LIBCALL
ModelDescrAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objmmdb2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ModelDescr ::= (self contained) */
      atp = AsnReadId(aip, amp, MODEL_DESCR);
   } else {
      atp = AsnLinkType(orig, MODEL_DESCR);    /* link in local tree */
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
   if (atp == MODEL_DESCR_name) {
      choice = ModelDescr_name;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == MODEL_DESCR_pdb_reso) {
      choice = ModelDescr_pdb_reso;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == MODEL_DESCR_pdb_method) {
      choice = ModelDescr_pdb_method;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == MODEL_DESCR_pdb_comment) {
      choice = ModelDescr_pdb_comment;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == MODEL_DESCR_other_comment) {
      choice = ModelDescr_other_comment;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == MODEL_DESCR_attribution) {
      choice = ModelDescr_attribution;
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
*    ModelDescrAsnWrite()
*
**************************************************/
Boolean LIBCALL 
ModelDescrAsnWrite(ModelDescrPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, MODEL_DESCR);   /* link local tree */
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
   case ModelDescr_name:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, MODEL_DESCR_name, &av);
      break;
   case ModelDescr_pdb_reso:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, MODEL_DESCR_pdb_reso, &av);
      break;
   case ModelDescr_pdb_method:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, MODEL_DESCR_pdb_method, &av);
      break;
   case ModelDescr_pdb_comment:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, MODEL_DESCR_pdb_comment, &av);
      break;
   case ModelDescr_other_comment:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, MODEL_DESCR_other_comment, &av);
      break;
   case ModelDescr_attribution:
      writetype = MODEL_DESCR_attribution;
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
*    ModelSpaceNew()
*
**************************************************/

ModelSpacePtr LIBCALL
ModelSpaceNew(void)
{
   ModelSpacePtr ptr = MemNew((size_t) sizeof(ModelSpace));

   return ptr;

}


/**************************************************
*
*    ModelSpaceFree()
*
**************************************************/

ModelSpacePtr LIBCALL
ModelSpaceFree(ModelSpacePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ReferenceFrameFree(ptr -> reference_frame);
   return MemFree(ptr);
}


/**************************************************
*
*    ModelSpaceAsnRead()
*
**************************************************/

ModelSpacePtr LIBCALL
ModelSpaceAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ModelSpacePtr ptr;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ModelSpace ::= (self contained) */
      atp = AsnReadId(aip, amp, MODEL_SPACE);
   } else {
      atp = AsnLinkType(orig, MODEL_SPACE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ModelSpaceNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MODEL_SPACE_coordinate_units) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> coordinate_units = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MODEL_SPACE_thermal_factor_units) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> thermal_factor_units = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MODEL_SPACE_occupancy_factor_units) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> occupancy_factor_units = av.intvalue;
      ptr -> OBbits__ |= 1<<1;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MODEL_SPACE_density_units) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> density_units = av.intvalue;
      ptr -> OBbits__ |= 1<<2;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MODEL_SPACE_reference_frame) {
      ptr -> reference_frame = ReferenceFrameAsnRead(aip, atp);
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
   ptr = ModelSpaceFree(ptr);
   goto ret;
}



/**************************************************
*
*    ModelSpaceAsnWrite()
*
**************************************************/
Boolean LIBCALL 
ModelSpaceAsnWrite(ModelSpacePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MODEL_SPACE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> coordinate_units;
   retval = AsnWrite(aip, MODEL_SPACE_coordinate_units,  &av);
   if (ptr -> thermal_factor_units || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> thermal_factor_units;
      retval = AsnWrite(aip, MODEL_SPACE_thermal_factor_units,  &av);
   }
   if (ptr -> occupancy_factor_units || (ptr -> OBbits__ & (1<<1) )){   av.intvalue = ptr -> occupancy_factor_units;
      retval = AsnWrite(aip, MODEL_SPACE_occupancy_factor_units,  &av);
   }
   if (ptr -> density_units || (ptr -> OBbits__ & (1<<2) )){   av.intvalue = ptr -> density_units;
      retval = AsnWrite(aip, MODEL_SPACE_density_units,  &av);
   }
   if (ptr -> reference_frame != NULL) {
      if ( ! ReferenceFrameAsnWrite(ptr -> reference_frame, aip, MODEL_SPACE_reference_frame)) {
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
*    ModelCoordinateSetNew()
*
**************************************************/

ModelCoordinateSetPtr LIBCALL
ModelCoordinateSetNew(void)
{
   ModelCoordinateSetPtr ptr = MemNew((size_t) sizeof(ModelCoordinateSet));

   return ptr;

}


/**************************************************
*
*    ModelCoordinateSetFree()
*
**************************************************/

ModelCoordinateSetPtr LIBCALL
ModelCoordinateSetFree(ModelCoordinateSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr -> descr, (AsnOptFreeFunc) ModelDescrFree);
   Coordinates_coordinatesFree(ptr -> Coordinates_coordinates);
   return MemFree(ptr);
}


/**************************************************
*
*    Coordinates_coordinatesFree()
*
**************************************************/
static 
Coordinates_coordinatesPtr LIBCALL
Coordinates_coordinatesFree(ValNodePtr anp)
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
   case Coordinates_coordinates_literal:
      CoordinatesFree(anp -> data.ptrvalue);
      break;
   case Coordinates_coordinates_reference:
      ChemGraphAlignmentFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    ModelCoordinateSetAsnRead()
*
**************************************************/

ModelCoordinateSetPtr LIBCALL
ModelCoordinateSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ModelCoordinateSetPtr ptr;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ModelCoordinateSet ::= (self contained) */
      atp = AsnReadId(aip, amp, MODEL_COORDINATE_SET);
   } else {
      atp = AsnLinkType(orig, MODEL_COORDINATE_SET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ModelCoordinateSetNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MODEL_COORDINATE_SET_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> id = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MODEL_COORDINATE_SET_descr) {
      ptr -> descr = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ModelDescrAsnRead, (AsnOptFreeFunc) ModelDescrFree);
      if (isError && ptr -> descr == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MODEL_COORDINATE_SET_coordinates) {
      ptr -> Coordinates_coordinates = Coordinates_coordinatesAsnRead(aip, atp);
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
   ptr = ModelCoordinateSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    Coordinates_coordinatesAsnRead()
*
**************************************************/
static 
Coordinates_coordinatesPtr LIBCALL
Coordinates_coordinatesAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objmmdb2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Coordinates_coordinates ::= (self contained) */
      atp = AsnReadId(aip, amp, MODEL_COORDINATE_SET_coordinates);
   } else {
      atp = AsnLinkType(orig, MODEL_COORDINATE_SET_coordinates);    /* link in local tree */
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
   if (atp == MODEL_COORDINATE_SET_coordinates_literal) {
      choice = Coordinates_coordinates_literal;
      func = (AsnReadFunc) CoordinatesAsnRead;
   }
   else if (atp == MODEL_COORDINATE_SET_coordinates_reference) {
      choice = Coordinates_coordinates_reference;
      func = (AsnReadFunc) ChemGraphAlignmentAsnRead;
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
*    ModelCoordinateSetAsnWrite()
*
**************************************************/
Boolean LIBCALL 
ModelCoordinateSetAsnWrite(ModelCoordinateSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MODEL_COORDINATE_SET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> id || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> id;
      retval = AsnWrite(aip, MODEL_COORDINATE_SET_id,  &av);
   }
   AsnGenericChoiceSeqOfAsnWrite(ptr -> descr, (AsnWriteFunc) ModelDescrAsnWrite, aip, MODEL_COORDINATE_SET_descr, MODEL_COORDINATE_SET_descr_E);
   if (ptr -> Coordinates_coordinates != NULL) {
      if ( ! Coordinates_coordinatesAsnWrite(ptr -> Coordinates_coordinates, aip, MODEL_COORDINATE_SET_coordinates)) {
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
*    Coordinates_coordinatesAsnWrite()
*
**************************************************/
static Boolean LIBCALL 
Coordinates_coordinatesAsnWrite(Coordinates_coordinatesPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, MODEL_COORDINATE_SET_coordinates);   /* link local tree */
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
   case Coordinates_coordinates_literal:
      writetype = MODEL_COORDINATE_SET_coordinates_literal;
      func = (AsnWriteFunc) CoordinatesAsnWrite;
      break;
   case Coordinates_coordinates_reference:
      writetype = MODEL_COORDINATE_SET_coordinates_reference;
      func = (AsnWriteFunc) ChemGraphAlignmentAsnWrite;
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
*    ReferenceFrameNew()
*
**************************************************/

ReferenceFramePtr LIBCALL
ReferenceFrameNew(void)
{
   ReferenceFramePtr ptr = MemNew((size_t) sizeof(ReferenceFrame));

   return ptr;

}


/**************************************************
*
*    ReferenceFrameFree()
*
**************************************************/

ReferenceFramePtr LIBCALL
ReferenceFrameFree(ReferenceFramePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   BiostrucIdFree(ptr -> biostruc_id);
   TransformFree(ptr -> rotation_translation);
   return MemFree(ptr);
}


/**************************************************
*
*    ReferenceFrameAsnRead()
*
**************************************************/

ReferenceFramePtr LIBCALL
ReferenceFrameAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ReferenceFramePtr ptr;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ReferenceFrame ::= (self contained) */
      atp = AsnReadId(aip, amp, REFERENCE_FRAME);
   } else {
      atp = AsnLinkType(orig, REFERENCE_FRAME);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ReferenceFrameNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == REFERENCE_FRAME_biostruc_id) {
      ptr -> biostruc_id = BiostrucIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == REFERENCE_FRAME_rotation_translation) {
      ptr -> rotation_translation = TransformAsnRead(aip, atp);
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
   ptr = ReferenceFrameFree(ptr);
   goto ret;
}



/**************************************************
*
*    ReferenceFrameAsnWrite()
*
**************************************************/
Boolean LIBCALL 
ReferenceFrameAsnWrite(ReferenceFramePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, REFERENCE_FRAME);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> biostruc_id != NULL) {
      if ( ! BiostrucIdAsnWrite(ptr -> biostruc_id, aip, REFERENCE_FRAME_biostruc_id)) {
         goto erret;
      }
   }
   if (ptr -> rotation_translation != NULL) {
      if ( ! TransformAsnWrite(ptr -> rotation_translation, aip, REFERENCE_FRAME_rotation_translation)) {
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
*    CoordinatesFree()
*
**************************************************/

CoordinatesPtr LIBCALL
CoordinatesFree(ValNodePtr anp)
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
   case Coordinates_atomic:
      AtomicCoordinatesFree(anp -> data.ptrvalue);
      break;
   case Coordinates_surface:
      SurfaceCoordinatesFree(anp -> data.ptrvalue);
      break;
   case Coordinates_density:
      DensityCoordinatesFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    CoordinatesAsnRead()
*
**************************************************/

CoordinatesPtr LIBCALL
CoordinatesAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objmmdb2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Coordinates ::= (self contained) */
      atp = AsnReadId(aip, amp, COORDINATES);
   } else {
      atp = AsnLinkType(orig, COORDINATES);    /* link in local tree */
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
   if (atp == COORDINATES_atomic) {
      choice = Coordinates_atomic;
      func = (AsnReadFunc) AtomicCoordinatesAsnRead;
   }
   else if (atp == COORDINATES_surface) {
      choice = Coordinates_surface;
      func = (AsnReadFunc) SurfaceCoordinatesAsnRead;
   }
   else if (atp == COORDINATES_density) {
      choice = Coordinates_density;
      func = (AsnReadFunc) DensityCoordinatesAsnRead;
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
*    CoordinatesAsnWrite()
*
**************************************************/
Boolean LIBCALL 
CoordinatesAsnWrite(CoordinatesPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, COORDINATES);   /* link local tree */
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
   case Coordinates_atomic:
      writetype = COORDINATES_atomic;
      func = (AsnWriteFunc) AtomicCoordinatesAsnWrite;
      break;
   case Coordinates_surface:
      writetype = COORDINATES_surface;
      func = (AsnWriteFunc) SurfaceCoordinatesAsnWrite;
      break;
   case Coordinates_density:
      writetype = COORDINATES_density;
      func = (AsnWriteFunc) DensityCoordinatesAsnWrite;
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
*    AtomicCoordinatesNew()
*
**************************************************/

AtomicCoordinatesPtr LIBCALL
AtomicCoordinatesNew(void)
{
   AtomicCoordinatesPtr ptr = MemNew((size_t) sizeof(AtomicCoordinates));

   return ptr;

}


/**************************************************
*
*    AtomicCoordinatesFree()
*
**************************************************/

AtomicCoordinatesPtr LIBCALL
AtomicCoordinatesFree(AtomicCoordinatesPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AtomPntrsFree(ptr -> atoms);
   ModelSpacePointsFree(ptr -> sites);
   AtomicTemperatureFactorsFree(ptr -> temperature_factors);
   AtomicOccupanciesFree(ptr -> occupancies);
   AlternateConformationIdsFree(ptr -> alternate_conf_ids);
   AsnGenericUserSeqOfFree(ptr -> conf_ensembles, (AsnOptFreeFunc) ConformationEnsembleFree);
   return MemFree(ptr);
}


/**************************************************
*
*    AtomicCoordinatesAsnRead()
*
**************************************************/

AtomicCoordinatesPtr LIBCALL
AtomicCoordinatesAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AtomicCoordinatesPtr ptr;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AtomicCoordinates ::= (self contained) */
      atp = AsnReadId(aip, amp, ATOMIC_COORDINATES);
   } else {
      atp = AsnLinkType(orig, ATOMIC_COORDINATES);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = AtomicCoordinatesNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ATOMIC_COORDINATES_number_of_points) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> number_of_points = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ATOMIC_COORDINATES_atoms) {
      ptr -> atoms = AtomPntrsAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ATOMIC_COORDINATES_sites) {
      ptr -> sites = ModelSpacePointsAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ATOMIC_COORDINATES_temperature_factors) {
      ptr -> temperature_factors = AtomicTemperatureFactorsAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ATOMIC_COORDINATES_occupancies) {
      ptr -> occupancies = AtomicOccupanciesAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ATOMIC_COORDINATES_alternate_conf_ids) {
      ptr -> alternate_conf_ids = AlternateConformationIdsAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ATOMIC_COORDINATES_conf_ensembles) {
      ptr -> conf_ensembles = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ConformationEnsembleAsnRead, (AsnOptFreeFunc) ConformationEnsembleFree);
      if (isError && ptr -> conf_ensembles == NULL) {
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
   ptr = AtomicCoordinatesFree(ptr);
   goto ret;
}



/**************************************************
*
*    AtomicCoordinatesAsnWrite()
*
**************************************************/
Boolean LIBCALL 
AtomicCoordinatesAsnWrite(AtomicCoordinatesPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ATOMIC_COORDINATES);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> number_of_points;
   retval = AsnWrite(aip, ATOMIC_COORDINATES_number_of_points,  &av);
   if (ptr -> atoms != NULL) {
      if ( ! AtomPntrsAsnWrite(ptr -> atoms, aip, ATOMIC_COORDINATES_atoms)) {
         goto erret;
      }
   }
   if (ptr -> sites != NULL) {
      if ( ! ModelSpacePointsAsnWrite(ptr -> sites, aip, ATOMIC_COORDINATES_sites)) {
         goto erret;
      }
   }
   if (ptr -> temperature_factors != NULL) {
      if ( ! AtomicTemperatureFactorsAsnWrite(ptr -> temperature_factors, aip, ATOMIC_COORDINATES_temperature_factors)) {
         goto erret;
      }
   }
   if (ptr -> occupancies != NULL) {
      if ( ! AtomicOccupanciesAsnWrite(ptr -> occupancies, aip, ATOMIC_COORDINATES_occupancies)) {
         goto erret;
      }
   }
   if (ptr -> alternate_conf_ids != NULL) {
      if ( ! AlternateConformationIdsAsnWrite(ptr -> alternate_conf_ids, aip, ATOMIC_COORDINATES_alternate_conf_ids)) {
         goto erret;
      }
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> conf_ensembles, (AsnWriteFunc) ConformationEnsembleAsnWrite, aip, ATOMIC_COORDINATES_conf_ensembles, ATOMIC_COORDINATES_conf_ensembles_E);
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
*    SurfaceCoordinatesNew()
*
**************************************************/

SurfaceCoordinatesPtr LIBCALL
SurfaceCoordinatesNew(void)
{
   SurfaceCoordinatesPtr ptr = MemNew((size_t) sizeof(SurfaceCoordinates));

   return ptr;

}


/**************************************************
*
*    SurfaceCoordinatesFree()
*
**************************************************/

SurfaceCoordinatesPtr LIBCALL
SurfaceCoordinatesFree(SurfaceCoordinatesPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ChemGraphPntrsFree(ptr -> contents);
   Surface_surfaceFree(ptr -> Surface_surface);
   return MemFree(ptr);
}


/**************************************************
*
*    Surface_surfaceFree()
*
**************************************************/
static 
Surface_surfacePtr LIBCALL
Surface_surfaceFree(ValNodePtr anp)
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
   case Surface_surface_sphere:
      SphereFree(anp -> data.ptrvalue);
      break;
   case Surface_surface_cone:
      ConeFree(anp -> data.ptrvalue);
      break;
   case Surface_surface_cylinder:
      CylinderFree(anp -> data.ptrvalue);
      break;
   case Surface_surface_brick:
      BrickFree(anp -> data.ptrvalue);
      break;
   case Surface_surface_tmesh:
      TMeshFree(anp -> data.ptrvalue);
      break;
   case Surface_surface_triangles:
      TrianglesFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    SurfaceCoordinatesAsnRead()
*
**************************************************/

SurfaceCoordinatesPtr LIBCALL
SurfaceCoordinatesAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   SurfaceCoordinatesPtr ptr;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* SurfaceCoordinates ::= (self contained) */
      atp = AsnReadId(aip, amp, SURFACE_COORDINATES);
   } else {
      atp = AsnLinkType(orig, SURFACE_COORDINATES);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = SurfaceCoordinatesNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == SURFACE_COORDINATES_contents) {
      ptr -> contents = ChemGraphPntrsAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SURFACE_COORDINATES_surface) {
      ptr -> Surface_surface = Surface_surfaceAsnRead(aip, atp);
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
   ptr = SurfaceCoordinatesFree(ptr);
   goto ret;
}



/**************************************************
*
*    Surface_surfaceAsnRead()
*
**************************************************/
static 
Surface_surfacePtr LIBCALL
Surface_surfaceAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objmmdb2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Surface_surface ::= (self contained) */
      atp = AsnReadId(aip, amp, SURFACE_COORDINATES_surface);
   } else {
      atp = AsnLinkType(orig, SURFACE_COORDINATES_surface);    /* link in local tree */
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
   if (atp == SURFACE_COORDINATES_surface_sphere) {
      choice = Surface_surface_sphere;
      func = (AsnReadFunc) SphereAsnRead;
   }
   else if (atp == SURFACE_COORDINATES_surface_cone) {
      choice = Surface_surface_cone;
      func = (AsnReadFunc) ConeAsnRead;
   }
   else if (atp == SURFACE_COORDINATES_surface_cylinder) {
      choice = Surface_surface_cylinder;
      func = (AsnReadFunc) CylinderAsnRead;
   }
   else if (atp == SURFACE_COORDINATES_surface_brick) {
      choice = Surface_surface_brick;
      func = (AsnReadFunc) BrickAsnRead;
   }
   else if (atp == SURFACE_COORDINATES_surface_tmesh) {
      choice = Surface_surface_tmesh;
      func = (AsnReadFunc) TMeshAsnRead;
   }
   else if (atp == SURFACE_COORDINATES_surface_triangles) {
      choice = Surface_surface_triangles;
      func = (AsnReadFunc) TrianglesAsnRead;
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
*    SurfaceCoordinatesAsnWrite()
*
**************************************************/
Boolean LIBCALL 
SurfaceCoordinatesAsnWrite(SurfaceCoordinatesPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, SURFACE_COORDINATES);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> contents != NULL) {
      if ( ! ChemGraphPntrsAsnWrite(ptr -> contents, aip, SURFACE_COORDINATES_contents)) {
         goto erret;
      }
   }
   if (ptr -> Surface_surface != NULL) {
      if ( ! Surface_surfaceAsnWrite(ptr -> Surface_surface, aip, SURFACE_COORDINATES_surface)) {
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
*    Surface_surfaceAsnWrite()
*
**************************************************/
static Boolean LIBCALL 
Surface_surfaceAsnWrite(Surface_surfacePtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, SURFACE_COORDINATES_surface);   /* link local tree */
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
   case Surface_surface_sphere:
      writetype = SURFACE_COORDINATES_surface_sphere;
      func = (AsnWriteFunc) SphereAsnWrite;
      break;
   case Surface_surface_cone:
      writetype = SURFACE_COORDINATES_surface_cone;
      func = (AsnWriteFunc) ConeAsnWrite;
      break;
   case Surface_surface_cylinder:
      writetype = SURFACE_COORDINATES_surface_cylinder;
      func = (AsnWriteFunc) CylinderAsnWrite;
      break;
   case Surface_surface_brick:
      writetype = SURFACE_COORDINATES_surface_brick;
      func = (AsnWriteFunc) BrickAsnWrite;
      break;
   case Surface_surface_tmesh:
      writetype = SURFACE_COORDINATES_surface_tmesh;
      func = (AsnWriteFunc) TMeshAsnWrite;
      break;
   case Surface_surface_triangles:
      writetype = SURFACE_COORDINATES_surface_triangles;
      func = (AsnWriteFunc) TrianglesAsnWrite;
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
*    DensityCoordinatesNew()
*
**************************************************/

DensityCoordinatesPtr LIBCALL
DensityCoordinatesNew(void)
{
   DensityCoordinatesPtr ptr = MemNew((size_t) sizeof(DensityCoordinates));

   return ptr;

}


/**************************************************
*
*    DensityCoordinatesFree()
*
**************************************************/

DensityCoordinatesPtr LIBCALL
DensityCoordinatesFree(DensityCoordinatesPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ChemGraphPntrsFree(ptr -> contents);
   BrickFree(ptr -> grid_corners);
   AsnGenericBaseSeqOfFree(ptr -> density ,ASNCODE_INTVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    DensityCoordinatesAsnRead()
*
**************************************************/

DensityCoordinatesPtr LIBCALL
DensityCoordinatesAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   DensityCoordinatesPtr ptr;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* DensityCoordinates ::= (self contained) */
      atp = AsnReadId(aip, amp, DENSITY_COORDINATES);
   } else {
      atp = AsnLinkType(orig, DENSITY_COORDINATES);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = DensityCoordinatesNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == DENSITY_COORDINATES_contents) {
      ptr -> contents = ChemGraphPntrsAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DENSITY_COORDINATES_grid_corners) {
      ptr -> grid_corners = BrickAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DENSITY_COORDINATES_grid_steps_x) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> grid_steps_x = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DENSITY_COORDINATES_grid_steps_y) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> grid_steps_y = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DENSITY_COORDINATES_grid_steps_z) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> grid_steps_z = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DENSITY_COORDINATES_fastest_varying) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> fastest_varying = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DENSITY_COORDINATES_slowest_varying) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> slowest_varying = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DENSITY_COORDINATES_scale_factor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> scale_factor = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DENSITY_COORDINATES_density) {
      ptr -> density = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> density == NULL) {
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
   ptr = DensityCoordinatesFree(ptr);
   goto ret;
}



/**************************************************
*
*    DensityCoordinatesAsnWrite()
*
**************************************************/
Boolean LIBCALL 
DensityCoordinatesAsnWrite(DensityCoordinatesPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, DENSITY_COORDINATES);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> contents != NULL) {
      if ( ! ChemGraphPntrsAsnWrite(ptr -> contents, aip, DENSITY_COORDINATES_contents)) {
         goto erret;
      }
   }
   if (ptr -> grid_corners != NULL) {
      if ( ! BrickAsnWrite(ptr -> grid_corners, aip, DENSITY_COORDINATES_grid_corners)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> grid_steps_x;
   retval = AsnWrite(aip, DENSITY_COORDINATES_grid_steps_x,  &av);
   av.intvalue = ptr -> grid_steps_y;
   retval = AsnWrite(aip, DENSITY_COORDINATES_grid_steps_y,  &av);
   av.intvalue = ptr -> grid_steps_z;
   retval = AsnWrite(aip, DENSITY_COORDINATES_grid_steps_z,  &av);
   av.intvalue = ptr -> fastest_varying;
   retval = AsnWrite(aip, DENSITY_COORDINATES_fastest_varying,  &av);
   av.intvalue = ptr -> slowest_varying;
   retval = AsnWrite(aip, DENSITY_COORDINATES_slowest_varying,  &av);
   av.intvalue = ptr -> scale_factor;
   retval = AsnWrite(aip, DENSITY_COORDINATES_scale_factor,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> density ,ASNCODE_INTVAL_SLOT, aip, DENSITY_COORDINATES_density, DENSITY_COORDINATES_density_E);
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
*    ModelSpacePointsNew()
*
**************************************************/

ModelSpacePointsPtr LIBCALL
ModelSpacePointsNew(void)
{
   ModelSpacePointsPtr ptr = MemNew((size_t) sizeof(ModelSpacePoints));

   return ptr;

}


/**************************************************
*
*    ModelSpacePointsFree()
*
**************************************************/

ModelSpacePointsPtr LIBCALL
ModelSpacePointsFree(ModelSpacePointsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericBaseSeqOfFree(ptr -> x ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> y ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> z ,ASNCODE_INTVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    ModelSpacePointsAsnRead()
*
**************************************************/

ModelSpacePointsPtr LIBCALL
ModelSpacePointsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ModelSpacePointsPtr ptr;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ModelSpacePoints ::= (self contained) */
      atp = AsnReadId(aip, amp, MODEL_SPACE_POINTS);
   } else {
      atp = AsnLinkType(orig, MODEL_SPACE_POINTS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ModelSpacePointsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MODEL_SPACE_POINTS_scale_factor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> scale_factor = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MODEL_SPACE_POINTS_x) {
      ptr -> x = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> x == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MODEL_SPACE_POINTS_y) {
      ptr -> y = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> y == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MODEL_SPACE_POINTS_z) {
      ptr -> z = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> z == NULL) {
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
   ptr = ModelSpacePointsFree(ptr);
   goto ret;
}



/**************************************************
*
*    ModelSpacePointsAsnWrite()
*
**************************************************/
Boolean LIBCALL 
ModelSpacePointsAsnWrite(ModelSpacePointsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MODEL_SPACE_POINTS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> scale_factor;
   retval = AsnWrite(aip, MODEL_SPACE_POINTS_scale_factor,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> x ,ASNCODE_INTVAL_SLOT, aip, MODEL_SPACE_POINTS_x, MODEL_SPACE_POINTS_x_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> y ,ASNCODE_INTVAL_SLOT, aip, MODEL_SPACE_POINTS_y, MODEL_SPACE_POINTS_y_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> z ,ASNCODE_INTVAL_SLOT, aip, MODEL_SPACE_POINTS_z, MODEL_SPACE_POINTS_z_E);
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
*    AtomicTemperatureFactorsFree()
*
**************************************************/

AtomicTemperatureFactorsPtr LIBCALL
AtomicTemperatureFactorsFree(ValNodePtr anp)
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
   case AtomicTemperatureFactors_isotropic:
      IsotropicTemperatureFactorsFree(anp -> data.ptrvalue);
      break;
   case AtomicTemperatureFactors_anisotropic:
      AnisotropicTemperatureFactorsFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    AtomicTemperatureFactorsAsnRead()
*
**************************************************/

AtomicTemperatureFactorsPtr LIBCALL
AtomicTemperatureFactorsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objmmdb2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AtomicTemperatureFactors ::= (self contained) */
      atp = AsnReadId(aip, amp, ATOMIC_TEMPERATURE_FACTORS);
   } else {
      atp = AsnLinkType(orig, ATOMIC_TEMPERATURE_FACTORS);    /* link in local tree */
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
   if (atp == ATOMIC_TEMPERATURE_FACTORS_isotropic) {
      choice = AtomicTemperatureFactors_isotropic;
      func = (AsnReadFunc) IsotropicTemperatureFactorsAsnRead;
   }
   else if (atp == ATOMIC_TEMPERATURE_FACTORS_anisotropic) {
      choice = AtomicTemperatureFactors_anisotropic;
      func = (AsnReadFunc) AnisotropicTemperatureFactorsAsnRead;
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
*    AtomicTemperatureFactorsAsnWrite()
*
**************************************************/
Boolean LIBCALL 
AtomicTemperatureFactorsAsnWrite(AtomicTemperatureFactorsPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, ATOMIC_TEMPERATURE_FACTORS);   /* link local tree */
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
   case AtomicTemperatureFactors_isotropic:
      writetype = ATOMIC_TEMPERATURE_FACTORS_isotropic;
      func = (AsnWriteFunc) IsotropicTemperatureFactorsAsnWrite;
      break;
   case AtomicTemperatureFactors_anisotropic:
      writetype = ATOMIC_TEMPERATURE_FACTORS_anisotropic;
      func = (AsnWriteFunc) AnisotropicTemperatureFactorsAsnWrite;
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
*    AtomicOccupanciesNew()
*
**************************************************/

AtomicOccupanciesPtr LIBCALL
AtomicOccupanciesNew(void)
{
   AtomicOccupanciesPtr ptr = MemNew((size_t) sizeof(AtomicOccupancies));

   return ptr;

}


/**************************************************
*
*    AtomicOccupanciesFree()
*
**************************************************/

AtomicOccupanciesPtr LIBCALL
AtomicOccupanciesFree(AtomicOccupanciesPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericBaseSeqOfFree(ptr -> o ,ASNCODE_INTVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    AtomicOccupanciesAsnRead()
*
**************************************************/

AtomicOccupanciesPtr LIBCALL
AtomicOccupanciesAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AtomicOccupanciesPtr ptr;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AtomicOccupancies ::= (self contained) */
      atp = AsnReadId(aip, amp, ATOMIC_OCCUPANCIES);
   } else {
      atp = AsnLinkType(orig, ATOMIC_OCCUPANCIES);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = AtomicOccupanciesNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ATOMIC_OCCUPANCIES_scale_factor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> scale_factor = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ATOMIC_OCCUPANCIES_o) {
      ptr -> o = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> o == NULL) {
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
   ptr = AtomicOccupanciesFree(ptr);
   goto ret;
}



/**************************************************
*
*    AtomicOccupanciesAsnWrite()
*
**************************************************/
Boolean LIBCALL 
AtomicOccupanciesAsnWrite(AtomicOccupanciesPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ATOMIC_OCCUPANCIES);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> scale_factor;
   retval = AsnWrite(aip, ATOMIC_OCCUPANCIES_scale_factor,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> o ,ASNCODE_INTVAL_SLOT, aip, ATOMIC_OCCUPANCIES_o, ATOMIC_OCCUPANCIES_o_E);
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
*    AlternateConformationIdsFree()
*
**************************************************/

AlternateConformationIdsPtr LIBCALL
AlternateConformationIdsFree(AlternateConformationIdsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericBaseSeqOfFree(ptr,ASNCODE_PTRVAL_SLOT);
   return NULL;
}


/**************************************************
*
*    AlternateConformationIdsAsnRead()
*
**************************************************/

AlternateConformationIdsPtr LIBCALL
AlternateConformationIdsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AlternateConformationIdsPtr ptr;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AlternateConformationIds ::= (self contained) */
      atp = AsnReadId(aip, amp, ALTERNATE_CONFORMATION_IDS);
   } else {
      atp = AsnLinkType(orig, ALTERNATE_CONFORMATION_IDS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = AlternateConformationIdsFree(ptr);
   goto ret;
}



/**************************************************
*
*    AlternateConformationIdsAsnWrite()
*
**************************************************/
Boolean LIBCALL 
AlternateConformationIdsAsnWrite(AlternateConformationIdsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ALTERNATE_CONFORMATION_IDS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr, ASNCODE_PTRVAL_SLOT, aip, atp, ALTERNATE_CONFORMATION_IDS_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    ConformationEnsembleNew()
*
**************************************************/

ConformationEnsemblePtr LIBCALL
ConformationEnsembleNew(void)
{
   ConformationEnsemblePtr ptr = MemNew((size_t) sizeof(ConformationEnsemble));

   return ptr;

}


/**************************************************
*
*    ConformationEnsembleFree()
*
**************************************************/

ConformationEnsemblePtr LIBCALL
ConformationEnsembleFree(ConformationEnsemblePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   AsnGenericBaseSeqOfFree(ptr -> alt_conf_ids ,ASNCODE_PTRVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    ConformationEnsembleAsnRead()
*
**************************************************/

ConformationEnsemblePtr LIBCALL
ConformationEnsembleAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ConformationEnsemblePtr ptr;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ConformationEnsemble ::= (self contained) */
      atp = AsnReadId(aip, amp, CONFORMATION_ENSEMBLE);
   } else {
      atp = AsnLinkType(orig, CONFORMATION_ENSEMBLE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ConformationEnsembleNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CONFORMATION_ENSEMBLE_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONFORMATION_ENSEMBLE_alt_conf_ids) {
      ptr -> alt_conf_ids = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> alt_conf_ids == NULL) {
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
   ptr = ConformationEnsembleFree(ptr);
   goto ret;
}



/**************************************************
*
*    ConformationEnsembleAsnWrite()
*
**************************************************/
Boolean LIBCALL 
ConformationEnsembleAsnWrite(ConformationEnsemblePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CONFORMATION_ENSEMBLE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, CONFORMATION_ENSEMBLE_name,  &av);
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> alt_conf_ids ,ASNCODE_PTRVAL_SLOT, aip, CONFORMATION_ENSEMBLE_alt_conf_ids, CONFORMATION_ENSEMBLE_alt_conf_ids_E);
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
*    IsotropicTemperatureFactorsNew()
*
**************************************************/

IsotropicTemperatureFactorsPtr LIBCALL
IsotropicTemperatureFactorsNew(void)
{
   IsotropicTemperatureFactorsPtr ptr = MemNew((size_t) sizeof(IsotropicTemperatureFactors));

   return ptr;

}


/**************************************************
*
*    IsotropicTemperatureFactorsFree()
*
**************************************************/

IsotropicTemperatureFactorsPtr LIBCALL
IsotropicTemperatureFactorsFree(IsotropicTemperatureFactorsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericBaseSeqOfFree(ptr -> b ,ASNCODE_INTVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    IsotropicTemperatureFactorsAsnRead()
*
**************************************************/

IsotropicTemperatureFactorsPtr LIBCALL
IsotropicTemperatureFactorsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   IsotropicTemperatureFactorsPtr ptr;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* IsotropicTemperatureFactors ::= (self contained) */
      atp = AsnReadId(aip, amp, ISOTROPIC_TEMPERATURE_FACTORS);
   } else {
      atp = AsnLinkType(orig, ISOTROPIC_TEMPERATURE_FACTORS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = IsotropicTemperatureFactorsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ISOTROPIC_TEMPERATURE_FACTORS_scale_factor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> scale_factor = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ISOTROPIC_TEMPERATURE_FACTORS_b) {
      ptr -> b = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> b == NULL) {
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
   ptr = IsotropicTemperatureFactorsFree(ptr);
   goto ret;
}



/**************************************************
*
*    IsotropicTemperatureFactorsAsnWrite()
*
**************************************************/
Boolean LIBCALL 
IsotropicTemperatureFactorsAsnWrite(IsotropicTemperatureFactorsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ISOTROPIC_TEMPERATURE_FACTORS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> scale_factor;
   retval = AsnWrite(aip, ISOTROPIC_TEMPERATURE_FACTORS_scale_factor,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> b ,ASNCODE_INTVAL_SLOT, aip, ISOTROPIC_TEMPERATURE_FACTORS_b, ISOTROPIC_TEMPERATURE_FACTORS_b_E);
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
*    AnisotropicTemperatureFactorsNew()
*
**************************************************/

AnisotropicTemperatureFactorsPtr LIBCALL
AnisotropicTemperatureFactorsNew(void)
{
   AnisotropicTemperatureFactorsPtr ptr = MemNew((size_t) sizeof(AnisotropicTemperatureFactors));

   return ptr;

}


/**************************************************
*
*    AnisotropicTemperatureFactorsFree()
*
**************************************************/

AnisotropicTemperatureFactorsPtr LIBCALL
AnisotropicTemperatureFactorsFree(AnisotropicTemperatureFactorsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericBaseSeqOfFree(ptr -> b_11 ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> b_12 ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> b_13 ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> b_22 ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> b_23 ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> b_33 ,ASNCODE_INTVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    AnisotropicTemperatureFactorsAsnRead()
*
**************************************************/

AnisotropicTemperatureFactorsPtr LIBCALL
AnisotropicTemperatureFactorsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AnisotropicTemperatureFactorsPtr ptr;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AnisotropicTemperatureFactors ::= (self contained) */
      atp = AsnReadId(aip, amp, ANISOTROPIC_TEMPERATURE_FACTORS);
   } else {
      atp = AsnLinkType(orig, ANISOTROPIC_TEMPERATURE_FACTORS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = AnisotropicTemperatureFactorsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ANISOTROPIC_TEMPERATURE_FACTORS_scale_factor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> scale_factor = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ANISOTROPIC_TEMPERATURE_FACTORS_b_11) {
      ptr -> b_11 = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> b_11 == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ANISOTROPIC_TEMPERATURE_FACTORS_b_12) {
      ptr -> b_12 = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> b_12 == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ANISOTROPIC_TEMPERATURE_FACTORS_b_13) {
      ptr -> b_13 = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> b_13 == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ANISOTROPIC_TEMPERATURE_FACTORS_b_22) {
      ptr -> b_22 = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> b_22 == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ANISOTROPIC_TEMPERATURE_FACTORS_b_23) {
      ptr -> b_23 = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> b_23 == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ANISOTROPIC_TEMPERATURE_FACTORS_b_33) {
      ptr -> b_33 = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> b_33 == NULL) {
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
   ptr = AnisotropicTemperatureFactorsFree(ptr);
   goto ret;
}



/**************************************************
*
*    AnisotropicTemperatureFactorsAsnWrite()
*
**************************************************/
Boolean LIBCALL 
AnisotropicTemperatureFactorsAsnWrite(AnisotropicTemperatureFactorsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ANISOTROPIC_TEMPERATURE_FACTORS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> scale_factor;
   retval = AsnWrite(aip, ANISOTROPIC_TEMPERATURE_FACTORS_scale_factor,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> b_11 ,ASNCODE_INTVAL_SLOT, aip, ANISOTROPIC_TEMPERATURE_FACTORS_b_11, ANISOTROPIC_TEMPERATURE_FACTORS_b_11_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> b_12 ,ASNCODE_INTVAL_SLOT, aip, ANISOTROPIC_TEMPERATURE_FACTORS_b_12, ANISOTROPIC_TEMPERATURE_FACTORS_b_12_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> b_13 ,ASNCODE_INTVAL_SLOT, aip, ANISOTROPIC_TEMPERATURE_FACTORS_b_13, ANISOTROPIC_TEMPERATURE_FACTORS_b_13_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> b_22 ,ASNCODE_INTVAL_SLOT, aip, ANISOTROPIC_TEMPERATURE_FACTORS_b_22, ANISOTROPIC_TEMPERATURE_FACTORS_b_22_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> b_23 ,ASNCODE_INTVAL_SLOT, aip, ANISOTROPIC_TEMPERATURE_FACTORS_b_23, ANISOTROPIC_TEMPERATURE_FACTORS_b_23_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> b_33 ,ASNCODE_INTVAL_SLOT, aip, ANISOTROPIC_TEMPERATURE_FACTORS_b_33, ANISOTROPIC_TEMPERATURE_FACTORS_b_33_E);
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
*    TMeshNew()
*
**************************************************/

TMeshPtr LIBCALL
TMeshNew(void)
{
   TMeshPtr ptr = MemNew((size_t) sizeof(TMesh));

   return ptr;

}


/**************************************************
*
*    TMeshFree()
*
**************************************************/

TMeshPtr LIBCALL
TMeshFree(TMeshPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericBaseSeqOfFree(ptr -> swap ,ASNCODE_BOOLVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> x ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> y ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> z ,ASNCODE_INTVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    TMeshAsnRead()
*
**************************************************/

TMeshPtr LIBCALL
TMeshAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   TMeshPtr ptr;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* TMesh ::= (self contained) */
      atp = AsnReadId(aip, amp, T_MESH);
   } else {
      atp = AsnLinkType(orig, T_MESH);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = TMeshNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == T_MESH_number_of_points) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> number_of_points = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == T_MESH_scale_factor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> scale_factor = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == T_MESH_swap) {
      ptr -> swap = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_BOOLVAL_SLOT, &isError);
      if (isError && ptr -> swap == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == T_MESH_x) {
      ptr -> x = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> x == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == T_MESH_y) {
      ptr -> y = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> y == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == T_MESH_z) {
      ptr -> z = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> z == NULL) {
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
   ptr = TMeshFree(ptr);
   goto ret;
}



/**************************************************
*
*    TMeshAsnWrite()
*
**************************************************/
Boolean LIBCALL 
TMeshAsnWrite(TMeshPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, T_MESH);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> number_of_points;
   retval = AsnWrite(aip, T_MESH_number_of_points,  &av);
   av.intvalue = ptr -> scale_factor;
   retval = AsnWrite(aip, T_MESH_scale_factor,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> swap ,ASNCODE_BOOLVAL_SLOT, aip, T_MESH_swap, T_MESH_swap_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> x ,ASNCODE_INTVAL_SLOT, aip, T_MESH_x, T_MESH_x_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> y ,ASNCODE_INTVAL_SLOT, aip, T_MESH_y, T_MESH_y_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> z ,ASNCODE_INTVAL_SLOT, aip, T_MESH_z, T_MESH_z_E);
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
*    TrianglesNew()
*
**************************************************/

TrianglesPtr LIBCALL
TrianglesNew(void)
{
   TrianglesPtr ptr = MemNew((size_t) sizeof(Triangles));

   return ptr;

}


/**************************************************
*
*    TrianglesFree()
*
**************************************************/

TrianglesPtr LIBCALL
TrianglesFree(TrianglesPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericBaseSeqOfFree(ptr -> x ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> y ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> z ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> v1 ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> v2 ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> v3 ,ASNCODE_INTVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    TrianglesAsnRead()
*
**************************************************/

TrianglesPtr LIBCALL
TrianglesAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   TrianglesPtr ptr;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Triangles ::= (self contained) */
      atp = AsnReadId(aip, amp, TRIANGLES);
   } else {
      atp = AsnLinkType(orig, TRIANGLES);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = TrianglesNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == TRIANGLES_number_of_points) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> number_of_points = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TRIANGLES_scale_factor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> scale_factor = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TRIANGLES_x) {
      ptr -> x = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> x == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TRIANGLES_y) {
      ptr -> y = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> y == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TRIANGLES_z) {
      ptr -> z = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> z == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TRIANGLES_number_of_triangles) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> number_of_triangles = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TRIANGLES_v1) {
      ptr -> v1 = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> v1 == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TRIANGLES_v2) {
      ptr -> v2 = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> v2 == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TRIANGLES_v3) {
      ptr -> v3 = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> v3 == NULL) {
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
   ptr = TrianglesFree(ptr);
   goto ret;
}



/**************************************************
*
*    TrianglesAsnWrite()
*
**************************************************/
Boolean LIBCALL 
TrianglesAsnWrite(TrianglesPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TRIANGLES);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> number_of_points;
   retval = AsnWrite(aip, TRIANGLES_number_of_points,  &av);
   av.intvalue = ptr -> scale_factor;
   retval = AsnWrite(aip, TRIANGLES_scale_factor,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> x ,ASNCODE_INTVAL_SLOT, aip, TRIANGLES_x, TRIANGLES_x_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> y ,ASNCODE_INTVAL_SLOT, aip, TRIANGLES_y, TRIANGLES_y_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> z ,ASNCODE_INTVAL_SLOT, aip, TRIANGLES_z, TRIANGLES_z_E);
   av.intvalue = ptr -> number_of_triangles;
   retval = AsnWrite(aip, TRIANGLES_number_of_triangles,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> v1 ,ASNCODE_INTVAL_SLOT, aip, TRIANGLES_v1, TRIANGLES_v1_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> v2 ,ASNCODE_INTVAL_SLOT, aip, TRIANGLES_v2, TRIANGLES_v2_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> v3 ,ASNCODE_INTVAL_SLOT, aip, TRIANGLES_v3, TRIANGLES_v3_E);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

