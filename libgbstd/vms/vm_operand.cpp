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
      case(kind::operation      ): m_data.op = rhs.m_data.op;break;
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
      case(kind::operation      ): m_data.op = rhs.m_data.op;break;
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
  case(kind::operation_label): m_data.s.~basic_string();break;
  case(kind::operation): ;break;
  case(kind::value): m_data.v.~value();break;
    }


  m_kind = kind::null;
}


bool
operand::
is_constant(const function&  fn) const noexcept
{
   return is_operation()? fn.find_operation(m_data.s)->is_constant(fn)
         :true;
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
    if(is_operation())
    {
      auto  op = frm.get_function().find_operation(m_data.s);

        if(op)
        {
          return op->evaluate(frm);
        }
    }


  return value::make_null();
}


void
operand::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::null): break;
  case(kind::operation_label): printf("%s",m_data.s.data());break;
  case(kind::operation): ;break;
  case(kind::value): m_data.v.print();break;
    }
}




}




