#include"libgbstd/image.hpp"
#include<algorithm>



namespace gbstd{


void
liner::
reset(int  x0, int  y0, int  x1, int  y1) noexcept
{
  constexpr int  base = (1<<shift_amount)/2;

  auto  x_min = std::min(x0,x1);
  auto  x_max = std::max(x0,x1);
  auto  y_min = std::min(y0,y1);
  auto  y_max = std::max(y0,y1);

  int  x_distance = x_max-x_min;
  int  y_distance = y_max-y_min;

  m_x = base+(x0<<shift_amount);
  m_y = base+(y0<<shift_amount);

    if(!x_distance)
    {
      m_x_add_amount = (0              );
      m_y_add_amount = (1<<shift_amount);

      m_distance = y_distance;
    }

  else
    if(!y_distance)
    {
      m_x_add_amount = (1<<shift_amount);
      m_y_add_amount = (0              );

      m_distance = x_distance;
    }

  else
    if(x_distance < y_distance)
    {
      m_x_add_amount = (x_distance<<shift_amount)/y_distance;
      m_y_add_amount = (         1<<shift_amount);

      m_distance = y_distance;
    }

  else
    if(x_distance > y_distance)
    {
      m_x_add_amount = (         1<<shift_amount);
      m_y_add_amount = (y_distance<<shift_amount)/x_distance;

      m_distance = x_distance;
    }

  else
    {
      m_x_add_amount = (1<<shift_amount);
      m_y_add_amount = (1<<shift_amount);

      m_distance = x_distance;
    }


    if(x0 > x1){m_x_add_amount *= -1;}
    if(y0 > y1){m_y_add_amount *= -1;}
}




void
liner::
step() noexcept
{
  m_x += m_x_add_amount;
  m_y += m_y_add_amount;

  --m_distance;
}


}





