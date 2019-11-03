#ifndef GBSTD_GPFS_hpp_was_included
#define GBSTD_GPFS_hpp_was_included


#include<cstdint>
#include<cstdio>
#include"libgbstd/utility.hpp"
#include"libgbstd/misc.hpp"
#include"libgbstd/video.hpp"
#include"libgbstd/control.hpp"


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

  operator bool() const noexcept{return m_data.size();}

  path&  assign(std::string_view  sv) noexcept;
  path&  append(std::string_view  sv) noexcept;

  int  get_number_of_elements() const noexcept{return m_data.size();}

  bool  is_absolute() const noexcept{return m_absolute;}

  const std::string&  get_first_name() const noexcept{return m_data.front();}
  const std::string&  get_last_name() const noexcept{return m_data.back();}

  const std::string*  begin() const noexcept{return m_data.data();}
  const std::string*    end() const noexcept{return m_data.data()+m_data.size();}

  void  print() const noexcept;

  static bool  test(std::string_view  sv) noexcept;

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

  bool  remove_node(node*  nd) noexcept;
  void  append_node(node*  nd) noexcept;

  void  clear() noexcept;

public:
  directory(const directory& ) noexcept=delete;
  directory(      directory&&) noexcept=delete;

  directory&  operator=(const directory& ) noexcept=delete;
  directory&  operator=(      directory&&) noexcept=delete;

  node&  operator[](std::string_view  nodepath) noexcept;

  bool  is_root() const noexcept;

  node*    get_self_node() const noexcept{return m_self_node;}
  node*  get_parent_node() const noexcept;

  node*  create_node_by_name(std::string_view      name) noexcept;
  node*  create_node_by_path(std::string_view  nodepath) noexcept;

  directory*  create_directory(std::string_view  dirpath) noexcept;

  node*  find_node_by_name(std::string_view      name) const noexcept;
  node*  find_node_by_path(std::string_view  nodepath) const noexcept;

  directory*  find_directory(std::string_view  dirpath) const noexcept;

  directory&  get_root_directory() const noexcept;
  directory&  get_parent_directory() const noexcept;

  bool  move(std::string_view  src_nodepath, std::string_view  dst_dirpath) noexcept;

  bool  remove(std::string_view  nodepath) noexcept;

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

    iterator&  operator++(   ) noexcept;
    iterator   operator++(int) noexcept;

  };

  iterator  begin() const noexcept{return iterator(m_first_node);}
  iterator    end() const noexcept{return iterator();}

};


class node;


class
node_reference
{
  friend class node;

  struct data;

  data*  m_data=nullptr;

  void  unrefer() noexcept;
  void  unpoint() noexcept;

  void  initialize(node&  nd) noexcept;

public:
  node_reference() noexcept{}
  node_reference(node&  nd) noexcept{assign(nd);}
  node_reference(const node_reference&   rhs) noexcept{assign(rhs);}
  node_reference(      node_reference&&  rhs) noexcept{assign(std::move(rhs));}
 ~node_reference(){unrefer();}

  node_reference&  operator=(node&  nd) noexcept{return assign(nd);}
  node_reference&  operator=(const node_reference&   rhs) noexcept{return assign(rhs);}
  node_reference&  operator=(      node_reference&&  rhs) noexcept{return assign(std::move(rhs));}

  operator bool() const noexcept;

  node&  operator*()  const noexcept;
  node*  operator->() const noexcept;

  node_reference&  assign(node&  nd) noexcept;
  node_reference&  assign(const node_reference&   rhs) noexcept;
  node_reference&  assign(      node_reference&&  rhs) noexcept;

  void  print() const noexcept;

};


class
node
{
  friend class node_reference;
  friend class directory;

  node*  m_previous=nullptr;
  node*  m_next=nullptr;

  node*  m_parent=nullptr;

  node_reference  m_self_reference;

  std::string  m_name;

  struct flags{
    static constexpr int  ignore = 1;
    static constexpr int    lock = 2;

  };

  status_value<int>  m_status;

  enum class kind{
         null,
    directory,
      integer,
  real_number,
      pointer,
    reference,
     callback,
       sprite,
        timer,
        clock,

  } m_kind=kind::null;

  union data{
    directory  dir;

    int     i;
    double  d;
    void*            ptr;
    callback_wrapper  cb;
    node_reference   ref;
    sprite  spr;
    timer   tmr;
    clock   clk;

    data() noexcept{}
   ~data(){}

  } m_data;

  node() noexcept;
  node(std::string_view  name) noexcept;

public:
  static node*  create_root() noexcept{return new node;}

 ~node(){  clear();  m_self_reference.unpoint();  m_self_reference.unrefer();}

  node(const node& ) noexcept=delete;
  node(      node&&) noexcept=delete;

  node&  operator=(const node& ) noexcept=delete;
  node&  operator=(      node&&) noexcept=delete;

  void  clear() noexcept;

  operator bool() const noexcept{return !is_null();}

  node&    set_ignore_flag() noexcept{  m_status.set(  flags::ignore);  return *this;}
  node&  unset_ignore_flag() noexcept{  m_status.unset(flags::ignore);  return *this;}

  bool  test_ignore_flag() const noexcept{return m_status.test(flags::ignore);}

  void*&             be_pointer() noexcept;
  int&               be_integer() noexcept;
  double&            be_real_number() noexcept;
  callback_wrapper&  be_callback() noexcept;
  directory&         be_directory() noexcept;
  node_reference&    be_reference() noexcept;
  sprite&            be_sprite() noexcept;
  timer&             be_timer() noexcept;
  clock&             be_clock() noexcept;

  const std::string&  get_name() const noexcept{return m_name;}
  void                set_name(std::string_view  sv) noexcept{m_name = sv;}

  bool  is_null()        const noexcept{return m_kind == kind::null;}
  bool  is_pointer()     const noexcept{return m_kind == kind::pointer;}
  bool  is_integer()     const noexcept{return m_kind == kind::integer;}
  bool  is_real_number() const noexcept{return m_kind == kind::real_number;}
  bool  is_callback()    const noexcept{return m_kind == kind::callback;}
  bool  is_sprite()      const noexcept{return m_kind == kind::sprite;}
  bool  is_timer()       const noexcept{return m_kind == kind::timer;}
  bool  is_clock()       const noexcept{return m_kind == kind::clock;}
  bool  is_directory()   const noexcept{return m_kind == kind::directory;}
  bool  is_reference()   const noexcept{return m_kind == kind::reference;}

  int&     get_integer() noexcept{return m_data.i;}
  double&  get_real_number() noexcept{return m_data.d;}

  callback_wrapper&  get_callback() noexcept{return m_data.cb;}
  sprite&            get_sprite() noexcept{return m_data.spr;}
  timer&             get_timer() noexcept{return m_data.tmr;}
  clock&             get_clock() noexcept{return m_data.clk;}

  node_reference&  get_reference() noexcept{return m_data.ref;}

  void*&  get_pointer() noexcept{return m_data.ptr;}

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




