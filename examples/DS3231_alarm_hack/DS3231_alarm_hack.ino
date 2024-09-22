/*
Stores data in DS3231 RTC alarms

to set the alarm just type a char in the input, this will call set_alarm()

Ps: You could also just call set_alarm() in the setup too :)
*/

#include <DS3231.h>
#include <Wire.h>

DS3231 myRTC;
RTClib timeRTC;

uint8_t a,b,c,d, flags;
bool dy;

// Print RTC now time 
void print_now() {
    DateTime now = timeRTC.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
}

void set_alarm() {
    Serial.println("Set Alarm1");

    /* 
        Set a real alarm to day 1 (Monday) 2h 3min 4s 
        To trigger when day of week, hours, minutes, and seconds match

        A1M1 A1M2 A1M3 A1M4 (AlarmBits)
        0    0    0    0
        A1Dy:  false = Day of the Month   true = Day of the week
        A1h12: false = use 24h,   true = use 12 AM/PM
        A1PM: use AM or PM
    */
    // myRTC.setA1Time(1,2,3,4,0,true,false,false);
    // myRTC.setValidAlarm(1, true);

    // --- Storing data in alarm ---
    myRTC.setValidAlarm(1, false);

    /* Set alarm that dont trigger every time */
    // myRTC.setA1Data(127, 127, 127, 63, 0, true);

    /* Set alarm using all the bits */
    // myRTC.setA1DataFull(255, 255, 255, 255);

    /* Set alarm using random data */
    a = random(0,255);
    b = random(0,255);
    c = random(0,255);
    d = random(0,255);
    String output = "a: " + String(a) + ", b: " + String(b) + ", c: " + String(c) + ", d: " + String(d);
    Serial.println(output);
    
    myRTC.setA1DataFull(a,b,c,d);
}


void setup() {
    // Start the serial port
    Serial.begin(115200);
    // Start the I2C interface
    Wire.begin();
    // wait a bit to things initalize
    delay(100);

    // Turn off both alarms
    myRTC.turnOffAlarm(1);
    myRTC.turnOffAlarm(2);
}


void loop() {
    if (Serial.available()) {
        char incomingByte = Serial.read();  // Just remove the data to make it unavailable again
        /* Set alarm 
          Input something for setting up the data in the alarm.
          Power off or reboot the board to confirm that the data persists 
        */
        set_alarm();
    }

    // Print current time in the RTC
    print_now();

    /* getA1Data gets the data considering the alarm rate flags */
    Serial.println("get Alarm1");
    myRTC.getA1Data(a,b,c,d, flags, dy);
    Serial.print(a);
    Serial.print(" ");
    Serial.print(b);
    Serial.print(" ");
    Serial.print(c);
    Serial.print(" ");
    Serial.print(d | (dy<<6));
    Serial.println();

    /* getA1DataFull consider all the data in the register */
    Serial.println("get Alarm1 full");
    myRTC.getA1DataFull(a,b,c,d);
    Serial.print(a);
    Serial.print(" ");
    Serial.print(b);
    Serial.print(" ");
    Serial.print(c);
    Serial.print(" ");
    Serial.print(d);
    Serial.println();

    // check if alarm happened and clears it
    bool alarm_happened = myRTC.checkIfAlarm(1) & myRTC.checkValidAlarm(1);
    if (alarm_happened) {
      Serial.println("Alarm 1 has been triggered");
    }
    
    Serial.println();
    // wait 5s
    delay(5000);
}
