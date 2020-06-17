#include"libgbstd/vm.hpp"




namespace gbstd{




function::
function(space_node&  nd, std::string_view  name, function_signature&&  sig) noexcept:
m_node(nd),
m_name(name),
m_signature(std::move(sig)),
m_main_block(&nd.create_block_space())
{
    for(auto&  para: m_signature.get_parameter_list())
    {
      auto  vi = std::make_unique<variable_info>(variable_scopes::parameter,para.get_type_info(),para.get_name());

      m_parameter_variable_info_table.emplace_back(std::move(vi));
    }
}




const variable_info*
function::
find_parameter_variable_info(std::string_view  name) const noexcept
{
    for(auto&  vi: m_parameter_variable_info_table)
    {
        if(vi->get_name() == name)
        {
          return vi.get();
        }
    }


  return nullptr;
}


void
function::
compile(compile_context&  ctx) const
{
  auto  s = m_name.data();

  ctx->add_label("%s__begin",s);

  m_main_block->compile(ctx);

  ctx->add_label("%s__end",s);
}


void
function::
print() const noexcept
{
  printf("function %s = %s(",m_name.data(),m_signature.get_return_type_info().get_name().data());

    for(auto&  vi: m_parameter_variable_info_table)
    {
      vi->print();

      printf(", ");
    }


  printf(")\n{\n");

    if(m_main_block)
    {
      m_main_block->print();
    }


  printf("}\n\n\n");
}




}




