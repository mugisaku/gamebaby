#include"libgbstd/gbstd.hpp"




namespace gbstd{




program_execution&
program_execution::
assign(program_space&  sp) noexcept
{
  m_space = &sp;

  reset();

  return *this;
}


void
program_execution::
invoke(const std::string&  id, dummy*  ptr) noexcept
{
    if(id.empty())
    {
      m_halt_flag = true;

      return;
    }


  auto  b = m_space->find_body(id);

    if(b)
    {
        if(ptr){(*b)(*ptr);}
      else     {(*b)(    );}
    }

  else
    {
      printf("[program_execution error] body %s not found\n",id.data());

      m_halt_flag = true;
    }
}


void
program_execution::
jump(const std::string&  cond, const std::string&  lb, bool  onzero, dummy*  ptr) noexcept
{
  bool  nocond = cond.empty();

  const condition_callback*  c = !nocond? m_space->find_condition(cond):nullptr;

  auto  pc = m_space->find_label(lb);

    if(!nocond && !c)
    {
      printf("[program_execution error] condition %s not found\n",cond.data());

      m_halt_flag = true;
    }

  else
    if(pc < 0)
    {
      printf("[program_execution error] lebel %s not found\n",lb.data());

      m_halt_flag = true;
    }

  else
    if(nocond)
    {
      m_pc = pc;
    }

  else
    {
      auto  res = ptr? (*c)(*ptr)
                 :     (*c)(    );

        if(onzero)
        {
          res = !res;
        }


        if(res)
        {
          m_pc = pc;
        }
    }
}


program_execution&
program_execution::
reset() noexcept
{
  m_pc = 0;

  m_halt_flag = false;

  return *this;
}


void
program_execution::
step(dummy*  ptr) noexcept
{
  auto&  sp = *m_space;

  int  safe_counter = 800;

    for(;;)
    {
        if(!safe_counter--)
        {
          printf("[program_execution error] safe_counter is zero\n");

          break;
        }


        if(m_pc >= sp.get_number_of_elements())
        {
          m_halt_flag = true;
        }


        if(m_halt_flag)
        {
          break;
        }


      auto&  el = sp[m_pc++];

      auto&  f = el.get_first();
      auto&  s = el.get_second();

           if(el.is_invoke()){invoke(f,ptr);}
      else if(el.is_jz()    ){jump(f,s, true,ptr);}
      else if(el.is_jnz()   ){jump(f,s,false,ptr);}
      else if(el.is_label() ){}


        if(el.test_interrupt_flag())
        {
          break;
        }
    }
}



}




