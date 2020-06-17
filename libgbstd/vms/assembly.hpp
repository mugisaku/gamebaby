#ifndef gbstd_vm_assembly_HPP
#define gbstd_vm_assembly_HPP


#include"libgbstd/typesystem.hpp"
#include"libgbstd/parser.hpp"
#include"libgbstd/vms/expression.hpp"


namespace gbstd{




enum class
asm_opcode
{
  nop,
  addi,
  addf,
  subi,
  subf,
  muli,
  mulf,
  divi,
  divf,
  rem,
  shl,
  shr,
  band,
  bor,
  bxor,
  eq,
  neq,
  lti,
  ltf,
  lteqi,
  lteqf,
  gti,
  gtf,
  gteqi,
  gteqf,
  land,
  lor,
  bnot,
  lnot,
  negi,
  negf,

  brz,
  brnz,

   jmp,
  rjmp,

  pshz,
  pshnz,
  pshi8,
  pshi16,
  pshi32,
  pshi64,

  ld8,
  ld16,
  ld32,
  ld64,

  ldpc,
  ldbp,
  ldsp,

  sti8,
  sti16,
  sti32,
  sti64,
  stf32,
  stf64,

  stpc,
  stbp,
  stsp,

  pop,
  dup,
  swp,

  i2f,
  f2i,

};


class asm_line;


class
asm_parameter
{
  enum class kinds{
    null, label, integer, fpn, reference,
  } m_kind=kinds::null;

  union data{
    std::string  lb;
    int64_t       i;
    double        f;

    const asm_line*  ln;

    data() noexcept{}
   ~data(){}
  } m_data;

public:
  asm_parameter() noexcept{}
  asm_parameter(const asm_parameter&   rhs) noexcept{assign(rhs);}
  asm_parameter(      asm_parameter&&  rhs) noexcept{assign(std::move(rhs));}
 ~asm_parameter(){clear();}

  template<class...  Args>
  explicit asm_parameter(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class...  Args>
  asm_parameter&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  asm_parameter&  assign(const asm_parameter&   rhs) noexcept;
  asm_parameter&  assign(      asm_parameter&&  rhs) noexcept;
  asm_parameter&  assign(int64_t  i) noexcept;
  asm_parameter&  assign(double   f) noexcept;
  asm_parameter&  assign(std::string_view  sv) noexcept;
  asm_parameter&  assign(const asm_line&  ln) noexcept;

  asm_parameter&  clear() noexcept;

  bool  is_label()     const noexcept{return m_kind == kinds::label;}
  bool  is_integer()   const noexcept{return m_kind == kinds::integer;}
  bool  is_fpn()       const noexcept{return m_kind == kinds::fpn;}
  bool  is_reference() const noexcept{return m_kind == kinds::reference;}

  const std::string&  get_label()     const noexcept{return m_data.lb;}
  int64_t             get_integer()   const noexcept{return m_data.i;}
  double              get_fpn()       const noexcept{return m_data.f;}
  const asm_line&     get_reference() const noexcept{return *m_data.ln;}

  void  print() const noexcept;

};


class
asm_line
{
  int64_t  m_number=0;

  asm_opcode  m_opcode;

  asm_parameter  m_parameter;

  std::string  m_comment;

public:
  asm_line(asm_opcode  opco, asm_parameter&&  para) noexcept:
  m_opcode(opco), m_parameter(std::move(para)){}

  int64_t  get_number() const noexcept{return m_number;}

  asm_opcode  get_opcode() const noexcept{return m_opcode;}

  const asm_parameter&  get_parameter() const noexcept{return m_parameter;}

  void  set_comment(std::string_view  sv) noexcept{m_comment = sv;}

  void  print() const noexcept;

};


class
asm_symbol
{
  std::string  m_name;

  typesystem::type_info  m_type_info;

  int64_t  m_offset;
};


class
asm_context
{
  std::vector<asm_symbol>  m_symbol_list;

  struct block{
    std::vector<std::string>  m_label_list;

    std::list<asm_line>  m_line_list;
  };

  std::vector<block>  m_block_list;

public:
  asm_context() noexcept{m_block_list.emplace_back();}

  void  add_label(const char*  fmt, ...) noexcept;

  void  add_comment(const char*  fmt, ...) noexcept;

  void  add_line(asm_opcode  opco) noexcept{m_block_list.back().m_line_list.emplace_back(opco,asm_parameter(static_cast<int64_t>(0)));}

  void  add_pshi(int64_t  i) noexcept;
  void  add_pshi(const char*  fmt, ...) noexcept;

  void  print() const noexcept;

};


class
asm_processor
{
  static constexpr int   m_stack_size = 1024;

  uint8_t  m_memory[1024*10];

  int64_t  m_pc=0;
  int64_t  m_sp=0;
  int64_t  m_bp=0;

  void  put_i8( int64_t  address, int8_t   i) noexcept;
  void  put_i16(int64_t  address, int16_t  i) noexcept;
  void  put_i32(int64_t  address, int32_t  i) noexcept;
  void  put_i64(int64_t  address, int64_t  i) noexcept;
  void  put_f32(int64_t  address, float    f) noexcept;
  void  put_f64(int64_t  address, double   f) noexcept;

  int8_t   get_i8( int64_t  address) noexcept;
  int16_t  get_i16(int64_t  address) noexcept;
  int32_t  get_i32(int64_t  address) noexcept;
  int64_t  get_i64(int64_t  address) noexcept;
  double   get_f32(int64_t  address) noexcept;
  double   get_f64(int64_t  address) noexcept;

  void  put_top_i64(int64_t  i) noexcept;
  void  put_top_f64(double   f) noexcept;

  int64_t  get_top_i64() noexcept;
  double   get_top_f64() noexcept;

  void  push_i64(int64_t  i) noexcept;
  void  push_f64(double   f) noexcept;

  int64_t  pop_i64() noexcept;
  double   pop_f64() noexcept;

  int8_t   read_i8()  noexcept;
  int16_t  read_i16() noexcept;
  int32_t  read_i32() noexcept;
  int64_t  read_i64() noexcept;

  template<class  T>
  void
  operate_unary_i(T  fn) noexcept{
    auto  v = get_top_i64();

    put_top_i64(fn(v));
  }


  template<class  T>
  void
  operate_unary_f(T  fn) noexcept{
    auto  v = get_top_f64();

    put_top_f64(fn(v));
  }


  template<class  T>
  void
  operate_binary_i(T  fn) noexcept{
    auto  r = get_top_i64();

    pop_i64();

    auto  l = get_top_i64();

    put_top_i64(fn(l,r));
  }


  template<class  T>
  void
  operate_binary_f(T  fn) noexcept{
    auto  r = get_top_f64();

    pop_i64();

    auto  l = get_top_f64();

    put_top_f64(fn(l,r));
  }

public:
  void  reset(int64_t  entry_point) noexcept;

  void  read_binary(const std::vector<uint8_t>&  bin) noexcept;

  void  step();

};




}




#endif




