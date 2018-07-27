/***************************************************************************
*   gbfeat.c:
*   -- all routines for checking genbank feature table
*   -- all extern variables are in gbftglob.c
*                                                                  10-11-93
$Revision: 6.9 $
*
* $Log: gbfeat.c,v $
* Revision 6.9  2003/10/09 15:35:51  bazhin
* Qualifier "rpt_unit" is removed from the list of ones to be splitted
* by commas.
*
* Revision 6.8  2001/12/06 17:00:41  kans
* TextSave takes size_t, not Int2, otherwise titin protein tries to allocate negative number
*
* Revision 6.7  2001/06/08 20:09:53  bazhin
* From now on "absent" is a legal value for /cons_splice qualifier.
*
* Revision 6.6  2000/02/02 22:10:19  kans
* use TextSave instead of TextSaveEx, which is not available
*
* Revision 6.5  2000/02/02 21:03:09  tatiana
* CkNumberType() added
*
* Revision 6.4  1998/06/15 15:00:17  tatiana
* UNIX compiler warnings fixed
*
* Revision 6.3  1998/04/30 21:44:05  tatiana
* *** empty log message ***
*
* Revision 6.2  1998/02/10 17:00:19  tatiana
* GBQualValidToAdd(0 added
*
* Revision 6.1  1998/01/08 23:42:35  tatiana
* type fixed in GBQual_names_split_ignore
*
* Revision 6.0  1997/08/25 18:05:54  madden
* Revision changed to 6.0
*
* Revision 5.5  1997/06/19 18:37:55  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.4  1997/01/21 23:11:36  tatiana
* error msg changed
*
 * Revision 5.3  1996/08/28  20:43:42  tatiana
 * skip gsdb_id qualifier in GBQualSemanticValid()
 *
 * Revision 5.3  1996/08/28  20:43:42  tatiana
 * skip gsdb_id qualifier in GBQualSemanticValid()
 *
 * Revision 5.2  1996/07/30  17:28:07  kans
 * ParFlat_... arrays now external in header file
 *
 * Revision 5.1  1996/07/29  19:45:59  tatiana
 * GBQual_names changed to use a structure
 *
 * Revision 4.4  1996/02/26  00:46:18  ostell
 * removed unused local variables and integer size mismatch fusses
 *
 * Revision 4.3  1995/11/08  22:54:38  tatiana
 * case sensitive feature key check
 *
 * Revision 4.2  1995/09/20  20:37:01  tatiana
 * a bug fixed in CkQualText
 *
 * Revision 4.1  1995/08/15  22:04:04  tatiana
 * change check for mandatory /citation and add additional check for sfp-cit
 *
 * Revision 1.23  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
****************************************************************************/

#include <utilpars.h>
#include "gbftdef.h"
#include <gbfeat.h>
#include <errdefn.h>

#define ParFlat_SPLIT_IGNORE 4
CharPtr GBQual_names_split_ignore[ParFlat_SPLIT_IGNORE] = {
"citation", "EC_number", "rpt_type", "usedin"};

/*------------------------- GBQualNameValid() ------------------------*/
/****************************************************************************
*  GBQualNameValid:
*  -- return index of the ParFlat_GBQual_names array if it is a valid
*     qualifier (ignore case), qual; otherwise return (-1) 
*                                                                   10-12-93
*	-- ignore case changed to case sensitive      10-08-95
*****************************************************************************/
NLM_EXTERN Int2 GBQualNameValid(CharPtr qual)
{
   Int2  i;

   for (i = 0; i < ParFlat_TOTAL_GBQUAL && qual != NULL; i++) {
       if (StringCmp(qual, ParFlat_GBQual_names[i].name) == 0)
          return (i);
   }

   return (-1);

} /* GBQualNameValid */

/*------------------------- GBQualSplit() ------------------------*/
/****************************************************************************
*  GBQualSplit:
*  -- return index of the GBQual_names_split_ignore array if it is a valid
*     qualifier (ignore case), qual; otherwise return (-1) 
*                                                                   10-12-93
*****************************************************************************/
NLM_EXTERN Int2 GBQualSplit(CharPtr qual)
{
   Int2  i;

   for (i = 0; i < ParFlat_SPLIT_IGNORE && qual != NULL; i++) {
       if (StringICmp(qual, GBQual_names_split_ignore[i]) == 0)
          return (i);
   }

   return (-1);

} /* GBQualSplit */

/*-------------------------- GBFeatKeyNameValid() -------------------------*/
/****************************************************************************
*  GBFeatKeyNameValid:
*  -- return "index" of the ParFlat_GBFeat array if it is a valid feature key (
*     ignore case), keystr; otherwise, return (-1), UnknownFeatKey
*                                                                  10-11-93
*	-- ignore case changed to case sensitive      10-08-95
****************************************************************************/
NLM_EXTERN Int2 GBFeatKeyNameValid(CharPtr PNTR keystr, Boolean error_msgs)
{
   Int2    j;

   if (StringCmp(*keystr, "-") == 0) 
   {
	if (error_msgs)
	{
            ErrPostStr(SEV_WARNING, ERR_FEATURE_FeatureKeyReplaced, 
           	"Featkey '-' is replaced by 'misc_feature'");
	}
   	*keystr = StringSave("misc_feature");
   }

   for (j = 0; j < ParFlat_TOTAL_GBFEAT; j++) 
   {
       if (StringCmp(ParFlat_GBFeat[j].key, *keystr) == 0)
          return (j);
   }

   return (-1);

} /* GBFeatKeyNameValid */

NLM_EXTERN Boolean GBQualValidToAdd(Int2 keyindx, CharPtr curr)
{
   Int2            i, qual, val;

	if (keyindx == -1)
		return FALSE;
	val = GBQualNameValid(curr);
	for (i = 0; i < ParFlat_GBFeat[keyindx].opt_num; i++) {
		qual = ParFlat_GBFeat[keyindx].opt_qual[i];
		if (qual == val) {
			return TRUE;
		}
	}
	for (i = 0; i < ParFlat_GBFeat[keyindx].mand_num; i++) {
		qual = ParFlat_GBFeat[keyindx].mand_qual[i];
		if (qual == val) {
			return TRUE;
		}
	}
	return FALSE;
}

