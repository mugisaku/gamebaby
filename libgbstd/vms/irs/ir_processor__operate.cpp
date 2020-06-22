#include"libgbstd/vms/ir.hpp"




namespace gbstd{




void
ir_processor::
operate(const ir_operation&  op)
{
  auto  opco = op.get_operator_code();

  auto     lb = op.get_label();
  auto&  opls = op.get_operand_list();

  auto  regptr = find(m_frame_stack.back().m_register_map,lb);

    if(opco == operator_code("nop"))
    {
    }

  else
    if(op.is_arithmetic())
    {
      operate_ari(opco,opls,*regptr);
    }

  else
    if(op.is_comparison())
    {
      operate_cmp(opco,opls,*regptr);
    }

  else
    if(op.is_bitwise())
    {
      operate_biw(opco,opls,*regptr);
    }

  else
    if(op.is_load())
    {
      operate_ld(opco,opls,*regptr);
    }

  else
    if(op.is_store())
    {
      operate_st(opco,opls);
    }

  else
    if(opco == operator_code("logi_or"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      regptr->set_value(l || r);
    }

  else
    if(opco == operator_code("logi_and"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      regptr->set_value(l && r);
    }

  else
    if(opco == operator_code("ret"))
    {
      auto  v = opls.size()? evaluate(opls[0])
               :0
               ;

      m_frame_stack.back().m_return_register->set_value(v);

      m_frame_stack.pop_back();
    }

  else
    if(opco == operator_code("cal"))
    {
      operate_cal(opls,*regptr);
    }

  else
    if(opco == operator_code("br"))
    {
      operate_br(opls);
    }

  else
    if(opco == operator_code("phi"))
    {
      auto&  o = opls[0];

        if(o.is_phi_element_list())
        {
          operate_phi(o.get_phi_element_list(),*regptr);
        }
    }

  else
    {
      throw ir_error("operate error, unknown operator");
    }
}




void
ir_processor::
operate_ari(operator_code  opco, const std::vector<ir_operand>&  opls, ir_register&  reg)
{
    if(opco == operator_code("add"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);
      
      reg.set_value(l+r);
    }

  else
    if(opco == operator_code("sub"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l-r);
    }

  else
    if(opco == operator_code("mul"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l*r);
    }

  else
    if(opco == operator_code("div"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

        if(!r)
        {
          throw ir_error("div error");
        }


      reg.set_value(l/r);
    }

  else
    if(opco == operator_code("rem"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

        if(!r)
        {
          throw ir_error("rem error");
        }


      reg.set_value(l%r);
    }
}


void
ir_processor::
operate_biw(operator_code  opco, const std::vector<ir_operand>&  opls, ir_register&  reg)
{
    if(opco == operator_code("shl"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l<<r);
    }

  else
    if(opco == operator_code("shr"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l>>r);
    }

  else
    if(opco == operator_code("bit_or"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l|r);
    }

  else
    if(opco == operator_code("bit_and"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l&r);
    }

  else
    if(opco == operator_code("bit_xor"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l^r);
    }

  else
    if(opco == operator_code("bit_not"))
    {
      auto  l = evaluate(opls[0]);

      reg.set_value(~l);
    }
}


void
ir_processor::
operate_cmp(operator_code  opco, const std::vector<ir_operand>&  opls, ir_register&  reg)
{
    if(opco == operator_code("eq"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l == r);
    }

  else
    if(opco == operator_code("neq"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l != r);
    }

  else
    if(opco == operator_code("lt"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l < r);
    }

  else
    if(opco == operator_code("lteq"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l <= r);
    }

  else
    if(opco == operator_code("gt"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l > r);
    }

  else
    if(opco == operator_code("gteq"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l >= r);
    }
}


void
ir_processor::
operate_ld(operator_code  opco, const std::vector<ir_operand>&  opls, ir_register&  reg)
{
    if(opco == operator_code("ld8"))
    {
      auto  addr = evaluate(opls[0]);

      reg.set_value(reinterpret_cast<int8_t&>(m_memory[addr]));
    }

  else
    if(opco == operator_code("ld16"))
    {
      auto  addr = evaluate(opls[0]);

      reg.set_value(reinterpret_cast<int16_t&>(m_memory[addr]));
    }

  else
    if(opco == operator_code("ld32"))
    {
      auto  addr = evaluate(opls[0]);

      reg.set_value(reinterpret_cast<int32_t&>(m_memory[addr]));
    }

  else
    if(opco == operator_code("ld64"))
    {
      auto  addr = evaluate(opls[0]);

      reg.set_value(reinterpret_cast<int64_t&>(m_memory[addr]));
    }
}


void
ir_processor::
operate_st(operator_code  opco, const std::vector<ir_operand>&  opls)
{
    if(opco == operator_code("st8"))
    {
      auto  addr = evaluate(opls[0]);
      auto   val = evaluate(opls[1]);

      reinterpret_cast<int8_t&>(m_memory[addr]) = val;
    }

  else
    if(opco == operator_code("st16"))
    {
      auto  addr = evaluate(opls[0]);
      auto   val = evaluate(opls[1]);

      reinterpret_cast<int16_t&>(m_memory[addr]) = val;
    }

  else
    if(opco == operator_code("st32"))
    {
      auto  addr = evaluate(opls[0]);
      auto   val = evaluate(opls[1]);

      reinterpret_cast<int32_t&>(m_memory[addr]) = val;
    }

  else
    if(opco == operator_code("st64"))
    {
      auto  addr = evaluate(opls[0]);
      auto   val = evaluate(opls[1]);

      reinterpret_cast<int64_t&>(m_memory[addr]) = val;
    }
}


void
ir_processor::
operate_cal(const std::vector<ir_operand>&  opls, ir_register&  reg)
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


      call(&reg,fn_name,args);

      return;
    }


  throw ir_error("cal error");
}


void
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
            }
        }
    }

  else
    {
      throw ir_error("br error");
    }
}


void
ir_processor::
operate_phi(const std::vector<ir_phi_element>&  phels, ir_register&  reg)
{
  auto  bi = m_frame_stack.back().m_previous_block_info;

    for(auto&  ph: phels)
    {
        if(bi->test_label(ph.get_label()))
        {
          auto  v = evaluate(ph.get_operand());

          reg.set_value(v);

          return;
        }
    }


  throw ir_error("phi error");
}




}




