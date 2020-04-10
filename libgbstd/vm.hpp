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


namespace type_infos{
constexpr int  boolean_size = sizeof(boolean_t);
constexpr int  pointer_size = sizeof(address_t);
constexpr int  function_pointer_size = sizeof(function*);
constexpr int     enum_size = 4;

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
  cold_object( int16_t  i) noexcept;
  cold_object( int32_t  i) noexcept;
  cold_object( int64_t  i) noexcept;
  cold_object(uint64_t  u) noexcept;
  cold_object(float   f) noexcept;
  cold_object(double  f) noexcept;
  cold_object(nullptr_t  ptr) noexcept;
  cold_object(const function&  fn) noexcept;
  cold_object(std::string_view  sv) noexcept;

  operator bool() const noexcept{return m_type_info;}

        memory&  get_memory()       noexcept{return m_memory;}
  const memory&  get_memory() const noexcept{return m_memory;}

  const type_info&  get_type_info() const noexcept{return *m_type_info;}

   int64_t  get_integer() const noexcept;

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
tepid_object   dereference(cold_object  o, const memory&  home_mem) noexcept;
cold_object      postfix_increment(hot_object  o) noexcept;
cold_object      postfix_decrement(hot_object  o) noexcept;

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

tepid_object  assign(hot_object  lo, cold_object  ro) noexcept;

cold_object           eq(cold_object  lo, cold_object  ro) noexcept;
cold_object          neq(cold_object  lo, cold_object  ro) noexcept;
cold_object           lt(cold_object  lo, cold_object  ro) noexcept;
cold_object         lteq(cold_object  lo, cold_object  ro) noexcept;
cold_object           gt(cold_object  lo, cold_object  ro) noexcept;
cold_object         gteq(cold_object  lo, cold_object  ro) noexcept;
cold_object   logical_or(cold_object  lo, cold_object  ro) noexcept;
cold_object  logical_and(cold_object  lo, cold_object  ro) noexcept;

tepid_object  subscript(cold_object  lo, cold_object  ro, const memory&  mem) noexcept;
tepid_object      arrow(cold_object  lo, const expression&  r, const memory&  mem) noexcept;
tepid_object        dot( hot_object  lo, const expression&  r) noexcept;
tepid_object     invoke(cold_object  lo, const expression&  r) noexcept;


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
prefix_unary_operation
{
protected:
  operator_code  m_opcode;

  std::unique_ptr<expression>  m_operand;

public:
  prefix_unary_operation(operator_code  op, std::unique_ptr<expression>&&  o) noexcept:
  m_opcode(op), m_operand(std::move(o)){}

  prefix_unary_operation(const prefix_unary_operation&  rhs) noexcept{assign(rhs);}

  prefix_unary_operation&  operator=(const prefix_unary_operation&  rhs) noexcept{return assign(rhs);}
  prefix_unary_operation&  assign(const prefix_unary_operation&  rhs) noexcept;

  operator_code  get_opcode() const noexcept{return m_opcode;}

        expression&  get_operand()       noexcept{return *m_operand;}
  const expression&  get_operand() const noexcept{return *m_operand;}

  type_info  get_type_info(const block_statement*  blkst) const noexcept;

};


class
postfix_unary_operation: public prefix_unary_operation
{
public:
  using prefix_unary_operation::prefix_unary_operation;

  type_info  get_type_info(const block_statement*  blkst) const noexcept;

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

        expression&  get_left()       noexcept{return *m_left;}
  const expression&  get_left() const noexcept{return *m_left;}

        expression&  get_right()       noexcept{return *m_right;}
  const expression&  get_right() const noexcept{return *m_right;}

  type_info  get_type_info(const block_statement*  blkst) const noexcept;

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
    int64_t      i;

    double  f;

     prefix_unary_operation   pre_un;
    postfix_unary_operation   pos_un;

    binary_operation  bin;

  data() noexcept{}
 ~data(){}

  } m_data;

  address_t  m_begin_address=0;
  address_t  m_end_address=0;

public:
  expression() noexcept{}
  expression(const expression&   rhs) noexcept{assign(rhs);}
  expression(      expression&&  rhs) noexcept{assign(std::move(rhs));}
  expression(undefined  u) noexcept{assign(u);}
  expression(nullptr_t  n) noexcept{assign(n);}
  expression(bool  b) noexcept{assign(b);}
  expression(identifier&&  id) noexcept{assign(std::move(id));}
  expression(std::string_view  sv) noexcept{assign(sv);}
  expression(int64_t  i) noexcept{assign(i);}
  expression(double  f) noexcept{assign(f);}
  expression(prefix_unary_operation&&  op) noexcept{assign(std::move(op));}
  expression(postfix_unary_operation&&  op) noexcept{assign(std::move(op));}
  expression(binary_operation&&  op) noexcept{assign(std::move(op));}
 ~expression(){clear();}

