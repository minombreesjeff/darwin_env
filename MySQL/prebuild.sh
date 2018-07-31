#!/usr/bin/env bash
mv mysql mysql-5.0.82;tar cf - mysql-5.0.82|gzip -f -9>mysql-5.0.82.tar.gz;rm -rf mysql-5.0.82