/*--------------------------- GBFeatKeyQualValid() -----------------------*/
/***************************************************************************
*  GBFeatKeyQualValid:
*  -- returns error severity level.
*    error dealt with here.  Messages output if parameter 'error_msgs' set,
*    repair done if 'perform_corrections' set 
*                                                                   10-11-93
*****************************************************************************/
NLM_EXTERN int GBFeatKeyQualValid(ValNodePtr cit, Int2 keyindx, GBQualPtr PNTR gbqp, 
   Boolean error_msgs, Boolean perform_corrections)
{
   Int2            i, qual, val;
   Boolean         fqual=FALSE;
   GBQualPtr       curq, preq = NULL, next_q, first_q;
   int retval = GB_FEAT_ERR_NONE;

                              /* unknown qual will be drop after the routine */
   retval = SplitMultiValQual(gbqp, error_msgs, perform_corrections);
   retval = GBQualSemanticValid(gbqp, error_msgs, perform_corrections);
/*----------------------------------------
     if the Semnatic QUALIFIER validator says drop, then
     at the feature level, it is repairable by dropping the
     qualifier.  The only DROP for a feature is lack of
     a manditory qualifier which is handled later in this function.
             -Karl 2/7/94
-----------------------------*/
   if (retval == GB_FEAT_ERR_DROP){
      retval = GB_FEAT_ERR_REPAIRABLE;
   }

   for (first_q = curq = *gbqp; curq != NULL; curq=next_q) {
   		if (StringCmp(curq->qual, "gsdb_id") == 0) {
       		next_q = curq -> next;
   			continue;
   		}
       next_q = curq -> next;

       fqual = FALSE;
       val = GBQualNameValid(curq->qual);

       for (i = 0; i < ParFlat_GBFeat[keyindx].opt_num; i++) {
           qual = ParFlat_GBFeat[keyindx].opt_qual[i];

           if (qual == val) {
              fqual = TRUE;
              break;
           }
       }

       if (!fqual) {
          /* go back to check, is this a mandatory qualifier ? */

          for (i = 0; i < ParFlat_GBFeat[keyindx].mand_num; i++) {
              qual = ParFlat_GBFeat[keyindx].mand_qual[i];
              if (qual == val) {
                 fqual = TRUE;
                 break;
              }
          }

          if (!fqual) {
             if (retval < GB_FEAT_ERR_REPAIRABLE){
                retval = GB_FEAT_ERR_REPAIRABLE;
             }
             if (error_msgs){ 
               ErrPostStr(SEV_ERROR, ERR_FEATURE_QualWrongThisFeat, 
                 curq -> qual?curq -> qual:"");

             }
             if (perform_corrections) {
                DeleteGBQualFromList(gbqp, curq, preq);
             }

          }
       }
       if (preq){
  /*---- we have retained a qualifier, previously ----*/     
          if (preq -> next != next_q){
   /*-- did not delete curq ----*/  
              preq = curq;     
          }
       }else {
    /* ---- no qualifier previously retained, is there a new head pointer? */
          if (first_q == *gbqp){
      /* ---- we have kept our first qualifier  */
             preq = curq;    
          }else{
      /*--- we deleted the head of the queue, record current first qualifier */
             first_q = *gbqp;     
          }   
       }
       if (*gbqp == NULL){
          break;  /* was one, is gone */
       }
   }
   if (ParFlat_GBFeat[keyindx].mand_num > 0) {
                        /* do they contain all the mandatory qualifiers? */
      for (i = 0; i < ParFlat_GBFeat[keyindx].mand_num; i++) {
          qual = ParFlat_GBFeat[keyindx].mand_qual[i];

          for (curq = *gbqp; curq != NULL; curq = curq->next) {
              fqual = FALSE;
              val = GBQualNameValid(curq->qual);

              if (qual == val) {
                 fqual = TRUE;
                 break;
              }
          }

          if (!fqual) {
            if (error_msgs){
            	if (qual == 7 && cit != NULL) {
            	/* don't do anything */
            	} else {
               		ErrPostEx(SEV_ERROR, ERR_FEATURE_MissManQual, 
                 	ParFlat_GBQual_names[qual].name );                 
             	}
             }
             if (perform_corrections) {
            		if (qual == 7 && cit != NULL) {
            	/* don't do anything */
            		} else {
             			retval = GB_FEAT_ERR_DROP;
             		}
             }
         }
      }
   }
                                           /* check optional qualifiers */

   return retval;

} /* GBFeatKeyQualValid */

/*-------------------------- SplitMultiValQual() ------------------------*/
/***************************************************************************
*  SplitMultiValQual:
*
*     
****************************************************************************/
NLM_EXTERN int SplitMultiValQual(GBQualPtr PNTR gbqp, 
     Boolean error_msgs, Boolean perform_corrections)
{
   Int2        val/*, len -- UNUSED */;
   GBQualPtr   next_q, curq, preq = NULL, first_q, tmp;
   int retval = GB_FEAT_ERR_NONE;
   CharPtr	bptr, ptr, buf;
   
   for (first_q = curq = *gbqp; curq != NULL; curq = next_q) {
        next_q = curq -> next;  /* in case deleted */

       val = GBQualSplit(curq->qual);
/*       len = StringLen(curq->qual); -- NO EFFECT */

       if (val == -1) {
       		preq = curq;
       		continue;
       }
       bptr = curq->val;
       if (bptr == NULL) {
       		preq = curq;
       		continue;
       }
       if (*bptr != '(') {
       		preq = curq;
       		continue;
       }
       if (*(bptr+StringLen(bptr)-1) != ')') {
       		preq = curq;
       		continue;
       }
 	   *(bptr+StringLen(bptr)-1) = '\0';
      if ((ptr = StringChr(bptr, ',')) == NULL) {
      		StringCpy(bptr, bptr+1);
       		preq = curq;
       		continue;
        }
      			
		ErrPostEx(SEV_WARNING, ERR_QUALIFIER_MultiValue,
		   "Splited qualifier %s", curq->qual); 
		buf = bptr;
		bptr++;
		curq->val = TextSave(bptr, ptr-bptr);
		bptr = ptr + 1;
		curq->next = NULL;
		while ((ptr = StringChr(bptr, ',')) != NULL) {
			tmp = GBQualNew();
			tmp->qual = StringSave(curq->qual);
			tmp->val = TextSave(bptr, ptr-bptr);
			curq = tie_qual(curq, tmp);
			bptr = ptr + 1;
		} 
		tmp = GBQualNew();
		tmp->qual = StringSave(curq->qual);
		tmp->val = StringSave(bptr);
		curq = tie_qual(curq, tmp);
		tmp->next = next_q;
		curq = tmp;
		MemFree(buf);
      
       if (preq){
  /*---- we have retained a qualifier, previously ----*/     
          if (preq -> next != next_q){
   /*-- did not delete curq ----*/  
              preq = curq;     
          }
       }else {
    /* ---- no qualifier previously retained, is there a new head pointer? */
          if (first_q == *gbqp){
      /* ---- we have kept our first qualifier  */
             preq = curq;    
          }else{
      /*--- we deleted the head of the queue, record current first qualifier */
             first_q = *gbqp;     
          }   
       }
      if (*gbqp == NULL){
          break;  /* was one, is gone */
       }
   }

   return retval;

} /* SplitMultiValQual */  


