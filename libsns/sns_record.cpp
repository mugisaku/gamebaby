#include"libsns/sns_table.hpp"




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
  printf("date = %ld(",m_timestamp.get_value());

  time_object  to(m_timestamp);

  to.print();

  printf("), text = \"%s\"",m_content.data());
}



}




