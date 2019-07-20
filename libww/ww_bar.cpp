#include"libww/ww_bar.hpp"




namespace ww{




void
bar::
update_increment() noexcept
{
  auto  diff = m_length-m_desired_length;

  m_increment = diff.abs()/64;

    if(m_length > m_desired_length)
    {
      m_increment = -m_increment;
    }
}




void
bar::
draw(const gbstd::canvas&  cv, bar&  b) noexcept
{
  auto&  l = b.m_length;

    if(l)
    {
      int  x = (b.is_left_to_right()? b.m_pos.x:b.m_pos.x-l);

      cv.fill_rectangle(b.m_color,x,b.m_pos.y,l,b.m_thickness);
    }
}


void
bar::
tick(bar&  b) noexcept
{
    if(!b.is_finished())
    {
      b.m_length += b.m_increment;

        if(b.m_increment < 0)
        {
            if(b.m_length < b.m_desired_length)
            {
              b.m_length = b.m_desired_length;
            }
        }

      else
        if(b.m_increment > 0)
        {
            if(b.m_length > b.m_desired_length)
            {
              b.m_length = b.m_desired_length;
            }
        }
    }
}




}




