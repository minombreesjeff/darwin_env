/*  $Id: vibmouse.c,v 6.4 2001/03/28 01:40:48 juran Exp $
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
*   user interface to manage mouse-event callbacks
*   program interface to describe groups of mouse-event callbacks
*
* ===========================================================================
* $Log: vibmouse.c,v $
* Revision 6.4  2001/03/28 01:40:48  juran
* "for (...) { continue; }", squelch warning
*
* Revision 6.3  1999/04/06 14:23:25  lewisg
* add opengl replacement for viewer3d
*
* Revision 6.2  1998/07/02 18:24:34  vakatov
* Cleaned the code & made it pass through the C++ compilation
*
* Revision 6.1  1997/11/26 21:30:31  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.0  1997/08/25 18:57:21  madden
* Revision changed to 6.0
*
* Revision 1.3  1997/05/06 16:27:07  vakatov
* Ignore the MA_Drag and MA_Release events not preceded by MA_Press
*
 * Revision 1.2  1997/03/21  16:15:12  vakatov
 * Removed #<ni_list.h> -- as the list stuff has been moved to "ncbimisk.[ch]"
 *
 * Revision 1.1  1997/03/20  16:23:26  vakatov
 * Initial revision
 *
*
* ==========================================================================
*/

#ifdef VAR_ARGS
#include <varargs.h>
#else
#include <stdarg.h>
#endif
#include <vibrant.h>  /* move out of header.  lyg */
#include <vibmouse.h>


#define BAD ASSERT(FALSE)


/*****************************************************************************
 *  Internal Data and Type Definitions
*****************************************************************************/

static const Char *MA_name[MK_Default][MA_Init] =
{
  {
    "Click",
    "Press",
    "DoubleClick",
    "Hold",
    "Drag",
    "Release",
    "Cancel"
  },

  {
    "Shift-Click",
    "Shift-Press",
    "Shift-DoubleClick",
    "Shift-Hold",
    "Shift-Drag",
    "Shift-Release",
    "Shift-Cancel"
  },

  {
    "Ctrl-Click",
    "Ctrl-Press",
    "Ctrl-DoubleClick",
    "Ctrl-Hold",
    "Ctrl-Drag",
    "Ctrl-Release",
    "Ctrl-Cancel"
  }
};


typedef struct _MAction
{
  enumMKey     mod_key;
  enumMAction  type;
  MA_Func      func;
  VoidPtr      data;
  CharPtr      name;
  IteM         item;
  MAPtr        ma;
}
MAction;

typedef struct _MA_Group
{
  CharPtr    name;  /* the group name as it appears in the group menu */
  MActionPtr init;
  MActionPtr done;
  MActionPtr action[MK_Default][MA_Init];
  Boolean    only  [MK_Default][MA_Init]; /* TRUE if cannot be replaced by a
                         "foreign" callback while the group is active */
  IteM       item;
  MAPtr      ma;
}
MA_Group;



typedef struct _MA
{
  /* interface */
  MenU        group_menu;
  MenU        action_menu;
  MenU        menu[MK_Default][MA_Init];

  /* all registered actions and groups */
  NodePtr     actions;
  NodePtr     groups;

  /* presently active groups and actions */
  NodePtr     active_groups;
  MActionPtr  action[MK_Default][MA_Init];
  Boolean     only  [MK_Default][MA_Init];

  /* presently active(snapshot during the last "Press" action) modifier */ 
  enumMKey    mod_key;

  /* type and position of the most recent mouse action(but MA_Hold) */
  enumMAction last_type;
  PoinT       last_pos;

  /* auxiliary temporary store area shared between the action callbacks */
  MA_Trace    trace;

  /* presently linked panel(slate) */
  PaneL       panel;

  /* user-specified pointer to an extra data */
  VoidPtr     extra;
}
MA;



/*****************************************************************************
 *  Internal (Static)  Functions
*****************************************************************************/

/* In the "action->ma", find the active group containing 
 * the "only" action of type "action->mod_key/type"
 */ 
static MA_GroupPtr GroupOfOnlyAction(MActionPtr action)
{
  MAPtr   ma   = action->ma;
  NodePtr node = ma->active_groups;

  ASSERT ( ma->only[action->mod_key][action->type] );

  while ( node )
    {
      MA_GroupPtr mag = (MA_GroupPtr)node->elem;
      if ( mag->only[action->mod_key][action->type] )
        return mag;

      node = ListGetNext( node );
    }

  BAD;
  return NULL;
}


