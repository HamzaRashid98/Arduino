#define sensor_pin 7     // what pin we're connected to

//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value
unsigned int c=0,I_RH,D_RH,I_Temp,D_Temp,CheckSum,current_millis;
int data[40];

void setup()
{
  Serial.begin(9600);
  Serial.println("Getting Started With DHT22");
}

void loop()
{
    current_millis = millis();
    Request();    /* send start pulse */
    Response();   /* receive response */
//    Serial.println("Getting Data");
//    Serial.println("Getting Data I_RH");
    I_RH=Receive_data();  /* store first eight bit in I_RH */
//    Serial.println("Getting Data D_RH");
//    D_RH=Receive_data();  /* store next eight bit in D_RH */
//    Serial.println("Getting Data I_Temp");
//    I_Temp=Receive_data();  /* store next eight bit in I_Temp */
//    Serial.println("Getting Data D_Temp");
//    D_Temp=Receive_data();  /* store next eight bit in D_Temp */
//    Serial.println("Getting Data Check Sum");
//    CheckSum=Receive_data();/* store next eight bit in CheckSum */
    
//    if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum)
//    {
//      Serial.println("Error");
//    }
//    
//    else
//    { 
//      Serial.print("I_Temp: ");
//      Serial.println(I_Temp);
//            
//      Serial.print("D_Temp: ");
//      Serial.println(D_Temp);
//      
//      Serial.print("CheckSum: ");
//      Serial.println(CheckSum);      
//    }

    delay(2500);
}

void Request()
{
//  Serial.println("Requesting DHT22");
  pinMode(sensor_pin, OUTPUT);
  digitalWrite(sensor_pin, LOW);
  delay(5);
  digitalWrite(sensor_pin, HIGH);
  delayMicroseconds(38);
  pinMode(sensor_pin, INPUT);
}

void Response()        /* receive response from DHT11 */
{  
//  Serial.println("Checking Response");
  while(digitalRead(sensor_pin) == 0);
//  Serial.println("Checking Response 1");
  while(digitalRead(sensor_pin) == 1);
}

unsigned int Receive_data()      /* receive data */
{ 
  Serial.println(millis());
  for (int i=0; i<10; i++)
  {    
    while(digitalRead(sensor_pin) != HIGH)  /* check received bit 0 or 1 */
    {
      //wait for pin to get HIGH
    }
    delayMicroseconds(50);
    if(digitalRead(sensor_pin) == HIGH)/* if high pulse is greater than 30ms */
    {
      data[i] = 1;  /* then its logic HIGH */ 
    }
    else      /* otherwise its logic LOW */
    {
      data[i] = 0;
    }
    Serial.print(data[i]);
//    Serial.print(digitalRead(sensor_pin));
    while(digitalRead(sensor_pin) == 1)
    {
      //wait for pin to get HIGH
    }
  }
  Serial.println("");
//  return c;
}
