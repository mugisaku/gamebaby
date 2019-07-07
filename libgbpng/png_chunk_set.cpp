#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




void
chunk_set::
clear() noexcept
{
  m_ihdr = nullptr;

  m_between_ihdr_and_idat.clear();

  m_plte = nullptr;
  m_trns = nullptr;
  m_bkgd = nullptr;

  m_idat.clear();

  m_after_idat.clear();
}


void
chunk_set::
read_after_ihdr(const chunk*&  it, const chunk*  it_end)
{
    while(it != it_end)
    {
      auto&  chk = *it++;

        if(chk == "IDAT")
        {
          m_idat.emplace_back(&chk);

          read_idat(      it,it_end);
          read_after_idat(it,it_end);

          return;
        }

      else
        if(chk == "PLTE")
        {
          m_between_ihdr_and_idat.emplace_back(&chk);

            if(m_plte)
            {
              printf("chunk_set read error: there are multiple palettes\n");
            }


          m_plte = &chk;
        }

      else
        if(chk == "tRNS")
        {
          m_between_ihdr_and_idat.emplace_back(&chk);

            if(m_trns)
            {
              printf("chunk_set read error: there are multiple tRNSes\n");
            }


          m_trns = &chk;
        }

      else
        if(chk == "bKGD")
        {
          m_between_ihdr_and_idat.emplace_back(&chk);

            if(m_bkgd)
            {
              printf("chunk_set read error: there are multiple bKGDes\n");
            }


          m_bkgd = &chk;
        }

      else
        {
          m_between_ihdr_and_idat.emplace_back(&chk);
        }
    }


  throw_error("have no IDAT chunk");
}


void
chunk_set::
read_idat(const chunk*&  it, const chunk*  it_end) noexcept
{
    while(it != it_end)
    {
      auto&  chk = *it;

        if(chk == "IDAT")
        {
          m_idat.emplace_back(&chk);

          ++it;
        }

      else
        {
          break;
        }
    }
}


void
chunk_set::
read_after_idat(const chunk*&  it, const chunk*  it_end) noexcept
{
    while(it != it_end)
    {
      auto&  chk = *it++;

      m_after_idat.emplace_back(&chk);
    }
}


chunk_set&
chunk_set::
assign(const chunk_list&  ls)
{
  clear();

  auto  it     = ls.begin();
  auto  it_end = ls.end();

    if((it == it_end) || (*it != "IHDR"))
    {
      throw_error("have no IHDR chunk at first");
    }


  m_ihdr = it++;

  read_after_ihdr(it,it_end);

  return *this;
}




void
chunk_set::
print() const noexcept
{
  printf("{\n");
  m_ihdr->print();
  printf("}\n\n");

    for(auto  chk: m_between_ihdr_and_idat)
    {
      printf("{\n");
      chk->print();
      printf("}\n\n");
    }


    if(m_plte)
    {
      printf("have palette\n");
    }


    if(m_trns)
    {
      printf("have tRNS\n");
    }


    if(m_bkgd)
    {
      printf("have bKGD\n");
    }


    for(auto  chk: m_idat)
    {
      printf("{\n");
      chk->print();
      printf("}\n\n");
    }


    for(auto  chk: m_after_idat)
    {
      printf("{\n");
      chk->print();
      printf("}\n\n");
    }
}




}




