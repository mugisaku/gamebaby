#ifndef SNS_HPP_WAS_INCLUDED
#define SNS_HPP_WAS_INCLUDED


#include"libsns/sns_account.hpp"
#include<list>
#include<memory>




namespace sns{




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
article
{
  account_observer  m_observer;

  timestamp  m_timestamp;

  std::u16string  m_name;
  std::u16string  m_date;

  std::string  m_content;

public:
  article() noexcept{}
  article(const account_observer&  obs, const record&  rec) noexcept{assign(obs,rec);}

  article&  assign(const account_observer&  obs, const record&  rec) noexcept;


  const account_observer&  get_observer() const noexcept{return m_observer;}

  timestamp  get_timestamp() const noexcept{return m_timestamp;}

  const std::u16string&  get_name() const noexcept{return m_name;}
  const std::u16string&  get_date() const noexcept{return m_date;}
  const std::string&  get_content() const noexcept{return m_content;}

};


class
timeline_node
{
  account_observer  m_observer;

  uint64_t     m_head_index;
  uint64_t  m_pretail_index;

public:
  timeline_node() noexcept{}
  timeline_node(const account_observer&  obs) noexcept{set_observer(obs);}

  operator bool() const noexcept{return m_observer->get_record_table();}

  timeline_node&           set_observer(const account_observer&  obs) noexcept;
  const account_observer&  get_observer() const noexcept{return m_observer;}

  timeline_node&  reset() noexcept;

  void  advance_head_index() noexcept{++m_head_index;}
  void  advance_pretail_index() noexcept{--m_pretail_index;}

  const record*  get_next_head() noexcept;
  const record*  get_next_tail() noexcept;

};


class
timeline
{
  std::vector<timeline_node>  m_nodes;

  std::vector<article>   m_main_table;
  std::vector<article>   m_temporary_table;

  timestamp  m_head_ts;
  timestamp  m_tail_ts;

public:
  void  add(const account_observer&  obs) noexcept{m_nodes.emplace_back(obs);}

  const std::vector<article>&  get_table() const noexcept{return m_main_table;}

  void  ready(timestamp  ts) noexcept;

  void  read_forward()  noexcept;
  void  read_backward() noexcept;

};




}




#endif




