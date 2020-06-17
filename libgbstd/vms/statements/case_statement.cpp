#include"libgbstd/vm.hpp"




namespace gbstd{




void
case_statement::
compile(const space_node&  nd, compile_context&  ctx) const
{
  expression_statement::compile(nd,ctx);
}


void
case_statement::
print() const noexcept
{
  printf("case ");

  get_expression().print();

  printf(":");
}




}




