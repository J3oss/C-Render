#include <point.h>

Point Point::operator+ (Point p) {
  Point S;
  S.x = x + p.x;
  S.y = y + p.y;
  S.z = z + p.z;
  return S;
}

Point Point::operator- (Point p) {
  Point D;
  D.x = x - p.x;
  D.y = y - p.y;
  D.z = z - p.z;
  return D;
}

void Point::Swap(Point &p1, Point &p2)
{
  int32_t temp = p1.x;
  p1.x = p2.x;
  p2.x = temp;

  temp = p1.y;
  p1.y = p2.y;
  p2.y = temp;
}
