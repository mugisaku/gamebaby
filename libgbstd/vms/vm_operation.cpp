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


  m_kind = kind::call;

  return *this;
}


operation&
operation::
reset(std::vector<std::string_view>&&  names) noexcept
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
print(const context*  ctx, const function*  fn) const noexcept
{
  static const char*  unary_names[] =
  {
    "ld",
    "bit_not",
    "logical_not",
    "get_size",
    "get_address",

  };


  static const char*  binary_names[] =
  {
    "add",
    "sub",
    "mul",
    "div",
    "rem",
    "shl",
    "shr",
    "eq",
    "neq",
    "lt",
    "lteq",
    "gt",
    "gteq",
    "logical_and",
    "logical_or",
    "bit_and",
    "bit_or",
    "bit_xor",
  };


  int  i = 0;

  m_operand_list[i++].print(ctx,fn);

    switch(m_kind)
    {
  case(kind::unary ):
      printf(" %s",unary_names[m_opcode]);
      break;
  case(kind::binary):
      printf(" %s",binary_names[m_opcode]);
      break;
  case(kind::call):
      printf(" call");
      break;
  case(kind::seek):
      printf(" seek");
      break;
    }


    while(i < m_operand_list.size())
    {
      printf(" ");

      m_operand_list[i++].print(ctx,fn);
    }
}




}




