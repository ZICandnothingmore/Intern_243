// #ifdef USE_RS485
// #ifdef USE_SOIL_7IN1

// #define XSNS_123 123  // ID module tùy chỉnh
// #define XRS485_40 40 

// struct SOIL_7IN1t
// {
//     bool valid = false;

//     float ph = 0;  
//     float moisture = 0;      
//     float temperature = 0;      
//     uint16_t conductivity = 0;       
//     uint16_t nitrogen = 0;       
//     uint16_t phosphorus = 0;       
//     uint16_t potassium = 0;

//     char name[10] = "SOIL_7IN1";
// } SOIL_7IN1;

// #define SOIL_7IN1_ADDRESS_ID            0x11
// #define SOIL_7IN1_FUNCTION_CODE         0x03
// #define SOIL_7IN1_ADDRESS_CHECK         0x0100 

// #define SOIL_7IN1_ADDRESS_PH            0x0006
// #define SOIL_7IN1_ADDRESS_MOISTURE      0x0012
// #define SOIL_7IN1_ADDRESS_TEMPERATURE   0x0013
// #define SOIL_7IN1_ADDRESS_CONDUCTIVITY  0x0015
// #define SOIL_7IN1_ADDRESS_NITROGEN      0x001E
// #define SOIL_7IN1_ADDRESS_PHOSPHORUS    0x001F
// #define SOIL_7IN1_ADDRESS_POTASSIUM     0x0020

// bool SOIL_7IN1isConnected() {
//     if (!RS485.active)
//         return false;

//     RS485.Rs485Modbus->Send(SOIL_7IN1_ADDRESS_ID, SOIL_7IN1_FUNCTION_CODE, SOIL_7IN1_ADDRESS_CHECK, 0x01);
//     delay(200);
//     RS485.Rs485Modbus->ReceiveReady();

//     uint8_t buffer[8];
//     uint8_t error = RS485.Rs485Modbus->ReceiveBuffer(buffer, 8);

//     if (error) {
//         AddLog(LOG_LEVEL_INFO, PSTR("Soil 7-in-1 sensor error %d"), error);
//         return false;
//     } else {
//         uint16_t check_val = (buffer[3] << 8) | buffer[4];
//         if (check_val == SOIL_7IN1_ADDRESS_ID)
//             return true;
//     }
//     return false;
// }

// void SOIL_7IN1Init() {
//     if (!RS485.active)
//         return;

//     SOIL_7IN1.valid = SOIL_7IN1isConnected();
//     if (SOIL_7IN1.valid)
//         Rs485SetActiveFound(SOIL_7IN1_ADDRESS_ID, SOIL_7IN1.name);
//     if (SOIL_7IN1.valid)
//         AddLog(LOG_LEVEL_INFO, PSTR("Soil 7-in-1 sensor is connected"));
//     else
//         AddLog(LOG_LEVEL_INFO, PSTR("Soil 7-in-1 sensor is not detected"));
// }

// const char HTTP_SNS_SOIL_7IN1[] PROGMEM =
//   "{s}Soil 7-in-1 Sensor{m}"
//   " pH: %.2f\\n"
//   " Moisture: %.2f (%RH)\\n"
//   " Temperature: %.2f (°C)\\n"
//   " Conductivity: %d (us/cm)\\n"
//   " Nitrogen: %d (mg/kg)\\n"
//   " Phosphorus: %d (mg/kg)\\n"
//   " Potassium: %d (mg/kg)";
// #define D_JSON_SOIL_7IN1 "SOIL_7IN1"

// void SOIL_7IN1ReadData() {
//     if (!SOIL_7IN1.valid) return;

//     if (isWaitingResponse(SOIL_7IN1_ADDRESS_ID)) return;

