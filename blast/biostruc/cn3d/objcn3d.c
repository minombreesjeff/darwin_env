#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <mapcn3d.h>
#include <objcn3d.h>

static Boolean loaded = FALSE;

#include <cn3d.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
objcn3dAsnLoad(void)
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
*    Generated object loaders for Module NCBI-Cn3d
*    Generated using ASNCODE Revision: 6.12 at Oct 17, 2001 12:45 PM
*
**************************************************/


/**************************************************
*
*    Cn3dStyleDictionaryNew()
*
**************************************************/
NLM_EXTERN 
Cn3dStyleDictionaryPtr LIBCALL
Cn3dStyleDictionaryNew(void)
{
   Cn3dStyleDictionaryPtr ptr = MemNew((size_t) sizeof(Cn3dStyleDictionary));

   return ptr;

}


/**************************************************
*
*    Cn3dStyleDictionaryFree()
*
**************************************************/
NLM_EXTERN 
Cn3dStyleDictionaryPtr LIBCALL
Cn3dStyleDictionaryFree(Cn3dStyleDictionaryPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   Cn3dStyleSettingsFree(ptr -> global_style);
   AsnGenericUserSeqOfFree(ptr -> style_table, (AsnOptFreeFunc) Cn3dStyleTableItemFree);
   return MemFree(ptr);
}


/**************************************************
*
*    Cn3dStyleDictionaryAsnRead()
*
**************************************************/
NLM_EXTERN 
Cn3dStyleDictionaryPtr LIBCALL
Cn3dStyleDictionaryAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Cn3dStyleDictionaryPtr ptr;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cn3dStyleDictionary ::= (self contained) */
      atp = AsnReadId(aip, amp, CN3D_STYLE_DICTIONARY);
   } else {
      atp = AsnLinkType(orig, CN3D_STYLE_DICTIONARY);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Cn3dStyleDictionaryNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CN3D_STYLE_DICTIONARY_global_style) {
      ptr -> global_style = Cn3dStyleSettingsAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_DICTIONARY_style_table) {
      ptr -> style_table = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Cn3dStyleTableItemAsnRead, (AsnOptFreeFunc) Cn3dStyleTableItemFree);
      if (isError && ptr -> style_table == NULL) {
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
   ptr = Cn3dStyleDictionaryFree(ptr);
   goto ret;
}



/**************************************************
*
*    Cn3dStyleDictionaryAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Cn3dStyleDictionaryAsnWrite(Cn3dStyleDictionaryPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CN3D_STYLE_DICTIONARY);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> global_style != NULL) {
      if ( ! Cn3dStyleSettingsAsnWrite(ptr -> global_style, aip, CN3D_STYLE_DICTIONARY_global_style)) {
         goto erret;
      }
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> style_table, (AsnWriteFunc) Cn3dStyleTableItemAsnWrite, aip, CN3D_STYLE_DICTIONARY_style_table, CN3D_STYLE_DICTIONARY_style_table_E);
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
*    Cn3dUserAnnotationsNew()
*
**************************************************/
NLM_EXTERN 
Cn3dUserAnnotationsPtr LIBCALL
Cn3dUserAnnotationsNew(void)
{
   Cn3dUserAnnotationsPtr ptr = MemNew((size_t) sizeof(Cn3dUserAnnotations));

   return ptr;

}


/**************************************************
*
*    Cn3dUserAnnotationsFree()
*
**************************************************/
NLM_EXTERN 
Cn3dUserAnnotationsPtr LIBCALL
Cn3dUserAnnotationsFree(Cn3dUserAnnotationsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> annotations, (AsnOptFreeFunc) Cn3dUserAnnotationFree);
   Cn3dViewSettingsFree(ptr -> view);
   return MemFree(ptr);
}


/**************************************************
*
*    Cn3dUserAnnotationsAsnRead()
*
**************************************************/
NLM_EXTERN 
Cn3dUserAnnotationsPtr LIBCALL
Cn3dUserAnnotationsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Cn3dUserAnnotationsPtr ptr;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cn3dUserAnnotations ::= (self contained) */
      atp = AsnReadId(aip, amp, CN3D_USER_ANNOTATIONS);
   } else {
      atp = AsnLinkType(orig, CN3D_USER_ANNOTATIONS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Cn3dUserAnnotationsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CN3D_USER_ANNOTATIONS_annotations) {
      ptr -> annotations = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Cn3dUserAnnotationAsnRead, (AsnOptFreeFunc) Cn3dUserAnnotationFree);
      if (isError && ptr -> annotations == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_USER_ANNOTATIONS_view) {
      ptr -> view = Cn3dViewSettingsAsnRead(aip, atp);
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
   ptr = Cn3dUserAnnotationsFree(ptr);
   goto ret;
}



/**************************************************
*
*    Cn3dUserAnnotationsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Cn3dUserAnnotationsAsnWrite(Cn3dUserAnnotationsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CN3D_USER_ANNOTATIONS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   AsnGenericUserSeqOfAsnWrite(ptr -> annotations, (AsnWriteFunc) Cn3dUserAnnotationAsnWrite, aip, CN3D_USER_ANNOTATIONS_annotations, CN3D_USER_ANNOTATIONS_annotations_E);
   if (ptr -> view != NULL) {
      if ( ! Cn3dViewSettingsAsnWrite(ptr -> view, aip, CN3D_USER_ANNOTATIONS_view)) {
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
*    Cn3dColorNew()
*
**************************************************/
NLM_EXTERN 
Cn3dColorPtr LIBCALL
Cn3dColorNew(void)
{
   Cn3dColorPtr ptr = MemNew((size_t) sizeof(Cn3dColor));

   ptr -> scale_factor = 255;
   ptr -> alpha = 255;
   return ptr;

}


/**************************************************
*
*    Cn3dColorFree()
*
**************************************************/
NLM_EXTERN 
Cn3dColorPtr LIBCALL
Cn3dColorFree(Cn3dColorPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    Cn3dColorAsnRead()
*
**************************************************/
NLM_EXTERN 
Cn3dColorPtr LIBCALL
Cn3dColorAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Cn3dColorPtr ptr;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cn3dColor ::= (self contained) */
      atp = AsnReadId(aip, amp, CN3D_COLOR);
   } else {
      atp = AsnLinkType(orig, CN3D_COLOR);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Cn3dColorNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CN3D_COLOR_scale_factor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> scale_factor = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_COLOR_red) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> red = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_COLOR_green) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> green = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_COLOR_blue) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> blue = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_COLOR_alpha) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> alpha = av.intvalue;
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
   ptr = Cn3dColorFree(ptr);
   goto ret;
}



