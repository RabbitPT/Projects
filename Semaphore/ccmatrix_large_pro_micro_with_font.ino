#include <FastLED.h>
#include "pitches.h"
#include "font.h"


/*
int latchPin = 44;
int clockPin = 40;
int dataPin = 42;
int addr0 = 46;
int addr1 = 48;
int addr2 = 50;
int aux = 52;
int ledsPIN = 38;
*/


//-------------------------
#define MAX 15

int tabela[] = {1,2,1,1,3,3,2,2,1,2,3,1,1,2,1,1,3,3,2,2,1,2,3,1,3,2,2,3}; //delete, just for debug 


int lastTime = 0;
int currentTime = 0;
int refreshRate = 500;
int counter = 0;
int auxcounter = 0;
int desaparece = 0;
int mxoffset = 0;

bool getOut = false;
uint8_t gHue = 0;


// NOTES IN THE MELODY
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// NOTE DURATION
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};


// TOP HORIZONTAL LEDS
#define leds_pinhr      9     //D4
#define num_ledshr      23
CRGB ledshr[num_ledshr];

// LEFT VERTICAL LEDS
#define leds_pinvl    14    //can be changed
#define num_ledsvl    18
CRGB ledsvl[num_ledsvl];

// RIGHT VERTICAL LEDS 
#define leds_pinvr    15    //can be changed
#define num_ledsvr    18
CRGB ledsvr[num_ledsvr];

// SEMAPHORE LEDS
#define leds_pinsm    16    //can be changed
#define num_ledssm    27
CRGB ledssm[num_ledssm];
//--------------------------


// BUZZER
#define buzzer_pin    10     //D10


#define led_type      WS2811
#define color_order   GRB
#define brightness    25
#define fps           120


// MATRIX
#define clock_pin     8     //D1
#define data_pin      7     //D2
#define latch_pin     6     //D5
#define addr0_pin     5     //D6
#define addr1_pin     4     //D7
#define addr2_pin     3     //D8
#define aux_pin       2     //D2



byte fullmatrix[7][24]={
// -GGGGGGG    GGGGGGGG    GGGGGGGG    GGGGGGGG    GGGGGGGG    GGGGGGGG    GGGGGGGG    -RRRRRRR    RRRRRRRR    RRRRRRRR    RRRRRRRR    RRRRRRRR    RRRRRRRR    RRRRRRRR    GGGGGGGG    GGGGGGGG    GGGGGGGG    GGGGGGGG    GGGGGGGG    RRRRRRRR    RRRRRRRR    RRRRRRRR    RRRRRRRR    RRRRRRRR
 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
 0b00001000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000
};


void setup() 
{
  
  delay(350);
  
  // set pins to output
  pinMode(latch_pin, OUTPUT);
  pinMode(clock_pin, OUTPUT);
  pinMode(data_pin, OUTPUT);
  pinMode(addr0_pin, OUTPUT);
  pinMode(addr1_pin, OUTPUT);
  pinMode(addr2_pin, OUTPUT);
  pinMode(aux_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);

  // set the aux_pin high
  digitalWrite(aux_pin, HIGH);

  //-------------------------------------------------------------------------------------------
  // create fastled object for horizontal leds
  FastLED.addLeds<led_type, leds_pinhr, color_order>(ledshr, num_ledshr).setCorrection(TypicalLEDStrip);
  
  // create fastled for vertical leds left
  FastLED.addLeds<led_type, leds_pinvl, color_order>(ledsvl, num_ledsvl).setCorrection(TypicalLEDStrip);

  // create fastled for vertical leds right
  FastLED.addLeds<led_type, leds_pinvr, color_order>(ledsvr, num_ledsvr).setCorrection(TypicalLEDStrip);

  // create fastled for semaphore leds
  FastLED.addLeds<led_type, leds_pinsm, color_order>(ledssm, num_ledssm).setCorrection(TypicalLEDStrip);
  //-------------------------------------------------------------------------------------------
  
  // set master brightness control
  FastLED.setBrightness(brightness);

  Serial.begin(115200);
}

