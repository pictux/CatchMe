#include <IRremote.h>

//all IR stuffs
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
IRsend irsend;
decode_results results;

//conductive thread grids to A4 & A5
const int gridPin = A4;
const int resetPin = A5;
int gridValue = 0;
int resetValue = 0;

//status pin (powered on=i've got it)
const int ledPin = 9;

//celo = (friendly italian term for) i've got it
boolean celo = false;

void setup() {
  Serial.begin(9600);  
  pinMode(ledPin, OUTPUT); 
}

void loop()
{ 
  //if i am free
  if (!celo) {
    //read the conductive thread grid on the back side
    gridValue = analogRead(gridPin);
    
    //DEBUG
    //Serial.print("grid = " );                       
    //Serial.print(gridValue);  
    //
    
    if (gridValue < 900) {
      Serial.println(" > TOUCHED" );                             
      celo = true;
      //send reset signal via IR to "reset" the other player
      for (int i = 0; i < 3; i++) {
        irsend.sendSony(0xa90, 12);
        delay(40);
      }
      //re-enable IR receiver 
      irrecv.enableIRIn();
      //turn on status led (to tell everyone i've got it)
      digitalWrite(ledPin, HIGH);
      delay(1000);
    } 
    else {  
      Serial.println();                      
    }
  }
  
  //if i've got it
  if (celo) {
    //read the conductive thread grid on the front side
    resetValue = analogRead(resetPin);
    
    //DEBUG
    //Serial.print("reset = " );                       
    //Serial.println(resetValue);  
    //
    
    //if i receive the "reset" ir signal from another player:
    if (irrecv.decode(&results)) {
      if (results.value == 2704) { //hex A90
        celo = false;
      } 
      irrecv.resume();
    }

    //..or, if i "reset" myself using the grid on the front side
    if (resetValue < 900) {
      celo = false;
    }  
    
    if (celo == false) {
      digitalWrite(ledPin, LOW);
    }
  } 
  delay(200);
}
