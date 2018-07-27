/*   cn3dmesh.h
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
* File Name:  cn3dmesh.h
*
* Author:  
*
* Version Creation Date:   
*
* $Revision: 6.2 $
*
* File Description: 
*                   
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* ==========================================================================
*/

/* cn3dmesh.h */

#ifndef _CN3DMESH_
#define _CN3DMESH_

#ifdef __cplusplus
extern "C" {
#endif

extern void Cn3D_OpenMesh(Nlm_IteM i);

typedef struct _Cn3D_Vert {
    Nlm_FloatHi Vertex[3];
    Nlm_FloatHi Normal[3];
} Cn3D_Vert;

typedef struct _Cn3D_Edge {
    Nlm_Int4 Vertex[2];
} Cn3D_Edge;

typedef struct _Cn3D_Tri {
    Nlm_Int4 Edge[3];
    Nlm_Int4 Vertex[3];
    Nlm_Int4 Atom;
} Cn3D_Tri;

typedef struct _Cn3D_Mesh {
    Cn3D_Vert *Vertices;
    Cn3D_Edge *Edges;
    Cn3D_Tri *Tri;
    Int4 NumTri;
    Int4 NumVert;
    Int4 NumEdge;
} Cn3D_Mesh;

extern Cn3D_Mesh Mesh;

extern Boolean MeshLoaded;

#ifdef __cplusplus
}
#endif
#endif
