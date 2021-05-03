#ifndef gbstd_vm_ir_operation_HPP
#define gbstd_vm_ir_operation_HPP


#include"libgbstd/syntax.hpp"
#include"libgbstd/vms/expression.hpp"




namespace gbstd{




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
  int  m_letter=0;
  int  m_size=0;

public:
  constexpr ir_type_info() noexcept{}
  constexpr ir_type_info(int  l, int  sz) noexcept: m_letter(l), m_size(sz){}

  constexpr bool  is_void()     const noexcept{return m_letter == 'v';}
  constexpr bool  is_integer()  const noexcept{return m_letter == 'i';}
  constexpr bool  is_floating() const noexcept{return m_letter == 'f';}

  constexpr int  size() const noexcept{return m_size;}

  void  print() const noexcept
  {
         if(m_letter == 'v'){printf("void");}
    else if(m_letter == 'i'){printf("i%d",8*m_size);}
    else if(m_letter == 'f'){printf("f%d",8*m_size);}
  }

};




class
ir_unary_operation
{
  small_string   m_first_opcode;
  small_string  m_second_opcode;

  std::u16string  m_operand;

public:
  small_string   first_opcode() const noexcept{return m_first_opcode;};
  small_string  second_opcode() const noexcept{return m_second_opcode;};

  const std::u16string&  operand() const noexcept{return m_operand;}

};


class
ir_binary_operation
{
  small_string   m_first_opcode;
  small_string  m_second_opcode;

  std::u16string  m_first_operand;
  std::u16string  m_second_operand;

public:
  small_string   first_opcode() const noexcept{return m_first_opcode;};
  small_string  second_opcode() const noexcept{return m_second_opcode;};

  const std::u16string&  first_operand() const noexcept{return m_first_operand;}
  const std::u16string&  second_operand() const noexcept{return m_second_operand;}

};


class
ir_define_operation
{
  enum class kind{
    integer,
    floating,
  } m_kind=kind::integer;

  union data{
    uint64_t  i;
    double    f;

    constexpr data(uint64_t  i_) noexcept: i(i_){}
    constexpr data(double  f_) noexcept: f(f_){}
  } m_data;

public:
  constexpr ir_define_operation(uint64_t  i) noexcept: m_kind(kind::integer), m_data(i){}
  constexpr ir_define_operation(double  f) noexcept: m_kind(kind::floating), m_data(f){}

  constexpr bool  is_integer()  const noexcept{return m_kind == kind::integer;}
  constexpr bool  is_floating() const noexcept{return m_kind == kind::floating;}

  constexpr uint64_t  integer() const noexcept{return m_data.i;}
  constexpr double   floating() const noexcept{return m_data.f;}

};


class
ir_load_operation
{
  ir_type_info  m_type_info;

  std::u16string  m_address_operand;

public:
  ir_load_operation(ir_type_info  ti, std::u16string_view  addro) noexcept: m_type_info(ti), m_address_operand(addro){}

  const ir_type_info&  type_info() const noexcept{return m_type_info;}

  const std::u16string&  address_operand() const noexcept{return m_address_operand;}

};


class
ir_address_operation
{
  std::u16string  m_identifier;

public:
  ir_address_operation(std::u16string_view  id) noexcept: m_identifier(id){}

  const std::u16string&  identifier() const noexcept{return m_identifier;}

};


class
ir_call_operation
{
  std::u16string  m_function_name;

  std::vector<std::u16string>  m_argument_list;

public:
  ir_call_operation(std::u16string_view  fnname, std::vector<std::u16string>&&  args) noexcept: m_function_name(fnname), m_argument_list(std::move(args)){}

  const std::u16string&  function_name() const noexcept{return m_function_name;}

  const std::vector<std::u16string>&  argument_list() const noexcept{return m_argument_list;}
  
};


class
ir_phi_element
{
  std::u16string    m_label;
  std::u16string  m_operand;

public:
  ir_phi_element(std::u16string_view   lb, std::u16string_view  o) noexcept: m_label(lb), m_operand(o){}

  const std::u16string&    label() const noexcept{return m_label;}
  const std::u16string&  operand() const noexcept{return m_operand;}

};


class
ir_phi_operation
{
  std::vector<ir_phi_element>  m_element_list;

public:
  ir_phi_operation(std::vector<ir_phi_element>&&  els) noexcept: m_element_list(std::move(els)){}

  const std::vector<ir_phi_element>&  element_list() const noexcept{return m_element_list;}

};




class
ir_operation
{
  enum class kind{
    null,
    define,
    unary,
    binary,
    load,
    address,
    call,
    phi,
  } m_kind=kind::null;

  union data{
    ir_unary_operation      un;
    ir_binary_operation    bin;
    ir_define_operation    def;
    ir_load_operation       ld;
    ir_address_operation  addr;
    ir_call_operation      cal;
    ir_phi_operation       phi;

    data() noexcept{}
   ~data(){}
  } m_data;

public:
  ir_operation() noexcept{}
  ir_operation(const ir_operation&   rhs) noexcept{assign(rhs);}
  ir_operation(      ir_operation&&  rhs) noexcept{assign(std::move(rhs));}
 ~ir_operation(){clear();}

  template<class...  Args>
  ir_operation(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  ir_operation&  operator=(const ir_operation&   rhs) noexcept{return assign(rhs);}
  ir_operation&  operator=(      ir_operation&&  rhs) noexcept{return assign(std::move(rhs));}

  template<class...  Args>
  ir_operation&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  ir_operation&  assign(const ir_operation&   rhs) noexcept;
  ir_operation&  assign(      ir_operation&&  rhs) noexcept;
  ir_operation&  assign(ir_unary_operation&&      un) noexcept;
  ir_operation&  assign(ir_binary_operation&&    bin) noexcept;
  ir_operation&  assign(ir_define_operation&&    def) noexcept;
  ir_operation&  assign(ir_load_operation&&       ld) noexcept;
  ir_operation&  assign(ir_address_operation&&  addr) noexcept;
  ir_operation&  assign(ir_call_operation&&      cal) noexcept;
  ir_operation&  assign(ir_phi_operation&&       phi) noexcept;

  void  clear() noexcept;

  bool  is_unary()   const noexcept{return m_kind == kind::unary;}
  bool  is_binary()  const noexcept{return m_kind == kind::binary;}
  bool  is_define()  const noexcept{return m_kind == kind::define;}
  bool  is_load()    const noexcept{return m_kind == kind::load;}
  bool  is_address() const noexcept{return m_kind == kind::address;}
  bool  is_call()    const noexcept{return m_kind == kind::call;}
  bool  is_phi()     const noexcept{return m_kind == kind::phi;}

  const ir_unary_operation&      unary() const noexcept{return m_data.un;}
  const ir_binary_operation&    binary() const noexcept{return m_data.bin;}
  const ir_define_operation&    define() const noexcept{return m_data.def;}
  const ir_load_operation&        load() const noexcept{return m_data.ld;}
  const ir_address_operation&  address() const noexcept{return m_data.addr;}
  const ir_call_operation&        call() const noexcept{return m_data.cal;}
  const ir_phi_operation&          phi() const noexcept{return m_data.phi;}

  void  print() const noexcept;

};




}




#endif