/**************************************************
*
*    Cn3dColorAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Cn3dColorAsnWrite(Cn3dColorPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CN3D_COLOR);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> scale_factor;
   retval = AsnWrite(aip, CN3D_COLOR_scale_factor,  &av);
   av.intvalue = ptr -> red;
   retval = AsnWrite(aip, CN3D_COLOR_red,  &av);
   av.intvalue = ptr -> green;
   retval = AsnWrite(aip, CN3D_COLOR_green,  &av);
   av.intvalue = ptr -> blue;
   retval = AsnWrite(aip, CN3D_COLOR_blue,  &av);
   av.intvalue = ptr -> alpha;
   retval = AsnWrite(aip, CN3D_COLOR_alpha,  &av);
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
*    Cn3dBackboneStyleNew()
*
**************************************************/
NLM_EXTERN 
Cn3dBackboneStylePtr LIBCALL
Cn3dBackboneStyleNew(void)
{
   Cn3dBackboneStylePtr ptr = MemNew((size_t) sizeof(Cn3dBackboneStyle));

   return ptr;

}


/**************************************************
*
*    Cn3dBackboneStyleFree()
*
**************************************************/
NLM_EXTERN 
Cn3dBackboneStylePtr LIBCALL
Cn3dBackboneStyleFree(Cn3dBackboneStylePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   Cn3dColorFree(ptr -> user_color);
   return MemFree(ptr);
}


/**************************************************
*
*    Cn3dBackboneStyleAsnRead()
*
**************************************************/
NLM_EXTERN 
Cn3dBackboneStylePtr LIBCALL
Cn3dBackboneStyleAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Cn3dBackboneStylePtr ptr;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cn3dBackboneStyle ::= (self contained) */
      atp = AsnReadId(aip, amp, CN3D_BACKBONE_STYLE);
   } else {
      atp = AsnLinkType(orig, CN3D_BACKBONE_STYLE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Cn3dBackboneStyleNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CN3D_BACKBONE_STYLE_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_BACKBONE_STYLE_style) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> style = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_BACKBONE_STYLE_color_scheme) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> color_scheme = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_BACKBONE_STYLE_user_color) {
      ptr -> user_color = Cn3dColorAsnRead(aip, atp);
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
   ptr = Cn3dBackboneStyleFree(ptr);
   goto ret;
}



/**************************************************
*
*    Cn3dBackboneStyleAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Cn3dBackboneStyleAsnWrite(Cn3dBackboneStylePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CN3D_BACKBONE_STYLE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> type;
   retval = AsnWrite(aip, CN3D_BACKBONE_STYLE_type,  &av);
   av.intvalue = ptr -> style;
   retval = AsnWrite(aip, CN3D_BACKBONE_STYLE_style,  &av);
   av.intvalue = ptr -> color_scheme;
   retval = AsnWrite(aip, CN3D_BACKBONE_STYLE_color_scheme,  &av);
   if (ptr -> user_color != NULL) {
      if ( ! Cn3dColorAsnWrite(ptr -> user_color, aip, CN3D_BACKBONE_STYLE_user_color)) {
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
*    Cn3dGeneralStyleNew()
*
**************************************************/
NLM_EXTERN 
Cn3dGeneralStylePtr LIBCALL
Cn3dGeneralStyleNew(void)
{
   Cn3dGeneralStylePtr ptr = MemNew((size_t) sizeof(Cn3dGeneralStyle));

   return ptr;

}


/**************************************************
*
*    Cn3dGeneralStyleFree()
*
**************************************************/
NLM_EXTERN 
Cn3dGeneralStylePtr LIBCALL
Cn3dGeneralStyleFree(Cn3dGeneralStylePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   Cn3dColorFree(ptr -> user_color);
   return MemFree(ptr);
}


/**************************************************
*
*    Cn3dGeneralStyleAsnRead()
*
**************************************************/
NLM_EXTERN 
Cn3dGeneralStylePtr LIBCALL
Cn3dGeneralStyleAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Cn3dGeneralStylePtr ptr;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cn3dGeneralStyle ::= (self contained) */
      atp = AsnReadId(aip, amp, CN3D_GENERAL_STYLE);
   } else {
      atp = AsnLinkType(orig, CN3D_GENERAL_STYLE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Cn3dGeneralStyleNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CN3D_GENERAL_STYLE_is_on) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> is_on = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_GENERAL_STYLE_style) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> style = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_GENERAL_STYLE_color_scheme) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> color_scheme = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_GENERAL_STYLE_user_color) {
      ptr -> user_color = Cn3dColorAsnRead(aip, atp);
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
   ptr = Cn3dGeneralStyleFree(ptr);
   goto ret;
}



