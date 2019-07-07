#include"libgbstd/gadget.hpp"




namespace gbstd{




tile_map&
tile_map::
resize(int  w, int  h) noexcept
{
  m_width  = w;
  m_height = h;

  m_table.resize(w*h);

    for(auto&  i: m_table)
    {
      i = 0;
    }


  return *this;
}




}




