#ifndef SNS_HPP_WAS_INCLUDED
#define SNS_HPP_WAS_INCLUDED


#include"libsns/sns_table.hpp"
#include"libsns/sns_account.hpp"
#include<list>
#include<memory>




namespace sns{


class
world
{
};


class
platform
{
  std::vector<std::unique_ptr<account>>  m_account_table;

public:
  const account_observer&  create_account(timestamp  ts) noexcept;

  const account_observer&  get_account_by_index(uint64_t  i) const noexcept{return m_account_table[i]->get_observer();}

  void  print() const noexcept;

};


class
timeline
{
  struct target{
    account_observer  m_observer;

    uint64_t  m_bottom_index;

    target(const account_observer&  obs) noexcept;

  };


  std::vector<target>  m_target_list;

  std::list<const article*>  m_article_table;

  timestamp  m_front_timestamp;
  timestamp  m_back_timestamp;

public:
  void  push(const account_observer&  obs) noexcept{m_target_list.emplace_back(obs);}

  void  reset(timestamp   base) noexcept;

  void  fetch_front_articles(timestamp   ts) noexcept;

  void  fetch_back_articles(              ) noexcept;
  void  fetch_back_articles(timestamp   ts) noexcept;

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




