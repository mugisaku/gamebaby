#ifndef SNS_HPP_WAS_INCLUDED
#define SNS_HPP_WAS_INCLUDED


#include"libsns/sns_table.hpp"
#include<list>




namespace sns{


class
world
{
};


class
article
{
  uint64_t  m_original_index;
  uint64_t  m_index;

  timestamp  m_timestamp;

  std::string  m_content;

public:

};


class
sex
{
  int  m_value;

public:
  constexpr sex(int  v=0) noexcept: m_value(v){}

  constexpr bool  is_unspecified() const noexcept{return m_value == 0;}
  constexpr bool  is_male()        const noexcept{return m_value == 1;}
  constexpr bool  is_female()      const noexcept{return m_value == 2;}

};


namespace sexes{
constexpr sex  unspecified = sex(0);
constexpr sex         male = sex(1);
constexpr sex       female = sex(2);
}


class
article_reference
{
  uint64_t  m_accouont_index;
  uint64_t     m_entry_index;

public:

};


class platform;
class  account;


class
account_observer
{
  struct private_data;

  private_data*  m_data=nullptr;

  void  unrefer() noexcept;

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

  account_observer&  assign(const account_observer&   rhs) noexcept{assign(rhs);}
  account_observer&  assign(      account_observer&&  rhs) noexcept{assign(std::move(rhs));}

};


class
account
{
  account_observer  m_observer;

  uint64_t  m_original_id;
  uint64_t  m_platform_id;

  uint64_t  m_index;

  std::string  m_name;

  uint64_t  m_birth_date;

  sex  m_sex;

  timestamp  m_create_timestamp;
  timestamp  m_modify_timestamp;

  table<article>  m_article_table;

  std::list<account_observer>    m_follow_list;
  std::list<account_observer>  m_follower_list;
  std::list<account_observer>     m_block_list;
  std::list<account_observer>      m_mute_list;

public:
  account() noexcept: m_observer(*this){}
 ~account(){clear();}

  void  clear() noexcept;

  const account_observer&  get_observer() const noexcept{return m_observer;}

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

};


class
platform
{
  table<account>  m_account_table;

public:
  const account_observer&  get_account_by_index(uint64_t  i) const noexcept{return m_account_table[i]->get_observer();}

};


class
personal_log
{
  uint64_t  m_id;

  table<record>  m_record_table;

public:

};


}




#endif




