#pragma once

class Color
{
public:
  float r, g, b, a;

  Color()
  {
    r = 0;  g = 0;  b = 0; a = 0;
  }

  Color(uint32_t R, uint32_t G, uint32_t B, uint32_t A)
  {
    r = R;  g = G;  b = B; a = A;
  }
};
