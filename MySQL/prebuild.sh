#!/usr/bin/env bash
mv mysql mysql-5.0.91;tar cf - mysql-5.0.91|gzip -f -9>mysql-5.0.91-apple.tar.gz;rm -rf mysql-5.0.91
