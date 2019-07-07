#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




namespace{
uint8_t
bitpack_grayscale4(const uint8_t*  ptr) noexcept
{
  return((ptr[0]>>4)<<4)|
        ((ptr[1]>>4)   );
}
uint8_t
bitpack_grayscale2(const uint8_t*  ptr) noexcept
{
  return((ptr[0]>>6)<<6)|
        ((ptr[1]>>6)<<4)|
        ((ptr[2]>>6)<<2)|
        ((ptr[3]>>6)   );
}
uint8_t
bitpack_grayscale1(const uint8_t*  ptr) noexcept
{
  return((ptr[0]>>7)<<7)|
        ((ptr[1]>>7)<<6)|
        ((ptr[2]>>7)<<5)|
        ((ptr[3]>>7)<<4)|
        ((ptr[4]>>7)<<3)|
        ((ptr[5]>>7)<<2)|
        ((ptr[6]>>7)<<1)|
        ((ptr[7]>>7)   );
}
uint8_t
bitpack_indexed4(const uint8_t*  ptr) noexcept
{
  return((ptr[0]&0xF)<<4)|
        ((ptr[1]&0xF)   );
}
uint8_t
bitpack_indexed2(const uint8_t*  ptr) noexcept
{
  return((ptr[0]&0b11)<<6)|
        ((ptr[1]&0b11)<<4)|
        ((ptr[2]&0b11)<<2)|
        ((ptr[3]&0b11)   );
}
uint8_t
bitpack_indexed1(const uint8_t*  ptr) noexcept
{
  return((ptr[0]&1)<<7)|
        ((ptr[1]&1)<<6)|
        ((ptr[2]&1)<<5)|
        ((ptr[3]&1)<<4)|
        ((ptr[4]&1)<<3)|
        ((ptr[5]&1)<<2)|
        ((ptr[6]&1)<<1)|
        ((ptr[7]&1)   );
}
void
write(const uint8_t*&  src, int  w, int  n, uint8_t  (*bitpack)(const uint8_t*), uint8_t*&  dst) noexcept
{
    while(w)
    {
      uint8_t  table[n];

        for(int  i = 0;  i < n;  ++i)
        {
          table[i] = 0;
        }


        for(int  i = 0;  i < n;  ++i)
        {
          table[i] = *src++;

            if(!--w)
            {
              break;
            }
        }
    

      *dst++ = bitpack(table);
    }
}
}


binary
get_bitpacked(const uint8_t*  src, const image_header&  ihdr) noexcept
{
  int  w = ihdr.get_width() ;
  int  h = ihdr.get_height();

  int  bit_depth = ihdr.get_bit_depth();

  auto  fmt = ihdr.get_pixel_format();

  int  src_pitch = w;
  int  dst_pitch = ihdr.get_pitch();

  binary  tmp(dst_pitch*h);

  auto  dst = tmp.begin();

    for(int  y = 0;  y < h;  ++y)
    {
        if(bit_depth == 8)
        {
            for(int  x = 0;  x < src_pitch;  ++x)
            {
              *dst++ = *src++;
            }
        }

      else
        {
            if(fmt == pixel_format::indexed)
            {
                switch(bit_depth)
                {
              case(4): write(src,w,2,bitpack_indexed4,dst);break;
              case(2): write(src,w,4,bitpack_indexed2,dst);break;
              case(1): write(src,w,8,bitpack_indexed1,dst);break;
                }
            }

          else
            if(fmt == pixel_format::grayscale)
            {
                switch(bit_depth)
                {
              case(4): write(src,w,2,bitpack_grayscale4,dst);break;
              case(2): write(src,w,4,bitpack_grayscale2,dst);break;
              case(1): write(src,w,8,bitpack_grayscale1,dst);break;
                }
            }
        }
    }


  return std::move(tmp);
}




}




