#include"libgbstd/gpfs.hpp"
#include<new>




namespace gbstd{
namespace gpfs{




node::
node(node*  parent, std::string_view  name) noexcept:
m_parent(parent),
m_name(name),
m_kind(kind::null)
{
}




node&
node::
be_pointer(void*  ptr) noexcept
{
  clear();

  m_kind = kind::pointer;

  m_data.ptr = ptr;

  return *this;
}


node&
node::
be_directory() noexcept
{
  clear();

  m_kind = kind::directory;

  new(&m_data) directory(m_parent);

  m_data.dir.set_self_node(*this);

  return *this;
}




void
node::
clear() noexcept
{
    if(is_directory())
    {
      m_data.dir.~directory();
    }


  m_kind = kind::null;
}


void
node::
print() const noexcept
{
  printf("name: \"%s\", type: %s,",m_name.data(),is_directory()? "directory":"pointer");

    if(is_directory())
    {
      printf("{\n");

      get_directory().print();

      printf("\n}\n");
    }
}


void
node::
hook(node*  a, node*  b) noexcept
{
    if(a){a->m_next     = b;}
    if(b){b->m_previous = a;}
}


void
node::
unhook() noexcept
{
    if(m_previous){m_previous->m_next =     m_next;}
    if(m_next    ){m_next->m_previous = m_previous;}
}




}}




