#include"libgbstd/gadget.hpp"




namespace gbstd{




tile_source&
tile_source::
make(int  w, int  h, int  n) noexcept
{
  m_width  = w;
  m_height = h;

  m_number_of_tiles = n;

  m_stream.resize(w*h*n);

  return *this;
}




}




