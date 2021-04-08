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
public:
  using pointer = std::unique_ptr<syntax_expression_element>;

private:
  int  m_code;

  pointer  m_left ;
  pointer  m_right;

public:
  syntax_expression() noexcept{}
  syntax_expression(const syntax_expression&   rhs) noexcept{assign(rhs);}
  syntax_expression(      syntax_expression&&  rhs) noexcept{assign(std::move(rhs));}

  template<class... Args>
  syntax_expression(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class... Args>
  syntax_expression&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  syntax_expression&  operator=(const syntax_expression&   rhs) noexcept{return assign(rhs);}
  syntax_expression&  operator=(      syntax_expression&&  rhs) noexcept{return assign(std::move(rhs));}

  syntax_expression&  assign(const syntax_expression&   rhs) noexcept;
  syntax_expression&  assign(      syntax_expression&&  rhs) noexcept;
  syntax_expression&  assign(int  c, syntax_expression_element&&  l, syntax_expression_element&&  r) noexcept;
  syntax_expression&  assign(syntax_expression_element&&  e) noexcept;

  syntax_expression&  set_code(int  c) noexcept{  m_code = c;  return *this;}
  syntax_expression&   set_left(syntax_expression_element&&  e) noexcept;
  syntax_expression&  set_right(syntax_expression_element&&  e) noexcept;

  int  code() const noexcept{return m_code;}

  const pointer&   left() const noexcept{return  m_left;}
  const pointer&  right() const noexcept{return m_right;}

  void  print_for_test() const noexcept;
  void  print() const noexcept;

};


class
syntax_keyword
{
  std::u16string  m_string;

public:
  explicit syntax_keyword(std::u16string_view  sv) noexcept: m_string(sv){}
  explicit syntax_keyword(std::u16string&&  s) noexcept: m_string(std::move(s)){}

        std::u16string&  get_string()       noexcept{return m_string;}
  const std::u16string&  get_string() const noexcept{return m_string;}

};


class
syntax_optional_expression
{
  syntax_expression  m_expression;

public:
  explicit syntax_optional_expression(syntax_expression&&  e) noexcept: m_expression(std::move(e)){}

  syntax_expression&  get_expression() noexcept{return m_expression;}

};


class
syntax_multiple_expression
{
  syntax_expression  m_expression;

public:
  explicit syntax_multiple_expression(syntax_expression&&  e) noexcept: m_expression(std::move(e)){}

  syntax_expression&  get_expression() noexcept{return m_expression;}

};


