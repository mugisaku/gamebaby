#include"libgbstd/vms/typesystem.hpp"




namespace gbstd{
namespace typesystem{




int
union_decl::
size() const noexcept
{
  return m_def? m_def->size():0;
}


int
union_decl::
align() const noexcept
{
  return m_def? m_def->align():0;
}


union_def&
union_decl::
set_def(union_def&&  def) noexcept
{
  m_def = std::make_unique<union_def>(std::move(def));

  return *m_def;
}


void
union_decl::
print() const noexcept
{
    if(m_def)
    {
      m_def->print();
    }
}




void
union_def::
push(type_info&&  ti, std::u16string_view  name) noexcept
{
  m_size  = std::max(m_size ,ti.size() );
  m_align = std::max(m_align,ti.align());

  m_member_list.emplace_back(std::move(ti),name);
}


type_info
union_def::
find(std::u16string_view  name) const noexcept
{
    for(auto&  m: m_member_list)
    {
        if(m.m_name == name)
        {
          return m.m_type_info;
        }
    }


  return type_info();
}


void
union_def::
print() const noexcept
{
  printf("{\n");

    for(auto&  m: m_member_list)
    {
      m.m_type_info.print();

      printf("  ");

      gbstd::print(m.m_name);

      printf(";\n");
    }


  printf("}\n");
}


}
}




