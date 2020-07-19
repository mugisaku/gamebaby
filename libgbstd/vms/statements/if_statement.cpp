#include"libgbstd/vm.hpp"




namespace gbstd{




void
if_statement::
compile(const space_node&  nd, compile_context&  ctx, const char*  base, int  number) const
{
}


void
if_statement::
print() const noexcept
{
  printf("if(");

  m_expression.print();

  printf(")");

  block_statement::print();
}




}




