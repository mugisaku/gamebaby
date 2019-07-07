#include"libonch/onch.hpp"
#include<cstring>
#include<cstdio>
#include<new>




namespace gbstd{




void
onch_text::
push(const onch_text&  txt) noexcept
{
    for(auto  w: txt.m_words)
    {
      m_words.emplace_back(w);
    }
}


uint32_t
onch_text::
get_output_length() const noexcept
{
  uint32_t  l = 0;

    for(auto  w: m_words)
    {
      l += w.get_output_length();
    }


  return l;
}




void
onch_text::
output(sound_kind  k, onch_output_context&  ctx) const noexcept
{
    for(auto  w: m_words)
    {
      w.output(k,ctx);
    }
}


void
onch_text::
print() const noexcept
{
  printf("text{");

    for(auto  w: m_words)
    {
      w.print();

      printf("\n");
    }


  printf("}");
}




}




