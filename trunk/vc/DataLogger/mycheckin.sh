#!/bin/bash
set -e

if [[ $# != 1 ]]; 
then
    echo "Usage: ci comment"
    exit -1
fi
output=`svn up`
echo $output
version=${output##*"At revision "}
version=${version%%"."}
echo $version
re_str="s/svnr\:[0-9]\{1,\}/svnr\:`echo ${version}`/g"
echo $re_str
sed -e $re_str DataLogger.rc > DataLogger.rc
svn ci -m "$1"

