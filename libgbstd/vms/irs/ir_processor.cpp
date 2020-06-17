#include"libgbstd/vms/ir.hpp"




namespace gbstd{




void
ir_processor::
assign(const ir_context&  ctx) noexcept
{
  m_context = &ctx;

  reset();
}


int64_t
ir_processor::
operate(const ir_operation&  op, bool&  returned)
{
  auto  opco = op.get_operator_code();

  auto&    lb = op.get_label();
  auto&  opls = op.get_operand_list();

    if(opco == operator_code("nop"))
    {
    }

  else
    if(opco == operator_code("add"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      return l+r;
    }

  else
    if(opco == operator_code("sub"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      return l-r;
    }

  else
    if(opco == operator_code("mul"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      return l*r;
    }

  else
    if(opco == operator_code("div"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

        if(!r)
        {
          throw 0;
        }


      return l/r;
    }

  else
    if(opco == operator_code("rem"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

        if(!r)
        {
          throw 0;
        }


      return l%r;
    }

  else
    if(opco == operator_code("shl"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      return l<<r;
    }

  else
    if(opco == operator_code("shr"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      return l>>r;
    }

  else
    if(opco == operator_code("bit_or"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      return l|r;
    }

  else
    if(opco == operator_code("bit_and"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      return l&r;
    }

  else
    if(opco == operator_code("bit_xor"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      return l^r;
    }

  else
    if(opco == operator_code("bit_not"))
    {
      auto  l = evaluate(opls[0]);

      return ~l;
    }

  else
    if(opco == operator_code("eq"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      return l == r;
    }

  else
    if(opco == operator_code("neq"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      return l != r;
    }

  else
    if(opco == operator_code("lt"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      return l < r;
    }

  else
    if(opco == operator_code("lteq"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      return l <= r;
    }

  else
    if(opco == operator_code("gt"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      return l > r;
    }

  else
    if(opco == operator_code("gteq"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      return l >= r;
    }

  else
    if(opco == operator_code("logi_or"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      return l || r;
    }

  else
    if(opco == operator_code("logi_and"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      return l && r;
    }

  else
    if(opco == operator_code("ret"))
    {
      auto  val = evaluate(opls[0]);

      m_frame_stack.pop_back();

      returned = true;

      return val;
    }

  else
    if(opco == operator_code("cal"))
    {
      return operate_call(opls);
    }

  else
    if(opco == operator_code("br"))
    {
      return operate_br(opls);
    }

  else
    if(opco == operator_code("phi"))
    {
      return operate_phi(opls);
    }

  else
    if(opco == operator_code("ld8"))
    {
      auto  addr = evaluate(opls[0]);

      return reinterpret_cast<int8_t&>(m_memory[addr]);
    }

  else
    if(opco == operator_code("ld16"))
    {
      auto  addr = evaluate(opls[0]);

      return reinterpret_cast<int16_t&>(m_memory[addr]);
    }

  else
    if(opco == operator_code("ld32"))
    {
      auto  addr = evaluate(opls[0]);

      return reinterpret_cast<int32_t&>(m_memory[addr]);
    }

  else
    if(opco == operator_code("ld64"))
    {
      auto  addr = evaluate(opls[0]);

      return reinterpret_cast<int64_t&>(m_memory[addr]);
    }

  else
    if(opco == operator_code("st8"))
    {
      auto  addr = evaluate(opls[0]);
      auto   val = evaluate(opls[1]);

      reinterpret_cast<int8_t&>(m_memory[addr]) = val;

      return 0;
    }

  else
    if(opco == operator_code("st16"))
    {
      auto  addr = evaluate(opls[0]);
      auto   val = evaluate(opls[1]);

      reinterpret_cast<int16_t&>(m_memory[addr]) = val;

      return 0;
    }

  else
    if(opco == operator_code("st32"))
    {
      auto  addr = evaluate(opls[0]);
      auto   val = evaluate(opls[1]);

      reinterpret_cast<int32_t&>(m_memory[addr]) = val;

      return 0;
    }

  else
    if(opco == operator_code("st64"))
    {
      auto  addr = evaluate(opls[0]);
      auto   val = evaluate(opls[1]);

      reinterpret_cast<int64_t&>(m_memory[addr]) = val;

      return 0;
    }


  throw 0;
}


int64_t
ir_processor::
operate_call(const std::vector<ir_operand>&  opls)
{
  auto  it     = opls.begin();
  auto  it_end = opls.end();

    if((it != it_end) && it->is_label())
    {
      std::vector<int64_t>  args;

      auto  fn_name = it++->get_string();

        while(it != it_end)
        {
          auto&  o = *it++;

          args.emplace_back(evaluate(o));
        }


      return call(fn_name,args);
    }


  throw 0;
}


int64_t
ir_processor::
operate_br(const std::vector<ir_operand>&  opls)
{
  auto  n = opls.size();

    if(n == 1)
    {
      auto&  o = opls[0];

        if(o.is_label())
        {
          jump(o.get_string());

          return 0;
        }
    }

  else
    if(n == 3)
    {
      auto&  o = opls[0];

        if(o.is_label())
        {
          auto  cond = evaluate(o);

          auto&  dst = cond? opls[1]
                      :      opls[2];

            if(dst.is_label())
            {
              jump(dst.get_string());

              return 0;
            }
        }
    }


  throw 0;
}


int64_t
ir_processor::
operate_phi(const std::vector<ir_operand>&  opls)
{
  auto&  blk = *m_frame_stack.back().m_previous_block;

    for(auto&  o: opls)
    {
        if(o.is_phi_element())
        {
          auto&  e = o.get_phi_element();

            if(blk.test_label(e.get_label()))
            {
              return evaluate(e.get_operand());
            }
        }
    }    


  throw 0;
}


int64_t
ir_processor::
evaluate(const ir_operand&  o)
{
    if(o.is_integer())
    {
      return o.get_integer();
    }

  else
    if(o.is_label())
    {
      auto&  lb = o.get_string();

        for(auto&  arg: m_frame_stack.back().m_argument_list)
        {
            if(arg.m_label == lb)
            {
              return arg.m_value;
            }
        }


      auto  op = m_frame_stack.back().m_function->find_operation(lb,nullptr);

        if(op)
        {
          bool  returned = true;

          return operate(*op,returned);
        }
    }


  throw 0;
}


void
ir_processor::
jump(std::string_view  label)
{
  auto&  frm = m_frame_stack.back();

  auto  blk = frm.m_function->find_block(label);

    if(blk)
    {
      jump(*blk);
    }
}


void
ir_processor::
jump(const ir_block&  blk)
{
  auto&  frm = m_frame_stack.back();

    if(frm.m_current_block)
    {
      frm.m_previous_block = frm.m_current_block       ;
                             frm.m_current_block = &blk;
    }

  else
    {
      frm.m_previous_block = &blk;
      frm.m_current_block  = &blk;
    }


  frm.m_current = &blk.m_operation_list.back();
  frm.m_end     = frm.m_current+blk.m_operation_list.size();
}


int64_t
ir_processor::
call(std::string_view  fn_name, const std::vector<int64_t>&  args)
{
  auto  fn = m_context->find_function(fn_name);

    if(fn)
    {
      auto&  frm = m_frame_stack.emplace_back();

      frm.m_function = fn;

      auto&  paras = fn->get_parameter_list();

        if(paras.size() == args.size())
        {
          auto  it = paras.begin();

            for(auto&  v: args)
            {
              auto&  arg = frm.m_argument_list.emplace_back();

              arg.m_label = *it++;
              arg.m_value =     v;
            }
        }

      else
        {
          throw 0;
        }


      jump(fn->get_block_list().back());

      return run();
    }


  throw 0;
}


void
ir_processor::
reset() noexcept
{
  m_frame_stack.clear();
}


int64_t
ir_processor::
run()
{
    while(m_frame_stack.size())
    {
      auto&  frm = m_frame_stack.back();

      bool  returned = false;

      auto  val = operate(*frm.m_current++,returned);

        if(returned)
        {
          return val;
        }
    }


  return 0;
}




}




