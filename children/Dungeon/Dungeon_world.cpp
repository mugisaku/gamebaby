#include"children/Dungeon/Dungeon.hpp"




namespace Dungeon{




void
world::
wait_input(gbstd::execution&  exec) noexcept
{
  auto&   pt = m_venturer.m_point;
  auto&  dir = m_venturer.m_direction;

    if(gbstd::get_keys().test_acr())
    {
      gbstd::barrier_keys();

      int  x = pt.x;
      int  y = pt.y;

           if(dir.is_x_negative()){--x;}
      else if(dir.is_x_positive()){++x;}
      else if(dir.is_y_negative()){--y;}
      else if(dir.is_y_positive()){++y;}

        if((x >= 0) &&
           (y >= 0) &&
           (x < (structure::m_width -1)) &&
           (y < (structure::m_height-1)))
        {
          m_structure.m_table[y][x].m_block = true;
        }
    }

  else
    if(gbstd::get_keys().test_diu())
    {
      gbstd::barrier_keys();

        if(dir.is_x_negative())
        {
            if(pt.x){--pt.x;}
        }

      else
        if(dir.is_x_positive())
        {
            if(pt.x < (structure::m_width-1))
            {
              ++pt.x;
            }
        }

      else
        if(dir.is_y_negative())
        {
            if(pt.y){--pt.y;}
        }

      else
        if(dir.is_y_positive())
        {
            if(pt.y < (structure::m_height-1))
            {
              ++pt.y;
            }
        }
    }

  else
    if(gbstd::get_keys().test_dil())
    {
      gbstd::barrier_keys();

      dir += directions::left;
    }

  else
    if(gbstd::get_keys().test_dir())
    {
      gbstd::barrier_keys();

      dir += directions::right;
    }

  else
    if(gbstd::get_keys().test_did())
    {
      gbstd::barrier_keys();

      dir += directions::back;
    }


  exec.interrupt();
}


void
world::
initialize(gbstd::execution&  exec) noexcept
{
  gbstd::get_root_directory()["/video/sprites/Dungeonn.spr00"]
    .be_sprite()
    .set_callback({*this,&world::draw})
  ;

  exec.replace({*this,&world::wait_input});
}


void
world::
draw_around(gbstd::point  base_pt, absolute_direction  dir, const gbstd::canvas&  cv) noexcept
{
    for(int  y = 0;  y < 3;  ++y){
    for(int  x = 0;  x < 3;  ++x){
      int  xx = base_pt.x+-1+x;
      int  yy = base_pt.y+-1+y;

        if((xx >= 0) && (xx < structure::m_width ) &&
           (yy >= 0) && (yy < structure::m_height))
        {
          auto&  sp = m_structure.m_table[yy][xx];

            if(sp.m_block)
            {
              cv.fill_rectangle(gbstd::colors::white,24*x,24*y,24,24);
            }
        }           
    }}


  int  src_x = ((dir.is_y_positive() || dir.is_y_negative() )? 4:5)*24;
  int  src_y =  (dir.is_y_positive() || dir.is_x_positive())? 0:24;

  cv.draw_canvas({gbstd::g_misc_image,src_x,src_y,24,24},24,24);
}


namespace{
const uint8_t
g_wall_image_bin[] =
{
#include"walls.txt"
};

gbstd::image
g_wall_image(g_wall_image_bin);
}


bool
world::
is_block(gbstd::point  pt) const noexcept
{
  constexpr gbstd::rectangle  rect(0,0,structure::m_width,structure::m_height);

  return (rect.test_point(pt) && m_structure.m_table[pt.y][pt.x].m_block);
}


void
world::
draw_walls(gbstd::point  base_pt, absolute_direction  dir, const gbstd::canvas&  cv) noexcept
{
  auto  lpt = base_pt+transform({ 1,2},dir);
  auto  cpt = base_pt+transform({ 0,2},dir);
  auto  rpt = base_pt+transform({-1,2},dir);

    if(is_block(lpt)){cv.draw_canvas(          {g_wall_image,   0,24*8,24*2,24*4},   0,0);}
    if(is_block(rpt)){cv.draw_canvas_reversely({g_wall_image,   0,24*8,24*2,24*4},24*2,0);}
    if(is_block(cpt)){cv.draw_canvas(          {g_wall_image,24*2,24*8,24*2,24*4},24*1,0);}


  lpt += transform({0,-1},dir);
  cpt += transform({0,-1},dir);
  rpt += transform({0,-1},dir);

    if(is_block(lpt)){cv.draw_canvas(          {g_wall_image,   0,24*4,24*2,24*4},   0,0);}
    if(is_block(rpt)){cv.draw_canvas_reversely({g_wall_image,   0,24*4,24*2,24*4},24*2,0);}
    if(is_block(cpt)){cv.draw_canvas(          {g_wall_image,24*2,24*4,24*2,24*4},24*1,0);}


  lpt += transform({0,-1},dir);
  rpt += transform({0,-1},dir);

    if(is_block(lpt)){cv.draw_canvas(          {g_wall_image,24*6,0,24*2,24*4},   0,0);}
    if(is_block(rpt)){cv.draw_canvas_reversely({g_wall_image,24*6,0,24*2,24*4},24*2,0);}
}


void
world::
draw(const gbstd::canvas&  cv) noexcept
{
  gbstd::string_form  sf;

  auto&  pt = m_venturer.m_point;
  auto  dir = m_venturer.m_direction;

  cv.draw_string(gbstd::colors::white,sf("x: %2d, y: %2d",pt.x,pt.y),0,24*8);

  cv.draw_canvas({g_wall_image,0,0,24*4,24*4},0,0);

  draw_around(pt,dir,{cv,0,24*8+16,24*3,24*3});
  draw_walls(pt,dir,{cv});
}


game_information
world::
get_information() noexcept
{
  game_information  inf;

  inf.screen_width  = 320;
  inf.screen_height = 320;

  inf.title = "おやめください　ダンナさま";
  inf.category_name = "3Dダンジョン";
  inf.description =
 R"(だいがくせいのタケトは　しんじていた　ゆうじんに　たがくの　しゃっきんを　せおわされてしまう
しついの　なか　へんさいの　しゅだんを　さがす　タケトのまえに　「あなたを　やといたい　という　ひとがいる」　と　はかくの　ほうしゅうを　ていじする　おとこが　あらわれる
あやしいと　おもいながらも　おとこに　つれられて　いったさきで　タケトは　おのれの　なかに　ねむっていた　おぞましい　よくぼうを　しることに　なるのだった
)";

  inf.boot = [](){
    gbstd::push_execution({*new world,&world::initialize});
  };


  return inf;
}




}




