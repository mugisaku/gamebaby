#ifndef gbstd_syntax_HPP
#define gbstd_syntax_HPP


#include"libgbstd/utility.hpp"
#include"libgbstd/character.hpp"
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




class syntax_operand;
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
  using pointer = std::unique_ptr<syntax_operand>;

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
  syntax_expression&  assign(int  c, syntax_operand&&  l, syntax_operand&&  r) noexcept;
  syntax_expression&  assign(syntax_operand&&  e) noexcept;

  syntax_expression&  set_code(int  c) noexcept{  m_code = c;  return *this;}
  syntax_expression&   set_left(syntax_operand&&  e) noexcept;
  syntax_expression&  set_right(syntax_operand&&  e) noexcept;

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
syntax_operand
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
  syntax_operand() noexcept{}
  syntax_operand(const syntax_operand&   rhs) noexcept{assign(rhs);}
  syntax_operand(      syntax_operand&&  rhs) noexcept{assign(std::move(rhs));}
 ~syntax_operand(){clear();}

  template<class... Args>
  syntax_operand(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class... Args>
  syntax_operand&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  syntax_operand&  operator=(const syntax_operand&   rhs) noexcept{return assign(rhs);}
  syntax_operand&  operator=(      syntax_operand&&  rhs) noexcept{return assign(std::move(rhs));}

  syntax_operand&  assign(const syntax_operand&   rhs) noexcept;
  syntax_operand&  assign(      syntax_operand&&  rhs) noexcept;
  syntax_operand&  assign(std::u16string_view  sv) noexcept;
  syntax_operand&  assign(std::u16string&&  s) noexcept;
  syntax_operand&  assign(syntax_keyword&&  kw) noexcept;
  syntax_operand&  assign(syntax_expression&&  exp) noexcept;
  syntax_operand&  assign(syntax_optional_expression&&  exp) noexcept;
  syntax_operand&  assign(syntax_multiple_expression&&  exp) noexcept;
  syntax_operand&  assign(const syntax_definition&  def) noexcept;

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

  void  start_read(syntax_token_iterator&  it);

  class wrapper{
    int  m_code;

    syntax_operand  m_operand;

  public:
    wrapper(int  c) noexcept: m_code(c){}
    wrapper(syntax_operand&&  o) noexcept: m_code(0), m_operand(std::move(o)){}

    bool  is_code() const noexcept{return m_code;}

    int  code() const noexcept{return m_code;}

    syntax_operand&  operand() noexcept{return m_operand;}

    void  print() const noexcept;

  };

  wrapper            read_expression_internal(syntax_token_iterator&  it);
  syntax_expression  read_expression(char16_t  close, syntax_token_iterator&  it);

  syntax_expression  make_expression(std::vector<wrapper>&&  stk);

public:
  syntax_rule() noexcept{}
  syntax_rule(std::string_view  sv) noexcept{assign(sv);}
  syntax_rule(std::u16string_view  sv) noexcept{assign(sv);}

  operator bool() const noexcept{return m_definition_list.size();}

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

  const syntax_definition&  first() const noexcept{return m_definition_list.front();}

  void  print() const noexcept;

};




class syntax_branch;
class syntax_branch_source;
class syntax_branch_element;


class
syntax_branch_source
{
  code_text  m_text;

  syntax_token_string  m_token_string;

public:
  syntax_branch_source() noexcept{}
  syntax_branch_source(std::string_view  sv) noexcept{assign(sv);}
  syntax_branch_source(std::u16string_view  sv) noexcept{assign(sv);}

  syntax_branch_source&  assign(std::string_view  sv) noexcept
  {
    m_text = sv;
    m_token_string = make_token_string(m_text.get_string());

    return *this;
  }

  syntax_branch_source&  assign(std::u16string_view  sv) noexcept
  {
    m_text = sv;
    m_token_string = make_token_string(m_text.get_string());

    return *this;
  }

  syntax_token_iterator  iterator() const noexcept{return {m_token_string};}

};


