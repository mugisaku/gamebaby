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

        if(m_type_info.is_object())
        {
          auto  sz = m_type_info.get_size();

          m_data.o = (char*)malloc(sz);

            if(m_data.o)
            {
              std::memcpy(m_data.o,rhs.m_data.o,sz);
            }
        }

      else
        {
          m_data.i = rhs.m_data.i;
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
      clear();

      std::swap(m_type_info,rhs.m_type_info);

        if(m_type_info.is_object())
        {
          std::swap(m_data.o,rhs.m_data.o);
        }

      else
        {
          m_data.i = rhs.m_data.i;
        }
    }


  return *this;
}


ir_value&
ir_value::
assign(int64_t  i) noexcept
{
  clear();

  m_type_info = ir_type_info::integer_type_info();

  m_data.i = i;

  return *this;
}


ir_value&
ir_value::
assign(double  f) noexcept
{
  clear();

  m_type_info = ir_type_info::fpn_type_info();

  m_data.f = f;

  return *this;
}


ir_value&
ir_value::
assign(std::string_view  sv) noexcept
{
  clear();

  m_type_info = ir_type_info::object_type_info(sv.size());

  m_data.o = (char*)malloc(sv.size());

    if(m_data.o)
    {
      std::memcpy(m_data.o,sv.data(),sv.size());
    }


  return *this;
}




void
ir_value::
clear() noexcept
{
    if(m_type_info.is_object())
    {
      free(m_data.o);
    }


  m_type_info = ir_type_info();

  m_data.o = nullptr;
}


void
ir_value::
print() const noexcept
{
    if(m_type_info.is_integer())
    {
      printf("%" PRIi64,m_data.i);
    }

  else
    if(m_type_info.is_fpn())
    {
      printf("%f",m_data.f);
    }

  else
    if(m_type_info.is_object())
    {
      printf("{");

      auto  sz = m_type_info.get_size();

        if(sz < 48)
        {
            for(int  i = 0;  i < sz;  ++i)
            {
              printf("%i,",m_data.o[i]);
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




