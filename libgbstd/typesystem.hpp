#ifndef gbstd_typesystem_HPP
#define gbstd_typesystem_HPP


#include<cstddef>
#include<cstdint>
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


class         pointer_type_info{};
class    null_pointer_type_info{};
class generic_pointer_type_info{};
class       reference_type_info{};
class         boolean_type_info{};
class            void_type_info{};


class
integer_type_info
{
  int  m_bitwidth=0;

public:
  integer_type_info(int  w) noexcept: m_bitwidth(w){}

  int  get_bitwidth() const noexcept{return m_bitwidth;}

};


class
unsigned_integer_type_info
{
  int  m_bitwidth=0;

public:
  unsigned_integer_type_info(int  w) noexcept: m_bitwidth(w){}

  int  get_bitwidth() const noexcept{return m_bitwidth;}

};




class
parameter_list
{
  std::vector<type_info*>  m_container;

  std::string  m_id;

public:
  parameter_list(std::initializer_list<type_info*>  ls={}) noexcept;

  parameter_list&  push(type_info&  ti) noexcept;

  const std::string&  get_id() const noexcept{return m_id;}

  type_info**  begin() noexcept{return m_container.data();}
  type_info**    end() noexcept{return m_container.data()+m_container.size();}

  type_info* const*  begin() const noexcept{return m_container.data();}
  type_info* const*    end() const noexcept{return m_container.data()+m_container.size();}

  int  get_number_of_elements() const noexcept{return m_container.size();}

};




class
struct_type_info
{
  struct member{
    type_info&  m_type_info;

    std::string  m_name;

    int  m_offset;

    member(type_info&  ti, std::string_view  name, int  offset) noexcept:
    m_type_info(ti), m_name(name), m_offset(offset){}
  };


  std::vector<member>  m_member_list;

  int  m_size =0;
  int  m_align=0;

  static uint32_t  m_id_source;

  std::string  m_id=std::string("st")+std::to_string(m_id_source++);

public:
  struct_type_info() noexcept{}

  void  push(type_info&  ti, std::string_view  name) noexcept;

  const type_info*  find(std::string_view  name) const noexcept;

  int   get_size() const noexcept{return m_size;}
  int  get_align() const noexcept{return m_align;}

  const std::string&  get_id() const noexcept{return m_id;}

  void  print() const noexcept;

};


class
union_type_info
{
  struct member{
    type_info&  m_type_info;

    std::string  m_name;

    member(type_info&  ti, std::string_view  name) noexcept: m_type_info(ti), m_name(name){}
  };

  std::vector<member>  m_member_list;

  int  m_size =0;
  int  m_align=0;

  static uint32_t  m_id_source;

  std::string  m_id=std::string("un")+std::to_string(m_id_source++);

public:
  union_type_info() noexcept{}

  void  push(type_info&  ti, std::string_view  name) noexcept;

  const type_info*  find(std::string_view  name) const noexcept;

  int   get_size() const noexcept{return m_size;}
  int  get_align() const noexcept{return m_align;}

  const std::string&  get_id() const noexcept{return m_id;}

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
  std::vector<enumerator>  m_enumerator_list;

  int  m_last_value=0;

  static uint32_t  m_id_source;

  std::string  m_id=std::string("en")+std::to_string(m_id_source++);

public:
  enum_type_info() noexcept{}

  void  push(std::string_view  name            ) noexcept;
  void  push(std::string_view  name, int  value) noexcept;

  const int*  find(std::string_view  name) const noexcept;

  int   get_size() const noexcept{return 4;}
  int  get_align() const noexcept{return 4;}

  std::string  get_id() const noexcept;

  void  print() const noexcept;

};




class type_collection;


