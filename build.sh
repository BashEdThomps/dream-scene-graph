#!/bin/bash

HOST=`uname -a`
BUILD=build

echo HOST is $HOST

if [[ "$HOST" == *"Darwin"* ]];
then
	echo Building for MacOS X
	if [[ "$HOST" == *"PPC"* ]];
	then
		echo Building for PowerPC
		export CC=/opt/local/bin/gcc-mp-4.9
		export CXX=/opt/local/bin/g++-mp-4.9
	else
		echo Building for Intel
	fi
fi

rm -rf $BUILD
mkdir -p $BUILD
cd $BUILD
cmake ..
make
CTEST_OUTPUT_ON_FAILURE=TRUE make test
