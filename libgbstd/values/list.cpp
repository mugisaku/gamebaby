#include"libgbstd/value.hpp"




namespace gbstd{




dn_list&
dn_list::
assign(std::string_view  sv) noexcept
{
  tokenizer  tknz;

  auto  blk = tknz(sv);

  token_iterator  it(blk);

  return assign(it);
}


dn_list&
dn_list::
assign(token_iterator&  it) noexcept
{
  clear();

    while(it)
    {
        if(it->is_operator_code(")") || it->is_operator_code("]"))
        {
          printf("dn_list::assign error");

          break;
        }

      else
        if(it->is_operator_code("}"))
        {
          break;
        }


      push_back(dn_object(it));
    }


  return *this;
}


dn_list&
dn_list::
assign(std::initializer_dn_list<dn_object>  ls) noexcept
{
  clear();

    for(auto&&  v: ls)
    {
      push_back(std::move(v));
    }


  return *this;
}


dn_list&
dn_list::
assign(const dn_list&   rhs) noexcept
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


dn_list&
dn_list::
assign(dn_list&&  rhs) noexcept
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
dn_list::
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
dn_list::
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
          push_back(dn_object());
        }
    }
}




dn_object&
dn_list::
push_front(dn_object&&  v) noexcept
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


  return nd->m_dn_object;
}


dn_object&
dn_list::
push_back(dn_object&&  v)  noexcept
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


  return nd->m_dn_object;
}


dn_object
dn_list::
pop_back()  noexcept
{
    if(m_last)
    {
      auto  v = std::move(m_last->m_dn_object);

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


  return dn_object();
}


dn_object
dn_list::
pop_front() noexcept
{
    if(m_first)
    {
      auto  v = std::move(m_first->m_dn_object);

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


  return dn_object();
}




dn_object&
dn_list::
insert_before(iterator  it, dn_object  v) noexcept
{
  auto  nd = new node(std::move(v));

  nd->m_prev = nullptr;

  nd->m_next = it.m_node             ;
               it.m_node->m_prev = nd;

  ++m_length;


  return nd->m_dn_object;
}


dn_object&
dn_list::
insert_after(iterator  it, dn_object  v) noexcept
{
  auto  nd = new node(std::move(v));

  nd->m_next = nullptr;

  nd->m_prev = it.m_node             ;
               it.m_node->m_next = nd;

  ++m_length;


  return nd->m_dn_object;
}


dn_list::iterator
dn_list::
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


dn_object*
dn_list::
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
dn_list::
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




}




