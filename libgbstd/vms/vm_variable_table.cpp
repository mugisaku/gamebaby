#include"libgbstd/vm.hpp"




namespace gbstd{




variable&
variable_table::
append(const variable&  v) noexcept
{
  m_container.emplace_back(v);

  return m_container.back();
}


variable&
variable_table::
append(const typesystem::type_info&  ti, std::string_view  lb) noexcept
{
  m_container.emplace_back(ti,lb);

  return m_container.back();
}




variable*
variable_table::
find(std::string_view  lb) noexcept
{
    for(auto&  v: m_container)
    {
        if(v.get_label() == lb)
        {
          return &v;
        }
    }


  return nullptr;
}


const variable*
variable_table::
find(std::string_view  lb) const noexcept
{
    for(auto&  v: m_container)
    {
        if(v.get_label() == lb)
        {
          return &v;
        }
    }


  return nullptr;
}




}




