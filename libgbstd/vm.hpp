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


namespace gbstd{




class evaluation_context;


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
  static constexpr value  make_defined() noexcept{return {kind::undefined,0};}
  static constexpr value  make_data(int  i) noexcept{return {kind::data,i};}
  static constexpr value  make_null_pointer() noexcept{return {kind::null_pointer,0};}
  static constexpr value  make_u8_pointer(  int  addr) noexcept{return {kind::u8_pointer,addr};}
  static constexpr value  make_u16_pointer(  int  addr) noexcept{return {kind::u16_pointer,addr};}
  static constexpr value  make_u32_pointer(  int  addr) noexcept{return {kind::u32_pointer,addr};}
  static constexpr value  make_pointer(  int  addr) noexcept{return {kind::pointer,addr};}

};


class
operand
{
  enum class kind{
    null,
    operation,
    value,
  } m_kind=kind::null;

  union data{
    std::string  s;
    value        v;

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

  bool  is_operation() const noexcept{return m_kind == kind::operation;}
  bool  is_value()     const noexcept{return m_kind == kind::value;}

  value  evaluate(evaluation_context&  ctx) const noexcept;

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


class codeline;
class block;
class function;
class space;


class
space
{
  memory*  m_memory;

  std::vector<std::unique_ptr<function>>  m_function_table;

public:
  memory&  get_memory() const noexcept{return *m_memory;}

  function&  create_function(std::string_view  name) noexcept;

  const function*  find_function(std::string_view  name) const noexcept;

  value  evaluate(std::string_view  name) const noexcept;

};


class
argument
{
  std::string  m_label;

  value  m_value;

public:
  argument(std::string_view  label, value  v) noexcept: m_label(label), m_value(v){}

  const value*  operator->() const noexcept{return &m_value;}

  const std::string&  get_name() const noexcept{return m_label;}
  const value&       get_value() const noexcept{return m_value;}

};


class
evaluation_frame
{
  const function&  m_function;

  std::vector<argument>  m_argument_list;

  const block*  m_current_block=nullptr;
  const block*  m_previous_block=nullptr;

  const codeline*  m_current_codeline=nullptr;
  const codeline*  m_end_codeline    =nullptr;

public:
  evaluation_frame(const function&  fn, std::initializer_list<argument>  args={}) noexcept;

  operator bool() const noexcept{return m_current_codeline < m_end_codeline;}

  const function&  get_function() const noexcept{return m_function;}

  const std::vector<argument>&  get_argument_list() const noexcept{return m_argument_list;}

  evaluation_frame&  seek_block(std::string_view  label) noexcept;

  const block*   get_current_block() const noexcept{return m_current_block;}
  const block*  get_previous_block() const noexcept{return m_previous_block;}

  const codeline&  get_codeline() const noexcept{return *m_current_codeline;}

  evaluation_frame&  advance() noexcept;

};


class
evaluation_context
{
  std::vector<evaluation_frame>  m_frame_stack;

  void  store(value  dst, value  src) noexcept;
  void  step(const codeline&  codeln) noexcept;

public:
  evaluation_context(const function&  fn) noexcept;

  operator bool() const noexcept{return m_frame_stack.size();}

  evaluation_frame&  operator *() noexcept{return  m_frame_stack.back();}
  evaluation_frame*  operator->() noexcept{return &m_frame_stack.back();}

  evaluation_frame&  push(evaluation_frame&&  frm) noexcept{  m_frame_stack.emplace_back(std::move(frm));  return m_frame_stack.back();}

  void  pop() noexcept{m_frame_stack.pop_back();}

  void  run() noexcept;

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

};


class
return_instruction
{
  operand  m_operand;

public:
  return_instruction(operand&&  o) noexcept:
  m_operand(std::move(o)){}

  const operand&  get_operand() const noexcept{return m_operand;}

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
  value  evaluate(evaluation_context&  ctx) const noexcept;

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
  value  evaluate(evaluation_context&  ctx) const noexcept;

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
phi_entry
{
  std::string  m_label;

  operand  m_operand;

public:
  phi_entry() noexcept{}
  phi_entry(std::string_view  label, operand&&  o) noexcept: m_label(label), m_operand(std::move(o)){}

  const std::string&  get_label() const noexcept{return m_label;}

  value  evaluate(evaluation_context&  ctx) const noexcept;

};


class
phi_operation
{
  std::vector<phi_entry>  m_entries;

public:
  phi_operation() noexcept{}

  value  evaluate(evaluation_context&  ctx) const noexcept;

};


class
call_operation
{
  std::string  m_function_name;

  std::vector<operand>  m_arguments;

public:
  call_operation(std::string_view  fn_name, std::initializer_list<operand>  args) noexcept:
  m_function_name(fn_name), m_arguments(args){}

  value  evaluate(evaluation_context&  ctx) const noexcept;

};


class
operation
{
  enum class kind{
    null,
    unary,
    binary,
    phi,
    call,

  } m_kind=kind::null;


  union data{
    unary_operation    un;
    binary_operation  bin;
    phi_operation     phi;
    call_operation    cal;

    data() noexcept{}
   ~data(){}
  } m_data;


  std::string  m_label;

  std::unique_ptr<value>  m_value=std::make_unique<value>();

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
  operation&  set_content(phi_operation&&      phi) noexcept;
  operation&  set_content(call_operation&&     cal) noexcept;

  operation&          set_label(std::string_view  lb)       noexcept{       m_label = lb;  return *this;}
  const std::string&  get_label(                    ) const noexcept{return m_label                    ;}

  const unary_operation&   get_unary()  const noexcept{return m_data.un;}
  const binary_operation&  get_binary() const noexcept{return m_data.bin;}
  const phi_operation&     get_phi()    const noexcept{return m_data.phi;}
  const call_operation&    get_call()   const noexcept{return m_data.cal;}

  bool  is_unary()  const noexcept{return m_kind == kind::unary;}
  bool  is_binary() const noexcept{return m_kind == kind::binary;}
  bool  is_phi()    const noexcept{return m_kind == kind::phi;}
  bool  is_call()   const noexcept{return m_kind == kind::call;}

  const value&  evaluate(evaluation_context&  ctx) const noexcept;

};


class
codeline
{
  enum class kind{
    null,
    label,
    store_instruction,
    branch_instruction,
    return_instruction,
    operation,
  } m_kind=kind::null;

  block*  m_block=nullptr;

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
  codeline(block&  blk) noexcept: m_block(&blk){}
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

  block&  get_block() const noexcept{return *m_block;}

  bool  is_null()               const noexcept{return m_kind == kind::null;}
  bool  is_store_instruction()  const noexcept{return m_kind == kind::store_instruction;}
  bool  is_branch_instruction() const noexcept{return m_kind == kind::branch_instruction;}
  bool  is_return_instruction() const noexcept{return m_kind == kind::return_instruction;}
  bool  is_operation()          const noexcept{return m_kind == kind::operation;}

};




class
block
{
  function*  m_function=nullptr;

  std::string  m_label;

  std::vector<codeline>  m_codelines;

public:
  block(function&  fn, std::string_view  lb) noexcept: m_function(&fn), m_label(lb){}

  const std::string&  get_label() const noexcept{return m_label;}

  function&  get_function() const noexcept{return *m_function;}

  const operation*  find_operation(std::string_view  label) const noexcept;

  codeline&  create_codeline() noexcept;

  const codeline*  begin() const noexcept{return m_codelines.data()                   ;}
  const codeline*    end() const noexcept{return m_codelines.data()+m_codelines.size();}

};


class
parameter
{
public:
  std::string  m_type_name;

  std::string  m_label;

};


class
function
{
  space*  m_space=nullptr;

  std::string  m_name;

  std::vector<parameter>  m_parameter_list;

  std::vector<std::unique_ptr<block>>  m_blocks;

public:
  function(space&  sp, std::string_view  name) noexcept: m_space(&sp), m_name(name){}

  const std::string&  get_name() const noexcept{return m_name;}

  space&  get_space() const noexcept{return *m_space;}

  block&  create_block(std::string_view  label) noexcept;

  const block*      find_block(    std::string_view  label) const noexcept;
  const operation*  find_operation(std::string_view  label) const noexcept;

  const block&  get_entry_block() const noexcept{return *m_blocks.front();}

};




}




#endif




