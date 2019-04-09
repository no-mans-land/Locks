#!/bin/bash
g++ -std=c++11 -pthread TTAS.cpp
sleep 3s
./a.out input.txt > output10TT.txt
echo "first completed"
sleep 1s
./a.out input15.txt > output15TT.txt
echo "second completed"
sleep 1s
./a.out input20.txt > output20TT.txt
echo "third completed"
sleep 1s
./a.out input25.txt > output25TT.txt
echo "fourth completed"
sleep 1s
./a.out input30.txt > output30TT.txt
echo "fifth completed"
sleep 1s
./a.out input35.txt > output35TT.txt
echo "sixth completed"
sleep 1s
./a.out input40.txt > output40TT.txt
echo "seventh completed"
