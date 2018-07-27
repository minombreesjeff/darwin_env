/*
	medline.h		Process Medline database fields

	MODULE:	medline
	FILES:	medline.c and medline.h (this one).

	The procedures in module medline support the decoding of the
	data the Medline database elements.  These are rather general
	extraction methods as opposed to more complex extractions
	which are highly field specific (such as the Comment field).

	Edit History:
	    16 August 1991 - Rand S. Huntzinger
		Added GetCharsetErrors();

	Work started: 25 July 1991 - Rand S. Huntzinger, NLM/NCBI.
*
*
* RCS Modification History:
* $Log: medfield.h,v $
* Revision 6.0  1997/08/25 18:36:31  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:55:08  epstein
* add RCS log revision history
*
*/

#ifndef	DEFS_MODULE_MEDLINE
#define	DEFS_MODULE_MEDLINE

/* Define the Medline unit record field element types */

#define MEDLINE_DA	100	/* DATE OF ENTRY                     */
#define MEDLINE_RO	101	/* RECORD ORIGINATOR                 */
#define MEDLINE_LR	102	/* LAST REVISION DATE                */
#define MEDLINE_CU	103	/* CLASSUP DATE                      */
#define MEDLINE_RN	261	/* CAS REGISTRY NUMBER               */
#define MEDLINE_IS	301	/* INTL STAND SER NO                 */
#define MEDLINE_TA	305	/* TITLE ABBREVIATION                */
#define MEDLINE_LA	306	/* LANGUAGE                          */
#define MEDLINE_ZN	308	/* MESH Z TREE NUMBER                */
#define MEDLINE_JC	320	/* JOURNAL TITLE CODE                */
#define MEDLINE_PY	323	/* INDEXING PRIORITY                 */
#define MEDLINE_SB	324	/* JOURNAL SUBSET                    */
#define MEDLINE_GS	328	/* GENE SYMBOL                       */
#define MEDLINE_LI	329	/* SPECIAL LIST IND                  */
#define MEDLINE_UI	350	/* UNIQUE IDENTIFIER                 */
#define MEDLINE_MH	351	/* MESH HEADING                      */
#define MEDLINE_TI	352	/* TITLE                             */
#define MEDLINE_PG	353	/* PAGINATION                        */
#define MEDLINE_DP	354	/* DATE OF PUBLICATION               */
#define MEDLINE_PT	360	/* PUBLICATION TYPE                  */
#define MEDLINE_MRI	368	/* MACHINE-READABLE ID               */
#define MEDLINE_NP	369	/* NOT FOR PUBLICATION               */
#define MEDLINE_AB	370	/* ABSTRACT                          */
#define MEDLINE_AA	371	/* ABSTRACT AUTHOR                   */
#define MEDLINE_AU	372	/* AUTHOR                            */
#define MEDLINE_IP	373	/* ISSUE/PART/SUPP                   */
#define MEDLINE_NI	376	/* NO-AUTHOR INDICATOR               */
#define MEDLINE_RF	377	/* NUMBER OF REFS                    */
#define MEDLINE_AD	378	/* ADDRESS                           */
#define MEDLINE_PS	379	/* PERSONAL NAME AS SUBJECT          */
#define MEDLINE_TT	380	/* TRANSLIT/VERNAC TITLE             */
#define MEDLINE_VI	381	/* VOLUME/ISSUE                      */
#define MEDLINE_EA	382	/* ENGLISH ABSTRACT INDEX            */
#define MEDLINE_EM	383	/* ENTRY MONTH                       */
#define MEDLINE_CM	440	/* COMMENTS                          */
#define MEDLINE_CA	525	/* CALL NUMBER                       */
#define MEDLINE_SI	606	/* SECONDARY SOURCE ID               */
#define MEDLINE_ID	640	/* ID NUMBER                         */

/* Define the codes specific to identifying Monographs in the JC/TA fields */

#define	MEDLINE_MONOGRAPH_JC	"IDM"
#define	MEDLINE_MONOGRAPH_TA	"(Monograph)"

#endif	/* DEFS_MODULE_MEDLINE */
