#include"libgbstd/vm.hpp"




namespace gbstd{




function&
space::
create_function(std::string_view  name) noexcept
{
  auto  ptr = new function(*this,name);

  m_function_table.emplace_back(ptr);

  return *ptr;
}


const function*
space::
find_function(std::string_view  name) const noexcept
{
    for(auto&  f: m_function_table)
    {
        if(f->get_name() == name)
        {
          return f.get();
        }
    }


  return nullptr;
}


value
space::
evaluate(std::string_view  name) const noexcept
{
/*
    for(auto&  o: m_operation_table)
    {
        if(o->get_name() == name)
        {
          return o->evaluate(*this);
        }
    }
*/

  return value::make_null();
}




}




