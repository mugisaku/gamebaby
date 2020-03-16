#include"libgbstd/vm.hpp"




namespace gbstd{




const statement&
block_statement::
operator[](int  i) const noexcept
{
  static const statement  null;

  return (i < m_statement_list.size())? m_statement_list[i]:null;
}


void
block_statement::
push(statement&&  st) noexcept
{
    if(st.is_expression())
    {
      auto&  e = st.get_expression();

      m_symbol_table.create(*e.get_type_info(),e.get_name(),0,nullptr);
    }


  m_statement_list.emplace_back(std::move(st));
}




}




