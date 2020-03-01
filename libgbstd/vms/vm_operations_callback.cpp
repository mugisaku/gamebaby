#include"libgbstd/vm.hpp"




namespace gbstd{




namespace unary_operations{
object
bit_not(context&  ctx, const object&  v) noexcept
{
  auto  deref_v = (v);

  auto&  ti = deref_v.get_type_info();

    if(ti.is_kind_of_integer())
    {
      return object(~deref_v.get_integer());
    }


  return object();
}


object
logical_not(context&  ctx, const object&  v) noexcept
{
  auto  deref_v = (v);

  auto&  ti = deref_v.get_type_info();

    if(ti.is_like_boolean())
    {
      return object(!deref_v.get_integer());
    }


  return object();
}


object
neg(context&  ctx, const object&  v) noexcept
{
  auto  deref_v = (v);

  auto&  ti = deref_v.get_type_info();

    if(ti.is_like_boolean())
    {
      return object(!deref_v.get_integer());
    }


  return object();
}


object
size(context&  ctx, const object&  v) noexcept
{
  auto  deref_v = (v);

  auto&  ti = deref_v.get_type_info();

  return object(static_cast<uint64_t>(ti.get_size()));
}


object
address(context&  ctx, const object&  v) noexcept
{
    if(v.get_type_info().is_reference())
    {
      return object(v.get_unsigned_integer());
    }


  return object();
}


object
prefix_increment(context&  ctx, const object&  v) noexcept
{
  return object();
}


object
prefix_decrement(context&  ctx, const object&  v) noexcept
{
  return object();
}


object
postfix_increment(context&  ctx, const object&  v) noexcept
{
  return object();
}


object
postfix_decrement(context&  ctx, const object&  v) noexcept
{
  return object();
}




}




namespace binary_operations{
object
add(context&  ctx, const object&  lv, const object&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer())
    {
      auto  li = lv.get_integer();

        if(rti.is_integer())
        {
          return object(li+rv.get_integer());
        }

      else
        if(rti.is_unsigned_integer())
        {
          return object(li+rv.get_unsigned_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
      auto  li = lv.get_unsigned_integer();

        if(rti.is_integer())
        {
          return object(li+rv.get_integer());
        }

      else
        if(rti.is_unsigned_integer())
        {
          return object(li+rv.get_unsigned_integer());
        }
    }

  else
    if(lti.is_pointer())
    {
      auto  li = lv.get_unsigned_integer();
      auto  sz = lti.get_pointer_type_info().get_base_type_info().get_size();

        if(rti.is_integer())
        {
          return object(li+(sz*rv.get_integer()));
        }

      else
        if(rti.is_unsigned_integer())
        {
          return object(li+(sz*rv.get_unsigned_integer()));
        }
    }


  return object();
}


object
sub(context&  ctx, const object&  lv, const object&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer())
    {
      auto  li = lv.get_integer();

        if(rti.is_integer())
        {
          return object(li-rv.get_integer());
        }

      else
        if(rti.is_unsigned_integer())
        {
          return object(li-rv.get_unsigned_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
      auto  li = lv.get_unsigned_integer();

        if(rti.is_integer())
        {
          return object(li-rv.get_integer());
        }

      else
        if(rti.is_unsigned_integer())
        {
          return object(li-rv.get_unsigned_integer());
        }
    }

  else
    if(lti.is_pointer())
    {
      auto  sz = lti.get_pointer_type_info().get_base_type_info().get_size();

        if(rti.is_pointer() && (lti == rti))
        {
          return object((lv.get_integer()-rv.get_integer())/sz);
        }

      else
        {
          auto  li = lv.get_unsigned_integer();

            if(rti.is_integer())
            {
              return object(li-(sz*rv.get_integer()));
            }

          else
            if(rti.is_unsigned_integer())
            {
              return object(li-(sz*rv.get_unsigned_integer()));
            }
        }
    }


  return object();
}


object
mul(context&  ctx, const object&  lv, const object&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return object( lv.get_integer()
                       *rv.get_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return object( lv.get_unsigned_integer()
                       *rv.get_unsigned_integer());
        }
    }


  return object();
}


object
div(context&  ctx, const object&  lv, const object&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return object( lv.get_integer()
                       /rv.get_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return object( lv.get_unsigned_integer()
                       /rv.get_unsigned_integer());
        }
    }


  return object();
}


object
rem(context&  ctx, const object&  lv, const object&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return object( lv.get_integer()
                       %rv.get_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return object( lv.get_unsigned_integer()
                       %rv.get_unsigned_integer());
        }
    }


