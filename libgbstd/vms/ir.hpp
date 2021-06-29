#ifndef gbstd_vm_ir_HPP
#define gbstd_vm_ir_HPP


#include"libgbstd/syntax.hpp"




namespace gbstd{



constexpr size_t  max_size(size_t  sz) noexcept{return (sz+7)/8*8;}


class
ir_error
{
public:
  std::string  m_comment;

  ir_error(std::string_view  sv) noexcept{m_comment = sv;}

};


class
ir_type_info
{
  int  m_letter;
  int  m_size;

public:
  constexpr ir_type_info(int  l='v', int  sz=0) noexcept: m_letter(l), m_size(sz){}

  constexpr bool  is_void()     const noexcept{return m_letter == 'v';}
  constexpr bool  is_integer()  const noexcept{return m_letter == 'i';}
  constexpr bool  is_floating() const noexcept{return m_letter == 'f';}
  constexpr bool  is_object() const noexcept{return m_letter == 'o';}

  constexpr int      size() const noexcept{return m_size;}
  constexpr int  max_size() const noexcept{return gbstd::max_size(m_size);}

  void  print() const noexcept
  {
         if(m_letter == 'v'){printf("void");}
    else if(m_letter == 'i'){printf("i%d",8*m_size);}
    else if(m_letter == 'f'){printf("f%d",8*m_size);}
    else if(m_letter == 'o'){printf("o%d",m_size);}
    else {printf("unknown");}
  }

};




using ir_address = int64_t;




enum class
ir_opcode
{
  nop,

  cpy,

  st8,st16,st32,st64,

  ld32,
  ld64hi,
  ld64lo,

  add,sub,mul,div,rem,

  shl,shr,and_,or_,xor_,not_,

  eq,neq,lt,lteq,gt,gteq,

  logi_and,logi_or,logi_not,

  neg,

  br,jmp,

  adr,

  cal,ret

};


template<int  N>
class
ir_bitcode
{
  static constexpr uint64_t  bit_mask(uint64_t  mask, int  n) noexcept{return n? bit_mask((mask<<1)|1,n-1):mask;}

  static constexpr const uint64_t  m_bitmask = bit_mask(0,N);

  uint64_t  m_data=0;

public:
  static constexpr const int  bit_number = N;

  constexpr ir_bitcode(uint64_t  u=0) noexcept: m_data(u&m_bitmask){}

  ir_bitcode&  operator=(uint64_t  u) noexcept{return assign(u);}
  ir_bitcode&  assign(uint64_t  u) noexcept{  m_data = u&m_bitmask;  return *this;}

  constexpr uint64_t  data() const noexcept{return m_data;}

};


class
ir_register_spec
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

  constexpr ir_register_spec(int  attr, uint16_t  i) noexcept: m_attribute(attr), m_index(i){}
  constexpr ir_register_spec(ir_bitcode<bit_width>  bc) noexcept: m_attribute(decode(bc.data()>>16)), m_index((bc.data()&0xFFFF)<<8){}

  constexpr bool  is_from_zero() const noexcept{return m_attribute == 'z';}
  constexpr bool  is_from_bp()   const noexcept{return m_attribute == 'b';}
  constexpr bool  is_from_sp()   const noexcept{return m_attribute == 's';}

  constexpr int  index() const noexcept{return m_index;}

  constexpr ir_bitcode<bit_width>  bitcode() const noexcept{return ir_bitcode<bit_width>((encode(m_attribute)<<16)|m_index);}

};


constexpr auto  g_zero_rs = ir_register_spec('z',0);
constexpr auto  g_pc_rs   = ir_register_spec('z',1);
constexpr auto  g_bp_rs   = ir_register_spec('z',2);
constexpr auto  g_sp_rs   = ir_register_spec('z',3);


