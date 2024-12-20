#ifndef TOUR_H
#define TOUR_H

#include <iostream>
#include <list>

using namespace std;

class tour
{
private:
    list<int> tour_seq;
    double cost;

public:
    tour(list<int>, double);
    list<int> get_seq();
    double get_cost();
};

#endif