/**************************************************
*
*    Cn3dGeneralStyleAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Cn3dGeneralStyleAsnWrite(Cn3dGeneralStylePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CN3D_GENERAL_STYLE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.boolvalue = ptr -> is_on;
   retval = AsnWrite(aip, CN3D_GENERAL_STYLE_is_on,  &av);
   av.intvalue = ptr -> style;
   retval = AsnWrite(aip, CN3D_GENERAL_STYLE_style,  &av);
   av.intvalue = ptr -> color_scheme;
   retval = AsnWrite(aip, CN3D_GENERAL_STYLE_color_scheme,  &av);
   if (ptr -> user_color != NULL) {
      if ( ! Cn3dColorAsnWrite(ptr -> user_color, aip, CN3D_GENERAL_STYLE_user_color)) {
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
*    Cn3dBackboneLabelStyleNew()
*
**************************************************/
NLM_EXTERN 
Cn3dBackboneLabelStylePtr LIBCALL
Cn3dBackboneLabelStyleNew(void)
{
   Cn3dBackboneLabelStylePtr ptr = MemNew((size_t) sizeof(Cn3dBackboneLabelStyle));

   return ptr;

}


/**************************************************
*
*    Cn3dBackboneLabelStyleFree()
*
**************************************************/
NLM_EXTERN 
Cn3dBackboneLabelStylePtr LIBCALL
Cn3dBackboneLabelStyleFree(Cn3dBackboneLabelStylePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    Cn3dBackboneLabelStyleAsnRead()
*
**************************************************/
NLM_EXTERN 
Cn3dBackboneLabelStylePtr LIBCALL
Cn3dBackboneLabelStyleAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Cn3dBackboneLabelStylePtr ptr;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cn3dBackboneLabelStyle ::= (self contained) */
      atp = AsnReadId(aip, amp, CN3D_BACKBONE_LABEL_STYLE);
   } else {
      atp = AsnLinkType(orig, CN3D_BACKBONE_LABEL_STYLE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Cn3dBackboneLabelStyleNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CN3D_BACKBONE_LABEL_STYLE_spacing) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> spacing = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_BACKBONE_LABEL_STYLE_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_BACKBONE_LABEL_STYLE_number) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> number = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_BACKBONE_LABEL_STYLE_termini) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> termini = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_BACKBONE_LABEL_STYLE_white) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> white = av.boolvalue;
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
   ptr = Cn3dBackboneLabelStyleFree(ptr);
   goto ret;
}



/**************************************************
*
*    Cn3dBackboneLabelStyleAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Cn3dBackboneLabelStyleAsnWrite(Cn3dBackboneLabelStylePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CN3D_BACKBONE_LABEL_STYLE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> spacing;
   retval = AsnWrite(aip, CN3D_BACKBONE_LABEL_STYLE_spacing,  &av);
   av.intvalue = ptr -> type;
   retval = AsnWrite(aip, CN3D_BACKBONE_LABEL_STYLE_type,  &av);
   av.intvalue = ptr -> number;
   retval = AsnWrite(aip, CN3D_BACKBONE_LABEL_STYLE_number,  &av);
   av.boolvalue = ptr -> termini;
   retval = AsnWrite(aip, CN3D_BACKBONE_LABEL_STYLE_termini,  &av);
   av.boolvalue = ptr -> white;
   retval = AsnWrite(aip, CN3D_BACKBONE_LABEL_STYLE_white,  &av);
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
*    Cn3dStyleSettingsNew()
*
**************************************************/
NLM_EXTERN 
Cn3dStyleSettingsPtr LIBCALL
Cn3dStyleSettingsNew(void)
{
   Cn3dStyleSettingsPtr ptr = MemNew((size_t) sizeof(Cn3dStyleSettings));

   return ptr;

}


/**************************************************
*
*    Cn3dStyleSettingsFree()
*
**************************************************/
NLM_EXTERN 
Cn3dStyleSettingsPtr LIBCALL
Cn3dStyleSettingsFree(Cn3dStyleSettingsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   Cn3dBackboneStyleFree(ptr -> protein_backbone);
   Cn3dBackboneStyleFree(ptr -> nucleotide_backbone);
   Cn3dGeneralStyleFree(ptr -> protein_sidechains);
   Cn3dGeneralStyleFree(ptr -> nucleotide_sidechains);
   Cn3dGeneralStyleFree(ptr -> heterogens);
   Cn3dGeneralStyleFree(ptr -> solvents);
   Cn3dGeneralStyleFree(ptr -> connections);
   Cn3dGeneralStyleFree(ptr -> helix_objects);
   Cn3dGeneralStyleFree(ptr -> strand_objects);
   Cn3dColorFree(ptr -> virtual_disulfide_color);
   Cn3dColorFree(ptr -> background_color);
   Cn3dBackboneLabelStyleFree(ptr -> protein_labels);
   Cn3dBackboneLabelStyleFree(ptr -> nucleotide_labels);
   return MemFree(ptr);
}


