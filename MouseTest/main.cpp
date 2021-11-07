#include "Pokitto.h"
#include "Smile.h"
#include "PS2/PS2Mouse.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;

/*
// test try using pins I have used in the past...
#define CLOCK 1<<23 // P1_23 (green)
#define DATA 1<<22 // P1_22 (white)
#define SET_CLOCK LPC_GPIO_PORT->SET[1] |= CLOCK
#define SET_DATA LPC_GPIO_PORT->SET[1] |= DATA
#define CLR_CLOCK LPC_GPIO_PORT->CLR[1] = CLOCK
#define CLR_DATA LPC_GPIO_PORT->CLR[1] = DATA
#define READ_CLOCK (((volatile uint32_t *) 0xA0002104)[0] >> 22) & 1
#define READ_DATA (((volatile uint32_t *) 0xA0002104)[1] >> 23) & 1
#define IN_CLOCK LPC_GPIO_PORT->DIR[1] &= ~CLOCK;
#define IN_DATA LPC_GPIO_PORT->DIR[1] &= ~DATA;
#define OUT_CLOCK LPC_GPIO_PORT->DIR[1] |= CLOCK;
#define OUT_DATA LPC_GPIO_PORT->DIR[1] |= DATA;

void clk_high(){
//  pinMode(pin, INPUT);
    IN_CLOCK;
//  digitalWrite(pin, HIGH);
    SET_CLOCK;
}
void clk_low(){
//  pinMode(pin, OUTPUT);
    OUT_CLOCK;
//  digitalWrite(pin, LOW);
    CLR_CLOCK;
}
void dat_high(){
    OUT_DATA;
    SET_DATA;
}
void dat_low(){
    IN_DATA;
    CLR_DATA;
}

void mouse_write(uint8_t myByte){
  char i;
  char parity = 1;

  //gohi(MDATA);
  dat_high();
  //gohi(MCLK);
  clk_high();
  //delayMicroseconds(300);
  wait_us(300);
  //golo(MCLK);
  clk_low();
  //delayMicroseconds(300);
  wait_us(300);
  //golo(MDATA);
  dat_low();
  //delayMicroseconds(10);
  wait_us(10);
  
  //gohi(MCLK);
  clk_high();
 
    //while (digitalRead(MCLK) == HIGH);
      while (READ_CLOCK == 1)
    ;
  
  for (i=0; i < 8; i++){
    if (myByte & 0x01){
      //gohi(MDATA);
      dat_high();
    } 
    else{
      //golo(MDATA);
      dat_low();
    }
    
    //while (digitalRead(MCLK) == LOW);
    while(READ_CLOCK == 0);
    //while (digitalRead(MCLK) == HIGH);
    while (READ_CLOCK == 1);
    parity = parity ^ (myByte & 0x01);
    myByte = myByte >> 1;
  }  
  // parity
  if (parity){
    //gohi(MDATA);
    dat_high();
  }else {
    //golo(MDATA);
    dat_low();
  }
  //while (digitalRead(MCLK) == LOW);
    while(READ_CLOCK == 0);
  //while (digitalRead(MCLK) == HIGH);
    while(READ_CLOCK == 1);
  //gohi(MDATA);
  dat_high();
  //delayMicroseconds(50);
  wait_us(50);
  //while (digitalRead(MCLK) == HIGH);
    while(READ_CLOCK == 1);
  //while ((digitalRead(MCLK) == LOW) || (digitalRead(MDATA) == LOW));
  while ((READ_CLOCK == 0) || (READ_DATA == 0));
  // put a hold on the incoming data.
  //golo(MCLK);
  clk_low();
  
}

uint8_t mouse_read(){
    return 0;
  uint8_t data = 0x00;
  int i;
  uint8_t bit = 0x01;

  //gohi(MCLK);
  clk_high();
  //gohi(MDATA);
  dat_high();
  //delayMicroseconds(50);
  wait_us(50);
  //while (digitalRead(MCLK) == HIGH);
  while (READ_CLOCK == 1);
  //delayMicroseconds(5);  // not sure why
  wait_us(5);
  //while (digitalRead(MCLK) == LOW); // eat start bit
  while (READ_CLOCK == 0); // eat start bit
    
  for (i=0; i < 8; i++){
    //while (digitalRead(MCLK) == HIGH);
    while (READ_CLOCK == 1);
    //if (digitalRead(MDATA) == HIGH){
    if (READ_DATA == 1){
      data = data | bit;
    }
    //while (digitalRead(MCLK) == LOW);
    while (READ_CLOCK == 0);
    bit = bit << 1;
  }
  //while (digitalRead(MCLK) == HIGH);
  while (READ_CLOCK == 1);
  //while (digitalRead(MCLK) == LOW);
  while (READ_CLOCK == 0);
  //while (digitalRead(MCLK) == HIGH);
  while (READ_CLOCK == 1);
  //while (digitalRead(MCLK) == LOW);
  while (READ_CLOCK == 0);
  //golo(MCLK);
  clk_low();
   
  return data;
}

void initMouse(){

  //gohi(MCLK);
  SET_CLOCK;
  //gohi(MDATA);
  SET_DATA;
  
  mouse_write(0xff);
  mouse_read();  // ack byte
  mouse_read();  // blank
  mouse_read();  // blank
  mouse_write(0xf3);  // Set rate command
  mouse_read();  // ack
  mouse_write(0xC8);  // Set rate command
  mouse_read();  // ack
  mouse_write(0xf3);  // Set rate command
  mouse_read();  // ack
  mouse_write(0x64);  // Set rate command
  mouse_read();  // ack
  mouse_write(0xf3);  // Set rate command
  mouse_read();  // ack
  mouse_write(0x50);  // Set rate command
  mouse_read();  // ack
  mouse_write(0xf2);  // Set rate command
  mouse_read();  // ack
  mouse_read();  // mouse id, if this value is 0x00 mouse is standard, if it is 0x03 mouse is Intellimouse
  mouse_write(0xe8);  // Set wheel resolution
  mouse_read();  // ack
  mouse_write(0x03);  // 8 counts per mm
  mouse_read();  // ack
  mouse_write(0xe6);  // scaling 1:1
  mouse_read();  // ack
  mouse_write(0xf3);  // Set sample rate
  mouse_read();  // ack
  mouse_write(0x28);  // Set sample rate
  mouse_read();  // ack
  mouse_write(0xf4);  // Enable device
  mouse_read();  // ack

  mouse_write(0xf0);  // remote mode
  mouse_read();  // ack
 
  //delayMicroseconds(100);
  wait_us(100);
}

*/

int main(){
    using PC=Pokitto::Core;
    using PS=Pokitto::Sound;
    using PD=Pokitto::Display;
    using PB=Pokitto::Buttons;

    PC::begin();
    PD::invisiblecolor = 0;
    PD::persistence = true;
    PD::adjustCharStep = 0;
    PD::adjustLineStep = 0;

    int MouseX = 0;
    int MouseY = 0;

    PS2Mouse ps2ms(P1_23, P1_22);

    //initMouse();

    while( PC::isRunning() ){
        
        if( !PC::update() ) 
            continue;


        PD::drawBitmap(MouseX, MouseY, Smile);
    }

    return(1);
}

