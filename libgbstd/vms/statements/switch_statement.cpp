#include"libgbstd/vm.hpp"




namespace gbstd{




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




