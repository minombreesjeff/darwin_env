#!/usr/bin/env bash
mv perl perl-5.8.6;tar cf - perl-5.8.6|bzip2 -f -9>perl-5.8.6.tar.bz2;rm -rf perl-5.8.6
cd Extras/Mac-Glue/;mv src Mac-Glue-1.23;tar cf - Mac-Glue-1.23|gzip -f -9>Mac-Glue-1.23.tar.gz;rm -rf Mac-Glue-1.23;cd ../../
cd Extras/Mac-OSA-Simple/;mv src Mac-OSA-Simple-1.08;tar cf - Mac-OSA-Simple-1.08|gzip -f -9>Mac-OSA-Simple-1.08.tar.gz;rm -rf Mac-OSA-Simple-1.08;cd ../../
cd Extras/MLDBM/;mv src MLDBM-2.01;tar cf - MLDBM-2.01|gzip -f -9>MLDBM-2.01.tar.gz;rm -rf MLDBM-2.01;cd ../../
cd Extras/Mac-Errors/;mv src Mac-Errors-1.00;tar cf - Mac-Errors-1.00|gzip -f -9>Mac-Errors-1.00.tar.gz;rm -rf Mac-Errors-1.00;cd ../../
cd Extras/Time-Epoch/;mv src Time-Epoch-0.02;tar cf - Time-Epoch-0.02|gzip -f -9>Time-Epoch-0.02.tar.gz;rm -rf Time-Epoch-0.02;cd ../../
cd Extras/Mac-Apps-Launch/;mv src Mac-Apps-Launch-1.91;tar cf - Mac-Apps-Launch-1.91|gzip -f -9>Mac-Apps-Launch-1.91.tar.gz;rm -rf Mac-Apps-Launch-1.91;cd ../../
cd Extras/Mac-AppleEvents-Simple/;mv src Mac-AppleEvents-Simple-1.16;tar cf - Mac-AppleEvents-Simple-1.16|gzip -f -9>Mac-AppleEvents-Simple-1.16.tar.gz;rm -rf Mac-AppleEvents-Simple-1.16;cd ../../
cd Extras/Mac-Carbon/;mv src Mac-Carbon-0.71;tar cf - Mac-Carbon-0.71|gzip -f -9>Mac-Carbon-0.71.tar.gz;rm -rf Mac-Carbon-0.71;cd ../../
cd Extras/URI/;mv src URI-1.35;tar cf - URI-1.35|gzip -f -9>URI-1.35.tar.gz;rm -rf URI-1.35.tar.gz;cd ../../
