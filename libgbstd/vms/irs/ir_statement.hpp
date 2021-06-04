#ifndef gbstd_vm_ir_statement_HPP
#define gbstd_vm_ir_statement_HPP


#include"libgbstd/vms/irs/ir_operation.hpp"




namespace gbstd{




class
ir_return_statement
{
  std::u16string  m_operand;

public:
  ir_return_statement() noexcept{}
  ir_return_statement(std::u16string_view  o) noexcept: m_operand(o){}

  const std::u16string&  operand() const noexcept{return m_operand;}

};


class
ir_branch_statement
{
  std::u16string  m_operand;
  std::u16string  m_label;

public:
  ir_branch_statement() noexcept{}
  ir_branch_statement(std::u16string_view  o, std::u16string_view  lb) noexcept: m_operand(o), m_label(lb){}

  const std::u16string&  operand() const noexcept{return m_operand;}
  const std::u16string&    label() const noexcept{return m_label;}

};


class
ir_label_statement
{
  std::u16string  m_identifier;

public:
  ir_label_statement(std::u16string_view  id) noexcept: m_identifier(id){}

  const std::u16string&  identifier() const noexcept{return m_identifier;}

};


class
ir_store_statement
{
  ir_type_info  m_type_info;

  std::u16string  m_address_operand;
  std::u16string    m_value_operand;

public:
  ir_store_statement(ir_type_info  ti, std::u16string_view  addro, std::u16string_view  valo) noexcept: m_type_info(ti), m_address_operand(addro), m_value_operand(valo){}

  const ir_type_info&  type_info() const noexcept{return m_type_info;}

  const std::u16string&  address_operand() const noexcept{return m_address_operand;}
  const std::u16string&    value_operand() const noexcept{return m_value_operand;}

};


class
ir_register_statement
{
  std::u16string  m_identifier;

  ir_operation  m_operation;

public:
  ir_register_statement(std::u16string_view  id, ir_operation&&  op) noexcept: m_identifier(id), m_operation(std::move(op)){}

  const std::u16string&  identifier() const noexcept{return m_identifier;}

  const ir_operation&  operation() const noexcept{return m_operation;}

};




class
ir_statement
{
  enum class kind{
    null,
    return_,
    branch,
    label,
    store,
    register_,
  } m_kind=kind::null;

  union data{
    ir_return_statement    ret;
    ir_branch_statement     br;
    ir_label_statement      lb;
    ir_store_statement      st;
    ir_register_statement  reg;

    data() noexcept{}
   ~data(){}
  } m_data;

public:
  ir_statement() noexcept{}
  ir_statement(const ir_statement&   rhs) noexcept{assign(rhs);}
  ir_statement(      ir_statement&&  rhs) noexcept{assign(std::move(rhs));}
 ~ir_statement(){clear();}

  template<class...  Args>
  ir_statement(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class...  Args>
  ir_statement&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  ir_statement&  operator=(const ir_statement&   rhs) noexcept{return assign(rhs);}
  ir_statement&  operator=(      ir_statement&&  rhs) noexcept{return assign(std::move(rhs));}

  void  clear() noexcept;

  ir_statement&  assign(const ir_statement&   rhs) noexcept;
  ir_statement&  assign(      ir_statement&&  rhs) noexcept;
  ir_statement&  assign(ir_return_statement&&    ret) noexcept;
  ir_statement&  assign(ir_branch_statement&&     br) noexcept;
  ir_statement&  assign(ir_label_statement&&      lb) noexcept;
  ir_statement&  assign(ir_store_statement&&      st) noexcept;
  ir_statement&  assign(ir_register_statement&&  reg) noexcept;

  bool  is_return()   const noexcept{return m_kind == kind::return_;}
  bool  is_branch()   const noexcept{return m_kind == kind::branch;}
  bool  is_label()    const noexcept{return m_kind == kind::label;}
  bool  is_store()    const noexcept{return m_kind == kind::store;}
  bool  is_register() const noexcept{return m_kind == kind::register_;}

  const ir_return_statement&      get_return() const noexcept{return m_data.ret;}
  const ir_branch_statement&      get_branch() const noexcept{return m_data.br;}
  const ir_label_statement&        get_label() const noexcept{return m_data.lb;}
  const ir_store_statement&        get_store() const noexcept{return m_data.st;}
  const ir_register_statement&  get_register() const noexcept{return m_data.reg;}

  void  print() const noexcept;

};




}




#endif