/**************************************************
*
*    Cn3dStyleSettingsAsnRead()
*
**************************************************/
NLM_EXTERN 
Cn3dStyleSettingsPtr LIBCALL
Cn3dStyleSettingsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Cn3dStyleSettingsPtr ptr;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cn3dStyleSettings ::= (self contained) */
      atp = AsnReadId(aip, amp, CN3D_STYLE_SETTINGS);
   } else {
      atp = AsnLinkType(orig, CN3D_STYLE_SETTINGS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Cn3dStyleSettingsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CN3D_STYLE_SETTINGS_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_protein_backbone) {
      ptr -> protein_backbone = Cn3dBackboneStyleAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_nucleotide_backbone) {
      ptr -> nucleotide_backbone = Cn3dBackboneStyleAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_protein_sidechains) {
      ptr -> protein_sidechains = Cn3dGeneralStyleAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_nucleotide_sidechains) {
      ptr -> nucleotide_sidechains = Cn3dGeneralStyleAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_heterogens) {
      ptr -> heterogens = Cn3dGeneralStyleAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_solvents) {
      ptr -> solvents = Cn3dGeneralStyleAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_connections) {
      ptr -> connections = Cn3dGeneralStyleAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_helix_objects) {
      ptr -> helix_objects = Cn3dGeneralStyleAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_strand_objects) {
      ptr -> strand_objects = Cn3dGeneralStyleAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_virtual_disulfides_on) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> virtual_disulfides_on = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_virtual_disulfide_color) {
      ptr -> virtual_disulfide_color = Cn3dColorAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_hydrogens_on) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> hydrogens_on = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_background_color) {
      ptr -> background_color = Cn3dColorAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_scale_factor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> scale_factor = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_space_fill_proportion) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> space_fill_proportion = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_ball_radius) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> ball_radius = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_stick_radius) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> stick_radius = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_tube_radius) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> tube_radius = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_tube_worm_radius) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> tube_worm_radius = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_helix_radius) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> helix_radius = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_strand_width) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> strand_width = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_strand_thickness) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> strand_thickness = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_protein_labels) {
      ptr -> protein_labels = Cn3dBackboneLabelStyleAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_nucleotide_labels) {
      ptr -> nucleotide_labels = Cn3dBackboneLabelStyleAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_SETTINGS_ion_labels) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> ion_labels = av.boolvalue;
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
   ptr = Cn3dStyleSettingsFree(ptr);
   goto ret;
}



/**************************************************
*
*    Cn3dStyleSettingsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Cn3dStyleSettingsAsnWrite(Cn3dStyleSettingsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CN3D_STYLE_SETTINGS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, CN3D_STYLE_SETTINGS_name,  &av);
   }
   if (ptr -> protein_backbone != NULL) {
      if ( ! Cn3dBackboneStyleAsnWrite(ptr -> protein_backbone, aip, CN3D_STYLE_SETTINGS_protein_backbone)) {
         goto erret;
      }
   }
   if (ptr -> nucleotide_backbone != NULL) {
      if ( ! Cn3dBackboneStyleAsnWrite(ptr -> nucleotide_backbone, aip, CN3D_STYLE_SETTINGS_nucleotide_backbone)) {
         goto erret;
      }
   }
   if (ptr -> protein_sidechains != NULL) {
      if ( ! Cn3dGeneralStyleAsnWrite(ptr -> protein_sidechains, aip, CN3D_STYLE_SETTINGS_protein_sidechains)) {
         goto erret;
      }
   }
   if (ptr -> nucleotide_sidechains != NULL) {
      if ( ! Cn3dGeneralStyleAsnWrite(ptr -> nucleotide_sidechains, aip, CN3D_STYLE_SETTINGS_nucleotide_sidechains)) {
         goto erret;
      }
   }
   if (ptr -> heterogens != NULL) {
      if ( ! Cn3dGeneralStyleAsnWrite(ptr -> heterogens, aip, CN3D_STYLE_SETTINGS_heterogens)) {
         goto erret;
      }
   }
   if (ptr -> solvents != NULL) {
      if ( ! Cn3dGeneralStyleAsnWrite(ptr -> solvents, aip, CN3D_STYLE_SETTINGS_solvents)) {
         goto erret;
      }
   }
   if (ptr -> connections != NULL) {
      if ( ! Cn3dGeneralStyleAsnWrite(ptr -> connections, aip, CN3D_STYLE_SETTINGS_connections)) {
         goto erret;
      }
   }
   if (ptr -> helix_objects != NULL) {
      if ( ! Cn3dGeneralStyleAsnWrite(ptr -> helix_objects, aip, CN3D_STYLE_SETTINGS_helix_objects)) {
         goto erret;
      }
   }
   if (ptr -> strand_objects != NULL) {
      if ( ! Cn3dGeneralStyleAsnWrite(ptr -> strand_objects, aip, CN3D_STYLE_SETTINGS_strand_objects)) {
         goto erret;
      }
   }
   av.boolvalue = ptr -> virtual_disulfides_on;
   retval = AsnWrite(aip, CN3D_STYLE_SETTINGS_virtual_disulfides_on,  &av);
   if (ptr -> virtual_disulfide_color != NULL) {
      if ( ! Cn3dColorAsnWrite(ptr -> virtual_disulfide_color, aip, CN3D_STYLE_SETTINGS_virtual_disulfide_color)) {
         goto erret;
      }
   }
   av.boolvalue = ptr -> hydrogens_on;
   retval = AsnWrite(aip, CN3D_STYLE_SETTINGS_hydrogens_on,  &av);
   if (ptr -> background_color != NULL) {
      if ( ! Cn3dColorAsnWrite(ptr -> background_color, aip, CN3D_STYLE_SETTINGS_background_color)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> scale_factor;
   retval = AsnWrite(aip, CN3D_STYLE_SETTINGS_scale_factor,  &av);
   av.intvalue = ptr -> space_fill_proportion;
   retval = AsnWrite(aip, CN3D_STYLE_SETTINGS_space_fill_proportion,  &av);
   av.intvalue = ptr -> ball_radius;
   retval = AsnWrite(aip, CN3D_STYLE_SETTINGS_ball_radius,  &av);
   av.intvalue = ptr -> stick_radius;
   retval = AsnWrite(aip, CN3D_STYLE_SETTINGS_stick_radius,  &av);
   av.intvalue = ptr -> tube_radius;
   retval = AsnWrite(aip, CN3D_STYLE_SETTINGS_tube_radius,  &av);
   av.intvalue = ptr -> tube_worm_radius;
   retval = AsnWrite(aip, CN3D_STYLE_SETTINGS_tube_worm_radius,  &av);
   av.intvalue = ptr -> helix_radius;
   retval = AsnWrite(aip, CN3D_STYLE_SETTINGS_helix_radius,  &av);
   av.intvalue = ptr -> strand_width;
   retval = AsnWrite(aip, CN3D_STYLE_SETTINGS_strand_width,  &av);
   av.intvalue = ptr -> strand_thickness;
   retval = AsnWrite(aip, CN3D_STYLE_SETTINGS_strand_thickness,  &av);
   if (ptr -> protein_labels != NULL) {
      if ( ! Cn3dBackboneLabelStyleAsnWrite(ptr -> protein_labels, aip, CN3D_STYLE_SETTINGS_protein_labels)) {
         goto erret;
      }
   }
   if (ptr -> nucleotide_labels != NULL) {
      if ( ! Cn3dBackboneLabelStyleAsnWrite(ptr -> nucleotide_labels, aip, CN3D_STYLE_SETTINGS_nucleotide_labels)) {
         goto erret;
      }
   }
   av.boolvalue = ptr -> ion_labels;
   retval = AsnWrite(aip, CN3D_STYLE_SETTINGS_ion_labels,  &av);
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
*    Cn3dStyleSettingsSetFree()
*
**************************************************/
NLM_EXTERN 
Cn3dStyleSettingsSetPtr LIBCALL
Cn3dStyleSettingsSetFree(Cn3dStyleSettingsSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) Cn3dStyleSettingsFree);
   return NULL;
}


