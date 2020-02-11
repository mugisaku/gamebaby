#include"libgbstd/vm.hpp"




namespace gbstd{




void
variable::
print() const noexcept
{
  printf("{var[%4" PRIu32 "]: {fn: \"%s\", name: \"%s\", value:",m_address.get(),m_function_name.data(),m_name.data());

  m_value.print();

  printf("}}\n");
}




}




