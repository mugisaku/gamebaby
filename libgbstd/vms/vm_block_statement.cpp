#include"libgbstd/vm.hpp"




namespace gbstd{




block_statement::
block_statement(const function&  fn, token_iterator&  it) noexcept:
m_global_space(&fn.get_global_space()),
m_function(&fn)
{
  read(it);
}




const statement&
block_statement::
operator[](int  i) const noexcept
{
  static const statement  null;

  return (i < m_statement_list.size())? m_statement_list[i]:null;
}


void
block_statement::
clear() noexcept
{
  m_type_info_table.clear();
  m_function_table.clear();
}


void
block_statement::
set_parent(const block_statement*  p) noexcept
{
  m_parent = p;
}


const function*
block_statement::
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


type_info
block_statement::
find_type_info_by_name(std::string_view  name) const noexcept
{
  return type_info();
}


type_info
block_statement::
find_type_info_by_id(  std::string_view  name) const noexcept
{
  return type_info();
}


void
block_statement::
push(statement&&  st) noexcept
{
  m_statement_list.emplace_back(std::move(st));
}


void
block_statement::
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


    for(auto&  st: m_statement_list)
    {
      st.print();

      printf("\n");
    }
}




}




