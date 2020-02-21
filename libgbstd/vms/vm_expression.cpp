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
      case(kind::operand      ): new(&m_data)           operand(std::move(rhs.m_data.op    ));break;
      case(kind::prefix_unary ):
      case(kind::postfix_unary): new(&m_data)  unary_expression(std::move(rhs.m_data.unexp ));break;
      case(kind::binary       ): new(&m_data) binary_expression(std::move(rhs.m_data.binexp));break;
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
assign(int  k, unary_expression&&  e) noexcept
{
  clear();

  new(&m_data) unary_expression(std::move(e));

  m_kind = (k == 'p')? kind::prefix_unary
          :            kind::postfix_unary
          ;

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
  case(kind::prefix_unary  ):
  case(kind::postfix_unary  ): m_data.unexp.~unary_expression();break;
  case(kind::binary ): m_data.binexp.~binary_expression();break;
    }


  m_kind = kind::null;

  return *this;
}


void
expression::
print() const noexcept
{
  printf("(");

    switch(m_kind)
    {
  case(kind::operand):
      m_data.op.print();
      break;
  case(kind::prefix_unary):
      m_data.unexp.get_expression().print();
      printf("%s",m_data.unexp.get_opcode().get_string());
      break;
  case(kind::postfix_unary):
      printf("%s",m_data.unexp.get_opcode().get_string());
      m_data.unexp.get_expression().print();
      break;
  case(kind::binary):
      m_data.binexp.get_left().print();
      printf("%s",m_data.binexp.get_opcode().get_string());
      m_data.binexp.get_right().print();
      break;
    }


  printf(")");
}




expression
make_expression(std::string_view  sv) noexcept
{
  exprrpn  rpn(sv);

  std::vector<expression>  buf;

    for(auto&  e: rpn.get_stack())
    {
        if(e.is_unsigned_integer())
        {
          buf.emplace_back(operand(e.get_unsigned_integer()));
        }

      else
        if(e.is_real_number())
        {
          buf.emplace_back(operand(e.get_real_number()));
        }

      else
        if(e.is_identifier())
        {
          auto  s = e.get_string();

            if(s == std::string_view("undefined"))
            {
              buf.emplace_back(operand(undefined()));
            }

          else
            if(s == std::string_view("nullptr"))
            {
              buf.emplace_back(operand(nullptr));
            }

          else
            if(s == std::string_view("true"))
            {
              buf.emplace_back(operand(true));
            }

          else
            if(s == std::string_view("false"))
            {
              buf.emplace_back(operand(false));
            }

          else
            {
              identifier  id(s);

              buf.emplace_back(operand(std::move(id)));
            }
        }

      else
        if(e.is_string())
        {
          buf.emplace_back(operand(e.get_string()));
        }

      else
        if(e.is_prefix_unary_operator())
        {
            if(buf.size() < 1)
            {
              report;

              return {};
            }


          auto  op = e.get_operator_code();
          auto  o  = std::make_unique<expression>(std::move(buf.back()));

          buf.back().assign('p',unary_expression(op,std::move(o)));
        }

      else
        if(e.is_postfix_unary_operator())
        {
            if(buf.size() < 1)
            {
              report;

              return {};
            }


          auto  op = e.get_operator_code();
          auto  o  = std::make_unique<expression>(std::move(buf.back()));

          buf.back().assign('P',unary_expression(op,std::move(o)));
        }

      else
        if(e.is_binary_operator())
        {
            if(buf.size() < 2)
            {
              report;

              return {};
            }


          auto  r = std::make_unique<expression>(std::move(buf.back()));

          buf.pop_back();

          auto  l = std::make_unique<expression>(std::move(buf.back()));

          auto  op = e.get_operator_code();

          buf.back() = binary_expression(op,std::move(l),std::move(r));
        }
    }


    if(buf.size() == 1)
    {
      return std::move(buf.back());
    }


  report;

  return expression();
}




/*
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
*/




}