class
syntax_expression_element
{
  enum class kind{
    null,
    keyword,
    string,
    expression,
    optional_expression,
    multiple_expression,
    definition,
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
  syntax_expression_element&  assign(syntax_keyword&&  kw) noexcept;
  syntax_expression_element&  assign(syntax_expression&&  exp) noexcept;
  syntax_expression_element&  assign(syntax_optional_expression&&  exp) noexcept;
  syntax_expression_element&  assign(syntax_multiple_expression&&  exp) noexcept;
  syntax_expression_element&  assign(const syntax_definition&  def) noexcept;

  bool  is_null()       const noexcept{return m_kind == kind::null;}
  bool  is_keyword()    const noexcept{return m_kind == kind::keyword;}
  bool  is_string()     const noexcept{return m_kind == kind::string;}
  bool  is_expression() const noexcept{return m_kind == kind::expression;}
  bool  is_optional_expression() const noexcept{return m_kind == kind::optional_expression;}
  bool  is_multiple_expression() const noexcept{return m_kind == kind::multiple_expression;}
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
  std::u16string  m_name;

public:
  syntax_definition(std::u16string_view  name) noexcept: m_name(name){}

  const std::u16string&  get_name() const noexcept{return m_name;}

  void  print() const noexcept;

};


class
syntax_rule
{
  std::list<syntax_definition>  m_definition_list;

  void  start_read(code_text::iterator&  it);

  syntax_keyword   read_keyword(code_text::iterator&  it);
  std::u16string   read_string(code_text::iterator&  it);
  std::u16string   read_identifier(code_text::iterator&  it);

  class wrapper{
    int  m_code;

    syntax_expression_element  m_element;

  public:
    wrapper(int  c) noexcept: m_code(c){}
    wrapper(syntax_expression_element&&  e) noexcept: m_code(0), m_element(std::move(e)){}

    bool  is_code() const noexcept{return m_code;}

    int  code() const noexcept{return m_code;}

    syntax_expression_element&  element() noexcept{return m_element;}

    void  print() const noexcept;

  };

  wrapper            read_expression_internal(code_text::iterator&  it);
  syntax_expression  read_expression(int  close, code_text::iterator&  it);

  syntax_expression  make_expression(std::vector<wrapper>&&  stk);

public:
  syntax_rule() noexcept{}
  syntax_rule(std::string_view  sv) noexcept{assign(sv);}
  syntax_rule(std::u16string_view  sv) noexcept{assign(sv);}

  syntax_rule&  operator =(std::string_view  sv) noexcept{return assign(sv);}
  syntax_rule&  operator+=(std::string_view  sv) noexcept{return append(sv);}
  syntax_rule&  operator =(std::u16string_view  sv) noexcept{return assign(sv);}
  syntax_rule&  operator+=(std::u16string_view  sv) noexcept{return append(sv);}

  syntax_rule&  assign(std::string_view     sv) noexcept;
  syntax_rule&  assign(std::u16string_view  sv) noexcept;
  syntax_rule&  append(std::string_view     sv) noexcept;
  syntax_rule&  append(std::u16string_view  sv) noexcept;

        syntax_definition*  find(std::u16string_view  name)       noexcept;
  const syntax_definition*  find(std::u16string_view  name) const noexcept;

  void  print() const noexcept;

};




class
syntax_tree_node: public tree_struct_node
{
  code_text::iterator  m_iterator;

  const syntax_definition*          m_definition=nullptr;
  const syntax_expression_element*  m_element=nullptr;

public:
  syntax_tree_node() noexcept{}
  syntax_tree_node(const code_text::iterator&  it) noexcept: m_iterator(it), m_element(nullptr){}
  syntax_tree_node(const code_text::iterator&  it, const syntax_definition&  def) noexcept: m_iterator(it), m_definition(&def){}
  syntax_tree_node(const code_text::iterator&  it, const syntax_expression_element&  e) noexcept: m_iterator(it), m_element(&e){}

  syntax_tree_node&  set_iterator(const code_text::iterator&  it) noexcept{  m_iterator = it;  return *this;}
  const code_text::iterator&  iterator() const noexcept{return m_iterator;}

  const syntax_definition*  definition() const noexcept{return m_definition;}
  const syntax_expression_element*  element() const noexcept{return m_element;}

  void  print() const noexcept;

};


struct
syntax_parse_error
{
};


class
syntax_parser
{
  syntax_rule  m_rule;

  code_text  m_text;

  std::vector<const syntax_definition*>  m_def_stack;

  syntax_tree_node  m_root;

  struct point{
    code_text::iterator  m_iterator;

    const syntax_expression*  m_expression;

  public:
    point(code_text::iterator  it, const syntax_expression&  e) noexcept: m_iterator(it), m_expression(&e){}

  };

  std::vector<point>  m_point_stack;

  using node_ptr = std::unique_ptr<syntax_tree_node>;
  using storage = std::vector<node_ptr>;

  using result = std::pair<code_text::iterator,storage>;

  static void  transfer(storage&&  src, storage&  dst) noexcept;

  result  process_keyword(const syntax_expression_element&  e, code_text::iterator  it);
  result  process_string(const syntax_expression_element&  e, code_text::iterator  it);
  result  process_optional(const syntax_expression_element&  e, code_text::iterator  it);
  result  process_multiple(const syntax_expression_element&  e, code_text::iterator  it);
  result  process_definition(const syntax_expression_element&  e, code_text::iterator  it);
  result  process_expression(const syntax_expression_element&  e, code_text::iterator  it);

  result  process_by_expression_element(const syntax_expression_element&  e, code_text::iterator  it);
  result  process_by_expression(const syntax_expression&  expr, code_text::iterator  it);

  result  process_and(  const syntax_expression_element&  l, const syntax_expression_element&  r, code_text::iterator  it);
  result  process_or(   const syntax_expression_element&  l, const syntax_expression_element&  r, code_text::iterator  it);
  result  process_colon(const syntax_expression&  expr, code_text::iterator  it);

  void  process_top(const syntax_definition&  def);

  std::pair<code_text::iterator,node_ptr>  process_by_definition(const syntax_definition&  def, code_text::iterator  it);

public:
        syntax_rule&  get_rule()       noexcept{return m_rule;}
  const syntax_rule&  get_rule() const noexcept{return m_rule;}

        code_text&  get_text()       noexcept{return m_text;}
  const code_text&  get_text() const noexcept{return m_text;}

  void  start(std::u16string_view  def_name);

};




}




#endif




