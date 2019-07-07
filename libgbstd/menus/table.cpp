#include"libgbstd/menu.hpp"




namespace gbstd{
namespace menus{




table&
table::
resize(int  w, int  h) noexcept
{
  m_width  = w;
  m_height = h;

  m_container.resize(w*h);

  int  i = 0;

  point  pos;

    for(auto&  cel: m_container)
    {
      cel.set_index(i++).set_position(pos);

        if(++pos.x >= m_width)
        {
          pos.x  = 0;
          pos.y += 1;
        }
    }


  return *this;
}


void
table::
draw(int  x, int  y, int  w, int  h, const gbstd::canvas&  cv) noexcept
{
    for(int  yy = 0;  yy < h;  ++yy){
    for(int  xx = 0;  xx < w;  ++xx){
      auto&  cel = *get_cell_pointer(x+xx,
                                     y+yy);

      gbstd::canvas  dst_cv(cv,m_cell_width *xx,
                               m_cell_height*yy,
                               m_cell_width,
                               m_cell_height);

      m_callback(cel,dst_cv);
    }}
}




}}