/* Set current action according to the submenu item click
 */
static void MA_ActionItemCB(IteM item)
{
  MActionPtr action = (MActionPtr)GetObjectExtra( item );
 
  VERIFY ( MA_SetAction(action, TRUE) );
}


/* Add the action item to the relevant submenu
 */
static Boolean MA_ShowAction(MActionPtr action)
{
  MAPtr ma;
  if (action == NULL  ||  action->name == NULL  ||
      (ma = action->ma) == NULL  ||  ma->action_menu == NULL  ||
      !ma->action_menu  ||  !ma->menu[action->mod_key][action->type])
    return FALSE;

  if (action->item != NULL)
    return TRUE;

  action->item = CommandItem(ma->menu[action->mod_key][action->type],
                             action->name, MA_ActionItemCB);
  if ( !action->item )
    return FALSE;

  SetObjectExtra(action->item, action, NULL);
  return TRUE;
}


/*  Activate/deactivate the mouse callback group "group"
 */
static void MA_GroupItemCB(IteM item)
{
  MA_GroupPtr group = (MA_GroupPtr)GetObjectExtra( item );

  if ( GetStatus( item ) )
    VERIFY ( MA_SetGroup( group ) );
  else
    VERIFY ( MA_UnsetGroup( group ) );
}


/*  Add the "group"'s name to the list of mouse callback groups
 */
static Boolean MA_ShowGroup(MA_GroupPtr group)
{
  if (group                 != NULL  &&
      group->name           != NULL  &&
      group->ma             != NULL  &&
      group->ma->group_menu != NULL  &&
      (group->item = StatusItem(group->ma->group_menu, group->name,
                                MA_GroupItemCB)) != NULL)
    {
      SetObjectExtra(group->item, group, NULL);
      return TRUE;
    }

  return FALSE;
}


/* Measure distance between the two points
 */
static double PointDist(PoinT p1, PoinT p2)
{
  return sqrt( (double)((p2.x - p1.x) * (p2.x - p1.x) +
                        (p2.y - p1.y) * (p2.y - p1.y)));
}

/* The Panel-to-MA callback dispatcher
 */
static void MA_Execute(MAPtr ma, PoinT pt, enumMAction type)
{
  ASSERT ( 0 <= type  &&  type < MA_Init );

  /* set the key status of the action in progress */
  if (type == MA_Press  ||  type == MA_DClick  ||  ma->mod_key == MK_Default)
    ma->mod_key = Nlm_shftKey ? MK_Shift :
                  Nlm_ctrlKey ? MK_Ctrl  :
                  MK_Normal;

  /* Refuse MA_Drag and MA_Release not preceded by MA_Press */
  if ((type == MA_Drag  ||  type == MA_Release)  &&
      ma->last_type != MA_Press  &&  ma->last_type != MA_Drag)
    return;

  /* MA_Click vs. MA_Drag|MA_Release */
  if (ma->last_type == MA_Press)
    {
      double CLICK_RADIUS = (double)8;
      switch ( type )
        {
        case MA_Drag:
          if (PointDist(pt, ma->last_pos) <= CLICK_RADIUS)
            return;
          break;

        case MA_Release:
          if (PointDist(pt, ma->last_pos) <= CLICK_RADIUS)
            type = MA_Click;
          break;

        default:
          break;
        }
    }

  {{ /* call the relevant user-specified action callback, if any */
    MActionPtr action = ma->action[ma->mod_key][type];
    if ( action )
      {
        ASSERT ( action->ma   == ma   );
        ASSERT ( action->type == type );
        (*action->func)(ma, &ma->trace, pt, action->data);
      }
  }}

  if (type == MA_Release  ||  type == MA_Cancel  ||  type == MA_DClick)
    ma->mod_key = MK_Default;  /* no action in progress */

  if (type != MA_Hold)
    {  
      ma->last_type = type;
      ma->last_pos  = pt;
    }
}

static void MA_ExecuteP(PaneL panel, PoinT pt, enumMAction type)
{
  MAPtr ma = NULL;
  GetPanelExtra(panel, &ma);
  MA_Execute(ma, pt, type);
}


