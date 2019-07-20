#include"libww/ww_string_display.hpp"




namespace ww{




display_string&
display_string::
set_color(gbstd::color  color) noexcept
{
  m_color = color;

  return *this;
}


display_string&
display_string::
set_string(std::u16string_view  sv) noexcept
{
  char16_t*  dst     = m_buffer;
  char16_t*  dst_end = m_buffer+(m_buffer_length-1);

  const char16_t*  src     = sv.data();
  const char16_t*  src_end = sv.data()+sv.size();

  m_length = 0;

    while((dst != dst_end) &&
          (src != src_end))
    {
      *dst++ = *src++;

      ++m_length;
    }


  *dst = 0;

  return *this;
}




void
string_display::
draw(const gbstd::canvas&  cv, string_display&  strdsp) noexcept
{
    if(strdsp.m_pointer)
    {
      auto  color = strdsp.m_pointer->get_color();

      auto  sv = strdsp.m_pointer->get_string_view();

        if(strdsp.m_align == align::left)
        {
          cv.draw_string_safely(color,sv,strdsp.m_pos.x,
                                         strdsp.m_pos.y);
        }

      else
        {
          cv.draw_string_safely_as_right_align(color,sv,strdsp.m_pos.x,
                                                        strdsp.m_pos.y);
        }
    }
}




}




