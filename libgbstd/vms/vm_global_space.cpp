#include"libgbstd/vm.hpp"




namespace gbstd{




local_space&
global_space::
create_local_space() noexcept
{
  return *m_local_spaces.emplace_back(std::make_unique<local_space>(*this));
}


function&
global_space::
create_function(std::string_view  name, const type_info&  retti, parameter_list&&  parals) noexcept
{
  return *m_function_table.emplace_back(std::make_unique<function>(create_local_space(),name,retti,std::move(parals)));
}


const memo_info&
global_space::
push(const type_info&  ti, std::string_view  name) noexcept
{
  return *m_memo_info_table.emplace_back(std::make_unique<memo_info>(ti,name));
}




}




