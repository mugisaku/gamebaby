#include"children/Dungeon/Dungeon.hpp"




namespace Dungeon{




void
venturer::
draw_around(const gbstd::canvas&  cv) noexcept
{
    for(int  y = 0;  y < 3;  ++y){
    for(int  x = 0;  x < 3;  ++x){
      int  xx = m_point.x+-1+x;
      int  yy = m_point.y+-1+y;

        if((xx >= 0) && (xx < floor::m_width ) &&
           (yy >= 0) && (yy < floor::m_height))
        {
          auto&  sp = m_floor->m_table[yy][xx];

            if(sp.m_block)
            {
              cv.fill_rectangle(gbstd::colors::white,24*x,24*y,24,24);
            }
        }           
    }}


  auto&  dir = m_direction;

  int  src_x = ((dir.is_y_positive() || dir.is_y_negative() )? 4:5)*24;
  int  src_y =  (dir.is_y_positive() || dir.is_x_positive())? 0:24;

  cv.draw_canvas({gbstd::g_misc_image,src_x,src_y,24,24},24,24);
}


void
venturer::
draw_status(const gbstd::canvas&  cv) noexcept
{
  gbstd::point  pt;

  gbstd::string_form  sf;

  cv.draw_string(gbstd::colors::white,sf("%s %2dかい",m_floor->m_structure->m_name.data(),m_floor->m_number+1),pt.move_x(0),pt.move_y(gbstd::g_font_height));
  cv.draw_string(gbstd::colors::white,"ほもも",pt.move_x(0),pt.move_y(gbstd::g_font_height));
  cv.draw_string(gbstd::colors::white,sf("HP %3d/%3d",m_hp,m_hp_max),pt.move_x(0),pt.move_y(gbstd::g_font_height));
  cv.draw_string(gbstd::colors::white,sf("STR %3d",m_strength),pt.move_x(0),pt.move_y(gbstd::g_font_height));
  cv.draw_string(gbstd::colors::white,sf("DEF %3d",m_defense ),pt.move_x(0),pt.move_y(gbstd::g_font_height));
  cv.draw_string(gbstd::colors::white,sf("AGL %3d",m_agility ),pt.move_x(0),pt.move_y(gbstd::g_font_height));
  cv.draw_string(gbstd::colors::white,sf("LUK %3d",m_luck    ),pt.move_x(0),pt.move_y(gbstd::g_font_height));
}




}




