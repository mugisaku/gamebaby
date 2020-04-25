#ifndef gbstd_vm_HPP
#define gbstd_vm_HPP


#include<cstdio>
#include<cstdint>
#include<cinttypes>
#include<cstdlib>
#include<memory>
#include<string>
#include<string_view>
#include<utility>
#include<vector>
#include"libgbstd/misc.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/typesystem.hpp"
#include"libgbstd/parser.hpp"


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


class function;
class context;
class symbol_table;
class expression;
class space_node;
class statement;
class return_statement;
class label_statement;
class jump_statement;
class control_statement;
class block_statement;
class condition_statement;
class if_string_statement;
class expression_statement;


using address_t = int32_t;
using boolean_t =  int8_t;


constexpr address_t
get_aligned_address(address_t  addr) noexcept
{
  constexpr auto  align = sizeof(int64_t);

  return (addr+(align-1))/align*align;
}


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




#include"libgbstd/vms/vm_expression.hpp"




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
memo_info
{
  type_info  m_type_info;

  std::string  m_name;

  address_t  m_address=0;

public:
  memo_info() noexcept{}
  memo_info(const type_info&  ti, std::string_view  name) noexcept: m_type_info(ti), m_name(name){}

  const type_info&  get_type_info() const noexcept{return m_type_info;}

  const std::string&  get_name() const noexcept{return m_name;}

  void       set_address(address_t  addr)       noexcept{m_address = addr;}
  address_t  get_addrees(               ) const noexcept{return m_address;}

  void  print() const noexcept{
    m_type_info.print();
    printf("  %s",m_name.data());
  }

};




class
basic_space
{
protected:
  space_node&  m_node;

  std::vector<type_info>  m_type_info_table;

  std::vector<std::unique_ptr<memo_info>>  m_memo_info_table;
  std::vector<std::reference_wrapper<function>>  m_function_reference_table;

public:
  basic_space(space_node&  nd) noexcept: m_node(nd){}

  void  clear() noexcept;

  space_node&  get_node() const noexcept{return m_node;}

  const std::vector<type_info>&  get_type_info_table() const noexcept{return m_type_info_table;}
  const std::vector<std::unique_ptr<memo_info>>&  get_memo_info_table() const noexcept{return m_memo_info_table;}
  const std::vector<std::reference_wrapper<function>>&    get_function_reference_table() const noexcept{return m_function_reference_table;}

  void  push_type_info(type_info&&  ti) noexcept{m_type_info_table.emplace_back(std::move(ti));}
  void  push_memo_info(const type_info&  ti, std::string_view  name) noexcept{m_memo_info_table.emplace_back(std::make_unique<memo_info>(ti,name));}
  void  push_function(function&  fn) noexcept{m_function_reference_table.emplace_back(fn);}

  const function*  find_function(std::string_view  name) const noexcept;

  type_info  find_type_info_by_name(std::string_view  name) const noexcept;
  type_info  find_type_info_by_id(  std::string_view    id) const noexcept;

  const memo_info*  find_memo_info(std::string_view  name) const noexcept;

  void  print() const noexcept;

};


#include"libgbstd/vms/vm_statement.hpp"




class global_space;
class block_statement;




class
function
{
  space_node&  m_node;

  std::vector<std::unique_ptr<memo_info>>  m_parameter_memo_info_table;

  std::string  m_name;

  function_signature  m_signature;

  const block_statement*  m_main_block=nullptr;

public:
  function(space_node&  nd, std::string_view  name, function_signature&&  sig) noexcept;

  space_node&  get_node() const noexcept{return m_node;}

  const std::vector<std::unique_ptr<memo_info>>&  get_parameter_memo_info_table() const noexcept {return m_parameter_memo_info_table;}

  void  set_main_block(const block_statement&  blk) noexcept{m_main_block = &blk;}

  const std::string&  get_name() const noexcept{return m_name;}

  const function_signature&  get_signature() const noexcept{return m_signature;}

  void  push_memo_info(const type_info&  ti, std::string_view  name) noexcept{m_parameter_memo_info_table.emplace_back(std::make_unique<memo_info>(ti,name));}

  void  print() const noexcept;

};


class
global_space: public basic_space
{
  std::string  m_source;

  void  initialize() noexcept;

public:
  global_space(space_node&  nd) noexcept: basic_space(nd){initialize();}
  global_space(space_node&  nd, std::string_view  sv): basic_space(nd){  initialize();  assign(sv);}

  global_space&  assign(std::string_view  sv);

  void  clear() noexcept;

};


