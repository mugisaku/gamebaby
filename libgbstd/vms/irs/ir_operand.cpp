#include"libgbstd/vms/ir.hpp"




namespace gbstd{




ir_operand&
ir_operand::
assign(const ir_operand&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        if(is_object() || is_label())
        {
          create_at(&m_data.s,rhs.m_data.s);
        }

      else
        if(is_phi_element_list())
        {
          create_at(&m_data.phels,rhs.m_data.phels);
        }

      else
        if(is_operand_list())
        {
          create_at(&m_data.opls,rhs.m_data.opls);
        }

      else
        {
          m_data.i = rhs.m_data.i;
        }
    }


  return *this;
}


ir_operand&
ir_operand::
assign(ir_operand&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        if(is_object() || is_label())
        {
          create_at(&m_data.s,std::move(rhs.m_data.s));
        }

      else
        if(is_phi_element_list())
        {
          create_at(&m_data.phels,std::move(rhs.m_data.phels));
        }

      else
        if(is_operand_list())
        {
          create_at(&m_data.opls,std::move(rhs.m_data.opls));
        }

      else
        {
          m_data.i = rhs.m_data.i;
        }
    }


  return *this;
}




ir_operand&
ir_operand::
assign(int64_t  i) noexcept
{
  clear();

  m_data.i = i;

  m_kind = kinds::integer;

  return *this;
}


ir_operand&
ir_operand::
assign(double   f) noexcept
{
  clear();

  m_data.f = f;

  m_kind = kinds::fpn;

  return *this;
}


ir_operand&
ir_operand::
assign(const void*  ptr, size_t  n) noexcept
{
  clear();

  create_at(&m_data.s,std::string_view(reinterpret_cast<const char*>(ptr),n));

  m_kind = kinds::object;

  return *this;
}


ir_operand&
ir_operand::
assign(std::string_view  sv) noexcept
{
  clear();

  create_at(&m_data.s,sv);

  m_kind = kinds::label;

  return *this;
}


ir_operand&
ir_operand::
assign(std::vector<ir_phi_element>&&  phels) noexcept
{
  clear();

  create_at(&m_data.phels,std::move(phels));

  m_kind = kinds::phi_element_list;

  return *this;
}


ir_operand&
ir_operand::
assign(std::vector<ir_operand>&&  opls) noexcept
{
  clear();

  create_at(&m_data.opls,std::move(opls));

  m_kind = kinds::operand_list;

  return *this;
}


void
ir_operand::
clear() noexcept
{
    if(is_object() || is_label())
    {
      std::destroy_at(&m_data.s);
    }

  else
    if(is_phi_element_list())
    {
      std::destroy_at(&m_data.phels);
    }

  else
    if(is_operand_list())
    {
      std::destroy_at(&m_data.opls);
    }


  m_kind = kinds::null;
}


void
ir_operand::
print() const noexcept
{
    switch(m_kind)
    {
  case(kinds::integer): printf("%" PRIi64,m_data.i);break;
  case(kinds::fpn    ): printf("%f",m_data.f);break;
  case(kinds::object ): printf("{...}");break;
  case(kinds::label  ): printf("%s",m_data.s.data());break;
  case(kinds::phi_element_list):
      printf("[");

        for(auto&  phel: m_data.phels)
        {
          phel.print();

          printf(", ");
        }


      printf("]");
      break;
  case(kinds::operand_list):
      printf("(");

        for(auto&  op: m_data.opls)
        {
          op.print();

          printf(", ");
        }


      printf(")");
      break;
    }
}




}




