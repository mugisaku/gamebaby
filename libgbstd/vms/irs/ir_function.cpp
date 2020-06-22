#include"libgbstd/vms/ir.hpp"




namespace gbstd{




void
ir_function::
read(token_iterator&  it)
{
    for(;;)
    {
        if(!it)
        {
          throw ir_error("function read error: close code not found");
        }

      else
        if(it->is_operator_code("}"))
        {
          ++it;

          break;
        }

      else
        if(it->is_identifier())
        {
          auto&  s = it++->get_string();

            if(it->is_operator_code(":"))
            {
              ++it;

              add_label("%s",s.data());
            }

          else
            {
              add_operation().read(s,it);
            }
        }

      else
        {
          throw ir_error("function read error: unkown element");
        }
    }
}




ir_operation&
ir_function::
add_operation() noexcept
{
    if(m_block_info_list.empty())
    {
      m_block_info_list.emplace_back(m_first_label);
    }


  auto&  bi = m_block_info_list.back();

  auto&  op = m_operation_list.emplace_back(bi);

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

  m_block_info_list.emplace_back(sv);

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




