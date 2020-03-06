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




class type_info;


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
pointer_type_info
{
  const type_info&  m_base_type_info;

  int  m_size;

public:
  constexpr pointer_type_info(const type_info&  base, int  sz) noexcept: m_base_type_info(base), m_size(sz){}

  constexpr const type_info&  get_base_type_info() const noexcept{return m_base_type_info;}

  constexpr int  get_size() const noexcept{return m_size;}

  std::string  make_id() const noexcept;

};


class
reference_type_info
{
  const type_info&  m_base_type_info;

  int  m_size;

public:
  constexpr reference_type_info(const type_info&  base, int  sz) noexcept: m_base_type_info(base), m_size(sz){}

  constexpr const type_info&  get_base_type_info() const noexcept{return m_base_type_info;}

  constexpr int  get_size() const noexcept{return m_size;}

  std::string  make_id() const noexcept;

};


class
array_type_info
{
  const type_info&  m_base_type_info;

  int  m_number_of_elements;

public:
  constexpr array_type_info(const type_info&  base, int  n) noexcept:
  m_base_type_info(base), m_number_of_elements(n){}

  const type_info&  get_base_type_info() const noexcept{return m_base_type_info;}

  constexpr int  get_number_of_elements() const noexcept{return m_number_of_elements;}

  int   get_size() const noexcept;
  int  get_align() const noexcept;

  std::string  make_id() const noexcept;

};




class
parameter_list
{
  std::vector<const type_info*>  m_container;

public:
  parameter_list(std::vector<const type_info*>&&  ls={}) noexcept: m_container(std::move(ls)){}

  parameter_list&  push(const type_info&  ti) noexcept{  m_container.emplace_back(&ti);  return *this;}

  const type_info**  begin() noexcept{return m_container.data();}
  const type_info**    end() noexcept{return m_container.data()+m_container.size();}

  const type_info* const*  begin() const noexcept{return m_container.data();}
  const type_info* const*    end() const noexcept{return m_container.data()+m_container.size();}

  int  get_number_of_elements() const noexcept{return m_container.size();}

  std::string  make_id() const noexcept;

  void  print() const noexcept;

};


class
function_signature
{
  const type_info&  m_return_type_info;

  parameter_list  m_parameter_list;

public:
  function_signature(const type_info&  ret_ti, parameter_list&&  parals) noexcept:
  m_return_type_info(ret_ti), m_parameter_list(std::move(parals)){}

  const type_info&  get_return_type_info() const noexcept{return m_return_type_info;}

  const parameter_list&  get_parameter_list() const noexcept{return m_parameter_list;}

  std::string  make_id() const noexcept;

  void  print() const noexcept;

};




class
struct_member
{
  const type_info&  m_type_info;

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
    const type_info&  m_type_info;

    std::string  m_name;

    member(const type_info&  ti, std::string_view  name) noexcept: m_type_info(ti), m_name(name){}
  };

  std::vector<member>  m_member_list;

  int  m_size =0;
  int  m_align=0;

  static uint32_t  m_id_source;

  uint32_t  m_id_number;

public:
  union_type_info() noexcept: m_id_number(m_id_source++){}

  void  push(const type_info&  ti, std::string_view  name) noexcept;

  const type_info*  find(std::string_view  name) const noexcept;

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




class type_collection;
class type_info;


