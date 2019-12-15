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

      std::swap(m_kind ,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::operation): new(&m_data) std::string(std::move(rhs.m_data.s));break;
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
  case(kind::value): m_data.v.~value();break;
    }


  m_kind = kind::null;
}


value
operand::
evaluate(evaluation_context&  ctx) const noexcept
{
    if(is_value())
    {
      return m_data.v;
    }

  else
    if(is_operation())
    {
      auto  op = ctx->get_function().find_operation(m_data.s);

        if(op)
        {
          return op->evaluate(ctx);
        }
    }


  return value::make_null();
}




}




