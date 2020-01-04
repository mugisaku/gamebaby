#include"libgbstd/typesystem.hpp"




namespace gbstd{
namespace typesystem{




uint32_t
struct_type_info::
m_id_source;


constexpr
int
get_aligned_offset(int  offset, int  align) noexcept
{
    if(align)
    {
      offset = (offset+(align-1))/align*align;
    }


  return offset;
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


void
struct_type_info::
print() const noexcept
{
  printf("struct{\n");

    for(auto&  m: m_member_list)
    {
      m.m_type_info.print();

      printf("  %s(offset:%d);\n",m.m_name.data(),m.m_offset);
    }


  printf("}\n");
}


}
}




