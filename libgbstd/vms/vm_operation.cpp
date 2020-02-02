#include"libgbstd/vm.hpp"




namespace gbstd{




operation&
operation::
reset(unary_opcodes  unop, operand&&  o) noexcept
{
  m_operand_list.resize(1);

  m_operand_list.emplace_back(std::move(o));

  m_opcode = static_cast<int>(unop);

  m_kind = kind::unary;

  return *this;
}


operation&
operation::
reset(binary_opcodes  binop, operand&&  l, operand&&  r) noexcept
{
  m_operand_list.resize(1);

  m_operand_list.emplace_back(std::move(l));
  m_operand_list.emplace_back(std::move(r));

  m_opcode = static_cast<int>(binop);

  m_kind = kind::binary;

  return *this;
}


operation&
operation::
reset(std::string_view  fn_name, std::vector<operand>&&  args) noexcept
{
  m_operand_list.resize(1);

  m_operand_list.emplace_back(fn_name);

    for(auto&  o: args)
    {
      m_operand_list.emplace_back(std::move(o));
    }


  args.clear();

  m_kind = kind::call;

  return *this;
}


operation&
operation::
reset(std::initializer_list<std::string_view>  names) noexcept
{
  m_operand_list.resize(1);

    for(auto  sv: names)
    {
      m_operand_list.emplace_back(sv);
    }


  m_kind = kind::seek;

  return *this;
}


void
operation::
print() const noexcept
{
/*
  printf("  %%%s = ",m_label.data());
    switch(m_kind)
    {
  case(kind::unary ): m_data.un.print(); break;
  case(kind::binary): m_data.bin.print();break;
  case(kind::call  ): m_data.cal.print();break;
    }
*/
}




}




