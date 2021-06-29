#ifndef gbstd_dn_HPP
#define gbstd_dn_HPP


#include"libgbstd/utility.hpp"
#include"libgbstd/character.hpp"
#include"libgbstd/syntax.hpp"
#include<new>
#include<utility>
#include<string>
#include<string_view>
#include<cstdint>


namespace  gbstd{


class dn_array;
class dn_list;


class
dn_object
{
  std::u16string  m_name;

  enum class kind{
    null,
    boolean,
    integer,
    unsigned_integer,
    string,
    floating,
    list,
    array,

    undefined,

  } m_kind=kind::undefined;

  union data{
    bool            b;
    int64_t         i;
    uint64_t        u;
    std::u16string  s;
    double          f;

    dn_array*     arr;
    dn_list*       ls;

    data(){}
   ~data(){}

  } m_data;

  dn_object(kind  k) noexcept: m_kind(k){}

public:
  dn_object() noexcept{}
  dn_object(const dn_object&   rhs) noexcept{assign(rhs);}
  dn_object(      dn_object&&  rhs) noexcept{assign(std::move(rhs));}
 ~dn_object(){clear();}

  template<class...  Args>
  explicit dn_object(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class...  Args>
  dn_object&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  dn_object&  assign(syntax_token_string::iterator&  it) noexcept;
  dn_object&  assign(bool  b) noexcept;
  dn_object&  assign(int  i) noexcept{return assign(static_cast<int64_t>(i));}
  dn_object&  assign(int64_t  i) noexcept;
  dn_object&  assign(uint64_t  u) noexcept;
  dn_object&  assign(double  f) noexcept;
  dn_object&  assign(const char16_t*  s) noexcept{return assign(std::u16string_view(s));}
  dn_object&  assign(std::u16string_view  sv) noexcept;
  dn_object&  assign(dn_array&&  arr) noexcept;
  dn_object&  assign(dn_list&&  ls) noexcept;
  dn_object&  assign(const dn_object&   rhs) noexcept;
  dn_object&  assign(      dn_object&&  rhs) noexcept;

  void  clear() noexcept;

  const std::u16string&  name() const noexcept{return m_name;}

  dn_object&  set_name(std::u16string_view  name) noexcept;

  bool  has_name() const noexcept{return m_name.size();}

  bool  is_null()             const noexcept{return m_kind == kind::null;}
  bool  is_undefined()        const noexcept{return m_kind == kind::undefined;}
  bool  is_boolean()          const noexcept{return m_kind == kind::boolean;}
  bool  is_integer()          const noexcept{return m_kind == kind::integer;}
  bool  is_unsigned_integer() const noexcept{return m_kind == kind::unsigned_integer;}
  bool  is_floating()         const noexcept{return m_kind == kind::floating;}
  bool  is_string()           const noexcept{return m_kind == kind::string;}
  bool  is_array()            const noexcept{return m_kind == kind::array;}
  bool  is_list()             const noexcept{return m_kind == kind::list;}

  bool                   boolean()   const noexcept{return m_data.b;}
  int64_t                integer()   const noexcept{return m_data.i;}
  uint64_t      unsigned_integer()   const noexcept{return m_data.u;}
  double                 floating()  const noexcept{return m_data.f;}
  const std::u16string&  string()    const noexcept{return m_data.s;}
  dn_array&              array()     const noexcept{return *m_data.arr;}
  dn_list&               list()      const noexcept{return *m_data.ls;}

  static dn_object  make_null() noexcept{return dn_object(kind::null);}
  static dn_object  make_undefined() noexcept{return dn_object(kind::undefined);}

  void  print() const noexcept;

};


dn_object  make_object_from_string(std::u16string_view  sv) noexcept;


class
dn_array
{
  dn_object*  m_data=nullptr;

  size_t  m_number_of_elements=0;

  size_t  m_length=0;

  void  reallocate(size_t  n) noexcept;

public:
  dn_array() noexcept{}
  dn_array(const dn_array&   rhs) noexcept{assign(rhs);}
  dn_array(      dn_array&&  rhs) noexcept{assign(std::move(rhs));}
 ~dn_array(){clear();}

  template<class...  Args>
  explicit dn_array(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class...  Args>
  dn_array&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  dn_object&  operator[](int  i) const noexcept{return m_data[i];}
  dn_object&  operator[](std::u16string_view  sv) const noexcept;

  dn_array&  assign(syntax_token_string::iterator&  it) noexcept;
  dn_array&  assign(std::initializer_list<dn_object>  ls) noexcept;
  dn_array&  assign(const dn_array&   rhs) noexcept;
  dn_array&  assign(      dn_array&&  rhs) noexcept;

  void  clear() noexcept;

  void  resize(size_t  n) noexcept;

  dn_object&  push(const dn_object&   v) noexcept{return push(dn_object(v));}
  dn_object&  push(      dn_object&&  v) noexcept;

  template<class...  Args>
  dn_object&  emplace(Args&&...  args) noexcept{return push(dn_object(std::forward<Args>(args)...));}

  dn_object&   pop()       noexcept;
  dn_object&  back() const noexcept{return m_data[m_length-1];}

  std::vector<std::reference_wrapper<dn_object>>  find(std::u16string_view  name) const noexcept;

  size_t  size() const noexcept{return m_length;}

  dn_object*  begin() const noexcept{return m_data;}
  dn_object*    end() const noexcept{return m_data+m_length;}

  void  print() const noexcept;

};


class
dn_list
{
  struct node{
    dn_object  m_object;

    node*  m_prev;
    node*  m_next;

    node(dn_object&&  v) noexcept: m_object(std::move(v)){}
  };

  node*  m_first=nullptr;
  node*  m_last =nullptr;

  size_t  m_length=0;

public:
  dn_list() noexcept{}
  dn_list(const dn_list&   rhs) noexcept{assign(rhs);}
  dn_list(      dn_list&&  rhs) noexcept{assign(std::move(rhs));}
 ~dn_list(){clear();}

  template<class...  Args>
  explicit dn_list(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class...  Args>
  dn_list&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  dn_list&  assign(syntax_token_string::iterator&  it) noexcept;
  dn_list&  assign(std::initializer_list<dn_object>  ls) noexcept;
  dn_list&  assign(const dn_list&   rhs) noexcept;
  dn_list&  assign(      dn_list&&  rhs) noexcept;

  void  clear() noexcept;

  void  resize(size_t  n) noexcept;

  dn_object&  push_front(const dn_object&   v) noexcept{return push_front(dn_object(v));}
  dn_object&  push_front(      dn_object&&  v) noexcept;

  dn_object&  push_back(const dn_object&   v)  noexcept{return push_back(dn_object(v));}
  dn_object&  push_back(      dn_object&&  v)  noexcept;


  template<class...  Args>
  dn_object&  emplace_front(Args&&...  args) noexcept{return push_front(dn_object(std::forward<Args>(args)...));}

  template<class...  Args>
  dn_object&  emplace_back(Args&&...  args) noexcept{return push_back(dn_object(std::forward<Args>(args)...));}


  dn_object   pop_back()  noexcept;
  dn_object   pop_front() noexcept;

  dn_object&  front() const noexcept{return m_first->m_object;}
  dn_object&   back() const noexcept{return m_last->m_object;}

  std::vector<std::reference_wrapper<dn_object>>  find(std::u16string_view  name) const noexcept;

  size_t  size() const noexcept{return m_length;}

  void  print() const noexcept;


  class iterator{
    node*  m_node=nullptr;

    friend class dn_list;

  public:
    constexpr iterator() noexcept{}
    constexpr iterator(node*  nd) noexcept: m_node(nd){}

    constexpr operator bool() const noexcept{return m_node;}

    constexpr bool  operator==(node*  nd) const noexcept{return m_node == nd;}
    constexpr bool  operator!=(node*  nd) const noexcept{return m_node != nd;}

    dn_object&  operator*() const noexcept{return m_node->m_object;}

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


  dn_object&  insert_before(iterator  it, dn_object  v) noexcept;
  dn_object&  insert_after( iterator  it, dn_object  v) noexcept;

  iterator  erase(iterator  it) noexcept;

};




}




#endif




