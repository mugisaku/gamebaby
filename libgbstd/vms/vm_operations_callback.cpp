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
  auto&  ti = o.get_type_info().strip_reference_type();

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


tepid_object
dereference(cold_object  o, const memory&  home_mem) noexcept
{
    if(o.get_type_info().is_pointer())
    {
      auto&  ref_ti = o.get_type_info().strip_pointer_type().form_reference_type(type_infos::pointer_size);

      return hot_object(home_mem,ref_ti,o.get_unsigned_integer());
    }


  return hot_object();
}


cold_object
postfix_increment(hot_object  o) noexcept
{
  cold_object  co(o);

  assign(o,add(o,1));

  return co;
}


cold_object
postfix_decrement(hot_object  o) noexcept
{
  cold_object  co(o);

  assign(o,sub(o,1));

  return co;
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


tepid_object
assign(hot_object  lo, cold_object  ro) noexcept
{
  return lo.write(ro);
}




tepid_object
subscript(cold_object  lo, cold_object  ro, const memory&  mem) noexcept
{
  auto&  lti = lo.get_type_info();
  auto&  rti = ro.get_type_info();

    if(lti.is_pointer() && rti.is_kind_of_integer())
    {
      auto&  ti = lti.strip_pointer_type();

      address_t  p = lo.get_unsigned_integer()+(ti.get_size()*ro.get_unsigned_integer());

      return hot_object(mem,ti,p);
    }

  else
    if(lti.is_kind_of_integer() && rti.is_pointer())
    {
      auto&  ti = rti.strip_pointer_type();

      address_t  p = ro.get_unsigned_integer()+(ti.get_size()*lo.get_unsigned_integer());

      return hot_object(mem,ti,p);
    }


  return tepid_object();
}


tepid_object
arrow(cold_object  lo, const expression&  r, const memory&  mem) noexcept
{
  return dot(tepid_object(lo,mem),r);
}


tepid_object
dot(hot_object  lo, const expression&  r) noexcept
{
    if(r.is_identifier())
    {
        if(lo.get_type_info().is_reference())
        {
          auto&  ti = lo.get_type_info().strip_reference_type();

            if(ti.is_struct())
            {
              hot_object  ho(lo);

              return ho.get_struct_member(r.get_string());
            }

          else
            if(ti.is_union())
            {
              hot_object  ho(lo);

              return ho.get_union_member(r.get_string());
            }

          else
            {
            }
        }

      else
        {
        }
    }


  return tepid_object();
}


tepid_object
invoke(cold_object  lo, const expression&  r) noexcept
{
//    if(lo.get_type_info().is_function_pointer())
    {
//      auto&  fn = *reinterpret_cast<const function*>(lo.get_unsigned_integer());

      auto  argsrcs = r.get_argument_source_list();

        for(auto  argsrc: argsrcs)
        {
          argsrc->print();

          printf("\n");
        }
    }


  return tepid_object();
}




}}




