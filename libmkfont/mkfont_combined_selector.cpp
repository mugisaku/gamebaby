#include"mkfont.hpp"




combined_selector::
combined_selector(editor&  e) noexcept:
m_editor(e)
{
  set_content_width( m_pixel_size*character::m_size  *m_x_number);
  set_content_height(m_pixel_size*character::m_size*2*m_y_number);
}


void
combined_selector::
render(const canvas&  cv) noexcept
{
  cv.fill(m_style.get_background_color());

  auto  it = &g_combineds[m_x_number*m_offset];

    for(int  y = 0;  y < m_y_number;  ++y){
    for(int  x = 0;  x < m_x_number;  ++x){

        if(it > &g_combineds.back())
        {
          return;
        }


      auto&  c = *it++;

      canvas  u_cv(cv,(m_pixel_size*character::m_size  )*x,
                      (m_pixel_size*character::m_size*2)*y,
                      (m_pixel_size*character::m_size  ),
                      (m_pixel_size*character::m_size*2));

      canvas  l_cv(cv,(m_pixel_size*character::m_size  )*x,
                      (m_pixel_size*character::m_size*2)*y+(m_pixel_size*character::m_size),
                      (m_pixel_size*character::m_size  ),
                      (m_pixel_size*character::m_size*2));

      get_character_by_unicode(c.m_upper).render(u_cv,m_pixel_size);
      get_character_by_unicode(c.m_lower).render(l_cv,m_pixel_size);
    }}
}





