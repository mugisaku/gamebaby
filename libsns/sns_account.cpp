#include"libsns/sns.hpp"




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
bool
remove(std::list<account_observer>&  ls, const account_observer&  obs) noexcept
{
  auto   it = ls.begin();
  auto  end = ls.end();

    while(it != end)
    {
        if(*it == obs)
        {
          ls.erase(it);

          return true;
        }
    }


  return false;
}
}


void
account::
add_follow(const account_observer&  obs) noexcept
{
    if(!obs)
    {
      return;
    }


    if(!test(m_follow_list,obs))
    {
      m_follow_list.emplace_back(obs);

        if(!test(obs->m_follower_list,m_observer))
        {
          obs->m_follower_list.emplace_back(m_observer);
        }
    }
}


void
account::
add_follower(const account_observer&  obs) noexcept
{
    if(!obs)
    {
      return;
    }


    if(!test(m_follower_list,obs))
    {
      m_follower_list.emplace_back(obs);

        if(!test(obs->m_follow_list,m_observer))
        {
          obs->m_follow_list.emplace_back(m_observer);
        }
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
    if(remove(m_follow_list,obs))
    {
      remove(obs->m_follower_list,m_observer);
    }
}


void
account::
remove_follower(const account_observer&  obs) noexcept
{
    if(remove(m_follower_list,obs))
    {
      remove(obs->m_follow_list,m_observer);
    }
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




}




