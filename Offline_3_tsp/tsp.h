#ifndef TSP_H
#define TSP_H

#include "point.h"
#include "tour.h"

class tsp
{
private:
    int dimension;
    point *points;
    double **distances;
    int random_number;
    void calc_distances();
    void reverse_section(int *, int, int);
    double gain_from_three_opt(int *tour_arr, int x1, int x2, int y1, int y2, int z1, int z2, int opt_case);
    void three_opt_move(int *tour_arr, int i, int j, int k, int opt_case);
    double gain_from_node_swap(int x0_pred, int x0, int x0_succ, int y0_pred, int y0, int y0_succ);

public:
    tsp(int dimension, point *points);
    tour nearest_neighbour();
    tour nearest_insertion();
    tour farthest_insertion();
    tour cheapest_insertion();
    tour two_opt(tour t);
    tour three_opt(tour t);
    tour node_shift(tour t);
    tour node_swap(tour t);
};

#endif