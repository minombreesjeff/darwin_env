#!/usr/bin/env bash
mv mysql mysql-5.0.45;tar cf - mysql-5.0.45|gzip -f -9>mysql-5.0.45.tar.gz;rm -rf mysql-5.0.45
