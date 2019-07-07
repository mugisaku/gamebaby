#include"libge/color_handler.hpp"




namespace ge{




constexpr int  rect_size = 16;




color_holder::
color_holder(int  size, gbstd::color&  color) noexcept:
m_colors(size),
m_color(color)
{
  set_content_width(rect_size+(gbstd::g_font_width*3));
  set_content_height(gbstd::g_font_height*m_colors.size());

    for(auto&  e: m_colors)
    {
      e = gbstd::color(0,0,0);
    }
}


color_holder::
color_holder(std::initializer_list<gbstd::color>  colors, gbstd::color&  color) noexcept:
m_colors(colors),
m_color(color)
{
  set_content_width(rect_size+(gbstd::g_font_width*3));
  set_content_height(gbstd::g_font_height*m_colors.size());
}




color_holder&
color_holder::
set_color(gbstd::color  col) noexcept
{
  m_colors[m_index] = col;

  request_redraw();

  return *this;
}


void
color_holder::
render(const gbstd::canvas&  cv) noexcept
{
  cv.fill(get_style().get_background_color());

  int  y = 0;

    for(auto&  color: m_colors)
    {
      cv.draw_rectangle(gbstd::colors::white,0,y  ,rect_size  ,rect_size  );
      cv.fill_rectangle(color               ,1,y+1,rect_size-2,rect_size-2);


      gbstd::string_form  sf("%d%d%d",color.get_r7(),color.get_g7(),color.get_b7());

      cv.draw_string(gbstd::colors::white,sf(),rect_size,y);

      y += rect_size;
    }



  cv.draw_rectangle(gbstd::colors::white,0,rect_size*m_index,get_content_width(),rect_size);
}


void
color_holder::
on_mouse_act(gbstd::point  mouse_pos) noexcept
{
    if(gbstd::g_input.test_mouse_left())
    {
      m_index = mouse_pos.y/rect_size;

      m_color = m_colors[m_index];

      get_userdata<color_maker>().update();

      request_redraw();
    }
}




}




