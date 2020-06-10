#include"libgbstd/vm.hpp"




namespace gbstd{




void
jump_statement::
print() const noexcept
{
  printf("jump %s",m_string.data());
}




}




