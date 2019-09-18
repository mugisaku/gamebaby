#include"libgbstd/character.hpp"




namespace gbstd{




string_form::
string_form(const char*  fmt, ...) noexcept
{
  va_list  ap;
  va_start(ap,fmt);

  auto  res = vsnprintf(m_buffer,sizeof(m_buffer),fmt,ap);

    if((res < -1) || (res >= sizeof(m_buffer)))
    {
      report;

      m_length = 0;
    }

  else
    {
      m_length = res;
    }


  va_end(ap);
}


const char*
string_form::
operator()(const char*  fmt, ...) noexcept
{
  va_list  ap;
  va_start(ap,fmt);

  auto  res = vsnprintf(m_buffer,sizeof(m_buffer),fmt,ap);

    if((res < -1) || (res >= sizeof(m_buffer)))
    {
      report;

      m_length = 0;
    }

  else
    {
      m_length = res;
    }


  va_end(ap);

  return m_buffer;
}




}




