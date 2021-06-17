#include"libgbstd/syntax.hpp"




namespace gbstd{




syntax_branch_element&
syntax_branch_element::
assign(const syntax_branch_element&  rhs) noexcept
{
    if(this != &rhs)
    {
      m_token  = rhs.m_token;
      m_branch = rhs.m_branch;
    }


  return *this;
}


syntax_branch_element&
syntax_branch_element::
assign(syntax_branch_element&&  rhs) noexcept
{
    if(this != &rhs)
    {
      std::swap(m_token ,rhs.m_token );
      std::swap(m_branch,rhs.m_branch);
    }


  return *this;
}


syntax_branch_element&
syntax_branch_element::
assign(const syntax_token&  tok) noexcept
{
  m_token = &tok;

  return *this;
}


syntax_branch_element&
syntax_branch_element::
assign(syntax_branch&&  bra) noexcept
{
  m_token = nullptr;

  m_branch = std::move(bra);

  return *this;
}




void
syntax_branch_element::
print(int  indent) const noexcept
{
    for(int  n = indent;  n;  --n)
    {
      printf("  ");
    }


    if(is_token())
    {
      m_token->print();

      printf("\n");
    }

  else
    {
      m_branch.print(indent+1);
    }
}



}




