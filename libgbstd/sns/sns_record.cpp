#include"libgbstd/sns.hpp"




namespace gbstd{
namespace sns{




sha256_hash
record::
generate_hash() const noexcept
{
  sha256_hash_generator  gen;

  gen.push_be64(m_index);
  gen.push_be64(m_timestamp.get_value());

    for(auto  c: m_content)
    {
      gen.push(c);
    }


    for(auto  c: m_seed_hash)
    {
      gen.push_be32(c);
    }


  gen.finish();

  return gen;
}


void
record::
print() const noexcept
{
  printf("date = %" PRIu64 "(",m_timestamp.get_value());

  auto  s = to_string(m_timestamp);

  printf("%s), text = \"%s\"",s.data(),m_content.data());
}



}}




