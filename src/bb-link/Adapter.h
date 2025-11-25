#pragma once
#ifndef ADAPTER_H
#define ADAPTER_H

#include "Arduino.h"



#if defined(ARDUINO_M5Stack_ATOM)
#include <M5Atom.h>
//#include <Adafruit_NeoPixel.h>
//#define PIN       27  //定义NeoPixel的控制引脚
//#define NUMPIXELS 1   //定义NeoPixel控制灯灯数量
#endif

#if defined(ARDUINO_TINYPICO)
#include <TinyPICO.h>
#endif

#include "StatusIndicatorBase.h"
#include "StatusIndicatorDummy.h"
#if defined(ARDUINO_TINYPICO)
#include "StatusIndicator.h"
#elif defined(ARDUINO_M5Stack_ATOM)  // ADDED: Conditional include for ATOM
#include "StatusIndicator_M5Atom.h"  // ADDED: New Status Indicator for M5.dis.drawpix()
#endif

#include "ButtonBase.h"
#include "ButtonDummy.h"
//#include "TouchButton.h"
//#include "PhysicalButton.h"

//#include "TouchButtonBase.h"
//#include "TouchButtonDummy.h"
#if defined(ARDUINO_TINYPICO)
#include "TouchButton.h"
#elif defined(ARDUINO_M5Stack_ATOM)  // ADDED: Conditional include for TouchButton
//#define ATOM_LITE_BUTTON_PIN 39   // The onboard physical button (GPIO39)
//#define ATOM_LITE_NEOPIXEL_PIN 27 // The onboard SK6812 RGB LED (GPIO27)
#include "M5Button.h"
#endif

#include "Bridge.h"
#include "FiniteStateMachine.h"

#include <esp_ota_ops.h>

#define CAPACITIVE_TOUCH_INPUT_PIN T0  // Pin 4
#define ADAPTER_NAME "B.B. Link"       // Changing this will prevent RadioMail to know that QSY is supported.

#define FIRMWARE_VERSION_MAJOR 0
#define FIRMWARE_VERSION_MINOR 7
#define FIRMWARE_VERSION_PATCH 8

#define DEVICE_NAMESPACE "bb-link-hw"
#define IDENTITY_KEY "identity"

enum hardware_board_t {
  hardware_board_unknown = 0,
  hardware_board_tinypico = 1,
  hardware_board_pico32 = 2,
  hardware_board_m5atom = 3
};


#if defined(ARDUINO_TINYPICO)
#define HARDWARE_BOARD hardware_board_tinypico
#define HARDWARE_VERSION_MAJOR 3
#define HARDWARE_VERSION_MINOR 0
#endif

#if defined(ARDUINO_ESP32_PICO)
#define HARDWARE_BOARD hardware_board_pico32
#define HARDWARE_VERSION_MAJOR 1
#define HARDWARE_VERSION_MINOR 0
#endif

#if defined(ARDUINO_M5Stack_ATOM)
#define HARDWARE_BOARD hardware_board_m5atom
#define HARDWARE_VERSION_MAJOR 1
#define HARDWARE_VERSION_MINOR 0
//#error "M5Stack ATOM hardware board from ESP32."
#endif

#if defined(ARDUINO_M5Stick_C)
#define HARDWARE_BOARD hardware_board_m5stick_c
#define HARDWARE_VERSION_MAJOR 1
#define HARDWARE_VERSION_MINOR 0
//#error "M5Stack Stick_C hardware board from ESP32."
#endif

#if defined(ARDUINO_M5STACK_ATOM)
#define HARDWARE_BOARD hardware_board_m5atom
#define HARDWARE_VERSION_MAJOR 1
#define HARDWARE_VERSION_MINOR 0
//#error "M5STACK ATOM hardware board from M5STACK."
#endif

#if defined(ARDUINO_M5STACK_STICKC_PLUS)
#define HARDWARE_BOARD hardware_board_m5stick_cp
#define HARDWARE_VERSION_MAJOR 1
#define HARDWARE_VERSION_MINOR 0
//#error "M5STACK_STICKC_PLUS hardware board from M5STACK."
#endif

#if !defined(HARDWARE_BOARD)
#error "Unknown hardware board. Please define HARDWARE_BOARD so that OTA updater knows what to do."
#endif

const char *getHardwareName();

DECLARE_STATE(AdapterState);

enum shutdown_reason_t {
  userInitiated = 0x00,
  idleTimeout = 0x01,
  lowBattery = 0x02
};

class Adapter : public BLECharacteristicCallbacks {
public:
  Adapter();
  void init();
  void perform();
  Bridge bridge;
  String getAdapterName();

private:
#if defined(ARDUINO_TINYPICO)
  StatusIndicator statusIndicator = StatusIndicator(tp);
  TouchButton button = TouchButton(CAPACITIVE_TOUCH_INPUT_PIN);

#elif defined(ARDUINO_M5Stack_ATOM)
  StatusIndicator_M5Atom statusIndicator = StatusIndicator_M5Atom();
  M5Button button = M5Button();  // ← Uses official M5.Btn

#else
  StatusIndicatorDummy statusIndicator = StatusIndicatorDummy();
  ButtonDummy button = ButtonDummy();
#endif

  unsigned long lastBatteryCheck = 0;
  shutdown_reason_t shutdownReason;

  AdapterState idleState;
  AdapterState inUseState;
  AdapterState shutdownState;
  AdapterState showBatteryState;
  AdapterState otaFlashState;
  FSMT<AdapterState> adapterStateMachine;

  BLECharacteristic *pOtaFlash;
  BLECharacteristic *pOtaIdentity;
  esp_ota_handle_t otaHandle = 0;

  void verifyFirmware();
  void onLongPressed();
  void onShortPressed();
  void lowBatteryWatchguard();
  void updateSendReceiveStatus();
  bool isUSBPower();
  void doShutdown();

  void shutdownEnter();
  void shutdownUpdate();
  void shutdownExit();
  void idleEnter();
  void idleUpdate();
  void idleExit();
  void inUseEnter();
  void inUseUpdate();
  void inUseExit();
  void showBatteryEnter();
  void showBatteryUpdate();
  void showBatteryExit();
  void otaFlashEnter();
  void otaFlashUpdate();
  void otaFlashExit();

  void initBLEOtaService();

  void onWrite(BLECharacteristic *pCharacteristic);
  void onRead(BLECharacteristic *pCharacteristic);

  String fetchAdapterName();
};

#endif