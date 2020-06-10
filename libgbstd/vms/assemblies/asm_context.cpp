#include"libgbstd/vms/assembly.hpp"




namespace gbstd{




void
asm_context::
add_label(const char*  fmt, ...) noexcept
{
  va_list  ap;

  va_start(ap,fmt);

  static char  buf[1024] = {0};

  vsnprintf(buf,sizeof(buf),fmt,ap);

  va_end(ap);

    if(m_block_list.back().m_line_list.size())
    {
      m_block_list.emplace_back();
    }


  m_block_list.back().m_label_list.emplace_back(buf);
}


void
asm_context::
print() const noexcept
{
    for(auto&  blk: m_block_list)
    {
        for(auto&  lb: blk.m_label_list)
        {
          printf("%s:\n",lb.data());
        }


        for(auto&  ln: blk.m_line_list)
        {
          printf("  ");

          ln.print();

          printf("\n");
        }
    }
}




}




