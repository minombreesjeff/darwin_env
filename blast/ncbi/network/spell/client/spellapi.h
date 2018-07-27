/*
*
*
* RCS Modification History:
* $Log: spellapi.h,v $
* Revision 6.0  1997/08/25 18:40:34  madden
* Revision changed to 6.0
*
* Revision 4.0  1995/07/26 13:55:42  ostell
* force revision to 4.0
*
 * Revision 1.3  1995/05/31  21:12:10  ostell
 * added SpellCheckData() to return user supplied data ptr and eliminate
 * the need for global static variables by calling functions
 *
 * Revision 1.2  1995/05/17  17:59:58  epstein
 * add RCS log revision history
 *
*/

Boolean SpellInit PROTO((void));
Boolean SpellFini PROTO((void));
int SpellCheck PROTO((CharPtr String, void (*CallBack)(CharPtr)));
int SpellCheckData PROTO((CharPtr String, void (*CallBack)(CharPtr,Pointer), Pointer userdata));
int StringInMedline PROTO((CharPtr String, void (*CallBack)(CharPtr)));

#define SPELL_SERV_RETRIES 2
