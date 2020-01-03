#include"libgbstd/typesystem.hpp"




namespace gbstd{
namespace typesystem{


void
union_type_info::
push(type_info&  ti, std::string_view  name) noexcept
{
  m_member_list.emplace_back(ti,name);

  m_size  = std::max(m_size ,ti.get_size() );
  m_align = std::max(m_align,ti.get_align());
}


void
union_type_info::
print() const noexcept
{
    for(auto&  m: m_member_list)
    {
      m.m_type_info.print();

      printf("  %s;\n",m.m_name.data());
    }
}


}
}




