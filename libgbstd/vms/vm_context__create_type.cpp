#include"libgbstd/vm.hpp"
#include"libgbstd/parser.hpp"




namespace gbstd{


using namespace typesystem;


namespace{


const type_info*
read_type(context&  ctx, token_block_view&  bv) noexcept;


parameter_list
read_parameter_list(context&  ctx, const token_block&  blk) noexcept
{
  auto&  tc = ctx.get_type_collection();

  token_block_view  bv(blk);

  parameter_list  ls;

    while(bv)
    {
      auto  ti = read_type(ctx,bv);

        if(ti)
        {
          ls.push(*ti);

            if(bv->is_identifier())
            {
              ++bv;
            }


            if(bv->is_operator_code(","))
            {
              ++bv;
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

  token_block_view  bv(blk);

  struct_type_info  sti;

    while(bv)
    {
      auto  ti = read_type(ctx,bv);

        if(ti)
        {
            if(bv->is_identifier())
            {
              sti.push(*ti,bv++->get_string());

                if(bv->is_operator_code(";"))
                {
                  ++bv;
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

  token_block_view  bv(blk);

  union_type_info  uti;

    while(bv)
    {
      auto  ti = read_type(ctx,bv);

        if(ti)
        {
            if(bv->is_identifier())
            {
              uti.push(*ti,bv++->get_string());

                if(bv->is_operator_code(";"))
                {
                  ++bv;
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

  token_block_view  bv(blk);

  enum_type_info  eti(gbstd::type_infos::enum_size);

  int  next = 0;

    while(bv)
    {
        if(bv->is_identifier())
        {
          auto&  id = bv++->get_string();

            if(bv->is_operator_code("="))
            {
              ++bv;

                if(bv->is_integer())
                {
                  next = bv++->get_integer();

                  eti.push(id,next++);
                }
            }

          else
            {
              eti.push(id,next++);
            }

            
            if(bv->is_operator_code(","))
            {
              ++bv;
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
read_derived_type(context&  ctx, token_block_view&  bv, std::string_view  name) noexcept
{
  auto&  tc = ctx.get_type_collection();

  auto  ti = tc.find_by_name(name);

    if(!ti)
    {
      goto END;
    }


    while(bv)
    {
        if(bv->is_block("[","]"))
        {
          auto&  blk = bv++->get_block();

            if((blk->size() == 1) && blk[0].is_integer())
            {
              ti = &ti->get_derivation().get_array_type(blk[0].get_integer());
            }

          else
            {
              report;

              return nullptr;
            }
        }

      else
        if(bv->is_operator_code("*"))
        {
          ti = &ti->get_derivation().get_pointer_type(gbstd::type_infos::pointer_size);

          ++bv;
        }

      else
        if(bv->is_operator_code("&"))
        {
          ti = &ti->get_derivation().get_reference_type(gbstd::type_infos::pointer_size);

          ++bv;
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
read_type(context&  ctx, token_block_view&  bv) noexcept
{
  auto&  tc = ctx.get_type_collection();

    if(!bv->is_identifier())
    {
      return nullptr;
    }


  auto&  name = bv++->get_string();

    if(name == std::string_view("struct"))
    {
        if(bv->is_block("{","}"))
        {
          return &ctx.append_type_info(std::make_unique<type_info>(read_struct_type(ctx,bv++->get_block())));
        }

      else
        {
          return nullptr;
        }
    }

  else
    if(name == std::string_view("union"))
    {
        if(bv->is_block("{","}"))
        {
          return &ctx.append_type_info(std::make_unique<type_info>(read_union_type(ctx,bv++->get_block())));
        }

      else
        {
          return nullptr;
        }
    }

  else
    if(name == std::string_view("enum"))
    {
        if(bv->is_block("{","}"))
        {
          return &ctx.append_type_info(std::make_unique<type_info>(read_enum_type(ctx,bv++->get_block())));
        }

      else
        {
          return nullptr;
        }
    }

  else
    if(name == std::string_view("function"))
    {
      auto  ti = read_type(ctx,bv);

        if(ti)
        {
            if(bv->is_block("(",")"))
            {
              return &ti->get_derivation().get_function_type(read_parameter_list(ctx,bv++->get_block()));
            }
        }


      return nullptr;
    }


  return read_derived_type(ctx,bv,name);
}


}




const type_info*
context::
create_type_from_string(std::string_view  sv) noexcept
{
  token_block  blk(sv);

  token_block_view  bv(blk);

  return read_type(*this,bv);
}




}