class
type_info
{
  const type_collection&  m_collection;

  type_info*  m_base_type_info=nullptr;

  enum class kind{
    null,
    void_,
    boolean,
    null_pointer,
    generic_pointer,
    array,
    pointer,
    reference,
    integer,
    unsigned_integer,
    function,
    struct_,
    union_,
    enum_,
  } m_kind=kind::null;

  union data{
             integer_type_info   int_ti;
    unsigned_integer_type_info  uint_ti;

    struct_type_info   str_ti;
    union_type_info    uni_ti;
    enum_type_info     enu_ti;

    parameter_list  parals;

    data() noexcept{}
   ~data(){}

  } m_data;

  int  m_number_of_elements=0;

  std::string  m_id;

  std::vector<std::unique_ptr<type_info>>  m_array_type_info_list;
  std::vector<std::unique_ptr<type_info>>  m_function_type_info_list;

  std::unique_ptr<type_info>    m_pointer_type_info;
  std::unique_ptr<type_info>  m_reference_type_info;

  static std::string  make_id(         integer_type_info  ti) noexcept;
  static std::string  make_id(unsigned_integer_type_info  ti) noexcept;
  static std::string  make_id(const type_info&  base, pointer_type_info    ti) noexcept;
  static std::string  make_id(const type_info&  base, reference_type_info    ti) noexcept;
  static std::string  make_id(const type_info&  base, int  n) noexcept;
  static std::string  make_id(const type_info&  ret, const parameter_list&  parals) noexcept;

public:
  type_info(type_collection&  tc)                          noexcept: m_collection(tc), m_kind(kind::null), m_id("null"){}
  type_info(type_collection&  tc, void_type_info)            noexcept: m_collection(tc), m_kind(kind::void_), m_id("void"){}
  type_info(type_collection&  tc, boolean_type_info)         noexcept: m_collection(tc), m_kind(kind::boolean), m_id("bool"){}
  type_info(type_collection&  tc, null_pointer_type_info)    noexcept: m_collection(tc), m_kind(kind::null_pointer), m_id("nullptr"){}
  type_info(type_collection&  tc, generic_pointer_type_info) noexcept: m_collection(tc), m_kind(kind::generic_pointer), m_id("geneptr"){}
  type_info(type_collection&  tc,          integer_type_info  ti)  noexcept: m_collection(tc), m_kind(kind::integer), m_id(make_id(ti)){new(&m_data) integer_type_info(ti);}
  type_info(type_collection&  tc, unsigned_integer_type_info  ti)  noexcept: m_collection(tc), m_kind(kind::unsigned_integer), m_id(make_id(ti)){new(&m_data) unsigned_integer_type_info(ti);}
  type_info(type_collection&  tc, struct_type_info&&  ti) noexcept: m_collection(tc), m_kind(kind::struct_), m_id(ti.get_id()){new(&m_data) struct_type_info(std::move(ti));}
  type_info(type_collection&  tc, union_type_info&&  ti)  noexcept: m_collection(tc), m_kind(kind::union_), m_id(ti.get_id()){new(&m_data) union_type_info(std::move(ti));}
  type_info(type_collection&  tc, enum_type_info&&  ti)   noexcept: m_collection(tc), m_kind(kind::enum_), m_id(ti.get_id()){new(&m_data) enum_type_info(std::move(ti));}
  type_info(type_info&  base, pointer_type_info    ti) noexcept: m_collection(base.m_collection), m_base_type_info(&base), m_kind(kind::pointer  ), m_id(make_id(base,ti)){}
  type_info(type_info&  base, reference_type_info  ti) noexcept: m_collection(base.m_collection), m_base_type_info(&base), m_kind(kind::reference), m_id(make_id(base,ti)){}
  type_info(type_info&  base, int  n) noexcept: m_collection(base.m_collection), m_base_type_info(&base), m_kind(kind::array), m_number_of_elements(n), m_id(make_id(base,n)){}
  type_info(type_info&  ret, parameter_list&&  parals) noexcept: m_collection(ret.m_collection), m_base_type_info(&ret), m_kind(kind::function){  new(&m_data) parameter_list(std::move(parals));  m_id = make_id(ret,m_data.parals);}
  type_info(const type_info&   rhs) noexcept=delete;
  type_info(const type_info&&  rhs) noexcept=delete;
 ~type_info(){clear();}

  bool  operator==(type_info const&  rhs) const noexcept{return m_id == rhs.m_id;}

  type_info&  operator=(const type_info&   rhs) noexcept=delete;
  type_info&  operator=(const type_info&&  rhs) noexcept=delete;

  type_info&  assign(const type_info&   rhs) noexcept;
  type_info&  assign(const type_info&&  rhs) noexcept;

  type_info&  clear() noexcept;

  const type_collection&  get_collection() const noexcept{return m_collection;}

  type_info*  get_base_type_info() const noexcept{return m_base_type_info;}

  const std::string&  get_id() const noexcept{return m_id;}

  int  get_number_of_elements() const noexcept{return m_number_of_elements;}

  int  get_size() const noexcept;
  int  get_align() const noexcept;

           integer_type_info const&           get_integer_type() const noexcept{return m_data.int_ti;}
  unsigned_integer_type_info const&  get_unsigned_integer_type() const noexcept{return m_data.uint_ti;}

  struct_type_info&        get_struct_type() noexcept{return m_data.str_ti;}
  struct_type_info const&  get_struct_type() const noexcept{return m_data.str_ti;}

  union_type_info&        get_union_type() noexcept{return m_data.uni_ti;}
  union_type_info const&  get_union_type() const noexcept{return m_data.uni_ti;}

  enum_type_info&        get_enum_type() noexcept{return m_data.enu_ti;}
  enum_type_info const&  get_enum_type() const noexcept{return m_data.enu_ti;}

  parameter_list&         get_parameter_list()       noexcept{return m_data.parals;}
  parameter_list const&   get_parameter_list() const noexcept{return m_data.parals;}

  bool  is_null()             const noexcept{return m_kind == kind::null;}
  bool  is_void()             const noexcept{return m_kind == kind::void_;}
  bool  is_boolean()          const noexcept{return m_kind == kind::boolean;}
  bool  is_null_pointer()     const noexcept{return m_kind == kind::null_pointer;}
  bool  is_generic_pointer()  const noexcept{return m_kind == kind::generic_pointer;}
  bool  is_integer()          const noexcept{return m_kind == kind::integer;}
  bool  is_unsigned_integer() const noexcept{return m_kind == kind::unsigned_integer;}
  bool  is_pointer()          const noexcept{return m_kind == kind::pointer;}
  bool  is_reference()        const noexcept{return m_kind == kind::reference;}
  bool  is_array()            const noexcept{return m_kind == kind::array;}
  bool  is_function()         const noexcept{return m_kind == kind::function;}
  bool  is_struct()           const noexcept{return m_kind == kind::struct_;}
  bool  is_enum()             const noexcept{return m_kind == kind::enum_;}
  bool  is_union()            const noexcept{return m_kind == kind::union_;}

  type_info&      make_array_type(int  n) noexcept;
  type_info&  make_reference_type() noexcept;
  type_info&    make_pointer_type() noexcept;
  type_info&   make_function_type(parameter_list&&  parals) noexcept;

  void  print() const noexcept;

};


