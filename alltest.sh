#!/usr/bin/env bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

function ctrl_c() {
        exit
}
trap ctrl_c INT

dirTest=$1
dirTest="$dirTest\/*"
rm -rf LogTests
mkdir LogTests

for filename in $dirTest
    do
        line=0
        read -t 1 line

        if [ $line = "kill" ]
            then
                exit 1
        fi

        gtimeout 20 sh ../test.sh $filename "LogTests/$(basename "$filename" .txt)_Log.txt" &> "LogTests/sh_$(basename "$filename" .txt)_Log.txt"

        result=$?

        if [ $result = 124 ]
            then
                echo "${RED}$filename TIMEOUT ${NC}"
        elif [ $result = 0 ]
            then
                echo "${GREEN}$filename SUCCESS ${NC}"
        else
            echo "${RED}$filename FAILURE ${NC}"
        fi
done