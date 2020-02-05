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
  std::vector<const type_info*>  m_container;

  std::string  m_id;

public:
  parameter_list(std::initializer_list<const type_info*>  ls={}) noexcept;

  parameter_list&  push(const type_info&  ti) noexcept;

  const std::string&  get_id() const noexcept{return m_id;}

  const type_info**  begin() noexcept{return m_container.data();}
  const type_info**    end() noexcept{return m_container.data()+m_container.size();}

  const type_info* const*  begin() const noexcept{return m_container.data();}
  const type_info* const*    end() const noexcept{return m_container.data()+m_container.size();}

  int  get_number_of_elements() const noexcept{return m_container.size();}

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

  std::string  m_id=std::string("st")+std::to_string(m_id_source++);

public:
  struct_type_info() noexcept{}

  void  push(const type_info&  ti, std::string_view  name) noexcept;

  const struct_member*  find(std::string_view  name) const noexcept;

  int   get_size() const noexcept{return m_size;}
  int  get_align() const noexcept{return m_align;}

  const std::string&  get_id() const noexcept{return m_id;}

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

  std::string  m_id=std::string("un")+std::to_string(m_id_source++);

public:
  union_type_info() noexcept{}

  void  push(const type_info&  ti, std::string_view  name) noexcept;

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

  static uint32_t  m_id_source;

  std::string  m_id=std::string("en")+std::to_string(m_id_source++);

public:
  enum_type_info() noexcept{}

  void  push(std::string_view  name, int  value) noexcept;

  const int*  find(std::string_view  name) const noexcept;

  int   get_size() const noexcept{return 4;}
  int  get_align() const noexcept{return 4;}

  const std::string&  get_id() const noexcept{return m_id;}

  void  print() const noexcept;

};




class type_collection;
class type_info;


class
type_derivation
{
  const type_info&  m_base;

  std::vector<std::unique_ptr<type_info>>  m_array_type_info_list;
  std::vector<std::unique_ptr<type_info>>  m_function_type_info_list;

  std::unique_ptr<type_info>    m_pointer_type_info;
  std::unique_ptr<type_info>  m_reference_type_info;

public:
  type_derivation(const type_info&  ti) noexcept: m_base(ti){}

  const type_info&      get_array_type(int  n) noexcept;
  const type_info&  get_reference_type() noexcept;
  const type_info&    get_pointer_type() noexcept;
  const type_info&   get_function_type(parameter_list&&  parals) noexcept;

};


