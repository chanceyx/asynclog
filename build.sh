#!/bin/bash
set -e

if [ ! -d `pwd`/build ]; then
    mkdir `pwd`/build
fi

rm -fr `pwd`/build/*
cd `pwd`/build &&
    cmake .. &&
    make

cd ..

if [ ! -d /usr/include/asynclog ]; then
    mkdir /usr/include/asynclog
fi

for header in `ls ./src/*.h`
do
    cp $header /usr/include/asynclog
done

if [ ! -d /usr/include/asynclog/util ]; then
    mkdir /usr/include/asynclog/util
fi

for header in `ls ./src/util/*.h`
do 
    cp $header /usr/include/asynclog/util
done

cp `pwd`/lib/libasynclog.so /usr/lib
