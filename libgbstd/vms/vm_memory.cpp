#include"libgbstd/vm.hpp"




namespace gbstd{




uint32_t&
memory::
get_reference_count() const noexcept
{
  return reinterpret_cast<uint32_t*>(m_data)[0];
}


void
memory::
unrefer() noexcept
{
    if(m_data)
    {
        if(!--get_reference_count())
        {
          free(m_data);
        }


      m_data = nullptr;
    }
}


uint8_t&
memory::
operator[](int  i) const noexcept
{
  return (m_data+(sizeof(uint32_t)*2))[i];
}


memory&
memory::
assign(const memory&  rhs) noexcept
{
    if(this != &rhs)
    {
      auto  sz = rhs.get_size();

      allocate(sz);

      std::memcpy((    m_data+(sizeof(uint32_t)*2)),
                  (rhs.m_data+(sizeof(uint32_t)*2)),sz);
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

      std::swap(m_data,rhs.m_data);
    }


  return *this;
}


void
memory::
allocate(uint32_t  sz) noexcept
{
  unrefer();

  m_data = (uint8_t*)malloc((sizeof(uint32_t)*2)+sz);

  reinterpret_cast<uint32_t*>(m_data)[0] =  1;
  reinterpret_cast<uint32_t*>(m_data)[1] = sz;
}


uint32_t
memory::
get_size() const noexcept
{
  return m_data? reinterpret_cast<const uint32_t*>(m_data)[1]:0;
}




}