//     if ((RS485.requestSent[SOIL_7IN1_ADDRESS_ID] == 0) && (RS485.lastRequestTime == 0)) {
//         RS485.Rs485Modbus->Send(SOIL_7IN1_ADDRESS_ID, SOIL_7IN1_FUNCTION_CODE, SOIL_7IN1_ADDRESS_PH, 7);
//         RS485.requestSent[SOIL_7IN1_ADDRESS_ID] = 1;
//         RS485.lastRequestTime = millis();
//     }

//     if ((RS485.requestSent[SOIL_7IN1_ADDRESS_ID] == 1) && (millis() - RS485.lastRequestTime >= 200)) {
//         if (!RS485.Rs485Modbus->ReceiveReady())
//             return;
//         uint8_t buffer[21];
//         uint8_t error = RS485.Rs485Modbus->ReceiveBuffer(buffer, 17); // 7 thanh ghi * 2 byte + 5 byte header/crc

//         if (error) {
// AddLog(LOG_LEVEL_INFO, PSTR("Soil 7-in-1 read error: %d"), error);
//         } else {
//             // buffer[3]~[4]: pH, [5]~[6]: moisture, [7]~[8]: temp, [9]~[10]: conductivity, [11]~[12]: N, [13]~[14]: P, [15]~[16]: K
//             SOIL_7IN1.ph = ((buffer[3] << 8) | buffer[4]) / 100.0f;
//             SOIL_7IN1.moisture = ((buffer[5] << 8) | buffer[6]) / 10.0f;
//             SOIL_7IN1.temperature = ((buffer[7] << 8) | buffer[8]) / 10.0f;
//             SOIL_7IN1.conductivity = (buffer[9] << 8) | buffer[10];
//             SOIL_7IN1.nitrogen = (buffer[11] << 8) | buffer[12];
//             SOIL_7IN1.phosphorus = (buffer[13] << 8) | buffer[14];
//             SOIL_7IN1.potassium = (buffer[15] << 8) | buffer[16];
//         }
//         RS485.requestSent[SOIL_7IN1_ADDRESS_ID] = 0;
//         RS485.lastRequestTime = 0;
//     }
// }

// void SOIL_7IN1Show(bool json) {
//     if (json) {
//         ResponseAppend_P(PSTR(",\"%s\":{"), SOIL_7IN1.name);
//         ResponseAppend_P(PSTR("\"" D_JSON_SOIL_7IN1 "\":{"
//             "\"pH\":%.2f,"
//             "\"Moisture\":%.2f,"
//             "\"Temperature\":%.2f,"
//             "\"Conductivity\":%d,"
//             "\"Nitrogen\":%d,"
//             "\"Phosphorus\":%d,"
//             "\"Potassium\":%d}"),
//             SOIL_7IN1.ph, SOIL_7IN1.moisture, SOIL_7IN1.temperature,
//             SOIL_7IN1.conductivity, SOIL_7IN1.nitrogen,
//             SOIL_7IN1.phosphorus, SOIL_7IN1.potassium);
//         ResponseJsonEnd();
//     }
// #ifdef USE_WEBSERVER
//     else {
//         WSContentSend_PD(HTTP_SNS_SOIL_7IN1,
//             SOIL_7IN1.ph,
//             SOIL_7IN1.moisture,
//             SOIL_7IN1.temperature,
//             SOIL_7IN1.conductivity,
//             SOIL_7IN1.nitrogen,
//             SOIL_7IN1.phosphorus,
//             SOIL_7IN1.potassium);
//     }
// #endif
// }

// bool Xsns123(uint32_t function) {
//     if (!Rs485Enabled(XRS485_30)) return false;
//     bool result = false;
//     if (FUNC_INIT == function) {
//         SOIL_7IN1Init();
//     }
//     else if (SOIL_7IN1.valid)
//     {
//         switch (function)
//         {
//         case FUNC_EVERY_250_MSECOND:
//             SOIL_7IN1ReadData();
//             break;
//         case FUNC_JSON_APPEND:
//             SOIL_7IN1Show(1);
//             break;
// #ifdef USE_WEBSERVER
//         case FUNC_WEB_SENSOR:
//             SOIL_7IN1Show(0);
//             break;
// #endif
//         }
//     }
//     return result;
// }
// #endif
// #endif


