#ifndef Typedefs_h
#define Typedefs_h
#include <LibEnums.h>
#include <EEPROM.h>
struct DisplaySettings{
    int8_t brightness = 60;
    int8_t brghtCorrector = 50;
    int8_t subZeroColor = 0;       // цвет отрисовки температуры 
    int8_t plusZeroColor = 20;    // цвет отрисовки температуры
    int8_t clockColor = 150;        // цвет отрисовки времени 
    int8_t iconsColor = 43;
    int8_t periodTemp = 60;
    int8_t durationTemp = 15;
    DisplaySettings operator=(DisplaySettings const &other){
        brightness = other.brightness;
        subZeroColor = other.subZeroColor;
        plusZeroColor = other.plusZeroColor;
        clockColor = other.clockColor;
        iconsColor = other.iconsColor;
        periodTemp = other.periodTemp;
        return *this;
    }
    int8_t getBrightness(){
        if(brightness < 60) brightness = 60;
        return brightness;
    }
    void save(){
        EEPROMWriteInt(0,27575);
        EEPROMWriteInt(2,clockColor);
        EEPROM.write(4,subZeroColor);
        EEPROM.write(5, iconsColor);
        EEPROM.write(6,plusZeroColor);
        EEPROM.write(7,durationTemp);
        EEPROM.write(8,brightness);
        EEPROM.write(9,periodTemp);

    }
    void load(){
        if(EEPROMReadInt(0) == 27575){
            this->clockColor =  EEPROMReadInt(2);
            this->subZeroColor = EEPROM.read(4);
            this->iconsColor = EEPROM.read(5);
            this->plusZeroColor = EEPROM.read(6);
            this->durationTemp = EEPROM.read(7);
            this->brightness = EEPROM.read(8);
            this->periodTemp = EEPROM.read(9);
        }else{
            save();
        }
        
    }
private:
    void EEPROMWriteInt(int p_address, int p_value){
        byte lowByte = ((p_value >> 0) & 0xFF);
        byte highByte = ((p_value >> 8) & 0xFF);

        EEPROM.write(p_address, lowByte);
        EEPROM.write(p_address + 1, highByte);
        }

//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
    int EEPROMReadInt(int p_address){
        byte lowByte = EEPROM.read(p_address);
        byte highByte = EEPROM.read(p_address + 1);

        return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
    }
};

struct LedPixel{
    bool isShowed = false;
    int8_t color = 0;
    showingLedEffects effect = DAYLY;
};

/*struct LedPixel{
    bool isShowed = false;
    CRGB color = CRGB::Green;
    showingLedEffects effect = DAYLY;
};*/

#endif