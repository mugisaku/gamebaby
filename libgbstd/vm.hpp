#ifndef gbstd_vm_HPP
#define gbstd_vm_HPP


#include"libgbstd/syntax.hpp"
#include<cinttypes>




namespace gbstd{




constexpr size_t  max_size(size_t  sz) noexcept{return (sz+7)/8*8;}


class
vm_error
{
public:
  std::string  m_comment;

  vm_error(std::string_view  sv) noexcept{m_comment = sv;}

};


class
vm_type_info
{
  int  m_letter;
  int  m_size;

public:
  constexpr vm_type_info(int  l='n', int  sz=0) noexcept: m_letter(l), m_size(sz){}

  constexpr bool  is_undefined() const noexcept{return m_letter == 'n';}
  constexpr bool  is_void()      const noexcept{return m_letter == 'v';}
  constexpr bool  is_integer()   const noexcept{return m_letter == 'i';}
  constexpr bool  is_unsigned_integer()   const noexcept{return m_letter == 'u';}
  constexpr bool  is_floating()  const noexcept{return m_letter == 'f';}
  constexpr bool  is_object()    const noexcept{return m_letter == 'o';}

  constexpr int      size() const noexcept{return m_size;}
  constexpr int  max_size() const noexcept{return gbstd::max_size(m_size);}

  void  print() const noexcept;

};




using vm_address = int64_t;
using vm_offset  = int64_t;
using vm_pointer = int64_t;




enum class
vm_opcode
{
  nop,

  trf,

  add,sub,mul,div,rem,
  fadd,fsub,fmul,fdiv,frem,
  eq,neq,lt,lteq,gt,gteq,
  feq,fneq,flt,flteq,fgt,fgteq,

  shl,shr,and_,or_,xor_,not_,

  logi_and,logi_or,logi_not,

  neg,fneg,

  br,jmp,

  cal,ret

};


std::u16string_view
get_string(vm_opcode  o) noexcept;


template<int  N>
class
vm_bitcode
{
  static constexpr uint64_t  bit_mask(uint64_t  mask, int  n) noexcept{return n? bit_mask((mask<<1)|1,n-1):mask;}

  static constexpr const uint64_t  m_bitmask = bit_mask(0,N);

  uint64_t  m_data=0;

public:
  static constexpr const int  bit_number = N;

  constexpr vm_bitcode(uint64_t  u=0) noexcept: m_data(u&m_bitmask){}

  vm_bitcode&  operator=(uint64_t  u) noexcept{return assign(u);}
  vm_bitcode&  assign(uint64_t  u) noexcept{  m_data = u&m_bitmask;  return *this;}

  constexpr uint64_t  data() const noexcept{return m_data;}

};


class
vm_register_spec
{
  int  m_attribute='g';

  uint16_t  m_index=0;

  static constexpr int  encode(int  attr) noexcept
  {
    return (attr == 'z')? 0b00
          :(attr == 'b')? 0b01
          :(attr == 's')? 0b10
          :0
          ;
  }

  static constexpr int  decode(int  code) noexcept
  {
    return (code == 0b00)? 'z'
          :(code == 0b01)? 'b'
          :(code == 0b10)? 's'
          :0
          ;
  }

public:
  static constexpr const int  bit_width = 18;

  constexpr vm_register_spec(int  attr, uint16_t  i) noexcept: m_attribute(attr), m_index(i){}
  constexpr vm_register_spec(vm_bitcode<bit_width>  bc) noexcept: m_attribute(decode(bc.data()>>16)), m_index((bc.data()&0xFFFF)<<8){}

  constexpr bool  is_from_zero() const noexcept{return m_attribute == 'z';}
  constexpr bool  is_from_bp()   const noexcept{return m_attribute == 'b';}
  constexpr bool  is_from_sp()   const noexcept{return m_attribute == 's';}

  constexpr int  index() const noexcept{return m_index;}

  constexpr vm_bitcode<bit_width>  bitcode() const noexcept{return vm_bitcode<bit_width>((encode(m_attribute)<<16)|m_index);}

};


constexpr auto  g_zero_rs = vm_register_spec('z',0);
constexpr auto  g_pc_rs   = vm_register_spec('z',1);
constexpr auto  g_bp_rs   = vm_register_spec('z',2);
constexpr auto  g_sp_rs   = vm_register_spec('z',3);


class
vm_instruction
{
  uint64_t  m_data=0;

public:
  constexpr vm_instruction() noexcept{}
  constexpr vm_instruction(vm_opcode  o) noexcept:
  m_data(static_cast<uint64_t>(o)<<58){}

  constexpr vm_instruction(vm_opcode  o, vm_register_spec  rs1_, vm_register_spec  rs3_) noexcept:
  m_data((static_cast<uint64_t>(o)<<58)|(rs1_.bitcode().data()<<36)|rs3_.bitcode().data()){}

  constexpr vm_instruction(vm_opcode  o, vm_register_spec  rs1_, vm_register_spec  rs2_, vm_register_spec  rs3_) noexcept:
  m_data((static_cast<uint64_t>(o)<<58)|(rs1_.bitcode().data()<<36)|(rs2_.bitcode().data()<<18)|rs3_.bitcode().data()){}

  constexpr vm_instruction(vm_opcode  o, uint32_t  u32, vm_register_spec  rs3_) noexcept:
  m_data((static_cast<uint64_t>(o)<<58)|(u32<<18)|rs3_.bitcode().data()){}

  constexpr vm_opcode  opcode() const noexcept{return static_cast<vm_opcode>(m_data>>58);}

  constexpr vm_register_spec  rs1() const noexcept{return vm_bitcode<18>(m_data>>36);}
  constexpr vm_register_spec  rs2() const noexcept{return vm_bitcode<18>(m_data>>18);}
  constexpr vm_register_spec  rs3() const noexcept{return vm_bitcode<18>(m_data    );}

  constexpr uint32_t  immu32() const noexcept{return (m_data>>18)&0xFFFFFFFF;}
  constexpr  int32_t  imm32()  const noexcept{return static_cast<int32_t>(immu32());}

};




class
vm_parameter
{
  vm_type_info  m_type_info;

  std::u16string  m_name;

public:
  vm_parameter() noexcept{}
  vm_parameter(vm_type_info  ti, std::u16string_view  nam) noexcept: m_type_info(ti), m_name(nam){}

  const vm_type_info&  type_info() const noexcept{return m_type_info;}

  const std::u16string&  name() const noexcept{return m_name;}

};


using vm_parameter_list = std::vector<vm_parameter>;


class
vm_value
{
  vm_type_info  m_type_info;

  union data{
     int64_t  i;
    uint64_t  u;
    double    f;

    uint8_t*  o;

    constexpr data(int64_t  i_=0) noexcept: i(i_){}
    constexpr data(uint64_t  u_) noexcept: u(u_){}
    constexpr data(double   f_) noexcept: f(f_){}

  } m_data;

  using counter_t = uint64_t;

  static constexpr size_t  size_of_counter = sizeof(counter_t);

  void  allocate(int  sz, const void*  o) noexcept;

public:
  constexpr vm_value() noexcept{}
  constexpr vm_value(int8_t   i) noexcept: m_type_info('i',1), m_data(static_cast<int64_t>(i)){}
  constexpr vm_value(int16_t  i) noexcept: m_type_info('i',2), m_data(static_cast<int64_t>(i)){}
  constexpr vm_value(int32_t  i) noexcept: m_type_info('i',4), m_data(static_cast<int64_t>(i)){}
  constexpr vm_value(int64_t  i) noexcept: m_type_info('i',8), m_data(i){}
  constexpr vm_value(uint8_t   u) noexcept: m_type_info('u',1), m_data(static_cast<uint64_t>(u)){}
  constexpr vm_value(uint16_t  u) noexcept: m_type_info('u',2), m_data(static_cast<uint64_t>(u)){}
  constexpr vm_value(uint32_t  u) noexcept: m_type_info('u',4), m_data(static_cast<uint64_t>(u)){}
  constexpr vm_value(uint64_t  u) noexcept: m_type_info('u',8), m_data(u){}
  constexpr vm_value(float    f) noexcept: m_type_info('f',4), m_data(static_cast<double>(f)){}
  constexpr vm_value(double   f) noexcept: m_type_info('f',8), m_data(f){}
  vm_value(int  sz, const void*  o) noexcept{assign(sz,o);}
  vm_value(std::string_view  sv) noexcept{assign(sv);}
  vm_value(vm_type_info  ti) noexcept{assign(ti);}
  vm_value(const vm_value&   rhs) noexcept{assign(rhs);}
  vm_value(      vm_value&&  rhs) noexcept{assign(std::move(rhs));}
 ~vm_value(){unrefer();}

  vm_value&  operator=(const vm_value&   rhs) noexcept{return assign(rhs);}
  vm_value&  operator=(      vm_value&&  rhs) noexcept{return assign(std::move(rhs));}

  void  unrefer() noexcept;

  vm_value&  assign(const vm_value&   rhs) noexcept;
  vm_value&  assign(      vm_value&&  rhs) noexcept;
  vm_value&  assign(int  sz, const void*  o) noexcept;
  vm_value&  assign(std::string_view  sv) noexcept{return assign(sv.size(),sv.data());}
  vm_value&  assign(vm_type_info  ti) noexcept;

  constexpr vm_type_info  type_info() const noexcept{return m_type_info;}

  constexpr int64_t            integer() const noexcept{return m_data.i;}
  constexpr uint64_t  unsigned_integer() const noexcept{return m_data.u;}
  constexpr double            floating() const noexcept{return m_data.f;}

  const void*  object() const noexcept{return m_data.o+size_of_counter;}

  void  print() const noexcept;

};




class
vm_memory
{
  std::vector<uint8_t>  m_space;

  vm_value  make_integer_value(vm_address  addr, size_t  sz) const noexcept;
  vm_value  make_floating_value(vm_address  addr, size_t  sz) const noexcept;
  vm_value  make_object_value(vm_address  addr, size_t  sz) const noexcept;

        int64_t&  get(int64_t  i)       noexcept{return reinterpret_cast<      int64_t&>(m_space[sizeof(int64_t)*i]);}
  const int64_t&  get(int64_t  i) const noexcept{return reinterpret_cast<const int64_t&>(m_space[sizeof(int64_t)*i]);}

public:
  vm_memory() noexcept{}
  vm_memory(size_t  sz) noexcept: m_space(sz){}

  void  resize(size_t  sz) noexcept{m_space.resize(sz);}
  void  extend(size_t  sz) noexcept{m_space.resize(size()+sz);}
  void  reduce(size_t  sz) noexcept{m_space.resize(size()-sz);}

  size_t  size() const noexcept{return m_space.size();}

  void   store8(vm_address  addr, int64_t   i) noexcept{reinterpret_cast<int8_t&>( m_space[addr]) = i;}
  void  store16(vm_address  addr, int64_t  i) noexcept{reinterpret_cast<int16_t&>(m_space[addr]) = i;}
  void  store32(vm_address  addr, int64_t  i) noexcept{reinterpret_cast<int32_t&>(m_space[addr]) = i;}
  void  store64(vm_address  addr, int64_t  i) noexcept{reinterpret_cast<int64_t&>(m_space[addr]) = i;}
  void  store64hi(vm_address  addr, uint32_t  i) noexcept{reinterpret_cast<int64_t&>(m_space[addr]) |= static_cast<int64_t>(i)<<32;}
  void  store64lo(vm_address  addr, uint32_t  i) noexcept{reinterpret_cast<int64_t&>(m_space[addr])  = i;}
  void  storef32(vm_address  addr, double  f) noexcept{reinterpret_cast<float&>(m_space[addr]) = f;}
  void  storef64(vm_address  addr, double  f) noexcept{reinterpret_cast<double&>(m_space[addr]) = f;}

  void  store(vm_address  addr, const void*  ptr, size_t  sz) noexcept{std::memcpy(&m_space[addr],ptr,sz);}

  int64_t   load8(vm_address  addr) const noexcept{return reinterpret_cast<const int8_t&>( m_space[addr]);}
  int64_t  load16(vm_address  addr) const noexcept{return reinterpret_cast<const int16_t&>(m_space[addr]);}
  int64_t  load32(vm_address  addr) const noexcept{return reinterpret_cast<const int32_t&>(m_space[addr]);}
  int64_t  load64(vm_address  addr) const noexcept{return reinterpret_cast<const int64_t&>(m_space[addr]);}

  double  loadf32(vm_address  addr) const noexcept{return reinterpret_cast<const float&>(m_space[addr]);}
  double  loadf64(vm_address  addr) const noexcept{return reinterpret_cast<const double&>(m_space[addr]);}

  void  load(vm_address  addr, void*  ptr, size_t  sz) const noexcept{std::memcpy(ptr,&m_space[addr],sz);}

  void  copy(vm_address  src_addr, vm_type_info  src_ti, vm_address  dst_addr, vm_type_info  dst_ti) noexcept;

  int64_t&  pc() noexcept{return get(1);}
  int64_t&  bp() noexcept{return get(2);}
  int64_t&  sp() noexcept{return get(3);}

  const int64_t&  pc() const noexcept{return get(1);}
  const int64_t&  bp() const noexcept{return get(2);}
  const int64_t&  sp() const noexcept{return get(3);}

  vm_value  make_value(vm_address  addr, vm_type_info  ti) const noexcept;

};


void  print_branch_vm_ir(const syntax_branch&  br);




class
vm_operand
{
  enum class kind{
    null, integer, unsigned_integer, floating, identifier,
  } m_kind=kind::null;

  union data{
     int64_t         i;
    uint64_t         u;
    double           f;
    const char16_t*  s;
    constexpr data( int64_t  i_) noexcept: i(i_){}
    constexpr data(uint64_t  u_) noexcept: u(u_){}
    constexpr data(double  f_) noexcept: f(f_){}
    constexpr data(const char16_t*  s_) noexcept: s(s_){}
   ~data(){}
  } m_data;

  size_t  m_length=0;

public:
  constexpr vm_operand() noexcept: m_kind(kind::null), m_data(nullptr){}
  constexpr vm_operand( int64_t  i) noexcept: m_kind(kind::integer), m_data(i){}
  constexpr vm_operand(uint64_t  u) noexcept: m_kind(kind::unsigned_integer), m_data(u){}
  constexpr vm_operand(double  f) noexcept: m_kind(kind::floating), m_data(f){}
  constexpr vm_operand(std::u16string_view  sv) noexcept: m_kind(kind::identifier), m_data(sv.data()), m_length(sv.size()){}

  constexpr operator bool() const noexcept{return m_kind != kind::null;}

  constexpr bool  is_integer() const noexcept{return m_kind == kind::integer;}
  constexpr bool  is_unsigned_integer() const noexcept{return m_kind == kind::unsigned_integer;}
  constexpr bool  is_floating() const noexcept{return m_kind == kind::floating;}
  constexpr bool  is_identifier() const noexcept{return m_kind == kind::identifier;}

  constexpr int64_t             integer() const noexcept{return m_data.i;}
  constexpr uint64_t   unsigned_integer() const noexcept{return m_data.u;}
  constexpr double             floating() const noexcept{return m_data.f;}
  constexpr std::u16string_view  string() const noexcept{return std::u16string_view(m_data.s,m_length);}

  void  print() const noexcept;

};


using vm_operand_list = std::vector<vm_operand>;


class
vm_phi_element
{
  std::u16string  m_label;

  vm_operand  m_operand;

public:
  vm_phi_element() noexcept;
  vm_phi_element(std::u16string_view  lb, vm_operand&&  o) noexcept:
  m_label(lb), m_operand(std::move(o)){}

  const std::u16string&  label() const noexcept{return m_label;}

  const vm_operand&  operand() const noexcept{return m_operand;}

  void  print() const noexcept;

};


using vm_phi_element_list = std::vector<vm_phi_element>;


class
vm_transfer
{
  vm_type_info    m_src_type_info;
  std::u16string  m_src_label;

  vm_type_info    m_dst_type_info;
  std::u16string  m_dst_label;

public:
  vm_transfer() noexcept{}
  vm_transfer(vm_type_info  src_ti, std::u16string_view  src_lb, vm_type_info  dst_ti, std::u16string_view  dst_lb) noexcept:
  m_src_type_info(src_ti), m_src_label(src_lb), m_dst_type_info(dst_ti), m_dst_label(dst_lb){}

  vm_type_info           src_type_info() const noexcept{return m_src_type_info;}
  const std::u16string&  src_label() const noexcept{return m_src_label;}

  vm_type_info           dst_type_info() const noexcept{return m_dst_type_info;}
  const std::u16string&  dst_label() const noexcept{return m_dst_label;}

  void  print() const noexcept;

};


class
vm_call
{
  std::u16string  m_target;

  vm_operand_list  m_operand_list;

public:
  vm_call() noexcept{}
  vm_call(std::u16string_view  t, vm_operand_list&&  ls) noexcept: m_target(t), m_operand_list(std::move(ls)){}

  const std::u16string&  target() const noexcept{return m_target;}

  const vm_operand_list&  operand_list() const noexcept{return m_operand_list;}

  void  print() const noexcept;

};


class
vm_return
{
  vm_operand  m_operand;

public:
  vm_return() noexcept{}
  vm_return(vm_operand&&  o) noexcept: m_operand(std::move(o)){}

  const vm_operand&  operand() const noexcept{return m_operand;}

  void  print() const noexcept;

};


class
vm_jump
{
  std::u16string  m_label;

public:
  vm_jump() noexcept{}
  vm_jump(std::u16string_view  lb) noexcept: m_label(lb){}

  const std::u16string&  label() const noexcept{return m_label;}

  void  print() const noexcept;

};


class
vm_branch
{
  vm_operand  m_operand;

  std::u16string  m_if_true;
  std::u16string  m_if_false;

public:
  vm_branch() noexcept{}
  vm_branch(vm_operand&&  o, std::u16string_view  t, std::u16string_view  f) noexcept:
  m_operand(std::move(o)), m_if_true(t), m_if_false(f){}

  const vm_operand&  operand() const noexcept{return m_operand;}

  const std::u16string&  if_true() const noexcept{return m_if_true;}
  const std::u16string&  if_false() const noexcept{return m_if_false;}

  void  print() const noexcept;

};


class
vm_operation
{
  vm_opcode  m_opcode;

  vm_operand  m_left_operand;
  vm_operand  m_right_operand;

public:
  vm_operation() noexcept{}
  vm_operation(vm_opcode  op, vm_operand&&  l, vm_operand&&  r=vm_operand()) noexcept:
  m_opcode(op), m_left_operand(std::move(l)), m_right_operand(std::move(r)){}

  vm_opcode  opcode() const noexcept{return m_opcode;}

  const vm_operand&   left_operand() const noexcept{return m_left_operand;}
  const vm_operand&  right_operand() const noexcept{return m_right_operand;}

  void  print() const noexcept;

};


class
vm_register
{
  vm_type_info  m_type_info;

  std::u16string  m_label;

  enum class kind{
    null,
    call,
    operation,
    phi,

  } m_kind=kind::null;

  union data{
    vm_operation            op;
    vm_call                cal;
    vm_phi_element_list  phels;

    data() noexcept{}
   ~data(){}

  } m_data;

public:
  vm_register() noexcept{}
 ~vm_register() noexcept{clear();}

  template<class...  Args>
  vm_register(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  vm_register(const vm_register&   rhs) noexcept{assign(rhs);}
  vm_register(      vm_register&&  rhs) noexcept{assign(std::move(rhs));}

  vm_register&  operator=(const vm_register&   rhs) noexcept{return assign(rhs);}
  vm_register&  operator=(      vm_register&&  rhs) noexcept{return assign(std::move(rhs));}


  void  clear() noexcept;

  vm_register&  assign(const vm_register&   rhs) noexcept;
  vm_register&  assign(      vm_register&&  rhs) noexcept;
  vm_register&  assign(vm_type_info  ti, std::u16string_view  lb, vm_call&&  ret) noexcept;
  vm_register&  assign(vm_type_info  ti, std::u16string_view  lb, vm_operation&&  op) noexcept;
  vm_register&  assign(vm_type_info  ti, std::u16string_view  lb, vm_phi_element_list&&  phels) noexcept;

  vm_type_info  type_info() const noexcept{return m_type_info;}

  const std::u16string&  label() const noexcept{return m_label;}

  bool  is_call() const noexcept{return m_kind == kind::call;}
  bool  is_operation() const noexcept{return m_kind == kind::operation;}
  bool  is_phi() const noexcept{return m_kind == kind::phi;}

  const vm_operation                 operation() const noexcept{return m_data.op;}
  const vm_call&                          call() const noexcept{return m_data.cal;}
  const vm_phi_element_list&  phi_element_list() const noexcept{return m_data.phels;}

  void  print() const noexcept;

};


class
vm_line
{
  enum class kind{
    null, ret, br, jmp, trf, cal, reg,
  } m_kind=kind::null;

  union data{
    vm_return    ret;
    vm_branch     br;
    vm_transfer  trf;
    vm_jump      jmp;
    vm_call      cal;
    vm_register  reg;

    data() noexcept{}
   ~data(){}
  } m_data;

  int  m_position;

public:
  vm_line() noexcept{}
 ~vm_line() noexcept{clear();}

  template<class...  Args>
  vm_line(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  vm_line(const vm_line&   rhs) noexcept{assign(rhs);}
  vm_line(      vm_line&&  rhs) noexcept{assign(std::move(rhs));}

  template<class...  Args>
  vm_line&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  vm_line&  operator=(const vm_line&   rhs) noexcept{return assign(rhs);}
  vm_line&  operator=(      vm_line&&  rhs) noexcept{return assign(std::move(rhs));}

  void  clear() noexcept;

  vm_line&  assign(const vm_line&   rhs) noexcept;
  vm_line&  assign(      vm_line&&  rhs) noexcept;
  vm_line&  assign(vm_return&&  ret) noexcept;
  vm_line&  assign(vm_branch&&  br) noexcept;
  vm_line&  assign(vm_jump&&  jmp) noexcept;
  vm_line&  assign(vm_transfer&&  trf) noexcept;
  vm_line&  assign(vm_call&&  cal) noexcept;
  vm_line&  assign(vm_register&&  reg) noexcept;

  void  set_position(int  pos) noexcept{m_position = pos;}

  int  position() const noexcept{return m_position;}

  bool   is_return()   const noexcept{return m_kind == kind::ret;}
  bool   is_branch()   const noexcept{return m_kind == kind::br;}
  bool   is_jump()     const noexcept{return m_kind == kind::jmp;}
  bool   is_transfer() const noexcept{return m_kind == kind::trf;}
  bool   is_call()     const noexcept{return m_kind == kind::cal;}
  bool   is_register() const noexcept{return m_kind == kind::reg;}

  const vm_return&    get_return() const noexcept{return m_data.ret;}
  const vm_branch&    get_branch() const noexcept{return m_data.br;}
  const vm_jump&      get_jump() const noexcept{return m_data.jmp;}
  const vm_transfer&  get_transfer() const noexcept{return m_data.trf;}
  const vm_call&      get_call() const noexcept{return m_data.cal;}
  const vm_register&  get_register() const noexcept{return m_data.reg;}

  void  print() const noexcept;

};


class
vm_block
{
  std::u16string_view  m_label;

  std::vector<vm_line>  m_lines;

public:
  vm_block(std::u16string_view  lb) noexcept: m_label(lb){}

  std::u16string_view  label() const noexcept{return m_label;}

        std::vector<vm_line>&  lines()       noexcept{return m_lines;}
  const std::vector<vm_line>&  lines() const noexcept{return m_lines;}

};




class
vm_symbol
{
  vm_type_info  m_type_info;

  std::u16string  m_name;

  vm_offset  m_offset;

  int  m_attribute=0;

public:
  vm_symbol(vm_type_info  ti, std::u16string_view  name, int  attr) noexcept:
  m_type_info(ti), m_name(name), m_attribute(attr){}

  void  set_offset(vm_offset  off) noexcept{m_offset  = off;}
  void  add_offset(vm_offset  off) noexcept{m_offset += off;}

  vm_type_info  type_info() const noexcept{return m_type_info;}

  const std::u16string&  name() const noexcept{return m_name;}

  vm_offset  offset() const noexcept{return m_offset;}

  bool  is_global() const noexcept{return m_attribute == 'g';}
  bool  is_local()  const noexcept{return m_attribute == 'l';}

  void  print() const noexcept;

};


using vm_symbol_table = std::vector<vm_symbol>;


class
vm_function
{
  vm_type_info  m_type_info;

  std::u16string_view  m_name;

  std::u16string  m_label;

  std::vector<vm_block>  m_block_array;

  vm_symbol_table  m_symbol_table;

  size_t  m_argument_size=0;
  size_t  m_stack_allocation_size=0;

  vm_operation   construct_unary_operation(const syntax_branch&  br) noexcept;
  vm_operation  construct_binary_operation(const syntax_branch&  br) noexcept;

  vm_opcode   construct_unary_opcode(const syntax_branch&  br) noexcept;
  vm_opcode  construct_binary_opcode(const syntax_branch&  br) noexcept;

  void  construct_address_operation(const syntax_branch&  br) noexcept;

  vm_phi_element       construct_phi_element(const syntax_branch&  br) noexcept;
  vm_phi_element_list  construct_phi_operation(const syntax_branch&  br) noexcept;

  vm_register  construct_operations(const syntax_branch&  br, vm_type_info  ti, std::u16string_view  lb) noexcept;

  vm_register  construct_register(const syntax_branch&  br) noexcept;
  vm_return    construct_return(const syntax_branch&  br) noexcept;
  vm_branch    construct_branch(const syntax_branch&  br) noexcept;
  vm_transfer  construct_store(const syntax_branch&  br) noexcept;

  vm_call  construct_call(const syntax_branch&  br) noexcept;

  vm_operand_list  construct_operand_list(const syntax_branch&  br) noexcept;

  void  construct_label_statement(const syntax_branch&  br) noexcept;
  void  construct_statement(const syntax_branch&  br) noexcept;
  void  construct_statement_list(const syntax_branch&  br) noexcept;

  static vm_type_info       construct_type_info(const syntax_branch&  br) noexcept;
  static vm_operand         construct_operand(const syntax_branch&  br) noexcept;
  static vm_parameter       construct_parameter(const syntax_branch&  br) noexcept;
  static vm_parameter_list  construct_parameter_list(const syntax_branch&  br) noexcept;

public:
  vm_function(const syntax_branch&  br) noexcept{assign(br);}

  vm_function&  operator=(const syntax_branch&  br) noexcept{return assign(br);}

  vm_function&  assign(const syntax_branch&  br) noexcept;

  vm_type_info  type_info() const noexcept{return m_type_info;}

  const vm_block*  find_block(std::u16string_view  label) const noexcept;

  std::u16string_view  name() const noexcept{return m_name;}

  const std::vector<vm_block>&  block_array() const noexcept{return m_block_array;}

  const vm_symbol_table&  symbol_table() const noexcept{return m_symbol_table;}

  size_t          argument_size() const noexcept{return m_argument_size;}
  size_t  stack_allocation_size() const noexcept{return m_stack_allocation_size;}

  vm_line&  last_line() noexcept{return m_block_array.back().lines().back();}

  vm_line&    add_line(vm_line&&  ln) noexcept;
  vm_block&  add_block(std::u16string_view  lb) noexcept;

  int  finalize(int  pos) noexcept;

  int  entry_position() const noexcept{return m_block_array.front().lines().front().position();}

  void  print() const noexcept;

};


class
vm_jobfield
{
  vm_symbol_table  m_symbol_table;

  std::vector<vm_function>  m_function_table;

public:
  vm_jobfield(const syntax_branch&  br) noexcept{assign(br);}

  vm_jobfield&  operator=(const syntax_branch&  br) noexcept{return assign(br);}

  vm_jobfield&  assign(const syntax_branch&  br) noexcept;

  const vm_function*  find_function(std::u16string_view  name) const noexcept;
  const vm_symbol*      find_symbol(std::u16string_view  name) const noexcept;

  const std::vector<vm_function>&  function_table() const noexcept{return m_function_table;}

  void  finalize() noexcept;

  vm_value  run(std::u16string_view  main_fn_name) const noexcept;

  void  print() const noexcept;

};


class
vm_execution
{
  const vm_jobfield*  m_jobfield=nullptr;

  std::vector<const vm_line*>  m_lines;

  vm_value  m_return_value;

  vm_memory  m_memory;

  int64_t  m_pc;
  int64_t  m_bp;
  int64_t  m_sp;

  std::vector<const vm_function*>  m_function_stack;

  bool  m_halt_flag;

  int64_t  integer(const vm_operand&  o) const;
  double  floating(const vm_operand&  o) const;

  void  store(std::u16string_view  dst, vm_value  v);
  void  store(vm_address  addr, vm_type_info  ti, vm_value  v);

  vm_value  process(const vm_phi_element_list&  phels) noexcept;
  vm_value  process(const vm_operation&  op) noexcept;

  void  jump(std::u16string_view  dst);

public:
  vm_execution() noexcept{}
  vm_execution(const vm_jobfield&  jf) noexcept{assign(jf);}

  vm_execution&  operator=(const vm_jobfield&  jf) noexcept{return assign(jf);}

  vm_execution&  assign(const vm_jobfield&  jf) noexcept;

  bool  is_halted() const noexcept{return m_halt_flag;}

  const vm_symbol*  find_symbol(std::u16string_view  name) const noexcept;

  vm_value  get_value_by_name(std::u16string_view  name) const noexcept;
  vm_value  get_value_by_operand(const vm_operand&  o) const noexcept;

  void  reset(std::u16string_view  main_fn_name) noexcept;

  void  push_argument(vm_type_info  ti, const vm_operand&  o) noexcept;

  void  call(std::u16string_view  fn_name, vm_address  retval_addr, const vm_operand*  o, int  n) noexcept;

  void  process(const vm_return&  ret) noexcept;
  void  process(const vm_branch&  br) noexcept;
  void  process(const vm_call&  cal, std::u16string_view  dst) noexcept;
  void  process(const vm_transfer&  trf) noexcept;
  void  process(const vm_register&  reg) noexcept;

  void  process(const vm_line&  ln) noexcept;

  void  step() noexcept;

  void  halt() noexcept{m_halt_flag = true;}

  const vm_value&  run() noexcept;

  const vm_value&  get_return_value() const noexcept{return m_return_value;}

  void   print() const noexcept;

};




}




#endif




