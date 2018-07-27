/*   cn3dmesh.c
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
* File Name:  cn3dmesh.c
*
* Author:  Lewis Geer
*
* First Version Creation Date:   1/31/96
*
* $Revision: 6.3 $
*
* File Description: 
*                   
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#include <vibrant.h>
#include <math.h>
#include <mmdbapi.h>
#include <cn3dmain.h>
#include <cn3dmesh.h>



static TexT Cn3D_tMesh;
static ButtoN Cn3D_bMeshAccept;

static WindoW Cn3D_wMesh;
static ButtoN Cn3D_bMeshBrowse;


static void Cn3D_MeshEnableProc(TexT t)
{
    Char str[32];
    GetTitle(Cn3D_tMesh, str, sizeof(str));
    if (StringLen(str) == 0) {
        Disable(Cn3D_bMeshAccept);
    } else {
        Enable(Cn3D_bMeshAccept);
    }
    return;
}



static void Cn3D_MeshBrowseProc(GraphiC g)
{
    Char path[PATH_MAX];

    path[0] = '\0';

    if (GetInputFileName(path, sizeof(path), NULL, NULL)) {
        SetTitle(Cn3D_tMesh, path);
        Cn3D_MeshEnableProc(NULL);
    }

    return;
}


static void Cn3D_MeshCancelProc(ButtoN b)
{
    Remove(Cn3D_wMesh);
    return;
}


Cn3D_Mesh Mesh;
Boolean MeshLoaded = FALSE;

static void Cn3D_MeshAcceptProc(ButtoN b)
{
    Char str[PATH_MAX];
    PMSD pmsdThis = NULL;
    FILE *hFile;
    FloatLo fDummy;
    Int4 lDummy, i;


    GetTitle(Cn3D_tMesh, str, sizeof(str));
    hFile = FileOpen(str, "r");
    fscanf(hFile, "%d %d %d", &(Mesh.NumVert), &(Mesh.NumEdge),
           &(Mesh.NumTri));

    Mesh.Vertices = MemNew(sizeof(Cn3D_Vert) * Mesh.NumVert);
    Mesh.Edges = MemNew(sizeof(Cn3D_Edge) * Mesh.NumEdge);
    Mesh.Tri = MemNew(sizeof(Cn3D_Tri) * Mesh.NumTri);

    for (i = 0; i < Mesh.NumVert; i++)
        fscanf(hFile, "%lf %lf %lf %lf %lf %lf %f %f %f %d %d %d",
               &(Mesh.Vertices[i].Vertex[0]),
               &(Mesh.Vertices[i].Vertex[1]),
               &(Mesh.Vertices[i].Vertex[2]),
               &(Mesh.Vertices[i].Normal[0]),
               &(Mesh.Vertices[i].Normal[1]),
               &(Mesh.Vertices[i].Normal[2]), &fDummy, &fDummy, &fDummy,
               &lDummy, &lDummy, &lDummy);

    for (i = 0; i < Mesh.NumEdge; i++)
        fscanf(hFile, "%d %d %d %d %d", &Mesh.Edges[i].Vertex[0],
               &Mesh.Edges[i].Vertex[1], &lDummy, &lDummy, &lDummy);

    for (i = 0; i < Mesh.NumTri; i++)
        fscanf(hFile, "%d %d %d %d %d %d %d %d %d", &Mesh.Tri[i].Edge[0],
               &Mesh.Tri[i].Edge[1], &Mesh.Tri[i].Edge[2],
               &Mesh.Tri[i].Vertex[0], &Mesh.Tri[i].Vertex[1],
               &Mesh.Tri[i].Vertex[2], &lDummy, &Mesh.Tri[i].Atom,
               &lDummy);

    MeshLoaded = TRUE;
    FileClose(hFile);


    Remove(Cn3D_wMesh);

    return;
}

void Cn3D_OpenMesh(IteM i)
{
    GrouP g;
    ButtoN b;


    Cn3D_wMesh = FixedWindow(-30, -20, -10, -10, " Open a mesh: ", NULL);
    g =
        NormalGroup(Cn3D_wMesh, 2, 1, " Enter mesh file name:", systemFont,
                    NULL);
    SetGroupMargins(g, 10, 10);
    SetGroupSpacing(g, 10, 20);
    Cn3D_tMesh = DialogText(g, "", 25, (TxtActnProc) Cn3D_MeshEnableProc);
    Cn3D_bMeshBrowse =
        PushButton(g, " browse...", (BtnActnProc) Cn3D_MeshBrowseProc);

    g = HiddenGroup(Cn3D_wMesh, 3, 1, NULL);
    SetGroupMargins(g, 10, 10);
    SetGroupSpacing(g, 30, 30);


    b = PushButton(g, "Cancel", (BtnActnProc) Cn3D_MeshCancelProc);
    Cn3D_bMeshAccept =
        DefaultButton(g, "OK", (BtnActnProc) Cn3D_MeshAcceptProc);

    Disable(Cn3D_bMeshAccept);
    Select(Cn3D_tMesh);
    Show(Cn3D_wMesh);
    return;
}