/* MA wrappers of the panel callbacks
 */
static void MA_ClickCB(PaneL panel, PoinT pt)
{
  if ( Nlm_dblClick )
    MA_ExecuteP(panel, pt, MA_DClick);
  else
    MA_ExecuteP(panel, pt, MA_Press);
}


static void MA_HoldCB(PaneL panel, PoinT pt)
{
  MA_ExecuteP(panel, pt, MA_Hold);
}


static void MA_DragCB(PaneL panel, PoinT pt)
{
  MA_ExecuteP(panel, pt, MA_Drag);
}


static void MA_ReleaseCB(PaneL panel, PoinT pt)
{
  MA_ExecuteP(panel, pt, MA_Release);
}



/*****************************************************************************
 *  External Functions
*****************************************************************************/

/* Creation functions
 */
extern MAPtr MA_Create(MenU group_menu, MenU action_menu)
{
  MAPtr ma = (MAPtr)MemNew( sizeof(MA) );
  if ( !ma ) {
    BAD;
    return NULL;
  }

  ma->group_menu  = group_menu;
  ma->action_menu = action_menu;

  MA_Reset( ma );
  return ma;
}


/* DUMMY MA callback function
 */
extern void DoNothingMA(MAPtr ma, MA_TracePtr trace, PoinT point,
                        VoidPtr extra)
{
  /* do nothing */
}


extern MActionPtr MA_AddAction(MAPtr            ma,
                               enumMKey         mod_key,
                               enumMAction      type,
                               MA_Func          func,
                               VoidPtr          data,
                               const Char PNTR  name)
{
  MActionPtr new_action;
  NodePtr    new_node;

  if (ma == NULL  ||  mod_key == MK_Default  ||  type == MA_Default  ||
      func == NULL)
    {
      BAD;
      return NULL;
    }
  ASSERT ( name == NULL  ||  *name != '\0' );

  new_action = (MActionPtr) MemNew( sizeof(MAction) );
  new_node = ListInsertPrev(new_action, ma->actions);
  if (new_node == NULL)
    {
      BAD;
      MemFree( new_action );
      return NULL;
    }
  if (ma->actions == NULL)
    ma->actions = new_node;

  new_action->mod_key = mod_key;
  new_action->type    = type;
  new_action->func    = func;
  new_action->data    = data;
  new_action->name    = StringSave( name );
  new_action->ma      = ma;
  MA_ShowAction( new_action );

  return new_action;
}


extern Boolean MA_SetAction(MActionPtr action, Boolean can_unset_group)
{
  MAPtr       ma        = action->ma;
  MActionPtr *ma_action = &ma->action[action->mod_key][action->type];

  if ( ma->only[action->mod_key][action->type] )
    {
      if ( can_unset_group )
        {
          VERIFY ( MA_UnsetGroup( GroupOfOnlyAction(action) ) );
          ASSERT ( !ma->only[action->mod_key][action->type] );
        }
      else
        return FALSE;
    }

  if (*ma_action  &&  (*ma_action)->item)
    Enable( (*ma_action)->item );
  if (action  &&  action->item)
    Disable( action->item );

  *ma_action = action;
  return TRUE;
}


extern Boolean MA_UnsetAll(MAPtr ma)
{
  while ( ma->active_groups )
    if ( !MA_UnsetGroup( (MA_GroupPtr)ma->active_groups->elem ) )
      {
        BAD;
        return FALSE;
      }

  {{
    int i,j;
    for (i = 0;  i < MK_Default;  i++)
    for (j = 0;  j < MA_Init;     j++)
      {
        ASSERT ( !ma->only[i][j] );
        if ( ma->action[i][j] )
          {
            if ( ma->action[i][j]->item )
              Enable( ma->action[i][j]->item );
            ma->action[i][j] = NULL;
          }
      }
  }}

  return TRUE;
}


#ifdef VAR_ARGS
extern MA_GroupPtr MA_AddGroup(ma, name, action, only, va_alist)
MAPtr            ma;
const Char PNTR  name;
MActionPtr       action;
long             only;
va_dcl
#else
extern MA_GroupPtr MA_AddGroup(MAPtr ma,  const Char PNTR name,
                               MActionPtr action,  long only,
                               /* and more pairs: <action>, <only>; */
                               /* then, the last arg. must be NULL! */
                               ...)
