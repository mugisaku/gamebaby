#include"libgbstd/typesystem.hpp"




namespace gbstd{
namespace typesystem{




bool
struct_type_info::
member::
test_align(size_t  offset_base) const noexcept
{
return 0;
//  return m_type_info.test_align(offset_base+m_offset);
}


void
struct_type_info::
push(type_info&  ti, std::string_view  name) noexcept
{
  int  offset = m_size;

  const auto     sz = ti.get_size();
  const auto  align = ti.get_align();

  offset = get_aligned_offset(offset,align);

  m_member_list.emplace_back(ti,name,offset);

  m_size  = offset+sz;
  m_align = std::max(m_align,align);
}


bool
struct_type_info::
test_align(size_t  offset_base) const noexcept
{
    for(auto&  m: m_member_list)
    {
        if(!m.test_align(offset_base))
        {
//          printf("[test align failed] %s\n",m.get_name().data());

          return false;
        }
    }


  return true;
}


void
struct_type_info::
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




