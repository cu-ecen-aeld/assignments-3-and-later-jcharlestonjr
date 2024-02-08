#!/bin/bash

writefile=$1
writestr=$2

if [ -z "$writefile" ];
then
	echo "Specify directory to create"
	exit 1
fi
if [ -z "$writestr" ];
then
	echo "Specify word to write"
	exit 1
fi


mkdir -p $writefile
cd $writefile
touch sample.txt
echo $writestr >  sample.txt
