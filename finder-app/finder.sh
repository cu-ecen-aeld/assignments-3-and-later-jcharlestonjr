#!/bin/sh

filesdir=$1
searchstr=$2

if [ -z "$filesdir"  ];
then
	echo "Need input Directory"
	exit 1
fi

if [ -z "$searchstr"  ];
then
	echo "Need input string"
	exit 1
fi

if [ ! -d "$filesdir" ];
then
	echo "Input existing directory"
	exit 1
fi

cd $filesdir
x=$(ls -1 | wc -l)
y=$(grep -r $searchstr | wc -l)
echo "The number of files are "${x}" and the number of matching lines are "${y}""
