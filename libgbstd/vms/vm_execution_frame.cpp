#include"libgbstd/vm.hpp"




namespace gbstd{




execution_frame::
execution_frame(const function&  fn, execution_frame*  parent, int  argc, const variable*  args) noexcept:
m_function(fn)
{
    while(argc--)
    {
//      m_variable_list.emplace_back(args++);
    }
}




variable&
execution_frame::
create_variable(std::string_view  lb) noexcept
{
  auto  var = find_variable(lb);

    if(!var)
    {
      m_variable_list.emplace_back(std::make_unique<variable>(lb));

      var = m_variable_list.back().get();
    }


  return *var;
}


variable*
execution_frame::
find_variable(std::string_view  lb) noexcept
{
    for(auto&  v: m_variable_list)
    {
        if(v->get_label() == lb)
        {
          return v.get();
        }
    }


  return nullptr;
}


const variable*
execution_frame::
find_variable(std::string_view  lb) const noexcept
{
    for(auto&  v: m_variable_list)
    {
        if(v->get_label() == lb)
        {
          return v.get();
        }
    }


  return nullptr;
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

      auto  var = find_variable(op.get_label());

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




