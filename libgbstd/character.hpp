#ifndef gbstd_character_HPP
#define gbstd_character_HPP


#include<string>
#include<cstring>
#include<vector>
#include<list>
#include<memory>
#include"libgbstd/utility.hpp"




namespace gbstd{




constexpr int  g_font_width  =  8;
constexpr int  g_font_height = 16;

struct
glyph
{
  char16_t  unicode;

  uint8_t  data[g_font_height];

};


const glyph&  get_glyph(char16_t  c) noexcept;




class
string_form
{
  char  m_buffer[512] = {0};

  int  m_length=0;

public:
  string_form() noexcept{}
  string_form(const char*  fmt, ...) noexcept;

  std::string_view  operator()(const char*  fmt, ...) noexcept;
  std::string_view  operator()() const noexcept{return std::string_view(m_buffer,m_length);}

  int  size() const noexcept{return m_length;}

};




class
utf8_decoder
{
  const char*  m_pointer=nullptr;
  const char*  m_end    =nullptr;

public:
  utf8_decoder(                    ) noexcept{}
  utf8_decoder(std::string_view  sv) noexcept{assign(sv);}

  utf8_decoder&  operator=(std::string_view  sv) noexcept{return assign(sv);}

  utf8_decoder&  assign(std::string_view  sv) noexcept;

  operator bool() const noexcept{return m_pointer < m_end;}

  char32_t  operator()() noexcept;

  const char*  get_pointer() const noexcept{return m_pointer;}

};


size_t  u8clen(unsigned char  c) noexcept;
size_t  u8slen(const char*  s) noexcept;


struct
utf8_encoder
{
  char  codes[8];

  utf8_encoder(char32_t  c=0) noexcept{(*this)(c);}

  utf8_encoder&  operator()(char32_t  c) noexcept;

};


std::string     make_string(std::u16string_view  sv) noexcept;
std::u16string  make_u16string(std::string_view  sv) noexcept;


void  print(std::u16string_view  sv) noexcept;
void  print_nl() noexcept;


class
code_text
{
  std::u16string  m_string;

  std::vector<const char16_t*>  m_line_heads;

public:
  code_text(std::string_view  sv="") noexcept{assign(sv);}

  code_text&  operator+=(std::string_view  sv) noexcept{return append(sv);}
  code_text&  operator =(std::string_view  sv) noexcept{return assign(sv);}
  code_text&  operator+=(std::u16string_view  sv) noexcept{return append(sv);}
  code_text&  operator =(std::u16string_view  sv) noexcept{return assign(sv);}

  code_text&  assign(std::string_view  sv) noexcept;
  code_text&  assign(std::u16string_view  sv) noexcept;

  code_text&  append(std::string_view  sv) noexcept;
  code_text&  append(std::u16string_view  sv) noexcept;

  class iterator{
    const char16_t*  m_pointer=nullptr;

    int  m_x_index=0;
    int  m_y_index=0;

  public:
    iterator(const char16_t*  p=nullptr) noexcept{assign(p);}

    iterator&  operator=(const char16_t*  p) noexcept{return assign(p);}

    iterator&  assign(const char16_t*  p=nullptr) noexcept;

    operator bool() const noexcept{return m_pointer;}

    const char16_t&  operator[](int  i) const noexcept{return m_pointer[i];}

    const char16_t&  operator*() const noexcept{return *m_pointer;}

    bool  operator==(const iterator&  rhs) const noexcept{return m_pointer == rhs.m_pointer;}
    bool  operator!=(const iterator&  rhs) const noexcept{return m_pointer != rhs.m_pointer;}

    int  get_x_index() const noexcept{return m_x_index;}
    int  get_y_index() const noexcept{return m_y_index;}

    iterator   operator+ (int  n) const noexcept;
    iterator&  operator+=(int  n) noexcept;

    iterator&  operator++(   ) noexcept;
    iterator   operator++(int) noexcept;

    void  skip_spaces() noexcept;
    void  skip_to_newline() noexcept;
    void  skip_to_block_end() noexcept;
    void  print() const noexcept;

  };

  iterator  get_iterator() const noexcept{return iterator(m_string.data());}

  const std::u16string&  get_string() const noexcept{return m_string;}

  int  get_number_of_lines() const noexcept{return m_line_heads.size();}

  void  print() const noexcept;

};


class
text
{
  struct line{
    line*  m_next;

    char16_t*  m_buffer;

    int  m_length=0;

    std::u16string_view  get_view() const noexcept{return std::u16string_view(m_buffer,m_length);}

  };


  std::vector<char16_t>  m_character_buffer;
  std::vector<line>      m_lines;

  line*      m_top_line=nullptr;
  line*   m_bottom_line=nullptr;
  line*  m_current_line=nullptr;

  int  m_width =0;
  int  m_height=0;

  bool  m_full_flag=false;

  std::vector<std::u16string_view>  m_line_list;

public:
  text(int  w=0, int  h=0) noexcept{resize(w,h);}
  text(const text&   rhs) noexcept=delete;
  text(      text&&  rhs) noexcept{assign(std::move(rhs));}
 ~text(){clear();}

  text&  operator=(const text&   rhs) noexcept=delete;
  text&  operator=(      text&&  rhs) noexcept{return assign(std::move(rhs));}

  text&  assign(const text&   rhs) noexcept=delete;
  text&  assign(      text&&  rhs) noexcept;

  text&  clear() noexcept;
  text&  erase() noexcept;

  text&  resize(int  w, int  h) noexcept;

  int  get_width()  const noexcept{return m_width;}
  int  get_height() const noexcept{return m_height;}

  bool  push(char16_t  c) noexcept;

  text&  rotate() noexcept;

  bool  is_full() const noexcept{return m_full_flag;}

  const std::vector<std::u16string_view>&  make_line_list() noexcept;

  void  print() const noexcept;

};


class
string_set
{
public:
  class section{
    friend class string_set;

    std::string  m_name;

    std::vector<std::pair<std::string,std::u16string>>  m_table;

  public:
    section() noexcept{}
    section(std::string&&  name) noexcept: m_name(std::move(name)){}

    std::u16string_view  get(std::string_view  name) const noexcept;

  };

private:
  std::vector<section>  m_sections;

  section*  get_section(std::string_view  name) noexcept;

public:
  string_set() noexcept{m_sections.emplace_back("");}

  const section*  get_section(std::string_view  name) const noexcept;

  int  load_from_file(  std::string_view  path) noexcept;
  int  load_from_string(std::string_view    sv) noexcept;

  void  print() const noexcept;

};




class
small_string
{
  char  m_data[4];

  static constexpr char  chr(std::string_view  sv, int  i) noexcept
  {
    return (i < sv.size())? sv[i]:0;
  }

public:
  constexpr small_string() noexcept: m_data{0,0,0,0}{}
  constexpr small_string(const char*  s) noexcept: m_data{chr(s,0),chr(s,1),chr(s,2),0}{}
  constexpr small_string(std::string_view  sv) noexcept: m_data{chr(sv,0),chr(sv,1),chr(sv,2),0}{}

  small_string&  operator=(std::string_view  sv) noexcept{return assign(sv);}

  small_string&  assign(std::string_view  sv) noexcept{
    m_data[0] = chr(sv,0);
    m_data[1] = chr(sv,1);
    m_data[2] = chr(sv,2);

    return *this;
  }

  constexpr  operator bool() const noexcept{return m_data[0];}

  constexpr const char&  operator[](int  i) const noexcept{return m_data[i];}

  constexpr const char*  data() const noexcept{return m_data;}
  constexpr int  size() const noexcept{return !m_data[0]? 0
                                             :!m_data[1]? 1
                                             :!m_data[2]? 2
                                             :            3;}

  void  clear() noexcept{reinterpret_cast<int32_t&>(m_data[0]) = 0;}

  constexpr  operator uint32_t() const noexcept{return (m_data[0]<<24)|(m_data[1]<<16)|(m_data[2]<<8);}

  constexpr bool  operator==(small_string  rhs) const noexcept
  {
    return (m_data[0] == rhs.m_data[0]) &&
           (m_data[1] == rhs.m_data[1]) &&
           (m_data[2] == rhs.m_data[2]);
  }

  constexpr bool  operator!=(small_string  rhs) const noexcept
  {
    return !(*this == rhs);
  }

  constexpr const char*  begin() const noexcept{return m_data;}
  constexpr const char*    end() const noexcept{return m_data+size();}

  void  print() const noexcept
  {
    printf("%s",m_data);
  }

};




constexpr bool
is_alphabet(char  c) noexcept
{
  return(((c >= 'a') && (c <= 'z')) ||
         ((c >= 'A') && (c <= 'Z')));
}


constexpr bool
is_number(char  c) noexcept
{
  return((c >= '0') && (c <= '9'));
}


constexpr bool
is_alphabet_or_number(char  c) noexcept
{
  return(is_alphabet(c) || is_number(c));
}




}




#endif




