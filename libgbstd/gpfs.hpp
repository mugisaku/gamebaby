#ifndef GBSTD_GPFS_hpp_was_included
#define GBSTD_GPFS_hpp_was_included


#include<cstdint>
#include<cstdio>
#include"libgbstd/utility.hpp"


namespace gbstd{
namespace gpfs{




class node;
class directory;


class
directory
{
  friend class node;

  node*  m_parent_node;
  node*  m_self_node=nullptr;

  node*  m_first_node=nullptr;
  node*  m_last_node=nullptr;

  uint32_t  m_number_of_nodes=0;

  void  set_self_node(node&  nd) noexcept;

public:
  directory(node*  parent_node=nullptr) noexcept;
 ~directory(){clear();}

  void  clear() noexcept;

  bool  is_root() const noexcept{return !m_parent_node;}

  node*  get_parent_node() const noexcept{return m_parent_node;}

  node&  create_node(std::string_view  name) noexcept;

  directory&  create_directory(std::string_view  name) noexcept;
  void          create_pointer(void*  ptr, std::string_view  name) noexcept;

  node*  find(std::string_view  name) const noexcept;

  void  remove(std::string_view  name) noexcept;
  void  remove(node*  ln) noexcept;

  void  print() const noexcept;

  class iterator{
    node*  m_pointer;
  public:
    constexpr iterator(node*  ptr=nullptr) noexcept: m_pointer(ptr){}

    constexpr operator bool() const noexcept{return m_pointer;}

    constexpr bool  operator==(iterator  rhs) const noexcept{return m_pointer == rhs.m_pointer;}
    constexpr bool  operator!=(iterator  rhs) const noexcept{return m_pointer != rhs.m_pointer;}

    node&  operator*()  const noexcept{return *m_pointer;}
    node*  operator->() const noexcept{return  m_pointer;}

    iterator&  operator++() noexcept;

  };

  iterator  begin() const noexcept{return iterator(m_first_node);}
  iterator    end() const noexcept{return iterator();}

};


class
node
{
  friend class directory;

  node*  m_previous=nullptr;
  node*  m_next=nullptr;

  node*  m_parent;

  std::string  m_name;

  enum class{
    static constexpr int  active = 1;
    static constexpr int    lock = 2;

  };

  status_value<int>  m_status;

  enum class kind{
         null,
      pointer,
    directory,

  } m_kind=kind::null;

  union data{
    void*      ptr;
    directory  dir;

    data() noexcept{}
   ~data(){}

  } m_data;

public:
  node(node*  parent, std::string_view  name) noexcept;
 ~node(){clear();}

  operator bool() const noexcept{return !is_null();}

  void  clear() noexcept;

  node&  be_pointer(void*  ptr) noexcept;
  node&  be_directory() noexcept;

  const std::string&  get_name() const noexcept{return m_name;}

  bool  is_null()      const noexcept{return m_kind == kind::null;}
  bool  is_pointer()   const noexcept{return m_kind == kind::pointer;}
  bool  is_directory() const noexcept{return m_kind == kind::directory;}

  void*  get_pointer() const noexcept{return m_data.ptr;}

  directory&  get_parent_directory() const noexcept{return m_parent_node->get_directory();}

        directory&  get_directory()       noexcept{return m_data.dir;}
  const directory&  get_directory() const noexcept{return m_data.dir;}

  void  print() const noexcept;

  static void  hook(node*  a, node*  b) noexcept;

  void  unhook() noexcept;

};




}}


#endif




