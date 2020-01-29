#include"libgbstd/vm.hpp"




namespace gbstd{




struct
value::
data
{
  uint32_t  m_reference_count=1;

  const typesystem::type_info*  m_type_info=nullptr;

  mutability  m_mutability;

  union datadata{
     int64_t  si;
    uint64_t  ui;

    uint8_t*  ptr;

    virtual_pointer  vptr;

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
assign(const typesystem::type_info&  ti) noexcept
{
  unrefer();

  m_data = new data;

  m_data->m_type_info = &ti;

  auto  sz = ti.get_size();

    if(sz > sizeof(int64_t))
    {
      m_data->m_data.ptr = (uint8_t*)malloc(sz);
    }


  return *this;
}




const typesystem::type_info&
value::
get_type_info() const noexcept
{
  return *m_data->m_type_info;
}


int64_t
value::
get_si() const noexcept
{
  auto&  ti = get_type_info();

    if(ti.is_integer())
    {
      return m_data->m_data.si;
    }

    if(ti.is_integer())
    {
      return m_data->m_data.si;
    }


  return m_data->m_data.si;
}


uint64_t
value::
get_ui() const noexcept
{
  return m_data->m_data.ui;
}


virtual_pointer
value::
get_pointer() const noexcept
{
  return m_data->m_data.vptr;
}


value
value::
update(int64_t  i, mutability  m) const noexcept
{
    if(m_data)
    {
        if(m)
        {
          m_data->m_mutability = mute;
        }


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
update(uint64_t  u, mutability  m) const noexcept
{
    if(m_data)
    {
        if(m)
        {
          m_data->m_mutability = mute;
        }


        if(m_data->m_reference_count == 1)
        {
          m_data->m_data.ui = u;

          return *this;
        }

      else
        {
          value  new_value(get_type_info());

          new_value.m_data->m_data.ui = u;

          return std::move(new_value);
        }
    }


  return value();
}


value
value::
update(memory_view  mv, mutability  m) const noexcept
{
    if(m_data)
    {
        if(m)
        {
          m_data->m_mutability = mute;
        }


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


bool
value::
is_mutable() const noexcept
{
  return m_data->m_mutability;
}


bool  value::is_integer() const noexcept{return m_data->m_type_info->is_integer();}
bool  value::is_boolean() const noexcept{return m_data->m_type_info->is_integer();}
bool  value::is_function() const noexcept{return m_data->m_type_info->is_integer();}




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




}




