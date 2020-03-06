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
using typesystem::type_collection;
using typesystem::pointer_type_info;
using typesystem::array_type_info;
using typesystem::struct_type_info;
using typesystem::union_type_info;


class function;
class context;
class expression;
class statement;
class return_statement;
class label_statement;
class jump_statement;
class control_statement;
class block_statement;
class condition_statement;
class if_string_statement;
class expression_statement;


using address_t = uint32_t;
using boolean_t =  uint8_t;


namespace type_infos{
constexpr int  boolean_size = sizeof(boolean_t);
constexpr int  pointer_size = sizeof(address_t);
constexpr int     enum_size = 4;

extern const type_info  s8;
extern const type_info  u8;
extern const type_info  s16;
extern const type_info  u16;
extern const type_info  s32;
extern const type_info  u32;
extern const type_info  s64;
extern const type_info  u64;
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


class   hot_object;
class tepid_object;
class  cold_object;


class
cold_object
{
  memory  m_memory;

  const type_info*  m_type_info=nullptr;

public:
  cold_object() noexcept{}
  cold_object(const tepid_object&  o) noexcept;
  cold_object(const   hot_object&  o) noexcept;
  cold_object(const type_info&  ti, address_t  addr) noexcept;
  cold_object(bool  b) noexcept;
  cold_object(  int8_t  i) noexcept;
  cold_object( uint8_t  u) noexcept;
  cold_object( int16_t  i) noexcept;
  cold_object(uint16_t  u) noexcept;
  cold_object( int32_t  i) noexcept;
  cold_object(uint32_t  u) noexcept;
  cold_object( int64_t  i) noexcept;
  cold_object(uint64_t  u) noexcept;
  cold_object(float   f) noexcept;
  cold_object(double  f) noexcept;
  cold_object(nullptr_t  ptr) noexcept;
  cold_object(std::string_view  sv) noexcept;

  operator bool() const noexcept{return m_type_info;}

        memory&  get_memory()       noexcept{return m_memory;}
  const memory&  get_memory() const noexcept{return m_memory;}

  const type_info&  get_type_info() const noexcept{return *m_type_info;}

   int64_t  get_integer()          const noexcept;
  uint64_t  get_unsigned_integer() const noexcept;

  double  get_fpn() const noexcept;

  int  get_size() const noexcept{return m_type_info->get_size();}

  void  print() const noexcept;

};


class
tepid_object: public cold_object
{
  memory  m_base_memory;

public:
  tepid_object() noexcept{}
  tepid_object(const hot_object&   ho) noexcept;
  tepid_object(const cold_object&  co, const memory&  base_mem) noexcept;

        memory&  get_base_memory()       noexcept{return m_base_memory;}
  const memory&  get_base_memory() const noexcept{return m_base_memory;}

  void  print() const noexcept;

};


class
hot_object
{
  memory  m_memory;

  const type_info*  m_type_info=nullptr;

  address_t  m_address=0;

public:
  hot_object() noexcept{}
  hot_object(const tepid_object&  o) noexcept;
  hot_object(const memory&  mem, const type_info&  ti, address_t  addr) noexcept:
  m_memory(mem), m_type_info(&ti), m_address(addr){}

  operator bool() const noexcept{return m_type_info;}

        memory&  get_memory()       noexcept{return m_memory;}
  const memory&  get_memory() const noexcept{return m_memory;}

  const type_info&  get_type_info() const noexcept{return *m_type_info;}

  address_t  get_address() const noexcept{return m_address;}

  int  get_size() const noexcept{return m_type_info->get_size();}

   int64_t  get_integer()          const noexcept;
  uint64_t  get_unsigned_integer() const noexcept;

  double  get_fpn() const noexcept;

  hot_object  get_struct_member(std::string_view  name) const noexcept;
  hot_object  get_union_member(std::string_view  name) const noexcept;
  hot_object  get_element(int  i) const noexcept;

  tepid_object  write(cold_object  co) noexcept;
	
};




namespace operations{
cold_object      bit_not(cold_object  o) noexcept;
cold_object  logical_not(cold_object  o) noexcept;
cold_object          neg(cold_object  o) noexcept;
cold_object         size(cold_object  o) noexcept;
cold_object      address(hot_object  o) noexcept;
hot_object   dereference(cold_object  o, const memory&  home_mem) noexcept;
hot_object       prefix_increment(hot_object  o) noexcept;
hot_object       prefix_decrement(hot_object  o) noexcept;
hot_object      postfix_increment(hot_object  o) noexcept;
hot_object      postfix_decrement(hot_object  o) noexcept;


cold_object      add(cold_object  lo, cold_object  ro) noexcept;
cold_object      sub(cold_object  lo, cold_object  ro) noexcept;
cold_object      mul(cold_object  lo, cold_object  ro) noexcept;
cold_object      div(cold_object  lo, cold_object  ro) noexcept;
cold_object      rem(cold_object  lo, cold_object  ro) noexcept;
cold_object      shl(cold_object  lo, cold_object  ro) noexcept;
cold_object      shr(cold_object  lo, cold_object  ro) noexcept;
cold_object   bit_or(cold_object  lo, cold_object  ro) noexcept;
cold_object  bit_and(cold_object  lo, cold_object  ro) noexcept;
cold_object  bit_xor(cold_object  lo, cold_object  ro) noexcept;

tepid_object      add_assign(hot_object  lo, cold_object  ro) noexcept;
tepid_object      sub_assign(hot_object  lo, cold_object  ro) noexcept;
tepid_object      mul_assign(hot_object  lo, cold_object  ro) noexcept;
tepid_object      div_assign(hot_object  lo, cold_object  ro) noexcept;
tepid_object      rem_assign(hot_object  lo, cold_object  ro) noexcept;
tepid_object      shl_assign(hot_object  lo, cold_object  ro) noexcept;
tepid_object      shr_assign(hot_object  lo, cold_object  ro) noexcept;
tepid_object   bit_or_assign(hot_object  lo, cold_object  ro) noexcept;
tepid_object  bit_and_assign(hot_object  lo, cold_object  ro) noexcept;
tepid_object  bit_xor_assign(hot_object  lo, cold_object  ro) noexcept;
tepid_object          assign(hot_object  lo, cold_object  ro) noexcept;

cold_object           eq(cold_object  lo, cold_object  ro) noexcept;
cold_object          neq(cold_object  lo, cold_object  ro) noexcept;
cold_object           lt(cold_object  lo, cold_object  ro) noexcept;
cold_object         lteq(cold_object  lo, cold_object  ro) noexcept;
cold_object           gt(cold_object  lo, cold_object  ro) noexcept;
cold_object         gteq(cold_object  lo, cold_object  ro) noexcept;
cold_object   logical_or(cold_object  lo, cold_object  ro) noexcept;
cold_object  logical_and(cold_object  lo, cold_object  ro) noexcept;

cold_object      comma(cold_object  lo, cold_object  ro) noexcept;
cold_object  subscript(cold_object  lo, cold_object  ro) noexcept;
}




class
mutability
{
  bool  m_value;

public:
  explicit constexpr mutability(bool  v=false) noexcept: m_value(v){}

  operator bool() const noexcept{return m_value;}

};


namespace mutabilities{
constexpr auto   mute = mutability( true);
constexpr auto  unmute = mutability(false);
}


using mutabilities::mute;
using mutabilities::unmute;


struct undefined{};

class
identifier
{
  std::string  m_string;

public:
  identifier() noexcept{}
  identifier(std::string_view  sv) noexcept: m_string(sv){}

  const std::string&  get_string() const noexcept{return m_string;}

  std::string  release() noexcept{return std::move(m_string);}

};


class expression;


class
unary_operation
{
  operator_code  m_opcode;

  std::unique_ptr<expression>  m_operand;

public:
  unary_operation(operator_code  op, std::unique_ptr<expression>&&  o) noexcept:
  m_opcode(op), m_operand(std::move(o)){}

  unary_operation(const unary_operation&  rhs) noexcept{assign(rhs);}

  unary_operation&  operator=(const unary_operation&  rhs) noexcept{return assign(rhs);}
  unary_operation&  assign(const unary_operation&  rhs) noexcept;

  operator_code  get_opcode() const noexcept{return m_opcode;}

  const expression&  get_operand() const noexcept{return *m_operand;}

};


class
binary_operation
{
  operator_code  m_opcode;

  std::unique_ptr<expression>  m_left;
  std::unique_ptr<expression>  m_right;

public:
  binary_operation(operator_code  op, std::unique_ptr<expression>&&  l, std::unique_ptr<expression>&&  r) noexcept:
  m_opcode(op), m_left(std::move(l)), m_right(std::move(r)){}

  binary_operation(const binary_operation&  rhs) noexcept{assign(rhs);}

  binary_operation&  operator=(const binary_operation&  rhs) noexcept{return assign(rhs);}
  binary_operation&  assign(const binary_operation&  rhs) noexcept;

  operator_code  get_opcode() const noexcept{return m_opcode;}

  const expression&  get_left()  const noexcept{return *m_left;}
  const expression&  get_right() const noexcept{return *m_right;}

};


class
expression
{
  enum class kind{
    null,

    identifier,
    undefined_literal,
    null_pointer_literal,
    boolean_literal,
    string_literal,
    integer_literal,
    fpn_literal,

     prefix_unary,
    postfix_unary,
    binary,

  } m_kind=kind::null;

  union data{
    bool         b;
    std::string  s;
    uint64_t     i;

    double  f;

     unary_operation   un;
    binary_operation  bin;

  data() noexcept{}
 ~data(){}

  } m_data;

public:
  expression() noexcept{}
  expression(const expression&   rhs) noexcept{assign(rhs);}
  expression(      expression&&  rhs) noexcept{assign(std::move(rhs));}
  expression(undefined  u) noexcept{assign(u);}
  expression(nullptr_t  n) noexcept{assign(n);}
  expression(bool  b) noexcept{assign(b);}
  expression(identifier&&  id) noexcept{assign(std::move(id));}
  expression(std::string_view  sv) noexcept{assign(sv);}
  expression(uint64_t  i) noexcept{assign(i);}
  expression(double  f) noexcept{assign(f);}
  expression(int  k, unary_operation&&  op) noexcept{assign(k,std::move(op));}
  expression(binary_operation&&  op) noexcept{assign(std::move(op));}
 ~expression(){clear();}

  expression&  operator=(const expression&   rhs) noexcept{return assign(rhs);}
  expression&  operator=(      expression&&  rhs) noexcept{return assign(std::move(rhs));}
  expression&  operator=(undefined  u) noexcept{return assign(u);}
  expression&  operator=(nullptr_t  n) noexcept{return assign(n);}
  expression&  operator=(bool  b) noexcept{return assign(b);}
  expression&  operator=(identifier&&  id) noexcept{return assign(std::move(id));}
  expression&  operator=(std::string_view  sv) noexcept{return assign(sv);}
  expression&  operator=(uint64_t  i) noexcept{return assign(i);}
  expression&  operator=(double  f) noexcept{return assign(f);}
  expression&  operator=(binary_operation&&  op) noexcept{return assign(std::move(op));}

  expression&  assign(const expression&   rhs) noexcept;
  expression&  assign(      expression&&  rhs) noexcept;
  expression&  assign(undefined  u) noexcept;
  expression&  assign(nullptr_t  n) noexcept;
  expression&  assign(bool  b) noexcept;
  expression&  assign(identifier&&  id) noexcept;
  expression&  assign(std::string_view  sv) noexcept;
  expression&  assign(uint64_t  i) noexcept;
  expression&  assign(double  f) noexcept;
  expression&  assign(int  k, unary_operation&&  op) noexcept;
  expression&  assign(binary_operation&&  op) noexcept;

  expression&  clear() noexcept;

  const std::string&  get_string() const noexcept{return m_data.s;}

  bool      get_boolean() const noexcept{return m_data.b;}
  uint64_t  get_integer() const noexcept{return m_data.i;}
  double    get_fpn()     const noexcept{return m_data.f;}

  bool  is_undefined_literal()    const noexcept{return m_kind == kind::undefined_literal;}
  bool  is_boolean_literal()      const noexcept{return m_kind == kind::boolean_literal;}
  bool  is_null_pointer_literal() const noexcept{return m_kind == kind::null_pointer_literal;}
  bool  is_identifier()           const noexcept{return m_kind == kind::identifier;}
  bool  is_string_literal()       const noexcept{return m_kind == kind::string_literal;}
  bool  is_integer_literal()      const noexcept{return m_kind == kind::integer_literal;}
  bool  is_fpn_literal()          const noexcept{return m_kind == kind::fpn_literal;}
  bool  is_prefix_unary()  const noexcept{return m_kind == kind::prefix_unary;}
  bool  is_postfix_unary() const noexcept{return m_kind == kind::postfix_unary;}
  bool  is_binary()        const noexcept{return m_kind == kind::binary;}

  tepid_object  evaluate(context&  ctx) const noexcept;

  void  print() const noexcept;

};


expression  make_expression(std::string_view  sv) noexcept;




class
declaration
{
  const type_info*  m_type_info=nullptr;

  std::string  m_name;

  int  m_attribute=0;

public:
  declaration() noexcept{}
  declaration(const type_info&  ti, std::string_view  nam, int  attr) noexcept:
  m_type_info(&ti), m_name(nam), m_attribute(attr){}

  const type_info*  get_type_info() const noexcept{return m_type_info;}

  const std::string&  get_name() const noexcept{return m_name;}

  int  get_attribute() const noexcept{return m_attribute;}

  uint32_t  get_size()  const noexcept{return m_type_info? m_type_info->get_size() :0;}
  uint32_t  get_align() const noexcept{return m_type_info? m_type_info->get_align():0;}

};


class
symbol: public declaration
{
  const function*  m_function=nullptr;

  address_t  m_address;

public:
  symbol(declaration&&  decl, const function*  fn, address_t  addr) noexcept:
  declaration(std::move(decl)), m_function(fn), m_address(addr){}

  const function*  get_function() const noexcept{return m_function;}

  address_t  get_address() const noexcept{return m_address;}

};


using declaration_list = std::vector<declaration>;

class
symbol_table
{
  address_t  m_base_address=0;
  address_t   m_end_address=0;

  std::vector<symbol>  m_content;

public:
  symbol_table(address_t  base_addr=0) noexcept:
  m_base_address(base_addr), m_end_address(base_addr){}

  address_t  get_base_address() const noexcept{return m_base_address;}
  address_t  get_end_address()  const noexcept{return m_end_address;}

  symbol&  push(const declaration&  decl, const function*  fn) noexcept;
  symbol&  push(const type_info&  ti, std::string_view  name) noexcept{return push(declaration(ti,name,0),nullptr);}

  void  pop() noexcept;

        symbol*  find(std::string_view  name)       noexcept;
  const symbol*  find(std::string_view  name) const noexcept;

  int  get_number_of_symbols() const noexcept{return m_content.size();}

  hot_object  make_object(std::string_view  name, const memory&  mem) const noexcept;

  bool  reallocate() noexcept;

  const symbol*  begin() const noexcept{return m_content.data();}
  const symbol*    end() const noexcept{return m_content.data()+m_content.size();}

  void  print(const memory&  mem) const noexcept;

};




class
mini_context
{
  memory  m_memory;

  symbol_table  m_symbol_table;

public:
  mini_context() noexcept: m_memory(1024){}

  tepid_object  operator[](std::string_view  name) const noexcept{return m_symbol_table.make_object(name,m_memory);}

        symbol_table&  get_symbol_table()       noexcept{return m_symbol_table;}
  const symbol_table&  get_symbol_table() const noexcept{return m_symbol_table;}

        memory&  get_memory()       noexcept{return m_memory;}
  const memory&  get_memory() const noexcept{return m_memory;}

  void  print() const noexcept{m_symbol_table.print(m_memory);}

};




class
context
{
  type_collection  m_type_collection;

  std::vector<std::unique_ptr<type_info>>  m_type_info_table;

  symbol_table   m_global_symbol_table;
  symbol_table  m_runtime_symbol_table;

  std::vector<const function*>  m_function_table;

  memory  m_memory;

  struct frame;

  frame*  m_current_frame=nullptr;

  bool  m_finalized=false;

  void  push_block(const block_statement&  st) noexcept;

  void  exit_block() noexcept;

  void  push_frame(uint32_t  st_p, const function&  fn, int  argc, const expression*  args) noexcept;
  void  pop_frame() noexcept;


  void  process(const            statement&  st) noexcept;
  void  process(const     return_statement&  st) noexcept;
  void  process(const       jump_statement&  st) noexcept;
  void  process(const      label_statement&  st) noexcept;
  void  process(const      block_statement&  st) noexcept;
  void  process(const    control_statement&  st) noexcept;
  void  process(const  if_string_statement&  st) noexcept;
  void  process(const expression_statement&  st) noexcept;

public:
  context() noexcept;
 ~context(){clear();}

  void  clear() noexcept;

        type_collection&  get_type_collection()       noexcept{return m_type_collection;}
  const type_collection&  get_type_collection() const noexcept{return m_type_collection;}

  void  append_function(const function&  fn) noexcept;

  const type_info&  append_type_info(std::unique_ptr<type_info>&&  ti) noexcept;

        symbol_table&  get_global_symbol_table()       noexcept{return m_global_symbol_table;}
  const symbol_table&  get_global_symbol_table() const noexcept{return m_global_symbol_table;}

        symbol_table&  get_runtime_symbol_table()       noexcept{return m_runtime_symbol_table;}
  const symbol_table&  get_runtime_symbol_table() const noexcept{return m_runtime_symbol_table;}

        memory&  get_memory()       noexcept{return m_memory;}
  const memory&  get_memory() const noexcept{return m_memory;}

  address_t  get_bp() const noexcept;

  void  reset() noexcept;
  bool   step() noexcept;
  void    run() noexcept;

  context&  finalize() noexcept;

  const type_info*  create_type_from_string(std::string_view  sv) noexcept;

  void  print() const noexcept;

};




class
statement
{
  enum class kind{
    null,
    return_,
    block,
    control,
    if_string,
    expression,
    jump,
    label,

  } m_kind=kind::null;

  void*  m_pointer=nullptr;

public:
  constexpr statement() noexcept{}
  statement(const statement&   rhs) noexcept=delete;
  statement(      statement&&  rhs) noexcept{assign(std::move(rhs));}
  statement(return_statement*       st) noexcept{assign(st);}
  statement( label_statement*       st) noexcept{assign(st);}
  statement( jump_statement*        st) noexcept{assign(st);}
  statement( if_string_statement*   st) noexcept{assign(st);}
  statement( block_statement*       st) noexcept{assign(st);}
  statement( control_statement*     st) noexcept{assign(st);}
  statement( expression_statement*  st) noexcept{assign(st);}
 ~statement(){clear();}

  statement&  operator=(const statement&   rhs) noexcept=delete;
  statement&  operator=(      statement&&  rhs) noexcept{return assign(std::move(rhs));}

  operator bool() const noexcept{return m_pointer;}

  statement&  assign(statement&&  rhs) noexcept;
  statement&  assign(return_statement*       st) noexcept;
  statement&  assign( label_statement*       st) noexcept;
  statement&  assign( jump_statement*        st) noexcept;
  statement&  assign( if_string_statement*   st) noexcept;
  statement&  assign( block_statement*       st) noexcept;
  statement&  assign( control_statement*     st) noexcept;
  statement&  assign( expression_statement*  st) noexcept;

  void  clear() noexcept;

  bool  is_null()       const noexcept{return m_kind == kind::null;}
  bool  is_return()     const noexcept{return m_kind == kind::return_;}
  bool  is_label()      const noexcept{return m_kind == kind::label;;}
  bool  is_jump()       const noexcept{return m_kind == kind::jump;}
  bool  is_if_string()  const noexcept{return m_kind == kind::if_string;}
  bool  is_block()      const noexcept{return m_kind == kind::block;}
  bool  is_control()    const noexcept{return m_kind == kind::control;}
  bool  is_expression() const noexcept{return m_kind == kind::expression;}

  const return_statement&      get_return()     const noexcept{return *static_cast<return_statement*>(m_pointer);}
  const label_statement&       get_label()      const noexcept{return *static_cast<label_statement*>(m_pointer);}
  const jump_statement&        get_jump()       const noexcept{return *static_cast<jump_statement*>(m_pointer);}
  const if_string_statement&   get_if_string()  const noexcept{return *static_cast<if_string_statement*>(m_pointer);}
  const block_statement&       get_block()      const noexcept{return *static_cast<block_statement*>(m_pointer);}
  const control_statement&     get_control()    const noexcept{return *static_cast<control_statement*>(m_pointer);}
  const expression_statement&  get_expression() const noexcept{return *static_cast<expression_statement*>(m_pointer);}

  void  print(const context*  ctx=nullptr, const function*  fn=nullptr) const noexcept;

};


namespace statements{
extern const statement  null;
}


class
return_statement
{
  expression  m_expression;

public:
  return_statement() noexcept{}
  return_statement(expression&&  e) noexcept: m_expression(std::move(e)){}

  const expression&  get_expression() const noexcept{return m_expression;}

  void  print() const noexcept{  printf("return ");  m_expression.print();}

};


class
label_statement
{
  std::string  m_string;

public:
  label_statement() noexcept{}
  label_statement(std::string_view  sv) noexcept: m_string(sv){}

  const std::string&  get_string() const noexcept{return m_string;}

  void  print() const noexcept{printf("label:%s",m_string.data());}

};


class
jump_statement
{
  std::string  m_string;

public:
  jump_statement() noexcept{}
  jump_statement(std::string_view  sv) noexcept: m_string(sv){}

  const std::string&  get_string() const noexcept{return m_string;}

  void  print() const noexcept{printf("jump %s",m_string.data());}

};


class
block_statement
{
  address_t  m_base_address;

  declaration_list  m_declaration_list;

  std::vector<std::unique_ptr<statement>>  m_statement_list;

public:
  block_statement(address_t  addr=0) noexcept: m_base_address(addr){}

  const statement&  operator[](int  i) const noexcept
  {return (i < m_statement_list.size())? *m_statement_list[i]:statements::null;}

  void       set_base_address(address_t  v)       noexcept{       m_base_address = v;}
  address_t  get_base_address(            ) const noexcept{return m_base_address    ;}

  void  push(statement&&  st) noexcept;

  const declaration*  find_declaration(std::string_view  name) const noexcept;

  const declaration_list&  get_declaration_list() const noexcept{return m_declaration_list;}

  void  print() const noexcept{}

};


class
control_statement
{
  std::string  m_string;

public:
  control_statement(std::string_view  sv) noexcept: m_string(sv){}

  bool  operator==(std::string_view  sv) const noexcept{return m_string == sv;}

  const std::string&  get_string() const noexcept{return m_string;}

  void  print() const noexcept{printf("%s",m_string.data());}

};


class
condition_statement
{
  expression  m_expression;

  statement  m_statement;

public:
  const expression&  get_expression() const noexcept{return m_expression;}

  const statement&  get_statement() const noexcept{return m_statement;}

};


class
if_string_statement
{
  std::vector<condition_statement>  m_if_list;

  statement  m_else_statement;

public:
  const statement&  get_else_statement() const noexcept{return m_else_statement;}

  void  print() const noexcept{}

};


class
expression_statement
{
  const type_info*  m_type_info=nullptr;

  std::string  m_name;

  expression  m_expression;

public:
  expression_statement(                                  std::string_view  sv, expression&&  e) noexcept: m_name(sv), m_expression(std::move(e)){}
  expression_statement(const typesystem::type_info&  ti, std::string_view  sv, expression&&  e) noexcept: m_type_info(&ti), m_name(sv), m_expression(std::move(e)){}

  const type_info*  get_type_info() const noexcept{return m_type_info;}

  const std::string&  get_name() const noexcept{return m_name;}

  const expression&  get_expression() const noexcept{return m_expression;}

  void  print() const noexcept{  printf("%s = ",m_name.data());  m_expression.print();}

};


class
statement_cursor
{
  struct frame{
    const block_statement&  m_block;

    int  m_counter=0;

    frame(const block_statement&  st) noexcept: m_block(st){}

  };

  std::vector<frame>  m_frame_stack;

public:
  statement_cursor() noexcept{}
  statement_cursor(const block_statement&  st) noexcept{assign(st);}

  operator bool() const noexcept{return m_frame_stack.size();}

  statement_cursor&  assign(const block_statement&  st) noexcept;

  statement_cursor&       push(const block_statement&  st) noexcept;
  const block_statement&  pop() noexcept;

  void  advance() noexcept;

  const statement&  get() const noexcept;

};


class
function
{
  const type_info&  m_type_info;

  std::string  m_name;

  std::vector<std::string>  m_argument_name_list;

  block_statement  m_block;

  bool  m_finalized=false;

//  void  resolve(operand&  o) const noexcept;

public:
  function(const type_info&  ti, std::string_view  name) noexcept:
  m_type_info(ti), m_name(name){}

  const std::string&  get_name() const noexcept{return m_name;}

  const type_info&  get_type_info() const noexcept{return m_type_info;}

  function&  set_argument_name_list(std::vector<std::string_view>&&  argnams) noexcept;

  const std::vector<std::string>&  get_argument_name_list() const noexcept{return m_argument_name_list;}

        block_statement&  get_block()       noexcept{return m_block;}
  const block_statement&  get_block() const noexcept{return m_block;}

  void  make_definition(const token_block&  blk) noexcept;
  void  make_definition(std::string_view  sv) noexcept;

  void  finalize() noexcept;

  void  print(const context&  ctx) const noexcept;

};




}




#endif




