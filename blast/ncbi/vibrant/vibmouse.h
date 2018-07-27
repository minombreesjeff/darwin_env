/*  $Id: vibmouse.h,v 6.2 2004/04/01 13:43:09 lavr Exp $
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
* Author:  Denis Vakatov
*
* File Description: 
*   menu-driven interface to manage mouse-event callbacks
*   program interface to describe groups of mouse-event callbacks
*
* ==========================================================================
* $Log: vibmouse.h,v $
* Revision 6.2  2004/04/01 13:43:09  lavr
* Spell "occurred", "occurrence", and "occurring"
*
* Revision 6.1  1999/04/06 14:23:26  lewisg
* add opengl replacement for viewer3d
*
* Revision 6.0  1997/08/25 18:57:23  madden
* Revision changed to 6.0
*
* Revision 1.1  1997/03/20 16:23:48  vakatov
* Initial revision
*
*
* ==========================================================================
*/

#ifndef VIBMOUSE_H
#define VIBMOUSE_H

/* #include <vibrant.h> lyg */
#include <vibtypes.h>
#include <vibprocs.h>


#ifdef __cplusplus
extern "C" {
#endif


/* The mouse action callback types and modifiers(enumerated)
 */
typedef enum
{
  MK_Normal = 0,
  MK_Shift,
  MK_Ctrl,

  /* this one must not be used by the user -- for internal affairs only!     */
  /* (in internals, it means that presently there is no action "in progress" */
  MK_Default
}
enumMKey;


typedef enum
{
  MA_Click = 0,
  MA_Press,
  MA_DClick,
  MA_Hold,
  MA_Drag,
  MA_Release,
  MA_Cancel,

  /* insensitive to the modifier keys */
  MA_Init,
  MA_Done,

  /* this one must not be used by the user -- for internal affairs only! */
  MA_Default
}  enumMAction;


/*---[ MA_Create ]----------------------------------------
 * Create new set of the MA(Mouse-Action) controls;
 *  settle these as a system of pulldown menus attached
 *  to the menu(s) "group_menu" and/or "action_menu";
 *  the latter two can be NULL and can be the same menu or submenu.
 * (If "menu" == NULL then just do not create an interface.)
 * Return handler of the created MA.
 */
struct _MA;
typedef struct _MA PNTR MAPtr;

extern MAPtr MA_Create(Nlm_MenU group_menu, Nlm_MenU action_menu);


/*---[ MA_AddAction ]----------------------------------------------------
 * For MA controls "ma": register new mouse action of type "type",
 * having the callback function "func", user data "data"(may be NULL)
 * and name "name"(may be NULL).
 */
struct _MAction;
typedef struct _MAction PNTR MActionPtr;

typedef struct
{
  Nlm_PoinT start;
  Nlm_PoinT end;
}
MA_Trace, PNTR MA_TracePtr;

/* Type of the user-defined callback function
 */
typedef void (*MA_Func)  PROTO (
(
 MAPtr       ma,     /* handle of the MA controls */
 MA_TracePtr trace,  /* aux. memory area(unique for each MA) */
 Nlm_PoinT       point,  /* present coordinates of the mouse */
 VoidPtr     extra   /* extra data(specified by user in MA_AddAction()) */
 )
);

extern MActionPtr MA_AddAction(MAPtr            ma,
                               enumMKey         mod_key,
                               enumMAction      type,
                               MA_Func          func,
                               VoidPtr          data,
                               const Char PNTR  name);

/* Dummy callback to use in MA_AddAction() -- just for user's convenience */
extern void DoNothingMA(MAPtr ma, MA_TracePtr trace, Nlm_PoinT point,
                        VoidPtr extra);


/*---[ MA_SetAction ]----------------------------------------------------
 * Set the "action".
 * If the action of this type is marked as "only" then:
 *   if "can_unset_group" is TRUE then the relevant group will be unset,
 *   else do nothing and return FALSE.
 * NOTE: The "action" must not be NULL
 */
extern Boolean MA_SetAction(MActionPtr action, Boolean can_unset_group);


/*---[ MA_UnsetAll ]----------------------------------------------------
 * Disable all currently set mouse actions.
 */
extern Boolean MA_UnsetAll(MAPtr ma);


/*---[ MA_AddGroup ]-----------------------------------------------------
 * Register new mouse action group.
 * The "name" must not be NULL and different groups must have different names;
 * all enlisted mouse actions must be registered by the moment by the means of
 * "Nlm_AddAction()" and must have different types(in the sense of the
 * ("enumMKey", "enumMAction") unique pair).
 * "only" == MA_ONLY means that the relevant "action" cannot be later changed
 * without unsetting the whole group.
 * NOTE: The last argument *must* be NULL!
 */
struct _MA_Group;
typedef struct _MA_Group PNTR MA_GroupPtr;


#define MA_SHARED ((long)0)
#define MA_ONLY   ((long)1)
extern MA_GroupPtr MA_AddGroup(MAPtr ma,  const Char PNTR name,
                               MActionPtr action, long only,
                               /* and more pairs: <action>, <only>; */
                               /* then, the last arg. must be NULL! */
                               ...);


/*---[ MA_SetGroup ]-----------------------------------------------------
 * Set the "group"'s mouse actions.
 * The "only"-actions of this group will replace any currently active
 * actions;  if one of these replaces an "only" action of another
 * (presently active) group then the whole latter group will be unset.
 * The non-"only"-actions will replace any presently active non-"only"
 * actions.
 * NOTE: The "group" must not be NULL
 */
extern Boolean MA_SetGroup(MA_GroupPtr group);


/*---[ MA_SetGroup ]-----------------------------------------------------
 * Unset the group of mouse actions.
 * Only "only" actions of the group will be disabled.
 * NOTE:  "group" must not be NULL
 */
extern Boolean MA_UnsetGroup(MA_GroupPtr group);


/*---[ MA_LinkPanel ]----------------------------------------------------
 * Wrap the control over the "panel"' mouse events into the "ma";
 * NOTE: Previously specified mouse-event handlers and first
 *       sizeof(MAPtr) bytes of 'extra' data of the "panel" will be
 *       overwritten and then used by the MA internals! (Thus, the
 *       panel's extra data must have size at least sizeof(MAPtr).)
 */
extern Boolean MA_LinkPanel(MAPtr ma, Nlm_PaneL panel);


/*---[ MA_GetPanel ]-----------------------------------------------------
 * Get handle of the panel presently linked to the "ma"
 */
extern Nlm_PaneL MA_GetPanel(MAPtr ma);


/*---[ MA_UnlinkPanel ]--------------------------------------------------
 * Give away the control over the mouse events occurred in the
 * panel(if any) presently wrapped by "ma";  disable all mouse handlers
 * previously set for this panel
 */
extern void MA_UnlinkPanel(MAPtr ma);


/*---[ MA_[Set/Get]Visible ]---------------------------------------------
 * Set/Get visibility of the MA controls referenced by "ma"
 */
extern Boolean MA_SetVisible(MAPtr ma, Boolean visibility);
extern Boolean MA_GetVisible(MAPtr ma);


/*---[ MA_[Set/Get]Extra ]-----------------------------------------------
 * Set/Get extra data pointer of the MA controls referenced by "ma"
 */
extern void    MA_SetExtra(MAPtr ma, VoidPtr extra);
extern VoidPtr MA_GetExtra(MAPtr ma);


/*---[ MA_Reset ]--------------------------------------------------------
 * Reset the MA internals and interface to the "just born" state -- i.e.
 * as if it was just created by the MA_Create() function
 * (extra & panel will be reset as well!)
 */
extern void MA_Reset(MAPtr ma);


/*---[ MA_Destroy ]------------------------------------------------------
 * Destroy the MA internal structures and user interface controls
 */
extern void MA_Destroy(MAPtr ma);


#ifdef __cplusplus
}
#endif

#endif  /* VIBMOUSE_H */

