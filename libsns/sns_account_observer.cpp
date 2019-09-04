#include"libsns/sns_account.hpp"




namespace sns{




struct
account_observer::
private_data
{
  uint64_t  count;

  account*  pointer;

};


void
account_observer::
unrefer() noexcept
{
    if(m_data)
    {
      auto&  c = m_data->count;

        if(!--c)
        {
          delete m_data;
        }


      m_data = nullptr;
    }
}


account_observer::
account_observer(account&  ref) noexcept:
m_data(new private_data)
{
  m_data->count   =    1;
  m_data->pointer = &ref;
}




account_observer::
operator bool() const noexcept
{
  return m_data && m_data->pointer;
}


account&  account_observer::operator *() const noexcept{return *m_data->pointer;}
account*  account_observer::operator->() const noexcept{return  m_data->pointer;}


account_observer&
account_observer::
assign(const account_observer&   rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      m_data = rhs.m_data;

        if(m_data)
        {
          ++m_data->count;
        }
    }


  return *this;
}


account_observer&
account_observer::
assign(account_observer&&  rhs) noexcept
{
    if(this != &rhs)
    {
      std::swap(m_data,rhs.m_data);
    }


  return *this;
}


void
account_observer::
die() noexcept
{
    if(m_data)
    {
      m_data->pointer = nullptr;
    }
}




}




