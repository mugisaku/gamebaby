#include"libgbstd/vm.hpp"




namespace gbstd{




namespace unary_operations{
value
bit_not(context&  ctx, const value&  v) noexcept
{
  auto  deref_v = ctx.dereference(v);

  auto&  ti = deref_v.get_type_info();

    if(ti.is_kind_of_integer())
    {
      return ctx.make_value(~deref_v.get_integer());
    }


  return value();
}


value
logical_not(context&  ctx, const value&  v) noexcept
{
  auto  deref_v = ctx.dereference(v);

  auto&  ti = deref_v.get_type_info();

    if(ti.is_like_boolean())
    {
      return ctx.make_value(!deref_v.get_integer());
    }


  return value();
}


value
get_size(context&  ctx, const value&  v) noexcept
{
  auto  deref_v = ctx.dereference(v);

  auto&  ti = deref_v.get_type_info();

  return ctx.make_value(static_cast<uint64_t>(ti.get_size()));
}


value
get_address(context&  ctx, const value&  v) noexcept
{
    if(v.get_type_info().is_reference())
    {
      return ctx.make_value(v.get_unsigned_integer());
    }


  return value();
}




callback
get_callback(unary_opcodes  op) noexcept
{
  constexpr callback  table[] =
  {
    bit_not,
    logical_not,
    get_size,
    get_address,
  };


  return table[static_cast<int>(op)];
}
}




namespace binary_operations{
value
add(context&  ctx, const value&  lv, const value&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer())
    {
      auto  li = lv.get_integer();

        if(rti.is_integer())
        {
          return ctx.make_value(li+rv.get_integer());
        }

      else
        if(rti.is_unsigned_integer())
        {
          return ctx.make_value(li+rv.get_unsigned_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
      auto  li = lv.get_unsigned_integer();

        if(rti.is_integer())
        {
          return ctx.make_value(li+rv.get_integer());
        }

      else
        if(rti.is_unsigned_integer())
        {
          return ctx.make_value(li+rv.get_unsigned_integer());
        }
    }

  else
    if(lti.is_pointer())
    {
      auto  li = lv.get_unsigned_integer();
      auto  sz = lti.get_pointer_type_info().get_base_type_info().get_size();

        if(rti.is_integer())
        {
          return ctx.make_value(li+(sz*rv.get_integer()));
        }

      else
        if(rti.is_unsigned_integer())
        {
          return ctx.make_value(li+(sz*rv.get_unsigned_integer()));
        }
    }


  return value();
}


value
sub(context&  ctx, const value&  lv, const value&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer())
    {
      auto  li = lv.get_integer();

        if(rti.is_integer())
        {
          return ctx.make_value(li-rv.get_integer());
        }

      else
        if(rti.is_unsigned_integer())
        {
          return ctx.make_value(li-rv.get_unsigned_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
      auto  li = lv.get_unsigned_integer();

        if(rti.is_integer())
        {
          return ctx.make_value(li-rv.get_integer());
        }

      else
        if(rti.is_unsigned_integer())
        {
          return ctx.make_value(li-rv.get_unsigned_integer());
        }
    }

  else
    if(lti.is_pointer())
    {
      auto  sz = lti.get_pointer_type_info().get_base_type_info().get_size();

        if(rti.is_pointer() && (lti == rti))
        {
          return ctx.make_value((lv.get_integer()-rv.get_integer())/sz);
        }

      else
        {
          auto  li = lv.get_unsigned_integer();

            if(rti.is_integer())
            {
              return ctx.make_value(li-(sz*rv.get_integer()));
            }

          else
            if(rti.is_unsigned_integer())
            {
              return ctx.make_value(li-(sz*rv.get_unsigned_integer()));
            }
        }
    }


  return value();
}


value
mul(context&  ctx, const value&  lv, const value&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return ctx.make_value( lv.get_integer()
                                *rv.get_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return ctx.make_value( lv.get_unsigned_integer()
                                *rv.get_unsigned_integer());
        }
    }


  return value();
}


value
div(context&  ctx, const value&  lv, const value&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return ctx.make_value( lv.get_integer()
                            /rv.get_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return ctx.make_value( lv.get_unsigned_integer()
                                /rv.get_unsigned_integer());
        }
    }


  return value();
}


value
rem(context&  ctx, const value&  lv, const value&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return ctx.make_value( lv.get_integer()
                                %rv.get_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return ctx.make_value( lv.get_unsigned_integer()
                                %rv.get_unsigned_integer());
        }
    }


