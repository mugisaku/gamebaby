#include"libgbstd/typesystem.hpp"




namespace gbstd{
namespace typesystem{




uint32_t
union_type_info::
m_id_source;


void
union_type_info::
push(type_info&  ti, std::string_view  name) noexcept
{
  m_member_list.emplace_back(ti,name);

  m_size  = std::max(m_size ,ti.get_size() );
  m_align = std::max(m_align,ti.get_align());
}


const type_info*
union_type_info::
find(std::string_view  name) const noexcept
{
    for(auto&  m: m_member_list)
    {
        if(m.m_name == name)
        {
          return &m.m_type_info;
        }
    }


  return nullptr;
}


void
union_type_info::
print() const noexcept
{
  printf("union{\n");

    for(auto&  m: m_member_list)
    {
      m.m_type_info.print();

      printf("  %s;\n",m.m_name.data());
    }


  printf("}\n");
}


}
}




