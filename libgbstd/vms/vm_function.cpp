#include"libgbstd/vm.hpp"




namespace gbstd{




function::
function(space_node&  nd, std::string_view  name, function_signature&&  sig) noexcept:
m_node(nd),
m_name(name),
m_signature(std::move(sig))
{
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
print() const noexcept
{
  printf("function %s = ",m_name.data());

  m_signature.print();

  printf("\n{\n");

    if(m_main_block)
    {
      m_main_block->print();
    }


  printf("}\n\n\n");
}




}




