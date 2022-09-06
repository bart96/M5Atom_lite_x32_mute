/*
 * the following code uses the Behringer X32 OSC protocol to mute / unmute a specified channel on master or a bus
 * the device subscribes to the channel to display mute / unmute status changes
 * if no button is pressed for some time (idle), the esp32 goes into deep sleep mode and is woken up again by pressing the button
 * (the deep sleep mode can also be achieved by pressing the button for more than 2 seconds)
 * 
 * the code is aligned to run on an esp32 "M5Stack Atom Light" which has a single button, a rgb led and wifi https://shop.m5stack.com/products/atom-lite-esp32-development-kit
 * 
 * see also "UNOFFICIAL X32/M32 OSC REMOTE PROTOCOL" by Patrick-Gilles Maillot https://sites.google.com/site/patrickmaillot/ for detailed information of Behringers X32 / Midas M32 OSC protocol
*/
#include <M5Atom.h> // https://github.com/m5stack/m5-docs/blob/master/docs/en/core/atom_lite.md
#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h> // https://github.com/CNMAT/OSC

#include "config.h"

const IPAddress X32_IP;
const unsigned int x32Port = X32_PORT;

WiFiUDP udp;

OSCMessage msgSubscribe("/subscribe");
OSCMessage msg(OSC_CHANNEL);
OSCMessage msgReceive;

bool mute = true;
int loopCounter = 0;
int idleTime = IDLE_SECONDS * 10;
int receivedPacketSize = 0;

unsigned long lastSubscribeCommand = 0;
unsigned long buttonTime = BUTTON_GO_TO_SLEEP_SECONDS * 1000;
unsigned long buttonPressedTime = 0;

void setup() {
  Serial.begin(115200);
  M5.begin(true, false, true);

  delay(50);

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_39, 0);

  const char* ssid = WLAN_SSID;
  const char* password = WLAN_PASSWORD;
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    connectingWiFi(250);
  }

  msgSubscribe.add(OSC_CHANNEL);
  msgSubscribe.add(OSC_SUBSCRIBE_UPDATES);

  udp.begin(x32Port);
  subscribe();

  delay(200);
}

void loop() {
  subscribe();

  receivedPacketSize = udp.parsePacket();
  if (receivedPacketSize > 0) {
    while (receivedPacketSize--) {
      msgReceive.fill(udp.read());
    }

    if (!msgReceive.hasError()) {
      msgReceive.dispatch(OSC_CHANNEL, receive);
    }

    msgReceive.empty();
  }


  M5.update();

  if (buttonPressedTime == 0) {
    if (M5.Btn.isPressed()) {
      buttonPressedTime = millis();
    }
  }
  else {
    if (!M5.Btn.isPressed()) {
      unsigned long diff = millis() - buttonPressedTime;

      if (diff > buttonTime) {
        goToSleep();
      }
      else {
        if (mute) {
          send(OSC_UNMUTE);
        }
        else {
          send(OSC_MUTE);
        }
      }

      loopCounter = 0;
      buttonPressedTime = 0;
    }
  }


  if (loopCounter++ > idleTime) {
    goToSleep();
  }

  delay(100);
}

void subscribe() {
  if (millis() - lastSubscribeCommand > 9000) {
    lastSubscribeCommand = millis();

    udp.beginPacket(x32IP, x32Port);
    msgSubscribe.send(udp);
    udp.endPacket();
  }
}

void send(int value) {
  msg.add(value);

  udp.beginPacket(x32IP, x32Port);
  msg.send(udp);
  udp.endPacket();

  msg.empty();
}

void receive(OSCMessage &msg) {
  if (msg.isInt(0)) {
    switch (msg.getInt(0)) {
      case OSC_MUTE:
        mute = true;
        led(CMUTE);
        break;
      case OSC_UNMUTE:
        mute = false;
        led(CUNMUTE);
        break;
      default:
        led(CPINK);
    }
    Serial.println(msg.getInt(0));
  }
}

void led(uint32_t color) {
  M5.dis.drawpix(0, color);
}

void connectingWiFi(int delayTime) {
  led(CPINK);
  delay(delayTime);
  led(CBLACK);
  delay(delayTime);
}

void goToSleep() {
  for (int i = 0; i < 4; i++) {
    led(CWHITE);
    delay(200);
    led(CBLACK);
    delay(150);
  }

  esp_deep_sleep_start();
}
