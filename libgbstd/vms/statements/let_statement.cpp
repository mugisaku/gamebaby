#include"libgbstd/vm.hpp"




namespace gbstd{




void
let_statement::
compile(const space_node&  nd, compile_context&  ctx) const
{
  expression_statement::compile(nd,ctx);
}


void
let_statement::
print() const noexcept
{
  printf("let %s",m_target_name.data());

    if(get_expression())
    {
      printf(" = (");

      get_expression().print();

      printf(")");
    }
}




}




