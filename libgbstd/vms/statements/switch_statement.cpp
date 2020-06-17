#include"libgbstd/vm.hpp"




namespace gbstd{




void
switch_statement::
compile(const space_node&  nd, compile_context&  ctx) const
{
  ctx.enter_switch_block();

  auto  ti = compile_expression(get_expression(),nd,ctx);

  ctx.leave_control_block();
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




