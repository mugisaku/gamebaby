#include"libgbstd/vm.hpp"




namespace gbstd{




void
global_space::
initialize() noexcept
{
  m_type_info_table.emplace_back(type_infos::int_);

  m_type_info_table.emplace_back(type_infos::i8);
  m_type_info_table.emplace_back(type_infos::i16);
  m_type_info_table.emplace_back(type_infos::i32);
  m_type_info_table.emplace_back(type_infos::i64);
  m_type_info_table.emplace_back(type_infos::f32);
  m_type_info_table.emplace_back(type_infos::f64);
  m_type_info_table.emplace_back(type_infos::void_);
  m_type_info_table.emplace_back(type_infos::null_pointer);
  m_type_info_table.emplace_back(type_infos::generic_pointer);
  m_type_info_table.emplace_back(type_infos::boolean);
  m_type_info_table.emplace_back(type_infos::undefined);
}


global_space&
global_space::
assign(std::string_view  sv)
{
  clear();

  initialize();

  m_source = sv;

  auto  toks = make_token_string(m_source);

  token_iterator  it(toks);

  read(it,"");

  allocate_address();

  return *this;
}




void
global_space::
clear() noexcept
{
  m_source.clear();

  basic_space::clear();
}


void
global_space::
allocate_address() noexcept
{
  address_t  end = 0;

    for(auto&  mi: m_memo_info_table)
    {
      end = mi->set_address(end);
    }


    for(auto&  child: m_node.get_children())
    {
        if(child->is_function())
        {
          child->get_function().allocate_address(end);
        }
    }
}




}




