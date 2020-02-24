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




using typesystem::value;


class function;
class context;


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


class
major_address
{
  uint32_t  m_value;

public:
  constexpr major_address() noexcept: m_value(0){}
  constexpr major_address(uint32_t  v) noexcept: m_value(v){}
  constexpr major_address(int  k, uint32_t  v) noexcept: m_value((k<<24)|(v&0x00FFFFFF)){}

  constexpr operator bool() const noexcept{return m_value;}

  constexpr int  get_kind() const noexcept{return m_value>>24;}

  constexpr bool  is_function() const noexcept{return get_kind() == 'f';}
  constexpr bool  is_global()   const noexcept{return get_kind() == 'g';}
  constexpr bool  is_local()    const noexcept{return get_kind() == 'l';}
  constexpr bool  is_null()     const noexcept{return get_kind() == 0;}

  constexpr uint32_t  get_raw_value() const noexcept{return m_value;}

  constexpr uint32_t  get(             ) const noexcept{return m_value&0x00FFFFFF;}
  void                set(uint32_t  v=0) noexcept{m_value = (m_value&0xFF000000)|(v&0x00FFFFFF);}

  void  print() const noexcept{printf("major{%c %d}",get_kind(),get());}

};


class
minor_address
{
  uint32_t  m_value;

public:
  constexpr minor_address(uint32_t  v=0) noexcept: m_value(v){}

  constexpr operator bool() const noexcept{return m_value;}

  constexpr uint32_t  get(             ) const noexcept{return m_value;}
  void                set(uint32_t  v=0) noexcept{m_value = v;}

};


class
address
{
  std::string  m_name;

  uint32_t  m_value;

public:
  address(uint32_t  v=0) noexcept: m_value(v){}

  operator bool() const noexcept{return m_value;}

  constexpr uint32_t  get(             ) const noexcept{return m_value;}
  void                set(uint32_t  v=0) noexcept{m_value = v;}

};


class
multi_pointer
{
public:
  using value_type = uint64_t;

private:
  major_address  m_major;
  minor_address  m_minor;

public:
  constexpr multi_pointer(value_type  v=0) noexcept: m_major(v>>32), m_minor(v&0xFFFFFFFF){}
  constexpr multi_pointer(major_address  maj, minor_address  min) noexcept: m_major(maj), m_minor(min){}

  constexpr operator bool() const noexcept{return m_major;}

  constexpr major_address  get_major() const noexcept{return m_major;}
  constexpr minor_address  get_minor() const noexcept{return m_minor;}

  constexpr value_type  get_packed() const noexcept{return (static_cast<uint64_t>(m_major.get_raw_value())<<32)|m_minor.get();}

};


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


class
variable
{
  std::string  m_function_name;

  major_address  m_address;

  value  m_value;

  std::string  m_name;

public:
  variable(std::string_view  fn_name, major_address  addr, const typesystem::type_info&  ti, std::string_view  name) noexcept:
  m_function_name(fn_name), m_address(addr), m_value(ti), m_name(name){}

  variable(std::string_view  fn_name, major_address  addr, value&&  v, std::string_view  name) noexcept:
  m_function_name(fn_name), m_address(addr), m_value(std::move(v)), m_name(name){}

  const std::string&  get_function_name() const noexcept{return m_function_name;}

  major_address  get_address() const noexcept{return m_address;}

  const std::string&  get_name() const noexcept{return m_name;}

        value&  get_value()       noexcept{return m_value;}
  const value&  get_value() const noexcept{return m_value;}

  void  print() const noexcept;

};


