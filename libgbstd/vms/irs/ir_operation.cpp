#include"libgbstd/vms/ir.hpp"




namespace gbstd{




ir_operation&
ir_operation::
assign(const ir_operation&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      set_label(rhs.get_label());

      m_operator_code = rhs.m_operator_code;

      m_operand_list = rhs.m_operand_list;
    }


  return *this;
}


ir_operation&
ir_operation::
assign(ir_operation&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_label       ,rhs.m_label       );
      std::swap(m_label_length,rhs.m_label_length);

      std::swap(m_operator_code,rhs.m_operator_code);

      std::swap(m_operand_list,rhs.m_operand_list);
    }


  return *this;
}




void
ir_operation::
clear() noexcept
{
  free(m_label)         ;
       m_label = nullptr;

  m_label_length = 0;

  m_operator_code.clear();

  m_operand_list.clear();
}


ir_operation&
ir_operation::
set_label(std::string_view  sv) noexcept
{
  auto  len = sv.size();

    if(m_label_length < len)
    {
      free(m_label)                      ;
           m_label = (char*)malloc(len+1);

        if(!m_label)
        {
          report;

          len = 0;
        }
    }


  m_label_length = len;

  auto  dst = m_label;
  auto  src = sv.data();

    while(len--)
    {
      *dst++ = *src++;
    }


  *dst = 0;

  return *this;
}


void
ir_operation::
print() const noexcept
{
    if(m_label_length)
    {
      printf("%s = ",m_label);
    }


  printf("%s",m_operator_code.get_string());

    for(auto&  o: m_operand_list)
    {
      printf(" ");

      o.print();
    }
}


}




