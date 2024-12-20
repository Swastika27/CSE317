#ifndef NODE_H
#define NODE_H

#include "board.h"

class node
{
private:
    node *parent_node;
    // board *parent;
    board *current;
    int cost;
    int estimated_cost;

public:
    node(node *parent_node, board *current, int cost, int estimated_cost);
    ~node();
    // void set_parent(board *b);
    node *get_parent_node();
    // board *get_parent();
    board *get_current();
    int get_cost();
    int get_estimated_cost();
};

bool compare_node_ptr(node *a, node *b);

#endif