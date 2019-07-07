#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




constexpr int  margin = 16;




color
frame::
m_default_color(colors::white);


color
frame::
m_default_line_color = colors::yellow;




frame::
frame(operating_node&  root) noexcept
{
  set_minimal_content_height(24);
}




frame&
frame::
set_content(node&  nd) noexcept
{
  add_child(nd,{4,24});

  return *this;
}


frame&
frame::
set_string(std::string_view  sv) noexcept
{
  m_string = make_u16string(sv);

  set_minimal_content_width(g_font_width*(m_string.size()+2));

  request_reform();

  return *this;
}


frame&
frame::
set_string(std::u16string_view  sv) noexcept
{
  m_string = sv;

  set_minimal_content_width(g_font_width*(m_string.size()+2));

  request_reform();

  return *this;
}


frame&
frame::
set_line_color(color  new_color) noexcept
{
  m_line_color = new_color;

  request_redraw();

  return *this;
}


void
frame::
render(const canvas&  cv) noexcept
{
  cv.draw_rectangle(m_line_color,0,0,get_content_width(),get_content_height());
  cv.draw_string(m_color,m_string,4,4);
}




}}




