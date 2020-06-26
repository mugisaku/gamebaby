#include"libgbstd/vms/ir.hpp"




namespace gbstd{




ir_operation::
ir_operation(const ir_block_info&  bi, std::string_view  lb, std::string_view  instr, std::vector<ir_operand>&&  opls) noexcept:
m_block_info(&bi),
m_instruction(instr),
m_operand_list(opls)
{
  set_label(lb);
  set_kind(instr);
}




void
ir_operation::
set_kind(std::string_view  instr) noexcept
{
  m_kind = ((instr == std::string_view("add")) ||
            (instr == std::string_view("sub")) ||
            (instr == std::string_view("mul")) ||
            (instr == std::string_view("div")) ||
            (instr == std::string_view("rem")))? kinds::arithmetic

          :((instr == std::string_view("shl"))     ||
            (instr == std::string_view("shr"))     ||
            (instr == std::string_view("bit_or"))  ||
            (instr == std::string_view("bit_and")) ||
            (instr == std::string_view("bit_xor")) ||
            (instr == std::string_view("bit_not")))? kinds::bitwise

          :((instr == std::string_view("eq"))   ||
            (instr == std::string_view("neq"))  ||
            (instr == std::string_view("lt"))   ||
            (instr == std::string_view("lteq")) ||
            (instr == std::string_view("gt"))   ||
            (instr == std::string_view("gteq")))? kinds::comparison

          :((instr == std::string_view("ld8"))  ||
            (instr == std::string_view("ld16")) ||
            (instr == std::string_view("ld32")) ||
            (instr == std::string_view("ld64")))? kinds::load

          :((instr == std::string_view("st8"))  ||
            (instr == std::string_view("st16")) ||
            (instr == std::string_view("st32")) ||
            (instr == std::string_view("st64")))? kinds::store

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

      m_instruction = rhs.m_instruction;

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

      std::swap(m_block_info   ,rhs.m_block_info   );
      std::swap(m_label        ,rhs.m_label        );
      std::swap(m_label_length ,rhs.m_label_length );
      std::swap(m_instruction  ,rhs.m_instruction  );
      std::swap(m_kind         ,rhs.m_kind         );
      std::swap(m_operand_list ,rhs.m_operand_list );
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

  m_block_info = nullptr;

  m_instruction.clear();

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


  printf("%s",m_instruction.data());

    for(auto&  o: m_operand_list)
    {
      printf(" ");

      o.print();
    }
}


}




