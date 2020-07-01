#ifndef gbstd_vm_ir_HPP
#define gbstd_vm_ir_HPP


#include"libgbstd/parser.hpp"
#include"libgbstd/vms/expression.hpp"




namespace gbstd{




class ir_operand;
class ir_operation;
class ir_phi_element;


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
  enum class kinds{
    null, integer, fpn, object,
  } m_kind=kinds::null;

  int  m_size=0;

  constexpr ir_type_info(kinds  k) noexcept: m_kind(k){}

  constexpr bool  is_obj(std::string_view  sv) noexcept{
    return((sv.size() > 3) &&
           (sv[0] == 'o')  &&
           (sv[1] == 'b')  &&
           (sv[2] == 'j'));
  }

  constexpr int  to_i(char  c) noexcept{
    return ((c >= '0') && (c <= '9'))? c-'0':0;
  }

  constexpr int  get_i(const char*  s, int  i) noexcept{
    return (s[0] == 0)? i: get_i(s+1,(i*10)+to_i(*s));
  }

  constexpr kinds  get_kind(std::string_view  sv) noexcept{
    return (sv == std::string_view("int"))?   kinds::integer
          :(sv == std::string_view("float"))? kinds::fpn
          :is_obj(sv)?                        kinds::object
          :kinds::null
          ;
  }

public:
  constexpr ir_type_info() noexcept{}
  constexpr ir_type_info(int  sz) noexcept: m_kind(kinds::object), m_size(sz){}
  constexpr ir_type_info(std::string_view  sv) noexcept: m_kind(get_kind(sv)), m_size(get_i(sv.data(),0)){}

  constexpr bool  is_integer() const noexcept{return m_kind == kinds::integer;}
  constexpr bool  is_fpn()     const noexcept{return m_kind == kinds::fpn;}
  constexpr bool  is_object()  const noexcept{return m_kind == kinds::object;}

  constexpr int  get_size()  const noexcept{return m_size;}

  static constexpr ir_type_info  integer_type_info() noexcept{return ir_type_info(kinds::integer);}
  static constexpr ir_type_info      fpn_type_info() noexcept{return ir_type_info(kinds::fpn);}
  static constexpr ir_type_info  object_type_info(int  sz) noexcept{return ir_type_info(sz);}

  void  print() const noexcept{
      switch(m_kind)
      {
    case(kinds::integer): printf("int");break;
    case(kinds::fpn    ): printf("float");break;
    case(kinds::object ): printf("obj%d",m_size);break;
      }
  }

};