  return value();
}


value
shl(context&  ctx, const value&  lv, const value&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer())
    {
        if(rti.is_kind_of_integer())
        {
          return ctx.make_value(  lv.get_integer()
                            <<rv.get_unsigned_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
        if(rti.is_kind_of_integer())
        {
          return ctx.make_value(  lv.get_unsigned_integer()
                            <<rv.get_unsigned_integer());
        }
    }


  return value();
}


value
shr(context&  ctx, const value&  lv, const value&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer())
    {
        if(rti.is_kind_of_integer())
        {
          return ctx.make_value(  lv.get_integer()
                            >>rv.get_unsigned_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
        if(rti.is_kind_of_integer())
        {
          return ctx.make_value(  lv.get_unsigned_integer()
                            >>rv.get_unsigned_integer());
        }
    }


  return value();
}


value
eq(context&  ctx, const value&  lv, const value&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

  auto  li = lv.get_integer();
  auto  ri = rv.get_integer();

    if((lti.is_integer()          && rti.is_integer()         ) ||
       (lti.is_unsigned_integer() && rti.is_unsigned_integer()))
    {
      return ctx.make_value(li == ri);
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return ctx.make_value(li == ri);
    }


  return value();
}


value
neq(context&  ctx, const value&  lv, const value&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

  auto  li = lv.get_integer();
  auto  ri = rv.get_integer();

    if((lti.is_integer()          && rti.is_integer()         ) ||
       (lti.is_unsigned_integer() && rti.is_unsigned_integer()))
    {
      return ctx.make_value(li != ri);
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return ctx.make_value(li != ri);
    }


  return value();
}


value
lt(context&  ctx, const value&  lv, const value&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer() && rti.is_integer())
    {
      return ctx.make_value(lv.get_integer() < rv.get_integer());
    }

  else
    if(lti.is_unsigned_integer() && rti.is_unsigned_integer())
    {
      return ctx.make_value(lv.get_unsigned_integer() < rv.get_unsigned_integer());
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return ctx.make_value(lv.get_unsigned_integer() < rv.get_unsigned_integer());
    }


  return value();
}


value
lteq(context&  ctx, const value&  lv, const value&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer() && rti.is_integer())
    {
      return ctx.make_value(lv.get_integer() <= rv.get_integer());
    }

  else
    if(lti.is_unsigned_integer() && rti.is_unsigned_integer())
    {
      return ctx.make_value(lv.get_unsigned_integer() <= rv.get_unsigned_integer());
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return ctx.make_value(lv.get_unsigned_integer() <= rv.get_unsigned_integer());
    }


  return value();
}


value
gt(context&  ctx, const value&  lv, const value&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer() && rti.is_integer())
    {
      return ctx.make_value(lv.get_integer() > rv.get_integer());
    }

  else
    if(lti.is_unsigned_integer() && rti.is_unsigned_integer())
    {
      return ctx.make_value(lv.get_unsigned_integer() > rv.get_unsigned_integer());
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return ctx.make_value(lv.get_unsigned_integer() > rv.get_unsigned_integer());
    }


  return value();
}


value
gteq(context&  ctx, const value&  lv, const value&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer() && rti.is_integer())
    {
      return ctx.make_value(lv.get_integer() >= rv.get_integer());
    }

  else
    if(lti.is_unsigned_integer() && rti.is_unsigned_integer())
    {
      return ctx.make_value(lv.get_unsigned_integer() >= rv.get_unsigned_integer());
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return ctx.make_value(lv.get_unsigned_integer() >= rv.get_unsigned_integer());
    }


  return value();
}


value
logical_or(context&  ctx, const value&  lv, const value&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_like_boolean() && rti.is_like_boolean())
    {
      return ctx.make_value(   lv.get_integer()
                            || rv.get_integer());
    }


  return value();
}


value
logical_and(context&  ctx, const value&  lv, const value&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_like_boolean() && rti.is_like_boolean())
    {
      return ctx.make_value(   lv.get_integer()
                            && rv.get_integer());
    }


  return value();
}


value
bit_or(context&  ctx, const value&  lv, const value&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_kind_of_integer() && rti.is_kind_of_integer())
    {
      return ctx.make_value( lv.get_unsigned_integer()
                            |rv.get_unsigned_integer());
    }


  return value();
}


value
bit_and(context&  ctx, const value&  lv, const value&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_kind_of_integer() && rti.is_kind_of_integer())
    {
      return ctx.make_value( lv.get_unsigned_integer()
                            &rv.get_unsigned_integer());
    }


  return value();
}


value
bit_xor(context&  ctx, const value&  lv, const value&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_kind_of_integer() && rti.is_kind_of_integer())
    {
      return ctx.make_value( lv.get_unsigned_integer()
                            ^rv.get_unsigned_integer());
    }


  return value();
}


callback
get_callback(binary_opcodes  op) noexcept
{
  constexpr callback  table[] =
  {
    add        ,
    sub        ,
    mul        ,
    div        ,
    rem        ,
    shl        ,
    shr        ,
    eq         ,
    neq        ,
    lt         ,
    lteq       ,
    gt         ,
    gteq       ,
    logical_or ,
    logical_and,
    bit_or     ,
    bit_and    ,
    bit_xor    ,
  };


  return table[static_cast<int>(op)];
}
}




}




