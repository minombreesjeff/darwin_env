/*   vibrant.h
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
* File Name:  vibrant.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   7/1/91
*
* $Revision: 6.1 $
*
* File Description: 
*       This is the master include file for the Vibrant (TM) portable user
*       interface.  It includes the Vibrant types, procedures, and defines,
*       and is the only header that most applications need to include.  The
*       first included file (vibtypes.h) includes ncbi.h.
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: vibrant.h,v $
* Revision 6.1  1999/05/06 21:23:44  vakatov
* Get rid of the erroneous 'extern "C"' around the #include's
*
* ==========================================================================
*/

#ifndef _VIBRANT_
#define _VIBRANT_


/***  GENERAL INFORMATION  ***/

/*
*  The NCBI portable user interface development library has been
*  written to allow new algorithms created by our scientists to
*  be quickly implemented and deployed to laboratory researchers
*  as user-friendly computer programs.  These programs run on
*  a variety of personal computers and graphics workstations that
*  are found in many molecular biology laboratories.  Because we
*  are a government agency, we will make the interface toolkit
*  available to scientists and mathematicians outside our group who
*  have similar needs, and to any interested commercial groups.
*/

/*
*  A program creates the windows, menus, and other user-responsive
*  graphical objects that it needs, attaches programmer-written
*  "callback" functions to appropriate objects (such as push buttons
*  or menu command items), and then calls ProcessEvents.  The object
*  itself handles all of its generic behavior automatically, while
*  the callback function is responsible for providing the specific
*  action that the user has requested.  Callback functions may modify
*  the appearance or behavior of other interface objects.  When the
*  program is ready to quit, it should call QuitProgram.  The ultimate
*  goal is to have identical application programs run properly on all
*  of the supported platforms.
*/

/*
*  A typical application program will need to include vibrant.h,
*  which imports portable numerical and graphic object types,
*  prototypes of the procedures for creating windows and their
*  graphic objects, and  define statements that alleviate the need
*  for Nlm_ prefixes in all procedures, types, and variables.  The
*  Nlm_ prefix is used in order to avoid collisions with symbols
*  defined by the various compilers and windowing systems under which
*  the interface will run.  When we provide documentation for use of
*  the interface, the definitions of procedures and types will not
*  have the Nlm_ prefix, although the .h and .c files will always
*  need them.  Similarly, object handle names end in a capital letter
*  (e.g., ButtoN, WindoW), in order to lessen the chance of collision
*  with operating system or application program symbols.
*/

/*
*  A SlatE object provides a generic drawing environment on all
*  platforms.  Each slate can contain several PaneL objects.  Each
*  panel object is given instance callback procedures for drawing,
*  responding to the initial mouse click, mouse drags, repeated
*  holding of the mouse button, and release of the mouse button.
*  The programmer can build new classes of objects for complicated
*  displays based on panel objects.
*/

/*
*  The platform is a four-dimensional specification of compiler,
*  operating system, processor, and windowing system flags for
*  conditional compilation of the NCBI User Interface Toolkit.
*  These (partially) independent dimensions allow us to write
*  one interface package that will run on the Macintosh under
*  THINK C and MPW C, on the IBM PC under Microsoft C and
*  Microsoft Windows, and on BSD and System V Unix machines
*  running the X Window or OSF/Motif windowing systems.
*/

/*
*  Familiarity with the NCBI core tools, and the platform concept,
*  are assumed.
*/

#ifndef _VIBTYPES_
#include <vibtypes.h>
#endif
#ifndef _VIBPROCS_
#include <vibprocs.h>
#endif
#ifndef _VIBDEFNS_
#include <vibdefns.h>
#endif
#ifndef _VIBFORMS_
#include <vibforms.h>
#endif

#endif
