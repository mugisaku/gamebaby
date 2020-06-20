#include"libgbstd/vms/ir.hpp"




namespace gbstd{




ir_function&
ir_context::
create_function(std::string_view  fn_name, std::vector<ir_parameter>&&  parals) noexcept
{
  return *m_function_list.emplace_back(std::make_unique<ir_function>(fn_name,std::move(parals)));
}


const ir_function*
ir_context::
find_function(std::string_view  label) const noexcept
{
    for(auto&  fn: m_function_list)
    {
        if(fn->get_name() == label)
        {
          return fn.get();
        }
    }


  return nullptr;
}


void
ir_context::
finalize() noexcept
{
    for(auto&  fn: m_function_list)
    {
      fn->finalize();
    }
}


void
ir_context::
print() const noexcept
{
    for(auto&  fn: m_function_list)
    {
      fn->print();

      printf("\n");
    }
}




}




