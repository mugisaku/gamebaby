#include"children/FallBlockPazzle/FallBlockPazzle.hpp"




namespace FallBlockPazzle{




void
space::
draw(const gbstd::canvas&  cv, int  y_offset, int  state) const noexcept
{
  int  x =          (m_size*m_point.x);
  int  y = y_offset+(m_size*m_point.y);

    if(m_number && state)
    {
      cv.fill_rectangle(get_color(),x+1,y+1,m_size-2,m_size-2);

      cv.draw_rectangle((state == 2)? gbstd::colors::white:gbstd::colors::gray,x,y,m_size,m_size);

      char  buf[2] = {static_cast<char>('0'+std::abs(m_number)),0};

      cv.draw_string(gbstd::colors::white,buf,x+4,y-4);
    }

  else
    {
      cv.fill_rectangle(gbstd::colors::black,x,y,m_size,m_size);
    }
}




}




