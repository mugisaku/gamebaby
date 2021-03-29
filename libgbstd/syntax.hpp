#ifndef gbstd_syntax_HPP
#define gbstd_syntax_HPP


#include"libgbstd/utility.hpp"
#include"libgbstd/character.hpp"
#include"libgbstd/tree_struct.hpp"
#include<list>


namespace gbstd{




class syntax_expression_element;
class syntax_expression;
class syntax_definition;


class
syntax_expression_error
{
};


class
syntax_expression
{
  small_string  m_code;

  using pointer = std::unique_ptr<syntax_expression_element>;

  pointer  m_left ;
  pointer  m_right;

  std::unique_ptr<syntax_expression_element>  pack_self() noexcept;

public:
  syntax_expression() noexcept{}
  syntax_expression(const syntax_expression&   rhs) noexcept{assign(rhs);}
  syntax_expression(      syntax_expression&&  rhs) noexcept{assign(std::move(rhs));}

  syntax_expression&  operator=(const syntax_expression&   rhs) noexcept{return assign(rhs);}
  syntax_expression&  operator=(      syntax_expression&&  rhs) noexcept{return assign(std::move(rhs));}

  syntax_expression&  assign(const syntax_expression&   rhs) noexcept;
  syntax_expression&  assign(      syntax_expression&&  rhs) noexcept;
  syntax_expression&  assign(small_string  c, pointer  l, pointer  r) noexcept;

  void  push(syntax_expression_element&&  e);
  void  push(small_string  c);

  const small_string&  code() const noexcept{return m_code;}

  const syntax_expression_element&   left() const noexcept{return  *m_left;}
  const syntax_expression_element&  right() const noexcept{return *m_right;}

  void  print_for_test() const noexcept;
  void  print() const noexcept;

};


class
syntax_expression_element
{
  enum class kind{
    null, string, expression, definition,
  } m_kind=kind::null;

  union data{
    std::u16string         str;
    syntax_expression      expr;
    const syntax_definition*  def;

  data() noexcept{}
 ~data()         {}

  } m_data;

public:
  syntax_expression_element() noexcept{}
  syntax_expression_element(const syntax_expression_element&   rhs) noexcept{assign(rhs);}
  syntax_expression_element(      syntax_expression_element&&  rhs) noexcept{assign(std::move(rhs));}
 ~syntax_expression_element(){clear();}

  template<class... Args>
  syntax_expression_element(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class... Args>
  syntax_expression_element&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  syntax_expression_element&  operator=(const syntax_expression_element&   rhs) noexcept{return assign(rhs);}
  syntax_expression_element&  operator=(      syntax_expression_element&&  rhs) noexcept{return assign(std::move(rhs));}

  syntax_expression_element&  assign(const syntax_expression_element&   rhs) noexcept;
  syntax_expression_element&  assign(      syntax_expression_element&&  rhs) noexcept;
  syntax_expression_element&  assign(std::u16string_view  sv) noexcept;
  syntax_expression_element&  assign(std::u16string&&  s) noexcept;
  syntax_expression_element&  assign(syntax_expression&&  exp) noexcept;
  syntax_expression_element&  assign(const syntax_definition&  def) noexcept;

  bool  is_string()     const noexcept{return m_kind == kind::string;}
  bool  is_expression() const noexcept{return m_kind == kind::expression;}
  bool  is_definition() const noexcept{return m_kind == kind::definition;}

  void  clear() noexcept;

  const std::u16string&  get_string() const noexcept{return m_data.str;}

  const syntax_expression&  get_expression() const noexcept{return  m_data.expr;}
  const syntax_definition&  get_definition() const noexcept{return *m_data.def;}

  void  print() const noexcept;

};


class
syntax_definition: public syntax_expression
{
  bool  m_key=false;

  std::u16string  m_name;

public:
  syntax_definition(bool  k, std::u16string_view  name) noexcept: m_key(k), m_name(name){}

  void  set_key() noexcept{m_key = true;}

  bool  is_key() const noexcept{return m_key;}

  const std::u16string&  get_name() const noexcept{return m_name;}

  void  print() const noexcept;

};


class
syntax_rule
{
  std::list<syntax_definition>  m_definition_list;

  void  start_read(bool  key, code_text::iterator&  it);

  std::u16string   read_string(code_text::iterator&  it);
  std::u16string   read_identifier(code_text::iterator&  it);

  void  read_expression_internal(syntax_expression&  expr, code_text::iterator&  it);
  void  read_expression(syntax_expression&  expr, int  close, code_text::iterator&  it);

public:
  syntax_rule(std::u16string_view  sv) noexcept{assign(sv);}

  syntax_rule&  operator =(std::u16string_view  sv) noexcept{return assign(sv);}
  syntax_rule&  operator+=(std::u16string_view  sv) noexcept{return append(sv);}

  syntax_rule&  assign(std::u16string_view  sv) noexcept;
  syntax_rule&  append(std::u16string_view  sv) noexcept;

        syntax_definition*  find(std::u16string_view  name)       noexcept;
  const syntax_definition*  find(std::u16string_view  name) const noexcept;

  void  print() const noexcept;

};




class
syntax_parser
{
  syntax_rule  m_rule;

  tree_struct_node<int>  m_root;

  struct frame{
    tree_struct_node<int>  m_node;

    code_text::iterator  m_iterator;

  };

  std::vector<frame>  m_stack;

  code_text::iterator  m_iterator;

  void  process_by_expression(const syntax_expression&  expr) noexcept;

public:
  void  start(std::u16string_view  def_name) noexcept;

};




}




#endif




