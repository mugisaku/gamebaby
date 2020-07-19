#ifndef gbstd_typesystem_HPP
#define gbstd_typesystem_HPP


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
#include"utility.hpp"




namespace gbstd{
namespace typesystem{




class  function_signature;
class  function_pointer_type_def;
class  typed_pointer_type_def;
class  reference_type_def;

class   alias_type_def;
class   array_type_def;

class  struct_type_decl;
class  struct_type_def;
class   union_type_decl;
class   union_type_def;
class    enum_type_decl;
class    enum_type_def;




class
basic_type_info
{
public:
  enum class kinds{
    null,
    undefined,
    alias,
    void_,
    boolean,
    character,
    integer,
    unsigned_integer,
    fpn,
    pointer,
    unsigned_pointer,
    null_pointer,
    function_pointer,
    typed_pointer,
    reference,
    array,
    struct_,
    union_,
    enum_,
  } m_kind=kinds::null;


protected:
  int  m_size =0;
  int  m_align=0;

  std::string_view  m_name;

public:
  constexpr basic_type_info() noexcept{}

  constexpr basic_type_info(kinds  k, int  sz, std::string_view  name) noexcept:
  m_kind(k), m_size(sz), m_align(sz), m_name(name){}

  constexpr basic_type_info(kinds  k, int  sz, int  al, std::string_view  name) noexcept:
  m_kind(k), m_size(sz), m_align(al), m_name(name){}

  constexpr std::string_view  get_name() const noexcept{return m_name;}

  constexpr int   get_size() const noexcept{return  m_size;}
  constexpr int  get_align() const noexcept{return m_align;}

  operator bool() const noexcept{return m_kind != kinds::null;}

  constexpr bool  is_null()             const noexcept{return m_kind == kinds::null;}
  constexpr bool  is_void()             const noexcept{return m_kind == kinds::void_;}
  constexpr bool  is_undefined()        const noexcept{return m_kind == kinds::undefined;}
  constexpr bool  is_boolean()          const noexcept{return m_kind == kinds::boolean;}
  constexpr bool  is_character()        const noexcept{return m_kind == kinds::character;}
  constexpr bool  is_null_pointer()     const noexcept{return m_kind == kinds::null_pointer;}
  constexpr bool  is_integer()          const noexcept{return m_kind == kinds::integer;}
  constexpr bool  is_unsigned_integer() const noexcept{return m_kind == kinds::unsigned_integer;}
  constexpr bool  is_fpn()              const noexcept{return m_kind == kinds::fpn;}
  constexpr bool  is_pointer()          const noexcept{return m_kind == kinds::pointer;}
  constexpr bool  is_unsigned_pointer() const noexcept{return m_kind == kinds::unsigned_pointer;}
  constexpr bool  is_typed_pointer()    const noexcept{return m_kind == kinds::typed_pointer;}
  constexpr bool  is_reference()        const noexcept{return m_kind == kinds::reference;}
  constexpr bool  is_array()            const noexcept{return m_kind == kinds::array;}
  constexpr bool  is_function_pointer() const noexcept{return m_kind == kinds::function_pointer;}
  constexpr bool  is_struct()           const noexcept{return m_kind == kinds::struct_;}
  constexpr bool  is_enum()             const noexcept{return m_kind == kinds::enum_;}
  constexpr bool  is_union()            const noexcept{return m_kind == kinds::union_;}

};


constexpr
basic_type_info
g_basic_type_info_table[] =
{
  basic_type_info(basic_type_info::kinds::undefined       ,0,"undef"),
  basic_type_info(basic_type_info::kinds::boolean         ,1,"bool8"),
  basic_type_info(basic_type_info::kinds::boolean         ,2,"bool16"),
  basic_type_info(basic_type_info::kinds::boolean         ,4,"bool32"),
  basic_type_info(basic_type_info::kinds::boolean         ,8,"bool64"),
  basic_type_info(basic_type_info::kinds::void_           ,0,"void"),
  basic_type_info(basic_type_info::kinds::null_pointer    ,0,"nullptr"),
  basic_type_info(basic_type_info::kinds::pointer         ,1,"intptr8"),
  basic_type_info(basic_type_info::kinds::pointer         ,2,"intptr16"),
  basic_type_info(basic_type_info::kinds::pointer         ,4,"intptr32"),
  basic_type_info(basic_type_info::kinds::pointer         ,8,"intptr64"),
  basic_type_info(basic_type_info::kinds::unsigned_pointer,1,"uintptr8"),
  basic_type_info(basic_type_info::kinds::unsigned_pointer,2,"uintptr16"),
  basic_type_info(basic_type_info::kinds::unsigned_pointer,4,"uintptr32"),
  basic_type_info(basic_type_info::kinds::unsigned_pointer,8,"uintptr64"),
  basic_type_info(basic_type_info::kinds::character       ,1,"char8"),
  basic_type_info(basic_type_info::kinds::character       ,2,"char16"),
  basic_type_info(basic_type_info::kinds::character       ,4,"char32"),
  basic_type_info(basic_type_info::kinds::integer         ,1,"int8"),
  basic_type_info(basic_type_info::kinds::integer         ,2,"int16"),
  basic_type_info(basic_type_info::kinds::integer         ,4,"int32"),
  basic_type_info(basic_type_info::kinds::integer         ,8,"int64"),
  basic_type_info(basic_type_info::kinds::unsigned_integer,1,"uint8"),
  basic_type_info(basic_type_info::kinds::unsigned_integer,2,"uint16"),
  basic_type_info(basic_type_info::kinds::unsigned_integer,4,"uint32"),
  basic_type_info(basic_type_info::kinds::unsigned_integer,8,"uint64"),
  basic_type_info(basic_type_info::kinds::fpn             ,4,"float32"),
  basic_type_info(basic_type_info::kinds::fpn             ,8,"float64"),
};


class
type_info: public basic_type_info
{
  class core;

  core*  m_core=nullptr;

  void  unrefer() noexcept;

  static constexpr basic_type_info
  seek_basic(const basic_type_info*   it,
                              const basic_type_info*  end,
                              std::string_view  name) noexcept
  {
    return (it < end)? (it->get_name() == name)? *it
                      :seek_basic(it+1,end,name)
          :basic_type_info()
          ;
  }

  static constexpr basic_type_info
  seek_basic(std::string_view  name) noexcept
  {
    return seek_basic(std::begin(g_basic_type_info_table),
                      std::end(  g_basic_type_info_table),
                      name
           );
  }

public:
  type_info(const type_info&   rhs) noexcept{assign(rhs);}
  type_info(      type_info&&  rhs) noexcept{assign(std::move(rhs));}

  constexpr type_info() noexcept{}
  constexpr type_info(std::string_view  name) noexcept: basic_type_info(seek_basic(name)){}
  type_info(alias_type_def&&            ti) noexcept;
  type_info(typed_pointer_type_def&&    ti) noexcept;
  type_info(reference_type_def&&  ti) noexcept;
  type_info(array_type_def&&      ti) noexcept;
  type_info(struct_type_decl&&    ti) noexcept;
  type_info(union_type_decl&&     ti) noexcept;
  type_info(enum_type_decl&&      ti) noexcept;
  type_info(function_pointer_type_def&&  td) noexcept;
 ~type_info(){unrefer();}

  type_info&  operator=(const type_info&   rhs) noexcept{return assign(rhs);}
  type_info&  operator=(      type_info&&  rhs) noexcept{return assign(std::move(rhs));}

  type_info&  assign(const type_info&   rhs) noexcept;
  type_info&  assign(      type_info&&  rhs) noexcept;

  void  update_size() noexcept;
  void  update_align() noexcept;
  void  update() noexcept;

  const function_pointer_type_def&  get_function_pointer_type_def() const noexcept;
  const typed_pointer_type_def&     get_typed_pointer_type_def()   const noexcept;
  const reference_type_def&         get_reference_type_def() const noexcept;
  const alias_type_def&             get_alias_type_def()     const noexcept;
  const array_type_def&             get_array_type_def()     const noexcept;

  struct_type_decl&  get_struct_type_decl() const noexcept;
  union_type_decl&   get_union_type_decl()  const noexcept;
  enum_type_decl&    get_enum_type_decl()   const noexcept;

  type_info  form_array(int  n)          const noexcept;
  type_info  form_typed_pointer(int  sz) const noexcept;
  type_info  form_reference(int  sz)     const noexcept;

  type_info  remove_array()         const noexcept;
  type_info  remove_typed_pointer() const noexcept;
  type_info  remove_reference()     const noexcept;

  void  print() const noexcept;

};




class
alias_type_def
{
  type_info  m_base_type_info;

  unique_string  m_name;

public:
  alias_type_def(const type_info&   base, std::string_view  name) noexcept: m_base_type_info(base), m_name(name){}
  alias_type_def(      type_info&&  base, std::string_view  name) noexcept: m_base_type_info(std::move(base)), m_name(name){}

  const type_info&  get_base_type_info() const noexcept{return m_base_type_info;}

  int  get_size()  const noexcept{return m_base_type_info.get_size();}
  int  get_align() const noexcept{return m_base_type_info.get_align();}

  std::string_view  get_name() const noexcept{return m_name;}

  void  print() const noexcept{m_base_type_info.print();}

};




class
parameter
{
  type_info  m_type_info;

  std::string  m_name;

public:
  parameter(const type_info&  ti, std::string_view  name) noexcept:
  m_type_info(ti), m_name(name){}

  parameter(type_info&&  ti, std::string_view  name) noexcept:
  m_type_info(std::move(ti)), m_name(name){}

  const type_info&  get_type_info() const noexcept{return m_type_info;}

  const std::string&  get_name() const noexcept{return m_name;}

};


using parameter_list = std::vector<parameter>;


class
function_signature
{
  type_info  m_return_type_info;

  parameter_list  m_parameter_list;

public:
  function_signature(const type_info&  retti, parameter_list&&  ls={}) noexcept:
  m_return_type_info(retti), m_parameter_list(std::move(ls)){}

  function_signature(type_info&&  retti, parameter_list&&  ls={}) noexcept:
  m_return_type_info(std::move(retti)), m_parameter_list(std::move(ls)){}

  const type_info&  get_return_type_info() const noexcept{return m_return_type_info;}

  const parameter_list&  get_parameter_list() const noexcept{return m_parameter_list;}

  std::string  make_name() const noexcept;

  void  print() const noexcept;

};


class
function_pointer_type_def
{
  function_signature  m_signature;

  int  m_size=0;

  unique_string  m_name;

  std::string  make_name() const noexcept{return m_signature.make_name();}

public:
  function_pointer_type_def(const function_signature&   sig, int  sz) noexcept: m_signature(sig),            m_size(sz){m_name = make_name();}
  function_pointer_type_def(      function_signature&&  sig, int  sz) noexcept: m_signature(std::move(sig)), m_size(sz){m_name = make_name();}

  const function_signature&  get_signature() const noexcept{return m_signature;}

  int  get_size() const noexcept{return m_size;}

  std::string_view  get_name() const noexcept{return m_name;}

};


class
typed_pointer_type_def
{
protected:
  type_info  m_base_type_info;

  int  m_size=0;

  unique_string  m_name;

  std::string  make_name() const noexcept{return std::string(m_base_type_info.get_name())+"*";}

  typed_pointer_type_def() noexcept{}

public:
  typed_pointer_type_def(const type_info&   base, int  sz) noexcept: m_base_type_info(base),            m_size(sz){m_name = make_name();}
  typed_pointer_type_def(      type_info&&  base, int  sz) noexcept: m_base_type_info(std::move(base)), m_size(sz){m_name = make_name();}

  const type_info&  get_base_type_info() const noexcept{return m_base_type_info;}

  int  get_size() const noexcept{return m_size;}

  std::string_view  get_name() const noexcept{return m_name;}

};


class
reference_type_def: public typed_pointer_type_def
{
  std::string  make_name() const noexcept{return std::string(m_base_type_info.get_name())+"&";}

public:
  reference_type_def(const type_info&   base, int  sz) noexcept: typed_pointer_type_def(base,           sz){m_name = make_name();}
  reference_type_def(      type_info&&  base, int  sz) noexcept: typed_pointer_type_def(std::move(base),sz){m_name = make_name();}

};


class
array_type_def
{
  type_info  m_base_type_info;

  int  m_number_of_elements;

  unique_string  m_name;

  std::string  make_name() const noexcept{
    std::string  s(m_base_type_info.get_name());

    return s+"["+std::to_string(m_number_of_elements)+"]";
  }

public:
  array_type_def(const type_info&  base, int  n) noexcept:
  m_base_type_info(base), m_number_of_elements(n){m_name = make_name();}

  array_type_def(type_info&&  base, int  n) noexcept:
  m_base_type_info(std::move(base)), m_number_of_elements(n){m_name = make_name();}

  const type_info&  get_base_type_info() const noexcept{return m_base_type_info;}

  int  get_number_of_elements() const noexcept{return m_number_of_elements;}

  int   get_size() const noexcept{return m_base_type_info.get_size()*m_number_of_elements;}
  int  get_align() const noexcept{return m_base_type_info.get_align();}

  std::string_view  get_name() const noexcept{return m_name;}

};




class
struct_type_decl
{
  std::unique_ptr<struct_type_def>  m_def;

  unique_string  m_name;

public:
  struct_type_decl(std::string_view  name) noexcept: m_name(name){}

  operator bool() const noexcept{return m_def.get();}

  struct_type_def&  operator*()  const noexcept{return *m_def;}
  struct_type_def*  operator->() const noexcept{return  m_def.get();}

  int   get_size() const noexcept;
  int  get_align() const noexcept;

  std::string_view  get_name() const noexcept{return m_name;}

  struct_type_def&  set_def(struct_type_def&&  def) noexcept;

  struct_type_def&  get_def() const noexcept{return *m_def;}

  void  print() const noexcept;

};


class
struct_member
{
  type_info  m_type_info;

  std::string  m_name;

  int  m_offset;

public:
  struct_member(type_info&&  ti, std::string_view  name, int  offset) noexcept:
  m_type_info(std::move(ti)), m_name(name), m_offset(offset){}

  const type_info&  get_type_info() const noexcept{return m_type_info;}

  const std::string&  get_name() const noexcept{return m_name;}

  int  get_offset() const noexcept{return m_offset;}

};


class
struct_type_def
{
  std::vector<struct_member>  m_member_list;

  int  m_size =0;
  int  m_align=0;

public:
  void  push(const type_info&   ti, std::string_view  name) noexcept{push(type_info(ti),name);}
  void  push(      type_info&&  ti, std::string_view  name) noexcept;

  const struct_member*  find(std::string_view  name) const noexcept;

  int   get_size() const noexcept{return m_size;}
  int  get_align() const noexcept{return m_align;}

  void  print() const noexcept;

};




class
union_type_decl
{
  std::unique_ptr<union_type_def>  m_def;

  unique_string  m_name;

public:
  union_type_decl(std::string_view  name) noexcept: m_name(name){}

  operator bool() const noexcept{return m_def.get();}

  union_type_def&  operator*()  const noexcept{return *m_def;}
  union_type_def*  operator->() const noexcept{return  m_def.get();}

  int   get_size() const noexcept;
  int  get_align() const noexcept;

  std::string_view  get_name() const noexcept{return m_name;}

  union_type_def&  set_def(union_type_def&&  def) noexcept;
  union_type_def&  get_def() const noexcept{return *m_def;}

  void  print() const noexcept;

};


class
union_type_def
{
  struct member{
    type_info  m_type_info;

    std::string  m_name;

    member(type_info&&  ti, std::string_view  name) noexcept: m_type_info(std::move(ti)), m_name(name){}
  };

  std::vector<member>  m_member_list;

  int  m_size =0;
  int  m_align=0;

public:
  void  push(const type_info&   ti, std::string_view  name) noexcept{push(type_info(ti),name);}
  void  push(      type_info&&  ti, std::string_view  name) noexcept;

  type_info  find(std::string_view  name) const noexcept;

  int   get_size() const noexcept{return m_size;}
  int  get_align() const noexcept{return m_align;}

  void  print() const noexcept;

};




class
enumerator
{
  unique_string  m_name;

  int64_t  m_value;

public:
  enumerator(std::string_view  name, int64_t  value) noexcept:
  m_name(name), m_value(value){}

  std::string_view  get_name() const noexcept{return m_name;}

  const int64_t&  get_value() const noexcept{return m_value;}

  void  print() const noexcept{printf("%s = %" PRIi64,m_name.data(),m_value);}

};


class
enum_type_decl
{
  std::unique_ptr<enum_type_def>  m_def;

  unique_string  m_name;

public:
  enum_type_decl(std::string_view  name) noexcept: m_name(name){}

  operator bool() const noexcept{return m_def.get();}

  enum_type_def&  operator*()  const noexcept{return *m_def;}
  enum_type_def*  operator->() const noexcept{return  m_def.get();}

  int  get_size() const noexcept;

  std::string_view  get_name() const noexcept{return m_name;}

  enum_type_def&  set_def(enum_type_def&&  def) noexcept;
  enum_type_def&  get_def() const noexcept{return *m_def;}

  void  print() const noexcept;

};


class
enum_type_def
{
  std::vector<enumerator>  m_enumerator_list;

  int  m_size=0;

public:
  enum_type_def(int  sz) noexcept: m_size(sz){}

  int  get_size() const noexcept{return m_size;}

  enumerator&  push(std::string_view  name, int  value) noexcept;

  const int64_t*  find(std::string_view  name) const noexcept;

  void  print() const noexcept;

};




}}


#endif




