#include "tsp.h"
#include <iostream>
#include <float.h>
#include <list>
#include <random>
#include <algorithm>

#include "tour.h"

using namespace std;

int get_random_number()
{
    // Create a random number generator
    std::random_device rd;  // Seed generator
    std::mt19937 gen(rd()); // Mersenne Twister RNG seeded with random_device

    // Define the range for the random numbers
    std::uniform_int_distribution<> dist(1, 100); // Range [1, 100]

    // Generate a random integer
    int random_number = dist(gen);

    // std::cout << "Random Number: " << random_number << std::endl;

    return random_number;
}

void tsp::calc_distances()
{
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            distances[i][j] = points[i].get_distance(points[j]);
        }
    }
}

tsp::tsp(int dim, point *points)
{
    this->dimension = dim;
    this->points = points;
    this->random_number = get_random_number() % dimension;

    distances = (double **)malloc(dim * sizeof(double *));
    for (int i = 0; i < dim; i++)
    {
        distances[i] = (double *)malloc(dim * sizeof(double));
    }
    calc_distances();
}

tour tsp::nearest_neighbour()
{
    int source = random_number;
    list<int> tour_seq;

    bool is_included[dimension];
    for (int i = 0; i < dimension; i++)
    {
        is_included[i] = false;
    }

    int last_visited = source;
    tour_seq.push_back(source);
    is_included[source] = true;
    double cost = 0.0;
    double nearest_distance;
    int nearest_point;
    for (int i = 0; i < dimension - 1; i++) // since one point is already added
    {
        nearest_distance = DBL_MAX;
        nearest_point = -1;
        for (int j = 0; j < dimension; j++)
        {
            // std::cout << "checking distance " << last_visited << ", " << j << "\n";
            if ((!is_included[j]) && (j != last_visited) && (distances[last_visited][j] < nearest_distance))
            {
                nearest_distance = distances[last_visited][j];
                nearest_point = j;
            }
        }
        is_included[nearest_point] = true;
        last_visited = nearest_point;
        tour_seq.push_back(last_visited);
        cost += nearest_distance;
    }
    tour_seq.push_back(source);
    cost += distances[source][last_visited];

    return tour(tour_seq, cost);
}

tour tsp::nearest_insertion()
{
    int source = random_number;
    list<int> tour_seq;
    tour_seq.push_front(source);

    bool is_included[dimension];
    for (int i = 0; i < dimension; i++)
    {
        is_included[i] = false;
    }
    is_included[source] = true;

    double nearest_distance;
    int nearest_point;
    // find nearest point
    while (tour_seq.size() < dimension)
    {
        nearest_distance = DBL_MAX;
        nearest_point = -1;
        for (auto it = tour_seq.begin(); it != tour_seq.end(); ++it)
        {
            for (int j = 0; j < dimension; j++)
            {
                if ((!is_included[j]) && (distances[*it][j] > 0.0) && (distances[*it][j] < nearest_distance))
                {
                    nearest_distance = distances[*it][j];
                    nearest_point = j;
                }
            }
        }

        if (tour_seq.size() == 1)
        {
            tour_seq.push_back(nearest_point);
            is_included[nearest_point] = true;
            tour_seq.push_back(source);

            continue;
        }

        // find edge to insert
        nearest_distance = DBL_MAX;
        auto i = tour_seq.begin();
        auto it1 = tour_seq.begin();
        for (int j = 0; j < tour_seq.size() - 1; j++)
        {
            if (distances[*it1][nearest_point] + distances[nearest_point][*(next(it1))] - distances[*it1][*(next(it1))] < nearest_distance)
            {
                nearest_distance = distances[*it1][nearest_point] + distances[nearest_point][*(next(it1))] - distances[*it1][*(next(it1))];
                i = it1;
            }
            it1++;
        }

        // insert
        advance(i, 1);
        tour_seq.insert(i, nearest_point);
        is_included[nearest_point] = true;
    }
    // calculate cost
    double cost = 0.0;
    for (auto it = tour_seq.begin(); it != tour_seq.end(); it++)
    {
        // cout << "calculating cost between " << *it << ", " << *(next(it)) << ": " << distances[*it][*(next(it))] << "\n";
        cost += distances[*it][*(next(it))];
    }
    return tour(tour_seq, cost);
}

