#include"children/Dungeon/Dungeon.hpp"




namespace Dungeon{




namespace{
const uint8_t
g_wall_image_bin[] =
{
#include"walls.txt"
};

gbstd::image
g_wall_image(g_wall_image_bin);
}




void
world::
draw_walls(gbstd::point  base_pt, absolute_direction  dir, const gbstd::canvas&  cv) noexcept
{
  auto  lpt = base_pt+transform({ 1,2},dir);
  auto  cpt = base_pt+transform({ 0,2},dir);
  auto  rpt = base_pt+transform({-1,2},dir);
/*
    if(is_block(lpt)){cv.draw_canvas(          {g_wall_image,   0,24*8,24*2,24*4},   0,0);}
    if(is_block(rpt)){cv.draw_canvas_reversely({g_wall_image,   0,24*8,24*2,24*4},24*2,0);}
    if(is_block(cpt)){cv.draw_canvas(          {g_wall_image,24*2,24*8,24*2,24*4},24*1,0);}
*/

  lpt += transform({0,-1},dir);
  cpt += transform({0,-1},dir);
  rpt += transform({0,-1},dir);

/*
    if(is_block(lpt)){cv.draw_canvas(          {g_wall_image,   0,24*4,24*2,24*4},   0,0);}
    if(is_block(rpt)){cv.draw_canvas_reversely({g_wall_image,   0,24*4,24*2,24*4},24*2,0);}
    if(is_block(cpt)){cv.draw_canvas(          {g_wall_image,24*2,24*4,24*2,24*4},24*1,0);}
*/

  lpt += transform({0,-1},dir);
  rpt += transform({0,-1},dir);

    if(is_block(lpt))
    {
        if(is_way(lpt,dir+directions::left)){cv.draw_canvas({g_wall_image,24*8,0,24*2,24*4},0,0);}
      else                                  {cv.draw_canvas({g_wall_image,24*6,0,24*2,24*4},0,0);}
    }


    if(is_block(rpt))
    {
        if(is_way(rpt,dir+directions::right)){cv.draw_canvas_reversely({g_wall_image,24*8,0,24*2,24*4},48,0);}
      else                                   {cv.draw_canvas_reversely({g_wall_image,24*6,0,24*2,24*4},48,0);}
    }
}


void
world::
draw_base(const gbstd::canvas&  cv) noexcept
{
  gbstd::string_form  sf;

  auto&  pt = m_venturer.m_point;
  auto  dir = m_venturer.m_direction;

  cv.draw_string(gbstd::colors::white,sf("x: %2d, y: %2d",pt.x,pt.y),0,24*8);

  cv.draw_canvas({g_wall_image,0,0,24*4,24*4},0,0);

  m_venturer.draw_status({cv,24*4,      0,gbstd::g_font_width*8,gbstd::g_font_height*8});
  m_venturer.draw_around({cv,   0,24*8+16,24*3,24*3});

  draw_walls(pt,dir,{cv});
}


void
world::
draw_text(const gbstd::canvas&  cv) noexcept
{
  auto  ls = m_text.make_line_list();

  gbstd::point  text_pt(0,120);

    for(auto&  sv: ls)
    {
      cv.draw_string(gbstd::colors::white,sv,text_pt.move_x(0),text_pt.move_y(gbstd::g_font_height));
    }
}




}