class
ir_instruction
{
  uint64_t  m_data=0;

public:
  constexpr ir_instruction() noexcept{}
  constexpr ir_instruction(ir_opcode  o) noexcept:
  m_data(static_cast<uint64_t>(o)<<58){}

  constexpr ir_instruction(ir_opcode  o, ir_register_spec  rs1_, ir_register_spec  rs3_) noexcept:
  m_data((static_cast<uint64_t>(o)<<58)|(rs1_.bitcode().data()<<36)|rs3_.bitcode().data()){}

  constexpr ir_instruction(ir_opcode  o, ir_register_spec  rs1_, ir_register_spec  rs2_, ir_register_spec  rs3_) noexcept:
  m_data((static_cast<uint64_t>(o)<<58)|(rs1_.bitcode().data()<<36)|(rs2_.bitcode().data()<<18)|rs3_.bitcode().data()){}

  constexpr ir_instruction(ir_opcode  o, uint32_t  u32, ir_register_spec  rs3_) noexcept:
  m_data((static_cast<uint64_t>(o)<<58)|(u32<<18)|rs3_.bitcode().data()){}

  constexpr ir_opcode  opcode() const noexcept{return static_cast<ir_opcode>(m_data>>58);}

  constexpr ir_register_spec  rs1() const noexcept{return ir_bitcode<18>(m_data>>36);}
  constexpr ir_register_spec  rs2() const noexcept{return ir_bitcode<18>(m_data>>18);}
  constexpr ir_register_spec  rs3() const noexcept{return ir_bitcode<18>(m_data    );}

  constexpr uint32_t  immu32() const noexcept{return (m_data>>18)&0xFFFFFFFF;}
  constexpr  int32_t  imm32()  const noexcept{return static_cast<int32_t>(immu32());}

};




class
ir_parameter
{
  ir_type_info  m_type_info;

  std::u16string  m_name;

public:
  ir_parameter() noexcept{}
  ir_parameter(ir_type_info  ti, std::u16string_view  nam) noexcept: m_type_info(ti), m_name(nam){}

  const ir_type_info&  type_info() const noexcept{return m_type_info;}

  const std::u16string&  name() const noexcept{return m_name;}

};


class
ir_value
{
  ir_type_info  m_type_info;

  union data{
    int64_t  i;
    double   f;

    uint8_t*  o;

    constexpr data(int64_t  i_=0) noexcept: i(i_){}
    constexpr data(double   f_) noexcept: f(f_){}

  } m_data;

  using counter_t = uint64_t;

  static constexpr size_t  size_of_counter = sizeof(counter_t);

  void  unrefer() noexcept;
  void  allocate(int  sz, const void*  o) noexcept;

public:
  constexpr ir_value() noexcept{}
  constexpr ir_value(int8_t   i) noexcept: m_type_info('i',1), m_data(static_cast<int64_t>(i)){}
  constexpr ir_value(int16_t  i) noexcept: m_type_info('i',2), m_data(static_cast<int64_t>(i)){}
  constexpr ir_value(int32_t  i) noexcept: m_type_info('i',4), m_data(static_cast<int64_t>(i)){}
  constexpr ir_value(int64_t  i) noexcept: m_type_info('i',8), m_data(i){}
  constexpr ir_value(float    f) noexcept: m_type_info('f',4), m_data(static_cast<double>(f)){}
  constexpr ir_value(double   f) noexcept: m_type_info('f',8), m_data(f){}
  ir_value(int  sz, const void*  o) noexcept{assign(sz,o);}
  ir_value(std::string_view  sv) noexcept{assign(sv);}
  ir_value(const ir_value&   rhs) noexcept{assign(rhs);}
  ir_value(      ir_value&&  rhs) noexcept{assign(std::move(rhs));}
 ~ir_value(){unrefer();}

  ir_value&  operator=(const ir_value&   rhs) noexcept{return assign(rhs);}
  ir_value&  operator=(      ir_value&&  rhs) noexcept{return assign(std::move(rhs));}

  ir_value&  assign(const ir_value&   rhs) noexcept;
  ir_value&  assign(      ir_value&&  rhs) noexcept;
  ir_value&  assign(int  sz, const void*  o) noexcept;
  ir_value&  assign(std::string_view  sv) noexcept{return assign(sv.size(),sv.data());}

  constexpr ir_type_info  type_info() const noexcept{return m_type_info;}

  constexpr int64_t  integer() const noexcept{return m_data.i;}
  constexpr double  floating() const noexcept{return m_data.f;}

  const void*  object() const noexcept{return m_data.o+size_of_counter;}

};