void draw_matrix(byte matrix[7][24])
{
  int x,y,t;
  
  for (y = 0; y < 8; y++)
  {
    
    digitalWrite(addr0_pin, bitRead(8-y, 0));
    digitalWrite(addr1_pin, bitRead(8-y, 1));
    digitalWrite(addr2_pin, bitRead(8-y, 2));

    digitalWrite(latch_pin, LOW);
    
    for (x = 0; x < 24; x++) 
    {
      shiftOut(data_pin, clock_pin, MSBFIRST, matrix[y][x]);  
     
    } 
    
    digitalWrite(latch_pin, HIGH);
  }
}


void set_pixel(byte matrix[7][24], int x, int y, int c)
{
  int m_row, m_col, m_bit, xbit;
  int xbit_offset = 0;
  int red_offset = 0;

  xbit=x+1;

  if (xbit<=55)
  {
    xbit_offset=0;
    red_offset=7;
  }
  else
  {
    xbit_offset = 7*8;
    red_offset = 5;
  }
   
  m_col=((xbit+xbit_offset)/8);
  m_bit=((xbit+xbit_offset)%8);
  m_row=y;
 
  switch (c)
  {  
    case 0:
      // black
      bitWrite(matrix[m_row][m_col], 7-m_bit, 0);
      bitWrite(matrix[m_row][m_col+red_offset], 7-m_bit, 0);
    break;
    case 1:
      // green
      bitWrite(matrix[m_row][m_col], 7-m_bit, 1);
      bitWrite(matrix[m_row][m_col+red_offset], 7-m_bit, 0);
    break;
    case 2:
      // yellow
      bitWrite(matrix[m_row][m_col], 7-m_bit, 1);
      bitWrite(matrix[m_row][m_col+red_offset], 7-m_bit, 1);
    break;
    case 3:
      // red
      bitWrite(matrix[m_row][m_col], 7-m_bit, 0);
      bitWrite(matrix[m_row][m_col+red_offset], 7-m_bit, 1);
    break;     
  }
}

// PLAY MELODY---------------------------------------------------------------------------------------------
void play_melody()
{
   for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, melody[thisNote], noteDuration);

    // SET TIME BETWEN THE NOTES TO DISTINGUISH THEM
    // 1.30 IS 30% OF THE NOTE DURATION AS DELAY
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // STOP PLAYING THE TONE
    noTone(8);
    }
}//---------------------------------------------------------------------------------------------------------



// FILL ALL TOP HORIZONTAL LEDS---------------------------------------------------------------------------
void top_horizontal_leds(byte r, byte g, byte b) 
{
    for (int i=0;i<num_ledshr;i++){
      ledshr[i].setRGB(r, g, b);
    }
}//---------------------------------------------------------------------------------------------------------




// FILL ALL LEFT VERICAL LEDS-----------------------------------------------------------------------------
void left_vertical_leds(byte r, byte g, byte b)
{
    for (int i=0;i<num_ledsvl;i++){
      ledsvl[i].setRGB(r, g, b);
    }
}//---------------------------------------------------------------------------------------------------------





// FILL ALL RIGHT VERTICAL LEDS---------------------------------------------------------------------------
void right_vertical_leds(byte r, byte g, byte b)
{
    for (int i=0;i<num_ledsvr;i++){
      ledsvr[i].setRGB(r, g, b);
    }
}//---------------------------------------------------------------------------------------------------------





// FILL ALL SEMAPHORE LEDS--------------------------------------------------------------------------------
void semaphore_leds(byte r, byte g, byte b)
{
    for (int i=0;i<num_ledssm;i++){
      ledssm[i].setRGB(r, g, b);
    }
}//---------------------------------------------------------------------------------------------------------



