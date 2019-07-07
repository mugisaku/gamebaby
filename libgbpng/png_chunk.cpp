#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




chunk::
chunk(const binary&  bin, chunk_name  name) noexcept:
binary(bin),
m_name(name)
{
  update_crc();
}


chunk::
chunk(binary&&  bin, chunk_name  name) noexcept:
binary(std::move(bin)),
m_name(name)
{
  update_crc();
}




chunk&
chunk::
operator=(const chunk&  rhs) noexcept
{
    if(this != &rhs)
    {
      binary::assign(rhs);

      m_name = rhs.m_name;

      m_crc = rhs.m_crc;
    }


  return *this;
}


chunk&
chunk::
operator=(chunk&&  rhs) noexcept
{
    if(this != &rhs)
    {
      binary::assign(std::move(rhs));

      m_name = rhs.m_name;

      m_crc = rhs.m_crc;
    }


  return *this;
}




void
chunk::
write(uint8_t*&  dst) const noexcept
{
  binary_cursor  bc(dst);

  bc.put_be32(get_data_size());


  char  buf[5];

  m_name.print(buf);

  bc.put_8(buf[0]);
  bc.put_8(buf[1]);
  bc.put_8(buf[2]);
  bc.put_8(buf[3]);

    for(int  i = 0;  i < get_data_size();  ++i)
    {
      bc.put_8(get_data()[i]);
    }


  bc.put_be32(m_crc);

  dst = bc.get_pointer();
}


void
chunk::
read(const uint8_t*&  src)
{
  binary_view  bv(src);

  auto  size = bv.get_be32();


  char  buf[5];

  buf[0] = bv.get_8();
  buf[1] = bv.get_8();
  buf[2] = bv.get_8();
  buf[3] = bv.get_8();
  buf[4] = 0;

  m_name = chunk_name(buf[0],buf[1],buf[2],buf[3]);

  binary::assign(bv.get_pointer(),size);

  bv.advance(size);

  m_crc = bv.get_be32();

    if(!test_crc())
    {
      throw_error("%s crc not matched\n",buf);
    }


  src = bv.get_pointer();
}




void
chunk::
print() const noexcept
{
  char  buf[5];

  m_name.print(buf);

  printf("data_size: %d\n",get_data_size());
  printf("name: %s\n",buf);
  printf("crc: 0x%08X\n",m_crc);

    switch(m_name)
    {
  case(chunk_name("IHDR")):
      {
        image_header  ihdr(*this);

        ihdr.print();
      }
      break;
  case(chunk_name("IDAT")):
      break;
  case(chunk_name("PLTE")):
      break;
  case(chunk_name("tRNS")):
      break;
    }
}




}




