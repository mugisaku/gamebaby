#include"libgbstd/vms/ir.hpp"




namespace gbstd{




void
ir_processor::
assign(const ir_context&  ctx)
{
  m_context = &ctx;

  reset();
}




ir_address
ir_processor::
find_variable(std::string_view  label) noexcept
{
    if(m_frame_stack.size())
    {
        for(auto&  ln: m_frame_stack.back().m_local_link_table)
        {
            if(ln.get_name() == label)
            {
              return ir_address(ln.get_index(),0);
            }
        }
    }


    for(auto&  ln: m_global_link_table)
    {
        if(ln.get_name() == label)
        {
          return ir_address(ln.get_index(),0);
        }
    }


  return ir_address();
}




ir_value
ir_processor::
evaluate(const ir_operand&  o)
{
    if(o.is_integer())
    {
      return ir_value(o.get_integer());
    }

  else
    if(o.is_label())
    {
      auto&  lb = o.get_string();

      auto  addr = find_variable(lb);

        if(addr)
        {
          return get_variable(addr);
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

  else
    {
      throw ir_error(form_string("jump error: %s block is missing",label.data()));
    }
}


void
ir_processor::
call(ir_address  retv_addr, std::string_view  fn_name, std::vector<ir_value>&&  args)
{
  auto  fn = m_context->find_function(fn_name);

    if(fn)
    {
      auto&  frm = m_frame_stack.emplace_back();

      frm.m_return_value_address = retv_addr;

      frm.m_function = fn;

      auto  index = m_variable_table.size();

      auto&  paras = fn->get_parameter_list();

      auto&  lntbl = frm.m_local_link_table;

        if(paras.size() == args.size())
        {
          auto  it = paras.begin();

            for(auto&  v: args)
            {
              m_variable_table.emplace_back(it->get_type_info()).assign(v);

              lntbl.emplace_back(index++,it++->get_label());
            }
        }

      else
        {
          throw ir_error("call error");
        }


        for(auto&  op: fn->get_operation_list())
        {
          auto&  ti = op.get_type_info();

            if(ti.is_integer())
            {
              m_variable_table.emplace_back(ti);

              lntbl.emplace_back(index++,op.get_label());
            }
        }


      frm.m_current =               fn->get_operation_list().data();
      frm.m_end     = frm.m_current+fn->get_operation_list().size();


      auto  biptr = frm.m_current->get_block_info();

      frm.m_previous_block_info = biptr;
      frm.m_current_block_info  = biptr;

      return;
    }


  throw ir_error("call error");
}


void
ir_processor::
pop_frame()
{
  auto  n = m_frame_stack.back().m_local_link_table.size();

    while(n--)
    {
      m_variable_table.pop_back();
    }


  m_frame_stack.pop_back();
}


void
ir_processor::
reset()
{
  m_status.clear();
  m_frame_stack.clear();
  m_variable_table.clear();
  m_global_link_table.clear();

  m_variable_table.emplace_back(ir_type_info());

  auto  index = 1;

    for(auto&  vi: m_context->get_variable_info_list())
    {
      auto&  ti = vi.get_type_info();

      auto&  mem = vi.get_memory();

        if(mem)
        {
          m_variable_table.emplace_back(ir_value(ti,mem.clone()));
        } 

      else
        {
          m_variable_table.emplace_back(ti);
        }


      m_global_link_table.emplace_back(index++,vi.get_name());
    }


  print();
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

          auto  bi = op.get_block_info();

            if(frm.m_current_block_info != bi)
            {
              frm.m_previous_block_info = frm.m_current_block_info     ;
                                          frm.m_current_block_info = bi;
            }


            if(m_status.test(flags::log))
            {
              op.print();
              printf("\n");
            }


          operate(op);
        }

      else
        {
          pop_frame();
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




void
ir_processor::
print_global_variables() const noexcept
{
  printf("global link table{\n");

    for(auto&  ln: m_global_link_table)
    {
      auto&  var = get_variable(ln.get_index());

      ln.print();

      printf(": ");

      var.print();

      printf("\n");
    }
}


void
ir_processor::
print() const noexcept
{
  print_global_variables();

  printf("call stack{\n");

    for(auto&  frm: m_frame_stack)
    {
      printf("%s->\n",frm.m_function->get_name().data());
    }


  printf("}\n");

    if(m_frame_stack.size())
    {
      printf("local link table{\n");

        for(auto&  ln: m_frame_stack.back().m_local_link_table)
        {
          auto&  var = get_variable(ln.get_index());

          ln.print();

          printf(": ");

          var.print();

          printf("\n");
        }


      printf("}\n");
    }
}




}




