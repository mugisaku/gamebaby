#include"libgbstd/vm.hpp"




namespace gbstd{
namespace operations{




cold_object
bit_not(cold_object  o) noexcept
{
  auto&  ti = o.get_type_info();

    if(ti.is_kind_of_integer())
    {
      return cold_object(~o.get_integer());
    }


  return cold_object();
}


cold_object
logical_not(cold_object  o) noexcept
{
  auto&  ti = o.get_type_info();

    if(ti.is_like_boolean())
    {
      return cold_object(!o.get_integer());
    }


  return cold_object();
}


cold_object
neg(cold_object  o) noexcept
{
  auto&  ti = o.get_type_info();

    if(ti.is_kind_of_integer())
    {
      return cold_object(!o.get_integer());
    }


  return cold_object();
}


cold_object
size(cold_object  o) noexcept
{
  auto&  ti = o.get_type_info();

  return cold_object(static_cast<uint64_t>(ti.get_size()));
}


cold_object
address(hot_object  o) noexcept
{
    if(o.get_type_info().is_reference())
    {
      auto&  ptr_ti = o.get_type_info().strip_reference_type();

      return cold_object(ptr_ti,o.get_address());
    }


  return cold_object();
}


hot_object
dereference(cold_object  o, const memory&  home_mem) noexcept
{
    if(o.get_type_info().is_pointer())
    {
      auto&  ref_ti = o.get_type_info().strip_pointer_type().form_reference_type(type_infos::pointer_size);

      return hot_object(home_mem,ref_ti,o.get_unsigned_integer());
    }


  return hot_object();
}


hot_object
prefix_increment(hot_object  o) noexcept
{
  return o;
}


hot_object
prefix_decrement(hot_object  o) noexcept
{
  return o;
}


hot_object
postfix_increment(hot_object  o) noexcept
{
  return o;
}


hot_object
postfix_decrement(hot_object  o) noexcept
{
  return o;
}




cold_object
add(cold_object  lo, cold_object  ro) noexcept
{
  auto&  lti = lo.get_type_info();
  auto&  rti = ro.get_type_info();

    if(lti.is_integer())
    {
      auto  li = lo.get_integer();

        if(rti.is_integer())
        {
          return cold_object(li+ro.get_integer());
        }

      else
        if(rti.is_unsigned_integer())
        {
          return cold_object(li+ro.get_unsigned_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
      auto  li = lo.get_unsigned_integer();

        if(rti.is_integer())
        {
          return cold_object(li+ro.get_integer());
        }

      else
        if(rti.is_unsigned_integer())
        {
          return cold_object(li+ro.get_unsigned_integer());
        }
    }

  else
    if(lti.is_pointer())
    {
      auto  li = lo.get_unsigned_integer();
      auto  sz = lti.get_pointer_type_info().get_base_type_info().get_size();

        if(rti.is_integer())
        {
          return cold_object(li+(sz*ro.get_integer()));
        }

      else
        if(rti.is_unsigned_integer())
        {
          return cold_object(li+(sz*ro.get_unsigned_integer()));
        }
    }


  return cold_object();
}


cold_object
sub(cold_object  lo, cold_object  ro) noexcept
{
  auto&  lti = lo.get_type_info();
  auto&  rti = ro.get_type_info();

    if(lti.is_integer())
    {
      auto  li = lo.get_integer();

        if(rti.is_integer())
        {
          return cold_object(li-ro.get_integer());
        }

      else
        if(rti.is_unsigned_integer())
        {
          return cold_object(li-ro.get_unsigned_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
      auto  li = lo.get_unsigned_integer();

        if(rti.is_integer())
        {
          return cold_object(li-ro.get_integer());
        }

      else
        if(rti.is_unsigned_integer())
        {
          return cold_object(li-ro.get_unsigned_integer());
        }
    }

  else
    if(lti.is_pointer())
    {
      auto  sz = lti.get_pointer_type_info().get_base_type_info().get_size();

        if(rti.is_pointer() && (lti == rti))
        {
          return cold_object((lo.get_integer()-ro.get_integer())/sz);
        }

      else
        {
          auto  li = lo.get_unsigned_integer();

            if(rti.is_integer())
            {
              return cold_object(li-(sz*ro.get_integer()));
            }

          else
            if(rti.is_unsigned_integer())
            {
              return cold_object(li-(sz*ro.get_unsigned_integer()));
            }
        }
    }


  return cold_object();
}


cold_object
mul(cold_object  lo, cold_object  ro) noexcept
{
  auto&  lti = lo.get_type_info();
  auto&  rti = ro.get_type_info();

    if(lti.is_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return cold_object( lo.get_integer()
                            *ro.get_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return cold_object( lo.get_unsigned_integer()
                            *ro.get_unsigned_integer());
        }
    }


  return cold_object();
}


cold_object
div(cold_object  lo, cold_object  ro) noexcept
{
  auto&  lti = lo.get_type_info();
  auto&  rti = ro.get_type_info();

    if(lti.is_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return cold_object( lo.get_integer()
                            /ro.get_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return cold_object( lo.get_unsigned_integer()
                            /ro.get_unsigned_integer());
        }
    }


  return cold_object();
}


cold_object
rem(cold_object  lo, cold_object  ro) noexcept
{
  auto&  lti = lo.get_type_info();
  auto&  rti = ro.get_type_info();

    if(lti.is_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return cold_object( lo.get_integer()
                            %ro.get_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
        if(rti.is_integer() || rti.is_unsigned_integer())
        {
          return cold_object( lo.get_unsigned_integer()
                            %ro.get_unsigned_integer());
        }
    }


  return cold_object();
}


cold_object
shl(cold_object  lo, cold_object  ro) noexcept
{
  auto&  lti = lo.get_type_info();
  auto&  rti = ro.get_type_info();

    if(lti.is_integer())
    {
        if(rti.is_kind_of_integer())
        {
          return cold_object(  lo.get_integer()
                            <<ro.get_unsigned_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
        if(rti.is_kind_of_integer())
        {
          return cold_object(  lo.get_unsigned_integer()
                            <<ro.get_unsigned_integer());
        }
    }


  return cold_object();
}


cold_object
shr(cold_object  lo, cold_object  ro) noexcept
{
  auto&  lti = lo.get_type_info();
  auto&  rti = ro.get_type_info();

    if(lti.is_integer())
    {
        if(rti.is_kind_of_integer())
        {
          return cold_object(  lo.get_integer()
                            >>ro.get_unsigned_integer());
        }
    }

  else
    if(lti.is_unsigned_integer())
    {
        if(rti.is_kind_of_integer())
        {
          return cold_object(  lo.get_unsigned_integer()
                            >>ro.get_unsigned_integer());
        }
    }


  return cold_object();
}


cold_object
eq(cold_object  lo, cold_object  ro) noexcept
{
  auto&  lti = lo.get_type_info();
  auto&  rti = ro.get_type_info();

  auto  li = lo.get_integer();
  auto  ri = ro.get_integer();

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
neq(cold_object  lo, cold_object  ro) noexcept
{
  auto&  lti = lo.get_type_info();
  auto&  rti = ro.get_type_info();

  auto  li = lo.get_integer();
  auto  ri = ro.get_integer();

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
lt(cold_object  lo, cold_object  ro) noexcept
{
  auto&  lti = lo.get_type_info();
  auto&  rti = ro.get_type_info();

    if(lti.is_integer() && rti.is_integer())
    {
      return cold_object(lo.get_integer() < ro.get_integer());
    }

  else
    if(lti.is_unsigned_integer() && rti.is_unsigned_integer())
    {
      return cold_object(lo.get_unsigned_integer() < ro.get_unsigned_integer());
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return cold_object(lo.get_unsigned_integer() < ro.get_unsigned_integer());
    }


  return cold_object();
}


cold_object
lteq(cold_object  lo, cold_object  ro) noexcept
{
  auto&  lti = lo.get_type_info();
  auto&  rti = ro.get_type_info();

    if(lti.is_integer() && rti.is_integer())
    {
      return cold_object(lo.get_integer() <= ro.get_integer());
    }

  else
    if(lti.is_unsigned_integer() && rti.is_unsigned_integer())
    {
      return cold_object(lo.get_unsigned_integer() <= ro.get_unsigned_integer());
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return cold_object(lo.get_unsigned_integer() <= ro.get_unsigned_integer());
    }


  return cold_object();
}


cold_object
gt(cold_object  lo, cold_object  ro) noexcept
{
  auto&  lti = lo.get_type_info();
  auto&  rti = ro.get_type_info();

    if(lti.is_integer() && rti.is_integer())
    {
      return cold_object(lo.get_integer() > ro.get_integer());
    }

  else
    if(lti.is_unsigned_integer() && rti.is_unsigned_integer())
    {
      return cold_object(lo.get_unsigned_integer() > ro.get_unsigned_integer());
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return cold_object(lo.get_unsigned_integer() > ro.get_unsigned_integer());
    }


  return cold_object();
}


cold_object
gteq(cold_object  lo, cold_object  ro) noexcept
{
  auto&  lti = lo.get_type_info();
  auto&  rti = ro.get_type_info();

    if(lti.is_integer() && rti.is_integer())
    {
      return cold_object(lo.get_integer() >= ro.get_integer());
    }

  else
    if(lti.is_unsigned_integer() && rti.is_unsigned_integer())
    {
      return cold_object(lo.get_unsigned_integer() >= ro.get_unsigned_integer());
    }

  else
    if(lti.is_pointer() && rti.is_pointer() && (lti == rti))
    {
      return cold_object(lo.get_unsigned_integer() >= ro.get_unsigned_integer());
    }


  return cold_object();
}


cold_object
logical_or(cold_object  lo, cold_object  ro) noexcept
{
  auto&  lti = lo.get_type_info();
  auto&  rti = ro.get_type_info();

    if(lti.is_like_boolean() && rti.is_like_boolean())
    {
      return cold_object(lo.get_integer() || ro.get_integer());
    }


  return cold_object();
}


cold_object
logical_and(cold_object  lo, cold_object  ro) noexcept
{
  auto&  lti = lo.get_type_info();
  auto&  rti = ro.get_type_info();

    if(lti.is_like_boolean() && rti.is_like_boolean())
    {
      return cold_object(lo.get_integer() && ro.get_integer());
    }


  return cold_object();
}


cold_object
bit_or(cold_object  lo, cold_object  ro) noexcept
{
  auto&  lti = lo.get_type_info();
  auto&  rti = ro.get_type_info();

    if(lti.is_kind_of_integer() && rti.is_kind_of_integer())
    {
      return cold_object(lo.get_unsigned_integer()|ro.get_unsigned_integer());
    }


  return cold_object();
}


cold_object
bit_and(cold_object  lo, cold_object  ro) noexcept
{
  auto&  lti = lo.get_type_info();
  auto&  rti = ro.get_type_info();

    if(lti.is_kind_of_integer() && rti.is_kind_of_integer())
    {
      return cold_object(lo.get_unsigned_integer()&ro.get_unsigned_integer());
    }


  return cold_object();
}


cold_object
bit_xor(cold_object  lo, cold_object  ro) noexcept
{
  auto&  lti = lo.get_type_info();
  auto&  rti = ro.get_type_info();

    if(lti.is_kind_of_integer() && rti.is_kind_of_integer())
    {
      return cold_object(lo.get_unsigned_integer()^ro.get_unsigned_integer());
    }


  return cold_object();
}


tepid_object      add_assign(hot_object  lo, cold_object  ro) noexcept{return assign(lo,    add(lo,ro));}
tepid_object      sub_assign(hot_object  lo, cold_object  ro) noexcept{return assign(lo,    sub(lo,ro));}
tepid_object      mul_assign(hot_object  lo, cold_object  ro) noexcept{return assign(lo,    mul(lo,ro));}
tepid_object      div_assign(hot_object  lo, cold_object  ro) noexcept{return assign(lo,    div(lo,ro));}
tepid_object      rem_assign(hot_object  lo, cold_object  ro) noexcept{return assign(lo,    rem(lo,ro));}
tepid_object      shl_assign(hot_object  lo, cold_object  ro) noexcept{return assign(lo,    shl(lo,ro));}
tepid_object      shr_assign(hot_object  lo, cold_object  ro) noexcept{return assign(lo,    shr(lo,ro));}
tepid_object   bit_or_assign(hot_object  lo, cold_object  ro) noexcept{return assign(lo, bit_or(lo,ro));}
tepid_object  bit_and_assign(hot_object  lo, cold_object  ro) noexcept{return assign(lo,bit_and(lo,ro));}
tepid_object  bit_xor_assign(hot_object  lo, cold_object  ro) noexcept{return assign(lo,bit_xor(lo,ro));}


tepid_object
assign(hot_object  lo, cold_object  ro) noexcept
{
  return lo.write(ro);
}




cold_object      comma(cold_object  lo, cold_object  ro) noexcept{return cold_object();}
cold_object  subscript(cold_object  lo, cold_object  ro) noexcept{return cold_object();}




}}




