#include"libgbstd/vms/ir.hpp"




namespace gbstd{




ir_processor&
ir_processor::
assign(const ir_package&  pk) noexcept
{
  m_begin =         pk.instruction_array().data();
  m_end   = m_begin+pk.instruction_array().size();

  m_memory = pk.memory();

  return *this;
}


ir_address
ir_processor::
get_address(ir_register_spec  rs) const noexcept
{
  return rs.is_from_zero()?               (rs.index()<<3)
        :rs.is_from_bp()  ? m_memory.bp()+(rs.index()<<3)
        :rs.is_from_sp()  ? m_memory.sp()+(rs.index()<<3)
        :0
        ;
}


void
ir_processor::
cal(ir_address  fnptr_addr, ir_address  retval_addr) noexcept
{
  auto&  pc = m_memory.pc();
  auto&  bp = m_memory.bp();
  auto&  sp = m_memory.sp();

  m_memory.store64(sp   ,bp);
  m_memory.store64(sp+ 8,pc);
  m_memory.store64(sp+16,retval_addr);

  bp = sp;
  pc = m_memory.load64(fnptr_addr);
}


void
ir_processor::
ret() noexcept
{
  auto&  bp = m_memory.bp();

  auto  previous_bp = m_memory.load64(bp                );
  auto           ra = m_memory.load64(bp+sizeof(int64_t));

  m_memory.sp() = bp              ;
                  bp = previous_bp;

  m_memory.pc() = ra;
}


void
ir_processor::
process(ir_instruction  instr) noexcept
{
  int64_t  l;
  int64_t  r;

  auto  r1addr = get_address(instr.rs1());
  auto  r2addr = get_address(instr.rs2());
  auto  r3addr = get_address(instr.rs3());

  auto&  mem = m_memory;

    switch(instr.opcode())
    {
  case(ir_opcode::nop): break;
  case(ir_opcode::add):
      l = mem.load64(r1addr);
      r = mem.load64(r2addr);

      mem.store64(r3addr,l+r);
      break;
  case(ir_opcode::sub):
      l = mem.load64(r1addr);
      r = mem.load64(r2addr);

      mem.store64(r3addr,l-r);
      break;
  case(ir_opcode::mul):
      l = mem.load64(r1addr);
      r = mem.load64(r2addr);

      mem.store64(r3addr,l*r);
      break;
  case(ir_opcode::div):
      l = mem.load64(r1addr);
      r = mem.load64(r2addr);

        if(r)
        {
          mem.store64(r3addr,l/r);
        }
      break;
  case(ir_opcode::rem):
      l = mem.load64(r1addr);
      r = mem.load64(r2addr);

        if(r)
        {
          mem.store64(r3addr,l%r);
        }
      break;
  case(ir_opcode::shl):
      l = mem.load64(r1addr);
      r = mem.load64(r2addr);

      mem.store64(r3addr,l<<r);
      break;
  case(ir_opcode::shr):
      l = mem.load64(r1addr);
      r = mem.load64(r2addr);

      mem.store64(r3addr,l>>r);
      break;
  case(ir_opcode::and_):
      l = mem.load64(r1addr);
      r = mem.load64(r2addr);

      mem.store64(r3addr,l&r);
      break;
  case(ir_opcode::or_):
      l = mem.load64(r1addr);
      r = mem.load64(r2addr);

      mem.store64(r3addr,l|r);
      break;
  case(ir_opcode::xor_):
      l = mem.load64(r1addr);
      r = mem.load64(r2addr);

      mem.store64(r3addr,l^r);
      break;
  case(ir_opcode::not_):
      l = mem.load64(r1addr);

      mem.store64(r3addr,~l);
      break;
  case(ir_opcode::eq):
      l = mem.load64(r1addr);
      r = mem.load64(r2addr);

      mem.store64(r3addr,(l == r)? 1:0);
      break;
  case(ir_opcode::neq):
      l = mem.load64(r1addr);
      r = mem.load64(r2addr);

      mem.store64(r3addr,(l != r)? 1:0);
      break;
  case(ir_opcode::lt):
      l = mem.load64(r1addr);
      r = mem.load64(r2addr);

      mem.store64(r3addr,(l < r)? 1:0);
      break;
  case(ir_opcode::lteq):
      l = mem.load64(r1addr);
      r = mem.load64(r2addr);

      mem.store64(r3addr,(l <= r)? 1:0);
      break;
  case(ir_opcode::gt):
      l = mem.load64(r1addr);
      r = mem.load64(r2addr);

      mem.store64(r3addr,(l > r)? 1:0);
      break;
  case(ir_opcode::gteq):
      l = mem.load64(r1addr);
      r = mem.load64(r2addr);

      mem.store64(r3addr,(l >= r)? 1:0);
      break;
  case(ir_opcode::logi_and):
      l = mem.load64(r1addr);
      r = mem.load64(r2addr);

      mem.store64(r3addr,(l && r)? 1:0);
      break;
  case(ir_opcode::logi_or):
      l = mem.load64(r1addr);
      r = mem.load64(r2addr);

      mem.store64(r3addr,(l || r)? 1:0);
      break;
  case(ir_opcode::logi_not):
      l = mem.load64(r1addr);

      mem.store64(r3addr,!l? 1:0);
      break;
  case(ir_opcode::neg):
      l = mem.load64(r1addr);

      mem.store64(r3addr,-l);
      break;
  case(ir_opcode::ld32):
      mem.store64(r1addr,static_cast<int64_t>(instr.imm32()));
      break;
  case(ir_opcode::ld64hi):
      mem.store64hi(r1addr,instr.immu32());
      break;
  case(ir_opcode::ld64lo):
      mem.store64lo(r1addr,instr.immu32());
      break;
  case(ir_opcode::cpy):
      l = mem.load64(r1addr);

      mem.store64(r3addr,l);
      break;
  case(ir_opcode::adr):
      mem.store64(r3addr,r1addr);
      break;
  case(ir_opcode::cal):
      cal(r1addr,r3addr);
      break;
  case(ir_opcode::ret):
      ret();
      break;
  case(ir_opcode::br):
      l = mem.load64(r1addr);

        if(l)
        {
          m_memory.pc() += instr.imm32();
        }
      break;
  case(ir_opcode::jmp):
      m_memory.pc() += instr.imm32();
      break;
    }
}


bool
ir_processor::
step() noexcept
{
  auto  current = &m_begin[m_memory.pc()++];

    if(current < m_end)
    {
      process(*current);

      return true;
    }


  return false;
}


void
ir_processor::
run() noexcept
{
    for(;;)
    {
        if(!step())
        {
          break;
        }
    }
}




}




