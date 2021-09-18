#pragma once

#include <vector>
#include <point.h>

class Mesh {
public:
  std::vector<uint32_t> _indices;
  std::vector<Point> _vertices;
};
