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


namespace gbstd{




using typesystem::value;


class codeline;
class operand;
class operation;
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
variable_pointer
{
  uint32_t  m_variable_index;
  uint32_t  m_byte_offset;

public:
  constexpr variable_pointer(uint64_t  packed=0) noexcept:
  m_variable_index(packed>>32), m_byte_offset(packed&0xFFFFFFFF){}

  constexpr variable_pointer(uint32_t  vi, bool  loc, uint32_t  off=0) noexcept:
  m_variable_index((vi<<1)|(loc?1:0)), m_byte_offset(off){}

  constexpr operator bool() const noexcept{return m_variable_index>>1;}

  constexpr bool  operator==(const variable_pointer&  rhs) const noexcept
  {return (m_variable_index == rhs.m_variable_index) && (m_byte_offset == rhs.m_byte_offset);}

  constexpr bool  operator!=(const variable_pointer&  rhs) const noexcept
  {return (m_variable_index != rhs.m_variable_index) || (m_byte_offset != rhs.m_byte_offset);}

  constexpr bool  operator<(const variable_pointer&  rhs) const noexcept
  {return (m_variable_index <= rhs.m_variable_index) && (m_byte_offset < rhs.m_byte_offset);}

  constexpr bool  operator<=(const variable_pointer&  rhs) const noexcept
  {return (m_variable_index <= rhs.m_variable_index) && (m_byte_offset <= rhs.m_byte_offset);}

  constexpr bool  operator>(const variable_pointer&  rhs) const noexcept
  {return (m_variable_index >= rhs.m_variable_index) && (m_byte_offset > rhs.m_byte_offset);}

  constexpr bool  operator>=(const variable_pointer&  rhs) const noexcept
  {return (m_variable_index >= rhs.m_variable_index) && (m_byte_offset >= rhs.m_byte_offset);}


  constexpr uint32_t  get_variable_index() const noexcept{return m_variable_index>>1;}
  constexpr uint32_t  get_byte_offset() const noexcept{return m_byte_offset;}

  constexpr bool  is_local() const noexcept{return m_variable_index&1;}

  constexpr uint64_t  get_packed() const noexcept{return (static_cast<uint64_t>(m_variable_index)<<32)|m_byte_offset;}

  constexpr variable_pointer  operator+(int  n) const noexcept{return {get_packed()+n};}
  constexpr variable_pointer  operator-(int  n) const noexcept{return {get_packed()-n};}

  variable_pointer&  operator+=(int  n) noexcept{  m_byte_offset += n;  return *this;}
  variable_pointer&  operator-=(int  n) noexcept{  m_byte_offset -= n;  return *this;}

};


class
function_pointer
{
  uint32_t  m_value;

public:
  constexpr function_pointer(uint32_t  v=0) noexcept: m_value(v){}

  constexpr operator bool() const noexcept{return m_value;}

  constexpr uint32_t  get() const noexcept{return m_value;}

};


class
operand
{
  enum class kind{
    null,
    identifier,
    integer_literal,
    null_pointer_literal,
    variable_pointer_literal,
    function_pointer_literal,
  } m_kind=kind::null;

  union data{
    std::string  s;
    int64_t      i;

    variable_pointer  vp;
    function_pointer  fp;

   data() noexcept{}
  ~data()         {}

  } m_data;

public:
  operand() noexcept{}
  operand(const operand&   rhs) noexcept{assign(rhs);}
  operand(      operand&&  rhs) noexcept{assign(std::move(rhs));}
  operand(std::string_view  sv) noexcept{assign(sv);}
  operand(int64_t  i) noexcept{assign(i);}
  operand(variable_pointer  vp) noexcept{assign(vp);}
  operand(function_pointer  fp) noexcept{assign(fp);}
 ~operand(){clear();}

  operand&  operator=(const operand&   rhs) noexcept{return assign(rhs);}
  operand&  operator=(      operand&&  rhs) noexcept{return assign(std::move(rhs));}
  operand&  operator=(std::string_view  sv) noexcept{return assign(sv);}
  operand&  operator=(int64_t  i) noexcept{return assign(i);}
  operand&  operator=(variable_pointer  vp) noexcept{return assign(vp);}
  operand&  operator=(function_pointer  fp) noexcept{return assign(fp);}

  operand&  assign(const operand&   rhs) noexcept;
  operand&  assign(      operand&&  rhs) noexcept;
  operand&  assign(std::string_view  sv) noexcept;
  operand&  assign(int64_t  i) noexcept;
  operand&  assign(variable_pointer  vp) noexcept;
  operand&  assign(function_pointer  fp) noexcept;

  void  clear() noexcept;

  bool  is_identifier()               const noexcept{return m_kind == kind::identifier;}
  bool  is_integer_literal()          const noexcept{return m_kind == kind::integer_literal;}
  bool  is_null_pointer_literal()     const noexcept{return m_kind == kind::null_pointer_literal;}
  bool  is_variable_pointer_literal() const noexcept{return m_kind == kind::variable_pointer_literal;}
  bool  is_function_pointer_literal() const noexcept{return m_kind == kind::function_pointer_literal;}

  const std::string&  get_identifier() const noexcept{return m_data.s;}

  int64_t  get_integer_literal() const noexcept{return m_data.i;}

  variable_pointer   get_variable_pointer() const noexcept{return m_data.vp;}
  function_pointer   get_function_pointer() const noexcept{return m_data.fp;}

  value  evaluate(context&  ctx) const noexcept;

  void  print() const noexcept;

};


enum class
unary_opcodes
{
  ld,
  bit_not,
  logical_not,
  get_size,
  get_address,

};


enum class
binary_opcodes
{
  add,
  sub,
  mul,
  div,
  rem,
  shl,
  shr,
  eq,
  neq,
  lt,
  lteq,
  gt,
  gteq,
  logical_and,
  logical_or,
  bit_and,
  bit_or,
  bit_xor,
};


class
variable
{
  uint32_t  m_address;

  value  m_value;

  std::string  m_name;

public:
  variable(uint32_t  addr, const typesystem::type_info&  ti, std::string_view  name) noexcept:
  m_address(addr), m_value(ti), m_name(name){}

  variable(uint32_t  addr, value&&  v, std::string_view  name) noexcept:
  m_address(addr), m_value(std::move(v)), m_name(name){}

  variable_pointer  get_address() const noexcept{return {m_address};}

  const std::string&  get_name() const noexcept{return m_name;}

        value&  get_value()       noexcept{return m_value;}
  const value&  get_value() const noexcept{return m_value;}

  void  print() const noexcept{  printf("%s ",m_name.data());  m_value.print();}

};


class return_instruction;
class branch_instruction;


class
context
{
  typesystem::type_collection  m_type_collection;

  std::vector<function>  m_function_table;
  std::vector<variable>  m_variable_table;

  struct frame;

  frame*  m_current_frame=nullptr;

  bool  m_finalized=false;

  void  push_variable(const typesystem::type_info&  ti, std::string_view  name) noexcept;

  void  push_frame(variable_pointer  p, const function&  fn, int  argc, const variable*  args) noexcept;
  void  pop_frame() noexcept;

  void  store(const value&  dst, const value&  src) noexcept;

  void  process(const branch_instruction&   br) noexcept;
  void  process(const return_instruction&  ret) noexcept;

  void  call(variable&  var, int  n, const operand*  ops) noexcept;
  void  seek(variable&  var, int  n, const operand*  ops) noexcept;

  value  operate( unary_opcodes  op, const operand&  o) noexcept;
  value  operate(binary_opcodes  op, const operand&  l, const operand&  r) noexcept;

  int  get_base_index() const noexcept;

public:
  context() noexcept;

  typesystem::type_collection&  get_type_collection() noexcept{return m_type_collection;}

  value  make_value(int64_t    i) noexcept{return value(*m_type_collection.find("int"),i);}
  value  make_value(uint64_t   u) noexcept{return value(*m_type_collection.find("uint"),u);}
  value  make_value(bool       b) noexcept{return value(*m_type_collection.find("bool"),static_cast<int64_t>(b));}
  value  make_value(nullptr_t  p) noexcept{return value(*m_type_collection.find("nullptr_t"));}

  function&  create_function(std::string_view  name) noexcept;

        function*  find_function(std::string_view  name)       noexcept;
  const function*  find_function(std::string_view  name) const noexcept;

        variable*  find_variable(std::string_view  name)       noexcept;
  const variable*  find_variable(std::string_view  name) const noexcept;

  variable&  operator[](variable_pointer  p) noexcept;
  function&  operator[](function_pointer  p) noexcept{return m_function_table[p.get()];}
  const variable&  operator[](variable_pointer  p) const noexcept;
  const function&  operator[](function_pointer  p) const noexcept{return m_function_table[p.get()];}

  value  dereference(const value&  v) noexcept;

  context&  entry(std::string_view  fn_name) noexcept{  push_frame(0,*find_function(fn_name),0,nullptr);  return *this;}

  void  step() noexcept;
  void   run() noexcept;

  context&  finalize() noexcept;

  void  print() const noexcept;

};




class
store_instruction
{
  operand  m_destination;
  operand  m_source;

public:
  store_instruction(operand  dst, operand  src) noexcept:
  m_destination(std::move(dst)), m_source(std::move(src)){}

        operand&  get_destination()       noexcept{return m_destination;}
  const operand&  get_destination() const noexcept{return m_destination;}

        operand&  get_source()       noexcept{return m_source;}
  const operand&  get_source() const noexcept{return m_source;}

  void  print() const noexcept
  {
    printf("st ");

    m_destination.print();

    printf(" ");

    m_source.print();
  }

};


class
branch_instruction
{
  operand  m_condition;
  operand  m_destination;

public:
  branch_instruction(operand&&  cond, operand&&  dest) noexcept:
  m_condition(std::move(cond)), m_destination(dest){}

        operand&  get_condition()       noexcept{return m_condition;}
  const operand&  get_condition() const noexcept{return m_condition;}

        operand&  get_destination()       noexcept{return m_destination;}
  const operand&  get_destination() const noexcept{return m_destination;}


  void  print() const noexcept
  {
    printf("br ");

    m_condition.print();

    printf(" ");

    m_destination.print();
  }

};


class
return_instruction
{
  operand  m_operand;

public:
  return_instruction(operand  o) noexcept:
  m_operand(std::move(o)){}

        operand&  get_operand()       noexcept{return m_operand;}
  const operand&  get_operand() const noexcept{return m_operand;}


  void  print() const noexcept
  {
    printf("ret ");

    m_operand.print();
  }

};


class
operation
{
  enum class kind{
    null,
    unary,
    binary,
    call,
    seek,

  } m_kind=kind::null;

  int  m_opcode=0;

  std::vector<operand>  m_operand_list;

public:
  operation(std::string_view  lb) noexcept: m_operand_list({lb}){}
  operation(std::string_view  lb, unary_opcodes  unop, operand&&  o) noexcept: m_operand_list({lb}){reset(unop,std::move(o));}
  operation(std::string_view  lb, binary_opcodes  binop, operand&&  l, operand&&  r) noexcept: m_operand_list({lb}){reset(binop,std::move(l),std::move(r));}
  operation(std::string_view  lb, std::string_view  fn_name, std::vector<operand>&&  args) noexcept: m_operand_list({lb}){reset(fn_name,std::move(args));}
  operation(std::string_view  lb, std::initializer_list<std::string_view>  names) noexcept: m_operand_list({lb}){reset(names);}
 ~operation(){}

  operation&  reset() noexcept;
  operation&  reset(unary_opcodes  unop, operand&&  o) noexcept;
  operation&  reset(binary_opcodes  binop, operand&&  l, operand&&  r) noexcept;
  operation&  reset(std::string_view  fn_name, std::vector<operand>&&  args) noexcept;
  operation&  reset(std::initializer_list<std::string_view>  names) noexcept;

  unary_opcodes   get_unary_opcodes()  const noexcept{return static_cast< unary_opcodes>(m_opcode);}
  binary_opcodes  get_binary_opcodes() const noexcept{return static_cast<binary_opcodes>(m_opcode);}

        std::vector<operand>&  get_operand_list()       noexcept{return m_operand_list;}
  const std::vector<operand>&  get_operand_list() const noexcept{return m_operand_list;}

  bool  is_unary()  const noexcept{return m_kind == kind::unary;}
  bool  is_binary() const noexcept{return m_kind == kind::binary;}
  bool  is_call()   const noexcept{return m_kind == kind::call;}
  bool  is_seek()   const noexcept{return m_kind == kind::seek;}

  void  print() const noexcept;

};


class
codeline
{
  enum class kind{
    null,
    store_instruction,
    branch_instruction,
    return_instruction,
    operation,
  } m_kind=kind::null;

  function*  m_function=nullptr;

  union data{
    store_instruction   st;
    branch_instruction  br;
    return_instruction  ret;

    operation  op;

    data() noexcept{}
   ~data(){}
  } m_data;

public:
  codeline() noexcept{}
  codeline(function&  fn) noexcept: m_function(&fn){}
  codeline(const codeline&   rhs) noexcept=delete;
  codeline(      codeline&&  rhs) noexcept{assign(std::move(rhs));}
  codeline(function&  fn, store_instruction&&   st) noexcept: m_function(&fn){assign(std::move(st));}
  codeline(function&  fn, branch_instruction&&  br) noexcept: m_function(&fn){assign(std::move(br));}
  codeline(function&  fn, return_instruction&&  ret) noexcept: m_function(&fn){assign(std::move(ret));}
  codeline(function&  fn, operation&&            op) noexcept: m_function(&fn){assign(std::move(op));}
 ~codeline(){clear();}

  codeline&  operator=(const codeline&   rhs) noexcept=delete;
  codeline&  operator=(      codeline&&  rhs) noexcept{return assign(std::move(rhs));}
  codeline&  operator=(store_instruction&&   st) noexcept{return assign(std::move(st));}
  codeline&  operator=(branch_instruction&&  br) noexcept{return assign(std::move(br));}
  codeline&  operator=(return_instruction&&  ret) noexcept{return assign(std::move(ret));}
  codeline&  operator=(operation&&            op) noexcept{return assign(std::move(op));}

//  codeline&  assign(const codeline&   rhs) noexcept;
  codeline&  assign(      codeline&&  rhs) noexcept;
  codeline&  assign(store_instruction&&   st) noexcept;
  codeline&  assign(branch_instruction&&  br) noexcept;
  codeline&  assign(return_instruction&&  ret) noexcept;
  codeline&  assign(operation&&            op) noexcept;


  void  clear() noexcept;

  const store_instruction&   get_store_instruction()  const noexcept{return m_data.st;}
  const branch_instruction&  get_branch_instruction() const noexcept{return m_data.br;}
  const return_instruction&  get_return_instruction() const noexcept{return m_data.ret;}
  const operation&           get_operation()          const noexcept{return m_data.op;}

  store_instruction&   get_store_instruction()  noexcept{return m_data.st;}
  branch_instruction&  get_branch_instruction() noexcept{return m_data.br;}
  return_instruction&  get_return_instruction() noexcept{return m_data.ret;}
  operation&           get_operation()          noexcept{return m_data.op;}

  function&  get_function() const noexcept{return *m_function;}

  bool  is_null()               const noexcept{return m_kind == kind::null;}
  bool  is_store_instruction()  const noexcept{return m_kind == kind::store_instruction;}
  bool  is_branch_instruction() const noexcept{return m_kind == kind::branch_instruction;}
  bool  is_return_instruction() const noexcept{return m_kind == kind::return_instruction;}
  bool  is_operation()          const noexcept{return m_kind == kind::operation;}

  void  print() const noexcept;

};




class
entry_point
{
  std::string  m_label;

  uint32_t  m_value=0;

public:
  entry_point() noexcept{}
  entry_point(std::string_view  lb, uint32_t  v) noexcept: m_label(lb), m_value(v){}

  const std::string&  get_label() const noexcept{return m_label;}

  uint32_t  get_value() const noexcept{return m_value;}

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

  uint32_t  m_address=0;

  const typesystem::type_info*  m_signature=nullptr;

  std::string  m_name;

  std::vector<std::string>  m_argument_name_list;

  std::vector<declaration>  m_declaration_list;

  std::vector<codeline>  m_codelines;

  std::vector<entry_point>   m_entry_point_list;

  bool  m_finalized=false;

  void  resolve(const context&  ctx, operand&  o) const noexcept;

public:
  function(context&  ctx, uint32_t  addr, std::string_view  name) noexcept:
  m_context(ctx), m_address(addr), m_name(name){}

  const codeline&  operator[](int  i) const noexcept{return m_codelines[i];}

  int  get_address() const noexcept{return m_address;}

  int  get_number_of_codelines() const noexcept{return m_codelines.size();}

  const std::string&  get_name() const noexcept{return m_name;}

  const typesystem::type_info&  get_signature() const noexcept{return *m_signature;}

  const declaration_list&  get_declaration_list() const noexcept{return m_declaration_list;}
  const std::vector<std::string>&  get_argument_name_list() const noexcept{return m_argument_name_list;}

  function&  append_store_instruction(operand  dst, operand  src) noexcept;
  function&  append_branch_instruction(operand  cond, std::string_view  lb) noexcept;
  function&  append_return_instruction(operand  o) noexcept;
  function&  append_operation(operation  op) noexcept;

  function&  append_declaration(std::string_view  type_name, std::string_view  var_name) noexcept;
  function&  append_entry_point(std::string_view  lb) noexcept;

  int  get_number_of_variables() const noexcept{return m_declaration_list.size()+m_argument_name_list.size();}

  const operation*  find_operation(std::string_view  label) const noexcept;

  const entry_point*  find_entry_point(std::string_view  label) const noexcept;

  void  finalize(const context&  ctx) noexcept;

  void  print() const noexcept;

};




}




#endif




