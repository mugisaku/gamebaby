#include"libgbstd/vm.hpp"




namespace gbstd{




symbol&
symbol_table::
push(const declaration&  decl, const function*  fn) noexcept
{
  constexpr auto  align = sizeof(int64_t);

  uint32_t  addr = m_end_address+(align-1)/align*align;

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


bool
symbol_table::
reallocate() noexcept
{
  uint32_t  offset = 0;

  return true;
}




}