  expression&  operator=(const expression&   rhs) noexcept{return assign(rhs);}
  expression&  operator=(      expression&&  rhs) noexcept{return assign(std::move(rhs));}
  expression&  operator=(undefined  u) noexcept{return assign(u);}
  expression&  operator=(nullptr_t  n) noexcept{return assign(n);}
  expression&  operator=(bool  b) noexcept{return assign(b);}
  expression&  operator=(identifier&&  id) noexcept{return assign(std::move(id));}
  expression&  operator=(std::string_view  sv) noexcept{return assign(sv);}
  expression&  operator=(int64_t  i) noexcept{return assign(i);}
  expression&  operator=(double  f) noexcept{return assign(f);}
  expression&  operator=(prefix_unary_operation&&  op) noexcept{return assign(std::move(op));}
  expression&  operator=(postfix_unary_operation&&  op) noexcept{return assign(std::move(op));}
  expression&  operator=(binary_operation&&  op) noexcept{return assign(std::move(op));}

  operator bool() const noexcept{return m_kind != kind::null;}

  expression&  assign(const expression&   rhs) noexcept;
  expression&  assign(      expression&&  rhs) noexcept;
  expression&  assign(undefined  u) noexcept;
  expression&  assign(nullptr_t  n) noexcept;
  expression&  assign(bool  b) noexcept;
  expression&  assign(identifier&&  id) noexcept;
  expression&  assign(std::string_view  sv) noexcept;
  expression&  assign(int64_t  i) noexcept;
  expression&  assign(double  f) noexcept;
  expression&  assign(prefix_unary_operation&&  op) noexcept;
  expression&  assign(postfix_unary_operation&&  op) noexcept;
  expression&  assign(binary_operation&&  op) noexcept;

  expression&  clear() noexcept;

  address_t  get_begin_address() const noexcept{return m_begin_address;}
  address_t  get_end_address()   const noexcept{return m_end_address;}

  const std::string&  get_string() const noexcept{return m_data.s;}

  bool      get_boolean() const noexcept{return m_data.b;}
  int64_t   get_integer() const noexcept{return m_data.i;}
  double    get_fpn()     const noexcept{return m_data.f;}

  const prefix_unary_operation&   get_prefix_unary_operation() const noexcept{return m_data.pre_un;}
  const postfix_unary_operation&  get_postfix_unary_operation() const noexcept{return m_data.pos_un;}
  const binary_operation&  get_binary_operation() const noexcept{return m_data.bin;}

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

  address_t  allocate_address() noexcept;

  type_info  get_type_info(const block_statement*  blkst) const noexcept;

  std::vector<const expression*>  get_argument_source_list() const noexcept;

  void  print() const noexcept;

};


expression  make_expression(token_iterator&  it) noexcept;
expression  make_expression(std::string_view  sv) noexcept;




class
parameter
{
  type_info  m_type_info;

  std::string  m_name;

public:
  parameter() noexcept{}
  parameter(const type_info&   ti, std::string_view  name) noexcept:
  m_type_info(ti), m_name(name){}

  const type_info&  get_type_info() const noexcept{return m_type_info;}

  const std::string&  get_name() const noexcept{return m_name;}

};


using parameter_list = std::vector<parameter>;


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


class global_space;
class  local_space;




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

};


class
global_space
{
  std::vector<std::unique_ptr<memo_info>>  m_memo_info_table;

  std::vector<std::unique_ptr<function>>  m_function_table;

  std::vector<std::unique_ptr<local_space>>  m_local_spaces;

public:
  local_space&  create_local_space() noexcept;

  function&  create_function(std::string_view  name, const type_info&  retti, parameter_list&&  parals) noexcept;

  const memo_info&  push(const type_info&  ti, std::string_view  name) noexcept;

};


class
local_space
{
  global_space&  m_global_space;

  std::vector<std::unique_ptr<memo_info>>  m_parameter_memo_info_table;
  std::vector<std::unique_ptr<memo_info>>   m_domestic_memo_info_table;

public:
  local_space(global_space&  gs) noexcept: m_global_space(gs){}

  global_space&  get_global_space() const noexcept{return m_global_space;}

  const memo_info&  push_parameter(const type_info&  ti, std::string_view  name) noexcept;
  const memo_info&  push_domestic( const type_info&  ti, std::string_view  name) noexcept;

};




class statement;


