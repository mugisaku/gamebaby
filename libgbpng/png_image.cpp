#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




uint8_t*
image::
allocate(int  bpp, int  w, int  h) noexcept
{
  m_binary.resize(bpp*w*h);

  m_width  = w;
  m_height = h;


  return m_binary.begin();
}


void
image::
store(binary&&  bin, int  w, int  h) noexcept
{
  m_binary = std::move(bin);

  m_width  = w;
  m_height = h;
}




}




