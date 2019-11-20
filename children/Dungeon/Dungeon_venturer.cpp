#include"children/Dungeon/Dungeon.hpp"




namespace Dungeon{




bool
venturer::
test_advance() const noexcept
{
  auto&  x = m_point.x;
  auto&  y = m_point.y;

  auto  w = floor::m_width;
  auto  h = floor::m_height;

  auto&  dir = m_direction;
  auto&   fl = *m_floor;

       if(dir.is_x_negative()){return (x >= (  2)) && fl.test_passability({x-1,y  }) && fl.test_passability({x-2,y  });}
  else if(dir.is_x_positive()){return (x <  (w-2)) && fl.test_passability({x+1,y  }) && fl.test_passability({x+2,y  });}
  else if(dir.is_y_negative()){return (y >= (  2)) && fl.test_passability({x  ,y-1}) && fl.test_passability({x  ,y-2});}
  else if(dir.is_y_positive()){return (y <  (h-2)) && fl.test_passability({x  ,y+1}) && fl.test_passability({x  ,y+2});}


  return false;
}


void
venturer::
advance() noexcept
{
  auto  dir = m_direction;

       if(dir.is_x_negative()){m_point.x -= 2;}
  else if(dir.is_x_positive()){m_point.x += 2;}
  else if(dir.is_y_negative()){m_point.y -= 2;}
  else if(dir.is_y_positive()){m_point.y += 2;}
}


void
venturer::
draw_around(const gbstd::canvas&  cv) noexcept
{
    for(int  y = 0;  y < 6;  ++y){
    for(int  x = 0;  x < 6;  ++x){
      int  xx = m_point.x+-3+x;
      int  yy = m_point.y+-3+y;

        if((xx >= 0) && (xx < floor::m_width ) &&
           (yy >= 0) && (yy < floor::m_height))
        {
          auto&  nd = m_floor->m_table[yy][xx];

            if(nd.is_wall())
            {
              cv.fill_rectangle(gbstd::colors::white,8*x,8*y,8,8);
            }
        }           
    }}


  auto&  dir = m_direction;

  int  src_x = ((dir.is_y_positive() || dir.is_y_negative() )? 0:8)+6*24;
  int  src_y =  (dir.is_y_positive() || dir.is_x_positive())?  0:8;

  cv.draw_canvas({gbstd::g_misc_image,src_x,src_y,8,8},8*3,8*3);
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




