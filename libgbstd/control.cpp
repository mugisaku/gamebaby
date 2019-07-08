#include"libgbstd/control.hpp"
#include<cstdio>
#include<cstring>




namespace gbstd{




uint32_t  g_time;

std::vector<point>  g_point_buffer;

std::vector<uint8_t>  g_dropped_file;

bool  g_needed_to_redraw;

bool  g_restain;

user_input  g_modified_input;
user_input           g_input;


namespace{
uint32_t  g_user_time;
uint32_t  g_anti_time;
uint32_t  g_stop_time;
uint32_t  g_vtime;
bool  g_user_time_flowing;
uint32_t  g_unbarrier_time;
}


void
barrier_input(uint32_t  interval) noexcept
{
  g_unbarrier_time = g_time+interval;
}


bool
test_input_barrier() noexcept
{
  return (g_time <= g_unbarrier_time);
}


bool
is_user_time_flowing() noexcept
{
  return g_user_time_flowing;
}


void
start_user_time() noexcept
{
    if(!g_user_time_flowing)
    {
      g_user_time_flowing = true;

      g_anti_time += g_time-g_stop_time;
    }
}


void
stop_user_time() noexcept
{
    if(g_user_time_flowing)
    {
      g_user_time_flowing = false;

      g_stop_time = g_time            ;
      g_user_time = g_time-g_anti_time;
    }
}


uint32_t
get_user_time() noexcept
{
  return g_user_time_flowing? (g_time-g_anti_time):g_user_time;
}


uint32_t
get_system_time() noexcept
{
  return g_time;
}


void
add_virtual_time(uint32_t  ms) noexcept
{
  g_vtime += ms;
}


uint32_t
get_virtual_time() noexcept
{
  return g_vtime;
}


void
print_time_status() noexcept
{
  uint32_t  at = g_anti_time;

    if(!g_user_time_flowing)
    {
      at += g_time-g_stop_time;
    }


  printf("sys:%8d, usr:%8d, stp:%8d, %s\n",
    g_time,
    get_user_time(),
    at,
    g_user_time_flowing?"":"stopped");
}




}




