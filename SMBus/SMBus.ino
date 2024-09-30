/*======================================================*
                    Code Description
  ======================================================*/
  /*
   * This code has been designed to read the status of a
   * battery pack that uses the SMBus format and has two
   * i2c devices with the same address. The SDA and SCL
   * are connected to both devices but the B1_PULLUP and
   * B2_PULLUP are connected to clock pin in series with
   * diodes. Then these pins are used as enable signal
   * for the deivces. In this way two devices with same
   * address can be controlled using diodes.
   */
/*======================================================*
                        Definitaions
  ======================================================*/
/****** Pin Assignment ******/
//   Atmega PD1
#define SDA_PORT PORTC
#define SDA_PIN 4

//  Atmega PD0
#define SCL_PORT PORTC
#define SCL_PIN 5

#define I2C_SLOWMODE 1

#define B1_PULLUP 12
#define B2_PULLUP 8

/****** Registers ******/
// Standard and common non-standard Smart Battery commands
#define BATTERY_MODE             0x03
#define TEMPERATURE              0x08
#define VOLTAGE                  0x09
#define CURRENT                  0x0A
#define RELATIVE_SOC             0x0D
#define ABSOLUTE_SOC             0x0E
#define REMAINING_CAPACITY       0x0F
#define FULL_CHARGE_CAPACITY     0x10
#define TIME_TO_FULL             0x13
#define CHARGING_CURRENT         0x14
#define CHARGING_VOLTAGE         0x15
#define BATTERY_STATUS           0x16
#define CYCLE_COUNT              0x17
#define DESIGN_CAPACITY          0x18
#define DESIGN_VOLTAGE           0x19
#define SPEC_INFO                0x1A
#define MFG_DATE                 0x1B
#define SERIAL_NUM               0x1C
#define MFG_NAME                 0x20   // String
#define DEV_NAME                 0x21   // String
#define CELL_CHEM                0x22   // String
#define MFG_DATA                 0x23   // String
#define CELL4_VOLTAGE            0x3C   // Indidual cell voltages don't work on Lenovo and Dell Packs
#define CELL3_VOLTAGE            0x3D
#define CELL2_VOLTAGE            0x3E
#define CELL1_VOLTAGE            0x3F
#define STATE_OF_HEALTH          0x4F
/*======================================================*
                      Header Files
  ======================================================*/
#include <SoftI2CMaster.h>
#include <SoftwareSerial.h>

/*======================================================*
                    Global Variables
  ======================================================*/
// standard I2C address for Smart Battery packs
byte deviceAddress = 11;

#define bufferLen 32
uint8_t i2cBuffer[bufferLen];

int Count = 1;
/*======================================================*
                      Setup Block
  ======================================================*/
void setup()
{
  pinMode(B1_PULLUP, OUTPUT);
  pinMode(B2_PULLUP, OUTPUT);

  digitalWrite(B1_PULLUP, HIGH);
  digitalWrite(B1_PULLUP, LOW);

  Serial.begin(115200);  // start serial for output
  Serial.println(i2c_init());


  while (!Serial) {
    ; // wait for Serial port to connect.
  }

  Serial.println("Console Initialized");

  i2c_init();
  Serial.println("I2C Inialized");

  //If you don't know the address of the slave device then run the following function
  //  scan();     

    while (Serial) {
      ; // wait for Serial port to connect.
    }
}
/*======================================================*
               Read The Data From The Device
  ======================================================*/
int fetchWord(byte func)
{
  i2c_start(deviceAddress << 1 | I2C_WRITE);
  i2c_write(func);
  i2c_rep_start(deviceAddress << 1 | I2C_READ);
  byte b1 = i2c_read(false);
  byte b2 = i2c_read(true);

  //  Serial.println(b1);
  //  Serial.println(b2);

  i2c_stop();
  return (int)b1 | ((( int)b2) << 8);
}

/*======================================================*
               Read The Data From The Device
  ======================================================*/
uint8_t i2c_smbus_read_block ( uint8_t command, uint8_t* blockBuffer, uint8_t blockBufferLen )
{
  uint8_t x, num_bytes;
  i2c_start((deviceAddress << 1) + I2C_WRITE);
  i2c_write(command);
  i2c_rep_start((deviceAddress << 1) + I2C_READ);
  num_bytes = i2c_read(false); // num of bytes; 1 byte will be index 0
  num_bytes = constrain(num_bytes, 0, blockBufferLen - 2); // room for null at the end
  for (x = 0; x < num_bytes - 1; x++) { // -1 because x=num_bytes-1 if x<y; last byte needs to be "nack"'d, x<y-1
    blockBuffer[x] = i2c_read(false);
  }
  blockBuffer[x++] = i2c_read(true); // this will nack the last byte and store it in x's num_bytes-1 address.
  blockBuffer[x] = 0; // and null it at last_byte+1
  i2c_stop();
  return num_bytes;
}

