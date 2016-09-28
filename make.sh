#!/bin/bash
set -e

echo -e "---- ---- ---- ----"
echo -e "       start       "
echo -e "---- ---- ---- ----"
echo -e "\n"

g++ libhex.cpp libhex_test.cpp -o libhex_test

./libhex_test

echo -e "\n"
echo -e "---- ---- ---- ----"
echo -e "        end        "
echo -e "---- ---- ---- ----"

