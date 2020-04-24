#include"libgbstd/vm.hpp"




namespace gbstd{




namespace type_infos{
const type_info  null{};
const type_info  undefined{typesystem::undefined_type_info(),"undefined"};
const type_info  null_pointer{typesystem::null_pointer_type_info(),"nullptr_t"};
const type_info         void_{typesystem::void_type_info(),"void"};
const type_info            i8{typesystem::integer_type_info(1),"i8_t"};
const type_info           i16{typesystem::integer_type_info(2),"i16_t"};
const type_info           i32{typesystem::integer_type_info(4),"i32_t"};
const type_info           i64{typesystem::integer_type_info(8),"i64_t"};
const type_info           int_{typesystem::integer_type_info(8),"int"};
const type_info           f32{typesystem::fpn_type_info(4),"f32_t"};
const type_info           f64{typesystem::fpn_type_info(8),"f64_t"};
const type_info          boolean{typesystem::boolean_type_info(boolean_size),"bool"};
const type_info  generic_pointer{typesystem::generic_pointer_type_info(pointer_size),"geneptr_t"};
}




constexpr int  g_mem_size = 1024*1024*16;


context::
context() noexcept:
m_memory(g_mem_size)
{
}


context::
context(const global_space&  gsp) noexcept:
m_memory(g_mem_size),
m_global_space(&gsp)
{
}




void
context::
call_function(std::string_view  fn_name, address_t  return_value_address) noexcept
{
/*
  auto  fn = m_block.find_function(fn_name);

    if(fn)
    {
      auto  retti = fn->get_return_type_info();

        if(!return_value_address)
        {
          m_bp = get_aligned_address(retti.get_size());
        }


      auto  sz = fn->get_block_statement()->get_symbol_table().get_end_address();

      push_stack_frame(0,return_value_address,sz);

      m_call_stack.emplace_back(*fn);
    }
*/
}




void
context::
print_stack_frame() const noexcept
{
  constexpr auto  address_size = sizeof(address_t);

    if(m_bp >= (address_size*4))
    {
      auto  return_value_address = *m_memory.get_pointer<address_t>(m_bp-(address_size  ));
      auto  return_address       = *m_memory.get_pointer<address_t>(m_bp-(address_size*2));
      auto  previous_bp          = *m_memory.get_pointer<address_t>(m_bp-(address_size*3));
      auto  code                 = *m_memory.get_pointer<address_t>(m_bp-(address_size*4));

      printf("stack_frame:{\n  code:%X,\n  return_address:%d,\n  return_value_address:%d,\n  previous_bp:%d\n}\n",
        code,
        return_address,
        return_value_address,
        previous_bp
      )
      ;
    }
}


void
context::
push_stack_frame(address_t  return_address, address_t  return_value_address, size_t  size) noexcept
{
  constexpr auto  address_size = sizeof(address_t);

  *m_memory.get_pointer<address_t>(m_sp                 ) = 0xDEADC0DE;
  *m_memory.get_pointer<address_t>(m_sp+(address_size  )) = m_bp;
  *m_memory.get_pointer<address_t>(m_sp+(address_size*2)) = return_address;
  *m_memory.get_pointer<address_t>(m_sp+(address_size*3)) = return_value_address;

  m_bp = get_aligned_address(m_sp+(address_size*4));
  m_sp = get_aligned_address(m_bp+size            );
}


void
context::
pop_stack_frame() noexcept
{
  constexpr auto  address_size = sizeof(address_t);

  auto  return_value_address = *m_memory.get_pointer<address_t>(m_bp-(address_size  ));
  auto  return_address       = *m_memory.get_pointer<address_t>(m_bp-(address_size*2));
  auto  previous_bp          = *m_memory.get_pointer<address_t>(m_bp-(address_size*3));
  auto  code                 = *m_memory.get_pointer<address_t>(m_bp-(address_size*4));

  m_sp = m_bp-(address_size*4);
  m_bp = previous_bp;


  printf("stack_frame:{\n  bp:%d\n  sp:%d\n}\n",
    m_bp,
    m_sp
  )
  ;
}


void
context::
print() const noexcept
{
//  m_block.print();
}




void
context::
reset() noexcept
{
  m_call_stack.clear();

  m_bp = 0;
  m_sp = 0;
}


bool
context::
step() noexcept
{
START:
    if(m_call_stack.size())
    {
      auto&  bk = m_call_stack.back();

        if(bk)
        {
          bk(*this);

          return true;
        }

      else
        {
          m_call_stack.pop_back();

          goto START;
        }
    }


  return false;
}


void
context::
run() noexcept
{
    while(step())
    {
    }
}




}




