#include"libww/ww_system.hpp"




namespace ww{




namespace{
std::vector<void*>  g_spilling_text_stack;
}


system::
system() noexcept
{
  m_string_set.load_from_file("../__test.txtset");
}




const gbstd::string_set::section*
system::
get_text_section(std::string_view  sv) const noexcept
{
  return m_string_set.get_section(sv);
}


spilling_text&
system::
create_spilling_text() noexcept
{
    if(g_spilling_text_stack.empty())
    {
      g_spilling_text_stack.emplace_back(new uint8_t[sizeof(spilling_text)]);
    }


  auto  ptr = static_cast<spilling_text*>(g_spilling_text_stack.back());

  g_spilling_text_stack.pop_back();

  new(ptr) spilling_text(*this);

  ++m_spilling_text_counter;

  return *ptr;
}


system&
system::
collect_spilling_text(spilling_text&  ref) noexcept
{
  gbstd::destruct(ref);

  g_spilling_text_stack.emplace_back(&ref);

  --m_spilling_text_counter;

  return *this;
}




}




