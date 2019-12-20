#ifndef gbstd_vm_HPP
#define gbstd_vm_HPP


#include<cstdio>
#include<cstdint>
#include<cstdlib>
#include<memory>
#include<string>
#include<string_view>
#include<utility>
#include<vector>
#include"libgbstd/misc.hpp"
#include"libgbstd/utility.hpp"


namespace gbstd{




class execution_frame;
class codeline;
class operand;
class operation;
class function;
class space;


class
value
{
  enum class kind{
    null,
    undefined,
    data,
    null_pointer,
    u8_pointer,
    u16_pointer,
    u32_pointer,
    pointer,

  } m_kind=kind::null;

  int  m_data=0;

  constexpr value(kind  k, int  d) noexcept: m_kind(k), m_data(d){}

public:
  constexpr value() noexcept{}

  constexpr bool  is_null()         const noexcept{return m_kind == kind::null;}
  constexpr bool  is_undefined()    const noexcept{return m_kind == kind::undefined;}
  constexpr bool  is_data()         const noexcept{return m_kind == kind::data;}
  constexpr bool  is_null_pointer() const noexcept{return m_kind == kind::null_pointer;}
  constexpr bool  is_u8_pointer()   const noexcept{return m_kind == kind::u8_pointer;}
  constexpr bool  is_u16_pointer()  const noexcept{return m_kind == kind::u16_pointer;}
  constexpr bool  is_u32_pointer()  const noexcept{return m_kind == kind::u32_pointer;}
  constexpr bool  is_pointer()      const noexcept{return m_kind == kind::pointer;}

  constexpr int  get_data() const noexcept{return m_data;}

  static constexpr value  make_null() noexcept{return {kind::null,0};}
  static constexpr value  make_undefined() noexcept{return {kind::undefined,0};}
  static constexpr value  make_data(int  i) noexcept{return {kind::data,i};}
  static constexpr value  make_null_pointer() noexcept{return {kind::null_pointer,0};}
  static constexpr value  make_u8_pointer(  int  addr) noexcept{return {kind::u8_pointer,addr};}
  static constexpr value  make_u16_pointer(  int  addr) noexcept{return {kind::u16_pointer,addr};}
  static constexpr value  make_u32_pointer(  int  addr) noexcept{return {kind::u32_pointer,addr};}
  static constexpr value  make_pointer(  int  addr) noexcept{return {kind::pointer,addr};}

  void  print() const noexcept
  {
      switch(m_kind)
      {
    case(kind::null        ): printf("null");break;
    case(kind::undefined   ): printf("undefined");break;
    case(kind::data        ): printf("%d(data)",m_data);break;
    case(kind::null_pointer): printf("null_pointer");break;
    case(kind::u8_pointer  ): printf("%d(u8ptr)",m_data);break;
    case(kind::u16_pointer ): printf("%d(u16ptr)",m_data);break;
    case(kind::u32_pointer ): printf("%d(u32ptr)",m_data);break;
    case(kind::pointer     ): printf("%d(ptr)",m_data);break;
      }
  }

};


class
operand
{
  enum class kind{
    null,
    operation_label,
    operation,
    value,
  } m_kind=kind::null;

  union data{
    std::string  s;
    value        v;
    const operation*  op;

   data() noexcept{}
  ~data()         {}

  } m_data;

public:
  operand() noexcept{}
  operand(const operand&   rhs) noexcept{assign(rhs);}
  operand(      operand&&  rhs) noexcept{assign(std::move(rhs));}
  operand(std::string_view  sv) noexcept{assign(sv);}
  operand(value  v) noexcept{assign(v);}
 ~operand(){clear();}

  operand&  operator=(const operand&   rhs) noexcept{return assign(rhs);}
  operand&  operator=(      operand&&  rhs) noexcept{return assign(std::move(rhs));}
  operand&  operator=(std::string_view  sv) noexcept{return assign(sv);}
  operand&  operator=(value  v) noexcept{return assign(v);}

  operand&  assign(const operand&   rhs) noexcept;
  operand&  assign(      operand&&  rhs) noexcept;
  operand&  assign(std::string_view  sv) noexcept;  operand&  assign(int  i) noexcept;
  operand&  assign(value  v) noexcept;

  void  clear() noexcept;

  bool  is_operation_label() const noexcept{return m_kind == kind::operation_label;}
  bool  is_operation()       const noexcept{return m_kind == kind::operation;}
  bool  is_value()           const noexcept{return m_kind == kind::value;}

  bool  is_constant(const function&  fn) const noexcept;

  value  evaluate(execution_frame&  frm) const noexcept;

  void  print() const noexcept;

};




class
memory
{
  std::vector<uint8_t>  m_content;

public:
  void  resize(int  size) noexcept{m_content.resize(size);}

  uint8_t*   get_ptr8( int  address) noexcept{return reinterpret_cast<uint8_t* >(&m_content[address   ]);}
  uint16_t*  get_ptr16(int  address) noexcept{return reinterpret_cast<uint16_t*>(&m_content[address&~1]);}
  uint32_t*  get_ptr32(int  address) noexcept{return reinterpret_cast<uint32_t*>(&m_content[address&~3]);}

};


class
space
{
  memory&  m_memory;

  std::vector<std::unique_ptr<function>>  m_function_table;

public:
  space(memory&  mem) noexcept: m_memory(mem){}

  memory&  get_memory() const noexcept{return m_memory;}

  function&  create_function(std::string_view  name) noexcept;

  const function*  find_function(std::string_view  name) const noexcept;

  value  evaluate(std::string_view  name) const noexcept;

};


class
variable
{
  std::string  m_label;

  value  m_value;

public:
  variable() noexcept{}
  variable(std::string_view  lb, value  v) noexcept: m_label(lb), m_value(v){}

  const std::string&  get_label() const noexcept{return m_label;}

  value&  get_value() noexcept{return m_value;}

};


class
execution_frame
{
  const function&  m_function;

  std::vector<variable>  m_variable_list;

  uint32_t  m_pc=0;

  value  m_returned_value;

  bool  m_returned;

  void  store(value  dst, value  src) noexcept;
  void  step(const codeline&  codeln) noexcept;

public:
  execution_frame(const function&  fn, execution_frame*  parent=nullptr) noexcept;

  const function&  get_function() const noexcept{return m_function;}

  value  get_returned_value() const noexcept{return m_returned_value;}

  value  run() noexcept;

};




class
store_instruction
{
  operand  m_destination;
  operand  m_source;

public:
  store_instruction(operand&&  dst, operand&&  src) noexcept:
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

  std::string  m_zero_label;
  std::string  m_nonzero_label;

public:
  branch_instruction(operand&&  cond, std::string_view  z, std::string_view  nz) noexcept:
  m_condition(std::move(cond)), m_zero_label(z), m_nonzero_label(nz){}

  const std::string&  get_zero_label()    const noexcept{return m_zero_label;}
  const std::string&  get_nonzero_label() const noexcept{return m_nonzero_label;}

  const operand&  get_condition() const noexcept{return m_condition;}


  void  print() const noexcept
  {
    printf("br ");

    m_condition.print();

    printf(" %s %s",m_nonzero_label.data(),m_zero_label.data());
  }

};


class
return_instruction
{
  operand  m_operand;

public:
  return_instruction(operand&&  o) noexcept:
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
    neg,
    bit_not,
    logical_not,
    get_size,
    get_address,
    dereference,
    
  } m_kind=kind::null;

