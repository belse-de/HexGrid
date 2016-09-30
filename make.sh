#!/bin/bash
clear

echo -e "---- ---- ---- ----"
echo -e "       start       "
echo -e "---- ---- ---- ----"
echo -e "\n"

g++ -Wall -Wextra libhex.cpp libhex_test.cpp -o libhex_test

./libhex_test

echo -e "\n"
echo -e "---- ---- ---- ----"
echo -e "        end        "
echo -e "---- ---- ---- ----"

