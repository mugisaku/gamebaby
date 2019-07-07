#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




namespace{
void
unfilter_none(const uint8_t*  src, int  bpp, int  w, uint8_t*  dst) noexcept
{
    for(int  x = 0;  x < w;  ++x)
    {
        for(int  i = 0;  i < bpp;  ++i)
        {
          *dst++ = *src++;
        }
    }
}


void
unfilter_sub(const uint8_t*  src, int  bpp, int  w, uint8_t*  dst) noexcept
{
  uint8_t  dummy[bpp];

    for(int  i = 0;  i < bpp;  ++i)
    {
      dummy[i] = 0;
    }


  const uint8_t*  left_src = dummy;

    for(int  x = 0;  x < w;  ++x)
    {
        for(int  i = 0;  i < bpp;  ++i)
        {
          auto   cur =      *src++;
          auto  left = *left_src++;

          *dst++ = cur+left;
        }


      left_src = dst-bpp;
    }
}


void
unfilter_up(const uint8_t*  src, const uint8_t*  up_src, int  bpp, int  w, uint8_t*  dst) noexcept
{
    for(int  x = 0;  x < w;  ++x)
    {
        for(int  i = 0;  i < bpp;  ++i)
        {
          auto   cur =    *src++;
          auto    up = *up_src++;

          *dst++ = (cur+up);
        }
    }
}


void
unfilter_average(const uint8_t*  src, const uint8_t*  up_src, int  bpp, int  w, uint8_t*  dst) noexcept
{
  uint8_t  dummy[bpp];

    for(int  i = 0;  i < bpp;  ++i)
    {
      dummy[i] = 0;
    }


  const uint8_t*  left_src = dummy;

    for(int  x = 0;  x < w;  ++x)
    {
        for(int  i = 0;  i < bpp;  ++i)
        {
          auto   cur =      *src++;
          auto    up =   *up_src++;
          auto  left = *left_src++;

          *dst++ = cur+((left+up)/2);
        }


      left_src = dst-bpp;
    }
}


constexpr int
get_abs(int  i) noexcept
{
  return (i < 0)? -i:i;
}


constexpr int
paeth(int  a, int  b, int  c) noexcept
{
  int  p = a+b-c;

  int  pa = get_abs(p-a);
  int  pb = get_abs(p-b);
  int  pc = get_abs(p-c);

    if((pa <= pb) && (pa <= pc))
    {
      return a;
    }


    if(pb <= pc)
    {
      return b;
    }


  return c;
}


void
unfilter_paeth(const uint8_t*  src, const uint8_t*  up_src, int  bpp, int  w, uint8_t*  dst) noexcept
{
  uint8_t  dummy[bpp];

    for(int  i = 0;  i < bpp;  ++i)
    {
      dummy[i] = 0;
    }


  const uint8_t*     left_src = dummy;
  const uint8_t*  up_left_src = dummy;

    for(int  x = 0;  x < w;  ++x)
    {
        for(int  i = 0;  i < bpp;  ++i)
        {
          auto      cur =         *src++;
          auto       up =      *up_src++;
          auto     left =    *left_src++;
          auto  up_left = *up_left_src++;

          *dst++ = cur+paeth(left,up,up_left);
        }


         left_src =    dst-bpp;
      up_left_src = up_src-bpp;
    }
}


}




binary
get_unfiltered(const uint8_t*  src, const image_header&  ihdr)
{
  constexpr int     none = 0;
  constexpr int      sub = 1;
  constexpr int       up = 2;
  constexpr int  average = 3;
  constexpr int    paeth = 4;

  int  w = ihdr.get_width() ;
  int  h = ihdr.get_height();

  int  bpp = ihdr.get_number_of_bytes_per_pixel();

  int  bit_depth = ihdr.get_bit_depth();

  int  pitch = ihdr.get_pitch();


  binary  tmp(pitch*h);

  auto  dst = tmp.begin();


  uint8_t  dummy_line[pitch];

    for(int  i = 0;  i < pitch;  ++i)
    {
      dummy_line[i] = 0;
    }


  const uint8_t*  up_src = dummy_line;

  int     none_count = 0;
  int      sub_count = 0;
  int       up_count = 0;
  int  average_count = 0;
  int    paeth_count = 0;

    for(int  y = 0;  y < h;  ++y)
    {
      auto  filter_type = *src++;

        if(filter_type == none)
        {
          ++none_count;

          unfilter_none(src,bpp,((bit_depth < 8)? pitch:w),dst);
        }

      else
        if(filter_type == sub)
        {
          ++sub_count;

          unfilter_sub(src,bpp,((bit_depth < 8)? pitch:w),dst);
        }

      else
        if(filter_type == up)
        {
          ++up_count;

          unfilter_up(src,up_src,bpp,((bit_depth < 8)? pitch:w),dst);
        }

      else
        if(filter_type == average)
        {
          ++average_count;

          unfilter_average(src,up_src,bpp,((bit_depth < 8)? pitch:w),dst);
        }

      else
        if(filter_type == paeth)
        {
          ++paeth_count;

          unfilter_paeth(src,up_src,bpp,((bit_depth < 8)? pitch:w),dst);
        }

      else
        {
          throw_error("unkmown filter type");
        }


      up_src = dst;

      src += pitch;
      dst += pitch;
    }


/*
  printf("   none: %6d\n",   none_count);
  printf("    sub: %6d\n",    sub_count);
  printf("     up: %6d\n",     up_count);
  printf("average: %6d\n",average_count);
  printf("  paeth: %6d\n",  paeth_count);
*/

  return std::move(tmp);
}




}




