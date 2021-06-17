#include"libgbstd/syntax.hpp"




namespace gbstd{




syntax_formula&
syntax_formula::
assign(const syntax_formula&  rhs) noexcept
{
    if(this != &rhs)
    {
      m_name = rhs.m_name;
      m_code = rhs.m_code;

      m_left  = rhs.m_left ? std::make_unique<syntax_element>(*m_left ):nullptr;
      m_right = rhs.m_right? std::make_unique<syntax_element>(*m_right):nullptr;
    }


  return *this;
}


syntax_formula&
syntax_formula::
assign(syntax_formula&&  rhs) noexcept
{
    if(this != &rhs)
    {
      std::swap(m_name,rhs.m_name);

      m_code = rhs.m_code;

      m_left  = std::move(rhs.m_left );
      m_right = std::move(rhs.m_right);
    }


  return *this;
}


syntax_formula&
syntax_formula::
assign(int  c, syntax_element&&  l, syntax_element&&  r) noexcept
{
  m_code = c;

  m_left  = std::make_unique<syntax_element>(std::move(l));
  m_right = std::make_unique<syntax_element>(std::move(r));

  return *this;
}


syntax_formula&
syntax_formula::
assign(syntax_element&&  o) noexcept
{
  m_code = 0;

  m_left = std::make_unique<syntax_element>(std::move(o));

  m_right.reset();

  return *this;
}




syntax_formula&
syntax_formula::
set_left(syntax_element&&  o) noexcept
{
  m_left = std::make_unique<syntax_element>(std::move(o));

  return *this;
}


syntax_formula&
syntax_formula::
set_right(syntax_element&&  o) noexcept
{
  m_right = std::make_unique<syntax_element>(std::move(o));

  return *this;
}




void
syntax_formula::
print_for_test() const noexcept
{
    if(m_left ){printf("has left\n");}
    if(m_code ){printf("has code\n");}
    if(m_right){printf("has right\n");}
}


void
syntax_formula::
print() const noexcept
{
    if(m_name.size())
    {
      gbstd::print(m_name);

      printf(" = ");
    }


    if(m_left)
    {
      m_left->print();

        if(m_right)
        {
          printf(" %c ",m_code);

          m_right->print();
        }
    }
}


}




