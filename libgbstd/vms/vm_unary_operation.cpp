#include"libgbstd/vm.hpp"




namespace gbstd{




value
unary_operation::
evaluate(execution_frame&  frm) const noexcept
{
  auto  v = m_operand.evaluate(frm);

    if(m_kind == kind::dereference)
    {
      auto&  mem = frm.get_function().get_space().get_memory();

        if(v.is_u8_pointer())
        {
          v = value::make_data(*mem.get_ptr8(v.get_data()));
        }

      else
        if(v.is_u16_pointer())
        {
          v = value::make_data(*mem.get_ptr16(v.get_data()));
        }

      else
        if(v.is_u32_pointer())
        {
          v = value::make_data(*mem.get_ptr32(v.get_data()));
        }
    }

  else
    {
        if(v.is_data())
        {
          auto  i = v.get_data();

            switch(m_kind)
            {
          case(kind::neg        ): v = value::make_data(-i);break;
          case(kind::bit_not    ): v = value::make_data(~i);break;
          case(kind::logical_not): v = value::make_data(!i);break;
            }
        }
    }



  return v;
}




}




