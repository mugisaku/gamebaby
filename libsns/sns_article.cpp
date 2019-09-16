#include"libsns/sns.hpp"




namespace sns{




article&
article::
assign(const account_observer&  obs, const record&  rec) noexcept
{
  m_observer = obs;

    if(obs)
    {
      m_name = to_u16string(obs->get_name());

      m_timestamp = rec.get_timestamp();

      m_date = to_u16string(to_string(rec.get_timestamp()));

      m_content = rec.get_content();
    }


  return *this;
}


}




