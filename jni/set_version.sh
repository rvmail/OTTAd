#!/bin/bash 
# Author : li.he@icntv.tv

VERSION_FILE=$PWD/jni/common.h

GIT_VERSION=`git --no-pager log --abbrev=7 -n 1 --pretty=format:"%h %ci" HEAD | awk \
'{gsub("-", "");print $2"-"$1}'`

PRE_VERSION=`grep "#define GIT_VERSION.*" jni/common.h | awk '{gsub("\x22","");print $3}'`

if [ $GIT_VERSION != $PRE_VERSION ]
then
    sed -i "s/#define\ GIT_VERSION.*/#define\ GIT_VERSION\ \"$GIT_VERSION\"/" $VERSION_FILE
fi