class
ir_memory
{
  std::vector<uint8_t>  m_space;

  ir_value  make_integer_value(ir_address  addr, size_t  sz) const noexcept;
  ir_value  make_floating_value(ir_address  addr, size_t  sz) const noexcept;
  ir_value  make_object_value(ir_address  addr, size_t  sz) const noexcept;

        int64_t&  get(int64_t  i)       noexcept{return reinterpret_cast<      int64_t&>(m_space[sizeof(int64_t)*i]);}
  const int64_t&  get(int64_t  i) const noexcept{return reinterpret_cast<const int64_t&>(m_space[sizeof(int64_t)*i]);}

public:
  ir_memory() noexcept{}
  ir_memory(size_t  sz) noexcept: m_space(sz){}

  void  resize(size_t  sz) noexcept{m_space.resize(sz);}

  size_t  size() const noexcept{return m_space.size();}

  void   store8(ir_address  addr, int64_t   i) noexcept{reinterpret_cast<int8_t&>( m_space[addr]) = i;}
  void  store16(ir_address  addr, int64_t  i) noexcept{reinterpret_cast<int16_t&>(m_space[addr]) = i;}
  void  store32(ir_address  addr, int64_t  i) noexcept{reinterpret_cast<int32_t&>(m_space[addr]) = i;}
  void  store64(ir_address  addr, int64_t  i) noexcept{reinterpret_cast<int64_t&>(m_space[addr]) = i;}
  void  store64hi(ir_address  addr, uint32_t  i) noexcept{reinterpret_cast<int64_t&>(m_space[addr]) |= static_cast<int64_t>(i)<<32;}
  void  store64lo(ir_address  addr, uint32_t  i) noexcept{reinterpret_cast<int64_t&>(m_space[addr])  = i;}
  void  storef32(ir_address  addr, double  f) noexcept{reinterpret_cast<float&>(m_space[addr]) = f;}
  void  storef64(ir_address  addr, double  f) noexcept{reinterpret_cast<double&>(m_space[addr]) = f;}

  void  store(ir_address  addr, const void*  ptr, size_t  sz) noexcept{std::memcpy(&m_space[addr],ptr,sz);}

  int64_t   load8(ir_address  addr) const noexcept{return reinterpret_cast<const int8_t&>( m_space[addr]);}
  int64_t  load16(ir_address  addr) const noexcept{return reinterpret_cast<const int16_t&>(m_space[addr]);}
  int64_t  load32(ir_address  addr) const noexcept{return reinterpret_cast<const int32_t&>(m_space[addr]);}
  int64_t  load64(ir_address  addr) const noexcept{return reinterpret_cast<const int64_t&>(m_space[addr]);}

  double  loadf32(ir_address  addr) const noexcept{return reinterpret_cast<const float&>(m_space[addr]);}
  double  loadf64(ir_address  addr) const noexcept{return reinterpret_cast<const double&>(m_space[addr]);}

  void  load(ir_address  addr, void*  ptr, size_t  sz) const noexcept{std::memcpy(ptr,&m_space[addr],sz);}

  void  copy(ir_address  src_addr, ir_type_info  src_ti, ir_address  dst_addr, ir_type_info  dst_ti) noexcept;

  int64_t&  pc() noexcept{return get(1);}
  int64_t&  bp() noexcept{return get(2);}
  int64_t&  sp() noexcept{return get(3);}

  const int64_t&  pc() const noexcept{return get(1);}
  const int64_t&  bp() const noexcept{return get(2);}
  const int64_t&  sp() const noexcept{return get(3);}

  ir_value  make_value(ir_address  addr, ir_type_info  ti) const noexcept;

};


class
ir_package
{
  std::vector<ir_instruction>  m_instruction_array;

  ir_memory  m_memory;

  int64_t  m_entry_point=0;

public:
  ir_package() noexcept{}
  ir_package(std::vector<ir_instruction>&&  iarr, ir_memory&&  mem, int64_t  ep) noexcept{assign(std::move(iarr),std::move(mem),ep);}

  const std::vector<ir_instruction>&  instruction_array() const noexcept{return m_instruction_array;}

  const ir_memory&  memory() const noexcept{return m_memory;}

  int64_t  entry_point() const noexcept{return m_entry_point;}

  ir_package&  assign(std::vector<ir_instruction>&&  iarr, ir_memory&&  mem, int64_t  ep) noexcept
  {
    m_instruction_array = std::move(iarr);

    m_memory = std::move(mem);

    m_entry_point = ep;

    return *this;
  }


};




ir_package  assemble(const syntax_branch&  br);

void  print_branch_as_ir(const syntax_branch&  br);


class
ir_processor
{
  const ir_instruction*  m_begin=nullptr;
  const ir_instruction*  m_end  =nullptr;

  ir_memory  m_memory;

  void  process(ir_instruction  instr) noexcept;

  ir_address  get_address(ir_register_spec  rs) const noexcept;

  void  cal(ir_address  fnptr_addr, ir_address  retval_addr) noexcept;
  void  ret(                                                ) noexcept;

public:
  ir_processor() noexcept{}

  ir_processor&  assign(const ir_package&  pk) noexcept;

  bool   step() noexcept;
  void    run() noexcept;

};




}




#endif




