#include"libgbstd/vm.hpp"




namespace gbstd{




void
block_statement::
push(statement&&  st) noexcept
{
    if(st.is_expression())
    {
      auto&  e = st.get_expression();

      m_symbol_table.create(*e.get_type_info(),e.get_name(),0,m_function);
    }


  m_statement_list.emplace_back(std::make_unique<statement>(std::move(st)));
}




}




