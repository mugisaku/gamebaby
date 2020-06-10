#include"libgbstd/vm.hpp"




namespace gbstd{




void
return_statement::
print() const noexcept
{
  printf("return ");

  get_expression().print();
}




}




