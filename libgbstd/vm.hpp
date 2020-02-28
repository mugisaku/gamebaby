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
  memory_sharer(const void*  ptr) noexcept{assign(ptr);}
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
  memory_sharer&  assign(const void*  ptr) noexcept;
  memory_sharer&  assign(const memory_sharer&  base, int  offset) noexcept;
  memory_sharer&  assign(const memory_sharer&  base, int  offset, int  length) noexcept;

  template<typename  T>
  T*  get_pointer() const noexcept{return &reinterpret_cast<T&>((*this)[m_offset]);}

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
  const type_info*  m_type_info=&typesystem::type_infos::undefined;

  memory_sharer  m_memory;

public:
  constexpr value() noexcept{}
  value(const void*  ptr) noexcept{assign(ptr);}
  value(const type_info&  ti) noexcept{assign(ti);}
  value(const type_info&  ti,  int64_t  i) noexcept{assign(ti,i);}
  value(const type_info&  ti, uint64_t  u) noexcept{assign(ti,u);}

  constexpr operator bool() const noexcept{return m_type_info;}

  value&  assign(const void*  ptr) noexcept;
  value&  assign(const type_info&  ti             ) noexcept;
  value&  assign(const type_info&  ti,  int64_t  i) noexcept;
  value&  assign(const type_info&  ti, uint64_t  u) noexcept{return assign(ti,static_cast<int64_t>(u));}

  const type_info&  get_type_info() const noexcept{return *m_type_info;}

  typesystem::type_derivation&  get_type_derivation() const noexcept{return m_type_info->get_derivation();}

        memory_sharer&  get_memory()       noexcept{return m_memory;}
  const memory_sharer&  get_memory() const noexcept{return m_memory;}

  int64_t   get_integer()          const noexcept;
  uint64_t  get_unsigned_integer() const noexcept;

  void  set_integer(          int64_t  i) const noexcept;
  void  set_unsigned_integer(uint64_t  u) const noexcept;

  value  get_element(int  i) const noexcept;
  value  get_member(std::string_view  name) const noexcept;

  void  print() const noexcept;

};




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
  bool  is_string()               const noexcept{return m_kind == kind::string_literal;}
  bool  is_integer_literal()      const noexcept{return m_kind == kind::integer_literal;}
  bool  is_fpn_literal()          const noexcept{return m_kind == kind::fpn_literal;}
  bool  is_prefix_unary()  const noexcept{return m_kind == kind::prefix_unary;}
  bool  is_postfix_unary() const noexcept{return m_kind == kind::postfix_unary;}
  bool  is_binary()        const noexcept{return m_kind == kind::binary;}

  value  evaluate(context&  ctx) const noexcept;

  void  print() const noexcept;

};


expression  make_expression(std::string_view  sv) noexcept;


namespace unary_operations{
value      bit_not(context&  ctx, const value&  v) noexcept;
value  logical_not(context&  ctx, const value&  v) noexcept;
value          neg(context&  ctx, const value&  v) noexcept;
value         size(context&  ctx, const value&  v) noexcept;
value      address(context&  ctx, const value&  v) noexcept;
value      dereference(context&  ctx, const value&  v) noexcept;
value      prefix_increment(context&  ctx, const value&  v) noexcept;
value      prefix_decrement(context&  ctx, const value&  v) noexcept;
value      postfix_increment(context&  ctx, const value&  v) noexcept;
value      postfix_decrement(context&  ctx, const value&  v) noexcept;
}


namespace binary_operations{
value          add(context&  ctx, const value&  lv, const value&  rv) noexcept;
value          add_assign(context&  ctx, const value&  lv, const value&  rv) noexcept;
value          sub(context&  ctx, const value&  lv, const value&  rv) noexcept;
value          sub_assign(context&  ctx, const value&  lv, const value&  rv) noexcept;
value          mul(context&  ctx, const value&  lv, const value&  rv) noexcept;
value          mul_assign(context&  ctx, const value&  lv, const value&  rv) noexcept;
value          div(context&  ctx, const value&  lv, const value&  rv) noexcept;
value          div_assign(context&  ctx, const value&  lv, const value&  rv) noexcept;
value          rem(context&  ctx, const value&  lv, const value&  rv) noexcept;
value          rem_assign(context&  ctx, const value&  lv, const value&  rv) noexcept;
value          shl(context&  ctx, const value&  lv, const value&  rv) noexcept;
value          shl_assign(context&  ctx, const value&  lv, const value&  rv) noexcept;
value          shr(context&  ctx, const value&  lv, const value&  rv) noexcept;
value          shr_assign(context&  ctx, const value&  lv, const value&  rv) noexcept;
value           eq(context&  ctx, const value&  lv, const value&  rv) noexcept;
value          neq(context&  ctx, const value&  lv, const value&  rv) noexcept;
value           lt(context&  ctx, const value&  lv, const value&  rv) noexcept;
value         lteq(context&  ctx, const value&  lv, const value&  rv) noexcept;
value           gt(context&  ctx, const value&  lv, const value&  rv) noexcept;
value         gteq(context&  ctx, const value&  lv, const value&  rv) noexcept;
value   logical_or(context&  ctx, const value&  lv, const value&  rv) noexcept;
value  logical_and(context&  ctx, const value&  lv, const value&  rv) noexcept;
value       bit_or(context&  ctx, const value&  lv, const value&  rv) noexcept;
value       bit_or_assign(context&  ctx, const value&  lv, const value&  rv) noexcept;
value      bit_and(context&  ctx, const value&  lv, const value&  rv) noexcept;
value      bit_and_assign(context&  ctx, const value&  lv, const value&  rv) noexcept;
value      bit_xor(context&  ctx, const value&  lv, const value&  rv) noexcept;
value      bit_xor_assign(context&  ctx, const value&  lv, const value&  rv) noexcept;
value      comma(context&  ctx, const value&  lv, const value&  rv) noexcept;
value      dot(context&  ctx, const value&  lv, const value&  rv) noexcept;
value      arrow(context&  ctx, const value&  lv, const value&  rv) noexcept;
value      scope_resolution(context&  ctx, const value&  lv, const value&  rv) noexcept;
value      subscript(context&  ctx, const value&  lv, const value&  rv) noexcept;
value      invoke(context&  ctx, const value&  lv, const value&  rv) noexcept;
value      assign(context&  ctx, const value&  lv, const value&  rv) noexcept;
}




