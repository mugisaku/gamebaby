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



syntax_branch&
syntax_branch::
append(syntax_branch&&   br) noexcept
{
    if(br.has_name())
    {
      m_elements.emplace_back(std::move(br));
    }

  else
    {
        for(auto&  e: br)
        {
          m_elements.emplace_back(std::move(e));
        }
    }


  return *this;
}


syntax_branch&
syntax_branch::
append(syntax_element&&  el) noexcept
{
  m_elements.emplace_back(std::move(el));

  return *this;
}


void
syntax_branch::
print(int  indent) const noexcept
{
    if(has_name())
    {
      gbstd::print(m_name);

      printf("\n");
    }


    for(auto&  e: m_elements)
    {
      e.print(indent);
    }
}


const syntax_branch_element*  syntax_branch::begin() const noexcept{return m_elements.data();}
const syntax_branch_element*    syntax_branch::end() const noexcept{return m_elements.data()+m_elements.size();}


}