#endif
{
  MA_GroupPtr new_group;

  if (ma == NULL  ||  name == NULL  ||  action == NULL)
    {
      BAD;
      return NULL;
    }

  new_group = (MA_GroupPtr)MemNew( sizeof(MA_Group) );
  
  {{ /* Extract the group actions from the list of arguments */
    va_list args;
#ifdef VAR_ARGS
    va_start(args);
#else
    va_start(args, only);
#endif
    for ( ; action != NULL;
         action = va_arg(args, MActionPtr),
         only   = va_arg(args, long))
      {
        MActionPtr *dest_action = NULL;

        switch ( action->type )
          {
          case MA_Init:
            dest_action = &new_group->init;
            break;

          case MA_Done:
            dest_action = &new_group->done;
            break;

          default:
            dest_action = &new_group->action[action->mod_key][action->type];
            new_group->only[action->mod_key][action->type] =
              (Boolean)(only == MA_ONLY);
            break;
          }
        
        if (*dest_action != NULL)
          {
            BAD;
            MemFree( new_group );
            return NULL;
          }

        *dest_action = action;
      }
    va_end(args);
  }}

  {{ /* Insert the new group to the end of the list of groups */
    NodePtr new_node = ListInsertPrev(new_group, ma->groups);
    if (new_node == NULL)
      {
        BAD;
        MemFree( new_group );
        return NULL;
      }
    if (ma->groups == NULL)
      ma->groups = new_node;
  }}

  new_group->name = StringSave( name );
  new_group->ma   = ma;
  MA_ShowGroup( new_group );

  return new_group;
}


extern Boolean MA_UnsetGroup(MA_GroupPtr group)
{
  MAPtr ma = group->ma;
 
  {{ /* find and exclude the "group" from the list of active groups */
    NodePtr node;
    for (node = ma->active_groups;
         node  &&  node->elem != group;
         node = ListGetNext( node ))
    {
      continue;
    }

    if ( !node )
      return FALSE;

    if (node == ma->active_groups)
      ma->active_groups = ListDelete( node );
    else
      ListDelete( node );
  }}

  {{ /* unset the "group"'s "only" actions */
    int i, j;
    for (i = 0;  i < MK_Default;  i++)
    for (j = 0;  j < MA_Init;     j++)
      if ( group->only[i][j] )
          {
            ASSERT ( ma->action[i][j] == group->action[i][j] );
            ASSERT ( ma->only  [i][j] );
            if ( ma->menu[i][j] )
              Enable( ma->menu[i][j] );
            ma->action[i][j] = NULL;
            ma->only  [i][j] = FALSE;
          }
  }}

  /* call the group's "done" method(i.e. the user-defined group destructor);
     always call the "cancel" method before -- if there is a mouse
     action "in progress" */
  if (ma->mod_key != MK_Default)
    MA_Execute(ma, ma->trace.end, MA_Cancel);

  if (group->item != NULL)
    SetStatus(group->item, FALSE);

  if ( group->done )
    (*group->done->func)(ma, &ma->trace, ma->trace.end, group->done->data);

  return TRUE;
}


extern Boolean MA_SetGroup(MA_GroupPtr group)
{
  int   i, j;
  MAPtr ma = group->ma;

  {{ /* check for the "only"-actions double-crossing; in that case
      * unset all "only" actions of the old group(i.e. unset the group) */
    for (i = 0;  i < MK_Default;  i++)
    for (j = 0;  j < MA_Init;     j++)
      if (ma->only[i][j]  &&  group->only[i][j]  &&
          !MA_UnsetGroup( GroupOfOnlyAction(group->action[i][j]) ))
        {
          BAD;
          return FALSE;
        }
  }}

  /* replace all old non-"only" actions with these from the "group" */
  for (i = 0;  i < MK_Default;  i++)
  for (j = 0;  j < MA_Init;     j++)
    {
      if (group->action[i][j] != NULL)
        {
          ASSERT ( !group->only[i][j]  ||  !ma->only[i][j]);
          if ( ma->only[i][j] )
            {
              ASSERT ( !group->only[i][j] );
            }
          else
            {
              if (ma->action[i][j]  &&  ma->action[i][j]->item)
                Enable( ma->action[i][j]->item );
              ma->action[i][j] = group->action[i][j];
              ma->only  [i][j] = group->only  [i][j];
            }
        }

      if ( ma->only[i][j] )
        Disable( ma->menu[i][j] );
    }

  ListConnectRing( ma->active_groups );
  ma->active_groups = ListInsertPrev(group, ma->active_groups);
  ListBreakRing( ma->active_groups );

  if (group->item != NULL)
    SetStatus(group->item, TRUE);

  /* call the group "init" method (user-defined group constructor) */
  ma->trace.end.x = ma->trace.end.y = 0;
  ma->trace.start = ma->trace.end;
  if ( group->init )
    (*group->init->func)(ma, &ma->trace, ma->trace.end, group->init->data);

  return TRUE;
}


