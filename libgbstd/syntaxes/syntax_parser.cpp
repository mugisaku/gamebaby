#include"libgbstd/syntax.hpp"




namespace gbstd{




void
syntax_parser::
process_by_expression(const syntax_expression&  expr) noexcept
{
}


void
syntax_parser::
start(std::u16string_view  def_name) noexcept
{
  auto  def = m_rule.find(def_name);

    if(def)
    {
      process_by_expression(*def);
    }
}




}




