#include"libgbstd/vm.hpp"




namespace gbstd{




struct
memory::
property
{
  uint32_t  m_size;
  uint32_t  m_reference_count;

};


void
memory::
unrefer() noexcept
{
    if(m_property)
    {
        if(!--(m_property->m_reference_count))
        {
            if(m_property->m_size > sizeof(int64_t))
            {
              free(m_byte_array);
            }


          free(m_property);
        }


      m_property = nullptr;
    }
}


memory&
memory::
assign(const memory&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      m_word     = rhs.m_word;
      m_property = rhs.m_property;

        if(m_property)
        {
          ++(m_property->m_reference_count);
        }
    }


  return *this;
}


memory&
memory::
assign(memory&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      std::swap(m_word    ,rhs.m_word    );
      std::swap(m_property,rhs.m_property);
    }


  return *this;
}


void
memory::
allocate(uint32_t  sz) noexcept
{
  unrefer();

  m_property = (property*)malloc(sizeof(property));

  m_property->m_size            = sz;
  m_property->m_reference_count =  1;

    if(sz > sizeof(int64_t))
    {
      m_byte_array = (uint8_t*)malloc(sz);

      std::memset(m_byte_array,0,sz);
    }

  else
    {
      m_word = 0;
    }
}


memory&
memory::
read(address_t  dst_addr, const memory&  src, address_t  src_addr, uint32_t  size) noexcept
{
  std::memcpy(get_base_pointer()+dst_addr,src.get_base_pointer()+src_addr,size);

  return *this;
}


uint32_t
memory::
get_size() const noexcept
{
  return m_property? m_property->m_size:0;
}




}




