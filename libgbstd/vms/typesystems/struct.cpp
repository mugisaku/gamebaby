#include"libgbstd/vms/typesystem.hpp"




namespace gbstd{
namespace typesystem{




int
struct_decl::
size() const noexcept
{
  return m_def? m_def->size():0;
}


int
struct_decl::
align() const noexcept
{
  return m_def? m_def->align():0;
}


struct_def&
struct_decl::
set_def(struct_def&&  def) noexcept
{
  m_def = std::make_unique<struct_def>(std::move(def));

  return *m_def;
}


void
struct_decl::
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
struct_def::
push(type_info&&  ti, std::u16string_view  name) noexcept
{
  int  offset = m_size;

  const auto     sz = ti.size();
  const auto  align = ti.align();

  offset = get_aligned_offset(offset,align);

  m_size  = offset+sz;
  m_align = std::max(m_align,align);

  m_member_list.emplace_back(std::move(ti),name,offset);
}


const struct_member*
struct_def::
find(std::u16string_view  name) const noexcept
{
    for(auto&  m: m_member_list)
    {
        if(m.name() == name)
        {
          return &m;
        }
    }


  return nullptr;
}


void
struct_def::
print() const noexcept
{
  printf("{\n");

    for(auto&  m: m_member_list)
    {
      m.type().print();

      printf("  ");

      gbstd::print(m.name());

      printf("(offset:%d);\n",m.offset());
    }


  printf("}\n");
}


}
}