// FILL SPECIFIC TOP HORIZONTAL LED-----------------------------------------------------------------------
void specific_top_horizontal_led(byte pos, byte r, byte g, byte b)
{
    ledshr[pos].setRGB(r, g, b);
}//---------------------------------------------------------------------------------------------------------





// FILL SPECIFIC LEFT VERTICAL LED------------------------------------------------------------------------
void specific_left_vertical_led(byte pos, byte r, byte g, byte b) 
{
    ledsvl[pos].setRGB(r, g, b);
}//---------------------------------------------------------------------------------------------------------





// FILL SPECIFIC RIGHT VERTICAL LED-----------------------------------------------------------------------
void specific_right_vertical_led(byte pos, byte r, byte g, byte b) 
{
    ledsvr[pos].setRGB(r, g, b);
}//---------------------------------------------------------------------------------------------------------





// FILL SPECIFIC SEMAPHORE LED----------------------------------------------------------------------------
void specific_semaphore_led(byte pos, byte r, byte g, byte b) 
{
    ledssm[pos].setRGB(r, g, b);
}//---------------------------------------------------------------------------------------------------------





//----------------------------1 color 1 string (with animation)------------------------------------------
void Text_Message(byte matrix[7][24], String txt,int color)
{
  int cn, cr, cb;
  int fp;
  int mx = MAX*6-mxoffset*6;

  lastTime = millis();
  getOut = false;


  //define the color
  int cor=4;

  if(color == 0){
      cor = 4;
    }
  else if(color == 1){
      cor = 1;
    }
  else if(color == 2){
      cor = 2;
    }
  else if(color == 3){
      cor = 3;
    }



  //will help me track how many times i run this function
  auxcounter++;


  //if the first letter is already in the begining of the matrix then start to eliminate the first characters
  if(mx <= 0){
    desaparece++;
    }
  else{
    mxoffset++;
    }

  //if all the characters in the string already displayed then i start to reduce the amount of memory i allocate to the pointer  
  if(auxcounter < 17){
    counter++;
    }

  //if nothing more is displayed in the matrix then repeat everything
  if(auxcounter == (txt.length() + 16)){
    counter = 0;
    mxoffset = 0;
    desaparece = 0;
    auxcounter = 0;
    }

  char *aux = malloc(sizeof(char) * (counter));
    
  for(int i=0;i<counter;i++){ 
    aux[i] = txt[i+desaparece]; 
    }

  //draw the String
  for (cn=0; cn < counter; cn++)
  {
    Serial.println(aux[cn]);
    Serial.println(int(aux[cn]));
    Serial.println("---");

    //percorre caracter
    for (cr=0; cr<5; cr++)
    {
      fp=(aux[cn] * 5) + cr;

            //percorre a matriz de 5 colunas para desenhar a letra
      for (cb=0; cb<7; cb++)
      {
        Serial.print(bitRead(font_5x7[fp],6-cb));
        set_pixel(matrix, mx, 6-cb, bitRead(font_5x7[fp],6-cb)*cor);
      }
      Serial.print(" ");
      
      Serial.print(fp);
      Serial.print(":");
      Serial.println(font_5x7[fp]);
      mx++;
      
    }
    Serial.println();
    mx++;
  }

  //will get stucked in the cicle for 250 milli seconds
  while(!getOut){
    currentTime = millis();
    if(currentTime - lastTime >= refreshRate){
      getOut = true;
      }
      
    draw_matrix(fullmatrix);
  }

  //free alocated space
  free(aux);
  
}
//-----------------------------------------------------------------------------------------------





//-------------------------------1 color per character in the string (with animation)-------------------------------------------------------

                                                              //   _>size of the array of colors
                                                              //  |
