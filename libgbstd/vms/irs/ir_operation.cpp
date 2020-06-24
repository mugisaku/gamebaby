#include"libgbstd/vms/ir.hpp"




namespace gbstd{




ir_operation::
ir_operation(const ir_block_info&  bi, std::string_view  lb, operator_code  op, std::vector<ir_operand>&&  opls) noexcept:
m_block_info(&bi),
m_operator_code(op),
m_operand_list(opls)
{
  set_label(lb);
  set_kind(op);
}




void
ir_operation::
set_kind(operator_code  opco) noexcept
{
  m_kind = ((opco == operator_code("add")) ||
            (opco == operator_code("sub")) ||
            (opco == operator_code("mul")) ||
            (opco == operator_code("div")) ||
            (opco == operator_code("rem")))? kinds::arithmetic

          :((opco == operator_code("shl"))     ||
            (opco == operator_code("shr"))     ||
            (opco == operator_code("bit_or"))  ||
            (opco == operator_code("bit_and")) ||
            (opco == operator_code("bit_xor")) ||
            (opco == operator_code("bit_not")))? kinds::bitwise

          :((opco == operator_code("eq"))   ||
            (opco == operator_code("neq"))  ||
            (opco == operator_code("lt"))   ||
            (opco == operator_code("lteq")) ||
            (opco == operator_code("gt"))   ||
            (opco == operator_code("gteq")))? kinds::comparison

          :((opco == operator_code("ld8"))  ||
            (opco == operator_code("ld16")) ||
            (opco == operator_code("ld32")) ||
            (opco == operator_code("ld64")))? kinds::load

          :((opco == operator_code("st8"))  ||
            (opco == operator_code("st16")) ||
            (opco == operator_code("st32")) ||
            (opco == operator_code("st64")))? kinds::store

          : kinds::others
          ;
}


ir_operation&
ir_operation::
assign(const ir_operation&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_block_info = rhs.m_block_info;

      set_label(rhs.get_label());

      m_operator_code = rhs.m_operator_code;

      m_kind = rhs.m_kind;

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

      std::swap(m_block_info,rhs.m_block_info);

//printf("%p  %p\n",m_block_info,rhs.m_block_info);

      std::swap(m_label       ,rhs.m_label       );
      std::swap(m_label_length,rhs.m_label_length);

      std::swap(m_operator_code,rhs.m_operator_code);

      std::swap(m_kind,rhs.m_kind);

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

//  m_block_info = nullptr;

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




