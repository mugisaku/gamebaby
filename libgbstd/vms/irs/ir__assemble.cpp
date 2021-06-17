#include"libgbstd/vms/ir.hpp"




namespace gbstd{




class
as_line
{
  int  m_position;

  ir_opcode  m_opcode;

  std::u16string_view  m_n_label;

  std::u16string_view  m_rs1_label;
  std::u16string_view  m_rs2_label;
  std::u16string_view  m_rs3_label;

  uint32_t  m_imm_value;

public:
  as_line(int  pos) noexcept: m_position(pos){}

  int  position() const noexcept{return m_position;}

  void  set_opcode(ir_opcode  opco) noexcept{m_opcode = opco;}
  void  set_n_label(std::u16string_view  lb) noexcept{m_n_label = lb;}
  void  set_rs1_label(std::u16string_view  lb) noexcept{m_rs1_label = lb;}
  void  set_rs2_label(std::u16string_view  lb) noexcept{m_rs2_label = lb;}
  void  set_rs3_label(std::u16string_view  lb) noexcept{m_rs3_label = lb;}
  void  set_imm_value(uint32_t  v) noexcept{m_imm_value = v;}

  ir_opcode  opcode() const noexcept{return m_opcode;}

  std::u16string_view  n_label() const noexcept{return m_n_label;}

  std::u16string_view  rs1_label() const noexcept{return m_rs1_label;}
  std::u16string_view  rs2_label() const noexcept{return m_rs2_label;}
  std::u16string_view  rs3_label() const noexcept{return m_rs3_label;}

  uint32_t  imm_value() const noexcept{return m_imm_value;}

};


class
as_block
{
  int  m_position;

  std::u16string_view  m_label;

  std::vector<as_line>  m_lines;

public:
  as_block(int  pos, std::u16string_view  lb) noexcept: m_position(pos), m_label(lb){}

  std::u16string_view  label() const noexcept{return m_label;}

  int       position() const noexcept{return m_position;}
  int  next_position() const noexcept{return m_position+m_lines.size();}

        std::vector<as_line>&  lines()       noexcept{return m_lines;}
  const std::vector<as_line>&  lines() const noexcept{return m_lines;}

};




class
symbol
{
  std::u16string  m_name;

  ir_type_info  m_type_info;

  ir_register_spec  m_register_spec;

public:
  symbol(std::u16string_view  name, ir_type_info  ti, ir_register_spec  rs) noexcept:
  m_name(name), m_type_info(ti), m_register_spec(rs){}

  const std::u16string&  name() const noexcept{return m_name;}

  ir_type_info  type_info() const noexcept{return m_type_info;}

  ir_register_spec  register_spec() const noexcept{return m_register_spec;}

};


struct
context
{
  std::vector<as_block>  m_block_array;

  std::vector<symbol>  m_global_symbol_table;
  std::vector<symbol>  m_local_symbol_table;

  const symbol*  find_symbol(std::u16string_view  name);

  void  append(const ir_function&  fn);

  as_line&  last_line() noexcept{return m_block_array.back().lines().back();}

  as_line&    add_line() noexcept;
  as_block&  add_block(std::u16string_view  lb) noexcept;

  void  push(const ir_return_statement&  ret);
  void  push(const ir_branch_statement&  br);
  void  push(const ir_store_statement&  st);

  void  push(const ir_unary_operation&  un);
  void  push(const ir_binary_operation&  bin);
  void  push(const ir_define_operation&  def);
  void  push(const ir_address_operation&  addr);
  void  push(const ir_call_operation&  cal);
  void  push(const ir_phi_operation&  phi);

};




as_line&
context::
add_line() noexcept
{
    if(m_block_array.empty())
    {
      add_block(u"");
    }


  auto&  blk = m_block_array.back();

  auto  pos = blk.next_position();

  return blk.lines().emplace_back(pos);
}


as_block&
context::
add_block(std::u16string_view  lb) noexcept
{
  auto  pos = m_block_array.empty()? 0:m_block_array.back().next_position();

  return m_block_array.emplace_back(pos,lb);
}


const symbol*
context::
find_symbol(std::u16string_view  name)
{
    for(auto&  sym: m_local_symbol_table)
    {
        if(sym.name() == name)
        {
          return &sym;
        }
    }


    for(auto&  sym: m_global_symbol_table)
    {
        if(sym.name() == name)
        {
          return &sym;
        }
    }


  return nullptr;
}


void
context::
append(const ir_function&  fn)
{
  m_local_symbol_table.clear();

  size_t  stack_size = 0;

  m_local_symbol_table.emplace_back(u"",ir_type_info('i',8),ir_register_spec('b',0));//previous bp
  m_local_symbol_table.emplace_back(u"",ir_type_info('i',8),ir_register_spec('b',0));//return address
  m_local_symbol_table.emplace_back(u"",ir_type_info('i',8),ir_register_spec('b',0));//return value address

    for(auto&  para: fn.parameter_list())
    {
      stack_size += para.type_info().max_size();

      m_local_symbol_table.emplace_back(para.name(),para.type_info(),ir_register_spec('b',0));
    }


    for(auto&  st: fn.statement_list())
    {
           if(st.is_return()  ){push(st.get_return());}
      else if(st.is_label()   ){add_block(st.get_label().identifier());}
      else if(st.is_branch()  ){push(st.get_branch());}
      else if(st.is_store()   ){push(st.get_store());}
      else if(st.is_register())
        {
          auto&  reg = st.get_register();

          auto&  ln = add_line();

          ln.set_rs3_label(reg.identifier());

          auto&  op = reg.operation();

               if(op.is_unary()  ){push(op.unary());}
          else if(op.is_binary() ){push(op.binary());}
          else if(op.is_define() ){push(op.define());}
          else if(op.is_address()){push(op.address());}
          else if(op.is_call()   ){push(op.call());}
          else if(op.is_phi()    ){}
        }
    }
}




void
context::
push(const ir_return_statement&  ret)
{
    if(ret.operand().size())
    {
      auto  sym = find_symbol(ret.operand());

    }


//  m_instruction_array.emplace_back(ir_opcode::ret);
}


void
context::
push(const ir_branch_statement&  br)
{
}


void
context::
push(const ir_store_statement&  st)
{
}


void
context::
push(const ir_unary_operation&  un)
{
  last_line().set_rs1_label(un.operand());

    if(un.first_opcode() == u"logi")
    {
        if(un.second_opcode() == u"not")
        {
          last_line().set_opcode(ir_opcode::logi_not);
        }
    }

  else
    if(un.first_opcode() == u"bitw")
    {
        if(un.second_opcode() == u"not")
        {
          last_line().set_opcode(ir_opcode::not_);
        }
    }

  else
    if(un.first_opcode() == u"neg")
    {
      last_line().set_opcode(ir_opcode::neg);
    }
}


void
context::
push(const ir_binary_operation&  bin)
{
  last_line().set_rs1_label(bin.first_operand());
  last_line().set_rs2_label(bin.second_operand());

    if(bin.first_opcode() == u"ari")
    {
           if(bin.second_opcode() == u"add"){last_line().set_opcode(ir_opcode::add);}
      else if(bin.second_opcode() == u"sub"){last_line().set_opcode(ir_opcode::sub);}
      else if(bin.second_opcode() == u"mul"){last_line().set_opcode(ir_opcode::mul);}
      else if(bin.second_opcode() == u"div"){last_line().set_opcode(ir_opcode::div);}
      else if(bin.second_opcode() == u"rem"){last_line().set_opcode(ir_opcode::rem);}
    }

  else
    if(bin.first_opcode() == u"cmp")
    {
           if(bin.second_opcode() == u"eq"  ){last_line().set_opcode(ir_opcode::eq);}
      else if(bin.second_opcode() == u"neq" ){last_line().set_opcode(ir_opcode::neq);}
      else if(bin.second_opcode() == u"lt"  ){last_line().set_opcode(ir_opcode::lt);}
      else if(bin.second_opcode() == u"lteq"){last_line().set_opcode(ir_opcode::lteq);}
      else if(bin.second_opcode() == u"gt"  ){last_line().set_opcode(ir_opcode::gt);}
      else if(bin.second_opcode() == u"gteq"){last_line().set_opcode(ir_opcode::gteq);}
    }

  else
    if(bin.first_opcode() == u"bitw")
    {
           if(bin.second_opcode() == u"shl"){last_line().set_opcode(ir_opcode::shl);}
      else if(bin.second_opcode() == u"shr"){last_line().set_opcode(ir_opcode::shr);}
      else if(bin.second_opcode() == u"and"){last_line().set_opcode(ir_opcode::and_);}
      else if(bin.second_opcode() == u"or" ){last_line().set_opcode(ir_opcode::or_);}
      else if(bin.second_opcode() == u"xor"){last_line().set_opcode(ir_opcode::xor_);}
    }

  else
    if(bin.first_opcode() == u"logi")
    {
           if(bin.second_opcode() == u"abd"){last_line().set_opcode(ir_opcode::logi_and);}
      else if(bin.second_opcode() == u"or" ){last_line().set_opcode(ir_opcode::logi_or);}
    }
}


void
context::
push(const ir_define_operation&  def)
{
  auto  v = def.integer();

    if(v <= 0xFFFFFFFF)
    {
      last_line().set_opcode(ir_opcode::ld32);
      last_line().set_imm_value(v);
    }
}


void
context::
push(const ir_address_operation&  addr)
{
  last_line().set_opcode(ir_opcode::adr);
  last_line().set_rs1_label(addr.identifier());
}


void
context::
push(const ir_call_operation&  cal)
{
}


void
context::
push(const ir_phi_operation&  phi)
{
}




ir_package
assemble(const ir_global_space&  gsp)
{
  context  ctx;

    for(auto&  fn: gsp.function_list())
    {
      ctx.append(fn);
    }


  return ir_package();
}




}




