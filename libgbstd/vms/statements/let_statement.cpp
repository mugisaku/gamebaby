#include"libgbstd/vm.hpp"




namespace gbstd{




void
let_statement::
print() const noexcept
{
  printf("let %s",m_target_name.data());

    if(m_expression)
    {
      printf(" = (");

      m_expression.print();

      printf(")");
    }
}




}




