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




namespace gbstd{
namespace typesystem{




class           integer_type_info;
class  unsigned_integer_type_info;
class               fpn_type_info;

class          boolean_type_info;
class  generic_pointer_type_info;
class          pointer_type_info;
class        reference_type_info;

class   array_type_info;
class  struct_type_info;
class   union_type_info;
class    enum_type_info;

class  function_pointer_type_info;

class      void_type_info{};
class undefined_type_info{};


class
integer_type_info
{
  int  m_size;

public:
  constexpr integer_type_info(int  sz) noexcept: m_size(sz){}

  constexpr int  get_size() const noexcept{return m_size;}

  std::string  make_id() const noexcept{return std::string("s")+std::to_string(8*m_size);}

};


class
unsigned_integer_type_info
{
  int  m_size;

public:
  constexpr unsigned_integer_type_info(int  sz) noexcept: m_size(sz){}

  constexpr int  get_size() const noexcept{return m_size;}

  std::string  make_id() const noexcept{return std::string("u")+std::to_string(8*m_size);}

};


class
fpn_type_info
{
  int  m_size;

public:
  constexpr fpn_type_info(int  sz) noexcept: m_size(sz){}

  constexpr int  get_size() const noexcept{return m_size;}

  std::string  make_id() const noexcept{return std::string("f")+std::to_string(8*m_size);}

};


class
boolean_type_info
{
  int  m_size;

public:
  constexpr boolean_type_info(int  sz) noexcept: m_size(sz){}

  constexpr int  get_size() const noexcept{return m_size;}

};


class
null_pointer_type_info
{
public:
  constexpr null_pointer_type_info() noexcept{}

};


class
generic_pointer_type_info
{
  int  m_size;

public:
  constexpr generic_pointer_type_info(int  sz) noexcept: m_size(sz){}

  constexpr int  get_size() const noexcept{return m_size;}

};




class
type_info
{
  enum class kind{
    null,
    undefined,
    void_,
    boolean,
    integer,
    unsigned_integer,
    fpn,
    null_pointer,
    generic_pointer,
    function_pointer,
    pointer,
    reference,
    array,
    struct_,
    union_,
    enum_,
  };


  class core;

  core*  m_core=nullptr;

  void  unrefer() noexcept;

  kind  get_kind() const noexcept;

public:
  type_info() noexcept{}
  type_info(const type_info&   rhs) noexcept{assign(rhs);}
  type_info(      type_info&&  rhs) noexcept{assign(std::move(rhs));}
  type_info(void_type_info)                   noexcept;
  type_info(undefined_type_info)              noexcept;
  type_info(null_pointer_type_info)           noexcept;
  type_info(boolean_type_info&&           ti) noexcept;
  type_info(generic_pointer_type_info&&   ti) noexcept;
  type_info(         integer_type_info&&  ti) noexcept;
  type_info(unsigned_integer_type_info&&  ti) noexcept;
  type_info(fpn_type_info&&               ti) noexcept;
  type_info(pointer_type_info&&           ti) noexcept;
  type_info(reference_type_info&&         ti) noexcept;
  type_info(struct_type_info&&            ti) noexcept;
  type_info(union_type_info&&             ti) noexcept;
  type_info(enum_type_info&&              ti) noexcept;
  type_info(array_type_info&&             ti) noexcept;
  type_info(function_pointer_type_info&&  ti) noexcept;
 ~type_info(){unrefer();}

  bool  operator==(const type_info&  rhs) const noexcept{return get_id() == rhs.get_id();}
  bool  operator!=(const type_info&  rhs) const noexcept{return get_id() != rhs.get_id();}

  operator bool() const noexcept{return get_kind() != kind::null;}

  type_info&  operator=(const type_info&   rhs) noexcept{return assign(rhs);}
  type_info&  operator=(      type_info&&  rhs) noexcept{return assign(std::move(rhs));}

  type_info&  assign(const type_info&   rhs) noexcept;
  type_info&  assign(      type_info&&  rhs) noexcept;

  void  set_name(std::string_view  name) noexcept;

  const std::string&  get_name() const noexcept;
  const std::string&    get_id() const noexcept;

  int  get_size() const noexcept;
  int  get_align() const noexcept;

  const          integer_type_info&           get_integer_type_info() const noexcept;
  const unsigned_integer_type_info&  get_unsigned_integer_type_info() const noexcept;

  const fpn_type_info&  get_fpn_type_info() const noexcept;

  const   pointer_type_info&    get_pointer_type_info() const noexcept;
  const reference_type_info&  get_reference_type_info() const noexcept;

  const array_type_info&    get_array_type_info() const noexcept;
  const struct_type_info&  get_struct_type_info() const noexcept;
  const union_type_info&    get_union_type_info() const noexcept;
  const enum_type_info&      get_enum_type_info() const noexcept;

  const function_pointer_type_info&  get_function_pointer_type_info() const noexcept;

  bool  is_null()             const noexcept{return get_kind() == kind::null;}
  bool  is_void()             const noexcept{return get_kind() == kind::void_;}
  bool  is_undefined()        const noexcept{return get_kind() == kind::undefined;}
  bool  is_boolean()          const noexcept{return get_kind() == kind::boolean;}
  bool  is_null_pointer()     const noexcept{return get_kind() == kind::null_pointer;}
  bool  is_generic_pointer()  const noexcept{return get_kind() == kind::generic_pointer;}
  bool  is_integer()          const noexcept{return get_kind() == kind::integer;}
  bool  is_unsigned_integer() const noexcept{return get_kind() == kind::unsigned_integer;}
  bool  is_fpn()              const noexcept{return get_kind() == kind::fpn;}
  bool  is_pointer()          const noexcept{return get_kind() == kind::pointer;}
  bool  is_reference()        const noexcept{return get_kind() == kind::reference;}
  bool  is_array()            const noexcept{return get_kind() == kind::array;}
  bool  is_function_pointer() const noexcept{return get_kind() == kind::function_pointer;}
  bool  is_struct()           const noexcept{return get_kind() == kind::struct_;}
  bool  is_enum()             const noexcept{return get_kind() == kind::enum_;}
  bool  is_union()            const noexcept{return get_kind() == kind::union_;}

  bool  is_kind_of_integer() const noexcept{return is_integer() || is_unsigned_integer();}
  bool  is_kind_of_pointer() const noexcept{return is_pointer() || is_function_pointer() || is_null_pointer() || is_generic_pointer();}
  bool  is_like_boolean()    const noexcept{return is_boolean() || is_kind_of_integer() || is_kind_of_pointer();}

  bool  is_word()   const noexcept{return is_kind_of_integer() || is_kind_of_pointer() || is_fpn() || is_boolean();}
  bool  is_memory() const noexcept{return is_array() || is_struct() || is_union();}

  type_info      form_array_type(int  n) const noexcept;
  type_info  form_reference_type(int  w) const noexcept;
  type_info    form_pointer_type(int  w) const noexcept;

  type_info    strip_pointer_type() const noexcept;
  type_info  strip_reference_type() const noexcept;
  type_info      strip_array_type() const noexcept;

  void  print() const noexcept;
  void  print_detail() const noexcept;

};




class
function_signature
{
  std::vector<type_info>  m_container;

public:
  function_signature(std::vector<type_info>&&  ls={}) noexcept: m_container(std::move(ls)){}

  function_signature&  push(const type_info&  ti) noexcept{  m_container.emplace_back(ti);  return *this;}

  const type_info*  begin() noexcept{return m_container.data();}
  const type_info*    end() noexcept{return m_container.data()+m_container.size();}

  const type_info*  begin() const noexcept{return m_container.data();}
  const type_info*    end() const noexcept{return m_container.data()+m_container.size();}

  int  get_number_of_elements() const noexcept{return m_container.size();}

  std::string  make_id() const noexcept;

  void  print() const noexcept;

};




class
function_pointer_type_info
{
  function_signature  m_signature;

  int  m_size;

public:
  function_pointer_type_info(function_signature&&  sig, int  sz) noexcept:
  m_signature(std::move(sig)), m_size(sz){}

  const function_signature&  get_signature() const noexcept{return m_signature;}

  int  get_size() const noexcept{return m_size;}

  std::string  make_id() const noexcept{return m_signature.make_id()+"p";}

  void  print() const noexcept{m_signature.print();}

};


class
pointer_type_info
{
  type_info  m_base_type_info;

  int  m_size;

public:
  pointer_type_info(const type_info&  base, int  sz) noexcept: m_base_type_info(base), m_size(sz){}

  const type_info&  get_base_type_info() const noexcept{return m_base_type_info;}

  int  get_size() const noexcept{return m_size;}

  std::string  make_id() const noexcept;

};


class
reference_type_info
{
  type_info  m_base_type_info;

  int  m_size;

public:
  reference_type_info(const type_info&  base, int  sz) noexcept: m_base_type_info(std::move(base)), m_size(sz){}

  const type_info&  get_base_type_info() const noexcept{return m_base_type_info;}

  int  get_size() const noexcept{return m_size;}

  std::string  make_id() const noexcept;

};


class
array_type_info
{
  type_info  m_base_type_info;

  int  m_number_of_elements;

public:
  array_type_info(const type_info&  base, int  n) noexcept:
  m_base_type_info(base), m_number_of_elements(n){}

  const type_info&  get_base_type_info() const noexcept{return m_base_type_info;}

  int  get_number_of_elements() const noexcept{return m_number_of_elements;}

  int   get_size() const noexcept;
  int  get_align() const noexcept;

  std::string  make_id() const noexcept;

};


class
struct_member
{
  type_info  m_type_info;

  std::string  m_name;

  int  m_offset;

public:
  struct_member(const type_info&  ti, std::string_view  name, int  offset) noexcept:
  m_type_info(ti), m_name(name), m_offset(offset){}

  const type_info&  get_type_info() const noexcept{return m_type_info;}

  const std::string&  get_name() const noexcept{return m_name;}

  int  get_offset() const noexcept{return m_offset;}

};


class
struct_type_info
{
  std::vector<struct_member>  m_member_list;

  int  m_size =0;
  int  m_align=0;

  static uint32_t  m_id_source;

  uint32_t  m_id_number;

public:
  struct_type_info() noexcept: m_id_number(m_id_source++){}

  void  push(const type_info&  ti, std::string_view  name) noexcept;

  const struct_member*  find(std::string_view  name) const noexcept;

  int   get_size() const noexcept{return m_size;}
  int  get_align() const noexcept{return m_align;}

  std::string  make_id() const noexcept{return std::string("st")+std::to_string(m_id_number);}

  void  print() const noexcept;

};


class
union_type_info
{
  struct member{
    type_info  m_type_info;

    std::string  m_name;

    member(const type_info&  ti, std::string_view  name) noexcept: m_type_info(std::move(ti)), m_name(name){}
  };

  std::vector<member>  m_member_list;

  int  m_size =0;
  int  m_align=0;

  static uint32_t  m_id_source;

  uint32_t  m_id_number;

public:
  union_type_info() noexcept: m_id_number(m_id_source++){}

  void  push(const type_info&  ti, std::string_view  name) noexcept;

  type_info  find(std::string_view  name) const noexcept;

  int   get_size() const noexcept{return m_size;}
  int  get_align() const noexcept{return m_align;}

  std::string  make_id() const noexcept{return std::string("un")+std::to_string(m_id_number);}

  void  print() const noexcept;

};


class
enumerator
{
  std::string  m_name;

  int  m_value;

public:
  enumerator(std::string_view  name, int  value) noexcept:
  m_name(name), m_value(value){}

  const std::string&  get_name() const noexcept{return m_name;}

  const int&  get_value() const noexcept{return m_value;}

  void  print() const noexcept{printf("%s = %d",m_name.data(),m_value);}

};


class
enum_type_info
{
  int  m_size;

  std::vector<enumerator>  m_enumerator_list;

  static uint32_t  m_id_source;

  uint32_t  m_id_number;

public:
  enum_type_info(int  sz) noexcept: m_size(sz), m_id_number(m_id_source++){}

  int  get_size() const noexcept{return m_size;}

  void  push(std::string_view  name, int  value) noexcept;

  const int*  find(std::string_view  name) const noexcept;

  std::string  make_id() const noexcept{return std::string("en")+std::to_string(m_id_number);}

  void  print() const noexcept;

};




inline std::string    pointer_type_info::make_id() const noexcept{return m_base_type_info.get_id()+"p";}
inline std::string  reference_type_info::make_id() const noexcept{return m_base_type_info.get_id()+"r";}




}}


#endif




