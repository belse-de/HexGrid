#!/bin/bash
clear

echo -e "---- ---- ---- ----"
echo -e "       start       "
echo -e "---- ---- ---- ----"
echo -e "\n"

g++ libhex.cpp libhex_test.cpp -o libhex_test 2> g++.err
cat g++.err
cat g++.err | wc -l
#2>&1 | tee | wc -l

./libhex_test

echo -e "\n"
echo -e "---- ---- ---- ----"
echo -e "        end        "
echo -e "---- ---- ---- ----"

