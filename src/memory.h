#include <EEPROM.h>
#include "Wire.h"

bool openMemmory()
{
    EEPROM.begin(512);
    return true;
}

void writeMemory(String token = "", String wifi = "", String password = "", String jsonData = "", String moduleCodes = "")
{
    EEPROM.begin(512);
    EEPROM.write(0, token.length());
    EEPROM.write(1, wifi.length());
    EEPROM.write(2, password.length());
    EEPROM.write(3, jsonData.length());
    EEPROM.write(4, moduleCodes.length());

    String data = token + wifi + password + jsonData + moduleCodes;
    for (int i = 0; i < data.length(); i++)
    {
        EEPROM.write(i + 5, data[i]);
    }

    EEPROM.commit();
}

void clearMemory()
{

    for (int i = 0; i < 512; i++)
    {
        EEPROM.write(i, 0);
    }
    EEPROM.commit();
}

String readMemory(String comType = "token")
{
    EEPROM.begin(512);

    int start = 0;
    int stop = 0;

    int tokenLen = EEPROM.read(0);
    int wifiLen = EEPROM.read(1);
    int passwordLen = EEPROM.read(2);
    int jsonDataLen = EEPROM.read(3);
    int moduleCodesLen = EEPROM.read(4);

    if (comType == "token")
    {
        start = 5;
        stop = 5 + tokenLen;
    }
    else if (comType == "wifi")
    {
        start = tokenLen + 5;
        stop = tokenLen + 5 + wifiLen;
    }
    else if (comType == "password")
    {
        start = tokenLen + wifiLen + 5;
        stop = tokenLen + wifiLen + 5 + passwordLen;
    }
    else if (comType == "jsonData")
    {
        start = tokenLen + wifiLen + 5 + passwordLen;
        stop = tokenLen + wifiLen + 5 + passwordLen + jsonDataLen;
    }
    else
    {
        start = tokenLen + wifiLen + 5 + passwordLen + jsonDataLen;
        stop = tokenLen + wifiLen + 5 + passwordLen + jsonDataLen + moduleCodesLen;
    }

    String dat = "";
    for (int i = start; i < stop; i++)
    {
        char a = EEPROM.read(i);
        dat += a;
    }
    return dat;
}


