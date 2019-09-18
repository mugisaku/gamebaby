#ifndef gbstd_value_HPP
#define gbstd_value_HPP


#include"libgbstd/utility.hpp"
#include"libgbstd/control.hpp"
#include<new>
#include<utility>
#include<string>
#include<cstdint>


namespace  gbstd{
namespace values{


class array;
class list;


class
object
{
  bool  m_has_name=false;

  std::string  m_name;

  enum class kind{
    null,
    boolean,
    integer,
    string,
    u16string,
    real_number,
    list,
    array,

    undefined,

  } m_kind=kind::undefined;

  union data{
    bool                 b;
    int                  i;
    std::string          s;
    std::u16string    u16s;
    double               r;

    array*     arr;
    list*       ls;

    data(){}
   ~data(){}

  } m_data;

  object(kind  k) noexcept: m_kind(k){}

public:
  object() noexcept{}
  object(bool  b) noexcept{assign(b);}
  object(int  i) noexcept{assign(i);}
  object(std::string_view  sv) noexcept{assign(sv);}
  object(std::u16string_view  sv) noexcept{assign(sv);}
  object(array&&  arr) noexcept{assign(std::move(arr));}
  object(list&&  ls) noexcept{assign(std::move(ls));}
  object(const object&   rhs) noexcept{*this = rhs;}
  object(      object&&  rhs) noexcept{*this = std::move(rhs);}
 ~object(){clear();}

  object&  operator=(bool  b) noexcept{return assign(b);}
  object&  operator=(int  i) noexcept{return assign(i);}
  object&  operator=(std::string_view  sv) noexcept{return assign(sv);}
  object&  operator=(std::u16string_view  sv) noexcept{return assign(sv);}
  object&  operator=(array&&  arr) noexcept{return assign(std::move(arr));}
  object&  operator=(list&&  ls) noexcept{return assign(std::move(ls));}
  object&  operator=(const object&   rhs) noexcept{return assign(rhs);}
  object&  operator=(      object&&  rhs) noexcept{return assign(std::move(rhs));}

  object&  assign(bool  b) noexcept;
  object&  assign(int  i) noexcept;
  object&  assign(std::string_view  sv) noexcept;
  object&  assign(std::u16string_view  sv) noexcept;
  object&  assign(array&&  arr) noexcept;
  object&  assign(list&&  ls) noexcept;
  object&  assign(const object&   rhs) noexcept;
  object&  assign(      object&&  rhs) noexcept;

  void  clear() noexcept;

  const std::string&  get_name(                      ) const noexcept{return m_name;}
  object&             set_name(std::string_view  name)       noexcept;
  object&           unset_name(                      )       noexcept;

  bool  has_name() const noexcept{return m_has_name;}

  bool  is_null()       const noexcept{return m_kind == kind::null;}
  bool  is_undefined()  const noexcept{return m_kind == kind::undefined;}
  bool  is_boolean()    const noexcept{return m_kind == kind::boolean;}
  bool  is_integer()    const noexcept{return m_kind == kind::integer;}
  bool  is_string()     const noexcept{return m_kind == kind::string;}
  bool  is_u16string()  const noexcept{return m_kind == kind::u16string;}
  bool  is_array()      const noexcept{return m_kind == kind::array;}
  bool  is_list()       const noexcept{return m_kind == kind::list;}

  bool                   get_boolean()    const noexcept{return m_data.b;}
  int                    get_integer()    const noexcept{return m_data.i;}
  const std::string&     get_string()     const noexcept{return m_data.s;}
  const std::u16string&  get_u16string()  const noexcept{return m_data.u16s;}
  array&                 get_array()      const noexcept{return *m_data.arr;}
  list&                  get_list()       const noexcept{return *m_data.ls;}

  static object  make_null() noexcept{return object(kind::null);}
  static object  make_undefined() noexcept{return object(kind::undefined);}

