rm a.out main.o point.o tsp.o tour.o
g++ -c main.cpp point.cpp tsp.cpp tour.cpp
g++ main.o point.o tsp.o tour.o
