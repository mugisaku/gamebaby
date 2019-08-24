#ifndef SNS_HPP_WAS_INCLUDED
#define SNS_HPP_WAS_INCLUDED


#include"libsns/sns_table.hpp"




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
account
{
  uint64_t  m_original_id;
  uint64_t  m_platform_id;

  std::string  m_name;

  uint64_t  m_birth_date;

  sex  m_sex;

  timestamp  m_create_timestamp;
  timestamp  m_modify_timestamp;

  table<article>  m_article_table;

public:

};


class
platform
{
  table<account>  m_account_table;

public:

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




