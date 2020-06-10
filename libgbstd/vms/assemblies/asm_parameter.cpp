#include"libgbstd/vms/assembly.hpp"




namespace gbstd{




asm_parameter&
asm_parameter::
assign(const asm_parameter&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        if(is_label())
        {
          create_at(&m_data.lb,rhs.m_data.lb);
        }

      else
        {
          std::memcpy(&m_data,&rhs.m_data,sizeof(data));
        }
    }


  return *this;
}


asm_parameter&
asm_parameter::
assign(asm_parameter&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        if(is_label())
        {
          create_at(&m_data.lb,std::move(rhs.m_data.lb));
        }

      else
        {
          std::memcpy(&m_data,&rhs.m_data,sizeof(data));
        }
    }


  return *this;
}


asm_parameter&
asm_parameter::
assign(int64_t  i) noexcept
{
  clear();

  m_data.i = i;

  m_kind = kinds::integer;

  return *this;
}


asm_parameter&
asm_parameter::
assign(double   f) noexcept
{
  clear();

  m_data.f = f;

  m_kind = kinds::fpn;

  return *this;
}


asm_parameter&
asm_parameter::
assign(std::string_view  sv) noexcept
{
  clear();

  create_at(&m_data.lb,sv);

  m_kind = kinds::label;

  return *this;
}


asm_parameter&
asm_parameter::
assign(const asm_line&  ln) noexcept
{
  clear();

  m_data.ln = &ln;

  m_kind = kinds::reference;

  return *this;
}


asm_parameter&
asm_parameter::
clear() noexcept
{
    if(is_label())
    {
      std::destroy_at(&m_data.lb);
    }


  m_kind = kinds::null;

  return *this;
}


void
asm_parameter::
print() const noexcept
{
    switch(m_kind)
    {
  case(kinds::integer): printf("%" PRIi64,m_data.i);break;
  case(kinds::fpn    ): printf("%f",m_data.f);break;
  case(kinds::label  ): printf("%s",m_data.lb.data());break;
//  case(kinds::): printf("",m_data.);break;
    }
}




}




