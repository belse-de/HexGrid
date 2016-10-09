#!/bin/bash
clear
set -e

echo -e "---- ---- ---- ----"
echo -e "       start       "
echo -e "---- ---- ---- ----"
echo -e "\n"

echo -e "     build test    "
echo -e "---- ---- ---- ----"
g++ -std=c++11 -O0 -ggdb3 -Wall -Wextra -fsanitize=address libhex.cpp libhex_test.cpp -o libhex_test

echo -e "       test        "
echo -e "---- ---- ---- ----"
./libhex_test

echo -e "       build       "
echo -e "---- ---- ---- ----"
g++ -std=c++11 -O0 -ggdb3 -Wall -Wextra -fsanitize=address -c libhex.cpp -o libhex.o 
g++ -std=c++11 -O0 -ggdb3 -Wall -Wextra -fsanitize=address -c main.cpp -o main.o
g++ -std=c++11 -O0 -ggdb3 -Wall -Wextra -fsanitize=address main.o libhex.o -o main -lGL -lGLU -lGLEW -lglut

echo -e "        run        "
echo -e "---- ---- ---- ----"

LSAN_OPTIONS=suppressions=suppress.txt ./main

echo -e "---- ---- ---- ----"
echo -e "        end        "
echo -e "---- ---- ---- ----"

