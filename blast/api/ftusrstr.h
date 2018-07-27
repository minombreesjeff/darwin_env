/****************************************************************************
*   ftusrstr.h:
*
*
* $Log: ftusrstr.h,v $
* Revision 6.0  1997/08/25 18:05:43  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/06/19 18:37:45  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.0  1996/05/28 13:23:23  ostell
* Set to revision 5.0
*
 * Revision 4.2  1996/05/01  22:05:21  tatiana
 * flat2asn_init_user_string() added
 *
 * Revision 1.2  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*                                     
****************************************************************************/
#ifndef _FTUSRSTR_
#define _FTUSRSTR_

/* used to install the user error strings (locus and accession)
   so that they will appear in ErrPost or ErrPostEx error
   messages, without the need to pass the 'pp' pointer. 
     -Karl */
     
#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

NLM_EXTERN void flat2asn_init_user_string PROTO ((void));
NLM_EXTERN void flat2asn_install_locus_user_string PROTO ((CharPtr locus));
NLM_EXTERN void flat2asn_delete_locus_user_string PROTO ((void));
NLM_EXTERN void flat2asn_install_accession_user_string PROTO ((CharPtr accession));
NLM_EXTERN void flat2asn_delete_accession_user_string PROTO ((void));
NLM_EXTERN void flat2asn_install_feature_user_string PROTO ((CharPtr key, CharPtr location));
NLM_EXTERN void flat2asn_delete_feature_user_string PROTO ((void));

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
