#include"libgbstd/vm.hpp"




namespace gbstd{




compile_context&
compile_context::
assign(std::string_view  src_code)
{
  m_source = src_code;

  auto&  gsp = m_root_node.get_global_space();

  gsp.assign(src_code);

  gsp.compile(*this);

  return *this;
}




const char*
compile_context::
add_begin_label(std::string_view  base_sv) noexcept
{
  char  begin_buf[32];
  char    end_buf[32];

  snprintf(begin_buf,sizeof(begin_buf),"%s__begin",base_sv.data());

  int  n = snprintf(end_buf,sizeof(end_buf),"%s__end"  ,base_sv.data());

  m_ir_function->add_label(begin_buf);

  auto  p = std::make_pair(std::string(base_sv),std::string(end_buf,n));

  return m_control_block_name_stack.emplace_back(std::move(p)).first.data();
}


const char*
compile_context::
enter_while_block() noexcept
{
  char  buf[32];

  int  n = snprintf(buf,sizeof(buf),"WHILE%04d",m_while_counter++);

  return add_begin_label(std::string_view(buf,n));
}


const char*
compile_context::
enter_for_block() noexcept
{
  char  buf[32];

  int  n = snprintf(buf,sizeof(buf),"FOR%04d",m_while_counter++);

  return add_begin_label(std::string_view(buf,n));
}


const char*
compile_context::
enter_switch_block() noexcept
{
  char  buf[32];

  int  n = snprintf(buf,sizeof(buf),"SWITCH%04d",m_while_counter++);

  return add_begin_label(std::string_view(buf,n));
}


const char*
compile_context::
enter_if_string_block() noexcept
{
  char  buf[32];

  int  n = snprintf(buf,sizeof(buf),"IFS%04d",m_while_counter++);

  return add_begin_label(std::string_view(buf,n));
}


void
compile_context::
leave_control_block()
{
    if(m_control_block_name_stack.empty())
    {
      throw compile_error(0,"制御ブロック名スタックが空");
    }


  auto&  name = m_control_block_name_stack.back();

  m_ir_function->add_label("%s",name.second.data());

  m_control_block_name_stack.pop_back();
}




void
compile_context::
print() const noexcept
{
  auto&  gsp = m_root_node.get_global_space();

  gsp.print();

  printf("\n----\n");

  m_ir_context.print();
}




}




