
#include <iostream>
#include <random>
#include <fstream>

#include "alpha_beta.h"
#include "mancala.h"
// #include "board.h"

using namespace std;

int main()
{
    int lower_bound = 1;
    int upper_bound = 10;

    // Create a random number generator
    random_device rd;        // Obtain a random seed from the hardware
    mt19937 generator(rd()); // Seed the generator
    uniform_int_distribution<int> distribution(lower_bound, upper_bound);

    FILE *file = fopen("report.txt", "w");
    if (!file)
    {
        printf("Error opening file");
        exit(1);
    }

    for (int heuristic_1 = 1; heuristic_1 <= 4; heuristic_1++)
    {
        for (int heuristic_2 = 1; heuristic_2 <= 4; heuristic_2++)
        {
            int player1_win = 0;
            int player2_win = 0;
            int draw = 0;

            for (int i = 1; i <= 10; i++)
                for (int j = 1; j <= 10; j++)
                {
                    mancala m(0);
                    // m.print_board();

                    int player1_depth = i;
                    int player2_depth = j;

                    // int player1_depth = distribution(generator);
                    // int player2_depth = distribution(generator);

                    while (true)
                    {
                        // printf("game end = %d\n", m.game_end());
                        if (m.game_end())
                            break;

                        int current_player = m.get_curr_player();
                        int move;
                        if (current_player == 0)
                        {
                            move = alpha_beta_search(m, current_player, heuristic_1, player1_depth);
                        }
                        else
                        {
                            move = alpha_beta_search(m, current_player, heuristic_2, player2_depth);
                        }
                        // printf("Player = %d, move = %d\n", current_player, move);
                        m.move(current_player, move);
                        // m.print_board();
                    }

                    // fprintf(file, "Player 1: depth %2d, Player 2: depth %2d --> winner %2d\n", i, j, m.get_winner());

                    switch (m.get_winner())
                    {
                    case 0:
                        player1_win++;
                        break;
                    case 1:
                        player2_win++;
                        break;
                    case 2:
                        draw++;
                        break;

                    default:
                        break;
                    }
                }

            fprintf(file, "\nPlayer 1: heuristic = %d\t", heuristic_1);
            fprintf(file, "Player 2: heuristic = %d\n", heuristic_2);
            fprintf(file, "Player 1 win: %d, Player 2 win: %d, Draw: %d\n\n", player1_win, player2_win, draw);
        }
    }

    // printf("game ended\n");
    return 0;
}