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
value(std::u16string_view  name) noexcept
{
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
    if(o.is_floating())
    {
      return o.floating();
    }


  auto  sym = find_symbol(o.string());

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

          return i;
        }

      else
        if(ti.is_floating())
        {
          return (sz == 4)? m_memory.loadf32(off)
                :(sz == 8)? m_memory.loadf64(off)
                :0
                ;
        }
    }


gbstd::print(o.string());

printf(" is not found\n");
report;
  throw 0;
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
    if(o.is_floating())
    {
      return o.floating();
    }


  auto  sym = find_symbol(o.string());

    if(sym)
    {
      auto  ti = sym->type_info();

      auto   sz = ti.size();
      auto  off = sym->offset();

        if(ti.is_integer())
        {
          return (sz == 1)? m_memory.load8(off)
                :(sz == 2)? m_memory.load16(off)
                :(sz == 4)? m_memory.load32(off)
                :(sz == 8)? m_memory.load64(off)
                :0
                ;
        }

      else
        if(ti.is_floating())
        {
          return (sz == 4)? m_memory.loadf32(off)
                :(sz == 8)? m_memory.loadf64(off)
                :0
                ;
        }
    }


  throw 0;
}


void
vm_execution::
store(std::u16string_view  dst, int64_t  v)
{
  auto  sym = find_symbol(dst);

    if(sym)
    {
      vm_operand  o(static_cast<uint64_t>(v));

      store(m_bp+sym->offset(),sym->type_info(),o);

      return;
    }


  throw 0;
}


void
vm_execution::
store(vm_address  addr, vm_type_info  ti, const vm_operand&  o)
{
  auto  sz = ti.size();

    if(ti.is_integer())
    {
      auto  i = integer(o);

printf("%8" PRIi64 " is stored   to address %8" PRIi64 ".\n",i,addr);

      m_memory.store64(addr,i);

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
call(const vm_operand_list&  ls, std::u16string_view  dst) noexcept
{
  auto  p = ls.data();
  auto  n = ls.size();

  auto  fn_name = p++->string();

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
        }


      retval_addr = m_bp+sym->offset();
    }


  call(fn_name,retval_addr,p,n-1);
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

      m_memory.store64(m_bp+0 ,previous_bp   );
      m_memory.store64(m_bp+8 ,m_sp          );
      m_memory.store64(m_bp+16,return_address);
      m_memory.store64(m_bp+24,retval_addr   );
    }
}


void
vm_execution::
do_return(const vm_operand*  o) noexcept
{
    if(m_function_stack.empty())
    {
      printf("function stack is empty.\n");

      return;
    }


  m_pc = m_memory.load64(m_bp+16);

  auto  previous_bp = m_memory.load64(m_bp+ 0);
  auto  retval_addr = m_memory.load64(m_bp+24);

    if(o)
    {
      store(retval_addr,m_function_stack.back()->type_info(),*o);
    }


  m_sp = m_memory.load64(previous_bp+8);

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
process(const vm_line&  ln) noexcept
{
    switch(ln.opcode())
    {
  case(vm_opcode::add):
      store(ln.destination(),integer(ln.left_source())+integer(ln.right_source()));
      break;
  case(vm_opcode::sub):
      store(ln.destination(),integer(ln.left_source())-integer(ln.right_source()));
      break;
  case(vm_opcode::mul):
      store(ln.destination(),integer(ln.left_source())*integer(ln.right_source()));
      break;
  case(vm_opcode::div):
      store(ln.destination(),integer(ln.left_source())/integer(ln.right_source()));
      break;
  case(vm_opcode::rem):
      store(ln.destination(),integer(ln.left_source())%integer(ln.right_source()));
      break;
  case(vm_opcode::shl):
      store(ln.destination(),integer(ln.left_source())<<integer(ln.right_source()));
      break;
  case(vm_opcode::shr):
      store(ln.destination(),integer(ln.left_source())>>integer(ln.right_source()));
      break;
  case(vm_opcode::and_):
      store(ln.destination(),integer(ln.left_source())&integer(ln.right_source()));
      break;
  case(vm_opcode::or_):
      store(ln.destination(),integer(ln.left_source())|integer(ln.right_source()));
      break;
  case(vm_opcode::xor_):
      store(ln.destination(),integer(ln.left_source())^integer(ln.right_source()));
      break;
  case(vm_opcode::eq):
      store(ln.destination(),integer(ln.left_source()) == integer(ln.right_source())? 1:0);
      break;
  case(vm_opcode::neq):
      store(ln.destination(),integer(ln.left_source()) != integer(ln.right_source())? 1:0);
      break;
  case(vm_opcode::lt):
      store(ln.destination(),integer(ln.left_source()) < integer(ln.right_source())? 1:0);
      break;
  case(vm_opcode::lteq):
      store(ln.destination(),integer(ln.left_source()) <= integer(ln.right_source())? 1:0);
      break;
  case(vm_opcode::gt):
      store(ln.destination(),integer(ln.left_source()) > integer(ln.right_source())? 1:0);
      break;
  case(vm_opcode::gteq):
      store(ln.destination(),integer(ln.left_source()) >= integer(ln.right_source())? 1:0);
      break;
  case(vm_opcode::logi_and):
      store(ln.destination(),integer(ln.left_source()) && integer(ln.right_source())? 1:0);
      break;
  case(vm_opcode::logi_or):
      store(ln.destination(),integer(ln.left_source()) || integer(ln.right_source())? 1:0);
      break;
  case(vm_opcode::neg):
      store(ln.destination(),-integer(ln.left_source()));
      break;
  case(vm_opcode::logi_not):
      store(ln.destination(),integer(ln.left_source())? 0:1);
      break;
  case(vm_opcode::not_):
      store(ln.destination(),~integer(ln.left_source()));
      break;
  case(vm_opcode::ld):
      store(ln.destination(),integer(ln.left_source()));
      break;
  case(vm_opcode::cal):
      call(ln.operand_list(),ln.destination());
      break;
  case(vm_opcode::ret):
      do_return(ln.operand_list().size()? ln.operand_list().data():nullptr);
      break;
    }
}


void
vm_execution::
step() noexcept
{
  auto  pos = m_pc++;

print();
    if(pos < m_lines.size())
    {
      auto&  ln = *m_lines[pos];

      process(ln);

      return;
    }


  m_halt_flag = true;
}


const vm_value&
vm_execution::
run() noexcept
{
    while(!m_halt_flag)
    {
      step();
    }


  printf("the value that was returned by main is %" PRIi64 ".\n",m_memory.load64(96));

  return m_return_value;
}


void
vm_execution::
print() const noexcept
{
  printf("[pc = %4" PRIi64 ","
         " bp = %4" PRIi64 ","
         " sp = %4" PRIi64 "] ",
         m_pc,m_bp,m_sp);


    if(m_function_stack.size())
    {
      gbstd::print(m_function_stack.back()->name());
    }


  printf("\n");
}




}




