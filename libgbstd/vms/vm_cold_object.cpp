#include"libgbstd/vm.hpp"




namespace gbstd{




constexpr auto  word_size = sizeof(int64_t);




cold_object&
cold_object::
assign(tepid_object&&  o) noexcept
{
  static_cast<cold_object&>(*this) = std::move(o);

    if(m_type_info->is_reference())
    {
      m_type_info = &m_type_info->get_reference_type_info().get_base_type_info();

      auto  addr = get_unsigned_integer();
      auto    sz = get_size();

      m_memory.allocate(sz);

      auto  dst = m_memory.get_u8ptr(0);

      std::memcpy(&*dst,&*o.get_base_memory().get_u8ptr(addr),sz);
    }


  return *this;
}




cold_object&
cold_object::
assign(bool  b) noexcept
{
  m_memory.allocate(word_size);

  *m_memory.get_u64ptr(0) = b;

  m_type_info = &type_infos::boolean;

  return *this;
}


cold_object&
cold_object::
assign(int8_t  i) noexcept
{
  m_memory.allocate(word_size);

  *m_memory.get_s64ptr(0) = i;

  m_type_info = &type_infos::s8;

  return *this;
}


cold_object&
cold_object::
assign(uint8_t  u) noexcept
{
  m_memory.allocate(word_size);

  *m_memory.get_u64ptr(0) = u;

  m_type_info = &type_infos::u8;

  return *this;
}


cold_object&
cold_object::
assign(int16_t  i) noexcept
{
  m_memory.allocate(word_size);

  *m_memory.get_s64ptr(0) = i;

  m_type_info = &type_infos::s16;

  return *this;
}


cold_object&
cold_object::
assign(uint16_t  u) noexcept
{
  m_memory.allocate(word_size);

  *m_memory.get_u64ptr(0) = u;

  m_type_info = &type_infos::u16;

  return *this;
}


cold_object&
cold_object::
assign(int32_t  i) noexcept
{
  m_memory.allocate(word_size);

  *m_memory.get_s64ptr(0) = i;

  m_type_info = &type_infos::s32;

  return *this;
}


cold_object&
cold_object::
assign(uint32_t  u) noexcept
{
  m_memory.allocate(word_size);

  *m_memory.get_u64ptr(0) = u;

  m_type_info = &type_infos::u32;

  return *this;
}


cold_object&
cold_object::
assign(int64_t  i) noexcept
{
  m_memory.allocate(word_size);

  *m_memory.get_s64ptr(0) = i;

  m_type_info = &type_infos::s64;

  return *this;
}


cold_object&
cold_object::
assign(uint64_t  u) noexcept
{
  m_memory.allocate(word_size);

  *m_memory.get_u64ptr(0) = u;

  m_type_info = &type_infos::u64;

  return *this;
}


cold_object&
cold_object::
assign(float  f) noexcept
{
  m_memory.allocate(word_size);

  *m_memory.get_f64ptr(0) = f;

  m_type_info = &type_infos::f32;

  return *this;
}


cold_object&
cold_object::
assign(double  f) noexcept
{
  m_memory.allocate(word_size);

  *m_memory.get_f64ptr(0) = f;

  m_type_info = &type_infos::f64;

  return *this;
}


cold_object&
cold_object::
assign(nullptr_t  ptr) noexcept
{
  m_memory = memory();

  m_type_info = &type_infos::null_pointer;

  return *this;
}


cold_object&
cold_object::
assign(std::string_view  sv) noexcept
{
  auto  sz = sv.size();

  m_memory.allocate(sz+1);

  m_type_info = &type_infos::s8.form_array_type(sz+1);

  std::memcpy(&m_memory[0],sv.data(),sz);

  m_memory[sz] = 0;

  return *this;
}


cold_object&
cold_object::
assign(const type_info&  ti, address_t  addr) noexcept
{
  m_memory.allocate(word_size);

  *m_memory.get_u64ptr(0) = addr;

  m_type_info = &ti;

  return *this;
}




void
cold_object::
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




