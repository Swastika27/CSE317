#ifndef MANCALA_H
#define MANCALA_H

class mancala
{
    int board[14];

    int curr_player;

    int captured_stones[2];
    int extra_moves[2];

    int w1;
    int w2;
    int w3;
    int w4;

    int get_pit_gems(int player);

public:
    mancala(int curr_player);
    ~mancala();
    void print_board();
    bool is_valid_move(int player, int pit);
    void move(int player, int pit);
    int get_curr_player();
    int get_heuristic(int player, int type);
    bool game_end();
    int get_extra_moves(int player);
    int get_winner();
};

#endif