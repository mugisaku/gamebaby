#include"libgbstd/vms/ir.hpp"




namespace gbstd{




template<class  T>
pointer_wrapper<ir_register>
find(T&  map, std::string_view  label) noexcept
{
    for(auto&  reg: map)
    {
        if(reg == label)
        {
          return &reg;
        }
    }


  return &map.emplace_back(label);
}




void
ir_processor::
assign(const ir_context&  ctx) noexcept
{
  m_context = &ctx;

  reset();
}


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
    if(opco == operator_code("add"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);
      
      regptr->set_value(l+r);
    }

  else
    if(opco == operator_code("sub"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      regptr->set_value(l-r);
    }

  else
    if(opco == operator_code("mul"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      regptr->set_value(l*r);
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


      regptr->set_value(l/r);
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


      regptr->set_value(l%r);
    }

  else
    if(opco == operator_code("shl"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      regptr->set_value(l<<r);
    }

  else
    if(opco == operator_code("shr"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      regptr->set_value(l>>r);
    }

  else
    if(opco == operator_code("bit_or"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      regptr->set_value(l|r);
    }

  else
    if(opco == operator_code("bit_and"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      regptr->set_value(l&r);
    }

  else
    if(opco == operator_code("bit_xor"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      regptr->set_value(l^r);
    }

  else
    if(opco == operator_code("bit_not"))
    {
      auto  l = evaluate(opls[0]);

      regptr->set_value(~l);
    }

  else
    if(opco == operator_code("eq"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      regptr->set_value(l == r);
    }

  else
    if(opco == operator_code("neq"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      regptr->set_value(l != r);
    }

  else
    if(opco == operator_code("lt"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      regptr->set_value(l < r);
    }

  else
    if(opco == operator_code("lteq"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      regptr->set_value(l <= r);
    }

  else
    if(opco == operator_code("gt"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      regptr->set_value(l > r);
    }

  else
    if(opco == operator_code("gteq"))
    {
      auto  l = evaluate(opls[0]);
      auto  r = evaluate(opls[1]);

      regptr->set_value(l >= r);
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
      operate_phi(opls,*regptr);
    }

  else
    if(opco == operator_code("ld8"))
    {
      auto  addr = evaluate(opls[0]);

      regptr->set_value(reinterpret_cast<int8_t&>(m_memory[addr]));
    }

  else
    if(opco == operator_code("ld16"))
    {
      auto  addr = evaluate(opls[0]);

      regptr->set_value(reinterpret_cast<int16_t&>(m_memory[addr]));
    }

  else
    if(opco == operator_code("ld32"))
    {
      auto  addr = evaluate(opls[0]);

      regptr->set_value(reinterpret_cast<int32_t&>(m_memory[addr]));
    }

  else
    if(opco == operator_code("ld64"))
    {
      auto  addr = evaluate(opls[0]);

      regptr->set_value(reinterpret_cast<int64_t&>(m_memory[addr]));
    }

  else
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

  else
    {
      throw ir_error("operate error, unknown operator");
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
operate_phi(const std::vector<ir_operand>&  opls, ir_register&  reg)
{
  auto  bi = m_frame_stack.back().m_previous_block_info;

    for(auto&  o: opls)
    {
        if(o.is_phi_element())
        {
          auto&  e = o.get_phi_element();

            if(bi->test_label(e.get_label()))
            {
              auto  v = evaluate(e.get_operand());

              reg.set_value(v);

              return;
            }
        }
    }


  throw ir_error("phi error");
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

      auto  regptr = find(m_frame_stack.back().m_register_map,lb);

        if(regptr)
        {
          return regptr->get_value();
        }


      printf("%s not found in %s",lb.data(),m_frame_stack.back().m_function->get_name().data());
    }


  throw ir_error("evaluate error");
}


void
ir_processor::
jump(std::string_view  label)
{
  auto&  frm = m_frame_stack.back();

  auto  bi = frm.m_function->find_block_info(label);

    if(bi)
    {
      m_frame_stack.back().m_current = bi->get_entry();
    }
}


void
ir_processor::
call(pointer_wrapper<ir_register>  retreg, std::string_view  fn_name, const std::vector<int64_t>&  args)
{
  auto  fn = m_context->find_function(fn_name);

    if(fn)
    {
      auto&  frm = m_frame_stack.emplace_back();

      frm.m_return_register = retreg;

      frm.m_function = fn;

      auto&  paras = fn->get_parameter_list();

        if(paras.size() == args.size())
        {
          auto  it = paras.begin();

          auto&  regmap = frm.m_register_map;

            for(auto&  v: args)
            {
              regmap.emplace_back(it++->get_label(),v);
            }
        }

      else
        {
          throw ir_error("call error");
        }


      frm.m_current =               fn->get_operation_list().data();
      frm.m_end     = frm.m_current+fn->get_operation_list().size();


      auto  biptr = &frm.m_current->get_block_info();

      frm.m_previous_block_info = biptr;
      frm.m_current_block_info  = biptr;

      return;
    }


  throw ir_error("call error");
}


void
ir_processor::
reset() noexcept
{
  m_frame_stack.clear();
}


void
ir_processor::
step()
{
    if(m_frame_stack.size())
    {
      auto&  frm = m_frame_stack.back();

        if(frm.m_current < frm.m_end)
        {
          auto&  op = *frm.m_current++;

          auto&  bi = op.get_block_info();

            if(frm.m_current_block_info != &bi)
            {
              frm.m_previous_block_info = frm.m_current_block_info      ;
                                          frm.m_current_block_info = &bi;
            }


          operate(op);
        }

      else
        {
          m_frame_stack.pop_back();
        }
    }
}


void
ir_processor::
run()
{
    while(m_frame_stack.size())
    {
      step();
    }
}




}




