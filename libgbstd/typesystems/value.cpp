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
assign(const type_info&  ti, int64_t  i) noexcept
{
  assign(ti).set_integer(i);

  return *this;
}




int64_t
value::
get_integer() const noexcept
{
  auto&  ti = *m_type_info;

  auto&  mem = m_memory;

  auto  sz = ti.get_size();

    if(ti.is_integer() || ti.is_boolean())
    {
      return (sz == 1)? mem.get_s8()
            :(sz == 2)? mem.get_s16()
            :(sz == 4)? mem.get_s32()
            :(sz == 8)? mem.get_s64()
            :0
            ;
    }

  else
    if(ti.is_unsigned_integer() || ti.is_kind_of_pointer() || ti.is_reference())
    {
      return (sz == 1)? mem.get_u8()
            :(sz == 2)? mem.get_u16()
            :(sz == 4)? mem.get_u32()
            :(sz == 8)? mem.get_u64()
            :0
            ;
    }


  return 0;
}


uint64_t
value::
get_unsigned_integer() const noexcept
{
  auto&  ti = *m_type_info;

  auto&  mem = m_memory;

  auto  sz = ti.get_size();

    if(ti.is_integer() || ti.is_boolean())
    {
      return (sz == 1)? mem.get_s8()
            :(sz == 2)? mem.get_s16()
            :(sz == 4)? mem.get_s32()
            :(sz == 8)? mem.get_s64()
            :0
            ;
    }

  else
    if(ti.is_unsigned_integer() || ti.is_kind_of_pointer() || ti.is_reference())
    {
      return (sz == 1)? mem.get_u8()
            :(sz == 2)? mem.get_u16()
            :(sz == 4)? mem.get_u32()
            :(sz == 8)? mem.get_u64()
            :0
            ;
    }


  return 0;
}


void
value::
set_integer(int64_t  i) const noexcept
{
  auto&  ti = *m_type_info;

  auto&  mem = m_memory;

  auto  sz = ti.get_size();

    if(ti.is_integer() || ti.is_boolean())
    {
           if(sz == 1){mem.get_s8()  = i;}
      else if(sz == 2){mem.get_s16() = i;}
      else if(sz == 4){mem.get_s32() = i;}
      else if(sz == 8){mem.get_s64() = i;}
    }

  else
    if(ti.is_unsigned_integer() || ti.is_kind_of_pointer() || ti.is_reference())
    {
           if(sz == 1){mem.get_u8()  = i;}
      else if(sz == 2){mem.get_u16() = i;}
      else if(sz == 4){mem.get_u32() = i;}
      else if(sz == 8){mem.get_u64() = i;}
    }
}


void
value::
set_unsigned_integer(uint64_t  u) const noexcept
{
  auto&  ti = *m_type_info;

  auto&  mem = m_memory;

  auto  sz = ti.get_size();

    if(ti.is_integer() || ti.is_boolean())
    {
           if(sz == 1){mem.get_s8()  = u;}
      else if(sz == 2){mem.get_s16() = u;}
      else if(sz == 4){mem.get_s32() = u;}
      else if(sz == 8){mem.get_s64() = u;}
    }

  else
    if(ti.is_unsigned_integer() || ti.is_kind_of_pointer() || ti.is_reference())
    {
           if(sz == 1){mem.get_u8()  = u;}
      else if(sz == 2){mem.get_u16() = u;}
      else if(sz == 4){mem.get_u32() = u;}
      else if(sz == 8){mem.get_u64() = u;}
    }
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
      auto&  ti = *m_type_info;

      auto  sz = ti.get_size();

        if(ti.is_integer())
        {
          printf("%" PRIi64,get_integer());
        }

      else
        if(ti.is_boolean())
        {
          printf("%s",get_integer()? "true":"false");
        }

      else
        if(ti.is_unsigned_integer() || ti.is_kind_of_pointer())
        {
          printf("%" PRIu64,get_unsigned_integer());
        }

      else
        if(ti.is_reference())
        {
          printf("reference");
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
    }

  else
    {
      printf("no memory or type");
    }
}




}}




