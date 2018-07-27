#include <objres.h>
#include <objseq.h>

#include <picture.h>
#include <viewer.h>

#include <seqport.h>
#include <seqgraph.h>
#include <seqmtrx.h>

/* defines */
#define FILTS  32
#define SQRDBF 8192

/* functions */

static ComMatPtr ComMatNew (ComMatPtr curcmtp)
{
  ComMatPtr   cmtp;
  Int4        i;

  cmtp = (ComMatPtr) MemNew (sizeof (ComMat));
  cmtp->next = NULL;
  cmtp->min = 0;
  cmtp->max = 0;
  for (i=0; i<FILTS; i++)
    cmtp->res[i] = 0;

  if (curcmtp != NULL)
  {
    while (curcmtp->next != NULL)
      curcmtp = curcmtp->next;
    curcmtp->next = cmtp;
  }
  return cmtp;
}

extern ComMatPtr ComMatFree (ComMatPtr headcmtp)
{
  ComMatPtr   cmtp;

  while (headcmtp != NULL)
  {
    cmtp = headcmtp->next;
    headcmtp->next = NULL;
    MemFree (headcmtp);
    headcmtp = cmtp;
  }
  return headcmtp;
}

extern ComMatPtr CompileMatrix (FloatHi scr[FILTS][FILTS], Int4 len,
                                Int4 maxval)
{
  Int4      i, j, tmp;
  Char      k[FILTS] = {'A', 'B', 'C', 'D', 'G', 'H', 'K', 'M',
                        'N', 'R', 'S', 'T', 'V', 'W', 'Y', 'N',
                        'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N',
                        'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N'};

  ComMatPtr cmtphead = NULL, cmtp = NULL;

  for (i = 0; i < len; i++)
  {
    cmtp = ComMatNew (cmtp);
    cmtp->max = maxval;
    if (cmtphead == NULL)
      cmtphead = cmtp;
    for (j = 0; j < FILTS; j++)
      cmtp->res[(Uint1) k[j] - (Uint1) 'A'] = (Int4) scr[j][i];
    tmp = 0;
    for (j = 0; j < FILTS; j++)
    {
      if (cmtp->res[j] > tmp)
        tmp = cmtp->res[j];
      cmtp->min = tmp;
    }
  }
  return cmtphead;
}

extern void InvertMatrix (ComMatPtr cmtp, Int4 window)
{
  Int4      i, j, k;
  Int4      ftemp;
  ComMatPtr cmtphead, cmtptail;

/* complement */
  cmtphead = cmtp;
  while (cmtp != NULL)
  {
/* A <-> T */
    ftemp = cmtp->res[19];
    cmtp->res[19] = cmtp->res[0];
    cmtp->res[0] = ftemp;
/* B <-> V */
    ftemp = cmtp->res[21];
    cmtp->res[21] = cmtp->res[1];
    cmtp->res[1] = ftemp;
/* C <-> G */
    ftemp = cmtp->res[6];
    cmtp->res[6] = cmtp->res[2];
    cmtp->res[2] = ftemp;
/* D <-> H */
    ftemp = cmtp->res[7];
    cmtp->res[7] = cmtp->res[3];
    cmtp->res[3] = ftemp;
/* K <-> M */
    ftemp = cmtp->res[12];
    cmtp->res[12] = cmtp->res[10];
    cmtp->res[10] = ftemp;
/* R <-> Y */
    ftemp = cmtp->res[17];
    cmtp->res[17] = cmtp->res[24];
    cmtp->res[24] = ftemp;
/* N ; S ; W */
    cmtp = cmtp->next;
  }

/* reverse */
  cmtp = cmtphead;
  j = window - 1;
  for (i = 0; i < window/2; i++)
  {
    cmtptail = cmtphead;
    for (k = 0; k < j; k++)
      cmtptail = cmtptail->next;
    j--;

    for (k = 0; k < FILTS; k++)
    {
      ftemp = cmtptail->res[k];
      cmtptail->res[k] = cmtp->res[k];
      cmtp->res[k] = ftemp;
    }
    k = cmtptail->min;
    cmtptail->min = cmtp->min;
    cmtp->min = k;
    cmtp = cmtp->next;
  }

  return;
}

