#include"libgbstd/vm.hpp"




namespace gbstd{




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




