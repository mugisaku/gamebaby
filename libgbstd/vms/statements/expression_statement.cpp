#include"libgbstd/vm.hpp"




namespace gbstd{




void
expression_statement::
print() const noexcept
{
  m_expression.print();
}




}




