#include"libgbstd/vm.hpp"




namespace gbstd{




struct
context::
frame
{
  frame*  m_previous=nullptr;

  const function&  m_function;

  address_t  m_store_pointer;

  statement_cursor m_cursor;

  frame(const function&  fn) noexcept:
  m_function(fn), m_cursor(fn.get_block()){}

  void  print() const noexcept
  {
/*
    auto&  pos = m_position_stack.back();

    printf("%s depth: %4d, pos: %4d\n",
         m_function.get_name().data(),
    (int)m_position_stack.size(),
    (int)(pos.m_current-pos.m_begin)
    );
*/
  }

};


bool
context::
step() noexcept
{
    if(m_current_frame)
    {
      auto&  cur = m_current_frame->m_cursor;

        if(cur)
        {
          auto&  st = cur.get();

            if(st)
            {
              cur.advance();

              process(st);
            }

          else
            {
              exit_block();
            }
        }

      else
        {
          pop_frame();
        }


      return true;
    }


  return false;
}



void
context::
push_frame(address_t  st_p, const function&  fn, int  argc, const expression*  argv) noexcept
{
  auto  paras = fn.get_parameter_list().begin();

    while(argc--)
    {
      auto&  decl = *paras++;
/*
      auto&  dst_var = push_variable(fn.get_name().data(),ti,name);

      auto  dst_ti = &dst_var.get_value().get_type_info();

      auto    src_v = argv++->evaluate(*this);
      auto&  src_ti = src_v.get_type_info();

        if(src_ti == *dst_ti)
        {
          dst_var.get_value() = src_v;
        }

      else
        {
            if(src_ti.is_reference())
            {
                if(src_ti.get_reference_type_info().get_base_type_info() == *dst_ti)
                {
                  auto  p = multi_pointer(src_v.get_unsigned_integer());

                  auto&  var = get_variable(p.get_major());

                  dst_var.get_value() = var.get_value();
                }
            }
        }
*/
    }


    if(m_current_frame)
    {
      m_current_frame->m_store_pointer = st_p;
    }


  auto  new_frame = new frame(fn);

  new_frame->m_previous = m_current_frame            ;
                          m_current_frame = new_frame;

  ++m_number_of_frames;
}


void
context::
pop_frame() noexcept
{
    while(m_current_frame->m_cursor)
    {
      exit_block();
    }


  auto  old_frame = m_current_frame                        ;
                    m_current_frame = old_frame->m_previous;

  delete old_frame;

  --m_number_of_frames;
}


void
context::
exit_block() noexcept
{
  auto&  blk = m_current_frame->m_cursor.pop();

  int  n = blk.get_symbol_table().get_number_of_symbols();

    while(n--)
    {
      m_runtime_symbol_table.pop();
    }
}




void
context::
process(const statement&  st) noexcept
{
       if(st.is_return()    ){process(st.get_return());}
  else if(st.is_jump()      ){process(st.get_jump());}
  else if(st.is_label()     ){process(st.get_label());}
  else if(st.is_block()     ){process(st.get_block());}
  else if(st.is_expression()){process(st.get_expression());}
  else if(st.is_if_string() ){process(st.get_if_string());}
  else if(st.is_control()   ){process(st.get_control());}
}


void
context::
process(const return_statement&  st) noexcept
{
  auto  val = st.get_expression().evaluate(m_runtime_symbol_table,m_memory);

  pop_frame();

    if(0)
    {
//      get_variable(m_current_frame->m_store_pointer).get_value() = std::move(val);
    }

  else
    {
/*
      auto&  val0 = m_variable_table[0].get_value();

      val0 = std::move(val);

      printf("\n====\n");

      val0.print();

      printf("\n====\n");
*/
    }
}


void
context::
process(const jump_statement&  st) noexcept
{
}


void
context::
process(const label_statement&  st) noexcept
{
}


void
context::
process(const block_statement&  st) noexcept
{
  m_current_frame->m_cursor.push(st);

    for(auto&  sym: st.get_symbol_table())
    {
      m_runtime_symbol_table.push(sym);
    }
}


void
context::
process(const control_statement&  st) noexcept
{
  auto&  s = st.get_string();

    if(s == "break")
    {
    }

  else
    if(s == "continue")
    {
    }

  else
    if(s == "interrupt")
    {
    }

  else
    {
    }
}


void
context::
process(const if_string_statement&  st) noexcept
{
}


void
context::
process(const expression_statement&  st) noexcept
{
  auto  v = st.get_expression().evaluate(m_runtime_symbol_table,m_memory);

  auto&  s = st.get_name();

  auto  sym = m_runtime_symbol_table.find(s);

    if(sym)
    {
//      m_variable_table[get_base_index()+sym->get_address()].get_value() = v;
    }

  else
    {
    }
}




}




