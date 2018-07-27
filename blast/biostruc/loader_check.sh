#! /bin/sh
MMDB=$1
FNZ=$2
SUB=$3
if [ x$SUB = x ]; then
  [ -d mmdb ] && SUB=mmdb
fi

TMPF=/tmp/$$
FN=`basename $FNZ .gz`
echo "comparision ... \c"
if [ -f ${MMDB}${FNZ} ] ; then
  gzip -dc ${MMDB}${FNZ} >$TMPF
elif [ -f ${MMDB}${FN} ] ; then
  cat ${MMDB}${FN} >$TMPF
else
  echo "FAILED (${MMDB}${FN}* has not been found)  \c"
  exit
fi
if cmp $SUB/${FN} $TMPF >$$ 2>/dev/null
then
  rm -f $SUB/${FN} $TMPF
  echo "ok \c"
else
  mv $TMPF $SUB/${FN}.orig
  cat $$
  cat $$ >>cmp_log
  echo "FAILED   \c"
fi
rm -f $$
