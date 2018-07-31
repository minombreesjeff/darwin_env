#!/usr/bin/env bash
mv mysql mysql-4.1.13a;tar cf - mysql-4.1.13a|gzip -f -9>mysql-4.1.13a.tar.gz;rm -rf mysql-4.1.13a
