#include"libgbstd/syntax.hpp"




namespace gbstd{




void
syntax_definition::
print() const noexcept
{
  printf("%s = ",m_name.data());

  syntax_group::print();

  printf(";\n");
}


}




