#include"libgbstd/typesystem.hpp"




namespace gbstd{
namespace typesystem{




struct
value::
data
{
  uint32_t  m_reference_count=1;

  const type_info*  m_type_info=nullptr;

  union datadata{
     int64_t  si;
    uint64_t  ui;

    uint8_t*  ptr;

    datadata() noexcept{}
   ~datadata()         {}

  } m_data;

  data() noexcept{}

};


void
value::
unrefer() noexcept
{
    if(m_data)
    {
        if(!--m_data->m_reference_count)
        {
          auto  ti = m_data->m_type_info;

            if(ti && (ti->get_size() > sizeof(uint64_t)))
            {
              free(m_data->m_data.ptr);
            }


          delete m_data;
        }


      m_data = nullptr;
    }
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
          m_data->m_reference_count += 1;
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

  m_data = new data;

  m_data->m_type_info = &ti;

  m_data->m_data.si = 0;


  return *this;
}


value&
value::
assign(const type_info&  ti,  int64_t  i) noexcept
{
  unrefer();

  m_data = new data;

  m_data->m_type_info = &ti;

  m_data->m_data.si = i;


  return *this;
}


value&
value::
assign(const type_info&  ti, memory_view  mv) noexcept
{
  unrefer();

  auto  l = mv.get_length();

    if(l > sizeof(int64_t))
    {
      m_data = new data;

      m_data->m_type_info = &ti;

      m_data->m_data.ptr = (uint8_t*)malloc(l);

      std::memcpy(m_data->m_data.ptr,mv.get_pointer(),l);
    }


  return *this;
}




const type_info&
value::
get_type_info() const noexcept
{
  return *m_data->m_type_info;
}


type_derivation&
value::
get_type_derivation() const noexcept
{
  return m_data->m_type_info->get_derivation();
}


int64_t
value::
get_integer() const noexcept
{
  return m_data->m_data.si;
}


uint64_t
value::
get_unsigned_integer() const noexcept
{
  return m_data->m_data.ui;
}


value
value::
update(int64_t  i) const noexcept
{
    if(m_data && (m_data->m_type_info->get_size() <= sizeof(int64_t)))
    {
        if(m_data->m_reference_count == 1)
        {
          m_data->m_data.si = i;

          return *this;
        }

      else
        {
          value  new_value(get_type_info());

          new_value.m_data->m_data.si = i;

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
update(memory_view  mv) const noexcept
{
    if(m_data && (m_data->m_type_info->get_size() > sizeof(int64_t)))
    {
        if(m_data->m_reference_count == 1)
        {
          std::memcpy(m_data->m_data.ptr,mv.get_pointer(),mv.get_length());

          return *this;
        }

      else
        {
          value  new_value(get_type_info());

          std::memcpy(new_value.m_data->m_data.ptr,mv.get_pointer(),mv.get_length());

          return std::move(new_value);
        }
    }


  return value();
}


value
value::
clone() const noexcept
{
  auto&  ti = get_type_info();

  value  v(ti);

  auto  sz = ti.get_size();

    if(sz > sizeof(int64_t))
    {
      std::memcpy(v.m_data->m_data.ptr,m_data->m_data.ptr,sz);
    }

  else
    {
      v.m_data->m_data.si = m_data->m_data.si;
    }


  return std::move(v);
}


memory_frame
value::
get_memory_frame() const noexcept
{
  return {m_data->m_data.ptr,m_data->m_type_info->get_size()};
}




void
value::
print() const noexcept
{
    if(m_data && m_data->m_type_info)
    {
      m_data->m_type_info->print();

      printf("%" PRIi64 "(ref: %d)",m_data->m_data.si,m_data->m_reference_count);
    }

  else
    {
      printf("no data&type");
    }
}




}}