/*-------------------------- GBQualSemanticValid() ------------------------*/
/***************************************************************************
*  GBQualSemanticValid:
*  -- returns GB_ERR level, outputs error messages if
*      'error_msgs', set
*
*  -- routine also drop out any unknown qualifier, if
*      'perform_corrections' is set  10-11-93
*     
****************************************************************************/
NLM_EXTERN int GBQualSemanticValid(GBQualPtr PNTR gbqp, 
     Boolean error_msgs, Boolean perform_corrections)
{
   Int2        val;
   GBQualPtr   next_q, curq, preq = NULL, first_q;
   int retval = GB_FEAT_ERR_NONE, ret;
   
   for (first_q = curq = *gbqp; curq != NULL; curq = next_q) {
   		if (StringCmp(curq->qual, "gsdb_id") == 0) {
        	next_q = curq -> next;
   			continue;
   		}
        next_q = curq -> next;  /* in case deleted */

       val = GBQualNameValid(curq->qual);

       if (val == -1) {
          if (retval < GB_FEAT_ERR_REPAIRABLE){
             retval = GB_FEAT_ERR_REPAIRABLE;
          }
             if (error_msgs){ 
               ErrPostEx(SEV_ERROR, ERR_QUALIFIER_UnknownSpelling,
                  curq -> qual); 
             }
          if (perform_corrections){
            DeleteGBQualFromList(gbqp, curq, preq);
          }
       } else {
          switch (ParFlat_GBQual_names[val].gbclass) {
                case Class_pos_aa:
                     ret = CkQualPosaa(gbqp, curq, preq, 
                           error_msgs, perform_corrections);
                     if (ret > retval){
                        retval = ret;
                     }
                     break;
                case Class_note:
                     ret = CkQualNote(gbqp, curq, preq, 
                           error_msgs, perform_corrections);
                     if (ret > retval){
                        retval = ret;
                     }
                     break;
                case Class_text:
                     ret = CkQualText( gbqp, curq, preq, NULL, 
                         FALSE, error_msgs, perform_corrections);
                     if (ret > retval){
                        retval = ret;
                     }
                     break;
                case Class_bracket_int:
                     ret = CkQualTokenType(gbqp, curq, preq, error_msgs, 
                         perform_corrections, ParFlat_BracketInt_type);
                     if (ret > retval){
                        retval = ret;
                     }
                     break;
                case Class_seq_aa:
                     ret = CkQualSeqaa(gbqp, curq, preq, 
                             error_msgs, perform_corrections);
                     if (ret > retval){
                        retval = ret;
                     }
                     break;
                case Class_int_or:
                     ret = CkQualMatchToken(gbqp, curq, preq, error_msgs,
                           perform_corrections,
                           ParFlat_IntOrString, ParFlat_TOTAL_IntOr);
                     if (ret > retval){
                        retval = ret;
                     }
                     break;
                case Class_site:
                     ret = CkQualSite(gbqp, curq, preq, error_msgs, 
                        perform_corrections);
                     if (ret > retval){
                        retval = ret;
                     }
                     break;
                case Class_L_R_B:
                     ret =  CkQualMatchToken(gbqp, curq, preq, error_msgs, 
                                perform_corrections,
                                ParFlat_LRBString, ParFlat_TOTAL_LRB);
                     if (ret > retval){
                        retval = ret;
                     }
                     break;
                case Class_ecnum:
                     ret =  CkQualEcnum(gbqp, curq, preq, error_msgs, 
                         perform_corrections);
                     if (ret > retval){
                        retval = ret;
                     }
                     break;
                case Class_exper:
                     ret =  CkQualMatchToken(gbqp, curq, preq, error_msgs, 
                            perform_corrections,
                            ParFlat_ExpString, ParFlat_TOTAL_Exp);
                     if (ret > retval){
                        retval = ret;
                     }
                     break;
                case Class_token:
                     ret = CkQualTokenType(gbqp, curq, preq, error_msgs, 
                               perform_corrections, ParFlat_Stoken_type);
                     if (ret > retval){
                        retval = ret;
                     }
                     break;
                case Class_int:
                     ret = CkQualTokenType(gbqp, curq, preq, error_msgs, 
                           perform_corrections, ParFlat_Integer_type);
                     if (ret > retval){
                        retval = ret;
                     }
                     break;
                case Class_number:
                     ret = CkQualTokenType(gbqp, curq, preq, error_msgs, 
                           perform_corrections, ParFlat_Number_type);
                     if (ret > retval){
                        retval = ret;
                     }
                     break;
                case Class_rpt:
                     ret = CkQualMatchToken(gbqp, curq, preq, error_msgs, 
                               perform_corrections,
                                ParFlat_RptString, ParFlat_TOTAL_Rpt);
                     if (ret > retval){
                        retval = ret;
                     }
                     break;
                case Class_flabel_base:
                     ret = CkQualTokenType(gbqp, curq, preq, error_msgs, 
                        perform_corrections, ParFlat_Stoken_type);
                     if (ret > retval){
                        retval = ret;
                     }
                     break;
                case Class_flabel_dbname:
                     ret = CkQualTokenType(gbqp, curq, preq, error_msgs, 
                         perform_corrections, ParFlat_Stoken_type);
                     if (ret > retval){
                        retval = ret;
                     }
                     break;
                case Class_none:
                     if (curq->val != NULL && *(curq->val)) {
                        if (error_msgs){ 
                           ErrPostEx(SEV_ERROR, ERR_QUALIFIER_Xtratext,
                             "/%s=%s",curq->qual,curq->val); 
                         }
                         retval = GB_FEAT_ERR_REPAIRABLE;
                         if (perform_corrections){
                           MemFree(curq->val);
                           curq->val = NULL;
                         }
                     }
                default:
                     break;
          } /* switch */
 
       
       } /* check qual's value */
       if (preq){
  /*---- we have retained a qualifier, previously ----*/     
          if (preq -> next != next_q){
   /*-- did not delete curq ----*/  
              preq = curq;     
          }
       }else {
    /* ---- no qualifier previously retained, is there a new head pointer? */
          if (first_q == *gbqp){
      /* ---- we have kept our first qualifier  */
             preq = curq;    
          }else{
      /*--- we deleted the head of the queue, record current first qualifier */
             first_q = *gbqp;     
          }   
       }
      if (*gbqp == NULL){
          break;  /* was one, is gone */
       }
   }

   return retval;

} /* GBQualSemanticValid */  


