#include"libgbstd/vms/expression.hpp"




namespace gbstd{




expression_element&
expression_element::
assign(unary_expression&&  e) noexcept
{
  m_operator_code = operator_code();

  m_expression = std::move(e);

  return *this;
}


expression_element&
expression_element::
assign(operator_code  op, unary_expression&&  e) noexcept
{
  m_operator_code = op;

  m_expression = std::move(e);

  return *this;
}


void
expression_element::
print() const noexcept
{
  printf("%s",m_operator_code.get_string());

  m_expression.print();
}




}




