#!/bin/bash
g++ -c node.cpp board.cpp main.cpp
g++ node.o board.o main.o -o a_star
