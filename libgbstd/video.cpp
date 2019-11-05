#include"libgbstd/video.hpp"
#include"libgbstd/gbstd.hpp"
#include"libgbstd/ilc.hpp"
#include"libgbstd/random.hpp"
#include<cstdio>
#include<cstring>
#include<memory>




namespace gbstd{




namespace{
bool  g_redraw_flag=true;

gpfs::directory*
g_video_dir;

gpfs::directory*
g_video_sprites_dir;

color  g_background_color;

image   g_image;
canvas  g_canvas;
}




sprite&
sprite::
set_blinking_rate(int  show, int  hide) noexcept
{
  m_blink_show_value = show;
  m_blink_hide_value = hide;

  return *this;
}


sprite&
sprite::
operator()(const canvas&  cv) noexcept
{
    if(is_blinking())
    {
        if(m_blink_counter)
        {
          --m_blink_counter;
        }

      else
        {
          m_blink_counter = m_status.test(flags::blink_bit)? m_blink_show_value
                           :                                 m_blink_hide_value;

          m_status.reverse(flags::blink_bit);
        }
    }


    if(!is_hiding())
    {
        if(!is_blinking() || m_status.test(flags::blink_bit))
        {
          m_callback(cv);
        }
    }


  return *this;
}




void
set_redraw_flag() noexcept
{
  g_redraw_flag = true;
}


void
unset_redraw_flag() noexcept
{
  g_redraw_flag = false;
}


bool
test_redraw_flag() noexcept
{
  return g_redraw_flag;
}




const color&  get_background_color(               ) noexcept{return g_background_color;}
void          set_background_color(color  bg_color) noexcept{g_background_color = bg_color;}


void
redraw_video() noexcept
{
  static bool  lock;

    if(!lock)
    {
      lock = true;

      g_canvas.fill(g_background_color);


      static std::vector<gpfs::node_reference>  dirref_stack;

      dirref_stack.emplace_back(g_video_sprites_dir->get_self_node()->get_self_reference());

      constexpr auto  test = [](const gpfs::node&  nd)->bool{return !nd.test_ignore_flag() && nd.is_sprite();};
      constexpr auto  proc = [](      gpfs::node&  nd)      {nd.get_sprite()(g_canvas);};

      process_directory_recursively(dirref_stack,test,proc);


      lock = false;
    }
}


const canvas&
set_video_size(int  w, int  h) noexcept
{
  static bool  initialized;

    if(!initialized)
    {
      initialize_random();

      initialize_javascript();

      g_video_dir         = get_root_directory().create_directory("/video");
      g_video_sprites_dir = g_video_dir->create_directory("sprites");

      initialized = true;
    }


  g_image.resize(w,h);

  return g_canvas.assign(g_image);
}


const canvas&
get_video_canvas() noexcept
{
  return g_canvas;
}


const image&
get_video_image() noexcept
{
  return g_image;
}


gpfs::directory&
get_video_sprites_directory() noexcept
{
  return *g_video_sprites_dir;
}




}




