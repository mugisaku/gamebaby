#include"libgbstd/vm.hpp"




namespace gbstd{




void
if_statement::
compile(const space_node&  nd, compile_context&  ctx, const char*  base, int  number) const
{
  ctx->add_label("%s_%04d__begin",base,number);

  auto  ti = compile_expression(m_expression,nd,ctx);

//  ctx->add_line(asm_opcode::brz,"%s_%04d__end",base,number);

  get_space().compile(ctx);

//  ctx->add_line(asm_opcode::setpc,"%s__end",base);

  ctx->add_label("%s_%04d__end",base,number);
}


void
if_statement::
print() const noexcept
{
  printf("if(");

  m_expression.print();

  printf(")");

  block_statement::print();
}




}




