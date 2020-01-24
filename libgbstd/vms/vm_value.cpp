#include"libgbstd/vm.hpp"




namespace gbstd{




struct
value::
data
{
  uint32_t  m_reference_count=0;

  const typesystem::type_info*  m_type_info=nullptr;

  union{
     int64_t  si;
    uint64_t  ui;

    uint8_t*  ptr;

    const value*  val;

  } m_data;

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
assign(const typesystem::type_info&  ti,  int64_t  i) noexcept
{
  unrefer();

  m_data = new data;

  m_data->m_type_info = &ti;

  m_data->m_data.si = i;

  return *this;
}


value&
value::
assign(const typesystem::type_info&  ti, uint64_t  u) noexcept
{
  unrefer();

  m_data = new data;

  m_data->m_type_info = &ti;

  m_data->m_data.ui = u;

  return *this;
}




const typesystem::type_info&
value::
get_type_info() const noexcept
{
  return *m_data->m_type_info;
}


int64_t&
value::
get_si() const noexcept
{
  return m_data->m_data.si;
}


uint64_t&
value::
get_ui() const noexcept
{
  return m_data->m_data.ui;
}


memory_frame
value::
get_memory_frame() const noexcept
{
  return {m_data->m_data.ptr,m_data->m_type_info->get_size()};
}


bool  value::is_integer() const noexcept{return m_data->m_type_info->is_integer();}
bool  value::is_boolean() const noexcept{return m_data->m_type_info->is_integer();}
bool  value::is_function() const noexcept{return m_data->m_type_info->is_integer();}




void
value::
print() const noexcept
{
}




}




