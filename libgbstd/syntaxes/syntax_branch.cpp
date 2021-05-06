#include"libgbstd/syntax.hpp"




namespace gbstd{




syntax_branch::
syntax_branch(syntax_branch_element&&  e) noexcept
{
  m_elements.emplace_back(std::move(e));
}


int
syntax_branch::
length() const noexcept
{
  return m_elements.size();
}


void
syntax_branch::
cut_back(int  l) noexcept
{
  m_elements.resize(l);
}


void
syntax_branch::
splice(syntax_branch&&  rhs) noexcept
{
    for(auto&  e: rhs.m_elements)
    {
      m_elements.emplace_back(std::move(e));
    }
}


void
syntax_branch::
print() const noexcept
{
    for(auto&  e: m_elements)
    {
      e.print();

      printf(" ");
    }
}


const syntax_branch_element*  syntax_branch::begin() const noexcept{return m_elements.data();}
const syntax_branch_element*    syntax_branch::end() const noexcept{return m_elements.data()+m_elements.size();}


}