/*------------------------------ CkQualPosSeqaa() -------------------------*/
/***************************************************************************
*  CkQualPosSeqaa:  (called by CkQaulPosaa and ChQualSeqaa)
*  
*  -- format       (...aa:amino_acid)
*  -- example     aa:Phe)
*                                          -Karl 1/28/94
****************************************************************************/

NLM_EXTERN int CkQualPosSeqaa(GBQualPtr PNTR head_gbqp, GBQualPtr gbqp, 
   GBQualPtr preq,
   Boolean error_msgs, Boolean perform_corrections, CharPtr aa, CharPtr eptr)
{
   CharPtr  str;
   int retval = GB_FEAT_ERR_NONE;

      DelTailBlank(aa);

      if (ValidAminoAcid(aa) != 255) {
         str = eptr;

         while (*str != '\0' && (*str == ' ' || *str == ')'))
             str++;

         if (*str == '\0') {
            MemFree(aa);
            return retval;  /* successful, format ok return */
         }
         else {
            MemFree(aa);
            if (error_msgs){ 
              ErrPostEx(SEV_ERROR, ERR_QUALIFIER_AA,
                "Extra text after end /%s=%s",gbqp->qual,gbqp->val); 
             }
             retval = GB_FEAT_ERR_DROP;
             if (perform_corrections){
               DeleteGBQualFromList(head_gbqp, gbqp, preq);
             }
            
         }
      }
      else {
            if (error_msgs){ 
              ErrPostEx(SEV_ERROR, ERR_QUALIFIER_AA,
                "Bad aa abbreviation<%s>, /%s=%s",
                aa, gbqp->qual,gbqp->val); 
             }
             retval = GB_FEAT_ERR_DROP;
             if (perform_corrections){
               DeleteGBQualFromList(head_gbqp, gbqp, preq);
             }
      }

      return retval;
 
}




/*------------------------------ CkQualPosaa() -------------------------*/
/***************************************************************************
*  CkQualPosaa:
*  
*  -- format       (pos:base_range, aa:amino_acid)
*  -- example      /anticodon=(pos:34..36,aa:Phe)
*                  /anticodon=(pos: 34..36, aa: Phe)
*                                                                 10-12-93
****************************************************************************/
NLM_EXTERN int CkQualPosaa(GBQualPtr PNTR head_gbqp, GBQualPtr gbqp, 
   GBQualPtr preq,
   Boolean error_msgs, Boolean perform_corrections)
{
   CharPtr  eptr, str, aa = NULL;
   int retval = GB_FEAT_ERR_NONE;

   str = gbqp->val;

   if (StringNICmp(str, "(pos:", 5) == 0) {
      str += 5;

      while (*str == ' ')
          ++str;

/*---I expect that we maight need to allow blanks here, 
            but not now... -Karl 1/28/94 */
      if ((eptr = StringChr(str, ',')) != NULL) { 
         while (str != eptr  && (IS_DIGIT(*str) || *str == '.'))
             str++;

         if (str == eptr) {
            while (*str != '\0' && (*str == ',' || *str == ' '))
                str++;

            if (StringNICmp(str, "aa:", 3) == 0) {
               str += 3;

               while (*str == ' ')
                   ++str;

               if ((eptr = StringChr(str, ')')) != NULL) {
                  aa = TextSave(str, eptr-str);
                  
                  
                 retval = CkQualPosSeqaa(head_gbqp,  gbqp, preq,
                    error_msgs, perform_corrections,  aa, eptr);
               }
            } /* if, aa: */ else{
               if (error_msgs){ 
                ErrPostEx(SEV_ERROR, ERR_QUALIFIER_AA,
                   "Missing aa: /%s=%s",gbqp->qual,gbqp->val); 
               }
               retval = GB_FEAT_ERR_DROP;
               if (perform_corrections){
                  DeleteGBQualFromList(head_gbqp, gbqp, preq);
               }
            }
         }
      }else{
         if (error_msgs){ 
            ErrPostEx(SEV_ERROR, ERR_QUALIFIER_SeqPosComma,
               "Missing \',\' /%s=%s",gbqp->qual,gbqp->val); 
   /* ) match */             
         }
         retval = GB_FEAT_ERR_DROP;
        if (perform_corrections){
             DeleteGBQualFromList(head_gbqp, gbqp, preq);
         }
      }
   } /* if, (pos: */  else{
      if (error_msgs){ 
         ErrPostEx(SEV_ERROR, ERR_QUALIFIER_Pos,
            "Missing (pos: /%s=%s",gbqp->qual,gbqp->val); 
/* ) match */             
      }
      retval = GB_FEAT_ERR_DROP;
     if (perform_corrections){
          DeleteGBQualFromList(head_gbqp, gbqp, preq);
      }
   }
                        
      return retval;

} /* CkQualPosaa */

