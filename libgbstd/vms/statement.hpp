#ifndef gbstd_vm_statement_HPP
#define gbstd_vm_statement_HPP


#include<cstdio>
#include<cstdint>
#include<cinttypes>
#include<cstdlib>
#include<memory>
#include<string>
#include<string_view>
#include<utility>
#include<vector>
#include"libgbstd/misc.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/typesystem.hpp"
#include"libgbstd/parser.hpp"
#include"libgbstd/vms/expression.hpp"
#include"libgbstd/vms/space.hpp"


namespace gbstd{




class context;


class
block_statement
{
  pointer_wrapper<block_space>  m_space;

public:
  const block_space&  get_space() const noexcept{return *m_space;}

  void  print() const noexcept{}

};


class
return_statement
{
  expression  m_expression;

public:
  return_statement() noexcept{}
  return_statement(expression&&  e) noexcept: m_expression(std::move(e)){}

        expression&  get_expression()       noexcept{return m_expression;}
  const expression&  get_expression() const noexcept{return m_expression;}

  void  print() const noexcept{  printf("return ");  m_expression.print();}

};


class
label_statement
{
  std::string  m_string;

public:
  label_statement() noexcept{}
  label_statement(std::string_view  sv) noexcept: m_string(sv){}

  const std::string&  get_string() const noexcept{return m_string;}

  void  print() const noexcept{printf("label:%s",m_string.data());}

};


class
jump_statement
{
  std::string  m_string;

public:
  jump_statement() noexcept{}
  jump_statement(std::string_view  sv) noexcept: m_string(sv){}

  const std::string&  get_string() const noexcept{return m_string;}

  void  print() const noexcept{printf("jump %s",m_string.data());}

};


class
control_statement
{
  std::string  m_string;

public:
  control_statement(std::string_view  sv) noexcept: m_string(sv){}

  bool  operator==(std::string_view  sv) const noexcept{return m_string == sv;}

  const std::string&  get_string() const noexcept{return m_string;}

  void  print() const noexcept{printf("%s",m_string.data());}

};


class
let_statement
{
  std::string  m_target_name;

  expression  m_expression;

public:
  let_statement(std::string_view  sv, expression&&  e) noexcept:
  m_target_name(sv), m_expression(std::move(e)){}

  const std::string&  get_target_name() const noexcept{return m_target_name;}

        expression&  get_expression()       noexcept{return m_expression;}
  const expression&  get_expression() const noexcept{return m_expression;}

  void  print() const noexcept{
    printf("let %s",m_target_name.data());
      if(m_expression){  printf(" = (");  m_expression.print();  printf(")");}
  }

};


class
condition_statement
{
  expression  m_expression;

  pointer_wrapper<block_statement>  m_block;

public:
  const expression&  get_expression() const noexcept{return m_expression;}

  const block_statement&  get_block() const noexcept{return *m_block;}

};


class
if_string_statement
{
  std::vector<condition_statement>  m_if_list;

  pointer_wrapper<block_statement>  m_else_block;

public:
  const block_statement&  get_else_block() const noexcept{return *m_else_block;}

  void  print() const noexcept{}

};


class
value
{
};


class
expression_statement
{
  expression  m_expression;

public:
  expression_statement(expression&&  e) noexcept: m_expression(e){}

  const expression&  get_expression() const noexcept{return m_expression;}

  value  evaluate(const context&  ctx) const noexcept;

  void  print() const noexcept{m_expression.print();}

};


class
statement
{
  enum class kind{
    null,
    return_,
    block,
    control,
    let,
    if_string,
    expression,
    jump,
    label,

  } m_kind=kind::null;

  union data{
    return_statement       ret;
    control_statement     ctrl;
    let_statement          let;
    if_string_statement    ifs;
    jump_statement         jmp;
    label_statement         lb;
    expression_statement  expr;
    block_statement        blk;

   data() noexcept{}
  ~data(){}
  } m_data;

public:
  statement() noexcept{}
  statement(statement&&  rhs) noexcept{assign(std::move(rhs));}
 ~statement(){clear();}

  template<class...  Args>
  explicit statement(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class...  Args>
  statement&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  operator bool() const noexcept{return m_kind != kind::null;}

//  statement&  assign(const statement&   rhs) noexcept;
  statement&  assign(      statement&&  rhs) noexcept;
  statement&  assign(return_statement&&      st) noexcept;
  statement&  assign(label_statement&&       st) noexcept;
  statement&  assign(jump_statement&&        st) noexcept;
  statement&  assign(if_string_statement&&   st) noexcept;
  statement&  assign(block_statement&&       st) noexcept;
  statement&  assign(control_statement&&     st) noexcept;
  statement&  assign(let_statement&&         st) noexcept;
  statement&  assign(expression_statement&&  st) noexcept;

  void  clear() noexcept;

  bool  is_null()       const noexcept{return m_kind == kind::null;}
  bool  is_return()     const noexcept{return m_kind == kind::return_;}
  bool  is_label()      const noexcept{return m_kind == kind::label;;}
  bool  is_jump()       const noexcept{return m_kind == kind::jump;}
  bool  is_if_string()  const noexcept{return m_kind == kind::if_string;}
  bool  is_block()      const noexcept{return m_kind == kind::block;}
  bool  is_control()    const noexcept{return m_kind == kind::control;}
  bool  is_let()        const noexcept{return m_kind == kind::let;}
  bool  is_expression() const noexcept{return m_kind == kind::expression;}

  return_statement&      get_return()     noexcept{return m_data.ret;}
  label_statement&       get_label()      noexcept{return m_data.lb;}
  jump_statement&        get_jump()       noexcept{return m_data.jmp;}
  if_string_statement&   get_if_string()  noexcept{return m_data.ifs;}
  block_statement&       get_block()      noexcept{return m_data.blk;}
  control_statement&     get_control()    noexcept{return m_data.ctrl;}
  let_statement&         get_let()        noexcept{return m_data.let;}
  expression_statement&  get_expression() noexcept{return m_data.expr;}

  const return_statement&      get_return()     const noexcept{return m_data.ret;}
  const label_statement&       get_label()      const noexcept{return m_data.lb;}
  const jump_statement&        get_jump()       const noexcept{return m_data.jmp;}
  const if_string_statement&   get_if_string()  const noexcept{return m_data.ifs;}
  const block_statement&       get_block()      const noexcept{return m_data.blk;}
  const control_statement&     get_control()    const noexcept{return m_data.ctrl;}
  const let_statement&         get_let()        const noexcept{return m_data.let;}
  const expression_statement&  get_expression() const noexcept{return m_data.expr;}

  void  print(const context*  ctx=nullptr, const function*  fn=nullptr) const noexcept;

};




}




#endif




