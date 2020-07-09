#include"libgbstd/vms/ir.hpp"




namespace gbstd{




uint8_t
ir_memory::
m_dummy;




void
ir_memory::
unrefer() noexcept
{
    if(m_data)
    {
      auto&  n = *m_data;

        if(!--n)
        {
          free(m_data);
        }


      m_data = nullptr;
    }
}


ir_memory&
ir_memory::
assign(const ir_memory&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      m_data = rhs.m_data;

        if(m_data)
        {
          ++*m_data;
        }
    }


  return *this;
}


ir_memory&
ir_memory::
assign(ir_memory&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      std::swap(m_data,rhs.m_data);
    }


  return *this;
}




namespace{
int64_t*
allocate(size_t  sz) noexcept
{
  return (int64_t*)malloc(sizeof(int64_t)+sizeof(int64_t)+sz);
}
}


ir_memory&
ir_memory::
assign(ir_type_info  ti) noexcept
{
  unrefer();

  auto  sz = ti.get_size();

  m_data = allocate(sz);

  m_data[0] =  1;
  m_data[1] = sz;

  std::memset(&m_data[2],0,sz);

  return *this;
}


ir_memory&
ir_memory::
assign(int64_t  i) noexcept
{
  assign(ir_type_info("int"));

  m_data[2] = i;

  return *this;
}


ir_memory&
ir_memory::
assign(double  f) noexcept
{
  assign(ir_type_info("float"));

  m_data[2] = reinterpret_cast<int64_t&>(f);

  return *this;
}


ir_memory&
ir_memory::
assign(std::string_view  sv) noexcept
{
  auto  sz = sv.size();

  assign(ir_type_info(sz));

  std::memcpy(&m_data[2],sv.data(),sz);

  return *this;
}


uint8_t*
ir_memory::
get_head_pointer() const noexcept
{
  return reinterpret_cast<uint8_t*>(&m_data[2]);
}




}