/*------------------------------ CkQualNote() --------------------------*/
/***************************************************************************
*  CkQualNote:
*  -- example: testfile  gbp63.seq gbp88.seq, gbp76.seq
*     /bound_moiety="repressor"
*     /note="Dinucleotide repeat, polymorphic among these rat 
      strains:LOU/N>F344/N=BUF/N=MNR/N=WBB1/N=WBB2/N=MR/N=LER/N=ACI/N=SR/Jr= 
      SHR/N=WKY/N>BN/SsN=LEW/N (the size of the allelesindicated)."
*     /note="guanine nucleotide-binding protein /hgml-locus_uid='LJ0088P'"
*     /note=" /map='6p21.3' /hgml_locus_uid='LU0011B'" 
*
*  -- embedded qualifer
*     -- convert all double quotes to single qutoes 
*        (this is unnecessary for the flat2asn parser program, because
*        it only grep first close double quote when "ParseQualifiers" routine
*        build GBQualPtr link list, but it would have post out message if
*        any data was truncated) (I add the conversion because someone may
*        use the routine which parsing the string different from the way I did)
*     -- convert the '/' characters at the start of the embedded-qualifier
*        token to '_'
*                                                                 12-20-93
****************************************************************************/
NLM_EXTERN int CkQualNote(GBQualPtr PNTR head_gbqp, GBQualPtr gbqp, 
   GBQualPtr preq,
   Boolean error_msgs, Boolean perform_corrections)
{

   CharPtr  str;
   Boolean has_embedded;
   int retval;
   
   retval = CkQualText( head_gbqp, gbqp, preq,
         & has_embedded, TRUE, error_msgs, 
         perform_corrections);
      if (has_embedded) {

         str = gbqp->val;
         for (; *str != '\0'; str++)
             if (*str == '\"')
                *str = '\'';
         ConvertEmbedQual(gbqp->val);
      }

   return retval;

} /* CkQualNote */

/*----------------------- ConvertEmbedQual() ----------------------------*/
/****************************************************************************
*   ConvertEmbedQual:
*   -- convert the '/' characters at the start of the embedded-qualifier
*      token to '_'
*                                                                 12-20-93
*****************************************************************************/
NLM_EXTERN void ConvertEmbedQual(CharPtr value)
{
   CharPtr  bptr, ptr, qualname, slash;
   Int2     val;

   if (value != NULL) {

      for (bptr = value; *bptr != '\0';) {

          for (;*bptr != '/' && *bptr != '\0'; bptr++)
              continue;

          if (*bptr == '/') {          
             for (slash = bptr, ++bptr, ptr = bptr; *bptr != '=' && *bptr != ' '
                                                   && *bptr != '\0'; bptr++)
                 continue;

             qualname = TextSave(ptr, bptr-ptr);

             val = GBQualNameValid(qualname);
             if (val >= 0)
                *slash = '_';

             MemFree(qualname);
          }

      } /* for */
   }

} /* ConvertEmbedQual */

/*----------------------- ScanEmbedQual() -----------------------------*/
/****************************************************************************
*  ScanEmbedQual:
*  -- retun NULL if no embedded qualifiers found; otherwise, return the
*     embedded qualifier.
*  -- scan embedded valid qualifier
*                                                                  6-29-93
*****************************************************************************/
NLM_EXTERN CharPtr ScanEmbedQual(CharPtr value)
{
   CharPtr  bptr, ptr, qual;
   Int2     val;

   if (value != NULL) {
      for (bptr = value; *bptr != '\0';) {
          for (;*bptr != '/' && *bptr != '\0'; bptr++)
              continue;

          if (*bptr == '/') {          
             for (++bptr, ptr = bptr; *bptr != '=' && *bptr != ' '
                                                   && *bptr != '\0'; bptr++)
                 continue;

             qual = TextSave(ptr, bptr-ptr);

             val = GBQualNameValid(qual);

             if (val >= 0)
                return (qual);

             MemFree(qual);
          }
      } /* for */
   }

   return (NULL);

} /* ScanEmbedQual */

/*------------------------------ CkQualText() -------------------------*/
/***************************************************************************
*  CkQualText:
*  -- return error severity
*  -- also check if embedded qualifier
*  -- format      "text"
*  if called from /note, ="" will cause qualifier to be dropped.
*  all others no error, all other, if no qualifier, will add "" value                                                        
****************************************************************************/
NLM_EXTERN int CkQualText (GBQualPtr PNTR head_gbqp,  GBQualPtr gbqp, 
   GBQualPtr preq,
   Boolean PNTR has_embedded, Boolean from_note, Boolean error_msgs, 
   Boolean perform_corrections)
{
   CharPtr  value=NULL, bptr, eptr, str;
   int retval = GB_FEAT_ERR_NONE;

   if (has_embedded != NULL){
     *has_embedded = FALSE;
   }
   if (gbqp->val == NULL) {
       if (from_note){
               if (error_msgs){ 
                ErrPostEx(SEV_ERROR, ERR_QUALIFIER_EmptyNote,
                   "/note with no text "); 
               }
               retval = GB_FEAT_ERR_DROP;
               if (perform_corrections){
                  DeleteGBQualFromList(head_gbqp, gbqp, preq);
               }
               return retval;
       } else {
           retval = GB_FEAT_ERR_SILENT;
           if (perform_corrections){
               gbqp ->val = StringSave("\"\"");  /* yup, a "" string is legal */
           } else {
           		return retval;
           }
       }                        
   }
   str = gbqp->val;
   while (*str != '\0' && (*str == ' ' || *str == '\"')){
    /* open double quote */
       str++; 
       if (*(str-1) == '\"'){
         break;  /* so does not continue through a "" string */
       }
    }
   /* find first close double quote */
   for (bptr = str; *str != '\0' && *str != '\"'; str++)
       continue;
   eptr = str;
   
   while (*str != '\0' && (*str == ' ' || *str == '\"'))
       str++;

   if (*str != '\0'){
/*   extra stuff is already rm in ParseQualifiers(). Tatiana*/
/* extra stuff, if perform corrections, remove it */
/* ERROR  here  sets retval*/
   }

      value = TextSave(bptr, eptr-bptr);
/* Some check must be done for illegal characters in gbpq->val
      for (s = value; *s != '\0'; s++) {
      	if (!IS_WHITESP(*s) && !IS_ALPHANUM(*s) && *s != '\"') {
            if (error_msgs){ 
                ErrPostEx(SEV_WARNING, ERR_QUALIFIER_IllegalCharacter,
                   "illegal char [%c] used in qualifier %s", s, gbqp ->qual);
             }      
             return (retval > GB_FEAT_ERR_REPAIRABLE) ? retval : 
            								GB_FEAT_ERR_REPAIRABLE;
      	}
      }
*/
 /* only finds first embedded qualifier */
      if (value != NULL && ((bptr = ScanEmbedQual(value)) != NULL)) {
 
         if (has_embedded != NULL) {
            *has_embedded = TRUE;
         }
         MemFree(value);
         MemFree(bptr);
         if (from_note){
            if (error_msgs){ 
                ErrPostEx(SEV_WARNING, ERR_QUALIFIER_NoteEmbeddedQual,
                   "/note with embedded qualifiers %s", gbqp ->val);
            }
            return (retval > GB_FEAT_ERR_REPAIRABLE) ? retval : 
            								GB_FEAT_ERR_REPAIRABLE;
         }else{
            if (error_msgs){ 
                ErrPostEx(SEV_WARNING, ERR_QUALIFIER_EmbeddedQual,
                   "/%s with embedded qualifiers %s", 
                   gbqp -> qual, gbqp ->val);
            }
            return retval;
         }

/*  This needs to be discussed some!, not sure -Karl 1/28/94 */
      }

      MemFree(value);

      return retval;
} /* CkQualText */

