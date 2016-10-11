#!/bin/bash
set -e

echo -e "     build test    "
echo -e "---- ---- ---- ----"
g++ -std=c++11 -O0 -ggdb3 -Wall -Wextra -fsanitize=address libhex.cpp libhex_test.cpp -o libhex_test

echo -e "       test        "
echo -e "---- ---- ---- ----"
./libhex_test


