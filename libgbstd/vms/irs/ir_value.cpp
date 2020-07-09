#include"libgbstd/vms/ir.hpp"




namespace gbstd{




ir_value&
ir_value::
assign(const ir_value&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_type_info = rhs.m_type_info;
      m_memory    = rhs.m_memory;
    }


  return *this;
}


ir_value&
ir_value::
assign(ir_value&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_type_info,rhs.m_type_info);
      std::swap(m_memory   ,rhs.m_memory   );
    }


  return *this;
}


ir_value&
ir_value::
assign(ir_type_info  ti) noexcept
{
  m_type_info = ti;
  m_memory    = ti;

  return *this;
}


ir_value&
ir_value::
assign(ir_type_info  ti, ir_memory&&  mem) noexcept
{
  m_type_info = ti;
  m_memory    = std::move(mem);

  return *this;
}


ir_value&
ir_value::
assign(int64_t  i) noexcept
{
  clear();

  m_type_info = ir_type_info("int");

  m_memory = ir_memory(i);

  return *this;
}


ir_value&
ir_value::
assign(double  f) noexcept
{
  clear();

  m_type_info = ir_type_info("float");

  m_memory = ir_memory(f);

  return *this;
}


ir_value&
ir_value::
assign(std::string_view  sv) noexcept
{
  clear();

  m_type_info = ir_type_info(sv.size());

  m_memory = ir_memory(sv);

  return *this;
}




void
ir_value::
clear() noexcept
{
  m_type_info = ir_type_info();
  m_memory    = ir_memory();
}


ir_value
ir_value::
clone() const noexcept
{
  ir_value  v;

  v.m_type_info = m_type_info;
  v.m_memory    = m_memory.clone();

  return std::move(v);
}


void
ir_value::
print() const noexcept
{
  m_type_info.print();

  printf(", ");

    if(m_type_info.is_integer())
    {
      printf("%" PRIi64,m_memory.get_content<int64_t>());
    }

  else
    if(m_type_info.is_fpn())
    {
      printf("%f",m_memory.get_content<double>());
    }

  else
    if(m_type_info.is_object())
    {
      printf("{");

      auto  sz = m_type_info.get_size();

        if(sz < 48)
        {
          auto  p = &m_memory.get_content<uint8_t>();

            for(int  i = 0;  i < sz;  ++i)
            {
              printf("%i,",p[i]);
            }
        }

      else
        {
          printf("...");
        }


      printf("}");
    }
}



}




