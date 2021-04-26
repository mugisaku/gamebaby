#include"libgbstd/vms/expression.hpp"




namespace gbstd{




expression&
expression::
assign(const expression&   rhs) noexcept
{
    if(this != &rhs)
    {
      m_left.reset( clone(rhs.m_left.get() ));
      m_right.reset(clone(rhs.m_right.get()));

      m_binary_operator = rhs.m_binary_operator;
    }


  return *this;
}


expression&
expression::
assign(expression&&  rhs) noexcept
{
    if(this != &rhs)
    {
      std::swap(m_left ,rhs.m_left );
      std::swap(m_right,rhs.m_right);

      m_binary_operator = rhs.m_binary_operator;
    }


  return *this;
}


expression&
expression::
assign(operand&&  l) noexcept
{
  m_left = std::make_unique<operand>(std::move(l));

  m_right.reset();

  m_binary_operator = small_string();

  return *this;
}


expression&
expression::
assign(operand&&  l, operand&&  r, small_string  binop) noexcept
{
  m_left  = std::make_unique<operand>(std::move(l));
  m_right = std::make_unique<operand>(std::move(r));

  m_binary_operator = binop;

  return *this;
}


simple_object
expression::
evaluate() const noexcept
{
    if(m_left)
    {
      auto  lo = m_left->evaluate();

        if(m_right)
        {
          auto  ro = m_right->evaluate();
        }


      return std::move(lo);
    }


  return simple_object();
}


void
expression::
print() const noexcept
{
    if(m_left)
    {
      m_left->print();

        if(m_right)
        {
          printf("%s",m_binary_operator.data());

          m_right->print();
        }
    }
}




}




