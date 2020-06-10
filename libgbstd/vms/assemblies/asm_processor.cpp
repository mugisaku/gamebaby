#include"libgbstd/vms/assembly.hpp"




namespace gbstd{




constexpr int  g_word_size = sizeof(int64_t);


void
asm_processor::
put_i8(int64_t  address, int8_t  i) noexcept
{
  reinterpret_cast<int8_t&>(m_memory[address]) = i;
}


void
asm_processor::
put_i16(int64_t  address, int16_t  i) noexcept
{
  reinterpret_cast<int16_t&>(m_memory[address]) = i;
}


void
asm_processor::
put_i32(int64_t  address, int32_t  i) noexcept
{
  reinterpret_cast<int32_t&>(m_memory[address]) = i;
}


void
asm_processor::
put_i64(int64_t  address, int64_t  i) noexcept
{
  reinterpret_cast<int64_t&>(m_memory[address]) = i;
}


void
asm_processor::
put_f32(int64_t  address, float  f) noexcept
{
  reinterpret_cast<float&>(m_memory[address]) = f;
}


void
asm_processor::
put_f64(int64_t  address, double  f) noexcept
{
  reinterpret_cast<double&>(m_memory[address]) = f;
}




int8_t   asm_processor::get_i8( int64_t  address) noexcept{return reinterpret_cast<int8_t&>(m_memory[address]);}
int16_t  asm_processor::get_i16(int64_t  address) noexcept{return reinterpret_cast<int16_t&>(m_memory[address]);}
int32_t  asm_processor::get_i32(int64_t  address) noexcept{return reinterpret_cast<int32_t&>(m_memory[address]);}
int64_t  asm_processor::get_i64(int64_t  address) noexcept{return reinterpret_cast<int64_t&>(m_memory[address]);}
double   asm_processor::get_f32(int64_t  address) noexcept{return reinterpret_cast<float&>(m_memory[address]);}
double   asm_processor::get_f64(int64_t  address) noexcept{return reinterpret_cast<double&>(m_memory[address]);}




void  asm_processor::put_top_i64(int64_t  i) noexcept{put_i64(m_sp,i);}
void  asm_processor::put_top_f64(double   f) noexcept{put_f64(m_sp,f);}

int64_t  asm_processor::get_top_i64() noexcept{return get_i64(m_sp);}
double   asm_processor::get_top_f64() noexcept{return get_f64(m_sp);}


void
asm_processor::
push_i64(int64_t  i) noexcept
{
  put_i64(m_sp,i);

  m_sp += g_word_size;
}


void
asm_processor::
push_f64(double  f) noexcept
{
  put_f64(m_sp,f);

  m_sp += g_word_size;
}


int64_t
asm_processor::
pop_i64() noexcept
{
  auto  v = get_top_i64();

  m_sp -= g_word_size;

  return v;
}


double
asm_processor::
pop_f64() noexcept
{
  auto  v = get_top_f64();

  m_sp -= g_word_size;

  return v;
}




int8_t
asm_processor::
read_i8() noexcept
{
  int8_t  i = m_memory[m_pc++];

  return i;
}


int16_t
asm_processor::
read_i16() noexcept
{
  int16_t  i  = m_memory[m_pc++];  i <<= 8;
           i |= m_memory[m_pc++];

  return i;
}


int32_t
asm_processor::
read_i32() noexcept
{
  int32_t  i  = m_memory[m_pc++];  i <<= 8;
           i |= m_memory[m_pc++];  i <<= 8;
           i |= m_memory[m_pc++];  i <<= 8;
           i |= m_memory[m_pc++];

  return i;
}


int64_t
asm_processor::
read_i64() noexcept
{
  int64_t  i  = m_memory[m_pc++];  i <<= 8;
           i |= m_memory[m_pc++];  i <<= 8;
           i |= m_memory[m_pc++];  i <<= 8;
           i |= m_memory[m_pc++];  i <<= 8;
           i |= m_memory[m_pc++];  i <<= 8;
           i |= m_memory[m_pc++];  i <<= 8;
           i |= m_memory[m_pc++];  i <<= 8;
           i |= m_memory[m_pc++];

  return i;
}




void
asm_processor::
step()
{
  auto  opcode = static_cast<asm_opcode>(m_memory[m_pc++]);

    switch(opcode)
    {
  case(asm_opcode::nop ): break;
  case(asm_opcode::addi): operate_binary_i([](int64_t  a, int64_t  b)->int64_t{return a+b;});break;
  case(asm_opcode::addf): operate_binary_f([](double  a, double  b)->double{return a+b;});break;
  case(asm_opcode::subi): operate_binary_i([](int64_t  a, int64_t  b)->int64_t{return a-b;});break;
  case(asm_opcode::subf): operate_binary_f([](double  a, double  b)->double{return a-b;});break;
  case(asm_opcode::muli): operate_binary_i([](int64_t  a, int64_t  b)->int64_t{return a*b;});break;
  case(asm_opcode::mulf): operate_binary_f([](double  a, double  b)->double{return a*b;});break;
  case(asm_opcode::divi): operate_binary_i([](int64_t  a, int64_t  b)->int64_t{return a/b;});break;
  case(asm_opcode::divf): operate_binary_f([](double  a, double  b)->double{return a/b;});break;
  case(asm_opcode::rem ): operate_binary_i([](int64_t  a, int64_t  b)->int64_t{return a%b;});break;
  case(asm_opcode::shl ): operate_binary_i([](int64_t  a, int64_t  b)->int64_t{return a<<b;});break;
  case(asm_opcode::shr ): operate_binary_i([](int64_t  a, int64_t  b)->int64_t{return a>>b;});break;
  case(asm_opcode::band): operate_binary_i([](int64_t  a, int64_t  b)->int64_t{return a&b;});break;
  case(asm_opcode::bor ): operate_binary_i([](int64_t  a, int64_t  b)->int64_t{return a|b;});break;
  case(asm_opcode::bxor): operate_binary_i([](int64_t  a, int64_t  b)->int64_t{return a^b;});break;

  case(asm_opcode::eq   ): operate_binary_i([](int64_t  a, int64_t  b)->int64_t{return a == b;});break;
  case(asm_opcode::neq  ): operate_binary_i([](int64_t  a, int64_t  b)->int64_t{return a != b;});break;
  case(asm_opcode::lti  ): operate_binary_i([](int64_t  a, int64_t  b)->int64_t{return a < b;});break;
  case(asm_opcode::ltf  ): operate_binary_f([](double  a, double  b)->double{return a < b;});break;
  case(asm_opcode::lteqi): operate_binary_i([](int64_t  a, int64_t  b)->int64_t{return a <= b;});break;
  case(asm_opcode::lteqf): operate_binary_f([](double  a, double  b)->double{return a <= b;});break;
  case(asm_opcode::gti  ): operate_binary_i([](int64_t  a, int64_t  b)->int64_t{return a > b;});break;
  case(asm_opcode::gtf  ): operate_binary_f([](double  a, double  b)->double{return a > b;});break;
  case(asm_opcode::gteqi): operate_binary_i([](int64_t  a, int64_t  b)->int64_t{return a >= b;});break;
  case(asm_opcode::gteqf): operate_binary_f([](double  a, double  b)->double{return a >= b;});break;

  case(asm_opcode::land): operate_binary_i([](int64_t  a, int64_t  b)->int64_t{return a && b;});break;
  case(asm_opcode::lor ): operate_binary_i([](int64_t  a, int64_t  b)->int64_t{return a || b;});break;

  case(asm_opcode::bnot): operate_unary_i([](int64_t  a)->int64_t{return ~a;});break;
  case(asm_opcode::lnot): operate_unary_i([](int64_t  a)->int64_t{return !a;});break;
  case(asm_opcode::negi): operate_unary_i([](int64_t  a)->int64_t{return -a;});break;
  case(asm_opcode::negf): operate_unary_f([](double  a)->double{return -a;});break;

  case(asm_opcode::setpc): m_pc  = pop_i64();break;
  case(asm_opcode::addpc): m_pc += pop_i64();break;
  case(asm_opcode::pshpc): push_i64(m_pc);break;

  case(asm_opcode::setbp): m_bp  = pop_i64();break;
  case(asm_opcode::addbp): m_bp += pop_i64();break;
  case(asm_opcode::pshbp): push_i64(m_bp);break;

  case(asm_opcode::setsp): m_sp  = pop_i64();break;
  case(asm_opcode::addsp): m_sp += pop_i64();break;
  case(asm_opcode::pshsp): push_i64(m_sp);break;

  case(asm_opcode::br): {
      auto   val = pop_i64();
      auto  addr = pop_i64();

        if(val)
        {
          m_pc += val;
        }
    } break;

  case(asm_opcode::pshz  ): push_i64(0);break;
  case(asm_opcode::pshnz ): push_i64(1);break;
  case(asm_opcode::pshi8 ): push_i64(read_i8() );break;
  case(asm_opcode::pshi16): push_i64(read_i16());break;
  case(asm_opcode::pshi32): push_i64(read_i32());break;
  case(asm_opcode::pshi64): push_i64(read_i64());break;

  case(asm_opcode::ld8 ): put_top_i64(get_i8( get_top_i64()));break;
  case(asm_opcode::ld16): put_top_i64(get_i16(get_top_i64()));break;
  case(asm_opcode::ld32): put_top_i64(get_i32(get_top_i64()));break;
  case(asm_opcode::ld64): put_top_i64(get_i64(get_top_i64()));break;

  case(asm_opcode::sti8): {
      auto   val = pop_i64();
      auto  addr = pop_i64();

      put_i8(addr,val);
    } break;

  case(asm_opcode::sti16): {
      auto   val = pop_i64();
      auto  addr = pop_i64();

      put_i16(addr,val);
    } break;

  case(asm_opcode::sti32): {
      auto   val = pop_i64();
      auto  addr = pop_i64();

      put_i32(addr,val);
    } break;

  case(asm_opcode::sti64): {
      auto   val = pop_i64();
      auto  addr = pop_i64();

      put_i64(addr,val);
    } break;

  case(asm_opcode::stf32): {
      auto   val = pop_f64();
      auto  addr = pop_i64();

      put_i32(addr,val);
    } break;

  case(asm_opcode::stf64): {
      auto   val = pop_f64();
      auto  addr = pop_i64();

      put_f64(addr,val);
    } break;

  case(asm_opcode::pop): pop_i64();break;
  case(asm_opcode::dup): push_i64(get_top_i64());break;

  case(asm_opcode::swp): {
      auto  a = pop_i64();
      auto  b = pop_i64();

      push_i64(a);
      push_i64(b);

    } break;

  case(asm_opcode::i2f):
      break;
  case(asm_opcode::f2i):
      break;
    }
}


void
asm_processor::
reset(int64_t  entry_point) noexcept
{
  m_pc = entry_point;

  m_sp = 0;
  m_bp = 0;
}


void
asm_processor::
read_binary(const std::vector<uint8_t>&  bin) noexcept
{
  auto  p = &m_memory[m_stack_size];

    for(auto& v: bin)
    {
      *p++ = v;
    }
}




}




