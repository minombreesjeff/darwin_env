#!/usr/bin/env bash
mv mysql mysql-3.23.53;tar cf - mysql-3.23.53|gzip -f -9>mysql-3.23.53.tar.gz;rm -rf mysql-3.23.53
