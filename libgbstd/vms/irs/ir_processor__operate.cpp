#include"libgbstd/vms/ir.hpp"




namespace gbstd{




void
ir_processor::
operate(const ir_operation&  op)
{
  auto&  instr = op.get_instruction();

  auto     lb = op.get_label();
  auto&  opls = op.get_operand_list();

  auto  regptr = find(m_frame_stack.back().m_register_map,lb);

    if(instr == std::string_view("nop"))
    {
    }

  else
    if(op.is_arithmetic())
    {
      operate_ari(instr,opls,*regptr);
    }

  else
    if(op.is_comparison())
    {
      operate_cmp(instr,opls,*regptr);
    }

  else
    if(op.is_bitwise())
    {
      operate_biw(instr,opls,*regptr);
    }

  else
    if(op.is_load())
    {
      operate_ld(instr,opls,*regptr);
    }

  else
    if(op.is_store())
    {
      operate_st(instr,opls);
    }

  else
    if(instr == std::string_view("logi_or"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      regptr->set_value(l || r);
    }

  else
    if(instr == std::string_view("logi_and"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      regptr->set_value(l && r);
    }

  else
    if(instr == std::string_view("ret"))
    {
      m_frame_stack.pop_back();
    }

  else
    if(instr == std::string_view("retv"))
    {
        if(opls.empty())
        {
          throw ir_error("operate error, retv no operand");
        }


      auto  v = evaluate(opls[0]);

      m_frame_stack.back().m_return_register->set_value(v);

      m_frame_stack.pop_back();
    }

  else
    if(instr == std::string_view("jmp"))
    {
      jump(opls[0].get_string());
    }

  else
    if(instr == std::string_view("cal"))
    {
      operate_cal(opls,*regptr);
    }

  else
    if(instr == std::string_view("br"))
    {
      operate_br(opls);
    }

  else
    if(instr == std::string_view("phi"))
    {
      auto&  o = opls[0];

        if(o.is_phi_element_list())
        {
          operate_phi(o.get_phi_element_list(),*regptr);
        }
    }

  else
    {
      op.print();

      throw ir_error("operate error, unknown operator");
    }
}




void
ir_processor::
operate_ari(std::string_view  instr, const std::vector<ir_operand>&  opls, ir_register&  reg)
{
    if(instr == std::string_view("add"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);
      
      reg.set_value(l+r);
    }

  else
    if(instr == std::string_view("sub"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l-r);
    }

  else
    if(instr == std::string_view("mul"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l*r);
    }

  else
    if(instr == std::string_view("div"))
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
    if(instr == std::string_view("rem"))
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
operate_biw(std::string_view  instr, const std::vector<ir_operand>&  opls, ir_register&  reg)
{
    if(instr == std::string_view("shl"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l<<r);
    }

  else
    if(instr == std::string_view("shr"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l>>r);
    }

  else
    if(instr == std::string_view("bit_or"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l|r);
    }

  else
    if(instr == std::string_view("bit_and"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l&r);
    }

  else
    if(instr == std::string_view("bit_xor"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l^r);
    }

  else
    if(instr == std::string_view("bit_not"))
    {
      auto  l = evaluate(opls[0]);

      reg.set_value(~l);
    }
}


void
ir_processor::
operate_cmp(std::string_view  instr, const std::vector<ir_operand>&  opls, ir_register&  reg)
{
    if(instr == std::string_view("eq"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l == r);
    }

  else
    if(instr == std::string_view("neq"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l != r);
    }

  else
    if(instr == std::string_view("lt"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l < r);
    }

  else
    if(instr == std::string_view("lteq"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l <= r);
    }

  else
    if(instr == std::string_view("gt"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l > r);
    }

  else
    if(instr == std::string_view("gteq"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      reg.set_value(l >= r);
    }
}


void
ir_processor::
operate_ld(std::string_view  instr, const std::vector<ir_operand>&  opls, ir_register&  reg)
{
    if(instr == std::string_view("ld8"))
    {
      auto  addr = evaluate(opls[0]);

      reg.set_value(reinterpret_cast<int8_t&>(m_memory[addr]));
    }

  else
    if(instr == std::string_view("ld16"))
    {
      auto  addr = evaluate(opls[0]);

      reg.set_value(reinterpret_cast<int16_t&>(m_memory[addr]));
    }

  else
    if(instr == std::string_view("ld32"))
    {
      auto  addr = evaluate(opls[0]);

      reg.set_value(reinterpret_cast<int32_t&>(m_memory[addr]));
    }

  else
    if(instr == std::string_view("ld64"))
    {
      auto  addr = evaluate(opls[0]);

      reg.set_value(reinterpret_cast<int64_t&>(m_memory[addr]));
    }
}


void
ir_processor::
operate_st(std::string_view  instr, const std::vector<ir_operand>&  opls)
{
    if(instr == std::string_view("st8"))
    {
      auto  addr = evaluate(opls[0]);
      auto   val = evaluate(opls[1]);

      reinterpret_cast<int8_t&>(m_memory[addr]) = val;
    }

  else
    if(instr == std::string_view("st16"))
    {
      auto  addr = evaluate(opls[0]);
      auto   val = evaluate(opls[1]);

      reinterpret_cast<int16_t&>(m_memory[addr]) = val;
    }

  else
    if(instr == std::string_view("st32"))
    {
      auto  addr = evaluate(opls[0]);
      auto   val = evaluate(opls[1]);

      reinterpret_cast<int32_t&>(m_memory[addr]) = val;
    }

  else
    if(instr == std::string_view("st64"))
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
    if((opls.size() == 2) &&
       opls[0].is_label() &&
       opls[1].is_operand_list())
    {
      std::vector<int64_t>  args;

      auto  fn_name = opls[0].get_string();

        for(auto&  op: opls[1].get_operand_list())
        {
          args.emplace_back(evaluate(op));
        }


      call(&reg,fn_name,args);

      return;
    }


    for(auto&  o: opls)
    {
      o.print();
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
      auto  cond = evaluate(opls[0]);

      auto&  dst = cond? opls[1]
                  :      opls[2];

        if(dst.is_label())
        {
          jump(dst.get_string());
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

    if(!bi)
    {
      throw ir_error("operate_phi error: previous_block_info is missing");
    }


    for(auto&  ph: phels)
    {
        if(bi->test_label(ph.get_label()))
        {
          auto  v = evaluate(ph.get_operand());

          reg.set_value(v);

          return;
        }
    }


  throw ir_error("phi error: all blocks is not matched");
}




}




