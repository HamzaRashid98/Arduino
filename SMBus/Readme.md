This code has been designed to read the status of a battery pack that uses the SMBus format and has two i2c devices with the same address. The SDA and SCL are connected to both devices but the B1_PULLUP and B2_PULLUP are connected to clock pin in series with diodes. Then these pins are used as enable signal for the deivces. In this way two devices with same address can be controlled using diodes.

Components Required: 
1. Arduino UNO x 1
2. 1N4007 Diodes x 2
3. 10kohm Resistors x 2
4. Slave devices with same address x 2

Connections:
![k2Iyt](https://github.com/user-attachments/assets/77a8b5ba-c2e7-4607-b6af-9f43e09954ae)
