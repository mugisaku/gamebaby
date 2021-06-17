#include"libgbstd/vms/ir.hpp"




namespace gbstd{




ir_value&
ir_value::
assign(const ir_value&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      m_type_info = rhs.m_type_info;
      m_data.i    = rhs.m_data.i;

        if(m_type_info.is_object())
        {
          auto&  c = *reinterpret_cast<counter_t*>(m_data.o);

          ++c;
        }
    }


  return *this;
}


ir_value&
ir_value::
assign(ir_value&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      std::swap(m_type_info,rhs.m_type_info);
      std::swap(m_data.i   ,rhs.m_data.i);
    }


  return *this;
}


ir_value&
ir_value::
assign(int  sz, const void*  o) noexcept
{
  unrefer();

  m_type_info = ir_type_info('o',sz);

  allocate(sz,o);

  return *this;
}




void
ir_value::
unrefer() noexcept
{
    if(m_type_info.is_object())
    {
      auto&  c = *reinterpret_cast<counter_t*>(m_data.o);

        if(!--c)
        {
          free(m_data.o);
        }
    }


  m_type_info = ir_type_info();
}


void
ir_value::
allocate(int  sz, const void*  o) noexcept
{
  auto  p = static_cast<uint8_t*>(malloc(size_of_counter+sz));

  *reinterpret_cast<counter_t*>(p) = 1;

  std::memcpy(p+size_of_counter,o,sz);

  m_data.o = p;
}




}




