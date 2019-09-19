#include"libgbstd/control.hpp"
#include<cstdio>
#include<cstring>




namespace gbstd{




std::vector<uint8_t>  g_dropped_file;


namespace{
bool  g_redraw_flag=true;

uint32_t  g_time;

key_state  g_previous_keys;
key_state  g_modified_keys;
key_state           g_keys;
key_state      g_null_keys;

uint32_t  g_unbarrier_timepoint;

point  g_a_point;
point  g_b_point;


image   g_screen_image;
canvas  g_screen_canvas;


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




uint32_t    get_time(           ) noexcept{return g_time;}
void     update_time(uint32_t  t) noexcept{g_time = t;}

const key_state&
get_modified_keys() noexcept
{
  return g_modified_keys;
}


const key_state&
get_keys() noexcept
{
  return (g_time >= g_unbarrier_timepoint)? g_keys:g_null_keys;
}


void
update_keys(const key_state&  keys) noexcept
{
  g_modified_keys = g_previous_keys^keys;

  g_previous_keys = g_keys       ;
                    g_keys = keys;

    if(g_modified_keys)
    {
      g_unbarrier_timepoint = 0;
    }
}


void
barrier_keys(uint32_t  interval) noexcept
{
  g_unbarrier_timepoint = g_time+interval;
}


void
update_point(point  pt) noexcept
{
  g_b_point = pt;
}


liner
make_liner() noexcept
{
  liner  ln(g_a_point,g_b_point);

  g_a_point = g_b_point;

  return ln;
}




const canvas&
set_screen_size(int  w, int  h) noexcept
{
  g_screen_image.resize(w,h);

  return g_screen_canvas.assign(g_screen_image);
}


const canvas&
get_screen_canvas() noexcept
{
  return g_screen_canvas;
}


const image&
get_screen_image() noexcept
{
  return g_screen_image;
}




}