extern Int4 ReadMatrix (CharPtr res, FloatHi scr[FILTS][FILTS],
                        CharPtr filename)
{
  FILE     *fin;
  Int2     i, n, val;
  Int4     type, numcol;
  long int ltype, lscore, lnumcol;
  Char     buf[PATH_MAX], buff[PATH_MAX];
  CharPtr  bptr;

  static Char c[] =
    {'A','B','C','D','G','H','K','M','N','R','S','T','V','W','Y'};

  for (i = 0; i < FILTS; i++)
    res[i] = '\0';
  for (i = 0; i < FILTS; i++)
    for (n = 0; n < FILTS; n++)
      scr[i][n] = 0.0;

  if (!FindPath ("ncbi", "ncbi", "data", buf, sizeof (buf)))
    return 0;

  FileBuildPath (buf, NULL, filename);
  if ((fin = FileOpen (buf, "r")) == NULL)
  {
    if ((fin = FileOpen (filename, "r")) == NULL)
    {
      return 0;
    }
  }

  val = 0;
  type = -1;
  numcol = 0;
  while ((FileGets (buff, sizeof (buff), fin)) != NULL)
  {
    if (buff[0] == ';')
      continue;
    if (type == -1)
    {
      FileGets (buff, sizeof (buff), fin);
      sscanf (buff, "%ld", &ltype);
      type = (Int4) ltype;
      if (type != 1 && type != 2)
      {
        FileClose (fin);
        return 0;
      }
      FileGets (buff, sizeof (buff), fin);
      sscanf (buff, "%ld", &lnumcol);
      numcol = (Int4) lnumcol;
      FileGets (buff, sizeof (buff), fin);
      continue;
    }
    res[val] = c[val];
    bptr = buff;
    for (i = 0; i < numcol; i++)
    {
      sscanf (bptr, "%ld", &lscore);
      scr[val][i] = (FloatHi) lscore;
      bptr += 4;
    }
    val++;
  }
  FileClose (fin);
  return numcol;
}

extern Int4 GridMatch (CharPtr seqhead, Int4 cur, Int4 end, Int4 matlen,
                       ComMatPtr cmtphead, Int4Ptr matval, Int4 maxval)
{
  CharPtr   seq;
  ComMatPtr cmtp;
  Int4      cutval, newval;

  end = end - matlen + 1;
  while (cur < end)
  {
    seq = seqhead;
    cmtp = cmtphead;
    *matval = 0;
    cutval = maxval;
    while (cmtp != NULL)
    {
      newval = cmtp->res[(((Uint1) *seq) - ((Int2) 'A'))];
      cutval = cutval - cmtp->min + newval;
      if (cutval < 0)
        break;
      *matval += newval;
      cmtp = cmtp->next;
      if (cmtp == NULL)
        return cur;
      seq++;
    }
    seqhead++;
    cur++;
  }
  return cur;
}

extern Int4 GridMatchSetUp (CharPtr seq)
{
  Int4 i;

  i = 0;
  while (*seq != '\0')
  {
    seq++;
    i++;
  }
  return i;
}

static CharPtr GetSeqChunk (SeqPortPtr spp, Int4 start, Int4 chunk, Int4 len)
{
  CharPtr seqhead, sequence;
  Int4    i, size;

  if ((start + chunk) > len)
    size = len - start;
  else
    size = chunk;

  seqhead = sequence = MemNew (sizeof (Char) * (size_t) (size+1));
  if (seqhead == NULL)
    return seqhead;

  SeqPortSeek (spp, start, SEEK_SET);
  for (i = 0; i < size; i++)
    *sequence++ = SeqPortGetResidue (spp);
  *sequence = '\0';

  return seqhead;
}

