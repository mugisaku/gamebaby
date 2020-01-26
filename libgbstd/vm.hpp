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




class execution_frame;
class codeline;
class operand;
class operation;
class function;
class space;


class
memory_frame
{
  uint8_t*  m_pointer=nullptr;

  int  m_length=0;

public:
  constexpr memory_frame(uint8_t*  ptr=nullptr, int  len=0) noexcept:
  m_pointer(ptr), m_length(len){}

  constexpr memory_frame(const memory_frame&  base, int  offset) noexcept:
  m_pointer(base.m_pointer+offset), m_length(base.m_length){}

  constexpr memory_frame(const memory_frame&  base, int  offset, int  length) noexcept:
  m_pointer(base.m_pointer+offset), m_length(length){}

  constexpr uint8_t&  operator[](int  i) const noexcept{return m_pointer[i];}

  constexpr int  get_length() const noexcept{return m_length;}

  constexpr uint8_t*  get_pointer() const noexcept{return m_pointer;}

  constexpr uint8_t*   get_ptr8( int  address) noexcept{return reinterpret_cast<uint8_t* >(m_pointer[address   ]);}
  constexpr uint16_t*  get_ptr16(int  address) noexcept{return reinterpret_cast<uint16_t*>(m_pointer[address&~1]);}
  constexpr uint32_t*  get_ptr32(int  address) noexcept{return reinterpret_cast<uint32_t*>(m_pointer[address&~3]);}
  constexpr uint64_t*  get_ptr64(int  address) noexcept{return reinterpret_cast<uint64_t*>(m_pointer[address&~7]);}

};


