/*   setrsrc.c
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
* File Name:  setrsrc.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   10/4/02
*
* $Revision: 1.4 $
*
* File Description:
*
*   Sets type and creator of files to get around a bug that prevents
*   CodeWarrior from seeing and loading them if they were checked out
*   directly on Mac OS X
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#include <cstdlib>
#include <cstring>
#include <Files.h>
#include <Processes.h>

#define DIRDELIMCHAR ':'
#define DIRDELIMSTRN ":"

static void C2PStr (char *ioStr)

{
  size_t len = strlen (ioStr);
  if (len > 255) {
    len = 255;
  }
  memmove (ioStr + 1, ioStr, len);
  ioStr [0] = len;
}

static void P2CStr (StringPtr ioStr)

{
  Byte len = ioStr [0];
  memmove (ioStr, ioStr + 1, len);
  ioStr [len] = '\0';
}

static char * MyStrMove (char * to, char * from)

{
  while (*from != '\0') {
    *to = *from;
    to++;
    from++;
  }
  *to = '\0';
  return to;
}

static char * MyFileBuildPath (char * root, char * sub_path, char * filename)

{
  char *         tmp;
  unsigned char  dir_start = 0;

  if (root == 0) return 0;

  tmp = root;
  if (*tmp != '\0') {
    dir_start = 1;
    while (*tmp != '\0') {
      tmp++;
    }

    if ((*(tmp - 1) != DIRDELIMCHAR) && (dir_start)) {
      *tmp = DIRDELIMCHAR;
      tmp++;
      *tmp = '\0';
    }
  }

  if (sub_path != 0) {
    if ((dir_start) && (*sub_path == DIRDELIMCHAR)) {
      sub_path++;
    }
    tmp = MyStrMove (tmp, sub_path);
    if (*(tmp-1) != DIRDELIMCHAR) {
      *tmp = DIRDELIMCHAR;
      tmp++;
      *tmp = '\0';
    }
  }

  if (filename != 0) {
    MyStrMove (tmp, filename);
  }

  return root;
}

static char * MyStrSave (const char * from)

{
  size_t  len;
 char *   to;

  len = strlen (from);
  to = malloc (len + 1);
  if (to != 0) {
    memcpy (to, from, len + 1);
  }
  return to;
}

typedef struct valnde {
  unsigned char    choice;
  void *           ptrvalue;
  struct valnde *  next;
} ValNde, * ValNdePtr;

static ValNdePtr ValNdeNew (ValNdePtr vnp)

{
  ValNdePtr  newnode;

  newnode = (ValNdePtr) malloc (sizeof (ValNde));
  if (newnode == 0) {
    return 0;
  }
  newnode->choice = 0;
  newnode->ptrvalue = 0;
  newnode->next = 0;
  if (vnp != 0) {
    while (vnp->next != NULL) {
      vnp = vnp->next;
    }
    vnp->next = newnode;
  }
  return newnode;
}

static ValNdePtr ValNdeAdd (ValNdePtr * head)

{
  ValNdePtr  newnode;

  if (head != 0) {
    newnode = ValNdeNew (*head);
    if (*head == 0) {
      *head = newnode;
    }
  } else {
    newnode = ValNdeNew (0);
  }

  return newnode;
}

static ValNdePtr ValNdeCopyStr (ValNdePtr * head, int choice, char * str)

{
  ValNdePtr  newnode;

  if (str == 0) return 0;

  newnode = ValNdeAdd (head);
  if (newnode != NULL) {
    newnode->choice = (unsigned char) choice;
    newnode->ptrvalue = MyStrSave (str);
  }

  return newnode;
}

static ValNdePtr ValNdeFreeData (ValNdePtr vnp)

{
  ValNdePtr  next;

  while (vnp != 0) {
    free (vnp->ptrvalue);
    next = vnp->next;
    free (vnp);
    vnp = next;
  }
  return 0;
}

static ValNdePtr MyDirCatalog (char * pathname)

{
  long            dirID;
  OSErr           err;
  short           index;
  unsigned short  num;
  char            path [256];
  CInfoPBRec      pbc;
  HParamBlockRec  pbh;
  short           vRefNum;
  ValNdePtr       vnp = 0;

  if (pathname != NULL && pathname [0] != '\0') {
    strncpy (path, pathname, sizeof (path));
    C2PStr ((char *) path);
    memset ((void *) (&pbh), 0, sizeof (HParamBlockRec));
    pbh.volumeParam.ioNamePtr = (StringPtr) path;
    pbh.volumeParam.ioVolIndex = -1;
    err = PBHGetVInfo (&pbh, FALSE);
    if (err != noErr) return NULL;
    vRefNum = pbh.volumeParam.ioVRefNum;
    strncpy (path, pathname, sizeof (path));
    C2PStr ((char *) path);
    memset ((void *) (&pbc), 0, sizeof (CInfoPBRec));
    pbc.dirInfo.ioNamePtr = (StringPtr) path;
    pbc.dirInfo.ioVRefNum = vRefNum;
    err = PBGetCatInfo (&pbc, FALSE);
    if (err != noErr) return NULL;
    if (pbc.dirInfo.ioFlAttrib & 16) {
      num = pbc.dirInfo.ioDrNmFls;
      dirID = pbc.dirInfo.ioDrDirID;
      for (index = 1; index <= num; index++) {
        memset ((void *) (&pbc), 0, sizeof (CInfoPBRec));
        pbc.dirInfo.ioNamePtr = (StringPtr) path;
        pbc.dirInfo.ioVRefNum = vRefNum;
        pbc.dirInfo.ioFDirIndex = index;
        pbc.dirInfo.ioDrDirID = dirID;
        pbc.dirInfo.ioACUser = 0;
        err = PBGetCatInfo (&pbc, FALSE);
        if (err == noErr) {
          P2CStr ((StringPtr) path);
          if (pbc.dirInfo.ioFlAttrib & 16) {
            ValNdeCopyStr (&vnp, 1, path);
          } else {
            ValNdeCopyStr (&vnp, 0, path);
          }
        }
      }
    }
  }
  return vnp;
}

static int IsSuffix (char * filename, char * suffix)

{
  size_t  flen;
  size_t  slen;

  flen = strlen (filename);
  slen = strlen (suffix);
  if (slen >= flen) return false;

  if (strcmp (filename + flen - slen, suffix) == 0) return true;

  return false;
}

static void SetTypeCreator (char * directory, char * filename, OSType creator, OSType type)

{
  OSErr  fError;
  FInfo  fInfo;
  char   path [256];

  strncpy (path, directory, sizeof (path));
  MyFileBuildPath (path, NULL, filename);
  C2PStr ((char *) path);
  fError = HGetFInfo (0, 0, (StringPtr) path, &fInfo);
  if (fError == noErr) {
    fInfo.fdCreator = creator;
    fInfo.fdType = type;
    fError = HSetFInfo (0, 0, (StringPtr) path, &fInfo);
  }
}

static void RecurseDir (char * directory, char * subpath)

{
  ValNdePtr  head;
  char       path [256];
  char *     str;
  ValNdePtr  vnp;

  strncpy (path, directory, sizeof (path));
  MyFileBuildPath (path, subpath, NULL);
  head = MyDirCatalog (path);
  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == 0) {
      str = (char *) vnp->ptrvalue;
      if (str == 0 || *str == '\0') continue;
      if (IsSuffix (str, ".h") ||
          IsSuffix (str, ".hpp") ||
          IsSuffix (str, ".c") ||
          IsSuffix (str, ".cpp") ||
          IsSuffix (str, ".r")) {
        SetTypeCreator (path, str, 'ttxt', 'TEXT');
      } else if (IsSuffix (str, ".met")) {
        if (strstr (path, "ncbi:make:") || strstr (path, "compilers:mac_prj:")) {
          SetTypeCreator (path, str, 'ToyS', 'TEXT');
        }
      } else if (IsSuffix (str, ".mcp")) {
        if (strstr (path, "ncbi:link:macmet:") || strstr (path, "compilers:mac_prj:")) {
          SetTypeCreator (path, str, 'CWIE', 'MMPr');
        }
      } else if (IsSuffix (str, ".plc")) {
        if (strstr (path, "ncbi:link:macmet:") || strstr (path, "compilers:mac_prj:")) {
          SetTypeCreator (path, str, 'ttxt', 'TEXT');
        }
      }
      /* set type/creator */
    } else if (vnp->choice == 1) {
      str = (char *) vnp->ptrvalue;
      RecurseDir (path, str);
    }
  }
  ValNdeFreeData (head);
}