static void MatrixMatch (SeqPortPtr spp, Int4 len, ComMatPtr cmtp,
                         Int4 matlen, FloatHiPtr fptr)
{
  Int4       i, n, length, chunk = SQRDBF;
  Int4       matval, cutoff = 0;
  CharPtr    seqhead, seq;
  FloatHiPtr fptrhead, fptrtemp;

  fptrhead = fptr;
  for (i = 0; i < len; i+=(chunk-matlen))
  {
    fptr = fptrhead;
    fptr += i;
    fptrtemp = fptr;
    seqhead = seq = GetSeqChunk (spp, i, chunk, len);

    n = 0;
    length = GridMatchSetUp (seqhead);
    while (n < length)
    {
      n = GridMatch (seq, n, length, matlen, cmtp, &matval, cmtp->max-cutoff);
      if (n < length)
      {
        fptr = fptrtemp + n;
        *fptr = (FloatHi) matval;
        *fptr = *fptr / cmtp->max * 100;
        n++;
        seq = seqhead + n;
      }
    }
    MemFree (seqhead);
  }
  return;
}

/* functions - external */

extern SeqGraphPtr MatrixSeq (BioseqPtr bsp, SeqGraphPtr sgptr,
                              ComMatPtr cmtp, Int4 window)
{
  FloatHiPtr    fptr;
  Int4          i, start, end;
  Int4          gwidth = 500;
  SeqPortPtr    spp;
  SeqGraphPtr   sgp;

  if (bsp != NULL)
  {
    if (sgptr != NULL)       /* should only be called once for aa's */
    {
      InvertMatrix (cmtp, window);
    }

    if ((sgp = SeqGraphNew ()) != NULL)
    {
/* type and number of values and compression */
      sgp->numval = bsp->length;
      sgp->compr = (Int4) (bsp->length / gwidth);
      if ((bsp->length%gwidth) != 0)
        sgp->compr += 1;
/* graph type */
      sgp->flags[2] = 1;
      sgp->values = (Pointer) MemNew ((sizeof (FloatHi)) * sgp->numval);
/* min/max */
      sgp->max.realvalue = 100.0;
      sgp->min.realvalue = 0.0;
      sgp->axis.realvalue = 0.0;
/* scaling */
      sgp->flags[1] = 1;
      sgp->a = 2;
      sgp->b = 0;
    }
    else
    {
      return sgptr;
    }
/* do it */
    fptr = (FloatHiPtr) sgp->values;
    for (i = 0; i < sgp->numval; i++)
    {
      *fptr = 0.0;
      fptr++;
    }
    fptr = (FloatHiPtr) sgp->values;
    start = 0;
    end = bsp->length - 1;

    if (ISA_na (bsp->mol))
    {
      spp = SeqPortNew (bsp, start, end, 0, Seq_code_iupacna);
    }
    else
    {
      spp = SeqPortNew (bsp, start, end, 0, Seq_code_iupacaa);
    }

    MatrixMatch (spp, sgp->numval, cmtp, window, fptr);

    SeqPortFree (spp);
    if (sgptr != NULL)
      sgptr->next = sgp;
    else
      sgptr = sgp;
  }
  return sgptr;
}

