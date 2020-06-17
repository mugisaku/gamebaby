#include"libgbstd/vm.hpp"




namespace gbstd{




void
if_string_statement::
compile(const space_node&  nd, compile_context&  ctx) const
{
  auto  s = ctx.enter_if_string_block();

  int  n = 0;

    for(auto&  ifst: m_if_list)
    {
      ifst.compile(nd,ctx,s,n++);
    }


    if(m_else_block_space)
    {
      m_else_block_space->compile(ctx);
    }


  ctx.leave_control_block();
}


void
if_string_statement::
print() const noexcept
{
    for(auto&  ifst: m_if_list)
    {
      ifst.print();

      printf("\n");
    }

    if(m_else_block_space)
    {
      printf("else");

      m_else_block_space->print();
    }
}




}




