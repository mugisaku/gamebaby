#include"libgbstd/typesystem.hpp"




namespace gbstd{
namespace typesystem{




int
union_type_decl::
get_size() const noexcept
{
  return m_def? m_def->get_size():0;
}


int
union_type_decl::
get_align() const noexcept
{
  return m_def? m_def->get_align():0;
}


union_type_def&
union_type_decl::
set_def(union_type_def&&  def) noexcept
{
  m_def = std::make_unique<union_type_def>(std::move(def));

  return *m_def;
}


void
union_type_decl::
print() const noexcept
{
    if(m_def)
    {
      m_def->print();
    }
}




void
union_type_def::
push(type_info&&  ti, std::string_view  name) noexcept
{
  m_size  = std::max(m_size ,ti.get_size() );
  m_align = std::max(m_align,ti.get_align());

  m_member_list.emplace_back(std::move(ti),name);
}


type_info
union_type_def::
find(std::string_view  name) const noexcept
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
union_type_def::
print() const noexcept
{
  printf("{\n");

    for(auto&  m: m_member_list)
    {
      m.m_type_info.print();

      printf("  %s;\n",m.m_name.data());
    }


  printf("}\n");
}


}
}