static SeqGraphPtr MatrixSearch (BioseqPtr bsp, Int2 matrixtype)
{
  Int4         i, n;
  Int4         window;
  Int2         moltype;
  Char         res[FILTS];
  FloatHi      scr[FILTS][FILTS];
  FloatHi      maxvalue, maxtemp;
  SeqGraphPtr  sgp;
  ComMatPtr    cmtp;

  switch (matrixtype)
  {
   case NA_MATRIX_DONOR1:
    window = ReadMatrix (res, scr, "KSdonor1.mat");
    break;
   case NA_MATRIX_DONOR2:
    window = ReadMatrix (res, scr, "KSdonor2.mat");
    break;
   case NA_MATRIX_ACCEPT:
    window = ReadMatrix (res, scr, "KSaccept.mat");
    break;
   case NA_MATRIX_BRANCH:
    window = ReadMatrix (res, scr, "KSbranch.mat");
    break;
   case NA_MATRIX_HR:
    window = ReadMatrix (res, scr, "KShr30bi.mat");
    break;
   default:
    return NULL;
  }
  if (window == 0)
    return NULL;

  maxvalue = 0.0;
  for (i = 0; i < window; i++)
  {
    maxtemp = 0.0;
    for (n = 0; n < FILTS; n++)
    {
      if (maxtemp < scr[n][i])
        maxtemp = scr[n][i];
    }
    maxvalue += maxtemp;
  }

  if (ISA_na (bsp->mol))
    moltype = 0;
  else
    moltype = 1;

  cmtp = CompileMatrix (scr, window, (Int4) maxvalue);

  sgp = NULL;
  sgp = MatrixSeq (bsp, sgp, cmtp, window);
  if (sgp == NULL)
  {
    ComMatFree (cmtp);
    return sgp;
  }
  if (moltype == 0)
  {
    sgp = MatrixSeq (bsp, sgp, cmtp, window);
    if (sgp->next == NULL)
      sgp = SeqGraphFree (sgp);
  }
  ComMatFree (cmtp);
  return sgp;
}

Int2 LIBCALLBACK SeqMatFunc (Pointer data)
{
  OMProcControlPtr      ompcp;
  BioseqPtr             bsp = NULL;
  SeqFeatPtr            sfp = NULL;
  SeqIdPtr              psip;
  WindoW                w;
  GraphViewFormPtr      gvp;
  SeqGraphPtr           sgp;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->input_itemtype == 0)
    return OM_MSG_RET_ERROR;

  switch (ompcp->input_itemtype)
  {
   case OBJ_BIOSEQ:
    bsp = (BioseqPtr) ompcp->input_data;
    break;
   case OBJ_SEQFEAT:
    sfp = (SeqFeatPtr) ompcp->input_data;
    break;
   default:
    return OM_MSG_RET_ERROR;
  }

  if (sfp != NULL)
  {
    if (sfp->data.choice != SEQFEAT_CDREGION)
      return OM_MSG_RET_ERROR;
    if ((psip = SeqLocId (sfp->product)) == NULL)
      return OM_MSG_RET_ERROR;
    if ((bsp = BioseqFind (psip)) == NULL)
      return OM_MSG_RET_ERROR;
  }

  if (bsp != NULL)
  {
    if (!ISA_na (bsp->mol))
      return OM_MSG_RET_ERROR;
    if ((w = (WindoW) CreateGraphViewForm (-50, -33, "Baculo enhancer", bsp,
                                           GRAPH_MATRIX_NA)) == NULL)
    {
      return OM_MSG_RET_ERROR;
    }
    if ((gvp = (GraphViewFormPtr) GetObjectExtra (w)) == NULL)
    {
      return OM_MSG_RET_ERROR;
    }
    else
    {
      gvp->graphtype = GRAPH_MATRIX_NA;
      gvp->entityID = ompcp->input_entityID;
      gvp->itemID = ompcp->input_itemID;
      sgp = MatrixSearch (bsp, NA_MATRIX_HR);
      if (sgp == NULL)
        return OM_MSG_RET_ERROR;
      gvp->sgp = sgp;
      BioseqPtrToGraphViewForm (gvp->form, sgp);
      BioseqPtrToGraphViewForm (gvp->form, sgp->next);
    }
  }
  else
  {
    return OM_MSG_RET_ERROR;
  }

  Show (w);
  Select (w);
  return OM_MSG_RET_DONE;
}
