#include"libgbstd/value.hpp"




namespace gbstd{
namespace values{




object&
array::
operator[](std::string_view  sv) const noexcept
{
  auto  ptr = find(sv);

  return ptr? *ptr:objects::null;
}


array&
array::
assign(std::string_view  sv) noexcept
{
  tokenizer  tknz;

  auto  blk = tknz(sv);

  token_iterator  it(blk);

  return assign(it);
}


array&
array::
assign(token_iterator&  it) noexcept
{
  clear();

    while(it)
    {
        if(it->is_operator_code(")") || it->is_operator_code("}"))
        {
          printf("array::assign error");

          break;
        }

      else
        if(it->is_operator_code("]"))
        {
          break;
        }


      push(object(it));
    }


  return *this;
}


array&
array::
assign(std::initializer_list<object>  ls) noexcept
{
  resize(ls.size());

  auto  it = ls.begin();

    for(int  i = 0;  i < m_length;  ++i)
    {
      m_data[i] = std::move(*it++);
    }


  return *this;
}


array&
array::
assign(const array&   rhs) noexcept
{
    if(this != &rhs)
    {
      resize(rhs.size());

      auto  dst     =     begin();
      auto  src     = rhs.begin();
      auto  src_end = rhs.end();

        while(src != src_end)
        {
          *dst++ = *src++;
        }
    }


  return *this;
}


array&
array::
assign(array&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_data  ,rhs.m_data  );
      std::swap(m_number_of_elements,rhs.m_number_of_elements);
      std::swap(m_length,rhs.m_length);
    }


  return *this;
}


void
array::
clear() noexcept
{
  delete[] m_data          ;
           m_data = nullptr;

  m_number_of_elements = 0;

  m_length = 0;
}


void
array::
reallocate(size_t  n) noexcept
{
    if(m_number_of_elements < n)
    {
      auto  old_data = m_data               ;
                       m_data = new object[n];

        for(int  i = 0;  i < m_length;  ++i)
        {
          m_data[i] = std::move(old_data[i]);
        }


      delete[] old_data;
    }


  m_number_of_elements = n;
}


void
array::
resize(size_t  n) noexcept
{
  reallocate(n);

  m_length = n;
}


object&
array::
push(object&&  v) noexcept
{
    if(m_length >= m_number_of_elements)
    {
      auto   n = m_number_of_elements;

      reallocate((n < 256)? (n+2)*2:n*2);
    }


  auto&  dst = m_data[m_length++];

  dst = std::move(v);

  return dst;
}


object&
array::
pop() noexcept
{
  return m_data[--m_length];
}




object*
array::
find(std::string_view  name) const noexcept
{
    for(auto&  o: *this)
    {
        if(o.has_name() && (o.get_name() == name))
        {
          return &o;
        }
    }


  return nullptr;
}




void
array::
print() const noexcept
{
  printf("[");

    for(auto&  o: *this)
    {
      o.print();

      printf(", ");
    }


  printf("]");
}




}}




