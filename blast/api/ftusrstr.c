/***********************************
* ftusrstr.c
*
*  All processing of User Error String handled in this file
*
* $Log: ftusrstr.c,v $
* Revision 6.3  1998/03/19 16:46:44  bazhin
* Removed memory leak in "flat2asn_install_accession_user_string()".
*
* Revision 6.2  1998/03/09 21:39:12  tatiana
* flat2asn_install_accession_user_string() changed
*
* Revision 6.1  1998/02/19 01:54:53  ostell
* added use of Thread Local Storage to make functions thread safe
*
* Revision 6.0  1997/08/25 18:05:42  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/06/19 18:37:44  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.0  1996/05/28 13:23:23  ostell
* Set to revision 5.0
*
 * Revision 4.2  1996/05/01  22:05:46  tatiana
 * flat2asn_init_user_string() added
 *
 * Revision 1.6  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
********************************/
#include <ncbi.h>
#include "ftusrstr.h"
#include <ncbithr.h>

typedef struct ftuserstruct {
	Uint1 Last_magic_locus_cookie,
		  Last_magic_accession_cookie,
		  Last_magic_feature_cookie;
} FtuserStruct, PNTR FtuserStructPtr;

static TNlmTls ftuserstr_tls = NULL;

static void FtuserCleanup (TNlmTls tls, VoidPtr ptr)
{
	MemFree(ptr);  /* easy */
	return;
}

static FtuserStruct PNTR NEAR GetFtuserStruct ( void )
{
	FtuserStructPtr fup = NULL;

	if (NlmTlsGetValue(ftuserstr_tls, (VoidPtr *)(&fup)))
	{
		if (fup == NULL)
		{
			fup = MemNew(sizeof(FtuserStruct));
			NlmTlsSetValue(&ftuserstr_tls, fup, FtuserCleanup);
		}
	}
	return fup;
}


/*-------flat2asn_init_user_string()-------*/

NLM_EXTERN void
flat2asn_init_user_string (void)
{
	FtuserStructPtr fup;

	fup = GetFtuserStruct();

	fup->Last_magic_locus_cookie=0;
	fup->Last_magic_accession_cookie=0;
	fup->Last_magic_feature_cookie=0;
}

/*-------flat2asn_install_locus_user_string()-------*/

NLM_EXTERN void
flat2asn_install_locus_user_string (CharPtr locus)
{ 
   char buf[30];
	FtuserStructPtr fup;

	fup = GetFtuserStruct();
                
    StringNCpy(buf,locus, sizeof (buf) - 3);
    StringCat(buf,":");
    buf[29]='\0';
	fup->Last_magic_locus_cookie = 
		Nlm_ErrUserInstall ( buf, fup->Last_magic_locus_cookie);
}

/*------flat2asn_delete_locus_user_string()---*/

NLM_EXTERN void
flat2asn_delete_locus_user_string(void)
{
	FtuserStructPtr fup;

	fup = GetFtuserStruct();

	Nlm_ErrUserDelete(fup->Last_magic_locus_cookie);
	fup->Last_magic_locus_cookie = 0;
}


/*-------flat2asn_install_accession_user_string()-------*/

NLM_EXTERN void
flat2asn_install_accession_user_string (CharPtr accession)
{
	CharPtr buf;
	Int2 buflen;
	FtuserStructPtr fup;

	fup = GetFtuserStruct();
    
    buflen = StringLen(accession)+2; 
    buf = MemNew(buflen);           
    sprintf(buf,"%s:", accession);

	fup->Last_magic_accession_cookie = 
		Nlm_ErrUserInstall ( buf, fup->Last_magic_accession_cookie);
	MemFree(buf);
}

/*------flat2asn_delete_accession_user_string()---*/

NLM_EXTERN void
flat2asn_delete_accession_user_string(void)
{
	FtuserStructPtr fup;

	fup = GetFtuserStruct();

	Nlm_ErrUserDelete(fup->Last_magic_accession_cookie);
	fup->Last_magic_accession_cookie = 0;
}

/*-------flat2asn_install_feature_user_string()-------*/

NLM_EXTERN void
flat2asn_install_feature_user_string (CharPtr key, CharPtr location)
{
   char buf[130];
   char install[160];
   CharPtr temp = install;
	FtuserStructPtr fup;

	fup = GetFtuserStruct();
    
    temp = StringMove(temp,"FEAT=");            
    StringNCpy(buf,key, 20);
    buf[19]='\0';
    temp = StringMove(temp,buf);
    temp = StringMove(temp,"[");
    StringNCpy(buf,location,sizeof (buf) - 3);
    buf[127]='\0';
    temp = StringMove(temp,buf);
    temp = StringMove(temp,"]");

	fup->Last_magic_feature_cookie = 
		Nlm_ErrUserInstall ( install, fup->Last_magic_feature_cookie);
}

/*------flat2asn_delete_feature_user_string()---*/

NLM_EXTERN void
flat2asn_delete_feature_user_string(void)
{
	FtuserStructPtr fup;

	fup = GetFtuserStruct();

	Nlm_ErrUserDelete(fup->Last_magic_feature_cookie);
	fup->Last_magic_feature_cookie = 0;
}
