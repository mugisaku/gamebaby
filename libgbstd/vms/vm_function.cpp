#include"libgbstd/vm.hpp"




namespace gbstd{




function::
function(local_space&  lsp, std::string_view  name, const type_info&  retti, parameter_list&&  parals) noexcept:
m_local_space(&lsp),
m_name(name),
m_return_type_info(&retti)
{
  update_pointer_type_info();
}




const type_info&
function::
update_pointer_type_info() noexcept
{
  function_signature  sig;
/*
  sig.push(*m_return_type_info);


  function_pointer_type_info  fnptr_ti(std::move(sig),type_infos::pointer_size);

  m_pointer_type_info = std::make_unique<type_info>(std::move(fnptr_ti));
*/
  return *m_pointer_type_info;
}


void
function::
print() const noexcept
{
/*
  auto&  tc = m_block->get_type_collection();

  auto  retti_ent = tc.find_entry(*m_return_type_info);

  printf("%s(",retti_ent? retti_ent->get_name().data():"unknown type");

  m_parameter_symbol_table.print();

  printf(")\n{\n");

  m_block.print();

  printf("}\n\n\n");
*/
}




}




