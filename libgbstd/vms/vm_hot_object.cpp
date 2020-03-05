#include"libgbstd/vm.hpp"




namespace gbstd{




hot_object::
hot_object(const tepid_object&  o) noexcept:
m_memory(o.get_base_memory()),
m_type_info(&o.get_type_info()),
m_address(o.get_unsigned_integer())
{
}


hot_object
hot_object::
get_struct_member(std::string_view  name) const noexcept
{
  auto  m = get_type_info().get_struct_type_info().find(name);

    if(m)
    {
      return hot_object(get_memory(),m->get_type_info(),get_address()+m->get_offset());
    }

  else
    {
      printf("ERROR: struct_value::get_member, %s is not found.\n",name.data());
    }


  return hot_object();
}




hot_object
hot_object::
get_union_member(std::string_view  name) const noexcept
{
  auto  member_ti = get_type_info().get_union_type_info().find(name);

    if(member_ti)
    {
      return hot_object(get_memory(),*member_ti,get_address());
    }

  else
    {
      printf("ERROR: union_value::get_member, %s is not found.\n",name.data());
    }


  return hot_object();
}




hot_object
hot_object::
get_element(int  i) const noexcept
{
  auto&  base_type = get_type_info().get_array_type_info().get_base_type_info();

  auto  elsz = base_type.get_size();

  return hot_object(get_memory(),base_type,get_address()+(elsz*i));
}


void
hot_object::
write(cold_object  co) const noexcept
{

}




}




