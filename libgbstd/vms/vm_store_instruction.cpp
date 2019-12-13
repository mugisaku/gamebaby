#include"libgbstd/vm.hpp"




namespace gbstd{




void
store_instruction::
execute(evaluation_context&  ctx) const noexcept
{
  auto  dst_v = m_destination.evaluate(ctx);
  auto  src_v = m_source.evaluate(     ctx);

    if(src_v.is_data())
    {
      auto&  mem = ctx->m_function->get_space().get_memory();

      auto  dst_p = dst_v.get_data();
      auto  src_d = src_v.get_data();

        if(dst_v.is_u8_pointer())
        {
          *mem.get_ptr8(dst_p) = src_d;
        }

      else
        if(dst_v.is_u16_pointer())
        {
          *mem.get_ptr16(dst_p) = src_d;
        }

      else
        if(dst_v.is_u32_pointer())
        {
          *mem.get_ptr32(dst_p) = src_d;
        }
    }
}




}