class
type_entry
{
  std::string  m_name;

  type_info*  m_type_info=nullptr;

public:
  type_entry(std::string_view  name, type_info*  ti=nullptr) noexcept;

  const std::string&  get_name() const noexcept{return m_name;}

  type_entry&  set_info(type_info&  ti) noexcept{  m_type_info = &ti;  return *this;}

  type_info*  get_info() const noexcept{return m_type_info;}

  void  print() const noexcept;

};


class
type_collection
{
  std::vector<std::unique_ptr<type_entry>>  m_entry_table;

  int  m_pointer_size=4;
  int  m_boolean_size=1;
  int  m_enum_size=4;

public:
  type_collection() noexcept{}

  int  get_pointer_size() const noexcept{return m_pointer_size;}
  int  get_boolean_size() const noexcept{return m_boolean_size;}
  int  get_enum_size()    const noexcept{return m_enum_size;}

  type_collection&  set_pointer_size(int  sz) noexcept{  m_pointer_size = sz;  return *this;}
  type_collection&  set_boolean_size(int  sz) noexcept{  m_boolean_size = sz;  return *this;}
  type_collection&  set_enum_size(int  sz)    noexcept{  m_enum_size    = sz;  return *this;}

  void  push_c_like_types() noexcept;

  void  push(std::string_view  name, type_info*  ti=nullptr) noexcept;

  type_info&  operator[](std::string_view  name) noexcept;
  type_info*  find(std::string_view  name) const noexcept;

  bool  make_alias(std::string_view  target_name, std::string_view  new_name) noexcept;

  type_info&  create_struct(std::string_view  name) noexcept;
  type_info&  create_union( std::string_view  name) noexcept;
  type_info&  create_enum(  std::string_view  name) noexcept;

  void  print() const noexcept;

};




}}


#endif




