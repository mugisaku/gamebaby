#include"libgbstd/vm.hpp"




namespace gbstd{




void
return_statement::
compile(const space_node&  nd, compile_context&  ctx) const
{
  ctx.write_local("void retv ");

  expression_statement::compile(nd,ctx);
}


void
return_statement::
print() const noexcept
{
  printf("return ");

  get_expression().print();
}




}




