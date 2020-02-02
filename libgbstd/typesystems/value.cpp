#include"libgbstd/typesystem.hpp"




namespace gbstd{
namespace typesystem{




void
value::
unrefer() noexcept
{
    if(m_data)
    {
        if(!--get_reference_count())
        {
          free(m_data);
        }


      m_data = nullptr;
    }
}


namespace{
uint8_t*
allocate(size_t  memsize) noexcept
{
  return (uint8_t*)calloc(1,(sizeof(intptr_t)*2)+memsize);
}
}


uintptr_t&
value::
get_reference_count() const noexcept
{
  return *reinterpret_cast<uintptr_t*>(m_data);
}


const type_info*&
value::
get_type_info_pointer() const noexcept
{
  return *reinterpret_cast<const type_info**>(m_data+sizeof(intptr_t));
}


uint8_t*
value::
get_memory_pointer() const noexcept
{
  return m_data+(sizeof(intptr_t)*2);
}


value&
value::
assign(const value&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      m_data = rhs.m_data;

        if(m_data)
        {
          ++get_reference_count();
        }
    }


  return *this;
}


value&
value::
assign(value&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      std::swap(m_data,rhs.m_data);
    }


  return *this;
}


value&
value::
assign(const type_info&  ti) noexcept
{
  unrefer();

  m_data = allocate(ti.get_size());

  get_type_info_pointer() = &ti;

  return *this;
}


value&
value::
assign(const type_info&  ti,  int64_t  i) noexcept
{
  unrefer();

  m_data = allocate(ti.get_size());

  get_type_info_pointer() = &ti;

  *reinterpret_cast<int64_t*>(get_memory_pointer()) = i;


  return *this;
}




const type_info&
value::
get_type_info() const noexcept
{
  return *get_type_info_pointer();
}


type_derivation&
value::
get_type_derivation() const noexcept
{
  return get_type_info_pointer()->get_derivation();
}


int64_t&
value::
get_integer() const noexcept
{
  return *reinterpret_cast<int64_t*>(get_memory_pointer());
}


uint64_t&
value::
get_unsigned_integer() const noexcept
{
  return *reinterpret_cast<uint64_t*>(get_memory_pointer());
}


value
value::
update(int64_t  i) const noexcept
{
    if(m_data)
    {
        if(get_reference_count() == 1)
        {
          get_integer() = i;

          return *this;
        }

      else
        {
          value  new_value(get_type_info());

          new_value.get_integer() = i;

          return std::move(new_value);
        }
    }


  return value();
}


value
value::
update(uint64_t  u) const noexcept
{
  return update(static_cast<int64_t>(u));
}


value
value::
clone() const noexcept
{
  auto&  ti = get_type_info();

  value  v(ti);

  auto  sz = ti.get_size();

  std::memcpy(v.get_memory_pointer(),get_memory_pointer(),sz);

  return std::move(v);
}


memory_frame
value::
get_memory_frame() const noexcept
{
  return {get_memory_pointer(),get_type_info().get_size()};
}




void
value::
print() const noexcept
{
    if(m_data && get_type_info_pointer())
    {
      get_type_info().print();

      printf("%" PRIi64 "(ref: %" PRIi64 ")",get_integer(),get_reference_count());
    }

  else
    {
      printf("no data&type");
    }
}




}}