/**************************************************
*
*    Cn3dStyleSettingsSetAsnRead()
*
**************************************************/
NLM_EXTERN 
Cn3dStyleSettingsSetPtr LIBCALL
Cn3dStyleSettingsSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Cn3dStyleSettingsSetPtr ptr;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cn3dStyleSettingsSet ::= (self contained) */
      atp = AsnReadId(aip, amp, CN3D_STYLE_SETTINGS_SET);
   } else {
      atp = AsnLinkType(orig, CN3D_STYLE_SETTINGS_SET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Cn3dStyleSettingsAsnRead, (AsnOptFreeFunc) Cn3dStyleSettingsFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = Cn3dStyleSettingsSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    Cn3dStyleSettingsSetAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Cn3dStyleSettingsSetAsnWrite(Cn3dStyleSettingsSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CN3D_STYLE_SETTINGS_SET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) Cn3dStyleSettingsAsnWrite, aip, atp, CN3D_STYLE_SETTINGS_SET_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    Cn3dStyleTableItemNew()
*
**************************************************/
NLM_EXTERN 
Cn3dStyleTableItemPtr LIBCALL
Cn3dStyleTableItemNew(void)
{
   Cn3dStyleTableItemPtr ptr = MemNew((size_t) sizeof(Cn3dStyleTableItem));

   return ptr;

}


/**************************************************
*
*    Cn3dStyleTableItemFree()
*
**************************************************/
NLM_EXTERN 
Cn3dStyleTableItemPtr LIBCALL
Cn3dStyleTableItemFree(Cn3dStyleTableItemPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   Cn3dStyleSettingsFree(ptr -> style);
   return MemFree(ptr);
}


/**************************************************
*
*    Cn3dStyleTableItemAsnRead()
*
**************************************************/
NLM_EXTERN 
Cn3dStyleTableItemPtr LIBCALL
Cn3dStyleTableItemAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Cn3dStyleTableItemPtr ptr;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cn3dStyleTableItem ::= (self contained) */
      atp = AsnReadId(aip, amp, CN3D_STYLE_TABLE_ITEM);
   } else {
      atp = AsnLinkType(orig, CN3D_STYLE_TABLE_ITEM);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Cn3dStyleTableItemNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CN3D_STYLE_TABLE_ITEM_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> id = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_STYLE_TABLE_ITEM_style) {
      ptr -> style = Cn3dStyleSettingsAsnRead(aip, atp);
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
   ptr = Cn3dStyleTableItemFree(ptr);
   goto ret;
}



/**************************************************
*
*    Cn3dStyleTableItemAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Cn3dStyleTableItemAsnWrite(Cn3dStyleTableItemPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CN3D_STYLE_TABLE_ITEM);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> id;
   retval = AsnWrite(aip, CN3D_STYLE_TABLE_ITEM_id,  &av);
   if (ptr -> style != NULL) {
      if ( ! Cn3dStyleSettingsAsnWrite(ptr -> style, aip, CN3D_STYLE_TABLE_ITEM_style)) {
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
*    Cn3dResidueRangeNew()
*
**************************************************/
NLM_EXTERN 
Cn3dResidueRangePtr LIBCALL
Cn3dResidueRangeNew(void)
{
   Cn3dResidueRangePtr ptr = MemNew((size_t) sizeof(Cn3dResidueRange));

   return ptr;

}


/**************************************************
*
*    Cn3dResidueRangeFree()
*
**************************************************/
NLM_EXTERN 
Cn3dResidueRangePtr LIBCALL
Cn3dResidueRangeFree(Cn3dResidueRangePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    Cn3dResidueRangeAsnRead()
*
**************************************************/
NLM_EXTERN 
Cn3dResidueRangePtr LIBCALL
Cn3dResidueRangeAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Cn3dResidueRangePtr ptr;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cn3dResidueRange ::= (self contained) */
      atp = AsnReadId(aip, amp, CN3D_RESIDUE_RANGE);
   } else {
      atp = AsnLinkType(orig, CN3D_RESIDUE_RANGE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Cn3dResidueRangeNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CN3D_RESIDUE_RANGE_from) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> from = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_RESIDUE_RANGE_to) {
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
   ptr = Cn3dResidueRangeFree(ptr);
   goto ret;
}



/**************************************************
*
*    Cn3dResidueRangeAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Cn3dResidueRangeAsnWrite(Cn3dResidueRangePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CN3D_RESIDUE_RANGE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> from;
   retval = AsnWrite(aip, CN3D_RESIDUE_RANGE_from,  &av);
   av.intvalue = ptr -> to;
   retval = AsnWrite(aip, CN3D_RESIDUE_RANGE_to,  &av);
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
*    Cn3dMoleculeLocationNew()
*
**************************************************/
NLM_EXTERN 
Cn3dMoleculeLocationPtr LIBCALL
Cn3dMoleculeLocationNew(void)
{
   Cn3dMoleculeLocationPtr ptr = MemNew((size_t) sizeof(Cn3dMoleculeLocation));

   return ptr;

}


/**************************************************
*
*    Cn3dMoleculeLocationFree()
*
**************************************************/
NLM_EXTERN 
Cn3dMoleculeLocationPtr LIBCALL
Cn3dMoleculeLocationFree(Cn3dMoleculeLocationPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> residues, (AsnOptFreeFunc) Cn3dResidueRangeFree);
   return MemFree(ptr);
}


