#pragma once

#include <cstdint>

class Point
{
public:
  int32_t x, y, z;

  Point()
  {}

  Point(int32_t X, int32_t Y, int32_t Z)
  {
    x = X;  y = Y;  z = Z;
  }

  Point operator+ (Point p);

  Point operator- (Point p);

  static void Swap(Point &p1, Point &p2);
};