/*------------------------- CkQualSeqaa() --------------------------*/
/***************************************************************************
*  CkQualSeqaa:
*  -- format       (seq:"codon-sequence", aa:amino_acid)
*  -- example      /codon=(seq:"ttt",aa:Leu)
*                  /codon=(seq: "ttt", aa: Leu )
*                                                                  6-29-93
***************************************************************************/
NLM_EXTERN int CkQualSeqaa (GBQualPtr PNTR head_gbqp, GBQualPtr gbqp, 
   GBQualPtr preq,
   Boolean error_msgs, Boolean perform_corrections)
{
   CharPtr  eptr, str, aa;
   int retval = GB_FEAT_ERR_NONE;

   str = gbqp->val;

   if (StringNICmp(str, "(seq:", 5) == 0) {
      str += 5;

      while (*str == ' ')
          ++str;

      if ((eptr = StringChr(str, ',')) != NULL) { 
         while (str != eptr)
             str++;

         while (*str != '\0' && (*str == ',' || *str == ' '))
             str++;

         if (StringNICmp(str, "aa:", 3) == 0) {
            str += 3;

            while (*str == ' ')
                ++str;

            if ((eptr = StringChr(str, ')')) != NULL) {
               aa = TextSave(str, eptr-str);
               
                 retval = CkQualPosSeqaa(head_gbqp,  gbqp, preq,
                    error_msgs, perform_corrections,  aa, eptr);

             }
           } /* if, aa: */ else{
            if (error_msgs){ 
               ErrPostEx(SEV_ERROR, ERR_QUALIFIER_AA,
                 "Missing aa: /%s=%s",gbqp->qual,gbqp->val); 
             }
             retval = GB_FEAT_ERR_DROP;
             if (perform_corrections){
                DeleteGBQualFromList(head_gbqp, gbqp, preq);
             }
            
         }
      }else{
         if (error_msgs){ 
            ErrPostEx(SEV_ERROR, ERR_QUALIFIER_SeqPosComma,
               "Missing \',\' /%s=%s",gbqp->qual,gbqp->val); 
   /* ) match */             
         }
         retval = GB_FEAT_ERR_DROP;
        if (perform_corrections){
             DeleteGBQualFromList(head_gbqp, gbqp, preq);
         }
      }
   } /* if, (seq: */ else {


        if (error_msgs){ 
           ErrPostEx(SEV_ERROR, ERR_QUALIFIER_Seq,
              "Missing (seq: /%s=%s",gbqp->qual,gbqp->val); 
/* ) match */             
        }
        retval = GB_FEAT_ERR_DROP;
       if (perform_corrections){
            DeleteGBQualFromList(head_gbqp, gbqp, preq);
        }
      }
  
      return retval;

} /* CkQualSeqaa */

/*------------------------- () -------------------------*/
/*****************************************************************************
*  CkQualMatchToken:
*                                                                6-29-93
*****************************************************************************/
NLM_EXTERN int CkQualMatchToken
(GBQualPtr PNTR head_gbqp, GBQualPtr gbqp, 
   GBQualPtr preq,
   Boolean error_msgs, Boolean perform_corrections, CharPtr array_string[],
   Int2 totalstr)
{
   CharPtr  msg=NULL, bptr, eptr, str;
   int retval = GB_FEAT_ERR_NONE;

	if(gbqp->val == NULL) {
        if (error_msgs){ 
           ErrPostEx(SEV_ERROR, ERR_QUALIFIER_InvalidDataFormat,
             "NULL value for (%s)", gbqp->qual); 
         }
         retval = GB_FEAT_ERR_DROP;
         if (perform_corrections){
            DeleteGBQualFromList(head_gbqp, gbqp, preq);
         }
   		return retval;
	} 
   str = gbqp->val;

   for (bptr = str; *str != '\0' && *str != ' '; str++)
       continue;
   eptr = str;
   
   while (*str != '\0' && *str == ' ')
       str++;

   if (*str == '\0') {
      msg = TextSave(bptr, eptr-bptr);

      if (MatchArrayStringIcase(array_string, totalstr, msg) == -1) {
        if (error_msgs){ 
           ErrPostEx(SEV_ERROR, ERR_QUALIFIER_InvalidDataFormat,
             "Value not in list of legal values /%s=%s",
            gbqp->qual,gbqp->val); 
         }
         retval = GB_FEAT_ERR_DROP;
         if (perform_corrections){
            DeleteGBQualFromList(head_gbqp, gbqp, preq);
         }
  	  }
   } else {
         if (error_msgs){ 
           ErrPostEx(SEV_ERROR, ERR_QUALIFIER_Too_many_tokens,
             "/%s=%s", gbqp->qual,gbqp->val); 
         }
         retval = GB_FEAT_ERR_DROP;
         if (perform_corrections){
            DeleteGBQualFromList(head_gbqp, gbqp, preq);
         }
   }

   MemFree(msg);
   return retval;
 
} /* CkQualMatchToken */

/*------------------------- CkQualEcnum() ---------------------------*/
/***************************************************************************
*   CkQualEcnum:
*   -- Ec_num has text format,
*      but the text only allow digits, period, and hyphen (-)
*                                                                12-10-93
****************************************************************************/
NLM_EXTERN int CkQualEcnum( GBQualPtr PNTR head_gbqp, GBQualPtr gbqp, 
   GBQualPtr preq,
   Boolean error_msgs, Boolean perform_corrections )
{
   CharPtr  str;
   int retval = GB_FEAT_ERR_NONE;
		

   retval = CkQualText(head_gbqp, gbqp, preq, NULL, FALSE, 
        error_msgs, perform_corrections);
		if (retval == GB_FEAT_ERR_NONE){
   
      str = gbqp->val;
                                                       /* open double quote */
      while (*str != '\0' && (*str == ' ' || *str == '\"'))
          str++;
   
      for (; *str != '\0' && *str != '\"'; str++)
          if (!IS_DIGIT(*str) && *str != '.' && *str != '-') {
            if (error_msgs){ 
               ErrPostEx(SEV_ERROR, ERR_QUALIFIER_BadECnum,
                 "At <%c>(%d) /%s=%s",
                 *str, (int) *str, gbqp->qual,gbqp->val); 
             }
             retval = GB_FEAT_ERR_DROP;
             if (perform_corrections){
                DeleteGBQualFromList(head_gbqp, gbqp, preq);
             }
           break;
      }
   }

   return retval;

} /* CkQualEcnum */

