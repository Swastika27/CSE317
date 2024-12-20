#include "point.h"
#include <cmath>

point::point()
{
    this->x = -1;
    this->y = -1;
}

point::point(double x, double y)
{
    this->x = x;
    this->y = y;
}

double point::get_x()
{
    return x;
}

double point::get_y()
{
    return y;
}

double point::get_distance(point p)
{
    return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
}
