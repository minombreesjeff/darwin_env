#!/usr/bin/env bash
mv perl perl-5.8.4;tar cf - perl-5.8.4|bzip2 -f -9>perl-5.8.4.tar.bz2;rm -rf perl-5.8.4
