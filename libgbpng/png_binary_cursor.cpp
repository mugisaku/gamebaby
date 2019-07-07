#include"libgbpng/png.hpp"




namespace gbpng{




binary_cursor&
binary_cursor::
assign(const binary&  bin) noexcept
{
  m_pointer = bin.begin();

  return *this;
}


void
binary_cursor::
put_be16(uint16_t  i) noexcept
{
  *m_pointer++ = (i>>8);
  *m_pointer++ = (i   );
}


void
binary_cursor::
put_be32(uint32_t  i) noexcept
{
  *m_pointer++ = (i>>24);
  *m_pointer++ = (i>>16);
  *m_pointer++ = (i>> 8);
  *m_pointer++ = (i    );
}




}




