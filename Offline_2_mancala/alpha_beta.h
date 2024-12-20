

#ifndef ALPHA_BETA_H
#define ALPHA_BETA_H

#include "mancala.h"

int min_value(mancala m, int alpha, int beta, int curr_player, bool isMax, int heuristic, int add_moves, int depth, int max_depth);
int max_value(mancala m, int alpha, int beta, int curr_player, bool isMax, int heuristic, int add_moves, int depth, int max_depth);
int alpha_beta_search(mancala m, int curr_player, int heuristic, int max_depth);

#endif