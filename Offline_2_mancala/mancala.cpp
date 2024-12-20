#include "mancala.h"

#include <iostream>
using namespace std;

int mancala::get_pit_gems(int player)
{
    int gems = 0;
    for (int i = (player * 7); i < (player * 7 + 6); i++)
    {
        gems += board[i];
    }
    return gems;
}

mancala::mancala(int curr_player)
{
    this->curr_player = curr_player;

    for (int i = 0; i < 14; i++)
        board[i] = 4;
    board[6] = board[13] = 0;

    w1 = 10;
    w2 = 2;
    w3 = 10;
    w4 = 2;

    captured_stones[0] = captured_stones[1] = 0;
    extra_moves[0] = extra_moves[1] = 0;
}

mancala::~mancala()
{
}

void mancala::print_board()
{
    printf("\n    ");
    for (int i = 12; i > 6; i--)
    {
        printf("%4d", board[i]);
    }
    printf("\n%4d     ", board[13]);
    // for (int i = 10; i > 6; i--)
    // {
    //     printf("     ");
    // }
    printf("\t\t\t");
    printf("%d\n    ", board[6]);
    for (int i = 0; i < 6; i++)
    {
        printf("%4d", board[i]);
    }
    printf("\n");
}

bool mancala::is_valid_move(int player, int pit)
{
    return (player == 0 || player == 1) && (pit >= 0 && pit <= 5) && board[player * 7 + pit];
}

void mancala::move(int player, int pit)
{
    pit = player * 7 + pit;
    int gems = board[pit];
    board[pit] = 0;

    while (true)
    {
        if (!gems)
            break;

        pit = (pit + 1) % 14;
        if (pit == ((1 - player) * 7 + 6))
        {
            continue;
        }
        else
        {
            board[pit]++;
            gems--;
        }
    }
    // last_pit = pit;
    if (pit >= (player * 7) && pit <= (player * 7 + 5) && board[pit] == 1 && board[12 - pit])
    {
        // capture situation
        int total_gems = board[pit] + board[12 - pit];
        board[(pit / 7) * 7 + 6] += total_gems;

        board[pit] = board[12 - pit] = 0;
        captured_stones[pit / 7] += total_gems;
    }
    if (pit == (player * 7 + 6))
    {
        // extra move
        curr_player = player;
    }
    else
    {
        curr_player = (1 - player);
    }
}

int mancala::get_curr_player()
{
    return curr_player;
}

int mancala::get_heuristic(int player, int type)
{
    // printf("heuristic-1 = %d, heuristic-2 = %d, heuristic-3 = %d, heuristic-4 = %d\n", w1, w2, w3, w4);
    switch (type)
    {
    case 1:
        return board[player * 7 + 6] - board[(1 - player) * 7 + 6];
        break;
    case 2:
        return w1 * (board[player * 7 + 6] - board[(1 - player) * 7 + 6]) + w2 * (get_pit_gems(player) - get_pit_gems(1 - player));
        break;
    case 3:
        return w1 * (board[player * 7 + 6] - board[(1 - player) * 7 + 6]) + w2 * (get_pit_gems(player) - get_pit_gems(1 - player)) + w3 * extra_moves[player];
        break;
    case 4:
        return w1 * (board[player * 7 + 6] - board[(1 - player) * 7 + 6]) + w2 * (get_pit_gems(player) - get_pit_gems(1 - player)) + w3 * extra_moves[player] + w4 * captured_stones[player];
        break;

    default:
        printf("invalid parameter for function get_heuristic: player = %d, type = %d\n", player, type);
        return -1;
        break;
    }
}

bool mancala::game_end()
{
    bool allempty = true;

    for (int i = 0; i < 6; i++)
    {
        if (board[i])
            allempty = false;
    }

    if (allempty)
        return true;

    allempty = true;

    for (int i = 7; i < 13; i++)
    {
        if (board[i])
            allempty = false;
    }
    return allempty;
}

int mancala::get_extra_moves(int player)
{
    return extra_moves[player];
}

int mancala::get_winner()
{
    if (game_end())
    {
        if (board[13] > board[6])
            return 1;
        else if (board[6] > board[13])
            return 0;
        else
            return 2;
    }
    return -1;
}
