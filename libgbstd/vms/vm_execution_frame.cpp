#include"libgbstd/vm.hpp"




namespace gbstd{




execution_frame::
execution_frame(const function&  fn, execution_frame*  parent, int  argc, variable*  args) noexcept:
m_function(fn)
{
    for(auto&  decl: fn.get_declaration_list())
    {
      m_variable_table.append(*decl.get_type_info(),decl.get_name());
    }


    while(argc--)
    {
      m_variable_table.append(std::move(*args++));
    }
}




void
execution_frame::
store(value  dst, value  src) noexcept
{
/*
    if(src.is_data())
    {
      auto&  mem = m_function.get_space().get_memory();

      auto  dst_p = dst.get_data();
      auto  src_d = src.get_data();

        if(dst.is_s8_pointer())
        {
          *reinterpret_cast<int8_t*>(mem.get_ptr8(dst_p)) = src_d;
        }

      else
        if(dst.is_u8_pointer())
        {
          *mem.get_ptr8(dst_p) = src_d;
        }

      else
        if(dst.is_s16_pointer())
        {
          *reinterpret_cast<int16_t*>(mem.get_ptr16(dst_p)) = src_d;
        }

      else
        if(dst.is_u16_pointer())
        {
          *mem.get_ptr16(dst_p) = src_d;
        }

      else
        if(dst.is_s32_pointer())
        {
          *reinterpret_cast<int32_t*>(mem.get_ptr32(dst_p)) = src_d;
        }

      else
        if(dst.is_u32_pointer())
        {
          *mem.get_ptr32(dst_p) = src_d;
        }

      else
        if(dst.is_s64_pointer())
        {
          *reinterpret_cast<int64_t*>(mem.get_ptr64(dst_p)) = src_d;
        }

      else
        if(dst.is_u64_pointer())
        {
          *mem.get_ptr64(dst_p) = src_d;
        }
    }
*/
}


void
execution_frame::
step(const codeline&  codeln) noexcept
{
    if(codeln.is_store_instruction())
    {
      auto&  st = codeln.get_store_instruction();

      value  dst_v = st.get_destination().evaluate(*this);
      value  src_v = st.get_source().evaluate(*this);

      store(dst_v,src_v);
    }

  else
    if(codeln.is_branch_instruction())
    {
      auto&  br = codeln.get_branch_instruction();

      value  v = br.get_condition().evaluate(*this);

      auto  ep = m_function.find_entry_point(br.get_label());

        if(ep)
        {
          m_pc = ep->get_value();
        }
    }

  else
    if(codeln.is_return_instruction())
    {
      auto&  ret = codeln.get_return_instruction();

      m_returned_value = ret.get_operand().evaluate(*this);

      m_returned = true;
    }

  else
    if(codeln.is_operation())
    {
      auto&  op = codeln.get_operation();

      auto  var = m_variable_table.find(op.get_label());

        if(var)
        {
          var->get_value() = op(*this);
        }
    }
}


value
execution_frame::
run() noexcept
{
  auto  n = m_function.get_number_of_codelines();

  m_pc       =     0;
  m_returned = false;

    while(m_pc < n)
    {
//printf("[%s %d]\n",m_function.get_name().data(),m_pc);

      step(m_function[m_pc++]);

        if(m_returned)
        {
          break;
        }
    }


  return m_returned_value;
}




}