  operand  m_operand;

  unary_operation(kind  k, operand&&  o) noexcept:
  m_kind(k), m_operand(std::move(o)){}

public:
  value  evaluate(execution_frame&  frm) const noexcept;

  bool  is_constant(const function&  fn) const noexcept{return m_operand.is_constant(fn);}

  static unary_operation  make_neg(        operand&&  o) noexcept{return {kind::neg        ,std::move(o)};}
  static unary_operation  make_bit_not(    operand&&  o) noexcept{return {kind::bit_not    ,std::move(o)};}
  static unary_operation  make_logical_not(operand&&  o) noexcept{return {kind::logical_not,std::move(o)};}

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

  binary_operation(kind  k, operand&&  l, operand&&  r) noexcept:
  m_kind(k), m_left(std::move(l)), m_right(std::move(r)){}

public:
  value  evaluate(execution_frame&  frm) const noexcept;

  bool  is_constant(const function&  fn) const noexcept{return m_left.is_constant(fn) && m_right.is_constant(fn);}

  static binary_operation  make_add(        operand&&  l, operand&&  r) noexcept{return {kind::add        ,std::move(l),std::move(r)};}
  static binary_operation  make_sub(        operand&&  l, operand&&  r) noexcept{return {kind::sub        ,std::move(l),std::move(r)};}
  static binary_operation  make_mul(        operand&&  l, operand&&  r) noexcept{return {kind::mul        ,std::move(l),std::move(r)};}
  static binary_operation  make_div(        operand&&  l, operand&&  r) noexcept{return {kind::div        ,std::move(l),std::move(r)};}
  static binary_operation  make_rem(        operand&&  l, operand&&  r) noexcept{return {kind::rem        ,std::move(l),std::move(r)};}
  static binary_operation  make_shl(        operand&&  l, operand&&  r) noexcept{return {kind::shl        ,std::move(l),std::move(r)};}
  static binary_operation  make_shr(        operand&&  l, operand&&  r) noexcept{return {kind::shr        ,std::move(l),std::move(r)};}
  static binary_operation  make_eq(         operand&&  l, operand&&  r) noexcept{return {kind::eq         ,std::move(l),std::move(r)};}
  static binary_operation  make_neq(        operand&&  l, operand&&  r) noexcept{return {kind::neq        ,std::move(l),std::move(r)};}
  static binary_operation  make_lt(         operand&&  l, operand&&  r) noexcept{return {kind::lt         ,std::move(l),std::move(r)};}
  static binary_operation  make_lteq(       operand&&  l, operand&&  r) noexcept{return {kind::lteq       ,std::move(l),std::move(r)};}
  static binary_operation  make_gt(         operand&&  l, operand&&  r) noexcept{return {kind::gt         ,std::move(l),std::move(r)};}
  static binary_operation  make_gteq(       operand&&  l, operand&&  r) noexcept{return {kind::gteq       ,std::move(l),std::move(r)};}
  static binary_operation  make_logical_and(operand&&  l, operand&&  r) noexcept{return {kind::logical_and,std::move(l),std::move(r)};}
  static binary_operation  make_logical_or( operand&&  l, operand&&  r) noexcept{return {kind::logical_or ,std::move(l),std::move(r)};}
  static binary_operation  make_bit_and(    operand&&  l, operand&&  r) noexcept{return {kind::bit_and    ,std::move(l),std::move(r)};}
  static binary_operation  make_bit_or(     operand&&  l, operand&&  r) noexcept{return {kind::bit_or     ,std::move(l),std::move(r)};}
  static binary_operation  make_bit_xor(    operand&&  l, operand&&  r) noexcept{return {kind::bit_xor    ,std::move(l),std::move(r)};}

};




