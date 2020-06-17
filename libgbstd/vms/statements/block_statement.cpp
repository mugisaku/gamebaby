#include"libgbstd/vm.hpp"




namespace gbstd{




void
block_statement::
compile(compile_context&  ctx) const
{
  get_space().compile(ctx);
}


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




