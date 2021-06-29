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

      m_keyword_flag = rhs.m_keyword_flag;
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

      std::swap(m_keyword_flag,rhs.m_keyword_flag);
    }


  return *this;
}


syntax_branch_element&
syntax_branch_element::
assign(const syntax_token&  tok, bool  k) noexcept
{
  m_token = &tok;

  m_keyword_flag = k && tok.is_identifier();

  return *this;
}


syntax_branch_element&
syntax_branch_element::
assign(syntax_branch&&  bra) noexcept
{
  m_token = nullptr;

  m_branch = std::move(bra);

  m_keyword_flag = false;

  return *this;
}




void
syntax_branch_element::
print(int  indent) const noexcept
{
    for(int  n = 0;  n < indent;  ++n)
    {
      printf("|__");
    }


    if(is_token())
    {
        if(is_keyword())
        {
          printf("KEYWORD,");
        }


      m_token->print();

      printf("\n");
    }

  else
    {
      m_branch.print(indent+1);
    }
}



}




