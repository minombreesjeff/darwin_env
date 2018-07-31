# Copyright (C) 2000 MySQL AB & MySQL Finland AB & TCX DataKonsult AB
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the Free
# Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
# MA 02111-1307, USA

## Process this file with automake to create Makefile.in

# This is the "real" file, it is copied at release build to overwrite 
# the one stored in the source tree hierarchy.
# It is (currently ?) not used in producing the documentation,
# but it must be maintained to always contain the list of files.
# Its sole purpose is to provide the list of image files which must
# be included in a source code distribution tar-ball.

pictures = myaccess-odbc.eps myaccess-odbc.gif myaccess-odbc.pdf myaccess-odbc.png myaccess-odbc.txt \
	myaccess.eps myaccess.jpg myaccess.pdf myaccess.png myaccess.txt \
	myarchitecture.eps myarchitecture.gif myarchitecture.graffle myarchitecture.pdf myarchitecture.png myarchitecture.txt \
	mydll-properties.eps mydll-properties.jpg mydll-properties.pdf mydll-properties.png mydll-properties.txt \
	mydsn-example.eps mydsn-example.jpg mydsn-example.pdf mydsn-example.png mydsn-example.txt \
	mydsn-icon.eps mydsn-icon.gif mydsn-icon.pdf mydsn-icon.png mydsn-icon.txt \
	mydsn-options.eps mydsn-options.jpg mydsn-options.pdf mydsn-options.png mydsn-options.txt \
	mydsn-setup.eps mydsn-setup.jpg mydsn-setup.pdf mydsn-setup.png mydsn-setup.txt \
	mydsn-test-fail.eps mydsn-test-fail.jpg mydsn-test-fail.pdf mydsn-test-fail.png mydsn-test-fail.txt \
	mydsn-test-success.eps mydsn-test-success.jpg mydsn-test-success.pdf mydsn-test-success.png mydsn-test-success.txt \
	mydsn-trace.eps mydsn-trace.jpg mydsn-trace.pdf mydsn-trace.png mydsn-trace.txt \
	mydsn.eps mydsn.jpg mydsn.pdf mydsn.png mydsn.txt \
	myflowchart.eps myflowchart.gif myflowchart.graffle myflowchart.pdf myflowchart.png myflowchart.txt

EXTRA_DIST = $(pictures)

all:

clean:

# Don't update the files from bitkeeper
%::SCCS/s.%