#ifdef USE_RS485
#ifdef USE_ES_SOIL_SENSOR

#define XSNS_123 123  // ID module tùy chỉnh
#define XRS485_34 34

#define SOIL_ADDRESS_ID 0x11 // địa chỉ đúng
#define SOIL_PH_ADDRESS_VALUE 0x0006 //pH
#define SOIL_MOIS_ADDRESS_VALUE 0x0012 //MOISTURE
#define SOIL_TEMP_ADDRESS_VALUE 0x0013  //TEMPERATURE
#define SOIL_CONDUC_ADDRESS_VALUE 0x0015 //CONDUCTIVITY
#define SOIL_NITRO_ADDRESS_VALUE 0x001E //NITROGEN
#define SOIL_PHOSPHO_ADDRESS_VALUE 0x001F //PHOSPHORUS
#define SOIL_KALI_ADDRESS_VALUE 0x0020 //POTASSIUM
#define SOIL_FUNCTION_CODE 0x03
#define SOIL_ADDRESS_CHECK 0x0100    // Dùng kiểm tra kết nối (Equipment Address)


struct ES_SOIL_Sensor_32 {
    bool valid = false;
    float pH = 0;
    float moisture = 0;
    float temperature = 0;
    uint16_t conductivity = 0;
    uint16_t nitrogen = 0;
    uint16_t phosphorus = 0;
    uint16_t potassium = 0;
    char name[15] = "ES SOIL 7 IN 1";
} SoilSensor;


int reg_val[7] = {
    SOIL_PH_ADDRESS_VALUE,          // index 0
    SOIL_MOIS_ADDRESS_VALUE,        // index 1
    SOIL_TEMP_ADDRESS_VALUE,        // index 2
    SOIL_CONDUC_ADDRESS_VALUE,      // index 3
    SOIL_NITRO_ADDRESS_VALUE,       // index 4
    SOIL_PHOSPHO_ADDRESS_VALUE,     // index 5
    SOIL_KALI_ADDRESS_VALUE         // index 6
};


bool SoilisConnected()
{
    if (!RS485.active)
        return false;
    
    RS485.Rs485Modbus->Send(SOIL_ADDRESS_ID, SOIL_FUNCTION_CODE, SOIL_ADDRESS_CHECK, 0x01);
    delay(200);
    RS485.Rs485Modbus->ReceiveReady();

    uint8_t buffer[8];
    uint8_t error = RS485.Rs485Modbus->ReceiveBuffer(buffer, 8);

    if (error)
    {
        AddLog(LOG_LEVEL_INFO, PSTR("Soil sensor error %d"), error);
        return false;
    }
    else
    {
        uint16_t check_val = (buffer[3] << 8) | buffer[4];
        if (check_val <= 255)  // Equipment Address là 0~255
            return true;
    }
    return false;
}
void SoilInit()
{
    if (!RS485.active)
        return;

    SoilSensor.valid = SoilisConnected();

    if (SoilSensor.valid)
        Rs485SetActiveFound(SOIL_ADDRESS_ID, SoilSensor.name);

    if (SoilSensor.valid)
        AddLog(LOG_LEVEL_INFO, PSTR("Soil sensor is connected"));
    else
        AddLog(LOG_LEVEL_INFO, PSTR("Soil sensor is not detected"));
}


const char HTTP_SNS_Soil[] PROGMEM =
    "{s}Soil pH{m} %.2f"
    "{s}Soil Moisture{m} %.1f %%"
    "{s}Soil Temp{m} %.1f °C"
    "{s}Conductivity{m} %d µS/cm"
    "{s}Nitrogen{m} %d mg/kg"
    "{s}Phosphorus{m} %d mg/kg"
    "{s}Potassium{m} %d mg/kg";

