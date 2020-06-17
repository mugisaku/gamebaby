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
add_pshi(int64_t  i) noexcept
{
  m_block_list.back().m_line_list.emplace_back(asm_opcode::pshi8 ,asm_parameter(i));
}


void
asm_context::
add_comment(const char*  fmt, ...) noexcept
{
  va_list  ap;

  va_start(ap,fmt);

  static char  buf[1024] = {0};

  vsnprintf(buf,sizeof(buf),fmt,ap);

  va_end(ap);

  auto&  lnls = m_block_list.back().m_line_list;

    if(lnls.size())
    {
      lnls.back().set_comment(buf);
    }
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




