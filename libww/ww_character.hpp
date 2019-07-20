#ifndef ww_character_hpp_is_included
#define ww_character_hpp_is_included


#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"
#include"libgbstd/process.hpp"
#include"libww/ww_string_display.hpp"
#include"libww/ww_bar.hpp"




namespace ww{
namespace battles{



class
side
{
  int  m_data;

public:
  static constexpr int   m_left_value = 0;
  static constexpr int  m_right_value = 1;

  constexpr side(int  v=0) noexcept: m_data(v){}

  constexpr bool  operator==(side  rhs) const noexcept{return m_data == rhs.m_data;}
  constexpr bool  operator!=(side  rhs) const noexcept{return m_data != rhs.m_data;}

  constexpr bool  is_left()  const noexcept{return m_data == m_left_value;}
  constexpr bool  is_right() const noexcept{return m_data == m_right_value;}

  constexpr side  get_opposite() const noexcept{return side(is_left()? m_right_value:m_left_value);}

};


namespace sides{
constexpr side   left = side(side::m_left_value);
constexpr side  right = side(side::m_right_value);
}




class
position
{
  int  m_value;

public:
  constexpr position(int  v=0) noexcept: m_value(v){}

  constexpr operator int() const noexcept{return m_value;}

  constexpr bool  operator==(position  rhs) const noexcept{return m_value == rhs.m_value;}
  constexpr bool  operator!=(position  rhs) const noexcept{return m_value != rhs.m_value;}

  constexpr bool  is_front()  const noexcept{return m_value == 0;}
  constexpr bool  is_back()   const noexcept{return m_value == 1;}
  constexpr bool  is_backup() const noexcept{return m_value == 2;}

};


namespace positions{
constexpr position   front(0);
constexpr position    back(1);
constexpr position  backup(2);
}


struct
character_head
{
  std::u16string  m_name;

};


struct
character_body
{
  int  m_hp=0;
  int  m_hp_max=0;

  position  m_position=positions::front;

};




struct
character
{
  static constexpr int  m_ap_max = 1000;

  struct flags{
    static constexpr int  active = 1;
  };


  gbstd::status_value<int>  m_status;

  battles::character_head  m_head;

  battles::character_body  m_original_body;
  battles::character_body  m_product_body;
  battles::character_body  m_proof_body;

  display_string  m_name_string;
  display_string  m_hp_string;

  string_display  m_name_display;
  string_display  m_hp_display;

  int  m_ap=0;


  side  m_side;

  bar  m_hp_bar;
  bar  m_ap_bar;

  int  m_last_hp=0;
  int  m_last_hp_max=0;

  int  m_last_ap=0;

  static void  draw(const gbstd::canvas&  cv, character&  c) noexcept;
  static void  tick(                          character&  c) noexcept;

  character() noexcept{}

  operator bool() const noexcept{return m_status.test(flags::active);}

  bool  can_continue_battle() const noexcept{return *this && (m_product_body.m_hp > 0);}

  void  copy_body() noexcept
  {
    m_product_body = m_original_body;
    m_proof_body   = m_original_body;
  }

};




}}




#endif




