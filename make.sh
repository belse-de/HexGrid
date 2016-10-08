#!/bin/bash
clear
set -e

echo -e "---- ---- ---- ----"
echo -e "       start       "
echo -e "---- ---- ---- ----"
echo -e "\n"

rm -f g++.err

g++ -std=c++11 -ggdb3 -Wall -Wextra -fsanitize=address libhex.cpp libhex_test.cpp -o libhex_test

./libhex_test

g++ -std=c++11 -ggdb3 -Wall -Wextra -fsanitize=address -c libhex.cpp -o libhex.o 
g++ -std=c++11 -ggdb3 -Wall -Wextra -fsanitize=address -c main.cpp -o main.o

g++ -std=c++11 -ggdb3 -Wall -Wextra -fsanitize=address main.o libhex.o -o main -lGL -lGLU -lGLEW -lglut 

./main

echo -e "\n"
echo -e "---- ---- ---- ----"
echo -e "        end        "
echo -e "---- ---- ---- ----"

