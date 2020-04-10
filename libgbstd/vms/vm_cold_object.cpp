#include"libgbstd/vm.hpp"




namespace gbstd{




cold_object::
cold_object(const hot_object&  o) noexcept
{
  *this = cold_object(tepid_object(o));
}


cold_object::
cold_object(const tepid_object&  o) noexcept
{
    if(o.get_type_info().is_reference())
    {
/*
      m_type_info = &o.get_type_info().strip_reference_type();

        if(m_type_info->is_array())
        {
          m_type_info = &m_type_info->strip_array_type().form_pointer_type(type_infos::pointer_size);

          m_memory = address_t(o.get_integer());
        }

      else
        {
          auto  addr = o.get_integer();
          auto    sz = o.get_size();

          m_memory.allocate(sz);

          auto  dst =            &*m_memory.get_pointer<int8_t>(0);
          auto  src = &*o.get_base_memory().get_pointer<int8_t>(addr);

          std::memcpy(dst,src,sz);
        }
*/
    }

  else
    {
      *this = static_cast<const cold_object&>(o);
    }
}




cold_object::
cold_object(bool  b) noexcept
{
  m_memory = boolean_t(b);

  m_type_info = &type_infos::boolean;
}


cold_object::
cold_object(int8_t  i) noexcept
{
  m_memory = i;

  m_type_info = &type_infos::i8;
}


cold_object::
cold_object(int16_t  i) noexcept
{
  m_memory = i;

  m_type_info = &type_infos::i16;
}


cold_object::
cold_object(int32_t  i) noexcept
{
  m_memory = i;

  m_type_info = &type_infos::i32;
}


cold_object::
cold_object(int64_t  i) noexcept
{
  m_memory = i;

  m_type_info = &type_infos::i64;
}


cold_object::
cold_object(float  f) noexcept
{
  m_memory = f;

  m_type_info = &type_infos::f32;
}


cold_object::
cold_object(double  f) noexcept
{
  m_memory = f;

  m_type_info = &type_infos::f64;
}


cold_object::
cold_object(nullptr_t  ptr) noexcept
{
  m_memory = memory();

  m_type_info = &type_infos::null_pointer;
}


cold_object::
cold_object(const function&  fn) noexcept
{
  m_memory = &fn;

  m_type_info = &fn.get_pointer_type_info();
}


cold_object::
cold_object(std::string_view  sv) noexcept
{
  auto  sz = sv.size();

  m_memory.allocate(sz+1);

//  m_type_info = &type_infos::i8.form_array_type(sz+1);

  std::memcpy(&m_memory[0],sv.data(),sz);

  m_memory[sz] = 0;
}


cold_object::
cold_object(const type_info&  ti, address_t  addr) noexcept
{
  m_memory = addr;

  m_type_info = &ti;
}




int64_t
cold_object::
get_integer() const noexcept
{
  auto  sz = get_size();

       if(sz == 1){return *m_memory.get_pointer< int8_t>(0);}
  else if(sz == 2){return *m_memory.get_pointer<int16_t>(0);}
  else if(sz == 4){return *m_memory.get_pointer<int32_t>(0);}
  else if(sz == 8){return *m_memory.get_pointer<int64_t>(0);}

  return 0;
}


double
cold_object::
get_fpn() const noexcept
{
  auto  sz = get_size();

       if(sz == 4){return *m_memory.get_pointer< float>(0);}
  else if(sz == 8){return *m_memory.get_pointer<double>(0);}

  return .0;
}




void
cold_object::
print() const noexcept
{
    if(m_type_info)
    {
      auto&  ti = *m_type_info;

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
        if(ti.is_fpn())
        {
          printf("fpn(%f)",get_fpn());
        }

      else
        if(ti.is_reference())
        {
          printf("reference(%" PRIi64 ")",get_integer());
        }

      else
        if(ti.is_null_pointer())
        {
          printf("nullptr");
        }

      else
        if(ti.is_pointer() || ti.is_generic_pointer())
        {
          printf("pointer(%" PRIi64 ")",get_integer());
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




