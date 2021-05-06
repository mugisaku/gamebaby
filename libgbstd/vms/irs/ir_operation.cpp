#include"libgbstd/vms/ir.hpp"




namespace gbstd{




ir_operation&
ir_operation::
assign(const ir_operation&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::unary  ): new(&m_data) ir_unary_operation(rhs.m_data.un);break;
      case(kind::binary ): new(&m_data) ir_binary_operation(rhs.m_data.bin);break;
      case(kind::define ): new(&m_data) ir_define_operation(rhs.m_data.def);break;
      case(kind::load   ): new(&m_data) ir_load_operation(rhs.m_data.ld);break;
      case(kind::address): new(&m_data) ir_address_operation(rhs.m_data.addr);break;
      case(kind::call   ): new(&m_data) ir_call_operation(rhs.m_data.cal);break;
      case(kind::phi    ): new(&m_data) ir_phi_operation(rhs.m_data.phi);break;
        }
    }


  return *this;
}


ir_operation&
ir_operation::
assign(ir_operation&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::unary  ): new(&m_data) ir_unary_operation(std::move(rhs.m_data.un));break;
      case(kind::binary ): new(&m_data) ir_binary_operation(std::move(rhs.m_data.bin));break;
      case(kind::define ): new(&m_data) ir_define_operation(std::move(rhs.m_data.def));break;
      case(kind::load   ): new(&m_data) ir_load_operation(std::move(rhs.m_data.ld));break;
      case(kind::address): new(&m_data) ir_address_operation(std::move(rhs.m_data.addr));break;
      case(kind::call   ): new(&m_data) ir_call_operation(std::move(rhs.m_data.cal));break;
      case(kind::phi    ): new(&m_data) ir_phi_operation(std::move(rhs.m_data.phi));break;
        }
    }


  return *this;
}


ir_operation&
ir_operation::
assign(ir_unary_operation&&  un) noexcept
{
  clear();

  m_kind = kind::unary;

  new(&m_data) ir_unary_operation(std::move(un));

  return *this;
}


ir_operation&
ir_operation::
assign(ir_binary_operation&&  bin) noexcept
{
  clear();

  m_kind = kind::binary;

  new(&m_data) ir_binary_operation(std::move(bin));

  return *this;
}


ir_operation&
ir_operation::
assign(ir_define_operation&&  def) noexcept
{
  clear();

  m_kind = kind::define;

  new(&m_data) ir_define_operation(std::move(def));

  return *this;
}


ir_operation&
ir_operation::
assign(ir_load_operation&&  ld) noexcept
{
  clear();

  m_kind = kind::load;

  new(&m_data) ir_load_operation(std::move(ld));

  return *this;
}


ir_operation&
ir_operation::
assign(ir_address_operation&&  addr) noexcept
{
  clear();

  m_kind = kind::address;

  new(&m_data) ir_address_operation(std::move(addr));

  return *this;
}


ir_operation&
ir_operation::
assign(ir_call_operation&&  cal) noexcept
{
  clear();

  m_kind = kind::call;

  new(&m_data) ir_call_operation(std::move(cal));

  return *this;
}


ir_operation&
ir_operation::
assign(ir_phi_operation&&  phi) noexcept
{
  clear();

  m_kind = kind::phi;

  new(&m_data) ir_phi_operation(std::move(phi));

  return *this;
}


void
ir_operation::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::unary  ): std::destroy_at(&m_data.un);break;
  case(kind::binary ): std::destroy_at(&m_data.bin);break;
  case(kind::define ): std::destroy_at(&m_data.def);break;
  case(kind::load   ): std::destroy_at(&m_data.ld);break;
  case(kind::address): std::destroy_at(&m_data.addr);break;
  case(kind::call   ): std::destroy_at(&m_data.cal);break;
  case(kind::phi    ): std::destroy_at(&m_data.phi);break;
    }


  m_kind = kind::null;
}




void
ir_operation::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::unary):
      gbstd::print(m_data.un.first_opcode());
      printf(" ");
      gbstd::print(m_data.un.second_opcode());
      printf(" ");
      gbstd::print(m_data.un.operand());
      break;
  case(kind::binary):
      gbstd::print(m_data.bin.first_opcode());
      printf(" ");
      gbstd::print(m_data.bin.second_opcode());
      printf(" ");
      gbstd::print(m_data.bin.first_operand());
      printf(" ");
      gbstd::print(m_data.bin.second_operand());
      break;
  case(kind::define):
      printf("def ");

           if(m_data.def.is_integer() ){printf("%" PRIu64,m_data.def.integer());}
      else if(m_data.def.is_floating()){printf("%f",m_data.def.floating());}
      break;
  case(kind::load):
      printf("ld ");

      m_data.ld.type_info().print();

      gbstd::print(m_data.ld.address_operand());
      break;
  case(kind::address):
      printf("addr ");

      gbstd::print(m_data.addr.identifier());
      break;
  case(kind::call):
      printf("cal ");

      gbstd::print(m_data.cal.identifier());

        for(auto&  a: m_data.cal.argument_list())
        {
          printf(" ");

          gbstd::print(a);
        }
      break;
  case(kind::phi):
      printf("phi ");

        for(auto&  e: m_data.phi.element_list())
        {
          printf(" ");

          gbstd::print(e.label());

          printf(" ");

          gbstd::print(e.operand());
        }
      break;
    }
}




}




