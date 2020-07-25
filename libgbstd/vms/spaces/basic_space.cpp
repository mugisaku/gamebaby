#include"libgbstd/vm.hpp"




namespace gbstd{




void
basic_space::
clear() noexcept
{
  m_type_info_table.clear();
  m_variable_info_table.clear();

  m_statement_list.clear();
}




type_info
basic_space::
find_type_info(std::string_view  name) const noexcept
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


const variable_info*
basic_space::
find_variable_info(std::string_view  name) const noexcept
{
    for(auto&  vi: m_variable_info_table)
    {
        if(vi->get_name() == name)
        {
          return vi.get();
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


    if(m_variable_info_table.size())
    {
      printf("variable info table{\n");

        for(auto&  vi: m_variable_info_table)
        {
          vi->print();

          printf("\n");
        }


      printf("}\n");
    }


  printf("function table{\n");

    for(auto&  child: *this)
    {
        if(child->is_function())
        {
          child->get_function().print();

          printf("\n");
        }
    }


  printf("}\n");


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




