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


function*
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


void
space::
print() const noexcept
{
    for(auto&  f: m_function_table)
    {
      f->print();

      printf("\n");
    }
}




}




