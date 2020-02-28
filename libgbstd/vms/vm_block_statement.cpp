#include"libgbstd/vm.hpp"




namespace gbstd{




void
block_statement::
push(statement&&  st) noexcept
{
    if(st.is_expression())
    {
      auto&  e = st.get_expression();

      m_declaration_list.emplace_back(*e.get_type_info(),e.get_name());
    }


  m_statement_list.emplace_back(std::make_unique<statement>(std::move(st)));
}


const declaration*
block_statement::
find_declaration(std::string_view  name) const noexcept
{
    for(auto&  decl: m_declaration_list)
    {
        if(decl.get_name() == name)
        {
          return &decl;
        }
    }


  return nullptr;
}


}




