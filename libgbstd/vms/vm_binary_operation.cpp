#include"libgbstd/vm.hpp"




namespace gbstd{




value
binary_operation::
operator()(execution_frame&  frm) const noexcept
{
  auto  lv = m_left.evaluate( frm);
  auto  rv = m_right.evaluate(frm);

/*
    if(lv.is_u8_pointer() && rv.is_data())
    {
      auto  l = lv.get_data();
      auto  r = rv.get_data();

        switch(m_kind)
        {
      case(kind::add): l += r;break;
      case(kind::sub): l -= r;break;
        }


      lv = value::make_u8_pointer(l);
    }

  else
    if(lv.is_u16_pointer() && rv.is_data())
    {
      auto  l = lv.get_data();
      auto  r = rv.get_data();

        switch(m_kind)
        {
      case(kind::add): l += 2*r;break;
      case(kind::sub): l -= 2*r;break;
        }


      lv = value::make_u16_pointer(l);
    }

  else
    if(lv.is_u32_pointer() && rv.is_data())
    {
      auto  l = lv.get_data();
      auto  r = rv.get_data();

        switch(m_kind)
        {
      case(kind::add): l += 4*r;break;
      case(kind::sub): l -= 4*r;break;
        }


      lv = value::make_u32_pointer(l);
    }

  else
    if(lv.is_data() && rv.is_data())
    {
      auto  l = lv.get_data();
      auto  r = rv.get_data();

        switch(m_kind)
        {
      case(kind::add        ): l += r;break;
      case(kind::sub        ): l -= r;break;
      case(kind::mul        ): l *= r;break;
      case(kind::div        ): l /= r;break;
      case(kind::rem        ): l %= r;break;

      case(kind::shl        ): l <<= r;break;
      case(kind::shr        ): l >>= r;break;

      case(kind::eq         ): l = (l == r)? 1:0;break;
      case(kind::neq        ): l = (l != r)? 1:0;break;
      case(kind::lt         ): l = (l <  r)? 1:0;break;
      case(kind::lteq       ): l = (l <= r)? 1:0;break;
      case(kind::gt         ): l = (l >  r)? 1:0;break;
      case(kind::gteq       ): l = (l >= r)? 1:0;break;

      case(kind::logical_and): l = (l && r)? 1:0;break;
      case(kind::logical_or ): l = (l || r)? 1:0;break;

      case(kind::bit_and    ): l &= r;break;
      case(kind::bit_or     ): l |= r;break;
      case(kind::bit_xor    ): l ^= r;break;
        }


      lv = value::make_data(l);
    }
*/


  return lv;
}


void
binary_operation::
print() const noexcept
{
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




}




