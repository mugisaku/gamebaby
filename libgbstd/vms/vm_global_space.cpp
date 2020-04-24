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
assign(std::string_view  sv) noexcept
{
  clear();

  initialize();

  m_source = sv;

  token_block  blk(m_source);

  token_iterator  it(blk);

  m_node.read(it);

  return *this;
}




void
global_space::
clear() noexcept
{
  m_source.clear();

  basic_space::clear();
}




}




