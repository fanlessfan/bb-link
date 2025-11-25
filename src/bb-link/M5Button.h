#pragma once
#ifndef M5BUTTON_H
#define M5BUTTON_H

#include "ButtonBase.h"
#include <M5Atom.h>  // Provides M5.Btn

class M5Button : public ButtonBase
{
public:
    M5Button() = default;

    void init() override
    {
        // M5.begin() already initializes Btn — nothing more needed
    }

    void process() override
    {
        // M5.update() MUST be called in Adapter::perform() before this

        // Long press: held for 2+ seconds
        if (M5.Btn.pressedFor(2000))
        {
            if (onLongPressed)
            {
                onLongPressed();
            }
            return;  // Consume the event (don't trigger short press)
        }

        // Short press: quick release after press
        if (M5.Btn.wasReleased())
        {
            if (onShortPressed)
            {
                onShortPressed();
            }
        }
    }

    void setOnShortPressed(std::function<void(void)> cb) override
    {
        onShortPressed = cb;
    }

    void setOnLongPressed(std::function<void(void)> cb) override
    {
        onLongPressed = cb;
    }

private:
    std::function<void(void)> onShortPressed = nullptr;
    std::function<void(void)> onLongPressed = nullptr;
};

#endif // M5BUTTON_H