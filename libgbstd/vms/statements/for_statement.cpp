#include"libgbstd/vm.hpp"




namespace gbstd{




void
for_statement::
compile(const space_node&  nd, compile_context&  ctx) const
{
  auto  s = ctx.enter_for_block();

  auto  ti = compile_expression(m_init,nd,ctx);

  char  buf[64];

  snprintf(buf,sizeof(buf),"%s__cond",s);

  ctx->add_label("%s",buf);

    if(m_cond)
    {
      compile_expression(m_cond,nd,ctx);
    }


  block_statement::compile(ctx);


  compile_expression(m_loop,nd,ctx);

  ctx.leave_control_block();
}


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




