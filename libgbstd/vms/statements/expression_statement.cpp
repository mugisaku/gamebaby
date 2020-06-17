#include"libgbstd/vm.hpp"




namespace gbstd{




void
expression_statement::
compile(const space_node&  nd, compile_context&  ctx) const
{
  compile_expression(get_expression(),nd,ctx);
}


void
expression_statement::
print() const noexcept
{
  m_expression.print();
}




}




