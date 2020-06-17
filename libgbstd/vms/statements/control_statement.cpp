#include"libgbstd/vm.hpp"




namespace gbstd{




void
control_statement::
compile(compile_context&  ctx) const
{
  
}


void
control_statement::
print() const noexcept
{
  printf("%s",m_string.data());
}




}




