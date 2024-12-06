// Character2.h
#ifndef CHARACTER2_H_INCLUDED
#define CHARACTER2_H_INCLUDED

#include "CharacterBase.h"

class Character2 : public CharacterBase {
public:
    void init() override;  // 僅聲明 init() 函數，不在此初始化 gifPath
};

#endif
