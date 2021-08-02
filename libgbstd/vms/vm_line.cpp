#include"libgbstd/vm.hpp"
#include<cinttypes>




namespace gbstd{




void
vm_line::
print() const noexcept
{
  printf("%4d| ",m_position);

  gbstd::print(get_string(m_opcode));

  printf(" ");

    switch(m_opcode)
    {
  case(vm_opcode::nop): break;

  case(vm_opcode::add):
  case(vm_opcode::sub):
  case(vm_opcode::mul):
  case(vm_opcode::div):
  case(vm_opcode::rem):
  case(vm_opcode::shl):
  case(vm_opcode::shr):
  case(vm_opcode::and_):
  case(vm_opcode::or_):
  case(vm_opcode::xor_):
  case(vm_opcode::eq):
  case(vm_opcode::neq):
  case(vm_opcode::lt):
  case(vm_opcode::lteq):
  case(vm_opcode::gt):
  case(vm_opcode::gteq):
  case(vm_opcode::logi_and):
  case(vm_opcode::logi_or):
  case(vm_opcode::logi_not):
      gbstd::print(destination());

      printf(" ");

      left_source().print();

      printf(" ");

      right_source().print();
      break;
  case(vm_opcode::not_):
      break;
  case(vm_opcode::neg):
      break;
  case(vm_opcode::ld):
      gbstd::print(destination());

      printf(" ");

      left_source().print();
      break;
  case(vm_opcode::st):
      break;
  case(vm_opcode::cpy):
      break;
  case(vm_opcode::adr):
      break;
  case(vm_opcode::cal):
        {
          gbstd::print(destination());

          printf(" ");

          auto  it     = m_operand_list.begin();
          auto  it_end = m_operand_list.end();

          it++->print();

          printf("(");

            if(it != it_end)
            {
              it++->print();

                while(it != it_end)
                {
                  printf(", ");

                  it++->print();
                }
            }


          printf(")");
        }
      break;
  case(vm_opcode::ret):
      left_source().print();
      break;
  case(vm_opcode::br):
      break;
  case(vm_opcode::jmp):
      break;
    }
}




}