class
call_operation
{
  std::string  m_function_name;

  std::vector<operand>  m_arguments;

public:
  call_operation(std::string_view  fn_name, std::initializer_list<operand>  args) noexcept:
  m_function_name(fn_name), m_arguments(args){}

  value  evaluate(execution_frame&  frm) const noexcept;

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

  struct result{
    static constexpr int      test_flag = 1;
    static constexpr int  constant_flag = 2;

    gbstd::status_value<int>  m_status;

    value  m_value=value::make_undefined();

    result() noexcept{}
  };

  std::unique_ptr<result>  m_result=std::make_unique<result>();

public:
  operation() noexcept{}

  operation(const operation&   rhs) noexcept{assign(rhs);}
  operation(      operation&&  rhs) noexcept{assign(std::move(rhs));}
 ~operation(){clear();}

  operation&  operator=(const operation&   rhs) noexcept{return assign(rhs);}
  operation&  operator=(      operation&&  rhs) noexcept{return assign(std::move(rhs));}

  operation&  assign(const operation&   rhs) noexcept;
  operation&  assign(      operation&&  rhs) noexcept;

  void  clear() noexcept;

  operation&  set_content(unary_operation&&     un) noexcept;
  operation&  set_content(binary_operation&&   bin) noexcept;
  operation&  set_content(call_operation&&     cal) noexcept;

  operation&          set_label(std::string_view  lb)       noexcept{       m_label = lb;  return *this;}
  const std::string&  get_label(                    ) const noexcept{return m_label                    ;}

  const unary_operation&   get_unary()  const noexcept{return m_data.un;}
  const binary_operation&  get_binary() const noexcept{return m_data.bin;}
  const call_operation&    get_call()   const noexcept{return m_data.cal;}

  bool  is_unary()  const noexcept{return m_kind == kind::unary;}
  bool  is_binary() const noexcept{return m_kind == kind::binary;}
  bool  is_call()   const noexcept{return m_kind == kind::call;}

  bool  is_constant(const function&  fn) const noexcept;

  const value&  evaluate(execution_frame&  frm) const noexcept;

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
  codeline(const codeline&   rhs) noexcept{assign(rhs);}
  codeline(      codeline&&  rhs) noexcept{assign(std::move(rhs));}
 ~codeline(){clear();}

  codeline&  operator=(const codeline&   rhs) noexcept{return assign(rhs);}
  codeline&  operator=(      codeline&&  rhs) noexcept{return assign(std::move(rhs));}

  codeline&  assign(const codeline&   rhs) noexcept;
  codeline&  assign(      codeline&&  rhs) noexcept;

  void  clear() noexcept;

  codeline&  set_content(store_instruction&&   st) noexcept;
  codeline&  set_content(branch_instruction&&  br) noexcept;
  codeline&  set_content(return_instruction&&  ret) noexcept;
  codeline&  set_content(operation&&            op) noexcept;

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
variable_initializer
{
  std::string  m_label;

  operand  m_operand;

public:
  variable_initializer() noexcept{}
  variable_initializer(std::string_view  lb, operand&&  o) noexcept: m_label(lb), m_operand(std::move(o)){}

  const std::string&  get_label() const noexcept{return m_label;}

  value  get_value(execution_frame&  frm) const noexcept{return m_operand.evaluate(frm);}

};


using variable_initializer_list = std::vector<variable_initializer>;


class
function
{
  space*  m_space=nullptr;

  std::string  m_name;

  variable_initializer_list  m_variable_initializer_list;

  std::vector<codeline>   m_codelines;

  std::vector<entry_point>   m_entry_point_list;

public:
  function(space&  sp, std::string_view  name) noexcept: m_space(&sp), m_name(name){}

  const codeline&  operator[](int  i) const noexcept{return m_codelines[i];}

  int  get_number_of_codelines() const noexcept{return m_codelines.size();}

  const std::string&  get_name() const noexcept{return m_name;}

  const variable_initializer_list&  get_variable_initializer_list() const noexcept{return m_variable_initializer_list;}

  function&  append_variable_initializer(std::string_view  lb, operand&&  o) noexcept;
  function&  append_entry_point(std::string_view  lb) noexcept;

  space&  get_space() const noexcept{return *m_space;}

  codeline&  create_codeline() noexcept;

  const operation*  find_operation(std::string_view  label) const noexcept;

  const entry_point*  find_entry_point(std::string_view  label) const noexcept;

  void  print() const noexcept;

};




}




#endif




