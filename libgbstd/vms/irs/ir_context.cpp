#include"libgbstd/vms/ir.hpp"




namespace gbstd{




const ir_function*
ir_context::
find_function(std::string_view  label) const noexcept
{
    for(auto&  fn: m_function_list)
    {
        if(fn.get_name() == label)
        {
          return &fn;
        }
    }


  return nullptr;
}


void
ir_context::
print() const noexcept
{
    for(auto&  fn: m_function_list)
    {
      fn.print();

      printf("\n");
    }
}




}




