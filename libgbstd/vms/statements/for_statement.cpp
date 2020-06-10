#include"libgbstd/vm.hpp"




namespace gbstd{




void
for_statement::
print() const noexcept
{
  printf("for(");

  m_init.print();

  printf(";");

  m_cond.print();

  printf(";");

  m_loop.print();

  printf(")");

  block_statement::print();
}




}




