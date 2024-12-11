// #include "Skill.h"
// #include <iostream>

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
