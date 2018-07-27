/*
 * $Id: vastgenDB.h,v 1.2 2003/01/14 20:27:00 chenj Exp $
 *
 *
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
 *
 * Author: Jie Chen
 *
 *
 * $Log: vastgenDB.h,v $
 * Revision 1.2  2003/01/14 20:27:00  chenj
 * Minor changes
 *
 *
 *
 *
 * =========================================================================
 *
 */

#ifndef _VASTGEN_
#define _VASTGEN_

#include "/netopt/structure/include/dart.h"
#include "/netopt/structure/include/dartutil.h"
#include "/home/he/proj/VastSrv/PubVastApi.h" 
#include "/home/he/proj/MmdbSrv/PubStructApi.h"
#include "/net/scarecrow/str3/web/dev/htdocs/Structure/chenj/bin/utilisCJ.h"

#define MaxSeqImgSize   620
#define GraphWidth      770

#define MAX_TBUFF	8192
#define LOG10_500       2.69897    /* -log10(500); database size correction */
#define LOG_10          2.302585        /* log(10.0) */
#define ASP_SCALE_FACTOR        10000
#define ParURL 		"\"%s%s?sdid=%d&sort=%d&"
#define PageSubsetURL	"doclistpage=%d&subset=%d&presubset=%d"
#define	VSParURL	"\"%s%s?chaindom=%d&sort=%d&"

#endif
