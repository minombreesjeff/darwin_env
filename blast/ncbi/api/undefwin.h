/*   $Id: undefwin.h,v 1.1 2000/03/06 17:58:21 lewisg Exp $
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
* File Name:  $Id: undefwin.h,v 1.1 2000/03/06 17:58:21 lewisg Exp $
*
* Author:  Lewis Geer
*
* Version Creation Date:   3/06/00
*
* $Revision: 1.1 $
*
* File Description:
*  undef's definitions in the toolkit that interfere with win32 definitions
*  in windows.h.  Behavior
*  of this is undetermined (the win32 macro can be completely different from
*  the toolkit macro), so use with caution.
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: undefwin.h,v $
* Revision 1.1  2000/03/06 17:58:21  lewisg
* file to undef macro conflicts with win32
*
*
* ==========================================================================
*/

#ifdef _WIN32
#undef Beep
#undef CreateFont
#undef FrameRgn
#undef EqualRgn
#undef InvertRgn
#undef LineTo
#undef PaintRgn
#undef StartPage
#undef EndPage
#undef DrawText
#undef FrameRect
#undef InvertRect
#undef UnionRect
#undef OffsetRect
#undef EqualRect
#undef PtInRect
#undef ChooseFont
#undef SetPort
#undef UnionRgn
#undef XorRgn
#undef DeleteFont
#undef SelectFont
#undef InsetRect
#undef HANDLE
#endif
