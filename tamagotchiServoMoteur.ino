/*******************************************************
  Serial Servo control for robotic hands pressing some buttons to interact with a Tamagotchi
  Based on a sketch written by Ryan Owens for SparkFun Electronics
  7/15/11

  This sketch listens to serial commands and uses the data
  to set the position of two servos.

  Serial Command Structure: 2 bytes - [ID Byte][Servo Position byte]
  ID byte should be 0 or 1.
  Servo position should be a value between 0 and 180.
  Invalid commands are ignored
  The servo position is not error checked.
  If ID byte is 255, then the servo communication is ignored the Arduino resets the Tamagotchi

  Hardware Setup
  Servos should be connected to pins 5 and 6 of the Arduino.
  The Arduino should be powered with an external power and not a computer's USB port, as two servos will draw a lot of current.
*/

#include <Servo.h>  //Used to control the Pan/Tilt Servos
#include <Button.h>  //the button library is used (https://github.com/t3db0t/Button)

//These are variables that hold the servo IDs.
char tiltChannel = 0, panChannel = 1;

Button button = Button(A0, BUTTON_PULLUP_INTERNAL, true, 150);
Button button1 = Button(A1, BUTTON_PULLUP_INTERNAL, true, 150);
Button button2 = Button(A2, BUTTON_PULLUP_INTERNAL, true, 150);

unsigned long lastButton = 0; //last time button was pressed
unsigned long lastButton1 = 0; //last time button1 was pressed
unsigned long lastButton2 = 0; //last time button2 was pressed



//These are the objects for each servo.
Servo servoTilt, servoPan;

void onPress(Button& b) {
  if (b == button) {
    if (millis() - lastButton > 700) { //if the button was not pressed in the last 700 ms (second debounce)
      Serial.println("A"); //notify the computer a button has been pressed
      openDrain(A3, 0); //open drain never writes HIGH to a pin, which would damage a Tamagotchi as it runs on 3 volts and not 5
      delay(100);
      openDrain(A3, 1);
      lastButton = millis();
    }
  }
  if (b == button1) {
    if (millis() - lastButton1 > 700) { //if the button was not pressed in the last 700 ms (second debounce)
      Serial.println("B"); //notify the computer a button has been pressed
      openDrain(A4, 0); //open drain never writes HIGH to a pin, which would damage a Tamagotchi as it runs on 3 volts and not 5
      delay(100);
      openDrain(A4, 1);
      lastButton1 = millis();
    }
  }
  if (b == button2) {
    if (millis() - lastButton2 > 700) { //if the button was not pressed in the last 700 ms (second debounce)
      Serial.println("C"); //notify the computer a button has been pressed
      openDrain(A5, 0); //open drain never writes HIGH to a pin, which would damage a Tamagotchi as it runs on 3 volts and not 5
      delay(100);
      openDrain(A5, 1);
      lastButton2 = millis();
    }
  }
}

//This is a character that will hold data from the Serial port.
unsigned char serialChar = 254;

void setup() {
  servoTilt.attach(5);  //The Tilt servo is attached to pin 5.
  servoPan.attach(6);   //The Pan servo is attached to pin 6.
  servoTilt.write(110);  //Initially put the tilt servo at 110
  servoPan.write(107);  //Initially put the pan servo at 107

  Serial.begin(9600);  //Set up a serial connection for 9600 bps.
  button.releaseHandler(onPress);
  button1.releaseHandler(onPress);
  button2.releaseHandler(onPress);

}

void loop() {
  if (Serial.available() > 0) { //Wait for a character on the serial port.
    serialChar = Serial.read();     //Copy the character from the serial port to the variable
  }
  if (serialChar == 255) { //Check for reset{
    openDrain(9, 0); //open drain never writes HIGH to a pin, which would damage a Tamagotchi as it runs on 3 volts and not 5
    delay(100);
    openDrain(9, 1);
  } else if (serialChar == tiltChannel || serialChar == panChannel) {
    Serial.println((int)serialChar);
    if (serialChar == tiltChannel) { //Check to see if the character is the servo ID for the tilt servo
      while (Serial.available() <= 0); //Wait for the second command byte from the serial port.
      servoTilt.write(Serial.read());  //Set the tilt servo position to the value of the second command byte received on the serial port
    }
    else if (serialChar == panChannel) { //Check to see if the initial serial character was the servo ID for the pan servo.
      while (Serial.available() <= 0); //Wait for the second command byte from the serial port.
      servoPan.write(Serial.read());   //Set the pan servo position to the value of the second command byte received from the serial port.
    }
  }
  if (serialChar != 254) {
    Serial.println((int)serialChar);
  }
  serialChar = 254;
  button.process();
  button1.process();
  button2.process();

  //If the character is not the pan or tilt servo ID, it is ignored.
}

void openDrain(byte pin, bool value)
{ if (value) pinMode(pin, INPUT);
  else pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}
