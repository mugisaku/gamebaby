#include"libgbstd/vms/ir.hpp"




namespace gbstd{




void
ir_function::
print() const noexcept
{
  printf("function\n");

  m_return_type_info.print();

  printf("(");

    for(auto&  para: m_parameter_list)
    {
      printf(" ");

      para.type_info().print();

      printf("  ");

      gbstd::print(para.name());

      printf(",");
    }


  printf(")\n");

  gbstd::print(m_name.data());

  printf("\n{\n");

    for(auto&  st: m_statement_list)
    {
      printf("  ");

      st.print();

      printf("\n");
    }


  printf("}\n");
}




}




