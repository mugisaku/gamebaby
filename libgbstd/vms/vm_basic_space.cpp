#include"libgbstd/vm.hpp"




namespace gbstd{




void
basic_space::
clear() noexcept
{
  m_type_info_table.clear();
  m_memo_info_table.clear();

  m_function_reference_table.clear();
}




type_info
basic_space::
find_type_info_by_name(std::string_view  name) const noexcept
{
    for(auto&  ti: m_type_info_table)
    {
        if(ti.get_name() == name)
        {
          return ti;
        }
    }


  return type_info();
}


type_info
basic_space::
find_type_info_by_id(std::string_view  id) const noexcept
{
    for(auto&  ti: m_type_info_table)
    {
        if(ti.get_id() == id)
        {
          return ti;
        }
    }


  return type_info();
}


const memo_info*
basic_space::
find_memo_info(std::string_view  name) const noexcept
{
    for(auto&  mi: m_memo_info_table)
    {
        if(mi->get_name() == name)
        {
          return mi.get();
        }
    }


  return nullptr;
}


const function*
basic_space::
find_function(std::string_view  name) const noexcept
{
    for(auto&  fnref: m_function_reference_table)
    {
      auto&  fn = fnref.get();

        if(fn.get_name() == name)
        {
          return &fn;
        }
    }


  return nullptr;
}




void
basic_space::
print() const noexcept
{
    if(m_type_info_table.size())
    {
      printf("type info table{\n");

        for(auto&  ti: m_type_info_table)
        {
          ti.print();

          printf("\n");
        }


      printf("}\n");
    }


    if(m_function_reference_table.size())
    {
      printf("function table{\n");

        for(auto&  fn: m_function_reference_table)
        {
          fn.get().print();

          printf("\n");
        }


      printf("}\n");
    }
}




}




