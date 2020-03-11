#include"libgbstd/vm.hpp"




namespace gbstd{




function::
function() noexcept:
m_block(*this)
{
}




function&
function::
set_signature(function_signature&&  sig) noexcept
{
  m_type_info = std::make_unique<type_info>(function_pointer_type_info(std::move(sig),type_infos::function_pointer_size));

  return *this;
}


const function_signature&
function::
get_signature() const noexcept
{
  return m_type_info->get_function_pointer_type_info().get_signature();
}


function&
function::
set_argument_name_list(std::vector<std::string_view>&&  argnams) noexcept
{
  m_argument_name_list.clear();

    for(auto  sv: argnams)
    {
      m_argument_name_list.emplace_back(sv);
    }


  return *this;
}


void
function::
make_definition(const token_block&  blk) noexcept
{
  token_iterator  it(blk);

  block_statement  blkst(*this);

    while(it)
    {
      auto&  tok = *it++;

        if(tok.is_identifier())
        {
          auto&  first = tok.get_string();

            if(first == "return")
            {
              blkst.push(statement(new return_statement()));
            }

          else
            if(first == "jump")
            {
                if(it->is_identifier())
                {
                  blkst.push(statement(new jump_statement(it++->get_string())));
                }

              else
                {
                  report;
                }
            }

          else
            if((first ==     "break") ||
               (first ==  "continue") ||
               (first == "interrupt"))
            {
              blkst.push(statement(new control_statement(first)));
            }

          else
            if(first == "if")
            {
            }

          else
            if(first == "while")
            {
            }

          else
            if(first == "for")
            {
            }

          else
            {
            }
        }

      else
        if(tok.is_operator_code(";"))
        {
        }

      else
        {
          report;
        }
    }


  m_block = std::move(blkst);
}


void
function::
make_definition(std::string_view  sv) noexcept
{
  token_block  blk(sv);

  make_definition(blk);
}


/*
void
function::
resolve(operand&  o) const noexcept
{
    if(!o.is_identifier())
    {
      return;
    }


  auto&  name = o.get_string();

  auto  fn = m_context.find_function(name);

    if(fn)
    {
      o = multi_pointer(fn->get_address(),0);

      return;
    }


  uint32_t  i = 0;

    for(auto&  id: m_argument_name_list)
    {
        if(id == name)
        {
          o = multi_pointer(major_address('l',i),0);

          return;
        }


      ++i;
    }


    for(auto&  decl: m_declaration_list)
    {
        if(decl.get_name() == name)
        {
          o = multi_pointer(major_address('l',i),0);

          return;
        }


      ++i;
    }


  auto  var = m_context.find_variable(name);

    if(var)
    {
      o = multi_pointer(var->get_address(),0);

      return;
    }


  auto  ep = find_entry_point(name);

    if(ep)
    {
      o = static_cast<uint64_t>(ep->get_value());

      return;
    }

  report;
}
*/


void
function::
finalize() noexcept
{
    if(m_finalized)
    {
      return;
    }


/*
    for(auto&  codeln: m_codelines)
    {
        if(codeln.is_return_instruction())
        {
          resolve(codeln.get_return_instruction().get_operand());
        }

      else
        if(codeln.is_store_instruction())
        {
          auto&  st = codeln.get_store_instruction();

          resolve(st.get_destination());
          resolve(st.get_source()     );
        }

      else
        if(codeln.is_branch_instruction())
        {
          auto&  br = codeln.get_branch_instruction();

          resolve(br.get_condition()  );
          resolve(br.get_destination());
        }

      else
        if(codeln.is_operation())
        {
          auto&  op = codeln.get_operation();

            for(auto&  o: op.get_operand_list())
            {
              resolve(o);
            }
        }
    }


*/
  m_finalized = true;
}


void
function::
print(const context&  ctx) const noexcept
{
  auto&  tc = ctx.get_type_collection();

  auto&  sig = get_signature();

  auto  retti_ent = tc.find_entry(sig.get_return_type_info());

  printf("%s(",retti_ent->get_name().data());

  auto  argnams_it = m_argument_name_list.begin();

    for(auto&  para: sig.get_parameter_list())
    {
      auto  ti_ent = tc.find_entry(*para);

      printf("%s  %s, ",ti_ent->get_name().data(),argnams_it++->data());
    }


  printf(")\n{\n");
/*

    for(auto&  decl: m_declaration_list)
    {
      printf("  %s  %s\n",tc.find_entry(*decl.get_type_info())->get_name().data(),decl.get_name().data());
    }
*/

  printf("\n");

  int  i = 0;

/*
    for(auto&  cl: m_codelines)
    {
      printf("%4d  ",i++);

      cl.print(&m_context,this);

      printf("\n");
    }
*/


  printf("}\n\n\n");
}




}




