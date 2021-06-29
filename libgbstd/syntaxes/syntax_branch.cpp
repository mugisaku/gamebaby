#include"libgbstd/syntax.hpp"




namespace gbstd{




syntax_branch::
syntax_branch(syntax_branch_element&&  e) noexcept
{
  m_elements.emplace_back(std::move(e));
}


const syntax_branch_element&
syntax_branch::
operator[](int  i) const noexcept
{
  static const syntax_branch_element  null;

  return (i < m_elements.size())? m_elements[i]:null;
}


int
syntax_branch::
length() const noexcept
{
  return m_elements.size();
}



syntax_branch&
syntax_branch::
append(syntax_branch_element&&  el) noexcept
{
  m_elements.emplace_back(std::move(el));

  return *this;
}


syntax_branch&
syntax_branch::
append(std::vector<syntax_branch_element>&&  els) noexcept
{
    for(auto&  el: els)
    {
      m_elements.emplace_back(std::move(el));
    }


  els.clear();

  return *this;
}




void
syntax_branch::
print(int  indent) const noexcept
{
    if(has_name())
    {
      printf("<");

      gbstd::print(m_name);

      printf(">\n");
    }

  else
    {
      printf("*NO NAME*\n");
    }


    for(auto&  e: m_elements)
    {
      e.print(indent);
    }
}


const syntax_branch_element*  syntax_branch::begin() const noexcept{return m_elements.data();}
const syntax_branch_element*    syntax_branch::end() const noexcept{return m_elements.data()+m_elements.size();}


}




