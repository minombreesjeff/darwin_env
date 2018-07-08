#!/bin/bash

if [[ $# != 1 ]] ; then
    echo "You need to specify the version." >&2
    exit 1
fi

VER=$1

git checkout -f upstream || exit 1

git status

echo "Make sure there are no files listed that should not be committed."
echo -n "Continue? (Y/N) "

read CONTINUE

case $CONTINUE in
    Y*|y*) ;;
    *) exit 1 ;;
esac

cd src

curl -LO http://www.kernel.org/pub/software/scm/git/git-$VER.tar.bz2
curl -LO http://www.kernel.org/pub/software/scm/git/git-htmldocs-$VER.tar.bz2
curl -LO http://www.kernel.org/pub/software/scm/git/git-manpages-$VER.tar.bz2

rm -rf git git-htmldocs git-manpages

mkdir git-htmldocs
cd git-htmldocs
tar xjvf ../git-htmldocs-$VER.tar.bz2
cd ..

mkdir git-manpages
cd git-manpages
tar xjvf ../git-manpages-$VER.tar.bz2
cd ..

tar xjvf git-$VER.tar.bz2
mv git-$VER git

openssl sha1 *$VER.tar.bz2

rm *$VER.tar.bz2

cd ..
git add -A