class
type_info
{
  const type_collection&  m_collection;

  const type_info*  m_base=nullptr;

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

  std::unique_ptr<type_derivation>  m_derivation;

  static std::string  make_id(         integer_type_info  ti) noexcept;
  static std::string  make_id(unsigned_integer_type_info  ti) noexcept;
  static std::string  make_id(const type_info&  base, pointer_type_info    ti) noexcept;
  static std::string  make_id(const type_info&  base, reference_type_info    ti) noexcept;
  static std::string  make_id(const type_info&  base, int  n) noexcept;
  static std::string  make_id(const type_info&  ret, const parameter_list&  parals) noexcept;

  std::unique_ptr<type_derivation>  mkdv() const noexcept{return std::make_unique<type_derivation>(*this);}

public:
  type_info(const type_collection&  tc)                          noexcept: m_collection(tc), m_kind(kind::null), m_id("null"), m_derivation(mkdv()){}
  type_info(const type_collection&  tc, void_type_info)            noexcept: m_collection(tc), m_kind(kind::void_), m_id("void"), m_derivation(mkdv()){}
  type_info(const type_collection&  tc, boolean_type_info)         noexcept: m_collection(tc), m_kind(kind::boolean), m_id("bool"), m_derivation(mkdv()){}
  type_info(const type_collection&  tc, null_pointer_type_info)    noexcept: m_collection(tc), m_kind(kind::null_pointer), m_id("nullptr"), m_derivation(mkdv()){}
  type_info(const type_collection&  tc, generic_pointer_type_info) noexcept: m_collection(tc), m_kind(kind::generic_pointer), m_id("geneptr"), m_derivation(mkdv()){}
  type_info(const type_collection&  tc,          integer_type_info  ti)  noexcept: m_collection(tc), m_kind(kind::integer), m_id(make_id(ti)), m_derivation(mkdv()){new(&m_data) integer_type_info(ti);}
  type_info(const type_collection&  tc, unsigned_integer_type_info  ti)  noexcept: m_collection(tc), m_kind(kind::unsigned_integer), m_id(make_id(ti)), m_derivation(mkdv()){new(&m_data) unsigned_integer_type_info(ti);}
  type_info(const type_collection&  tc, struct_type_info&&  ti) noexcept: m_collection(tc), m_kind(kind::struct_), m_id(ti.get_id()), m_derivation(mkdv()){new(&m_data) struct_type_info(std::move(ti));}
  type_info(const type_collection&  tc, union_type_info&&  ti)  noexcept: m_collection(tc), m_kind(kind::union_), m_id(ti.get_id()), m_derivation(mkdv()){new(&m_data) union_type_info(std::move(ti));}
  type_info(const type_collection&  tc, enum_type_info&&  ti)   noexcept: m_collection(tc), m_kind(kind::enum_), m_id(ti.get_id()), m_derivation(mkdv()){new(&m_data) enum_type_info(std::move(ti));}
  type_info(const type_info&  base, pointer_type_info    ti) noexcept: m_collection(base.m_collection), m_base(&base), m_kind(kind::pointer  ), m_id(make_id(base,ti)), m_derivation(mkdv()){}
  type_info(const type_info&  base, reference_type_info  ti) noexcept: m_collection(base.m_collection), m_base(&base), m_kind(kind::reference), m_id(make_id(base,ti)), m_derivation(mkdv()){}
  type_info(const type_info&  base, int  n) noexcept: m_collection(base.m_collection), m_base(&base), m_kind(kind::array), m_number_of_elements(n), m_id(make_id(base,n)), m_derivation(mkdv()){}
  type_info(const type_info&  ret, parameter_list&&  parals) noexcept: m_collection(ret.m_collection), m_base(&ret), m_kind(kind::function), m_derivation(mkdv()){  new(&m_data) parameter_list(std::move(parals));  m_id = make_id(ret,m_data.parals);}
  type_info(const type_info&   rhs) noexcept=delete;
  type_info(const type_info&&  rhs) noexcept=delete;
 ~type_info(){clear();}

  bool  operator==(const type_info&  rhs) const noexcept{return m_id == rhs.m_id;}
  bool  operator!=(const type_info&  rhs) const noexcept{return m_id != rhs.m_id;}

  type_info&  operator=(const type_info&   rhs) noexcept=delete;
  type_info&  operator=(const type_info&&  rhs) noexcept=delete;

  type_info&  clear() noexcept;

  const type_info*  get_base() const noexcept{return m_base;}

  const type_collection&  get_collection() const noexcept{return m_collection;}

  const std::string&  get_id() const noexcept{return m_id;}

  int  get_number_of_elements() const noexcept{return m_number_of_elements;}

  int  get_size() const noexcept;
  int  get_align() const noexcept;

           integer_type_info const&           get_integer_type_info() const noexcept{return m_data.int_ti;}
  unsigned_integer_type_info const&  get_unsigned_integer_type_info() const noexcept{return m_data.uint_ti;}

  struct_type_info&        get_struct_type_info() noexcept{return m_data.str_ti;}
  struct_type_info const&  get_struct_type_info() const noexcept{return m_data.str_ti;}

  union_type_info&        get_union_type_info() noexcept{return m_data.uni_ti;}
  union_type_info const&  get_union_type_info() const noexcept{return m_data.uni_ti;}

  enum_type_info&        get_enum_type_info() noexcept{return m_data.enu_ti;}
  enum_type_info const&  get_enum_type_info() const noexcept{return m_data.enu_ti;}

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

  bool  is_kind_of_integer() const noexcept{return is_integer() || is_unsigned_integer();}
  bool  is_kind_of_pointer() const noexcept{return is_pointer() || is_null_pointer() || is_generic_pointer();}
  bool  is_like_boolean()    const noexcept{return is_boolean() || is_kind_of_integer() || is_kind_of_pointer();}

  type_derivation&  get_derivation() const noexcept{return *m_derivation;}

  void  print() const noexcept;

};


class
type_entry
{
  std::string  m_name;

  const type_info&  m_type_info;

public:
  type_entry(std::string_view  name, const type_info&  ti) noexcept;

  const std::string&  get_name() const noexcept{return m_name;}
  const type_info&    get_info() const noexcept{return m_type_info;}

  void  print() const noexcept;

};




