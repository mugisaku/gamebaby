#include"libgbstd/vm.hpp"




namespace gbstd{




vm_execution&
vm_execution::
assign(const vm_jobfield&  jf) noexcept
{
  m_jobfield = &jf;

  m_lines.clear();

    for(auto&  fn: jf.function_table())
    {
        for(auto&  blk: fn.block_array())
        {
            for(auto&  ln: blk.lines())
            {
              m_lines.emplace_back(&ln);
            }
        }
    }


  return *this;
}




const vm_symbol*
vm_execution::
find_symbol(std::u16string_view  name) const noexcept
{
    if(m_function_stack.size())
    {
        for(auto&  sym: m_function_stack.back()->symbol_table())
        {
            if(sym.name() == name)
            {
              return &sym;
            }
        }
    }


  return nullptr;
}


vm_value
vm_execution::
get_value_by_name(std::u16string_view  name) const noexcept
{
  auto  sym = find_symbol(name);

    if(sym)
    {
      auto  ti = sym->type_info();

      auto   sz = ti.size();
      auto  off = m_bp+sym->offset();

        if(ti.is_integer())
        {
          int64_t  i = (sz == 1)? m_memory.load8(off)
                      :(sz == 2)? m_memory.load16(off)
                      :(sz == 4)? m_memory.load32(off)
                      :(sz == 8)? m_memory.load64(off)
                      :0
                      ;


printf("%8" PRIi64 " is loaded from address %8" PRIi64 ".\n",i,off);

          return vm_value(i);
        }

      else
        if(ti.is_integer())
        {
         uint64_t  i = (sz == 1)? m_memory.load8(off)
                      :(sz == 2)? m_memory.load16(off)
                      :(sz == 4)? m_memory.load32(off)
                      :(sz == 8)? m_memory.load64(off)
                      :0
                      ;


printf("%8" PRIi64 " is loaded from address %8" PRIu64 ".\n",i,off);

          return vm_value(i);
        }

      else
        if(ti.is_floating())
        {
          double  f = (sz == 4)? m_memory.loadf32(off)
                     :(sz == 8)? m_memory.loadf64(off)
                     :0
                     ;

          return vm_value(f);
        }
    }


printf("get_value_by_name: ");

gbstd::print(name);

printf(" is not found\n");

  return vm_value();
}


vm_value
vm_execution::
get_value_by_operand(const vm_operand&  o) const noexcept
{
    if(o.is_integer())
    {
      return vm_value(o.integer());
    }

  else
    if(o.is_unsigned_integer())
    {
      return vm_value(o.unsigned_integer());
    }

  else
    if(o.is_floating())
    {
      return vm_value(o.floating());
    }

  else
    if(o.is_identifier())
    {
      return get_value_by_name(o.string());
    }


  printf("value error\n");

  return vm_value();
}


int64_t
vm_execution::
integer(const vm_operand&  o) const
{
    if(o.is_integer())
    {
      return o.integer();
    }

  else
    if(o.is_unsigned_integer())
    {
      return o.unsigned_integer();
    }

  else
    if(o.is_floating())
    {
      return o.floating();
    }

  else
    if(o.is_identifier())
    {
      auto  v = get_value_by_name(o.string());

      return v.integer();
    }

o.print();

  return -1;
}


double
vm_execution::
floating(const vm_operand&  o) const
{
    if(o.is_integer())
    {
      return o.integer();
    }

  else
    if(o.is_unsigned_integer())
    {
      return o.unsigned_integer();
    }

  else
    if(o.is_floating())
    {
      return o.floating();
    }


  auto  v = get_value_by_name(o.string());

  return v.floating();
}


void
vm_execution::
store(std::u16string_view  dst, vm_value  v)
{
  auto  sym = find_symbol(dst);

    if(sym)
    {
      store(m_bp+sym->offset(),sym->type_info(),v);

      return;
    }


  throw 0;
}


void
vm_execution::
store(vm_address  addr, vm_type_info  ti, vm_value  v)
{
  auto  sz = ti.size();

    if(ti.is_integer())
    {
      auto  i = v.integer();

printf("%8" PRIi64 " is stored   to address %8" PRIi64 ".\n",i,addr);

           if(sz == 1){m_memory.store8( addr,i);}
      else if(sz == 2){m_memory.store16(addr,i);}
      else if(sz == 4){m_memory.store32(addr,i);}
      else if(sz == 8){m_memory.store64(addr,i);}

      return;
    }


  ti.print();
}


void
vm_execution::
push_argument(vm_type_info  ti, const vm_operand&  o) noexcept
{
    if(ti.is_integer())
    {
      auto  i = integer(o);

      m_memory.resize(m_sp+8);

           if(ti.size() == 1){m_memory.store8( m_sp,i);}
      else if(ti.size() == 2){m_memory.store16(m_sp,i);}
      else if(ti.size() == 4){m_memory.store32(m_sp,i);}
      else if(ti.size() == 8){m_memory.store64(m_sp,i);}

      m_sp += 8;
    }
}


void
vm_execution::
call(std::u16string_view  fn_name, vm_address  retval_addr, const vm_operand*  o, int  n) noexcept
{
  auto  fn = m_jobfield->find_function(fn_name);

    if(fn)
    {
        if(m_function_stack.empty())
        {
          auto  sz = fn->type_info().max_size();

            if(sz < 64)
            {
              sz = 64;
            }

                 m_bp = sz;
          m_sp = m_bp     ;

          m_memory.resize(m_bp);
          m_memory.store64( 0,0);
          m_memory.store64( 8,0);
          m_memory.store64(16,0);
          m_memory.store64(24,0);
        }


        if(o)
        {
          auto  it = fn->symbol_table().begin();

            while(n--)
            {
              push_argument(it++->type_info(),*o++);
            }
        }


      m_function_stack.emplace_back(fn);

      m_halt_flag = false;

      auto  previous_bp = m_bp       ;
                          m_bp = m_sp;

      auto  return_address = m_pc                       ;
                             m_pc = fn->entry_position();

      auto  sz = fn->stack_allocation_size();

      m_sp += sz;

      m_memory.resize(m_bp+sz);

      m_memory.store64(m_bp+ 0,previous_bp   );
      m_memory.store64(m_bp+ 8,m_sp          );
      m_memory.store64(m_bp+16,return_address);
      m_memory.store64(m_bp+24,retval_addr);

      printf("called , return value address is %" PRIi64 "\n",retval_addr);
    }
}




void
vm_execution::
reset(std::u16string_view  main_fn_name) noexcept
{
  m_pc = 0;
  m_bp = 0;
  m_sp = 0;

  m_function_stack.clear();

  call(main_fn_name,0,nullptr,0);
}




void
vm_execution::
process(const vm_return&  ret) noexcept
{
    if(m_function_stack.empty())
    {
      printf("function stack is empty.\n");

      halt();

      return;
    }


  m_sp = m_memory.load64(m_bp+ 8);
  m_pc = m_memory.load64(m_bp+16);

  auto  previous_bp = m_memory.load64(m_bp+ 0);
  auto  retval_addr = m_memory.load64(m_bp+24);

    if(ret.operand())
    {
      store(retval_addr,m_function_stack.back()->type_info(),get_value_by_operand(ret.operand()));
    }


  m_bp = previous_bp;

  m_memory.resize(m_sp);

  m_function_stack.pop_back();

    if(m_function_stack.empty())
    {
      halt();
    }
}


void
vm_execution::
process(const vm_branch&  br) noexcept
{
  auto  dst = integer(br.operand())? br.if_true()
             :                       br.if_false()
             ;

  jump(dst);
}


void
vm_execution::
jump(std::u16string_view  dst)
{
  auto&  fn = *m_function_stack.back();

    for(auto&  blk: fn.block_array())
    {
        if(blk.label() == dst)
        {
          m_pc = blk.lines().front().position();

          return;
        }
    }


  printf("jump error: destination ");

  gbstd::print(dst);

  printf(" is not found.\n");

  halt();
}


void
vm_execution::
process(const vm_call&  cal, std::u16string_view  dst) noexcept
{
  auto&  ols = cal.operand_list();

  vm_address  retval_addr = 0;

    if(dst.size())
    {
      auto  sym = find_symbol(dst);

        if(!sym)
        {
          printf("destination ");

          gbstd::print(dst);

          printf(" is not found.\n");

          halt();

          return;
        }


      retval_addr = m_bp+sym->offset();
    }


  call(cal.target(),retval_addr,ols.data(),ols.size());
}


void
vm_execution::
process(const vm_transfer&  trf) noexcept
{
  auto  v = get_value_by_name(trf.src_label());

  auto  sym = find_symbol(trf.dst_label());

    if(sym)
    {
      store(m_bp+sym->offset(),trf.dst_type_info(),v);
    }
}


vm_value
vm_execution::
process(const vm_phi_element_list&  phels) noexcept
{
  return vm_value();
}


vm_value
vm_execution::
process(const vm_operation&  op) noexcept
{
  int64_t  l =                     integer(op.left_operand() );
  int64_t  r = op.right_operand()? integer(op.right_operand()):0;

    switch(op.opcode())
    {
  case(vm_opcode::add):
      return vm_value(l+r);
      break;
  case(vm_opcode::sub):
      return vm_value(l-r);
      break;
  case(vm_opcode::mul):
      return vm_value(l*r);
      break;
  case(vm_opcode::div):
      return vm_value(l/r);
      break;
  case(vm_opcode::rem):
      return vm_value(l%r);
      break;
  case(vm_opcode::shl):
      return vm_value(l<<r);
      break;
  case(vm_opcode::shr):
      return vm_value(l>>r);
      break;
  case(vm_opcode::and_):
      return vm_value(l&r);
      break;
  case(vm_opcode::or_):
      return vm_value(l|r);
      break;
  case(vm_opcode::xor_):
      return vm_value(l^r);
      break;
  case(vm_opcode::eq):
      return vm_value(l == r? 1:0);
      break;
  case(vm_opcode::neq):
      return vm_value(l != r? 1:0);
      break;
  case(vm_opcode::lt):
      return vm_value(l < r? 1:0);
      break;
  case(vm_opcode::lteq):
      return vm_value(l <= r? 1:0);
      break;
  case(vm_opcode::gt):
      return vm_value(l > r? 1:0);
      break;
  case(vm_opcode::gteq):
      return vm_value(l >= r? 1:0);
      break;
  case(vm_opcode::logi_and):
      return vm_value(l && r? 1:0);
      break;
  case(vm_opcode::logi_or):
      return vm_value(l || r? 1:0);
      break;
  case(vm_opcode::neg):
      return vm_value(-l);
      break;
  case(vm_opcode::logi_not):
      return vm_value(l? 0:1);
      break;
  case(vm_opcode::not_):
      return vm_value(~l);
      break;
    }


  return vm_value();
}


void
vm_execution::
process(const vm_register&  reg) noexcept
{
    if(reg.is_call())
    {
      process(reg.call(),reg.label());

      return;
    }


  auto  sym = find_symbol(reg.label());

    if(sym)
    {
      auto  v = reg.is_phi()?       process(reg.phi_element_list())
               :reg.is_operation()? process(reg.operation())
               :vm_value()
               ;

      store(m_bp+sym->offset(),sym->type_info(),v);
    }
}


void
vm_execution::
process(const vm_line&  ln) noexcept
{
       if(ln.is_return()  ){process(ln.get_return());}
  else if(ln.is_branch()  ){process(ln.get_branch());}
  else if(ln.is_jump()    ){jump(ln.get_jump().label());}
  else if(ln.is_call()    ){process(ln.get_call(),u"");}
  else if(ln.is_transfer()){process(ln.get_transfer());}
  else if(ln.is_register()){process(ln.get_register());}
}


void
vm_execution::
step() noexcept
{
print();

  auto  pos = m_pc++;

    if(pos < m_lines.size())
    {
      auto&  ln = *m_lines[pos];

      process(ln);

      return;
    }


  halt();
}


const vm_value&
vm_execution::
run() noexcept
{
    while(!m_halt_flag)
    {
      step();
    }


  printf("the value that was returned by main is %" PRIi64 ".\n",m_memory.load64(0));

  return m_return_value;
}


void
vm_execution::
print() const noexcept
{
  printf("[pc = %4" PRIi64 ","
         " bp = %4" PRIi64 ","
         " sp = %4" PRIi64 "] "
         ,m_pc,m_bp,m_sp);


    if(m_function_stack.size())
    {
      gbstd::print(m_function_stack.back()->name());
    }


  printf("\n");
}




}




