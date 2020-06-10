#include"libgbstd/vms/assembly.hpp"




namespace gbstd{




void
asm_line::
print() const noexcept
{
  static const std::pair<asm_opcode,std::string_view>  table[] = {
    std::make_pair(asm_opcode::nop,std::string_view("nop")),
    std::make_pair(asm_opcode::addi,std::string_view("addi")),
    std::make_pair(asm_opcode::addf,std::string_view("addf")),
    std::make_pair(asm_opcode::subi,std::string_view("subi")),
    std::make_pair(asm_opcode::subf,std::string_view("subf")),
    std::make_pair(asm_opcode::muli,std::string_view("muli")),
    std::make_pair(asm_opcode::mulf,std::string_view("mulf")),
    std::make_pair(asm_opcode::divi,std::string_view("divi")),
    std::make_pair(asm_opcode::divf,std::string_view("divf")),
    std::make_pair(asm_opcode::rem,std::string_view("rem")),
    std::make_pair(asm_opcode::shl,std::string_view("shl")),
    std::make_pair(asm_opcode::shr,std::string_view("shr")),
    std::make_pair(asm_opcode::band,std::string_view("band")),
    std::make_pair(asm_opcode::bor,std::string_view("bor")),
    std::make_pair(asm_opcode::bxor,std::string_view("bxor")),
    std::make_pair(asm_opcode::eq,std::string_view("eq")),
    std::make_pair(asm_opcode::neq,std::string_view("neq")),
    std::make_pair(asm_opcode::lti,std::string_view("lti")),
    std::make_pair(asm_opcode::ltf,std::string_view("ltf")),
    std::make_pair(asm_opcode::lteqi,std::string_view("lteqi")),
    std::make_pair(asm_opcode::lteqf,std::string_view("lteqf")),
    std::make_pair(asm_opcode::gti,std::string_view("gti")),
    std::make_pair(asm_opcode::gtf,std::string_view("gtf")),
    std::make_pair(asm_opcode::gteqi,std::string_view("gteqi")),
    std::make_pair(asm_opcode::gteqf,std::string_view("gteqf")),
    std::make_pair(asm_opcode::land,std::string_view("land")),
    std::make_pair(asm_opcode::lor,std::string_view("lor")),
    std::make_pair(asm_opcode::bnot,std::string_view("bnot")),
    std::make_pair(asm_opcode::lnot,std::string_view("lnot")),
    std::make_pair(asm_opcode::negi,std::string_view("negi")),
    std::make_pair(asm_opcode::negf,std::string_view("negf")),

    std::make_pair(asm_opcode::setpc,std::string_view("setpc")),
    std::make_pair(asm_opcode::pshpc,std::string_view("pshpc")),
    std::make_pair(asm_opcode::addpc,std::string_view("addpc")),

    std::make_pair(asm_opcode::setbp,std::string_view("setbp")),
    std::make_pair(asm_opcode::pshbp,std::string_view("pshbp")),
    std::make_pair(asm_opcode::addbp,std::string_view("addbp")),

    std::make_pair(asm_opcode::setsp,std::string_view("setsp")),
    std::make_pair(asm_opcode::pshsp,std::string_view("pshsp")),
    std::make_pair(asm_opcode::addsp,std::string_view("addsp")),

    std::make_pair(asm_opcode::br,std::string_view("br")),

    std::make_pair(asm_opcode::pshz,std::string_view("pshz")),
    std::make_pair(asm_opcode::pshnz,std::string_view("pshnz")),
    std::make_pair(asm_opcode::pshi8,std::string_view("pshi8")),
    std::make_pair(asm_opcode::pshi16,std::string_view("pshi16")),
    std::make_pair(asm_opcode::pshi32,std::string_view("pshi32")),
    std::make_pair(asm_opcode::pshi64,std::string_view("pshi64")),

    std::make_pair(asm_opcode::ld8,std::string_view("ld8")),
    std::make_pair(asm_opcode::ld16,std::string_view("ld16")),
    std::make_pair(asm_opcode::ld32,std::string_view("ld32")),
    std::make_pair(asm_opcode::ld64,std::string_view("ld64")),

    std::make_pair(asm_opcode::sti8,std::string_view("sti8")),
    std::make_pair(asm_opcode::sti16,std::string_view("sti16")),
    std::make_pair(asm_opcode::sti32,std::string_view("sti32")),
    std::make_pair(asm_opcode::sti64,std::string_view("sti64")),
    std::make_pair(asm_opcode::stf32,std::string_view("stf32")),
    std::make_pair(asm_opcode::stf64,std::string_view("stf64")),

    std::make_pair(asm_opcode::pop,std::string_view("pop")),
    std::make_pair(asm_opcode::dup,std::string_view("dup")),
    std::make_pair(asm_opcode::swp,std::string_view("swp")),

    std::make_pair(asm_opcode::i2f,std::string_view("i2f")),
    std::make_pair(asm_opcode::f2i,std::string_view("f2i")),
  };


  printf("%04" PRIi64 " ",m_number);

    for(auto&  p: table)
    {
        if(p.first == m_opcode)
        {
          printf("%s",p.second.data());

          break;
        }
    }
}




}