class
memory_sharer
{
  uint8_t*  m_data=nullptr;

  uint32_t  m_offset=0;
  uint32_t  m_length=0;

  void  unrefer() noexcept;

  uint64_t&  get_reference_count() const noexcept;

  uint8_t*  get_memory_pointer() const noexcept;

public:
  constexpr memory_sharer() noexcept{}
  memory_sharer(uint32_t  length) noexcept{assign(length);}
  memory_sharer(const memory_sharer&   rhs) noexcept{assign(rhs);}
  memory_sharer(      memory_sharer&&  rhs) noexcept{assign(std::move(rhs));}
  memory_sharer(const memory_sharer&  base, int  offset) noexcept{assign(base,offset);}
  memory_sharer(const memory_sharer&  base, int  offset, int  length) noexcept{assign(base,offset,length);}
 ~memory_sharer(){unrefer();}

  constexpr operator bool() const noexcept{return m_data;}

  uint8_t&  operator[](int  i) const noexcept{return get_memory_pointer()[m_offset+i];}

  memory_sharer&  operator=(const memory_sharer&   rhs) noexcept{return assign(rhs);}
  memory_sharer&  operator=(      memory_sharer&&  rhs) noexcept{return assign(std::move(rhs));}
  memory_sharer&  operator=(uint32_t  length) noexcept{return assign(length);}

  memory_sharer&  assign(const memory_sharer&   rhs) noexcept;
  memory_sharer&  assign(      memory_sharer&&  rhs) noexcept;
  memory_sharer&  assign(uint32_t  length) noexcept;
  memory_sharer&  assign(const memory_sharer&  base, int  offset) noexcept;
  memory_sharer&  assign(const memory_sharer&  base, int  offset, int  length) noexcept;

   int8_t&  get_s8(int  i=0) const noexcept{return reinterpret_cast<int8_t&>((*this)[m_offset+i]);}
  uint8_t&  get_u8(int  i=0) const noexcept{return                           (*this)[m_offset+i] ;}

   int16_t&  get_s16(int  i=0) const noexcept{return reinterpret_cast< int16_t&>((*this)[(m_offset&~1)+(2*i)]);}
  uint16_t&  get_u16(int  i=0) const noexcept{return reinterpret_cast<uint16_t&>((*this)[(m_offset&~1)+(2*i)]);}

   int32_t&  get_s32(int  i=0) const noexcept{return reinterpret_cast< int32_t&>((*this)[(m_offset&~3)+(4*i)]);}
  uint32_t&  get_u32(int  i=0) const noexcept{return reinterpret_cast<uint32_t&>((*this)[(m_offset&~3)+(4*i)]);}

   int64_t&  get_s64(int  i=0) const noexcept{return reinterpret_cast< int64_t&>((*this)[(m_offset&~7)+(8*i)]);}
  uint64_t&  get_u64(int  i=0) const noexcept{return reinterpret_cast<uint64_t&>((*this)[(m_offset&~7)+(8*i)]);}

  constexpr uint32_t  get_offset() const noexcept{return m_offset;}
  constexpr uint32_t  get_length() const noexcept{return m_length;}

  uint64_t  get_count() const noexcept{return get_reference_count();}

  memory_sharer  clone() const noexcept;

  memory_sharer&  copy(const memory_sharer&  src) noexcept;

  memory_sharer  operator+(int  n) const noexcept{return {*this, n};}
  memory_sharer  operator-(int  n) const noexcept{return {*this,-n};}

};


class
value
{
  const type_info*  m_type_info=nullptr;

  memory_sharer  m_memory;

public:
  constexpr value() noexcept{}
  value(const type_info&  ti) noexcept{assign(ti);}
  value(const type_info&  ti,  int64_t  i) noexcept{assign(ti,i);}
  value(const type_info&  ti, uint64_t  u) noexcept{assign(ti,u);}

  constexpr operator bool() const noexcept{return m_type_info;}

  value&  assign(const type_info&  ti             ) noexcept;
  value&  assign(const type_info&  ti,  int64_t  i) noexcept;
  value&  assign(const type_info&  ti, uint64_t  u) noexcept{return assign(ti,static_cast<int64_t>(u));}

  const type_info&  get_type_info() const noexcept{return *m_type_info;}
  type_derivation&  get_type_derivation() const noexcept{return m_type_info->get_derivation();}

        memory_sharer&  get_memory()       noexcept{return m_memory;}
  const memory_sharer&  get_memory() const noexcept{return m_memory;}

  int64_t&   get_integer()          const noexcept{return m_memory.get_s64();}
  uint64_t&  get_unsigned_integer() const noexcept{return m_memory.get_u64();}

  value  get_element(int  i) const noexcept;
  value  get_member(std::string_view  name) const noexcept;

  void  print() const noexcept;

};


class
type_collection
{
  using alias = std::pair<std::string,std::string>;

  std::vector<type_entry>  m_entry_table;
  std::vector<alias>       m_alias_table;

  std::vector<std::unique_ptr<type_info>>  m_info_table;

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

  const type_info&  push(std::string_view  name, const type_info&  ti) noexcept;
  const type_info&  push(std::string_view  name, std::unique_ptr<type_info>&&  ti) noexcept;
  const type_info&  push(                        std::unique_ptr<type_info>&&  ti) noexcept;

  const type_info*  find_by_id(  std::string_view    id) const noexcept;
  const type_info*  find_by_name(std::string_view  name) const noexcept;

  bool  make_alias(std::string_view  target_name, std::string_view  new_name) noexcept;

  const type_info*  create_from_string(const char*&  begin, const char*  end) noexcept;
  const type_info*  create_from_string(std::string_view  sv) noexcept{  auto  p = sv.data();  return create_from_string(p,p+sv.size());}

  void  print() const noexcept;

};




}}


#endif




