#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




operating_node::
operating_node() noexcept
{
}




void
operating_node::
relay_redraw_request(node&  nd) noexcept
{
  m_queue.emplace_back(&nd);

  nd.get_status().set(flags::queued);
}


void
operating_node::
relay_reform_request() noexcept
{
  m_reform_status = 1;
}




void
operating_node::
reform_if_necessary() noexcept
{
    if(m_reform_status == 1)
    {
      reform(point());

      m_queue.clear();

      m_reform_status = 2;
    }
}


bool
operating_node::
redraw_if_necessary() noexcept
{
  reform_if_necessary();

    if(m_reform_status == 2)
    {
      m_canvas.fill(get_style().get_background_color());

      redraw(m_canvas);

      m_reform_status = 0;

      return true;
    }

  else
    if(m_queue.size())
    {
        for(auto  nodeptr: m_queue)
        {
          nodeptr->redraw(m_canvas);
        }


      m_queue.clear();

      return true;
    }


  return false;
}


void
operating_node::
process_by_mouse_position(point&  pt) noexcept
{
    if(!m_current)
    {
      m_current = get_node_by_point(pt);

        if(m_current)
        {
          m_current->on_mouse_enter();
        }
    }

  else
    if(!m_current->test_by_point(pt))
    {
        if(g_input.test_mouse_left() ||
           g_input.test_mouse_right())
        {
          auto  cnt_pt = m_current->get_content_position();

          int    left = cnt_pt.x                                  ;
          int   right = cnt_pt.x+m_current->get_content_width() -1;
          int     top = cnt_pt.y                                  ;
          int  bottom = cnt_pt.y+m_current->get_content_height()-1;

               if(pt.x <  left){pt.x =  left;}
          else if(pt.x > right){pt.x = right;}

               if(pt.y <    top){pt.y =    top;}
          else if(pt.y > bottom){pt.y = bottom;}
        }

      else
        {
          m_current->on_mouse_leave();

          m_current = get_node_by_point(pt);

            if(m_current)
            {
              m_current->on_mouse_enter();
            }
        }
    }
}


void
operating_node::
process_user_input(point  pt) noexcept
{
  process_by_mouse_position(pt);

    if(m_current)
    {
      m_current->on_mouse_act(pt-m_current->get_content_position());
    }
}


node&
operating_node::
create_table_column(std::initializer_list<std::reference_wrapper<node>>  ls) noexcept
{
  auto&  nd = create_node();

  nd.add_child_as_column(ls);

  return nd;
}


node&
operating_node::
create_table_row(std::initializer_list<std::reference_wrapper<node>>  ls) noexcept
{
  auto&  nd = create_node();

  nd.add_child_as_row(ls);

  return nd;
}




}}




