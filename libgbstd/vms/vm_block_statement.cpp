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
clear() noexcept
{
  basic_space::clear();

  m_statement_list.clear();
}


void
block_statement::
push_statement(statement&&  st) noexcept
{
  m_statement_list.emplace_back(std::move(st));
}


void
block_statement::
print() const noexcept
{
}




}