class
syntax_branch
{
  std::vector<syntax_branch_element>  m_elements;

public:
  syntax_branch() noexcept{}
  syntax_branch(syntax_branch_element&&  e) noexcept;

  int  length() const noexcept;

  template<class...  Args>
  syntax_branch_element&  emplace_back(Args&&...  args) noexcept{return m_elements.emplace_back(std::forward<Args>(args)...);}

  void  cut_back(int  l=0) noexcept;

  void  splice(syntax_branch&&  rhs) noexcept;
  void  print() const noexcept;

};


class
syntax_branch_element
{
  const syntax_token*  m_token=nullptr;

  syntax_operand  m_operand;
  syntax_branch    m_branch;

public:
  syntax_branch_element() noexcept{}
  syntax_branch_element(const syntax_branch_element&   rhs) noexcept{assign(rhs);}
  syntax_branch_element(      syntax_branch_element&&  rhs) noexcept{assign(std::move(rhs));}
 ~syntax_branch_element(){clear();}

  template<class...  Args>
  syntax_branch_element(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  syntax_branch_element&  operator=(const syntax_branch_element&   rhs) noexcept{return assign(rhs);}
  syntax_branch_element&  operator=(      syntax_branch_element&&  rhs) noexcept{return assign(std::move(rhs));}

  syntax_branch_element&  assign(const syntax_branch_element&   rhs) noexcept;
  syntax_branch_element&  assign(      syntax_branch_element&&  rhs) noexcept;

  syntax_branch_element&  assign(const syntax_token&  tok, const syntax_operand&  o) noexcept;
  syntax_branch_element&  assign(const syntax_token&  tok, const syntax_definition&  def, syntax_branch&&  br) noexcept;

  void  clear() noexcept;

  const syntax_token&      token() const noexcept{return *m_token;}
  const syntax_operand&  operand() const noexcept{return m_operand;}
  const syntax_branch&    branch() const noexcept{return m_branch;}

  syntax_branch  cut_child() noexcept;

  void  print() const noexcept;

};


struct
syntax_parse_error
{
};


class
syntax_parser
{
  const syntax_rule*  m_rule=nullptr;

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

  class result{
    syntax_token_iterator  m_iterator;
    syntax_branch            m_branch;

  public:
    result() noexcept{}
    result(syntax_token_iterator  it, syntax_branch&&  br=syntax_branch()) noexcept: m_iterator(it), m_branch(std::move(br)){}

    operator bool() const noexcept{return m_iterator;}

    syntax_token_iterator  iterator() const noexcept{return m_iterator;}
    syntax_branch&           branch()       noexcept{return m_branch;}

    syntax_branch  release() noexcept{return std::move(m_branch);}

  };


  static result  combine(result&&  l, result&&  r) noexcept;

  result  process_keyword(const syntax_operand&  e, syntax_token_iterator  it);
  result  process_string(const syntax_operand&  e, syntax_token_iterator  it);
  result  process_optional(const syntax_operand&  e, syntax_token_iterator  it);
  result  process_multiple(const syntax_operand&  e, syntax_token_iterator  it);

  result  process_by_operand(const syntax_operand&  o, syntax_token_iterator  it);
  result  process_by_expression(const syntax_expression&  expr, syntax_token_iterator  it);

  result  process_and(  const syntax_operand&  l, const syntax_operand&  r, syntax_token_iterator  it);
  result  process_or(   const syntax_operand&  l, const syntax_operand&  r, syntax_token_iterator  it);
  result  process_colon(const syntax_expression&  expr, syntax_token_iterator  it);

  result  step(const syntax_definition&  def, syntax_token_iterator  it);

  result  process_by_definition(const syntax_definition&  def, syntax_token_iterator  it);

public:
  syntax_parser&  set_rule(const syntax_rule&  r) noexcept;

  const syntax_rule*  rule() const noexcept{return m_rule;}

  syntax_branch  start(const syntax_branch_source&  src);

};




}




#endif




