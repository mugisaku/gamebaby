#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>
#include<zlib.h>




namespace gbpng{




binary&
binary::
assign(const binary&  rhs) noexcept
{
    if(this != &rhs)
    {
      assign(rhs.m_data,rhs.m_data_size);
    }


  return *this;
}


binary&
binary::
assign(binary&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear_data();

      std::swap(m_data     ,rhs.m_data     );
      std::swap(m_data_size,rhs.m_data_size);
    }


  return *this;
}




binary&
binary::
assign(const void*  ptr, uint32_t  size) noexcept
{
  resize(size);

  std::memcpy(m_data,ptr,size);


  return *this;
}


void
binary::
resize(uint32_t  size) noexcept
{
    if(m_data_size < size)
    {
      delete[] m_data                    ;
               m_data = new uint8_t[size];
    }


  m_data_size = size;
}


void
binary::
write(const uint8_t*  ptr, size_t  size, uint32_t  i) noexcept
{
  std::memcpy(m_data+i,ptr,size);
}


void
binary::
clear_data() noexcept
{
  m_data_size = 0;

  delete[] m_data          ;
           m_data = nullptr;
}




binary
binary::
get_compressed() const
{
  unsigned long  dst_size = (m_data_size*2)+12;

  uint8_t  buf[dst_size];

    if(compress(buf,&dst_size,m_data,m_data_size) != Z_OK)
    {
      throw_error("compression is failed\n");
    }


  return binary(buf,dst_size);
}


binary
binary::
get_uncompressed() const
{
  binary  tmp(m_data_size*2);

    for(;;)
    {
      unsigned long int  size = tmp.m_data_size;

      auto  res = uncompress(tmp.m_data,&size,m_data,m_data_size);

        if(res == Z_OK)
        {
          tmp.resize(size);

          break;
        }

      else
        if(res == Z_DATA_ERROR)
        {
          throw_error("invalid input data");
        }

      else
        if(res == Z_MEM_ERROR)
        {
          throw_error("could not allocate memory\n");
        }

      else
        if(res == Z_BUF_ERROR)
        {
          tmp.resize(tmp.m_data_size*2);
        }

      else
        {
          throw_error("get_uncompressed error: unkonwn error\n");
        }
    }


  return std::move(tmp);
}




}




