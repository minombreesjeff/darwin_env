#include <asn.h>

#include <objegkludge.h>

/* allows asncode-generated Entrezgene object loaders to use hand-coded Dbtag reader/writer */

/**************************************************
*
*    EGDbtagNew()
*
**************************************************/
NLM_EXTERN 
EGDbtagPtr LIBCALL
EGDbtagNew(void)
{
   EGDbtagPtr ptr = MemNew((size_t) sizeof(EGDbtag));

   return ptr;

}


/**************************************************
*
*    EGDbtagFree()
*
**************************************************/
NLM_EXTERN 
EGDbtagPtr LIBCALL
EGDbtagFree(EGDbtagPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> db);
   ObjectIdFree(ptr -> tag);
   return MemFree(ptr);
}


/**************************************************
*
*    EGDbtagAsnRead()
*
**************************************************/
NLM_EXTERN 
EGDbtagPtr LIBCALL
EGDbtagAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DbtagPtr dbt;
   EGDbtagPtr ptr;

   if (aip == NULL) return NULL;

   dbt = DbtagAsnRead (aip, orig);
   if (dbt == NULL) return NULL;

   ptr = EGDbtagNew();
   if (ptr == NULL) return NULL;
 
   ptr->db = dbt->db;
   ptr->tag = dbt->tag;
 
   MemFree (dbt);

   return ptr;
}



/**************************************************
*
*    EGDbtagAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
EGDbtagAsnWrite(EGDbtagPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DbtagPtr dbt;
   Boolean retval = FALSE;

   if (aip == NULL || ptr == NULL) return FALSE;

   dbt = DbtagNew ();
   if (dbt == NULL) return FALSE;

   dbt->db = ptr->db;
   dbt->tag = ptr->tag;
   retval = DbtagAsnWrite (dbt, aip, orig);

   MemFree (dbt);

   return retval;
}


