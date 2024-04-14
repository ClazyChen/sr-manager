cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (sr-manager LANGUAGES CXX)

if (MSVC)
  add_compile_options(/utf-8)
  add_compile_options(/EHsc)
endif()

add_executable (sr-manager
    "main.cpp"
    "battle/AddEffect.hpp"
    "battle/Attack.hpp"
    "battle/Battle.hpp"
    "battle/BattleUnit.hpp"
    "battle/Break.hpp"
    "battle/CostSkillPoint.hpp"
    "battle/Die.hpp"
    "battle/Dying.hpp"
    "battle/EmitSkill.hpp"
    "battle/Game.hpp"
    "battle/GameStart.hpp"
    "battle/GetSkillPoint.hpp"
    "battle/HpDown.hpp"
    "battle/HpUp.hpp"
    "battle/MpDown.hpp"
    "battle/MpUp.hpp"
    "battle/Procedure.hpp"
    "battle/ReduceToughness.hpp"
    "battle/SpeedBar.hpp"
    "battle/TurnBegin.hpp"
    "cli/BattleCli.hpp"
    "cli/CreatorCli.hpp"
    "cli/HpBar.hpp"
    "cli/Painter.hpp"
    "utils/Character.hpp"
    "utils/CharacterData.hpp"
    "utils/Effect.hpp"
    "utils/EffectList.hpp"
    "utils/Random.hpp"
    "utils/Skill.hpp"
    "utils/Trigger.hpp"
    "utils/TriggerList.hpp"
    "utils/Utils.hpp"
    "utils/ai/Target.hpp"
    "utils/effects/Bleed.hpp"
    "utils/effects/Burn.hpp"
    "utils/effects/Fast.hpp"
    "utils/effects/ResistDown.hpp"
    "utils/effects/Slow.hpp"
    "utils/effects/TempAtk.hpp"
    "utils/effects/TempPen.hpp"
    "utils/effects/WindShear.hpp"
    "utils/Fire/Himeko/Himeko.hpp"
    "utils/Fire/Himeko/HimekoAeq.hpp"
    "utils/Fire/Himeko/HimekoPassive.hpp"
    "utils/Fire/Himeko/HimekoStars.hpp"
    "utils/physical/TailblazerPhysical/ADestructingGlance.hpp"
    "utils/physical/TailblazerPhysical/AFallingStar.hpp"
    "utils/physical/TailblazerPhysical/AnUnwillingHost.hpp"
    "utils/physical/TailblazerPhysical/ATrailblazingWill.hpp"
    "utils/physical/TailblazerPhysical/FarewellHit.hpp"
    "utils/physical/TailblazerPhysical/ImmortalThirdStrike.hpp"
    "utils/physical/TailblazerPhysical/PerfectPickoff.hpp"
    "utils/physical/TailblazerPhysical/RipHomeRun.hpp"
    "utils/physical/TailblazerPhysical/StardustAce.hpp"
    "utils/physical/TailblazerPhysical/TailblazerPhysical.hpp"
    "utils/wind/DanHeng/CloudlancerArtNorthWind.hpp"
    "utils/wind/DanHeng/CloudlancerArtTorrent.hpp"
    "utils/wind/DanHeng/DanHeng.hpp"
    "utils/wind/DanHeng/DanHengStars.hpp"
    "utils/wind/DanHeng/EtherealDream.hpp"
    "utils/wind/DanHeng/SplittingSpearhead.hpp"
    "utils/wind/DanHeng/SuperiorityOfReach.hpp"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET sr-manager PROPERTY CXX_STANDARD 20)
endif()
