#include <iostream>

#include "board.h"
using namespace std;

board::board(int k)
{
    this->k = k;
    arr = new int *[k];
    for (int i = 0; i < k; i++)
        arr[i] = new int[k];
}

board::~board()
{
    for (int i = 0; i < k; i++)
        delete[] arr;
    delete arr;
}

void board::set_value(int val, int i, int j)
{
    if (i < k && j < k)
    {
        arr[i][j] = val;

        if (val == 0)
        {
            blank_row = i;
            blank_col = j;
        }
    }
    else
    {
        cout << "array index out of bounds\n";
    }
}

int board::get_value(int i, int j)
{
    if (i < k && j < k)
    {
        return arr[i][j];
    }
    return -1;
}

void board::print_board()
{
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            if (arr[i][j] == 0)
                printf("%5s", "*");
            else
                printf("%5d", arr[i][j]);
        }
        cout << "\n";
    }
    cout << "\n";
}

int board::get_inversion_count()
{
    int inversion_count = 0;
    for (int i = 0; i < k * k; i++)
    {
        int row = i / k;
        int col = i % k;
        for (int j = i + 1; j < k * k; j++)
        {
            int row2 = j / k;
            int col2 = j % k;
            if (arr[row][col] && arr[row2][col2] && arr[row][col] > arr[row2][col2])
            {
                inversion_count++;
                // cout << "inver " << arr[row][col] << " " << arr[row2][col2] << "\n";
            }
        }
    }
    // cout << "inversion count " << inversion_count << "\n";
    return inversion_count;
}

bool board::is_solvable()
{
    if (k % 2 == 1)
        return (get_inversion_count() % 2 == 0);
    return (get_inversion_count() + blank_row) % 2 == 1;
}

int board::get_hamming_distance()
{
    int distance = 0;
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            if (arr[i][j] && (arr[i][j] != (i * k + j + 1)))
                distance++;
        }
    }

    return distance;
}

int board::get_manhattan_distance()
{
    int distance = 0;
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            if (arr[i][j])
            {
                int goal_row = (arr[i][j] - 1) / k;
                int goal_col = (arr[i][j] - 1) % k;
                distance += abs(i - goal_row) + abs(j - goal_col);
                // cout << arr[i][j] << " current pos " << i << " " << j << ", goal pos " << goal_row << ", " << goal_col << "\n";
                // cout << abs(i - goal_row) << " " << abs(j - goal_col) << "\n";
            }
        }
    }

    return distance;
}

bool board::operator==(board const &b)
{
    if (k != b.k)
        return false;

    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            if (arr[i][j] != b.arr[i][j])
                return false;
        }
    }

    return true;
}

board *board::up()
{
    if (blank_row - 1 < 0)
        return nullptr;

    board *next = new board(k);
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            next->set_value(arr[i][j], i, j);
        }
    }

    int new_blank_row = (blank_row - 1);

    next->set_value(arr[new_blank_row][blank_col], blank_row, blank_col);
    next->set_value(0, new_blank_row, blank_col);

    return next;
}

board *board::down()
{
    if (blank_row + 1 >= k)
        return nullptr;

    board *next = new board(k);
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            next->set_value(arr[i][j], i, j);
        }
    }

    int new_blank_row = (blank_row + 1);

    next->set_value(arr[new_blank_row][blank_col], blank_row, blank_col);
    next->set_value(0, new_blank_row, blank_col);

    return next;
}

board *board::left()
{
    if (blank_col - 1 < 0)
        return nullptr;

    board *next = new board(k);
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            next->set_value(arr[i][j], i, j);
        }
    }

    int new_blank_col = (blank_col - 1);

    next->set_value(arr[blank_row][new_blank_col], blank_row, blank_col);
    next->set_value(0, blank_row, new_blank_col);

    return next;
}

board *board::right()
{
    if (blank_col + 1 >= k)
        return nullptr;

    board *next = new board(k);
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            next->set_value(arr[i][j], i, j);
        }
    }

    int new_blank_col = (blank_col + 1);

    next->set_value(arr[blank_row][new_blank_col], blank_row, blank_col);
    next->set_value(0, blank_row, new_blank_col);

    return next;
}
