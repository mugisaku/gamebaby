#include"libgbstd/vm.hpp"




namespace gbstd{




void
jump_statement::
compile(compile_context&  ctx) const
{
}


void
jump_statement::
print() const noexcept
{
  printf("jump %s",m_string.data());
}




}




