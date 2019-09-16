#include"libsns/sns_record.hpp"




namespace sns{




range
record_table::
search_internal(const record* const*  base_pointer, uint64_t  bottom, uint64_t  top, timestamp  ts) const noexcept
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


bool
record_table::
fix_bottom(uint64_t&  bottom, uint64_t  top, timestamp  a, timestamp  b) const noexcept
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


bool
record_table::
fix_top(uint64_t  bottom, uint64_t&  top, timestamp  a, timestamp  b) const noexcept
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


void
record_table::
clear() noexcept
{
    for(auto  ptr: m_container)
    {
      delete ptr;
    }


  m_container.clear();
}


void
record_table::
append(timestamp  ts, std::string_view  sv) noexcept
{
  auto  n = m_container.size();

    if(n && (ts <= m_last_timestamp))
    {
      printf("table: timestamp error\n");

      return;
    }


  sha256_hash  hash = n? m_container[n-1]->generate_hash():sha256_hash();

  m_container.emplace_back(new record(n,ts,sv,hash));

  m_last_timestamp = ts;
}


range
record_table::
search(timestamp  ts) const noexcept
{
  int  n = m_container.size();

  return (n == 0)? ranges::wrong
        :(n == 1)? range(0,0)
        :(n == 2)? range(0,1)
        :search_internal(m_container.data(),0,n-1,ts);
}


range
record_table::
make_range(timestamp  a, timestamp  b) const noexcept
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




}




