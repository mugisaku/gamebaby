#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{





transparency_info&
transparency_info::
assign(const transparency_info&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_pixel_format     = rhs.m_pixel_format;
      m_number_of_values = rhs.m_number_of_values;

        switch(m_pixel_format)
        {
      case(pixel_format::null):
          break;
      case(pixel_format::indexed):
          m_uint8 = new uint8_t[m_number_of_values];

          std::memcpy(m_uint8,rhs.m_uint8,m_number_of_values);
          break;
      case(pixel_format::grayscale):
          m_uint16 = new uint16_t[m_number_of_values];

          std::memcpy(m_uint16,rhs.m_uint16,2*m_number_of_values);
          break;
      case(pixel_format::rgb):
          m_uint16 = new uint16_t[3*m_number_of_values];

          std::memcpy(m_uint16,rhs.m_uint16,6*m_number_of_values);
          break;
      default:
          printf("transparency_info assign error: invalid format\n");
        }
    }


  return *this;
}


transparency_info&
transparency_info::
assign(transparency_info&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_pixel_format    ,rhs.m_pixel_format    );
      std::swap(m_number_of_values,rhs.m_number_of_values);

        switch(m_pixel_format)
        {
      case(pixel_format::null):
          break;
      case(pixel_format::indexed):
          std::swap(m_uint8,rhs.m_uint8);
          break;
      case(pixel_format::grayscale):
      case(pixel_format::rgb):
          std::swap(m_uint16,rhs.m_uint16);
          break;
      default:
          printf("transparency_info assign error: invalid format\n");
        }
    }


  return *this;
}


transparency_info&
transparency_info::
assign(const chunk&  chk, pixel_format  fmt) noexcept
{
  clear();

  m_pixel_format = fmt;

  binary_view  bv(chk);

    switch(m_pixel_format)
    {
  case(pixel_format::null):
      break;
  case(pixel_format::indexed):
      m_number_of_values = chk.get_data_size();

      m_uint8 = new uint8_t[chk.get_data_size()];

      std::memcpy(m_uint8,chk.begin(),chk.get_data_size());
      break;
  case(pixel_format::grayscale):
      m_number_of_values = chk.get_data_size()/sizeof(uint16_t);

      m_uint16 = new uint16_t[chk.get_data_size()/sizeof(uint16_t)];

        for(int  i = 0;  i < m_number_of_values;  ++i)
        {
          m_uint16[i] = bv.get_be16();
        }
      break;
  case(pixel_format::rgb):
      m_number_of_values = chk.get_data_size()/sizeof(uint16_t)/3;

      m_uint16 = new uint16_t[chk.get_data_size()/sizeof(uint16_t)];

        for(int  i = 0;  i < m_number_of_values;  ++i)
        {
          auto  dst = &m_uint16[3*i];

          *dst++ = bv.get_be16();
          *dst++ = bv.get_be16();
          *dst++ = bv.get_be16();
        }
      break;
  default:
      printf("transparency_info assign error: invalid format\n");
    }


  return *this;
}




void
transparency_info::
clear() noexcept
{
    switch(m_pixel_format)
    {
  case(pixel_format::null):
      break;
  case(pixel_format::indexed):
      delete[] m_uint8          ;
               m_uint8 = nullptr;
      break;
  case(pixel_format::grayscale):
  case(pixel_format::rgb):
      delete[] m_uint16          ;
               m_uint16 = nullptr;
      break;
  default:
      printf("transparency_info clear error: invalid format\n");
    }


  m_pixel_format = pixel_format::null;

  m_number_of_values = 0;
}


uint8_t
transparency_info::
get_alpha(uint8_t  i) const noexcept
{
    if(m_pixel_format == pixel_format::indexed)
    {
      return (i < m_number_of_values)? m_uint8[i]:255;
    }


  return 255;
}


uint8_t
transparency_info::
get_alpha(uint16_t  v) const noexcept
{
    if(m_pixel_format == pixel_format::grayscale)
    {
        for(int  i = 0;  i < m_number_of_values;  ++i)
        {
            if(m_uint16[i] == v)
            {
              return 255;
            }
        }


      return 0;
    }


  return 255;
}


uint8_t
transparency_info::
get_alpha(uint16_t  r, uint16_t  g, uint16_t  b) const noexcept
{
    if(m_pixel_format == pixel_format::rgb)
    {
      const uint16_t*  p     = m_uint16;
      const uint16_t*  p_end = m_uint16+(3*m_number_of_values);

        while(p != p_end)
        {
            if((p[0] == r) &&
               (p[1] == g) &&
               (p[2] == b))
            {
              return 255;
            }


          p += 3;
        }


      return 0;
    }


  return 255;
}




chunk
transparency_info::
make_chunk() const noexcept
{
  binary  bin;

  binary_cursor  bc;

    switch(m_pixel_format)
    {
  case(pixel_format::null):
      break;
  case(pixel_format::indexed):
      bin = binary(m_number_of_values);

      bc = binary_cursor(bin);

        for(int  i = 0;  i < m_number_of_values;  ++i)
        {
          bc.put_8(m_uint8[i]);
        }
      break;
  case(pixel_format::grayscale):
      bin = binary(2*m_number_of_values);

      bc = binary_cursor(bin);

        for(int  i = 0;  i < m_number_of_values;  ++i)
        {
          bc.put_be16(m_uint16[i]);
        }
      break;
  case(pixel_format::rgb):
      bin = binary(6*m_number_of_values);

      bc = binary_cursor(bin);

        for(int  i = 0;  i < m_number_of_values;  ++i)
        {
          auto  src = &m_uint16[3*i];

          bc.put_be16(*src++);
          bc.put_be16(*src++);
          bc.put_be16(*src++);
        }
      break;
  default:
      printf("transparency_info assign error: invalid format\n");
    }


  return chunk(std::move(bin),chunk_name("tRNS"));
}




void
transparency_info::
print() const noexcept
{
  printf("%d values",m_number_of_values);
}



}




