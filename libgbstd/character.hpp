#ifndef gbstd_text_HPP
#define gbstd_text_HPP


#include"libgbstd/control.hpp"
#include<string>
#include<cstring>




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
utf8_decoder
{
  const char*  m_pointer=nullptr;

public:
  constexpr utf8_decoder(              ) noexcept{}
  constexpr utf8_decoder(const char*  s) noexcept: m_pointer(s){}

  utf8_decoder&  operator=(const char*  s) noexcept{m_pointer = s;  return *this;}

  operator bool() const noexcept{return *m_pointer;}

  char32_t  operator()() noexcept;

  const char*  get_pointer() const noexcept{return m_pointer;}

};


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


class text;


class
text_line
{
  friend class text;

  static constexpr int  m_buffer_length = 80;

  char16_t  m_buffer[m_buffer_length];

  int  m_content_length=0;
  int  m_display_length=0;

public:
  const char16_t&  operator[](int  i) const noexcept{return m_buffer[i];}

  bool  is_full() const noexcept{return m_content_length >= m_buffer_length;}

  bool  is_displaying_all() const noexcept{return m_display_length == m_content_length;}

  void  push(char16_t  c) noexcept{m_buffer[m_content_length++] = c;}

  int  get_length() const noexcept{return m_content_length;}
  int  get_display_length() const noexcept{return m_display_length;}

  std::u16string_view  get_view() const noexcept{return std::u16string_view(m_buffer,m_display_length);}

  const char16_t*  begin() const noexcept{return m_buffer                 ;}
  const char16_t*    end() const noexcept{return m_buffer+m_display_length;}

};


class
text
{
  struct node{
    text_line  m_line;

    node*  m_next;
  };

  static node*  m_stock_pointer;

  node*     m_top_pointer=nullptr;
  node*  m_bottom_pointer=nullptr;

  node*  m_current_pointer=nullptr;

  uint32_t  m_character_counter=0;

  int  m_number_of_lines=0;

  node*  new_node() noexcept;

public:
  text() noexcept{}
  text(const text&   rhs) noexcept=delete;
  text(      text&&  rhs) noexcept{assign(std::move(rhs));}
 ~text(){clear();}

  text&  operator=(const text&   rhs) noexcept=delete;
  text&  operator=(      text&&  rhs) noexcept{return assign(std::move(rhs));}

//  text&  assign(const text&   rhs) noexcept;
  text&  assign(      text&&  rhs) noexcept;

  text&  clear() noexcept;

  int  get_number_of_lines() const noexcept{return m_number_of_lines;}

  text&  push(std::string_view     sv) noexcept;
  text&  push(std::u16string_view  sv) noexcept;

  text&   pop() noexcept;

  bool  expose_one_character() noexcept;
  bool  expose_all_characters_of_current_line() noexcept;

  bool  is_displaying_all_characters() const noexcept{return !m_character_counter;}

  void  print() const noexcept;


  class iterator{
    const node*  m_pointer=nullptr;

  public:
    iterator(const node*  ptr) noexcept: m_pointer(ptr){}

    const text_line&  operator *() const noexcept{return  m_pointer->m_line;}
    const text_line*  operator->() const noexcept{return &m_pointer->m_line;}

    operator bool() const noexcept{return m_pointer;}

    bool  operator==(iterator  rhs) noexcept{return m_pointer == rhs.m_pointer;}
    bool  operator!=(iterator  rhs) noexcept{return m_pointer != rhs.m_pointer;}

    iterator&  operator++() noexcept{  m_pointer = m_pointer->m_next;  return *this;}
    iterator  operator++(int) noexcept;

    iterator  operator+(int  n) const noexcept;
    iterator&  operator+=(int  n) noexcept;

  };


  iterator  bottom() const noexcept{return iterator(m_bottom_pointer);}

  iterator  begin() const noexcept{return iterator(m_top_pointer);}
  iterator    end() const noexcept{return iterator(nullptr);}

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


}




#endif




