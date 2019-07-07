#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




binary
get_filtered(const uint8_t*  src, const image_header&  ihdr) noexcept
{
  int  h = ihdr.get_height();

  int  pitch = ihdr.get_pitch();

  binary  tmp((1+pitch)*h);

  auto  dst = tmp.begin();

    for(int  y = 0;  y < h;  ++y)
    {
      *dst++ = 0;

        for(int  x = 0;  x < pitch;  ++x)
        {
          *dst++ = *src++;
        }
    }


  return std::move(tmp);
}




}




