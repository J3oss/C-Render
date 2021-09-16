#include <vector>
#include <point.h>

class Mesh {
public:
  uint32_t _index_count;
  uint32_t _vertex_count;

  std::vector<uint32_t> _indices;
  std::vector<Point> _vertices;
};
