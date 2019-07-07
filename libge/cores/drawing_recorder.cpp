#include"libge/core.hpp"




namespace ge{


struct
drawing_recorder::
record
{
  record*  next;

  bool  solid;

  size_t  number_of_dots;

  dot  dots[];

};




drawing_recorder&
drawing_recorder::
operator=(drawing_recorder&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap( m_dot_buffer, rhs.m_dot_buffer);
      std::swap(m_record_list,rhs.m_record_list);
    }


  return *this;
}




bool
drawing_recorder::
rollback(const gbstd::canvas&  cv) noexcept
{
START:
    if(m_dot_buffer.size())
    {
      auto  dot = m_dot_buffer.back();

      m_dot_buffer.pop_back();

      cv.get_pixel_pointer(dot.x,dot.y)->color = dot.color;

      return true;
    }

  else
    if(m_record_list)
    {
      --m_number_of_records;


      auto  rec = m_record_list            ;
                  m_record_list = rec->next;

        if(rec->solid)
        {
            for(int  i = 0;  i < rec->number_of_dots;  ++i)
            {
              auto&  dot = rec->dots[i];

              cv.get_pixel_pointer(dot.x,dot.y)->color = dot.color;
            }


          free(rec);

          return true;
        }

      else
        {
            for(int  i = 0;  i < rec->number_of_dots;  ++i)
            {
              m_dot_buffer.emplace_back(rec->dots[i]);
            }


          free(rec);

          goto START;
        }
    }


  return false;
}


bool
drawing_recorder::
push(bool  solid) noexcept
{
  int  n = m_dot_buffer.size();

    if(n)
    {
      auto  rec_size = sizeof(record)+(sizeof(dot)*n);

      auto  rec = reinterpret_cast<record*>(malloc(rec_size));

      rec->solid = solid;

      rec->number_of_dots = n;

      std::copy(m_dot_buffer.cbegin(),m_dot_buffer.cend(),rec->dots);

      m_dot_buffer.resize(0);

      rec->next = m_record_list      ;
                  m_record_list = rec;


      ++m_number_of_records;

      return true;
    }


  return false;
}


void
drawing_recorder::
clear() noexcept
{
  m_number_of_records = 0;

    while(m_record_list)
    {
      auto  next = m_record_list->next;

      free(m_record_list)      ;
           m_record_list = next;
    }


  m_record_list = nullptr;

  m_dot_buffer.resize(0);
}



}