static void MyProgramPath (char * buf, size_t maxsize)

{
  FSSpec               apFileSpec;
  CInfoPBRec           block;
  short                nErr;
  char                 path [256];
  ProcessInfoRec       pirec;
  ProcessSerialNumber  psn;
  char                 temp [256];

  GetCurrentProcess (&psn);
  pirec.processInfoLength = sizeof (ProcessInfoRec);
  pirec.processName = NULL;
  pirec.processAppSpec = &apFileSpec;
  GetProcessInformation (&psn, &pirec);
  P2CStr ((StringPtr) apFileSpec.name);

  if (buf != NULL && maxsize > 0) {
    *buf = '\0';
    memset (&block, 0, sizeof (CInfoPBRec));
    strncpy (path, (char *) apFileSpec.name, sizeof (path));

    block.dirInfo.ioNamePtr = (StringPtr) path;
    block.dirInfo.ioDrParID = apFileSpec.parID;

    do {
      strcpy (temp, path);
      block.dirInfo.ioVRefNum = apFileSpec.vRefNum;
      block.dirInfo.ioFDirIndex = -1;
      block.dirInfo.ioDrDirID = block.dirInfo.ioDrParID;
      nErr = PBGetCatInfo (&block, FALSE);
      if (nErr != noErr) break;
      P2CStr ((StringPtr) path);
      strcat (path, DIRDELIMSTRN);
      strcat (path, temp);
    } while (block.dirInfo.ioDrDirID != fsRtDirID);

    strncpy (buf, path, maxsize);
  }
}

extern int main (void)

{
  ValNdePtr  head;
  char       path [256];
  char *     ptr;
  char *     str;
  ValNdePtr  vnp;

  /* original place is inside ncbi or ncbi_cxx directory */

  MyProgramPath (path, sizeof (path));
  ptr = strrchr (path, DIRDELIMCHAR);
  if (ptr != 0) {
    *ptr = '\0';
  }
  RecurseDir ((char *) path, 0);

  /* look for executable in ncbi:link:macmet directory */

  ptr = strstr (path, ":ncbi:link:macmet");
  if (ptr != 0) {
    *ptr = '\0';
  } else {

    /* otherwise look for ncbi and ncbi_cxx directories as siblings of parent */

    ptr = strrchr (path, DIRDELIMCHAR);
    if (ptr != 0) {
      *ptr = '\0';
    }
  }

  head = MyDirCatalog (path);
  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == 1) {
      str = (char *) vnp->ptrvalue;
      if (strcmp (str, "ncbi") == 0 ||
          strcmp (str, "ncbi_cxx") == 0) {
        RecurseDir ((char *) path, str);
      }
    }
  }
  ValNdeFreeData (head);

  return 0;
}

