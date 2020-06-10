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
#include"libgbstd/vms/assembly.hpp"


namespace gbstd{




class context;
class function;
class block_space;
class space_node;
class compile_context;




class
expression_statement
{
  expression  m_expression;

public:
  expression_statement(expression&&  e) noexcept: m_expression(e){}

  const expression&  get_expression() const noexcept{return m_expression;}

  void  print() const noexcept;

};


class
block_statement
{
  pointer_wrapper<block_space>  m_space;

public:
  block_statement() noexcept{}
  block_statement(block_space&  bsp) noexcept: m_space(&bsp){}

  const block_space&  get_space() const noexcept{return *m_space;}

  void  print() const noexcept;

};


class
if_statement: public block_statement
{
  expression  m_expression;

public:
  if_statement(expression&&  e, block_space&  bsp) noexcept: block_statement(bsp), m_expression(std::move(e)){}

  const expression&  get_expression() const noexcept{return m_expression;}

  void  print() const noexcept;

};


class
if_string_statement
{
  std::vector<if_statement>  m_if_list;

  pointer_wrapper<block_space>  m_else_block_space;

public:
  if_string_statement(std::vector<if_statement>&&  ls) noexcept:
  m_if_list(std::move(ls)){}

  if_string_statement(std::vector<if_statement>&&  ls, block_space&  els) noexcept:
  m_if_list(std::move(ls)), m_else_block_space(&els){}

  const std::vector<if_statement>&  get_if_list() const noexcept{return m_if_list;}

  pointer_wrapper<block_space>  get_else_block_space() const noexcept{return m_else_block_space;}

  void  print() const noexcept;

};


class
for_statement: public block_statement
{
  expression  m_init;
  expression  m_cond;
  expression  m_loop;

public:
  for_statement(expression&&  init, expression&&  cond, expression&&  loop, block_space&  bsp) noexcept:
  block_statement(bsp), m_init(std::move(init)), m_cond(std::move(cond)), m_loop(std::move(loop)){}

  const expression&  get_init() const noexcept{return m_init;}
  const expression&  get_cond() const noexcept{return m_cond;}
  const expression&  get_loop() const noexcept{return m_loop;}

  void  print() const noexcept;

};


class
while_statement: public if_statement
{
public:
  using if_statement::if_statement;

  void  print() const noexcept;

};


class
case_statement: public expression_statement
{
public:
  using expression_statement::expression_statement;

  void  print() const noexcept;

};


class
switch_statement: public if_statement
{
public:
  using if_statement::if_statement;

  void  print() const noexcept;

};


class
return_statement: public expression_statement
{
public:
  using expression_statement::expression_statement;

  void  print() const noexcept;

};


class
label_statement
{
protected:
  std::string  m_string;

public:
  label_statement() noexcept{}
  label_statement(std::string_view  sv) noexcept: m_string(sv){}

  bool  operator==(std::string_view  sv) const noexcept{return m_string == sv;}

  const std::string&  get_string() const noexcept{return m_string;}

  void  print() const noexcept;

};


class
jump_statement: public label_statement
{
public:
  using label_statement::label_statement;

  void  print() const noexcept;

};


class
control_statement: public label_statement
{
public:
  using label_statement::label_statement;

  void  print() const noexcept;

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

  void  print() const noexcept;

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
    while_,
    for_,
    switch_,
    case_,
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
    for_statement           fo;
    while_statement        whi;
    switch_statement       swi;
    case_statement         cas;
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
  statement&  assign(for_statement&&         st) noexcept;
  statement&  assign(while_statement&&       st) noexcept;
  statement&  assign(switch_statement&&      st) noexcept;
  statement&  assign(case_statement&&        st) noexcept;
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
  bool  is_for()        const noexcept{return m_kind == kind::for_;}
  bool  is_while()      const noexcept{return m_kind == kind::while_;}
  bool  is_switch()     const noexcept{return m_kind == kind::switch_;}
  bool  is_case()       const noexcept{return m_kind == kind::case_;}
  bool  is_control()    const noexcept{return m_kind == kind::control;}
  bool  is_let()        const noexcept{return m_kind == kind::let;}
  bool  is_expression() const noexcept{return m_kind == kind::expression;}

  return_statement&      get_return()     noexcept{return m_data.ret;}
  label_statement&       get_label()      noexcept{return m_data.lb;}
  jump_statement&        get_jump()       noexcept{return m_data.jmp;}
  if_string_statement&   get_if_string()  noexcept{return m_data.ifs;}
  block_statement&       get_block()      noexcept{return m_data.blk;}
  for_statement&         get_for()        noexcept{return m_data.fo;}
  while_statement&       get_while()      noexcept{return m_data.whi;}
  switch_statement&      get_switch()     noexcept{return m_data.swi;}
  case_statement&        get_case()       noexcept{return m_data.cas;}
  control_statement&     get_control()    noexcept{return m_data.ctrl;}
  let_statement&         get_let()        noexcept{return m_data.let;}
  expression_statement&  get_expression() noexcept{return m_data.expr;}

  const return_statement&      get_return()     const noexcept{return m_data.ret;}
  const label_statement&       get_label()      const noexcept{return m_data.lb;}
  const jump_statement&        get_jump()       const noexcept{return m_data.jmp;}
  const if_string_statement&   get_if_string()  const noexcept{return m_data.ifs;}
  const block_statement&       get_block()      const noexcept{return m_data.blk;}
  const for_statement&         get_for()        const noexcept{return m_data.fo;}
  const while_statement&       get_while()      const noexcept{return m_data.whi;}
  const switch_statement&      get_switch()     const noexcept{return m_data.swi;}
  const case_statement&        get_case()       const noexcept{return m_data.cas;}
  const control_statement&     get_control()    const noexcept{return m_data.ctrl;}
  const let_statement&         get_let()        const noexcept{return m_data.let;}
  const expression_statement&  get_expression() const noexcept{return m_data.expr;}

  void  compile(compile_context&  ctx) const;

  void  print(const context*  ctx=nullptr, const function*  fn=nullptr) const noexcept;

};


typesystem::type_info  compile(const expression&  e, compile_context&  ctx);


}




#endif




