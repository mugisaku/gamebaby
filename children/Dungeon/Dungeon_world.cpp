#include"children/Dungeon/Dungeon.hpp"




namespace Dungeon{




void
world::
display_text1() noexcept
{
  push_text(u"バーニラ　バニラ　バーニラ　きゅうじん　バーニラ　バニラで　アルバイトー");

  m_text_sprite->unset_ignore_flag();
}


void
world::
undisplay_text() noexcept
{
  m_text.erase();

  m_text_sprite->set_ignore_flag();
}


void
world::
display_text2() noexcept
{
//  push_text(u"てきすとてすと２");

//  start_stream_text(exec);
}


bool
world::
stream_text() noexcept
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

      return false;
    }


  m_rapid_stream = gbstd::get_keys().test_acr();

  return true;
}


void
world::
execute_program(gbstd::execution&  exec) noexcept
{
    if(m_proex)
    {
      m_proex.step(*this);

      exec.interrupt();
    }

  else
    {
      exec.pop();
    }
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
          auto&  sp = m_venturer.m_floor->m_table[y][x];

          auto&  blk = sp.m_block;

            if(!blk)
            {
              blk = true;
            }

          else
            {
              sp.set_way_flag(dir+directions::front);
            }
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

        if(sp.m_event_code.size())
        {
          m_prosp.clear_image();

          m_prosp.load_source(sp.m_event_code);

          m_proex = m_prosp;

          exec.push({*this,&world::execute_program});
        }
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
/*
  m_venturer.get_current_floor().get_space({0,1}).m_event_code =
R"(
  display_text1
loop:
 *stream_text,loop.
  undisplay_text
)";

  m_venturer.get_current_floor().get_space({0,2}).m_event_code =
R"(
display_text2
)";

  m_prosp.add_condition({"stream_text",&world::stream_text});
  m_prosp.add_body({"undisplay_text",&world::undisplay_text});
  m_prosp.add_body({"display_text1",&world::display_text1});
  m_prosp.add_body({"display_text2",&world::display_text2});
*/

  exec.replace({*this,&world::wait_input});
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


bool
world::
is_way(gbstd::point  pt, absolute_direction  dir) const noexcept
{
  return m_venturer.m_floor->m_table[pt.y][pt.x].test_way_flag(dir);
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




