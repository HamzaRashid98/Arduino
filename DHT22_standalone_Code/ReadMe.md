*** DHT22 Sensor with Arduino UNO - Temperature and Humidity Monitor ***
Project Description:
This project demonstrates how to use a DHT22 sensor with an Arduino Uno to measure temperature and humidity. The sensor is connected to the Arduino, and the measured values are displayed on the Serial Monitor. The DHT22 is known for its accuracy and reliability in monitoring environmental conditions, making it ideal for weather stations and other sensor projects.

Components Needed:
> Arduino Uno
> DHT22 Sensor
> 10kΩ Resistor
> Jumper Wires
> Breadboard

Connect the DHT22 Sensor:
> Pin 1 (VCC): Connect to the 5V pin on the Arduino.
> Pin 2 (Data): Connect to digital pin 7 on the Arduino.
> Pin 4 (GND): Connect to the GND pin on the Arduino.
> 10kΩ Resistor: Connect the 10kΩ resistor between Pin 1 (VCC) and Pin 2 (Data) of the DHT22 sensor.
> Power the Arduino: Connect the Arduino to your computer via USB or use an external power source.

![image](https://github.com/user-attachments/assets/06461d88-d71c-43b3-ad95-2d3eece68a29)

Upload Code: Upload the Arduino sketch to the Uno that reads temperature and humidity from the DHT22 and prints it to the Serial Monitor.

Open Serial Monitor: After uploading the code, open the Serial Monitor from the Arduino IDE to view the data readings.

About the DHT22 Sensor
The DHT22 is a digital sensor capable of measuring both temperature and humidity. It provides accurate readings over a wide range, making it suitable for environmental monitoring applications.

Temperature Measurement: It can measure temperatures from -40°C to +80°C with an accuracy of ±0.5°C.
Humidity Measurement: It measures relative humidity between 0% and 100%, with an accuracy of ±2-5%.
Digital Output: The DHT22 outputs readings in digital form, making it easy to interface with microcontrollers like the Arduino.
Long-Term Stability: The sensor is designed to maintain consistent performance over extended periods, ensuring reliable data collection in long-term projects.
