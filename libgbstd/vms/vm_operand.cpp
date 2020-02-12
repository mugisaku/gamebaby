#include"libgbstd/vm.hpp"




namespace gbstd{




operand&
operand::
assign(const operand&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::identifier     ): new(&m_data) std::string(rhs.m_data.s);break;
      case(kind::integer_literal): new(&m_data) int64_t(rhs.m_data.i);break;
      case(kind::pointer_literal): new(&m_data) multi_pointer(rhs.m_data.p);break;
        }
    }


  return *this;
}


operand&
operand::
assign(operand&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind ,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::identifier     ): new(&m_data) std::string(std::move(rhs.m_data.s));break;
      case(kind::integer_literal): new(&m_data) int64_t(std::move(rhs.m_data.i));break;
      case(kind::pointer_literal): new(&m_data) multi_pointer(std::move(rhs.m_data.p));break;
        }
    }


  return *this;
}


operand&
operand::
assign(std::string_view  sv) noexcept
{
  clear();

  new(&m_data) std::string(sv);

  m_kind = kind::identifier;

  return *this;
}


operand&
operand::
assign(int64_t  i) noexcept
{
  clear();

  new(&m_data) int64_t(i);

  m_kind = kind::integer_literal;

  return *this;
}


operand&
operand::
assign(multi_pointer  p) noexcept
{
  clear();

  new(&m_data) multi_pointer(p);

  m_kind = kind::pointer_literal;

  return *this;
}


void
operand::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::null): break;
  case(kind::identifier     ): m_data.s.~basic_string();break;
  case(kind::integer_literal): /*m_data.i.~int()*/;break;
  case(kind::pointer_literal): m_data.p.~multi_pointer();break;
    }


  m_kind = kind::null;
}


value
operand::
evaluate(context&  ctx) const noexcept
{
  auto&  tc = ctx.get_type_collection();

    if(is_integer_literal())
    {
      return ctx.make_value(m_data.i);
    }

  else
    if(is_identifier())
    {
      printf("%s is not resolved",m_data.s.data());
    }

  else
    if(is_pointer_literal())
    {
      auto  p = get_pointer();

      auto  maj = get_pointer().get_major();

      auto&  tc = ctx.get_type_collection();

        if(maj.is_null())
        {
          return ctx.make_value(nullptr);
        }

      else
        if(maj.is_global() || maj.is_local())
        {
          auto&  val = ctx.get_variable(maj).get_value();

          auto  ti = &val.get_type_info();

            if(ti->is_reference())
            {
              p = multi_pointer(val.get_unsigned_integer());
            }

          else
            {
              ti = &val.get_type_derivation().get_reference_type(tc.get_pointer_size());
            }


          return value(*ti,p.get_packed());
        }

      else
        if(maj.is_function())
        {
          auto&  fn = ctx.get_function(maj);
	
          return value(fn.get_type_info(),m_data.p.get_packed());
        }
    }


  return value(ctx.get_void_type_info());
}


void
operand::
print(const context*  ctx, const function*  fn) const noexcept
{
    switch(m_kind)
    {
  case(kind::null): break;
  case(kind::identifier     ): printf("%s",m_data.s.data());break;
  case(kind::integer_literal): printf("%" PRIi64,m_data.i);break;
  case(kind::pointer_literal):
        if(ctx)
        {
          auto  maj = m_data.p.get_major();

            if(maj.is_function())
            {
              printf("%s",ctx->get_function(maj).get_name().data());
            }

          else
            if(maj.is_global())
            {
              printf("%s",ctx->get_variable(maj).get_name().data());
            }

          else
            if(maj.is_local())
            {
                if(fn)
                {
                  auto&   args = fn->get_argument_name_list();
                  auto&  decls = fn->get_declaration_list();

                  auto  num_args = args.size();

                  auto  i = maj.get();

                    if(i < num_args)
                    {
                      printf("%s",args[i].data());
                    }

                  else
                    if(i < num_args+decls.size())
                    {
                      printf("%s",decls[i-num_args].get_name().data());
                    }

                  else
                    {
                      printf("ERROR: operand print multi pointer\n");
                    }
                }

              else
                {
                  printf("Error:");
                }
            }

          else
            if(maj.is_null())
            {
              printf("nullptr");
            }
        }

      else
        {
          printf("%" PRIu32,m_data.p.get_major().get());
        }
      break;
    }
}




}




