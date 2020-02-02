#include"libgbstd/vm.hpp"




namespace gbstd{




value
context::
operate(unary_opcodes  op, const operand&  o) noexcept
{
  auto  ov = o.evaluate(*this);

  auto  v = dereference(ov);

  auto&  t = v.get_type_info();

    if(op == unary_opcodes::ld)
    {
        if(t.is_pointer())
        {
          auto  tt = t.get_base();

            if(tt->is_integer())
            {
              auto  inf = tt->get_integer_type_info();

              return make_value(~v.get_integer());
            }

          else
            if(tt->is_unsigned_integer())
            {
            }
        }
    }

  else
    if(op == unary_opcodes::bit_not)
    {
        if(t.is_kind_of_integer())
        {
          return make_value(~v.get_integer());
        }
    }

  else
    if(op == unary_opcodes::logical_not)
    {
        if(t.is_like_boolean())
        {
          return make_value(!v.get_integer());
        }
    }

  else
    if(op == unary_opcodes::get_size)
    {
      return make_value(static_cast<uint64_t>(t.get_size()));
    }

  else
    if(op == unary_opcodes::get_address)
    {
        if(ov.get_type_info().is_reference())
        {
          return make_value(ov.get_unsigned_integer());
        }
    }


report;

  return value();
}


value
context::
operate(binary_opcodes  op, const operand&  l, const operand&  r) noexcept
{
  auto  olv = l.evaluate(*this);
  auto  orv = r.evaluate(*this);

  auto  lv = dereference(olv);
  auto  rv = dereference(orv);

  auto&  lt = lv.get_type_info();
  auto&  rt = rv.get_type_info();


    if(op == binary_opcodes::add)
    {
        if(lt.is_integer())
        {
          auto  li = lv.get_integer();

            if(rt.is_integer())
            {
              return make_value(li+rv.get_integer());
            }

          else
            if(rt.is_unsigned_integer())
            {
              return make_value(li+rv.get_unsigned_integer());
            }
        }

      else
        if(lt.is_unsigned_integer())
        {
          auto  li = lv.get_unsigned_integer();

            if(rt.is_integer())
            {
              return make_value(li+rv.get_integer());
            }

          else
            if(rt.is_unsigned_integer())
            {
              return make_value(li+rv.get_unsigned_integer());
            }
        }

      else
        if(lt.is_pointer())
        {
          auto  li = lv.get_unsigned_integer();
          auto  sz = lt.get_base()->get_size();

            if(rt.is_integer())
            {
              return make_value(li+(sz*rv.get_integer()));
            }

          else
            if(rt.is_unsigned_integer())
            {
              return make_value(li+(sz*rv.get_unsigned_integer()));
            }
        }
    }

  else
    if(op == binary_opcodes::sub)
    {
        if(lt.is_integer())
        {
          auto  li = lv.get_integer();

            if(rt.is_integer())
            {
              return make_value(li-rv.get_integer());
            }

          else
            if(rt.is_unsigned_integer())
            {
              return make_value(li-rv.get_unsigned_integer());
            }
        }

      else
        if(lt.is_unsigned_integer())
        {
          auto  li = lv.get_unsigned_integer();

            if(rt.is_integer())
            {
              return make_value(li-rv.get_integer());
            }

          else
            if(rt.is_unsigned_integer())
            {
              return make_value(li-rv.get_unsigned_integer());
            }
        }

      else
        if(lt.is_pointer())
        {
          auto  sz = lt.get_base()->get_size();

            if(rt.is_pointer() && (lt == rt))
            {
              return make_value((lv.get_integer()-rv.get_integer())/sz);
            }

          else
            {
              auto  li = lv.get_unsigned_integer();

                if(rt.is_integer())
                {
                  return make_value(li-(sz*rv.get_integer()));
                }

              else
                if(rt.is_unsigned_integer())
                {
                  return make_value(li-(sz*rv.get_unsigned_integer()));
                }
            }
        }
    }

  else
    if(op == binary_opcodes::mul)
    {
        if(lt.is_integer())
        {
            if(rt.is_integer() || rt.is_unsigned_integer())
            {
              return make_value( lv.get_integer()
                                *rv.get_integer());
            }
        }

      else
        if(lt.is_unsigned_integer())
        {
            if(rt.is_integer() || rt.is_unsigned_integer())
            {
              return make_value( lv.get_unsigned_integer()
                                *rv.get_unsigned_integer());
            }
        }
    }

  else
    if(op == binary_opcodes::div)
    {
        if(lt.is_integer())
        {
            if(rt.is_integer() || rt.is_unsigned_integer())
            {
              return make_value( lv.get_integer()
                                /rv.get_integer());
            }
        }

      else
        if(lt.is_unsigned_integer())
        {
            if(rt.is_integer() || rt.is_unsigned_integer())
            {
              return make_value( lv.get_unsigned_integer()
                                /rv.get_unsigned_integer());
            }
        }
    }

  else
    if(op == binary_opcodes::rem)
    {
        if(lt.is_integer())
        {
            if(rt.is_integer() || rt.is_unsigned_integer())
            {
              return make_value( lv.get_integer()
                                %rv.get_integer());
            }
        }

      else
        if(lt.is_unsigned_integer())
        {
            if(rt.is_integer() || rt.is_unsigned_integer())
            {
              return make_value( lv.get_unsigned_integer()
                                %rv.get_unsigned_integer());
            }
        }
    }

  else
    if(op == binary_opcodes::shl)
    {
        if(lt.is_integer())
        {
            if(rt.is_kind_of_integer())
            {
              return make_value(  lv.get_integer()
                                <<rv.get_unsigned_integer());
            }
        }

      else
        if(lt.is_unsigned_integer())
        {
            if(rt.is_kind_of_integer())
            {
              return make_value(  lv.get_unsigned_integer()
                                <<rv.get_unsigned_integer());
            }
        }
    }

  else
    if(op == binary_opcodes::shr)
    {
        if(lt.is_integer())
        {
            if(rt.is_kind_of_integer())
            {
              return make_value(  lv.get_integer()
                                >>rv.get_unsigned_integer());
            }
        }

      else
        if(lt.is_unsigned_integer())
        {
            if(rt.is_kind_of_integer())
            {
              return make_value(  lv.get_unsigned_integer()
                                >>rv.get_unsigned_integer());
            }
        }
    }

  else
    if(op == binary_opcodes::eq)
    {
      auto  li = lv.get_integer();
      auto  ri = rv.get_integer();

        if((lt.is_integer()          && rt.is_integer()         ) ||
           (lt.is_unsigned_integer() && rt.is_unsigned_integer()))
        {
          return make_value(li == ri);
        }

      else
        if(lt.is_pointer() && rt.is_pointer() && (lt == rt))
        {
          return make_value(li == ri);
        }
   }

  else
    if(op == binary_opcodes::neq)
    {
      auto  li = lv.get_integer();
      auto  ri = rv.get_integer();

        if((lt.is_integer()          && rt.is_integer()         ) ||
           (lt.is_unsigned_integer() && rt.is_unsigned_integer()))
        {
          return make_value(li != ri);
        }

      else
        if(lt.is_pointer() && rt.is_pointer() && (lt == rt))
        {
          return make_value(li != ri);
        }
    }

  else
    if(op == binary_opcodes::lt)
    {
        if(lt.is_integer() && rt.is_integer())
        {
          return make_value(lv.get_integer() < rv.get_integer());
        }

      else
        if(lt.is_unsigned_integer() && rt.is_unsigned_integer())
        {
          return make_value(lv.get_unsigned_integer() < rv.get_unsigned_integer());
        }

      else
        if(lt.is_pointer() && rt.is_pointer() && (lt == rt))
        {
          return make_value(lv.get_unsigned_integer() < rv.get_unsigned_integer());
        }
    }

  else
    if(op == binary_opcodes::lteq)
    {
        if(lt.is_integer() && rt.is_integer())
        {
          return make_value(lv.get_integer() <= rv.get_integer());
        }

      else
        if(lt.is_unsigned_integer() && rt.is_unsigned_integer())
        {
          return make_value(lv.get_unsigned_integer() <= rv.get_unsigned_integer());
        }

      else
        if(lt.is_pointer() && rt.is_pointer() && (lt == rt))
        {
          return make_value(lv.get_unsigned_integer() <= rv.get_unsigned_integer());
        }
    }

  else
    if(op == binary_opcodes::gt)
    {
        if(lt.is_integer() && rt.is_integer())
        {
          return make_value(lv.get_integer() > rv.get_integer());
        }

      else
        if(lt.is_unsigned_integer() && rt.is_unsigned_integer())
        {
          return make_value(lv.get_unsigned_integer() > rv.get_unsigned_integer());
        }

      else
        if(lt.is_pointer() && rt.is_pointer() && (lt == rt))
        {
          return make_value(lv.get_unsigned_integer() > rv.get_unsigned_integer());
        }
    }

  else
    if(op == binary_opcodes::gteq)
    {
        if(lt.is_integer() && rt.is_integer())
        {
          return make_value(lv.get_integer() >= rv.get_integer());
        }

      else
        if(lt.is_unsigned_integer() && rt.is_unsigned_integer())
        {
          return make_value(lv.get_unsigned_integer() >= rv.get_unsigned_integer());
        }

      else
        if(lt.is_pointer() && rt.is_pointer() && (lt == rt))
        {
          return make_value(lv.get_unsigned_integer() >= rv.get_unsigned_integer());
        }
    }

  else
    if(op == binary_opcodes::logical_or)
    {
        if(lt.is_like_boolean() && rt.is_like_boolean())
        {
          return make_value(   lv.get_integer()
                            || rv.get_integer());
        }
    }

  else
    if(op == binary_opcodes::logical_and)
    {
        if(lt.is_like_boolean() && rt.is_like_boolean())
        {
          return make_value(   lv.get_integer()
                            && rv.get_integer());
        }
    }

  else
    if(op == binary_opcodes::bit_or)
    {
        if(lt.is_kind_of_integer() && rt.is_kind_of_integer())
        {
          return make_value( lv.get_unsigned_integer()
                            |rv.get_unsigned_integer());
        }
    }

  else
    if(op == binary_opcodes::bit_and)
    {
        if(lt.is_kind_of_integer() && rt.is_kind_of_integer())
        {
          return make_value( lv.get_unsigned_integer()
                            &rv.get_unsigned_integer());
        }
    }

  else
    if(op == binary_opcodes::bit_xor)
    {
        if(lt.is_kind_of_integer() && rt.is_kind_of_integer())
        {
          return make_value( lv.get_unsigned_integer()
                            ^rv.get_unsigned_integer());
        }
    }


report;
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




