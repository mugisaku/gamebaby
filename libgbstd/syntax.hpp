#ifndef gbstd_syntax_HPP
#define gbstd_syntax_HPP


#include"libgbstd/utility.hpp"
#include"libgbstd/character.hpp"
#include"libgbstd/tree_struct.hpp"
#include<list>


namespace gbstd{




class
syntax_identifier
{
  std::u16string  m_string;

public:
  syntax_identifier(std::u16string&&  s) noexcept: m_string(std::move(s)){}

  std::u16string&  string() noexcept{return m_string;}

};


struct
syntax_token_parse_error
{
};


class
syntax_token
{
  enum class kind{
    null,
    identifier,
    string,
    integer,
    floating,
    others,

  } m_kind=kind::null;

  union data{
    uint64_t        i;
    std::u16string  s;
    double          f;

    data() noexcept{}
   ~data()         {}
  } m_data;

  int  m_x_index=0;
  int  m_y_index=0;

public:
  syntax_token() noexcept{}
  syntax_token(const syntax_token&   rhs) noexcept{assign(rhs);}
  syntax_token(      syntax_token&&  rhs) noexcept{assign(std::move(rhs));}
 ~syntax_token(){clear();}

  template<class...  Args>
  explicit syntax_token(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  operator bool() const noexcept{return !is_null();}

  template<class...  Args>
  syntax_token&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  syntax_token&  assign(const syntax_token&   rhs) noexcept;
  syntax_token&  assign(      syntax_token&&  rhs) noexcept;
  syntax_token&  assign(uint64_t  i) noexcept;
  syntax_token&  assign(double  f) noexcept;
  syntax_token&  assign(std::u16string&&  s) noexcept;
  syntax_token&  assign(syntax_identifier&&  id) noexcept;
  syntax_token&  assign(char16_t  c) noexcept;

  void  clear() noexcept;

  bool  is_null()       const noexcept{return m_kind == kind::null;}
  bool  is_integer()    const noexcept{return m_kind == kind::integer;}
  bool  is_identifier() const noexcept{return m_kind == kind::identifier;}
  bool  is_string()     const noexcept{return m_kind == kind::string;}
  bool  is_floating()   const noexcept{return m_kind == kind::floating;}
  bool  is_others()     const noexcept{return m_kind == kind::others;}
  bool  is_others(char16_t  c) const noexcept{return is_others() && (static_cast<char16_t>(m_data.i) == c);}

  uint64_t               get_integer()  const noexcept{return m_data.i;}
  double                 get_floating() const noexcept{return m_data.f;}
  const std::u16string&  get_string()   const noexcept{return m_data.s;}

  void  set_point(int  x, int  y) noexcept;

  void  print() const noexcept;

};


using syntax_token_string = std::vector<syntax_token>;


class
syntax_token_iterator
{
  const syntax_token*  m_current=nullptr;
  const syntax_token*  m_end    =nullptr;

  static const syntax_token  m_null;

public:
  syntax_token_iterator() noexcept{}
  syntax_token_iterator(const syntax_token_string&  s) noexcept{assign(s);}

  syntax_token_iterator&  operator=(const syntax_token_string&  s) noexcept{return assign(s);}

  syntax_token_iterator&  assign(const syntax_token_string&  s) noexcept;

  operator bool() const noexcept{return m_current < m_end;}

  bool  operator==(const syntax_token_iterator&  rhs) const noexcept{return m_current == rhs.m_current;}
  bool  operator!=(const syntax_token_iterator&  rhs) const noexcept{return m_current != rhs.m_current;}

  const syntax_token&  operator[](int  i) const noexcept{return ((m_current+i) < m_end)? m_current[i]:m_null;}

  const syntax_token&  operator*()  const noexcept{return *m_current;}
  const syntax_token*  operator->() const noexcept{return  m_current;}

  syntax_token_iterator&  operator++() noexcept;
  syntax_token_iterator   operator++(int) noexcept;

  syntax_token_iterator&  operator+=(int  n) noexcept;
  syntax_token_iterator   operator+ (int  n) const noexcept;

  bool  test(std::u16string_view  sv) const noexcept;

  void  skip() noexcept;

};


class
syntax_tokenizer
{
  code_text::iterator  m_iterator;

  syntax_token_string  m_buffer;

  void       read_binary_number() noexcept;
  void        read_octal_number() noexcept;
  void      read_decimal_number() noexcept;
  void  read_hexadecimal_number() noexcept;
  void  read_floating_point_number(uint64_t  i) noexcept;

  void  read_number_that_begins_by_zero() noexcept;
  void  read_number() noexcept;

  void  read_string();
  void  read_raw_string();

  static bool  test_head_of_identifier_defaultly(char16_t  c) noexcept;
  static bool  test_body_of_identifier_defaultly(char16_t  c) noexcept;

  bool  (*m_test_head_of_identifier)(char16_t  c)=test_head_of_identifier_defaultly;
  bool  (*m_test_body_of_identifier)(char16_t  c)=test_body_of_identifier_defaultly;

  void  step();

public:
  syntax_token_string  operator()(std::u16string_view  sv);

};


syntax_token_string  make_token_string(std::u16string_view  sv);




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

    integer_literal,
    floating_literal,
    string_literal,
    identifier,

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

  bool  is_null()             const noexcept{return m_kind == kind::null;}
  bool  is_integer_literal()  const noexcept{return m_kind == kind::integer_literal;}
  bool  is_floating_literal() const noexcept{return m_kind == kind::floating_literal;}
  bool  is_string_literal()   const noexcept{return m_kind == kind::string_literal;}
  bool  is_identifier()       const noexcept{return m_kind == kind::identifier;}
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
  const syntax_token*  m_token=nullptr;

  const syntax_definition*          m_definition=nullptr;
  const syntax_expression_element*  m_element=nullptr;

public:
  syntax_tree_node() noexcept{}
  syntax_tree_node(const syntax_token&  tok) noexcept: m_token(&tok), m_element(nullptr){}
  syntax_tree_node(const syntax_token&  tok, const syntax_definition&  def) noexcept: m_token(&tok), m_definition(&def){}
  syntax_tree_node(const syntax_token&  tok, const syntax_expression_element&  e) noexcept: m_token(&tok), m_element(&e){}

  const syntax_token*  token() const noexcept{return m_token;}

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

  syntax_token_string  m_token_string;

  syntax_tree_node  m_root;

  int  m_depth;

  bool  m_debugging=false;

  class point{
    syntax_token_iterator  m_iterator;

    const syntax_expression*  m_expression;

  public:
    point(syntax_token_iterator  it, const syntax_expression&  e) noexcept: m_iterator(it), m_expression(&e){}

    syntax_token_iterator  iterator() const noexcept{return m_iterator;}

    const syntax_expression&  expression() const noexcept{return *m_expression;}

  };

  std::vector<point>  m_point_stack;

  using node_ptr = std::unique_ptr<syntax_tree_node>;
  using storage = std::vector<node_ptr>;

  using result = std::pair<syntax_token_iterator,storage>;

  template<class...  Args>
  storage  make_storage(Args&&...  args) noexcept
  {
    storage  st;

    st.emplace_back(std::make_unique<syntax_tree_node>(std::forward<Args>(args)...));

    return std::move(st);
  }

  static void  transfer(storage&&  src, storage&  dst) noexcept;

  result  process_keyword(const syntax_expression_element&  e, syntax_token_iterator  it);
  result  process_string(const syntax_expression_element&  e, syntax_token_iterator  it);
  result  process_optional(const syntax_expression_element&  e, syntax_token_iterator  it);
  result  process_multiple(const syntax_expression_element&  e, syntax_token_iterator  it);
  result  process_definition(const syntax_expression_element&  e, syntax_token_iterator  it);
  result  process_expression(const syntax_expression_element&  e, syntax_token_iterator  it);

  result  process_by_expression_element(const syntax_expression_element&  e, syntax_token_iterator  it);
  result  process_by_expression(const syntax_expression&  expr, syntax_token_iterator  it);

  result  process_and(  const syntax_expression_element&  l, const syntax_expression_element&  r, syntax_token_iterator  it);
  result  process_or(   const syntax_expression_element&  l, const syntax_expression_element&  r, syntax_token_iterator  it);
  result  process_colon(const syntax_expression&  expr, syntax_token_iterator  it);

  syntax_token_iterator  step(const syntax_definition&  def, syntax_token_iterator  it);

  std::pair<syntax_token_iterator,node_ptr>  process_by_definition(const syntax_definition&  def, syntax_token_iterator  it);

public:
        syntax_rule&  get_rule()       noexcept{return m_rule;}
  const syntax_rule&  get_rule() const noexcept{return m_rule;}

        code_text&  get_text()       noexcept{return m_text;}
  const code_text&  get_text() const noexcept{return m_text;}

  void  start(std::u16string_view  def_name);

  void  reset() noexcept;

  void  print() const noexcept;

};




}




#endif




