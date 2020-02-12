#include"libgbstd/vm.hpp"




namespace gbstd{




context::
context() noexcept
{
  m_type_collection.set_pointer_size(sizeof(multi_pointer::value_type));

  m_type_collection.push_c_like_types();

  push_variable("GLOB",*m_type_collection.find_by_name("int"),"final return value");
}




function&
context::
create_function(std::string_view  sig, std::string_view  name) noexcept
{
  auto  ti = m_type_collection.create_from_string(sig);

  m_function_table.emplace_back(*this,major_address('f',m_function_table.size()),*ti,name);

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


variable&
context::
push_variable(std::string_view  fn_name, const typesystem::type_info&  ti, std::string_view  name) noexcept
{
  m_variable_table.emplace_back(fn_name,major_address('l',m_variable_table.size()),ti,name);

  return m_variable_table.back();
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


value
context::
get_value(multi_pointer  p) const noexcept
{
  auto  maj = p.get_major();

  return maj.is_function()? get_function(maj).get_value()
        :get_variable(maj).get_value()
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

  major_address  m_store_pointer;

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
print_debug() const noexcept
{
    if(m_current_frame)
    {
      printf("[%s]:\n",m_current_frame->m_function.get_name().data());
    }


    for(auto&  v: m_variable_table)
    {
      v.print();

      printf("\n");
    }
}


void
context::
process(const return_instruction&  ret) noexcept
{
//  print_debug();

  auto  val = dereference(ret.get_operand().evaluate(*this));

  pop_frame();

    if(m_current_frame)
    {
      get_variable(m_current_frame->m_store_pointer).get_value() = std::move(val);
    }

  else
    {
      auto&  val0 = m_variable_table[0].get_value();

      val0 = std::move(val);

      printf("\n====\n");

      val0.print();

      printf("\n====\n");
    }
}


void
context::
process(const operation&  op) noexcept
{
  auto&  opls = op.get_operand_list();

  auto&  dst = get_variable(opls[0].get_pointer().get_major());

    if(op.is_unary())
    {
      auto  v = operate(op.get_unary_opcodes(),opls[1]);

      dst.get_value() = dereference(v);
    }

  else
    if(op.is_binary())
    {
      auto  v = operate(op.get_binary_opcodes(),opls[1],opls[2]);

      dst.get_value() = std::move(v);
    }

  else
    if(op.is_call())
    {
      push_frame(opls[0].get_pointer().get_major(),get_function(opls[1].get_pointer().get_major()),opls.size()-2,&opls[2]);
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




void
context::
push_frame(major_address  st_p, const function&  fn, int  argc, const operand*  argv) noexcept
{
  int  new_base_index = m_variable_table.size();

  auto  paras = fn.get_type_info().get_function_signature().get_parameter_list().begin();
  auto  names = fn.get_argument_name_list().begin();

    while(argc--)
    {
      auto&    ti = **paras++;
      auto&  name =  *names++;

      auto&  dst_var = push_variable(fn.get_name().data(),ti,name);

      auto  dst_ti = &dst_var.get_value().get_type_info();

      auto    src_v = argv++->evaluate(*this);
      auto&  src_ti = src_v.get_type_info();

        if(src_ti == *dst_ti)
        {
          dst_var.get_value() = src_v;
        }

      else
        {
            if(src_ti.is_reference())
            {
                if(src_ti.get_reference_type_info().get_base_type_info() == *dst_ti)
                {
                  auto  p = multi_pointer(src_v.get_unsigned_integer());

                  auto&  var = get_variable(p.get_major());

                  dst_var.get_value() = var.get_value();
                }
            }
        }
    }


    if(m_current_frame)
    {
      m_current_frame->m_store_pointer = st_p;
    }


  auto  new_frame = new frame(fn,new_base_index);

  new_frame->m_previous = m_current_frame            ;
                          m_current_frame = new_frame;

    for(auto&  decl: fn.get_declaration_list())
    {
      push_variable(fn.get_name().data(),*decl.get_type_info(),decl.get_name());
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


bool
context::
step() noexcept
{
START:
    if(!m_current_frame)
    {
      return false;
    }


    if(!*m_current_frame)
    {
      pop_frame();

      goto START;
    }


  auto&  codeln = **m_current_frame;

//printf("%s pc: %4d\n",m_current_frame->m_function.get_name().data(),m_current_frame->m_pc);
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


  return true;
}


void
context::
run() noexcept
{
    while(step())
    {
    }
}




context&
context::
finalize() noexcept
{
    for(auto&  fn: m_function_table)
    {
      fn.finalize();
    }


  return *this;
}


}




