#!/bin/bash
clear

echo -e "---- ---- ---- ----"
echo -e "       start       "
echo -e "---- ---- ---- ----"
echo -e "\n"

rm -f g++.err

g++ -std=c++11 -Wall -Wextra libhex.cpp libhex_test.cpp -o libhex_test 2> g++.err

head  g++.err
wc -l g++.err

./libhex_test

echo -e "\n"
echo -e "---- ---- ---- ----"
echo -e "        end        "
echo -e "---- ---- ---- ----"

