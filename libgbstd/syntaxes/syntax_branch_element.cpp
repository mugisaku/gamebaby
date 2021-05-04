#include"libgbstd/syntax.hpp"




namespace gbstd{




syntax_branch_element&
syntax_branch_element::
assign(const syntax_branch_element&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_token   = rhs.m_token;
      m_operand = rhs.m_operand;
      m_branch  = rhs.m_branch;
    }


  return *this;
}


syntax_branch_element&
syntax_branch_element::
assign(syntax_branch_element&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_token  ,rhs.m_token);
      std::swap(m_operand,rhs.m_operand);
      std::swap(m_branch ,rhs.m_branch);
    }


  return *this;
}


syntax_branch_element&
syntax_branch_element::
assign(const syntax_token&  tok, const syntax_operand&  o) noexcept
{
  clear();

  m_token = &tok;

  m_operand = o;

  return *this;
}


syntax_branch_element&
syntax_branch_element::
assign(const syntax_token&  tok, const syntax_definition&  def, syntax_branch&&  br) noexcept
{
  clear();

  m_token = &tok;

  m_operand.assign(def);

  m_branch = std::move(br);

  return *this;
}




void
syntax_branch_element::
clear() noexcept
{
  m_token = nullptr;

  m_operand.clear();

  m_branch.cut_back();
}


syntax_branch
syntax_branch_element::
cut_child() noexcept
{
  m_operand.clear();

  return std::move(m_branch);
}


void
syntax_branch_element::
print() const noexcept
{
    if(m_operand.is_definition())
    {
      gbstd::print(m_operand.get_definition().get_name());

      printf("{\n");

      m_branch.print();

      printf("}\n");
    }

  else
    {
      m_operand.print();
    }
}



}




