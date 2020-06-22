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
parse_error
{
public:
  int  m_line_number;

  std::string  m_comment;

  parse_error(int  ln, std::string_view  sv) noexcept: m_line_number(ln), m_comment(sv){}

};




class
operator_code
{
  char  m_data[4];

  static constexpr char  chr(std::string_view  sv, int  i) noexcept
  {
    return (i < sv.size())? sv[i]:0;
  }

public:
  constexpr operator_code() noexcept: m_data{0,0,0,0}{}
  constexpr operator_code(const char*  s) noexcept: m_data{chr(s,0),chr(s,1),chr(s,2),0}{}
  constexpr operator_code(std::string_view  sv) noexcept: m_data{chr(sv,0),chr(sv,1),chr(sv,2),0}{}

  operator_code&  operator=(std::string_view  sv) noexcept{return assign(sv);}

  operator_code&  assign(std::string_view  sv) noexcept{
    m_data[0] = chr(sv,0);
    m_data[1] = chr(sv,1);
    m_data[2] = chr(sv,2);

    return *this;
  }

  constexpr  operator bool() const noexcept{return m_data[0];}

  constexpr const char*  get_string() const noexcept{return m_data;}

  void  clear() noexcept{reinterpret_cast<int32_t&>(m_data[0]) = 0;}

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

  void  print() const noexcept
  {
    printf("%s",m_data);
  }

};


class token;
class tokenizer;


class
token
{
  const char*  m_begin=nullptr;
  const char*  m_end  =nullptr;

  int  m_line_number=0;

  enum class kind{
    null,
    identifier,
    single_quoted,
    double_quoted,
    integer,
    floating_point_number,
    operator_code,

  } m_kind=kind::null;

  union data{
    uint64_t     n;
    std::string  s;
    double       f;

    operator_code  opco;

    data() noexcept{}
   ~data()         {}
  } m_data;


  static void  print_string(const std::string&  s, char  punct) noexcept;

public:
  token() noexcept{}
  token(const token&   rhs) noexcept{assign(rhs);}
  token(      token&&  rhs) noexcept{assign(std::move(rhs));}
 ~token(){clear();}

  template<class...  Args>
  explicit token(Args&&...  args) noexcept{assign(std::forward<Args>(args)...);}

  operator bool() const noexcept{return !is_null();}

  template<class...  Args>
  token&  operator=(Args&&...  args) noexcept{return assign(std::forward<Args>(args)...);}

  token&  assign(const token&   rhs) noexcept;
  token&  assign(      token&&  rhs) noexcept;
  token&  assign(const char*  begin, const char*  end, int  ln, uint64_t  n) noexcept;
  token&  assign(const char*  begin, const char*  end, int  ln, double  f) noexcept;
  token&  assign(const char*  begin, const char*  end, int  ln, std::string&&  s, int  sym) noexcept;
  token&  assign(const char*  begin, const char*  end, int  ln, operator_code  opco) noexcept;

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

  int  get_line_number() const noexcept{return m_line_number;}

  uint64_t            get_integer()               const noexcept{return m_data.n;}
  double              get_floating_point_number() const noexcept{return m_data.f;}
  const std::string&  get_string()                const noexcept{return m_data.s;}
  operator_code       get_operator_code()         const noexcept{return m_data.opco;}

  void  print(int  indent=0) const noexcept;

  static const token  null;

};


class
tokenizer
{
  const char*  m_source_begin=nullptr;

  int  m_line_number=0;

  const char*  m_begin  =nullptr;
  const char*  m_current=nullptr;
  const char*  m_end    =nullptr;

  std::vector<token>  m_buffer;

  void       read_binary_number() noexcept;
  void        read_octal_number() noexcept;
  void      read_decimal_number() noexcept;
  void  read_hexadecimal_number() noexcept;
  void  read_floating_point_number(uint64_t  i) noexcept;

  void  read_number_that_begins_by_zero() noexcept;
  void  read_number() noexcept;

  void  read_operator_code();

  void  read_quoted_string(char  close_char);

  static bool  test_head_of_identifier_defaultly(char  c) noexcept;
  static bool  test_body_of_identifier_defaultly(char  c) noexcept;

  bool  (*m_test_head_of_identifier)(char  c)=test_head_of_identifier_defaultly;
  bool  (*m_test_body_of_identifier)(char  c)=test_body_of_identifier_defaultly;

  void  skip_spaces();

  void  skip_linestyle_comment() noexcept;
  void  skip_blockstyle_comment();

  void  push(token&&  tok) noexcept{m_buffer.emplace_back(std::move(tok));}

  void  step();

  int  get_line_number() const noexcept;

public:
  std::vector<token>  operator()(std::string_view  sv);

};


std::vector<token>  make_token_string(std::string_view  sv);




class
token_iterator
{
  const token*  m_begin=nullptr;
  const token*  m_end  =nullptr;

  token_iterator(const token*  begin, const token*  end) noexcept:
  m_begin(begin), m_end(end){}

public:
  token_iterator() noexcept{}
  token_iterator(const std::vector<token>&  ts) noexcept:
  m_begin(ts.data()), m_end(ts.data()+ts.size()){}

  operator bool() const noexcept{return m_begin < m_end;}

  const token&  operator[](int  i) const noexcept{return ((m_begin+i) < m_end)? m_begin[i]:token::null;}

  const token&  operator*()  const noexcept{return (m_begin < m_end)? *m_begin: token::null;}
  const token*  operator->() const noexcept{return (m_begin < m_end)?  m_begin:&token::null;}

  bool  operator==(const token_iterator&  rhs) const noexcept{return m_begin == rhs.m_begin;}
  bool  operator!=(const token_iterator&  rhs) const noexcept{return m_begin != rhs.m_begin;}
  bool  operator< (const token_iterator&  rhs) const noexcept{return m_begin <  rhs.m_begin;}
  bool  operator<=(const token_iterator&  rhs) const noexcept{return m_begin <= rhs.m_begin;}
  bool  operator> (const token_iterator&  rhs) const noexcept{return m_begin >  rhs.m_begin;}
  bool  operator>=(const token_iterator&  rhs) const noexcept{return m_begin >= rhs.m_begin;}

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




}




#endif




