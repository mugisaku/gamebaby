#include"libgbstd/vm.hpp"




namespace gbstd{




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




function&
function::
append_declaration(std::string_view  type_name, std::string_view  var_name) noexcept
{
  m_declaration_list.emplace_back(*m_context.get_type_collection().find_by_name(type_name),var_name);

  return *this;
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


value
function::
get_value() const noexcept
{
  auto&  tc = m_context.get_type_collection();

  return m_type_info.get_derivation().get_reference_type(tc.get_pointer_size());
}


void
function::
print() const noexcept
{
  auto&  tc = m_context.get_type_collection();

  auto&  sig = m_type_info.get_function_signature();

  auto  retti_ent = tc.find_entry(sig.get_return_type_info());

  printf("%s(",retti_ent->get_name().data());

  auto  argnams_it = m_argument_name_list.begin();

    for(auto&  para: sig.get_parameter_list())
    {
      auto  ti_ent = tc.find_entry(*para);

      printf("%s  %s, ",ti_ent->get_name().data(),argnams_it++->data());
    }


  printf(")\n%s\n{\n",m_name.data());

    for(auto&  decl: m_declaration_list)
    {
      printf("  %s  %s\n",tc.find_entry(*decl.get_type_info())->get_name().data(),decl.get_name().data());
    }


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




