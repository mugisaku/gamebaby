#include"libgbstd/vm.hpp"




namespace gbstd{
namespace operations{




cold_object
bit_not(cold_object  co) noexcept
{
  auto&  ti = co.get_type_info();

    if(ti.is_kind_of_integer())
    {
      return cold_object(~co.get_integer());
    }


  return cold_object();
}


cold_object
logical_not(cold_object  co) noexcept
{
  auto&  ti = co.get_type_info();

    if(ti.is_like_boolean())
    {
      return cold_object(!co.get_integer());
    }


  return cold_object();
}


cold_object
neg(cold_object  co) noexcept
{
  auto&  ti = co.get_type_info();

    if(ti.is_kind_of_integer())
    {
      return cold_object(!co.get_integer());
    }


  return cold_object();
}


cold_object
size(cold_object  co) noexcept
{
  auto&  ti = co.get_type_info();

  return cold_object(static_cast<uint64_t>(ti.get_size()));
}


cold_object
address(hot_object  ho) noexcept
{
    if(ho.get_type_info().is_reference())
    {
      auto&  ptr_ti = ho.get_type_info().get_reference_type_info().get_base_type_info().form_pointer_type(type_infos::pointer_size);

      return cold_object(ptr_ti,ho.get_address());
    }


  return cold_object();
}


hot_object
dereference(cold_object  co) noexcept
{
    if(co.get_type_info().is_pointer())
    {
      auto&  ref_ti = co.get_type_info().get_pointer_type_info().get_base_type_info().form_reference_type(type_infos::pointer_size);

//      return hot_object(co.get_memory(),ref_ti,co.get_address());
    }


  return hot_object();
}


hot_object
prefix_increment(hot_object  ho) noexcept
{
  return hot_object();
}


hot_object
prefix_decrement(hot_object  ho) noexcept
{
  return hot_object();
}


cold_object
postfix_increment(hot_object  ho) noexcept
{
  return cold_object();
}


cold_object
postfix_decrement(hot_object  ho) noexcept
{
  return cold_object();
}




cold_object
add(cold_object  lco, cold_object  rco) noexcept
{
  auto&  lti = lco.get_type_info();
  auto&  rti = rco.get_type_info();

    if(lti.is_integer())
    {
      auto  li = lco.get_integer();

        if(rti.is_integer())
        {
          return cold_object(li+rco.get_integer());
        }

      else
        if(rti.is_unsigned_integer())
        {
          return cold_object(li+rco.get_unsigned_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
      auto  li = lco.get_unsigned_integer();

        if(rti.is_integer())
        {
          return cold_object(li+rco.get_integer());
        }

      else
        if(rti.is_unsigned_integer())
        {
          return cold_object(li+rco.get_unsigned_integer());
        }
    }

  else
    if(lti.is_pointer())
    {
      auto  li = lco.get_unsigned_integer();
      auto  sz = lti.get_pointer_type_info().get_base_type_info().get_size();

        if(rti.is_integer())
        {
          return cold_object(li+(sz*rco.get_integer()));
        }

      else
        if(rti.is_unsigned_integer())
        {
          return cold_object(li+(sz*rco.get_unsigned_integer()));
        }
    }


  return cold_object();
}


cold_object
sub(cold_object  lco, cold_object  rco) noexcept
{
  auto&  lti = lco.get_type_info();
  auto&  rti = rco.get_type_info();

    if(lti.is_integer())
    {
      auto  li = lco.get_integer();

        if(rti.is_integer())
        {
          return cold_object(li-rco.get_integer());
        }

      else
        if(rti.is_unsigned_integer())
        {
          return cold_object(li-rco.get_unsigned_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
      auto  li = lco.get_unsigned_integer();

        if(rti.is_integer())
        {
          return cold_object(li-rco.get_integer());
        }

      else
        if(rti.is_unsigned_integer())
        {
          return cold_object(li-rco.get_unsigned_integer());
        }
    }

  else
    if(lti.is_pointer())
    {
      auto  sz = lti.get_pointer_type_info().get_base_type_info().get_size();

        if(rti.is_pointer() && (lti == rti))
        {
          return cold_object((lco.get_integer()-rco.get_integer())/sz);
        }

      else
        {
          auto  li = lco.get_unsigned_integer();

            if(rti.is_integer())
            {
              return cold_object(li-(sz*rco.get_integer()));
            }

          else
            if(rti.is_unsigned_integer())
            {
              return cold_object(li-(sz*rco.get_unsigned_integer()));
            }
        }
    }


  return cold_object();
}


cold_object
mul(cold_object  lco, cold_object  rco) noexcept
{
  auto&  lti = lco.get_type_info();
  auto&  rti = rco.get_type_info();

    if(lti.is_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return cold_object( lco.get_integer()
                            *rco.get_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return cold_object( lco.get_unsigned_integer()
                            *rco.get_unsigned_integer());
        }
    }


  return cold_object();
}


cold_object
div(cold_object  lco, cold_object  rco) noexcept
{
  auto&  lti = lco.get_type_info();
  auto&  rti = rco.get_type_info();

    if(lti.is_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return cold_object( lco.get_integer()
                            /rco.get_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return cold_object( lco.get_unsigned_integer()
                            /rco.get_unsigned_integer());
        }
    }


  return cold_object();
}


cold_object
rem(cold_object  lco, cold_object  rco) noexcept
{
  auto&  lti = lco.get_type_info();
  auto&  rti = rco.get_type_info();

    if(lti.is_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return cold_object( lco.get_integer()
                            %rco.get_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return cold_object( lco.get_unsigned_integer()
                            %rco.get_unsigned_integer());
        }
    }


  return cold_object();
}


cold_object
shl(cold_object  lco, cold_object  rco) noexcept
{
  auto&  lti = lco.get_type_info();
  auto&  rti = rco.get_type_info();

    if(lti.is_integer())
    {
        if(rti.is_kind_of_integer())
        {
          return cold_object(  lco.get_integer()
                            <<rco.get_unsigned_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
        if(rti.is_kind_of_integer())
        {
          return cold_object(  lco.get_unsigned_integer()
                            <<rco.get_unsigned_integer());
        }
    }


  return cold_object();
}


cold_object
shr(cold_object  lco, cold_object  rco) noexcept
{
  auto&  lti = lco.get_type_info();
  auto&  rti = rco.get_type_info();

    if(lti.is_integer())
    {
        if(rti.is_kind_of_integer())
        {
          return cold_object(  lco.get_integer()
                            >>rco.get_unsigned_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
        if(rti.is_kind_of_integer())
        {
          return cold_object(  lco.get_unsigned_integer()
                            >>rco.get_unsigned_integer());
        }
    }


  return cold_object();
}


cold_object
eq(cold_object  lco, cold_object  rco) noexcept
{
  auto&  lti = lco.get_type_info();
  auto&  rti = rco.get_type_info();

  auto  li = lco.get_integer();
  auto  ri = rco.get_integer();

    if((lti.is_integer()          && rti.is_integer()         ) ||
       (lti.is_unsigned_integer() && rti.is_unsigned_integer()))
    {
      return cold_object(li == ri);
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return cold_object(li == ri);
    }


  return cold_object();
}


cold_object
neq(cold_object  lco, cold_object  rco) noexcept
{
  auto&  lti = lco.get_type_info();
  auto&  rti = rco.get_type_info();

  auto  li = lco.get_integer();
  auto  ri = rco.get_integer();

    if((lti.is_integer()          && rti.is_integer()         ) ||
       (lti.is_unsigned_integer() && rti.is_unsigned_integer()))
    {
      return cold_object(li != ri);
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return cold_object(li != ri);
    }


  return cold_object();
}


cold_object
lt(cold_object  lco, cold_object  rco) noexcept
{
  auto&  lti = lco.get_type_info();
  auto&  rti = rco.get_type_info();

    if(lti.is_integer() && rti.is_integer())
    {
      return cold_object(lco.get_integer() < rco.get_integer());
    }

  else
    if(lti.is_unsigned_integer() && rti.is_unsigned_integer())
    {
      return cold_object(lco.get_unsigned_integer() < rco.get_unsigned_integer());
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return cold_object(lco.get_unsigned_integer() < rco.get_unsigned_integer());
    }


  return cold_object();
}


cold_object
lteq(cold_object  lco, cold_object  rco) noexcept
{
  auto&  lti = lco.get_type_info();
  auto&  rti = rco.get_type_info();

    if(lti.is_integer() && rti.is_integer())
    {
      return cold_object(lco.get_integer() <= rco.get_integer());
    }

  else
    if(lti.is_unsigned_integer() && rti.is_unsigned_integer())
    {
      return cold_object(lco.get_unsigned_integer() <= rco.get_unsigned_integer());
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return cold_object(lco.get_unsigned_integer() <= rco.get_unsigned_integer());
    }


  return cold_object();
}


cold_object
gt(cold_object  lco, cold_object  rco) noexcept
{
  auto&  lti = lco.get_type_info();
  auto&  rti = rco.get_type_info();

    if(lti.is_integer() && rti.is_integer())
    {
      return cold_object(lco.get_integer() > rco.get_integer());
    }

  else
    if(lti.is_unsigned_integer() && rti.is_unsigned_integer())
    {
      return cold_object(lco.get_unsigned_integer() > rco.get_unsigned_integer());
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return cold_object(lco.get_unsigned_integer() > rco.get_unsigned_integer());
    }


  return cold_object();
}


cold_object
gteq(cold_object  lco, cold_object  rco) noexcept
{
  auto&  lti = lco.get_type_info();
  auto&  rti = rco.get_type_info();

    if(lti.is_integer() && rti.is_integer())
    {
      return cold_object(lco.get_integer() >= rco.get_integer());
    }

  else
    if(lti.is_unsigned_integer() && rti.is_unsigned_integer())
    {
      return cold_object(lco.get_unsigned_integer() >= rco.get_unsigned_integer());
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return cold_object(lco.get_unsigned_integer() >= rco.get_unsigned_integer());
    }


  return cold_object();
}


cold_object
logical_or(cold_object  lco, cold_object  rco) noexcept
{
  auto&  lti = lco.get_type_info();
  auto&  rti = rco.get_type_info();

    if(lti.is_like_boolean() && rti.is_like_boolean())
    {
      return cold_object(lco.get_integer() || rco.get_integer());
    }


  return cold_object();
}


cold_object
logical_and(cold_object  lco, cold_object  rco) noexcept
{
  auto&  lti = lco.get_type_info();
  auto&  rti = rco.get_type_info();

    if(lti.is_like_boolean() && rti.is_like_boolean())
    {
      return cold_object(lco.get_integer() && rco.get_integer());
    }


  return cold_object();
}


cold_object
bit_or(cold_object  lco, cold_object  rco) noexcept
{
  auto&  lti = lco.get_type_info();
  auto&  rti = rco.get_type_info();

    if(lti.is_kind_of_integer() && rti.is_kind_of_integer())
    {
      return cold_object(lco.get_unsigned_integer()|rco.get_unsigned_integer());
    }


  return cold_object();
}


cold_object
bit_and(cold_object  lco, cold_object  rco) noexcept
{
  auto&  lti = lco.get_type_info();
  auto&  rti = rco.get_type_info();

    if(lti.is_kind_of_integer() && rti.is_kind_of_integer())
    {
      return cold_object(lco.get_unsigned_integer()&rco.get_unsigned_integer());
    }


  return cold_object();
}


cold_object
bit_xor(cold_object  lco, cold_object  rco) noexcept
{
  auto&  lti = lco.get_type_info();
  auto&  rti = rco.get_type_info();

    if(lti.is_kind_of_integer() && rti.is_kind_of_integer())
    {
      return cold_object(lco.get_unsigned_integer()^rco.get_unsigned_integer());
    }


  return cold_object();
}


cold_object          add_assign(cold_object  lco, cold_object  rco) noexcept{return cold_object();}
cold_object          sub_assign(cold_object  lco, cold_object  rco) noexcept{return cold_object();}
cold_object          mul_assign(cold_object  lco, cold_object  rco) noexcept{return cold_object();}
cold_object          div_assign(cold_object  lco, cold_object  rco) noexcept{return cold_object();}
cold_object          rem_assign(cold_object  lco, cold_object  rco) noexcept{return cold_object();}
cold_object          shl_assign(cold_object  lco, cold_object  rco) noexcept{return cold_object();}
cold_object          shr_assign(cold_object  lco, cold_object  rco) noexcept{return cold_object();}
cold_object       bit_or_assign(cold_object  lco, cold_object  rco) noexcept{return cold_object();}
cold_object      bit_and_assign(cold_object  lco, cold_object  rco) noexcept{return cold_object();}
cold_object      bit_xor_assign(cold_object  lco, cold_object  rco) noexcept{return cold_object();}
cold_object      comma(cold_object  lco, cold_object  rco) noexcept{return cold_object();}
cold_object      dot(cold_object  lco, cold_object  rco) noexcept{return cold_object();}
cold_object      arrow(cold_object  lco, cold_object  rco) noexcept{return cold_object();}
cold_object      scope_resolution(cold_object  lco, cold_object  rco) noexcept{return cold_object();}
cold_object      subscript(cold_object  lco, cold_object  rco) noexcept{return cold_object();}
cold_object      invoke(cold_object  lco, cold_object  rco) noexcept{return cold_object();}
cold_object      assign(cold_object  lco, cold_object  rco) noexcept{return cold_object();}




}}




