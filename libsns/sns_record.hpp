#ifndef SNS_RECORD_HPP_WAS_INCLUDED
#define SNS_RECORD_HPP_WAS_INCLUDED


#include<cstdint>
#include<cstdio>
#include<cstring>
#include<cctype>
#include<vector>
#include<string>
#include<string_view>
#include"libsns/sns_sha256.hpp"
#include"libsns/sns_times.hpp"


namespace sns{




class
record
{
  uint64_t  m_index=0;

  timestamp  m_timestamp;

  std::string  m_content;

  sha256_hash  m_seed_hash;

public:
  record() noexcept{}
  record(uint64_t  i, timestamp  ts, std::string_view  sv, const sha256_hash&  seed_hash) noexcept:
  m_index(i),
  m_timestamp(ts),
  m_content(sv),
  m_seed_hash(seed_hash){}

  bool  operator< (timestamp  ts) const noexcept{return m_timestamp <  ts;}
  bool  operator<=(timestamp  ts) const noexcept{return m_timestamp <= ts;}
  bool  operator> (timestamp  ts) const noexcept{return m_timestamp >  ts;}
  bool  operator>=(timestamp  ts) const noexcept{return m_timestamp >= ts;}
  bool  operator==(timestamp  ts) const noexcept{return m_timestamp == ts;}
  bool  operator!=(timestamp  ts) const noexcept{return m_timestamp != ts;}

  uint64_t  get_index() const noexcept{return m_index;}

  timestamp  get_timestamp() const noexcept{return m_timestamp;}

  const sha256_hash&  get_seed_hash() const noexcept{return m_seed_hash;}

  sha256_hash  generate_hash() const noexcept;

  const std::string&   get_content() const noexcept{return m_content;}

  void  print() const noexcept;

};




struct
range
{
  uint64_t  bottom=0;
  uint64_t     top=0;

  constexpr range(uint64_t  b=0, uint64_t  t=0) noexcept: bottom(b), top(t){}

  constexpr operator bool() const noexcept{return bottom <= top;}

  constexpr uint64_t  get_length() const noexcept{return 1+(top-bottom);}

};


namespace ranges{
constexpr range  wrong(1,0);
}


class record_table;

class
index_list
{
  std::vector<uint64_t>  m_container;

public:
  index_list(range  r) noexcept{assign(r);}

  index_list&  operator=(range  r) noexcept{return assign(r);}
  index_list&  assign(range  r) noexcept;

  const uint64_t&  operator[](int  i) const noexcept{return m_container[i];}

  int  get_number_of_indexes() const noexcept{return m_container.size();}

  const uint64_t*  begin() const noexcept{return m_container.data();}
  const uint64_t*    end() const noexcept{return m_container.data()+m_container.size();}

  template<typename  Tester>
  index_list&
  filter(const record_table&  tbl, Tester  t) noexcept;


};




class
record_table
{
  timestamp  m_last_timestamp;

  std::vector<record*>  m_container;

  range  search_internal(const record* const*  base_pointer, uint64_t  bottom, uint64_t  top, timestamp  ts) const noexcept;

  bool  fix_bottom(uint64_t&  bottom, uint64_t   top, timestamp  a, timestamp  b) const noexcept;
  bool  fix_top(   uint64_t   bottom, uint64_t&  top, timestamp  a, timestamp  b) const noexcept;

public:
 ~record_table(){clear();}

  void  clear() noexcept;

  int  get_number_of_records() const noexcept{return m_container.size();}

  operator bool() const noexcept{return m_container.size();}

  const record&  operator[](uint64_t  i) const noexcept{return *m_container[i];}

  const record&  get_first() const noexcept{return *m_container.front();}
  const record&  get_last()  const noexcept{return *m_container.back();}

  void  append(timestamp  ts, std::string_view  sv) noexcept;

  range  search(timestamp  ts) const noexcept;

  range  make_range(timestamp  a, timestamp  b) const noexcept;

  index_list  make_index_list(timestamp  a, timestamp  b) const noexcept
  {
    return index_list(make_range(a,b));
  }

  class iterator{
    const record* const*  m_pp;
  public:
    constexpr iterator(const record* const*   pp) noexcept: m_pp(pp){}

    constexpr const record&  operator*() const noexcept{return **m_pp;}
    constexpr bool           operator!=(iterator  rhs) const noexcept{return m_pp != rhs.m_pp;}

    iterator&  operator++() noexcept{  ++m_pp;  return *this;}

  };

  iterator  begin() const noexcept{return iterator(m_container.data()                   );}
  iterator    end() const noexcept{return iterator(m_container.data()+m_container.size());}

};


class
tester
{
  const char*  m_condition;

public:
  constexpr tester(const char*  cond) noexcept: m_condition(cond){}

  bool  operator()(const char*  content) const noexcept;

};



template<typename  Tester>
index_list&
index_list::
filter(const record_table&  tbl, Tester  t) noexcept
{
  std::vector<uint64_t>  tmp;

    for(auto  i: m_container)
    {
        if(t(tbl[i].get_content().data()))
        {
          tmp.emplace_back(i);
        }
    }


  m_container = std::move(tmp);

  return *this;
}


}




#endif




