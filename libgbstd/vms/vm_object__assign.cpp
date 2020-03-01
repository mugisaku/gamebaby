#include"libgbstd/vm.hpp"




namespace gbstd{




object&
object::
assign(const object&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_type_info = rhs.m_type_info;

        if(m_type_info)
        {
            if(m_type_info->is_memory())
            {
              new(&m_memory) memory(rhs.m_memory);
            }

          else
            {
              m_word = rhs.m_word;
            }
        }
    }


  return *this;
}


object&
object::
assign(object&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_type_info,rhs.m_type_info);

        if(m_type_info)
        {
            if(m_type_info->is_memory())
            {
              new(&m_memory) memory(std::move(rhs.m_memory));
            }

          else
            {
              m_word = rhs.m_word;
            }
        }
    }


  return *this;
}




object&
object::
assign(bool  b) noexcept
{
  clear();

  m_word = b;

  m_type_info = &type_infos::boolean;

  return *this;
}


object&
object::
assign(int8_t  i) noexcept
{
  clear();

  m_word = i;

  m_type_info = &type_infos::s8;

  return *this;
}


object&
object::
assign(uint8_t  u) noexcept
{
  clear();

  m_uword = u;

  m_type_info = &type_infos::u8;

  return *this;
}


object&
object::
assign(int16_t  i) noexcept
{
  clear();

  m_word = i;

  m_type_info = &type_infos::s16;

  return *this;
}


object&
object::
assign(uint16_t  u) noexcept
{
  clear();

  m_uword = u;

  m_type_info = &type_infos::u16;

  return *this;
}


object&
object::
assign(int32_t  i) noexcept
{
  clear();

  m_word = i;

  m_type_info = &type_infos::s32;

  return *this;
}


object&
object::
assign(uint32_t  u) noexcept
{
  clear();

  m_uword = u;

  m_type_info = &type_infos::u32;

  return *this;
}


object&
object::
assign(int64_t  i) noexcept
{
  clear();

  m_word = i;

  m_type_info = &type_infos::s64;

  return *this;
}


object&
object::
assign(uint64_t  u) noexcept
{
  clear();

  m_uword = u;

  m_type_info = &type_infos::u64;

  return *this;
}


object&
object::
assign(float  f) noexcept
{
  clear();

  m_fword = f;

  m_type_info = &type_infos::f32;

  return *this;
}


object&
object::
assign(double  f) noexcept
{
  clear();

  m_fword = f;

  m_type_info = &type_infos::f64;

  return *this;
}


object&
object::
assign(nullptr_t  ptr) noexcept
{
  clear();

  m_type_info = &type_infos::null_pointer;

  return *this;
}


object&
object::
assign(std::string_view  sv) noexcept
{
  clear();

  auto  sz = sv.size();

  m_memory.allocate(sz+1);

  m_type_info = &type_infos::s8.form_array_type(sz+1);

  std::memcpy(&m_memory[0],sv.data(),sz);

  m_memory[sz] = 0;

  return *this;
}


object&
object::
assign(const memory&  mem, address_t  addr, const type_info&  ti) noexcept
{
  clear();

  m_memory = mem;

  m_address = addr;

  m_type_info = &ti;

  return *this;
}




}




