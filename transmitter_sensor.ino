// transmitter.pde
//
// Simple example of how to use VirtualWire to transmit messages
// Implements a simplex (one-way) transmitter with an TX-C1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: transmitter.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $

#include <VirtualWire.h>
#include <dht11.h>
dht11 DHT;

const int led_pin = 11;
const int transmit_pin = 1;
const int receive_pin = 2;
const int sensor_pin = 3;
const String sensor_name = "bed1";
const bool debug_mode = false;

void setup()
{
  delay(1000);
  
  if (debug_mode) {
    Serial.begin(115200);	// Debugging only
    Serial.println("Starting receiver");
  }
    // Initialise the IO and ISR
    vw_set_tx_pin(transmit_pin);
//    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(500);       // Bits per sec
    pinMode(led_pin, OUTPUT);
}

void loop()
{
    int chk = DHT.read(sensor_pin);
  switch (chk) {
    case DHTLIB_OK:
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.println("Checksum error,\t");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.println("Time out error,\t");
      break;
    default:
      Serial.println("Unknown error,\t");
      break;
  }
  
  
  int temp     = (int) DHT.temperature; //accuracy is only 2deg, so don't bother using floats
  int humidity = (int) DHT.humidity;
  
  String message;
  
  message = "R>" + sensor_name + ":temp=" + (String) temp; // + ";humidity=" + (String) humidity;
  sendMessage(message);
  
  message = "R>" + sensor_name + ":humidity=" + (String) humidity;
  sendMessage(message);

}

void sendMessage(String message) { 
  
  if (debug_mode) {
    Serial.println(message);
  }
  int buf_len = message.length() +1;
  
  char buf[buf_len];
  message.toCharArray(buf, buf_len);
  
  digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
  vw_send((uint8_t *)buf, buf_len);
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(led_pin, LOW);
  delay(2000);
 
}
