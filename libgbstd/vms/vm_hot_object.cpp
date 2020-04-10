#include"libgbstd/vm.hpp"




namespace gbstd{




hot_object::
hot_object(const tepid_object&  o) noexcept:
m_memory(o.get_base_memory()),
//m_type_info(&o.get_type_info().form_reference_type(type_infos::pointer_size)),
m_address(o.get_integer())
{
}


hot_object
hot_object::
get_struct_member(std::string_view  name) const noexcept
{
/*
  auto  m = get_type_info().get_struct_type_info().find(name);

    if(m)
    {
      return hot_object(get_memory(),m->get_type_info(),get_address()+m->get_offset());
    }

  else
    {
      printf("ERROR: struct_value::get_member, %s is not found.\n",name.data());
    }
*/


  return hot_object();
}




hot_object
hot_object::
get_union_member(std::string_view  name) const noexcept
{
/*
  auto  member_ti = get_type_info().get_union_type_info().find(name);

    if(member_ti)
    {
      return hot_object(get_memory(),*member_ti,get_address());
    }

  else
    {
      printf("ERROR: union_value::get_member, %s is not found.\n",name.data());
    }
*/


  return hot_object();
}




hot_object
hot_object::
get_element(int  i) const noexcept
{
/*
  auto&  base_type = get_type_info().get_array_type_info().get_base_type_info();

  auto  elsz = base_type.get_size();

  return hot_object(get_memory(),base_type,get_address()+(elsz*i));
*/
  return hot_object();
}


tepid_object
hot_object::
write(cold_object  co) noexcept
{
/*
  auto&  hti = m_type_info->strip_reference_type();
  auto&  cti = co.get_type_info();

  auto  sz = hti.get_size();

    if(cti.is_integer())
    {
      auto  i = co.get_integer();

        if(hti.is_integer())
        {
               if(sz == 1){*m_memory.get_pointer< int8_t>(m_address) = i;}
          else if(sz == 2){*m_memory.get_pointer<int16_t>(m_address) = i;}
          else if(sz == 4){*m_memory.get_pointer<int32_t>(m_address) = i;}
          else if(sz == 8){*m_memory.get_pointer<int64_t>(m_address) = i;}
        }

      else
        if(hti.is_fpn())
        {
               if(sz == 4){*m_memory.get_pointer< float>(m_address) = i;}
          else if(sz == 8){*m_memory.get_pointer<double>(m_address) = i;}
        }

      else
        if(hti.is_boolean())
        {
          *m_memory.get_pointer<boolean_t>(m_address) = i;
        }

      else
        if(hti.is_null_pointer())
        {
        }

      else
        if(hti.is_generic_pointer())
        {
          *m_memory.get_pointer<address_t>(m_address) = i;
        }

      else
        if(hti.is_pointer())
        {
          *m_memory.get_pointer<address_t>(m_address) = i;
        }
    }

  else
    if(cti.is_fpn())
    {
      auto  f = co.get_fpn();

        if(hti.is_integer())
        {
               if(sz == 1){*m_memory.get_pointer< int8_t>(m_address) = f;}
          else if(sz == 2){*m_memory.get_pointer<int16_t>(m_address) = f;}
          else if(sz == 4){*m_memory.get_pointer<int32_t>(m_address) = f;}
          else if(sz == 8){*m_memory.get_pointer<int64_t>(m_address) = f;}
        }

      else
        if(hti.is_fpn())
        {
               if(sz == 4){*m_memory.get_pointer< float>(m_address) = f;}
          else if(sz == 8){*m_memory.get_pointer<double>(m_address) = f;}
        }

      else
        if(hti.is_boolean())
        {
          *m_memory.get_pointer<boolean_t>(m_address) = f;
        }

      else
        if(hti.is_null_pointer())
        {
        }

      else
        if(hti.is_generic_pointer())
        {
          *m_memory.get_pointer<address_t>(m_address) = f;
        }

      else
        if(hti.is_pointer())
        {
          *m_memory.get_pointer<address_t>(m_address) = f;
        }
    }

  else
    if(cti.is_boolean())
    {
    }

  else
    if(cti.is_null_pointer())
    {
    }

  else
    if(cti.is_generic_pointer())
    {
    }

  else
    if(cti.is_pointer())
    {
    }
*/

  return tepid_object(*this);
}




}




