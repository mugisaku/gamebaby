#include"libgbstd/vm.hpp"




namespace gbstd{




const memo_info&
local_space::
push_parameter(const type_info&  ti, std::string_view  name) noexcept
{
  return *m_parameter_memo_info_table.emplace_back(std::make_unique<memo_info>(ti,name));
}


const memo_info&
local_space::
push_domestic( const type_info&  ti, std::string_view  name) noexcept
{
  return *m_domestic_memo_info_table.emplace_back(std::make_unique<memo_info>(ti,name));
}




}




