#include <Wire.h>

#define DEVICE 0x18

char readed;

void setup() {
  Serial.begin(115200);
  Wire.begin(); // join i2c bus (address optional for master)
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, LOW);
  //setup ctrlReg1
  Serial.println("setting up Control Register");
  Wire.beginTransmission(DEVICE);
  //set ctrl_reg1 to get data from all the axis
  //0x20 is the register address
  //0x07 is x, y, and z axis on
  //0x10 is to set the data acquisition rate (see datasheet)
  //0x10 = 1Hz
  Wire.write(byte(0x20));
  Wire.write(byte(0x07 | 0x10));
  Wire.endTransmission(true);
  Wire.beginTransmission(DEVICE);
  //set hig resolution mode (16bit)
  Wire.write(byte(0x23));
  Wire.write(byte(0x88));
  Wire.endTransmission(false);
  Serial.println("DataRate 1 Hz");
}

void loop() {
  Wire.beginTransmission(DEVICE); // transmit to device #0011000b
  // device address is specified in datasheet
  Wire.write(byte(0x27));  //select register to be read

  Wire.endTransmission(false);     // stop with restart
  Wire.requestFrom(DEVICE, 1, true);
  readed = Wire.read();
  //Serial.println(readed, HEX);
  if (readed&0x08 != 0x00)
    ReadValue();
  delay(100);
}

void ReadValue() {
  Wire.beginTransmission(DEVICE);
  Wire.write(byte(0xA8));
  Wire.endTransmission(false);
  Wire.requestFrom(DEVICE, 6, true);
  long x = Wire.read();
  x |= (Wire.read() << 0x08);
  long y = Wire.read();
  y |= (Wire.read() << 0x08);
  long z = Wire.read();
  z |= (Wire.read() << 0x08);
  
  Serial.print("x: ");
  Serial.print(x);
  Serial.print(" = ");
  //express x in ug (micro g)
  x *= 61;
  Serial.println(x);
  Serial.print("y: ");
  Serial.print(y);
  Serial.print(" = ");
  y *= 61;
  Serial.println(y);
  Serial.print("z: ");
  Serial.print(z);
  Serial.print(" = ");
  z *= 61;
  Serial.println(z);
}
