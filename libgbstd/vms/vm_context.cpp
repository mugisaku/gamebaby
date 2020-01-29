#include"libgbstd/vm.hpp"




namespace gbstd{




function&
context::
create_function(std::string_view  name) noexcept
{
  m_function_table.emplace_back(*this,m_function_table.size(),name);

  return m_function_table.back();
}


function*
context::
find_function(std::string_view  name) noexcept
{
    for(auto&  f: m_function_table)
    {
        if(f.get_name() == name)
        {
          return &f;
        }
    }


  return nullptr;
}


const function*
context::
find_function(std::string_view  name) const noexcept
{
    for(auto&  f: m_function_table)
    {
        if(f.get_name() == name)
        {
          return &f;
        }
    }


  return nullptr;
}


variable*
context::
find_variable(std::string_view  name) noexcept
{
    for(auto&  v: m_variable_table)
    {
        if(v.get_name() == name)
        {
          return &v;
        }
    }


  return nullptr;
}


const variable*
context::
find_variable(std::string_view  name) const noexcept
{
    for(auto&  v: m_variable_table)
    {
        if(v.get_name() == name)
        {
          return &v;
        }
    }


  return nullptr;
}


void
context::
print() const noexcept
{
    for(auto&  f: m_function_table)
    {
      f.print();

      printf("\n");
    }
}




struct
context::
frame
{
  frame*  m_previous=nullptr;

  const function&  m_function;

  const uint32_t  m_base_index=0;

  variable_pointer  m_store_pointer;

  uint32_t  m_pc=0;

  frame(const function&  fn, uint32_t  bi) noexcept:
  m_function(fn), m_base_index(bi){}

  uint32_t  get_next_base_index() const noexcept{return m_base_index+m_function.get_number_of_variables();}

  operator bool() const noexcept{return m_pc < m_function.get_number_of_codelines();}

  const codeline&  operator*() noexcept{return m_function[m_pc++];}

};




variable&
context::
operator[](variable_pointer  p) noexcept
{
  return m_variable_table[(p.is_local()? get_base_index():0)+p.get()];
}


const variable&
context::
operator[](variable_pointer  p) const noexcept
{
  return m_variable_table[(p.is_local()? get_base_index():0)+p.get()];
}


int
context::
get_base_index() const noexcept
{
  return m_current_frame->m_base_index;
}


void
context::
store(value  dst, value  src) noexcept
{
/*
    if(src.is_data())
    {
      auto&  mem = get_memory();

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
context::
push_frame(variable_pointer  p, const function&  fn, int  argc, const variable*  argv) noexcept
{
    if(m_current_frame)
    {
      m_current_frame->m_store_pointer = p;
    }


  auto  i = m_current_frame? m_current_frame->get_next_base_index()
                           :                                      0;

  auto  new_frame = new frame(fn,i);

  new_frame->m_previous = m_current_frame            ;
                          m_current_frame = new_frame;


    while(argc--)
    {
//      m_variable_table.append(std::move(*args++));
    }


    for(auto&  decl: fn.get_declaration_list())
    {
      m_variable_table.emplace_back(i++,*decl.get_type_info(),decl.get_name());
    }
}


void
context::
pop_frame() noexcept
{
  auto  numvars = m_current_frame->m_function.get_number_of_variables();

    while(numvars--)
    {
      m_variable_table.pop_back();
    }


  auto  old_frame = m_current_frame                        ;
                    m_current_frame = old_frame->m_previous;

  delete old_frame;
}


void
context::
step() noexcept
{
    if(!m_current_frame)
    {
      return;
    }


    if(!*m_current_frame)
    {
      pop_frame();

      return;
    }


  auto&  codeln = **m_current_frame;

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

      auto  ep = m_current_frame->m_function.find_entry_point(br.get_label());

        if(ep)
        {
          m_current_frame->m_pc = ep->get_value();
        }
    }

  else
    if(codeln.is_return_instruction())
    {
      auto&  ret = codeln.get_return_instruction();

      auto  val = ret.get_operand().evaluate(*this);

      pop_frame();

        if(m_current_frame)
        {
          (*this)[m_current_frame->m_store_pointer].get_value() = std::move(val);
        }

      else
        {
          m_last_value = std::move(val);

          m_last_value.print();
        }
    }

  else
    if(codeln.is_operation())
    {
      auto&  op = codeln.get_operation();

      auto&  opls = op.get_operand_list();

      auto  vp = opls[0].get_variable_pointer();

      auto&  var = m_variable_table[vp.get()];

        if(op.is_unary())
        {
          var.get_value() = operate(op.get_unary_opcodes(),opls[1]);
        }

      else
        if(op.is_binary())
        {
          var.get_value() = operate(op.get_binary_opcodes(),opls[1],opls[2]);
        }

      else
        if(op.is_call())
        {
          auto&  target = (*this)[opls[1].get_function_pointer()];

          auto  argtypes = target.get_signature().get_parameter_list().begin();
          auto  argnames = target.get_argument_name_list().begin();

          auto  it     = &opls[2];
          auto  it_end = &opls.back()+1;

          std::vector<variable>  args;

            while(it != it_end)
            {
//                      args.emplace_back(it++->evaluate(*this),*argnames++);
            }


          push_frame(vp,target,args.size(),args.data());
        }
    }
}


void
context::
run() noexcept
{
    for(;;)
    {
        if(!m_current_frame || !*m_current_frame)
        {
          break;
        }


      step();
    }
}




context&
context::
finalize() noexcept
{
    for(auto&  fn: m_function_table)
    {
      fn.finalize(*this);
    }


  return *this;
}


}




