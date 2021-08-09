#include"libgbstd/vm.hpp"




namespace gbstd{




std::u16string_view
get_string(vm_opcode  o) noexcept
{
  return std::u16string_view((o == vm_opcode::nop)? u"nop"
                            :(o == vm_opcode::trf)? u"trf"
                            :(o == vm_opcode::add)? u"add"
                            :(o == vm_opcode::sub)? u"sub"
                            :(o == vm_opcode::mul)? u"mul"
                            :(o == vm_opcode::div)? u"div"
                            :(o == vm_opcode::rem)? u"rem"
                            :(o == vm_opcode::fadd)? u"fadd"
                            :(o == vm_opcode::fsub)? u"fsub"
                            :(o == vm_opcode::fmul)? u"fmul"
                            :(o == vm_opcode::fdiv)? u"fdiv"
                            :(o == vm_opcode::frem)? u"frem"
                            :(o == vm_opcode::eq)? u"eq"
                            :(o == vm_opcode::neq)? u"neq"
                            :(o == vm_opcode::lt)? u"lt"
                            :(o == vm_opcode::lteq)? u"lteq"
                            :(o == vm_opcode::gt)? u"gt"
                            :(o == vm_opcode::gteq)? u"gteq"
                            :(o == vm_opcode::feq)? u"feq"
                            :(o == vm_opcode::fneq)? u"fneq"
                            :(o == vm_opcode::flt)? u"flt"
                            :(o == vm_opcode::flteq)? u"flteq"
                            :(o == vm_opcode::fgt)? u"fgt"
                            :(o == vm_opcode::fgteq)? u"fgteq"
                            :(o == vm_opcode::shl)? u"shl"
                            :(o == vm_opcode::shr)? u"shr"
                            :(o == vm_opcode::and_)? u"and"
                            :(o == vm_opcode::or_)? u"or"
                            :(o == vm_opcode::xor_)? u"xor"
                            :(o == vm_opcode::not_)? u"not"
                            :(o == vm_opcode::logi_and)? u"logi_and"
                            :(o == vm_opcode::logi_or)? u"logi_or"
                            :(o == vm_opcode::logi_not)? u"logi_not"
                            :(o == vm_opcode::neg)? u"neg"
                            :(o == vm_opcode::fneg)? u"fneg"
                            :(o == vm_opcode::br)? u"br"
                            :(o == vm_opcode::jmp)? u"jmp"
                            :(o == vm_opcode::cal)? u"cal"
                            :(o == vm_opcode::ret)? u"ret"
                            :u"");
}




}




