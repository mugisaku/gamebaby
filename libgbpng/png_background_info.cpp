#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




background_info&
background_info::
assign(const chunk&  chk, pixel_format  fmt) noexcept
{
  m_pixel_format = fmt;

  binary_view  bv(chk);

    switch(m_pixel_format)
    {
  case(pixel_format::null):
      break;
  case(pixel_format::indexed):
      m_values[0] = bv.get_8();
      break;
  case(pixel_format::grayscale):
  case(pixel_format::grayscale_with_alpha):
      m_values[0] = bv.get_be16();
      break;
  case(pixel_format::rgb):
  case(pixel_format::rgba):
      m_values[0] = bv.get_be16();
      m_values[1] = bv.get_be16();
      m_values[2] = bv.get_be16();
      break;
  default:
      printf("background_info assign error: invalid format\n");
    }


  return *this;
}


chunk
background_info::
make_chunk() const noexcept
{
  binary  bin;

  binary_cursor  bc;

    switch(m_pixel_format)
    {
  case(pixel_format::null):
      break;
  case(pixel_format::indexed):
      bin = binary(1);

      *bin.begin() = m_values[0];
      break;
  case(pixel_format::grayscale):
  case(pixel_format::grayscale_with_alpha):
      bin = binary(2);

      bc = binary_cursor(bin);

      bc.put_be16(m_values[0]);
      break;
  case(pixel_format::rgb):
  case(pixel_format::rgba):
      bin = binary(6);

      bc = binary_cursor(bin);

      bc.put_be16(m_values[0]);
      bc.put_be16(m_values[1]);
      bc.put_be16(m_values[2]);
      break;
  default:
      printf("background_info assign error: invalid format\n");
    }


  return chunk(std::move(bin),chunk_name("bKGD"));
}


void
background_info::
print() const noexcept
{
  printf("");
}




}




