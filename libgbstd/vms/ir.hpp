#ifndef gbstd_vm_assembly_HPP
#define gbstd_vm_assembly_HPP


#include"libgbstd/typesystem.hpp"
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
ir_type
{
  enum class kinds{
    null, integer, fpn, pointer, array, structure, 
  } m_kind;

};


class
ir_operand
{
  enum class kinds{
    null, integer, fpn, label, phi_element,
  } m_kind=kinds::null;

  union data{
    int64_t  i;
    double   f;

    std::string  s;

    ir_phi_element*  phel;

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
  ir_operand&  assign(ir_phi_element&&  phel) noexcept;

  void  clear() noexcept;

  bool  is_integer()     const noexcept{return m_kind == kinds::integer;}
  bool  is_fpn()         const noexcept{return m_kind == kinds::fpn;}
  bool  is_label()       const noexcept{return m_kind == kinds::label;}
  bool  is_phi_element() const noexcept{return m_kind == kinds::phi_element;}

  int64_t                    get_integer() const noexcept{return m_data.i;}
  double                         get_fpn() const noexcept{return m_data.f;}
  const std::string&          get_string() const noexcept{return m_data.s;}
  const ir_phi_element&  get_phi_element() const noexcept{return *m_data.phel;}

  void  print() const noexcept;

};


class
ir_phi_element
{
  std::string  m_label;

  ir_operand  m_operand;

public:
  ir_phi_element(std::string_view  lb, ir_operand&&  o) noexcept:
  m_label(lb), m_operand(std::move(o)){}

  const std::string&  get_label() const noexcept{return m_label;}

  const ir_operand&  get_operand() const noexcept{return m_operand;}

};




class
ir_block_info
{
  std::vector<std::string>  m_label_list;

  const ir_operation*  m_entry=nullptr;

  int  m_number_of_operations=0;

public:
  ir_block_info() noexcept{}
  ir_block_info(std::string_view  lb) noexcept{m_label_list.emplace_back(lb);}

  ir_block_info&  operator++() noexcept{  ++m_number_of_operations;  return *this;}

  void                 set_entry(const ir_operation*  e)       noexcept{m_entry = e;}
  const ir_operation*  get_entry(                      ) const noexcept{return m_entry;}

  const ir_operation*  get_end() const noexcept;

  int  get_number_of_operations() const noexcept{return m_number_of_operations;}

  void  add_label(std::string_view  sv) noexcept{m_label_list.emplace_back(sv);}

  bool  test_label(std::string_view  label) const noexcept;

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

public:
  ir_operation(const ir_block_info&  bi, std::string_view  lb, operator_code  op, std::vector<ir_operand>&&  opls={}) noexcept:
  m_block_info(&bi), m_operator_code(op), m_operand_list(opls){set_label(lb);}

  ir_operation(const ir_operation&   rhs) noexcept{assign(rhs);}
  ir_operation(      ir_operation&&  rhs) noexcept{assign(std::move(rhs));}
 ~ir_operation(){clear();}

  ir_operation&  operator=(const ir_operation&   rhs) noexcept{return assign(rhs);}
  ir_operation&  operator=(      ir_operation&&  rhs) noexcept{return assign(std::move(rhs));}

  ir_operation&  assign(const ir_operation&   rhs) noexcept;
  ir_operation&  assign(      ir_operation&&  rhs) noexcept;

  void  clear() noexcept;

  ir_operation&     set_label(std::string_view  sv) noexcept;
  std::string_view  get_label() const noexcept{return std::string_view(m_label,m_label_length);}

  const ir_block_info&  get_block_info() const noexcept{return *m_block_info;}

  const std::vector<ir_operand>&  get_operand_list() const noexcept{return m_operand_list;}

  operator_code  get_operator_code() const noexcept{return m_operator_code;}

  void  add_operand() noexcept{}

  template<class  T, class...  Args>
  void  add_operand(T&&  o, Args&&...  args) noexcept{
    m_operand_list.emplace_back(std::move(o));

    add_operand(std::forward<Args>(args)...);
  }

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

  int  m_number=0;

  ir_operation&  create_operation(operator_code  opco) noexcept;

  static constexpr std::string_view  m_first_label = "entry";

public:
  ir_function() noexcept{m_block_info_list.emplace_back(m_first_label);}
  ir_function(std::string_view  name, std::vector<ir_parameter>&&  parals={}) noexcept:
  m_name(name), m_parameter_list(std::move(parals)){m_block_info_list.emplace_back(m_first_label);}

  const std::string&  get_name() const noexcept{return m_name;}

  const std::vector<ir_parameter>&  get_parameter_list() const noexcept{return m_parameter_list;}
  const std::vector<ir_operation>&  get_operation_list() const noexcept{return m_operation_list;}

  const ir_block_info*  find_block_info(std::string_view  label) const noexcept;
  const ir_operation*    find_operation(std::string_view  label) const noexcept;

  void  add_parameter() noexcept{}

  template<class  T, class...  Args>
  ir_function&  add_parameter(T&&  label, Args&&...  args) noexcept{
    m_parameter_list.emplace_back(std::move(label));

    add_parameter(std::forward<Args>(args)...);

    return *this;
  }

  ir_function&  add_label(const char*  fmt, ...) noexcept;

  std::string_view  get_last_label() const noexcept{
    return m_operation_list.back().get_label();
  }

  template<class...  Args>
  ir_function&  add_operation(operator_code  opco, Args&&...  args) noexcept{
    create_operation(opco).add_operand(std::forward<Args>(args)...);

    return *this;
  }

  void  finalize() noexcept;

  void  print() const noexcept;

};


class
ir_context
{
  std::vector<std::unique_ptr<ir_function>>  m_function_list;

public:
  void  clear() noexcept{m_function_list.clear();}

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

  void  operate_cal(const std::vector<ir_operand>&  opls, ir_register&  reg);
  void  operate_br( const std::vector<ir_operand>&  opls);
  void  operate_phi(const std::vector<ir_operand>&  opls, ir_register&  reg);

  int64_t  evaluate(const ir_operand&  o);

  void  jump(std::string_view  label);
  void  jump(const ir_operation&  op);

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




