#include"libgbstd/vm.hpp"




namespace gbstd{




void
jump_statement::
compile(compile_context&  ctx) const
{
  char  buf[64];

  snprintf(buf,sizeof(buf),"LABEL__%s",m_string.data());
}


void
jump_statement::
print() const noexcept
{
  printf("jump %s",m_string.data());
}




}




