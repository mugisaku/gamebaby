#include"libgbstd/vms/ir.hpp"




namespace gbstd{




void
ir_memory::
copy(ir_address  src_addr, ir_type_info  src_ti, ir_address  dst_addr, ir_type_info  dst_ti) noexcept
{
  auto  sz = src_ti.size();

    if(src_ti.is_object() && dst_ti.is_object())
    {
        if(sz == dst_ti.size())
        {
            while(!sz--)
            {
              m_space[dst_addr++] = m_space[src_addr++];
            }
        }
    }

  else
    if(src_ti.is_integer())
    {
      int64_t  src_i = (sz == 1)?  load8(src_addr)
                      :(sz == 2)? load16(src_addr)
                      :(sz == 4)? load32(src_addr)
                      :(sz == 8)? load64(src_addr)
                      :0
                      ;

        if(dst_ti.is_integer())
        {
            switch(dst_ti.size())
            {
          case 1: store8( dst_addr,src_i);break;
          case 2: store16(dst_addr,src_i);break;
          case 4: store32(dst_addr,src_i);break;
          case 8: store64(dst_addr,src_i);break;
            }
        }

      else
        if(dst_ti.is_floating())
        {
            switch(dst_ti.size())
            {
          case 4: storef32(dst_addr,src_i);break;
          case 8: storef64(dst_addr,src_i);break;
            }
        }
    }

  else
    if(src_ti.is_floating())
    {
      double  src_f =  (sz == 4)? loadf32(src_addr)
                      :(sz == 8)? loadf64(src_addr)
                      :0
                      ;

        if(dst_ti.is_integer())
        {
            switch(dst_ti.size())
            {
          case 1: store8( dst_addr,src_f);break;
          case 2: store16(dst_addr,src_f);break;
          case 4: store32(dst_addr,src_f);break;
          case 8: store64(dst_addr,src_f);break;
            }
        }

      else
        if(dst_ti.is_floating())
        {
            switch(dst_ti.size())
            {
          case 4: storef32(dst_addr,src_f);break;
          case 8: storef64(dst_addr,src_f);break;
            }
        }
    }
}




ir_value
ir_memory::
make_integer_value(ir_address  addr, size_t  sz) const noexcept
{
  return (sz == 1)? reinterpret_cast<const int8_t&>( m_space[addr])
        :(sz == 2)? reinterpret_cast<const int16_t&>(m_space[addr])
        :(sz == 4)? reinterpret_cast<const int32_t&>(m_space[addr])
        :(sz == 8)? reinterpret_cast<const int64_t&>(m_space[addr])
        :ir_value()
        ;
}


ir_value
ir_memory::
make_floating_value(ir_address  addr, size_t  sz) const noexcept
{
  return (sz == 4)? reinterpret_cast<const float&>( m_space[addr])
        :(sz == 8)? reinterpret_cast<const double&>(m_space[addr])
        :ir_value()
        ;
}


ir_value
ir_memory::
make_object_value(ir_address  addr, size_t  sz) const noexcept
{
  return ir_value(sz,&m_space[addr]);
}




ir_value
ir_memory::
make_value(ir_address  addr, ir_type_info  ti) const noexcept
{
  return ti.is_integer()?  make_integer_value(addr,ti.size())
        :ti.is_floating()? make_floating_value(addr,ti.size())
        :ti.is_object()?   make_object_value(addr,ti.size())
        :ir_value()
        ;
}




}




