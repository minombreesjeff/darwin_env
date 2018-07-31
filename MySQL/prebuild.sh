#!/usr/bin/env bash
mv mysql mysql-5.0.88;tar cf - mysql-5.0.88|gzip -f -9>mysql-5.0.88a.tar.gz;rm -rf mysql-5.0.88