void Text_Message(byte matrix[7][24], String txt, int color[], size_t n)
{
  int cn, cr, cb;
  int fp;
  int mx = MAX*6-mxoffset*6;

  int *cor=malloc(sizeof(int) * txt.length());  

  //start count the time
  lastTime = millis();
  getOut = false;

  //if the color array is bigger than the length of the string
  if(n >= txt.length()){
 
    for(int i=0;i<txt.length();i++){ 
      if(color[i] == 0){
          cor[i] = 4;
        }
      else if(color[i] == 1){
          cor[i] = 1;
        }
      else if(color[i] == 2){
          cor[i] = 2;
        }
      else if(color[i] == 3){
          cor[i] = 3;
        }
      }
    }
  else{ 
    for(int i=0;i<txt.length();i++){ 
      if(i < n){
        if(color[i] == 0){
            cor[i] = 4;
          }
        else if(color[i] == 1){
            cor[i] = 1;
          }
        else if(color[i] == 2){
            cor[i] = 2;
          }
        else if(color[i] == 3){
            cor[i] = 3;
          }      
        }
        else{
            cor[i] = 1; //will paint the rest of the characters with a default color of green
          }

      }
    }



  //will help me track how many times i run this function
  auxcounter++;


  //if the first letter is already in the begining of the matrix then start to eliminate the first characters
  if(mx <= 0){
    desaparece++;
    }
  else{
    mxoffset++;
    }

  //if all the characters in the string already displayed then i start to reduce the amount of memory i allocate to the pointer  
  if(auxcounter < 17){
    counter++;
    }

  //if nothing more is displayed in the matrix then repeat everything
  if(auxcounter == (txt.length() + 16)){
    counter = 0;
    mxoffset = 0;
    desaparece = 0;
    auxcounter = 0;
    }

  char *aux = malloc(sizeof(char) * counter);
  int *auxcor = malloc(sizeof(char) * counter);
  for(int i=0;i<counter;i++){ 
    aux[i] = txt[i+desaparece]; 
    auxcor[i] = cor[i+desaparece];
    }


    
  for (cn=0; cn < counter; cn++)
  {
    Serial.println(aux[cn]);
    Serial.println(int(aux[cn]));
    Serial.println("---");

    //percorre caracter
    for (cr=0; cr<5; cr++)
    {
      fp=(aux[cn] * 5) + cr;

      //percorre a matriz de 5 colunas para desenhar a letra
      for (cb=0; cb<7; cb++)
      {
        Serial.print(bitRead(font_5x7[fp],6-cb));
        set_pixel(matrix, mx, 6-cb, bitRead(font_5x7[fp],6-cb)*auxcor[cn]);
      }
      Serial.print(" ");
      
      Serial.print(fp);
      Serial.print(":");
      Serial.println(font_5x7[fp]);
      mx++;
      
    }
    Serial.println();
    mx++;
  }

  //will get stucked in the cicle for 250 milli seconds
  while(!getOut){
    currentTime = millis();
    if(currentTime - lastTime >= refreshRate){
      getOut = true;
      }
      
    draw_matrix(fullmatrix);
  }

  //free alocated space
  free(aux);
  free(cor);
  free(auxcor);
}
//-----------------------------------------------------------------------------------------------

void Fill_Matrix(byte matrix[7][24],int color){



  //define the color
  int cor=4;

  if(color == 0){
      cor = 4;
    }
  else if(color == 1){
      cor = 1;
    }
  else if(color == 2){
      cor = 2;
    }
  else if(color == 3){
      cor = 3;
    }



  for(int i=0; i<7;i++){
    for(int l=0;l<94;l++){
      set_pixel(matrix,l,i,cor);
      }
    }
   while(!getOut){ 
    draw_matrix(fullmatrix);
  }  
}



void loop() 
{
  
  //Text_Message( fullmatrix,"--Dognaedis by JR & LE--",tabela,28);
  //Fill_Matrix(fullmatrix,2);
  fill_rainbow( ledshr, num_ledshr, gHue, 7);
  Text_Message( fullmatrix,"--Dognaedis by JR & LE--",1);
  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/fps);
}


