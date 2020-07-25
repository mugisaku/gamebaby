#include"libgbstd/vm.hpp"




namespace gbstd{




function::
function(token_iterator&  it, std::string_view  name, function_signature&&  sig): ast_node(it,"function")
m_name(name),
m_signature(std::move(sig)),
m_main_block(&nd.create_block_space())
{
    for(auto&  para: m_signature.get_parameter_list())
    {
      auto  vi = std::make_unique<variable_info>(variable_scopes::parameter,para.get_type_info(),para.get_name());

      m_parameter_variable_info_table.emplace_back(std::move(vi));
    }


  read(it);
}




void
function::
read(token_iterator&  it)
{
  
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
  ctx.start_function_block(*this);

  m_main_block->compile(ctx);

  ctx.finish_block();
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