tour tsp::farthest_insertion()
{
    int source = random_number;
    list<int> tour_seq;
    tour_seq.push_front(source);

    bool is_included[dimension];
    for (int i = 0; i < dimension; i++)
    {
        is_included[i] = false;
    }
    is_included[source] = true;

    double farthest_distance;
    int farthest_point;
    // find nearest point
    while (tour_seq.size() < dimension)
    {
        farthest_distance = DBL_MIN;
        farthest_point = -1;
        for (auto it = tour_seq.begin(); it != tour_seq.end(); ++it)
        {
            for (int j = 0; j < dimension; j++)
            {
                if ((!is_included[j]) && (distances[*it][j] > 0.0) && (distances[*it][j] > farthest_distance))
                {
                    farthest_distance = distances[*it][j];
                    farthest_point = j;
                }
            }
        }

        if (tour_seq.size() == 1)
        {
            tour_seq.push_back(farthest_point);
            is_included[farthest_point] = true;
            tour_seq.push_back(source);

            continue;
        }

        // find edge to insert
        farthest_distance = DBL_MIN;
        auto i = tour_seq.begin();
        auto it1 = tour_seq.begin();
        for (int j = 0; j < tour_seq.size() - 1; j++)
        {
            if (distances[*it1][farthest_point] + distances[farthest_point][*(next(it1))] - distances[*it1][*(next(it1))] < farthest_distance)
            {
                farthest_distance = distances[*it1][farthest_point] + distances[farthest_point][*(next(it1))] - distances[*it1][*(next(it1))];
                i = it1;
            }
            it1++;
        }

        // insert
        advance(i, 1);
        tour_seq.insert(i, farthest_point);
        is_included[farthest_point] = true;
    }
    // calculate cost
    double cost = 0.0;
    for (auto it = tour_seq.begin(); it != tour_seq.end(); it++)
    {
        // cout << "calculating cost between " << *it << ", " << *(next(it)) << ": " << distances[*it][*(next(it))] << "\n";
        cost += distances[*it][*(next(it))];
    }
    return tour(tour_seq, cost);
}

tour tsp::cheapest_insertion()
{
    int source = random_number;
    list<int> tour_seq;
    tour_seq.push_back(source);

    bool is_included[dimension];
    for (int i = 0; i < dimension; i++)
    {
        is_included[i] = false;
    }
    is_included[source] = true;

    double nearest_distance = DBL_MAX;
    int nearest_point = -1;
    for (int j = 0; j < dimension; j++)
    {
        if (j != source && (distances[source][j] < nearest_distance))
        {
            nearest_distance = distances[source][j];
            nearest_point = j;
        }
    }

    tour_seq.push_back(nearest_point);
    is_included[nearest_point] = true;
    tour_seq.push_back(source);

    while (tour_seq.size() < (dimension + 1))
    {
        // find edge to insert
        nearest_distance = DBL_MAX;
        auto it = tour_seq.begin();
        auto pos = tour_seq.begin();
        int point_to_insert = -1;
        for (int i = 0; i < (tour_seq.size() - 1); i++)
        {
            for (int j = 0; j < dimension; j++)
            {
                if (!is_included[j] && (distances[*it][j] + distances[j][*(next(it))] - distances[*it][*(next(it))]) < nearest_distance)
                {
                    nearest_distance = distances[*it][j] + distances[j][*(next(it))] - distances[*it][*(next(it))];
                    point_to_insert = j;
                    pos = it;
                }
            }
            it++;
        }

        // insert
        advance(pos, 1);
        tour_seq.insert(pos, point_to_insert);
        is_included[point_to_insert] = true;
    }
    // calculate cost
    double cost = 0.0;
    for (auto it = tour_seq.begin(); it != tour_seq.end(); it++)
    {
        // cout << "calculating cost between " << *it << ", " << *(next(it)) << ": " << distances[*it][*(next(it))] << "\n";

        // cout << *it << ",";
        cost += distances[*it][*(next(it))];
    }
    return tour(tour_seq, cost);
}

tour tsp::two_opt(tour t)
{
    list<int> tour_seq = t.get_seq();
    double cost = t.get_cost();

    bool improved = true;

    while (improved)
    {
        improved = false;
        auto it1 = tour_seq.begin();
        for (int i = 0; i < tour_seq.size() - 1; i++)
        {
            auto it2 = next(next(it1));
            for (int j = i + 2; j < tour_seq.size() - 1; j++)
            {
                double improvement = distances[*it1][*(next(it1))] + distances[*it2][*(next(it2))] - distances[*it1][*it2] - distances[*(next(it1))][*(next(it2))];
                if (improvement > 0.1)
                {
                    improved = true;
                    cost -= improvement;
                    reverse(next(it1), next(it2));
                }

                it2++;
            }
            it1++;
        }
    }

    return tour(tour_seq, cost);
}