class
memory_view
{
  const uint8_t*  m_pointer=nullptr;

  int  m_length=0;

public:
  constexpr memory_view(const uint8_t*  ptr=nullptr, int  len=0) noexcept:
  m_pointer(ptr), m_length(len){}

  constexpr memory_view(const memory_frame&  base, int  offset) noexcept:
  m_pointer(base.get_pointer()+offset), m_length(base.get_length()){}

  constexpr memory_view(const memory_view&  base, int  offset) noexcept:
  m_pointer(base.m_pointer+offset), m_length(base.m_length){}

  constexpr memory_view(const memory_frame&  base, int  offset, int  length) noexcept:
  m_pointer(base.get_pointer()+offset), m_length(length){}

  constexpr memory_view(const memory_view&  base, int  offset, int  length) noexcept:
  m_pointer(base.m_pointer+offset), m_length(length){}

  constexpr const uint8_t&  operator[](int  i) const noexcept{return m_pointer[i];}

  constexpr int  get_length() const noexcept{return m_length;}

  constexpr const uint8_t*  get_pointer() const noexcept{return m_pointer;}

  constexpr const uint8_t*   get_ptr8( int  address) noexcept{return reinterpret_cast<uint8_t* >(m_pointer[address   ]);}
  constexpr const uint16_t*  get_ptr16(int  address) noexcept{return reinterpret_cast<uint16_t*>(m_pointer[address&~1]);}
  constexpr const uint32_t*  get_ptr32(int  address) noexcept{return reinterpret_cast<uint32_t*>(m_pointer[address&~3]);}
  constexpr const uint64_t*  get_ptr64(int  address) noexcept{return reinterpret_cast<uint64_t*>(m_pointer[address&~7]);}

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


class
value
{
  struct data;

  data*  m_data=nullptr;

  void  unrefer() noexcept;

public:
  value() noexcept;
  value(const typesystem::type_info&  ti) noexcept{assign(ti);}
  value(const value&   rhs) noexcept{assign(rhs);}
  value(      value&&  rhs) noexcept{assign(std::move(rhs));}
 ~value(){unrefer();}

  operator bool() const noexcept{return m_data;}

  value&  operator=(const value&   rhs) noexcept{return assign(rhs);}
  value&  operator=(      value&&  rhs) noexcept{return assign(std::move(rhs));}
  value&  operator=(const typesystem::type_info&  ti) noexcept{return assign(ti);}

  value&  assign(const value&   rhs) noexcept;
  value&  assign(      value&&  rhs) noexcept;
  value&  assign(const typesystem::type_info&  ti) noexcept;

  const typesystem::type_info&  get_type_info() const noexcept;

  int64_t   get_si() const noexcept;
  uint64_t  get_ui() const noexcept;
  value  update( int64_t  i, mutability  m) const noexcept;
  value  update(uint64_t  u, mutability  m) const noexcept;
  value  update(memory_view  mv, mutability  m) const noexcept;

  value  clone() const noexcept;

  memory_frame  get_memory_frame() const noexcept;

  bool  is_mutable() const noexcept;

  bool  is_integer() const noexcept;
  bool  is_boolean() const noexcept;
  bool  is_function() const noexcept;

  void  print() const noexcept;

};


class
operand
{
  enum class kind{
    null,
    operation_label,
    integer_literal,
  } m_kind=kind::null;

  union data{
    std::string  s;
    int64_t      i;

   data() noexcept{}
  ~data()         {}

  } m_data;

public:
  operand() noexcept{}
  operand(const operand&   rhs) noexcept{assign(rhs);}
  operand(      operand&&  rhs) noexcept{assign(std::move(rhs));}
  operand(std::string_view  sv) noexcept{assign(sv);}
  operand(int64_t  i) noexcept{assign(i);}
 ~operand(){clear();}

  operand&  operator=(const operand&   rhs) noexcept{return assign(rhs);}
  operand&  operator=(      operand&&  rhs) noexcept{return assign(std::move(rhs));}
  operand&  operator=(std::string_view  sv) noexcept{return assign(sv);}
  operand&  operator=(int64_t  i) noexcept{return assign(i);}

  operand&  assign(const operand&   rhs) noexcept;
  operand&  assign(      operand&&  rhs) noexcept;
  operand&  assign(std::string_view  sv) noexcept;
  operand&  assign(int64_t  i) noexcept;

  void  clear() noexcept;

  bool  is_operation_label() const noexcept{return m_kind == kind::operation_label;}
  bool  is_integer_literal() const noexcept{return m_kind == kind::integer_literal;}

  value  evaluate(execution_frame&  frm) const noexcept;

  void  print() const noexcept;

};




class
variable
{
  value  m_value;

  std::string  m_label;

public:
  variable(const typesystem::type_info&  ti, std::string_view  lb) noexcept: m_value(ti), m_label(lb){}

  const std::string&  get_label() const noexcept{return m_label;}

        value&  get_value()       noexcept{return m_value;}
  const value&  get_value() const noexcept{return m_value;}

  void  print() const noexcept{  printf("%s ",m_label.data());  m_value.print();}

};


class
variable_table
{
  std::vector<variable>  m_container;

public:
  variable&  append(const variable&  v) noexcept;
  variable&  append(const typesystem::type_info&  ti, std::string_view  lb) noexcept;

        variable*  find(std::string_view  lb)       noexcept;
  const variable*  find(std::string_view  lb) const noexcept;

};


class
execution_frame
{
  const function&  m_function;

  variable_table  m_variable_table;

  uint32_t  m_pc=0;

  value  m_returned_value;

  bool  m_returned;

  void  store(value  dst, value  src) noexcept;
  void  step(const codeline&  codeln) noexcept;

public:
  execution_frame(const function&  fn, execution_frame*  parent=nullptr, int  argc=0, variable*  args=nullptr) noexcept;

  const function&  get_function() const noexcept{return m_function;}

  const variable_table&  get_variable_table() const noexcept{return m_variable_table;}

  value  get_returned_value() const noexcept{return m_returned_value;}

  value  run() noexcept;

};


class
space
{
  typesystem::type_collection  m_type_collection;

  std::vector<std::unique_ptr<function>>  m_function_table;

  struct frame_wrapper{
    execution_frame  m_frame;

    frame_wrapper*  m_previous;

  };

  frame_wrapper*  m_current_frame;

  variable_table  m_variable_table;

public:
  space() noexcept{}

  typesystem::type_collection&  get_type_collection() noexcept{return m_type_collection;}

  const variable_table&  get_variable_table() const noexcept{return m_variable_table;}

  function&  create_function(std::string_view  name) noexcept;

  function*  find_function(std::string_view  name) const noexcept;

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

  const operand&  get_destination() const noexcept{return m_destination;}
  const operand&       get_source() const noexcept{return      m_source;}

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

  std::string  m_label;

public:
  branch_instruction(operand&&  cond, std::string_view  lb) noexcept:
  m_condition(std::move(cond)), m_label(lb){}

  const std::string&  get_label() const noexcept{return m_label;}

  const operand&  get_condition() const noexcept{return m_condition;}


  void  print() const noexcept
  {
    printf("br ");

    m_condition.print();

    printf(" %s",m_label.data());
  }

};


class
return_instruction
{
  operand  m_operand;

public:
  return_instruction(operand  o) noexcept:
  m_operand(std::move(o)){}

  const operand&  get_operand() const noexcept{return m_operand;}


  void  print() const noexcept
  {
    printf("ret ");

    m_operand.print();
  }

};


class
unary_operation
{
  enum class kind{
    null,
    bit_not,
    logical_not,
    get_size,
    get_address,
    dereference,
    
  } m_kind=kind::null;

  operand  m_operand;

  unary_operation(kind  k, operand  o) noexcept:
  m_kind(k), m_operand(std::move(o)){}

public:
  value  operator()(execution_frame&  frm) const noexcept;

  void  print() const noexcept;

  static unary_operation  make_bit_not(    operand  o) noexcept{return {kind::bit_not    ,std::move(o)};}
  static unary_operation  make_logical_not(operand  o) noexcept{return {kind::logical_not,std::move(o)};}

};


class
binary_operation
{
  enum class kind{
    null,
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

  } m_kind=kind::null;

  operand  m_left;
  operand  m_right;

  binary_operation(kind  k, operand  l, operand  r) noexcept:
  m_kind(k), m_left(std::move(l)), m_right(std::move(r)){}

public:
  value  operator()(execution_frame&  frm) const noexcept;

  void  print() const noexcept;

  static binary_operation  make_add(        operand  l, operand  r) noexcept{return {kind::add        ,std::move(l),std::move(r)};}
  static binary_operation  make_sub(        operand  l, operand  r) noexcept{return {kind::sub        ,std::move(l),std::move(r)};}
  static binary_operation  make_mul(        operand  l, operand  r) noexcept{return {kind::mul        ,std::move(l),std::move(r)};}
  static binary_operation  make_div(        operand  l, operand  r) noexcept{return {kind::div        ,std::move(l),std::move(r)};}
  static binary_operation  make_rem(        operand  l, operand  r) noexcept{return {kind::rem        ,std::move(l),std::move(r)};}
  static binary_operation  make_shl(        operand  l, operand  r) noexcept{return {kind::shl        ,std::move(l),std::move(r)};}
  static binary_operation  make_shr(        operand  l, operand  r) noexcept{return {kind::shr        ,std::move(l),std::move(r)};}
  static binary_operation  make_eq(         operand  l, operand  r) noexcept{return {kind::eq         ,std::move(l),std::move(r)};}
  static binary_operation  make_neq(        operand  l, operand  r) noexcept{return {kind::neq        ,std::move(l),std::move(r)};}
  static binary_operation  make_lt(         operand  l, operand  r) noexcept{return {kind::lt         ,std::move(l),std::move(r)};}
  static binary_operation  make_lteq(       operand  l, operand  r) noexcept{return {kind::lteq       ,std::move(l),std::move(r)};}
  static binary_operation  make_gt(         operand  l, operand  r) noexcept{return {kind::gt         ,std::move(l),std::move(r)};}
  static binary_operation  make_gteq(       operand  l, operand  r) noexcept{return {kind::gteq       ,std::move(l),std::move(r)};}
  static binary_operation  make_logical_and(operand  l, operand  r) noexcept{return {kind::logical_and,std::move(l),std::move(r)};}
  static binary_operation  make_logical_or( operand  l, operand  r) noexcept{return {kind::logical_or ,std::move(l),std::move(r)};}
  static binary_operation  make_bit_and(    operand  l, operand  r) noexcept{return {kind::bit_and    ,std::move(l),std::move(r)};}
  static binary_operation  make_bit_or(     operand  l, operand  r) noexcept{return {kind::bit_or     ,std::move(l),std::move(r)};}
  static binary_operation  make_bit_xor(    operand  l, operand  r) noexcept{return {kind::bit_xor    ,std::move(l),std::move(r)};}

};




class
call_operation
{
  std::string  m_function_name;

  std::vector<operand>  m_arguments;

public:
  call_operation(std::string_view  fn_name, std::initializer_list<operand>  args) noexcept:
  m_function_name(fn_name), m_arguments(args){}

  value  operator()(execution_frame&  frm) const noexcept;

  void  print() const noexcept;

};


class
operation
{
  enum class kind{
    null,
    unary,
    binary,
    call,

  } m_kind=kind::null;


  union data{
    unary_operation    un;
    binary_operation  bin;
    call_operation    cal;

    data() noexcept{}
   ~data(){}
  } m_data;


  std::string  m_label;

public:
  operation(std::string_view  lb) noexcept: m_label(lb){}
  operation(std::string_view  lb, unary_operation&&     un) noexcept: m_label(lb){assign(std::move( un));}
  operation(std::string_view  lb, binary_operation&&   bin) noexcept: m_label(lb){assign(std::move(bin));}
  operation(std::string_view  lb, call_operation&&     cal) noexcept: m_label(lb){assign(std::move(cal));}
  operation(const operation&   rhs) noexcept=delete;
  operation(      operation&&  rhs) noexcept{assign(std::move(rhs));}
 ~operation(){unset_content();}

  operation&  operator=(const operation&   rhs) noexcept=delete;
  operation&  operator=(      operation&&  rhs) noexcept{return assign(std::move(rhs));}

  operation&  assign(const operation&   rhs) noexcept=delete;
  operation&  assign(      operation&&  rhs) noexcept;
  operation&  assign(unary_operation&&     un) noexcept;
  operation&  assign(binary_operation&&   bin) noexcept;
  operation&  assign(call_operation&&     cal) noexcept;

  void  unset_content() noexcept;

  operation&  set_content(unary_operation&&     un) noexcept;
  operation&  set_content(binary_operation&&   bin) noexcept;
  operation&  set_content(call_operation&&     cal) noexcept;

  const std::string&  get_label() const noexcept{return m_label;}

  const unary_operation&   get_unary()  const noexcept{return m_data.un;}
  const binary_operation&  get_binary() const noexcept{return m_data.bin;}
  const call_operation&    get_call()   const noexcept{return m_data.cal;}

  bool  is_unary()  const noexcept{return m_kind == kind::unary;}
  bool  is_binary() const noexcept{return m_kind == kind::binary;}
  bool  is_call()   const noexcept{return m_kind == kind::call;}

  value  operator()(execution_frame&  frm) const noexcept;

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
  const operation&           get_operation()     const noexcept{return m_data.op;}

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
  space*  m_space=nullptr;

  const typesystem::type_info*  m_type_info=nullptr;

  std::string  m_name;

  std::vector<declaration>  m_declaration_list;

  std::vector<std::string>  m_argument_name_list;

  std::vector<codeline>   m_codelines;

  std::vector<entry_point>   m_entry_point_list;

public:
  function(space&  sp, std::string_view  name) noexcept: m_space(&sp), m_name(name){}

  const codeline&  operator[](int  i) const noexcept{return m_codelines[i];}

  int  get_number_of_codelines() const noexcept{return m_codelines.size();}

  const std::string&  get_name() const noexcept{return m_name;}

  const typesystem::type_info&  get_signature() const noexcept{return *m_type_info;}

  const declaration_list&  get_declaration_list() const noexcept{return m_declaration_list;}
  const std::vector<std::string>&  get_argument_name_list() const noexcept{return m_argument_name_list;}

  function&  append_store_instruction(operand  dst, operand  src) noexcept;
  function&  append_branch_instruction(operand  cond, std::string_view  lb) noexcept;
  function&  append_return_instruction(operand  o) noexcept;
  function&  append_operation(operation  op) noexcept;

  function&  append_declaration(                                  std::string_view  lb) noexcept;
  function&  append_declaration(const typesystem::type_info&  ti, std::string_view  lb) noexcept;
  function&  append_entry_point(std::string_view  lb) noexcept;

  space&  get_space() const noexcept{return *m_space;}

  const operation*  find_operation(std::string_view  label) const noexcept;

  const entry_point*  find_entry_point(std::string_view  label) const noexcept;

  void  print() const noexcept;

};




}




#endif




