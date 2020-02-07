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
multi_pointer
{
  char  m_kind;

  uint32_t  m_value;

public:
  constexpr multi_pointer(char  k=0, uint32_t  v=0) noexcept: m_kind(k), m_value(v){}
  constexpr multi_pointer(uint64_t  v) noexcept: m_kind(v>>32), m_value(v&0xFFFFFFFF){}

  constexpr operator bool() const noexcept{return m_value;}

  constexpr char  get_kind() const noexcept{return m_kind;}

  constexpr bool  is_function() const noexcept{return m_kind == 'f';}
  constexpr bool  is_global()   const noexcept{return m_kind == 'g';}
  constexpr bool  is_local()    const noexcept{return m_kind == 'l';}
  constexpr bool  is_null()     const noexcept{return m_kind == 'n';}

  constexpr uint32_t  get() const noexcept{return m_value;}
  constexpr uint64_t  get_packed() const noexcept{return (static_cast<uint64_t>(m_kind)<<32)|m_value;}

};


class
operand
{
  enum class kind{
    null,
    identifier,
    integer_literal,
    pointer_literal,
  } m_kind=kind::null;

  union data{
    std::string  s;
    int64_t      i;

    multi_pointer  p;

   data() noexcept{}
  ~data()         {}

  } m_data;

public:
  operand() noexcept{}
  operand(const operand&   rhs) noexcept{assign(rhs);}
  operand(      operand&&  rhs) noexcept{assign(std::move(rhs));}
  operand(std::string_view  sv) noexcept{assign(sv);}
  operand(int64_t  i) noexcept{assign(i);}
  operand(multi_pointer  p) noexcept{assign(p);}
 ~operand(){clear();}

  operand&  operator=(const operand&   rhs) noexcept{return assign(rhs);}
  operand&  operator=(      operand&&  rhs) noexcept{return assign(std::move(rhs));}
  operand&  operator=(std::string_view  sv) noexcept{return assign(sv);}
  operand&  operator=(int64_t  i) noexcept{return assign(i);}
  operand&  operator=(multi_pointer  p) noexcept{return assign(p);}

  operand&  assign(const operand&   rhs) noexcept;
  operand&  assign(      operand&&  rhs) noexcept;
  operand&  assign(std::string_view  sv) noexcept;
  operand&  assign(int64_t  i) noexcept;
  operand&  assign(multi_pointer  p) noexcept;

  void  clear() noexcept;

  bool  is_identifier()      const noexcept{return m_kind == kind::identifier;}
  bool  is_integer_literal() const noexcept{return m_kind == kind::integer_literal;}
  bool  is_pointer_literal() const noexcept{return m_kind == kind::pointer_literal;}

  const std::string&  get_identifier() const noexcept{return m_data.s;}

  int64_t  get_integer_literal() const noexcept{return m_data.i;}

  const multi_pointer&  get_pointer() const noexcept{return m_data.p;}

  value  evaluate(context&  ctx) const noexcept;

  void  print(const context*  ctx=nullptr, const function*  fn=nullptr) const noexcept;

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


using address_t = uint32_t;


class
variable
{
  address_t  m_address;

  value  m_value;

  std::string  m_name;

public:
  variable(address_t  addr, const typesystem::type_info&  ti, std::string_view  name) noexcept:
  m_address(addr), m_value(ti), m_name(name){}

  variable(address_t  addr, value&&  v, std::string_view  name) noexcept:
  m_address(addr), m_value(std::move(v)), m_name(name){}

  address_t  get_address() const noexcept{return m_address;}

  const std::string&  get_name() const noexcept{return m_name;}

        value&  get_value()       noexcept{return m_value;}
  const value&  get_value() const noexcept{return m_value;}

  void  print() const noexcept
  {
    printf("{var: {address: %4" PRIu32 ", name:%s, value:",m_address,m_name.data());

    m_value.print();

    printf("}}");
  }

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

  variable&  push_variable(const typesystem::type_info&  ti, std::string_view  name) noexcept;

  void  push_frame(uint32_t  st_p, const function&  fn, int  argc, const operand*  args) noexcept;
  void  pop_frame() noexcept;

  void  store(const value&  dst, const value&  src) noexcept;

  void  process(const branch_instruction&   br) noexcept;
  void  process(const return_instruction&  ret) noexcept;
  void  process(const operation&  op) noexcept;

  void  seek(variable&  dst, value&&  src, std::string_view  name) noexcept;

  value  operate( unary_opcodes  op, const operand&  o) noexcept;
  value  operate(binary_opcodes  op, const operand&  l, const operand&  r) noexcept;

  int  get_base_index() const noexcept;

public:
  context() noexcept;

  typesystem::type_collection&  get_type_collection() noexcept{return m_type_collection;}

  value  make_value(int64_t    i) noexcept{return value(*m_type_collection.find_by_name("int"),i);}
  value  make_value(uint64_t   u) noexcept{return value(*m_type_collection.find_by_name("uint"),u);}
  value  make_value(bool       b) noexcept{return value(*m_type_collection.find_by_name("bool"),static_cast<int64_t>(b));}
  value  make_value(nullptr_t  p) noexcept{return value(*m_type_collection.find_by_name("nullptr_t"));}

  function&  create_function(std::string_view  sig, std::string_view  name) noexcept;

        function*  find_function(std::string_view  name)       noexcept;
  const function*  find_function(std::string_view  name) const noexcept;

        variable*  find_variable(std::string_view  name)       noexcept;
  const variable*  find_variable(std::string_view  name) const noexcept;

        variable&  get_global_variable(uint32_t  p) noexcept{return m_variable_table[p];}
        variable&  get_local_variable( uint32_t  p) noexcept{return m_variable_table[get_base_index()+p];}
        function&  get_function(       uint32_t  p) noexcept{return m_function_table[p];}
  const variable&  get_global_variable(uint32_t  p) const noexcept{return m_variable_table[p];}
  const variable&  get_local_variable( uint32_t  p) const noexcept{return m_variable_table[get_base_index()+p];}
  const function&  get_function(       uint32_t  p) const noexcept{return m_function_table[p];}

  variable&  get_variable(multi_pointer  p) noexcept;

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
  operation(std::string_view  lb, std::vector<std::string_view>&&  names) noexcept: m_operand_list({lb}){reset(std::move(names));}
 ~operation(){}

  operation&  reset() noexcept;
  operation&  reset(unary_opcodes  unop, operand&&  o) noexcept;
  operation&  reset(binary_opcodes  binop, operand&&  l, operand&&  r) noexcept;
  operation&  reset(std::string_view  fn_name, std::vector<operand>&&  args) noexcept;
  operation&  reset(std::vector<std::string_view>&&  names) noexcept;

  unary_opcodes   get_unary_opcodes()  const noexcept{return static_cast< unary_opcodes>(m_opcode);}
  binary_opcodes  get_binary_opcodes() const noexcept{return static_cast<binary_opcodes>(m_opcode);}

        std::vector<operand>&  get_operand_list()       noexcept{return m_operand_list;}
  const std::vector<operand>&  get_operand_list() const noexcept{return m_operand_list;}

  bool  is_unary()  const noexcept{return m_kind == kind::unary;}
  bool  is_binary() const noexcept{return m_kind == kind::binary;}
  bool  is_call()   const noexcept{return m_kind == kind::call;}
  bool  is_seek()   const noexcept{return m_kind == kind::seek;}

  void  print(const context*  ctx=nullptr, const function*  fn=nullptr) const noexcept;

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

  void  print(const context*  ctx=nullptr, const function*  fn=nullptr) const noexcept;

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

  address_t  m_address=0;

  const typesystem::type_info&  m_signature;

  std::string  m_name;

  std::vector<std::string>  m_argument_name_list;

  std::vector<declaration>  m_declaration_list;

  std::vector<codeline>  m_codelines;

  std::vector<entry_point>   m_entry_point_list;

  bool  m_finalized=false;

  void  resolve(const context&  ctx, operand&  o) const noexcept;

public:
  function(context&  ctx, address_t  addr, const typesystem::type_info&  sig, std::string_view  name) noexcept:
  m_context(ctx), m_address(addr), m_signature(sig), m_name(name){}

  const codeline&  operator[](int  i) const noexcept{return m_codelines[i];}

  address_t  get_address() const noexcept{return m_address;}

  int  get_number_of_codelines() const noexcept{return m_codelines.size();}

  const std::string&  get_name() const noexcept{return m_name;}

  const typesystem::type_info&  get_signature() const noexcept{return m_signature;}

  function&  set_argument_name_list(std::vector<std::string_view>&&  argnams) noexcept;

  const declaration_list&          get_declaration_list() const noexcept{return m_declaration_list;}
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

  value  get_value() const noexcept{return value(m_signature,static_cast<uint64_t>(m_address));}

  void  print(const context*  ctx=nullptr) const noexcept;

};




}




#endif




