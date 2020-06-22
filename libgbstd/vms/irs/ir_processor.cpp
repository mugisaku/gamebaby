#include"libgbstd/vms/ir.hpp"




namespace gbstd{




void
ir_processor::
assign(const ir_context&  ctx) noexcept
{
  m_context = &ctx;

  reset();
}




pointer_wrapper<ir_register>
ir_processor::
find(std::vector<ir_register>&  map, std::string_view  label) noexcept
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