/*------------------------- CkQualSite() --------------------------*/
/***************************************************************************
*  CkQualSite:
*  -- format       (5'site:boolean, 3'site:boolean)
*  -- example      /cons_splice=(5'site:YES, 3'site:NO)
*                                                                  6-29-93
***************************************************************************/
NLM_EXTERN int CkQualSite ( GBQualPtr PNTR head_gbqp, GBQualPtr gbqp, 
   GBQualPtr preq,
   Boolean error_msgs, Boolean perform_corrections )
{
   int retval = GB_FEAT_ERR_NONE;
   CharPtr  bptr, str;
   Boolean ok=FALSE;
   CharPtr yes_or_no = "not \'YES\', \'NO\' or \'ABSENT\'";

   str = gbqp->val;
   if (StringNICmp(str, "(5'site:", 8) == 0) {
      bptr = str;
      str += 8;

      if (StringNICmp(str, "YES", 3) == 0 || StringNICmp(str, "NO", 2) == 0 ||
          StringNICmp(str, "ABSENT", 6) == 0) {

         if (StringNICmp(str, "YES", 3) == 0)
            str += 3;
         else if (StringNICmp(str, "NO", 2) == 0)
            str += 2;
         else
            str += 6;

         for (; *str == ' '; str++);
         for (; *str == ','; str++);
         for (; *str == ' '; str++);
         

         if (StringNICmp(str, "3'site:", 7) == 0) {
            str += 7;

            if (StringNICmp(str, "YES", 3) == 0 ||
                StringNICmp(str, "NO", 2) == 0 ||
                StringNICmp(str, "ABSENT", 6) == 0) {
               if (StringNICmp(str, "YES", 3) == 0)
                  str += 3;
               else if (StringNICmp(str, "NO", 2) == 0)
                  str += 2;
               else
                  str += 6;

               if (*str == ')') {
   
                  while (*str != '\0' && (*str == ' ' || *str == ')'))
                      str++;

                  if (*str == '\0')
                    ok=TRUE;
                  else {
                     bptr = "extra characters";
                  }

               } /* if, ")" */ else{
               }
            } /* if, yes or no */ else{
               bptr = yes_or_no;
            }
         } /* if, 3'site */ else{
            bptr="3\' site";
         }
      } /* if, yes or no */else {
         bptr = yes_or_no;
      }
   } /* if, 5'site */else {
     bptr="5\' site";
   }

  if (! ok){
      if (error_msgs){ 
         ErrPostEx(SEV_ERROR, ERR_QUALIFIER_Cons_splice,
           "%s /%s=%s", bptr, gbqp->qual,gbqp->val); 
       }
       retval = GB_FEAT_ERR_DROP;
       if (perform_corrections){
          DeleteGBQualFromList(head_gbqp, gbqp, preq);
       }
  }
  return retval;

} /* CkQualSite */

/*------------------------- CkQualTokenType() --------------------------*/
/***************************************************************************
*  CkQualTokenType:
*  -- format   single token
*  -- example  ParFlat_Stoken_type        /label=Albl_exonl  /mod_base=m5c
*              ParFlat_BracketInt_type    /citation=[3] or /citation= ([1],[3])
*              ParFlat_Integer_type           /transl_table=4
*				ParFlat_Number_type			/number=4b
*  -- not implemented yet, treat as ParFlat_Stoken_type:
*     -- feature_label or base_range              
*                 /rpt_unit=Alu_rpt1   /rpt_unit=202..245
*     -- Accession-number:feature-name or
*                            Database_name:Acc_number:feature_label
*        /usedin=X10087:proteinx
*                                                                 10-12-93
***************************************************************************/
NLM_EXTERN int CkQualTokenType (GBQualPtr PNTR head_gbqp,  GBQualPtr gbqp, 
   GBQualPtr preq,
   Boolean error_msgs, Boolean perform_corrections, Uint1 type)
{
   CharPtr  token = NULL, bptr, eptr, str;
   Boolean token_there = FALSE;
   int retval = GB_FEAT_ERR_NONE;

   str = gbqp->val;

   if (str != NULL)
    if (*str != '\0'){
			token_there = TRUE;
    }
   if (! token_there) {
     if (error_msgs){ 
        ErrPostEx(SEV_ERROR, ERR_QUALIFIER_InvalidDataFormat,
          "Missing value /%s=...",gbqp->qual); 
      }
      retval = GB_FEAT_ERR_DROP;
      if (perform_corrections){
         DeleteGBQualFromList(head_gbqp, gbqp, preq);
      }
   }else{
/*  token there */
   for (bptr = str; *str != '\0' && *str != ' '; str++)
       continue;
   eptr = str;
   
   while (*str != '\0' && *str == ' ')
       str++;

   if (*str == '\0') {
/*------single token found ----*/
      token = TextSave(bptr, eptr-bptr);

      bptr = token;

      switch (type) {
         case ParFlat_BracketInt_type:
/*-------this can be made to be much more rigorous --Karl ---*/
			  str = CkBracketType(token);			
               break;
         case ParFlat_Integer_type:
              for (str = token; *str != '\0' && IS_DIGIT(*str); str++)
              		continue;
              if (*str == '\0') {
              	str = NULL;
              }
              break;
         case ParFlat_Stoken_type:
         		str = CkLabelType(token);
               break;
         case ParFlat_Number_type:
         		str = CkNumberType(token);
               break;
        default:
              str = NULL;
              break;
      }

      if (str != NULL) {
         switch (type) {
            case ParFlat_BracketInt_type:
                         bptr = "Invalid [integer] format"; 
                         break;
            case ParFlat_Integer_type:
                         bptr = "Not an integer number";
                         break;
            case ParFlat_Stoken_type:
                         bptr = "Invalid format";
                         break;
/*-- logically can not happen, as coded now -Karl  1/31/94 --*/
            default:     bptr = "Bad qualifier value";
                         break;
         }
        if (error_msgs){ 
           ErrPostEx(SEV_ERROR, ERR_QUALIFIER_InvalidDataFormat,
             "%s=%s, at %s", gbqp->qual,gbqp->val, str); 
         }
         retval = GB_FEAT_ERR_DROP;
         if (perform_corrections){
            DeleteGBQualFromList(head_gbqp, gbqp, preq);
         }
      }
   } else{
/*-- more than a single token found ---*/
     if (error_msgs){ 
        ErrPostEx(SEV_ERROR, ERR_QUALIFIER_Xtratext,
          "extra text found /%s=%s, at %s",gbqp->qual,gbqp->val, str); 
      }
      retval = GB_FEAT_ERR_DROP;
      if (perform_corrections){
         DeleteGBQualFromList(head_gbqp, gbqp, preq);
      }
   }
 } /* token there */

      MemFree(token);
      return retval;

} /* CkQualTokenType */

