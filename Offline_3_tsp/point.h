#ifndef POINT_H
#define POINT_H

class point
{
private:
    double x;
    double y;

public:
    point();
    point(double, double);
    double get_x();
    double get_y();
    double get_distance(point);
};

#endif