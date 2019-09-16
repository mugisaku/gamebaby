#ifndef SNS_ACCOUNT_HPP_WAS_INCLUDED
#define SNS_ACCOUNT_HPP_WAS_INCLUDED


#include"libsns/sns_record.hpp"
#include<list>




namespace sns{




class
sex
{
  int  m_value;

public:
  constexpr sex(int  v=0) noexcept: m_value(v){}

  constexpr bool  is_unspecified() const noexcept{return m_value == 0;}
  constexpr bool  is_male()        const noexcept{return m_value == 1;}
  constexpr bool  is_female()      const noexcept{return m_value == 2;}

  constexpr std::string_view  get_name() const noexcept
  {
    return (m_value == 0)? "unspecified"
          :(m_value == 1)? "male"
          :(m_value == 2)? "female"
          :"invalid";
  }

};


namespace sexes{
constexpr sex  unspecified = sex(0);
constexpr sex         male = sex(1);
constexpr sex       female = sex(2);
}


class platform;
class  account;


class
account_observer
{
  struct private_data;

  private_data*  m_data=nullptr;

public:
  account_observer() noexcept{}
  account_observer(account&  ref) noexcept;
  account_observer(const account_observer&   rhs) noexcept{assign(rhs);}
  account_observer(      account_observer&&  rhs) noexcept{assign(std::move(rhs));}
 ~account_observer(){unrefer();}

  operator bool() const noexcept;

  account_observer&  operator=(const account_observer&   rhs) noexcept{assign(rhs);}
  account_observer&  operator=(      account_observer&&  rhs) noexcept{assign(std::move(rhs));}

  account&  operator *() const noexcept;
  account*  operator->() const noexcept;

  bool  operator==(const account_observer&  rhs) const noexcept{return m_data == rhs.m_data;}
  bool  operator!=(const account_observer&  rhs) const noexcept{return m_data != rhs.m_data;}

  void  die() noexcept;

  void  unrefer() noexcept;

  account_observer&  assign(const account_observer&   rhs) noexcept;
  account_observer&  assign(      account_observer&&  rhs) noexcept;

};


class
account
{
  account_observer  m_observer;

  uint64_t  m_index;

  std::string  m_name;

  uint64_t  m_birth_date;

  sex  m_sex;

  timestamp  m_create_timestamp;
  timestamp  m_modify_timestamp;

  record_table  m_record_table;

  std::list<account_observer>    m_follow_list;
  std::list<account_observer>  m_follower_list;
  std::list<account_observer>     m_block_list;
  std::list<account_observer>      m_mute_list;

public:
  account(uint64_t  i, timestamp  ts) noexcept: m_observer(*this), m_index(i), m_create_timestamp(ts){}
  account(const account& ) noexcept=delete;
  account(      account&&) noexcept=delete;
 ~account(){clear();}

  account&  operator=(const account& ) noexcept=delete;
  account&  operator=(      account&&) noexcept=delete;

  void  clear() noexcept;

  uint64_t  get_index() const noexcept{return m_index;}

  const std::string&  get_name(                    ) const noexcept{return m_name                    ;}
  account&            set_name(std::string_view  sv)       noexcept{       m_name = sv;  return *this;}

  sex        get_sex(      ) const noexcept{return m_sex                   ;}
  account&   set_sex(sex  s)       noexcept{       m_sex = s;  return *this;}

  const account_observer&  get_observer() const noexcept{return m_observer;}

  const record_table&  get_record_table() const noexcept{return m_record_table;}

  account&  add_record(timestamp  ts, std::string_view  sv) noexcept;

  timestamp  get_last_post_timestamp() const noexcept;

  void  add_follow(  const account_observer&  obs) noexcept;
  void  add_follower(const account_observer&  obs) noexcept;
  void  add_block(   const account_observer&  obs) noexcept;
  void  add_mute(    const account_observer&  obs) noexcept;

  bool  test_follow(  const account_observer&  obs) const noexcept;
  bool  test_follower(const account_observer&  obs) const noexcept;
  bool   test_block(  const account_observer&  obs) const noexcept;
  bool    test_mute(  const account_observer&  obs) const noexcept;

  void  remove_follow(  const account_observer&  obs) noexcept;
  void  remove_follower(const account_observer&  obs) noexcept;
  void  remove_block(   const account_observer&  obs) noexcept;
  void  remove_mute(    const account_observer&  obs) noexcept;

  void  print() const noexcept;

};




/*
class
article::
link
{
  uint64_t  m_account_index;
  uint64_t    m_entry_index;

public:
  constexpr link(uint64_t  acc_i=0, uint64_t  ent_i=0) noexcept:
  m_account_index(acc_i),
  m_entry_index(ent_i){}

  constexpr uint64_t  get_account_index() const noexcept{return m_account_index;}
  constexpr uint64_t  get_entry_index()   const noexcept{return m_entry_index;}

};
*/




}




#endif




