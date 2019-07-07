#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{





node::iterator
node::iterator::
operator++(int) noexcept
{
  auto  tmp = *this;

  m_pointer = m_pointer->m_next_sibling;

  return tmp;
}


node::iterator
node::iterator::
operator--(int) noexcept
{
  auto  tmp = *this;

  m_pointer = m_pointer->m_previous_sibling;

  return tmp;
}


node::iterator&
node::iterator::
operator+=(int  n) noexcept
{
    if(n < 0)
    {
        while(n++)
        {
          ++(*this);
        }
    }

  else
    if(n > 0)
    {
        while(n--)
        {
          ++(*this);
        }
    }


  return *this;
}


node::iterator&
node::iterator::
operator-=(int  n) noexcept
{
    if(n < 0)
    {
        while(n++)
        {
          --(*this);
        }
    }

  else
    if(n > 0)
    {
        while(n--)
        {
          --(*this);
        }
    }


  return *this;
}




}}




