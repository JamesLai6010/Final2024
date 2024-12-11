// #ifndef SKILL_H_INCLUDED
// #define SKILL_H_INCLUDED

// #include "../Character/CharacterBase.h"
// #include <iostream>

// class CharacterBase;

// class Skill {
// public:
//     virtual void execute(CharacterBase& caster, CharacterBase& target) = 0; // 執行技能
//     virtual ~Skill() {}
// };

// // 第一個技能：持續扣血
// class DamageOverTimeSkill : public Skill {
// public:
//     void execute(CharacterBase& caster, CharacterBase& target) override {
//         target._set_HP(-50); // 直接扣除 50 點生命值
//         std::cout << "Damage Over Time Skill activated! Target HP: " << target._get_HP() << "\n";
//     }
// };

// // 第二個技能：擊退
// class KnockbackSkill : public Skill {
// public:
//     void execute(CharacterBase& caster, CharacterBase& target) override {
//         double direction = (caster._get_dir() ? -1 : 1); // 根據角色朝向決定方向
//         target.shape->update_center_x(target.shape->center_x() + direction * 50); // 擊退 50 單位
//         std::cout << "Knockback Skill activated! Target position updated.\n";
//     }
// };

// #endif
