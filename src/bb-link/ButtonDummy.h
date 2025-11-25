#pragma once
#include "ButtonBase.h"

class ButtonDummy : public ButtonBase
{
public:
    void init() override {}
    void process() override {}
    void setOnShortPressed(std::function<void(void)>) override {}
    void setOnLongPressed(std::function<void(void)>) override {}
};
