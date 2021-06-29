#ifndef gbstd_syntax_HPP
#define gbstd_syntax_HPP


#include"libgbstd/utility.hpp"
#include"libgbstd/character.hpp"
#include"libgbstd/file_op.hpp"
#include<list>


namespace gbstd{




struct syntax_identifier{};


struct
syntax_token_parse_error
{
};


class
syntax_token_info
{
  source_code   m_source;

  int  m_x_position=0;
  int  m_y_position=0;

public:
  syntax_token_info() noexcept{}
  syntax_token_info(const source_code::iterator&  it) noexcept{assign(it);}

  syntax_token_info&  operator=(const source_code::iterator&  it) noexcept{return assign(it);}

  syntax_token_info&  assign(const source_code::iterator&  it) noexcept{
    m_source = it.source();

    m_x_position = it.x_position();
    m_y_position = it.y_position();

    return *this;
  }

  void  clear() noexcept{
    m_source.unrefer();

    m_x_position = 0;
    m_y_position = 0;
  }

  const source_code&  source() const noexcept{return m_source;}

  int  x_position() const noexcept{return m_x_position;}
  int  y_position() const noexcept{return m_y_position;}

  void  print() const noexcept{printf("[%3d,%3d]",1+m_y_position,1+m_x_position);}

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

  syntax_token_info  m_info;

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
  syntax_token&  assign(std::u16string&&  s, syntax_identifier) noexcept;
  syntax_token&  assign(char16_t  c) noexcept;

  void  clear() noexcept;

  bool  is_null()       const noexcept{return m_kind == kind::null;}
  bool  is_integer()    const noexcept{return m_kind == kind::integer;}
  bool  is_identifier() const noexcept{return m_kind == kind::identifier;}
  bool  is_string()     const noexcept{return m_kind == kind::string;}
  bool  is_floating()   const noexcept{return m_kind == kind::floating;}
  bool  is_others()     const noexcept{return m_kind == kind::others;}
  bool  is_others(char16_t  c) const noexcept{return is_others() && (static_cast<char16_t>(m_data.i) == c);}

  bool      is_string(std::u16string_view  sv) const noexcept{return     is_string() && (m_data.s == sv);}
  bool  is_identifier(std::u16string_view  sv) const noexcept{return is_identifier() && (m_data.s == sv);}

  bool  is(std::u16string_view  sv) const noexcept{return (is_string() || is_identifier()) && (m_data.s == sv);}

  uint64_t              integer() const noexcept{return m_data.i;}
  double               floating() const noexcept{return m_data.f;}
  const std::u16string&  string() const noexcept{return m_data.s;}

  syntax_token&  add_info(syntax_token_info&&  info) noexcept{  m_info = std::move(info);  return *this;}

  const syntax_token_info&  info() const noexcept{return m_info;}

  void  print() const noexcept;

};


class
syntax_token_string
{
  std::vector<syntax_token>  m_buffer;

  void       read_binary_number(source_code::iterator&  it) noexcept;
  void        read_octal_number(source_code::iterator&  it) noexcept;
  void      read_decimal_number(source_code::iterator&  it) noexcept;
  void  read_hexadecimal_number(source_code::iterator&  it) noexcept;
  void  read_floating_point_number(source_code::iterator&  it, uint64_t  i) noexcept;

  void  read_number_that_begins_by_zero(source_code::iterator&  it) noexcept;
  void  read_number(source_code::iterator&  it) noexcept;

  void  read_string(source_code::iterator&  it);
  void  read_raw_string(source_code::iterator&  it);

  static bool  test_head_of_identifier(char16_t  c) noexcept;
  static bool  test_body_of_identifier(char16_t  c) noexcept;

  void  step(source_code::iterator&  it);

public:
  syntax_token_string(){}
  syntax_token_string(const source_code&  src){assign(src);}

  syntax_token_string&  operator=(const source_code&  src){return assign(src);}

  syntax_token_string&  assign(const source_code&  src);

  const syntax_token*  data() const noexcept{return m_buffer.data();}
  size_t  size() const noexcept{return m_buffer.size();}

  class  iterator;

};


class
syntax_token_string::
iterator
{
  const syntax_token*  m_current=nullptr;
  const syntax_token*  m_end    =nullptr;

  static const syntax_token  m_null;

public:
  iterator() noexcept{}
  iterator(const syntax_token_string&  s) noexcept{assign(s);}

  iterator&  operator=(const syntax_token_string&  s) noexcept{return assign(s);}

  iterator&  assign(const syntax_token_string&  s) noexcept;

  operator bool() const noexcept{return m_current < m_end;}

  bool  operator==(const iterator&  rhs) const noexcept{return m_current == rhs.m_current;}
  bool  operator!=(const iterator&  rhs) const noexcept{return m_current != rhs.m_current;}

  const syntax_token&  operator[](int  i) const noexcept{return ((m_current+i) < m_end)? m_current[i]:m_null;}

  const syntax_token&  operator*()  const noexcept{return *m_current;}
  const syntax_token*  operator->() const noexcept{return  m_current;}

  iterator&  operator++() noexcept;
  iterator   operator++(int) noexcept;

  iterator&  operator+=(int  n) noexcept;
  iterator   operator+ (int  n) const noexcept;

  bool  test(std::u16string_view  sv) const noexcept;

  void  skip() noexcept;

};




class syntax_element;
class syntax_formula;


class
syntax_formula_error
{
};


class
syntax_formula
{
public:
  using pointer = std::unique_ptr<syntax_element>;

private:
  std::u16string  m_name;

  int  m_code=0;

  pointer  m_left ;
  pointer  m_right;

public:
  syntax_formula() noexcept{}
  syntax_formula(const std::u16string&  name) noexcept: m_name(name){}
  syntax_formula(std::u16string_view  name) noexcept: m_name(name){}
  syntax_formula(const syntax_formula&   rhs) noexcept{assign(rhs);}
  syntax_formula(      syntax_formula&&  rhs) noexcept{assign(std::move(rhs));}

  template<class... Args>
  syntax_formula(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class... Args>
  syntax_formula&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  syntax_formula&  operator=(const syntax_formula&   rhs) noexcept{return assign(rhs);}
  syntax_formula&  operator=(      syntax_formula&&  rhs) noexcept{return assign(std::move(rhs));}

  syntax_formula&  assign(const syntax_formula&   rhs) noexcept;
  syntax_formula&  assign(      syntax_formula&&  rhs) noexcept;
  syntax_formula&  assign(int  c, syntax_element&&  l, syntax_element&&  r) noexcept;
  syntax_formula&  assign(syntax_element&&  e) noexcept;

  syntax_formula&  set_name(std::u16string_view  name) noexcept{  m_name = name;  return *this;}
  syntax_formula&  set_code(int  c) noexcept{  m_code = c;  return *this;}
  syntax_formula&   set_left(syntax_element&&  e) noexcept;
  syntax_formula&  set_right(syntax_element&&  e) noexcept;

  const std::u16string&  name() const noexcept{return m_name;}

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

        std::u16string&  string()       noexcept{return m_string;}
  const std::u16string&  string() const noexcept{return m_string;}

};


class
syntax_reference
{
  std::u16string  m_string;

public:
  explicit syntax_reference(std::u16string_view  sv) noexcept: m_string(sv){}
  explicit syntax_reference(std::u16string&&  s) noexcept: m_string(std::move(s)){}

        std::u16string&  string()       noexcept{return m_string;}
  const std::u16string&  string() const noexcept{return m_string;}

};


class
syntax_optional_formula
{
  syntax_formula  m_formula;

public:
  explicit syntax_optional_formula(syntax_formula&&  f) noexcept: m_formula(std::move(f)){}

  syntax_formula&  formula() noexcept{return m_formula;}

};


class
syntax_multiple_formula
{
  syntax_formula  m_formula;

public:
  explicit syntax_multiple_formula(syntax_formula&&  f) noexcept: m_formula(std::move(f)){}

  syntax_formula&  formula() noexcept{return m_formula;}

};


class
syntax_element
{
  enum class kind{
    null,

    integer_literal,
    floating_literal,
    string_literal,
    identifier,

    keyword,
    string,
    formula,
    optional_formula,
    multiple_formula,
    reference,
  } m_kind=kind::null;

  union data{
    std::u16string  s;
    syntax_formula  f;

  data() noexcept{}
 ~data()         {}

  } m_data;

public:
  syntax_element() noexcept{}
  syntax_element(const syntax_element&   rhs) noexcept{assign(rhs);}
  syntax_element(      syntax_element&&  rhs) noexcept{assign(std::move(rhs));}
 ~syntax_element(){clear();}

  template<class... Args>
  syntax_element(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class... Args>
  syntax_element&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  syntax_element&  operator=(const syntax_element&   rhs) noexcept{return assign(rhs);}
  syntax_element&  operator=(      syntax_element&&  rhs) noexcept{return assign(std::move(rhs));}

  syntax_element&  assign(const syntax_element&   rhs) noexcept;
  syntax_element&  assign(      syntax_element&&  rhs) noexcept;
  syntax_element&  assign(std::u16string_view  sv) noexcept;
  syntax_element&  assign(const std::u16string&  s) noexcept{return assign(std::u16string_view(s));}
  syntax_element&  assign(std::u16string&&  s) noexcept;
  syntax_element&  assign(syntax_keyword&&  kw) noexcept;
  syntax_element&  assign(syntax_formula&&  f) noexcept;
  syntax_element&  assign(syntax_optional_formula&&  f) noexcept;
  syntax_element&  assign(syntax_multiple_formula&&  f) noexcept;
  syntax_element&  assign(syntax_reference&&  ref) noexcept;

  bool  is_null()             const noexcept{return m_kind == kind::null;}
  bool  is_integer_literal()  const noexcept{return m_kind == kind::integer_literal;}
  bool  is_floating_literal() const noexcept{return m_kind == kind::floating_literal;}
  bool  is_string_literal()   const noexcept{return m_kind == kind::string_literal;}
  bool  is_identifier()       const noexcept{return m_kind == kind::identifier;}
  bool  is_keyword()    const noexcept{return m_kind == kind::keyword;}
  bool  is_string()     const noexcept{return m_kind == kind::string;}
  bool  is_formula() const noexcept{return m_kind == kind::formula;}
  bool  is_optional_formula() const noexcept{return m_kind == kind::optional_formula;}
  bool  is_multiple_formula() const noexcept{return m_kind == kind::multiple_formula;}
  bool  is_reference() const noexcept{return m_kind == kind::reference;}

  void  clear() noexcept;

  const std::u16string&   string() const noexcept{return m_data.s;}
  const syntax_formula&  formula() const noexcept{return m_data.f;}

  void  print() const noexcept;

};


class
syntax_rule
{
  std::list<syntax_formula>  m_formula_list;

  void  start_read(syntax_token_string::iterator&  it);

  class wrapper{
    int  m_code;

    syntax_element  m_element;

  public:
    wrapper(int  c) noexcept: m_code(c){}
    wrapper(syntax_element&&  e) noexcept: m_code(0), m_element(std::move(e)){}

    bool  is_code() const noexcept{return m_code;}

    int  code() const noexcept{return m_code;}

    syntax_element&  element() noexcept{return m_element;}

    void  print() const noexcept;

  };

  wrapper            read_formula_internal(syntax_token_string::iterator&  it);
  syntax_formula  read_formula(char16_t  close, syntax_token_string::iterator&  it);

  syntax_formula  make_formula(std::vector<wrapper>&&  stk);

public:
  syntax_rule() noexcept{}
  syntax_rule(const source_code&  src) noexcept{assign(src);}

  operator bool() const noexcept{return m_formula_list.size();}

  syntax_rule&  operator =(const source_code&  src) noexcept{return assign(src);}
  syntax_rule&  operator+=(const source_code&  src) noexcept{return append(src);}

  syntax_rule&  assign(const source_code&  src) noexcept;
  syntax_rule&  append(const source_code&  src) noexcept;

        syntax_formula*  find(std::u16string_view  name)       noexcept;
  const syntax_formula*  find(std::u16string_view  name) const noexcept;

  const syntax_formula&  first() const noexcept{return m_formula_list.front();}

  void  print() const noexcept;

};




class syntax_branch;
class syntax_branch_element;


class
syntax_branch
{
  std::u16string_view  m_name;

  std::vector<syntax_branch_element>  m_elements;

public:
  syntax_branch() noexcept{}
  syntax_branch(const std::u16string&  name) noexcept: m_name(name){}
  syntax_branch(std::u16string_view  name) noexcept: m_name(name){}
  syntax_branch(std::vector<syntax_branch_element>&&  els) noexcept: m_elements(std::move(els)){}
  syntax_branch(syntax_branch_element&&  e) noexcept;

  template<class...  Args>
  syntax_branch(std::u16string_view  name, Args&&...  args) noexcept: m_name(name){append(std::forward<Args>(args)...);}

  const syntax_branch_element&  operator[](int  i) const noexcept;

  syntax_branch&  operator+=(syntax_branch_element&&  el) noexcept{return append(std::move(el));}

  std::u16string_view  name() const noexcept{return m_name;}

  bool  has_name() const noexcept{return m_name.size();}

  syntax_branch&  set_name(std::u16string_view  name) noexcept{  m_name = name;  return *this;}

  int  length() const noexcept;

  syntax_branch&  append(syntax_branch_element&&  el) noexcept;
  syntax_branch&  append(std::vector<syntax_branch_element>&&  els) noexcept;

  void  print(int  indent=0) const noexcept;

  std::vector<syntax_branch_element>  release() noexcept{return std::move(m_elements);}

  const syntax_branch_element*  begin() const noexcept;
  const syntax_branch_element*    end() const noexcept;

};


class
syntax_branch_element
{
  const syntax_token*  m_token=nullptr;

  syntax_branch  m_branch;

  bool  m_keyword_flag=false;

public:
  syntax_branch_element() noexcept{}
  syntax_branch_element(const syntax_branch_element&   rhs) noexcept{assign(rhs);}
  syntax_branch_element(      syntax_branch_element&&  rhs) noexcept{assign(std::move(rhs));}

  template<class...  Args>
  syntax_branch_element(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  template<class...  Args>
  syntax_branch_element&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  syntax_branch_element&  operator=(const syntax_branch_element&   rhs) noexcept{return assign(rhs);}
  syntax_branch_element&  operator=(      syntax_branch_element&&  rhs) noexcept{return assign(std::move(rhs));}

  syntax_branch_element&  assign(const syntax_branch_element&   rhs) noexcept;
  syntax_branch_element&  assign(      syntax_branch_element&&  rhs) noexcept;

  syntax_branch_element&  assign(const syntax_token&  tok, bool  k=false) noexcept;
  syntax_branch_element&  assign(syntax_branch&&  bra) noexcept;

  bool  is_keyword(                       ) const noexcept{return m_keyword_flag;}
  bool  is_keyword(std::u16string_view  sv) const noexcept{return is_keyword() && (m_token->string() == sv);}

  bool  is_token()  const noexcept{return  m_token;}
  bool  is_branch() const noexcept{return !m_token;}
  bool  is_branch(std::u16string_view  name) const noexcept{return is_branch() && (m_branch.name() == name);}

  const syntax_token&    token() const noexcept{return *m_token;}
  const syntax_branch&  branch() const noexcept{return m_branch;}

  uint64_t            integer() const noexcept{return m_token->integer();}
  double             floating() const noexcept{return m_token->floating();}
  std::u16string_view  string() const noexcept{return m_token->string();}

  void  print(int  indent) const noexcept;

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
    syntax_token_string::iterator  m_iterator;

    const syntax_formula*  m_formula;

  public:
    point(syntax_token_string::iterator  it, const syntax_formula&  e) noexcept: m_iterator(it), m_formula(&e){}

    syntax_token_string::iterator  iterator() const noexcept{return m_iterator;}
    const syntax_formula&   formula() const noexcept{return *m_formula;}

  };

  std::vector<point>  m_point_stack;

  class result{
    syntax_token_string::iterator  m_iterator;

    std::vector<syntax_branch_element>  m_elements;

  public:
    result() noexcept{}
    result(result&&  l, result&&  r) noexcept;
    result(syntax_token_string::iterator  it) noexcept: m_iterator(it){}
    result(syntax_token_string::iterator  it, syntax_branch_element&&  e) noexcept: m_iterator(it){m_elements.emplace_back(std::move(e));}

    operator bool() const noexcept{return m_iterator;}

    syntax_token_string::iterator  iterator() const noexcept{return m_iterator;}

    std::vector<syntax_branch_element>  release() noexcept{return std::move(m_elements);}

  };


  result  process_keyword(const syntax_element&  e, syntax_token_string::iterator  it);
  result  process_string(const syntax_element&  e, syntax_token_string::iterator  it);
  result  process_optional(const syntax_element&  e, syntax_token_string::iterator  it);
  result  process_multiple(const syntax_element&  e, syntax_token_string::iterator  it);

  result  process_by_element(const syntax_element&  e, syntax_token_string::iterator  it);
  result  process_by_formula(const syntax_formula&  f, syntax_token_string::iterator  it);

  result  process_and(  const syntax_element&  l, const syntax_element&  r, syntax_token_string::iterator  it);
  result  process_or(   const syntax_element&  l, const syntax_element&  r, syntax_token_string::iterator  it);
  result  process_colon(const syntax_formula&  expr, syntax_token_string::iterator  it);

  result  step(const syntax_formula&  f, syntax_token_string::iterator  it);

  result  process_by_named_formula(const syntax_formula&  f, syntax_token_string::iterator  it);
  result  process_by_reference(std::u16string_view  name, syntax_token_string::iterator  it);

public:
  syntax_parser&  set_rule(const syntax_rule&  r) noexcept;

  const syntax_rule*  rule() const noexcept{return m_rule;}

  syntax_branch  start(const syntax_token_string&  toks);

};




}




#endif




