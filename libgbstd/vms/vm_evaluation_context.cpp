#include"libgbstd/vm.hpp"




namespace gbstd{




evaluation_context::
evaluation_context(const function&  fn) noexcept
{
  auto&  frm = push();

  frm.m_function = &fn;

  frm.m_block_stack.emplace_back(&fn.get_entry_block());
}




void
evaluation_context::
start() noexcept
{
    while(m_frame_stack.size())
    {
      
    }
}




}




