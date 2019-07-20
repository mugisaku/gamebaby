#include"libww/ww_spilling_text.hpp"
#include"libww/ww_system.hpp"




namespace ww{




spilling_text::
spilling_text(system&  sys) noexcept:
m_system(sys)
{
}




spilling_text&
spilling_text::
reset(gbstd::clock&  clock, gbstd::point  bottom_pos, int  offset) noexcept
{
  live();
  show();
  set_interval(20);
  set_clock_watch(clock);
  update_next_time();


  m_bottom_pos =   bottom_pos;
  m_pos        = m_bottom_pos;

  m_pos.y += offset;

  m_y_vector = -8;


  return *this;
}


void
spilling_text::
work() noexcept
{
  m_pos.y += m_y_vector     ;
             m_y_vector += 2;

    if(m_pos.y >= m_bottom_pos.y+16)
    {
      m_y_vector /= 4;

        if(m_y_vector > 1)
        {
          m_y_vector = -m_y_vector;
        }

      else
        {
          sleep(m_time);
          die_when_getup();
          discard_when_die();
        }
    }
}


void
spilling_text::
render(const gbstd::canvas&  cv) noexcept
{
  cv.draw_string_safely(m_color,m_text.data(),m_pos.x,m_pos.y);
}


void
spilling_text::
finish() noexcept
{
  m_system.collect_spilling_text(*this);
}




}




