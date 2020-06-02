#ifndef gbstd_vm_HPP
#define gbstd_vm_HPP


#include"libgbstd/vms/space.hpp"


namespace gbstd{




using typesystem::type_info;
using typesystem::parameter;
using typesystem::parameter_list;
using typesystem::function_signature;
using typesystem::function_pointer_type_info;
using typesystem::pointer_type_info;
using typesystem::array_type_info;
using typesystem::struct_type_info;
using typesystem::union_type_info;


using boolean_t =  int8_t;


class
compile_error
{
public:
  int  m_line_number;

  std::string  m_comment;

  compile_error(int  ln, std::string_view  comm) noexcept: m_line_number(ln), m_comment(comm){}

};


namespace type_infos{
constexpr int  boolean_size = sizeof(boolean_t);
constexpr int  pointer_size = sizeof(address_t);
constexpr int  function_pointer_size = sizeof(function*);
constexpr int     enum_size = 4;

extern const type_info  int_;

extern const type_info  i8;
extern const type_info  i16;
extern const type_info  i32;
extern const type_info  i64;
extern const type_info  f32;
extern const type_info  f64;
extern const type_info  undefined;
extern const type_info  null_pointer;
extern const type_info  void_;
extern const type_info  null;
extern const type_info          boolean;
extern const type_info  generic_pointer;
}


class
memory
{
  union{
    uint8_t*  m_byte_array;
     int64_t  m_word;

  };

  struct property;

  property*  m_property=nullptr;

  void  unrefer() noexcept;

        uint8_t*  get_base_pointer()       noexcept{return (get_size() > sizeof(int64_t))? m_byte_array:reinterpret_cast<      uint8_t*>(&m_word);}
  const uint8_t*  get_base_pointer() const noexcept{return (get_size() > sizeof(int64_t))? m_byte_array:reinterpret_cast<const uint8_t*>(&m_word);}

public:
  memory(uint32_t  sz=sizeof(int64_t)) noexcept{allocate(sz);}
  memory(const memory&   rhs) noexcept{assign(rhs);}
  memory(      memory&&  rhs) noexcept{assign(std::move(rhs));}
 ~memory(){unrefer();}

  memory&  operator=(const memory&   rhs) noexcept{return assign(rhs);}
  memory&  operator=(      memory&&  rhs) noexcept{return assign(std::move(rhs));}

  template<typename  T>
  memory&  operator=(T  t) noexcept{return assign(t);}


  uint8_t&  operator[](int  i) noexcept{return get_base_pointer()[i];}

  void  allocate(uint32_t  sz) noexcept;

  template<typename  T>
  pointer_wrapper<T>  get_pointer(address_t  addr) noexcept{return reinterpret_cast<T*>(get_base_pointer()+addr);}

  template<typename  T>
  const T*  get_pointer(address_t  addr) const noexcept{return reinterpret_cast<const T*>(get_base_pointer()+addr);}


  memory&  assign(const memory&   rhs) noexcept;
  memory&  assign(      memory&&  rhs) noexcept;

  template<typename  T>
  memory&  assign(T  t) noexcept
  {
    allocate(sizeof(T));

    *get_pointer<T>(0) = t;

    return *this;
  }


  memory&  read(address_t  dst_addr, const memory&  src, address_t  src_addr, uint32_t  size) noexcept;

  uint32_t   get_size() const noexcept;

};




class
symbol
{
  type_info  m_type_info;

  std::string  m_name;

  int  m_attribute=0;

  const function*  m_function=nullptr;

  address_t  m_address=0;

public:
  symbol() noexcept{}
  symbol(const type_info&  ti, std::string_view  name, int attr, const function*  fn) noexcept:
  m_type_info(ti), m_name(name), m_attribute(attr), m_function(fn){}

  const type_info&  get_type_info() const noexcept{return m_type_info;}

  const std::string&  get_name() const noexcept{return m_name;}

  int  get_attribute() const noexcept{return m_attribute;}

  const function*  get_function() const noexcept{return m_function;}

  symbol&    set_address(address_t  addr)       noexcept{       m_address = addr;  return *this;}
  address_t  get_address(               ) const noexcept{return m_address                      ;}

};


class
symbol_table
{
public:
  enum class directions{  absolute, relative_forward, relative_backward,};

private:
  std::vector<std::unique_ptr<symbol>>  m_content;

  directions  m_direction;

  address_t  m_end_address=0;

public:
  symbol_table(directions  dir=directions::absolute) noexcept: m_direction(dir){}

        symbol&  operator[](int  i)       noexcept{return *m_content[i];}
  const symbol&  operator[](int  i) const noexcept{return *m_content[i];}

  void  clear() noexcept{  m_content.clear();  m_end_address = 0;}

  directions  get_direction() const noexcept{return m_direction;}

  bool  is_absolute()          const noexcept{return m_direction == directions::absolute;}
  bool  is_relative_forward()  const noexcept{return m_direction == directions::relative_forward;}
  bool  is_relative_backward() const noexcept{return m_direction == directions::relative_backward;}

  address_t  get_end_address() const noexcept{return m_end_address;}

  symbol&  push(const type_info&  ti, std::string_view  name, int  attr, const function*  fn) noexcept;
  symbol&  push(const symbol&  sym) noexcept;

  symbol_table&  push(const parameter_list&  ls) noexcept;

  const std::unique_ptr<symbol>&  find(std::string_view  name)       noexcept;
  const symbol*                   find(std::string_view  name) const noexcept;

  int  get_number_of_symbols() const noexcept{return m_content.size();}

  const std::unique_ptr<symbol>*  begin() const noexcept{return m_content.data();}
  const std::unique_ptr<symbol>*    end() const noexcept{return m_content.data()+m_content.size();}

  void  print(const memory*  mem=nullptr) const noexcept;

};




class
call_frame
{
  const function&  m_function;

  address_t  m_return_value_address;
  address_t  m_previous_bp;

  struct sub_frame{
    const block_statement&  m_block;

    const statement*  m_begin;
    const statement*  m_current;
    const statement*  m_end;

    sub_frame(const block_statement&  blk) noexcept:
    m_block(blk),
    m_begin(blk.get_space().get_statement_list().data()),
    m_current(blk.get_space().get_statement_list().data()),
    m_end(blk.get_space().get_statement_list().data()+blk.get_space().get_statement_list().size()){}

  };


  std::vector<sub_frame>  m_sub_stack;

  void  process(const     return_statement&  st, context&  ctx) noexcept;
  void  process(const       jump_statement&  st) noexcept;
  void  process(const      label_statement&  st) noexcept;
  void  process(const      block_statement&  st) noexcept;
  void  process(const    control_statement&  st) noexcept;
  void  process(const  if_string_statement&  st) noexcept;

public:
  call_frame(address_t&  bp, address_t&  sp, const function&  fn) noexcept;

  operator bool() const noexcept{return m_sub_stack.size();}

  void  operator()(context&  ctx) noexcept;

};


class
context
{
  const global_space*  m_global_space=nullptr;

  address_t  m_sp=0;
  address_t  m_bp=0;

  std::vector<call_frame>  m_call_stack;

  memory  m_memory;

public:
  context() noexcept;
  context(const global_space&  gsp) noexcept;

  void  clear() noexcept;

  address_t  get_bp() const noexcept{return m_bp;}

        memory&  get_memory()       noexcept{return m_memory;}
  const memory&  get_memory() const noexcept{return m_memory;}

  void  call_function(std::string_view  fn_name) noexcept;

  void  reset(std::string_view  fn_name) noexcept;
  bool   step() noexcept;
  void    run() noexcept;

  void  print() const noexcept;

};




}




#endif




