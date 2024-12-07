#ifndef CHARACTER_FACTORY_H_INCLUDED
#define CHARACTER_FACTORY_H_INCLUDED

#include "CharacterBase.h"
#include "Character1.h"
#include "Character2.h"
// 需要定義其他角色，例如 Character2, Character3, Character4
#include <stdexcept>

class CharacterFactory {
public:
    static CharacterBase* create_character(int number) {
        switch (number) {
            case 1: return new Character1();
            case 2: return new Character2();
            case 3: return new Character2();
            case 4: return new Character2();
            default: throw std::invalid_argument("Invalid character number");
        }
    }
};

#endif
