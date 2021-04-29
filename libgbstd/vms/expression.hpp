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
#include"libgbstd/vms/typesystem.hpp"


namespace gbstd{




class
expression_error
{
public:
  std::string  m_comment;

  expression_error(std::string_view  comm="") noexcept: m_comment(comm){}

};




class operand;
class expression;
class postfix_element;
class simple_value;
class simple_object;




using address_t = int32_t;


constexpr address_t
get_aligned_address(address_t  addr) noexcept
{
  constexpr auto  align = sizeof(int64_t);

  return (addr+(align-1))/align*align;
}




class
simple_value
{
  enum class kind{
    null,
    boolean,
    integer, 
    floating,
    object,
  } m_kind=kind::null;

  union data{
    int64_t  i;
    bool     b;
    double   f;

    simple_object*   o;

    data() noexcept{}
   ~data(){}
  } m_data;


  int  m_offset=0;

public:
  simple_value() noexcept{}
  simple_value(const simple_value&   rhs) noexcept{assign(rhs);}
  simple_value(      simple_value&&  rhs) noexcept{assign(std::move(rhs));}
 ~simple_value(){clear();}

  template<class...  Args>
  simple_value(int  off, Args&&...  args) noexcept: m_offset(off){assign(std::forward<Args>(args)...);}

  template<class...  Args>
  simple_value&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  simple_value&  operator=(const simple_value&   rhs) noexcept{return assign(rhs);}
  simple_value&  operator=(      simple_value&&  rhs) noexcept{return assign(std::move(rhs));}

  simple_value&  assign(const simple_value&   rhs) noexcept;
  simple_value&  assign(      simple_value&&  rhs) noexcept;
  simple_value&  assign(int64_t  i) noexcept;
  simple_value&  assign(bool     b) noexcept;
  simple_value&  assign(double   f) noexcept;
  simple_value&  assign(simple_object&&  o) noexcept;

  void  clear() noexcept;

  int  offset() const noexcept{return m_offset;}

  int64_t               get_integer() const noexcept{return m_data.i;}
  bool                  get_boolean() const noexcept{return m_data.b;}
  double               get_floating() const noexcept{return m_data.f;}
  const simple_object&   get_object() const noexcept{return *m_data.o;}

  bool  is_integer()  const noexcept{return m_kind == kind::integer;}
  bool  is_boolean()  const noexcept{return m_kind == kind::boolean;}
  bool  is_floating() const noexcept{return m_kind == kind::floating;}
  bool  is_object()   const noexcept{return m_kind == kind::object;}

};


class
simple_object
{
  std::vector<simple_value>  m_value_list;

public:
  simple_object() noexcept{}

  template<class...  Args>
  simple_object(Args&&...  args) noexcept{append(std::forward<Args>(args)...);}

  template<class...  Args>
  simple_object&  operator+=(Args&&...  args) noexcept{return append(std::forward<Args>(args)...);}

  template<class...  Args>
  simple_object&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  template<class...  Args>
  simple_object&  append(Args&&...  args) noexcept{m_value_list.emplace_back((int)m_value_list.size(),std::forward<Args>(args)...);  return *this;}

  template<class...  Args>
  simple_object&  assign(Args&&...  args) noexcept{  m_value_list.clear();  return append(std::forward<Args>(args)...);}

  operator bool() const noexcept{return m_value_list.size();}

        simple_value&  operator[](int  i)       noexcept{return m_value_list[i];}
  const simple_value&  operator[](int  i) const noexcept{return m_value_list[i];}

};




class
expression
{
  std::unique_ptr<operand>  m_left;
  std::unique_ptr<operand>  m_right;

  small_string  m_binary_operator;

public:
  expression() noexcept{}
  expression(const expression&   rhs) noexcept{assign(rhs);}
  expression(      expression&&  rhs) noexcept{assign(std::move(rhs));}

  expression&  operator=(const expression&   rhs) noexcept{return assign(rhs);}
  expression&  operator=(      expression&&  rhs) noexcept{return assign(std::move(rhs));}

  expression&  assign(const expression&   rhs) noexcept;
  expression&  assign(      expression&&  rhs) noexcept;

  expression&  assign(operand&&  l) noexcept;
  expression&  assign(operand&&  l, operand&&  r, small_string  binop) noexcept;

  const operand*   left() const noexcept{return m_left.get();}
  const operand*  right() const noexcept{return m_right.get();}

  small_string  binary_operator() const noexcept{return m_binary_operator;}

  simple_object  evaluate() const noexcept;

  void  print() const noexcept;

};




class
identifier
{
  std::u16string  m_string;

public:
  identifier() noexcept{}
  identifier(std::u16string_view  sv) noexcept: m_string(sv){}

  const std::u16string&  get() const noexcept{return m_string;}
  std::u16string     release() noexcept{return std::move(m_string);}

};


class
operand
{
  enum class kinds{
    null,

    null_pointer,
    boolean,
    integer,
    floating,
    string,
    identifier,
    expression,

  } m_kind=kinds::null;

  union data{
    bool            b;
    std::u16string  s;
    uint64_t        i;

    double  f;

    expression  expr;

  data() noexcept{}
 ~data(){}

  } m_data;


  std::vector<small_string>      m_prefix_list;
  std::vector<postfix_element>  m_postfix_list;

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
  operand&  assign(std::u16string_view  sv) noexcept;
  operand&  assign(uint64_t  i) noexcept;
  operand&  assign(double  f) noexcept;
  operand&  assign(identifier&&  id) noexcept;
  operand&  assign(expression&&  e) noexcept;

  operand&  clear() noexcept;

  bool       get_boolean() const noexcept{return m_data.b;}
  uint64_t   get_integer() const noexcept{return m_data.i;}
  double    get_floating()     const noexcept{return m_data.f;}

  const std::u16string&     get_string() const noexcept{return m_data.s;}
  const expression&  get_expression() const noexcept{return m_data.expr;}

  bool  is_boolean()      const noexcept{return m_kind == kinds::boolean;}
  bool  is_null_pointer() const noexcept{return m_kind == kinds::null_pointer;}
  bool  is_string()       const noexcept{return m_kind == kinds::string;}
  bool  is_integer()      const noexcept{return m_kind == kinds::integer;}
  bool  is_flooating()    const noexcept{return m_kind == kinds::floating;}
  bool  is_identifier() const noexcept{return m_kind == kinds::identifier;}
  bool  is_expression() const noexcept{return m_kind == kinds::expression;}

  simple_object  evaluate() const noexcept;

  void  print() const noexcept;

};


class
postfix_element
{
  enum class kinds{
    null,
    selector,
    index,
    call,

  } m_kind=kinds::null;

  union data{
    std::u16string                s;
    expression                 expr;
    std::vector<expression>  exprls;

    data() noexcept{}
   ~data(){}

  } m_data;

public:
  postfix_element() noexcept{}
  postfix_element(const postfix_element&   rhs) noexcept{assign(rhs);}
  postfix_element(      postfix_element&&  rhs) noexcept{assign(std::move(rhs));}
 ~postfix_element(){clear();}

  template<class... Args>
  postfix_element(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class... Args>
  postfix_element&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  postfix_element&  operator=(const postfix_element&   rhs) noexcept{return assign(rhs);}
  postfix_element&  operator=(      postfix_element&&  rhs) noexcept{return assign(std::move(rhs));}

  operator bool() const noexcept{return m_kind != kinds::null;}

  postfix_element&  assign(const postfix_element&   rhs) noexcept;
  postfix_element&  assign(      postfix_element&&  rhs) noexcept;
  postfix_element&  assign(std::u16string_view  id) noexcept;
  postfix_element&  assign(expression&&  e) noexcept;
  postfix_element&  assign(std::vector<expression>&&  els) noexcept;

  postfix_element&  clear() noexcept;

  const std::u16string&                    get_string() const noexcept{return m_data.s;}
  const expression&                    get_expression() const noexcept{return m_data.expr;}
  const std::vector<expression>&  get_expression_list() const noexcept{return m_data.exprls;}

  bool  is_selector() const noexcept{return m_kind == kinds::selector;}
  bool  is_index()    const noexcept{return m_kind == kinds::index;}
  bool  is_call()     const noexcept{return m_kind == kinds::call;}

  void  print() const noexcept;

};




}




#endif




