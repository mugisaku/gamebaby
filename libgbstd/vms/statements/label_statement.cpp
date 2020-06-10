#include"libgbstd/vm.hpp"




namespace gbstd{




void
label_statement::
print() const noexcept
{
  printf("label:%s",m_string.data());
}




}




