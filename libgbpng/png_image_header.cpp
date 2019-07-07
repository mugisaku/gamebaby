#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{


image_header::
image_header(const chunk&  chk) noexcept
{
  binary_view  bv(chk);

  m_width     = bv.get_be32();
  m_height    = bv.get_be32();
  m_bit_depth = bv.get_8();
 
  set_pixel_format(static_cast<pixel_format>(bv.get_8()));

  m_compression_method = bv.get_8();
  m_filter_method      = bv.get_8();
  m_interlace_method   = bv.get_8();
}




void
image_header::
check_error() const
{
    if(!m_width)
    {
      throw_error("width is zero");
    }


    if(!m_height)
    {
      throw_error("height is zero");
    }


    if(m_bit_depth == 16)
    {
      throw_error("this programs does not handle format with bit depth of 16");
    }


    if((m_bit_depth != 1) &&
       (m_bit_depth != 2) &&
       (m_bit_depth != 4) &&
       (m_bit_depth != 8))
    {
      throw_error("bit depth is invalid");
    }


    if(m_compression_method != 0)
    {
      throw_error("unknown compression method");
    }


    if(m_filter_method != 0)
    {
      throw_error("unknown filter method");
    }


    if(m_interlace_method != 0)
    {
      throw_error("this programs does not handle interlaced image");
    }
}


const char*
image_header::
get_pixel_format_name() const noexcept
{
    switch(m_pixel_format)
    {
  case(pixel_format::grayscale           ): return "grayscale";
  case(pixel_format::grayscale_with_alpha): return "grayscale with alpha";
  case(pixel_format::indexed             ): return "indexed color";
  case(pixel_format::rgb                 ): return "RGB";
  case(pixel_format::rgba                ): return "RGBA";
  case(pixel_format::unknown             ): return "unknown";
  case(pixel_format::null                ): return "null";
    }


  return "unknown";
}


int
image_header::
get_number_of_bytes_per_pixel() const noexcept
{
    switch(m_pixel_format)
    {
  case(pixel_format::grayscale           ): return 1;
  case(pixel_format::grayscale_with_alpha): return 2;
  case(pixel_format::indexed             ): return 1;
  case(pixel_format::rgb                 ): return 3;
  case(pixel_format::rgba                ): return 4;
  case(pixel_format::unknown             ): return 1;
  case(pixel_format::null                ): return 1;
    }


  return 0;
}


int
image_header::
get_pitch() const noexcept
{
  return  (m_bit_depth == 1)? ((m_width+7)/8)
         :(m_bit_depth == 2)? ((m_width+3)/4)
         :(m_bit_depth == 4)? ((m_width+1)/2)
         :                    (get_number_of_bytes_per_pixel()*m_width);
}


chunk
image_header::
make_chunk() const noexcept
{
  constexpr size_t  size = 13;

  binary  bin(size);

  binary_cursor  bc(bin);

  bc.put_be32(m_width );
  bc.put_be32(m_height);

  bc.put_8(m_bit_depth);
  bc.put_8(static_cast<int>(m_pixel_format));
  bc.put_8(m_compression_method);
  bc.put_8(m_filter_method);
  bc.put_8(m_interlace_method);

  return chunk(std::move(bin),chunk_name("IHDR"));
}


void
image_header::
print() const noexcept
{
  printf("width: %d\n",m_width);
  printf("height: %d\n",m_height);
  printf("bit_depth: %d\n",m_bit_depth);
  printf("color_type: %s\n",get_pixel_format_name());


  printf("interlace: %d\n",m_interlace_method);
}




}