class
context
{
  typesystem::type_collection  m_type_collection;

  std::vector<function>  m_function_table;
  std::vector<variable>  m_variable_table;

  struct frame;

  frame*  m_current_frame=nullptr;

  bool  m_finalized=false;

  variable&  push_variable(std::string_view  fn_name, const typesystem::type_info&  ti, std::string_view  name) noexcept;

  void  push_frame(major_address  st_p, const function&  fn, int  argc, const expression*  args) noexcept;
  void  pop_frame() noexcept;

  void  store(const value&  dst, const value&  src) noexcept;

  void  seek(variable&  dst, value&&  src, std::string_view  name) noexcept;

  int  get_base_index() const noexcept;

public:
  context() noexcept;

        typesystem::type_collection&  get_type_collection()       noexcept{return m_type_collection;}
  const typesystem::type_collection&  get_type_collection() const noexcept{return m_type_collection;}

  value  make_value(int64_t    i) noexcept{return value(*m_type_collection.find_by_name("int"),i);}
  value  make_value(uint64_t   u) noexcept{return value(*m_type_collection.find_by_name("uint"),u);}
  value  make_value(bool       b) noexcept{return value(*m_type_collection.find_by_name("bool"),static_cast<int64_t>(b));}
  value  make_value(nullptr_t  p) noexcept{return value(*m_type_collection.find_by_name("nullptr_t"));}

  function&  create_function(std::string_view  sig, std::string_view  name) noexcept;

        function*  find_function(std::string_view  name)       noexcept;
  const function*  find_function(std::string_view  name) const noexcept;

        variable*  find_variable(std::string_view  name)       noexcept;
  const variable*  find_variable(std::string_view  name) const noexcept;

        variable&  get_variable(major_address  addr)       noexcept{return m_variable_table[(addr.is_local()? get_base_index():0)+addr.get()];}
  const variable&  get_variable(major_address  addr) const noexcept{return m_variable_table[(addr.is_local()? get_base_index():0)+addr.get()];}

        function&  get_function(major_address  addr)       noexcept{return m_function_table[addr.get()];}
  const function&  get_function(major_address  addr) const noexcept{return m_function_table[addr.get()];}

  const typesystem::type_info&  get_void_type_info() const noexcept{return *m_type_collection.find_by_name("void");}

  value  get_value(multi_pointer  p) const noexcept;

  value  dereference(const value&  v) noexcept;

  context&  entry(std::string_view  fn_name) noexcept{  push_frame(0,*find_function(fn_name),0,nullptr);  return *this;}

  bool  step() noexcept;
  void   run() noexcept;

  context&  finalize() noexcept;

  void  print() const noexcept;
  void  print_debug() const noexcept;

};




class statement;


class
return_statement
{
  expression  m_expression;

public:
  void  print() const noexcept{}

};


class
label_statement
{
  std::string  m_string;

public:
  label_statement() noexcept{}
  label_statement(std::string_view  sv) noexcept: m_string(sv){}

  const std::string&  get_string() const noexcept{return m_string;}

  void  print() const noexcept{}

};


class
jump_statement
{
  std::string  m_string;

public:
  jump_statement() noexcept{}
  jump_statement(std::string_view  sv) noexcept: m_string(sv){}

  const std::string&  get_string() const noexcept{return m_string;}

  void  print() const noexcept{}

};


class
block_statement
{
  std::vector<std::unique_ptr<statement>>  m_statement_list;

public:
  void  print() const noexcept{}

};


class
condition_statement
{
  expression  m_expression;

//  std::unique_ptr<statement>  m_statement;

public:

};


class
if_string_statement
{
  std::vector<condition_statement>  m_if_list;

//  std::unique_ptr<statement>  m_else_statement;

public:
  void  print() const noexcept{}

};


class
expression_statement
{
  std::string  m_name;

  expression  m_expression;

public:
  expression_statement(std::string_view  sv, expression&&  e) noexcept: m_name(sv), m_expression(std::move(e)){}

  const std::string&  get_name() const noexcept{return m_name;}

  const expression&  get_expression() const noexcept{return m_expression;}

  void  print() const noexcept{}

};


