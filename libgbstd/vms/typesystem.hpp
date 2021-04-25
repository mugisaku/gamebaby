#ifndef gbstd_vms_typesystem_HPP
#define gbstd_vms_typesystem_HPP


#include<cstddef>
#include<cstdint>
#include<cstring>
#include<cinttypes>
#include<cstdio>
#include<string>
#include<string_view>
#include<new>
#include<list>
#include<array>
#include<memory>
#include<vector>
#include"libgbstd/utility.hpp"
#include"libgbstd/character.hpp"




namespace gbstd{
namespace typesystem{




class  basic_type_info;
class  type_info;
class  type_lib;

class    pointer_info;
class  reference_info;
class      alias_info;
class      array_info;

class  struct_decl;
class  struct_def;
class   union_decl;
class   union_def;
class    enum_decl;
class    enum_def;

class  fn_parameter;
class  fn_signature;
class  fn_pointer_info;




class
basic_type_info
{
  char  m_letter=0;
  char  m_size=0;

public:
  constexpr basic_type_info(int  l=0, int  s=0) noexcept: m_letter(l), m_size(s){}

  constexpr int  size() const noexcept{return m_size;}

  constexpr bool  is_null()             const noexcept{return m_letter == 'n';}
  constexpr bool  is_void()             const noexcept{return m_letter == 'v';}
  constexpr bool  is_undefined()        const noexcept{return m_letter == 'd';}
  constexpr bool  is_null_pointer()     const noexcept{return m_letter == 'p';}
  constexpr bool  is_boolean()          const noexcept{return m_letter == 'b';}
  constexpr bool  is_character()        const noexcept{return m_letter == 'c';}
  constexpr bool  is_integer()          const noexcept{return m_letter == 'i';}
  constexpr bool  is_unsigned_integer() const noexcept{return m_letter == 'u';}
  constexpr bool  is_floating()         const noexcept{return m_letter == 'f';}

  void  print() const noexcept{printf("%c%d",m_letter,8*m_size);}

};


class
type_info
{
  const type_lib*  m_lib=nullptr;

  enum class kind{
    null,
    basic,
    alias,
    pointer,
    reference,
    array,
    struct_,
    union_,
    enum_,
    function_pointer,
  } m_kind=kind::null;

  union data{
    basic_type_info  bas;

    pointer_info*     ptr;
    reference_info*   ref;
    alias_info*       ali;
    array_info*       arr;

    const struct_decl*  st;
    const  union_decl*  un;
    const   enum_decl*  en;

    data() noexcept{}
   ~data(){}

  } m_data;

public:
  type_info() noexcept{}
  type_info(const type_info&   rhs) noexcept{assign(rhs);}
  type_info(      type_info&&  rhs) noexcept{assign(std::move(rhs));}
 ~type_info(){clear();}

  template<class...  Args>
  type_info(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class...  Args>
  type_info&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  type_info&  operator=(const type_info&   rhs) noexcept{return assign(rhs);}
  type_info&  operator=(      type_info&&  rhs) noexcept{return assign(std::move(rhs));}

  type_info&  assign(const type_info&   rhs) noexcept;
  type_info&  assign(      type_info&&  rhs) noexcept;
  type_info&  assign(basic_type_info  b) noexcept;
  type_info&  assign(pointer_info&&  ptr) noexcept;
  type_info&  assign(reference_info&&  ref) noexcept;
  type_info&  assign(alias_info&&  ali) noexcept;
  type_info&  assign(array_info&&  arr) noexcept;


  void  clear() noexcept;

  const type_lib&  lib() const noexcept{return *m_lib;}

  bool  is_basic()            const noexcept{return m_kind == kind::basic;}
  bool  is_alias()            const noexcept{return m_kind == kind::alias;}
  bool  is_pointer()          const noexcept{return m_kind == kind::pointer;}
  bool  is_reference()        const noexcept{return m_kind == kind::reference;}
  bool  is_array()            const noexcept{return m_kind == kind::array;}
  bool  is_struct()           const noexcept{return m_kind == kind::struct_;}
  bool  is_union()            const noexcept{return m_kind == kind::union_;}
  bool  is_function_pointer() const noexcept{return m_kind == kind::function_pointer;}

  int   size() const noexcept;
  int  align() const noexcept;

  const basic_type_info&   get_basic_type_info() const noexcept{return m_data.bas;}

  const pointer_info&       get_pointer_info() const noexcept{return *m_data.ptr;}
  const reference_info&   get_reference_info() const noexcept{return *m_data.ref;}
  const alias_info&           get_alias_info() const noexcept{return *m_data.ali;}
  const array_info&           get_array_info() const noexcept{return *m_data.arr;}

  const struct_decl&  get_struct_decl() const noexcept{return *m_data.st;}
  const union_decl&    get_union_decl() const noexcept{return *m_data.un;}
  const enum_decl&      get_enum_decl() const noexcept{return *m_data.en;}

  type_info  form_array(int  n) const noexcept;
  type_info  form_pointer(int  sz) const noexcept;
  type_info  form_reference(int  sz) const noexcept;

  type_info  remove_array() const noexcept;
  type_info  remove_pointer() const noexcept;
  type_info  remove_reference() const noexcept;

  void  print() const noexcept;

};




class
type_lib
{
  const type_lib*  m_parent;

  std::list<alias_info>    m_alias_info_list;
  std::list<struct_decl>  m_struct_decl_list;
  std::list<union_decl>    m_union_decl_list;
  std::list<enum_decl>      m_enum_decl_list;

public:


};




class
pointer_info
{
protected:
  type_info  m_target;

  int  m_size=0;

public:
  pointer_info(const type_info&   target, int  sz) noexcept: m_target(target),            m_size(sz){}
  pointer_info(      type_info&&  target, int  sz) noexcept: m_target(std::move(target)), m_size(sz){}

  const type_info&  target() const noexcept{return m_target;}

  int   size() const noexcept{return m_size;}
  int  align() const noexcept{return m_size;}

  void  print() const noexcept
  {
    m_target.print();

    printf("*");
  }

};


class
reference_info: public pointer_info
{
public:
  reference_info(const type_info&   target, int  sz) noexcept: pointer_info(target,           sz){}
  reference_info(      type_info&&  target, int  sz) noexcept: pointer_info(std::move(target),sz){}

  void  print() const noexcept
  {
    m_target.print();

    printf("&");
  }

};




class
array_info
{
  type_info  m_target;

  int  m_number_of_elements;

public:
  array_info(const type_info&  t, int  n) noexcept:
  m_target(t), m_number_of_elements(n){}

  array_info(type_info&&  t, int  n) noexcept:
  m_target(std::move(t)), m_number_of_elements(n){}

  const type_info&  target() const noexcept{return m_target;}

  int  number_of_elements() const noexcept{return m_number_of_elements;}

  int   size() const noexcept{return m_target.size()*m_number_of_elements;}
  int  align() const noexcept{return m_target.align();}

  void  print() const noexcept
  {
    m_target.print();

    printf("[%d]",m_number_of_elements);
  }

};




class
alias_info
{
  type_info  m_target;

  std::u16string  m_name;

public:
  alias_info(const type_info&   t, std::u16string_view  name) noexcept: m_target(t), m_name(name){}
  alias_info(      type_info&&  t, std::u16string_view  name) noexcept: m_target(std::move(t)), m_name(name){}

  const type_info&  target() const noexcept{return m_target;}

  int   size() const noexcept{return m_target.size();}
  int  align() const noexcept{return m_target.align();}

  const std::u16string&  name() const noexcept{return m_name;}

  void  print() const noexcept{m_target.print();}

};




class
struct_decl
{
  std::unique_ptr<struct_def>  m_def;

  std::u16string  m_name;

public:
  struct_decl(std::u16string_view  name) noexcept: m_name(name){}

  operator bool() const noexcept{return m_def.get();}

  struct_def&  operator*()  const noexcept{return *m_def;}
  struct_def*  operator->() const noexcept{return  m_def.get();}

  int   size() const noexcept;
  int  align() const noexcept;

  const std::u16string&  name() const noexcept{return m_name;}

  struct_def&  set_def(struct_def&&  def) noexcept;

  struct_def&  get_def() const noexcept{return *m_def;}

  void  print() const noexcept;

};


class
struct_member
{
  type_info  m_type_info;

  std::u16string  m_name;

  int  m_offset;

public:
  struct_member(type_info&&  ti, std::u16string_view  name, int  offset) noexcept:
  m_type_info(std::move(ti)), m_name(name), m_offset(offset){}

  const type_info&  type() const noexcept{return m_type_info;}

  const std::u16string&  name() const noexcept{return m_name;}

  int  offset() const noexcept{return m_offset;}

};


class
struct_def
{
  std::vector<struct_member>  m_member_list;

  int  m_size =0;
  int  m_align=0;

public:
  void  push(const type_info&   ti, std::u16string_view  name) noexcept{push(type_info(ti),name);}
  void  push(      type_info&&  ti, std::u16string_view  name) noexcept;

  const struct_member*  find(std::u16string_view  name) const noexcept;

  int   size() const noexcept{return m_size;}
  int  align() const noexcept{return m_align;}

  void  print() const noexcept;

};




class
union_decl
{
  std::unique_ptr<union_def>  m_def;

  std::u16string  m_name;

public:
  union_decl(std::u16string_view  name) noexcept: m_name(name){}

  operator bool() const noexcept{return m_def.get();}

  union_def&  operator*()  const noexcept{return *m_def;}
  union_def*  operator->() const noexcept{return  m_def.get();}

  int   size() const noexcept;
  int  align() const noexcept;

  const std::u16string&  name() const noexcept{return m_name;}

  union_def&  set_def(union_def&&  def) noexcept;
  union_def&  get_def() const noexcept{return *m_def;}

  void  print() const noexcept;

};


class
union_def
{
  struct member{
    type_info  m_type_info;

    std::u16string  m_name;

    member(type_info&&  ti, std::u16string_view  name) noexcept: m_type_info(std::move(ti)), m_name(name){}
  };

  std::vector<member>  m_member_list;

  int  m_size =0;
  int  m_align=0;

public:
  void  push(const type_info&   ti, std::u16string_view  name) noexcept{push(type_info(ti),name);}
  void  push(      type_info&&  ti, std::u16string_view  name) noexcept;

  type_info  find(std::u16string_view  name) const noexcept;

  int   size() const noexcept{return m_size;}
  int  align() const noexcept{return m_align;}

  void  print() const noexcept;

};




class
enumerator
{
  std::u16string  m_name;

  int64_t  m_value;

public:
  enumerator(std::u16string_view  name, int64_t  value) noexcept:
  m_name(name), m_value(value){}

  const std::u16string&  name() const noexcept{return m_name;}

  const int64_t&  value() const noexcept{return m_value;}

  void  print() const noexcept
  {
    gbstd::print(m_name);
    printf(" = %" PRIi64,m_value);
  }

};


class
enum_decl
{
  std::unique_ptr<enum_def>  m_def;

  std::u16string  m_name;

public:
  enum_decl(std::u16string_view  name) noexcept: m_name(name){}

  operator bool() const noexcept{return m_def.get();}

  enum_def&  operator*()  const noexcept{return *m_def;}
  enum_def*  operator->() const noexcept{return  m_def.get();}

  int  size() const noexcept;

  const std::u16string&  name() const noexcept{return m_name;}

  enum_def&  set_def(enum_def&&  def) noexcept;
  enum_def&  get_def() const noexcept{return *m_def;}

  void  print() const noexcept;

};


class
enum_def
{
  std::vector<enumerator>  m_enumerator_list;

  int  m_size=0;

public:
  enum_def(int  sz) noexcept: m_size(sz){}

  int  size() const noexcept{return m_size;}

  enumerator&  push(std::u16string_view  name, int  value) noexcept;

  const int64_t*  find(std::u16string_view  name) const noexcept;

  void  print() const noexcept;

};




class
fn_parameter
{
  type_info  m_type_info;

  std::u16string  m_name;

public:
  fn_parameter(const type_info&  ti, std::u16string_view  name) noexcept:
  m_type_info(ti), m_name(name){}

  fn_parameter(type_info&&  ti, std::u16string_view  name) noexcept:
  m_type_info(std::move(ti)), m_name(name){}

  const type_info&  type() const noexcept{return m_type_info;}

  const std::u16string&  name() const noexcept{return m_name;}

};


using fn_parameter_list = std::vector<fn_parameter>;


class
fn_signature
{
  type_info  m_return_type_info;

  fn_parameter_list  m_parameter_list;

public:
  fn_signature(const type_info&  retti, fn_parameter_list&&  ls={}) noexcept:
  m_return_type_info(retti), m_parameter_list(std::move(ls)){}

  fn_signature(type_info&&  retti, fn_parameter_list&&  ls={}) noexcept:
  m_return_type_info(std::move(retti)), m_parameter_list(std::move(ls)){}

  const type_info&  return_type() const noexcept{return m_return_type_info;}

  const fn_parameter_list&  parameter_list() const noexcept{return m_parameter_list;}

  void  print() const noexcept;

};


class
fn_pointer_info
{
  fn_signature  m_signature;

  int  m_size=0;

public:
  fn_pointer_info(const fn_signature&   sig, int  sz) noexcept: m_signature(sig),            m_size(sz){}
  fn_pointer_info(      fn_signature&&  sig, int  sz) noexcept: m_signature(std::move(sig)), m_size(sz){}

  const fn_signature&  signature() const noexcept{return m_signature;}

  int  size() const noexcept{return m_size;}

};




}}


#endif




