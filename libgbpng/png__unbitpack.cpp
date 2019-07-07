#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




namespace{
void
unbitpack_grayscale4(uint8_t  src, uint8_t*  dst) noexcept
{
  dst[0] = ((src>>4)&0xF)<<4;
  dst[1] = ((src   )&0xF)<<4;
}
void
unbitpack_grayscale2(uint8_t  src, uint8_t*  dst) noexcept
{
  dst[0] = ((src>>6)&0b11)<<6;
  dst[1] = ((src>>4)&0b11)<<6;
  dst[2] = ((src>>2)&0b11)<<6;
  dst[3] = ((src   )&0b11)<<6;
}
void
unbitpack_grayscale1(uint8_t  src, uint8_t*  dst) noexcept
{
  dst[0] = ((src>>7)&1)<<7;
  dst[1] = ((src>>6)&1)<<7;
  dst[2] = ((src>>5)&1)<<7;
  dst[3] = ((src>>4)&1)<<7;
  dst[4] = ((src>>3)&1)<<7;
  dst[5] = ((src>>2)&1)<<7;
  dst[6] = ((src>>1)&1)<<7;
  dst[7] = ((src   )&1)<<7;
}
void
unbitpack_indexed4(uint8_t  src, uint8_t*  dst) noexcept
{
  dst[0] = (src>>4)&0xF;
  dst[1] = (src   )&0xF;
}
void
unbitpack_indexed2(uint8_t  src, uint8_t*  dst) noexcept
{
  dst[0] = (src>>6)&0b11;
  dst[1] = (src>>4)&0b11;
  dst[2] = (src>>2)&0b11;
  dst[3] = (src   )&0b11;
}
void
unbitpack_indexed1(uint8_t  src, uint8_t*  dst) noexcept
{
  dst[0] = (src>>7)&1;
  dst[1] = (src>>6)&1;
  dst[2] = (src>>5)&1;
  dst[3] = (src>>4)&1;
  dst[4] = (src>>3)&1;
  dst[5] = (src>>2)&1;
  dst[6] = (src>>1)&1;
  dst[7] = (src   )&1;
}
void
read(const uint8_t*&  src, int  w, int  n, void  (*unbitpack)(uint8_t,uint8_t*), uint8_t*&  dst) noexcept
{
    while(w)
    {
      uint8_t  table[n];

        for(int  i = 0;  i < n;  ++i)
        {
          table[i] = 0;
        }


      unbitpack(*src++,table);

        for(int  i = 0;  i < n;  ++i)
        {
          *dst++ = table[i];

            if(!--w)
            {
              break;
            }
        }
    }
}
}




binary
get_unbitpacked(const uint8_t*  src, const image_header&  ihdr) noexcept
{
  int  bit_depth = ihdr.get_bit_depth();

  int  w = ihdr.get_width() ;
  int  h = ihdr.get_height();

  int  src_pitch = ihdr.get_pitch();
  int  dst_pitch = w;


  binary  tmp(dst_pitch*h);

  auto  dst = tmp.begin();

    for(int  y = 0;  y < h;  ++y)
    {
        if(bit_depth == 8)
        {
            for(int  x = 0;  x < w;  ++x)
            {
              *dst++ = *src++;
            }
        }

      else
        {
            if(ihdr.get_pixel_format() == pixel_format::indexed)
            {
                switch(bit_depth)
                {
              case(4): read(src,w,2,unbitpack_indexed4,dst);break;
              case(2): read(src,w,4,unbitpack_indexed2,dst);break;
              case(1): read(src,w,8,unbitpack_indexed1,dst);break;
                }
            }

          else
            if(ihdr.get_pixel_format() == pixel_format::grayscale)
            {
                switch(bit_depth)
                {
              case(4): read(src,w,2,unbitpack_grayscale4,dst);break;
              case(2): read(src,w,4,unbitpack_grayscale2,dst);break;
              case(1): read(src,w,8,unbitpack_grayscale1,dst);break;
                }
            }
        }
    }


  return std::move(tmp);
}




}




