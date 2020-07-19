#include"libgbstd/vm.hpp"




namespace gbstd{




void
if_string_statement::
compile(const space_node&  nd, compile_context&  ctx) const
{
  int  n = 0;

  ctx.start_if_string_block();

    for(auto&  ifst: m_if_list)
    {
      ifst.compile(nd,ctx,"",n++);
    }


    if(m_else_block_space)
    {
      m_else_block_space->compile(ctx);
    }


  ctx.finish_block();
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




