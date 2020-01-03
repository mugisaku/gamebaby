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


struct   pointer_type_info{};
struct reference_type_info{};




class
integer_type_info
{
  bool  m_signed=false;

  int  m_bitwidth=0;

public:
  integer_type_info(bool  s, int  w) noexcept: m_signed(s), m_bitwidth(w){}

  bool  is_signed() const noexcept{return m_signed;}

  int  get_bitwidth() const noexcept{return m_bitwidth;}

};




class
parameter
{
  type_info&  m_type_info;

  std::string  m_name;

public:
  parameter(type_info&  ti, std::string_view  name) noexcept:
  m_type_info(ti), m_name(name){}

  type_info&  get_type_info() const noexcept{return m_type_info;}

  const std::string&  get_name() const noexcept{return m_name;}

};


class
parameter_list
{
  std::vector<parameter>  m_container;

  std::string  m_id;

public:
  parameter_list(std::initializer_list<std::pair<type_info&,std::string_view>>  ls={}) noexcept;

  parameter_list&  push(type_info&  ti, std::string_view  name) noexcept;

  const std::string&  get_id() const noexcept{return m_id;}

  const parameter*  begin() const noexcept{return m_container.data();}
  const parameter*    end() const noexcept{return m_container.data()+m_container.size();}

  int  get_number_of_elements() const noexcept{return m_container.size();}

};




constexpr
size_t
get_aligned_offset(size_t  offset, size_t  align) noexcept
{
    if(align)
    {
      offset = (offset+(align-1))/align*align;
    }


  return offset;
}




class
struct_type_info
{
  struct member{
    type_info&  m_type_info;

    std::string  m_name;

    size_t  m_offset;

    member(type_info&  ti, std::string_view  name, size_t  offset) noexcept:
    m_type_info(ti), m_name(name), m_offset(offset){}

    bool  test_align(size_t  offset_base) const noexcept;

    void  print(FILE*  f=stdout, size_t  offset_base=0) const noexcept
    {
      fprintf(f,"(offset %zu) align is %s",offset_base+m_offset,test_align(offset_base)? "right":"wrong");
    }
  };


  std::vector<member>  m_member_list;

  int  m_size =0;
  int  m_align=0;

  std::string  m_id;

public:
  struct_type_info() noexcept{}

  void  push(type_info&  ti, std::string_view  name) noexcept;

  int   get_size() const noexcept{return m_size;}
  int  get_align() const noexcept{return m_align;}

  bool  test_align(size_t  offset_base) const noexcept;

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

  std::string  m_id;

public:
  union_type_info() noexcept{}

  void  push(type_info&  ti, std::string_view  name) noexcept;

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

  int  get_value() const noexcept{return m_value;}

  void  print() const noexcept{printf("%s = %d",m_name.data(),m_value);}

};


class
enum_type_info
{
  std::vector<enumerator>  m_enumerator_list;

  int  m_last_value=0;

public:
  enum_type_info() noexcept{}

  void  push(std::string_view  name            ) noexcept;
  void  push(std::string_view  name, int  value) noexcept;

  int   get_size() const noexcept{return 4;}
  int  get_align() const noexcept{return 4;}

  std::string  get_id() const noexcept;

  void  print() const noexcept;

};




