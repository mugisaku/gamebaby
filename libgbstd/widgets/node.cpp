#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




rectangle
node::
get_content_rectangle() const noexcept
{
  return rectangle(get_content_left(),
                   get_content_top(),
                   get_content_width(),
                   get_content_height());
}


bool
node::
test_by_point(point  pt) const noexcept
{
  auto  pos = get_content_position();

  return((pt.x >=  pos.x                      ) &&
         (pt.y >=  pos.y                      ) &&
         (pt.x <  (pos.x+get_content_width() )) &&
         (pt.y <  (pos.y+get_content_height())));
}


node::iterator
node::
get_node_by_point(point  pt) noexcept
{
    if(test_by_point(pt) && is_displayed())
    {
        if(is_autonomous() || !m_first_child)
        {
          return this;
        }

      else
        {
            for(auto&  child: *this)
            {
                if(child.is_displayed())
                {
                  auto  it = child.get_node_by_point(pt);

                    if(it)
                    {
                      return it;
                    }
                }
            }
        }
    }


  return nullptr;
}


node::iterator
node::
get_node_by_id(std::string_view  id) const noexcept
{
    for(auto&  child: *this)
    {
        if(child.get_id() == id)
        {
          return &child;
        }

      else
        {
          auto  it = child.get_node_by_id(id);

            if(it)
            {
              return it;
            }
        }
    }


  return nullptr;
}




node&
node::
get_nodes_by_name(std::string_view  name, std::vector<iterator>&  ls) noexcept
{
    for(auto&  child: *this)
    {
        if(child.get_name() == name)
        {
          ls.emplace_back(&child);
        }


      child.get_nodes_by_name(name,ls);
    }


  return *this;
}


node&
node::
get_nodes_by_class_name(std::string_view  name, std::vector<iterator>&  ls) noexcept
{
    for(auto&  child: *this)
    {
        if(child.get_class_name() == name)
        {
          ls.emplace_back(&child);
        }


      child.get_nodes_by_class_name(name,ls);
    }


  return *this;
}


std::vector<node::iterator>
node::
get_nodes_by_name(std::string_view  name) noexcept
{
  std::vector<iterator>  ls;

  get_nodes_by_name(name,ls);

  return std::move(ls);
}


std::vector<node::iterator>
node::
get_nodes_by_class_name(std::string_view  name) noexcept
{
  std::vector<iterator>  ls;

  get_nodes_by_class_name(name,ls);

  return std::move(ls);
}




node&
node::
clear() noexcept
{
  auto  child = m_first_child;

    while(child)
    {
      auto  next = child->m_next_sibling;

      delete child       ;
             child = next;
    }


  m_first_child = nullptr;
  m_last_child  = nullptr;

    if(m_previous_sibling)
    {
      m_previous_sibling->m_next_sibling = m_next_sibling;
    }

  else
    if(m_parent)
    {
      m_parent->m_first_child = m_next_sibling;

        if(!m_next_sibling)
        {
          m_parent->m_last_child = nullptr;
        }
    }


    if(m_next_sibling)
    {
      m_next_sibling->m_previous_sibling = m_previous_sibling;
    }

  else
    if(m_parent)
    {
      m_parent->m_last_child = m_previous_sibling;

        if(!m_previous_sibling)
        {
          m_parent->m_first_child = nullptr;
        }
    }


  m_parent           = nullptr;
  m_previous_sibling = nullptr;
  m_next_sibling     = nullptr;

  m_number_of_children = 0;

  m_coworker_node_list.clear();

  return *this;
}


node&
node::
add_child(node&  child, point  pt) noexcept
{
    if(child.m_parent)
    {
      report;

      return *this;
    }


  child.m_parent = this;

    if(m_last_child)
    {
                                 m_last_child->m_next_sibling = &child;
      child.m_previous_sibling = m_last_child                         ;
    }

  else
    {
      m_first_child = &child;
    }


  m_last_child = &child;

  ++m_number_of_children;

  child.m_relative_position = pt;

  request_reform();

  return *this;
}




node&
node::
add_child_as_column(std::initializer_list<std::reference_wrapper<node>>  children) noexcept
{
    for(auto&  child: children)
    {
      child.get().m_follow_style = follow_style::bottom;

      add_child(child.get(),{});
    }


  return *this;
}




node&
node::
add_child_as_row(std::initializer_list<std::reference_wrapper<node>>  children) noexcept
{
    for(auto&  child: children)
    {
      child.get().m_follow_style = follow_style::right;

      add_child(child.get(),{});
    }


  return *this;
}




node&
node::
request_redraw() noexcept
{
    if(m_parent && !m_status.test(flags::queued))
    {
      m_parent->relay_redraw_request(*this);
    }


    for(auto  ptr: m_coworker_node_list)
    {
      ptr->request_redraw();
    }


  return *this;
}


node&
node::
request_perfect_redraw() noexcept
{
    if(m_parent)
    {
      m_parent->request_perfect_redraw();
    }


  return *this;
}


void
node::
reform(point  parent_content_position) noexcept
{
  process_before_reform();

  m_absolute_position = parent_content_position+m_relative_position;

    if(!m_number_of_children)
    {
      return;
    }


  int  w = m_minimal_content_width ;
  int  h = m_minimal_content_height;

  const node*  previous = nullptr;

    for(auto&  child: *this)
    {
        if(child.is_displayed())
        {
          auto  pos = get_content_position();

            if(previous)
            {
                if(child.m_follow_style == follow_style::right)
                {
                  pos = previous->get_absolute_position();

                  pos.x += previous->get_width();
                }

              else
                if(child.m_follow_style == follow_style::bottom)
                {
                  pos = previous->get_absolute_position();

                  pos.y += previous->get_height();
                }
            }


          child.reform(pos);


          auto  child_pos = child.get_content_position()-get_content_position();

          w = std::max(w,child_pos.x+child.get_width() );
          h = std::max(h,child_pos.y+child.get_height());

          previous = &child;
        }
    }


  set_content_width( w);
  set_content_height(h);
}


void
node::
redraw(const canvas&  cv) noexcept
{
    if(!is_hidden() && is_displayed())
    {
      auto  rect = get_content_rectangle();

      render({cv,rect});

        for(auto&  child: *this)
        {
          child.redraw(cv);
        }
    }


  m_status.unset(flags::queued);
}




node&
node::
add_coworker(std::initializer_list<std::reference_wrapper<node>>  ls) noexcept
{
    for(auto&  ref: ls)
    {
      m_coworker_node_list.emplace_back(&ref.get());
    }


  return *this;
}




void
node::
fill(const canvas&  cv, gbstd::color  color) const noexcept
{
  cv.fill_rectangle(color,0,0,get_content_width(),get_content_height());
}


void
node::
fill(const canvas&  cv, color  color0, color  color1, int  interval) const noexcept
{
  cv.draw_stripe_rectangle(color0,color1,interval,0,0,get_content_width(),get_content_height());
}





}}