extern Boolean MA_LinkPanel(MAPtr ma, PaneL panel)
{
  if (ma == NULL  ||  panel == NULL)
    return FALSE;

  SetPanelClick(panel, MA_ClickCB, MA_DragCB, MA_HoldCB, MA_ReleaseCB);
  SetPanelExtra(panel, &ma); 
  ma->panel = panel;
  return TRUE;
}


extern PaneL MA_GetPanel(MAPtr ma)
{
  return (PaneL) (ma ? ma->panel : NULL);
}


extern void MA_UnlinkPanel(MAPtr ma)
{
  MAPtr ma_NULL = NULL;

  if (ma == NULL  ||  ma->panel == NULL)
    return;

  SetPanelClick(ma->panel, NULL, NULL, NULL, NULL);
  SetPanelExtra(ma->panel, &ma_NULL);
  ma->panel = NULL;
}


extern Boolean MA_SetVisible(MAPtr ma, Boolean visibility)
{
  if (!ma  ||  (!ma->group_menu  &&  !ma->action_menu))
    return FALSE;

  if ( visibility )
    {
      Show( ma->group_menu  );
      Show( ma->action_menu );
    }
  else
    {
      Hide( ma->group_menu  );
      Hide( ma->action_menu );
    }

  return TRUE;
}


extern Boolean MA_GetVisible(MAPtr ma)
{
  return (Boolean)
    (ma  &&
     ((ma->group_menu   &&  Visible(ma->group_menu )) ||
      (ma->action_menu  &&  Visible(ma->action_menu))));
}


extern void MA_SetExtra(MAPtr ma, VoidPtr extra)
{
  ma->extra = extra;
}


extern VoidPtr MA_GetExtra(MAPtr ma)
{
  return (ma ? ma->extra : NULL);
}


extern void MA_Reset(MAPtr ma)
{
  int i, j;

  /* unset all presently active groups */
  while ( ma->active_groups )
    VERIFY ( MA_UnsetGroup( (MA_GroupPtr)ma->active_groups->elem ) );

  /* Unlink panel */
  MA_UnlinkPanel( ma );

  /* Reset menu-based interface */
  if ( ma->action_menu )
    {
      Reset( ma->action_menu );

      for (i = 0;  i < MK_Default;  i++)
      for (j = 0;  j < MA_Init;     j++)
        ma->menu[i][j] = SubMenu(ma->action_menu, (CharPtr)MA_name[i][j]);
    }

  if ( ma->group_menu )
    {
      if (ma->group_menu == ma->action_menu)
        SeparatorItem( ma->action_menu );
      else
        Reset( ma->group_menu );
    }

  /* Reset internal data */
  MA_SetExtra(ma, NULL);

  while (ma->groups != NULL)
    {
      MemFree( ((MA_GroupPtr)ma->groups->elem)->name );
      MemFree( ma->groups->elem );
      ma->groups = ListDelete( ma->groups );
    }

  for (i = 0;  i < MK_Default;  i++)
  for (j = 0;  j < MA_Init;     j++)
    {
      ma->action[i][j] = NULL;
      ma->only  [i][j] = FALSE;
    }

  while ( ma->actions )
    {
      MemFree( ((MActionPtr)ma->actions->elem)->name );
      MemFree( ma->actions->elem );
      ma->actions = ListDelete( ma->actions );
    }

  ma->mod_key   = MK_Default;
  ma->last_type = MA_Default;
}


extern void MA_Destroy(MAPtr ma)
{
  MA_Reset( ma );
  MemFree( ma );
}


/* EOF ($RCSfile: vibmouse.c,v $) */

