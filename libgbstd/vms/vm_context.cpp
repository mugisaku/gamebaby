#include"libgbstd/vm.hpp"




namespace gbstd{




context::
context() noexcept
{
  m_type_collection.push_c_like_types();

  push_variable(m_type_collection["int"],"final return value");
}




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


variable&
context::
get_variable(multi_pointer  p) noexcept
{
  return p.is_global()? get_global_variable(p.get())
        :p.is_local() ? get_local_variable( p.get())
        :m_variable_table[0]
        ;
}


value
context::
get_value(multi_pointer  p) const noexcept
{
  return p.is_global()  ? get_global_variable(p.get()).get_value()
        :p.is_local()   ? get_local_variable( p.get()).get_value()
        :p.is_function()? get_function(       p.get()).get_value()
        :value()
        ;
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

  uint32_t  m_store_pointer;

  uint32_t  m_pc=0;

  frame(const function&  fn, uint32_t  bi) noexcept:
  m_function(fn), m_base_index(bi){}

  operator bool() const noexcept{return m_pc < m_function.get_number_of_codelines();}

  const codeline&  operator*() noexcept{return m_function[m_pc++];}

};


value
context::
dereference(const value&  v) noexcept
{
    if(v.get_type_info().is_reference())
    {
      auto  p = multi_pointer(v.get_unsigned_integer());

      return get_value(p);
    }


  return v;
}


int
context::
get_base_index() const noexcept
{
  return m_current_frame->m_base_index;
}


void
context::
store(const value&  dst, const value&  src) noexcept
{
    if(!dst.get_type_info().is_reference())
    {
      report;
      return;
    }


  auto  srcv = dereference(src);

  auto  val = get_value(multi_pointer(dst.get_unsigned_integer()));

  val.get_memory().copy(srcv.get_memory());
}


void
context::
process(const branch_instruction&   br) noexcept
{
  value  condv = br.get_condition().evaluate(*this);

    if(condv.get_integer())
    {
      value  destv = br.get_destination().evaluate(*this);

      m_current_frame->m_pc = destv.get_integer();
    }
}


void
context::
process(const return_instruction&  ret) noexcept
{
  auto  val = dereference(ret.get_operand().evaluate(*this));

  pop_frame();

    if(m_current_frame)
    {
      get_global_variable(m_current_frame->m_store_pointer).get_value() = std::move(val);
    }

  else
    {
      auto&  val0 = m_variable_table[0].get_value();

      val0 = std::move(val);

      val0.print();

      printf("%" PRIi64 "\n",val0.get_integer());
    }
}


void
context::
process(const operation&  op) noexcept
{
  auto&  opls = op.get_operand_list();

  auto&  dst = get_variable(opls[0].get_pointer());

    if(op.is_unary())
    {
      dst.get_value() = operate(op.get_unary_opcodes(),opls[1]);
    }

  else
    if(op.is_binary())
    {
      dst.get_value() = operate(op.get_binary_opcodes(),opls[1],opls[2]);
    }

  else
    if(op.is_call())
    {
      push_frame(dst.get_address(),get_function(opls[1].get_pointer()),opls.size()-2,&opls[2]);
    }

  else
    if(op.is_seek())
    {
      seek(dst,get_value(opls[1].get_pointer()),opls[2].get_identifier());
    }
}


void
context::
seek(variable&  dst, value&&  src, std::string_view  name) noexcept
{
  
}




variable&
context::
push_variable(const typesystem::type_info&  ti, std::string_view  name) noexcept
{
  m_variable_table.emplace_back(m_variable_table.size(),ti,name);

  return m_variable_table.back();
}


void
context::
push_frame(uint32_t  st_p, const function&  fn, int  argc, const operand*  argv) noexcept
{
    if(m_current_frame)
    {
      m_current_frame->m_store_pointer = st_p;
    }


  auto  new_frame = new frame(fn,m_variable_table.size());

  new_frame->m_previous = m_current_frame            ;
                          m_current_frame = new_frame;

  auto  paras = fn.get_signature().get_parameter_list().begin();
  auto  names = fn.get_argument_name_list().begin();

    while(argc--)
    {
      auto&    ti = **paras++;
      auto&  name =  *names++;

      push_variable(ti,name).get_value() = argv++->evaluate(*this);
    }


    for(auto&  decl: fn.get_declaration_list())
    {
      push_variable(*decl.get_type_info(),decl.get_name());
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
      process(codeln.get_branch_instruction());
    }

  else
    if(codeln.is_return_instruction())
    {
      process(codeln.get_return_instruction());
    }

  else
    if(codeln.is_operation())
    {
      process(codeln.get_operation());
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




