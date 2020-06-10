#include"libgbstd/vm.hpp"




namespace gbstd{




void
case_statement::
print() const noexcept
{
  printf("case ");

  get_expression().print();

  printf(":");
}




}