#define D_JSON_Soil "Soil"

void SoilReadData()
{
    static int read_index = 0;
    static const int reg_count = 7;
    static uint16_t temp_values[7] = {0};  // mảng tạm

    if (!RS485.requestSent[SOIL_ADDRESS_ID] && RS485.lastRequestTime == 0) {
        RS485.Rs485Modbus->Send(SOIL_ADDRESS_ID, SOIL_FUNCTION_CODE, reg_val[read_index], 0x01);
        RS485.requestSent[SOIL_ADDRESS_ID] = 1;
        RS485.lastRequestTime = millis();
    }

    if (RS485.requestSent[SOIL_ADDRESS_ID] && millis() - RS485.lastRequestTime > 200) {
        if (RS485.Rs485Modbus->ReceiveReady()) {
            uint8_t buffer[8];
            uint8_t error = RS485.Rs485Modbus->ReceiveBuffer(buffer, 8);
            if (!error) {
                uint16_t value = (buffer[3] << 8) | buffer[4];
                temp_values[read_index] = value;  // lưu vào mảng tạm
            }
            read_index = (read_index + 1) % reg_count;
            RS485.requestSent[SOIL_ADDRESS_ID] = 0;
            RS485.lastRequestTime = 0;
            

            // Khi đã đọc đủ 7 giá trị, cập nhật vào struct SoilSensor
            if (read_index == 0) {
                SoilSensor.pH = temp_values[0];
                SoilSensor.moisture = temp_values[1];
                SoilSensor.temperature = temp_values[2];
                SoilSensor.conductivity = temp_values[3];
                SoilSensor.nitrogen = temp_values[4];
                SoilSensor.phosphorus = temp_values[5];
                SoilSensor.potassium = temp_values[6];
            }
        }
    }
}

void SoilShow(bool json)
{
    if (json)
    {
        ResponseAppend_P(PSTR(",\"%s\":{"), SoilSensor.name);
        ResponseAppend_P(PSTR("\"pH\":%.2f,"), SoilSensor.pH / 100.0);
        ResponseAppend_P(PSTR("\"moisture\":%.1f,"), SoilSensor.moisture / 10.0);
        ResponseAppend_P(PSTR("\"temperature\":%.1f,"), SoilSensor.temperature / 10.0);
        ResponseAppend_P(PSTR("\"conductivity\":%d,"), SoilSensor.conductivity);
        ResponseAppend_P(PSTR("\"nitrogen\":%d,"), SoilSensor.nitrogen);
        ResponseAppend_P(PSTR("\"phosphorus\":%d,"), SoilSensor.phosphorus);
        ResponseAppend_P(PSTR("\"potassium\":%d"), SoilSensor.potassium);
        ResponseJsonEnd();
    }
#ifdef USE_WEBSERVER
    else
    {
        WSContentSend_PD(
            HTTP_SNS_Soil,
            SoilSensor.pH / 100.0,
            SoilSensor.moisture / 10.0,
            SoilSensor.temperature / 10.0,
            SoilSensor.conductivity,
            SoilSensor.nitrogen,
            SoilSensor.phosphorus,
            SoilSensor.potassium
        );
    }
#endif
}



bool Xsns123(uint32_t function)
{
    if (!Rs485Enabled(XRS485_34))
        return false;

    bool result = false;

    if (FUNC_INIT == function)
    {
        SoilInit();
    }
    else if (SoilSensor.valid)
    {
        switch (function)
        {
        case FUNC_EVERY_250_MSECOND:
            SoilReadData();
            break;
        case FUNC_JSON_APPEND:
            SoilShow(true);
            break;
#ifdef USE_WEBSERVER
        case FUNC_WEB_SENSOR:
            SoilShow(false);
            break;
#endif
        }
    }
    return result;
}
#endif
#endif