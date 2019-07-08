#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




color
label::
m_default_color = colors::white;




label::
label(operating_node&  root) noexcept
{
  m_style.set_left_padding(4)
         .set_right_padding(4)
         .set_top_padding(4)
         .set_bottom_padding(4);
}




label&
label::
set_string(std::string_view  sv) noexcept
{
  set_string_without_reform(sv);

  set_content_width( g_font_width*m_string.size());
  set_content_height(g_font_height               );

  request_reform();

  return *this;
}


label&
label::
set_string(std::u16string_view  sv) noexcept
{
  set_string_without_reform(sv);

  set_content_width( g_font_width*m_string.size());
  set_content_height(g_font_height               );

  request_reform();

  return *this;
}


label&
label::
set_string_without_reform(std::string_view  sv) noexcept
{
  m_string = make_u16string(sv);

  request_redraw();

  return *this;
}


label&
label::
set_string_without_reform(std::u16string_view  sv) noexcept
{
  m_string = sv;

  request_redraw();

  return *this;
}


label&
label::
set_color(color  c) noexcept
{
  m_color = c;

  request_redraw();

  return *this;
}


void
label::
render(const canvas&  cv) noexcept
{
  cv.fill(m_style.get_background_color());

  cv.draw_string(m_color,m_string,0,0);
}




}}




