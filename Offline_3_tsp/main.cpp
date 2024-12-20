#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <iomanip>

#include "tour.h"
#include "point.h"
#include "tsp.h"

using namespace std;

void read_points(point **points, int *dimension, ifstream &file)
{
    // cout << "inside read file\n";
    string line;

    // Read headers
    getline(file, line); // name
    getline(file, line); // comment
    getline(file, line); // type
    getline(file, line); // dimension

    // Parse dimension
    istringstream iss(line);
    string label;
    string l2;
    string l3;
    iss >> label >> l2 >> l3;
    *dimension = (l3 == "") ? atoi(l2.c_str()) : atoi(l3.c_str());

    getline(file, line); // edge weight type
    getline(file, line); // node coord

    *points = new point[*dimension]; // Allocate memory
    // cout << "after allocating memory\n";

    for (int i = 0; i < *dimension; i++)
    {
        getline(file, line);
        int index;
        double x_coord, y_coord;

        istringstream s_line(line);
        s_line >> index >> x_coord >> y_coord;

        (*points)[i] = point(x_coord, y_coord); // Use allocated memory
    }
}

int main(int argc, char *argv[])
{
    ifstream names_file("input_file_names.txt");
    if (!names_file.is_open())
    {
        cout << "Failed to open input_file_names.txt\n";
        return 1;
    }

    string str;
    getline(names_file, str);
    int file_count = stoi(str); // Use stoi instead of atoi for error checking
                                // cout << "file count " << file_count << "ab\n";
    std::ofstream file("output.csv");

    // Save the original buffer of std::cout
    std::streambuf *originalBuffer = std::cout.rdbuf();

    // Redirect std::cout to the file
    std::cout.rdbuf(file.rdbuf());
    // cout << "file"
    //      << "," << "NN" << "," << "NI" << "," << "FI" << "," << "CI"
    //      << "," << "NN + 2-opt" << "," << "NI + 2-opt" << "," << "FI + 2-opt" << "," << "CI + 2-opt"
    //      << "," << "NN + 3-opt" << "," << "NI + 3-opt" << "," << "FI + 3-opt" << "," << "CI + 3-opt"
    //      << "," << "NN + shift" << "," << "NI + shift" << "," << "FI + shift" << "," << "CI + shift"
    //      << "," << "NN + 2-opt + shift" << "," << "NI + 2-opt + shift" << "," << "FI + 2-opt + shift" << "," << "CI + 2-opt + shift"
    //      << "," << "NN + shift + 2-opt" << "," << "NI + shift + 2-opt" << "," << "FI + shift + 2-opt" << "," << "CI + shift + 2-opt" << "\n";

    for (int j = 0; j < file_count; j++)
    {
        string file_name;
        getline(names_file, file_name);

        // Trim whitespace from file_name
        file_name.erase(file_name.find_last_not_of(" \n\r\t") + 1);
        // cout << "Attempting to open file: benchmark_data/" << file_name << endl;

        string file_path = "benchmark_data/" + file_name;
        ifstream input_file(file_path);
        if (!input_file.is_open())
        {
            cout << "Failed to open the file " << "benchmark_data/" + file_name << "\n";
            continue;
        }

        point *points = nullptr;
        int dimension = 0;

        read_points(&points, &dimension, input_file);
        // for (int i = 0; i < dimension; i++)
        // {
        //     cout << points[i].get_x() << ", " << points[i].get_y() << "\n";
        // }

        input_file.close();

        tsp travelling_salesman(dimension, points);
        // printf("created tsp\n");

        //         cout << file_name
        //              << "," << travelling_salesman.nearest_neighbour().get_cost()
        //              << "," << travelling_salesman.nearest_insertion().get_cost()
        //              << "," << travelling_salesman.farthest_insertion().get_cost()
        //              << "," << travelling_salesman.cheapest_insertion().get_cost()
        //              << "," << travelling_salesman.two_opt(travelling_salesman.nearest_neighbour()).get_cost()
        //              << "," << travelling_salesman.two_opt(travelling_salesman.nearest_insertion()).get_cost()
        //              << "," << travelling_salesman.two_opt(travelling_salesman.farthest_insertion()).get_cost()
        //              << "," << travelling_salesman.two_opt(travelling_salesman.cheapest_insertion()).get_cost()
        //              << "," << travelling_salesman.node_swap(travelling_salesman.nearest_neighbour()).get_cost()
        //              << "," << travelling_salesman.node_swap(travelling_salesman.nearest_insertion()).get_cost()
        //              << "," << travelling_salesman.node_swap(travelling_salesman.farthest_insertion()).get_cost()
        //              << "," << travelling_salesman.node_swap(travelling_salesman.cheapest_insertion()).get_cost()
        //              << "," << travelling_salesman.node_shift(travelling_salesman.nearest_neighbour()).get_cost()
        //              << "," << travelling_salesman.node_shift(travelling_salesman.nearest_insertion()).get_cost()
        //              << "," << travelling_salesman.node_shift(travelling_salesman.farthest_insertion()).get_cost()
        //              << "," << travelling_salesman.node_shift(travelling_salesman.cheapest_insertion()).get_cost()
        //              << "," << travelling_salesman.node_shift(travelling_salesman.two_opt(travelling_salesman.nearest_neighbour())).get_cost()
        //              << "," << travelling_salesman.node_shift(travelling_salesman.two_opt(travelling_salesman.nearest_insertion())).get_cost()
        //              << "," << travelling_salesman.node_shift(travelling_salesman.two_opt(travelling_salesman.farthest_insertion())).get_cost()
        //              << "," << travelling_salesman.node_shift(travelling_salesman.two_opt(travelling_salesman.cheapest_insertion())).get_cost()
        //              << "," << travelling_salesman.two_opt(travelling_salesman.node_shift(travelling_salesman.nearest_neighbour())).get_cost()
        //              << "," << travelling_salesman.two_opt(travelling_salesman.node_shift(travelling_salesman.nearest_insertion())).get_cost()
        //              << "," << travelling_salesman.two_opt(travelling_salesman.node_shift(travelling_salesman.farthest_insertion())).get_cost()
        //              << "," << travelling_salesman.two_opt(travelling_salesman.node_shift(travelling_salesman.cheapest_insertion())).get_cost() << "\n";
        // cout << travelling_salesman.three_opt(travelling_salesman.nearest_neighbour()).get_cost();

        cout << file_name << "\n";
        cout << ",no opt,2-opt,3-opt,node shift\n";
        cout << "Nearest neighbour," << travelling_salesman.nearest_neighbour().get_cost() << ","
             << travelling_salesman.two_opt(travelling_salesman.nearest_neighbour()).get_cost() << ","
             << travelling_salesman.three_opt(travelling_salesman.nearest_neighbour()).get_cost() << ","
             << travelling_salesman.node_shift(travelling_salesman.nearest_neighbour()).get_cost() << "\n";

        cout << "Nearest insertion," << travelling_salesman.nearest_insertion().get_cost() << ","
             << travelling_salesman.two_opt(travelling_salesman.nearest_insertion()).get_cost() << ","
             << travelling_salesman.three_opt(travelling_salesman.nearest_insertion()).get_cost() << ","
             << travelling_salesman.node_shift(travelling_salesman.nearest_insertion()).get_cost() << "\n";

        cout << "Furthest insertion," << travelling_salesman.farthest_insertion().get_cost() << ","
             << travelling_salesman.two_opt(travelling_salesman.farthest_insertion()).get_cost() << ","
             << travelling_salesman.three_opt(travelling_salesman.farthest_insertion()).get_cost() << ","
             << travelling_salesman.node_shift(travelling_salesman.farthest_insertion()).get_cost() << "\n";

        cout << "Cheapest insertion," << travelling_salesman.cheapest_insertion().get_cost() << ","
             << travelling_salesman.two_opt(travelling_salesman.cheapest_insertion()).get_cost() << ","
             << travelling_salesman.three_opt(travelling_salesman.cheapest_insertion()).get_cost() << ","
             << travelling_salesman.node_shift(travelling_salesman.cheapest_insertion()).get_cost() << "\n\n";

        delete[] points;
    }

    names_file.close();
    return 0;
}
