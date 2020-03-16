#ifndef gbstd_parser_HPP
#define gbstd_parser_HPP


#include<cstddef>
#include<cstdint>
#include<cinttypes>
#include<string>
#include<vector>
#include"libgbstd/utility.hpp"


namespace gbstd{


class
operator_code
{
  char  m_data[4];

  static constexpr size_t  len(const char*  s) noexcept
  {
    return !s[0]? 0
          :!s[1]? 1
          :!s[2]? 2
          :       3;
          
  }

  static constexpr char  chr(const char*  s, int  i) noexcept
  {
    return (i < len(s))? s[i]:0;
  }

public:
  constexpr operator_code() noexcept: m_data{0,0,0,0}{}
  constexpr operator_code(const char*  s) noexcept: m_data{chr(s,0),chr(s,1),chr(s,2),0}{}

  constexpr  operator bool() const noexcept{return m_data[0];}

  constexpr const char*  get_string() const noexcept{return m_data;}
  constexpr int  get_length() const noexcept{return len(m_data);}

  constexpr  operator uint32_t() const noexcept{return (m_data[0]<<24)|(m_data[1]<<16)|(m_data[2]<<8);}

  constexpr bool  operator==(operator_code  rhs) const noexcept
  {
    return (m_data[0] == rhs.m_data[0]) &&
           (m_data[1] == rhs.m_data[1]) &&
           (m_data[2] == rhs.m_data[2]);
  }

  constexpr bool  operator!=(operator_code  rhs) const noexcept
  {
    return !(*this == rhs);
  }

};


class token;
class tokenizer;


class
token_block
{
  std::vector<token>  m_container;

  operator_code  m_open ;
  operator_code  m_close;

public:
   token_block() noexcept{}
   token_block(std::string_view  sv);
   token_block(operator_code  open, operator_code  close) noexcept: m_open(open), m_close(close){}
  ~token_block(){clear();}

  const token&  operator[](int  i) const noexcept{return m_container[i];}

  token_block&  operator+=(token&&  tok) noexcept{  m_container.emplace_back(std::move(tok));  return *this;}

  void  clear() noexcept;

  bool  test(operator_code  open, operator_code  close) const noexcept{return (m_open == open) && (m_close == close);}

  operator_code   get_open_code() const noexcept{return m_open;}
  operator_code  get_close_code() const noexcept{return m_close;}

  const std::vector<token>*  operator->() const noexcept{return &m_container;}

  void  print(const char*  base=nullptr, int  indent=0) const noexcept;

};


class
token
{
  const char*  m_begin=nullptr;
  const char*  m_end  =nullptr;

  enum class kind{
    null,
    identifier,
    single_quoted,
    double_quoted,
    integer,
    floating_point_number,
    operator_code,
    block,

  } m_kind=kind::null;

  union data{
    uint64_t     n;
    std::string  s;
    double       f;

    operator_code  opco;

    token_block  blk;

    data() noexcept{}
   ~data()         {}
  } m_data;


  static void  print_string(const std::string&  s, char  punct) noexcept;

public:
  token() noexcept{}
  token(const token&   rhs) noexcept{assign(rhs);}
  token(      token&&  rhs) noexcept{assign(std::move(rhs));}
  token(const char*  begin, const char*  end, uint64_t  n)                noexcept{assign(begin,end,n);}
  token(const char*  begin, const char*  end, double  f)                  noexcept{assign(begin,end,f);}
  token(const char*  begin, const char*  end, std::string&&  s, int  sym) noexcept{assign(begin,end,std::move(s),sym);}
  token(const char*  begin, const char*  end, operator_code  opco)        noexcept{assign(begin,end,opco);}
  token(const char*  begin, const char*  end, token_block&&  blk)         noexcept{assign(begin,end,std::move(blk));}
 ~token(){clear();}

  operator bool() const noexcept{return !is_null();}

  token&  operator=(const token&   rhs) noexcept{return assign(rhs);}
  token&  operator=(      token&&  rhs) noexcept{return assign(std::move(rhs));}

  token&  assign(const token&   rhs) noexcept;
  token&  assign(      token&&  rhs) noexcept;
  token&  assign(const char*  begin, const char*  end, uint64_t  n) noexcept;
  token&  assign(const char*  begin, const char*  end, double  f) noexcept;
  token&  assign(const char*  begin, const char*  end, std::string&&  s, int  sym) noexcept;
  token&  assign(const char*  begin, const char*  end, operator_code  opco) noexcept;
  token&  assign(const char*  begin, const char*  end, token_block&&  blk) noexcept;

  void  clear() noexcept;

  const char*  get_begin() const noexcept{return m_begin;}
  const char*  get_end()   const noexcept{return m_end;}

  bool  is_null()                             const noexcept{return m_kind == kind::null;}
  bool  is_integer()                          const noexcept{return m_kind == kind::integer;}
  bool  is_identifier()                       const noexcept{return m_kind == kind::identifier;}
  bool  is_keyword(std::string_view  sv)      const noexcept{return is_identifier() && (m_data.s == sv);}
  bool  is_single_quoted()                    const noexcept{return m_kind == kind::single_quoted;}
  bool  is_double_quoted()                    const noexcept{return m_kind == kind::double_quoted;}
  bool  is_string()                           const noexcept{return is_single_quoted() || is_double_quoted();}
  bool  is_operator_code()                    const noexcept{return m_kind == kind::operator_code;}
  bool  is_operator_code(operator_code  opco) const noexcept{return is_operator_code() && (m_data.opco == opco);}
  bool  is_floating_point_number()            const noexcept{return m_kind == kind::floating_point_number;}
  bool  is_block()                            const noexcept{return m_kind == kind::block;}
  bool  is_block(operator_code  open, operator_code  close) const noexcept;

  uint64_t            get_integer()               const noexcept{return m_data.n;}
  double              get_floating_point_number() const noexcept{return m_data.f;}
  const std::string&  get_string()                const noexcept{return m_data.s;}
  operator_code       get_operator_code()         const noexcept{return m_data.opco;}
  const token_block&  get_block()                 const noexcept{return m_data.blk;}

  void  print(const char*  base=nullptr, int  indent=0) const noexcept;
  void  print_source(const char*  base=nullptr) const noexcept;

};


class
tokenizer
{
  struct element{
    const char*  m_begin;
    token_block  m_block;

  element(const char*  begin, operator_code  open, operator_code  close) noexcept: m_begin(begin), m_block(open,close){}
  };

  std::vector<element>  m_elements;

  const char*  m_begin  =nullptr;
  const char*  m_current=nullptr;
  const char*  m_end    =nullptr;

  void       read_binary_number() noexcept;
  void        read_octal_number() noexcept;
  void      read_decimal_number() noexcept;
  void  read_hexadecimal_number() noexcept;
  void  read_floating_point_number(uint64_t  i) noexcept;

  void  read_number_that_begins_by_zero() noexcept;
  void  read_number() noexcept;

  void  read_operator_code() noexcept;

  void  read_quoted_string(char  close_char);

  void  read_block(operator_code  open, operator_code  close);

  static bool  test_head_of_identifier_defaultly(char  c) noexcept;
  static bool  test_body_of_identifier_defaultly(char  c) noexcept;

  bool  (*m_test_head_of_identifier)(char  c)=test_head_of_identifier_defaultly;
  bool  (*m_test_body_of_identifier)(char  c)=test_body_of_identifier_defaultly;

  void  skip_spaces();

  void  skip_linestyle_comment() noexcept;
  void  skip_blockstyle_comment();

  void  push(token&&  tok) noexcept{m_elements.back().m_block += std::move(tok);}


  void  step(operator_code  close, int close_len);

public:
  token_block  operator()(std::string_view  sv);

};




class
token_iterator
{
  const token*  m_begin=nullptr;
  const token*  m_end  =nullptr;

  static const token  m_null;

  token_iterator(const token*  begin, const token*  end) noexcept:
  m_begin(begin), m_end(end){}

public:
  token_iterator() noexcept{}
  token_iterator(const token_block&  blk) noexcept:
  m_begin(blk->data()), m_end(blk->data()+blk->size()){}

  operator bool() const noexcept{return m_begin < m_end;}

  const token&  operator[](int  i) const noexcept{return ((m_begin+i) < m_end)? m_begin[i]:m_null;}

  const token&  operator*()  const noexcept{return (m_begin < m_end)? *m_begin: m_null;}
  const token*  operator->() const noexcept{return (m_begin < m_end)?  m_begin:&m_null;}

  token_iterator&  operator++(   ) noexcept{  ++m_begin;  return *this;}
  token_iterator   operator++(int) noexcept{  auto  ret = *this;  ++m_begin;  return ret;}

  token_iterator&  operator--(   ) noexcept{  --m_begin;  return *this;}
  token_iterator   operator--(int) noexcept{  auto  ret = *this;  --m_begin;  return ret;}

  token_iterator&  operator+=(int  n) noexcept{  m_begin += n;  return *this;}
  token_iterator&  operator-=(int  n) noexcept{  m_begin -= n;  return *this;}

  token_iterator  operator+(int  n) const noexcept{return token_iterator(m_begin+n,m_end);}
  token_iterator  operator-(int  n) const noexcept{return token_iterator(m_begin-n,m_end);}

  int  size() const noexcept{return m_end-m_begin;}

};


class
exprelem
{
  char  m_kind=0;

  union data{
   int64_t  i;
  uint64_t  u;
    double  f;

  std::string_view  sv;

    operator_code  o;

  constexpr data(int64_t   i_) noexcept: i(i_){}
  constexpr data(uint64_t  u_) noexcept: u(u_){}
  constexpr data(double    f_) noexcept: f(f_){}
  constexpr data(std::string_view  sv_) noexcept: sv(sv_){}
  constexpr data(operator_code  o_) noexcept: o(o_){}

  } m_data;

public:
  constexpr exprelem(int64_t        i=0) noexcept: m_kind('i'), m_data(i){}
  constexpr exprelem(uint64_t       u  ) noexcept: m_kind('u'), m_data(u){}
  constexpr exprelem(double         f  ) noexcept: m_kind('f'), m_data(f){}
  constexpr exprelem(char  k, std::string_view  sv) noexcept: m_kind(k), m_data(sv){}
  constexpr exprelem(char  k, operator_code  o) noexcept: m_kind(k), m_data(o){}

  constexpr bool  is_integer()                const noexcept{return m_kind == 'i';}
  constexpr bool  is_unsigned_integer()       const noexcept{return m_kind == 'u';}
  constexpr bool  is_real_number()            const noexcept{return m_kind == 'f';}
  constexpr bool  is_string()                 const noexcept{return m_kind == 's';}
  constexpr bool  is_identifier()             const noexcept{return m_kind == 'I';}
  constexpr bool  is_operand()                const noexcept{return is_integer() || is_unsigned_integer() || is_real_number() || is_string() || is_identifier();}
  constexpr bool  is_prefix_unary_operator()  const noexcept{return m_kind == 'p';}
  constexpr bool  is_postfix_unary_operator() const noexcept{return m_kind == 'P';}
  constexpr bool  is_binary_operator()        const noexcept{return m_kind == 'B';}
  constexpr bool  is_operator()               const noexcept{return m_kind == 'o';}

  constexpr  int64_t             get_integer() const noexcept{return m_data.i;}
  constexpr uint64_t    get_unsigned_integer() const noexcept{return m_data.u;}
  constexpr   double         get_real_number() const noexcept{return m_data.f;}
  constexpr std::string_view      get_string() const noexcept{return m_data.sv;}
  constexpr operator_code  get_operator_code() const noexcept{return m_data.o;}

  constexpr operator  int64_t() const noexcept
  {
    return is_integer()?          static_cast<int64_t>(m_data.i)
          :is_unsigned_integer()? static_cast<int64_t>(m_data.u)
          :                       static_cast<int64_t>(m_data.f)
          ;
  }

  constexpr operator uint64_t() const noexcept
  {
    return is_integer()?          static_cast<uint64_t>(m_data.i)
          :is_unsigned_integer()? static_cast<uint64_t>(m_data.u)
          :                       static_cast<uint64_t>(m_data.f)
          ;
  }
  constexpr operator double() const noexcept
  {
    return is_integer()?          static_cast<double>(m_data.i)
          :is_unsigned_integer()? static_cast<double>(m_data.u)
          :                       static_cast<double>(m_data.f)
          ;
  }

  void  print() const noexcept
  {
         if(is_real_number()           ){printf("%f",m_data.f);}
    else if(is_unsigned_integer()      ){printf("%" PRIu64,m_data.u);}
    else if(is_integer()               ){printf("%" PRIi64,m_data.i);}
    else if(is_string()                ){printf("\"%s\"",m_data.sv.data());}
    else if(is_identifier()            ){printf("%s",m_data.sv.data());}
    else if(is_prefix_unary_operator() ){printf("p%s",m_data.o.get_string());}
    else if(is_postfix_unary_operator()){printf("P%s",m_data.o.get_string());}
    else if(is_binary_operator()       ){printf("B%s",m_data.o.get_string());}
    else if(is_operator()              ){printf("%s",m_data.o.get_string());}
    else                                {printf("<NULL>");}
  }

};


class
exprrpn
{
  std::vector<std::string>  m_strings;

  std::vector<exprelem>  m_stack;

  void  finish(std::vector<exprelem>&&  opstack, std::vector<exprelem>&  dst) noexcept;

  void   preprocess(token_iterator&  top_it) noexcept;
  void  postprocess() noexcept;

public:
  exprrpn() noexcept{}
  exprrpn(const exprrpn&) noexcept=delete;
  exprrpn(std::string_view  sv) noexcept{assign(sv);}
  exprrpn(token_iterator&  it) noexcept{assign(it);}

  exprrpn&  operator=(const exprrpn&) noexcept=delete;
  exprrpn&  operator=(std::string_view  sv) noexcept{return assign(sv);}
  exprrpn&  operator=(token_iterator&  it) noexcept{return assign(it);}

  exprrpn&  assign(std::string_view  sv) noexcept;
  exprrpn&  assign(token_iterator&  it) noexcept;

  const std::vector<exprelem>&  get_stack() const noexcept{return m_stack;}

  void  print() const noexcept;

  static exprelem  evaluate(const exprrpn&  rpn) noexcept;

};


}




#endif




