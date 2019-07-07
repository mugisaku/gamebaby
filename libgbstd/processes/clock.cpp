#include"libgbstd/process.hpp"
#include<cstdio>
#include<cstring>




namespace gbstd{




clock&
clock::
clear() noexcept
{
  reset();

  m_status.clear();

  return *this;
}


clock&
clock::
reset(uint32_t  time, uint32_t  fraction) noexcept
{
  m_time     =     time;
  m_fraction = fraction;

  return *this;
}


clock&
clock::
add(uint32_t  t) noexcept
{
    if(is_working() && !is_pausing())
    {
      constexpr int  shift_amount = 16;

      m_fraction += (t<<shift_amount)/1000*m_permil;

      m_time += (m_fraction>>shift_amount);

      m_fraction &= 0xFFFF;
    }


  return *this;
}


const clock
clock_watch::
m_null;




}




