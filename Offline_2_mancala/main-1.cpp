#include <iostream>
using namespace std;

#include "alpha_beta.h"
#include "mancala.h"

int main()
{
    mancala m(0);
    // m.print_board();

    int heuristic_1 = 1;
    int heuristic_2 = 4;

    int player1_depth = 6;
    int player2_depth = 6;

    for (int i = 1; i <= 10; i++)
    {
        player1_depth = player2_depth = i;
        mancala m(0);
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
            // printf("Player %d's move\n", current_player + 1);
            m.move(current_player, move);
            // m.print_board();

            switch (m.get_winner())
            {
            case 0:
                printf("Player 1 wins\n");
                break;
            case 1:
                printf("Player 2 wins\n");
                break;
            case 2:
                printf("Game draw\n");
                break;

            default:
                break;
            }
        }
    }

    // while (true)
    // {
    //     // printf("game end = %d\n", m.game_end());
    //     if (m.game_end())
    //         break;

    //     int current_player = m.get_curr_player();
    //     int move;
    //     if (current_player == 0)
    //     {
    //         move = alpha_beta_search(m, current_player, heuristic_1, player1_depth);
    //     }
    //     else
    //     {
    //         move = alpha_beta_search(m, current_player, heuristic_2, player2_depth);
    //     }
    //     // printf("Player = %d, move = %d\n", current_player, move);
    //     printf("Player %d's move\n", current_player + 1);
    //     m.move(current_player, move);
    //     m.print_board();
    // }

    // switch (m.get_winner())
    // {
    // case 0:
    //     printf("Player 1 wins\n");
    //     break;
    // case 1:
    //     printf("Player 2 wins\n");
    //     break;
    // case 2:
    //     printf("Game draw\n");
    //     break;

    // default:
    //     break;
    // }
}