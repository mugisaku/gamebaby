#include"children/Dungeon/Dungeon.hpp"




namespace Dungeon{




void
world::
event_nop(gbstd::execution&  exec) noexcept
{
}


void
world::
finish_event(gbstd::execution&  exec) noexcept
{
  exec.pop();
}


void
world::
event_text1(gbstd::execution&  exec) noexcept
{
  push_text(u"てきすとてすと１");

  start_stream_text(exec);
}


void
world::
event_text2(gbstd::execution&  exec) noexcept
{
  push_text(u"てきすとてすと２");

  start_stream_text(exec);
}




void
world::
start_stream_text(gbstd::execution&  exec) noexcept
{
  m_text_sprite->unset_ignore_flag();

  exec.replace({*this,&world::stream_text});
}


void
world::
stream_text(gbstd::execution&  exec) noexcept
{
    if(m_character_buffer.size())
    {
        if(m_rapid_stream || m_text_alarm)
        {
          m_text_alarm.reset();

          auto  c = m_character_buffer.front();

          auto  res = m_text.push(c);

            if(res)
            {
              m_character_buffer.pop_front();
            }

          else
            if(gbstd::get_keys().test_acr())
            {
              m_text.rotate();
            }
        }
    }

  else
    if(!m_stream_finished)
    {
      m_stream_finished = true;

        if(gbstd::get_keys().test_acr())
        {
          gbstd::barrier_keys();
        }
    }

  else
    if(gbstd::get_keys().test_acr())
    {
      gbstd::barrier_keys();

      m_text_sprite->set_ignore_flag();

      exec.pop();
    }


  m_rapid_stream = gbstd::get_keys().test_acr();


  exec.interrupt();
}


void
world::
wait_input(gbstd::execution&  exec) noexcept
{
  auto   prev_pt = m_venturer.m_point;
  auto  prev_dir = m_venturer.m_direction;

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
           (x < (floor::m_width -1)) &&
           (y < (floor::m_height-1)))
        {
          m_venturer.m_floor->m_table[y][x].m_block = true;
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
            if(pt.x < (floor::m_width-1))
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
            if(pt.y < (floor::m_height-1))
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


    if((prev_pt  !=  pt) ||
       (prev_dir != dir))
    {
      auto&  sp = m_venturer.m_floor->m_table[pt.y][pt.x];

      drive_event(sp.m_event_index,exec);
    }

  else
    {
      exec.interrupt();
    }
}


void
world::
initialize(gbstd::execution&  exec) noexcept
{
  m_base_sprite = gbstd::get_root_directory()["/video/sprites/Dungeon.spr00"];
  m_text_sprite = gbstd::get_root_directory()["/video/sprites/Dungeon.spr01"];

  m_base_sprite->be_sprite()
    .set_callback({*this,&world::draw_base})
  ;

  m_text_sprite->be_sprite()
    .set_callback({*this,&world::draw_text})
  ;

  m_structure.reset();
  m_structure.m_name = "はじまりのまち";
  m_venturer.m_floor = m_structure.m_floors;

  m_text.resize(10,3);

  m_text_alarm.assign(gbstd::get_root_directory()["/clocks/Dungeon.clk00"].be_clock()).set_interval(100);

  m_event_table = {
    event("なにも　なし",&world::event_nop),
    event("テキスト１",&world::event_text1),
    event("テキスト２",&world::event_text2),
  };

  m_venturer.get_current_floor().get_space({0,1}).m_event_index = 1;
  m_venturer.get_current_floor().get_space({0,2}).m_event_index = 2;

  exec.replace({*this,&world::wait_input});
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


void
world::
push_text(std::string  sv) noexcept
{
  auto  u16s = gbstd::make_u16string(sv);

  push_text(u16s);
}


void
world::
push_text(std::u16string  sv) noexcept
{
    for(auto  c: sv)
    {
      m_character_buffer.emplace_back(c);
    }


  m_stream_finished = false;
}


bool
world::
is_block(gbstd::point  pt) const noexcept
{
  constexpr gbstd::rectangle  rect(0,0,floor::m_width,floor::m_height);

  return (rect.test_point(pt) && m_venturer.m_floor->m_table[pt.y][pt.x].m_block);
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

  gbstd::point  text_pt;

    for(auto&  sv: ls)
    {
      cv.draw_string(gbstd::colors::white,sv,text_pt.move_x(0),text_pt.move_y(gbstd::g_font_height));
    }
}


game_information
world::
get_information() noexcept
{
  game_information  inf;

  inf.screen_width  = 320;
  inf.screen_height = 320;

  inf.title = "ディープなダンジョン";
  inf.category_name = "3Dダンジョン";
  inf.description = R"()";

  inf.boot = [](){
    gbstd::push_execution({*new world,&world::initialize});
  };


  return inf;
}




}




