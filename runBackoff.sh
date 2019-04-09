#!/bin/bash
g++ -std=c++11 -pthread Backoff.cpp
sleep 3s
./a.out input.txt > output10B.txt
echo "first completed"
sleep 1s
./a.out input15.txt > output15B.txt
echo "second completed"
sleep 1s
./a.out input20.txt > output20B.txt
echo "third completed"
sleep 1s
./a.out input25.txt > output25B.txt
echo "fourth completed"
sleep 1s
./a.out input30.txt > output30B.txt
echo "fifth completed"
sleep 1s
./a.out input35.txt > output35B.txt
echo "sixth completed"
sleep 1s
./a.out input40.txt > output40B.txt
echo "seventh completed"
