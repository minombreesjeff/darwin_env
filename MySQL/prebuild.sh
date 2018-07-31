#!/usr/bin/env bash
mv mysql mysql-4.0.14;tar cf - mysql-4.0.14|gzip -f -9>mysql-4.0.14.tar.gz;rm -rf mysql-4.0.14
