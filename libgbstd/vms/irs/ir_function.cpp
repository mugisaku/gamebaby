#include"libgbstd/vms/ir.hpp"




namespace gbstd{




const ir_block*
ir_function::
find_block(std::string_view  label) const noexcept
{
    for(auto&  blk: m_block_list)
    {
        for(auto&  lb: blk.m_label_list)
        {
            if(lb == label)
            {
              return &blk;
            }
        }
    }


  return nullptr;
}


const ir_operation*
ir_function::
find_operation(std::string_view  label, const ir_block**  blkptr) const noexcept
{
    for(auto&  blk: m_block_list)
    {
        for(auto&  op: blk.m_operation_list)
        {
            if(op.get_label() == label)
            {
                if(blkptr)
                {
                  *blkptr = &blk;
                }


              return &op;
            }
        }
    }


  return nullptr;
}


void
ir_function::
add_label(const char*  fmt, ...) noexcept
{
  va_list  ap;

  va_start(ap,fmt);

  static char  buf[1024] = {0};

  vsnprintf(buf,sizeof(buf),fmt,ap);

  va_end(ap);

    if(m_block_list.back().m_operation_list.size())
    {
      m_block_list.emplace_back();
    }


  m_block_list.back().m_label_list.emplace_back(buf);
}


void
ir_function::
print() const noexcept
{
  printf("function %s(",m_name.data());

    for(auto&  para: m_parameter_list)
    {
      printf("%s,",para.data());
    }


  printf(")\n{\n");

    for(auto&  blk: m_block_list)
    {
        for(auto&  lb: blk.m_label_list)
        {
          printf("%s:\n",lb.data());
        }


        for(auto&  op: blk.m_operation_list)
        {
          printf("  %s = %s ",op.get_label().data(),op.get_operator_code().get_string());

            for(auto&  o: op.get_operand_list())
            {
              printf(" ");

              o.print();
            }


          printf("\n");
        }
    }


  printf("}\n");
}




}




