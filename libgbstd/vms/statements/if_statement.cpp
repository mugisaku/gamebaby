#include"libgbstd/vm.hpp"




namespace gbstd{




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