class
type_info
{
  enum class kind{
    null,
    undefined,
    void_,
    boolean,
    null_pointer,
    generic_pointer,
    array,
    pointer,
    reference,
    integer,
    unsigned_integer,
    fpn,
    function,
    struct_,
    union_,
    enum_,
  } m_kind=kind::null;

  union data{
             integer_type_info   int_ti;
    unsigned_integer_type_info  uint_ti;
                 fpn_type_info   fpn_ti;

    boolean_type_info           bool_ti;
    generic_pointer_type_info   gptr_ti;
    pointer_type_info            ptr_ti;
    reference_type_info          ref_ti;

    array_type_info    arr_ti;
    struct_type_info   str_ti;
    union_type_info    uni_ti;
    enum_type_info     enu_ti;

    function_signature  fnsig;

    data() noexcept{}
   ~data(){}

  } m_data;


  std::string  m_id;

  struct derivation{
    std::list<type_info>  m_array_type_info_list;
    std::list<type_info>  m_function_type_info_list;

    std::unique_ptr<type_info>    m_pointer_type_info;
    std::unique_ptr<type_info>  m_reference_type_info;
  };

  class holder{
    derivation*  m_pointer;
  public:
    holder() noexcept: m_pointer(new derivation){}
   ~holder(){delete m_pointer;}

    derivation*  operator->() const noexcept{return m_pointer;}
  } m_holder;

public:
  type_info()noexcept: m_kind(kind::null), m_id("null"){}
  type_info(const type_info&   rhs) noexcept=delete;
  type_info(const type_info&&  rhs) noexcept=delete;
  type_info(void_type_info)                 noexcept: m_kind(kind::void_           ), m_id("void")          {}
  type_info(undefined_type_info)            noexcept: m_kind(kind::undefined       ), m_id("undefined")     {}
  type_info(null_pointer_type_info)         noexcept: m_kind(kind::null_pointer    ), m_id("nullptr")       {}
  type_info(boolean_type_info           ti) noexcept: m_kind(kind::boolean         ), m_id("bool")          {new(&m_data) boolean_type_info(ti);}
  type_info(generic_pointer_type_info   ti) noexcept: m_kind(kind::generic_pointer ), m_id("geneptr")       {new(&m_data) generic_pointer_type_info(ti);}
  type_info(pointer_type_info           ti) noexcept: m_kind(kind::pointer         ), m_id(ti.make_id())    {new(&m_data) pointer_type_info(ti);}
  type_info(reference_type_info         ti) noexcept: m_kind(kind::reference       ), m_id(ti.make_id())    {new(&m_data) reference_type_info(ti);}
  type_info(         integer_type_info  ti) noexcept: m_kind(kind::integer         ), m_id(ti.make_id())    {new(&m_data) integer_type_info(ti);}
  type_info(unsigned_integer_type_info  ti) noexcept: m_kind(kind::unsigned_integer), m_id(ti.make_id())    {new(&m_data) unsigned_integer_type_info(ti);}
  type_info(fpn_type_info               ti) noexcept: m_kind(kind::fpn             ), m_id(ti.make_id())    {new(&m_data) fpn_type_info(ti);}
  type_info(struct_type_info&&          ti) noexcept: m_kind(kind::struct_         ), m_id(ti.make_id())    {new(&m_data) struct_type_info(std::move(ti));}
  type_info(union_type_info&&           ti) noexcept: m_kind(kind::union_          ), m_id(ti.make_id())    {new(&m_data) union_type_info(std::move(ti));}
  type_info(enum_type_info&&            ti) noexcept: m_kind(kind::enum_           ), m_id(ti.make_id())    {new(&m_data) enum_type_info(std::move(ti));}
  type_info(array_type_info&&           ti) noexcept: m_kind(kind::array           ), m_id(ti.make_id())    {new(&m_data) array_type_info(std::move(ti));}
  type_info(function_signature&&     fnsig) noexcept: m_kind(kind::function        ), m_id(fnsig.make_id()) {new(&m_data) function_signature(std::move(fnsig));}
 ~type_info(){clear();}

  bool  operator==(const type_info&  rhs) const noexcept{return m_id == rhs.m_id;}
  bool  operator!=(const type_info&  rhs) const noexcept{return m_id != rhs.m_id;}

  type_info&  operator=(const type_info&   rhs) noexcept=delete;
  type_info&  operator=(const type_info&&  rhs) noexcept=delete;

  type_info&  clear() noexcept;

  const std::string&  get_id() const noexcept{return m_id;}

  int  get_size() const noexcept;
  int  get_align() const noexcept;

  const          integer_type_info&           get_integer_type_info() const noexcept{return m_data.int_ti;}
  const unsigned_integer_type_info&  get_unsigned_integer_type_info() const noexcept{return m_data.uint_ti;}

  const fpn_type_info&  get_fpn_type_info() const noexcept{return m_data.fpn_ti;}

  const   pointer_type_info&    get_pointer_type_info() const noexcept{return m_data.ptr_ti;}
  const reference_type_info&  get_reference_type_info() const noexcept{return m_data.ref_ti;}

  const array_type_info&    get_array_type_info() const noexcept{return m_data.arr_ti;}
  const struct_type_info&  get_struct_type_info() const noexcept{return m_data.str_ti;}
  const union_type_info&    get_union_type_info() const noexcept{return m_data.uni_ti;}
  const enum_type_info&      get_enum_type_info() const noexcept{return m_data.enu_ti;}

  const function_signature&  get_function_signature() const noexcept{return m_data.fnsig;}

  bool  is_null()             const noexcept{return m_kind == kind::null;}
  bool  is_void()             const noexcept{return m_kind == kind::void_;}
  bool  is_undefined()        const noexcept{return m_kind == kind::undefined;}
  bool  is_boolean()          const noexcept{return m_kind == kind::boolean;}
  bool  is_null_pointer()     const noexcept{return m_kind == kind::null_pointer;}
  bool  is_generic_pointer()  const noexcept{return m_kind == kind::generic_pointer;}
  bool  is_integer()          const noexcept{return m_kind == kind::integer;}
  bool  is_unsigned_integer() const noexcept{return m_kind == kind::unsigned_integer;}
  bool  is_fpn()              const noexcept{return m_kind == kind::fpn;}
  bool  is_pointer()          const noexcept{return m_kind == kind::pointer;}
  bool  is_reference()        const noexcept{return m_kind == kind::reference;}
  bool  is_array()            const noexcept{return m_kind == kind::array;}
  bool  is_function()         const noexcept{return m_kind == kind::function;}
  bool  is_struct()           const noexcept{return m_kind == kind::struct_;}
  bool  is_enum()             const noexcept{return m_kind == kind::enum_;}
  bool  is_union()            const noexcept{return m_kind == kind::union_;}

  bool  is_kind_of_integer() const noexcept{return is_integer() || is_unsigned_integer();}
  bool  is_kind_of_pointer() const noexcept{return is_pointer() || is_null_pointer() || is_generic_pointer();}
  bool  is_like_boolean()    const noexcept{return is_boolean() || is_kind_of_integer() || is_kind_of_pointer();}

  bool  is_word()   const noexcept{return is_kind_of_integer() || is_kind_of_pointer() || is_fpn() || is_boolean();}
  bool  is_memory() const noexcept{return is_array() || is_struct() || is_union();}

  const type_info&   form_function_type(parameter_list&&  parals) const noexcept;
  const type_info&      form_array_type(int  n) const noexcept;
  const type_info&  form_reference_type(int  w) const noexcept;
  const type_info&    form_pointer_type(int  w) const noexcept;

  const type_info&  strip_pointer_type() const noexcept;
  const type_info&  strip_reference_type() const noexcept;
  const type_info&  strip_array_type() const noexcept;

  void  print() const noexcept;
  void  print_detail() const noexcept;

};


