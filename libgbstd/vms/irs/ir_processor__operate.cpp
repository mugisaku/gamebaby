#include"libgbstd/vms/ir.hpp"




namespace gbstd{




void
ir_processor::
operate(const ir_operation&  op)
{
  auto&  instr = op.get_instruction();

  auto     lb = op.get_label();
  auto&  opls = op.get_operand_list();

  auto  addr = find_variable(lb);

  auto&  var = get_variable(addr);

    if(instr == std::string_view("nop"))
    {
    }

  else
    if(op.is_arithmetic())
    {
      operate_ari(instr,opls,var);
    }

  else
    if(op.is_comparison())
    {
      operate_cmp(instr,opls,var);
    }

  else
    if(op.is_bitwise())
    {
      operate_biw(instr,opls,var);
    }

  else
    if(op.is_load())
    {
      operate_ld(instr,opls,var);
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

        if(l->is_integer() &&
           r->is_integer())
        {
          var = ir_value(l.get_integer() || r.get_integer());
        }
    }

  else
    if(instr == std::string_view("logi_and"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

        if(l->is_integer() &&
           r->is_integer())
        {
          var = ir_value(l.get_integer() && r.get_integer());
        }
    }

  else
    if(instr == std::string_view("ret"))
    {
      pop_frame();
    }

  else
    if(instr == std::string_view("retv"))
    {
      auto  addr = m_frame_stack.back().m_return_value_address;

        if(addr)
        {
          get_variable(addr) = evaluate(opls[0]);

          pop_frame();

          return;
        }


      throw ir_error("operate error, retv no variable for return");
    }

  else
    if(instr == std::string_view("adr"))
    {
        if(opls.size() != 1)
        {
          throw ir_error("operate error, adr invalid number of operands");
        }


        if(opls[0].is_label())
        {
          auto  addr = find_variable(opls[0].get_string());

            if(addr)
            {
              var = addr;

              return;
            }
        }


      throw ir_error(form_string("operate adr error: %s is not found",opls[0].get_string().data()));
    }

  else
    if(instr == std::string_view("jmp"))
    {
      jump(opls[0].get_string());
    }

  else
    if(instr == std::string_view("cal"))
    {
      operate_cal(addr,opls);
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
          operate_phi(o.get_phi_element_list(),var);
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
operate_ari(std::string_view  instr, const std::vector<ir_operand>&  opls, ir_variable&  var)
{
  auto  l = evaluate(opls[0]);
  auto  r = evaluate(opls[1]);

    if(instr == std::string_view("add"))
    {
        if(l->is_integer() &&
           r->is_integer())
        {
          var = ir_value(l.get_integer()+r.get_integer());
        }
    }

  else
    if(instr == std::string_view("sub"))
    {
        if(l->is_integer() &&
           r->is_integer())
        {
          var = ir_value(l.get_integer()+r.get_integer());
        }
    }

  else
    if(instr == std::string_view("mul"))
    {
        if(l->is_integer() &&
           r->is_integer())
        {
          var = ir_value(l.get_integer()+r.get_integer());
        }
    }

  else
    if(instr == std::string_view("div"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

        if(l->is_integer() &&
           r->is_integer())
        {
          var = ir_value(l.get_integer()/r.get_integer());
        }
    }

  else
    if(instr == std::string_view("rem"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

        if(l->is_integer() &&
           r->is_integer())
        {
          var = ir_value(l.get_integer()%r.get_integer());
        }
    }
}


void
ir_processor::
operate_biw(std::string_view  instr, const std::vector<ir_operand>&  opls, ir_variable&  var)
{
    if(instr == std::string_view("shl"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

        if(l->is_integer() &&
           r->is_integer())
        {
          var = ir_value(l.get_integer()<<r.get_integer());
        }
    }

  else
    if(instr == std::string_view("shr"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

        if(l->is_integer() &&
           r->is_integer())
        {
          var = ir_value(l.get_integer()>>r.get_integer());
        }
    }

  else
    if(instr == std::string_view("bit_or"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

        if(l->is_integer() &&
           r->is_integer())
        {
          var = ir_value(l.get_integer()|r.get_integer());
        }
    }

  else
    if(instr == std::string_view("bit_and"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

        if(l->is_integer() &&
           r->is_integer())
        {
          var = ir_value(l.get_integer()&r.get_integer());
        }
    }

  else
    if(instr == std::string_view("bit_xor"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

        if(l->is_integer() &&
           r->is_integer())
        {
          var = ir_value(l.get_integer()^r.get_integer());
        }
    }

  else
    if(instr == std::string_view("bit_not"))
    {
      auto  v = evaluate(opls[0]);

        if(v->is_integer())
        {
          var = ir_value(~v.get_integer());
        }
    }
}


void
ir_processor::
operate_cmp(std::string_view  instr, const std::vector<ir_operand>&  opls, ir_variable&  var)
{
  auto  l = evaluate(opls[0]);
  auto  r = evaluate(opls[1]);

  bool  b;

    if(instr == std::string_view("eq"))
    {
        if(l->is_integer() &&
           r->is_integer())
        {
          b = l.get_integer() == r.get_integer();
        }
    }

  else
    if(instr == std::string_view("neq"))
    {
        if(l->is_integer() &&
           r->is_integer())
        {
          b = l.get_integer() != r.get_integer();
        }
    }

  else
    if(instr == std::string_view("lt"))
    {
        if(l->is_integer() &&
           r->is_integer())
        {
          b = l.get_integer() < r.get_integer();
        }
    }

  else
    if(instr == std::string_view("lteq"))
    {
        if(l->is_integer() &&
           r->is_integer())
        {
          b = l.get_integer() <= r.get_integer();
        }
    }

  else
    if(instr == std::string_view("gt"))
    {
        if(l->is_integer() &&
           r->is_integer())
        {
          b = l.get_integer() > r.get_integer();
        }
    }

  else
    if(instr == std::string_view("gteq"))
    {
        if(l->is_integer() &&
           r->is_integer())
        {
          b = l.get_integer() >= r.get_integer();
        }
    }


  var = ir_value(b);
}


void
ir_processor::
operate_ld(std::string_view  instr, const std::vector<ir_operand>&  opls, ir_variable&  var)
{
  auto  v = evaluate(opls[0]);

    if(!v->is_integer())
    {
      throw ir_error("operate ld error");
    }


  ir_address  addr(v.get_integer());

  auto&  src = get_variable(addr);

  auto  mem = src.get_memory();

  void*  ptr = &mem[addr.get_offset()];

  int64_t  i;

    if(instr == std::string_view("ld8"))
    {
      i = *static_cast<int8_t*>(ptr);
    }

  else
    if(instr == std::string_view("ld16"))
    {
      i = *static_cast<int16_t*>(ptr);
    }

  else
    if(instr == std::string_view("ld32"))
    {
      i = *static_cast<int32_t*>(ptr);
    }

  else
    if(instr == std::string_view("ld64"))
    {
      i = *static_cast<int64_t*>(ptr);
    }


  var = ir_value(i);
}


void
ir_processor::
operate_st(std::string_view  instr, const std::vector<ir_operand>&  opls)
{
  auto  addr_v = evaluate(opls[0]);
  auto     val = evaluate(opls[1]);

    if(!addr_v->is_integer())
    {
      throw ir_error("operate st error: first value is not integer");
    }


  ir_address  addr(addr_v.get_integer());

  auto&  mem = get_variable(addr).get_memory();

  void*  ptr = &mem[addr.get_offset()];

  int64_t  i = val.get_integer();

    if(instr == std::string_view("st8"))
    {
      *static_cast<int8_t*>(ptr) = i;
    }

  else
    if(instr == std::string_view("st16"))
    {
      *static_cast<int16_t*>(ptr) = i;
    }

  else
    if(instr == std::string_view("st32"))
    {
      *static_cast<int32_t*>(ptr) = i;
    }

  else
    if(instr == std::string_view("st64"))
    {
      *static_cast<int64_t*>(ptr) = i;
    }
}


void
ir_processor::
operate_cal(ir_address  retv_addr, const std::vector<ir_operand>&  opls)
{
    if((opls.size() == 2) &&
       opls[0].is_label() &&
       opls[1].is_operand_list())
    {
      std::vector<ir_value>  args;

      auto  fn_name = opls[0].get_string();

        for(auto&  op: opls[1].get_operand_list())
        {
          args.emplace_back(evaluate(op));
        }


      call(retv_addr,fn_name,std::move(args));

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

      auto&  dst = cond.get_integer()? opls[1]
                  :                    opls[2];

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
operate_phi(const std::vector<ir_phi_element>&  phels, ir_variable&  var)
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
          var = evaluate(ph.get_operand());

          return;
        }
    }


  throw ir_error("phi error: all blocks is not matched");
}




}