void tsp::reverse_section(int *tour_seq, int start, int end)
{
    int i = start;
    int j = end;
    while (1)
    {
        // cout << "i " << i << " j " << j << "\n";
        if (i == j)
            break;
        if (j == (i - 1 + dimension) % dimension)
            break;
        int tmp = tour_seq[i];
        tour_seq[i] = tour_seq[j];
        tour_seq[j] = tmp;

        i = (i + 1) % dimension;
        j = (j - 1 + dimension) % dimension;
    }
}

double tsp::gain_from_three_opt(int *tour_arr, int x1, int x2, int y1, int y2, int z1, int z2, int opt_case)
{
    switch (opt_case)
    {
    case 3:
        // cout << "3: add edge " << x1 << "," << y1 << " " << x2 << "," << y2 << "\n";
        // cout << "3: " << distances[x1][x2] << " + " << distances[y1][y2] << " - " << distances[x1][y1] << " - " << distances[x2][y2];
        return (distances[x1][x2] + distances[y1][y2]) - (distances[x1][y1] + distances[x2][y2]);
        break;
    case 6:
        // cout << "6: add edge " << x1 << "," << y2 << " " << z1 << "," << y1 << " " << x2 << "," << z2 << "\n";
        // cout << "6: " << distances[x1][x2] << " + " << distances[y1][y2] << " + " << distances[z1][z2] << " - " << distances[x1][y2] << " - " << distances[z1][y1] << " - " << distances[x2][z2];
        return (distances[x1][x2] + distances[y1][y2] + distances[z1][z2]) - (distances[x1][y2] + distances[z1][y1] + distances[x2][z2]);
        break;
    case 7:
        // cout << "7: add edge " << x1 << "," << y2 << " " << z1 << "," << x2 << " " << y1 << "," << z2 << "\n";
        // cout << "7: " << distances[x1][x2] << " + " << distances[y1][y2] << " + " << distances[z1][z2] << " - " << distances[x1][y2] << " - " << distances[z1][x2] << " - " << distances[y1][z2];

        return (distances[x1][x2] + distances[y1][y2] + distances[z1][z2]) - (distances[x1][y2] + distances[z1][x2] + distances[y1][z2]);
        break;
    default:
        cout << "unknown opt case\n";
        return -1;
        break;
    }
}

void tsp::three_opt_move(int *tour_arr, int i, int j, int k, int opt_case)
{
    switch (opt_case)
    {
    case 3:
        reverse_section(tour_arr, (i + 1) % dimension, j);
        break;
    case 6:
        reverse_section(tour_arr, (k + 1) % dimension, i);
        reverse_section(tour_arr, (j + 1) % dimension, k);
        break;

    case 7:
        reverse_section(tour_arr, (k + 1) % dimension, i);
        reverse_section(tour_arr, (i + 1) % dimension, j);
        reverse_section(tour_arr, (j + 1) % dimension, k);
        break;
    default:
        cout << "unknown opt case\n";
        break;
    }
}

double tsp::gain_from_node_swap(int x0_pred, int x0, int x0_succ, int y0_pred, int y0, int y0_succ)
{
    double del_length;
    double add_length;

    if (y0 == x0_succ || x0_succ == y0_pred)
    {
        del_length = distances[x0_pred][x0] + distances[y0][y0_succ];
        add_length = distances[x0_pred][y0] + distances[x0][y0_succ];
    }
    else if (x0 == y0_succ || y0_succ == x0_pred)
    {
        del_length = distances[y0_pred][y0] + distances[x0][x0_succ];
        add_length = distances[y0_pred][x0] + distances[y0][x0_succ];
    }
    else
    {
        del_length = distances[x0_pred][x0] + distances[x0][x0_succ] + distances[y0_pred][y0] + distances[y0][y0_succ];
        add_length = distances[x0_pred][y0] + distances[y0][x0_succ] + distances[y0_pred][x0] + distances[x0][y0_succ];
    }
    return del_length - add_length;
}

