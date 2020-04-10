#include"libgbstd/vm.hpp"




namespace gbstd{




symbol&
symbol_table::
push(const type_info&  ti, std::string_view  name, int  attr, const function*  fn) noexcept
{
  m_content.emplace_back(std::make_unique<symbol>(ti,name,attr,fn));

  auto&  bk = m_content.back();

  auto  sz = ti.get_size();

    if(m_direction != directions::relative_backward)
    {
      bk->set_address(m_end_address);

      m_end_address = get_aligned_address(m_end_address+sz);
    }

  else
    {
      constexpr auto  align = sizeof(int64_t);

      m_end_address = (m_end_address-sz-(align-1))/align*align;

      bk->set_address(m_end_address);
    }


  return *bk;
}


symbol&
symbol_table::
push(const symbol&  sym) noexcept
{
  return push(sym.get_type_info(),sym.get_name(),sym.get_attribute(),sym.get_function());
}


symbol_table&
symbol_table::
push(const parameter_list&  ls) noexcept
{
    for(auto&  para: ls)
    {
      push(para.get_type_info(),para.get_name(),0,nullptr);
    }


  return *this;
}




const std::unique_ptr<symbol>&
symbol_table::
find(std::string_view  name) noexcept
{
  static const std::unique_ptr<symbol>  null;

    for(auto&  ptr: m_content)
    {
        if(ptr->get_name() == name)
        {
          return ptr;
        }
    }


  return null;
}


const symbol*
symbol_table::
find(std::string_view  name) const noexcept
{
  auto  it     = m_content.rbegin();
  auto  it_end = m_content.rend();

    while(it != it_end)
    {
      auto&  sym = **(it++);

        if(sym.get_name() == name)
        {
          return &sym;
        }
    }


  return nullptr;
}


void
symbol_table::
print(const memory*  mem) const noexcept
{
    for(auto&  sym: m_content)
    {
      auto&  ti = sym->get_type_info();

      auto  addr = sym->get_address();

      printf("%s %s(address: %6d)",ti.get_id().data(),sym->get_name().data(),addr);

        if(mem)
        {
          printf(":");

          hot_object  ho(*mem,ti,addr);

          tepid_object  to(ho);

          to.print();
        }


      printf("\n");
    }
}




}




