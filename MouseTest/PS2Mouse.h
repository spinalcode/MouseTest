#ifndef _PS2MOUSE_H_
#define _PS2MOUSE_H_

#define sensorConfig 0x00
#define sensorStatus 0x01
#define Delta_Y 0x02
#define Delta_X 0x03
 
DigitalInOut pinC(P1_19);
DigitalInOut pinD(P0_11);

class MOUSE {
 
    protected:
 
    public:
 
//=====================================
//====== Set of useful functions ======
//=====================================
 
    void init(){
        pinC.output();
        pinD.input();
    }
 
    void reSync(){
        pinC = 1; // ReSync (startup) mouse
        wait_us(5);
        pinC = 0;
        wait_us(1);
        pinC = 1; 
        wait_us(1000);  // wait for OptiMouse serial transaction timer to time out:
    }
    
    uint8_t readRegister(uint8_t address){ // Bitbang SPI read operation
        int i = 7;
        uint8_t r = 0;
        pinD.output();   // Write the address of the register we want to read:
        for (; i>=0; i--){
            pinC = 0;
            pinD = (address & (1 << i));
            pinC = 1;
        }
        pinD.input();    // Switch data line from OUTPUT to INPUT
        wait_us(100);   // Wait according to datasheet
        for (i=7; i>=0; i--){     // Fetch the data!                          
            pinC = 0;
            pinC = 1;
            r |= ((pinD.read()) << i);
        }
        wait_us(100);
        return r;
    }
 
    void writeRegister(uint8_t address, uint8_t data){
        int i = 7;  
        address |= 0x80;// Set MSB high, to indicate write operation:
        pinD.output();  // Write the address:
        for (; i>=0; i--){
            pinC = 0;
            pinD = (address & (1 << i));
            pinC = 1;
        } 
        for (i=7; i>=0; i--){ // Write the data:
            pinC = 0;
            pinD = (data & (1 << i));
            pinC = 1;
        }
    }
 
    void forceAwake(char value){
        if (value>0) writeRegister(sensorConfig,0x01); 
        else writeRegister(sensorConfig,0x00);
    }
 
    signed char getDx(void){
        return (signed char) readRegister(Delta_X);
    }
 
    signed char getDy(void){
        return (signed char) readRegister(Delta_Y);
    }
 
    signed char status(void){
        return (signed char) readRegister(sensorStatus);
    }
};

#endif