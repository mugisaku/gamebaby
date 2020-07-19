#include"libgbstd/vm.hpp"




namespace gbstd{




compile_context&
compile_context::
assign(std::string_view  src_code)
{
  m_source = src_code;

  auto&  gsp = m_root_node.get_global_space();

  gsp.assign(src_code);

  m_ir_text.clear();

  m_stack.clear();
  m_stack.emplace_back();

  gsp.compile(*this);

  return *this;
}




namespace{
std::string_view
to_ir_type_info_string(const type_info&  ti) noexcept
{
    if(ti.is_boolean() ||
       ti.is_enum()    ||
       ti.is_integer() ||
       ti.is_pointer() ||
       ti.is_reference())
    {
      return "int";
    }

  else
    if(ti.is_fpn())
    {
      return "float";
    }

  else
    {
    }


  return "";
}
}


std::string
compile_context::
create_variable_name() noexcept
{
  snprintf(m_buffer,sizeof(m_buffer),"var%04d",m_stack.back().m_variable_counter++);

  return m_buffer;
}


void
compile_context::
write_global(const char*  fmt, ...) noexcept
{
  va_list  ap;

  va_start(ap,fmt);

  vsnprintf(m_buffer,sizeof(m_buffer),fmt,ap);

  va_end(ap);

  m_ir_text += m_buffer;
}


void
compile_context::
write_local(const char*  fmt, ...) noexcept
{
  va_list  ap;

  va_start(ap,fmt);

  vsnprintf(m_buffer,sizeof(m_buffer),fmt,ap);

  va_end(ap);

  m_stack.back().m_ir_text += m_buffer;
}


void
compile_context::
start_function_block(const function&  fn)
{
  auto&  t = m_stack.emplace_back();

  t.m_ir_text += "function\n";

  auto&  sig = fn.get_signature();

  t.m_ir_text += to_ir_type_info_string(sig.get_return_type_info());
  t.m_ir_text += "\n";

  t.m_ir_text += fn.get_name();
  t.m_ir_text += "(";

    for(auto&  para: sig.get_parameter_list())
    {
      t.m_ir_text += to_ir_type_info_string(para.get_type_info());

      t.m_ir_text += "  ";

      t.m_ir_text += para.get_name();

      t.m_ir_text += ", ";
    }


  t.m_ir_text += ")\n{\n";
}


void
compile_context::
start_if_string_block()
{
  auto&  t = m_stack.back();

  snprintf(m_buffer,sizeof(m_buffer),"IF_STRING_BLOCK%04d",t.m_if_string_counter++);

  auto&  s = t.m_block_name_stack.emplace_back(m_buffer);

  snprintf(m_buffer,sizeof(m_buffer),"%s:\n",s.data());

  t.m_ir_text += m_buffer;
}


void
compile_context::
start_for_block()
{
  auto&  t = m_stack.back();

  snprintf(m_buffer,sizeof(m_buffer),"FOR_BLOCK%04d",t.m_for_counter++);

  auto&  s = t.m_block_name_stack.emplace_back(m_buffer);

  snprintf(m_buffer,sizeof(m_buffer),"%s:\n",s.data());

  t.m_ir_text += m_buffer;
}


void
compile_context::
start_while_block()
{
  auto&  t = m_stack.back();

  snprintf(m_buffer,sizeof(m_buffer),"WHILE_BLOCK%04d",t.m_while_counter++);

  auto&  s = t.m_block_name_stack.emplace_back(m_buffer);

  snprintf(m_buffer,sizeof(m_buffer),"%s:\n",s.data());

  t.m_ir_text += m_buffer;
}


void
compile_context::
start_switch_block()
{
  auto&  t = m_stack.back();

  snprintf(m_buffer,sizeof(m_buffer),"SWITCH_BLOCK%04d",t.m_switch_counter++);

  auto&  s = t.m_block_name_stack.emplace_back(m_buffer);

  snprintf(m_buffer,sizeof(m_buffer),"%s:\n",s.data());

  t.m_ir_text += m_buffer;
}


void
compile_context::
finish_block() noexcept
{
  auto&  t = m_stack.back();

    if(t.m_block_name_stack.size())
    {
      auto&  s = t.m_block_name_stack.back();

      snprintf(m_buffer,sizeof(m_buffer),"%s__end:\n",s.data());

      t.m_ir_text += m_buffer;

      t.m_block_name_stack.pop_back();
    }

  else
    {
      m_ir_text += t.m_ir_text;

      m_ir_text += "}\n\n\n";

      m_stack.pop_back();
    }
}


void
compile_context::
print() const noexcept
{
  auto&  gsp = m_root_node.get_global_space();

  gsp.print();

  printf("\n----\n");

  printf("%s\n",m_ir_text.data());
}




}




