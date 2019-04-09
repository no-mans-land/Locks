#!/bin/bash
g++ -std=c++11 -pthread TAS.cpp
sleep 3s
./a.out input.txt > output.txt
echo "first completed"
sleep 1s
./a.out input15.txt > output15.txt
sleep 1s
./a.out input20.txt > output20.txt
sleep 1s
./a.out input25.txt > output25.txt
sleep 1s
./a.out input30.txt > output30.txt
sleep 1s
./a.out input35.txt > output35.txt
sleep 1s
./a.out input40.txt > output40.txt
