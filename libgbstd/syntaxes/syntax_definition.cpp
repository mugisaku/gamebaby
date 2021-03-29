#include"libgbstd/syntax.hpp"




namespace gbstd{




void
syntax_definition::
print() const noexcept
{
    if(m_key)
    {
      printf("*");
    }


  gbstd::print(m_name);

  printf(" = ");

  syntax_expression::print();

  printf(";\n");
}


}




