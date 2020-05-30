#include"libgbstd/vms/expression.hpp"




namespace gbstd{




unary_expression&
unary_expression::
assign(std::vector<operator_code>&&  ops, primary_expression&&  e) noexcept
{
  m_operator_code_list = std::move(ops);

  m_primary_expression = std::move(e);

  return *this;
}


unary_expression&
unary_expression::
clear() noexcept
{
  m_operator_code_list.clear();

  m_primary_expression.clear();

  return *this;
}


simple_value
unary_expression::
evaluate() const noexcept
{
  simple_value  v = m_primary_expression.evaluate();

  auto  it     = m_operator_code_list.rbegin();
  auto  it_end = m_operator_code_list.rend();

    while(it != it_end)
    {
      auto&  opco = *it++;

           if(opco == operator_code("+")){;}
      else if(opco == operator_code("-")){v = -v;}
      else if(opco == operator_code("*")){;}
      else if(opco == operator_code("&")){;}
      else if(opco == operator_code("~")){v = ~v;}
      else if(opco == operator_code("!")){v = v;}
    }


  return v;
}


void
unary_expression::
print() const noexcept
{
    for(auto  op: m_operator_code_list)
    {
      printf("%s",op.get_string());
    }


  m_primary_expression.print();
}




}