class
type_info
{
  type_info*  m_base_type_info=nullptr;

  enum class kind{
    null,
    array,
    pointer,
    reference,
    integer,
    function,
    struct_,
    union_,
    enum_,
  } m_kind=kind::null;

  union data{
    integer_type_info  int_ti;
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

  static std::string  make_id(integer_type_info  ti) noexcept;
  static std::string  make_id(const type_info&  base, pointer_type_info    ti) noexcept;
  static std::string  make_id(const type_info&  base, reference_type_info    ti) noexcept;
  static std::string  make_id(const type_info&  base, int  n) noexcept;
  static std::string  make_id(const type_info&  ret, const parameter_list&  parals) noexcept;

public:
  static constexpr int  get_pointer_size() noexcept{return 4;}

  type_info(integer_type_info  ti)  noexcept: m_kind(kind::integer), m_id(make_id(ti)){new(&m_data) integer_type_info(ti);}
  type_info(struct_type_info&&  ti) noexcept: m_kind(kind::struct_), m_id(ti.get_id()){new(&m_data) struct_type_info(std::move(ti));}
  type_info(union_type_info&&  ti)  noexcept: m_kind(kind::union_), m_id(ti.get_id()){new(&m_data) union_type_info(std::move(ti));}
  type_info(enum_type_info&&  ti)   noexcept: m_kind(kind::enum_), m_id(ti.get_id()){new(&m_data) enum_type_info(std::move(ti));}
  type_info(type_info&  base, pointer_type_info    ti) noexcept: m_base_type_info(&base), m_kind(kind::pointer  ), m_id(make_id(base,ti)){}
  type_info(type_info&  base, reference_type_info  ti) noexcept: m_base_type_info(&base), m_kind(kind::reference), m_id(make_id(base,ti)){}
  type_info(type_info&  base, int  n) noexcept: m_base_type_info(&base), m_kind(kind::array), m_number_of_elements(n), m_id(make_id(base,n)){}
  type_info(type_info&  ret, parameter_list&&  parals) noexcept: m_base_type_info(&ret), m_kind(kind::function){  new(&m_data) parameter_list(std::move(parals));  m_id = make_id(ret,m_data.parals);}
  type_info(const type_info&   rhs) noexcept=delete;
  type_info(const type_info&&  rhs) noexcept=delete;
 ~type_info(){clear();}

  bool  operator==(type_info const&  rhs) const noexcept{return m_id == rhs.m_id;}

  type_info&  operator=(const type_info&   rhs) noexcept=delete;
  type_info&  operator=(const type_info&&  rhs) noexcept=delete;

  type_info&  assign(const type_info&   rhs) noexcept;
  type_info&  assign(const type_info&&  rhs) noexcept;

  type_info&  clear() noexcept;

  type_info*  get_base_type_info() const noexcept{return m_base_type_info;}

  const std::string&  get_id() const noexcept{return m_id;}

  int  get_number_of_elements() const noexcept{return m_number_of_elements;}

  int  get_size() const noexcept;
  int  get_align() const noexcept;

  integer_type_info&        get_integer_type() noexcept{return m_data.int_ti;}
  integer_type_info const&  get_integer_type() const noexcept{return m_data.int_ti;}

  struct_type_info&        get_struct_type() noexcept{return m_data.str_ti;}
  struct_type_info const&  get_struct_type() const noexcept{return m_data.str_ti;}

  union_type_info&        get_union_type() noexcept{return m_data.uni_ti;}
  union_type_info const&  get_union_type() const noexcept{return m_data.uni_ti;}

  enum_type_info&        get_enum_type() noexcept{return m_data.enu_ti;}
  enum_type_info const&  get_enum_type() const noexcept{return m_data.enu_ti;}

  parameter_list&         get_parameter_list()       noexcept{return m_data.parals;}
  parameter_list const&   get_parameter_list() const noexcept{return m_data.parals;}

  bool  is_null()      const noexcept{return m_kind == kind::null;}
  bool  is_integer()   const noexcept{return m_kind == kind::integer;}
  bool  is_pointer()   const noexcept{return m_kind == kind::pointer;}
  bool  is_reference() const noexcept{return m_kind == kind::reference;}
  bool  is_array()     const noexcept{return m_kind == kind::array;}
  bool  is_function()  const noexcept{return m_kind == kind::function;}
  bool  is_struct()    const noexcept{return m_kind == kind::struct_;}
  bool  is_enum()      const noexcept{return m_kind == kind::enum_;}
  bool  is_union()     const noexcept{return m_kind == kind::union_;}

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
  type_info  m_s8_type_info;
  type_info  m_u8_type_info;
  type_info  m_s16_type_info;
  type_info  m_u16_type_info;
  type_info  m_s32_type_info;
  type_info  m_u32_type_info;
  type_info  m_s64_type_info;
  type_info  m_u64_type_info;

  std::vector<std::unique_ptr<type_entry>>  m_entry_table;

public:
  type_collection() noexcept;

  void  push(std::string_view  name, type_info*  ti=nullptr) noexcept;

  type_entry*  find(std::string_view  name) const noexcept;

  bool  make_alias(std::string_view  target_name, std::string_view  new_name) noexcept;

  void  print() const noexcept;

};




}}


#endif