class
ir_value
{
  ir_type_info  m_type_info;

  union data{
    int64_t   i;
    double    f;
    char*     o;
  } m_data;

public:
   ir_value() noexcept{}
   ir_value(ir_type_info  ti) noexcept: m_type_info(ti){}
   ir_value(const ir_value&   rhs) noexcept{assign(rhs);}
   ir_value(      ir_value&&  rhs) noexcept{assign(std::move(rhs));}

   template<class...  Args>
   ir_value(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

   ir_value&  operator=(const ir_value&   rhs) noexcept{return assign(rhs);}
   ir_value&  operator=(      ir_value&&  rhs) noexcept{return assign(std::move(rhs));}

   template<class...  Args>
   ir_value&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

   ir_value&  assign(const ir_value&   rhs) noexcept;
   ir_value&  assign(      ir_value&&  rhs) noexcept;
   ir_value&  assign(bool  b) noexcept{return assign(static_cast<int64_t>(b? 1:0));}
   ir_value&  assign(int64_t  i) noexcept;
   ir_value&  assign(double  f) noexcept;
   ir_value&  assign(std::string_view  sv) noexcept;

  void  clear() noexcept;

  const ir_type_info*  operator->() const noexcept{return &m_type_info;}

  const ir_type_info&  get_type_info() const noexcept{return m_type_info;}

  int64_t  get_integer() const noexcept{return m_data.i;}
  double       get_fpn() const noexcept{return m_data.f;}

  std::string_view  get_object() const noexcept{return std::string_view(m_data.o,m_type_info.get_size());}

  void  print() const noexcept;

};


class
ir_operand
{
  enum class kinds{
    null, integer, fpn, object, label, phi_element_list, operand_list
  } m_kind=kinds::null;

  union data{
    int64_t  i;
    double   f;

    std::string  s;

    std::vector<ir_phi_element>  phels;
    std::vector<ir_operand>       opls;

    data() noexcept{}
   ~data(){}
  } m_data;

public:
  ir_operand() noexcept{}
  ir_operand(const ir_operand&   rhs) noexcept{assign(rhs);}
  ir_operand(      ir_operand&&  rhs) noexcept{assign(std::move(rhs));}
 ~ir_operand(){clear();}

  template<class...  Args>
  explicit ir_operand(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class...  Args>
  ir_operand&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  ir_operand&  operator=(const ir_operand&   rhs) noexcept{return assign(rhs);}
  ir_operand&  operator=(      ir_operand&&  rhs) noexcept{return assign(std::move(rhs));}

  ir_operand&  assign(const ir_operand&   rhs) noexcept;
  ir_operand&  assign(      ir_operand&&  rhs) noexcept;
  ir_operand&  assign(int      i) noexcept{return assign(static_cast<int64_t>(i));}
  ir_operand&  assign(int64_t  i) noexcept;
  ir_operand&  assign(double   f) noexcept;
  ir_operand&  assign(const void*  ptr, size_t  n) noexcept;
  ir_operand&  assign(std::string_view  sv) noexcept;
  ir_operand&  assign(std::vector<ir_phi_element>&&  phels) noexcept;
  ir_operand&  assign(std::vector<ir_operand>&&  opls) noexcept;

  void  clear() noexcept;

  bool  is_integer()          const noexcept{return m_kind == kinds::integer;}
  bool  is_fpn()              const noexcept{return m_kind == kinds::fpn;}
  bool  is_object()           const noexcept{return m_kind == kinds::object;}
  bool  is_label()            const noexcept{return m_kind == kinds::label;}
  bool  is_phi_element_list() const noexcept{return m_kind == kinds::phi_element_list;}
  bool  is_operand_list()     const noexcept{return m_kind == kinds::operand_list;}

  int64_t                             get_integer()          const noexcept{return m_data.i;}
  double                              get_fpn()              const noexcept{return m_data.f;}
  const std::string&                  get_string()           const noexcept{return m_data.s;}
  const std::vector<ir_phi_element>&  get_phi_element_list() const noexcept{return m_data.phels;}
  const std::vector<ir_operand>&      get_operand_list()     const noexcept{return m_data.opls;}

  void  print() const noexcept;

};


class
ir_phi_element
{
  std::string  m_label;

  ir_operand  m_operand;

public:
  template<class  T>
  ir_phi_element(std::string_view  lb, T&&  t) noexcept:
  m_label(lb), m_operand(std::move(t)){}

  const std::string&  get_label() const noexcept{return m_label;}

  const ir_operand&  get_operand() const noexcept{return m_operand;}

  void  print() const noexcept{
    printf("%s:",m_label.data());
    m_operand.print();
  }

};




class
ir_block_info
{
  std::string  m_label;

  const ir_operation*  m_entry=nullptr;

  int  m_number_of_operations=0;

public:
  ir_block_info() noexcept{}
  ir_block_info(std::string_view  lb) noexcept: m_label(lb){}

  ir_block_info&  operator++() noexcept;

  void                 set_entry(const ir_operation*  e)       noexcept{m_entry = e;}
  const ir_operation*  get_entry(                      ) const noexcept{return m_entry;}

  const ir_operation*  get_end() const noexcept;

  int  get_number_of_operations() const noexcept{return m_number_of_operations;}

  void  set_label(std::string_view  sv) noexcept{m_label = sv;}

  const std::string&  get_label() const noexcept{return m_label;}

  bool  test_label(std::string_view  label) const noexcept{return m_label == label;}

  void  print() const noexcept;

};


class
ir_operation
{
  const ir_block_info*  m_block_info=nullptr;

  char*  m_label=nullptr;

  int  m_label_length=0;

  std::vector<ir_operand>  m_operand_list;

  std::string  m_instruction;

  enum class kinds{
    arithmetic,
    bitwise,
    comparison,
    load,
    store,
    others,

  } m_kind;


  void  set_kind(std::string_view  sv) noexcept;

  void  read_assign(std::string_view  sv, token_iterator&  it);

public:
  ir_operation() noexcept{}
  ir_operation(const ir_block_info&  bi, std::string_view  lb, std::string_view  instr, std::vector<ir_operand>&&  opls={}) noexcept;

  ir_operation(const ir_operation&   rhs) noexcept{assign(rhs);}
  ir_operation(      ir_operation&&  rhs) noexcept{assign(std::move(rhs));}
 ~ir_operation(){clear();}

  ir_operation&  operator=(const ir_operation&   rhs) noexcept{return assign(rhs);}
  ir_operation&  operator=(      ir_operation&&  rhs) noexcept{return assign(std::move(rhs));}

  ir_operation&  assign(const ir_operation&   rhs) noexcept;
  ir_operation&  assign(      ir_operation&&  rhs) noexcept;

  void  clear() noexcept;

  bool  is_arithmetic() const noexcept{return m_kind == kinds::arithmetic;}
  bool  is_bitwise()    const noexcept{return m_kind == kinds::bitwise;}
  bool  is_comparison() const noexcept{return m_kind == kinds::comparison;}
  bool  is_load()       const noexcept{return m_kind == kinds::load;}
  bool  is_store()      const noexcept{return m_kind == kinds::store;}
  bool  is_others()     const noexcept{return m_kind == kinds::others;}

  void  read(std::string_view  sv, token_iterator&  it);

  ir_operation&     set_label(std::string_view  sv) noexcept;
  std::string_view  get_label() const noexcept{return std::string_view(m_label,m_label_length);}

  ir_operation&         set_block_info(const ir_block_info*  bi)       noexcept{  m_block_info = bi;  return *this;}
  const ir_block_info*  get_block_info(                        ) const noexcept{return m_block_info;}

  const std::vector<ir_operand>&  get_operand_list() const noexcept{return m_operand_list;}

  void                set_instruction(std::string_view  sv) noexcept{  m_instruction = sv;  set_kind(sv);}
  const std::string&  get_instruction(                    ) const noexcept{return m_instruction;}

  void  print() const noexcept;

};


class
ir_parameter
{
  ir_type_info  m_type_info;

  std::string  m_label;

public:
  ir_parameter() noexcept{}
  ir_parameter(ir_type_info  ti, std::string_view  lb) noexcept: m_type_info(ti), m_label(lb){}

  const ir_type_info&  get_type_info() const noexcept{return m_type_info;}

  const std::string&  get_label() const noexcept{return m_label;}

};


class
ir_function
{
  ir_type_info  m_type_info;

  std::string  m_name;

  std::vector<ir_parameter>  m_parameter_list;
  std::vector<ir_operation>  m_operation_list;

  std::list<ir_block_info>  m_block_info_list;

public:
  static constexpr std::string_view  m_first_label = "entry";

  ir_function() noexcept{}
  ir_function(ir_type_info  ti, std::string_view  name, std::vector<ir_parameter>&&  parals={}) noexcept:
  m_type_info(ti), m_name(name), m_parameter_list(std::move(parals)){}

  void  read(token_iterator&  it);

  const ir_type_info&  get_type_info() const noexcept{return m_type_info;}

  const std::string&  get_name() const noexcept{return m_name;}

  const std::vector<ir_parameter>&  get_parameter_list() const noexcept{return m_parameter_list;}
  const std::vector<ir_operation>&  get_operation_list() const noexcept{return m_operation_list;}

  const ir_block_info*  find_block_info(std::string_view  label) const noexcept;
  const ir_operation*    find_operation(std::string_view  label) const noexcept;

  ir_function&  add_label(const char*  fmt, ...) noexcept;

  std::string_view  get_last_label() const noexcept{
    return m_operation_list.back().get_label();
  }

  ir_operation&  add_operation() noexcept;

  void  finalize() noexcept;

  void  print() const noexcept;

};


class
ir_register: public ir_value
{
  std::string  m_label;

public:
  ir_register(std::string_view  lb="no name") noexcept: m_label(lb){}
  ir_register(std::string_view  lb, ir_value&&  v) noexcept: ir_value(std::move(v)), m_label(lb){}

  ir_register(const ir_value&   v) noexcept{assign(v);}
  ir_register(      ir_value&&  v) noexcept{assign(std::move(v));}

  ir_register&  operator=(const ir_value&   v) noexcept{return assign(v);}
  ir_register&  operator=(      ir_value&&  v) noexcept{return assign(std::move(v));}

  ir_register&  assign(const ir_value&   v) noexcept{  ir_value::assign(v);  return *this;}
  ir_register&  assign(      ir_value&&  v) noexcept{  ir_value::assign(std::move(v));  return *this;}

  bool  operator==(std::string_view  sv) const noexcept{return m_label == sv;}

  const std::string&  get_label() const noexcept{return m_label;}

  void  print() const noexcept{  printf("%s: ",m_label.data());  ir_value::print();}

};


class
ir_context
{
  std::vector<ir_register>  m_register_list;

  std::vector<std::unique_ptr<ir_function>>  m_function_list;

  void  read_function(token_iterator&  it);
  void  read_variable(token_iterator&  it);

public:
  void  clear() noexcept{m_function_list.clear();}

  void  read(token_iterator&  it);
  void  read(std::string_view  sv);

  ir_function&  create_function(ir_type_info  ret_ti, std::string_view  fn_name, std::vector<ir_parameter>&&  parals) noexcept;

  const ir_function*  find_function(std::string_view  label) const noexcept;
  const ir_register*  find_register(std::string_view  label) const noexcept;

  void  finalize() noexcept;

  void  print() const noexcept;

};


class
ir_processor
{
  uint8_t  m_memory[1024*1024];

  const ir_context*  m_context;

  struct frame{
    pointer_wrapper<ir_register>  m_return_register;

    const ir_function*  m_function=nullptr;

    std::vector<ir_register>  m_register_map;

    const ir_block_info*  m_previous_block_info=nullptr;
    const ir_block_info*  m_current_block_info=nullptr;

    const ir_operation*  m_current=nullptr;
    const ir_operation*  m_end=nullptr;

  };

  std::vector<frame>  m_frame_stack;

  void  operate(const ir_operation&  op);

  void  operate_ari(std::string_view  instr, const std::vector<ir_operand>&  opls, ir_register&  reg);
  void  operate_biw(std::string_view  instr, const std::vector<ir_operand>&  opls, ir_register&  reg);
  void  operate_cmp(std::string_view  instr, const std::vector<ir_operand>&  opls, ir_register&  reg);
  void  operate_ld(std::string_view  instr, const std::vector<ir_operand>&  opls, ir_register&  reg);
  void  operate_st(std::string_view  instr, const std::vector<ir_operand>&  opls);

  void  operate_cal(const std::vector<ir_operand>&  opls, ir_register&  reg);
  void  operate_br( const std::vector<ir_operand>&  opls);
  void  operate_phi(const std::vector<ir_phi_element>&  phels, ir_register&  reg);

  ir_value  evaluate(const ir_operand&  o);

  void  jump(std::string_view  label);
  void  jump(const ir_operation&  op);

  pointer_wrapper<ir_register>  find(std::vector<ir_register>&  map, std::string_view  label) noexcept;

public:
  ir_processor() noexcept{}

  template<class...  Args>
  ir_processor(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class...  Args>
  ir_processor&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  void  assign(const ir_context&  ctx) noexcept;

  void  reset() noexcept;

  void  call(pointer_wrapper<ir_register>  retreg, std::string_view  fn_name, std::vector<ir_value>&&  args);

  void  step();

  void  run();

  void  print() const noexcept;

};




}




#endif




