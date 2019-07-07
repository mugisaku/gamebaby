#include"libgbstd/process.hpp"




namespace gbstd{




namespace{
constexpr uint32_t  g_offset_of_frame_nameptr = 0;
constexpr uint32_t  g_offset_of_previous_pc   = 1;
constexpr uint32_t  g_offset_of_previous_bp   = 2;
constexpr uint32_t  g_offset_of_entry_array   = 3;
constexpr uint32_t  g_size_of_entry           = 3;
}



execution&
execution::
operator++() noexcept
{
    if(m_status.test(flags::pc_barrier))
    {
      printf("execution error: pc is barriered\n");
    }

  else
    {
      m_pc += g_size_of_entry;

      unset_jump();
    }


  return *this;
}


execution&
execution::
clear() noexcept
{
    for(auto  ptr: m_task_list)
    {
      ptr->finish();
    }


  m_clock_list.clear();
  m_task_list.clear();

  m_status.clear();

  return *this;
}


execution&
execution::
reset() noexcept
{
  m_pc = 0;
  m_lc = 0;
  m_sp = 0;
  m_bp = 0;

  m_equipped_clock.reset();

  m_memory[g_offset_of_previous_pc] = 0;
  m_memory[g_offset_of_previous_bp] = 0;

  m_status.clear();

  return *this;
}




execution_context
execution::
get_context() const noexcept
{
  execution_context  ctx;

  ctx.m_pc = m_pc;
  ctx.m_lc = m_lc;
  ctx.m_sp = m_sp;
  ctx.m_bp = m_bp;

  return ctx;
}


int
execution::
set_jump(execution_context&  ctx) noexcept
{
    if(m_status.test(flags::jumped))
    {
      return m_jump_value;
    }


  ctx = get_context();

  return 0;
}


void
execution::
pop_current_context(int  v) noexcept
{
    if(m_context_stack.empty())
    {
      report;
    }

  else
    {
      jump(m_context_stack.back(),v);

      m_context_stack.pop_back();
    }
}




void
execution::
jump(const execution_context&  ctx, int  v) noexcept
{
  m_pc = ctx.m_pc;
  m_lc = ctx.m_lc;
  m_sp = ctx.m_sp;
  m_bp = ctx.m_bp;

  m_status.set(flags::jumped);

  m_jump_value = !v? 1:v;
}




execution&
execution::
push_frame(std::initializer_list<execution_entry>  ls, const char*  name) noexcept
{
  auto  prev_pc = m_pc;
  auto  prev_bp = m_bp;

  m_bp = m_sp                           ;
         m_sp += g_offset_of_entry_array;


  m_pc = m_sp;

  m_memory[m_bp+g_offset_of_frame_nameptr] = name? reinterpret_cast<uintptr_t>(name):0;
  m_memory[m_bp+g_offset_of_previous_pc]   = prev_pc;
  m_memory[m_bp+g_offset_of_previous_bp]   = prev_bp;

    for(auto  ent: ls)
    {
      m_memory[m_sp++] = ent.m_nameptr;
      m_memory[m_sp++] = ent.m_callback;
      m_memory[m_sp++] = ent.m_data;
    }


  ++m_lc;

    if(m_status.test(flags::verbose))
    {
      print();

      printf("\n*frame:\"%s\" pushed\n\n\n",name? name:"-noname-");
    }


  m_status.set(flags::pc_barrier);

  unset_jump();

  return *this;
}


execution&
execution::
pop_frame() noexcept
{
    if(m_lc)
    {
      auto  name = reinterpret_cast<const char*>(m_memory[m_bp+g_offset_of_frame_nameptr]);

        if(test_verbose_flag())
        {
          print();

          printf("\n*frame:\"%s\" popped\n\n\n",name? name:"-noname-");
        }


      m_pc = m_memory[m_bp+g_offset_of_previous_pc];

      m_sp = m_bp                                         ;
             m_bp = m_memory[m_bp+g_offset_of_previous_bp];

      --m_lc;
    }


  unset_jump();

  return *this;
}




execution&
execution::
remove_clocks_by_name(std::string_view  name) noexcept
{
  auto  it = m_clock_list.begin();

    while(it != m_clock_list.end())
    {
        if((*it)->get_name() == name)
        {
          it = m_clock_list.erase(it);
        }

      else
        {
          ++it;
        }
    }


  return *this;
}


execution&
execution::
remove_tasks_by_name(std::string_view  name) noexcept
{
  auto  it = m_task_list.begin();

    while(it != m_task_list.end())
    {
        if((*it)->get_name() == name)
        {
          (*it)->finish();

          it = m_task_list.erase(it);
        }

      else
        {
          ++it;
        }
    }


  return *this;
}


execution&
execution::
remove_clock_by_id(std::string_view  id) noexcept
{
  auto  it = m_clock_list.begin();

    while(it != m_clock_list.end())
    {
        if((*it)->get_id() == id)
        {
          m_clock_list.erase(it);

          break;
        }

      else
        {
          ++it;
        }
    }


  return *this;
}


execution&
execution::
remove_task_by_id(std::string_view  id) noexcept
{
  auto  it = m_task_list.begin();

    while(it != m_task_list.end())
    {
        if((*it)->get_id() == id)
        {
          (*it)->finish();

          m_task_list.erase(it);

          break;
        }

      else
        {
          ++it;
        }
    }


  return *this;
}


execution&
execution::
remove_dead_tasks() noexcept
{
  auto  it = m_task_list.begin();

    while(it != m_task_list.end())
    {
        if(!(*it)->is_living())
        {
          (*it)->finish();

          it = m_task_list.erase(it);
        }

      else
        {
          ++it;
        }
    }


  return *this;
}




pointer_wrapper<clock>
execution::
get_clock_by_id(std::string_view  id) const noexcept
{
    for(auto  ptr: m_clock_list)
    {
        if(ptr->get_id() == id)
        {
          return ptr;
        }
    }


  return nullptr;
}


pointer_wrapper<task>
execution::
get_task_by_id(std::string_view  id) const noexcept
{
    for(auto  ptr: m_task_list)
    {
        if(ptr->get_id() == id)
        {
          return ptr;
        }
    }


  return nullptr;
}




void
execution::
print() const noexcept
{
  printf("{pc:%3u, bp:%3u, sp: %3u}\n",m_pc,m_bp,m_sp);

  printf("clock_list: {\n");
 
    for(auto  ptr: m_clock_list)
    {
      printf("{name:\"%s\", id:\"%s\", time: %8u}\n",ptr->get_name().data(),ptr->get_id().data(),ptr->get_time());
    }


  printf("}\n");

  printf("task_list: {\n");
 
    for(auto  ptr: m_task_list)
    {
      printf("{name:\"%s\", id:\"%s\", next_time:%d, %s}\n",ptr->get_name().data(),ptr->get_id().data(),
        ptr->get_next_time(),
        !ptr->is_living()? "dead"
       :ptr->is_sleeping()? "sleeping"
       :"living");
    }


  printf("}\n");
}




}




