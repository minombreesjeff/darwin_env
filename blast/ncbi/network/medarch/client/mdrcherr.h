/*
*
*
* RCS Modification History:
* $Log: mdrcherr.h,v $
* Revision 6.1  2003/10/01 13:07:50  bazhin
* Added a couple of new warning messages ERR_REFERENCE_NoConsortAuthors
* and ERR_REFERENCE_DiffConsortAuthors. The severity of message
* ERR_REFERENCE_MedlineMatchIgnored changed from WARNING to ERROR.
*
* Revision 6.0  1997/08/25 18:35:37  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 14:11:11  ostell
* Set to revision 5.0
*
 * Revision 4.0  1995/07/26  13:55:12  ostell
 * force revision to 4.0
 *
 * Revision 1.5  1995/05/17  17:54:01  epstein
 * add RCS log revision history
 *
*/

#ifndef __MODULE_medarch__
#define __MODULE_medarch__

#define ERR_REFERENCE  1,0
#define ERR_REFERENCE_MuidNotFound  1,1
#define ERR_REFERENCE_SuccessfulMuidLookup  1,2
#define ERR_REFERENCE_OldInPress  1,3
#define ERR_REFERENCE_No_reference  1,4
#define ERR_REFERENCE_Multiple_ref  1,5
#define ERR_REFERENCE_Multiple_muid  1,6
#define ERR_REFERENCE_MedlineMatchIgnored  1,7
#define ERR_REFERENCE_MuidMissmatch  1,8
#define ERR_REFERENCE_NoConsortAuthors 1,9
#define ERR_REFERENCE_DiffConsortAuthors 1,10
#define ERR_PRINT  2,0
#define ERR_PRINT_Failed  2,1

#endif
