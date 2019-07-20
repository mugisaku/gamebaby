#include"mkfont.hpp"




character_selector::
character_selector(editor&  e) noexcept:
m_editor(e)
{
  set_content_width( m_pixel_size*character::m_size*m_x_number);
  set_content_height(m_pixel_size*character::m_size*m_y_number);
}


void
character_selector::
on_mouse_act(point  pt) noexcept
{
  int  x = pt.x/(m_pixel_size*character::m_size);
  int  y = pt.y/(m_pixel_size*character::m_size);

  m_cursor_position.x = x;
  m_cursor_position.y = y;

  auto&  c  = g_characters[m_x_number*(m_offset+m_cursor_position.y)+m_cursor_position.x];

  string_form  sf;

  sf("U+%04X",c.m_unicode);

    if(g_input.test_mouse_left())
    {
      g_current_character = std::ref(c);

      m_editor.m_bitmap.request_redraw();

      m_editor.m_selected_label.set_string_without_reform(sf());
    }


  m_editor.m_character_label.set_string_without_reform(sf());

  request_redraw();
}


void
character_selector::
render(const canvas&  cv) noexcept
{
  cv.fill(m_style.get_background_color());

  auto  it = &g_characters[m_x_number*m_offset];

    for(int  y = 0;  y < m_y_number;  ++y){
    for(int  x = 0;  x < m_x_number;  ++x){

        if(it > &g_characters.back())
        {
          goto NEXT;
        }


      auto&  c = *it++;

      canvas  tmp_cv(cv,(m_pixel_size*character::m_size)*x,
                        (m_pixel_size*character::m_size)*y,
                        (m_pixel_size*character::m_size),
                        (m_pixel_size*character::m_size));

      c.render(tmp_cv,m_pixel_size);
    }}


NEXT:
  cv.draw_double_rectangle(colors::white,colors::black,
    m_cursor_position.x*m_pixel_size*character::m_size,
    m_cursor_position.y*m_pixel_size*character::m_size,
    m_pixel_size*character::m_size,
    m_pixel_size*character::m_size);
}