  return object();
}


object
shl(context&  ctx, const object&  lv, const object&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer())
    {
        if(rti.is_kind_of_integer())
        {
          return object(  lv.get_integer()
                       <<rv.get_unsigned_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
        if(rti.is_kind_of_integer())
        {
          return object(  lv.get_unsigned_integer()
                       <<rv.get_unsigned_integer());
        }
    }


  return object();
}


object
shr(context&  ctx, const object&  lv, const object&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer())
    {
        if(rti.is_kind_of_integer())
        {
          return object(  lv.get_integer()
                       >>rv.get_unsigned_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
        if(rti.is_kind_of_integer())
        {
          return object(  lv.get_unsigned_integer()
                       >>rv.get_unsigned_integer());
        }
    }


  return object();
}


object
eq(context&  ctx, const object&  lv, const object&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

  auto  li = lv.get_integer();
  auto  ri = rv.get_integer();

    if((lti.is_integer()          && rti.is_integer()         ) ||
       (lti.is_unsigned_integer() && rti.is_unsigned_integer()))
    {
      return object(li == ri);
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return object(li == ri);
    }


  return object();
}


object
neq(context&  ctx, const object&  lv, const object&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

  auto  li = lv.get_integer();
  auto  ri = rv.get_integer();

    if((lti.is_integer()          && rti.is_integer()         ) ||
       (lti.is_unsigned_integer() && rti.is_unsigned_integer()))
    {
      return object(li != ri);
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return object(li != ri);
    }


  return object();
}


object
lt(context&  ctx, const object&  lv, const object&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer() && rti.is_integer())
    {
      return object(lv.get_integer() < rv.get_integer());
    }

  else
    if(lti.is_unsigned_integer() && rti.is_unsigned_integer())
    {
      return object(lv.get_unsigned_integer() < rv.get_unsigned_integer());
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return object(lv.get_unsigned_integer() < rv.get_unsigned_integer());
    }


  return object();
}


object
lteq(context&  ctx, const object&  lv, const object&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer() && rti.is_integer())
    {
      return object(lv.get_integer() <= rv.get_integer());
    }

  else
    if(lti.is_unsigned_integer() && rti.is_unsigned_integer())
    {
      return object(lv.get_unsigned_integer() <= rv.get_unsigned_integer());
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return object(lv.get_unsigned_integer() <= rv.get_unsigned_integer());
    }


  return object();
}


object
gt(context&  ctx, const object&  lv, const object&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer() && rti.is_integer())
    {
      return object(lv.get_integer() > rv.get_integer());
    }

  else
    if(lti.is_unsigned_integer() && rti.is_unsigned_integer())
    {
      return object(lv.get_unsigned_integer() > rv.get_unsigned_integer());
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return object(lv.get_unsigned_integer() > rv.get_unsigned_integer());
    }


  return object();
}


object
gteq(context&  ctx, const object&  lv, const object&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_integer() && rti.is_integer())
    {
      return object(lv.get_integer() >= rv.get_integer());
    }

  else
    if(lti.is_unsigned_integer() && rti.is_unsigned_integer())
    {
      return object(lv.get_unsigned_integer() >= rv.get_unsigned_integer());
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return object(lv.get_unsigned_integer() >= rv.get_unsigned_integer());
    }


  return object();
}


object
logical_or(context&  ctx, const object&  lv, const object&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_like_boolean() && rti.is_like_boolean())
    {
      return object(   lv.get_integer()
                   || rv.get_integer());
    }


  return object();
}


object
logical_and(context&  ctx, const object&  lv, const object&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_like_boolean() && rti.is_like_boolean())
    {
      return object(   lv.get_integer()
                   && rv.get_integer());
    }


  return object();
}


object
bit_or(context&  ctx, const object&  lv, const object&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_kind_of_integer() && rti.is_kind_of_integer())
    {
      return object( lv.get_unsigned_integer()
                   |rv.get_unsigned_integer());
    }


  return object();
}


object
bit_and(context&  ctx, const object&  lv, const object&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_kind_of_integer() && rti.is_kind_of_integer())
    {
      return object( lv.get_unsigned_integer()
                   &rv.get_unsigned_integer());
    }


  return object();
}


object
bit_xor(context&  ctx, const object&  lv, const object&  rv) noexcept
{
  auto&  lti = lv.get_type_info();
  auto&  rti = rv.get_type_info();

    if(lti.is_kind_of_integer() && rti.is_kind_of_integer())
    {
      return object( lv.get_unsigned_integer()
                   ^rv.get_unsigned_integer());
    }


  return object();
}


object          add_assign(context&  ctx, const object&  lv, const object&  rv) noexcept{return object();}
object          sub_assign(context&  ctx, const object&  lv, const object&  rv) noexcept{return object();}
object          mul_assign(context&  ctx, const object&  lv, const object&  rv) noexcept{return object();}
object          div_assign(context&  ctx, const object&  lv, const object&  rv) noexcept{return object();}
object          rem_assign(context&  ctx, const object&  lv, const object&  rv) noexcept{return object();}
object          shl_assign(context&  ctx, const object&  lv, const object&  rv) noexcept{return object();}
object          shr_assign(context&  ctx, const object&  lv, const object&  rv) noexcept{return object();}
object       bit_or_assign(context&  ctx, const object&  lv, const object&  rv) noexcept{return object();}
object      bit_and_assign(context&  ctx, const object&  lv, const object&  rv) noexcept{return object();}
object      bit_xor_assign(context&  ctx, const object&  lv, const object&  rv) noexcept{return object();}
object      comma(context&  ctx, const object&  lv, const object&  rv) noexcept{return object();}
object      dot(context&  ctx, const object&  lv, const object&  rv) noexcept{return object();}
object      arrow(context&  ctx, const object&  lv, const object&  rv) noexcept{return object();}
object      scope_resolution(context&  ctx, const object&  lv, const object&  rv) noexcept{return object();}
object      subscript(context&  ctx, const object&  lv, const object&  rv) noexcept{return object();}
object      invoke(context&  ctx, const object&  lv, const object&  rv) noexcept{return object();}
object      assign(context&  ctx, const object&  lv, const object&  rv) noexcept{return object();}




}




}




