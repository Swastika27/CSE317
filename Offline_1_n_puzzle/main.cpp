#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <sstream>

#include "board.h"
#include "node.h"

using namespace std;

int get_hieuristic(board *b, int priority_func)
{
    if (priority_func == 0)
        return b->get_hamming_distance();
    if (priority_func == 1)
        return b->get_manhattan_distance();
    return -1;
}

void print_solution(node *n)
{
    stack<node *> s;
    while (n)
    {
        s.push(n);
        n = n->get_parent_node();
    }

    cout << "Solution: \n";
    int i = 0;

    while (!s.empty())
    {
        node *curr = s.top();
        cout << "Step " << i++ << "\n";
        // cout << "Step " << i++ << " cost " << curr->get_cost() << "\n";
        curr->get_current()->print_board();
        s.pop();
    }
}

bool is_in_list(vector<board *> v, board *b)
{
    for (const auto &ptr : v)
    {
        if (*ptr == *b)
            return true;
    }
    return false;
}

node *A_star_search(board *start, board *goal, int priority_func) // 1 -> manhattan, 0 -> hamming
{
    // cout << "inside a star\n";
    // start->print_board();
    // goal->print_board();

    int explored_node_count = 0;
    int expanded_node_count = 0;

    vector<board *> closed_list;

    node *start_node = new node(nullptr, start, 0, 0 + get_hieuristic(start, priority_func));

    priority_queue<node *, vector<node *>, decltype(&compare_node_ptr)> q(compare_node_ptr);

    q.push(start_node);
    explored_node_count++;
    closed_list.push_back(start_node->get_current());

    while (!q.empty())
    {
        node *current_node = q.top();

        // cout << "current node: cost " << current_node->get_cost() << " estimated total cost " << current_node->get_estimated_cost() << "\n";
        // current_node->get_current()->print_board();
        q.pop();

        if (*(current_node->get_current()) == *goal)
        {
            cout << "explored node: " << explored_node_count << "\n";
            cout << "expanded node: " << expanded_node_count << "\n";
            return current_node;
        }

        expanded_node_count++;

        board *next_board;

        next_board = current_node->get_current()->left();
        if (next_board && (!is_in_list(closed_list, next_board)))
        {
            // cout << "left\n";
            // next_board->print_board();
            explored_node_count++;
            q.push(new node(current_node, next_board, current_node->get_cost() + 1, current_node->get_cost() + 1 + get_hieuristic(next_board, priority_func)));
            closed_list.push_back(next_board);
        }

        next_board = current_node->get_current()->right();
        if (next_board && (!is_in_list(closed_list, next_board)))
        {
            // cout << "right\n";
            // next_board->print_board();
            explored_node_count++;
            q.push(new node(current_node, next_board, current_node->get_cost() + 1, current_node->get_cost() + 1 + get_hieuristic(next_board, priority_func)));
            closed_list.push_back(next_board);
        }

        next_board = current_node->get_current()->up();

        if (next_board && (!is_in_list(closed_list, next_board)))
        {
            // cout << "up\n";
            // next_board->print_board();
            explored_node_count++;
            q.push(new node(current_node, next_board, current_node->get_cost() + 1, current_node->get_cost() + 1 + get_hieuristic(next_board, priority_func)));
            closed_list.push_back(next_board);
        }

        next_board = current_node->get_current()->down();

        if (next_board && (!is_in_list(closed_list, next_board)))
        {
            // cout << "down\n";
            // next_board->print_board();
            explored_node_count++;
            q.push(new node(current_node, next_board, current_node->get_cost() + 1, current_node->get_cost() + 1 + get_hieuristic(next_board, priority_func)));
            closed_list.push_back(next_board);
        }
    }

    return nullptr;
}

int main()
{

    int k;
    board *start;
    board *goal;

    // // input from console
    // cout << "Enter value of k: ";
    // cin >> k;

    // start = new board(k);
    // for(int i = 0; i < k; i++)
    //     for(int j = 0; j < k; j++) {
    //         string s;
    //         cout << "Enter block at position (" << i << ", " << j << "): ";
    //         cin >> s;
    //         if(s == "*") start->set_value(0, i, j);
    //         else start->set_value(atoi(s.c_str()), i, j);
    //     }

    // input from file
    string line;
    ifstream input_file;
    input_file.open("input.txt");
    if (input_file)
    {
        getline(input_file, line);
        k = atoi(line.c_str());

        start = new board(k);

        for (int i = 0; i < k; i++)
        {
            getline(input_file, line);
            istringstream iss(line);

            string s;
            for (int j = 0; j < k; j++)
            {
                iss >> s;
                if (s == "*")
                    start->set_value(0, i, j);
                else
                    start->set_value(atoi(s.c_str()), i, j);
            }
        }
    }
    // start->print_board();

    goal = new board(k);
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            goal->set_value(i * k + j + 1, i, j);
        }
    }
    goal->set_value(0, k - 1, k - 1);
    // goal->print_board();

    // if (start->up())
    //     start->up()->print_board();
    // if (start->down())
    //     start->down()->print_board();
    // if (start->left())
    //     start->left()->print_board();
    // if (start->right())
    //     start->right()->print_board();

    cout << "Solvable? " << start->is_solvable() << "\n";
    // cout << "Hamming distance " << start->get_hamming_distance() << "\n";
    // cout << "Manhattan distance " << start->get_manhattan_distance() << "\n";

    if (start->is_solvable())
    {
        cout << "Using Hamming distance \n";
        node *solution = A_star_search(start, goal, 0);
        cout << "Optimal cost: " << solution->get_cost() << "\n";
        // print_solution(solution);

        cout << "\nUsing Manhattan distance \n";
        solution = A_star_search(start, goal, 1);
        cout << "Optimal cost: " << solution->get_cost() << "\n\n";
        print_solution(solution);
    }
}