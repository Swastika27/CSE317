#ifndef BOARD_H
#define BOARD_H

class board
{
private:
    int **arr;
    int k;
    int blank_row;
    int blank_col;

public:
    board(int k);
    ~board();
    void set_value(int val, int i, int j);
    int get_value(int i, int j);
    void print_board();
    int get_inversion_count();
    bool is_solvable();
    int get_hamming_distance();
    int get_manhattan_distance();
    bool operator==(board const &b);
    board *up();
    board *down();
    board *left();
    board *right();
};

#endif