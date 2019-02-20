#!/usr/bin/env bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

function ctrl_c() {
        exit
}
trap ctrl_c INT

if [ $2 ]
    then
        dirTest=$2
        dirTest="$dirTest/*.ut"
    else
        dirTest=tests
        dirTest="$dirTest\/*/*.ut"
fi

echo $dirTest
rm -rf logTests
mkdir logTests

for filename in $dirTest
    do
        line=0
        read -t 1 line

        if [ $line = "kill" ]
            then
                exit 1
        fi

        gtimeout 2000 sh test.sh tests/$(basename "$filename" .ut) $(basename "$filename" .ut) "logTests/$(basename "$filename" .ut).output" ${1} &> "logTests/sh_$(basename "$filename" .ut).output"

        result=$?

        if [ $result = 124 ]
            then
                echo "${RED}$filename TIMEOUT ${NC}"
        elif [ $result = 0 ]
            then
                echo "${GREEN}$filename SUCCESS ${NC}"
        elif [ $result = 2 ]
            then
                echo "${RED}$filename DIFF ${NC}"
        else
            echo "${RED}$filename FAILURE ${NC}"
        fi
done