#include"libgbstd/gpfs.hpp"




namespace gbstd{
namespace gpfs{




directory::
directory(node*  parent_node) noexcept:
m_parent_node(parent_node)
{
}




void
directory::
set_self_node(node&  node) noexcept
{
  m_self_node = &node;
}


void
directory::
clear() noexcept
{
  auto  ptr = m_first_node;

    while(ptr)
    {
      auto  next = ptr->m_next;

      delete ptr       ;
             ptr = next;
    }


  m_number_of_nodes = 0;
}


node&
directory::
create_node(std::string_view  name) noexcept
{
  auto  nd = new node(m_self_node,name);

  node::hook(m_last_node,nd);

  m_last_node = nd;

    if(!m_first_node)
    {
      m_first_node = nd;
    }


  ++m_number_of_nodes;

  return *nd;
}


directory&
directory::
create_directory(std::string_view  name) noexcept
{
  auto&  nd = create_node(name);

  nd.be_directory();

  return nd.get_directory();
}


void
directory::
create_pointer(void*  ptr, std::string_view  name) noexcept
{
  auto&  nd = create_node(name);

  nd.be_pointer(ptr);
}


node*
directory::
find(std::string_view  name) const noexcept
{
    for(auto&  nd: *this)
    {
        if(nd.get_name() == name)
        {
          return &nd;
        }
    }


  return nullptr;
}


void
directory::
remove(std::string_view  name) noexcept
{
  auto  nd = find(name);

    if(nd)
    {
      remove(nd);
    }
}


void
directory::
remove(node*  nd) noexcept
{
    if(nd->m_parent == m_self_node)
    {
        if(m_last_node == nd)
        {
          m_last_node = m_last_node->m_previous;
        }


        if(m_first_node == nd)
        {
          m_first_node = m_first_node->m_next;
        }


      nd->unhook();

      delete nd;

      --m_number_of_nodes;
    }
}


void
directory::
print() const noexcept
{
  int  d = 0;
  int  p = 0;

    for(auto&  nd: *this)
    {
        if(nd.is_directory()){++d;}
      else                   {++p;}

      nd.print();

      printf("\n");
    }


  printf("%d個のノード(%d個のディレクトリと、%d個のポインタ)\n",m_number_of_nodes,d,p);
}




directory::iterator&
directory::iterator::
operator++() noexcept
{
  m_pointer = m_pointer->m_next;

  return *this;
}




}}




