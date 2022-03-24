#!/bin/bash

set -e

# 如果没有build目录 创建该目录
if [ ! -d `pwd`/build ]; then
    mkdir `pwd`/build
fi

rm -fr `pwd`/build/*
cd `pwd`/build &&
    cmake .. &&
    make

# 回到项目根目录
cd ..

# 把头文件拷贝到 /usr/include/hummingbird       .so库拷贝到 /usr/lib
if [ ! -d /usr/include/asynclog ]; then
    mkdir /usr/include/asynclog
fi

for header in `ls ./src/*.h`
do
    cp $header /usr/include/asynclog
done

cp `pwd`/lib/libasynclog.so /usr/lib
