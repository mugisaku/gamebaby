#include"libgbstd/vm.hpp"




namespace gbstd{




expression&
expression::
assign(expression&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::operand): new(&m_data)           operand(std::move(rhs.m_data.op    ));break;
      case(kind::unary  ): new(&m_data)  unary_expression(std::move(rhs.m_data.unexp ));break;
      case(kind::binary ): new(&m_data) binary_expression(std::move(rhs.m_data.binexp));break;
        }
    }


  return *this;
}


expression&
expression::
assign(operand&&  o) noexcept
{
  clear();

  new(&m_data) operand(std::move(o));

  m_kind = kind::operand;

  return *this;
}


expression&
expression::
assign(unary_expression&&  e) noexcept
{
  clear();

  new(&m_data) unary_expression(std::move(e));

  m_kind = kind::unary;

  return *this;
}


expression&
expression::
assign(binary_expression&&  e) noexcept
{
  clear();

  new(&m_data) binary_expression(std::move(e));

  m_kind = kind::binary;

  return *this;
}




expression&
expression::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::operand): m_data.op.~operand();break;
  case(kind::unary  ): m_data.unexp.~unary_expression();break;
  case(kind::binary ): m_data.binexp.~binary_expression();break;
    }


  m_kind = kind::null;

  return *this;
}




value
unary_expression::
evaluate(context&  ctx) const noexcept
{
  auto  v = m_expression->evaluate(ctx);

  return unary_operations::get_callback(m_opcode)(ctx,v);
}


value
binary_expression::
evaluate(context&  ctx) const noexcept
{
  auto  lv = m_left->evaluate(ctx);
  auto  rv = m_left->evaluate(ctx);

  return binary_operations::get_callback(m_opcode)(ctx,lv,rv);
}


value
expression::
evaluate(context&  ctx) const noexcept
{
  return is_operand()? m_data.op.evaluate(ctx)
        :is_unary()?   m_data.unexp.evaluate(ctx)
        :is_binary()?  m_data.binexp.evaluate(ctx)
        :value()
        ;
}




}




