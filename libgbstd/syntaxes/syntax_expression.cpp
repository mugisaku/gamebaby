#include"libgbstd/syntax.hpp"




namespace gbstd{




syntax_expression&
syntax_expression::
assign(const syntax_expression&  rhs) noexcept
{
    if(this != &rhs)
    {
      m_code = rhs.m_code;

      m_left  = rhs.m_left ? std::make_unique<syntax_operand>(*m_left ):nullptr;
      m_right = rhs.m_right? std::make_unique<syntax_operand>(*m_right):nullptr;
    }


  return *this;
}


syntax_expression&
syntax_expression::
assign(syntax_expression&&  rhs) noexcept
{
    if(this != &rhs)
    {
      m_code = rhs.m_code;

      m_left  = std::move(rhs.m_left );
      m_right = std::move(rhs.m_right);
    }


  return *this;
}


syntax_expression&
syntax_expression::
assign(int  c, syntax_operand&&  l, syntax_operand&&  r) noexcept
{
  m_code = c;

  m_left  = std::make_unique<syntax_operand>(std::move(l));
  m_right = std::make_unique<syntax_operand>(std::move(r));

  return *this;
}


syntax_expression&
syntax_expression::
assign(syntax_operand&&  o) noexcept
{
  m_code = 0;

  m_left = std::make_unique<syntax_operand>(std::move(o));

  m_right.reset();

  return *this;
}




syntax_expression&
syntax_expression::
set_left(syntax_operand&&  o) noexcept
{
  m_left = std::make_unique<syntax_operand>(std::move(o));

  return *this;
}


syntax_expression&
syntax_expression::
set_right(syntax_operand&&  o) noexcept
{
  m_right = std::make_unique<syntax_operand>(std::move(o));

  return *this;
}




void
syntax_expression::
print_for_test() const noexcept
{
    if(m_left ){printf("has left\n");}
    if(m_code ){printf("has code\n");}
    if(m_right){printf("has right\n");}
}


void
syntax_expression::
print() const noexcept
{
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




