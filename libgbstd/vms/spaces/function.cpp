#include"libgbstd/vm.hpp"




namespace gbstd{




function::
function(space_node&  nd, std::string_view  name, function_signature&&  sig) noexcept:
m_node(nd),
m_name(name),
m_signature(std::move(sig)),
m_main_block(std::make_unique<block_space>(nd.create_child()))
{
    for(auto&  para: m_signature.get_parameter_list())
    {
      auto  mi = std::make_unique<memo_info>(para.get_type_info(),para.get_name());

      m_parameter_memo_info_table.emplace_back(std::move(mi));
    }
}




const memo_info*
function::
find_parameter_memo_info(std::string_view  name) const noexcept
{
    for(auto&  mi: m_parameter_memo_info_table)
    {
        if(mi->get_name() == name)
        {
          return mi.get();
        }
    }


  return nullptr;
}


void
function::
allocate_address(address_t&  global_end) noexcept
{
  address_t  local_end = 0;

  m_operation_stack_size = 0;

    for(auto&  child: m_node.get_children())
    {
        if(child->is_block_space())
        {
          child->get_block_space().allocate_address(global_end,local_end,m_operation_stack_size);
        }
    }


    for(auto&  mi: m_parameter_memo_info_table)
    {
      local_end = mi->set_address(local_end);
    }
}


void
function::
print() const noexcept
{
  printf("function %s = %s(",m_name.data(),m_signature.get_return_type_info().get_name().data());

    for(auto&  mi: m_parameter_memo_info_table)
    {
      mi->print();

      printf(", ");
    }


  printf(")\noperation stack size: %d\n{\n",m_operation_stack_size);

    if(m_main_block)
    {
      m_main_block->print();
    }


  printf("}\n\n\n");
}




}




