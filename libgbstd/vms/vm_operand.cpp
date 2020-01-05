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
      case(kind::operation_label): new(&m_data) std::string(rhs.m_data.s);break;
      case(kind::integer_literal): new(&m_data) int64_t(rhs.m_data.i);break;
      case(kind::value          ): new(&m_data) value(rhs.m_data.v);break;
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

      std::swap(m_kind ,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::operation_label): new(&m_data) std::string(std::move(rhs.m_data.s));break;
      case(kind::integer_literal): new(&m_data) int64_t(std::move(rhs.m_data.i));break;
      case(kind::value          ): new(&m_data) value(rhs.m_data.v);break;
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

  m_kind = kind::operation_label;

  return *this;
}


operand&
operand::
assign(int64_t  i) noexcept
{
  clear();

  new(&m_data) int64_t(i);

  m_kind = kind::integer_literal;

  return *this;
}


operand&
operand::
assign(value&&  v) noexcept
{
  clear();

  new(&m_data) value(std::move(v));

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
  case(kind::operation_label): m_data.s.~basic_string();break;
  case(kind::integer_literal): /*m_data.i.~int()*/;break;
  case(kind::value): m_data.v.~value();break;
    }


  m_kind = kind::null;
}


value
operand::
evaluate(execution_frame&  frm) const noexcept
{
    if(is_value())
    {
      return m_data.v;
    }

  else
    if(is_integer_literal())
    {
//      return value::make_data(m_data.i);
    }

  else
    if(is_operation_label())
    {
      auto  var = frm.find_variable(m_data.s);

        if(var)
        {
          return var->get_value();
        }


      printf("%s not found",m_data.s.data());
    }


  return value();
}


void
operand::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::null): break;
  case(kind::operation_label): printf("%s",m_data.s.data());break;
  case(kind::integer_literal): printf("%lld",m_data.i);break;
  case(kind::value): m_data.v.print();break;
    }
}




}




