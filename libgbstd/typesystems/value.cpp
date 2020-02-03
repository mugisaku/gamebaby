#include"libgbstd/typesystem.hpp"




namespace gbstd{
namespace typesystem{




value&
value::
assign(const type_info&  ti) noexcept
{
  m_memory = memory_sharer(ti.get_size());

  m_type_info = &ti;

  return *this;
}


value&
value::
assign(const type_info&  ti,  int64_t  i) noexcept
{
  assign(ti).get_integer() = i;

  return *this;
}




value
value::
get_element(int  i) const noexcept
{
    if(m_type_info && m_type_info->is_array())
    {
      auto&  base_type = *m_type_info->get_base();

      auto  elsz = base_type.get_size();

      value  v;

      v.m_memory = memory_sharer(m_memory,elsz*i,elsz);
      v.m_type_info = &base_type;

      return std::move(v);
    }


  return value();
}


value
value::
get_member(std::string_view  name) const noexcept
{
    if(m_type_info)
    {
      value  v;

      auto&  ti = *m_type_info;

        if(ti.is_struct())
        {
          auto  m = ti.get_struct_type_info().find(name);

            if(m)
            {
              auto&  member_ti = m->get_type_info();

              v.m_memory    = memory_sharer(m_memory,m->get_offset(),member_ti.get_size());
              v.m_type_info = &member_ti;
            }
        }

      else
        if(ti.is_union())
        {
          auto  member_ti = ti.get_union_type_info().find(name);

            if(member_ti)
            {
              v.m_memory    = memory_sharer(m_memory,0,member_ti->get_size());
              v.m_type_info = member_ti;
            }
        }


      return std::move(v);
    }


  return value();
}




void
value::
print() const noexcept
{
    if(m_memory && m_type_info)
    {
      m_type_info->print();
    }

  else
    {
      printf("no memory or type");
    }
}




}}




