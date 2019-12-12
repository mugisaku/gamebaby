#include"libgbstd/vm.hpp"




namespace gbstd{




operand&
operand::
assign(const operand&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::operation): new(&m_data) std::string(rhs.m_data.s);break;
      case(kind::integer  ): new(&m_data) int(rhs.m_data.i);break;
      case(kind::value    ): new(&m_data) value(rhs.m_data.v);break;
        }
    }


  return *this;
}


operand&
operand::
assign(operand&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::operation): new(&m_data) std::string(std::move(rhs.m_data.s));break;
      case(kind::integer  ): new(&m_data) int(rhs.m_data.i);break;
      case(kind::value    ): new(&m_data) value(rhs.m_data.v);break;
        }
    }


  return *this;
}


operand&
operand::
assign(std::string_view  sv) noexcept
{
  clear();

  new(&m_data) std::string(sv);

  m_kind = kind::operation;

  return *this;
}


operand&
operand::
assign(int  i) noexcept
{
  clear();

  new(&m_data) int(i);

  m_kind = kind::integer;

  return *this;
}


operand&
operand::
assign(value  v) noexcept
{
  clear();

  new(&m_data) value(v);

  m_kind = kind::value;

  return *this;
}


void
operand::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::null): break;
  case(kind::operation): m_data.s.~basic_string();break;
  case(kind::integer): break;
  case(kind::value): m_data.v.~value();break;
    }


  m_kind = kind::null;
}


value
operand::
evaluate(const space&  sp) const noexcept
{
  return is_integer()? value::make_data(m_data.i)
        :is_value()?   m_data.v
        :is_operation()? sp.evaluate(m_data.s)
        :value::make_null();
}




}




