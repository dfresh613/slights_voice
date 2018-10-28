#include <FastLED.h>
#include <HashMap.h>

#define DATA_PIN 6  //this is the data pin connected to the LED strip.  If using WS2801 you also need a clock pin
// The number of lettters/characters we want to store in the hashmap
const byte HASH_SIZE = 28;
HashType<char*,int> hashRawArray[HASH_SIZE]; 
//handles the storage [search,retrieve,insert]
HashMap<char*,int> charToLed = HashMap<char*,int>( hashRawArray , HASH_SIZE ); 

#define NUM_LEDS 100 //change this for the number of LEDs in the strip
#define COLOR_ORDER RGB
String incoming_message = "";
String msgStr = "";
String tmp_message = "";

CRGB aqua = CRGB(0, 255, 255);
CRGB hot_pink = CRGB (255,105,180);
CRGB dark_orchid = CRGB(153, 50, 204);
CRGB spring_green = CRGB(0,255,127);
CRGB dark_purple = CRGB (76, 0, 153);
CRGB dark_green = CRGB (0,128,0);
CRGB orange = CRGB(255,165,0); 
CRGB red = CRGB(255,0,0);
CRGB blue = CRGB(0,0, 255);
CRGB royal_blue = CRGB (65,105,255);
CRGB yellow = CRGB(255,255,0);
int r,g,b;
CRGB color, color2;


CRGB leds[NUM_LEDS]; 
boolean running=false;

int y = 1;

void setup(){
    FastLED.addLeds<WS2811, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS); //setting up the FastLED
    Serial.begin(9600);
    /**  
    * Array location for each letter. We can use this to dynamically create messages based on strings.
    */
    charToLed[0]('a',0);
    charToLed[1]('b',4);
    charToLed[3]('c',8);
    charToLed[4]('d',12);
    charToLed[5]('e',16);
    charToLed[6]('f',20);
    charToLed[7]('g',24);
    charToLed[8]('h',27);
    charToLed[9]('i',30);
    charToLed[10]('j',33);
    charToLed[11]('k',36);
    charToLed[12]('l',39);
    charToLed[13]('m',42);
    charToLed[14]('n',48);
    charToLed[15]('o',50);
    charToLed[16]('p',54);
    charToLed[17]('q',57);
    charToLed[18]('r',61);
    charToLed[19]('s',63);
    charToLed[20]('t',68);
    charToLed[21]('u',71);
    charToLed[22]('v',74);
    charToLed[23]('w',77);
    charToLed[24]('x',80);
    charToLed[25]('y',84);
    charToLed[26]('z',87);
    
    randomSeed(analogRead(0));
}

/*
void loop(){
    if (running){
      return;
    }else{
      running=true;
    }
    FastLED.clear();
    FastLED.show();
    
    notifyPi();
    delay(random(2500,5000)); //delay to wait for serial response, randomize the delay to make it creepier
    String msgStr = "random";
    if(Serial.available()>0){
        msgStr = Serial.readString();
        interpretMessage(msgStr, false); // Change false to true if you want to enable random effects when no messages are queued
    }else{
      doRandom();
    }
    running=false;

}
*/


void loop(){

  if ( message_in_queue() ){
    tmp_message = incoming_message;
    incoming_message = "";
    interpretMessage(tmp_message);
  }
  
  
}

void interpretMessage(String msgStr){
  if (msgStr.equals("thinking")){
    glowRedUp();
  }else if(msgStr.equals("lightaround")){
    lightRun();
  } else{
    displayMessage(msgStr);
  }
  
}



boolean message_in_queue(){
  if(Serial.available()>0){
      incoming_message = Serial.readString();
   }

  // Where this if statement is decideds whether messages will be ovewritten before attempted to be displayed. 
  if ( !incoming_message.equals("") ){
    return true;
  }
  return false;
   
}

void notifyPi(){
  Serial.println("ready");
}

void doRandom(){
  int rNum=random(6);
  switch(rNum){
    case 0:
      glowRedUp();
      break;
    case 1:
      lightRun();
      break;
    case 2:
      christmas();
      break;
    case 3: 
      blinkSection(0, 99, random(1 ,5));
      break;
    case 4:
      endsToMiddle();
      break;
    case 5:
      randomLights();
      break;
    default:
      lightRun();
      break;

    /* Disabled message displays as I don't want them interfering with user messages
    case 3:
      displayMessage("die");
      break;
    case 4:
      displayMessage("helpme");
      break;
    case 5:
      displayMessage("itscoming");
      break;
      */
  }
}



