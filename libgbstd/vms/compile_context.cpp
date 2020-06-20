#include"libgbstd/vm.hpp"




namespace gbstd{




compile_context&
compile_context::
assign(std::string_view  src_code)
{
  m_source = src_code;

  auto&  gsp = m_root_node.get_global_space();

  gsp.assign(src_code);

  m_ir_context.clear();

  enter_function("GLOBAL",{});

  gsp.compile(*this);

  return *this;
}




const char*
compile_context::
add_begin_label(std::string_view  base_sv) noexcept
{
  m_stack.back().m_base_name_stack.emplace_back(base_sv);


  auto  s = get_control_block_start_label();

  (**this).add_label("%s",s);

  return s;
}


void
compile_context::
enter_function(std::string_view  fn_name, std::vector<ir_parameter>&&  parals) noexcept
{
  auto&  st = m_stack.emplace_back();

  st.m_ir_function = &m_ir_context.create_function(fn_name,std::move(parals));

  st.m_if_string_counter = 0;
  st.m_for_counter       = 0;
  st.m_while_counter     = 0;
  st.m_switch_counter    = 0;
}


const char*
compile_context::
enter_while_block() noexcept
{
  int  n = snprintf(m_buffer,sizeof(m_buffer),"WHILE%04d",m_stack.back().m_while_counter++);

  return add_begin_label(std::string_view(m_buffer,n));
}


const char*
compile_context::
enter_for_block() noexcept
{
  int  n = snprintf(m_buffer,sizeof(m_buffer),"FOR%04d",m_stack.back().m_for_counter++);

  return add_begin_label(std::string_view(m_buffer,n));
}


const char*
compile_context::
enter_switch_block() noexcept
{
  int  n = snprintf(m_buffer,sizeof(m_buffer),"SWITCH%04d",m_stack.back().m_switch_counter++);

  return add_begin_label(std::string_view(m_buffer,n));
}


const char*
compile_context::
enter_if_string_block() noexcept
{
  int  n = snprintf(m_buffer,sizeof(m_buffer),"IFS%04d",m_stack.back().m_if_string_counter++);

  return add_begin_label(std::string_view(m_buffer,n));
}


void
compile_context::
leave_control_block()
{
  (**this).add_label("%s",get_control_block_end_label());

  m_stack.back().m_base_name_stack.pop_back();
}


const char*
compile_context::
get_control_block_start_label() noexcept
{
  auto  s = get_control_block_base_name();

  snprintf(m_buffer,sizeof(m_buffer),"%s__start",s);

  return m_buffer;
}


const char*
compile_context::
get_control_block_end_label() noexcept
{
  auto  s = get_control_block_base_name();

  snprintf(m_buffer,sizeof(m_buffer),"%s__end",s);

  return m_buffer;
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




