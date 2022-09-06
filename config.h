#define WLAN_SSID     "SSID"
#define WLAN_PASSWORD "Password"

#define X32_IP    x32IP(10, 10, 0, 32)
#define X32_PORT  10023

#define IDLE_SECONDS                500
#define BUTTON_GO_TO_SLEEP_SECONDS  2

#define OSC_SUBSCRIBE_UPDATES 5 // updates received over a 10 seconds period Math.max(3, 200 / x) --> 0: 200, 2: 100, 4: 50, 5: 40, ... 40: 5
//#define OSC_CHANNEL "/ch/01/mix/on"     // channel 01 on main
#define OSC_CHANNEL "/ch/01/mix/03/on"    // channel 01 on bus 03
//#define OSC_CHANNEL "/ch/01/mix/04/on"  // channel 01 on bus 04

#define OSC_UNMUTE  1
#define OSC_MUTE    0

#define CBLACK  0
#define CWHITE  0xFFFFFF
#define CRED    0xFF0000
#define CGREEN  0x00FF00
#define CBLUE   0x0000FF
#define CORANGE 0xFF9900
#define CPINK   0xFF0066

#define CMUTE   CRED
#define CUNMUTE CGREEN
//#define CMUTE   CORANGE
//#define CUNMUTE CBLUE