/*======================================================*
       Scan to identify the address of unknown device
  ======================================================*/
void scan()
{
  byte i = 0;
  for ( i = 0; i < 127; i++  )
  {
    Serial.print("Address: 0x");
    Serial.print(i, HEX);
    bool ack = i2c_start(i << 1 | I2C_WRITE);
    //    Serial.print("\nACK: ");
    //    Serial.println(ack);
    if ( ack ) {
      Serial.println(": OK");
      Serial.flush();
    }
    else {
      Serial.println(": -");
      Serial.flush();
    }
    i2c_stop();
  }
}

/*======================================================*
                   Forever Loop Block
  ======================================================*/
void loop()
{
  if (Count == 1)
  {
    digitalWrite(B1_PULLUP, HIGH);
    digitalWrite(B2_PULLUP, LOW);
  }
  else if (Count == 2)
  {
    digitalWrite(B1_PULLUP, LOW);
    digitalWrite(B2_PULLUP, HIGH);
  }

  uint8_t length_read = 0;

  Serial.print("Manufacturer Name: ");
  length_read = i2c_smbus_read_block(MFG_NAME, i2cBuffer, bufferLen);
  Serial.write(i2cBuffer, length_read);
  Serial.println(length_read);
  Serial.println("");

  Serial.print("Device Name: ");
  length_read = i2c_smbus_read_block(DEV_NAME, i2cBuffer, bufferLen);
  Serial.write(i2cBuffer, length_read);
  Serial.println(length_read);
  Serial.println("");

  Serial.print("Chemistry ");
  length_read = i2c_smbus_read_block(CELL_CHEM, i2cBuffer, bufferLen);
  Serial.write(i2cBuffer, length_read);
  Serial.println(length_read);
  Serial.println("");

  String formatted_date = "Manufacture Date (Y-M-D): ";
  int mdate = fetchWord(MFG_DATE);
  int mday = B00011111 & mdate;
  int mmonth = mdate >> 5 & B00001111;
  int myear = 1980 + (mdate >> 9 & B01111111);
  formatted_date += myear;
  formatted_date += "-";
  formatted_date += mmonth;
  formatted_date += "-";
  formatted_date += mday;
  Serial.println(formatted_date);

  Serial.print("Serial Number: ");
  Serial.println(fetchWord(SERIAL_NUM));

  Serial.print("Voltage: ");
  Serial.println((float)fetchWord(VOLTAGE) / 1000);

  // These aren't part of the standard, but work with some packs.
  // They don't work with the Lenovo and Dell packs we've tested
  Serial.print("Cell 1 Voltage: ");
  Serial.println((float)fetchWord(CELL1_VOLTAGE) / 1000);

  Serial.print("Cell 2 Voltage: ");
  Serial.println((float)fetchWord(CELL2_VOLTAGE) / 1000);

  Serial.print("Cell 3 Voltage: ");
  Serial.println((float)fetchWord(CELL3_VOLTAGE) / 1000);

  Serial.print("Cell 4 Voltage: ");
  Serial.println((float)fetchWord(CELL4_VOLTAGE) / 1000);

  Serial.print("State of Health: ");
  Serial.println(fetchWord(STATE_OF_HEALTH));


  Serial.print("Charging Current: ");
  Serial.println((float)fetchWord(CHARGING_CURRENT) / 1000);

  Serial.print("Charging Voltage: ");
  Serial.println((float)fetchWord(CHARGING_VOLTAGE) / 1000);

  Serial.print("Temp: ");
  unsigned int tempk = fetchWord(TEMPERATURE);
  Serial.println((float) (tempk / 10.0) - 273.15);
  //  Serial.println(tempk);

  Serial.print("CYCLE_COUNT: ");
  Serial.println(fetchWord(CYCLE_COUNT));

  Serial.print("SPEC_INFO: ");
  Serial.println(fetchWord(SPEC_INFO));

  Serial.print("REMAINING_CAPACITY: ");
  Serial.println(fetchWord(REMAINING_CAPACITY));

  Serial.print("RELATIVE_SOC: ");
  Serial.println(fetchWord(RELATIVE_SOC));

  Serial.print("ABSOLUTE_SOC: ");
  Serial.println(fetchWord(ABSOLUTE_SOC));


  //fetchWord(SERIAL_NUM);

  Count++;

  if (Count > 2)
  {
    Count = 1;
  }
  delay(2000);
}
