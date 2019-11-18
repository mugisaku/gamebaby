#include"libgbstd/gbstd.hpp"




namespace gbstd{




void
execution::
reset(callback_wrapper  cb) noexcept
{
    if(!m_status.test(flags::change))
    {
      m_stack.clear();

      m_stack.emplace_back(cb);

      m_status.set(flags::change);
      m_status.unset(flags::halt);
    }

  else
    {
      printf("[execution reset error]\n");
    }
}


void
execution::
push(callback_wrapper  cb) noexcept
{
    if(!m_status.test(flags::change))
    {
      m_stack.emplace_back(cb);

      m_status.set(flags::change);
    }

  else
    {
      printf("[execution multi push error]\n");
    }
}


void
execution::
replace(callback_wrapper  cb) noexcept
{
    if(!m_status.test(flags::change))
    {
      m_stack.back() = cb;

      m_status.set(flags::change);
    }

  else
    {
      printf("[execution replace error]\n");
    }
}


void
execution::
pop(int  v) noexcept
{
    if(!m_status.test(flags::change))
    {
      m_stack.pop_back();

      m_control_value = v;

      m_status.set(flags::change);
    }

  else
    {
      printf("[execution multi pop error]\n");
    }
}




void
execution::
step() noexcept
{
  static bool  lock;

    if(m_stack.empty())
    {
      halt();

      return;
    }


    if(!lock)
    {
      lock = true;

      m_status.unset(flags::interrupt);
      m_status.unset(flags::change   );


      auto  cb = m_stack.back();

      int  safe_counter = 80;

//      int  x_counter = 0;

        while(*this)
        {
            if(!safe_counter--)
            {
              printf("warning: step execution, safe counter is reached zero.\n");

              break;
            }


          cb(*this);

//          ++x_counter;

            if(m_status.test(flags::interrupt))
            {
              break;
            }


            if(m_status.test(flags::change))
            {
                if(m_stack.size())
                {
                  cb = m_stack.back();

                  m_status.unset(flags::change);
                }

              else
                {
                  halt();

                  break;
                }
            }
        }


//      printf("executes %d times.\n",x_counter);
      
      lock = false;
    }
}




int
execution::
get_control_value() noexcept
{
  return m_control_value;
}


void
execution::
interrupt() noexcept
{
  m_status.set(flags::interrupt);
}


void
execution::
halt() noexcept
{
  m_status.set(flags::halt);
}


}




