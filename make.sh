#!/bin/bash
clear
set -e

echo -e "       build       "
echo -e "---- ---- ---- ----"
cd build
ninja
cd ..

echo -e "\n        run        "
echo -e "---- ---- ---- ----"

LSAN_OPTIONS=suppressions=suppress.txt ./build/Hexagon
echo -e "\n---- ---- ---- ----"
echo -e "   Exit Code: " $?

