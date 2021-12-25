#ifndef Settings_h
#define Settings_h
#include <Arduino.h>
#include <EEPROM.h>
class Settings
{
private:
    
    int8_t clockColor;
    int8_t subZeroColor;
    int8_t plusZeroColor;
    int8_t brightness;
    
    void EEPROMWriteInt(int p_address, int p_value){
        byte lowByte = ((p_value >> 0) & 0xFF);
        byte highByte = ((p_value >> 8) & 0xFF);

        EEPROM.write(p_address, lowByte);
        EEPROM.write(p_address + 1, highByte);
        }

//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
    byte EEPROMReadInt(int p_address){
        byte lowByte = EEPROM.read(p_address);
        byte highByte = EEPROM.read(p_address + 1);

        return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
    }
public:
    Settings(int8_t clockColorHui, int8_t subZeroColorHue, int8_t plusZeroColorHue, int8_t brightnes){
        this->clockColor = clockColorHui;
        this->subZeroColor = subZeroColorHue;
        this->plusZeroColor = plusZeroColorHue;
        this->brightness = brightnes;

    }   
    void save(){
        EEPROMWriteInt(0,27575);
        EEPROMWriteInt(2,clockColor);
        EEPROMWriteInt(4,subZeroColor);
        EEPROMWriteInt(6,plusZeroColor);
        EEPROMWriteInt(8,brightness);

    }
    void load(){
        if(EEPROMReadInt(0) == 27575){
            this->clockColor =  EEPROMReadInt(2);
            this->subZeroColor = EEPROMReadInt(4);
            this->plusZeroColor = EEPROMReadInt(6);
            this->brightness = EEPROMReadInt(8);
        }else{
            save();
        }
        
    }
    int8_t getClockColor(){
        return clockColor;
    }
    int8_t getSuZeroColor(){
        return subZeroColor;
    }
    int8_t getPlusZeroColor(){
        return plusZeroColor;
    }
    int8_t getBrightness(){
        return brightness;
    }
};

#endif