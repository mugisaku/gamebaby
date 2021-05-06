#ifndef gbstd_vm_ir_HPP
#define gbstd_vm_ir_HPP


#include"libgbstd/syntax.hpp"
#include"libgbstd/vms/irs/ir_statement.hpp"




namespace gbstd{




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


using ir_parameter_list = std::vector<ir_parameter>;
using ir_statement_list = std::vector<ir_statement>;


class
ir_function
{
  ir_type_info  m_return_type_info;

  std::u16string  m_name;

  ir_parameter_list  m_parameter_list;
  ir_statement_list  m_statement_list;

public:
  ir_function() noexcept{}
  ir_function(ir_type_info  ti, ir_parameter_list&&  paras, std::u16string_view  name, ir_statement_list&&  stmts) noexcept:
  m_return_type_info(ti), m_parameter_list(std::move(paras)), m_name(name), m_statement_list(std::move(stmts)){}

  const ir_type_info&  return_type_info() const noexcept{return m_return_type_info;}

  const std::u16string&  name() const noexcept{return m_name;}

  const ir_parameter_list&  parameter_list() const noexcept{return m_parameter_list;}
  const ir_statement_list&  statement_list() const noexcept{return m_statement_list;}

  void  print() const noexcept;

};


ir_parameter            make_parameter(const syntax_branch&  br) noexcept;
ir_statement            make_statement(const syntax_branch&  br) noexcept;
ir_parameter_list  make_parameter_list(const syntax_branch&  br) noexcept;
ir_statement_list  make_statement_list(const syntax_branch&  br) noexcept;
ir_type_info            make_type_info(const syntax_branch&  br) noexcept;
std::u16string_view    make_identifier(const syntax_branch&  br) noexcept;
ir_function              make_function(const syntax_branch&  br) noexcept;


class
ir_global_space
{
  std::vector<ir_function>  m_function_list;

public:
  ir_global_space() noexcept{}

  template<class...  Args>
  ir_global_space(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class...  Args>
  ir_global_space&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  ir_global_space&  assign(const syntax_branch&  br) noexcept;

  void  print() const noexcept;

};




}




#endif




