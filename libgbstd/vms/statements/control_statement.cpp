#include"libgbstd/vm.hpp"




namespace gbstd{




void
control_statement::
print() const noexcept
{
  printf("%s",m_string.data());
}




}