using address_t = uint32_t;


class
declaration
{
  const type_info*  m_type_info=nullptr;

  std::string  m_name;

public:
  declaration(                      std::string_view  nam="") noexcept:                   m_name(nam){}
  declaration(const type_info&  ti, std::string_view  nam   ) noexcept: m_type_info(&ti), m_name(nam){}

  const type_info*  get_type_info() const noexcept{return m_type_info;}

  const std::string&  get_name() const noexcept{return m_name;}

  uint32_t  get_size()  const noexcept{return m_type_info? m_type_info->get_size() :0;}
  uint32_t  get_align() const noexcept{return m_type_info? m_type_info->get_align():0;}

};


class
symbol: public declaration
{
  int  m_id;

  address_t  m_address;

public:
  symbol(declaration&&  decl, int  id, address_t  addr) noexcept: declaration(std::move(decl)), m_id(id), m_address(addr){}

  int  get_id() const noexcept{return m_id;}

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

  symbol&  push(const declaration&  decl, int  id) noexcept;

  void  pop() noexcept;

        symbol*  find(std::string_view  name)       noexcept;
  const symbol*  find(std::string_view  name) const noexcept;

  int  get_number_of_symbols() const noexcept{return m_content.size();}

  bool  reallocate() noexcept;

  const symbol*  begin() const noexcept{return m_content.data();}
  const symbol*    end() const noexcept{return m_content.data()+m_content.size();}

};




class
memory
{
  uint8_t*  m_content;

  uint32_t  m_size=0;

public:
  memory(uint32_t  sz=0) noexcept: m_content(new uint8_t[sz]), m_size(sz){}
  memory(const memory& ) noexcept=delete;
  memory(      memory&&) noexcept=delete;
 ~memory(){delete[] m_content;}

  memory&  operator=(const memory& ) noexcept=delete;
  memory&  operator=(      memory&&) noexcept=delete;

  uint8_t&  operator[](int  i) const noexcept{return m_content[i];}

};


namespace type_infos{
constexpr int  boolean_size = 1;
constexpr int  pointer_size = sizeof(address_t);
constexpr int     enum_size = 4;

using typesystem::type_infos::s8;
using typesystem::type_infos::u8;
using typesystem::type_infos::s16;
using typesystem::type_infos::u16;
using typesystem::type_infos::s32;
using typesystem::type_infos::u32;
using typesystem::type_infos::s64;
using typesystem::type_infos::u64;
using typesystem::type_infos::f32;
using typesystem::type_infos::f64;
using typesystem::type_infos::undefined;
using typesystem::type_infos::null_pointer;
using typesystem::type_infos::void_;
using typesystem::type_infos::null;

extern const type_info          boolean;
extern const type_info  generic_pointer;
}


class
context
{
  type_collection  m_type_collection;

  std::vector<std::unique_ptr<type_info>>  m_type_info_table;

  symbol_table  m_global_symbol_table;

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

  value  make_value(int64_t    i) noexcept{return value(*m_type_collection.find_by_name("int"),i);}
  value  make_value(uint64_t   u) noexcept{return value(*m_type_collection.find_by_name("uint"),u);}
  value  make_value(bool       b) noexcept{return value(*m_type_collection.find_by_name("bool"),static_cast<int64_t>(b));}
  value  make_value(nullptr_t  p) noexcept{return value(*m_type_collection.find_by_name("nullptr_t"));}
  value  make_pointer(uint32_t  i) noexcept{return value(*m_type_collection.find_by_name("nullptr_t"));}

  void  append_function(const function&  fn) noexcept;

  const type_info&  append_type_info(std::unique_ptr<type_info>&&  ti) noexcept;

        symbol_table&  get_global_symbol_table()       noexcept{return m_global_symbol_table;}
  const symbol_table&  get_global_symbol_table() const noexcept{return m_global_symbol_table;}

  const symbol*  find_symbol(std::string_view  name) const noexcept;

  address_t  get_bp() const noexcept;

  value  dereference(const value&  v) noexcept;

  bool  step() noexcept;
  void   run() noexcept;

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




