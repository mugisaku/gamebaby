#include"libgbstd/gpfs.hpp"




namespace gbstd{
namespace gpfs{




bool
directory::
is_root() const noexcept
{
  return !m_self_node->m_parent;
}


node*
directory::
get_parent_node() const noexcept
{
  return m_self_node->m_parent;
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
  auto  nd = new node(*m_self_node,name);

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

  return nd.be_directory();
}


node*
directory::
find_by_name(std::string_view  name) const noexcept
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


node*
directory::
find_by_path(const gpfs::path&  path) const noexcept
{
  auto  dir = path.is_absolute()? &get_root_directory():this;

  node*  nd = path.is_absolute()? dir->m_self_node:nullptr;

    for(auto&  copath: path)
    {
        if(copath.front() == '.')
        {
            if(copath.size() > 1)
            {
              dir = &dir->get_parent_directory();
            }


          nd = dir->m_self_node;
        }

      else
        {
          nd = dir->find_by_name(copath);

            if(!nd)
            {
              break;
            }


            if(nd->is_directory())
            {
              dir = &nd->get_directory();
            }
        }
    }


  return nd;
}


directory*
directory::
find_directory_by_name(std::string_view  name) const noexcept
{
    for(auto&  nd: *this)
    {
        if(nd.is_directory() && (nd.get_name() == name))
        {
          return &nd.get_directory();
        }
    }


  return nullptr;
}


directory&
directory::
get_root_directory() const noexcept
{
  auto  nd = m_self_node;

    while(nd->m_parent)
    {
      nd = nd->m_parent;
    }


  return nd->get_directory();
}


directory&
directory::
get_parent_directory() const noexcept
{
  auto  parent = m_self_node->m_parent;

  return (parent? parent:m_self_node)->get_directory();
}


void
directory::
remove(std::string_view  name) noexcept
{
  auto  nd = find_by_name(name);

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




