#include <stdio.h>
#include <ncbi.h>
#include <toasn3.h>

typedef struct wholefeat {
	Int2 count;
	SeqFeatPtr sfp;
} WholeFeat, PNTR WholeFeatPtr;

WholeFeatPtr WholeFeatNew PROTO((void));
void WholeFeatFree PROTO((WholeFeatPtr wfp));
Int2 StringExtCmp PROTO((CharPtr s1, CharPtr s2));

CharPtr  StripAllSpace PROTO((CharPtr str)); 
Int2 StringIgnoreSpaceCmp PROTO((CharPtr s1, CharPtr s2));
Int2 AStringIgnoreSpaceCmp PROTO((CharPtr s1, CharPtr s2));

CharPtr StrStripSpaces PROTO((CharPtr pchSrc, CharPtr pchDest));
Boolean IsStringEmpty PROTO((CharPtr str));
CharPtr ShortenString PROTO((CharPtr str, Int4 iLen));
CharPtr StringNStr PROTO((CharPtr pchSource, CharPtr pchTemplate, size_t cMax));

CharPtr FlatSafeSize PROTO((CharPtr ptr, Int4 limit));

SeqFeatPtr tie_feat PROTO((SeqFeatPtr head, SeqFeatPtr next));

MolFixPtr tie_next_mol PROTO((MolFixPtr head, MolFixPtr next));

CodeBreakPtr tie_next_cbp PROTO((CodeBreakPtr head, CodeBreakPtr next));

OrgFixPtr tie_next_biosource PROTO((OrgFixPtr head, OrgFixPtr next));

SubSourcePtr tie_next_subtype PROTO((SubSourcePtr head, SubSourcePtr next));

OrgModPtr tie_next_OrgMod PROTO((OrgModPtr head, OrgModPtr next));

SeqFeatPtr SeqFeatExtract PROTO((SeqFeatPtr PNTR headptr, Nlm_Int2 choice));

SeqFeatPtr SeqFeatExtractList PROTO((SeqFeatPtr PNTR headptr, Nlm_Int2 choice));

SeqFeatPtr SourceFeatExtract PROTO((SeqFeatPtr PNTR headptr));

SeqFeatPtr ExtractSourceFeatList PROTO((SeqFeatPtr PNTR headptr));

SeqIdPtr find_id PROTO((SeqEntryPtr sep));

OrgModPtr remove_OrgMod PROTO((OrgModPtr head, OrgModPtr x));
SubSourcePtr remove_subtype PROTO((SubSourcePtr head, SubSourcePtr x));

SeqFeatXrefPtr remove_xref PROTO((SeqFeatXrefPtr head, SeqFeatXrefPtr x));

GBQualPtr remove_qual PROTO((GBQualPtr head, GBQualPtr x));

Boolean check_whole PROTO((SeqFeatPtr f, Int4 len));

Boolean true_multiple PROTO((SeqAnnotPtr sap, Int4 len));

CharPtr copy_qvalue PROTO((GBQualPtr qlist, CharPtr qual));

CharPtr get_qvalue PROTO((GBQualPtr curq, CharPtr qual));

CharPtr laststr PROTO((CharPtr str, CharPtr word));

void DelFeatFromList PROTO((SeqFeatPtr PNTR gbqp, SeqFeatPtr curq, SeqFeatPtr preq));

void DelNodeFromList PROTO((ValNodePtr PNTR gbqp, ValNodePtr curq, ValNodePtr preq));

ValNodePtr GetOrfList PROTO((BioseqPtr bsp, Int2 limit));
ValNodePtr GetAltList PROTO((ValNodePtr list));
