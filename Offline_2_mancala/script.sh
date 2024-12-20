#!/bin/bash

rm mancala
g++ main.cpp mancala.cpp alpha_beta.cpp -o mancala
echo "compiled program successfully"
./mancala