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
draw_walls1(gbstd::point  base_pt, absolute_direction  dir, const gbstd::canvas&  cv) noexcept
{
  int  w =  8*3;
  int  h = 48*2;

  auto&  lnd = m_venturer.get_current_floor().get_node(base_pt+transform({ 1,0},dir));
  auto&  rnd = m_venturer.get_current_floor().get_node(base_pt+transform({-1,0},dir));
/*
+++++++
+ |@| +
+ + + +
+     +
+ + + +
+     +
+++++++
*/
       if(lnd.is_wall()      ){cv.draw_canvas({g_wall_image,48*3,0,w,h},0,0);}
  else if(lnd.is_holed_wall()){cv.draw_canvas({g_wall_image,48*4,0,w,h},0,0);}


       if(rnd.is_holed_wall()){cv.draw_canvas({g_wall_image,48*4+w,0,w,h},48+w,0);}
  else if(rnd.is_wall()      ){cv.draw_canvas({g_wall_image,48*3+w,0,w,h},48+w,0);}
}


void
world::
draw_walls2(gbstd::point  base_pt, absolute_direction  dir, const gbstd::canvas&  cv) noexcept
{
  int  y = 48*2;
  int  w =  8*2;
  int  h = 48*2;

  auto&  lfnd = m_venturer.get_current_floor().get_node(base_pt+transform({ 2,1},dir));
  auto&   cnd = m_venturer.get_current_floor().get_node(base_pt+transform({ 0,1},dir));
  auto&  rfnd = m_venturer.get_current_floor().get_node(base_pt+transform({-2,1},dir));
  auto&  lsnd = m_venturer.get_current_floor().get_node(base_pt+transform({ 1,2},dir));
  auto&  rsnd = m_venturer.get_current_floor().get_node(base_pt+transform({-1,2},dir));
/*
+++++++
+  @  +
+-+-+-+
+ | | +
+ + + +
+     +
+++++++
*/
    
       if(lfnd.is_holed_wall()){cv.draw_canvas({g_wall_image,48+24,y,24,h},0,0);}
  else if(lfnd.is_wall())      {cv.draw_canvas({g_wall_image,   24,y,24,h},0,0);}

       if(lsnd.is_holed_wall()){cv.draw_canvas({g_wall_image,48*3,y,w,h},w+8,0);}
  else if(lsnd.is_wall()      ){cv.draw_canvas({g_wall_image,48*2,y,w,h},w+8,0);}

       if(rfnd.is_holed_wall()){cv.draw_canvas({g_wall_image,48,y,24,h},48+24,0);}
  else if(rfnd.is_wall()      ){cv.draw_canvas({g_wall_image, 0,y,24,h},48+24,0);}

       if(rsnd.is_holed_wall()){cv.draw_canvas({g_wall_image,48*3+8*4,y,w,h},48+w-8,0);}
  else if(rsnd.is_wall()      ){cv.draw_canvas({g_wall_image,48*2+8*4,y,w,h},48+w-8,0);}

       if(cnd.is_holed_wall()){cv.draw_canvas({g_wall_image,48,y,48,h},w+8,0);}
  else if(cnd.is_wall()      ){cv.draw_canvas({g_wall_image, 0,y,48,h},w+8,0);}
}


void
world::
draw_walls3(gbstd::point  base_pt, absolute_direction  dir, const gbstd::canvas&  cv) noexcept
{
  int  y = 48*2;
  int  w =  8*2;
  int  h = 48*2;

  auto&  lfnd = m_venturer.get_current_floor().get_node(base_pt+transform({ 2,1},dir));
  auto&   cnd = m_venturer.get_current_floor().get_node(base_pt+transform({ 0,1},dir));
  auto&  rfnd = m_venturer.get_current_floor().get_node(base_pt+transform({-2,1},dir));
  auto&  lsnd = m_venturer.get_current_floor().get_node(base_pt+transform({ 1,2},dir));
  auto&  rsnd = m_venturer.get_current_floor().get_node(base_pt+transform({-1,2},dir));
/*
+++++++++++
+    @    +
+ + + + + +
+         +
+-+-+-+-+-+
+ | | | | +
+++++++++++
*/
/*
    if(lfnd.is_wall())
    {
        if(lfnd.has_wayhole()){cv.draw_canvas({g_wall_image,48+24,y,24,h},0,0);}
      else                    {cv.draw_canvas({g_wall_image,   24,y,24,h},0,0);}
    }


    if(lsnd.is_wall())
    {
        if(lsnd.has_wayhole()){cv.draw_canvas({g_wall_image,48*3,y,w,h},w+8,0);}
      else                    {cv.draw_canvas({g_wall_image,48*2,y,w,h},w+8,0);}
    }


    if(rfnd.is_wall())
    {
        if(rfnd.has_wayhole()){cv.draw_canvas({g_wall_image,48,y,24,h},48+24,0);}
      else                    {cv.draw_canvas({g_wall_image, 0,y,24,h},48+24,0);}
    }


    if(rsnd.is_wall())
    {
        if(rsnd.has_wayhole()){cv.draw_canvas({g_wall_image,48*3+8*4,y,w,h},48+w-8,0);}
      else                    {cv.draw_canvas({g_wall_image,48*2+8*4,y,w,h},48+w-8,0);}
    }


    if(cnd.is_wall())
    {
        if(cnd.has_wayhole()){cv.draw_canvas({g_wall_image,48,y,48,h},w+8,0);}
      else                   {cv.draw_canvas({g_wall_image, 0,y,48,h},w+8,0);}
    }
*/
}


void
world::
draw_base(const gbstd::canvas&  cv) noexcept
{
  gbstd::string_form  sf;

  auto   pt = m_venturer.get_point();
  auto  dir = m_venturer.get_direction();

  cv.draw_string(gbstd::colors::white,sf("x: %2d, y: %2d",pt.x,pt.y),0,24*8);

  cv.draw_canvas({g_wall_image,0,0,24*4,24*4},0,0);

  m_venturer.draw_status({cv,24*4,      0,gbstd::g_font_width*8,gbstd::g_font_height*8});
  m_venturer.draw_around({cv,   0,24*8+16,8*7,8*7});

//  draw_walls3(pt,dir,{cv});
  draw_walls2(pt,dir,{cv});
  draw_walls1(pt,dir,{cv});

  auto  faname = m_venturer.get_facility_name();

    if(faname.size())
    {
      cv.draw_string(gbstd::colors::white,faname,24*2-gbstd::g_font_width*3/2,24*2);
    }
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


void
world::
draw_creation_menu(const gbstd::canvas&  cv) noexcept
{
  auto  cont_pt = m_menu.m_window.get_content_position();

  auto  cont_cv = m_menu.m_window.render(cv);

  int  y = 0;

    for(auto&  item: m_menu.m_items)
    {
      cont_cv.draw_string(gbstd::colors::white,item.first,0,y);

      y += gbstd::g_font_height;
    }


  cv.draw_canvas({gbstd::g_misc_image,0,0,24,24},cont_pt.x-24,cont_pt.y+4+gbstd::g_font_height*m_menu.m_index);
}




}




