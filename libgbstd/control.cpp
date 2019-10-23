#include"libgbstd/control.hpp"
#include"libgbstd/video.hpp"
#include<cstdio>
#include<cstring>
#include<memory>




namespace gbstd{




std::vector<uint8_t>  g_dropped_file;


clock&
clock::
operator>>=(uint32_t  t) noexcept
{
    if(m_permil)
    {
      constexpr int  shift_amount = 16;

      m_time_fraction += (t<<shift_amount)/1000*m_permil;

      m_time_integer += (m_time_fraction>>shift_amount);

      m_time_fraction &= 0xFFFF;
    }


  return *this;
}


timer&
timer::
operator()() noexcept
{
    if(!m_status.test(flags::lock) && m_interval && m_clock_pointer)
    {
      m_status.set(flags::lock);

      auto  tm = m_clock_pointer->get_time();

        while(tm >= m_next_time)
        {
          m_callback();

          m_next_time += m_interval;
        }


      m_status.unset(flags::lock);
    }


  return *this;
}




namespace{
uint32_t  g_time;
uint32_t  g_unbarrier_timepoint;

key_state  g_previous_keys;
key_state  g_modified_keys;
key_state           g_keys;
key_state      g_null_keys;




namespace flags{
constexpr int     change = 1; 
constexpr int  interrupt = 2; 
}


status_value<int>
g_execution_status;


std::vector<callback_wrapper>
g_execution_stack;


int
g_control_value;


gpfs::directory  g_root_dir;

gpfs::directory*  g_clock_dir;
gpfs::directory*  g_timer_dir;


point  g_a_point;
point  g_b_point;


}




void
reset_execution(callback_wrapper  cb) noexcept
{
    if(!g_execution_status.test(flags::change))
    {
      g_execution_stack.clear();

      g_execution_stack.emplace_back(cb);

      g_execution_status.set(flags::change);
    }

  else
    {
      printf("[execution reset error]\n");
    }
}


void
push_execution(callback_wrapper  cb) noexcept
{
    if(!g_execution_status.test(flags::change))
    {
      g_execution_stack.emplace_back(cb);

      g_execution_status.set(flags::change);
    }

  else
    {
      printf("[execution multi push error]\n");
    }
}


void
replace_execution(callback_wrapper  cb) noexcept
{
    if(!g_execution_status.test(flags::change))
    {
      g_execution_stack.back() = cb;

      g_execution_status.set(flags::change);
    }

  else
    {
      printf("[execution replace error]\n");
    }
}


void
pop_execution(int  v) noexcept
{
    if(!g_execution_status.test(flags::change))
    {
      g_execution_stack.pop_back();

      g_control_value = v;

      g_execution_status.set(flags::change);
    }

  else
    {
      printf("[execution multi pop error]\n");
    }
}




namespace{
void
step_execution() noexcept
{
  static bool  lock;

    if(!lock && g_execution_stack.size())
    {
      lock = true;

      g_execution_status.unset(flags::interrupt);
      g_execution_status.unset(flags::change   );

      auto  exec = g_execution_stack.back();

      int  safe_counter = 80;

//      int  x_counter = 0;

        for(;;)
        {
            if(!safe_counter--)
            {
              printf("warning: step execution, safe counter is reached zero.\n");

              break;
            }


          exec();

//          ++x_counter;

            if(g_execution_status.test(flags::interrupt))
            {
              break;
            }


            if(g_execution_status.test(flags::change))
            {
                if(g_execution_stack.size())
                {
                  exec = g_execution_stack.back();

                  g_execution_status.unset(flags::change);
                }

              else
                {
                  break;
                }
            }
        }


//      printf("executes %d times.\n",x_counter);
      
      lock = false;
    }
}
}




int
get_control_value() noexcept
{
  return g_control_value;
}


void
interrupt_execution() noexcept
{
  g_execution_status.set(flags::interrupt);
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

        for(auto&  clk: g_clock_table)
        {
            if(!clk->is_stopped() && !clk->is_paused())
            {
              *clk >>= diff;
            }
        }


        for(auto&  tm: g_timer_table)
        {
            if(!tm->is_stopped() && !tm->is_paused())
            {
              (*tm)();
            }
        }


      step_execution();

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
      g_root_dir.create_directory("clocks");
      g_root_dir.create_directory("timers");

      g_root_dir.create_directory("video")
        .create_directory("sprites");

      initialized = true;
    }


  return g_root_dir;
}




}




