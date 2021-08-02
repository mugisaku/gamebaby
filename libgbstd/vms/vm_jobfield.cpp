#include"libgbstd/vm.hpp"




namespace gbstd{




vm_jobfield&
vm_jobfield::
assign(const syntax_branch&  br) noexcept
{
  m_symbol_table.clear();
  m_function_table.clear();

    for(auto&  e: br)
    {
        if(e.is_branch(u"element"))
        {
            for(auto&  ee: e.branch())
            {
                if(ee.is_branch(u"function_declaration"))
                {
                  m_function_table.emplace_back(ee.branch());
                }
            }
        }
    }


  return *this;
}




const vm_function*
vm_jobfield::
find_function(std::u16string_view  name) const noexcept
{
    for(auto&  fn: m_function_table)
    {
        if(fn.name() == name)
        {
          return &fn;
        }
    }


  return nullptr;
}


const vm_symbol*
vm_jobfield::
find_symbol(std::u16string_view  name) const noexcept
{
    for(auto&  sym: m_symbol_table)
    {
        if(sym.name() == name)
        {
          return &sym;
        }
    }


  return nullptr;
}


void
vm_jobfield::
finalize() noexcept
{
  int  pos = 0;

    for(auto&  fn: m_function_table)
    {
      pos = fn.finalize(pos);
    }
}


vm_value
vm_jobfield::
run(std::u16string_view  main_fn_name) const noexcept
{
  auto  fn = find_function(main_fn_name);

    if(fn)
    {
      vm_execution  exe(*this);

      return vm_value();
    }


  return vm_value();
}


void
vm_jobfield::
print() const noexcept
{
    for(auto&  fn: m_function_table)
    {
      fn.print();

      printf("\n");
    }
}




}




