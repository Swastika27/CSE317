#include "alpha_beta.h"
#include <iostream>
using namespace std;

// need to know if current move is by player 1 or 2
// whether current player is MIN or MAX
// who is the next player
// all these trouble bc it allows bonus move
// if we want to limit depth, need to store depth as well
int max_value(mancala m, int alpha, int beta, int curr_player, bool isMax, int heuristic, int add_moves, int depth, int max_depth)
{
    // it isMax is redundant. jar upor max call hoise she to max e
    // printf("inside max_val depth = %d, max_depth = %d\n", depth, max_depth);
    int max_player = isMax ? curr_player : (1 - curr_player);
    // if (b.game_end())
    //     return b.get_heuristic(max_player, heuristic, add_moves);
    if (depth >= max_depth || m.game_end())
        return m.get_heuristic(curr_player, heuristic);

    // printf("depth < max_depth");
    int value = __INT32_MAX__ * (-1);
    mancala m_copy = m;
    for (int i = 0; i < 6; i++)
    {
        if (m.is_valid_move(curr_player, i))
        {
            m.move(curr_player, i);
            int next_player = m.get_curr_player();
            int next_value = (next_player == curr_player) ? max_value(m, alpha, beta, next_player, 1, heuristic, add_moves + 1, depth + 1, max_depth)
                                                          : min_value(m, alpha, beta, next_player, 0, heuristic, add_moves, depth + 1, max_depth);

            // printf("player %d, move %d, depth %d, value %d\n", curr_player, i, depth, next_value);
            value = (next_value > value) ? next_value : value;
            if (value >= beta)
                return value;
            alpha = (alpha > value) ? alpha : value;
        }
        m = m_copy;
    }
    // printf("returning from max value : %d\n", value);
    return value;
}

int min_value(mancala m, int alpha, int beta, int curr_player, bool isMax, int heuristic, int add_moves, int depth, int max_depth)
{
    // j player er jonne min_val call hoise she to max player hoite pare na
    // printf("inside min_val depth = %d, max_depth = %d\n", depth, max_depth);
    int max_player = isMax ? curr_player : (1 - curr_player);
    // if (b.game_end())
    //     return b.get_heuristic(max_player, heuristic, add_moves);
    if (depth >= max_depth || m.game_end())
        return m.get_heuristic(1 - curr_player, heuristic);

    // printf("depth < max_depth\n");

    int value = __INT32_MAX__;
    mancala m_copy = m;
    for (int i = 5; i >= 0; i--)
    {
        if (m.is_valid_move(curr_player, i))
        {
            m.move(curr_player, i);
            int next_player = m.get_curr_player();
            int next_value = (next_player == curr_player) ? min_value(m, alpha, beta, next_player, 0, heuristic, add_moves, depth + 1, max_depth)
                                                          : max_value(m, alpha, beta, next_player, 1, heuristic, add_moves, depth + 1, max_depth);

            // printf("player %d, move %d, depth %d, value %d\n", curr_player, i, depth, next_value);
            value = (next_value < value) ? next_value : value;
            if (value <= alpha)
                return value;
            beta = (value < beta) ? value : beta;
        }
        m = m_copy;
    }
    // printf("returning from min value : %d\n", value);
    return value;
}

int alpha_beta_search(mancala m, int curr_player, int heuristic, int max_depth)
{
    // current player is MAX
    // returns the pit number current player should choose
    int value = __INT32_MAX__ * (-1);
    int best_move = 0;
    mancala m_copy = m;
    for (int i = 0; i < 6; i++)
    {
        if (m.is_valid_move(curr_player, i))
        {
            m.move(curr_player, i);
            int next_player = m.get_curr_player();
            int next_value = (next_player == curr_player) ? max_value(m, __INT32_MAX__ * (-1), __INT_MAX__, next_player, 1, heuristic, 1, 1, max_depth)
                                                          : min_value(m, __INT32_MAX__ * (-1), __INT_MAX__, next_player, 0, heuristic, 0, 1, max_depth);

            // printf("player %d, move %d, depth %d, value %d\n", curr_player, i, 0, next_value);
            // b.print_player_1_pov();
            if (next_value > value)
            {
                value = next_value;
                best_move = i;
            }
            m = m_copy;
        }
    }
    // printf("returning from alpha beta \n");
    return best_move;
}