class
space_node
{
  space_node*  m_parent=nullptr;
  space_node*  m_root=nullptr;

  std::vector<std::unique_ptr<space_node>>  m_children;
 

  enum class kinds{
    null, global_space, function, block
  } m_kind=kinds::null;

  union data{
    global_space     gsp;
    function          fn;
    block_statement  blk;

    data() noexcept{}
   ~data(){}
  } m_data;

  parameter_list  read_parameter_list(token_iterator&  it) noexcept;

  statement    read_return(token_iterator&  it) noexcept;
  statement    read_jump(token_iterator&  it) noexcept;
  statement    read_label(token_iterator&  it) noexcept;
  statement    read_if(token_iterator&  it) noexcept;
  statement    read_for(token_iterator&  it) noexcept;
  statement    read_while(token_iterator&  it) noexcept;
  statement    read_switch(token_iterator&  it) noexcept;
  statement    read_let(token_iterator&  it);

  type_info   read_struct_type_info(token_iterator&  it) noexcept;
  type_info    read_union_type_info(token_iterator&  it) noexcept;
  type_info     read_enum_type_info(token_iterator&  it) noexcept;
  type_info  read_derived_type_info(token_iterator&  it) noexcept;

  type_info                    read_alias(token_iterator&  it) noexcept;
  type_info   read_named_struct_type_info(token_iterator&  it) noexcept;
  type_info    read_named_union_type_info(token_iterator&  it) noexcept;
  type_info     read_named_enum_type_info(token_iterator&  it) noexcept;

  const function*  read_function(token_iterator&  it) noexcept;

  void  read_element_that_begins_with_identifier(token_iterator&  it);

public:
  space_node() noexcept;
  space_node(std::string_view  src);
  space_node(space_node&  parent, std::string_view  name, function_signature&&  fnsig) noexcept;
  space_node(space_node&  parent, nullptr_t) noexcept;
 ~space_node();

  space_node&  read(token_iterator&  it);
  space_node&  read(std::string_view  sv);

  pointer_wrapper<function>  create_function(std::string_view  name, function_signature&&  fnsig) noexcept;
  pointer_wrapper<block_statement>  create_block() noexcept;

  bool  push_type_info(type_info&&  ti) noexcept;
  bool  push_memo_info(const type_info&  ti, std::string_view  name) noexcept;
  bool  push_statement(statement&&  st) noexcept;

  const function*  find_function(std::string_view  name) const noexcept;

  type_info  find_type_info_by_name(std::string_view  name) const noexcept;
  type_info  find_type_info_by_id(  std::string_view    id) const noexcept;

  const memo_info*  find_memo_info(std::string_view  name) const noexcept;

  bool  is_global_space() const noexcept{return m_kind == kinds::global_space;}
  bool      is_function() const noexcept{return m_kind == kinds::function;}
  bool         is_block() const noexcept{return m_kind == kinds::block;}

  global_space&  get_global_space() noexcept{return m_data.gsp;}
  function&          get_function() noexcept{return m_data.fn;}
  block_statement&      get_block() noexcept{return m_data.blk;}

  void  print() const noexcept;

};




class
call_frame
{
  const function*  m_function;

  struct state{
    const statement*  m_begin;
    const statement*  m_current;
    const statement*  m_end;

  };


  std::vector<state>  m_state_stack;

  void  process(const     return_statement&  st, context&  ctx) noexcept;
  void  process(const       jump_statement&  st) noexcept;
  void  process(const      label_statement&  st) noexcept;
  void  process(const      block_statement&  st) noexcept;
  void  process(const    control_statement&  st) noexcept;
  void  process(const  if_string_statement&  st) noexcept;

public:
  call_frame(const function&  fn) noexcept;

  operator bool() const noexcept{return m_state_stack.size();}

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

  void  call_function(std::string_view  fn_name, address_t  return_value_address) noexcept;

  void  print_stack_frame() const noexcept;
  void  push_stack_frame(address_t  return_address, address_t  return_value_address, size_t  size) noexcept;
  void   pop_stack_frame() noexcept;

  void  reset() noexcept;
  bool   step() noexcept;
  void    run() noexcept;

  void  print() const noexcept;

};




}




#endif




