#include"libgbstd/vm.hpp"




namespace gbstd{




void
switch_statement::
compile(const space_node&  nd, compile_context&  ctx) const
{
  ctx.start_switch_block();

  ctx.finish_block();
}


void
switch_statement::
print() const noexcept
{
  printf("switch(");

  get_expression().print();

  printf(")");

  block_statement::print();
}




}




