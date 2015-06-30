#!/bin/bash 

files=`find . -name "*.h"`

for file in $files;
do
   rm $file
done
