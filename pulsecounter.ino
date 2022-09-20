//=========================HEADER=============================================================

   
   LS7366 Breakout    -------------   Arduino
   -----------------                    -------
            MOSI   -------------------   SDO (D11)
            MISO   -------------------   SDI (D12)
            SCK    -------------------   SCK (D13)
            SS1    -------------------   SS1 (D10)
            SS2    -------------------   SS2 (D9)
            GND    -------------------   GND
            VDD    -------------------   VCC (5.0V)
      
#include <SPI.h>

// Slave Select pins for encoders 1 and 2
// Feel free to reallocate these pins to best suit your circuit
const int slaveSelectEnc1 = 10;
//const int slaveSelectEnc2 = 9;


// These hold the current encoder count.
signed long encoder1count = 0;
//signed long encoder2count = 0;

void initEncoders() {
  
  // Set slave selects as outputs
  pinMode(slaveSelectEnc1, OUTPUT);
//  pinMode(slaveSelectEnc2, OUTPUT);
//  pinMode(slaveSelectEnc1, INPUT);
  
  
  // Raise select pins
  // Communication begins when you drop the individual select signsl
  digitalWrite(slaveSelectEnc1,HIGH);
//  digitalWrite(slaveSelectEnc2,HIGH);
  
  SPI.begin();
  
  // Initialize encoder 1
  //    Clock division factor: 00 
  //    Negative index input
  //    free-running count mode
  //    x4 quatrature count mode (four counts per quadrature cycle)
  // NOTE: For more information on commands, see datasheet
  digitalWrite(slaveSelectEnc1,LOW);        // Begin SPI conversation
  SPI.transfer(0x88);                       // Write to MDR0
  SPI.transfer(0x03);                       // Configure to 4 byte mode
  digitalWrite(slaveSelectEnc1,HIGH);       // Terminate SPI conversation 

  digitalWrite(slaveSelectEnc1, LOW); 
  int value_1 = SPI.transfer(0x48);                       // read to MDR0
  int value_2 = SPI.transfer(0x03);  
  Serial.print(value_1);
  Serial.print(value_2);
  digitalWrite(slaveSelectEnc1, HIGH);
  // Initialize encoder 2
  //    Clock division factor: 0
  //    Negative index input
  //    free-running count mode
  //    x4 quatrature count mode (four counts per quadrature cycle)
  // NOTE: For more information on commands, see datasheet
//  digitalWrite(slaveSelectEnc2,LOW);        // Begin SPI conversation
//  SPI.transfer(0x88);                       // Write to MDR0
//  SPI.transfer(0x03);                       // Configure to 4 byte mode
//  digitalWrite(slaveSelectEnc2,HIGH);       // Terminate SPI conversation 
//
//  digitalWrite(slaveSelectEnc2, LOW); 
//  int val_1 = SPI.transfer(0x48);                       // read to MDR0
//  int val_2 = SPI.transfer(0x03);  
//  Serial.print(val_1);
//  Serial.print(val_2);
//  digitalWrite(slaveSelectEnc2, HIGH);

//  digitalRead(slaveSelectEnc2,LOW);        // Begin SPI conversation
//  SPI.transfer(0x48);                       // Read to MDR0
//  SPI.transfer(0x03);                       // Configure to 4 byte mode
//  digitalRead(slaveSelectEnc2,HIGH);       // Terminate SPI conversation 

}

long readEncoder(int encoder) {
  
  // Initialize temporary variables for SPI read
  unsigned int count_1, count_2, count_3, count_4;
  long count_value;  
  
  // Read encoder 1
  if (encoder == 1) {
    digitalWrite(slaveSelectEnc1,LOW);      // Begin SPI conversation
    SPI.transfer(0x60);                     // Request count
    count_1 = SPI.transfer(0x00);           // Read highest order byte
    count_2 = SPI.transfer(0x00);           
    count_3 = SPI.transfer(0x00);           
    count_4 = SPI.transfer(0x00);           // Read lowest order byte
    digitalWrite(slaveSelectEnc1,HIGH);     // Terminate SPI conversation 
  }
  
  // Read encoder 2
//  else if (encoder == 2) {
//    digitalWrite(slaveSelectEnc2,LOW);      // Begin SPI conversation
//    SPI.transfer(0x68);                      // Request count
//    count_1 = SPI.transfer(0x00);           // Read highest order byte
//    count_2 = SPI.transfer(0x00);           
//    count_3 = SPI.transfer(0x00);           
//    count_4 = SPI.transfer(0x00);           // Read lowest order byte
//    digitalWrite(slaveSelectEnc2,HIGH);     // Terminate SPI conversation 
//  }
  
  // Calculate encoder count
  count_value = (count_1 << 8) + count_2;
  count_value = (count_value << 8) + count_3;
  count_value = (count_value << 8) + count_4;
  
  return count_value;
}

void clearEncoderCount() {
    
  // Set encoder1's data register to 0
  digitalWrite(slaveSelectEnc1,LOW);      // Begin SPI conversation  
  // Write to DTR
  SPI.transfer(0x98);    
  // Load data
  SPI.transfer(0x00);  // Highest order byte
  SPI.transfer(0x00);           
  SPI.transfer(0x00);           
  SPI.transfer(0x00);  // lowest order byte
  digitalWrite(slaveSelectEnc1,HIGH);     // Terminate SPI conversation 
  
  delayMicroseconds(100);  // provides some breathing room between SPI conversations
  
  // Set encoder1's current data register to center
  digitalWrite(slaveSelectEnc1,LOW);      // Begin SPI conversation  
  SPI.transfer(0xE0);    
  digitalWrite(slaveSelectEnc1,HIGH);     // Terminate SPI conversation   
  
  // Set encoder2's data register to 0
//  digitalWrite(slaveSelectEnc2,LOW);      // Begin SPI conversation  
//  // Write to DTR
//  SPI.transfer(0x98);    
//  // Load data
//  SPI.transfer(0x00);  // Highest order byte
//  SPI.transfer(0x00);           
//  SPI.transfer(0x00);           
//  SPI.transfer(0x00);  // lowest order byte
//  digitalWrite(slaveSelectEnc2,HIGH);     // Terminate SPI conversation 
//  
//  delayMicroseconds(100);  // provides some breathing room between SPI conversations
  
  // Set encoder2's current data register to center
//  digitalWrite(slaveSelectEnc2,LOW);      // Begin SPI conversation  
//  SPI.transfer(0xE0);    
//  digitalWrite(slaveSelectEnc2,HIGH);     // Terminate SPI conversation 
}


void setup() {
 Serial.begin(9600);      // Serial com for data output
 
 initEncoders();       Serial.println("Encoders Initialized...");  
 clearEncoderCount();  Serial.println("Encoders Cleared...");
}

void loop() {
 delay(500);
 
 // Retrieve current encoder counters
 encoder1count = readEncoder(1); 
// encoder2count = readEncoder(2);
 
 Serial.print("Enc1: "); Serial.print(encoder1count);
 Serial.println(); 
// Serial.print(" Enc2: "); Serial.println(encoder2count); 
}
