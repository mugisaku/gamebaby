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


std::string_view
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

  return std::string_view(m_buffer,m_length);
}




}




