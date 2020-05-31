#include"libgbstd/vm.hpp"




namespace gbstd{




void
basic_space::
clear() noexcept
{
  m_type_info_table.clear();
  m_memo_info_table.clear();

  m_function_table.clear();

  m_statement_list.clear();
}




function&
basic_space::
create_function(std::string_view  name, function_signature&&  sig) noexcept
{
//  return *m_function_table.emplace_back(std::make_unique<function>(m_node,name,std::move(sig)));
  return *m_function_table.emplace_back(new function(m_node,name,std::move(sig)));
}


const type_info*
basic_space::
find_type_info_by_name(std::string_view  name) const noexcept
{
    for(auto&  ti: m_type_info_table)
    {
        if(ti.get_name() == name)
        {
          return &ti;
        }
    }


  return nullptr;
}


const type_info*
basic_space::
find_type_info_by_id(std::string_view  id) const noexcept
{
    for(auto&  ti: m_type_info_table)
    {
        if(ti.get_id() == id)
        {
          return &ti;
        }
    }


  return nullptr;
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
    for(auto&  fn: m_function_table)
    {
        if(fn->get_name() == name)
        {
          return fn.get();
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


    if(m_memo_info_table.size())
    {
      printf("memo info table{\n");

        for(auto&  mi: m_memo_info_table)
        {
          mi->print();

          printf("\n");
        }


      printf("}\n");
    }


    if(m_function_table.size())
    {
      printf("function table{\n");

        for(auto&  fn: m_function_table)
        {
          fn->print();

          printf("\n");
        }


      printf("}\n");
    }


    if(m_statement_list.size())
    {
      printf("{\n");

        for(auto&  st: m_statement_list)
        {
          printf("  ");

          st.print();

          printf(";\n");
        }


      printf("}\n");
    }
}




}




