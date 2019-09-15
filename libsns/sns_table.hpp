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


template<typename  T> class  table;
template<typename  T> class  index_list;


template<typename  T>
class
table
{
  timestamp  m_last_timestamp;

  using pointer_type = T*;

  std::vector<pointer_type>  m_container;

  range  search_internal(const pointer_type*  base_pointer, uint64_t  bottom, uint64_t  top, timestamp  ts) const noexcept
  {
    auto  length = 1+(top-bottom);

      if(length < 4)
      {
        return range(bottom,top);
      }


    uint64_t  center = bottom+(length/2);

    auto  sample_ts = base_pointer[center]->get_timestamp();

    return (sample_ts < ts)? search_internal(base_pointer,bottom+1,top  ,ts)
          :(sample_ts > ts)? search_internal(base_pointer,bottom  ,top-1,ts)
          :                  range(bottom,top);
  }


  bool  fix_bottom(uint64_t&  bottom, uint64_t  top, timestamp  a, timestamp  b) const noexcept
  {
      for(;;)
      {
        auto&  t = *m_container[bottom];

          if((t >= a) &&
             (t <= b))
          {
            return true;
          }


          if(++bottom > top)
          {
            return false;
          }
      }
  }


  bool  fix_top(uint64_t  bottom, uint64_t&  top, timestamp  a, timestamp  b) const noexcept
  {
      for(;;)
      {
        auto&  t = *m_container[top];

          if((t >= a) &&
             (t <= b))
          {
            return true;
          }


          if(--top < bottom)
          {
            return false;
          }
      }
  }

public:
 ~table(){clear();}

  void  clear() noexcept
  {
      for(auto  ptr: m_container)
      {
        delete ptr;
      }


    m_container.clear();
  }

  int  get_number_of_records() const noexcept{return m_container.size();}

  operator bool() const noexcept{return m_container.size();}

  const T&  operator[](uint64_t  i) const noexcept{return *m_container[i];}

  const T&  get_first() const noexcept{return *m_container.front();}
  const T&  get_last()  const noexcept{return *m_container.back();}

  void  append(timestamp  ts, std::string_view  sv) noexcept
  {
      if(ts <= m_last_timestamp)
      {
        printf("table: timestamp error\n");

        return;
      }


    auto  n = m_container.size();

    sha256_hash  hash = n? m_container[n-1]->generate_hash():sha256_hash();

    m_container.emplace_back(new T(n,ts,sv,hash));

    m_last_timestamp = ts;
  }


  range  search(timestamp  ts) const noexcept
  {
    int  n = m_container.size();

    return (n == 0)? ranges::wrong
          :(n == 1)? range(0,0)
          :(n == 2)? range(0,1)
          :search_internal(m_container.data(),0,n-1,ts);
  }


  index_list<T>  make_index_list() const noexcept
  {
    return index_list<T>(*this);
  }

  range  make_range(timestamp  a, timestamp  b) const noexcept
  {
      if(a > b)
      {
        std::swap(a,b);
      }


      if(m_container.size())
      {
        auto  a_result = search(a);
        auto  b_result = search(b);

        auto&  bottom = a_result.bottom;
        auto&     top = b_result.top;

          if(fix_bottom(bottom,top,a,b) && 
             fix_top(   bottom,top,a,b))
          {
            return range(bottom,top);
          }
      }


    return ranges::wrong;
  }

  index_list<T>  make_index_list(timestamp  a, timestamp  b) const noexcept
  {
    return index_list<T>(*this,make_range(a,b));
  }

  class iterator{
    const pointer_type*  m_pp;
  public:
    constexpr iterator(const pointer_type*   pp) noexcept: m_pp(pp){}

    constexpr const T&  operator*() const noexcept{return **m_pp;}
    constexpr bool  operator!=(iterator  rhs) const noexcept{return m_pp != rhs.m_pp;}

    iterator&  operator++() noexcept{  ++m_pp;  return *this;}

  };

  iterator  begin() const noexcept{return iterator(m_container.data()                   );}
  iterator    end() const noexcept{return iterator(m_container.data()+m_container.size());}

};


template<typename  T>
class
index_list
{
  const table<T>&  m_table;

  std::vector<uint64_t>  m_container;

  void  copy_indexes() noexcept
  {
    m_container.clear();

    auto  n = m_table.get_number_of_records();

      for(auto  i = 0;   i < n;  ++i)
      {
        m_container.emplace_back(i);
      }
  }

  void  copy_indexes(range   r) noexcept
  {
    m_container.clear();

      if(r)
      {
          for(auto  i = r.bottom;   i <= r.top;  ++i)
          {
            m_container.emplace_back(i);
          }
      }
  }

public:
  index_list(const table<T>&  tbl          ) noexcept: m_table(tbl){copy_indexes( );}
  index_list(const table<T>&  tbl, range  r) noexcept: m_table(tbl){copy_indexes(r);}

  const uint64_t&  operator[](int  i) const noexcept{return m_container[i];}

  int  get_number_of_indexes() const noexcept{return m_container.size();}

  const uint64_t*  begin() const noexcept{return m_container.data();}
  const uint64_t*    end() const noexcept{return m_container.data()+m_container.size();}

  template<typename  Tester>
  index_list&  filter(Tester  t) noexcept
  {
    std::vector<uint64_t>  tmp;

      for(auto  i: m_container)
      {
          if(t(m_table[i]->get_content().data()))
          {
            tmp.emplace_back(i);
          }
      }


    m_container = std::move(tmp);

    return *this;
  }

};


class
tester
{
  const char*  m_condition;

public:
  constexpr tester(const char*  cond) noexcept: m_condition(cond){}

  bool  operator()(const char*  content) const noexcept;

};


}




#endif




