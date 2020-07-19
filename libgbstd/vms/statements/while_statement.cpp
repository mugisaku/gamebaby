#include"libgbstd/vm.hpp"




namespace gbstd{




void
while_statement::
compile(const space_node&  nd, compile_context&  ctx) const
{
  ctx.start_while_block();

  auto  ti = compile_expression(get_expression(),nd,ctx);

  ctx.finish_block();
}


void
while_statement::
print() const noexcept
{
  printf("while(");

  get_expression().print();

  printf(")");

  block_statement::print();
}




}




