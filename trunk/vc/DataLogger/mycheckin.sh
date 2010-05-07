#!/bin/bash
set -e

if [[ $# != 1 ]]; 
then
    echo "Usage: ci comment"
    exit -1
fi
output=`svn st|grep -v ^?`
echo $output
if [[ $output != "" ]];
then
	output=`svn up`
	echo $output
	version=${output##*"At revision "}
	version=${version%%"."}
	echo $version
	version=$((version+1))
	echo $version
	re_str="s/svnr\:[0-9]\{1,\}/svnr\:`echo ${version}`/g"
	echo $re_str
	sed -e $re_str DataLogger.rc > DataLogger.rc.back
	mv DataLogger.rc.back DataLogger.rc
	echo "$1"
	svn ci -m "$1"
fi