#include "tour.h"

tour::tour(list<int> seq, double cost)
{
    this->tour_seq = seq;
    this->cost = cost;
}

list<int> tour::get_seq()
{
    return tour_seq;
}

double tour::get_cost()
{
    return cost;
}
