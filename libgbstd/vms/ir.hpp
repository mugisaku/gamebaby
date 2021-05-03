#ifndef gbstd_vm_ir_HPP
#define gbstd_vm_ir_HPP


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
  ir_function(ir_type_info  ti, std::u16string_view  name, std::vector<ir_parameter>&&  parals={}) noexcept:
  m_return_type_info(ti), m_name(name), m_parameter_list(std::move(parals)){}

  const ir_type_info&  return_type_info() const noexcept{return m_return_type_info;}

  const std::u16string&  name() const noexcept{return m_name;}

  const ir_parameter_list&  parameter_list() const noexcept{return m_parameter_list;}
  const ir_statement_list&  statement_list() const noexcept{return m_statement_list;}

  void  print() const noexcept;

};




}




#endif




