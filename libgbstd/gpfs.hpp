#ifndef GBSTD_GPFS_hpp_was_included
#define GBSTD_GPFS_hpp_was_included


#include<cstdint>
#include<cstdio>
#include"libgbstd/utility.hpp"
#include"libgbstd/misc.hpp"


namespace gbstd{
namespace gpfs{




class node;
class directory;


class
path
{
  std::vector<std::string>  m_data;

  bool  m_absolute;

public:
  path() noexcept;
  path(std::string_view  sv) noexcept{assign(sv);}

  path&  operator =(std::string_view  sv) noexcept{return assign(sv);}
  path&  operator+=(std::string_view  sv) noexcept{return append(sv);}

  path&  assign(std::string_view  sv) noexcept;
  path&  append(std::string_view  sv) noexcept;

  bool  is_absolute() const noexcept{return m_absolute;}

  const std::string*  begin() const noexcept{return m_data.data();}
  const std::string*    end() const noexcept{return m_data.data()+m_data.size();}

  void  print() const noexcept;

};


class
directory
{
  friend class node;

  node*  m_self_node=nullptr;

  node*  m_first_node=nullptr;
  node*  m_last_node=nullptr;

  uint32_t  m_number_of_nodes=0;

  directory() noexcept{}
 ~directory(){clear();}

  void  clear() noexcept;

public:
  directory(const directory& ) noexcept=delete;
  directory(      directory&&) noexcept=delete;

  directory&  operator=(const directory& ) noexcept=delete;
  directory&  operator=(      directory&&) noexcept=delete;

  bool  is_root() const noexcept;

  node*    get_self_node() const noexcept{return m_self_node;}
  node*  get_parent_node() const noexcept;

  node&  create_node(std::string_view  name) noexcept;

  directory&  create_directory(std::string_view  name) noexcept;
  void          create_pointer(void*  ptr, std::string_view  name) noexcept;

  node*  find_by_name(std::string_view  name) const noexcept;
  node*  find_by_path(const gpfs::path&  path) const noexcept;

  directory*  find_directory_by_name(std::string_view  name) const noexcept;
  directory&  get_root_directory() const noexcept;
  directory&  get_parent_directory() const noexcept;

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

  struct flags{
    static constexpr int  active = 1;
    static constexpr int    lock = 2;

  };

  status_value<int>  m_status;

  enum class kind{
         null,
    directory,
      integer,
  real_number,
      pointer,
     callback,

  } m_kind=kind::null;

  union data{
    directory  dir;

    int     i;
    double  d;
    void*            ptr;
    callback_wrapper  cb;

    data() noexcept{}
   ~data(){}

  } m_data;

public:
  node() noexcept;
  node(node&  parent, std::string_view  name) noexcept;
 ~node(){clear();}

  node(const node& ) noexcept=delete;
  node(      node&&) noexcept=delete;

  node&  operator=(const node& ) noexcept=delete;
  node&  operator=(      node&&) noexcept=delete;

  void  clear() noexcept;

  operator bool() const noexcept{return !is_null();}

  node&  be_pointer(void*  ptr) noexcept;
  node&  be_integer(int  i) noexcept;
  node&  be_real_number(double  d) noexcept;
  node&  be_callback(callback_wrapper  cb) noexcept;
  node&  be_directory() noexcept;

  const std::string&  get_name() const noexcept{return m_name;}
  void                set_name(std::string_view  sv) noexcept{m_name = sv;}

  bool  is_null()        const noexcept{return m_kind == kind::null;}
  bool  is_pointer()     const noexcept{return m_kind == kind::pointer;}
  bool  is_integer()     const noexcept{return m_kind == kind::integer;}
  bool  is_real_number() const noexcept{return m_kind == kind::real_number;}
  bool  is_callback()    const noexcept{return m_kind == kind::callback;}
  bool  is_directory()   const noexcept{return m_kind == kind::directory;}

  int&     get_integer() noexcept{return m_data.i;}
  double&  get_real_number() noexcept{return m_data.d;}

  callback_wrapper&  get_callback() noexcept{return m_data.cb;}

  void*  get_pointer() const noexcept{return m_data.ptr;}

  template<typename  T>
  T*  get_pointer() const noexcept{return static_cast<T*>(m_data.ptr);}

  directory&  get_parent_directory() const noexcept{return m_parent->get_directory();}

        directory&  get_directory()       noexcept{return m_data.dir;}
  const directory&  get_directory() const noexcept{return m_data.dir;}

  void  print() const noexcept;

  static void  hook(node*  a, node*  b) noexcept;

  void  unhook() noexcept;

};




}}


#endif




