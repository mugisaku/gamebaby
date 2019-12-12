#include"libgbstd/vm.hpp"




namespace gbstd{




void
store_instruction::
execute(const space&  sp) const noexcept
{
  auto  dst_v = m_destination.evaluate(sp);
  auto  src_v = m_source.evaluate(     sp);

    if(src_v.is_data())
    {
      auto  dst_p = dst_v.get_data();
      auto  src_d = src_v.get_data();

        if(dst_v.is_u8_pointer())
        {
          *sp.get_memory().get_ptr8(dst_p) = src_d;
        }

      else
        if(dst_v.is_u16_pointer())
        {
          *sp.get_memory().get_ptr16(dst_p) = src_d;
        }

      else
        if(dst_v.is_u32_pointer())
        {
          *sp.get_memory().get_ptr32(dst_p) = src_d;
        }
    }
}




}




