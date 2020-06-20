#include"libgbstd/vms/ir.hpp"




namespace gbstd{




ir_operation&
ir_function::
create_operation(operator_code  opco) noexcept
{
  char  buf[16];

  int  n = snprintf(buf,sizeof(buf),"%%%04d",m_number++);

  auto&  bi = m_block_info_list.back();

  auto&  op = m_operation_list.emplace_back(bi,std::string_view(buf,n),opco);

  ++bi;

  return op;
}


const ir_block_info*
ir_function::
find_block_info(std::string_view  label) const noexcept
{
    for(auto&  bi: m_block_info_list)
    {
        if(bi.test_label(label))
        {
          return &bi;
        }
    }


  return nullptr;
}


const ir_operation*
ir_function::
find_operation(std::string_view  label) const noexcept
{
    for(auto&  op: m_operation_list)
    {
        if(op.get_label() == label)
        {
          return &op;
        }
    }


  return nullptr;
}


ir_function&
ir_function::
add_label(const char*  fmt, ...) noexcept
{
  va_list  ap;

  va_start(ap,fmt);

  static char  buf[1024] = {0};

  int  n = vsnprintf(buf,sizeof(buf),fmt,ap);

  va_end(ap);

  std::string_view  sv(buf,n);

    if(m_block_info_list.back().get_number_of_operations())
    {
      m_block_info_list.emplace_back(sv);
    }

  else
    {
      m_block_info_list.back().add_label(sv);
    }


  return *this;
}


void
ir_function::
finalize() noexcept
{
  auto  p = m_operation_list.data();

    for(auto&  bi: m_block_info_list)
    {
      bi.set_entry(p);

      p += bi.get_number_of_operations();
    }
}


void
ir_function::
print() const noexcept
{
  printf("function %s(",m_name.data());

    for(auto&  para: m_parameter_list)
    {
      printf("%s,",para.get_label().data());
    }


  printf(")\n{\n");

    for(auto&  bi: m_block_info_list)
    {
      bi.print();

      printf("\n");
    }


  printf("}\n");
}




}




