#!/bin/bash

mkdir $1
autoreconf -fi
sh make_precompiled_header.sh
./configure --prefix=$1
sh extract_svn_revision.sh
make 
make install
mkdir $1/etc $1/bin/DBC $1/bin/maps $1/bin/vmaps $1/bin/logs $1/bin/scripts
cp src/*.conf $1/etc
