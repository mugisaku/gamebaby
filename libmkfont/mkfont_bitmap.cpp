#include"mkfont.hpp"




bitmap::
bitmap(editor&  e) noexcept:
m_editor(e)
{
  set_content_width( m_pixel_size*character::m_size);
  set_content_height(m_pixel_size*character::m_size);
}


void
bitmap::
on_mouse_act(point  pt) noexcept
{
  int  x = pt.x/m_pixel_size;
  int  y = pt.y/m_pixel_size;


  m_cursor_position.x = pt.x/m_pixel_size;
  m_cursor_position.y = pt.y/m_pixel_size;

  auto&  ln = g_current_character.get().m_data[m_cursor_position.y];

  auto  flag = 0x80>>m_cursor_position.x;

       if(g_input.test_mouse_left() ){  ln |=  flag;  m_editor.m_chrsel.request_redraw();  m_editor.m_cmbsel.request_redraw();}
  else if(g_input.test_mouse_right()){  ln &= ~flag;  m_editor.m_chrsel.request_redraw();  m_editor.m_cmbsel.request_redraw();}


  request_redraw();
}


void
bitmap::
render(const canvas&  cv) noexcept
{
  g_current_character.get().render(cv,m_pixel_size);

  cv.draw_double_rectangle(colors::white,colors::black,
    m_cursor_position.x*m_pixel_size,
    m_cursor_position.y*m_pixel_size,
    m_pixel_size,
    m_pixel_size);
}





