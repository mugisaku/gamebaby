#ifndef gbstd_vm_assembly_HPP
#define gbstd_vm_assembly_HPP


#include"libgbstd/typesystem.hpp"
#include"libgbstd/parser.hpp"
#include"libgbstd/vms/expression.hpp"


namespace gbstd{




class ir_operand;
class ir_phi_element;


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

  ir_operand&  assign(const ir_operand&   rhs) noexcept;
  ir_operand&  assign(      ir_operand&&  rhs) noexcept;
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
ir_operation
{
  std::string  m_label;

  std::vector<ir_operand>  m_operand_list;

  operator_code  m_operator_code;

public:
  ir_operation(std::string_view  lb, operator_code  op, std::vector<ir_operand>&&  opls) noexcept:
  m_label(lb), m_operator_code(op), m_operand_list(opls){}

  const std::string&  get_label() const noexcept{return m_label;}

  const std::vector<ir_operand>&  get_operand_list() const noexcept{return m_operand_list;}

  operator_code  get_operator_code() const noexcept{return m_operator_code;}

};


struct
ir_block
{
  std::vector<std::string>  m_label_list;

  std::list<ir_operation>  m_operation_list;

  bool  test_label(std::string_view  label) const noexcept
  {
      for(auto&  lb: m_label_list)
      {
          if(lb == label)
          {
            return true;
          }
      }


    return false;
  }

};


class
ir_function
{
  std::string  m_name;

  std::vector<std::string>  m_parameter_list;

  std::vector<ir_block>  m_block_list;

public:
  ir_function() noexcept{m_block_list.emplace_back();}

  const std::string&  get_name() const noexcept{return m_name;}

  const std::vector<std::string>&  get_parameter_list() const noexcept{return m_parameter_list;}

  const std::vector<ir_block>&  get_block_list() const noexcept{return m_block_list;}

  const ir_block*  find_block(std::string_view  label) const noexcept;

  const ir_operation*  find_operation(std::string_view  label, const ir_block**  blkptr=nullptr) const noexcept;

  void  add_label(const char*  fmt, ...) noexcept;

  template<class...  Args>
  void  add_operation(Args&&...  args) noexcept{m_block_list.back().m_operation_list.emplace_back(std::forward<Args>(args)...);}

  void  print() const noexcept;

};


class
ir_context
{
  std::vector<ir_function>  m_function_list;

public:
  const ir_function*  find_function(std::string_view  label) const noexcept;

  void  print() const noexcept;

};


struct
ir_argument
{
  std::string  m_label;

  int64_t  m_value;

};


class
ir_processor
{
  uint8_t  m_memory[1024*1024];

  const ir_context*  m_context;

  struct frame{
    const ir_function*  m_function=nullptr;

    std::vector<ir_argument>  m_argument_list;

    const ir_block*  m_previous_block=nullptr;
    const ir_block*  m_current_block=nullptr;

    const ir_operation*  m_current=nullptr;
    const ir_operation*  m_end=nullptr;
  };

  std::vector<frame>  m_frame_stack;

  int64_t  operate(const ir_operation&  op, bool&  returned);

  int64_t  operate_call(const std::vector<ir_operand>&  opls);
  int64_t  operate_br(const std::vector<ir_operand>&  opls);
  int64_t  operate_phi(const std::vector<ir_operand>&  opls);

  int64_t  evaluate(const ir_operand&  o);

  void  jump(std::string_view  label);
  void  jump(const ir_block&  blk);

public:
  void  assign(const ir_context&  ctx) noexcept;

  void  reset() noexcept;

  int64_t  call(std::string_view  fn_name, const std::vector<int64_t>&  args);

  int64_t  run();

};




}




#endif




