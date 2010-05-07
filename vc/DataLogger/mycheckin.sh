#!/bin/bash
set -e

if [[ $# != 1 ]]; 
then
    echo "Usage: ci comment"
    exit -1
fi
output=`svn st|grep -v ^?`
if [[ $output != "" ]]
then
	output=`svn up`
	echo $output
	version=${output##*"At revision "}
	version=${version%%"."}
	version=$((version+1))
	re_str="s/svnr\:[0-9]\{1,\}/svnr\:`echo ${version}`/g"
	sed -e $re_str DataLogger.rc > DataLogger.rc.back
	mv DataLogger.rc.back DataLogger.rc
	svn ci -m "$1"
else
	echo "Nothing to check in!!!"
fi