class
return_statement
{
  expression  m_expression;

public:
  return_statement() noexcept{}
  return_statement(expression&&  e) noexcept: m_expression(std::move(e)){}

        expression&  get_expression()       noexcept{return m_expression;}
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
  const block_statement*  m_parent=nullptr;

  const function*  m_function=nullptr;

  pointer_wrapper<global_space>  m_global_space;

  std::vector<type_info>  m_type_info_table;

  std::vector<const memo_info*>  m_memo_info_table;

  std::vector<std::unique_ptr<function>>  m_function_table;

  std::vector<statement>  m_statement_list;

  parameter_list  read_parameter_list(token_iterator&  it) noexcept;

  statement    read_return(token_iterator&  it) noexcept;
  statement    read_jump(token_iterator&  it) noexcept;
  statement    read_label(token_iterator&  it) noexcept;
  statement    read_if(token_iterator&  it) noexcept;
  statement    read_for(token_iterator&  it) noexcept;
  statement    read_while(token_iterator&  it) noexcept;
  statement    read_switch(token_iterator&  it) noexcept;
  statement    read_let(token_iterator&  it) noexcept;

  type_info   read_struct_type_info(token_iterator&  it) noexcept;
  type_info    read_union_type_info(token_iterator&  it) noexcept;
  type_info     read_enum_type_info(token_iterator&  it) noexcept;
  type_info  read_derived_type_info(token_iterator&  it) noexcept;

  type_info                    read_alias(token_iterator&  it) noexcept;
  type_info   read_named_struct_type_info(token_iterator&  it) noexcept;
  type_info    read_named_union_type_info(token_iterator&  it) noexcept;
  type_info     read_named_enum_type_info(token_iterator&  it) noexcept;

  type_info  read_user_defined_type_info(token_iterator&  it) noexcept;

  const function*  read_function(token_iterator&  it) noexcept;

public:
  block_statement() noexcept{}
  block_statement(global_space&  gsp, token_iterator&  it) noexcept: m_global_space(&gsp){read(it);}
  block_statement(const function&  fn, token_iterator&  it) noexcept;
  block_statement(const block_statement*  p, token_iterator&  it) noexcept{read(it);}

  const statement&  operator[](int  i) const noexcept;

  void  set_parent(const block_statement*  p) noexcept;

  const function*  get_function() const noexcept{return m_function;}

  const function*  find_function(std::string_view  name) const noexcept;

  type_info  find_type_info_by_name(std::string_view  name) const noexcept;
  type_info  find_type_info_by_id(  std::string_view  name) const noexcept;

  void  clear() noexcept;

  void  push(statement&&  st) noexcept;

  const std::vector<statement>&  get_statement_list() const noexcept{return m_statement_list;}

  block_statement&  read(token_iterator&  it) noexcept;
  block_statement&  read(std::string_view  sv) noexcept;

  type_info  create_type_from_string(std::string_view  sv) noexcept;

  void  print() const noexcept;

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
let_statement
{
  std::string  m_target_name;

  expression  m_expression;

public:
  let_statement(std::string_view  sv, expression&&  e) noexcept:
  m_target_name(sv), m_expression(std::move(e)){}

  const std::string&  get_target_name() const noexcept{return m_target_name;}

        expression&  get_expression()       noexcept{return m_expression;}
  const expression&  get_expression() const noexcept{return m_expression;}

  void  print() const noexcept{
    printf("let %s",m_target_name.data());
      if(m_expression){  printf(" = (");  m_expression.print();  printf(")");}
  }

};


class
condition_statement
{
  expression  m_expression;

  block_statement  m_block;

public:
  const expression&  get_expression() const noexcept{return m_expression;}

  const block_statement&  get_block() const noexcept{return m_block;}

};


class
if_string_statement
{
  std::vector<condition_statement>  m_if_list;

  block_statement  m_else_block;

public:
  const block_statement&  get_else_block() const noexcept{return m_else_block;}

  void  print() const noexcept{}

};


class
statement
{
  const block_statement*  m_block=nullptr;

  enum class kind{
    null,
    return_,
    block,
    control,
    let,
    if_string,
    expression,
    jump,
    label,

  } m_kind=kind::null;

  union data{
    return_statement       ret;
    control_statement     ctrl;
    let_statement          let;
    if_string_statement    ifs;
    jump_statement         jmp;
    label_statement         lb;
    expression            expr;

    std::unique_ptr<block_statement>  blk;

   data() noexcept{}
  ~data(){}
  } m_data;

public:
  statement() noexcept{}
  statement(const statement&   rhs) noexcept=delete;//{assign(rhs);}
  statement(      statement&&  rhs) noexcept{assign(std::move(rhs));}
  statement(const block_statement*  blk, return_statement&&       st) noexcept{assign(blk,std::move(st));}
  statement(const block_statement*  blk,  label_statement&&       st) noexcept{assign(blk,std::move(st));}
  statement(const block_statement*  blk,  jump_statement&&        st) noexcept{assign(blk,std::move(st));}
  statement(const block_statement*  blk,  if_string_statement&&   st) noexcept{assign(blk,std::move(st));}
  statement(const block_statement*  blk,  std::unique_ptr<block_statement>&&       st) noexcept{assign(blk,std::move(st));}
  statement(const block_statement*  blk,  control_statement&&     st) noexcept{assign(blk,std::move(st));}
  statement(const block_statement*  blk,  let_statement&&         st) noexcept{assign(blk,std::move(st));}
  statement(const block_statement*  blk, expression&&  e) noexcept{assign(blk,std::move(e));}
 ~statement(){clear();}

  statement&  operator=(const statement&   rhs) noexcept=delete;//{return assign(rhs);}
  statement&  operator=(      statement&&  rhs) noexcept{return assign(std::move(rhs));}

  operator bool() const noexcept{return m_kind != kind::null;}

//  statement&  assign(const statement&   rhs) noexcept;
  statement&  assign(      statement&&  rhs) noexcept;
  statement&  assign(const block_statement*  blk, return_statement&&       st) noexcept;
  statement&  assign(const block_statement*  blk,  label_statement&&       st) noexcept;
  statement&  assign(const block_statement*  blk,  jump_statement&&        st) noexcept;
  statement&  assign(const block_statement*  blk,  if_string_statement&&   st) noexcept;
  statement&  assign(const block_statement*  blk,  std::unique_ptr<block_statement>&&       st) noexcept;
  statement&  assign(const block_statement*  blk,  control_statement&&     st) noexcept;
  statement&  assign(const block_statement*  blk,  let_statement&&         st) noexcept;
  statement&  assign(const block_statement*  blk,  expression&&  e) noexcept;

  void  clear() noexcept;

  bool  is_null()       const noexcept{return m_kind == kind::null;}
  bool  is_return()     const noexcept{return m_kind == kind::return_;}
  bool  is_label()      const noexcept{return m_kind == kind::label;;}
  bool  is_jump()       const noexcept{return m_kind == kind::jump;}
  bool  is_if_string()  const noexcept{return m_kind == kind::if_string;}
  bool  is_block()      const noexcept{return m_kind == kind::block;}
  bool  is_control()    const noexcept{return m_kind == kind::control;}
  bool  is_let()        const noexcept{return m_kind == kind::let;}
  bool  is_expression() const noexcept{return m_kind == kind::expression;}

  const return_statement&      get_return()     const noexcept{return m_data.ret;}
  const label_statement&       get_label()      const noexcept{return m_data.lb;}
  const jump_statement&        get_jump()       const noexcept{return m_data.jmp;}
  const if_string_statement&   get_if_string()  const noexcept{return m_data.ifs;}
  const block_statement&       get_block()      const noexcept{return *m_data.blk;}
  const control_statement&     get_control()    const noexcept{return m_data.ctrl;}
  const let_statement&         get_let()        const noexcept{return m_data.let;}
  const expression&            get_expression() const noexcept{return m_data.expr;}

  void  print(const context*  ctx=nullptr, const function*  fn=nullptr) const noexcept;

};




class
function
{
  pointer_wrapper<local_space>  m_local_space;

  std::unique_ptr<type_info>  m_pointer_type_info;

  std::string  m_name;

  const type_info*  m_return_type_info;

  block_statement  m_block;

public:
  function() noexcept{}
  function(local_space&  lsp, std::string_view  name, const type_info&  retti, parameter_list&&  parals) noexcept;

  global_space&  get_global_space() const noexcept{return m_local_space->get_global_space();}
  local_space&    get_local_space() const noexcept{return *m_local_space;}

  const type_info&  update_pointer_type_info() noexcept;

  const std::string&  get_name() const noexcept{return m_name;}

  const type_info&  get_pointer_type_info() const noexcept{return *m_pointer_type_info;}
  const type_info&   get_return_type_info() const noexcept{return *m_return_type_info;}

        block_statement&  get_block_statement()       noexcept{return m_block;}
  const block_statement&  get_block_statement() const noexcept{return m_block;}

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
  std::string  m_source;

  global_space  m_global_space;

  block_statement  m_block;

  address_t  m_sp=0;
  address_t  m_bp=0;

  std::vector<call_frame>  m_call_stack;

  memory  m_memory;

public:
  context() noexcept;
 ~context(){clear();}

  void  clear() noexcept;

  context&  assign(std::string_view  sv) noexcept;

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




