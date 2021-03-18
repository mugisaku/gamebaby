#ifndef gbstd_character_HPP
#define gbstd_character_HPP


#include<string>
#include<cstring>
#include<vector>
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


std::u16string  make_u16string(std::string_view  sv) noexcept;


void  print(std::u16string_view  sv) noexcept;


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
text_iterator
{
public:
  struct line{
    const char*  m_begin;

    int  m_number_of_bytes=0;
    int  m_number_of_characters=0;

    line(const char*  p) noexcept: m_begin(p){}

  };

private:
  std::vector<line>  m_lines;

  size_t  m_length=0;

  const char*  m_current=nullptr;
  const char*  m_end=nullptr;

public:
  text_iterator(std::string_view  sv) noexcept{assign(sv);}

  text_iterator&  operator=(std::string_view  sv) noexcept{return assign(sv);}

  text_iterator&  assign(std::string_view  sv) noexcept;

  operator bool() const noexcept{return m_current < m_end;}

  std::string_view  operator*() const noexcept{return get();}

  std::string_view  get() const noexcept{return {m_current,m_length};}

  text_iterator&  operator++() noexcept{return advance();}

  text_iterator&  advance() noexcept;

  void  skip_spaces() noexcept;

  const std::vector<line>&  get_lines() const noexcept{return m_lines;}

  int  get_number_of_bytes()      const noexcept;
  int  get_number_of_characters() const noexcept;

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