/**************************************************
*
*    Cn3dMoleculeLocationAsnRead()
*
**************************************************/
NLM_EXTERN 
Cn3dMoleculeLocationPtr LIBCALL
Cn3dMoleculeLocationAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Cn3dMoleculeLocationPtr ptr;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cn3dMoleculeLocation ::= (self contained) */
      atp = AsnReadId(aip, amp, CN3D_MOLECULE_LOCATION);
   } else {
      atp = AsnLinkType(orig, CN3D_MOLECULE_LOCATION);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Cn3dMoleculeLocationNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CN3D_MOLECULE_LOCATION_molecule_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> molecule_id = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_MOLECULE_LOCATION_residues) {
      ptr -> residues = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Cn3dResidueRangeAsnRead, (AsnOptFreeFunc) Cn3dResidueRangeFree);
      if (isError && ptr -> residues == NULL) {
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
   ptr = Cn3dMoleculeLocationFree(ptr);
   goto ret;
}



/**************************************************
*
*    Cn3dMoleculeLocationAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Cn3dMoleculeLocationAsnWrite(Cn3dMoleculeLocationPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CN3D_MOLECULE_LOCATION);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> molecule_id;
   retval = AsnWrite(aip, CN3D_MOLECULE_LOCATION_molecule_id,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> residues, (AsnWriteFunc) Cn3dResidueRangeAsnWrite, aip, CN3D_MOLECULE_LOCATION_residues, CN3D_MOLECULE_LOCATION_residues_E);
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
*    Cn3dObjectLocationNew()
*
**************************************************/
NLM_EXTERN 
Cn3dObjectLocationPtr LIBCALL
Cn3dObjectLocationNew(void)
{
   Cn3dObjectLocationPtr ptr = MemNew((size_t) sizeof(Cn3dObjectLocation));

   return ptr;

}


/**************************************************
*
*    Cn3dObjectLocationFree()
*
**************************************************/
NLM_EXTERN 
Cn3dObjectLocationPtr LIBCALL
Cn3dObjectLocationFree(Cn3dObjectLocationPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   BiostrucIdFree(ptr -> structure_id);
   AsnGenericUserSeqOfFree(ptr -> residues, (AsnOptFreeFunc) Cn3dMoleculeLocationFree);
   return MemFree(ptr);
}


/**************************************************
*
*    Cn3dObjectLocationAsnRead()
*
**************************************************/
NLM_EXTERN 
Cn3dObjectLocationPtr LIBCALL
Cn3dObjectLocationAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Cn3dObjectLocationPtr ptr;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cn3dObjectLocation ::= (self contained) */
      atp = AsnReadId(aip, amp, CN3D_OBJECT_LOCATION);
   } else {
      atp = AsnLinkType(orig, CN3D_OBJECT_LOCATION);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Cn3dObjectLocationNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CN3D_OBJECT_LOCATION_structure_id) {
      ptr -> structure_id = BiostrucIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_OBJECT_LOCATION_residues) {
      ptr -> residues = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Cn3dMoleculeLocationAsnRead, (AsnOptFreeFunc) Cn3dMoleculeLocationFree);
      if (isError && ptr -> residues == NULL) {
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
   ptr = Cn3dObjectLocationFree(ptr);
   goto ret;
}



/**************************************************
*
*    Cn3dObjectLocationAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Cn3dObjectLocationAsnWrite(Cn3dObjectLocationPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CN3D_OBJECT_LOCATION);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> structure_id != NULL) {
      if ( ! BiostrucIdAsnWrite(ptr -> structure_id, aip, CN3D_OBJECT_LOCATION_structure_id)) {
         goto erret;
      }
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> residues, (AsnWriteFunc) Cn3dMoleculeLocationAsnWrite, aip, CN3D_OBJECT_LOCATION_residues, CN3D_OBJECT_LOCATION_residues_E);
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
*    Cn3dUserAnnotationNew()
*
**************************************************/
NLM_EXTERN 
Cn3dUserAnnotationPtr LIBCALL
Cn3dUserAnnotationNew(void)
{
   Cn3dUserAnnotationPtr ptr = MemNew((size_t) sizeof(Cn3dUserAnnotation));

   return ptr;

}


/**************************************************
*
*    Cn3dUserAnnotationFree()
*
**************************************************/
NLM_EXTERN 
Cn3dUserAnnotationPtr LIBCALL
Cn3dUserAnnotationFree(Cn3dUserAnnotationPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   MemFree(ptr -> description);
   AsnGenericUserSeqOfFree(ptr -> residues, (AsnOptFreeFunc) Cn3dObjectLocationFree);
   return MemFree(ptr);
}


