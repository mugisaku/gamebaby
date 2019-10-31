#include"libgbstd/gbstd.hpp"
#include<memory>




namespace gbstd{




std::vector<uint8_t>  g_dropped_file;




namespace{
uint32_t  g_time;
uint32_t  g_unbarrier_timepoint;

key_state  g_previous_keys;
key_state  g_modified_keys;
key_state           g_keys;
key_state      g_null_keys;


std::vector<std::unique_ptr<execution>>
g_execution_stack;

gpfs::node*  g_root_node;

gpfs::directory*  g_clocks_dir;
gpfs::directory*  g_timers_dir;


point  g_a_point;
point  g_b_point;


}




void
push_execution(callback_wrapper  cb) noexcept
{
  g_execution_stack.emplace_back(std::make_unique<execution>(cb));
}


void
pop_execution() noexcept
{
  g_execution_stack.pop_back();
}




uint32_t
get_time() noexcept
{
  return g_time;
}


void
update_time(uint32_t  t) noexcept
{
  static bool  lock;

    if(!lock && (t > g_time))
    {
      lock = true;

      auto  diff = t-g_time;

      g_time = t;

        for(auto&  nd: *g_clocks_dir)
        {
            if(nd.is_clock())
            {
              auto&  clk = nd.get_clock();

                if(!clk.is_stopped() && !clk.is_paused())
                {
                  clk >>= diff;
                }
            }
        }


        for(auto&  nd: *g_timers_dir)
        {
            if(nd.is_timer())
            {
              auto&  tm = nd.get_timer();

                if(!tm.is_stopped() && !tm.is_paused())
                {
                  tm();
                }
            }
        }


        if(g_execution_stack.size())
        {
          g_execution_stack.back()->step();
        }


      redraw_video();

      lock = false;
    }
}


const key_state&
get_modified_keys() noexcept
{
  return g_modified_keys;
}


const key_state&
get_pressed_keys() noexcept
{
  return g_keys;
}


const key_state&
get_keys() noexcept
{
  return (g_time >= g_unbarrier_timepoint)? g_keys:g_null_keys;
}


void
update_keys(const key_state&  keys) noexcept
{
  g_previous_keys = g_keys       ;
                    g_keys = keys;

  g_modified_keys = g_previous_keys^g_keys;

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


gpfs::
directory&
get_root_directory() noexcept
{
  static bool  initialized;

    if(!initialized)
    {
      g_root_node = gpfs::node::create_root();

      auto&  root_dir = g_root_node->get_directory();

      g_clocks_dir = root_dir.create_directory("/clocks");
      g_timers_dir = root_dir.create_directory("/timers");

      initialized = true;
    }


  return g_root_node->get_directory();
}




}




