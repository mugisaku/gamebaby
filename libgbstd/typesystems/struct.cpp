#include"libgbstd/typesystem.hpp"




namespace gbstd{
namespace typesystem{




int
struct_type_decl::
get_size() const noexcept
{
  return m_def? m_def->get_size():0;
}


int
struct_type_decl::
get_align() const noexcept
{
  return m_def? m_def->get_align():0;
}


struct_type_def&
struct_type_decl::
set_def(struct_type_def&&  def) noexcept
{
  m_def = std::make_unique<struct_type_def>(std::move(def));

  return *m_def;
}


void
struct_type_decl::
print() const noexcept
{
    if(m_def)
    {
      m_def->print();
    }
}




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
struct_type_def::
push(type_info&&  ti, std::string_view  name) noexcept
{
  int  offset = m_size;

  const auto     sz = ti.get_size();
  const auto  align = ti.get_align();

  offset = get_aligned_offset(offset,align);

  m_size  = offset+sz;
  m_align = std::max(m_align,align);

  m_member_list.emplace_back(std::move(ti),name,offset);
}


const struct_member*
struct_type_def::
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
struct_type_def::
print() const noexcept
{
  printf("{\n");

    for(auto&  m: m_member_list)
    {
      m.get_type_info().print();

      printf("  %s(offset:%d);\n",m.get_name().data(),m.get_offset());
    }


  printf("}\n");
}


}
}