/*--------------------------- GBFeatErrSpec() ------------------*/
/***************************************************************************
*   GBFeatErrSpec:
*   -- return a err_specific string by a given index
*   -- GenBank feature table semantic checking is return a link list of 
*      ValNodePtr, where
*      vnp->choice points to "err_specific", start from 1 
*      vnp->data.prtvalue points to "err_msg" string, a short message to
*      describe the "err_specific"
*                                                                10-14-93
****************************************************************************/
NLM_EXTERN CharPtr GBFeatErrSpec(Uint1 indx)
{
   switch (indx) {
     case  1: return("MissMandQual");
     case  2: return("QualNotFound");
     case  3: return("UnkQual");
     case  4: return("BadQualValPosaa");
     case  5: return("BadQualValText");
     case  6: return("BadQualValBracketInt");
     case  7: return("BadQualValSeqaa");
     case  8: return("BadQualValNot1or2or3");
     case  9: return("BadQualValSite");
     case 10: return("BadQualValNotLorRorB");
     case 11: return("BadQualValBadEcnum");
     case 12: return("BadQualValExporNotexp");
     case 13: return("BadQualValFeatLabel");
     case 14: return("BadQualValNotInteger");
     case 15: return("BadQualValNotInStringList");
     case 16: return("BadQualValFlabelBase");
     case 17: return("BadQualValFlabelDbname");
     case 18: return("NoValueQualHasVal");
     case 19: return("EmbeddedQual");
     case 20: return("EmbeddedQualInNote");
     default: return("AddMoreInGBFeatErrSpec");
   }

} /* GBFeatErrSpec */

/*------------------------ DeleteGBQualFromList() --------------------*/
/*****************************************************************************
*   DeleteGBQualFromList:
*     all deletes of GBQuals done here
*                                                          -Karl 1/28/94
******************************************************************************/
NLM_EXTERN void DeleteGBQualFromList (GBQualPtr PNTR gbqp, GBQualPtr curq,
     GBQualPtr preq)
{
    GBQualPtr   temp;
     
          if (preq == NULL)
             *gbqp = curq->next;  /* will change first_q in calling function */
          else
             preq->next = curq->next;  /* will cause next_q == preq -> next */

          temp = curq;
          temp->next = NULL;
          GBQualFree(temp);
}

/*------------------------ CkBracketType() --------------------*/
/*****************************************************************************
*   CkBracketType:
*	checks /citation=([1],[3])
*     May be we should check for only single value here like
*	/citation=[digit]
*                                                          -Tatiana 1/28/95
******************************************************************************/
NLM_EXTERN CharPtr CkBracketType(CharPtr str)
{	
	if (str == NULL)
		return "NULL value";
	if (*str == '[') {
		str++;
		if (!IS_DIGIT(*str)) {
			return str;
		} else {
			while (IS_DIGIT(*str)) {
				str++;
			}
			if (*str != ']') {
				return str;
			}
			str++;
			if (*str != '\0') {
				return str;
			}  
			return NULL;
		}
	} else {
		return str;
	}
}

/*------------------------ CkNumberType() --------------------*/
/*****************************************************************************
*   CkNumberType:
*	checks /number=single_token  - numbers and letters
*	/number=4 or /number=6b
*                                                          -Tatiana 2/1/00
******************************************************************************/
NLM_EXTERN CharPtr CkNumberType(CharPtr str)
{
		for (;  *str != '\0' && !IS_ALPHANUM(*str); str++)
			continue;
		if (*str != '\0') {
			return NULL;  
		}
		return str;
} 

/*------------------------ CkLabelType() --------------------*/
/*****************************************************************************
*   CkLabelType:
*	checks /label=,feature_label> or /label=<base_range>
*                                                          -Tatiana 1/28/95
******************************************************************************/
NLM_EXTERN CharPtr CkLabelType(CharPtr str)
{
	Boolean range = TRUE, label = TRUE;
	CharPtr		bptr;
	
	if (IS_DIGIT(*str)) {
		for (; IS_DIGIT(*str); str++)
			continue;
		if (*str == '.' && *(str+1) == '.') {
			str += 2;
			if (!IS_DIGIT(*str)) {
				range = FALSE;
			} else {
				while (IS_DIGIT(*str)) {
					str++;
				}
			}
			if (*str != '\0') {
				range = FALSE;
			}
		} else {
			range = FALSE;
		}
		
	} 
	if (!range) {
		bptr = str;
		for (;  *str != '\0' && !IS_ALPHA(*str); str++)
			continue;
		if (*str == '\0') {
			label = FALSE;    /* must be at least one letter */
		}
		for (str = bptr; *str != '\0' && IS_ALPHA(*str) || IS_DIGIT(*str) 
			|| *str == '-' || *str == '_' || *str == '\'' || *str == '*';
			str++)
			continue;
		if (*str != '\0') {
			label = FALSE;
		}
	}
	if (range || label) {
		return NULL;
	} else {
		return str;
	}
}

/*--------------------------- tie_qual() ---------------------------*/
/****************************************************************************
*  tie_qual:
*  -- ties next ValNode to the end of the chain
*                                                                    08-4-93
****************************************************************************/
NLM_EXTERN GBQualPtr tie_qual(GBQualPtr head, GBQualPtr next)
{
	GBQualPtr v;

	if (head == NULL) {
		return next;
	}
	for (v = head; v->next != NULL; v = v->next) {
		v = v;
	}
	v->next = next;
	return head;
}
