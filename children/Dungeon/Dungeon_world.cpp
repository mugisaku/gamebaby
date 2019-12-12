#include"children/Dungeon/Dungeon.hpp"
#include"libgbstd/value.hpp"




namespace Dungeon{




namespace{
constexpr const char*
g_data_path = "dungeon_data.txt.cpp";
}




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
start_creation_menu(gbstd::execution&  exec) noexcept
{
  auto&  i = m_menu.m_index;

    if(gbstd::get_keys().test_acr())
    {
      gbstd::barrier_keys();

      m_menu();
    }

  else
    if(gbstd::get_keys().test_acd())
    {
      gbstd::barrier_keys();

      m_menu_sprite->set_ignore_flag();

      exec.pop();
    }

  else
    if(gbstd::get_keys().test_diu())
    {
      gbstd::barrier_keys();

        if(i)
        {
          --i;
        }
    }

  else
    if(gbstd::get_keys().test_did())
    {
      gbstd::barrier_keys();

        if(i < (m_menu.m_items.size()-1))
        {
          ++i;
        }
    }


  exec.interrupt();
}


void
world::
wait_input(gbstd::execution&  exec) noexcept
{
  auto   prev_pt = m_venturer.get_point();
  auto  prev_dir = m_venturer.get_direction();

    if(gbstd::get_keys().test_acr())
    {
      gbstd::barrier_keys();


      auto  pt = m_venturer.get_point();

      int&  x = pt.x;
      int&  y = pt.y;

           if(prev_dir.is_x_negative()){--x;}
      else if(prev_dir.is_x_positive()){++x;}
      else if(prev_dir.is_y_negative()){--y;}
      else if(prev_dir.is_y_positive()){++y;}

      auto&  nd = m_venturer.get_current_floor().get_node(pt);

        if(nd.is_space())
        {
          nd.be_soft_wall();
        }

      else
        if(nd.is_holed_wall())
        {
          nd.be_space();
        }

      else
        if(nd.is_soft_wall())
        {
          nd.be_holed_wall();
        }
    }

  else
    if(gbstd::get_keys().test_acd())
    {
      gbstd::barrier_keys();

      m_menu_sprite->unset_ignore_flag();

      exec.push({*this,&world::start_creation_menu});
    }

  else
    if(gbstd::get_keys().test_diu())
    {
      gbstd::barrier_keys();

        if(m_venturer.test_advance())
        {
          m_venturer.advance();
        }
    }

  else
    if(gbstd::get_keys().test_dil())
    {
      gbstd::barrier_keys();

      m_venturer += directions::left;
    }

  else
    if(gbstd::get_keys().test_dir())
    {
      gbstd::barrier_keys();

      m_venturer += directions::right;
    }

  else
    if(gbstd::get_keys().test_did())
    {
      gbstd::barrier_keys();

      m_venturer += directions::back;
    }


    if((prev_pt  != m_venturer.get_point()) ||
       (prev_dir != m_venturer.get_direction()))
    {
      auto&  nd = m_venturer.get_current_floor().get_node(m_venturer.get_point());

      auto  c = nd.get_event_code();

        if(c)
        {
/*
          m_prosp.clear_image();

          m_prosp.load_source(sp.m_event_code);

          m_proex = m_prosp;

          exec.push({*this,&world::execute_program});
*/
        }
    }

  else
    {
      exec.interrupt();
    }
}




void  world::move_to_upper_floor() noexcept{m_venturer.move_above();}
void  world::move_to_lower_floor() noexcept{m_venturer.move_below();}
void  world::put_upstairs() noexcept{m_venturer.get_current_node().be_up_way();}
void  world::put_downstairs() noexcept{m_venturer.get_current_node().be_down_way();}
void  world::unput_stairs() noexcept{m_venturer.get_current_node().be_space();}




void
world::
initialize(gbstd::execution&  exec) noexcept
{
  scan(
#include"dungeon_data.txt.cpp"
  );


  m_base_sprite = gbstd::get_root_directory()["/video/sprites/Dungeon.spr00"];
  m_text_sprite = gbstd::get_root_directory()["/video/sprites/Dungeon.spr01"];
  m_menu_sprite = gbstd::get_root_directory()["/video/sprites/Dungeon.menu.spr"];

  m_base_sprite->be_sprite()
    .set_callback({*this,&world::draw_base})
  ;

  m_text_sprite->be_sprite()
    .set_callback({*this,&world::draw_text})
  ;

  m_menu_sprite->be_sprite()
    .set_callback({*this,&world::draw_creation_menu})
  ;

  m_menu_sprite->set_ignore_flag();

  m_menu.m_window.set_x_position(200);
  m_menu.m_window.set_y_position(200);

  m_menu.reset({menu::item(u"うえへ　まいりたい",{*this,&world::move_to_upper_floor}),
                menu::item(u"したへ　まいりたい",{*this,&world::move_to_lower_floor}),
                menu::item(u"のぼりかいだんを　おくぞ",{*this,&world::put_upstairs}),
                menu::item(u"くだりかいだんを　おくそ",{*this,&world::put_downstairs}),
                menu::item(u"かいだんなど　ふようだ",{*this,&world::unput_stairs})});

  m_venturer.set_current_floor((*m_structures[0])[0]);
  m_venturer.set_point({1,1});

  m_text.resize(10,3);

  m_text_alarm.assign(gbstd::get_root_directory()["/clocks/Dungeon.clk00"].be_clock()).set_interval(100);


  auto&  fl = m_venturer.get_current_floor();

//  fl.get_node({0,1}).m_block = true;
//  fl.get_space({2,1}).m_block = true;

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


structure&
world::
push_structure() noexcept
{
  m_structures.emplace_back(std::make_unique<structure>());

  return *m_structures.back();
}




void
world::
print() const noexcept
{
}


void
world::
print(std::string&  sbuf) const noexcept
{
  sbuf += "structures:[\n";

    for(auto&  st: m_structures)
    {
      st->print(sbuf);

      sbuf += ",\n";
    }
}


void
world::
scan(std::string_view  sv) noexcept
{
  gbstd::values::array  root(sv);

  auto  st = root.find("structures");

    if(st && st->is_array())
    {
      m_structures.clear();

        for(auto&  o: st->get_array())
        {
            if(o.is_array())
            {
              m_structures.emplace_back(std::make_unique<structure>(o.get_array()));
            }
        }
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




