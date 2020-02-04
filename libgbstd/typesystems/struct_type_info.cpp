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
push(const type_info&  ti, std::string_view  name) noexcept
{
  int  offset = m_size;

  const auto     sz = ti.get_size();
  const auto  align = ti.get_align();

  offset = get_aligned_offset(offset,align);

  m_member_list.emplace_back(ti,name,offset);

  m_size  = offset+sz;
  m_align = std::max(m_align,align);
}


const struct_member*
struct_type_info::
find(std::string_view  name) const noexcept
{
    for(auto&  m: m_member_list)
    {
        if(m.get_name() == name)
        {
          return &m;
        }
    }


  return nullptr;
}


void
struct_type_info::
print() const noexcept
{
  printf("struct{\n");

    for(auto&  m: m_member_list)
    {
      m.get_type_info().print();

      printf("  %s(offset:%d);\n",m.get_name().data(),m.get_offset());
    }


  printf("}\n");
}


}
}




