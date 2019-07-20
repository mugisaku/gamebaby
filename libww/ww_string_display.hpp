#ifndef ww_string_display_hpp_is_included
#define ww_string_display_hpp_is_included


#include"libgbstd/instruction_queue.hpp"
#include"libgbstd/process.hpp"


namespace ww{




class
display_string
{
  static constexpr int  m_buffer_length = 16;

  gbstd::color  m_color;

  char16_t  m_buffer[m_buffer_length]={0};

  int  m_length=0;

public:
  gbstd::color  get_color() const noexcept{return m_color;}

  std::u16string_view  get_string_view() const noexcept{return std::u16string_view(m_buffer,m_length);}

  display_string&  set_color(gbstd::color  color) noexcept;

  display_string&  set_string(std::u16string_view  sv) noexcept;

};




class
string_display
{
  gbstd::point  m_pos;

  const display_string*  m_pointer;

  enum class align{
    left,
    right
  } m_align=align::left;

public:
  string_display(const display_string*  ptr=nullptr) noexcept: m_pointer(ptr){}

  string_display&  set_position(gbstd::point  pos) noexcept{  m_pos = pos;  return *this;}
  string_display&  set_align_to_left()  noexcept{  m_align =  align::left;  return *this;}
  string_display&  set_align_to_right() noexcept{  m_align = align::right;  return *this;}

  static void  draw(const gbstd::canvas&  cv, string_display&  strdsp) noexcept;

};


}




#endif




