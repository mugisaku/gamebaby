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

  uint32_t  get_next_base_index() const noexcept{return m_base_index+m_function.get_number_of_variables();}

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

    if(val.get_type_info().get_size() <= sizeof(int64_t))
    {
      val.get_integer() = srcv.get_integer();
    }
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
      call(dst,0,nullptr);
    }

  else
    if(op.is_seek())
    {
      seek(dst,get_value(opls[1].get_pointer()),opls[2].get_identifier());
    }
}


void
context::
call(variable&  var, int  n, const operand*  ops) noexcept
{
  auto&  target = get_function(ops++->get_pointer());

  auto  argtypes = target.get_signature().get_parameter_list().begin();
  auto  argnames = target.get_argument_name_list().begin();

  std::vector<variable>  args;

    while(n--)
    {
  //                      args.emplace_back(it++->evaluate(*this),*argnames++);
    }


  push_frame(var.get_address(),target,args.size(),args.data());
}


void
context::
seek(variable&  dst, value&&  src, std::string_view  name) noexcept
{
  
}




void
context::
push_variable(const typesystem::type_info&  ti, std::string_view  name) noexcept
{
  m_variable_table.emplace_back(m_variable_table.size(),ti,name);
}


void
context::
push_frame(uint32_t  st_p, const function&  fn, int  argc, const variable*  argv) noexcept
{
    if(m_current_frame)
    {
      m_current_frame->m_store_pointer = st_p;
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




