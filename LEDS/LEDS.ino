// Adafruit IO Publish & Subscribe Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
//int count = 0;
String LED_ROJA;
String LED_AMA;
String temperatura;

// Track time of last published messages and limit feed->save events to once
// every IO_LOOP_DELAY milliseconds.
//
// Because this sketch is publishing AND subscribing, we can't use a long
// delay() function call in the main loop since that would prevent io.run()
// from being called often enough to receive all incoming messages.
//
// Instead, we can use the millis() function to get the current time in
// milliseconds and avoid publishing until IO_LOOP_DELAY milliseconds have
// passed.
#define IO_LOOP_DELAY 5000
unsigned long lastUpdate = 0;

// set up the 'counter' feed
AdafruitIO_Feed *LED_R = io.feed("LED_ROJA");
AdafruitIO_Feed *LED_A = io.feed("LED_AMA");
//AdafruitIO_Feed *Temp  = io.feed("TEMPERATURA");

void setup() {

  // start the serial connection
  Serial.begin(9600);
  Serial2.begin(9600,SERIAL_8N1,16,17);

  // wait for serial monitor to open
  while(! Serial);
  while(! Serial2);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  LED_R->onMessage(handleMessage1);
  LED_A->onMessage(handleMessage2);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  LED_R->get();
  LED_A->get();

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  //LED_ROJA -> save(count);
  //LED_AMA -> save(count);

  /*if ( Serial2.read() == '#' ){

  temperatura = "";
  temperatura = temperatura + Serial2.read();
  temperatura = temperatura + Serial2.read();
  temperatura = temperatura + char(Serial2.read());
  temperatura = temperatura + Serial2.read();
  temperatura = temperatura + Serial2.read();
  }

  Temp -> save (temperatura);*/
  Serial.print(Serial2.read());
  delay(550);
}

// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleMessage1(AdafruitIO_Data *data) {

LED_ROJA = data->value();

if (LED_ROJA == "ON"){
Serial.print("ON");
Serial2.write(1);
} else if (LED_ROJA == "OFF"){
  Serial.print("OFF");
  Serial2.write(0);
}

}

void handleMessage2(AdafruitIO_Data *data) {

LED_AMA = data->value();

if (LED_AMA == "ON"){
Serial.print("ON");
Serial2.write(3);
} else if (LED_AMA == "OFF"){
  Serial.print("OFF");
  Serial2.write(2);
}

}
