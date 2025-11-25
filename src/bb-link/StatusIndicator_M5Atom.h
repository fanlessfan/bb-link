#pragma once
#ifndef STATUS_INDICATOR_M5ATOM_H
#define STATUS_INDICATOR_M5ATOM_H

#include "StatusIndicatorBase.h"
#include <M5Atom.h>

class StatusIndicator_M5Atom : public StatusIndicatorBase
{
public:
    StatusIndicator_M5Atom() {}

    void init() override
    {
        M5.dis.setBrightness(20);  // Slightly dimmer than full to save power
        set(disconnected);          // Start off
    }

    void render() override
    {
        // drawpix() updates immediately, no need for extra render loop
    }

    void set(status_t state) override
    {
        uint32_t color = 0x000000;  // Off by default

        switch (state)
        {
        case disconnected:
            color = 0x000000;        // Off
            break;
        case connected:
            color = 0x0000FF;        // Blue
            break;
        case scanning:
            color = 0xCCCC00;        // Amber/Yellow
            break;
        case ready:
            color = 0x00FF00;        // Green
            break;
        case rx:
            color = 0x00FFFF;        // Cyan
            break;
        case tx:
            color = 0xFF00FF;        // Magenta
            break;
        case duplex:
            color = 0xFFFFFF;        // White
            break;
        case actionRegistered:
            color = 0xFF8800;        // Orange
            break;
        case error:
        case batteryLow:
        case batteryShutdown:
            color = 0xFF0000;        // Red
            break;
        case shutdown:
            color = 0x800080;        // Purple
            break;
        case batteryFull:
            color = 0x00FF00;        // Green
            break;
        case otaFlash:
            color = 0xAA5500;        // Brownish
            break;
        default:
            color = 0x101010;        // Dim white for unknown
            break;
        }

        M5.dis.drawpix(0, color);
    }

    void sleep() override
    {
        M5.dis.drawpix(0, 0x000000);
    }

    // Remove update() entirely — not in base class
    // void update() override { }  ← DELETE THIS LINE AND FUNCTION
};

#endif // STATUS_INDICATOR_M5ATOM_H