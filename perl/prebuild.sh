#!/usr/bin/env bash
mv perl perl-5.8.8;tar cf - perl-5.8.8|bzip2 -f -9>perl-5.8.8.tar.bz2;rm -rf perl-5.8.8
cd updates/Pod-Simple/;mv src Pod-Simple-3.05;tar cf - Pod-Simple-3.05|gzip -f -9>Pod-Simple-3.05.tar.gz;rm -rf Pod-Simple-3.05;cd ../../
cd updates/podlators/;mv src podlators-2.0.5;tar cf - podlators-2.0.5|gzip -f -9>podlators-2.0.5.tar.gz;rm -rf podlators-2.0.5;cd ../../
cd updates/Pod-Escapes/;mv src Pod-Escapes-1.04;tar cf - Pod-Escapes-1.04|gzip -f -9>Pod-Escapes-1.04.tar.gz;rm -rf Pod-Escapes-1.04;cd ../../
