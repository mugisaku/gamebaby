#include"libgbpng/png.hpp"




namespace gbpng{




binary_view&
binary_view::
assign(const binary&  bin) noexcept
{
  m_pointer = bin.begin();

  return *this;
}


uint16_t
binary_view::
get_be16() noexcept
{
  uint16_t  i;

  i  = (*m_pointer++)<<8;
  i |= (*m_pointer++)   ;

  return i;
}


uint32_t
binary_view::
get_be32() noexcept
{
  uint32_t  i;

  i  = (*m_pointer++)<<24;
  i |= (*m_pointer++)<<16;
  i |= (*m_pointer++)<< 8;
  i |= (*m_pointer++)    ;

  return i;
}




}




