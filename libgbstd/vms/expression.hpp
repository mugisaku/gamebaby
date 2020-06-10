#ifndef gbstd_vm_expression_HPP
#define gbstd_vm_expression_HPP


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


namespace gbstd{




class
expression_error
{
public:
  std::string  m_comment;

  expression_error(std::string_view  comm="") noexcept: m_comment(comm){}

};




class operand;
class primary_expression_element;
class expression_element;
class unary_expression;




using address_t = int32_t;


constexpr address_t
get_aligned_address(address_t  addr) noexcept
{
  constexpr auto  align = sizeof(int64_t);

  return (addr+(align-1))/align*align;
}


class
memo_info
{
  typesystem::type_info  m_type_info;

  std::string  m_name;

  address_t  m_address=0;

public:
  memo_info() noexcept{}
  memo_info(const typesystem::type_info&  ti, std::string_view  name) noexcept: m_type_info(ti), m_name(name){}

  const typesystem::type_info&  get_type_info() const noexcept{return m_type_info;}

  const std::string&  get_name() const noexcept{return m_name;}

  address_t  set_address(address_t  addr) noexcept{
    m_address = get_aligned_address(addr);

    return m_address+m_type_info.get_size();
  }

  address_t  get_address() const noexcept{return m_address;}

  void  print() const noexcept{
    m_type_info.print();
    printf("  %s[%d]",m_name.data(),m_address);
  }

};




class
simple_value
{
  enum class kinds{
    null, boolean, fpn, integer,
  } m_kind=kinds::null;

  union data{
    int64_t  i;
    bool     b;
    double   f;

    data() noexcept{}
   ~data(){}
  } m_data;

public:
  simple_value() noexcept{}

  template<class...  Args>
  explicit simple_value(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class...  Args>
  simple_value&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  simple_value  operator-() const noexcept{
    return is_integer()? simple_value(-m_data.i)
          :is_fpn()    ? simple_value(-m_data.f)
          :simple_value()
          ;
  }

  simple_value  operator!() const noexcept{
    return is_integer()? simple_value(!m_data.i)
          :is_boolean()? simple_value(!m_data.b)
          :simple_value()
          ;
  }

  simple_value  operator~() const noexcept{
    return is_integer()? simple_value(~m_data.i)
          :simple_value()
          ;
  }

  simple_value&  assign(const simple_value&   rhs) noexcept{  if(this != &rhs){  std::memcpy(this,&rhs,sizeof(*this));};  return *this;}
  simple_value&  assign(      simple_value&&  rhs) noexcept{  if(this != &rhs){  std::memcpy(this,&rhs,sizeof(*this));};  return *this;}
  simple_value&  assign(int64_t  i) noexcept{  m_kind = kinds::integer;  m_data.i = i;  return *this;}
  simple_value&  assign(bool     b) noexcept{  m_kind = kinds::boolean;  m_data.b = b;  return *this;}
  simple_value&  assign(double   f) noexcept{  m_kind = kinds::fpn    ;  m_data.f = f;  return *this;}

  int64_t  get_integer() const noexcept{return m_data.i;}
  bool     get_boolean() const noexcept{return m_data.b;}
  double   get_fpn()     const noexcept{return m_data.f;}

  bool  is_integer() const noexcept{return m_kind == kinds::integer;}
  bool  is_boolean() const noexcept{return m_kind == kinds::boolean;}
  bool  is_fpn()     const noexcept{return m_kind == kinds::fpn;}

};


class
primary_expression
{
  struct data;

  data*  m_data=nullptr;

public:
  primary_expression() noexcept{}
  primary_expression(const primary_expression&   rhs) noexcept{assign(rhs);}
  primary_expression(      primary_expression&&  rhs) noexcept{assign(std::move(rhs));}
 ~primary_expression(){clear();}

  template<class... Args>
  explicit primary_expression(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class... Args>
  primary_expression&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  operator bool() const noexcept{return m_data;}

  primary_expression&  assign(const primary_expression&   rhs) noexcept;
  primary_expression&  assign(      primary_expression&&  rhs) noexcept;
  primary_expression&  assign(operand&&  o, std::vector<primary_expression_element>&&  els) noexcept;

  primary_expression&  clear() noexcept;

        operand&  get_operand()       noexcept;
  const operand&  get_operand() const noexcept;

        std::vector<primary_expression_element>&  get_elements()       noexcept;
  const std::vector<primary_expression_element>&  get_elements() const noexcept;

  simple_value  evaluate() const noexcept;

  void  print() const noexcept;

};


class
expression
{
  struct data;

  data*  m_data=nullptr;

public:
  expression() noexcept{}
  expression(const expression&   rhs) noexcept{assign(rhs);}
  expression(      expression&&  rhs) noexcept{assign(std::move(rhs));}
 ~expression(){clear();}

  template<class... Args>
  explicit expression(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class... Args>
  expression&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  template<class... Args>
  expression&  operator+=(Args&&...  args) noexcept{return append(std::forward<Args>(args)...);}

  operator bool() const noexcept{return m_data;}

  expression&  assign(const expression&   rhs) noexcept;
  expression&  assign(      expression&&  rhs) noexcept;
  expression&  assign(std::vector<expression_element>&&  els) noexcept;

  expression&  append(expression_element&&  els) noexcept;

  expression&  clear() noexcept;

        std::vector<expression_element>&  get_elements()       noexcept;
  const std::vector<expression_element>&  get_elements() const noexcept;

  simple_value  evaluate() const noexcept;

  void  print() const noexcept;

};




class
identifier
{
  std::string  m_string;

public:
  identifier() noexcept{}
  identifier(std::string_view  sv) noexcept: m_string(sv){}

  const std::string&  get() const noexcept{return m_string;}
  std::string     release() noexcept{return std::move(m_string);}

};


class
operand
{
  enum class kinds{
    null,

    null_pointer,
    boolean,
    integer,
    fpn,
    string,
    identifier,
    expression,

  } m_kind=kinds::null;

  union data{
    bool         b;
    std::string  s;
    int64_t      i;

    double  f;

    expression  expr;

  data() noexcept{}
 ~data(){}

  } m_data;

public:
  operand() noexcept{}
  operand(const operand&   rhs) noexcept{assign(rhs);}
  operand(      operand&&  rhs) noexcept{assign(std::move(rhs));}
 ~operand(){clear();}

  template<class... Args>
  explicit operand(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class... Args>
  operand&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  operator bool() const noexcept{return m_kind != kinds::null;}

  operand&  assign(const operand&   rhs) noexcept;
  operand&  assign(      operand&&  rhs) noexcept;
  operand&  assign(nullptr_t  n) noexcept;
  operand&  assign(bool  b) noexcept;
  operand&  assign(std::string_view  sv) noexcept;
  operand&  assign(int64_t  i) noexcept;
  operand&  assign(double  f) noexcept;
  operand&  assign(identifier&&  id) noexcept;
  operand&  assign(expression&&  e) noexcept;

  operand&  clear() noexcept;

  bool      get_boolean() const noexcept{return m_data.b;}
  int64_t   get_integer() const noexcept{return m_data.i;}
  double    get_fpn()     const noexcept{return m_data.f;}

  const std::string&     get_string() const noexcept{return m_data.s;}
  const expression&  get_expression() const noexcept{return m_data.expr;}

  bool  is_boolean()      const noexcept{return m_kind == kinds::boolean;}
  bool  is_null_pointer() const noexcept{return m_kind == kinds::null_pointer;}
  bool  is_string()       const noexcept{return m_kind == kinds::string;}
  bool  is_integer()      const noexcept{return m_kind == kinds::integer;}
  bool  is_fpn()          const noexcept{return m_kind == kinds::fpn;}
  bool  is_identifier() const noexcept{return m_kind == kinds::identifier;}
  bool  is_expression() const noexcept{return m_kind == kinds::expression;}

  simple_value  evaluate() const noexcept;

  void  print() const noexcept;

};


class
assignment
{
  operator_code  m_operator_code;

  expression  m_expression;

public:
  assignment() noexcept{}
  assignment(expression&&  e) noexcept{assign("",std::move(e));}
  assignment(operator_code  op, expression&&  e) noexcept{assign(op,std::move(e));}

  assignment&  assign(operator_code  op, expression&&  e) noexcept{
    m_operator_code =           op;
    m_expression    = std::move(e);

    return *this;
  }

  operator_code  get_operator_code() const noexcept{return m_operator_code;}

        expression&  get_expression()       noexcept{return m_expression;}
  const expression&  get_expression() const noexcept{return m_expression;}

  void  print() const noexcept{  m_operator_code.print();  m_expression.print();}

};


class
primary_expression_element
{
  enum class kinds{
    null,
    selector,
    index,
    call,
    assignment,

  } m_kind=kinds::null;

  union data{
    std::string                   s;
    expression                 expr;
    std::vector<expression>  exprls;
    assignment                  ass;

    data() noexcept{}
   ~data(){}

  } m_data;

public:
  primary_expression_element() noexcept{}
  primary_expression_element(const primary_expression_element&   rhs) noexcept{assign(rhs);}
  primary_expression_element(      primary_expression_element&&  rhs) noexcept{assign(std::move(rhs));}
 ~primary_expression_element(){clear();}

  template<class... Args>
  explicit primary_expression_element(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class... Args>
  primary_expression_element&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  operator bool() const noexcept{return m_kind != kinds::null;}

  primary_expression_element&  assign(const primary_expression_element&   rhs) noexcept;
  primary_expression_element&  assign(      primary_expression_element&&  rhs) noexcept;
  primary_expression_element&  assign(std::string_view  id) noexcept;
  primary_expression_element&  assign(expression&&  e) noexcept;
  primary_expression_element&  assign(std::vector<expression>&&  els) noexcept;
  primary_expression_element&  assign(assignment&&  ass) noexcept;

  primary_expression_element&  clear() noexcept;

  const std::string&                       get_string() const noexcept{return m_data.s;}
  const expression&                    get_expression() const noexcept{return m_data.expr;}
  const std::vector<expression>&  get_expression_list() const noexcept{return m_data.exprls;}
  const assignment&                    get_assignment() const noexcept{return m_data.ass;}

  bool  is_selector() const noexcept{return m_kind == kinds::selector;}
  bool  is_index()    const noexcept{return m_kind == kinds::index;}
  bool  is_call()     const noexcept{return m_kind == kinds::call;}
  bool  is_assignment() const noexcept{return m_kind == kinds::assignment;}

  void  print() const noexcept;

};




class
unary_expression
{
  std::vector<operator_code>  m_operator_code_list;

  primary_expression  m_primary_expression;

public:
  unary_expression() noexcept{}
  unary_expression(std::vector<operator_code>&&  ops, primary_expression&&  e) noexcept{assign(std::move(ops),std::move(e));}

  unary_expression&  assign(std::vector<operator_code>&&  ops, primary_expression&&  e) noexcept;

  unary_expression&  clear() noexcept;

  const std::vector<operator_code>&  get_operator_code_list() const noexcept{return m_operator_code_list;}

        primary_expression&  get_primary_expression()       noexcept{return m_primary_expression;}
  const primary_expression&  get_primary_expression() const noexcept{return m_primary_expression;}

  simple_value  evaluate() const noexcept;

  void  print() const noexcept;

};


class
expression_element
{
  operator_code  m_operator_code;

  unary_expression  m_expression;

public:
  template<class... Args>
  explicit expression_element(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class... Args>
  expression_element&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  expression_element&  assign(                   unary_expression&&  e) noexcept;
  expression_element&  assign(operator_code  op, unary_expression&&  e) noexcept;

  expression_element&  set_operator_code(operator_code  opco)       noexcept{  m_operator_code = opco;  return *this;;}
  operator_code        get_operator_code(                   ) const noexcept{return m_operator_code;}

        unary_expression&  get_expression()       noexcept{return m_expression;}
  const unary_expression&  get_expression() const noexcept{return m_expression;}

  void  print() const noexcept;

};


expression  read_expression(token_iterator&  it, operator_code  close_code);
expression  read_expression(std::string_view  sv, operator_code  close_code);




}




#endif




