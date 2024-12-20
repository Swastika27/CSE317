#include "node.h"

node::node(node *parent_node, board *current, int cost, int estimated_cost)
{
    this->parent_node = parent_node;
    // this->parent = parent;
    this->current = current;
    this->cost = cost;
    this->estimated_cost = estimated_cost;
}

node::~node()
{
}

// void node::set_parent(board *b)
// {
//     // this->parent = b;
// }

node *node::get_parent_node()
{
    return parent_node;
}

// board *node::get_parent()
// {
//     // return parent;
// }

board *node::get_current()
{
    return current;
}

int node::get_cost()
{
    return cost;
}

int node::get_estimated_cost()
{
    return estimated_cost;
}

bool compare_node_ptr(node *a, node *b)
{
    return a->get_estimated_cost() > b->get_estimated_cost();
}
