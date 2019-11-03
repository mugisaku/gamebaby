#include"libgbstd/gpfs.hpp"




namespace gbstd{
namespace gpfs{




node&
directory::
operator[](std::string_view  nodepath) noexcept
{
  auto  nd = find_node_by_path(nodepath);

    if(nd)
    {
      return *nd;
    }


  return *create_node_by_path(nodepath);
}




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


node*
directory::
create_node_by_name(std::string_view  name) noexcept
{
  auto  nd = find_node_by_name(name);

    if(nd)
    {
      return nd;
    }


  nd = new node(name);

  append_node(nd);

  return nd;
}


node*
directory::
create_node_by_path(std::string_view  nodepath) noexcept
{
  gpfs::path  path(nodepath);

  auto  dir = path.is_absolute()? &get_root_directory():this;

  auto  head = &path.get_first_name();
  auto  tail = &path.get_last_name();

    while(head != tail)
    {
      auto&  name = *head++;

      auto  res = dir->find_node_by_name(name);

        if(!res)
        {
          dir = dir->create_directory(name);
        }

      else
        if(res->is_directory())
        {
          dir = &res->get_directory();
        }

      else
        {
          printf("[error: create node by path] can not make directory");

          return nullptr;
        }
    }


  return dir->create_node_by_name(*tail);
}


directory*
directory::
create_directory(std::string_view  nodepath) noexcept
{
  auto  nd = create_node_by_path(nodepath);

    if(nd)
    {
      return  nd->is_null()?      &nd->be_directory()
             :nd->is_directory()? &nd->get_directory()
             :nullptr;
    }


  return nullptr;
}


node*
directory::
find_node_by_name(std::string_view  name) const noexcept
{
    if(name.size())
    {
        if(name[0] == '.')
        {
          auto  parent = m_self_node->m_parent;

          return ((name.size() > 1) && parent)? parent:m_self_node;
        }


        for(auto&  nd: *this)
        {
            if(nd.get_name() == name)
            {
              return &nd;
            }
        }
    }


  return nullptr;
}


node*
directory::
find_node_by_path(std::string_view  nodepath) const noexcept
{
  gpfs::path  path(nodepath);

  auto  dir = path.is_absolute()? &get_root_directory():this;

  node*  nd = path.is_absolute()? dir->m_self_node:nullptr;

    for(auto&  copath: path)
    {
      nd = dir->find_node_by_name(copath);

        if(!nd || !nd->is_directory())
        {
          break;
        }


      dir = &nd->get_directory();
    }


  return nd;
}


directory*
directory::
find_directory(std::string_view  dirpath) const noexcept
{
  auto  nd = find_node_by_path(dirpath);

  return (nd && nd->is_directory())? &nd->get_directory():nullptr;
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


bool
directory::
move(std::string_view  src_nodepath, std::string_view  dst_dirpath) noexcept
{
  auto  src_nd = find_node_by_path(src_nodepath);

    if(src_nd)
    {
      auto  src_parent = src_nd->m_parent;

        if(src_parent)
        {
          auto  dst_dir = create_directory(dst_dirpath);

            if(dst_dir)
            {
                if(src_parent->get_directory().remove_node(src_nd))
                {
                  dst_dir->append_node(src_nd);

                  return true;
                }
            }
        }
    }


  return false;
}


void
directory::
append_node(node*  nd) noexcept
{
  nd->m_parent = m_self_node;

  node::hook(m_last_node,nd);

  m_last_node = nd;

    if(!m_first_node)
    {
      m_first_node = nd;
    }


  ++m_number_of_nodes;
}


bool
directory::
remove(std::string_view  nodepath) noexcept
{
  auto  nd = find_node_by_path(nodepath);

    if(nd)
    {
      auto  parent = nd->m_parent;

        if(parent && parent->get_directory().remove_node(nd))
        {
          delete nd;

          return true;
        }
    }


  return false;
}


bool
directory::
remove_node(node*  nd) noexcept
{
    if(nd->m_parent && (nd->m_parent == m_self_node))
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

      nd->m_previous = nullptr;
      nd->m_next     = nullptr;

      --m_number_of_nodes;

      return true;
    }


  return false;
}


void
directory::
print() const noexcept
{
  int  d = 0;

    for(auto&  nd: *this)
    {
        if(nd.is_directory())
        {
          ++d;
        }

 
      nd.print();

      printf("\n");
    }


  printf("%d個のノード(%d個のディレクトリ)\n",m_number_of_nodes,d);
}




directory::iterator&
directory::iterator::
operator++() noexcept
{
  m_pointer = m_pointer->m_next;

  return *this;
}


directory::iterator
directory::iterator::
operator++(int) noexcept
{
  directory::iterator  tmp(m_pointer)                   ;
                           m_pointer = m_pointer->m_next;

  return tmp;
}




}}




