#include"libgbstd/vm.hpp"




namespace gbstd{




object
object::
dereference() const noexcept
{
    if(m_type_info->is_reference())
    {
      auto&  ti = m_type_info->get_reference_type_info().get_base_type_info();

      return object(m_memory,m_address,ti);
    }


  return *this;
}


object
object::
get_struct_member(std::string_view  name) const noexcept
{
    if(m_type_info)
    {
      auto  m = m_type_info->get_struct_type_info().find(name);

        if(m)
        {
          return object(m_memory,m_address+m->get_offset(),m->get_type_info());
        }

      else
        {
          printf("ERROR: struct_value::get_member, %s is not found.\n",name.data());
        }
    }


  return object();
}




object
object::
get_union_member(std::string_view  name) const noexcept
{
    if(m_type_info)
    {
      auto  member_ti = m_type_info->get_union_type_info().find(name);

        if(member_ti)
        {
          return object(m_memory,m_address,*member_ti);
        }

      else
        {
          printf("ERROR: union_value::get_member, %s is not found.\n",name.data());
        }
    }


  return object();
}




object
object::
get_element(int  i) const noexcept
{
  auto&  base_type = m_type_info->get_array_type_info().get_base_type_info();

  auto  elsz = base_type.get_size();

  return object(m_memory,m_address+(elsz*i),base_type.form_reference_type(type_infos::pointer_size));
}




void
object::
clear() noexcept
{
    if(m_type_info && m_type_info->is_memory())
    {
      m_memory.~memory();
    }


  m_address = 0;

  m_type_info = nullptr;
}


void
object::
print() const noexcept
{
    if(m_type_info)
    {
      auto&  ti = *m_type_info;

      auto  sz = ti.get_size();

        if(ti.is_integer())
        {
          printf("int(%" PRIi64 ")",get_integer());
        }

      else
        if(ti.is_boolean())
        {
          printf("%s",get_integer()? "true":"false");
        }

      else
        if(ti.is_unsigned_integer())
        {
          printf("uint(%" PRIu64 ")",get_unsigned_integer());
        }

      else
        if(ti.is_reference())
        {
          printf("reference(%" PRIu64 ")",get_unsigned_integer());
        }

      else
        if(ti.is_null_pointer())
        {
          printf("nullptr");
        }

      else
        if(ti.is_pointer() || ti.is_generic_pointer())
        {
          printf("pointer(%" PRIu64 ")",get_unsigned_integer());
        }

      else
        if(ti.is_void())
        {
          printf("void");
        }

      else
        if(ti.is_undefined())
        {
          printf("undefined");
        }
    }

  else
    {
      printf("no type");
    }
}




}




