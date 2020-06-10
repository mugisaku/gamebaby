#include"libgbstd/vm.hpp"




namespace gbstd{




void
block_statement::
print() const noexcept
{
  printf("{\n");

    for(auto&  st: m_space->get_statement_list())
    {
      st.print();

      printf("\n");
    }

  printf("}");
}




}




