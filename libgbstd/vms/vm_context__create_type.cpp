#include"libgbstd/vm.hpp"
#include"libgbstd/parser.hpp"




namespace gbstd{


using namespace typesystem;


namespace{


const type_info*
read_type(context&  ctx, token_iterator&  it) noexcept;


parameter_list
read_parameter_list(context&  ctx, const token_block&  blk) noexcept
{
  auto&  tc = ctx.get_type_collection();

  token_iterator  it(blk);

  parameter_list  ls;

    while(it)
    {
      auto  ti = read_type(ctx,it);

        if(ti)
        {
          ls.push(*ti);

            if(it->is_identifier())
            {
              ++it;
            }


            if(it->is_operator_code(","))
            {
              ++it;
            }
        }

      else
        {
          break;
        }
    }


  return std::move(ls);
}


struct_type_info
read_struct_type(context&  ctx, const token_block&  blk) noexcept
{
  auto&  tc = ctx.get_type_collection();

  token_iterator  it(blk);

  struct_type_info  sti;

    while(it)
    {
      auto  ti = read_type(ctx,it);

        if(ti)
        {
            if(it->is_identifier())
            {
              sti.push(*ti,it++->get_string());

                if(it->is_operator_code(";"))
                {
                  ++it;
                }
            }

          else
            {
              break;
            }
        }

      else
        {
          break;
        }
    }


  return std::move(sti);
}


union_type_info
read_union_type(context&  ctx, const token_block&  blk) noexcept
{
  auto&  tc = ctx.get_type_collection();

  token_iterator  it(blk);

  union_type_info  uti;

    while(it)
    {
      auto  ti = read_type(ctx,it);

        if(ti)
        {
            if(it->is_identifier())
            {
              uti.push(*ti,it++->get_string());

                if(it->is_operator_code(";"))
                {
                  ++it;
                }
            }

          else
            {
              break;
            }
        }

      else
        {
          break;
        }
    }


  return std::move(uti);
}


enum_type_info
read_enum_type(context&  ctx, const token_block&  blk) noexcept
{
  auto&  tc = ctx.get_type_collection();

  token_iterator  it(blk);

  enum_type_info  eti(gbstd::type_infos::enum_size);

  int  next = 0;

    while(it)
    {
        if(it->is_identifier())
        {
          auto&  id = it++->get_string();

            if(it->is_operator_code("="))
            {
              ++it;

                if(it->is_integer())
                {
                  next = it++->get_integer();

                  eti.push(id,next++);
                }
            }

          else
            {
              eti.push(id,next++);
            }

            
            if(it->is_operator_code(","))
            {
              ++it;
            }
        }

      else
        {
          break;
        }
    }


  return std::move(eti);
}


const type_info*
read_derived_type(context&  ctx, token_iterator&  it, std::string_view  name) noexcept
{
  auto&  tc = ctx.get_type_collection();

  auto  ti = tc.find_by_name(name);

    if(!ti)
    {
      goto END;
    }


    while(it)
    {
        if(it->is_block("[","]"))
        {
          auto&  blk = it++->get_block();

            if((blk->size() == 1) && blk[0].is_integer())
            {
              ti = &ti->form_array_type(blk[0].get_integer());
            }

          else
            {
              report;

              return nullptr;
            }
        }

      else
        if(it->is_operator_code("*"))
        {
          ti = &ti->form_pointer_type(gbstd::type_infos::pointer_size);

          ++it;
        }

      else
        if(it->is_operator_code("&"))
        {
          ti = &ti->form_reference_type(gbstd::type_infos::pointer_size);

          ++it;
        }

      else
        {
          break;
        }
    }


END:
  return ti;
}


const type_info*
read_type(context&  ctx, token_iterator&  it) noexcept
{
  auto&  tc = ctx.get_type_collection();

    if(!it->is_identifier())
    {
      return nullptr;
    }


  auto&  name = it++->get_string();

    if(name == std::string_view("struct"))
    {
        if(it->is_block("{","}"))
        {
          return &ctx.append_type_info(std::make_unique<type_info>(read_struct_type(ctx,it++->get_block())));
        }

      else
        {
          return nullptr;
        }
    }

  else
    if(name == std::string_view("union"))
    {
        if(it->is_block("{","}"))
        {
          return &ctx.append_type_info(std::make_unique<type_info>(read_union_type(ctx,it++->get_block())));
        }

      else
        {
          return nullptr;
        }
    }

  else
    if(name == std::string_view("enum"))
    {
        if(it->is_block("{","}"))
        {
          return &ctx.append_type_info(std::make_unique<type_info>(read_enum_type(ctx,it++->get_block())));
        }

      else
        {
          return nullptr;
        }
    }

  else
    if(name == std::string_view("function"))
    {
      auto  ti = read_type(ctx,it);

        if(ti)
        {
            if(it->is_block("(",")"))
            {
              function_signature  fnsig(*ti,read_parameter_list(ctx,it++->get_block()));

              function_pointer_type_info  fnptr_ti(std::move(fnsig),type_infos::pointer_size);

              return &ctx.append_type_info(std::make_unique<type_info>(std::move(fnptr_ti)));
            }
        }


      return nullptr;
    }


  return read_derived_type(ctx,it,name);
}


}




const type_info*
context::
create_type_from_string(std::string_view  sv) noexcept
{
  token_block  blk(sv);

  token_iterator  it(blk);

  return read_type(*this,it);
}




}




