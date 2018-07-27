/*   cn3dmsel.c
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* File Name:  cn3dmsel.c
*
* Author:  Christopher Hogue
*
* Version Creation Date:   1/31/96
*
* File Description: Reusable group for model selection in mmdbapi 
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: cn3dmsel.c,v $
* Revision 6.1  1999/10/29 14:15:29  thiessen
* ran all Cn3D source through GNU Indent to prettify
*
* Revision 6.0  1997/08/25 18:13:35  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 14:05:44  ostell
* Set to revision 5.0
*
 * Revision 1.4  1996/05/09  15:41:27  hogue
 * Fixed SGI compiler warnings.
 *
 * Revision 1.3  1996/04/26  18:42:34  vakatov
 * CN3D sources ported to MS-Windows;
 * the portability errors and warnings fixed, etc.
 *
 * Revision 1.2  1996/02/02  19:40:04  hogue
 * Initial Revision
 *
*
* ==========================================================================
*/

#include <vibrant.h>
#include <mmdbapi.h>
#include <cn3dmsel.h>
#include <cn3dmain.h>


static GrouP Cn3D_gModelSelect;
static LisT Cn3D_lModels;
static Boolean VectorKeeper = FALSE;

static void Cn3D_ModelPickProc(LisT l)
{
    Int2 iCount = 0;
    PDNMS pdnmsThis;
    PDNML pdnmlThis;
    PMSD pmsdThis;
    PMLD pmldThis;

    /* sets all models' status at once */

    pdnmsThis = GetSelectedModelstruc();
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    pdnmlThis = pmsdThis->pdnmlModels;
    while (pdnmlThis) {
        pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
        if ((pmldThis->iType == Model_type_ncbi_vector)
            && (VectorKeeper == FALSE)) {
            pmldThis->bSelected &= (Byte) 0xFE; /* don't keep vectors - ignore 'em */
        } else {
            iCount++;
            ASSERT(iCount > 0);
            if (GetItemStatus(Cn3D_lModels, iCount))
                pmldThis->bSelected |= (Byte) 0x01;
            else
                pmldThis->bSelected &= (Byte) 0xFE;
        }
        pdnmlThis = pdnmlThis->next;
    }
    return;
}



GrouP LIBCALL Cn3D_ModelSelect(GrouP g, Boolean KeepVectors)
{
    PDNMS pdnmsThis = NULL;
    PMSD pmsdThis = NULL;
    PDNML pdnmlThis = NULL;     /* list of models */
    PMLD pmldThis = NULL;
    ValNodePtr pvnModelStrings = NULL;
    ValNodePtr pvnTemp = NULL;
    Int2 iCount = 0;
    Char pcModelString[35];


    pdnmsThis = GetSelectedModelstruc();
    if (pdnmsThis)
        pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    if (pmsdThis)
        pdnmlThis = pmsdThis->pdnmlModels;
    if (!pdnmlThis) {
        Cn3D_gModelSelect =
            NormalGroup(g, 0, 1, "Select Models:", systemFont, NULL);
        SetGroupMargins(Cn3D_gModelSelect, 10, 10);
        SetGroupSpacing(Cn3D_gModelSelect, 10, 5);
        StaticPrompt(Cn3D_gModelSelect, "No Models  ", 0, 0, systemFont,
                     'l');
        return Cn3D_gModelSelect;
    }
    VectorKeeper = KeepVectors;
    while (pdnmlThis) {
        pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
        if (!
            ((pmldThis->iType == Model_type_ncbi_vector)
             && (KeepVectors == FALSE))) {
            iCount++;
            pcModelString[0] = '\0';
            sprintf(pcModelString, "%d ", (int) pdnmlThis->choice);
            switch (pmldThis->iType) {
            case Model_type_ncbi_vector:
                StringCat(pcModelString, "Vector Model");
                break;
            case Model_type_ncbi_backbone:
                StringCat(pcModelString, "Backbone Model");
                break;
            case Model_type_ncbi_all_atom:
                StringCat(pcModelString, "One XYZ Coord/Atom Model");
                break;
            case Model_type_pdb_model:
                StringCat(pcModelString, "Original PDB Model");
                break;
            default:
            case Model_type_other:
                StringCat(pcModelString, "Other model");
            }
            ValNodeCopyStr(&pvnModelStrings, pdnmlThis->choice,
                           pcModelString);
        }
        pdnmlThis = pdnmlThis->next;
    }
    /* set up a group enclosing structures - models selection lists and - "info strings" */

    Cn3D_gModelSelect =
        NormalGroup(g, 0, 1, "Select Models:", systemFont, NULL);
    SetGroupMargins(Cn3D_gModelSelect, 10, 10);
    SetGroupSpacing(Cn3D_gModelSelect, 10, 5);
    Cn3D_lModels = MultiList(Cn3D_gModelSelect, 18, 3, Cn3D_ModelPickProc);
    pvnTemp = pvnModelStrings;
    while (pvnTemp) {
        ListItem(Cn3D_lModels, (CharPtr) pvnTemp->data.ptrvalue);
        pvnTemp = pvnTemp->next;
    }
    if (pvnModelStrings)
        ValNodeFreeData(pvnModelStrings);
    iCount = 0;
    pdnmlThis = pmsdThis->pdnmlModels;
    while (pdnmlThis) {         /* walk models, set select status for list */
        pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
        if (!
            ((pmldThis->iType == Model_type_ncbi_vector)
             && (KeepVectors == FALSE))) {
            iCount++;
            SetItemStatus(Cn3D_lModels, iCount,
                          (Boolean) (pmldThis->bSelected & (Byte) 0x01));
        } else {
            pmldThis->bSelected &= (Byte) 0xFE;
        }
        pdnmlThis = pdnmlThis->next;
    }
    return Cn3D_gModelSelect;
}


Int2Ptr LIBCALL PickedModels(Int4Ptr piNumModels)
{

    Int4 iCount = 0;
    PDNMS pdnmsMain = NULL;
    PMSD pmsdThis = NULL;
    PDNML pdnmlThis = NULL;
    PMLD pmldThis = NULL;
    ValNodePtr pvnModelNos = NULL;
    ValNodePtr pvnTemp = NULL;
    Int2Ptr i2Vec = NULL;

    pdnmsMain = GetSelectedModelstruc();
    if (!pdnmsMain) {
        *piNumModels = iCount;
        return NULL;
    }
    pmsdThis = (PMSD) pdnmsMain->data.ptrvalue;
    pdnmlThis = pmsdThis->pdnmlModels;

    /* make a linked-list of selected model id's */
    if (!pdnmlThis) {
        *piNumModels = iCount;
        return NULL;
    }
    while (pdnmlThis) {
        pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
        if (pmldThis->bSelected & (Byte) 0x01) {
            ValNodeAddInt(&pvnModelNos, pdnmlThis->choice,
                          pdnmlThis->choice);
            iCount++;
        }
        pdnmlThis = pdnmlThis->next;
    }

    /* Allocate the vector */

    if (iCount)
        i2Vec = I2Vector(0, iCount);
    else {
        return NULL;
    }
    /* copy the model numbers into vector */

    iCount = 0;
    pvnTemp = pvnModelNos;
    while (pvnTemp) {
        i2Vec[iCount] = pvnTemp->choice;
        iCount++;
        pvnTemp = pvnTemp->next;
    }

    /* free the linked-list */
    if (pvnModelNos)
        ValNodeFree(pvnModelNos);
    *piNumModels = iCount;
    return i2Vec;
}