inline std::string    pointer_type_info::make_id() const noexcept{return m_base_type_info.get_id()+"p";}
inline std::string  reference_type_info::make_id() const noexcept{return m_base_type_info.get_id()+"r";}


class
type_entry
{
  std::string  m_name;

  const type_info&  m_type_info;

public:
  type_entry(std::string_view  name, const type_info&  ti) noexcept;

  const type_info&  operator*()  const noexcept{return m_type_info;}
  const type_info*  operator->() const noexcept{return &m_type_info;}

  const std::string&  get_name() const noexcept{return m_name;}
  const type_info&    get_info() const noexcept{return m_type_info;}

  void  print() const noexcept;

};




class
type_collection
{
  using alias = std::pair<std::string,std::string>;

  std::vector<type_entry>  m_entry_table;
  std::vector<alias>       m_alias_table;

public:
  type_collection() noexcept{}

  const type_entry&  push(std::string_view  name, const type_info&  ti) noexcept;

  const type_info*  find_by_id(  std::string_view    id) const noexcept;
  const type_info*  find_by_name(std::string_view  name) const noexcept;

  const type_entry*  find_entry(const type_info&  ti) const noexcept;

  bool  make_alias(std::string_view  target_name, std::string_view  new_name) noexcept;

  void  print() const noexcept;

};




}}


#endif




