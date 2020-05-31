#include"libgbstd/vms/expression.hpp"




namespace gbstd{




struct
primary_expression::
data
{
  operand  m_operand;

  std::vector<primary_expression_element>  m_elements;

  data() noexcept{}
  data(const data&  rhs) noexcept:
  m_operand(rhs.m_operand),
  m_elements(rhs.m_elements){}

};




primary_expression&
primary_expression::
assign(const primary_expression&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

        if(rhs.m_data)
        {
          m_data = new data(*rhs.m_data);
        }
    }


  return *this;
}


primary_expression&
primary_expression::
assign(primary_expression&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_data,rhs.m_data);
    }


  return *this;
}


primary_expression&
primary_expression::
assign(operand&&  o, std::vector<primary_expression_element>&&  els) noexcept
{
  clear();

  m_data = new data;

  m_data->m_operand = std::move(o);

  m_data->m_elements = std::move(els);

  return *this;
}




primary_expression&
primary_expression::
clear() noexcept
{
  delete m_data          ;
         m_data = nullptr;

  return *this;
}


operand&
primary_expression::
get_operand() noexcept
{
  return m_data->m_operand;
}

std::vector<primary_expression_element>&
primary_expression::
get_elements() noexcept
{
  return m_data->m_elements;
}

const std::vector<primary_expression_element>&
primary_expression::
get_elements() const noexcept
{
  return m_data->m_elements;
}


simple_value
primary_expression::
evaluate() const noexcept
{
  simple_value  v = m_data->m_operand.evaluate();

    for(auto&  e: m_data->m_elements)
    {
    }


  return v;
}


void
primary_expression::
print() const noexcept
{
    if(m_data)
    {
      m_data->m_operand.print();

        for(auto&  e: m_data->m_elements)
        {
          e.print();
        }
    }
}




}




