#include"libgbstd/process.hpp"
#include<cstdio>
#include<cstring>




namespace gbstd{


timer&
timer::
reset() noexcept
{
  m_remain_time = m_initial_time;

  m_counter = 0;

  turnon();

  return *this;
}


timer&
timer::
reset(uint32_t  init_tm) noexcept
{
  m_initial_time = init_tm;

  return reset();
}


uint32_t
timer::
operator()(uint32_t  t) noexcept
{
    if(m_initial_time)
    {
        if(t >= m_remain_time)
        {
            for(;;)
            {
              t -= m_remain_time                 ;
                   m_remain_time = m_initial_time;

              ++m_counter;

               if(t < m_remain_time)
               {
                 break;
               }
            }
        }


      m_remain_time -= t;
    }


  return m_counter;
}


}




