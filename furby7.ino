// Furby JL Borges
// Roni Bandini April 2023
// MIT License

// Install https://github.com/PowerBroker2/FireTimer
// Install https://github.com/PowerBroker2/DFPlayerMini_Fast
// Use a 1k resistor for DfPlayer tx pin 11
// PIR clockwise more sensitive

#include <DFPlayerMini_Fast.h>

#if !defined(UBRR1H)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
#endif

DFPlayerMini_Fast myMP3;
int myRandom=0;

// Pins
const int enA = 9;
const int in1 = 8;
const int in2 = 7;
const int pinCamHome=5;
const int pinPir=6;

// settings
const int howManyAudios=250;
const int motorSpeed=254;
const int speakOffset=20;
const int closeAndOpenDelay=1000;
const int closeDelay=600;
const int openMouthDelay=300;
const int openMouthRandom=50;
const int loopDelay=250;
const int endTalkDelay=1500;

int pirState = LOW;  

void findHome(){
  
 Serial.println("Finding home");
 digitalWrite(in1, HIGH);
 digitalWrite(in2, LOW); 
 analogWrite(enA, motorSpeed);

 int f=0;
 
 while (f==0){

  int camHomeValue=digitalRead(pinCamHome);
  
  Serial.println("Cam home value: " + String(camHomeValue));

  if (camHomeValue==0){    
    f=1;    
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);        
  }  
  delay(15);
 }
 
 Serial.println("Found it!");
 delay(2000);
 
}

void speak(){
 
 myRandom=random(openMouthDelay,openMouthDelay+openMouthRandom);
 
 digitalWrite(in1, HIGH);
 digitalWrite(in2, LOW); 
 analogWrite(enA, motorSpeed); 
 delay(myRandom);
  
 digitalWrite(in1, LOW);
 digitalWrite(in2, HIGH);
 analogWrite(enA, motorSpeed); 
 delay(myRandom+speakOffset);
 
}

void stopMovement(){
  Serial.println("Stop"); 
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);    
}


void closeEyes(){
 Serial.println("Close eyes"); 
 digitalWrite(in1, HIGH);
 digitalWrite(in2, LOW); 
 analogWrite(enA, motorSpeed); 
 delay(closeDelay);  
 stopMovement();   
}

void closeAndOpenEyes(){
  Serial.println("Close and open eyes"); 
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW); 
  analogWrite(enA, motorSpeed); 
  delay(closeAndOpenDelay);  
  stopMovement();   
}

void setup()
{
  Serial.begin(115200);

  Serial.println("Furby Borges 2.0 - Roni Bandini");
  
  // Set motor pins to outputs
  pinMode(enA, OUTPUT);  
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // Set inputs
  pinMode(pinCamHome, INPUT_PULLUP);
  pinMode(pinPir, INPUT);
  
  randomSeed(analogRead(0));
  
  #if !defined(UBRR1H)
    mySerial.begin(9600);
    myMP3.begin(mySerial, true);
  #else
    Serial1.begin(9600);
    myMP3.begin(Serial1, true);
  #endif    
  
  Serial.println("Max volume");
  myMP3.volume(30);

  findHome();
  closeEyes(); 

  delay(3000);
  
}
    

void loop()
{

  pirState=digitalRead(pinPir);
  
  if (pirState==1){
    
    Serial.println("Movement detected");   

    findHome();

    myRandom=random(1,howManyAudios);
    
    Serial.println("Speech: "+String(myRandom));  

    myMP3.play(myRandom);
 
    while (myMP3.isPlaying()){
     Serial.println("Track is playing");
     speak();     
    }
   
   stopMovement();     
   findHome(); 
   closeEyes();   
   delay(endTalkDelay);     
    
  }
  
  delay(loopDelay);  
  
}
