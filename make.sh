#!/bin/bash
clear
set -e

echo -e "       build       "
echo -e "---- ---- ---- ----"
cd build
ninja
cd ..

echo -e "        run        "
echo -e "---- ---- ---- ----"

LSAN_OPTIONS=suppressions=suppress.txt ./build/main
echo -e "\n  Exit Code: " $?