tour tsp::three_opt(tour t)
{
    list<int> tour_seq = t.get_seq();
    int tour_arr[dimension + 1];
    int i = 0;
    for (auto v : tour_seq)
    {
        tour_arr[i++] = v;
    }
    double cost = t.get_cost();

    // double actual_cost = 0.0;
    // for (int m = 0; m < dimension; m++)
    // {
    //     // cout << "(" << tour_arr[m] << ", " << tour_arr[(m + 1) % dimension] << ") -> " << distances[tour_arr[m]][tour_arr[(m + 1) % dimension]] << "\n";
    //     actual_cost += distances[tour_arr[m]][tour_arr[(m + 1) % dimension]];
    // }
    // cout << "actual cost " << actual_cost << "\n";

    int max_loop = dimension;
    bool improved = true;
    while (max_loop--)
    {
        improved = false;
        for (int counter_1 = 0; counter_1 < dimension && !improved; counter_1++)
        {
            for (int counter_2 = counter_1 + 2; counter_2 < dimension && !improved; counter_2++)
            {
                for (int counter_3 = counter_2 + 2; counter_3 < dimension && !improved; counter_3++)
                {
                    int i = counter_1;
                    int x_1 = tour_arr[i];
                    int x_2 = tour_arr[(i + 1) % dimension];

                    int j = counter_2;
                    int y_1 = tour_arr[j];
                    int y_2 = tour_arr[(j + 1) % dimension];

                    int k = counter_3;
                    int z_1 = tour_arr[k];
                    int z_2 = tour_arr[(k + 1) % dimension];

                    int best_case = -1;
                    double best_move_gain = 0.0;
                    for (auto v : {3, 6, 7})
                    {
                        // cout << "calc gain case " << v << "\n";
                        double expected_gain = gain_from_three_opt(tour_arr, x_1, x_2, y_1, y_2, z_1, z_2, v);

                        if (expected_gain - best_move_gain > 0.1)
                        {
                            best_move_gain = expected_gain;
                            best_case = v;
                        }
                    }
                    if (best_move_gain > 0.1)
                    {
                        // cout << i << " " << j << " " << k << " (" << x_1 << "," << x_2 << "), (" << y_1 << "," << y_2 << "), (" << z_1 << "," << z_2 << ")\n";

                        // switch (best_case)
                        // {
                        // case 3:
                        //     cout << "3: add edge " << x_1 << "," << y_1 << " " << x_2 << "," << y_2 << "\n";
                        //     cout << "3: " << distances[x_1][x_2] << " + " << distances[y_1][y_2] << " - " << distances[x_1][y_1] << " - " << distances[x_2][y_2];

                        //     break;

                        // case 6:
                        //     cout << "6: add edge " << x_1 << "," << y_2 << " " << z_1 << "," << y_1 << " " << x_2 << "," << z_2 << "\n";
                        //     cout << "6: " << distances[x_1][x_2] << " + " << distances[y_1][y_2] << " + " << distances[z_1][z_2]
                        //          << " - " << distances[x_1][y_2] << " - " << distances[z_1][y_1] << " - " << distances[x_2][z_2];

                        //     break;

                        // case 7:
                        //     cout << "7: add edge " << x_1 << "," << y_2 << " " << z_1 << "," << x_2 << " " << y_1 << "," << z_2 << "\n";
                        //     cout << "7: " << distances[x_1][x_2] << " + " << distances[y_1][y_2] << " + " << distances[z_1][z_2]
                        //          << " - " << distances[x_1][y_2] << " - " << distances[z_1][x_2] << " - " << distances[y_1][z_2];

                        //     break;

                        // default:
                        //     break;
                        // }
                        // cout << " = " << best_move_gain << "\n";

                        // for (int m = 0; m < dimension; m++)
                        // {
                        //     cout << tour_arr[m] << " ";
                        // }
                        // cout << "\n";
                        // cout << counter_1 << " " << counter_2 << " " << counter_3 << " opt case " << best_case
                        //      << " expected gain " << best_move_gain << " cost " << cost << "\n";

                        three_opt_move(tour_arr, i, j, k, best_case);
                        cost -= best_move_gain;

                        improved = true;
                        // double actual_cost = 0.0;
                        // for (int m = 0; m < dimension; m++)
                        // {
                        //     // cout << tour_arr[m] << ", ";
                        //     cout << "(" << tour_arr[m] << ", " << tour_arr[(m + 1) % dimension] << ") -> " << distances[tour_arr[m]][tour_arr[(m + 1) % dimension]] << "\n";
                        //     actual_cost += distances[tour_arr[m]][tour_arr[(m + 1) % dimension]];
                        // }
                        // cout << "actual cost " << actual_cost << "\n";
                        break;
                    }
                }
            }
        }
    }

    return tour(list<int>(tour_arr, tour_arr + dimension + 1), cost);
}