/**************************************************
*
*    Cn3dUserAnnotationAsnRead()
*
**************************************************/
NLM_EXTERN 
Cn3dUserAnnotationPtr LIBCALL
Cn3dUserAnnotationAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Cn3dUserAnnotationPtr ptr;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cn3dUserAnnotation ::= (self contained) */
      atp = AsnReadId(aip, amp, CN3D_USER_ANNOTATION);
   } else {
      atp = AsnLinkType(orig, CN3D_USER_ANNOTATION);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Cn3dUserAnnotationNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CN3D_USER_ANNOTATION_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_USER_ANNOTATION_description) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> description = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_USER_ANNOTATION_style_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> style_id = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_USER_ANNOTATION_residues) {
      ptr -> residues = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Cn3dObjectLocationAsnRead, (AsnOptFreeFunc) Cn3dObjectLocationFree);
      if (isError && ptr -> residues == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_USER_ANNOTATION_is_on) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> is_on = av.boolvalue;
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
   ptr = Cn3dUserAnnotationFree(ptr);
   goto ret;
}



/**************************************************
*
*    Cn3dUserAnnotationAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Cn3dUserAnnotationAsnWrite(Cn3dUserAnnotationPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CN3D_USER_ANNOTATION);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, CN3D_USER_ANNOTATION_name,  &av);
   }
   if (ptr -> description != NULL) {
      av.ptrvalue = ptr -> description;
      retval = AsnWrite(aip, CN3D_USER_ANNOTATION_description,  &av);
   }
   av.intvalue = ptr -> style_id;
   retval = AsnWrite(aip, CN3D_USER_ANNOTATION_style_id,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> residues, (AsnWriteFunc) Cn3dObjectLocationAsnWrite, aip, CN3D_USER_ANNOTATION_residues, CN3D_USER_ANNOTATION_residues_E);
   av.boolvalue = ptr -> is_on;
   retval = AsnWrite(aip, CN3D_USER_ANNOTATION_is_on,  &av);
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
*    Cn3dGLMatrixNew()
*
**************************************************/
NLM_EXTERN 
Cn3dGLMatrixPtr LIBCALL
Cn3dGLMatrixNew(void)
{
   Cn3dGLMatrixPtr ptr = MemNew((size_t) sizeof(Cn3dGLMatrix));

   return ptr;

}


/**************************************************
*
*    Cn3dGLMatrixFree()
*
**************************************************/
NLM_EXTERN 
Cn3dGLMatrixPtr LIBCALL
Cn3dGLMatrixFree(Cn3dGLMatrixPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    Cn3dGLMatrixAsnRead()
*
**************************************************/
NLM_EXTERN 
Cn3dGLMatrixPtr LIBCALL
Cn3dGLMatrixAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Cn3dGLMatrixPtr ptr;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cn3dGLMatrix ::= (self contained) */
      atp = AsnReadId(aip, amp, CN3D_GL_MATRIX);
   } else {
      atp = AsnLinkType(orig, CN3D_GL_MATRIX);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Cn3dGLMatrixNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CN3D_GL_MATRIX_m0) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m0 = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_GL_MATRIX_m1) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m1 = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_GL_MATRIX_m2) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m2 = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_GL_MATRIX_m3) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m3 = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_GL_MATRIX_m4) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m4 = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_GL_MATRIX_m5) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m5 = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_GL_MATRIX_m6) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m6 = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_GL_MATRIX_m7) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m7 = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_GL_MATRIX_m8) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m8 = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_GL_MATRIX_m9) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m9 = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_GL_MATRIX_m10) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m10 = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_GL_MATRIX_m11) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m11 = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_GL_MATRIX_m12) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m12 = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_GL_MATRIX_m13) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m13 = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_GL_MATRIX_m14) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m14 = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_GL_MATRIX_m15) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> m15 = av.realvalue;
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
   ptr = Cn3dGLMatrixFree(ptr);
   goto ret;
}



/**************************************************
*
*    Cn3dGLMatrixAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Cn3dGLMatrixAsnWrite(Cn3dGLMatrixPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CN3D_GL_MATRIX);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.realvalue = ptr -> m0;
   retval = AsnWrite(aip, CN3D_GL_MATRIX_m0,  &av);
   av.realvalue = ptr -> m1;
   retval = AsnWrite(aip, CN3D_GL_MATRIX_m1,  &av);
   av.realvalue = ptr -> m2;
   retval = AsnWrite(aip, CN3D_GL_MATRIX_m2,  &av);
   av.realvalue = ptr -> m3;
   retval = AsnWrite(aip, CN3D_GL_MATRIX_m3,  &av);
   av.realvalue = ptr -> m4;
   retval = AsnWrite(aip, CN3D_GL_MATRIX_m4,  &av);
   av.realvalue = ptr -> m5;
   retval = AsnWrite(aip, CN3D_GL_MATRIX_m5,  &av);
   av.realvalue = ptr -> m6;
   retval = AsnWrite(aip, CN3D_GL_MATRIX_m6,  &av);
   av.realvalue = ptr -> m7;
   retval = AsnWrite(aip, CN3D_GL_MATRIX_m7,  &av);
   av.realvalue = ptr -> m8;
   retval = AsnWrite(aip, CN3D_GL_MATRIX_m8,  &av);
   av.realvalue = ptr -> m9;
   retval = AsnWrite(aip, CN3D_GL_MATRIX_m9,  &av);
   av.realvalue = ptr -> m10;
   retval = AsnWrite(aip, CN3D_GL_MATRIX_m10,  &av);
   av.realvalue = ptr -> m11;
   retval = AsnWrite(aip, CN3D_GL_MATRIX_m11,  &av);
   av.realvalue = ptr -> m12;
   retval = AsnWrite(aip, CN3D_GL_MATRIX_m12,  &av);
   av.realvalue = ptr -> m13;
   retval = AsnWrite(aip, CN3D_GL_MATRIX_m13,  &av);
   av.realvalue = ptr -> m14;
   retval = AsnWrite(aip, CN3D_GL_MATRIX_m14,  &av);
   av.realvalue = ptr -> m15;
   retval = AsnWrite(aip, CN3D_GL_MATRIX_m15,  &av);
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
*    Cn3dVectorNew()
*
**************************************************/
NLM_EXTERN 
Cn3dVectorPtr LIBCALL
Cn3dVectorNew(void)
{
   Cn3dVectorPtr ptr = MemNew((size_t) sizeof(Cn3dVector));

   return ptr;

}


