#!/usr/bin/env bash
mv mysql mysql-5.0.67;tar cf - mysql-5.0.67|gzip -f -9>mysql-5.0.67.tar.gz;rm -rf mysql-5.0.67
