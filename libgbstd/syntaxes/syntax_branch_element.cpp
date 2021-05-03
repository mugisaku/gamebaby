#include"libgbstd/syntax.hpp"




namespace gbstd{




syntax_branch_element&
syntax_branch_element::
assign(const syntax_branch_element&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_token  = rhs.m_token;
      m_data.o = rhs.m_data.o;

      m_child = clone(rhs.m_child);
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

      std::swap(m_token ,rhs.m_token);
      std::swap(m_data.o,rhs.m_data.o);
      std::swap(m_child ,rhs.m_child);
    }


  return *this;
}


syntax_branch_element&
syntax_branch_element::
assign(const syntax_token&  tok, const syntax_operand&  o) noexcept
{
  clear();

  m_token = &tok;
  m_data.o = &o;

  return *this;
}


syntax_branch_element&
syntax_branch_element::
assign(const syntax_token&  tok, std::unique_ptr<syntax_branch>&&  br, const syntax_definition&  def) noexcept
{
  clear();

  m_token = &tok;

  m_child = br.release();

  m_data.def = &def;

  return *this;
}




void
syntax_branch_element::
clear() noexcept
{
  m_token  = nullptr;
  m_data.o = nullptr;

  delete m_child          ;
         m_child = nullptr;
}


std::unique_ptr<syntax_branch>
syntax_branch_element::
cut_child() noexcept
{
  std::unique_ptr<syntax_branch>   u(m_child)         ;
                                     m_child = nullptr;

  m_data.def = nullptr;

  return std::move(u);
}


void
syntax_branch_element::
print() const noexcept
{
    if(m_child)
    {
      gbstd::print(m_data.def->get_name());

      printf("{\n");

      m_child->print();

      printf("}\n");
    }

  else
    if(m_data.o)
    {
      m_data.o->print();
    }
}



}




