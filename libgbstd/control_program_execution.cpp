#include"libgbstd/gbstd.hpp"
#include"libgbstd/parser.hpp"




namespace gbstd{




void
program_execution::
step(dummy*  ptr) noexcept
{
  auto&  sp = *m_space;

    while(m_pc < sp.get_number_of_elements())
    {
      auto&  el = sp[m_pc++];

      auto&  f = el.get_first();
      auto&  s = el.get_second();

        if(el.is_invoke())
        {
          auto  b = sp.find_body(f);

            if(b)
            {
                if(ptr)
                {
                  (*b)(*ptr);
                }

              else
                {
                  (*b)();
                }
            }

          else
            {
              printf("[program_execution error] body %s not found\n",f.data());
            }
        }

      else
        if(el.is_branch())
        {
          bool  nocond = f.empty();

          const condition_callback*  c = !nocond? sp.find_condition(f):nullptr;

          auto  pc = sp.find_label(    s);

            if(!nocond && !c)
            {
              printf("[program_execution error] condition %s not found\n",f.data());
            }

          else
            if(pc < 0)
            {
              printf("[program_execution error] lebel %s not found\n",s.data());
            }

          else
            {
                if(ptr)
                {
                    if(nocond || (*c)(*ptr))
                    {
                      m_pc = pc;
                    }
                }

              else
                {
                    if(nocond || (*c)())
                    {
                      m_pc = pc;
                    }
                }
            }
        }

      else
        if(el.is_label())
        {
        }

      else
        if(el.is_jump())
        {
          auto  pc = sp.find_label(el.get_first());

            if(pc >= 0)
            {
              m_pc = pc;
            }

          else
            {
              printf("[program_execution error] %s not found\n",f.data());
            }
        }


        if(el.test_interrupt_flag())
        {
          break;
        }
    }
}



}




