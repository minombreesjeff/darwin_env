#!/usr/bin/env bash
mv mysql mysql-4.1.22;tar cf - mysql-4.1.22|gzip -f -9>mysql-4.1.22.tar.gz;rm -rf mysql-4.1.22
