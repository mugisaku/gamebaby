#include"libgbstd/vm.hpp"




namespace gbstd{




value
unary_operation::
operator()(execution_frame&  frm) const noexcept
{
  auto  v = m_operand.evaluate(frm);

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
/*
        if(v.is_data())
        {
          auto  i = v.get_data();

            switch(m_kind)
            {
          case(kind::bit_not    ): v = value::make_data(~i);break;
          case(kind::logical_not): v = value::make_data(!i);break;
            }
        }
*/
    }



  return value();
}


void
unary_operation::
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


  printf(" ");

  m_operand.print();
}




}