class
statement
{
  function*  m_function=nullptr;

  enum class kind{
    null,
    return_,
    block,
    if_string,
    expression,
    jump,
    label,

  } m_kind=kind::null;

  union data{
    return_statement       ret;
    block_statement        blk;
    if_string_statement    ifs;
    expression_statement  expr;
    jump_statement         jmp;
    label_statement         lb;

  data() noexcept{}
 ~data(){}
  } m_data;

public:
  statement() noexcept{}
  statement(const statement&   rhs) noexcept=delete;
  statement(      statement&&  rhs) noexcept{assign(std::move(rhs));}
  statement(function&  fn, return_statement&&      st) noexcept: m_function(&fn){assign(std::move(st));}
  statement(function&  fn, label_statement&&       st) noexcept: m_function(&fn){assign(std::move(st));}
  statement(function&  fn, jump_statement&&        st) noexcept: m_function(&fn){assign(std::move(st));}
  statement(function&  fn, if_string_statement&&   st) noexcept: m_function(&fn){assign(std::move(st));}
  statement(function&  fn, block_statement&&       st) noexcept: m_function(&fn){assign(std::move(st));}
  statement(function&  fn, expression_statement&&  st) noexcept: m_function(&fn){assign(std::move(st));}
 ~statement(){clear();}

  statement&  operator=(const statement&   rhs) noexcept=delete;
  statement&  operator=(      statement&&  rhs) noexcept{return assign(std::move(rhs));}

  statement&  assign(statement&&  rhs) noexcept;
  statement&  assign(return_statement&&       st) noexcept;
  statement&  assign( label_statement&&       st) noexcept;
  statement&  assign( jump_statement&&        st) noexcept;
  statement&  assign( if_string_statement&&   st) noexcept;
  statement&  assign( block_statement&&       st) noexcept;
  statement&  assign( expression_statement&&  st) noexcept;

  void  clear() noexcept;

  function&  get_function() const noexcept{return *m_function;}

  bool  is_null()       const noexcept{return m_kind == kind::null;}
  bool  is_return()     const noexcept{return m_kind == kind::return_;}
  bool  is_label()      const noexcept{return m_kind == kind::label;;}
  bool  is_jump()       const noexcept{return m_kind == kind::jump;}
  bool  is_if_string()  const noexcept{return m_kind == kind::if_string;}
  bool  is_block()      const noexcept{return m_kind == kind::block;}
  bool  is_expression() const noexcept{return m_kind == kind::expression;}

  const return_statement&      get_return()     const noexcept{return m_data.ret;}
  const label_statement&       get_label()      const noexcept{return m_data.lb;;}
  const jump_statement&        get_jump()       const noexcept{return m_data.jmp;}
  const if_string_statement&   get_if_string()  const noexcept{return m_data.ifs;}
  const block_statement&       get_block()      const noexcept{return m_data.blk;}
  const expression_statement&  get_expression() const noexcept{return m_data.expr;}

  void  print(const context*  ctx=nullptr, const function*  fn=nullptr) const noexcept;

};




class
declaration
{
  const typesystem::type_info*  m_type_info=nullptr;

  std::string  m_name;

public:
  declaration(                                  std::string_view  nam="") noexcept:                   m_name(nam){}
  declaration(const typesystem::type_info&  ti, std::string_view  nam   ) noexcept: m_type_info(&ti), m_name(nam){}

  const typesystem::type_info*  get_type_info() const noexcept{return m_type_info;}

  const std::string&  get_name() const noexcept{return m_name;}

};


using declaration_list = std::vector<declaration>;


class
function
{
  context&  m_context;

  major_address  m_address;

  const typesystem::type_info&  m_type_info;

  std::string  m_name;

  std::vector<std::string>  m_argument_name_list;
  std::vector<declaration>    m_declaration_list;

  block_statement  m_block;

  bool  m_finalized=false;

//  void  resolve(operand&  o) const noexcept;

public:
  function(context&  ctx, major_address  addr, const typesystem::type_info&  ti, std::string_view  name) noexcept:
  m_context(ctx), m_address(addr), m_type_info(ti), m_name(name){}

  major_address  get_address() const noexcept{return m_address;}

  const std::string&  get_name() const noexcept{return m_name;}

  const typesystem::type_info&  get_type_info() const noexcept{return m_type_info;}

  function&  set_argument_name_list(std::vector<std::string_view>&&  argnams) noexcept;

  const declaration_list&            get_declaration_list() const noexcept{return m_declaration_list;}
  const std::vector<std::string>&  get_argument_name_list() const noexcept{return m_argument_name_list;}

  const block_statement&  get_block() const noexcept{return m_block;}

  function&  append_declaration(std::string_view  type_name, std::string_view  var_name) noexcept;

  int  get_number_of_variables() const noexcept{return m_declaration_list.size()+m_argument_name_list.size();}

  void  finalize() noexcept;

  value  get_value() const noexcept;

  void  print() const noexcept;

};




}




#endif




