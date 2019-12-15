#include"libgbstd/vm.hpp"




namespace gbstd{




evaluation_context::
evaluation_context(const function&  fn) noexcept
{
  push({fn});
}




void
evaluation_context::
store(value  dst, value  src) noexcept
{
    if(src.is_data())
    {
      auto&  mem = (*this)->get_function().get_space().get_memory();

      auto  dst_p = dst.get_data();
      auto  src_d = src.get_data();

        if(dst.is_u8_pointer())
        {
          *mem.get_ptr8(dst_p) = src_d;
        }

      else
        if(dst.is_u16_pointer())
        {
          *mem.get_ptr16(dst_p) = src_d;
        }

      else
        if(dst.is_u32_pointer())
        {
          *mem.get_ptr32(dst_p) = src_d;
        }
    }
}


void
evaluation_context::
step(const codeline&  codeln) noexcept
{
    if(codeln.is_store_instruction())
    {
      auto&  st = codeln.get_store_instruction();

      store(st.get_destination().evaluate(*this),
                 st.get_source().evaluate(*this));
    }

  else
    if(codeln.is_branch_instruction())
    {
    }

  else
    if(codeln.is_return_instruction())
    {
    }

  else
    if(codeln.is_operation())
    {
      (*this)->advance();
    }
}


void
evaluation_context::
run() noexcept
{
    while(m_frame_stack.size())
    {
      auto&  frm = m_frame_stack.back();

        if(frm)
        {
          step(frm.get_codeline());
        }

      else
        {
          break;
        }
    }
}




}




