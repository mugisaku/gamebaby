#include"libgbstd/value.hpp"




namespace gbstd{
namespace values{




list&
list::
assign(std::initializer_list<object>  ls) noexcept
{
  clear();

    for(auto&&  v: ls)
    {
      push_back(std::move(v));
    }


  return *this;
}


list&
list::
assign(const list&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

        for(auto&  v: rhs)
        {
          push_back(v);
        }
    }


  return *this;
}


list&
list::
assign(list&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_first ,rhs.m_first );
      std::swap(m_last  ,rhs.m_last  );
      std::swap(m_length,rhs.m_length);
    }


  return *this;
}


void
list::
clear() noexcept
{
  auto  next = m_first;

    while(next)
    {
      auto  nd = next;
                 next = next->m_next;

      delete nd;
    }


  m_length = 0;
}


void
list::
resize(size_t  n) noexcept
{
    if(n < m_length)
    {
        while(n > m_length)
        {
          pop_back();
        }
    }

  else
    if(n > m_length)
    {
        while(n < m_length)
        {
          push_back(object());
        }
    }
}




object&
list::
push_front(object&&  v) noexcept
{
  auto  nd = new node(std::move(v));

  nd->m_prev = nullptr;

    if(m_first)
    {
      m_first->m_prev = nd;
    }

  else
    {
      m_last = nd;
    }


  nd->m_next = m_first     ;
               m_first = nd;

  ++m_length;


  return nd->m_object;
}


object&
list::
push_back(object&&  v)  noexcept
{
  auto  nd = new node(std::move(v));

  nd->m_next = nullptr;

    if(m_last)
    {
      m_last->m_next = nd;
    }

  else
    {
      m_first = nd;
    }


  nd->m_prev = m_last     ;
               m_last = nd;

  ++m_length;


  return nd->m_object;
}


object
list::
pop_back()  noexcept
{
    if(m_last)
    {
      auto  v = std::move(m_last->m_object);

      auto  new_last = m_last->m_prev;

      delete m_last;
             m_last = new_last;

      new_last->m_next = nullptr;

        if(!m_length--)
        {
          m_first = nullptr;
        }



      return std::move(v);
    }


  return object();
}


object
list::
pop_front() noexcept
{
    if(m_first)
    {
      auto  v = std::move(m_first->m_object);

      auto  new_first = m_first->m_next;

      delete m_first            ;
             m_first = new_first;

      new_first->m_prev = nullptr;

        if(!m_length--)
        {
          m_last = nullptr;
        }


      return std::move(v);
    }


  return object();
}




object&
list::
insert_before(iterator  it, object  v) noexcept
{
  auto  nd = new node(std::move(v));

  nd->m_prev = nullptr;

  nd->m_next = it.m_node             ;
               it.m_node->m_prev = nd;

  ++m_length;


  return nd->m_object;
}


object&
list::
insert_after(iterator  it, object  v) noexcept
{
  auto  nd = new node(std::move(v));

  nd->m_next = nullptr;

  nd->m_prev = it.m_node             ;
               it.m_node->m_next = nd;

  ++m_length;


  return nd->m_object;
}


list::iterator
list::
erase(iterator  it) noexcept
{
    if(it)
    {
      auto  next = it.m_node->m_next;

      delete it.m_node;

      return iterator(next);
    }


  return iterator();
}


object*
list::
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
list::
print() const noexcept
{
  printf("{");

    for(auto&  o: *this)
    {
      o.print();

      printf(", ");
    }


  printf("}");
}




}}




