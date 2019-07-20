#include"libww/ww_battle_section.hpp"
#include"libww/ww_context.hpp"




namespace ww{




void
battle_section::
loop_battle(gbstd::execution&  exec, battle_section&  b) noexcept
{
  exec.push_frame({{"*",start_battle,b}});
}


void
battle_section::
start_battle(gbstd::execution&  exec, battle_section&  b) noexcept
{
//  ctx.m_left_party.clear();
//  ctx.m_right_party.clear();

//  ctx.m_left_party.push( {{u"ひだりぐんA",800,},{u"ひだりぐんB",800,},{u"ひだりぐんC",800,},});
//  ctx.m_right_party.push({{u"みぎぐんA",800,},{u"みぎぐんB",800,},{u"みぎぐんC",800,},});

//  ctx.s_battle.entry_party( ctx.m_left_party,battles::sides::left );
//  ctx.s_battle.entry_party(ctx.m_right_party,battles::sides::right);


  ++exec;

  exec.push_frame({
    {"*",initialize_battle,b},
    {"*",       run_battle,b},
    {"*",  finalize_battle,b},
  });
}


void
battle_section::
initialize_battle(gbstd::execution&  exec, battle_section&  b) noexcept
{
  ++exec;
}


battle_result
battle_section::
judge_battle() const noexcept
{
  int  l = 0;
  int  r = 0;

    for(auto&  c: m_character_table)
    {
        if(c.can_continue_battle())
        {
            if(c.m_side.is_left()){++l;}
          else                    {++r;}
        }
    }


  return (l && r)? battle_result::continuing
        :(l     )? battle_result::left_won
        :(     r)? battle_result::right_won
        :          battle_result::draw;
}




bool
battle_section::
judge_hit(const battles::character&  actor, const battles::character&  target) const noexcept
{
  static gbstd::uniform_rand  hit_rand(1,10);

  return (hit_rand() <= 8);
}


int
battle_section::
calculate_damage(const battles::character&  actor, const battles::character&  target) const noexcept
{
  static gbstd::uniform_rand  damage_rand(80,1000);

  return damage_rand();
}


void
battle_section::
distribute_ap(int  v) noexcept
{
    for(auto&  c: m_character_table)
    {
        if(c.can_continue_battle())
        {
          c.m_ap += v;
        }
    }
}


battles::character*
battle_section::
get_battle_character_by_ap() noexcept
{
  battles::character*  highest = nullptr;

    for(auto&  c: m_character_table)
    {
        if(!highest || (c.m_ap > highest->m_ap))
        {
          highest = &c;
        }
    }


  return highest;
}


void
battle_section::
fight(gbstd::execution&  exec) noexcept
{
  struct tester{
    battles::side  m_side;

    bool  operator()(const battles::character&  c) const noexcept
    {
      return (c.m_side == m_side);
    }

  };


START:
  auto  actor = get_battle_character_by_ap();

    if(actor && (actor->m_ap >= battles::character::m_ap_max))
    {
      actor->m_ap -= battles::character::m_ap_max;

      static gbstd::uniform_rand  rand;

      auto  opposite_side = actor->m_side.get_opposite();

      auto&  table = filter(tester{opposite_side});

      auto  n = table.size();

        if(n >= 1)
        {
          rand.reset(0,n-1);

          auto&  target = table[rand()];

//          auto  target_i = target.m_index;

/*
          q.emplace_back(battle_action_kind::attack,target_i);

            if(judge_hit(*actor,target))
            {
              auto  damage = calculate_damage(*actor,target);

              q.emplace_back(battle_action_kind::receive,0,target_i);
            }

          else
            {
              q.emplace_back(battle_action_kind::avoid,0,target_i);
            }
*/
        }


      exec.push_frame({
        {"*",wait_until_end_movie,*this},
      });
    }

  else
    {
      constexpr int  v = 33;

      distribute_ap( v);

      goto START;
    }
}


void
battle_section::
run_battle(gbstd::execution&  exec, battle_section&  b) noexcept
{
    if(!gbstd::g_input.test_start() && gbstd::g_modified_input.test_start())
    {
      ++exec;
    }

return;
  struct tester{
    battles::side  m_side;

    bool  operator()(const battles::character&  c) const noexcept
    {
      return (c.m_side == m_side);
    }

  };


  auto  res = b.judge_battle();

    if(res == battle_result::continuing)
    {
      b.fight(exec);

      return;
    }


    if(res == battle_result::left_won)
    {
      auto&  table = b.filter(tester{battles::sides::left});

        for(auto&  c: table)
        {
        }
    }

  else
    if(res == battle_result::right_won)
    {
      auto&  table = b.filter(tester{battles::sides::right});

        for(auto&  c: table)
        {
        }
    }

  else
    {
    }


  ++exec;
}


void
battle_section::
wait_until_end_movie(gbstd::execution&  exec, battle_section&  b) noexcept
{
}


void
battle_section::
finalize_battle(gbstd::execution&  exec, battle_section&  b) noexcept
{

  ++exec;
}




}




