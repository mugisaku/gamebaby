#include"libgbstd/vm.hpp"




namespace gbstd{




value
context::
operate(unary_opcodes  op, const operand&  o) noexcept
{
/*
  auto  v = m_operand.evaluate(ctx);

  auto&  ti = v.get_type_info();

    if(m_kind == kind::dereference)
    {
    }

  else
    if(m_kind == kind::bit_not)
    {
    }

  else
    if(m_kind == kind::logical_not)
    {
    }

  else
    {
        if(v.is_data())
        {
          auto  i = v.get_data();

            switch(m_kind)
            {
          case(kind::bit_not    ): v = value::make_data(~i);break;
          case(kind::logical_not): v = value::make_data(!i);break;
            }
        }
    }
*/


  return value();
}


value
context::
operate(binary_opcodes  op, const operand&  l, const operand&  r) noexcept
{
  auto  lv = l.evaluate(*this);
  auto  rv = r.evaluate(*this);

  auto&  lt = lv.get_type_info();
  auto&  rt = rv.get_type_info();

    if(op == binary_opcodes::add)
    {
        if(lt.is_integer())
        {
            if(rt.is_integer())
            {
            }
        }

      else
        if(lt.is_unsigned_integer())
        {
        }

      else
        if(lt.is_pointer())
        {
        }
    }

  else
    if(op == binary_opcodes::sub)
    {
    }

  else
    if(op == binary_opcodes::eq)
    {
    }

  else
    if(op == binary_opcodes::neq)
    {
    }

  else
    if(op == binary_opcodes::lt)
    {
    }

  else
    if(op == binary_opcodes::lteq)
    {
    }

  else
    if(op == binary_opcodes::gt)
    {
    }

  else
    if(op == binary_opcodes::gteq)
    {
    }


/*
  value  retval(m_type_collection["int"]);

  retval.update(lv.get_si()+rv.get_si(),mute);
*/

  return value();
}


/*void
binary_operation::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::bit_not    ): printf("bit_not");break;
  case(kind::logical_not): printf("logical_not");break;
  case(kind::get_size   ): printf("get_size");break;
  case(kind::get_address): printf("get_address");break;
  case(kind::dereference): printf("dereference");break;
    }
        switch(m_kind)
        {
      case(kind::add        ): printf("add");break;
      case(kind::sub        ): printf("sub");break;
      case(kind::mul        ): printf("mul");break;
      case(kind::div        ): printf("div");break;
      case(kind::rem        ): printf("rem");break;
      case(kind::shl        ): printf("shl");break;
      case(kind::shr        ): printf("shr");break;
      case(kind::eq         ): printf("eq");break;
      case(kind::neq        ): printf("neq");break;
      case(kind::lt         ): printf("lt");break;
      case(kind::lteq       ): printf("lteq");break;
      case(kind::gt         ): printf("gt");break;
      case(kind::gteq       ): printf("gteq");break;
      case(kind::logical_and): printf("and");break;
      case(kind::logical_or ): printf("or");break;
      case(kind::bit_and    ): printf("and");break;
      case(kind::bit_or     ): printf("or");break;
      case(kind::bit_xor    ): printf("xor");break;
        }


  printf(" ");

  m_left.print();

  printf(" ");

  m_right.print();
}
*/



}