/**************************************************
*
*    Cn3dVectorFree()
*
**************************************************/
NLM_EXTERN 
Cn3dVectorPtr LIBCALL
Cn3dVectorFree(Cn3dVectorPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    Cn3dVectorAsnRead()
*
**************************************************/
NLM_EXTERN 
Cn3dVectorPtr LIBCALL
Cn3dVectorAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Cn3dVectorPtr ptr;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cn3dVector ::= (self contained) */
      atp = AsnReadId(aip, amp, CN3D_VECTOR);
   } else {
      atp = AsnLinkType(orig, CN3D_VECTOR);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Cn3dVectorNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CN3D_VECTOR_x) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> x = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_VECTOR_y) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> y = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_VECTOR_z) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> z = av.realvalue;
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
   ptr = Cn3dVectorFree(ptr);
   goto ret;
}



/**************************************************
*
*    Cn3dVectorAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Cn3dVectorAsnWrite(Cn3dVectorPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CN3D_VECTOR);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.realvalue = ptr -> x;
   retval = AsnWrite(aip, CN3D_VECTOR_x,  &av);
   av.realvalue = ptr -> y;
   retval = AsnWrite(aip, CN3D_VECTOR_y,  &av);
   av.realvalue = ptr -> z;
   retval = AsnWrite(aip, CN3D_VECTOR_z,  &av);
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
*    Cn3dViewSettingsNew()
*
**************************************************/
NLM_EXTERN 
Cn3dViewSettingsPtr LIBCALL
Cn3dViewSettingsNew(void)
{
   Cn3dViewSettingsPtr ptr = MemNew((size_t) sizeof(Cn3dViewSettings));

   return ptr;

}


/**************************************************
*
*    Cn3dViewSettingsFree()
*
**************************************************/
NLM_EXTERN 
Cn3dViewSettingsPtr LIBCALL
Cn3dViewSettingsFree(Cn3dViewSettingsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   Cn3dGLMatrixFree(ptr -> matrix);
   Cn3dVectorFree(ptr -> rotation_center);
   return MemFree(ptr);
}


/**************************************************
*
*    Cn3dViewSettingsAsnRead()
*
**************************************************/
NLM_EXTERN 
Cn3dViewSettingsPtr LIBCALL
Cn3dViewSettingsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Cn3dViewSettingsPtr ptr;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cn3dViewSettings ::= (self contained) */
      atp = AsnReadId(aip, amp, CN3D_VIEW_SETTINGS);
   } else {
      atp = AsnLinkType(orig, CN3D_VIEW_SETTINGS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Cn3dViewSettingsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CN3D_VIEW_SETTINGS_camera_distance) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> camera_distance = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_VIEW_SETTINGS_camera_angle_rad) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> camera_angle_rad = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_VIEW_SETTINGS_camera_look_at_X) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> camera_look_at_X = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_VIEW_SETTINGS_camera_look_at_Y) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> camera_look_at_Y = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_VIEW_SETTINGS_camera_clip_near) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> camera_clip_near = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_VIEW_SETTINGS_camera_clip_far) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> camera_clip_far = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_VIEW_SETTINGS_matrix) {
      ptr -> matrix = Cn3dGLMatrixAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CN3D_VIEW_SETTINGS_rotation_center) {
      ptr -> rotation_center = Cn3dVectorAsnRead(aip, atp);
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
   ptr = Cn3dViewSettingsFree(ptr);
   goto ret;
}



/**************************************************
*
*    Cn3dViewSettingsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Cn3dViewSettingsAsnWrite(Cn3dViewSettingsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcn3dAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CN3D_VIEW_SETTINGS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.realvalue = ptr -> camera_distance;
   retval = AsnWrite(aip, CN3D_VIEW_SETTINGS_camera_distance,  &av);
   av.realvalue = ptr -> camera_angle_rad;
   retval = AsnWrite(aip, CN3D_VIEW_SETTINGS_camera_angle_rad,  &av);
   av.realvalue = ptr -> camera_look_at_X;
   retval = AsnWrite(aip, CN3D_VIEW_SETTINGS_camera_look_at_X,  &av);
   av.realvalue = ptr -> camera_look_at_Y;
   retval = AsnWrite(aip, CN3D_VIEW_SETTINGS_camera_look_at_Y,  &av);
   av.realvalue = ptr -> camera_clip_near;
   retval = AsnWrite(aip, CN3D_VIEW_SETTINGS_camera_clip_near,  &av);
   av.realvalue = ptr -> camera_clip_far;
   retval = AsnWrite(aip, CN3D_VIEW_SETTINGS_camera_clip_far,  &av);
   if (ptr -> matrix != NULL) {
      if ( ! Cn3dGLMatrixAsnWrite(ptr -> matrix, aip, CN3D_VIEW_SETTINGS_matrix)) {
         goto erret;
      }
   }
   if (ptr -> rotation_center != NULL) {
      if ( ! Cn3dVectorAsnWrite(ptr -> rotation_center, aip, CN3D_VIEW_SETTINGS_rotation_center)) {
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

