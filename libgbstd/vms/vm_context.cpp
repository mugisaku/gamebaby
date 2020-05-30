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
call_function(std::string_view  fn_name) noexcept
{
  auto  fn = m_global_space->find_function(fn_name);

    if(fn)
    {
      m_call_stack.emplace_back(m_bp,m_sp,*fn);
    }
}




void
context::
print() const noexcept
{
//  m_block.print();
}




void
context::
reset(std::string_view  fn_name) noexcept
{
  m_bp = 0;
  m_sp = 0;

  m_call_stack.clear();

  auto&  mi_tbl = m_global_space->get_memo_info_table();

    if(mi_tbl.size())
    {
      auto&  mi = *mi_tbl.back();

      m_bp = get_aligned_address(mi.get_address()+mi.get_type_info().get_size());

      m_sp = m_bp;
    }



  call_function(fn_name);
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




