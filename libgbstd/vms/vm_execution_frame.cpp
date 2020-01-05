#include"libgbstd/vm.hpp"




namespace gbstd{




execution_frame::
execution_frame(const function&  fn, execution_frame*  parent, int  argc, const operand*  args) noexcept:
m_function(fn)
{
  auto  frm = parent? *parent:*this;

/*
    for(auto&  p: m_function.get_parameter_list())
    {
        if(argc--)
        {
          m_variable_list.emplace_back(p,args++->evaluate(frm));
        }

      else
        {
          break;
        }
    }
*/

    for(auto&  vinit: m_function.get_variable_initializer_list())
    {
      m_variable_list.emplace_back(vinit.get_label(),vinit.get_value(*this));
    }
}




const variable&
execution_frame::
push_variable(std::string_view  lb, value  v) noexcept
{
  auto  var = find_variable(lb);

    if(!var)
    {
//      var = &m_variable_list.emplace_back(lb);
    }


  var->update_value(std::move(v));

  return *var;
}


variable*
execution_frame::
find_variable(std::string_view  lb) noexcept
{
    for(auto&  v: m_variable_list)
    {
        if(v.get_label() == lb)
        {
          return &v;
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
        if(v.get_label() == lb)
        {
          return &v;
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

      store(st.get_destination().evaluate(*this),
                 st.get_source().evaluate(*this));
    }

  else
    if(codeln.is_branch_instruction())
    {
      auto&  br = codeln.get_branch_instruction();

      auto  v = br.get_condition().evaluate(*this);
/*
      auto&  lb = v.get_data()? br.get_nonzero_label():br.get_zero_label();

      auto  ep = m_function.find_entry_point(lb);

        if(ep)
        {
          m_pc = ep->get_value();
        }
*/
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
      codeln.get_operation().evaluate(*this);
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




