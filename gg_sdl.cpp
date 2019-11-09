#include"children/FallBlockPazzle/FallBlockPazzle.hpp"
#include"children/Dungeon/Dungeon.hpp"
#include"libgbstd/gpfs.hpp"
#include"libsdlglu/sdl.hpp"


using namespace gbstd;


namespace{
const std::vector<game_information>
g_games =
{
  FallBlockPazzle::stage::get_information(),
          Dungeon::world::get_information(),
};


int
g_index;


int
g_ask_index;


class
info_window: public window
{
public:
  info_window() noexcept
  {
    set_x_position(20);
    set_y_position(20);

    set_content_width( g_font_width *30);
    set_content_height(g_font_height*14);

    get_style().set_fill_color(colors::blue);

    get_style().set_frame_top_width(8);
    get_style().set_frame_left_width(8);
    get_style().set_frame_right_width(8);
    get_style().set_frame_bottom_width(8);
  }

} g_inf_window;


class
ask_window: public window
{
public:
  ask_window() noexcept
  {
    set_x_position(80);
    set_y_position(200);

    set_content_width( g_font_width *12);
    set_content_height(g_font_height*2);

    get_style().set_fill_color(colors::blue);

    get_style().set_frame_top_width(8);
    get_style().set_frame_left_width(8);
    get_style().set_frame_right_width(8);
    get_style().set_frame_bottom_width(8);
  }

} g_ask_window;


void
draw_information(const canvas&  cv) noexcept
{
  auto&  inf = g_games[g_index];

  auto  cont_cv = g_inf_window.render(cv);

  string_form  sf;

  cont_cv.draw_string(colors::white,sf("タイトル: %s",inf.title.data()),0,0);
  cont_cv.draw_string(colors::white,sf("カテゴリィ: %s",inf.category_name.data()),0,g_font_height*2);
  cont_cv.draw_string(colors::white,"せつめい:",0,g_font_height*4);
  cont_cv.draw_string_in_area(colors::white,inf.description,0,g_font_height*5,g_font_width*30,g_font_height*8);
}


void
draw_ask_window(const canvas&  cv) noexcept
{
  canvas  misc_cv(g_misc_image,0,0,24,24);

  auto  cont_cv = g_ask_window.render(cv);

  point  pt(0,0);

  cont_cv.draw_string(colors::white,"このゲームと　あそぶ",pt.move_x(0),pt.move_y(            0));
  cont_cv.draw_string(colors::white,"うん　やだ "         ,pt.move_x(0),pt.move_y(g_font_height));


  int  x = g_ask_window.get_x_position();
  int  y = g_ask_window.get_y_position()+28;

  cv.draw_canvas(misc_cv,!g_ask_index? x-20:x+4,y);
}


void  select(execution&  exec) noexcept;


void
ask(execution&  exec) noexcept
{
    if(get_keys().test_dil() && g_ask_index)
    {
      --g_ask_index;
    }

  else
    if(get_keys().test_dir() && !g_ask_index)
    {
      ++g_ask_index;
    }

  else
    if(get_keys().test_acr())
    {
      barrier_keys();

      get_root_directory()["/video/sprites/spr01"]
        .set_ignore_flag()
      ;

        if(!g_ask_index)
        {
          get_root_directory()["/video/sprites/spr00"]
            .set_ignore_flag()
          ;

          g_games[g_index].boot();
        }

      else
        {
          g_ask_index = 0;

          exec.replace(select);
        }
    }


  exec.interrupt();
}


void
select(execution&  exec) noexcept
{
    if(get_keys().test_dil() && g_index)
    {
      --g_index;
    }

  else
    if(get_keys().test_dir() && (g_index < (g_games.size()-1)))
    {
      ++g_index;
    }

  else
    if(get_keys().test_acr())
    {
      barrier_keys();

      get_root_directory()["/video/sprites/spr01"]
        .unset_ignore_flag()
        .be_sprite().set_callback({draw_ask_window})
      ;

      exec.replace(ask);
    }


  exec.interrupt();
}


void
initialize(execution&  exec) noexcept
{
  get_root_directory()["/video/sprites/spr00"].be_sprite().set_callback({draw_information});

  exec.replace(select);
}


}




int
main(int  argc, char**  argv)
{
  int  w = 320;
  int  h = 320;

    for(auto&  inf: g_games)
    {
      w = std::max(w,inf.screen_width );
      h = std::max(h,inf.screen_height);
    }


  gbstd::push_execution({initialize});

  sdl::init(w,h);

  sdl::start_loop();


  return 0;
}