  void  print() const noexcept;

};


extern const object       null;
extern const object  undefined;


class
array
{
  object*  m_data=nullptr;

  size_t  m_number_of_elements=0;

  size_t  m_length=0;

  void  reallocate(size_t  n) noexcept;

public:
  array() noexcept{}
  array(std::initializer_list<object>  ls) noexcept{assign(ls);}
  array(const array&   rhs) noexcept{assign(rhs);}
  array(      array&&  rhs) noexcept{assign(std::move(rhs));}
 ~array(){clear();}

  array&  operator=(std::initializer_list<object>  ls) noexcept{return assign(ls);}
  array&  operator=(const array&   rhs) noexcept{return assign(rhs);}
  array&  operator=(      array&&  rhs) noexcept{return assign(std::move(rhs));}

  object&  operator[](int  i) const noexcept{return m_data[i];}

  array&  assign(std::initializer_list<object>  ls) noexcept;
  array&  assign(const array&   rhs) noexcept;
  array&  assign(      array&&  rhs) noexcept;

  void  clear() noexcept;

  void  resize(size_t  n) noexcept;

  object&  push(const object&   v) noexcept{return push(object(v));}
  object&  push(      object&&  v) noexcept;

  object&   pop()       noexcept;
  object&  back() const noexcept{return m_data[m_length-1];}

  object*  find(std::string_view  name) const noexcept;

  size_t  size() const noexcept{return m_length;}

  object*  begin() const noexcept{return m_data;}
  object*    end() const noexcept{return m_data+m_length;}

  void  print() const noexcept;

};


class
list
{
  struct node{
    object  m_object;

    node*  m_prev;
    node*  m_next;

    node(object&&  v) noexcept: m_object(std::move(v)){}
  };

  node*  m_first=nullptr;
  node*  m_last =nullptr;

  size_t  m_length=0;

public:
  list() noexcept{}
  list(std::initializer_list<object>  ls) noexcept{assign(ls);}
  list(const list&   rhs) noexcept{assign(rhs);}
  list(      list&&  rhs) noexcept{assign(std::move(rhs));}
 ~list(){clear();}

  list&  operator=(std::initializer_list<object>  ls) noexcept{return assign(ls);}
  list&  operator=(const list&   rhs) noexcept{return assign(rhs);}
  list&  operator=(      list&&  rhs) noexcept{return assign(std::move(rhs));}

  list&  assign(std::initializer_list<object>  ls) noexcept;
  list&  assign(const list&   rhs) noexcept;
  list&  assign(      list&&  rhs) noexcept;

  void  clear() noexcept;

  void  resize(size_t  n) noexcept;

  object&  push_front(const object&   v) noexcept{return push_front(object(v));}
  object&  push_front(      object&&  v) noexcept;

  object&  push_back(const object&   v)  noexcept{return push_back(object(v));}
  object&  push_back(      object&&  v)  noexcept;

  object   pop_back()  noexcept;
  object   pop_front() noexcept;

  object&  front() const noexcept{return m_first->m_object;}
  object&   back() const noexcept{return m_last->m_object;}

  object*  find(std::string_view  name) const noexcept;

  size_t  size() const noexcept{return m_length;}

  void  print() const noexcept;


  class iterator{
    node*  m_node=nullptr;

    friend class list;

  public:
    constexpr iterator() noexcept{}
    constexpr iterator(node*  nd) noexcept: m_node(nd){}

    constexpr operator bool() const noexcept{return m_node;}

    constexpr bool  operator==(node*  nd) const noexcept{return m_node == nd;}
    constexpr bool  operator!=(node*  nd) const noexcept{return m_node != nd;}

    object&  operator*() const noexcept{return m_node->m_object;}

    iterator&  operator++() noexcept
    {
      m_node = m_node->m_next;

      return *this;
    }

    iterator&  operator--() noexcept
    {
      m_node = m_node->m_prev;

      return *this;
    }

    iterator  operator++(int) noexcept
    {
      auto  tmp = *this;

      m_node = m_node->m_next;

      return tmp;
    }

    iterator  operator--(int) noexcept
    {
      auto  tmp = *this;

      m_node = m_node->m_prev;

      return tmp;
    }

  };


  iterator  begin() const noexcept{return iterator(m_first);}
  iterator    end() const noexcept{return iterator(       );}


  object&  insert_before(iterator  it, object  v) noexcept;
  object&  insert_after( iterator  it, object  v) noexcept;

  iterator  erase(iterator  it) noexcept;

};




}}




#endif




