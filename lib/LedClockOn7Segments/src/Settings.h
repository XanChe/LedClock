#include <Arduino.h>
#include <EEPROM.h>
class Settings
{
private:
    
    int8_t clockColorHue;
    int8_t subZeroColorHue;
    int8_t plusZeroColorHue;
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
        this->clockColorHue = clockColorHui;
        this->subZeroColorHue = subZeroColorHue;
        this->plusZeroColorHue = plusZeroColorHue;
        this->brightness = brightnes;

    }   
    void save(){
        EEPROMWriteInt(0,27597532);
        EEPROMWriteInt(2,clockColorHue);
        EEPROMWriteInt(4,subZeroColorHue);
        EEPROMWriteInt(6,plusZeroColorHue);
        EEPROMWriteInt(8,brightness);

    }
    void load(){
        if(EEPROMReadInt(0) == 27597532){
            this->clockColorHue =  EEPROMReadInt(2);
            this->subZeroColorHue = EEPROMReadInt(4);
            this->plusZeroColorHue = EEPROMReadInt(6);
            this->brightness = EEPROMReadInt(8);
        }else{
            save();
        }
        
    }
    int8_t getClockColorHue(){
        return clockColorHue;
    }
    int8_t getSuZeroColorHue(){
        return subZeroColorHue;
    }
    int8_t getPlusZeroColorHue(){
        return plusZeroColorHue;
    }
    int8_t getBrightness(){
        return brightness;
    }
};

