#include"libgbstd/typesystem.hpp"



namespace gbstd{
namespace typesystem{




parameter_list::
parameter_list(std::initializer_list<const type_info*>  ls) noexcept
{
    for(auto  p: ls)
    {
      push(*p);
    }
}


parameter_list&
parameter_list::
push(const type_info&  ti) noexcept
{
  m_container.emplace_back(&ti);

  m_id += ti.get_id();

  return *this;
}




}}




