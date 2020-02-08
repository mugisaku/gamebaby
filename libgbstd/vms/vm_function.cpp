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
append_store_instruction(operand  dst, operand  src) noexcept
{
  m_codelines.emplace_back(*this,store_instruction(std::move(dst),std::move(src)));

  return *this;
}


function&
function::
append_branch_instruction(operand  cond, std::string_view  lb) noexcept
{
  m_codelines.emplace_back(*this,branch_instruction(std::move(cond),lb));

  return *this;
}


function&
function::
append_return_instruction(operand  o) noexcept
{
  m_codelines.emplace_back(*this,return_instruction(std::move(o)));

  return *this;
}


function&
function::
append_operation(operation  op) noexcept
{
  m_codelines.emplace_back(*this,std::move(op));

  return *this;
}




function&
function::
append_declaration(std::string_view  type_name, std::string_view  var_name) noexcept
{
  m_declaration_list.emplace_back(*m_context.get_type_collection().find_by_name(type_name),var_name);

  return *this;
}


function&
function::
append_entry_point(std::string_view  lb) noexcept
{
  m_entry_point_list.emplace_back(lb,m_codelines.size());

  return *this;
}


const operation*
function::
find_operation(std::string_view  label) const noexcept
{
    for(auto&  cl: m_codelines)
    {
        if(cl.is_operation())
        {
          auto&  op = cl.get_operation();

            if(op.get_operand_list()[0].get_identifier() == label)
            {
              return &op;
            }
        }
    }


  return nullptr;
}


const entry_point*
function::
find_entry_point(std::string_view  label) const noexcept
{
    for(auto&  ep: m_entry_point_list)
    {
        if(ep.get_label() == label)
        {
          return &ep;
        }
    }


  return nullptr;
}


void
function::
resolve(const context&  ctx, operand&  o) const noexcept
{
    if(!o.is_identifier())
    {
      return;
    }


  auto&  name = o.get_identifier();

  auto  fn = ctx.find_function(name);

    if(fn)
    {
      o = multi_pointer('f',fn->get_address());

      return;
    }


  uint32_t  i = 0;

    for(auto&  id: m_argument_name_list)
    {
        if(id == name)
        {
          o = multi_pointer('l',i);

          return;
        }


      ++i;
    }


    for(auto&  decl: m_declaration_list)
    {
        if(decl.get_name() == name)
        {
          o = multi_pointer('l',i);

          return;
        }


      ++i;
    }


  auto  var = ctx.find_variable(name);

    if(var)
    {
      o = multi_pointer('g',var->get_address());

      return;
    }


  auto  ep = find_entry_point(name);

    if(ep)
    {
      o = ep->get_value();

      return;
    }


  report;
}


void
function::
finalize(const context&  ctx) noexcept
{
    if(m_finalized)
    {
      return;
    }


    for(auto&  codeln: m_codelines)
    {
        if(codeln.is_return_instruction())
        {
          resolve(ctx,codeln.get_return_instruction().get_operand());
        }

      else
        if(codeln.is_store_instruction())
        {
          auto&  st = codeln.get_store_instruction();

          resolve(ctx,st.get_destination());
          resolve(ctx,st.get_source()     );
        }

      else
        if(codeln.is_branch_instruction())
        {
          auto&  br = codeln.get_branch_instruction();

          resolve(ctx,br.get_condition()  );
          resolve(ctx,br.get_destination());
        }

      else
        if(codeln.is_operation())
        {
          auto&  op = codeln.get_operation();

            for(auto&  o: op.get_operand_list())
            {
              resolve(ctx,o);
            }
        }
    }


  m_finalized = true;
}


void
function::
print(const context*  ctx) const noexcept
{
  m_signature.get_return_type_info().print();  

  m_signature.get_parameter_list().print();  

  printf(" %s{\n",m_name.data());

    for(auto&  cl: m_codelines)
    {
      cl.print(ctx,this);

      printf("\n");
    }


  printf("}\n\n\n");
}




}




