#include"libgbstd/typesystem.hpp"



namespace gbstd{
namespace typesystem{




parameter_list::
parameter_list(std::initializer_list<std::pair<type_info&,std::string_view>>  ls) noexcept
{
    for(auto  p: ls)
    {
      push(p.first,p.second);
    }
}


parameter_list&
parameter_list::
push(type_info&  ti, std::string_view  name) noexcept
{
  m_container.emplace_back(ti,name);

  m_id += ti.get_id();

  return *this;
}




}}




