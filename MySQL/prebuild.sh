#!/usr/bin/env bash
mv mysql mysql-4.1.10a;tar cf - mysql-4.1.10a|gzip -f -9>mysql-4.1.10a.tar.gz;rm -rf mysql-4.1.10a
