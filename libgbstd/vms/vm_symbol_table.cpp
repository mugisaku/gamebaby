#include"libgbstd/vm.hpp"




namespace gbstd{




symbol&
symbol_table::
push(const declaration&  decl, const function*  fn) noexcept
{
  auto  align = decl.get_type_info()->get_align();

  uint32_t  addr = (m_end_address+(align-1))/align*align;

  m_content.emplace_back(declaration(decl),fn,addr);


  auto  ti = decl.get_type_info();

    if(ti)
    {
      m_end_address = addr+ti->get_size();
    }


  return m_content.back();
}


void
symbol_table::
pop() noexcept
{
  m_content.pop_back();

    if(m_content.size())
    {
      auto&  last = m_content.back();

      m_end_address = last.get_address()+last.get_size();
    }

  else
    {
      m_end_address = 0;
    }
}


symbol*
symbol_table::
find(std::string_view  name) noexcept
{
  auto  it     = m_content.rbegin();
  auto  it_end = m_content.rend();

    while(it != it_end)
    {
      auto&  sym = *it++;

        if(sym.get_name() == name)
        {
          return &sym;
        }
    }


  return nullptr;
}


const symbol*
symbol_table::
find(std::string_view  name) const noexcept
{
  auto  it     = m_content.rbegin();
  auto  it_end = m_content.rend();

    while(it != it_end)
    {
      auto&  sym = *it++;

        if(sym.get_name() == name)
        {
          return &sym;
        }
    }


  return nullptr;
}


hot_object
symbol_table::
make_object(std::string_view  name, const memory&  mem) const noexcept
{
  auto  sym = find(name);

    if(sym)
    {
      return hot_object(mem,sym->get_type_info()->form_reference_type(type_infos::pointer_size),sym->get_address());
    }


  return {};
}


bool
symbol_table::
reallocate() noexcept
{
  uint32_t  offset = 0;

  return true;
}


void
symbol_table::
print(const memory&  mem) const noexcept
{
    for(auto&  sym: m_content)
    {
      auto  ti = sym.get_type_info();

      auto  addr = sym.get_address();

      printf("%s %s(%6d)",ti? ti->get_id().data():"no type",sym.get_name().data(),addr);

        if(ti)
        {
          printf(":");

          hot_object  ho(mem,*ti,addr);

          tepid_object  to(ho);

          to.print();
        }


      printf("\n");
    }
}




}




