#include"libgbstd/sns.hpp"




namespace gbstd{
namespace sns{




void
account::
clear() noexcept
{
    for(auto&  o: m_follow_list)
    {
        if(o)
        {
          o->remove_follower(m_observer);
        }
    }


    for(auto&  o: m_follower_list)
    {
        if(o)
        {
          o->remove_follow(m_observer);
        }
    }


  m_follow_list.clear();
  m_follower_list.clear();
  m_block_list.clear();
  m_mute_list.clear();

  m_observer.die();

  m_record_table.clear();
}


account&
account::
add_record(timestamp  ts, std::string_view  sv) noexcept
{
  m_record_table.append(ts,sv);

  return *this;
}


timestamp
account::
get_last_post_timestamp() const noexcept
{
  return m_record_table? m_record_table.get_last().get_timestamp():timestamp();
}




namespace{
bool
test(const std::list<account_observer>&  ls, const account_observer&  obs) noexcept
{
    for(auto&  o: ls)
    {
        if(o == obs)
        {
          return true;
        }
    }


  return false;
}
void
remove(std::list<account_observer>&  ls, const account_observer&  obs) noexcept
{
  auto   it = ls.begin();
  auto  end = ls.end();

    while(it != end)
    {
        if(*it == obs)
        {
          ls.erase(it);

          return;
        }
    }
}
}


void
account::
add_follow(const account_observer&  obs) noexcept
{
    if(!test(m_follow_list,obs))
    {
      m_follow_list.emplace_back(obs);
    }
}


void
account::
add_follower(const account_observer&  obs) noexcept
{
    if(!test(m_follower_list,obs))
    {
      m_follower_list.emplace_back(obs);
    }
}


void
account::
add_block(const account_observer&  obs) noexcept
{
    if(!test(m_follow_list,obs))
    {
      m_follow_list.emplace_back(obs);
    }
}


void
account::
add_mute(const account_observer&  obs) noexcept
{
    if(!test(m_mute_list,obs))
    {
      m_mute_list.emplace_back(obs);
    }
}




bool
account::
test_follow(const account_observer&  obs) const noexcept
{
  return test(m_follow_list,obs);
}


bool
account::
test_follower(const account_observer&  obs) const noexcept
{
  return test(m_follower_list,obs);
}


bool
account::
test_block(const account_observer&  obs) const noexcept
{
  return test(m_block_list,obs);
}


bool
account::
test_mute(const account_observer&  obs) const noexcept
{
  return test(m_mute_list,obs);
}




void
account::
remove_follow(const account_observer&  obs) noexcept
{
  remove(m_follow_list,obs);
}


void
account::
remove_follower(const account_observer&  obs) noexcept
{
  remove(m_follower_list,obs);
}


void
account::
remove_block(const account_observer&  obs) noexcept
{
  remove(m_block_list,obs);
}


void
account::
remove_mute(const account_observer&  obs) noexcept
{
  remove(m_mute_list,obs);
}




void
account::
print() const noexcept
{
  printf("name = \"%s\",\n",m_name.data());
  printf("sex = \"%s\",\n",m_sex.get_name().data());
}




}}




