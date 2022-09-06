# ESP32 Behringer X32 OSC mute / unmute

This project uses the Behringer X32 OSC protocol to mute / unmute a specified channel on master or a bus
the device subscribes to the channel to display mute / unmute status changes
if no button is pressed for some time (idle), the esp32 goes into deep sleep mode and is woken up again by pressing the button
(the deep sleep mode can also be achieved by pressing the button for more than 2 seconds)

the code is aligned to run on an esp32 "M5Stack Atom Light" which has a single button, a rgb LED and wifi https://shop.m5stack.com/products/atom-lite-esp32-development-kit

> see also "UNOFFICIAL X32/M32 OSC REMOTE PROTOCOL" by Patrick-Gilles Maillot https://sites.google.com/site/patrickmaillot/ for detailed information of Behringers X32 / Midas M32 OSC protocol