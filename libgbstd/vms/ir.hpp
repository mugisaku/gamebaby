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

  ir_error(const char*  s) noexcept{m_comment = s;}

};


class
ir_type_info
{
  enum class kinds{
    null, integer, fpn, object,
  } m_kind;

  int  m_number_of_words=0;

  constexpr ir_type_info(kinds  k) noexcept: m_kind(k){}

public:
  constexpr ir_type_info(int  n) noexcept: m_kind(kinds::object), m_number_of_words(n){}

  constexpr bool  is_void()    const noexcept{return m_kind == kinds::null;}
  constexpr bool  is_integer() const noexcept{return m_kind == kinds::integer;}
  constexpr bool  is_fpn()     const noexcept{return m_kind == kinds::fpn;}
  constexpr bool  is_object()  const noexcept{return m_kind == kinds::object;}

  constexpr int  get_number_of_words()  const noexcept{return m_number_of_words;}

  static constexpr ir_type_info     void_type_info() noexcept{return ir_type_info(kinds::null);}
  static constexpr ir_type_info  integer_type_info() noexcept{return ir_type_info(kinds::integer);}
  static constexpr ir_type_info      fpn_type_info() noexcept{return ir_type_info(kinds::fpn);}
  static constexpr ir_type_info  object_type_info(int  n) noexcept{return ir_type_info(n);}

};


class
ir_operand
{
  enum class kinds{
    null, integer, fpn, label, phi_element_list,
  } m_kind=kinds::null;

  union data{
    int64_t  i;
    double   f;

    std::string  s;

    std::vector<ir_phi_element>  phels;

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
  ir_operand&  assign(std::string_view  sv) noexcept;
  ir_operand&  assign(std::vector<ir_phi_element>&&  phels) noexcept;

  void  clear() noexcept;

  bool  is_integer()          const noexcept{return m_kind == kinds::integer;}
  bool  is_fpn()              const noexcept{return m_kind == kinds::fpn;}
  bool  is_label()            const noexcept{return m_kind == kinds::label;}
  bool  is_phi_element_list() const noexcept{return m_kind == kinds::phi_element_list;}

  int64_t                             get_integer()          const noexcept{return m_data.i;}
  double                              get_fpn()              const noexcept{return m_data.f;}
  const std::string&                  get_string()           const noexcept{return m_data.s;}
  const std::vector<ir_phi_element>&  get_phi_element_list() const noexcept{return m_data.phels;}

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

  operator_code  m_operator_code;

  enum class kinds{
    arithmetic,
    bitwise,
    comparison,
    load,
    store,
    others,

  } m_kind;


  void  set_kind(operator_code  opco) noexcept;

public:
  ir_operation(const ir_block_info&  bi) noexcept: m_block_info(&bi){}
  ir_operation(const ir_block_info&  bi, std::string_view  lb, operator_code  op, std::vector<ir_operand>&&  opls={}) noexcept;

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

  const ir_block_info&  get_block_info() const noexcept{return *m_block_info;}

  const std::vector<ir_operand>&  get_operand_list() const noexcept{return m_operand_list;}

  void           set_operator_code(operator_code  opco) noexcept{  m_operator_code = opco;  set_kind(opco);}
  operator_code  get_operator_code() const noexcept{return m_operator_code;}

  void  print() const noexcept;

};


class
ir_parameter
{
  std::string  m_label;

public:
  ir_parameter(std::string_view  lb) noexcept: m_label(lb){}

  const std::string&  get_label() const noexcept{return m_label;}

};


class
ir_function
{
  std::string  m_name;

  std::vector<ir_parameter>  m_parameter_list;
  std::vector<ir_operation>  m_operation_list;

  std::list<ir_block_info>  m_block_info_list;

public:
  static constexpr std::string_view  m_first_label = "entry";

  ir_function() noexcept{}
  ir_function(std::string_view  name, std::vector<ir_parameter>&&  parals={}) noexcept:
  m_name(name), m_parameter_list(std::move(parals)){}

  void  read(token_iterator&  it);

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
ir_context
{
  std::vector<std::unique_ptr<ir_function>>  m_function_list;

public:
  void  clear() noexcept{m_function_list.clear();}

  void  read(token_iterator&  it);
  void  read(std::string_view  sv);

  ir_function&  create_function(std::string_view  fn_name, std::vector<ir_parameter>&&  parals) noexcept;

  const ir_function*  find_function(std::string_view  label) const noexcept;

  void  finalize() noexcept;

  void  print() const noexcept;

};


class
ir_register
{
  std::string_view  m_label;

  int64_t  m_value;

public:
  ir_register(std::string_view  lb, int64_t  v=0) noexcept: m_label(lb), m_value(v){}

  bool  operator==(std::string_view  sv) const noexcept{return m_label == sv;}

  void     set_value(int64_t  v)       noexcept{m_value = v;}
  int64_t  get_value(          ) const noexcept{return m_value;}

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

  void  operate_ari(operator_code  opco, const std::vector<ir_operand>&  opls, ir_register&  reg);
  void  operate_biw(operator_code  opco, const std::vector<ir_operand>&  opls, ir_register&  reg);
  void  operate_cmp(operator_code  opco, const std::vector<ir_operand>&  opls, ir_register&  reg);
  void  operate_ld(operator_code  opco, const std::vector<ir_operand>&  opls, ir_register&  reg);
  void  operate_st(operator_code  opco, const std::vector<ir_operand>&  opls);

  void  operate_cal(const std::vector<ir_operand>&  opls, ir_register&  reg);
  void  operate_br( const std::vector<ir_operand>&  opls);
  void  operate_phi(const std::vector<ir_phi_element>&  phels, ir_register&  reg);

  int64_t  evaluate(const ir_operand&  o);

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

  void  call(pointer_wrapper<ir_register>  retreg, std::string_view  fn_name, const std::vector<int64_t>&  args);

  void  step();

  void  run();

};




}




#endif