tour tsp::node_shift(tour t)
{
    list<int> tour_seq = t.get_seq();
    int tour_arr[dimension + 1];
    int i = 0;
    for (auto v : tour_seq)
    {
        tour_arr[i++] = v;
    }
    double cost = t.get_cost();

    double actual_cost = 0.0;
    // for (int m = 0; m < dimension; m++)
    // {
    //     // cout << tour_arr[m] << " ";
    //     actual_cost += distances[tour_arr[m]][tour_arr[(m + 1) % dimension]];
    // }
    // cout << "before loop actual cost " << actual_cost << "\n";
    bool improved = true;
    int max_loop = dimension;
    while (max_loop--)
    {
        improved = false;

        for (int i = 0; i < dimension; i++)
        {
            int x0_pred = tour_arr[(i + dimension - 1) % dimension];
            int x0 = tour_arr[i];
            int x0_succ = tour_arr[(i + 1) % dimension];

            for (int j = i + 1; j < dimension - 1; j++)
            {
                // int new_pos = (i + j) % dimension;
                int y_1 = tour_arr[j];
                int y_2 = tour_arr[(j + 1) % dimension];

                double improvement = (distances[x0_pred][x0] + distances[x0][x0_succ] + distances[y_1][y_2]) - (distances[x0_pred][x0_succ] + distances[y_1][x0] + distances[x0][y_2]);
                // cout << "i " << i << " j " << j << " improvement " << improvement << "\n";
                if (improvement > 1)
                {
                    cost -= improvement;

                    improved = true;
                    // cout << "i " << i << " j " << j << " improvement " << improvement << "\n";
                    // Make_Node_Shift_Move(tour, i, j)

                    int shift_len = (j - i + dimension) % dimension;
                    int x_0 = tour_arr[i];
                    int left = i;
                    int right = left;
                    // cout << "shifting " << i << " to " << j << "\n";
                    for (int k = 0; k < shift_len; k++)
                    {
                        // cout << "inside shiftlen\n";
                        right = (left + 1) % dimension;
                        tour_arr[left] = tour_arr[right];
                        left = right;
                    }
                    tour_arr[j] = x_0;

                    // double actual_cost = 0.0;
                    // for (int m = 0; m < dimension; m++)
                    // {
                    //     // cout << tour_arr[m] << " ";
                    //     actual_cost += distances[tour_arr[m]][tour_arr[(m + 1) % dimension]];
                    // }
                    // cout << "inside loop actual cost " << actual_cost << "\n";

                    break;
                }
            }
        }
    }
    list<int> l(tour_arr, tour_arr + dimension);
    l.push_back(tour_arr[0]);

    return tour(l, cost);
}

tour tsp::node_swap(tour t)
{
    list<int> tour_seq = t.get_seq();
    int tour_arr[dimension + 1];
    int i = 0;
    for (auto v : tour_seq)
    {
        tour_arr[i++] = v;
    }
    double cost = t.get_cost();

    double actual_cost = 0.0;
    // for (int m = 0; m < dimension; m++)
    // {
    //     // cout << tour_arr[m] << " ";
    //     actual_cost += distances[tour_arr[m]][tour_arr[(m + 1) % dimension]];
    // }
    // cout << "before loop actual cost " << actual_cost << "\n";
    int max_loop = dimension;
    while (max_loop--)
    {

        for (int i = 0; i < dimension; i++)
        {
            int x0_pred = tour_arr[(i + dimension - 1) % dimension];
            int x0 = tour_arr[i];
            int x0_succ = tour_arr[(i + 1) % dimension];

            for (int j = i + 1; j < dimension; j++)
            {
                // int new_pos = (i + j) % dimension;
                int y0 = tour_arr[j];
                int y0_pred = tour_arr[(j - 1 + dimension) % dimension];
                int y0_succ = tour_arr[(j + dimension) % dimension];

                double improvement = gain_from_node_swap(x0_pred, x0, x0_succ, y0_pred, y0, y0_succ);
                // cout << "i " << i << " j " << j << " improvement " << improvement << "\n";
                if (improvement > 1)
                {
                    cost -= improvement;
                    int tmp = tour_arr[i];
                    tour_arr[i] = tour_arr[j];
                    tour_arr[j] = tmp;
                    break;
                }
            }
        }
    }
    list<int> l(tour_arr, tour_arr + dimension);
    l.push_back(tour_arr[0]);

    return tour(l, cost);
}
