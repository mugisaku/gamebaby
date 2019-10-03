#include"libgbstd/sns.hpp"
#include"libgbstd/character.hpp"




namespace gbstd{
namespace sns{




article&
article::
assign(const account_observer&  obs, const record&  rec) noexcept
{
  m_observer = obs;

    if(obs)
    {
      m_name = gbstd::make_u16string(obs->get_name());

      m_timestamp = rec.get_timestamp();

      auto  s = to_string(rec.get_timestamp());

      m_date = gbstd::make_u16string(s);

      m_content = rec.get_content();
    }


  return *this;
}


void
article::
link(article*  f, article*  b) noexcept
{
    if(f){f->m_backward = b;}
    if(b){b->m_forward  = f;}
}




}}




