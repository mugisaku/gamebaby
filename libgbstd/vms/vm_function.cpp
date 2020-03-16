#include"libgbstd/vm.hpp"




namespace gbstd{




const type_info&
function::
update_pointer_type_info() noexcept
{
  function_signature  sig;

  sig.push(*m_return_type_info);

    for(auto&  para: m_parameter_list)
    {
      sig.push(para.get_type_info());
    }


  function_pointer_type_info  fnptr_ti(std::move(sig),type_infos::pointer_size);

  m_pointer_type_info = std::make_unique<type_info>(std::move(fnptr_ti));

  return *m_pointer_type_info;
}


void
function::
print(const context&  ctx) const noexcept
{
  auto&  tc = ctx.get_type_collection();

  auto  retti_ent = tc.find_entry(*m_return_type_info);

  printf("%s(",retti_ent->get_name().data());

    for(auto&  para: m_parameter_list)
    {
      auto  ti_ent = tc.find_entry(para.get_type_info());

      printf("%s  %s, ",ti_ent? ti_ent->get_name().data():"(no entry)",para.get_name().data());
    }


  printf(")\n{\n");
/*

    for(auto&  decl: m_declaration_list)
    {
      printf("  %s  %s\n",tc.find_entry(*decl.get_type_info())->get_name().data(),decl.get_name().data());
    }
*/

  printf("\n");

  int  i = 0;

/*
    for(auto&  cl: m_codelines)
    {
      printf("%4d  ",i++);

      cl.print(&m_context,this);

      printf("\n");
    }
*/


  printf("}\n\n\n");
}




}