//Start at the last light, and beginning light and meet in the middle
void endsToMiddle(){
  FastLED.clear();
  FastLED.show();
  int endIdx = NUM_LEDS-1;

  color = randomColor();
  for(int i =0; i< NUM_LEDS/2; i++){
    FastLED.clear();
    leds[i] = color;
    leds[endIdx -i] = color;
    FastLED.show();
    delay(100);
  }
  
  for(int i=0; i<NUM_LEDS/2; i++){
    color = randomColor();
    leds[(NUM_LEDS/2) -i] = color;
    leds[(NUM_LEDS/2)+ i] = color;
    FastLED.show();
    delay(20);
  }
  delay(1000);
  
}

//Turn on Random strips of 5
void randomLights(){
  int startingLight;
  int endingLight;
  int numLights;
  int sections;
      sections = random(2,5);
      //do for random sections of lights
      for(int s=0; s<sections; s++){
        startingLight=random(100)%94;
        endingLight=startingLight+5;
        blinkSection(startingLight,endingLight, 1);
      }
}

//flashes lights starting at startingPos position to end position numlights and flashes numFlashes times 
void blinkSection(int startingPos, int endPos, int numFlashes){
  FastLED.clear();
  int ledNum;
  //this first loop is for how many times the blinking effect will occur
  for(int x=0; x < numFlashes; x++){
    //this second loop makes the leds flash multuple times
    for(int quickflash = 1; quickflash<5; quickflash++){
      //this third loop loop turns on each Led with a different color
      for(ledNum=startingPos; ledNum<=endPos; ledNum++){
        leds[ledNum] = randomColor();
      }
       FastLED.show();
       delay(100);
       FastLED.clear();
    }
    FastLED.show();
    delay(1000);
  }
  FastLED.show();

}


void displayMessage(String message){
  int prev_led = -1;
  int ledNum;
  char letter;
  for(int i =0; i< message.length(); i+=1){
    FastLED.clear();
    FastLED.show();
    
    letter = message.charAt(i);
    if (isWhitespace(letter)){
        delay(2000);
        continue;
    }

    ledNum = charToLed.getValueOf(letter);
    leds[ledNum] = randomColor();
    leds[ledNum+1] = randomColor();
    prev_led=ledNum;

    if (message_in_queue()){
      break;
    }
    
    FastLED.show();
    delay(2000);
  }

  /*
  //flash all lights in the message
  FastLED.clear();
  for(int i=0; i< message.length(); i+=1){
    letter=message.charAt(i);
    ledNum=charToLed.getValueOf(letter);
    leds[ledNum] = randomColor();
    leds[ledNum+1]= randomColor();
  }
  FastLED.show();
  */
  FastLED.clear();
  FastLED.show();
  
}

void lightRun() {
  for (int i =0 ;i< NUM_LEDS; i+=1){
      leds[i] = randomColor();
      FastLED.show();
      delay(20);
      if (message_in_queue()){
        return;
      }
  }
  
  for(int i=NUM_LEDS-1; i>=0; i-=1){
    leds[i] = randomColor();
    FastLED.show();
    delay(20);
    if (message_in_queue()){
        return;
    }
  }
}

int randomColorCode(){
  int rColor = random(255);
  return rColor;
}

CRGB randomColor(){
  r = randomColorCode();
  g = randomColorCode();
  b = randomColorCode();
  
  return CRGB(r,g,b);
}

//Cool functions created by bxl4662
void christmas() {
  FastLED.clear();
  for (int i=0; i<100; i++){
    int modNum = i % 10;
    if (modNum== 0){
      leds[i] = aqua;
    }else if(modNum== 1){
      leds[i] = dark_orchid;
    }else if(modNum== 2){
      leds[i] = yellow;
    }else if(modNum== 3){
      leds[i] = spring_green;
    }else if(modNum== 4){
      leds[i] = orange;
    }else if(modNum== 5){
      leds[i] = royal_blue; 
    }else if(modNum== 6){
      leds[i] = dark_purple;
    }else if(modNum== 7){
      leds[i] = hot_pink;
    }else if(modNum== 8){
      leds[i] = dark_green;
    }else if(modNum== 9){
      leds[i] = red;
    }
  }
   
  FastLED.show();
  delay(7000);
  FastLED.clear(); 
}
  
void glowRedUp(){
     for( int i = 20; i < 150; i = i + y ) {

      r = i;  
      b = 0;  
      g = 0;   

      for(int x = 0; x < NUM_LEDS; x++){
          leds[x] = CRGB(r,g,b);
      }
          
      FastLED.show();
      if (message_in_queue()){
        break;
      }
      delay(100); 
    } 
}
void glowRedDown(){
     for(int i = 150; i > 20; i = i - y) {

      r = i;  
      b = 0;  
      g = 0;   

      for(int x = 0; x < NUM_LEDS; x++){
          leds[x] = CRGB(r,g,b);
      }
          
      FastLED.show();
      delay(100); 
    } 
}

