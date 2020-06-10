#include"libgbstd/vm.hpp"




namespace gbstd{




compile_context&
compile_context::
assign(std::string_view  src_code)
{
  m_source = src_code;

  auto&  gsp = m_root_node.get_global_space();

  gsp.assign(src_code);

  gsp.compile(*this);

  return *this;
}


void
compile_context::
print() const noexcept
{
  auto&  gsp = m_root_node.get_global_space();

  gsp.print();

  printf("\n----\n");

  m_asm_context.print();
}




}




